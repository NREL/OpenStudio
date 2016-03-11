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
%import <runmanager/lib/RunManager.i>
%import <osversion/OSVersion.i>
%import <analysisdriver/AnalysisDriver.i>

%import(module="openstudiomodel") <model/Model.hpp>
%import(module="openstudiomodel") <model/ModelObject.hpp>
%import(module="openstudiomodel") <model/Component.hpp>

%{
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  #include <model/Component.hpp>
  #include <analysis/Measure.hpp>
  #include <analysis/NullMeasure.hpp>
  #include <analysis/RubyMeasure.hpp>
  #include <runmanager/lib/RunManager.hpp>
  #include <runmanager/lib/WorkItem.hpp>
  #include <openstudio_lib/FileOperations.hpp>
  #include <openstudio_lib/OSDocument.hpp>
  
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
    
    namespace runmanager{
      class JobStatusWidget;
    }
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
%ignore openstudio::runmanager::JobStatusWidget;

// ignore base class of OSDocument
%ignore OSQObjectController;

%include <openstudio_lib/FileOperations.hpp>
%include <openstudio_lib/OSDocument.hpp>

#endif //OPENSTUDIOLIB_I 
