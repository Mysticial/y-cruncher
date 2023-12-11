/* InsertOrderMap.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 11/05/2023
 *  Last Modified   : 11/05/2023
 * 
 *  Practically the same as std::map, but iterates in the order of insertion.
 * 
 */

#pragma once
#ifndef ymp_Containers_InsertOrderMap_H
#define ymp_Containers_InsertOrderMap_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <list>
#include <map>
#include <iostream>
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value>
class InsertOrderMap{
    using ListType = std::list<std::pair<const Key&, Value&>>;
public:
    using iterator = typename ListType::iterator;
    using const_iterator = typename ListType::const_iterator;


public:
    void clear(){
        m_list.clear();
        m_map.clear();
    }

    bool empty() const{
        return m_list.empty();
    }
    size_t size() const{
        return m_list.size();
    }

    template <class... Args>
    bool contains(Args&&... args) const{
        return m_map.find(std::forward<Args>(args)...);
    }

    template <class... Args>
    const_iterator find(Args&&... args) const{
        auto iter = m_map.find(std::forward<Args>(args)...);
        if (iter == m_map.end()){
            return m_list.end();
        }
        return iter->second.iter;
    }
    template <class... Args>
    iterator find(Args&&... args){
        auto iter = m_map.find(std::forward<Args>(args)...);
        if (iter == m_map.end()){
            return m_list.end();
        }
        return iter->second.iter;
    }

    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args){
        auto ret = m_map.emplace(std::forward<Args>(args)...);
        if (ret.second){
            try{
                m_list.emplace_back(ret.first->first, ret.first->second.value);
                ret.first->second.iter = --m_list.end();
            }catch (...){
                m_map.erase(ret.first);
                throw;
            }
        }
        return std::pair<iterator, bool>(ret.first->second.iter, ret.second);
    }

    template <class... Args>
    size_t erase(Args&&... args){
        auto iter = m_map.find(std::forward<Args>(args)...);
        if (iter == m_map.end()){
            return 0;
        }
        m_list.erase(iter->second.iter);
        m_map.erase(iter);
        return 1;
    }
    const_iterator erase(const_iterator iter){
        m_map.erase(iter->first);
        const_iterator ret = iter;
        m_list.erase(iter);
        return ++ret;
    }
    iterator erase(iterator iter){
        m_map.erase(iter->first);
        iterator ret = iter;
        m_list.erase(iter);
        return ++ret;
    }


public:
    //  Debugging
    void print() const{
        std::cout << "Insertion Order:" << std::endl;
        for (const auto& item : m_list){
            std::cout << "    " << item.first << " : " << item.second << std::endl;
        }
        std::cout << "Sorted Order:" << std::endl;
        for (const auto& item : m_map){
            std::cout << "    " << item.first << " : " << item.second.value << " => " << item.second.iter->first << std::endl;
        }
    }


public:
    const_iterator  cbegin() const{ return m_list.cbegin(); }
    const_iterator  begin () const{ return m_list.cbegin(); }
    iterator        begin ()      { return m_list.begin(); }
    const_iterator  cend  () const{ return m_list.cend(); }
    const_iterator  end   () const{ return m_list.cend(); }
    iterator        end   ()      { return m_list.end(); }


private:
    struct Entry{
        Value value;
        iterator iter;

        template <class... Args>
        Entry(Args&&... args)
            : value(std::forward<Args>(args)...)
        {}

        operator const Value&() const{ return value; }
        operator       Value&()      { return value; }
    };

    ListType m_list;
    std::map<Key, Entry> m_map;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
