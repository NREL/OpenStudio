#ifndef UTILITIES_ALFALFA_I
#define UTILITIES_ALFALFA_I

%{
  namespace openstudio{  }
  using namespace openstudio;
  #include <utilities/alfalfa/AlfalfaComponent.hpp>
  #include <utilities/alfalfa/AlfalfaComponents.hpp>
  #include <utilities/alfalfa/AlfalfaPoint.hpp>

%}

%template(AlfalfaPointVector) std::vector<openstudio::alfalfa::AlfalfaPoint>;
%template(OptionalAlfalfaPoint) boost::optional<openstudio::alfalfa::AlfalfaPoint>;

%template(OptionalAlfalfaComponent) boost::optional<openstudio::alfalfa::AlfalfaComponent>;

%include <utilities/alfalfa/AlfalfaComponent.hpp>
%include <utilities/alfalfa/AlfalfaComponents.hpp>
%include <utilities/alfalfa/AlfalfaPoint.hpp>

#endif
