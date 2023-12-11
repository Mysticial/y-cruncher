/* ConfigObject.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 09/27/2016
 *  Last Modified   : 11/03/2023
 * 
 */

#pragma once
#ifndef ymp_ConfigurationFiles_ConfigObject_H
#define ymp_ConfigurationFiles_ConfigObject_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <set>
#include "PublicLibs/BasicLibs/Containers/InsertOrderMap.h"
#include "ConfigValue.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ConfigObject{
public:
    ConfigObject(ConfigObject&&) = default;
    ConfigObject& operator=(ConfigObject&&) = default;
private:
    //  Private to avoid accidental copying.
    ConfigObject(const ConfigObject& x);
    void operator=(const ConfigObject& x);
public:
    ConfigObject clone() const;

public:
    ConfigObject(const std::string& description = "")
        : m_description(description)
    {}

    void clear();

    //  Add node to object using specified key.
    void            add_bool    (const std::string& key, bool value,                    std::string comment = "");
    void            add_integer (const std::string& key, siL_t value,                   std::string comment = "");
    void            add_string  (const std::string& key, std::string value,             std::string comment = "");
    ConfigValue&    add_value   (const std::string& key, ConfigValue&& value,           std::string comment = "");
    ConfigArray&    add_array   (const std::string& key,                                std::string comment = "");
    ConfigObject&   add_object  (const std::string& key, std::string description = "",  std::string comment = "");

    //  Move all sub-nodes from the specified object into this one.
    void combine(ConfigObject&& obj);
    void operator+=(ConfigObject&& obj);

public:
    struct Entry{
        std::string comment;
        ConfigValue value;

        template <class... Args>
        Entry(std::string p_comment, Args&&... args)
            : comment(std::move(p_comment))
            , value(std::forward<Args>(args)...)
        {}
    };

    bool empty() const{ return m_map.empty(); }
    upL_t size() const{ return m_map.size(); }

    const ConfigValue& operator[](const std::string& key) const;
          ConfigValue& operator[](const std::string& key);

    const ConfigValue* try_get(const std::string& key) const;
          ConfigValue* try_get(const std::string& key);

    auto begin() const{ return m_map.begin(); }
    auto begin()      { return m_map.begin(); }
    auto end() const{ return m_map.end(); }
    auto end()      { return m_map.end(); }

public:
    //  Throws if key not found.
    bool                get_bool    (const std::string& key) const;
    siL_t               get_integer (const std::string& key) const;
    const std::string&  get_string  (const std::string& key) const;
    const ConfigArray&  get_array   (const std::string& key) const;
    const ConfigObject& get_object  (const std::string& key) const;

    //  Use default if key is missing.
    bool                get_bool    (const std::string& key, bool default_value) const;
    siL_t               get_integer (const std::string& key, siL_t default_value) const;
    const std::string&  get_string  (const std::string& key, const std::string& default_value) const;

    //  Verifies that all keys in this object are in the specified set.
    void verify_keys(const char* function, const std::set<std::string>& set) const;

    friend bool operator==(const ConfigObject& a, const ConfigObject& b);
    std::string to_acfg_string(upL_t depth = 0, bool comments = true) const;
    std::string to_json_string(bool trailing_comma, upL_t depth = 0) const;

private:
    void sanitize_key(const std::string& key) const;

private:
    std::string m_description;
    InsertOrderMap<std::string, Entry> m_map;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
