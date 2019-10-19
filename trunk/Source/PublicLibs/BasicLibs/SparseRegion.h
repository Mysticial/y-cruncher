/* SparseRegion.h
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 02/09/2018
 * Last Modified    : 02/09/2018
 * 
 */

#pragma once
#ifndef ymp_SparseRegion_H
#define ymp_SparseRegion_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <map>
#include "PublicLibs/ConsoleIO/BasicIO.h"
#include "PublicLibs/Exceptions/InvalidParametersException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type>
struct Region{
    const Type s;
    const Type e;

    Region(Type is, Type ie);
    bool operator==(const Region& x) const;
};
////////////////////////////////////////////////////////////////////////////////
template <typename Type>
class SparseRegion{
public:
    SparseRegion() = default;
    SparseRegion(const Region<Type>& region);

    bool operator==(const Region<Type>& region) const;

    void operator|=(const Region<Type>& region);
//    void operator|=(const SparseRegion& region);

    void print() const;

private:
    std::map<Type, Type> m_ranges;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Implementations
template <typename Type>
Region<Type>::Region(Type is, Type ie)
    : s(is), e(ie)
{
    if (e <= s){
        throw InvalidParametersException("Region::Region()", "Invalid Range - Failed: s < e");
    }
}
template <typename Type>
bool Region<Type>::operator==(const Region& x) const{
    return s == x.s && e == x.e;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename Type>
SparseRegion<Type>::SparseRegion(const Region<Type>& region){
    m_ranges[region.s] = region.e;
}
template <typename Type>
bool SparseRegion<Type>::operator==(const Region<Type>& region) const{
    if (m_ranges.size() != 1){
        return false;
    }
    auto iter = m_ranges.begin();
    return Region<Type>(iter->first, iter->second) == region;
}
template <typename Type>
void SparseRegion<Type>::operator|=(const Region<Type>& region){
    using Iterator = typename std::map<Type, Type>::iterator;

    Type s = region.s;
    Type e = region.e;

    auto ret = m_ranges.emplace(s, e);
    Iterator node = ret.first;

    //  Node already exists. Merge with it.
    if (!ret.second){
        e = std::max(e, node->second);
        node->second = e;
    }

    //  If previous node exists, attempt to merge with it.
    if (node != m_ranges.begin()){
        Iterator prev = node;
        --prev;
        if (prev->second >= s){
            //  Merge with previous node.
            s = prev->first;
            e = std::max(e, prev->second);
            prev->second = e;
            m_ranges.erase(node);
            node = prev;
        }
    }

    //  Attempt to merge with later nodes.
    while (true){
        Iterator next = node;
        ++next;

        //  No next node.
        if (next == m_ranges.end()){
            return;
        }

        //  Next node does not touch current node.
        if (e < next->first){
            return;
        }

        //  Merge with next node.
        e = std::max(e, next->second);
        node->second = e;
        m_ranges.erase(next);
    }
}
//template <typename Type>
//void SparseRegion<Type>::operator|=(const SparseRegion& region){
//    for (const auto& item : region.m_ranges){
//        set(item.first, item.second);
//    }
//}
template <typename Type>
void SparseRegion<Type>::print() const{
    Console::println("Sparse Region:");
    for (const auto& item : m_ranges){
        Console::println("    [" + std::to_string(item.first) + ", " + std::to_string(item.second) + ")");
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
