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
 ** File: 4004-as/src/main.c
 **
 ** Description:
 ** This translation (compilation) unit contains the main function for the 4004
 ** assembler.
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
#ifndef _STDLIB_H
#include <stdlib.h>
#endif

//Project-wide #includes
#ifndef ___UNIVERSAL_H___
#include "universal.h"
#endif

//Project #includes
#ifndef ___ARGUMENTS_H___
#include "arguments.h"
#endif
#ifndef ___BSTREE_H___
#include "bstree.h"
#endif
#ifndef ___FILES_H___
#include "files.h"
#endif
#ifndef ___LEXER_H___
#include "lexer.h"
#endif

//Reflective #includes
#ifndef ___MAIN_H___
#include "main.h"
#endif

//------------------------------------------------------------------------------
//Global Data
//None

//------------------------------------------------------------------------------
//Static Data
//None

//------------------------------------------------------------------------------
//Static Prototypes
//None

//==============================================================================
//Functions
/*------------------------------------------------------------------------------
 * Function name:  main
 * Function Description:  Main entry point of program.
 * Parameters:
 * argc - Number of arguments passed to the program.
 * argv - Array of character pointers passed to the program.
 * Returns:  Exit value.
------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    int iReturnValue;

    //Process the passed arguments.
    iReturnValue = process_arguments(argc, argv);
    if(iReturnValue == EXIT_SUCCESS)
    {
        //All arguments have been processed successfully so we can start the
        //assembly.
        iReturnValue = do_assembly();
        if(iReturnValue == EXIT_SUCCESS)
            printf("Assembly successful.\n");
    }
    else if(iReturnValue == 1)
    {
        //Help was requested.  This is not an error so set the return value to
        //zero.
        iReturnValue = 0;
    }

    //Free all allocated memory from the arguments translation (compilation)
    //unit.
    free_argument_memory();

    //Free all allocated memory from the lexer translation (compilation) unit.
    free_lexer_memory();

    //Close all open files from the files translation (compilation) unit.
    close_all_files();

    return iReturnValue;
}
