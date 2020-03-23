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
 ** File: 4004-as/src/arguments.c
 **
 ** Description:
 ** This translation (compilation) unit contains the functions necessary for
 ** parsing the arguments passed to the 4004 assembler.
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
#ifndef _GETOPT_H
#include <getopt.h>
#endif
#ifndef _STDLIB_H
#include <stdlib.h>
#endif
#ifndef _STRING_H
#include <string.h>
#endif
#ifndef _LIBGEN_H
#include <libgen.h>
#endif

//Project-wide #includes
#ifndef ___UNIVERSAL_H___
#include "universal.h"
#endif

//Project #includes
#ifndef ___LOG_H___
#include "log.h"
#endif

//Reflective #includes
#ifndef ___ARGUMENTS_H___
#include "arguments.h"
#endif

//------------------------------------------------------------------------------
//Global Data
//None

//------------------------------------------------------------------------------
//Static Data
static char* s_cpPassedFilePath = NULL;
static char* s_cpPassedFileName = NULL;
static char* s_cpArgumentCopy = NULL;
static char* s_cpPassedBaseFileName = NULL;

static uint8_t s_bIsListingFileEnabled = TRUE;
static uint8_t s_bIsSymbolTableEnabled = TRUE;

static const struct option s_aLongOptions[] =
{
    {"help", no_argument, NULL, 'h'},
    {"listing-file", required_argument, NULL, 'l'},
    {"symbol-table", required_argument, NULL, 's'},
    {"version", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

//------------------------------------------------------------------------------
//Static Prototypes
static int parse_passed_file_path_and_name(const char* cpArgument);
static void display_usage(void);

//==============================================================================
//Functions
/*------------------------------------------------------------------------------
 * Function name:  process_arguments
 * Function Description:  Processes the arguments passed to the program.
 * Parameters:
 * iArgc - Number of arguments passed to the program.
 * acpArgv - Array of character pointers passed to the program.
 * Returns:  Zero for success, one if the help or version option was parsed, and
 *           a negative number if a error occurs.
------------------------------------------------------------------------------*/
int process_arguments(int iArgc, char* acpArgv[])
{
    extern char* optarg;

    extern int optind;

    int iOption;

    //Initialize variables.
    s_cpPassedFilePath = NULL;
    s_cpPassedFileName = NULL;
    s_cpArgumentCopy = NULL;
    s_cpPassedBaseFileName = NULL;

    //Parse the options until there are none left or an error is encountered.
    while((iOption = getopt_long(iArgc, acpArgv, "hl:s:v", s_aLongOptions, NULL)) != -1)
    {
        switch(iOption)
        {
            case 'h' :
                //Help request.  Print usage and return.
                display_usage();
                return 1;
                break;
            case 'l' :
                //Listing file option.
                if(strcasecmp(optarg, "LIST") == 0)
                {
                    //Enable listing file.
                    s_bIsListingFileEnabled = TRUE;
                }
                else if(strcasecmp(optarg, "NOLIST") == 0)
                {
                    //Disable listing file.
                    s_bIsListingFileEnabled = FALSE;
                }
                else
                {
                    //Unknown argument.
                    print_error(__func__, UnknownListingFileOptionArgument);
                    display_usage();
                    return -UnknownListingFileOptionArgument;
                }
                break;
            case 's' :
                //Symbol table option.  Should be a string of either "YES" or
                //"NO".
                if(strcasecmp(optarg, "SYM") == 0)
                {
                    //Enable listing file.
                    s_bIsSymbolTableEnabled = TRUE;
                }
                else if(strcasecmp(optarg, "NOSYM") == 0)
                {
                    //Disable listing file.
                    s_bIsSymbolTableEnabled = FALSE;
                }
                else
                {
                    //Unknown argument.
                    print_error(__func__, UnknownSymbolTableOptionArgument);
                    display_usage();
                    return -UnknownSymbolTableOptionArgument;
                }
                break;
            case 'v' :
                //Print version number.
                printf("4004 ASSEMBLER %s\n", VERSION);
                return 1;
                break;
            default :
                //Unknown option so print the error, display the usage and
                //return with the error.
                print_error(__func__, UnknownOption);
                display_usage();
                return -UnknownOption;
                break;
        }
    }

    //We are done parsing options and if we are here there were no errors.  The
    //option index should be one less than the argument count since the last
    //remaining argument is the file path and name.
    if((optind + 1) != iArgc)
    {
        //The option index is not what was expected so print the error, display
        //the usage and set the return value.
        print_error(__func__, MalformedCommandLine);
        display_usage();
        return -MalformedCommandLine;
    }

    //The option index is what it is suppose to be so pass the final argument to
    //the parser for the file path and name.
    return parse_passed_file_path_and_name(acpArgv[(uint32_t)iArgc - 1]);
}

/*------------------------------------------------------------------------------
 * Function name:  is_listing_file_enabled
 * Function Description:  Getter function for the listing file option.
 * Parameters:  None.
 * Returns:  TRUE if the listing file is to be printed and FALSE otherwise.
------------------------------------------------------------------------------*/
uint8_t is_listing_file_enabled(void)
{
    return s_bIsListingFileEnabled;
}

/*------------------------------------------------------------------------------
 * Function name:  is_symbol_table_enabled
 * Function Description:  Getter function for the symbol table option.
 * Parameters:  None.
 * Returns:  TRUE if the symbol table is to be printed and FALSE otherwise.
------------------------------------------------------------------------------*/
uint8_t is_symbol_table_enabled(void)
{
    return s_bIsSymbolTableEnabled;
}

/*------------------------------------------------------------------------------
 * Function name:  get_assembly_source_file_path
 * Function Description:  Getter function for the assembly source file path that
 *                        was passed to the program.
 * Parameters:  None.
 * Returns:  Character pointer to the path.
------------------------------------------------------------------------------*/
const char* get_assembly_source_file_path(void)
{
    return (const char*)s_cpPassedFilePath;
}

/*------------------------------------------------------------------------------
 * Function name:  get_assembly_source_full_file_name
 * Function Description:  Getter function for the assembly source file name that
 *                        was passed to the program.  This is the full file name
 *                        that contains both the base and extension.
 * Parameters:  None.
 * Returns:  Character pointer to the path.
------------------------------------------------------------------------------*/
const char* get_assembly_source_full_file_name(void)
{
    return (const char*)s_cpPassedFileName;
}

/*------------------------------------------------------------------------------
 * Function name:  get_assembly_source_base_file_name
 * Function Description:  Getter function for the assembly source file name that
 *                        was passed to the program.  This only returns the base
 *                        portion of the file name without the extension.
 * Parameters:  None.
 * Returns:  Character pointer to the path.
------------------------------------------------------------------------------*/
const char* get_assembly_source_base_file_name(void)
{
    return (s_cpPassedBaseFileName != NULL) ? (const char*)s_cpPassedBaseFileName : (const char*)s_cpPassedFileName;
}

/*------------------------------------------------------------------------------
 * Function name:  free_argument_memory
 * Function Description:  Frees all allocated memory used by the passed
 *                        arguments.
 * Parameters:  None.
 * Returns:  None.
------------------------------------------------------------------------------*/
void free_argument_memory(void)
{
    if(s_cpPassedFilePath != NULL)
        free(s_cpPassedFilePath);

    if(s_cpPassedFileName != NULL)
        free(s_cpPassedFileName);

    if(s_cpArgumentCopy != NULL)
        free(s_cpArgumentCopy);

    if(s_cpPassedBaseFileName != NULL)
        free(s_cpPassedBaseFileName);
}

/*------------------------------------------------------------------------------
 * Function name:  parse_passed_file_path_and_name
 * Function Description:  Parser function that takes the passed argument and
 *                        saves the directory portion and the file name portion.
 * Parameters:
 * cpArgument - Character pointer to the passed argument.
 * Returns:  Zero for success and a negative number for failure.
------------------------------------------------------------------------------*/
static int parse_passed_file_path_and_name(const char* cpArgument)
{
    char* cpString;

    size_t iArgumentLength;
    size_t iPathLength;
    size_t iNameLength;
    size_t iBaseNameLength;

    //Check if a NULL pointer was passed.
    if(cpArgument == NULL)
    {
        print_error(__func__, NullPathAndFileName);
        return -NullPathAndFileName;
    }

    //The pointer is not NULL.  Check the first character of the passed character
    //pointer to see if it is a '-'.  An option might have been passed as the
    //last argument which is incorrect.
    if(cpArgument[0] == '-')
    {
        print_error(__func__, CommandLineSourceFileOutOfPlace);
        return -CommandLineSourceFileOutOfPlace;
    }

    //Get the length of the string passed as the argument.
    iArgumentLength = strlen(cpArgument);
    if(iArgumentLength == 0)
    {
        print_error(__func__, PathAndFileNameEmptyString);
        return -PathAndFileNameEmptyString;
    }

    //We have a non-empty string to work with.  We are going to use dirname() and
    //basename() library functions which alter the passed argument so we need to
    //make a copy of the passed argument.  First alocate memory.
    s_cpArgumentCopy = (char*)malloc(iArgumentLength + NULL_TERMINATING_BYTE_LENGTH);
    if(s_cpArgumentCopy == NULL)
    {
        print_error(__func__, MallocReturnedNull);
        return -MallocReturnedNull;
    }

    //Get the path.
    strncpy(s_cpArgumentCopy, cpArgument, iArgumentLength + NULL_TERMINATING_BYTE_LENGTH);
    cpString = dirname(s_cpArgumentCopy);

    //Allocate memory for the path string.  Function dirname() returns the string
    //up to, but not including, the final '\' so add one to the length because we
    //going to concatenate that character into the string.  Also, add one for the
    //NULL terminating byte.
    iPathLength = strlen(cpString) + 1 + NULL_TERMINATING_BYTE_LENGTH;
    s_cpPassedFilePath = (char*)malloc(iPathLength);
    if(s_cpPassedFilePath == NULL)
    {
        print_error(__func__, MallocReturnedNull);
        return -MallocReturnedNull;
    }

    //Memory allocated so build the path name.
    strncpy(s_cpPassedFilePath, cpString, iPathLength);
    strncat(s_cpPassedFilePath, "\\", 1);

    //Get the file name.
    strncpy(s_cpArgumentCopy, cpArgument, iArgumentLength + NULL_TERMINATING_BYTE_LENGTH);
    cpString = basename(s_cpArgumentCopy);

    //Allocate memory for the file name string.  The GNU version of basename()
    //returns the empty string when path has a trailing slash so check for a
    //length of zero.
    iNameLength = strlen(cpString) + NULL_TERMINATING_BYTE_LENGTH;
    if(iNameLength == 0)
    {
        print_error(__func__, FileNameLengthZero);
        return -FileNameLengthZero;
    }

    s_cpPassedFileName = (char*)malloc(iNameLength);
    if(s_cpPassedFileName == NULL)
    {
        print_error(__func__, MallocReturnedNull);
        return -MallocReturnedNull;
    }

    //Memory allocated so copy in the file name.
    strncpy(s_cpPassedFileName, cpString, iNameLength);

    //Check if this is a legitimate file name.  Look for:
    //1.  Length of one with only character being '/'
    //2.  Length of one with only character being '.'
    //3.  Length of two with only characters being ".."
    if(
       (strlen(s_cpPassedFileName) == 1 && s_cpPassedFileName[0] == '/') ||
       (strlen(s_cpPassedFileName) == 1 && s_cpPassedFileName[0] == '.') ||
       (strlen(s_cpPassedFileName) == 2 && s_cpPassedFileName[0] == '.' && s_cpPassedFileName[1] == '.')
      )
    {
        print_error(__func__, InvalidFileName);
        return -InvalidFileName;
    }

    //We want to check for an extension and if one exists then record just the
    //base name.
    cpString = strrchr(s_cpPassedFileName, '.');
    if(cpString != NULL)
    {
        //There is an extension.  The pointer cpString is currently pointing at
        //the '.' character that separates the base from the extension in the
        //file name.  The length of the base name is the current pointer position
        //minus the starting pointer of the string.  Add one for the NULL
        //terminating byte.
        iBaseNameLength = (size_t)cpString - (size_t)s_cpPassedFileName + NULL_TERMINATING_BYTE_LENGTH;

        //Allocate memory for the base name.
        s_cpPassedBaseFileName = (char*)malloc(iBaseNameLength);
        if(s_cpPassedBaseFileName == NULL)
        {
            print_error(__func__, MallocReturnedNull);
            return -MallocReturnedNull;
        }

        //Memory allocated so copy in only the base part of the file name.  Since
        //the length of the base name if less than the length of the entire file
        //name strncpy() will not copy in a NULL terminating byte.
        memset(s_cpPassedBaseFileName, '\0', iBaseNameLength);
        strncpy(s_cpPassedBaseFileName, s_cpPassedFileName, iBaseNameLength - NULL_TERMINATING_BYTE_LENGTH);
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  display_usage
 * Function Description:  Displays the proper usage for the program.
 * Parameters:  None.
 * Returns:  None.
------------------------------------------------------------------------------*/
static void display_usage(void)
{
    printf("4004 ASSEMBLER\n\n");
    printf("Assembler for the Intel 4004 CPU.  Generates a program listing file and\n");
    printf("binary machine code file from the passed assembly source file.\n");
    printf("\n");
    printf("Usage:  4004-as.exe [OPTIONS] FILE\n");
    printf("\n");
    printf("Mandatory arguments to long options are mandatory for short options too.\n");
    printf("\n");
    printf("Options:\n");
    printf("-h, --help                 display this help and exit.\n");
    printf("-l, --listing-file=ACTION  if ACTION is LIST, a listing file will be\n");
    printf("                           printed.  If ACTION is NOLIST, a listing file\n");
    printf("                           will not be printed.  Without this option a\n");
    printf("                           listing file will be printed.\n");
    printf("-s, --symbol-table=ACTION  if ACTION is SYM, a symbol table will be\n");
    printf("                           included in the listing file.  If ACTION is\n");
    printf("                           NOSYM, the symbol table will be excluded from\n");
    printf("                           the listing file.  Without this option a\n");
    printf("                           symbol table will be included in the listing\n");
    printf("                           file.\n");
    printf("-v, --version              output the version number and exit.\n");
}
