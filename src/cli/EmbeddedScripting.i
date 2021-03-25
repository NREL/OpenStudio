#ifndef EMBEDDED_SCRIPTING_I
#define EMBEDDED_SCRIPTING_I

%module EmbeddedScripting

%include <std_string.i>

// DLM: including these files causes a crash, I don't know why
//%include <utilities/core/CommonInclude.i>
//%import <utilities/core/CommonImport.i>
//%import <utilities/Utilities.i>

%{
  #include <embedded_files.hxx>
  #include "EmbeddedHelp.hpp"
%}

%ignore embedded_files::fileNames;

%include <embedded_files.hxx>
%include "EmbeddedHelp.hpp"

#endif // EMBEDDED_SCRIPTING_I
