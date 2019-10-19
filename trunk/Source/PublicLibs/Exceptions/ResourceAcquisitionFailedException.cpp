/* ResourceAcquisitionFailedException.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/16/2019
 * Last Modified    : 02/16/2019
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "PublicLibs/ExportSafeLibs/Stream.h"
#include "PublicLibs/BasicLibs/StringTools/ToString.h"
#include "ExceptionSerialization.h"
#include "ResourceAcquisitionFailedException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YMP_EXCEPTION_DEFINITIONS(ResourceAcquisitionFailedException)
YMP_EXCEPTION_DEFINITIONS(MemoryAllocationFailedException)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
MemoryAllocationFailedException::MemoryAllocationFailedException(const char* function, uiL_t bytes)
    : ResourceAcquisitionFailedException(
        function,
        "Allocation Type: (unspecified)\n"
        "Allocation Size: " + StringTools::tostr(bytes, StringTools::BYTES_EXPANDED)
    )
{}
MemoryAllocationFailedException::MemoryAllocationFailedException(const char* function, const char* type, uiL_t bytes)
    : ResourceAcquisitionFailedException(
        function,
        "Allocation Type:  " + std::string(type) + "\n" 
        "Allocation Size:  " + StringTools::tostr(bytes, StringTools::BYTES_EXPANDED)
    )
    , m_type(type)
    , m_bytes(bytes)
{}
void MemoryAllocationFailedException::print() const{
    Console::println("\n", 'R');
    Console::println_labelc("Exception Encountered", get_typename());
    Console::println();
    if (!m_function.empty()){
        Console::println_labelc("In Function", m_function);
        Console::println();
    }
    Console::println_labelm("Allocation Type:", m_type, 'Y');
    Console::println_labelm_ebytes("Allocation Size:", m_bytes, 'Y');
    Console::println("\n");
    Console::SetColor('w');
}
MemoryAllocationFailedException::MemoryAllocationFailedException(SerializationPassKey key, const char*& stream)
    : ResourceAcquisitionFailedException(key, stream)
{
    ExceptionTools::parse(stream, m_type);
    ExceptionTools::parse(stream, m_bytes);
}
void MemoryAllocationFailedException::serialize(std::string& stream) const{
    ResourceAcquisitionFailedException::serialize(stream);
    ExceptionTools::write(stream, m_type);
    ExceptionTools::write(stream, m_bytes);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
