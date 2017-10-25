Digit Viewer
===========

This is the project folder for the Digit Viewer.

A stand-alone version of the Digit Viewer can be found here: https://github.com/Mysticial/DigitViewer

-----

**Compile Instructions:**

Windows:
 - Open the Visual Studio project and compile from within the IDE.
 - Visual Studio 2015 is required.

Linux:
 - Run `VSS - DigitViewer/Compile-Linux.sh` from the directory it is in. It will create a folder `Binaries` with the three executables.
 - x64 and a sufficiently new version of GCC is required.

-----

**Build Modes:**

 - x86 : Release
 - x86 : 04-SSE3
 - x64 : 04-SSE3
 - x64 : 07-Penryn (SSE4.1)
 - x64 : 13-Haswell (AVX2)

The x86 modes are not supported in Linux.

-----

To date, I've been too lazy to write a document on the compression format and how the .ycd files are laid out.
But it should be pretty easy to see from just examining it through a hex viewer.

Two things that might be useful:
 - Base 16 .ycd files are stored as 64-bit integers words with 16 digits per word.
 - Base 10 .ycd files are stored as 64-bit integers words with 19 digits per word.

In both cases, each 8-byte word is little-endian.

-----

**Directories:**

 - [`Source/DigitViewer/`](https://github.com/Mysticial/y-cruncher/tree/master/trunk/Source/DigitViewer) - Digit Viewer root directory.
 - `Source/DigitViewer/DigitReaders/` - The Digit Reader classes.
 - `Source/DigitViewer/DigitWriter/` - The Digit Writer classes.
 - `Source/DigitViewer/DigitConverter/` - The highly optimized (and unreadable) digit conversion primitives.

