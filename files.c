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
 ** File: 4004-as/src/files.c
 **
 ** Description:
 ** This translation (compilation) unit contains the functions for working with
 ** all the source, listing, and object files.
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
#ifndef _LIMITS_H
#include <limits.h>
#endif
#ifndef _TYPES_H_
#include <sys/types.h>
#endif

//Project-wide #includes
#ifndef ___UNIVERSAL_H___
#include "universal.h"
#endif

//Project #includes
#ifndef ___ARGUMENTS_H___
#include "arguments.h"
#endif
#ifndef ___LOG_H___
#include "log.h"
#endif

//Reflective #includes
#ifndef ___FILES_H___
#include "files.h"
#endif

//------------------------------------------------------------------------------
//Global Data
//None

//------------------------------------------------------------------------------
//Static Data
static FILE* s_pSourceFile = NULL;
static FILE* s_pListingFile = NULL;
static FILE* s_pBinaryFile = NULL;

//------------------------------------------------------------------------------
//Static Prototypes
#if _POSIX_C_SOURCE < 200809L
static ssize_t getline(char** cppBuffer, size_t* ipBufferSize, FILE* pFileStream);
static ssize_t getdelim(char** cppBuffer, size_t* ipBufferSize, int iDelimiter, FILE* pFileStream);
#endif

