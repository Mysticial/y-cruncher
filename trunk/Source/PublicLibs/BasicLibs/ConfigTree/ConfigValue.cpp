/* ConfigValue.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/27/2016
 *  Last Modified   : 11/03/2023
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Exceptions/ParseException.h"
#include "ConfigValue.h"
#include "ConfigArray.h"
#include "ConfigObject.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string indent(upL_t depth){
    return std::string(depth * 4, ' ');
}
const char* type_to_string(ConfigValueType type){
    switch (type){
    case ConfigValueType::EMPTY:
        return "Null";
    case ConfigValueType::BOOLEAN:
        return "Boolean";
    case ConfigValueType::INTEGER:
        return "Integer";
    case ConfigValueType::STRING:
        return "String";
    case ConfigValueType::ARRAY:
        return "Array";
    case ConfigValueType::OBJECT:
        return "Object";
    }
    return "invalid type";
}
void process_label(std::string& label){
    if (!label.empty()){
        label += ": ";
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigValue::~ConfigValue(){
    clear();
}
ConfigValue::ConfigValue(ConfigValue&& x) noexcept
    : m_type(x.m_type)
{
    u = x.u;
    x.m_type = ConfigValueType::EMPTY;
}
void ConfigValue::operator=(ConfigValue&& x) noexcept{
    if (this == &x){
        return;
    }
    clear();
    m_type = x.m_type;
    u = x.u;
    x.m_type = ConfigValueType::EMPTY;
}
ConfigValue::ConfigValue(const ConfigValue& x)
    : m_type(x.m_type)
{
    switch (m_type){
    case ConfigValueType::STRING:
        u.m_string = new std::string(*x.u.m_string);
        break;
    case ConfigValueType::ARRAY:
        u.m_array = new ConfigArray(*x.u.m_array);
        break;
    case ConfigValueType::OBJECT:
        u.m_object = new ConfigObject(x.u.m_object->clone());
        break;
    default:
        u = x.u;
    }
}
void ConfigValue::operator=(const ConfigValue& x){
    if (this == &x){
        return;
    }
    ConfigValue tmp(x);
    operator=(std::move(tmp));
}
ConfigValue ConfigValue::clone() const{
    return *this;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ConfigValue::clear(){
    switch (m_type){
    case ConfigValueType::STRING:
        delete u.m_string;
        break;
    case ConfigValueType::ARRAY:
        delete u.m_array;
        break;
    case ConfigValueType::OBJECT:
        delete u.m_object;
        break;
    default:;
    }
    m_type = ConfigValueType::EMPTY;
}
ConfigValue::ConfigValue(bool x)
    : m_type(ConfigValueType::BOOLEAN)
{
    u.m_bool = x;
}
ConfigValue::ConfigValue(siL_t x)
    : m_type(ConfigValueType::INTEGER)
{
    u.m_integer = x;
}
ConfigValue::ConfigValue(const char* x)
    : m_type(ConfigValueType::STRING)
{
    u.m_string = new std::string(x);
}
ConfigValue::ConfigValue(std::string x)
    : m_type(ConfigValueType::STRING)
{
    u.m_string = new std::string(x);
}
ConfigValue::ConfigValue(ConfigArray&& x)
    : m_type(ConfigValueType::ARRAY)
{
    u.m_array = new ConfigArray(std::move(x));
}
ConfigValue::ConfigValue(ConfigObject&& x)
    : m_type(ConfigValueType::OBJECT)
{
    u.m_object = new ConfigObject(std::move(x));
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
[[noreturn]] void throw_type_mismatch(ConfigValueType expected, ConfigValueType actual, std::string label){
    process_label(label);
    label += "Expected ";
    label += type_to_string(expected);
    label += ", got ";
    label += type_to_string(actual);
    label += ".";
    throw ParseException(std::move(label));
}
bool ConfigValue::to_boolean_throw(const std::string& label) const{
    if (m_type == ConfigValueType::BOOLEAN){
        return u.m_bool;
    }
    if (m_type == ConfigValueType::STRING){
        if (*u.m_string == "true"){
            return true;
        }
        if (*u.m_string == "false"){
            return false;
        }
    }
    throw_type_mismatch(ConfigValueType::BOOLEAN, m_type, label);
}
int64_t ConfigValue::to_integer_throw(const std::string& label) const{
    if (m_type == ConfigValueType::INTEGER){
        return u.m_integer;
    }
    throw_type_mismatch(ConfigValueType::INTEGER, m_type, label);
}
const std::string& ConfigValue::to_string_throw(const std::string& label) const{
    if (m_type == ConfigValueType::STRING){
        return *u.m_string;
    }
    throw_type_mismatch(ConfigValueType::STRING, m_type, label);
}
std::string& ConfigValue::to_string_throw(const std::string& label){
    if (m_type == ConfigValueType::STRING){
        return *u.m_string;
    }
    throw_type_mismatch(ConfigValueType::STRING, m_type, label);
}
const ConfigArray& ConfigValue::to_array_throw(const std::string& label) const{
    if (m_type == ConfigValueType::ARRAY){
        return *u.m_array;
    }
    throw_type_mismatch(ConfigValueType::ARRAY, m_type, label);
}
ConfigArray& ConfigValue::to_array_throw(const std::string& label){
    if (m_type == ConfigValueType::ARRAY){
        return *u.m_array;
    }
    throw_type_mismatch(ConfigValueType::ARRAY, m_type, label);
}
const ConfigObject& ConfigValue::to_object_throw(const std::string& label) const{
    if (m_type == ConfigValueType::OBJECT){
        return *u.m_object;
    }
    throw_type_mismatch(ConfigValueType::OBJECT, m_type, label);
}
ConfigObject& ConfigValue::to_object_throw(const std::string& label){
    if (m_type == ConfigValueType::OBJECT){
        return *u.m_object;
    }
    throw_type_mismatch(ConfigValueType::OBJECT, m_type, label);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const std::string* ConfigValue::to_string_nothrow() const{
    if (m_type != ConfigValueType::STRING){
        return nullptr;
    }
    return u.m_string;
}
std::string* ConfigValue::to_string_nothrow(){
    if (m_type != ConfigValueType::STRING){
        return nullptr;
    }
    return u.m_string;
}
const ConfigArray* ConfigValue::to_array_nothrow() const{
    return m_type == ConfigValueType::ARRAY ? u.m_array : nullptr;
}
ConfigArray* ConfigValue::to_array_nothrow(){
    return m_type == ConfigValueType::ARRAY ? u.m_array : nullptr;
}
const ConfigObject* ConfigValue::to_object_nothrow() const{
    return m_type == ConfigValueType::OBJECT ? u.m_object : nullptr;
}
ConfigObject* ConfigValue::to_object_nothrow(){
    return m_type == ConfigValueType::OBJECT ? u.m_object : nullptr;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool operator==(const ConfigValue& a, const ConfigValue& b){
    if (a.m_type != b.m_type){
        return false;
    }
    switch (a.m_type){
    case ConfigValueType::EMPTY:
        return true;
    case ConfigValueType::BOOLEAN:
        return a.u.m_bool == b.u.m_bool;
    case ConfigValueType::INTEGER:
        return a.u.m_integer == b.u.m_integer;
    case ConfigValueType::STRING:
        return *a.u.m_string == *b.u.m_string;
    case ConfigValueType::ARRAY:
        return *a.u.m_array == *b.u.m_array;
    case ConfigValueType::OBJECT:
        return *a.u.m_object == *b.u.m_object;
    }
    return false;
}
bool operator!=(const ConfigValue& a, const ConfigValue& b){
    return !(a == b);
}
std::string ConfigValue::to_acfg_string(upL_t depth, bool comments) const{
    switch (m_type){
    case ConfigValueType::EMPTY:
        return "null";
    case ConfigValueType::BOOLEAN:
        return u.m_bool ? "true" : "false";
    case ConfigValueType::INTEGER:
        return std::to_string(u.m_integer);
    case ConfigValueType::STRING:{
        std::string ret = "\"";
        for (char ch : *u.m_string){
            switch (ch){
            case '\\':
                ret += "\\\\";
                break;
            case '\"':
                ret += "\\\"";
                break;
            default:
                ret += ch;
            }
        }
        ret += "\"";
        return ret;
    }
    case ConfigValueType::ARRAY:
        return u.m_array->to_acfg_string(depth, comments);
    case ConfigValueType::OBJECT:
        return u.m_object->to_acfg_string(depth, comments);
    }
    return "";
}
std::string ConfigValue::to_json_string(bool trailing_comma, upL_t depth) const{
    std::string ret;
    switch (m_type){
    case ConfigValueType::EMPTY:
        ret = "null";
        break;
    case ConfigValueType::BOOLEAN:
        ret = u.m_bool ? "true" : "false";
        break;
    case ConfigValueType::INTEGER:
        ret = std::to_string(u.m_integer);
        break;
    case ConfigValueType::STRING:{
        ret = "\"";
        for (char ch : *u.m_string){
            switch (ch){
            case '\\':
                ret += "\\\\";
                break;
            case '\"':
                ret += "\\\"";
                break;
            default:
                ret += ch;
            }
        }
        ret += "\"";
        break;
    }
    case ConfigValueType::ARRAY:
        return u.m_array->to_json_string(trailing_comma, depth);
    case ConfigValueType::OBJECT:
        return u.m_object->to_json_string(trailing_comma, depth);
    }
    if (trailing_comma){
        ret += ",";
    }
    return ret;
}
upL_t ConfigValue::depth() const{
    switch (m_type){
    case ConfigValueType::EMPTY:
    case ConfigValueType::BOOLEAN:
    case ConfigValueType::INTEGER:
    case ConfigValueType::STRING:
        return 1;
    case ConfigValueType::ARRAY:
        return u.m_array->depth() + 1;
    case ConfigValueType::OBJECT:
        return u.m_object->depth() + 1;
    }
    return 1;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
