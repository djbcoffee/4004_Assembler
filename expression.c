/*
 ********************************************************************************
 ** Copyright (C) 2018 Donald J. Bartley <djbcoffee@gmail.com>
 **
 ** This source file may be used and distributed without restriction provided
 ** that this copyright statement is not removed from the file and that any
 ** derivative work contains the original copyright notice and the associated
 ** disclaimer.
 **
 ** This source file is free software; you can redistribute it and/or modify it
 ** under the terms of the GNU General Public License as published by the Free
 ** Software Foundation; either version 2 of the License, or (at your option) any
 ** later version.
 **
 ** This source file is distributed in the hope that it will be useful, but
 ** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 ** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 ** more details.
 **
 ** You should have received a copy of the GNU General Public License along with
 ** this source file.  If not, see <http://www.gnu.org/licenses/> or write to the
 ** Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 ** 02110-1301, USA.
 ********************************************************************************
 ** File: 4004-as/src/expression.c
 **
 ** Description:
 ** This translation (compilation) unit contains the functions for parsing a
 ** mathematical expression using the following grammar:
 ** <b-expression>              ::= <b-term> [<orop> <b-term>]*
 ** <b-term>                    ::= <expression> [<andop> <expression>]*
 ** <expression>                ::= <term> [<addop> <term>]*
 ** <term>                      ::= <signed factor> [<mulop> <factor>]*
 ** <signed factor>             ::= [<addop>] <factor>
 ** <factor>                    ::= (<b-expression>) | <symbol> | <lc symbol> | <number>
 ** <symbol>                    ::= <letter> [<letter> | <underscore> | <digit>]*
 ** <lc symbol>                 ::= .
 ** <number>                    ::= <binary number> | <decimal number> | <hexadecimal number>
 ** <binary number>             ::= <binary number prefix> <binary digit> [<binary digit>]*
 ** <decimal number>            ::= <digit> [<digit>]*
 ** <hexadecimal number>        ::= <hexadecimal number prefix> <hexadecimal character> [<hexadecimal character>]*
 ** <binary number prefix>      ::= 0B | %
 ** <binary digit>              ::= 0 | 1
 ** <digit>                     ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 ** <hexadecimal number prefix> ::= 0X | $
 ** <hexadecimal character>     ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F
 ** <letter>                    ::= A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | Y | W | X | Y | Z
 ** <underscore>                ::= _
 ** <addop>                     ::= + | -
 ** <mulop>                     ::= * | /
 ** <andop>                     ::= &
 ** <orop>                      ::= |
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

//System #includes
#ifndef _WINDOWS_H
#include <windows.h>
#endif
#ifndef _STDINT_H
#include <stdint.h>
#endif
#ifndef _STRING_H
#include <string.h>
#endif
#ifndef _CTYPE_H
#include <ctype.h>
#endif
#ifndef _STDDEF_H
#include <stddef.h>
#endif
#ifndef _STDLIB_H
#include <stdlib.h>
#endif

//Project-wide #includes
#ifndef ___UNIVERSAL_H___
#include "universal.h"
#endif

//Project #includes
#ifndef ___BSTREE_H___
#include "bstree.h"
#endif
#ifndef ___LEXER_H___
#include "lexer.h"
#endif

//Reflective #includes
#ifndef ___EXPRESSION_H___
#include "expression.h"
#endif

//------------------------------------------------------------------------------
//Global Data
//None

//------------------------------------------------------------------------------
//Static Data
//None

//------------------------------------------------------------------------------
//Static Prototypes
static int get_b_expression(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int get_b_term(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int get_expression(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int get_term(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int get_signed_factor(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int get_factor(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int get_number(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine);
static int push_number_stack(ExpressionParseInfo* pExpressionInfo);
static int pop_number_stack(ExpressionParseInfo* pExpressionInfo);
static void inc_chr_pointer(char** cppSourceLine);

//==============================================================================
//Functions
/*------------------------------------------------------------------------------
 * Function name:  do_expression
 * Function Description:  Parses an expression from the source line.
 * Parameters:
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * ipValue - A pointer to where to store the final value of the expression.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
int do_expression(char** cppSourceLine, int* ipValue)
{
    int iFunctionReturnValue;

    ExpressionParseInfo expressionInfo;

    //Initialize variables.
    expressionInfo.m_iNumberStackIndex = NUMBER_STACK_MAX;

    //Start getting the expression.
    iFunctionReturnValue = get_b_expression(&expressionInfo, cppSourceLine);
    if(iFunctionReturnValue != EXIT_SUCCESS)
        return iFunctionReturnValue;

    //We are done with the expression.  Set the vale of the expression.
    *ipValue = expressionInfo.m_iRightOperand;

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_symbol
 * Function Description:  Processes a symbol from the source file line.
 * Parameters:
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * cpSymbol - Pointer to where the NULL terminated symbol is to be stored.
 * Returns:  Zero for success and negative number for failure.
------------------------------------------------------------------------------*/
int get_symbol(char** cppSourceLine, char* cpSymbol)
{
    //Evaluate the label for correct syntax, it must begin with a letter.
   if(isalpha(**cppSourceLine) != 0)
   {
       //The first character is a letter.  Store it and then increment to the
       //next character.
       *cpSymbol = **cppSourceLine;
       cpSymbol++;
       (*cppSourceLine)++;
   }
   else
   {
       //The first character is not a letter.
       return -LetterExpectedSyntaxError;
   }

   //Loop until a non-symbol character (letters, digits, and underscores) is
   //found.
   while(isalpha(**cppSourceLine) != 0 || isdigit(**cppSourceLine) != 0 || **cppSourceLine == '_')
   {
       //Valid character, store it and then increment to the next character.
       *cpSymbol = **cppSourceLine;
       cpSymbol++;
       (*cppSourceLine)++;
   }

   //NULL Terminate the symbol.
   *cpSymbol = '\0';

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_b_expression
 * Function Description:  Parses the b-expression portion of the source line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int get_b_expression(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    int iFunctionReturnValue;

    //Get the first b-term.
    iFunctionReturnValue = get_b_term(pExpressionInfo, cppSourceLine);
    if(iFunctionReturnValue != EXIT_SUCCESS)
        return iFunctionReturnValue;

    //Now check for orops.  Loop as long as they continue.
    while(**cppSourceLine == '|')
    {
        //Put the current value in the right operand unto the stack.
        iFunctionReturnValue = push_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //The current number was placed in storage.  Now point to the next
        //character and get the next b-term.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_b_term(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Next b-term retrieved into right operand.  Put the current number on
        //the stack into the left operand.
        iFunctionReturnValue = pop_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Do bitwise OR.
        pExpressionInfo->m_iRightOperand = pExpressionInfo->m_iLeftOperand | pExpressionInfo->m_iRightOperand;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_b_term
 * Function Description:  Parses the b-term portion of the source line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int get_b_term(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    int iFunctionReturnValue;

    //Get the first expression.
    iFunctionReturnValue = get_expression(pExpressionInfo, cppSourceLine);
    if(iFunctionReturnValue != EXIT_SUCCESS)
        return iFunctionReturnValue;

    //Now check for andops.  Loop as long as they continue.
    while(**cppSourceLine == '&')
    {
        //Put the current value in the right operand unto the stack.
        iFunctionReturnValue = push_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //The current number was placed in storage.  Now point to the next
        //character and get the next expression.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_expression(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Next expression retrieved into right operand.  Put the current number
        //on the stack into the left operand.
        iFunctionReturnValue = pop_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Do bitwise AND.
        pExpressionInfo->m_iRightOperand = pExpressionInfo->m_iLeftOperand & pExpressionInfo->m_iRightOperand;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_expression
 * Function Description:  Parses the expression portion of the source line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int get_expression(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    char cAddOp;

    int iFunctionReturnValue;

    //Get the first term.
    iFunctionReturnValue = get_term(pExpressionInfo, cppSourceLine);
    if(iFunctionReturnValue != EXIT_SUCCESS)
        return iFunctionReturnValue;

    //Now check for addops.  Loop as long as they continue.
    while(**cppSourceLine == '+' || **cppSourceLine == '-')
    {
        //Save the current addop.
        cAddOp = **cppSourceLine;

        //Put the current value in the right operand unto the stack.
        iFunctionReturnValue = push_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //The current number was placed in storage.  Now point to the next
        //character and get the next term.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_term(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Next term retrieved into right operand.  Put the current number on the
        //stack into the left operand.
        iFunctionReturnValue = pop_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Do work based on the addop.
        if(cAddOp == '+')
            pExpressionInfo->m_iRightOperand = pExpressionInfo->m_iLeftOperand + pExpressionInfo->m_iRightOperand;
        else
            pExpressionInfo->m_iRightOperand = pExpressionInfo->m_iLeftOperand - pExpressionInfo->m_iRightOperand;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_term
 * Function Description:  Parses the term portion of the source line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int get_term(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    char cMulOp;

    int iFunctionReturnValue;

    //Get the first signed factor.
    iFunctionReturnValue = get_signed_factor(pExpressionInfo, cppSourceLine);
    if(iFunctionReturnValue != EXIT_SUCCESS)
        return iFunctionReturnValue;

    //Now check for mulops.  Loop as long as they continue.
    while(**cppSourceLine == '*' || **cppSourceLine == '/')
    {
        //Save the current mulop.
        cMulOp = **cppSourceLine;

        //Put the current value in the right operand unto the stack.
        iFunctionReturnValue = push_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //The current number was placed in storage.  Now point to the next
        //character and get the next signed factor.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_signed_factor(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Next signed factor retrieved into right operand.  Put the current
        //number on the stack into the left operand.
        iFunctionReturnValue = pop_number_stack(pExpressionInfo);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Do work based on the mulop.
        if(cMulOp == '*')
            pExpressionInfo->m_iRightOperand = pExpressionInfo->m_iLeftOperand * pExpressionInfo->m_iRightOperand;
        else
            pExpressionInfo->m_iRightOperand = pExpressionInfo->m_iLeftOperand / pExpressionInfo->m_iRightOperand;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_signed_factor
 * Function Description:  Parses the signed factor portion of the source line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int get_signed_factor(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    int iFunctionReturnValue;

    if(**cppSourceLine == '+')
    {
        //It is a positive signed factor.  Increment past the '+' character and
        //then get the factor.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_factor(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;
    }
    else if(**cppSourceLine == '-')
    {
        //It is a negative signed factor.  Increment past the '-' character and
        //then get the factor.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_factor(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Since it is a negative factor make it so.
        pExpressionInfo->m_iRightOperand = 0 - pExpressionInfo->m_iRightOperand;
    }
    else
    {
        //No signed character found so just get the factor.
        iFunctionReturnValue = get_factor(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_factor
 * Function Description:  Parses the factor portion of the source line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int get_factor(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    char caSymbol[MAX_SYMBOL_SIZE + NULL_TERMINATING_BYTE_LENGTH];

    int iFunctionReturnValue;

    BSTreeNode* pNode;

    //(<b-expression>) | <symbol> | <lc symbol> | <number>

    if(**cppSourceLine == '(')
    {
        //It is a left parenthesis.  Increment past it then get the b-expression
        //inside of it.
        inc_chr_pointer(cppSourceLine);
        iFunctionReturnValue = get_b_expression(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //The b-expression was retrieved.  Check for an ending right parenthesis.
        if(**cppSourceLine == ')')
        {
            //It is a right parenthesis.  Increment past it.
            inc_chr_pointer(cppSourceLine);
        }
        else
        {
            //It is not a right parenthesis, this is an error.
            return -RightParenthesisExpected;
        }
    }
    else if(isalpha(**cppSourceLine) != 0)
    {
        //Try and get a symbol.
        iFunctionReturnValue = get_symbol(cppSourceLine, caSymbol);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Successfully parsed a symbol.  Search for the symbol.
        pNode = bstree_search(get_symbol_table_root(), caSymbol, bstree_key_compare);
        if(pNode != NULL)
        {
            //The symbol is known so save the value.
            pExpressionInfo->m_iRightOperand = (int)*((uint32_t*)(pNode->m_vpDataElement));
        }
        else
        {
            //The symbol is not known.
            return -UnknownSymbolError;
        }

        //If we are pointing at a white space then increment past it.
        if(isspace(**cppSourceLine) != 0)
            inc_chr_pointer(cppSourceLine);
    }
    else if(**cppSourceLine == '.')
    {
        //Location counter symbol.  Save the current value of the location
        //counter.
        pExpressionInfo->m_iRightOperand = (int)get_location_counter_value();

        //Increment past the character.
        inc_chr_pointer(cppSourceLine);
    }
    else
    {
        //Not a left parenthesis, symbol, or location counter symbol so try and
        //get a number.
        iFunctionReturnValue = get_number(pExpressionInfo, cppSourceLine);
        if(iFunctionReturnValue != EXIT_SUCCESS)
            return iFunctionReturnValue;

        //Successfully parsed a number.  If we are pointing at a white space then
        //increment past it.
        if(isspace(**cppSourceLine) != 0)
            inc_chr_pointer(cppSourceLine);
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  get_number
 * Function Description:  Processes a number from the source file line.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  Zero for success and negative number for failure.
------------------------------------------------------------------------------*/
static int get_number(ExpressionParseInfo* pExpressionInfo, char** cppSourceLine)
{
    if(**cppSourceLine == '%' || strncmp(*cppSourceLine, "0B", 2) == 0)
    {
        //Binary number prefix found.  Skip past the prefix.
        *cppSourceLine = (**cppSourceLine == '%') ? *cppSourceLine + 1 : *cppSourceLine + 2;

        //The next character must be a binary digit or this is an error.
        if(**cppSourceLine != '0' && **cppSourceLine != '1')
            return -InvalidCharacterSyntaxError;

        //Convert the number.
        pExpressionInfo->m_iRightOperand = 0;
        while(**cppSourceLine == '0' || **cppSourceLine == '1')
        {
            pExpressionInfo->m_iRightOperand <<= 1;
            if(**cppSourceLine == '1')
                pExpressionInfo->m_iRightOperand |= 1;

            (*cppSourceLine)++;
        }
    }
    else if(**cppSourceLine == '$' || strncmp(*cppSourceLine, "0X", 2) == 0)
    {
        //Hexadecimal number prefix found.  Skip past the prefix and convert the
        //number.
        *cppSourceLine = (**cppSourceLine == '$') ? *cppSourceLine + 1 : *cppSourceLine + 2;

        //The next character must be a hexadecimal character or this is an error.
        if(isdigit(**cppSourceLine) != 0 || (**cppSourceLine >= 'A' && **cppSourceLine <= 'F'))
        {
            pExpressionInfo->m_iRightOperand = strtol(*cppSourceLine, cppSourceLine, 16);
        }
        else
        {
            return -InvalidCharacterSyntaxError;
        }
    }
    else if(isdigit(**cppSourceLine) != 0)
    {
        //Decimal number prefix found.
        pExpressionInfo->m_iRightOperand = strtol(*cppSourceLine, cppSourceLine, 10);
    }
    else
    {
        //Not a valid start of a number.
        return -InvalidCharacterSyntaxError;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  push_number_stack
 * Function Description:  Pushes the current value of the right operand onto the
 *                        number stack.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int push_number_stack(ExpressionParseInfo* pExpressionInfo)
{
    //Make sure there is room on the stack.
    if(pExpressionInfo->m_iNumberStackIndex != 0)
    {
        pExpressionInfo->m_iNumberStackIndex--;
        pExpressionInfo->m_iNumberStack[pExpressionInfo->m_iNumberStackIndex] = pExpressionInfo->m_iRightOperand;
    }
    else
    {
        return -NumberStackFull;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  pop_number_stack
 * Function Description:  Pops the current value from the stack and places it
 *                        into the left operand.
 * Parameters:
 * pExpressionInfo - Pointer to the expression information structure.
 * Returns:  Zero for success and a negative value if an error occurs.
------------------------------------------------------------------------------*/
static int pop_number_stack(ExpressionParseInfo* pExpressionInfo)
{
    //Make sure there is a number on the stack.
    if(pExpressionInfo->m_iNumberStackIndex != NUMBER_STACK_MAX)
    {
        pExpressionInfo->m_iLeftOperand = pExpressionInfo->m_iNumberStack[pExpressionInfo->m_iNumberStackIndex];
        pExpressionInfo->m_iNumberStackIndex++;
    }
    else
    {
        return -NumberStackEmpty;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  inc_chr_pointer
 * Function Description:  Increments past the current character and skips all
 *                        white spaces afterwards.
 * Parameters:
 * cppSourceLine - Pointer to a character pointer which is the current position
 *                 within the source line.
 * Returns:  None.
------------------------------------------------------------------------------*/
static void inc_chr_pointer(char** cppSourceLine)
{
    //Increment the character pointer.
    (*cppSourceLine)++;

    //Skip all white spaces.
    while(isspace(**cppSourceLine) != 0)
        (*cppSourceLine)++;
}
