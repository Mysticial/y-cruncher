/* Compiler_VSC.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 12/30/2009
 *  Last Modified   : 08/09/2015
 * 
 *      This includes the Intel Compiler on Windows since it supports all of
 *  Visual Studio's features.
 * 
 */

#pragma once
#ifndef ymp_Compiler_VSC_H
#define ymp_Compiler_VSC_H
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Disable Bad/Buggy Compilers
#if (defined _MSC_VER) && (_MSC_VER < 1928)
#error "MSVC++ 16.9 or later is required."
//  https://developercommunity.visualstudio.com/content/problem/549433/mixing-integer-and-floating-point-sse-intrinsics-l.html
//  https://developercommunity.visualstudio.com/content/problem/767759/sse-intrinsics-get-miscompiled-leading-to-incorrec.html
//  https://developercommunity.visualstudio.com/content/problem/746347/incorrect-multiply-by-1-sse-optimization.html
#endif 
#if (defined __INTEL_COMPILER_BUILD_DATE) && (__INTEL_COMPILER_BUILD_DATE < 20201208)
#error "ICC 19.2 or later is required.""
//  https://software.intel.com/en-us/forums/intel-c-compiler/topic/815553
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define YM_NO_INLINE        __declspec(noinline)
#define YM_FORCE_INLINE     inline __forceinline
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Restrict
template <typename type> using r_ptr  = type *__restrict;
template <typename type> using c_ptr  = type const*__restrict;
template <typename type> using r_ref  = type &__restrict;
template <typename type> using c_ref  = type const&__restrict;
template <typename type> using r_rref = type &&__restrict;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dynamic Linking
#ifdef YMP_STANDALONE
#define YMP_EXPORT
#else
#ifdef _WINDLL
#define YMP_EXPORT __declspec(dllexport)
#else
#define YMP_EXPORT __declspec(dllimport)
#define YMP_IMPORT
#endif
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Warnings
#pragma warning(disable:4100)   //  Unreferenced Formal Parameter
#pragma warning(disable:4127)   //  Constant Expression
#pragma warning(disable:4324)   //  Structure Padding
//#pragma warning(disable:4373)   //  Restrict ptr/ref difference in override
#pragma warning(disable:4456)   //  Variable Shadowing
//#pragma warning(disable:4457)   //  Variable Hides Parameter
//#pragma warning(disable:4458)   //  Member Hiding
//#pragma warning(disable:4459)   //  Parameter Hides Global
#pragma warning(disable:4505)   //  Unused Static Function
#pragma warning(disable:4661)   //  No Definition for Template Instantiation
#pragma warning(disable:26495)  //  Struct member is uninitialized.

//  Analysis Warnings
//#pragma warning(disable:6011)   //  Dereference NULL pointer
#pragma warning(disable:6246)   //  Variable Shadowing
#pragma warning(disable:6285)   //  Always Non-Zero Constant
#pragma warning(disable:6294)   //  Loop body never executed
#pragma warning(disable:6326)   //  Constant Comparison

//  Disable specific ICC warnings
#ifdef __INTEL_COMPILER
#pragma warning disable 186     //  Unsigned comparison with zero
#pragma warning disable 280     //  Const switch expression
#pragma warning disable 411     //  No default constructor
#pragma warning disable 532     //  No Definition for Template Instantiation
//#pragma warning disable 1125    //  Virtual function hiding
//#pragma warning disable 3280    //  Member hiding
#pragma warning disable 2553    //  wmain()
#endif
#ifdef __INTEL_LLVM_COMPILER
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wimplicitly-unsigned-literal"
#pragma clang diagnostic ignored "-Wconstant-conversion"
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#pragma clang diagnostic ignored "-Wundefined-var-template"
#pragma clang diagnostic ignored "-Wmicrosoft-template"
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Compiler Bugs
#if (defined _MSC_VER) && (_MSC_VER < 1910)
#pragma warning(disable:4189)   //  Unused Local Variable (in static polymorphism)
#endif
#if defined __INTEL_COMPILER
#pragma warning disable 1879    //  unimplemented pragma ignored (in TBB header)
#pragma warning disable 3346    //  dynamic exception specifications are deprecated (in TBB header)
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif
