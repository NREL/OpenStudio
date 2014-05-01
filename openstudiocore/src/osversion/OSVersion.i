#ifndef OSVERSION_I
#define OSVERSION_I

#ifdef SWIGPYTHON
%module openstudioosversion
#endif


#define UTILITIES_API
#define MODEL_API
#define OSVERSION_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
%import <model/Model.i>
#else
%import(module="openstudiomodel") <model/Model.hpp>
%import(module="openstudiomodel") <model/ModelObject.hpp>
#endif

%{
  #include <osversion/VersionTranslator.hpp>

  #include <model/Model.hpp>
  #include <model/Component.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::osversion;
%}

%include <osversion/VersionTranslator.hpp>

#endif // OSVERSION_I

