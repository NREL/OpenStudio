#ifndef MODEL_COMMON_INCLUDE_I
#define MODEL_COMMON_INCLUDE_I

#define UTILITIES_API
#define MODEL_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>

%ignore openstudio::model::detail;

// Ignore the ostream operator<<, especially for CSharp where it'll throw a multiple definition
// This is done in utilities/core/csharp/LanguageSpecific.i
//%ignore openstudio::model::operator<<;

%{
  #include <model/Model.hpp>
  #include <model/Model_Impl.hpp>
  #include <model/Component.hpp>
  #include <model/Component_Impl.hpp>
  #include <model/ModelObject.hpp>
  #include <model/ModelObject_Impl.hpp>
  #include <model/ModelExtensibleGroup.hpp>
  #include <model/GenericModelObject.hpp>
  #include <model/GenericModelObject_Impl.hpp>

  #include <model/FileOperations.hpp>

  // central list of all concrete ModelObject header files (_Impl and non-_Impl)
  #include <model/ConcreteModelObjects.hpp>

  #include <utilities/data/EndUses.hpp>

  #include <utilities/units/Unit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/WhUnit.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
%}

%define MODELOBJECT_TEMPLATES(_name)
  %ignore std::vector<openstudio::model::_name>::vector(size_type);
  %ignore std::vector<openstudio::model::_name>::resize(size_type);

  %template(_name##Vector) std::vector<openstudio::model::_name>;
  %template(Optional##_name) boost::optional<openstudio::model::_name>;
%enddef

%define UNIQUEMODELOBJECT_TEMPLATES(_name)
  %template(Optional##_name) boost::optional<openstudio::model::_name>;
%enddef

%define MODELEXTENSIBLEGROUP_TEMPLATES(_name)
  %ignore std::vector<openstudio::model::_name>::vector(size_type);
  %ignore std::vector<openstudio::model::_name>::resize(size_type);
  %template(_name##Vector) std::vector<openstudio::model::_name>;
  %template(Optional##_name) boost::optional<openstudio::model::_name>;
%enddef


#if defined SWIGRUBY

  %define MODELOBJECT_EXTENSION(_name)
  %init %{
    rb_eval_string("OpenStudio::IdfObject.class_eval { define_method(:to_" #_name ") { OpenStudio::Model::to" #_name "(self); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:get" #_name ") { |handle| OpenStudio::Model::get" #_name "(self, handle); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:get" #_name "s) { OpenStudio::Model::get" #_name "s(self); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:get" #_name "ByName) { |name| OpenStudio::Model::get" #_name "ByName(self, name); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:get" #_name "sByName) { |name, exactMatch| OpenStudio::Model::get" #_name "sByName(self, name, exactMatch); } }");
  %}
  %enddef

  %define UNIQUEMODELOBJECT_EXTENSION(_name)
  %init %{
    rb_eval_string("OpenStudio::IdfObject.class_eval { define_method(:to_" #_name ") { OpenStudio::Model::to" #_name "(self); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:get" #_name ") { OpenStudio::Model::get" #_name "(self); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:getOptional" #_name ") { OpenStudio::Model::getOptional" #_name "(self); } }");
  %}
  %enddef

  %define MODELEXTENSIBLEGROUP_EXTENSION(_name)
  %init %{
    rb_eval_string("OpenStudio::IdfExtensibleGroup.class_eval { define_method(:to_" #_name ") { OpenStudio::Model::to" #_name "(self); } }");
  %}
  %enddef

#elif defined SWIGCSHARP

  // should be able to do something here as C# supports partial classes
  // http://www.swig.org/Doc1.3/CSharp.html#csharp_extending_proxy_class

  // extensions in C# should probably not have to_name added to IdfObject
  // user should have to do idfObject.to_ModelObject.get.to_name

  %define MODELOBJECT_EXTENSION(_name)
    //%pragma(csharp) imclasscode=%{
    %typemap(csimports) openstudio::model::##_name %{
      using System;
      using System.Runtime.InteropServices;

      public partial class IdfObject {
        public Optional##_name to_##_name()
        {
          return OpenStudio.##_csharp_module_name##.to##_name(this);
        }
      }

      public partial class Model : Workspace {
        public _name##Vector get##_name##s()
        {
          return OpenStudio.##_csharp_module_name##.get##_name##s(this);
        }

        public Optional##_name get##_name##(OpenStudio.UUID handle)
        {
          return OpenStudio.##_csharp_module_name##.get##_name##(this, handle);
        }

        public Optional##_name get##_name##ByName(String name)
        {
          return OpenStudio.##_csharp_module_name##.get##_name##ByName(this, name);
        }

        public _name##Vector get##_name##sByName(String name, bool exactMatch)
        {
          return OpenStudio.##_csharp_module_name##.get##_name##sByName(this, name, exactMatch);
        }
      }
    %}
  %enddef

  %define UNIQUEMODELOBJECT_EXTENSION(_name)
    %typemap(csimports) openstudio::model::##_name %{
      using System;
      using System.Runtime.InteropServices;

      public partial class IdfObject {
        public Optional##_name to_##_name()
        {
          return OpenStudio.##_csharp_module_name##.to##_name(this);
        }
      }

      public partial class Model : Workspace {
        public _name get##_name##()
        {
          return OpenStudio.##_csharp_module_name##.get##_name##(this);
        }

        public Optional##_name getOptional##_name##() {
          return OpenStudio.##_csharp_module_name##.getOptional##_name##(this);
        }
      }
    %}
  %enddef

  %define MODELEXTENSIBLEGROUP_EXTENSION(_name)
    %typemap(csimports) openstudio::model::##_name %{
      using System;
      using System.Runtime.InteropServices;

      public partial class IdfExtensibleGroup {
        public Optional##_name to_##_name()
        {
          return OpenStudio.##_csharp_module_name##.to##_name(this);
        }
      }
    %}
  %enddef

#else

  #define MODELOBJECT_EXTENSION(_name)
  #define UNIQUEMODELOBJECT_EXTENSION(_name)
  #define MODELEXTENSIBLEGROUP_EXTENSION(_name)

#endif


%define SWIG_MODELOBJECT(_name, _isConcrete)

  #if defined SWIGJAVA
    %ignore openstudio::model::_name::iddObjectType();
  #endif

  #if defined SWIGCSHARP
    %typemap(csclassmodifiers) openstudio::model::##_name "public partial class"
    MODELOBJECT_EXTENSION(_name)
  #endif

  %include <model/##_name##.hpp>
  namespace openstudio {
    namespace model {

      boost::optional<_name> to##_name(const openstudio::IdfObject& idfObject);
      boost::optional<_name> get##_name(const Model &t_model, const openstudio::Handle &t_handle);
      std::vector<_name> get##_name##s(const Model &t_model);
      boost::optional<_name> get##_name##ByName(const Model &t_model, const std::string &t_name);
      std::vector<_name> get##_name##sByName(const Model &t_model, const std::string &t_name, bool t_exactMatch);
    }
  }
  %inline {
    namespace openstudio {
    namespace model {
      boost::optional<_name> to##_name(const openstudio::IdfObject& idfObject) {
        return idfObject.optionalCast<_name>();
      }
      boost::optional<_name> get##_name(const Model &t_model, const openstudio::Handle &t_handle) {
        return t_model.getModelObject<_name>(t_handle);
      }
      std::vector<_name> get##_name##s(const Model &t_model) {
        %#if _isConcrete
          return t_model.getConcreteModelObjects<_name>();
        %#else
          return t_model.getModelObjects<_name>();
        %#endif
      }
      boost::optional<_name> get##_name##ByName(const Model &t_model, const std::string &t_name) {
        %#if _isConcrete
          return t_model.getConcreteModelObjectByName<_name>(t_name);
        %#else
          return t_model.getModelObjectByName<_name>(t_name);
        %#endif
      }
      std::vector<_name> get##_name##sByName(const Model &t_model, const std::string &t_name, bool t_exactMatch) {
        %#if _isConcrete
          if (t_exactMatch){
            return t_model.getModelObjectsByName<_name>(t_name, t_exactMatch);
          }
          return t_model.getConcreteModelObjectsByName<_name>(t_name);
        %#else
          return t_model.getModelObjectsByName<_name>(t_name, t_exactMatch);
        %#endif
      }
    }
    }
  }

  #if defined SWIGRUBY
    MODELOBJECT_EXTENSION(_name)
  #endif

