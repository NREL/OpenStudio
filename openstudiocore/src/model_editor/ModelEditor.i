#ifndef MODELEDITOR_LIB_I
#define MODELEDITOR_LIB_I

#ifdef SWIGPYTHON
%module openstudiomodeleditor
#endif


#define UTILITIES_API
#define MODEL_API
#define MODELEDITOR_API

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
  #include <model_editor/InspectorGadget.hpp>
  #include <model_editor/InspectorDialog.hpp>
  #include <model_editor/ModalDialogs.hpp>
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  
  using namespace openstudio;
  using namespace openstudio::model;
  
  // to be ignored
  class QAction;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QNetworkAccessManager;
  class QThread;  
  namespace openstudio{
    class Plot2D;
    class LinePlot;
    class FloodPlot;
    class ProgressBar;
    class UpdateManager;
    class PathWatcher;
    class IdfObjectWatcher;
    class BCL;
	class RemoteBCL;
	class LocalBCL;
    class WorkspaceObjectWatcher;
    class WorkspaceWatcher;
  }
%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QThread;
%ignore openstudio::Plot2D;
%ignore openstudio::LinePlot;
%ignore openstudio::FloodPlot;
%ignore openstudio::ProgressBar;
%ignore openstudio::UpdateManager;
%ignore openstudio::PathWatcher;
%ignore openstudio::IdfObjectWatcher;
%ignore openstudio::BCL;
%ignore openstudio::RemoteBCL;
%ignore openstudio::LocalBCL;
%ignore openstudio::WorkspaceWatcher;

%include <model_editor/InspectorGadget.hpp>

%feature("director") InspectorDialog;
%include <model_editor/InspectorDialog.hpp>

%feature("director") ModelObjectSelectorDialogWatcher;
%include <model_editor/ModalDialogs.hpp>

#endif //MODELEDITOR_LIB_I 
