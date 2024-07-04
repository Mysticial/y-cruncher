/* ConfigValue.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/27/2016
 *  Last Modified   : 11/03/2023
 * 
 *      This configuration class supports two formats: JSON and ACFG
 * 
 *  JSON is a standard, but this class doesn't support all of it. Missing
 *  features are:
 *      -   Floating-point numeric types. All numeric types must be integers.
 *      -   The majority of escape sequences. Only '\\' and '\"' are supported.
 *  
 *  ACFG (Annotated Config) is custom to y-cruncher. It is largely the same as
 *  JSON, but with these differences:
 * 
 *      -   There are no commas between array and object fields.
 *      -   For objects, the key value is not a string. It is an alphanumeric identifier.
 *      -   For objects, ordering is preserved by the parser.
 *      -   Comments are allowed and are deliminated by C-style "//".
 * 
 * 
 *  The purpose of the ACFG format is to be a little bit more user-friendly for
 *  manual editing than standard JSON.
 * 
 */

#pragma once
#ifndef ymp_ConfigurationFiles_ConfigValue_H
#define ymp_ConfigurationFiles_ConfigValue_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <string>
#include "PublicLibs/Types.h"
#include "ConfigTypes.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum class ConfigValueType{
    EMPTY,
    BOOLEAN,
    INTEGER,
    STRING,
    ARRAY,
    OBJECT,
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ConfigValue{
public:
    ~ConfigValue();
    ConfigValue(ConfigValue&& x) noexcept;
    void operator=(ConfigValue&& x) noexcept;
private:
    //  Private to avoid accidental copying.
    ConfigValue(const ConfigValue& x);
    void operator=(const ConfigValue& x);
public:
    ConfigValue clone() const;

public:
    ConfigValue() = default;
    ConfigValue(bool x);
    ConfigValue(siL_t x);
    ConfigValue(const char* x);
    ConfigValue(std::string x);
    ConfigValue(ConfigArray&& x);
    ConfigValue(ConfigObject&& x);

    void clear();

public:
    ConfigValueType type() const{ return m_type; }
    bool is_null        () const{ return m_type == ConfigValueType::EMPTY; }
    bool is_boolean     () const{
        //  Backwards compatibility.
        if (m_type == ConfigValueType::BOOLEAN){
            return true;
        }
        if (m_type != ConfigValueType::STRING){
            return false;
        }
        return *u.m_string == "true" || *u.m_string == "false";
    }
    bool is_integer     () const{ return m_type == ConfigValueType::INTEGER; }
    bool is_string      () const{ return m_type == ConfigValueType::STRING; }
    bool is_array       () const{ return m_type == ConfigValueType::ARRAY; }
    bool is_object      () const{ return m_type == ConfigValueType::OBJECT; }

    //  Get the value. Throws if the type doesn't match.
    bool                to_boolean_throw    (const std::string& label = "") const;
    int64_t             to_integer_throw    (const std::string& label = "") const;
    const std::string&  to_string_throw     (const std::string& label = "") const;
          std::string&  to_string_throw     (const std::string& label = "");
    const ConfigArray&  to_array_throw      (const std::string& label = "") const;
          ConfigArray&  to_array_throw      (const std::string& label = "");
    const ConfigObject& to_object_throw     (const std::string& label = "") const;
          ConfigObject& to_object_throw     (const std::string& label = "");

    //  Get a pointer to the data for this value.
    //  If the type matches, returns the pointer.
    //  If the type does match, returns nullptr.
    const std::string*  to_string_nothrow   () const;
          std::string*  to_string_nothrow   ();
    const ConfigArray*  to_array_nothrow    () const;
          ConfigArray*  to_array_nothrow    ();
    const ConfigObject* to_object_nothrow   () const;
          ConfigObject* to_object_nothrow   ();

public:
    friend bool operator==(const ConfigValue& a, const ConfigValue& b);
    friend bool operator!=(const ConfigValue& a, const ConfigValue& b);

    //  Convert object tree to Annotated Config string.
    std::string to_acfg_string(upL_t depth = 0, bool comments = true) const;

    //  Convert object tree to JSON string.
    std::string to_json_string(bool trailing_comma, upL_t depth = 0) const;

    upL_t depth() const;

private:
    ConfigValueType m_type = ConfigValueType::EMPTY;
    union{
        bool m_bool;
        siL_t m_integer;
        std::string* m_string;
        ConfigArray* m_array;
        ConfigObject* m_object;
    } u;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string indent(upL_t depth);
const char* type_to_string(ConfigValueType type);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
