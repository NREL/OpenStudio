#ifndef BIMSERVER_I
#define BIMSERVER_I

#ifdef SWIGPYTHON
%module openstudiobimserver
#endif


#define UTILITIES_API
#define BIMSERVER_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <bimserver/ProjectImportation.hpp>
  using namespace openstudio::bimserver;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  
%}

// #ifdef SWIGCSHARP
%rename(BIMserverProjectImportation) openstudio::bimserver::ProjectImportation;
// #endif

%include bimserver/ProjectImportation.hpp>

#endif //BIMSERVER_I 
