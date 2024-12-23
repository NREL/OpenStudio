
#ifndef MODEL_CORE_I
#define MODEL_CORE_I

#ifdef SWIGPYTHON
  %module(directors="1") openstudiomodelcore
#endif

%include <model/Model_Common_Include.i>

#if defined SWIGRUBY

  %init %{
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_Model) { OpenStudio::Model::toModel(self); } }");
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_OptionalModel) { OpenStudio::Model::toOptionalModel(self); } }");
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_Component) { OpenStudio::Model::toComponent(self); } }");
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_OptionalComponent) { OpenStudio::Model::toOptionalComponent(self); } }");
  %}

#elif defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelCore

  // Ignore AccessPolicyStore which is used by GUIs only
  %ignore openstudio::model::AccessPolicyStore;

  // ignore simulation objects for now, add back in with partial classes in ModelSimulation.i
  %ignore openstudio::model::Model::runPeriod; // Special case, it behaves like a Unique ModelObject
  %ignore openstudio::model::Model::climateZones;
  %ignore openstudio::model::Model::convergenceLimits;
  %ignore openstudio::model::Model::environmentalImpactFactors;
  %ignore openstudio::model::Model::foundationKivaSettings;
  %ignore openstudio::model::Model::heatBalanceAlgorithm;
  %ignore openstudio::model::Model::insideSurfaceConvectionAlgorithm;
  %ignore openstudio::model::Model::lightingSimulationControl;
  %ignore openstudio::model::Model::outputControlFiles;
  %ignore openstudio::model::Model::outputControlReportingTolerances;
  %ignore openstudio::model::Model::outputControlTableStyle;
  %ignore openstudio::model::Model::outputControlTimestamp;
  %ignore openstudio::model::Model::outputDebuggingData;
  %ignore openstudio::model::Model::outputDiagnostics;
  %ignore openstudio::model::Model::outputJSON;
  %ignore openstudio::model::Model::outputSQLite;
  %ignore openstudio::model::Model::outputTableSummaryReports;
  %ignore openstudio::model::Model::outputSchedules;
  %ignore openstudio::model::Model::outputConstructions;
  %ignore openstudio::model::Model::outsideSurfaceConvectionAlgorithm;
  %ignore openstudio::model::Model::performancePrecisionTradeoffs;
  %ignore openstudio::model::Model::runPeriodControlDaylightSavingTime;
  %ignore openstudio::model::Model::shadowCalculation;
  %ignore openstudio::model::Model::simulationControl;
  %ignore openstudio::model::Model::siteGroundReflectance;
  %ignore openstudio::model::Model::siteGroundTemperatureBuildingSurface;
  %ignore openstudio::model::Model::siteGroundTemperatureDeep;
  %ignore openstudio::model::Model::siteGroundTemperatureFCfactorMethod;
  %ignore openstudio::model::Model::siteGroundTemperatureShallow;
  %ignore openstudio::model::Model::siteWaterMainsTemperature;
  %ignore openstudio::model::Model::sizingParameters;
  %ignore openstudio::model::Model::timestep;
  %ignore openstudio::model::Model::weatherFile;
  %ignore openstudio::model::Model::yearDescription;
  %ignore openstudio::model::Model::zoneAirContaminantBalance;
  %ignore openstudio::model::Model::zoneAirHeatBalanceAlgorithm;
  %ignore openstudio::model::Model::zoneAirMassFlowConservation;
  %ignore openstudio::model::Model::zoneCapacitanceMultiplierResearchSpecial;

  // ignore geometry objects for now, add back in with partial classes in ModelGeometry.i
  %ignore openstudio::model::Model::building;
  %ignore openstudio::model::Model::facility;
  %ignore openstudio::model::Model::site;
  %ignore openstudio::model::Model::plenumSpaceType;

  // Ignore hvac objects for now, add back in with partial classes in ModelHVAC.i
  %ignore openstudio::model::Model::outdoorAirNode;

  // Ignore airflow objects for now, add back in with partial classes in ModelAirflow.i
  %ignore openstudio::model::Model::airflowNetworkSimulationControl;

  // Ignore resources objects for now, add back in with partial classes in ModelResources.i
  %ignore openstudio::model::Model::pythonPluginSearchPaths;

  // EnergyManagementSystemActuator: depends on Space (ModelGeometry.i),
  %ignore openstudio::model::EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& modelObject,
                                                                                            const std::string& actuatedComponentType,
                                                                                            const std::string& actuatedComponentControlType,
                                                                                            const Space& space);
  %ignore openstudio::model::EnergyManagementSystemActuator::setSpace;

  // depends on ThermalZone (ModelHVAC.i)
  %ignore openstudio::model::EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& modelObject,
                                                                                            const std::string& actuatedComponentType,
                                                                                            const std::string& actuatedComponentControlType,
                                                                                            const ThermalZone& thermalZone);
  %ignore openstudio::model::EnergyManagementSystemActuator::setThermalZone;


  // depends on Constructions (Geometry.i)
  %ignore openstudio::model::EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(const Model& model, const Construction& construction);
  // Note JM 2019-04-16: setConstructioObject doesn't need to be ignored and reimplemented because it takes a ModelObject and not a Construction

  // Depends on Curves (ModelResources.i)
  %ignore openstudio::model::EnergyManagementSystemCurveOrTableIndexVariable::EnergyManagementSystemCurveOrTableIndexVariable(const Model& model, const Curve& curve);
  %ignore openstudio::model::EnergyManagementSystemCurveOrTableIndexVariable::setCurveOrTableObject;
  // getter curveOrTableObject doesn't need to be ignored and reimplemented because it returns a ModelObject

  // Overload resolution: prefer std::string over char const *
  %ignore openstudio::model::AdditionalProperties::setFeature(std::string const&, char const*);


  // should be able to do something here as C# supports partial classes
  // http://www.swig.org/Doc1.3/CSharp.html#csharp_extending_proxy_class
  %typemap(csclassmodifiers) openstudio::model::Model "public partial class"
  %typemap(csclassmodifiers) openstudio::model::ModelObject "public partial class"

  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    //public partial class IdfObject {
    //  public OptionalModelObject to_ModelObject()
    //  {
    //   return OpenStudio.OpenStudioModelCore.toModelObject(this);
    //  }
    //}
  %}

