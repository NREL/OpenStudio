#ifndef UTILITIES_ECONOMICS_ECONOMICS_I
#define UTILITIES_ECONOMICS_ECONOMICS_I

%{
  #include <utilities/economics/CostObject.hpp>
  #include <utilities/economics/Economics.hpp>
%}

// These functions return via reference parameters - something we cannot support with SWIG
%ignore openstudio::Economics::getDistrictCool;
%ignore openstudio::Economics::getDistrictHeat;
%ignore openstudio::Economics::getElec;
%ignore openstudio::Economics::getGas;
%ignore openstudio::Economics::getIRR;
%ignore openstudio::Economics::recursiveNewtonCalc;

%template(CostObjectPtr) std::shared_ptr<openstudio::CostObject>;
%include <utilities/economics/CostObject.hpp>
%include <utilities/economics/Economics.hpp>

#endif //UTILITIES_ECONOMICS_ECONOMICS_I 
