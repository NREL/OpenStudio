#ifndef ALFALFA_I
#define ALFALFA_I

#ifdef SWIGPYTHON
%module(directors="1") openstudioalfalfa
#endif

%include <utilities/UtilitiesAPI.hpp>
#define ALFALFA_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>

%ignore openstudio::alfalfa::detail;

%{
  #include <alfalfa/AlfalfaComponent.hpp>
  #include <alfalfa/AlfalfaActuator.hpp>
  #include <alfalfa/AlfalfaConstant.hpp>
  #include <alfalfa/AlfalfaGlobalVariable.hpp>
  #include <alfalfa/AlfalfaMeter.hpp>
  #include <alfalfa/AlfalfaOutputVariable.hpp>
  #include <alfalfa/AlfalfaPoint.hpp>
  #include <alfalfa/AlfalfaJSON.hpp>
  #include <alfalfa/AlfalfaJSON_Impl.hpp>


  #include <utilities/idf/IdfObject.hpp>
  #include <model/ModelObject.hpp>


  using namespace openstudio;
  using namespace openstudio::alfalfa;
%}

%ignore openstudio::alfalfa::AlfalfaComponentBase;
%ignore openstudio::alfalfa::AlfalfaActuator::clone;
%ignore openstudio::alfalfa::AlfalfaConstant::clone;
%ignore openstudio::alfalfa::AlfalfaMeter::clone;
%ignore openstudio::alfalfa::AlfalfaGlobalVariable::clone;
%ignore openstudio::alfalfa::AlfalfaOutputVariable::clone;

%include <alfalfa/AlfalfaComponentBase.hpp>
%include <alfalfa/AlfalfaComponent.hpp>
%include <alfalfa/AlfalfaActuator.hpp>
%include <alfalfa/AlfalfaConstant.hpp>
%include <alfalfa/AlfalfaGlobalVariable.hpp>
%include <alfalfa/AlfalfaMeter.hpp>
%include <alfalfa/AlfalfaOutputVariable.hpp>
%include <alfalfa/AlfalfaPoint.hpp>
%include <alfalfa/AlfalfaJSON.hpp>

%ignore std::vector<openstudio::alfalfa::AlfalfaPoint>::vector(size_type);
%ignore std::vector<openstudio::alfalfa::AlfalfaPoint>::resize(size_type);
%template(AlfalfaPointVector) std::vector<openstudio::alfalfa::AlfalfaPoint>;
%template(OptionalAlfalfaPoint) boost::optional<openstudio::alfalfa::AlfalfaPoint>;
%template(OptionalAlfalfaComponent) boost::optional<openstudio::alfalfa::AlfalfaComponent>;

namespace openstudio::alfalfa {
  %extend AlfalfaPoint {
    void setOutput(const AlfalfaComponentBase& component) {
      AlfalfaComponent alfalfa_component(component);
      $self->setOutput(alfalfa_component);
    }

    void setInput(const AlfalfaComponentBase& component) {
      AlfalfaComponent alfalfa_component(component);
      $self->setInput(alfalfa_component);
    }
  }

  %extend AlfalfaJSON {
    boost::optional<AlfalfaPoint> AlfalfaJSON::exposeFromComponent(const AlfalfaComponentBase& component, const std::string& display_name = std::string()) {
      AlfalfaComponent alfalfa_component(component);
      return $self->exposeFromComponent(alfalfa_component, display_name);
    }
  }
}


#endif
