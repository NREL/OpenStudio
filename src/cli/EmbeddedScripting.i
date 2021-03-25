%module EmbeddedScripting

#define UTILITIES_API

%include <std_string.i>

// DLM: including these files causes a crash, I don't know why
//%include <utilities/core/CommonInclude.i>
//%import <utilities/core/CommonImport.i>
//%import <utilities/Utilities.i>

%ignore embedded_files::fileNames;

%include <embedded_files.hxx>
%include "EmbeddedHelp.hpp"


%{
#include <embedded_files.hxx>
#include "EmbeddedHelp.hpp"

%}


