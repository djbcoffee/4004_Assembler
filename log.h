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
 ** File: 4004-as/src/log.h
 **
 ** Description:
 ** Header file that goes with log.c
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___LOG_H___
#define ___LOG_H___

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
void print_error(const char* cpFunction, uint8_t iErrorNumber);

#endif /*___LOG_H___*/
