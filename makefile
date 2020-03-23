#################################################################################
## Copyright (C) 2018 Donald J. Bartley <djbcoffee@gmail.com>
##
## This source file may be used and distributed without restriction provided that
## this copyright statement is not removed from the file and that any derivative
## work contains the original copyright notice and the associated disclaimer.
##
## This source file is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 of the License, or (at your option) any
## later version.
##
## This source file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this source file.  If not, see <http://www.gnu.org/licenses/> or write to the
## Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
## 02110-1301, USA.
#################################################################################
## File: makefile
##
## Description:
## Make file for the 4004 assembler.
#################################################################################
## DJB 10/13/18 Created.
#################################################################################

# Add input sources and outputs to the build variables
C_SRCS += \
arguments.c \
bstree.c \
expression.c \
files.c \
lexer.c \
log.c \
main.c 

OBJS += \
arguments.o \
bstree.o \
expression.o \
files.o \
lexer.o \
log.o \
main.o 

C_DEPS += \
arguments.d \
bstree.d \
expression.d \
files.d \
lexer.d \
log.d \
main.d

# Rules for building sources
%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_WIN32_WINNT=_WIN32_WINNT_WINXP -Os -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: 4004-as.exe

# Tool invocations
4004-as.exe: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C Linker'
	gcc  -o "4004-as.exe" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	rm -rf $(OBJS)$(C_DEPS) 4004-as.exe 4004-as.lst
	@echo ' '

# Post build rules
post-build:
	@echo 'Invoking: Create Listing'
	objdump --all-headers --demangle --disassemble --wide "4004-as.exe" > "4004-as.lst"
	@echo ' '
	@echo 'Invoking: Print Size'
	size --format=berkeley "4004-as.exe"
	@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
