/* SMC_DigitViewer.cpp - Single Module Compile
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/25/2015
 * Last Modified    : 01/25/2015
 * 
 */

#ifdef YMP_DIGITVIEWER_STANDALONE
#include "PublicLibs/ConsoleIO/BasicIO.cpp"
#include "PublicLibs/ConsoleIO/Margin.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ConsoleIO/Array.h"
#include "PublicLibs/Exceptions/SMC_Exceptions.cpp"
#include "PublicLibs/BasicLibs/StringTools/ToString.cpp"
#include "PublicLibs/BasicLibs/StringTools/Unicode.cpp"
#include "PublicLibs/BasicLibs/Memory/AlignedMalloc.cpp"
#include "PublicLibs/SystemLibs/FileIO/FileException.cpp"
#include "PublicLibs/SystemLibs/FileIO/BasicFile.cpp"
#include "PublicLibs/SystemLibs/FileIO/FileIO.cpp"
#endif

#include "DigitConverter/DigitConverter.cpp"

#include "DigitReaders/InconsistentMetadataException.cpp"
#include "DigitReaders/ParsingTools.cpp"
#include "DigitReaders/DigitReader.cpp"
#include "DigitReaders/TextDigitReader.cpp"
#include "DigitReaders/YCDDigitReader.cpp"
#include "DigitReaders/YCDFileReader.cpp"

#include "DigitWriters/DigitWriter.cpp"
#include "DigitWriters/TextDigitWriter.cpp"
#include "DigitWriters/YCDDigitWriter.cpp"
#include "DigitWriters/YCDFileWriter.cpp"

#include "PrintHelpers.cpp"
#include "EndPointTasks.cpp"
#include "Menu_DigitViewer.cpp"
