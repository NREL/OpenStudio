#ifndef MODEL_CORE_I
#define MODEL_CORE_I

#ifdef SWIGPYTHON
  %module openstudiomodelcore
#endif


%include <model/Model_Common_Include.i>

#if defined SWIGRUBY

  %init %{
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_Model) { OpenStudio::Model::toModel(self); } }");
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_OptionalModel) { OpenStudio::Model::toOptionalModel(self); } }");
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_Component) { OpenStudio::Component::toComponent(self); } }");
    rb_eval_string("OpenStudio::Workspace.class_eval { define_method(:to_OptionalComponent) { OpenStudio::Component::toOptionalComponent(self); } }");  
  %}

#elif defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelCore
  
  // ignore building for now
  %ignore openstudio::model::Model::building;
  
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
  
#else

#endif


%{
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>
  #include <utilities/geometry/ThreeJS.hpp>

  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
%}

#if defined SWIGJAVA
  %rename(loadComponent) openstudio::model::Component::load;
  %ignore openstudio::model::Meter::name;
  %ignore openstudio::model::Meter::setName;
#endif

// templates for non-ModelObjects
%template(ModelVector) std::vector<openstudio::model::Model>;
%template(OptionalModel)boost::optional<openstudio::model::Model>;
%ignore std::vector<openstudio::model::Component>::vector(size_type);
%ignore std::vector<openstudio::model::Component>::resize(size_type);
%template(ComponentVector) std::vector<openstudio::model::Component>;
%template(OptionalComponent) boost::optional<openstudio::model::Component>;
%ignore std::vector<openstudio::model::Relationship>::vector(size_type);
%ignore std::vector<openstudio::model::Relationship>::resize(size_type);
// %template(RelationshipVector) std::vector<openstudio::model::Relationship>;
// %template(OptionalRelationship) boost::optional<openstudio::model::Relationship>;

// ignore visitor for now.
%ignore openstudio::model::ModelObject::accept;

// Ignore rawImpl, should that even be in the public interface?
%ignore openstudio::model::Model::rawImpl;

// Ignore plenum space type
%ignore openstudio::model::Model::plenumSpaceType;

// templates 
%ignore std::vector<openstudio::model::ModelObject>::vector(size_type);
%ignore std::vector<openstudio::model::ModelObject>::resize(size_type);
%template(ModelObjectVector)std::vector<openstudio::model::ModelObject>;
%template(ModelObjectVectorVector) std::vector<std::vector<openstudio::model::ModelObject> >;
%template(ModelObjectSet) std::set<openstudio::model::ModelObject>;
%template(OptionalModelObject)boost::optional<openstudio::model::ModelObject>;
%template(getModelObjectHandles) openstudio::getHandles<openstudio::model::ModelObject>;
%template(ScheduleTypeKey) std::pair<std::string,std::string>;
%template(ScheduleTypeKeyVector) std::vector< std::pair<std::string,std::string> >;

// include initial objects
%include <model/AccessPolicyStore.hpp>
%include <model/ModelObject.hpp>
%include <model/Model.hpp>
%include <model/ModelExtensibleGroup.hpp>
%include <model/Component.hpp>
// %include <model/Relationship.hpp>
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

  // This really shouldn't be necessary
  IdfObject toIdfObject() const {
    return *self;
  }
};

//MODELOBJECT_TEMPLATES(ModelObject); // swig preprocessor did not seem to see these for other objects so these are defined above 
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
MODELOBJECT_TEMPLATES(GenericModelObject);
MODELOBJECT_TEMPLATES(ModelObjectList);
MODELOBJECT_TEMPLATES(EnergyManagementSystemSensor);
MODELOBJECT_TEMPLATES(EnergyManagementSystemActuator);
MODELOBJECT_TEMPLATES(EnergyManagementSystemConstructionIndexVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemCurveOrTableIndexVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemGlobalVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemInternalVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemMeteredOutputVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemTrendVariable);
MODELOBJECT_TEMPLATES(EnergyManagementSystemSubroutine);
MODELOBJECT_TEMPLATES(EnergyManagementSystemProgram);
MODELOBJECT_TEMPLATES(EnergyManagementSystemProgramCallingManager);
MODELOBJECT_TEMPLATES(EnergyManagementSystemOutputVariable);
UNIQUEMODELOBJECT_TEMPLATES(OutputEnergyManagementSystem);

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
SWIG_MODELOBJECT(GenericModelObject, 0); 
SWIG_MODELOBJECT(ModelObjectList, 1);
SWIG_MODELOBJECT(EnergyManagementSystemSensor, 1);
SWIG_MODELOBJECT(EnergyManagementSystemActuator, 1);
SWIG_MODELOBJECT(EnergyManagementSystemConstructionIndexVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemCurveOrTableIndexVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemGlobalVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemInternalVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemMeteredOutputVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemTrendVariable, 1);
SWIG_MODELOBJECT(EnergyManagementSystemSubroutine, 1);
SWIG_MODELOBJECT(EnergyManagementSystemProgram, 1);
SWIG_MODELOBJECT(EnergyManagementSystemProgramCallingManager, 1);
SWIG_MODELOBJECT(EnergyManagementSystemOutputVariable, 1);
SWIG_UNIQUEMODELOBJECT(OutputEnergyManagementSystem);

#endif //MODEL_CORE_I 
