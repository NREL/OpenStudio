#ifndef PLUGIN_I
#define PLUGIN_I

#ifdef SWIGPYTHON
%module openstudioplugin
#endif


#define UTILITIES_API
#define MODEL_API
#define MODELEDITOR_API
#define OPENSTUDIO_API
#define PLUGIN_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <utilities/core/Qt.i>

%{
  #include <plugin/ExternalPlugin.hpp>
  #include <plugin/ExternalModelInterface.hpp>
  #include <plugin/ExternalObjectInterface.hpp>
  
  #include <utilities/core/Exception.hpp>
  
  using namespace openstudio;
  using namespace openstudio::plugin;
  
  // to be ignored
  class QAction;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QNetworkAccessManager; 
  class QComboBox;  
  class QMainWindow;  
  class QDialog;  
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
%ignore QComboBox;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QMainWindow;
%ignore QDialog;
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

%feature("director") ExternalPlugin;  
%include <plugin/ExternalPlugin.hpp>

%feature("director") ExternalModelInterface;  
%newobject ExternalModelInterface::create;
%include <plugin/ExternalModelInterface.hpp>

%feature("director") ExternalObjectInterface;  
%newobject ExternalObjectInterface::create;
%include <plugin/ExternalObjectInterface.hpp>

#endif //PLUGIN_I 
