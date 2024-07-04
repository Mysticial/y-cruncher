/* ConfigArray.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/27/2016
 *  Last Modified   : 11/03/2023
 * 
 */

#pragma once
#ifndef ymp_ConfigurationFiles_ConfigArray_H
#define ymp_ConfigurationFiles_ConfigArray_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <vector>
#include "ConfigValue.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ConfigArray{
public:
    ConfigArray(ConfigArray&&) = default;
private:
    //  Private to avoid accidental copying.
    ConfigArray(const ConfigArray& x);

public:
    ConfigArray() = default;

    upL_t size() const{
        return m_value.size();
    }
    void assert_size(upL_t size) const;

public:
    void operator+=(siL_t value);
    void operator+=(ConfigValue&& value);

    const ConfigValue& operator[](upL_t index) const{
        return m_value[index];
    }
    ConfigValue& operator[](upL_t index){
        return m_value[index];
    }

    auto begin() const{
        return m_value.begin();
    }
    auto begin(){
        return m_value.begin();
    }
    auto end() const{
        return m_value.end();
    }
    auto end(){
        return m_value.end();
    }

public:
    friend bool operator==(const ConfigArray& a, const ConfigArray& b);
    std::string to_acfg_string(upL_t depth = 0, bool comments = true) const;
    std::string to_json_string(bool trailing_comma, upL_t depth = 0) const;

    std::vector<siL_t> get_integers(std::string label = "") const;

    upL_t depth() const;

private:
    bool print_as_one_line() const;

private:
    friend class ConfigValue;
    std::vector<ConfigValue> m_value;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