#elif defined SWIGJAVA

  // ignore building for now
  %ignore openstudio::model::Model::building;

  // Ignore plenum space type for now
  %ignore openstudio::model::Model::plenumSpaceType;

  // Ignore outdoor air node for now
  %ignore openstudio::model::Model::outdoorAirNode;

  %rename(loadComponent) openstudio::model::Component::load;
  %ignore openstudio::model::Meter::name;
  %ignore openstudio::model::Meter::setName;

#elif defined SWIGPYTHON
  // This is the only module where this isn't needed, since we ARE in openstudiomodelcore so Model already exists
  // %pythoncode %{
  //  Model = openstudiomodelcore.Model
  // %}

  // At this point, 'Model' and co aren't declared yet, so forward declare type hints
  %pythoncode %{
    def _workspace_to_Model(self) -> "Model":
        """Cast the Workspace to a Model.

        Throws if not an actual Model

        :return: A Model.
        """
        return toModel(self)
    openstudioutilitiesidf.Workspace.to_Model = _workspace_to_Model

    def _workspace_to_OptionalModel(self) -> "OptionalModel":
        """Try to cast the Workspace to a Model.

        :return: An OptionalModel.
        """
        return toOptionalModel(self)
    openstudioutilitiesidf.Workspace.to_OptionalModel = _workspace_to_OptionalModel



    def _workspace_to_Component(self) -> "Component":
        """Cast the Workspace to a Component.

        Throws if not an actual Component

        :return: A Component.
        """
        return toComponent(self)
    openstudioutilitiesidf.Workspace.to_Component = _workspace_to_Component

    def _workspace_to_OptionalComponent(self) -> "OptionalComponent":
        """Try to cast the Workspace to a Component.

        :return: An OptionalComponent.
        """
        return toOptionalComponent(self)
    openstudioutilitiesidf.Workspace.to_OptionalComponent = _workspace_to_OptionalComponent
  %}

#else

#endif


%{
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>
  #include <utilities/geometry/ThreeJS.hpp>

  #include <utilities/units/Unit.hpp>
%}

// templates for non-ModelObjects
%template(ModelVector) std::vector<openstudio::model::Model>;
%template(OptionalModel)boost::optional<openstudio::model::Model>;
%ignore std::vector<openstudio::model::Component>::vector(size_type);
%ignore std::vector<openstudio::model::Component>::resize(size_type);
%template(ComponentVector) std::vector<openstudio::model::Component>;
%template(OptionalComponent) boost::optional<openstudio::model::Component>;

// ignore visitor for now.
%ignore openstudio::model::ModelObject::accept;

// Ignore rawImpl, should that even be in the public interface?
%ignore openstudio::model::Model::rawImpl;