%enddef

%define SWIG_UNIQUEMODELOBJECT(_name)

  #if defined SWIGJAVA
    %ignore openstudio::model::_name::iddObjectType();
  #endif

  #if defined SWIGCSHARP
    %typemap(csclassmodifiers) openstudio::model::##_name "public partial class"
    UNIQUEMODELOBJECT_EXTENSION(_name)
  #endif

  %include <model/##_name##.hpp>
  namespace openstudio {
    namespace model {

      boost::optional<_name> to##_name(const openstudio::IdfObject& idfObject);
      _name get##_name(openstudio::model::Model& t_model);
      boost::optional<_name> getOptional##_name(const openstudio::model::Model& t_model);
    }
  }
  %inline {
    namespace openstudio {
    namespace model {
      boost::optional<_name> to##_name(const openstudio::IdfObject& idfObject) {
        return idfObject.optionalCast<_name>();
      }
      _name get##_name(openstudio::model::Model& t_model) {
        return t_model.getUniqueModelObject<openstudio::model::##_name>();
      }
      boost::optional<_name> getOptional##_name(const openstudio::model::Model& t_model) {
        return t_model.getOptionalUniqueModelObject<openstudio::model::##_name>();
      }
    }
    }
  }

  #if defined SWIGRUBY
    UNIQUEMODELOBJECT_EXTENSION(_name)
  #endif

%enddef

%define SWIG_MODELEXTENSIBLEGROUP(_name)

  #if defined SWIGJAVA

  #endif

  #if defined SWIGCSHARP
    %typemap(csclassmodifiers) openstudio::model::##_name "public partial class"
    MODELEXTENSIBLEGROUP_EXTENSION(_name)
  #endif

  namespace openstudio {
  namespace model {
    boost::optional<_name> to##_name(const openstudio::IdfExtensibleGroup& extensibleGroup);
  }
  }
  %inline {
    namespace openstudio {
    namespace model {
      boost::optional<_name> to##_name(const openstudio::IdfExtensibleGroup& extensibleGroup) {
        return extensibleGroup.optionalCast<_name>();
      }
    }
    }
  }

  #if defined SWIGRUBY
    MODELEXTENSIBLEGROUP_EXTENSION(_name)
  #endif

%enddef


#endif //MODEL_I
