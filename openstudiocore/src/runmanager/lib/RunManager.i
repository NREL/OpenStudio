#ifndef RUNMANAGER_RUNMANAGER_I
#define RUNMANAGER_RUNMANAGER_I

#ifdef SWIGPYTHON
%module openstudiorunmanager
#endif


#define UTILITIES_API
#define RUNMANAGER_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <ruleset/Ruleset.i>
%import <utilities/core/Qt.i>

%{
  #include <runmanager/lib/ToolInfo.hpp>
  #include <runmanager/lib/JobErrors.hpp>
  #include <runmanager/lib/JobParam.hpp>
  #include <runmanager/lib/JobType.hpp>
  #include <runmanager/lib/Job.hpp>
  #include <runmanager/lib/JobFactory.hpp>
  #include <runmanager/lib/RunManager.hpp>
  #include <runmanager/lib/FileInfo.hpp>
  #include <runmanager/lib/ConfigOptions.hpp>
  #include <runmanager/lib/ProcessCreator.hpp>
  #include <runmanager/lib/AdvancedStatus.hpp>
  #include <runmanager/lib/Workflow.hpp>
  #include <runmanager/lib/TreeStatus.hpp>
  #include <runmanager/lib/JobStatusWidget.hpp>
  #include <runmanager/lib/RubyJobUtils.hpp>
  #include <runmanager/lib/WorkItem.hpp>
  
  #include <ruleset/OSArgument.hpp>
  #include <utilities/core/Path.hpp>
  #include <utilities/core/Containers.hpp>
  #include <utilities/core/Exception.hpp>
  #include <utilities/plot/Plot2D.hpp>
  #include <utilities/plot/LinePlot.hpp>
  #include <utilities/plot/FloodPlot.hpp>

  #include <model/Component.hpp>
  
  // to be ignored
  class QAction;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QNetworkAccessManager;
  class QComboBox;
  class QMainWindow;   
  namespace openstudio{
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

%ignore openstudio::runmanager::detail;
%ignore openstudio::runmanager::RubyJobBuilder::inputFiles;
%ignore openstudio::runmanager::RunManager::statistics;
%ignore openstudio::runmanager::RunManager::getQItemModel;


// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QComboBox;
%ignore QMainWindow;   
%ignore openstudio::ProgressBar;
%ignore openstudio::UpdateManager;
%ignore openstudio::PathWatcher;
%ignore openstudio::IdfObjectWatcher;
%ignore openstudio::BCL;
%ignore openstudio::RemoteBCL;
%ignore openstudio::LocalBCL;
%ignore openstudio::WorkspaceWatcher;


// no default constructors
%ignore std::vector<openstudio::runmanager::Job>::vector(size_type); 
%ignore std::vector<openstudio::runmanager::Job>::resize(size_type); 

%ignore std::vector<openstudio::runmanager::FileInfo>::vector(size_type); 
%ignore std::vector<openstudio::runmanager::FileInfo>::resize(size_type); 

%ignore std::vector<openstudio::runmanager::JobParam>::vector(size_type); 
%ignore std::vector<openstudio::runmanager::JobParam>::resize(size_type); 

%ignore std::vector<openstudio::runmanager::Workflow>::vector(size_type); 
%ignore std::vector<openstudio::runmanager::Workflow>::resize(size_type); 

%ignore openstudio::runmanager::Job::start;

%ignore openstudio::runmanager::JobStatusWidget::publicActions;
%ignore openstudio::runmanager::Job::history;

%template(EnergyPlusVersionPathPair) std::pair<openstudio::runmanager::ToolVersion, openstudio::path>;
%template(EnergyPlusVersionPathVector) std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::path> >;
%template(OptionalJob) boost::optional<openstudio::runmanager::Job>;
%template(OptionalJobErrors) boost::optional<openstudio::runmanager::JobErrors>;

%template(ErrorTypeStringPair) std::pair<openstudio::runmanager::ErrorType, std::string>;

// create an instantiation of the vector classes
%template(JobVector) std::vector<openstudio::runmanager::Job>;
%template(JobTypeVector) std::vector<openstudio::runmanager::JobType>;
%template(FileInfoVector) std::vector<openstudio::runmanager::FileInfo>;
%template(ToolInfoVector) std::vector<openstudio::runmanager::ToolInfo>;
%template(JobParamList) std::vector<openstudio::runmanager::JobParam>;
%template(WorkflowVector) std::vector<openstudio::runmanager::Workflow>;
%template(WorkItemVector) std::vector<openstudio::runmanager::WorkItem>;
%template(RubyJobBuilderVector) std::vector<openstudio::runmanager::RubyJobBuilder>;
%template(ToolInfoPair) std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo>;
%template(ToolInfoPairVector) std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >;
%template(ErrorTypeStringPairVector) std::vector<std::pair<openstudio::runmanager::ErrorType, std::string> >;


%include <runmanager/lib/Workflow.hpp>
%include <runmanager/lib/WorkItem.hpp>
%include <runmanager/lib/JobType.hpp>
%include <runmanager/lib/JobParam.hpp>
%include <runmanager/lib/JobErrors.hpp>
%include <runmanager/lib/ToolInfo.hpp>
%include <runmanager/lib/FileInfo.hpp>
%include <runmanager/lib/AdvancedStatus.hpp>
%include <runmanager/lib/Job.hpp>
%include <runmanager/lib/JobFactory.hpp>
%include <runmanager/lib/RunManager.hpp>
%include <runmanager/lib/RubyJobUtils.hpp>
%include <runmanager/lib/ConfigOptions.hpp>
%include <runmanager/lib/TreeStatus.hpp>
%include <runmanager/lib/JobStatusWidget.hpp>

#endif // RUNMANAGER_RUNMANAGER_I

