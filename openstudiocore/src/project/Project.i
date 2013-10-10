#ifndef PROJECT_PROJECT_I
#define PROJECT_PROJECT_I

#ifdef SWIGPYTHON
%module openstudioproject
#endif


#define UTILITIES_API
#define PROJECT_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>
%import <ruleset/Ruleset.i>
%import <analysis/Analysis.i>
%import <runmanager/lib/RunManager.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::runmanager::detail;
%ignore openstudio::ruleset::detail;
%ignore openstudio::analysis::detail;
%ignore openstudio::project::detail;

%{
  #include <project/ProjectDatabase.hpp>
  #include <project/ProjectDatabase_Impl.hpp>
  #include <project/ProjectDatabaseRecord.hpp>
  #include <project/ProjectDatabaseRecord_Impl.hpp>
  #include <project/ConcreteObjectRecords.hpp>
    
  #include <analysis/ConcreteAnalysisObjects.hpp>
  
  #include <model/ModelObject.hpp>
  #include <model/Model.hpp>
  #include <model/Component.hpp>
  
  #include <runmanager/lib/Workflow.hpp>
  #include <runmanager/lib/SimulationEngine.hpp>
  #include <runmanager/lib/ErrorEstimation.hpp>
  
  #include <utilities/cloud/AWSProvider.hpp>
  #include <utilities/cloud/VagrantProvider.hpp>
  #include <utilities/data/Attribute.hpp>
  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>
  #include <utilities/units/GPDUnit.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/Misc1Unit.hpp>
  #include <utilities/units/MPHUnit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/WhUnit.hpp>

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
%ignore openstudio::WorkspaceWatcher;
%ignore openstudio::runmanager::JobStatusWidget;
%ignore openstudio::runmanager::detail::JobItem;

// no need to export join records
%ignore openstudio::project::JoinRecord;
%ignore openstudio::project::Record::joinRecords;

#ifdef SWIGCSHARP
  // hides static methods in derived classes
  %ignore openstudio::project::Record::databaseTableName;
#endif

// it seems that SWIG tries to create conversions of QObjects to these
%ignore openstudio::Attribute;
%ignore openstudio::EndUses;
%ignore openstudio::ProgressBar;
%ignore openstudio::UpdateManager;
%ignore openstudio::PathWatcher;
%ignore openstudio::IdfObjectWatcher;
%ignore openstudio::WorkspaceObjectWatcher;
%ignore openstudio::WorkspaceWatcher;
%ignore openstudio::runmanager::detail::JobItem;
%ignore openstudio::URLSearchPath;

// ignore these
%ignore openstudio::project::RemoveUndo::removedObjects;
%ignore std::pair<openstudio::UUID, openstudio::project::RemoveUndo>;
%ignore std::vector<std::pair<openstudio::UUID, openstudio::project::RemoveUndo> >;

%define PROJECT_WRAP(_name) 
  // ignore impl
  %ignore boost::shared_ptr<openstudio::project::detail##_name_Impl>;
 
  // create an instantiation of the vector class
  %ignore std::vector<openstudio::project::##_name>::vector(size_type);
  %ignore std::vector<openstudio::project::##_name>::resize(size_type);
  %template(_name##Vector) std::vector<openstudio::project::##_name>;
  
  // create an instantiation of the optional class
  %template(Optional##_name) boost::optional<openstudio::project::##_name>;
  
  // include the file
  %include <project/##_name.hpp>

  #if defined(SWIGRUBY)
      %extend openstudio::project::Record {
        %template(to_##_name) optionalCast<openstudio::project::##_name>;
      };
  #endif
%enddef

PROJECT_WRAP(Record);

// create an instantiation of the vector classes
%ignore std::vector<openstudio::project::RemoveUndo>::vector(size_type);
%ignore std::vector<openstudio::project::RemoveUndo>::resize(size_type);
%template(RemoveUndoVector) std::vector<openstudio::project::RemoveUndo>;

%ignore std::vector<openstudio::project::ProjectDatabase>::vector(size_type);
%ignore std::vector<openstudio::project::ProjectDatabase>::resize(size_type);
%template(ProjectDatabaseVector) std::vector<openstudio::project::ProjectDatabase>;

// create an instantiation of the optional class
%template(OptionalRemoveUndo) boost::optional<openstudio::project::RemoveUndo>;
%template(OptionalProjectDatabase) boost::optional<openstudio::project::ProjectDatabase>;

// include the file
%include <project/ProjectDatabase.hpp>
 
%define OBJECTRECORD_WRAP(_name)
  PROJECT_WRAP(_name)
  namespace openstudio {
    namespace project {
      boost::optional<_name> get##_name##ByName(ProjectDatabase &t_database, const std::string &t_name);
    }
  }
  %inline {
    namespace openstudio {
    namespace project {
      boost::optional<_name> get##_name##ByName(ProjectDatabase &t_database, const std::string &t_name) {
        return t_database.getObjectRecordByName<_name>(t_name);
      }
    }
    }
  }
%enddef

PROJECT_WRAP(ObjectRecord);
PROJECT_WRAP(ProjectDatabaseRecord);


OBJECTRECORD_WRAP(AttributeRecord);
OBJECTRECORD_WRAP(FileReferenceRecord);
OBJECTRECORD_WRAP(CloudSessionRecord);
OBJECTRECORD_WRAP(CloudSettingsRecord);
OBJECTRECORD_WRAP(TagRecord);
OBJECTRECORD_WRAP(UrlRecord);
OBJECTRECORD_WRAP(URLSearchPathRecord);
OBJECTRECORD_WRAP(VagrantSessionRecord);
OBJECTRECORD_WRAP(VagrantSettingsRecord);

OBJECTRECORD_WRAP(WorkflowRecord);

OBJECTRECORD_WRAP(OSArgumentRecord);

OBJECTRECORD_WRAP(MeasureRecord);
OBJECTRECORD_WRAP(NullMeasureRecord);
OBJECTRECORD_WRAP(RubyMeasureRecord);
OBJECTRECORD_WRAP(FunctionRecord);
OBJECTRECORD_WRAP(LinearFunctionRecord);
OBJECTRECORD_WRAP(VariableRecord);
OBJECTRECORD_WRAP(InputVariableRecord);
OBJECTRECORD_WRAP(DiscreteVariableRecord);
OBJECTRECORD_WRAP(MeasureGroupRecord);
OBJECTRECORD_WRAP(ContinuousVariableRecord);
OBJECTRECORD_WRAP(RubyContinuousVariableRecord);
OBJECTRECORD_WRAP(OutputVariableRecord);
OBJECTRECORD_WRAP(OutputAttributeVariableRecord);
OBJECTRECORD_WRAP(ProblemRecord);
OBJECTRECORD_WRAP(OptimizationProblemRecord);
OBJECTRECORD_WRAP(AlgorithmRecord);
OBJECTRECORD_WRAP(OpenStudioAlgorithmRecord);
OBJECTRECORD_WRAP(DesignOfExperimentsRecord);
OBJECTRECORD_WRAP(SequentialSearchRecord);
OBJECTRECORD_WRAP(DakotaAlgorithmRecord);
OBJECTRECORD_WRAP(DDACEAlgorithmRecord);
OBJECTRECORD_WRAP(DataPointRecord);
OBJECTRECORD_WRAP(OptimizationDataPointRecord);
OBJECTRECORD_WRAP(DataPointValueRecord);
OBJECTRECORD_WRAP(AnalysisRecord);
OBJECTRECORD_WRAP(PSUADEDaceAlgorithmRecord);
OBJECTRECORD_WRAP(FSUDaceAlgorithmRecord);
OBJECTRECORD_WRAP(ParameterStudyAlgorithmRecord);
OBJECTRECORD_WRAP(SamplingAlgorithmRecord);

#endif // PROJECT_PROJECT_I