namespace openstudio {
namespace model {

// forward declarations
class SpaceType;
class Node;

// For EMS
class Space;
class ThermalZone;
class Curve;
class Construction;

// DLM: forward declaring these classes and requesting the valuewrapper feature seems to be sufficient for the Ruby bindings
// For C# we ignore any methods using these and then reimplement using partial class later
%feature("valuewrapper") SpaceType;
%feature("valuewrapper") Node;
%feature("valuewrapper") Space;
%feature("valuewrapper") ThermalZone;
%feature("valuewrapper") Curve;
%feature("valuewrapper") Construction;

%feature("director") Model;

}
}

// templates
%ignore std::vector<openstudio::model::ModelObject>::vector(size_type);
%ignore std::vector<openstudio::model::ModelObject>::resize(size_type);
%template(ModelObjectVector)std::vector<openstudio::model::ModelObject>;
%template(ModelObjectVectorVector) std::vector<std::vector<openstudio::model::ModelObject> >;
%template(ModelObjectSet) std::set<openstudio::model::ModelObject>;
%template(OptionalModelObject)boost::optional<openstudio::model::ModelObject>;
%template(getModelObjectHandles) openstudio::getHandles<openstudio::model::ModelObject>;

// include initial objects
%include <model/ModelObject.hpp>
%include <model/Model.hpp>
%include <model/ModelExtensibleGroup.hpp>
%include <model/Component.hpp>
%include <model/FileOperations.hpp>


namespace openstudio {
namespace model {
  Model toModel(const openstudio::Workspace& workspace);
  boost::optional<Model> toOptionalModel(const openstudio::Workspace& workspace);
  Component toComponent(const openstudio::Workspace& workspace);
  boost::optional<Component> toOptionalComponent(const openstudio::Workspace& workspace);
}
}

%inline {

  namespace openstudio {
  namespace model {
    Model toModel(const openstudio::Workspace& workspace) {
      return workspace.cast<Model>();
    }
    boost::optional<Model> toOptionalModel(const openstudio::Workspace& workspace) {
      return workspace.optionalCast<Model>();
    }
    Component toComponent(const openstudio::Workspace& workspace) {
      return workspace.cast<Component>();
    }
    boost::optional<Component> toOptionalComponent(const openstudio::Workspace& workspace) {
      return workspace.optionalCast<Component>();
    }
  }
  }
}

// extend classes
%extend openstudio::model::ModelObject{
  std::string __str__() {
    std::ostringstream os;
    os << self->idfObject();
    return os.str();
  }

  // This really should not be necessary
  IdfObject toIdfObject() const {
    return *self;
  }
};

//MODELOBJECT_TEMPLATES(ModelObject); // swig preprocessor did not seem to see these for other objects so these are defined above
MODELOBJECT_TEMPLATES(ScheduleTypeKey);
MODELOBJECT_TEMPLATES(EMSActuatorNames);
MODELEXTENSIBLEGROUP_TEMPLATES(ModelExtensibleGroup);
MODELOBJECT_TEMPLATES(ParentObject);
MODELOBJECT_TEMPLATES(ResourceObject);
UNIQUEMODELOBJECT_TEMPLATES(Version);
UNIQUEMODELOBJECT_TEMPLATES(LifeCycleCostParameters);
UNIQUEMODELOBJECT_TEMPLATES(RadianceParameters);
MODELOBJECT_TEMPLATES(OutputMeter);
MODELOBJECT_TEMPLATES(MeterCustom);
MODELOBJECT_TEMPLATES(MeterCustomDecrement);
MODELOBJECT_TEMPLATES(LifeCycleCost);
MODELOBJECT_TEMPLATES(UtilityBill);
MODELEXTENSIBLEGROUP_TEMPLATES(BillingPeriod)
MODELOBJECT_TEMPLATES(ComponentData);
MODELOBJECT_TEMPLATES(ScheduleTypeLimits); // Needed for OutputVariable
MODELOBJECT_TEMPLATES(ScheduleBase); // Needed for OutputVariable
MODELOBJECT_TEMPLATES(Schedule); // Needed for OutputVariable
MODELOBJECT_TEMPLATES(ScheduleDay);
MODELOBJECT_TEMPLATES(ScheduleWeek);
MODELOBJECT_TEMPLATES(ScheduleYear);
MODELOBJECT_TEMPLATES(ScheduleRule);
MODELOBJECT_TEMPLATES(ScheduleRuleset);
MODELOBJECT_TEMPLATES(OutputVariable);
MODELOBJECT_TEMPLATES(AdditionalProperties);
MODELOBJECT_TEMPLATES(GenericModelObject);
MODELOBJECT_TEMPLATES(ModelObjectList);
MODELOBJECT_TEMPLATES(EnergyManagementSystemSensor);
MODELOBJECT_TEMPLATES(EnergyManagementSystemActuator);
MODELOBJECT_TEMPLATES(EnergyManagementSystemConstructionIndexVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemCurveOrTableIndexVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemGlobalVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemInternalVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemTrendVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemSubroutine);
MODELOBJECT_TEMPLATES(EnergyManagementSystemProgram);
MODELOBJECT_TEMPLATES(EnergyManagementSystemProgramCallingManager);
MODELOBJECT_TEMPLATES(EnergyManagementSystemOutputVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemMeteredOutputVariable);
UNIQUEMODELOBJECT_TEMPLATES(OutputEnergyManagementSystem);
UNIQUEMODELOBJECT_TEMPLATES(ExternalInterface);
MODELOBJECT_TEMPLATES(ExternalInterfaceActuator);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitExportFromVariable);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitExportToActuator);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitExportToSchedule);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitExportToVariable);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitImport);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitImportFromVariable);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitImportToActuator);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitImportToSchedule);
MODELOBJECT_TEMPLATES(ExternalInterfaceFunctionalMockupUnitImportToVariable);
MODELOBJECT_TEMPLATES(ExternalInterfaceSchedule);
MODELOBJECT_TEMPLATES(ExternalInterfaceVariable);

