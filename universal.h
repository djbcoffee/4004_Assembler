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
 ** File: 4004-as/src/universal.h
 **
 ** Description:
 ** Header file that contains universal information for the project.
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___UNIVERSAL_H___
#define ___UNIVERSAL_H___

//------------------------------------------------------------------------------
//Defines
#ifndef FALSE
#define FALSE                           (0)
#endif
#ifndef TRUE
#define TRUE                            (1)
#endif

#define NULL_TERMINATING_BYTE_LENGTH    (1)

#define VERSION                         "VERSION 1.0.0"

//------------------------------------------------------------------------------
//Macros
//(void)(&_min1 == &_min2) is a guaranteed "no-op".  It forces the compiler to
//issue a diagnostic when the types of x and y are not the same.  Note that
//testing with _min1 == _min2 would implicitly convert one of the values to the
//other type.  If writing a header file that must work when included in ISO C
//programs, write __typeof__ instead of typeof.
#define bmc_min(x, y)                               (                                           \
                                                        {                                       \
                                                            __typeof__(x) _min1 = (x);          \
                                                            __typeof__(y) _min2 = (y);          \
                                                            (void)(&_min1 == &_min2);           \
                                                            (_min1 < _min2) ? _min1 : _min2;    \
                                                        }                                       \
                                                    )

//(void)(&_max1 == &_max2) is a guaranteed "no-op".  It forces the compiler to
//issue a diagnostic when the types of x and y are not the same.  Note that
//testing with _max1 == _max2 would implicitly convert one of the values to the
//other type.  If writing a header file that must work when included in ISO C
//programs, write __typeof__ instead of typeof.
#define bmc_max(x, y)                               (                                           \
                                                        {                                       \
                                                            __typeof__(x) _max1 = (x);          \
                                                            __typeof__(y) _max2 = (y);          \
                                                            (void)(&_max1 == &_max2);           \
                                                            (_max1 > _max2) ? _max1 : _max2;    \
                                                        }                                       \
                                                    )

//------------------------------------------------------------------------------
//Enumerations
enum Errors
{
    NoError = 0,
    UnknownOption,
    UnknownListingFileOptionArgument,
    UnknownSymbolTableOptionArgument,
    MalformedCommandLine,
    NullPathAndFileName,
    CommandLineSourceFileOutOfPlace,
    PathAndFileNameEmptyString,
    MallocReturnedNull,
    FileNameLengthZero,
    InvalidFileName,
    FileAlreadyOpen,
    FileOpenError,
    FileNotOpen,
    GetlineError,
    ResetFileError,
    FileWriteError,
    LetterExpectedSyntaxError,
    InvalidCharacterSyntaxError,
    DuplicateSymbolError,
    EndDirectiveNotAloneError,
    UnexpectedEndOfStatementSyntaxError,
    SymbolLengthExceedsMaximumError,
    InvalidValueError,
    ExpectedEquAfterSymbolSyntaxError,
    EndOfStatementExpectedError,
    UnknownSymbolError,
    TooManyOperandsSyntaxError,
    ExceededProgramMemoryError,
    IncorrectNumberOfOperandsError,
    NumberStackFull,
    RightParenthesisExpected,
    NumberStackEmpty,
    LocationCounterBackwards
};

//------------------------------------------------------------------------------
//Structures
//None

//------------------------------------------------------------------------------
//Prototypes
//None

#endif /*___UNIVERSAL_H___*/
