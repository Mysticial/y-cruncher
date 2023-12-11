/* ConfigObject.cpp
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
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "PublicLibs/Exceptions/ParseException.h"
#include "ConfigArray.h"
#include "ConfigObject.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigObject::ConfigObject(const ConfigObject& x){
    for (const auto& item : x.m_map){
        add_value(item.first, item.second.value.clone(), item.second.comment);
    }
}
void ConfigObject::operator=(const ConfigObject& x){
    if (this == &x){
        return;
    }
    *this = ConfigObject(x);
}
ConfigObject ConfigObject::clone() const{
    return *this;
}
void ConfigObject::clear(){
    m_description.clear();
    m_map.clear();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Add Value
void ConfigObject::sanitize_key(const std::string& key) const{
    if (key.empty()){
        throw InvalidParametersException("ConfigObject::sanitize_key()", "Key cannot be empty.");
    }
    {
        char ch = key[0];
        if (!(('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z'))){
            throw InvalidParametersException("ConfigObject::sanitize_key()", "Key must start with a letter.");
        }
    }
    for (char ch : key){
        if ('A' <= ch && ch <= 'Z'){
            continue;
        }
        if ('a' <= ch && ch <= 'z'){
            continue;
        }
        if ('0' <= ch && ch <= '9'){
            continue;
        }
        switch (ch){
        case '_':
        case '-':
        case '.':
            continue;
        }
        throw InvalidParametersException("ConfigObject::sanitize_key()", "Key must be alphanumeric.");
    }
}
void ConfigObject::add_bool(const std::string& key, bool value, std::string comment){
    sanitize_key(key);
    auto iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::move(comment), value)
    );
    if (!iter.second){
        throw InvalidParametersException("ConfigObject::add_bool()", "Attempted to add an existing key.");
    }
}
void ConfigObject::add_integer(const std::string& key, siL_t value, std::string comment){
    sanitize_key(key);
    auto iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::move(comment), value)
    );
    if (!iter.second){
        throw InvalidParametersException("ConfigObject::add_integer()", "Attempted to add an existing key.");
    }
}
void ConfigObject::add_string(const std::string& key, std::string value, std::string comment){
    sanitize_key(key);
    auto iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::move(comment), std::move(value))
    );
    if (!iter.second){
        throw InvalidParametersException("ConfigObject::add_string()", "Attempted to add an existing key.");
    }
}
ConfigValue& ConfigObject::add_value(const std::string& key, ConfigValue&& value, std::string comment){
    sanitize_key(key);
    auto iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::move(comment), std::move(value))
    );
    if (!iter.second){
        throw InvalidParametersException("Object::add_node()", "Attempted to add an existing key.");
    }
    return iter.first->second.value;
}
ConfigArray& ConfigObject::add_array(const std::string& key, std::string comment){
    sanitize_key(key);
    auto iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::move(comment), ConfigArray())
    );
    if (!iter.second){
        throw InvalidParametersException("ConfigObject::add_array()", "Attempted to add an existing key.");
    }
    return iter.first->second.value.to_array_throw();
}
ConfigObject& ConfigObject::add_object(const std::string& key, std::string description, std::string comment){
    sanitize_key(key);
    auto iter = m_map.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(std::move(comment), ConfigObject(std::move(description)))
    );
    if (!iter.second){
        throw InvalidParametersException("ConfigObject::add_object()", "Attempted to add an existing key.");
    }
    return iter.first->second.value.to_object_throw();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Combine Objects
void ConfigObject::combine(ConfigObject&& obj){
    if (this == &obj){
        return;
    }
    for (auto& item : obj){
        add_value(item.first, std::move(item.second.value), std::move(item.second.comment));
    }
    obj.m_map.clear();
}
void ConfigObject::operator+=(ConfigObject&& obj){
    if (this == &obj){
        return;
    }
    for (auto& item : obj){
        add_value(item.first, std::move(item.second.value), std::move(item.second.comment));
    }
    obj.m_map.clear();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const ConfigValue& ConfigObject::operator[](const std::string& key) const{
    auto iter = m_map.find(key);
    if (iter != m_map.end()){
        return iter->second.value;
    }
    throw KeyNotFoundException(key);
}
ConfigValue& ConfigObject::operator[](const std::string& key){
    auto iter = m_map.find(key);
    if (iter != m_map.end()){
        return iter->second.value;
    }

    throw KeyNotFoundException(key);
}
const ConfigValue* ConfigObject::try_get(const std::string& key) const{
    auto iter = m_map.find(key);
    if (iter != m_map.end()){
        return &iter->second.value;
    }
    return nullptr;
}
ConfigValue* ConfigObject::try_get(const std::string& key){
    auto iter = m_map.find(key);
    if (iter != m_map.end()){
        return &iter->second.value;
    }
    return nullptr;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool ConfigObject::get_bool(const std::string& key) const{
    return operator[](key).to_boolean_throw();
}
siL_t ConfigObject::get_integer(const std::string& key) const{
    return operator[](key).to_integer_throw();
}
const std::string& ConfigObject::get_string(const std::string& key) const{
    return operator[](key).to_string_throw();
}
const ConfigArray& ConfigObject::get_array(const std::string& key) const{
    return operator[](key).to_array_throw();
}
const ConfigObject& ConfigObject::get_object(const std::string& key) const{
    return operator[](key).to_object_throw();
}
bool ConfigObject::get_bool(const std::string& key, bool default_value) const{
    const ConfigValue* value = try_get(key);
    if (value == nullptr){
        return default_value;
    }
    return value->to_boolean_throw();
}
siL_t ConfigObject::get_integer(const std::string& key, siL_t default_value) const{
    const ConfigValue* value = try_get(key);
    if (value == nullptr){
        return default_value;
    }
    return value->to_integer_throw();
}
const std::string& ConfigObject::get_string(const std::string& key, const std::string& default_value) const{
    const ConfigValue* value = try_get(key);
    if (value == nullptr){
        return default_value;
    }
    return value->to_string_throw();
}
void ConfigObject::verify_keys(const char* function, const std::set<std::string>& set) const{
    for (const auto& item : m_map){
        if (set.find(item.first) == set.end()){
            throw InvalidParametersException(
                function,
                "Invalid Key: " + item.first
            );
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool operator==(const ConfigObject& a, const ConfigObject& b){
    if (a.size() != b.size()){
        return false;
    }
    auto iter0 = a.begin();
    auto iter1 = b.begin();
    for (; iter0 != a.end(); ++iter0, ++iter1){
        if (iter0->first != iter1->first){
            return false;
        }
        if (!(iter0->second.value == iter1->second.value)){
            return false;
        }
    }
    return true;
}
std::string ConfigObject::to_acfg_string(upL_t depth, bool comments) const{
    //  Empty vector
    if (m_map.empty()){
        return "{}";
    }

    std::string ret = "{";
    if (comments && !m_description.empty()){
        ret += "   //  ";
        ret += m_description;
    }
    ret += "\r\n";
    for (const auto& value : m_map){
        const std::string& comment = value.second.comment;
        if (comments && !comment.empty()){
            ret += "\r\n";
            bool new_line = true;
            for (char ch : comment){
                if (new_line){
                    ret += indent(depth + 1) + "//  ";
                }
                new_line = ch == '\n';
                ret += ch;
            }
            if (!new_line){
                ret += "\r\n";
            }
        }
        ret += indent(depth + 1) + value.first + " : " + value.second.value.to_acfg_string(depth + 1, comments);
        if (ret.back() != '\n'){
            ret += "\r\n";
        }
    }
    ret += indent(depth) + "}\r\n";
    return ret;
}
std::string ConfigObject::to_json_string(bool trailing_comma, upL_t depth) const{
    //  Empty vector
    if (m_map.empty()){
        return trailing_comma ? "{}," : "{}";
    }

    std::string ret = "{\r\n";
    upL_t size = m_map.size();
    upL_t c = 0;
    for (const auto& item : m_map){
        ret += indent(depth + 1) + "\"" + item.first + "\": ";
        ret += item.second.value.to_json_string(c < size - 1, depth + 1);
        if (ret.back() != '\n'){
            ret += "\r\n";
        }
        c++;
    }
    ret += indent(depth);
    ret += trailing_comma ? "},\r\n" : "}\r\n";
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
