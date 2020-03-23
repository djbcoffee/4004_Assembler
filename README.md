# Intel 4004 CPU Assembler
A Windows command line assembler for the Intel 4004 CPU.

The project page for a homegrown MCS-4 system built around the Intel 4004 CPU and supporting chip set, with hardware information and assembler user manual, can be found [here](https://sites.google.com/view/m-chips/mcs-4).

## Archive content

The following files are provided:
* arguments.c - Source code file
* arguments.h - Source code file
* bstree.c - Source code file
* bstree.h - Source code file
* expression.c - Source code file
* expression.h - Source code file
* files.c - Source code file
* files.h - Source code file
* lexer.c - Source code file
* lexer.h - Source code file
* log.c - Source code file
* log.h - Source code file
* main.c - Source code file
* main.h - Source code file
* universal.h - Source code file
* makefile - Make file for building the source
* 4004-as.exe - Windows command line executable
* Notepad++ 4004 User Defined Language.xml - Notepad++ User defined language for 4004 assembly
* LICENSE - License text
* README.md - This file

## Prerequisites

To build the source code MinGW needs to be installed on your PC.  MinGW can be obtained from [here](https://osdn.net/projects/mingw/releases/).

Only MinGW is needed, MSYS is not mandatory.  A Windows version of make, echo, and rm is also needed.  MinGW comes with its own make but this [one](http://gnuwin32.sourceforge.net/packages/make.htm) was used for the build.

echo and rm can also be obtained via gnuwin32 as part of the core utilities [here](http://gnuwin32.sourceforge.net/packages/coreutils.htm).

## Installing

Place the files into any convenient location on your PC.

The executable that was supplied, 4004-as.exe, was compiled with MinGW GCC-6.3.0-1.  This can be used to assemble programs as is with no further setup.  If you wish to do a build continue with the following steps.

Open the Windows command prompt and navigate to the folder which contains the source code.  If the path for the MinGW binaries, make, echo, and rm were added to PATH you can skip this step:\
Execute this command in the command window to set the path to the MinGW GCC compiler:\
```set PATH=%PATH%;C:\path\to\mingw\binaries```

Repeat the above command to add the paths to make, echo, and rm.

Once the paths are properly set type in the following to make the executable:\
```make all```

## Operating instructions

The user manual can be found [here](https://sites.google.com/view/m-chips/mcs-4).

The source file can be created and edited within any text editor program.  Once a source file is ready to assemble, you must first save it to a convenient location on your PC. Once this is completed, you will run the assembler through the command line.  The following is the usage from the program:

<pre>
Usage:  4004-as.exe [OPTIONS] FILE
&nbsp;
Mandatory arguments to long options are mandatory for short options too.
&nbsp;
Options:
-h, --help	display this help and exit.
-l, --listing-file=ACTION	if ACTION is LIST, a listing file will be printed. If ACTION is NOLIST, a listing file will not be printed.  Without this option a listing file will be printed.
-s, --symbol-table=ACTION	if ACTION is SYM, a symbol table will be included in the listing file.  If ACTION is NOSYM, the symbol table will be excluded from the listing file.  Without this option a symbol table will be included in the listing file.
-v, --version	output the version number and exit.
</pre>

Assembly of a file called test.asm:\
```4004-as.exe c:\4004\test.asm```

Assembly of a file called test.asm with no listing file:\
```4004-as.exe –listing-file=NOLIST c:\4004\test.asm```

A successful assembly will show the following on the command line:
<pre>
Pass 1 completed successfully.
Pass 2 completed successfully.
Assembly successful.
</pre>

Errors will generate an error on the command line that explains the type of error, indicates the line number the error is on, shows the source line, and the location of the error.  For example, this error shows an end of statement expected error:
<pre>
Pass 1 completed successfully.
ERROR:  In function statement_lexer() end of statement expected error.
Line 12:
   LDM 0 A     ;Load 0 to accumulator
         ^
</pre>

This shows that pass 1 completed successfully but during pass 2 assembly the LDM instruction on line 12 did not end with the single operand because of the character which is pointed to by the carat ‘^’ symbol.  The ‘A’ is out of place.  However, if a comma is added between the ‘0’ and the ‘A’ an error still occurs because LDM is only supposed to have one operand:
<pre>
ERROR:  In function statement_lexer() incorrect number of operands error.
Line 12:
   LDM 0,A     ;Load 0 to accumulator
       ^
</pre>

In this error the carat points to the beginning of the operands and not the extra operand but the explanation “incorrect number of operands error” indicates the actual cause of the error.  For information about all the errors the assembler can throw see the [4004 Assembler User Manual](https://sites.google.com/view/m-chips/mcs-4).

## Built With

* [MinGW GCC-6.3.0-1](https://osdn.net/projects/mingw/releases/) - The toolchain used

## Version History

* v1.0.0 - 2018 
	- Initial release

## Authors

* **Donald J Bartley** - *Initial work* - [djbcoffee](https://github.com/djbcoffee)

## License

This project is licensed under the GNU Public License 2 - see the [LICENSE](LICENSE) file for details
