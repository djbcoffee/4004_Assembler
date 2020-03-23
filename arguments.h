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
 ** File: 4004-as/src/arguments.h
 **
 ** Description:
 ** Header file that goes with arguments.c
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___ARGUMENTS_H___
#define ___ARGUMENTS_H___

//------------------------------------------------------------------------------
//Defines
//None

//------------------------------------------------------------------------------
//Enumerations
//None

//------------------------------------------------------------------------------
//Structures
//None

//------------------------------------------------------------------------------
//Prototypes
int process_arguments(int iArgc, char* acpArgv[]);
uint8_t is_listing_file_enabled(void);
uint8_t is_symbol_table_enabled(void);
const char* get_assembly_source_file_path(void);
const char* get_assembly_source_full_file_name(void);
const char* get_assembly_source_base_file_name(void);
void free_argument_memory(void);

#endif /*___ARGUMENTS_H___*/
