/* ExceptionSerialization.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 04/09/2017
 *  Last Modified   : 04/09/2017
 * 
 */

#pragma once
#ifndef ymp_Exceptions_ExceptionSerialization_H
#define ymp_Exceptions_ExceptionSerialization_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <type_traits>
#include <string>
namespace ymp{
namespace ExceptionTools{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename PrimitiveType>
void write(std::string& stream, const PrimitiveType& data){
    static_assert(std::is_trivially_copyable<PrimitiveType>::value, "Type must be trivially copyable.");
    stream.append((const char*)&data, sizeof(data));
}
template <typename PrimitiveType>
void parse(const char*& stream, PrimitiveType& data){
    static_assert(std::is_trivially_copyable<PrimitiveType>::value, "Type must be trivially copyable.");
    memcpy(&data, stream, sizeof(data));
    stream += sizeof(data);
}
////////////////////////////////////////////////////////////////////////////////
inline void write(std::string& stream, const std::string& data){
    write(stream, data.size());
    stream.append(data.c_str(), data.size());
}
inline void parse(const char*& stream, std::string& data){
    size_t size;
    parse(stream, size);

    data.resize(size);
    memcpy(&data[0], stream, size);
    stream += size;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
#endif
