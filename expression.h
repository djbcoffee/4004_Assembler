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
 ** File: 4004-as/src/expression.h
 **
 ** Description:
 ** Header file that goes with expression.c
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___EXPRESSION_H___
#define ___EXPRESSION_H___

//------------------------------------------------------------------------------
//Defines
#define NUMBER_STACK_MAX    (50)

//------------------------------------------------------------------------------
//Enumerations
//None

//------------------------------------------------------------------------------
//Structures
typedef struct tagExpressionParseInfo
{
    int m_iRightOperand;
    int m_iLeftOperand;
    int m_iNumberStack[NUMBER_STACK_MAX];
    uint32_t m_iNumberStackIndex;
} ExpressionParseInfo;

//------------------------------------------------------------------------------
//Prototypes
int do_expression(char** cppSourceLine, int* ipValue);
int get_symbol(char** cppSourceLine, char* cpSymbol);

#endif /*___EXPRESSION_H___*/
