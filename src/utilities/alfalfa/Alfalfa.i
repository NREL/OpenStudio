#ifndef UTILITIES_ALFALFA_I
#define UTILITIES_ALFALFA_I

%{
  namespace openstudio{  }
  using namespace openstudio;
  #include <utilities/alfalfa/AlfalfaComponent.hpp>
  #include <utilities/alfalfa/AlfalfaConstant.hpp>
  #include <utilities/alfalfa/AlfalfaActuator.hpp>
  #include <utilities/alfalfa/AlfalfaMeter.hpp>
  #include <utilities/alfalfa/AlfalfaOutputVariable.hpp>
  #include <utilities/alfalfa/AlfalfaGlobalVariable.hpp>
  #include <utilities/alfalfa/AlfalfaPoint.hpp>

%}

%template(AlfalfaPointVector) std::vector<openstudio::alfalfa::AlfalfaPoint>;
%template(OptionalAlfalfaPoint) boost::optional<openstudio::alfalfa::AlfalfaPoint>;

%include <utilities/alfalfa/AlfalfaComponent.hpp>
%include <utilities/alfalfa/AlfalfaConstant.hpp>
%include <utilities/alfalfa/AlfalfaActuator.hpp>
%include <utilities/alfalfa/AlfalfaMeter.hpp>
%include <utilities/alfalfa/AlfalfaOutputVariable.hpp>
%include <utilities/alfalfa/AlfalfaGlobalVariable.hpp>
%include <utilities/alfalfa/AlfalfaPoint.hpp>

#endif
