/* SMC_DigitViewer.cpp - Single Module Compile
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/25/2015
 * Last Modified    : 01/25/2015
 * 
 */

#ifdef YMP_DIGITVIEWER_STANDALONE
#include "PublicLibs/StringTools/ToString.cpp"
#include "PublicLibs/StringTools/Unicode.cpp"
#include "PublicLibs/ConsoleIO/BasicIO.cpp"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Array.h"
#include "PublicLibs/Exceptions/SMC_Exceptions.cpp"
#include "PublicLibs/Memory/AlignedMalloc.cpp"
#include "PublicLibs/FileIO/FileException.cpp"
#include "PublicLibs/FileIO/BasicFile.cpp"
#include "PublicLibs/FileIO/FileIO.cpp"
#endif

#include "DigitConverter/DigitConverter.cpp"

#include "DigitReaders/DigitReader.cpp"
#include "DigitReaders/TextReader.cpp"
#include "DigitReaders/YCDReader.cpp"
#include "DigitReaders/YCDFileReader.cpp"

#include "DigitWriters/DigitWriter.cpp"
#include "DigitWriters/TextWriter.cpp"
#include "DigitWriters/YCDWriter.cpp"
#include "DigitWriters/YCDFileWriter.cpp"

#include "PrintHelpers.cpp"
#include "EndPointTasks.cpp"
#include "Menu_DigitViewer.cpp"
