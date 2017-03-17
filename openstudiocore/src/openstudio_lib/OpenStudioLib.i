#ifndef OPENSTUDIOLIB_I
#define OPENSTUDIOLIB_I

#ifdef SWIGPYTHON
%module openstudiolib
#endif


#define UTILITIES_API
#define MODEL_API
#define MODELEDITOR_API
#define OPENSTUDIO_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>
%import <osversion/OSVersion.i>

%import(module="openstudiomodel") <model/Model.hpp>
%import(module="openstudiomodel") <model/ModelObject.hpp>
%import(module="openstudiomodel") <model/Component.hpp>

%{
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  #include <model/Component.hpp>
  #include <openstudio_lib/FileOperations.hpp>
  
  using namespace openstudio;
  
  // to be ignored
  class QAction;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QNetworkAccessManager;
  class QThread;  
  class QComboBox;  
  class QMainWindow;  
  class QDialog;  
  namespace openstudio{
    class ProgressBar;
    class UpdateManager;
    class PathWatcher;
    class IdfObjectWatcher;
    class RemoteBCL;
    class WorkspaceObjectWatcher;
    class WorkspaceWatcher;
  }
%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QComboBox;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QThread;
%ignore QMainWindow;
%ignore QDialog;
%ignore openstudio::ProgressBar;
%ignore openstudio::UpdateManager;
%ignore openstudio::PathWatcher;
%ignore openstudio::IdfObjectWatcher;
%ignore openstudio::RemoteBCL;
%ignore openstudio::WorkspaceWatcher;

// ignore base class of OSDocument
%ignore OSQObjectController;

%include <openstudio_lib/FileOperations.hpp>

#endif //OPENSTUDIOLIB_I 
