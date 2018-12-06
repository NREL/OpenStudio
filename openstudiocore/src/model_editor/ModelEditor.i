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
  #include <model_editor/Application.hpp>
  #include <model_editor/InspectorGadget.hpp>
  #include <model_editor/InspectorDialog.hpp>
  #include <model_editor/ModalDialogs.hpp>
  #include <model_editor/OSProgressBar.hpp>
  #include <model_editor/PathWatcher.hpp>
 
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
    class ProgressBar;
    class UpdateManager;
    class IdfObjectWatcher;
    class BCL;
    class RemoteBCL;
    class LocalBCL;
    class WorkspaceObjectWatcher;
    class WorkspaceWatcher;
  }
%}

%include <model_editor/Qt.i>

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QThread;
%ignore openstudio::UpdateManager;
%ignore openstudio::IdfObjectWatcher;
%ignore openstudio::BCL;
%ignore openstudio::RemoteBCL;
%ignore openstudio::LocalBCL;
%ignore openstudio::WorkspaceWatcher;

%template(Application) openstudio::Singleton<openstudio::ApplicationSingleton>;
%include <model_editor/Application.hpp>

%feature("director") PathWatcher;
%include <model_editor/PathWatcher.hpp>

%include <model_editor/InspectorGadget.hpp>

%feature("director") InspectorDialog;
%include <model_editor/InspectorDialog.hpp>

// do not know why SWIG is not pulling in these methods from QMainWindow base class
%extend InspectorDialog {        
    void setVisible(bool visible){$self->setVisible(visible); }
    void setHidden(bool hidden){$self->setHidden(hidden); }
    void show(){$self->show(); }
    void hide(){$self->hide(); }

    void showMinimized(){$self->showMinimized(); }
    void showMaximized(){$self->showMaximized(); }
    void showFullScreen(){$self->showFullScreen(); }
    void showNormal(){$self->showNormal(); }

    bool close(){return $self->close(); }
    void raise(){$self->raise(); }
    void lower(){$self->lower(); }
};

%feature("director") ModelObjectSelectorDialogWatcher;
%include <model_editor/ModalDialogs.hpp>

%feature("director") OSProgressBar;
%include <model_editor/OSProgressBar.hpp>

#endif //MODELEDITOR_LIB_I
