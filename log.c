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
 ** File: 4004-as/src/log.c
 **
 ** Description:
 ** This translation (compilation) unit contains the functions necessary for
 ** printing and/or logging information.
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
#ifndef _STDIO_H
#include <stdio.h>
#endif
#ifndef _STDINT_H
#include <stdint.h>
#endif

//Project-wide #includes
#ifndef ___UNIVERSAL_H___
#include "universal.h"
#endif

//Project #includes
//None

//Reflective #includes
#ifndef ___LOG_H___
#include "log.h"
#endif

//------------------------------------------------------------------------------
//Global Data
//None

//------------------------------------------------------------------------------
//Static Data
static const char* s_cpaErrorMessage[] =
{
    "no error",
    "an unknown option was passed on the command line",
    "unknown listing file option command line argument",
    "unknown symbol table option command line argument",
    "the command line is malformed, refer to usage",
    "a NULL pointer was passed for the path and file name",
    "the path and file of the assembly source file is required to be the last argument",
    "an empty string was passed for the path and file name",
    "malloc() returned a NULL",
    "a length of zero was returned for the file name",
    "an invalid file name was passed",
    "file is already opened",
    "could not open file",
    "file is not open",
    "getline() failed",
    "could not reset file back to the beginning",
    "could not write file",
    "letter expected syntax error",
    "invalid character syntax error",
    "duplicate symbol error",
    "END directive not alone error",
    "unexpected end of statement syntax error",
    "symbol length exceeds maximum error",
    "invalid value error",
    "expected EQU after symbol syntax error",
    "end of statement expected error",
    "unknown symbol error",
    "too many operands syntax error",
    "exceeded program memory error",
    "incorrect number of operands error",
    "expression number stack full",
    "right parenthesis expected syntax error",
    "expression number stack empty",
    "an attempt was made to move the location counter backwards"
};

//------------------------------------------------------------------------------
//Static Prototypes
//None

//==============================================================================
//Functions
/*------------------------------------------------------------------------------
 * Function name:  print_error
 * Function Description:  Prints an error message to stdout.
 * Parameters:
 * cpFunction - Character pointer to the name of the function in which the error
 *              occurred.
 * iErrorNumber - The number of the error.
 * Returns:  None.
------------------------------------------------------------------------------*/
void print_error(const char* cpFunction, uint8_t iErrorNumber)
{
    printf("ERROR:  In function %s() %s.\n", cpFunction, s_cpaErrorMessage[iErrorNumber]);
}
