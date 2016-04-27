%module EmbeddedScripting

%include <std_string.i>
%include <embedded_files.hxx>
%include "EmbeddedHelp.hpp"

std::string helloworld();


%{
#include <embedded_files.hxx>
#include "EmbeddedHelp.hpp"

std::string helloworld() {
  return "Hello World";
}

%}


