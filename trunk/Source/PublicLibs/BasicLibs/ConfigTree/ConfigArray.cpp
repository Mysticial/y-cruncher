/* ConfigArray.cpp
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
#include "ConfigArray.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigArray::ConfigArray(const ConfigArray& x){
    for (const auto& item : x.m_value){
        m_value.emplace_back(item.clone());
    }
}
void ConfigArray::operator+=(siL_t value){
    m_value.emplace_back(value);
}
void ConfigArray::operator+=(ConfigValue&& value){
    m_value.emplace_back(std::move(value));
}
bool operator==(const ConfigArray& a, const ConfigArray& b){
    if (a.size() != b.size()){
        return false;
    }
    for (upL_t c = 0; c < a.size(); c++){
        if (!(a[c] == b[c])){
            return false;
        }
    }
    return true;
}
std::string ConfigArray::to_acfg_string(upL_t depth, bool comments) const{
    if (m_value.empty()){
        return "[]";
    }

    bool one_line;
    {
        bool all_integer = true;
        for (const auto& item : m_value){
            if (!item.is_integer()){
                all_integer = false;
            }
        }
        bool one_string = m_value.size() == 1 && m_value[0].is_string();
        one_line = all_integer || one_string;
    }

    std::string ret;
    if (one_line){
        ret += "[" + m_value[0].to_acfg_string();
        for (upL_t c = 1; c < m_value.size(); c++){
            ret += " ";
            ret += m_value[c].to_acfg_string(depth + 1);
        }
        ret += "]";
    }else{
        ret += "[\r\n";
        for (const auto& value : m_value){
            ret += indent(depth + 1) + value.to_acfg_string(depth + 1, comments);
            if (ret.back() != '\n'){
                ret += "\r\n";
            }
        }
        ret += indent(depth) + "]\r\n";
    }
    return ret;
}
std::string ConfigArray::to_json_string(bool trailing_comma, upL_t depth) const{
    if (m_value.empty()){
        return trailing_comma ? "[]," : "[]";
    }

    bool one_line;
    {
        bool all_integer = true;
        for (const auto& item : m_value){
            if (!item.is_integer()){
                all_integer = false;
            }
        }
        bool one_string = m_value.size() == 1 && m_value[0].is_string();
        one_line = all_integer || one_string;
    }

    std::string ret;
    if (one_line){
        ret += "[" + m_value[0].to_json_string(false);
        for (upL_t c = 1; c < m_value.size(); c++){
            ret += ", ";
            ret += m_value[c].to_json_string(false, depth + 1);
        }
        ret += trailing_comma ? "]," : "]";
    }else{
        ret += "[\r\n";
        upL_t size = m_value.size();
        for (upL_t c = 0; c < size; c++){
            ret += indent(depth + 1) + m_value[c].to_json_string(c < size - 1, depth + 1);
            if (ret.back() != '\n'){
                ret += "\r\n";
            }
        }
        ret += indent(depth);
        ret += trailing_comma ? "],\r\n" : "]\r\n";
    }
    return ret;
}
std::vector<siL_t> ConfigArray::get_integers(std::string label) const{
    std::vector<siL_t> ret;
    for (const auto& item : m_value){
        ret.emplace_back(item.to_integer_throw(std::move(label)));
    }
    return ret;
}
upL_t ConfigArray::depth() const{
    upL_t ret = 0;
    for (const auto& item : *this){
        ret = std::max(ret, item.depth());
    }
    return ret + 1;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
