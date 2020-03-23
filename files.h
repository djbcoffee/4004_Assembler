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
 ** File: 4004-as/src/files.h
 **
 ** Description:
 ** Header file that goes with files.c
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___FILES_H___
#define ___FILES_H___

//------------------------------------------------------------------------------
//Defines
#define LISTING_FILE_EXTENSION          ".lst"
#define BINARY_FILE_EXTENSION           ".bin"

#define LISTING_FILE_TITLE              "4004 ASSEMBLER"

#define LISTING_FILE_MAX_COLUMNS                        (80)
#define LISTING_FILE_LINE_MAX_DIGITS                    (5)
#define LISTING_FILE_LINE_FIELD_LENGTH                  (7)
#define LISTING_FILE_LC_MAX_CHARACTERS                  (4)
#define LISTING_FILE_LC_FIELD_LENGTH                    (6)
#define LISTING_FILE_OBJECT_CODE_MAX_CHARACTERS         (8)
#define LISTING_FILE_OBJECT_CODE_FIELD_LENGTH           (10)
#define LISTING_FILE_OBJECT_CODE_BYTE_MAX_CHARACTERS    (3)
#define LISTING_FILE_OBJECT_CODE_BYTES_PER_FIELD        (3)

//------------------------------------------------------------------------------
//Enumerations
//None

//------------------------------------------------------------------------------
//Structures
//None

//------------------------------------------------------------------------------
//Prototypes
int open_source_file(void);
int open_listing_file(void);
int open_binary_file(void);
int reset_source_file(void);
ssize_t read_line_from_source_file(char** cppBuffer, size_t* ipBufferSize);
int write_line_to_listing_file(char* cpString);
int write_data_to_binary_file(uint8_t* ipData, size_t iLength);
void close_all_files(void);

#endif /*___FILES_H___*/
