/* ToString.cpp - String Tools
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 07/07/2013
 * Last Modified    : 08/24/2014
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <iomanip>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/ConsoleIO/Label.h"
#include "ToString.h"
namespace ymp{
namespace StringTools{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Helpers
YM_NO_INLINE std::string tostr_u_commas(uiL_t x){
    //  Prints out x with comma separators.

    std::string str = std::to_string(x);
    std::string out;

    const char* ptr = str.c_str();
    upL_t len = str.size();

    upL_t commas = (len + 2) / 3 - 1;
    upL_t shift = len - commas * 3;

    while (1){
        char ch = *ptr++;
        if (ch == '\0')
            break;
        if (shift == 0){
            out += ',';
            shift = 3;
        }
        out += ch;
        shift--;
    }

    return out;
}
YM_NO_INLINE std::string tostr_s_commas(siL_t x){
    if (x < 0)
        return std::string("-") + tostr_u_commas(-x);
    else
        return tostr_u_commas(x);
}
YM_NO_INLINE std::string tostr_u_bytes(uiL_t bytes){
    //  Prints out bytes in one of the following forms:
    //  0.xx suffix
    //  x.xx suffix
    //  xx.x suffix
    //   xxx suffix

    const char* BYTE_NAMES[] = {
        " bytes",
        " KiB",
        " MiB",
        " GiB",
        " TiB",
        " PiB",
        " EiB",
    };

    std::string out;
    if (bytes < 1000){
        if (bytes < 10){
            out += " ";
        }
        out += std::to_string(bytes);
        out += BYTE_NAMES[0];
        return out;
    }

    int suffix_index = 1;
    while (bytes >= 1024000){
        bytes >>= 10;
        suffix_index++;
    }

    bytes *= 1000;
    bytes >>= 10;

    //  .xxx or (1.00)
    if (bytes < 1000){
        bytes += 5;
        bytes /= 10;

        if (bytes == 100){
            out += "1.00";
            out += BYTE_NAMES[suffix_index];
            return out;
        }

        out += "0.";
        out += std::to_string(bytes);
        out += BYTE_NAMES[suffix_index];
        return out;
    }

    //  x.xx or (10.0)
    if (bytes < 10000){
        bytes += 5;
        bytes /= 10;

        if (bytes == 1000){
            out += "10.0";
            out += BYTE_NAMES[suffix_index];
            return out;
        }

        out += std::to_string(bytes / 100);
        bytes %= 100;
        out += ".";
        if (bytes >= 10){
            out += std::to_string(bytes);
        }else{
            out += "0";
            out += std::to_string(bytes);
        }
        out += BYTE_NAMES[suffix_index];
        return out;
    }

    //  xx.x or (0.98)
    if (bytes < 100000){
        bytes += 50;
        bytes /= 100;

        if (bytes == 1000){
            out += " 100";
            out += BYTE_NAMES[suffix_index];
            return out;
        }

        out += std::to_string(bytes / 10);
        bytes %= 10;
        out += ".";
        out += std::to_string(bytes);
        out += BYTE_NAMES[suffix_index];
        return out;
    }

    //  xxx or (1.00)
    {
        bytes += 500;
        bytes /= 1000;

        if (bytes == 1000){
            out += "0.98";
            out += BYTE_NAMES[suffix_index + 1];
            return out;
        }

        out += " ";
        out += std::to_string(bytes);
        out += BYTE_NAMES[suffix_index];
        return out;
    }
}
YM_NO_INLINE std::string tostr_s_bytes(siL_t bytes){
    if (bytes < 0){
        return std::string("-") + tostr_u_bytes(-bytes);
    }else{
        return tostr_u_bytes(bytes);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
YM_NO_INLINE std::string tostr(uiL_t x, NumberFormat format){
    switch (format){
    case COMMAS:
        return tostr_u_commas(x);
    case BYTES:
        return tostr_u_bytes(x);
    case BYTES_EXPANDED:{
        auto out = tostr_u_commas(x);
        out += " (";
        out += tostr_u_bytes(x);
        out += ")";
        return out;
    }
    default:
        return std::to_string(x);
    }
}
YM_NO_INLINE std::string tostr(siL_t x, NumberFormat format){
    switch (format){
    case COMMAS:
        return tostr_s_commas(x);
    case BYTES:
        return tostr_s_bytes(x);
    case BYTES_EXPANDED:{
        auto out = tostr_s_commas(x);
        out += " (";
        out += tostr_s_bytes(x);
        out += ")";
        return out;
    }
    default:
        return std::to_string(x);
    }
}
YM_NO_INLINE std::string tostrln(uiL_t x, NumberFormat format){
    return tostr(x, format) += "\r\n";
}
YM_NO_INLINE std::string tostrln(siL_t x, NumberFormat format){
    return tostr(x, format) += "\r\n";
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Float
YM_NO_INLINE std::string tostr_float(double x, int precision){
    std::ostringstream out;
    out << std::setprecision(precision);
    out << x;
    return out.str();
}
YM_NO_INLINE std::string tostrln_float(double x, int precision){
    return tostr_float(x, precision) += "\r\n";
}
YM_NO_INLINE std::string tostr_fixed(double x, int precision){
    std::ostringstream out;
    out << std::setprecision(precision);
    out << std::fixed;
    out << x;
    return out.str();
}
YM_NO_INLINE std::string tostrln_fixed(double x, int precision){
    return tostr_fixed(x, precision) += "\r\n";
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
