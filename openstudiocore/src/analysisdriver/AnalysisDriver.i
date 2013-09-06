#ifndef ANALYSISDRIVER_ANALYSISDRIVER_I
#define ANALYSISDRIVER_ANALYSISDRIVER_I

#ifdef SWIGPYTHON
%module openstudioanalysisdriver
#endif


#define UTILITIES_API
#define ANALYSISDRIVER_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>
%import <ruleset/Ruleset.i>
%import <runmanager/lib/RunManager.i>
%import <project/Project.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::runmanager::detail;
%ignore openstudio::ruleset::detail;
%ignore openstudio::project::detail;
%ignore openstudio::analysisdriver::detail;

%{

  #include <analysisdriver/AnalysisDriver.hpp>
  #include <analysisdriver/CurrentAnalysis.hpp>
  #include <analysisdriver/AnalysisRunOptions.hpp>
  #include <analysisdriver/AnalysisDriverWatcher.hpp>
  #include <analysisdriver/SimpleProject.hpp>

  #include <project/ProjectDatabase.hpp>
  #include <project/AnalysisRecord.hpp>

  #include <runmanager/lib/JobErrors.hpp>
  #include <runmanager/lib/SimulationEngine.hpp>
  #include <runmanager/lib/ErrorEstimation.hpp>

  #include <analysis/ConcreteAnalysisObjects.hpp>

  #include <utilities/cloud/AWSProvider.hpp>
  #include <utilities/cloud/VagrantProvider.hpp>
  #include <utilities/core/FileReference.hpp>
  #include <utilities/idf/URLSearchPath.hpp>

  // to be ignored
  class QAction;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QNetworkAccessManager;
  class QWidget;
  class QDialog;
  class QComboBox;
  class QMainWindow;
  namespace openstudio{
    //class Attribute;
    //class EndUses;
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
    //class URLSearchPath;

    namespace runmanager{
      class JobStatusWidget;
      namespace detail{
        class JobItem;
      }
    }
  }

%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QWidget;
%ignore QDialog;
%ignore QComboBox;
%ignore QMainWindow;
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
%ignore openstudio::WorkspaceObjectWatcher;
%ignore openstudio::WorkspaceWatcher;
%ignore openstudio::runmanager::JobStatusWidget;
%ignore openstudio::runmanager::detail::JobItem;

%define ANALYSISDRIVER_WRAP(_name)

  // include the file
  %include <analysisdriver/##_name.hpp>

%enddef

%ignore std::vector<openstudio::analysisdriver::CurrentAnalysis>::vector(size_type);
%ignore std::vector<openstudio::analysisdriver::CurrentAnalysis>::resize(size_type);
%template(CurrentAnalysisVector) std::vector<openstudio::analysisdriver::CurrentAnalysis>;

%ignore std::vector<openstudio::analysisdriver::SimpleProject>::vector(size_type);
%ignore std::vector<openstudio::analysisdriver::SimpleProject>::resize(size_type);
%template(SimpleProjectVector) std::vector<openstudio::analysisdriver::SimpleProject>;
%template(OptionalSimpleProject) boost::optional<openstudio::analysisdriver::SimpleProject>;

ANALYSISDRIVER_WRAP(AnalysisRunOptions)
ANALYSISDRIVER_WRAP(CurrentAnalysis)
ANALYSISDRIVER_WRAP(AnalysisDriver)
%feature("director") AnalysisDriverWatcher;

#ifdef SWIGRUBY
// propogate errors in ruby class back to C++
%feature("director:except") {
 throw std::runtime_error(StringValuePtr($error));
}
#endif

ANALYSISDRIVER_WRAP(AnalysisDriverWatcher)
ANALYSISDRIVER_WRAP(SimpleProject)

#endif // ANALYSISDRIVER_ANALYSISDRIVER_I