SWIG_MODELOBJECT(ModelObject, 0);
SWIG_MODELEXTENSIBLEGROUP(ModelExtensibleGroup);
SWIG_MODELOBJECT(ParentObject, 0);
SWIG_MODELOBJECT(ResourceObject, 0);
SWIG_UNIQUEMODELOBJECT(Version);
SWIG_UNIQUEMODELOBJECT(LifeCycleCostParameters);
SWIG_UNIQUEMODELOBJECT(RadianceParameters);
SWIG_MODELOBJECT(OutputMeter, 1);
SWIG_MODELOBJECT(MeterCustom, 1);
SWIG_MODELOBJECT(MeterCustomDecrement, 1);
SWIG_MODELOBJECT(LifeCycleCost, 1);
SWIG_MODELOBJECT(UtilityBill, 1);
SWIG_MODELEXTENSIBLEGROUP(BillingPeriod);
SWIG_MODELOBJECT(ComponentData, 1);
SWIG_MODELOBJECT(ScheduleTypeLimits, 1); // Needed for OutputVariable
SWIG_MODELOBJECT(ScheduleBase, 0); // Needed for OutputVariable
SWIG_MODELOBJECT(Schedule, 0);  // Needed for OutputVariable
SWIG_MODELOBJECT(ScheduleDay, 1);
SWIG_MODELOBJECT(ScheduleWeek, 1);
SWIG_MODELOBJECT(ScheduleYear, 1);
SWIG_MODELOBJECT(ScheduleRule, 1);
SWIG_MODELOBJECT(ScheduleRuleset, 1);
SWIG_MODELOBJECT(OutputVariable, 1);
SWIG_MODELOBJECT(AdditionalProperties, 1);
SWIG_MODELOBJECT(GenericModelObject, 0);
SWIG_MODELOBJECT(ModelObjectList, 1);
SWIG_MODELOBJECT(EnergyManagementSystemSensor, 1);
SWIG_MODELOBJECT(EnergyManagementSystemActuator, 1);
SWIG_MODELOBJECT(EnergyManagementSystemConstructionIndexVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemCurveOrTableIndexVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemGlobalVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemInternalVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemTrendVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemSubroutine, 1);
SWIG_MODELOBJECT(EnergyManagementSystemProgram, 1);
SWIG_MODELOBJECT(EnergyManagementSystemProgramCallingManager, 1);
SWIG_MODELOBJECT(EnergyManagementSystemOutputVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemMeteredOutputVariable, 1);
SWIG_UNIQUEMODELOBJECT(OutputEnergyManagementSystem);
SWIG_UNIQUEMODELOBJECT(ExternalInterface);
SWIG_MODELOBJECT(ExternalInterfaceActuator, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitExportFromVariable, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitExportToActuator, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitExportToSchedule, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitExportToVariable, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitImport, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitImportFromVariable, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitImportToActuator, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitImportToSchedule, 1);
SWIG_MODELOBJECT(ExternalInterfaceFunctionalMockupUnitImportToVariable, 1);
SWIG_MODELOBJECT(ExternalInterfaceSchedule, 1);
SWIG_MODELOBJECT(ExternalInterfaceVariable, 1);

#endif //MODEL_CORE_I
