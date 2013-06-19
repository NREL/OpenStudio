/*******************************************************************/
/* Without conditional compilation of the function declarations    */
/* in the DLL, there doesn't appear to be any way to get just      */
/* straight C/C++ code along with project settings to run properly */
/* with both the Microsoft and Borland compilers. The compilers    */
/* mangle the names differently when using the same calling        */
/* convention, so it is necessary to use the WINAPI (__stdcall)    */
/* convention when using Borland C++ Builder and the default       */
/* convention (__cdecl) when using Microsoft Visual C++.           */
/*                                                                 */
/* http://www.folding-hyperspace.com/program_tip_4.htm             */
/* http://msdn2.microsoft.com/en-us/library/ms682589.aspx          */
/* http://edmulroy.portbridge.com/howto10.htm                      */
/*******************************************************************/

#ifndef CLIPSWin32defs_H
#define CLIPSWin32defs_H
 
#ifdef __BORLANDC__
#define CALL_CONV WINAPI
#endif

#ifdef _MSC_VER
#define CALL_CONV
#endif

#endif