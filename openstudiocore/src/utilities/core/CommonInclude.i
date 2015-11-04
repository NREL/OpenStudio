#ifndef UTILITIES_COMMONINCLUDE_I
#define UTILITIES_COMMONINCLUDE_I

%include <stl.i>

#ifdef SWIGRUBY
%begin %{
// this must be included early because ruby.h 2.0.0  breaks Qt
#include <QByteArray>
#include <QString>
%}

%header %{
// Must undo more of the damage caused by ruby/win32.h 2.0.0 
#ifdef access
#undef access
#endif

#ifdef truncate
#undef truncate
#endif
%}

#endif

%include <utilities/core/Macro.hpp>

#define BOOST_SERIALIZATION_ASSUME_ABSTRACT(T) 

#define BOOST_CLASS_EXPORT_GUID(T,K) 

#define BOOST_CLASS_EXPORT(T) 

// Disable some standard warnings here, that we do not care about
// Namely: nothing known about base class, operator ignored, "wrong" class and "wrong" constant names
// and nested classes and "nothing known about <type>"
// also ignoring #509, shadowed method. Its a red herring for all of our cases 
//#pragma SWIG nowarn=401,362,365,366,368,378,503,801,312
#pragma SWIG nowarn=362,365,366,368,378,503,801,312,509,401

// Todo: Remove this one after other swig warnings have been cleared up
#pragma SWIG nowarn=315


#ifdef SWIGRUBY
%include <utilities/core/ruby/LanguageSpecific.i>
#endif

#ifdef SWIGPYTHON
%include <utilities/core/python/LanguageSpecific.i>
#endif

#ifdef SWIGCSHARP
%include <utilities/core/csharp/LanguageSpecific.i>
#endif

#ifdef SWIGJAVA
%include <utilities/core/java/LanguageSpecific.i>
#endif


#ifdef SWIGJAVASCRIPT
%include <utilities/core/v8/LanguageSpecific.i>
#endif



%include <utilities/core/Enum.i>

#endif // UTILITIES_COMMONINCLUDE_I