//==============================================================================
//Functions
/*------------------------------------------------------------------------------
 * Function name:  open_source_file
 * Function Description:  Opens the source file for reading.
 * Parameters:  None.
 * Returns:  Zero for success and negative number for failure.
------------------------------------------------------------------------------*/
int open_source_file(void)
{
    char caSourceFile[PATH_MAX + FILENAME_MAX + NULL_TERMINATING_BYTE_LENGTH];

    //Make sure the file isn't already open.
    if(s_pSourceFile != NULL)
    {
        print_error(__func__, FileAlreadyOpen);
        return -FileAlreadyOpen;
    }

    //The file is not yet open.  Create the full path and file name string.
    strncpy(caSourceFile, get_assembly_source_file_path(), sizeof(caSourceFile));
    strcat(caSourceFile, get_assembly_source_full_file_name());

    //Open the file.
    s_pSourceFile = fopen(caSourceFile, "rb");
    if(s_pSourceFile == NULL)
    {
        print_error(__func__, FileOpenError);
        return -FileOpenError;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  open_listing_file
 * Function Description:  Opens the listing file for writing.
 * Parameters:  None.
 * Returns:  Zero for success and non-zero for failure.
------------------------------------------------------------------------------*/
int open_listing_file(void)
{
    char caListingFile[PATH_MAX + FILENAME_MAX + NULL_TERMINATING_BYTE_LENGTH];

    //Make sure the file isn't already open.
    if(s_pListingFile != NULL)
    {
        print_error(__func__, FileAlreadyOpen);
        return -FileAlreadyOpen;
    }

    //The file is not yet open.  Create the full path and file name string.
    strncpy(caListingFile, get_assembly_source_file_path(), sizeof(caListingFile));
    strcat(caListingFile, get_assembly_source_base_file_name());
    strcat(caListingFile, LISTING_FILE_EXTENSION);

    //Open the file.
    s_pListingFile = fopen(caListingFile, "wb");
    if(s_pListingFile == NULL)
    {
        print_error(__func__, FileOpenError);
        return -FileOpenError;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  open_binary_file
 * Function Description:  Opens the binary object file for writing.
 * Parameters:  None.
 * Returns:  Zero for success and non-zero for failure.
------------------------------------------------------------------------------*/
int open_binary_file(void)
{
    char caBinaryFile[PATH_MAX + FILENAME_MAX + NULL_TERMINATING_BYTE_LENGTH];

    //Make sure the file isn't already open.
    if(s_pBinaryFile != NULL)
    {
        print_error(__func__, FileAlreadyOpen);
        return -FileAlreadyOpen;
    }

    //The file is not yet open.  Create the full path and file name string.
    strncpy(caBinaryFile, get_assembly_source_file_path(), sizeof(caBinaryFile));
    strcat(caBinaryFile, get_assembly_source_base_file_name());
    strcat(caBinaryFile, BINARY_FILE_EXTENSION);

    //Open the file.
    s_pBinaryFile = fopen(caBinaryFile, "wb");
    if(s_pBinaryFile == NULL)
    {
        print_error(__func__, FileOpenError);
        return -FileOpenError;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  reset_source_file
 * Function Description:  Resets the open source file back to the beginning.
 * Parameters:  None.
 * Returns:  Zero for success and non-zero for failure.
------------------------------------------------------------------------------*/
int reset_source_file(void)
{
    //Make sure the source file is opened.
    if(s_pSourceFile == NULL)
    {
        print_error(__func__, FileNotOpen);
        return -FileNotOpen;
    }

    //The file open so reset the current file position back to the beginning of
    //the file.
    if(fseek(s_pSourceFile, 0, SEEK_SET) != EXIT_SUCCESS)
    {
        print_error(__func__, ResetFileError);
        return -ResetFileError;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  read_line_from_source_file
 * Function Description:  Reads a line of text from the source file.  Before
 *                        calling, you should place in *cppBuffer the address of
 *                        a buffer *ipBufferSize bytes long, allocated with
 *                        malloc.  If this buffer is long enough to hold the
 *                        line, the text is stored in this buffer.  Otherwise,
 *                        the buffer will be made bigger using realloc, storing
 *                        the new buffer address back in *cppBuffer and the
 *                        increased size back in *ipBufferSize.  If you set
 *                        *cppBuffer to a NULL pointer, and *ipBufferSize to
 *                        zero, before the call, then the initial buffer will be
 *                        allocated for you by calling malloc.
 * Parameters:
 * cppBuffer - Address of a char* allocated with malloc where the line text is to
 *             be stored.  May be NULL in which case a buffer will be allocated
 *             using malloc.
 * ipBufferSize - A pointer to the size of the passed buffer.  If NULL is passed
 *                for *cppBuffer set the buffer size to zero and a buffer will be
 *                allocated using malloc.
 * Returns:  When successful, it returns the number of characters read (including
 *           the newline, but not including the terminating NULL).  If end of
 *           file is reached without any bytes read zero is returned.  If an error
 *           occurs a negative number is returned.
------------------------------------------------------------------------------*/
ssize_t read_line_from_source_file(char** cppBuffer, size_t* ipBufferSize)
{
    ssize_t iFunctionReturnValue;

    //Make sure the file is open.
    if(s_pSourceFile == NULL)
    {
        print_error(__func__, FileNotOpen);
        return -FileNotOpen;
    }

    //The file is open so read a line of text from the stream.
    iFunctionReturnValue = getline(cppBuffer, ipBufferSize, s_pSourceFile);
    if(iFunctionReturnValue != -1)
    {
        //If EOF is reached before a newline character then getline() doesn't
        //NULL terminate the string so check if we need to do so here.
        if(feof(s_pSourceFile) != 0)
            (*cppBuffer)[iFunctionReturnValue] = '\0';

        //A good line read was done so return the amount of characters read.
        return iFunctionReturnValue;
    }
    else
    {
        //We got a negative one which means an error occurred or EOF was reached.
        //Check for EOF and return the proper value.
        if(feof(s_pSourceFile) != 0)
        {
            //EOF.
            return 0;
        }
        else
        {
            //Error.
            print_error(__func__, GetlineError);
            return -GetlineError;
        }
    }
}

/*------------------------------------------------------------------------------
 * Function name:  write_line_to_listing_file
 * Function Description:  Writes a passed string to the listing file.
 * Parameters:
 * cpString - The text to write the the listing file.
 * Returns:  Zero for success and non-zero for failure.
------------------------------------------------------------------------------*/
int write_line_to_listing_file(char* cpString)
{
    //Make sure the file is open.
    if(s_pListingFile == NULL)
    {
        print_error(__func__, FileNotOpen);
        return -FileNotOpen;
    }

    //The file is open so write the line.
    if(fprintf(s_pListingFile, "%s\r\n", cpString) <= 0)
    {
        print_error(__func__, FileWriteError);
        return -FileWriteError;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  write_data_to_binary_file
 * Function Description:  Writes data to the binary object file.
 * Parameters:
 * ipData - Pointer to the data bytes to the written.
 * iLength - The amount of data bytes to write.
 * Returns:  Zero for success and non-zero for failure.
------------------------------------------------------------------------------*/
int write_data_to_binary_file(uint8_t* ipData, size_t iLength)
{
    //Make sure the file is open.
    if(s_pBinaryFile == NULL)
    {
        print_error(__func__, FileNotOpen);
        return -FileNotOpen;
    }

    //The file is open.  Write all the data bytes.
    while(iLength != 0)
    {
        if(fputc(*ipData, s_pBinaryFile) == EOF)
        {
            //EOF is returned when an error occurs.
            print_error(__func__, FileWriteError);
            return -FileWriteError;
        }

        //No error occurred so increment to the next data byte and decrement the
        //length.
        ipData++;
        iLength--;
    }

    return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * Function name:  close_all_files
 * Function Description:  Closes all open files.
 * Parameters:  None.
 * Returns:  None.
------------------------------------------------------------------------------*/
void close_all_files(void)
{
    if(s_pSourceFile != NULL)
        fclose(s_pSourceFile);
    if(s_pListingFile != NULL)
        fclose(s_pListingFile);
    if(s_pBinaryFile != NULL)
        fclose(s_pBinaryFile);
}

#if _POSIX_C_SOURCE < 200809L
/*------------------------------------------------------------------------------
 * Function name:  getline
 * Function Description:  Reads an entire line from pFileStream, storing the text
 *                        (including the newline and a terminating NULL
 *                        character) in a buffer and storing the buffer address
 *                        in *cppBuffer.  Before calling getline, you should
 *                        place in *cppBuffer the address of a buffer
 *                        *ipBufferSize bytes long, allocated with malloc.  If
 *                        this buffer is long enough to hold the line, getline
 *                        stores the line in this buffer.  Otherwise, getline
 *                        makes the buffer bigger using realloc, storing the new
 *                        buffer address back in *cppBuffer and the increased
 *                        size back in *ipBufferSize.  If you set *ccpBuffer to a
 *                        NULL pointer, and *ipBufferSize to zero, before the
 *                        call, then getline allocates the initial buffer for you
 *                        by calling malloc.  In either case, when getline
 *                        returns, *cppBuffer is a char* which points to the text
 *                        of the line.
 * Parameters:
 * cppBuffer - Address of a char* allocated with malloc where the line text is to
 *             be stored.  May be NULL in which case getline will allocate the
 *             buffer using malloc.
 * ipBufferSize - A pointer to the size of the passed buffer.  If NULL is passed
 *                for *cppBuffer set the buffer size to zero and getline will
 *                allocate the buffer using malloc.
 * pFileStream - Pointer to the file stream on which to do the read.
 * Returns:  When successful, it returns the number of characters read (including
 *           the newline, but not including the terminating NULL).  This value
 *           enables you to distinguish NULL characters that are part of the line
 *           from the NULL character inserted as a terminator.  If an error
 *           occurs or end of file is reached without any bytes read, -1 is
 *           returned.
------------------------------------------------------------------------------*/
static ssize_t getline(char** cppBuffer, size_t* ipBufferSize, FILE* pFileStream)
{
    return getdelim(cppBuffer, ipBufferSize, '\n', pFileStream);
}

/*------------------------------------------------------------------------------
 * Function name:  getdelim
 * Function Description:  Reads text from pFileStream, storing the text
 *                        (including the delimiting character iDelimiter and a
 *                        terminating NULL character) in a buffer and storing the
 *                        buffer address in *cppBuffer.  Before calling getdelim,
 *                        you should place in *cppBuffer the address of a buffer
 *                        *ipBufferSize bytes long, allocated with malloc.  If
 *                        this buffer is long enough to hold the line, getdelim
 *                        stores the line in this buffer.  Otherwise, getdelim
 *                        makes the buffer bigger using realloc, storing the new
 *                        buffer address back in *cppBuffer and the increased
 *                        size back in *ipBufferSize.  If you set *ccpBuffer to a
 *                        NULL pointer, and *ipBufferSize to zero, before the
 *                        call, then getdelim allocates the initial buffer for
 *                        you by calling malloc.  In either case, when getdelim
 *                        returns, *cppBuffer is a char* which points to the text
 *                        of the line.
 * Parameters:
 * cppBuffer - Address of a char* allocated with malloc where the line text is to
 *             be stored.  May be NULL in which case getdelim will allocate the
 *             buffer using malloc.
 * ipBufferSize - A pointer to the size of the passed buffer.  If NULL is passed
 *                for *cppBuffer set the buffer size to zero and getdelim will
 *                allocate the buffer using malloc.
 * iDelimiter - The delimiting character on which to stop the read.
 * pFileStream - Pointer to the file stream on which to do the read.
 * Returns:  When successful, it returns the number of characters read (including
 *           the delimiting character iDelimiter, but not including the
 *           terminating NULL).  This value enables you to distinguish NULL
 *           characters that are part of the line from the NULL character
 *           inserted as a terminator.  If an error occurs or end of file is
 *           reached without any bytes read, -1 is returned.
------------------------------------------------------------------------------*/
static ssize_t getdelim(char** cppBuffer, size_t* ipBufferSize, int iDelimiter, FILE* pFileStream)
{
    char* cpBufferPosition;
    char* cpBufferEnd;
    char* cpNewBuffer;

    int iCharacter;

    ssize_t iReturnValue;
    ssize_t iNumCharactersSoFar;

    size_t iNewBufferSize;

    //If a NULL was passed for the buffer or the buffer size is zero then
    //allocate a buffer using the standard buffer size.
    if(*cppBuffer == NULL || *ipBufferSize == 0)
    {
        *ipBufferSize = BUFSIZ;
        if((*cppBuffer = malloc(*ipBufferSize)) == NULL)
            return -1;
    }

    //We have a buffer to which to write text.  Start the position at the
    //beginning of the buffer and mark where the end of the buffer size based on
    //the size of the buffer.
    cpBufferPosition = *cppBuffer;
    cpBufferEnd = *cppBuffer + *ipBufferSize;

    //We will exit this loop when one of the following conditions is met:
    //1.  End of file is reached.
    //2.  An error occurs while reading from the stream.
    //3.  The delimiting character is reached.
    //4.  An error occurs while trying to expand the buffer size.
    //Initially set the return value as an error occurred.
    iReturnValue = -1;
    while(TRUE)
    {
        //Get a character from the stream.  If EOF is returned then either the
        //end of the file was reached for an error occurred.  In that case create
        //the return value and break from the while() loop.
        iCharacter = fgetc(pFileStream);
        if(iCharacter == EOF)
        {
            if(feof(pFileStream) != 0 && cpBufferPosition != *cppBuffer)
                iReturnValue = cpBufferPosition - *cppBuffer;
            break;
        }

        //Character was successfully read.  Store it in the buffer at the current
        //position and then increment the position.
        *cpBufferPosition = iCharacter;
        cpBufferPosition++;

        //Check if the character was the delimiter and if so place a NULL
        //terminating byte at the end of the text, calculate the return value,
        //and break from the while() loop.
        if(iCharacter == iDelimiter)
        {
            *cpBufferPosition = '\0';
            iReturnValue = cpBufferPosition - *cppBuffer;
            break;
        }

        //The character was not the delimiting character and there is more text
        //to read.  If we are near the limit of the passed buffer then the buffer
        //size needs to be expanded.  Add two to the current position, one for
        //the delimiting character and one for the NULL terminating byte.
        if((cpBufferPosition + 2) >= cpBufferEnd)
        {
            //Double the buffer size.
            iNewBufferSize = *ipBufferSize * 2;

            //Make note of how many characters are stored in the buffer so far.
            iNumCharactersSoFar = cpBufferPosition - *cppBuffer;

            //Expand the buffer size
            if((cpNewBuffer = realloc(*cppBuffer, iNewBufferSize)) == NULL)
                break;

            //Buffer expanded, store the new information about the expanded
            //buffer.
            *cppBuffer = cpNewBuffer;
            *ipBufferSize = iNewBufferSize;
            cpBufferEnd = cpNewBuffer + iNewBufferSize;
            cpBufferPosition = cpNewBuffer + iNumCharactersSoFar;
        }
    }

    return iReturnValue;
}
#endif
