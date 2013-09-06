#ifndef MODEL_CORE_I
#define MODEL_CORE_I

#ifdef SWIGPYTHON
%module openstudiomodelcore
#endif


%include <model/Model_Common_Include.i>

#if defined SWIGRUBY

%init %{
  rb_eval_string("OpenStudio::IdfObject.class_eval { define_method(:to_ModelObject) { OpenStudio::Model::toModelObject(self); } }");
  rb_eval_string("OpenStudio::IdfExtensibleGroup.class_eval { define_method(:to_ModelExtensibleGroup) { OpenStudio::Model::toModelExtensibleGroup(self); } }");
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
  
#else

#endif


%{
  #include <utilities/data/TimeSeries.hpp>
  #include <utilities/sql/SqlFile.hpp>

  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
%}


// templates for non-ModelObjects
%template(ModelVector) std::vector<openstudio::model::Model>;
%ignore std::vector<openstudio::model::Component>::vector(size_type);
%ignore std::vector<openstudio::model::Component>::resize(size_type);
%template(ComponentVector) std::vector<openstudio::model::Component>;
%template(OptionalModel) boost::optional<openstudio::model::Model>;
%template(OptionalComponent) boost::optional<openstudio::model::Component>;
%ignore std::vector<openstudio::model::Relationship>::vector(size_type);
%ignore std::vector<openstudio::model::Relationship>::resize(size_type);
%template(RelationshipVector) std::vector<openstudio::model::Relationship>;
%template(OptionalRelationship) boost::optional<openstudio::model::Relationship>;

// ignore visitor for now.
%ignore openstudio::model::ModelObject::accept;

// Ignore rawImpl, should that even be in the public interface?
%ignore openstudio::model::Model::rawImpl;

// templates for ModelObject
%ignore std::vector<openstudio::model::ModelObject>::vector(size_type);
%ignore std::vector<openstudio::model::ModelObject>::resize(size_type);
%template(ModelObjectVector) std::vector<openstudio::model::ModelObject>;
%template(ModelObjectVectorVector) std::vector<std::vector<openstudio::model::ModelObject> >;
%template(ModelObjectSet) std::set<openstudio::model::ModelObject>;
%template(OptionalModelObject) boost::optional<openstudio::model::ModelObject>;
%template(getModelObjectHandles) openstudio::getHandles<openstudio::model::ModelObject>;
%ignore std::vector<openstudio::model::ModelExtensibleGroup>::vector(size_type);
%ignore std::vector<openstudio::model::ModelExtensibleGroup>::resize(size_type);
%template(ModelExtensibleGroupVector) std::vector<openstudio::model::ModelExtensibleGroup>;
%template(OptionalModelExtensibleGroup) boost::optional<openstudio::model::ModelExtensibleGroup>;
%template(ScheduleTypeKey) std::pair<std::string,std::string>;
%template(ScheduleTypeKeyVector) std::vector< std::pair<std::string,std::string> >;

// include initial objects
%include <model/AccessPolicyStore.hpp>
%include <model/ModelObject.hpp>
%include <model/Model.hpp>
%include <model/ModelExtensibleGroup.hpp>
%include <model/Component.hpp>
%include <model/Relationship.hpp>

%extend openstudio::model::Model {
  %template(getModelObjects) getModelObjects<openstudio::model::ModelObject>;
}

namespace openstudio {
namespace model {
  boost::optional<ModelObject> toModelObject(const openstudio::IdfObject& idfObject);
  boost::optional<ModelExtensibleGroup> toModelExtensibleGroup(const openstudio::IdfExtensibleGroup& idfEG);
  Model toModel(const openstudio::Workspace& workspace);
  boost::optional<Model> toOptionalModel(const openstudio::Workspace& workspace);
  Component toComponent(const openstudio::Workspace& workspace);
  boost::optional<Component> toOptionalComponent(const openstudio::Workspace& workspace);
}
}

%inline {

  namespace openstudio {
  namespace model {
    boost::optional<ModelObject> toModelObject(const openstudio::IdfObject& idfObject) {
      return idfObject.optionalCast<ModelObject>();
    }
    boost::optional<ModelExtensibleGroup> toModelExtensibleGroup(const openstudio::IdfExtensibleGroup& idfEG) 
    {
      return idfEG.optionalCast<ModelExtensibleGroup>();
    }
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

  // This really shouldnt be necessary
  IdfObject toIdfObject() const {
    return *self;
  }
};

MODELOBJECT_TEMPLATES(ParentObject);
MODELOBJECT_TEMPLATES(ResourceObject);
UNIQUEMODELOBJECT_TEMPLATES(Version);
UNIQUEMODELOBJECT_TEMPLATES(LifeCycleCostParameters);
UNIQUEMODELOBJECT_TEMPLATES(RadianceParameters);
MODELOBJECT_TEMPLATES(Meter);
MODELOBJECT_TEMPLATES(LifeCycleCost); // Probably need to make a ModelEconomics.i file for these
MODELOBJECT_TEMPLATES(UtilityBill); // Probably need to make a ModelEconomics.i file for these
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

SWIG_MODELOBJECT(ParentObject);
SWIG_MODELOBJECT(ResourceObject);
SWIG_UNIQUEMODELOBJECT(Version);
SWIG_UNIQUEMODELOBJECT(LifeCycleCostParameters);
SWIG_UNIQUEMODELOBJECT(RadianceParameters);
SWIG_MODELOBJECT(Meter);
SWIG_MODELOBJECT(LifeCycleCost); // Probably need to make a ModelEconomics.i file for these
SWIG_MODELOBJECT(UtilityBill); // Probably need to make a ModelEconomics.i file for these
SWIG_MODELOBJECT(ComponentData);
SWIG_MODELOBJECT(ScheduleTypeLimits); // Needed for OutputVariable
SWIG_MODELOBJECT(ScheduleBase); // Needed for OutputVariable
SWIG_MODELOBJECT(Schedule);  // Needed for OutputVariable
SWIG_MODELOBJECT(ScheduleDay);
SWIG_MODELOBJECT(ScheduleWeek);
SWIG_MODELOBJECT(ScheduleYear);
SWIG_MODELOBJECT(ScheduleRule);
SWIG_MODELOBJECT(ScheduleRuleset);
SWIG_MODELOBJECT(OutputVariable);
SWIG_MODELOBJECT(GenericModelObject); 

#endif //MODEL_CORE_I 
