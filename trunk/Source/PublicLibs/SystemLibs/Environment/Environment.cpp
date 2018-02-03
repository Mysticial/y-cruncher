/* Environment.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 01/04/2015
 * Last Modified    : 01/01/2016
 * 
 */

#ifdef _WIN32
#if _WIN32_WINNT >= 0x0601
#include "Environment_WindowsCommon.ipp"
#include "Environment_Windows7.ipp"
#else
#include "Environment_WindowsCommon.ipp"
#include "Environment_WindowsVista.ipp"
#endif
#endif

#ifdef __linux
#include "Environment_Linux.ipp"
#endif
