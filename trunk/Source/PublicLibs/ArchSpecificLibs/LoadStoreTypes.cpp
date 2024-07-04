/* LoadStoreTypes.cpp
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 05/17/2024
 *  Last Modified   : 05/17/2024
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/Exceptions/InvalidParametersException.h"
#include "LoadStoreTypes.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char* to_string(LoadType type){
    switch (type){
    case LoadType::NORMAL:
        return "normal";
#ifdef ymp_LoadType_has_NON_TEMPORAL
    case LoadType::NON_TEMPORAL:
        return "non-temporal";
#endif
#ifdef ymp_LoadType_has_LOAD_AND_EVICT
    case LoadType::LOAD_AND_EVICT:
        return "load-and-evict";
#endif
    default:
        throw InvalidParametersException("Invalid LoadType enum: " + std::to_string((int)type));
    }
}
LoadType parse_LoadType(const std::string& str){
    if (str == "normal"){
        return LoadType::NORMAL;
    }
#ifdef ymp_LoadType_has_NON_TEMPORAL
    if (str == "non-temporal"){
        return LoadType::NON_TEMPORAL;
    }
#endif
#ifdef ymp_LoadType_has_LOAD_AND_EVICT
    if (str == "load-and-evict"){
        return LoadType::LOAD_AND_EVICT;
    }
#endif
    throw InvalidParametersException("Invalid LoadType string: " + str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char* to_string(StoreType type){
    switch (type){
    case StoreType::NORMAL:
        return "normal";
#ifdef ymp_StoreType_has_NON_TEMPORAL
    case StoreType::NON_TEMPORAL:
        return "non-temporal";
#endif
#ifdef ymp_StoreType_has_EVICT_TO_SHARED
    case StoreType::EVICT_TO_SHARED:
        return "evict-to-shared";
#endif
    default:
        throw InvalidParametersException("Invalid StoreType enum: " + std::to_string((int)type));
    }
}
StoreType parse_StoreType(const std::string& str){
    if (str == "normal"){
        return StoreType::NORMAL;
    }
#ifdef ymp_StoreType_has_NON_TEMPORAL
    if (str == "non-temporal"){
        return StoreType::NON_TEMPORAL;
    }
#endif
#ifdef ymp_StoreType_has_EVICT_TO_SHARED
    if (str == "evict-to-shared"){
        return StoreType::EVICT_TO_SHARED;
    }
#endif
    throw InvalidParametersException("Invalid StoreType string: " + str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char* to_string(LoadPrefetch type){
    switch (type){
    case LoadPrefetch::NONE:
        return "none";
#ifdef YMP_Arch_2001_x86_SSE2
    case LoadPrefetch::PREFETCH_NTA:
        return "prefetch-nta";
    case LoadPrefetch::PREFETCH_T0:
        return "prefetch-t0";
    case LoadPrefetch::PREFETCH_T1:
        return "prefetch-t1";
    case LoadPrefetch::PREFETCH_T2:
        return "prefetch-t2";
#endif
    default:
        throw InvalidParametersException("Invalid LoadPrefetch enum: " + std::to_string((int)type));
    }
}
LoadPrefetch parse_LoadPrefetch(const std::string& str){
    if (str == "normal"){
        return LoadPrefetch::NONE;
    }
#ifdef YMP_Arch_2001_x86_SSE2
    if (str == "prefetch-nta"){
        return LoadPrefetch::PREFETCH_NTA;
    }
    if (str == "prefetch-t0"){
        return LoadPrefetch::PREFETCH_T0;
    }
    if (str == "prefetch-t1"){
        return LoadPrefetch::PREFETCH_T1;
    }
    if (str == "prefetch-t2"){
        return LoadPrefetch::PREFETCH_T2;
    }
#endif
    throw InvalidParametersException("Invalid LoadPrefetch string: " + str);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
