#ifndef UTILITIES_UNITS_UNIT_I
#define UTILITIES_UNITS_UNIT_I

%{
  #include <utilities/units/Unit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/SIUnit_Impl.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/IPUnit_Impl.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/BTUUnit_Impl.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/CFMUnit_Impl.hpp>
  #include <utilities/units/GPDUnit.hpp>
  #include <utilities/units/GPDUnit_Impl.hpp>
  #include <utilities/units/MPHUnit.hpp>
  #include <utilities/units/MPHUnit_Impl.hpp>
  #include <utilities/units/WhUnit.hpp>
  #include <utilities/units/WhUnit_Impl.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/ThermUnit_Impl.hpp>
  #include <utilities/units/Misc1Unit.hpp>
  #include <utilities/units/Misc1Unit_Impl.hpp>
  #include <utilities/units/TemperatureUnit.hpp>
  #include <utilities/units/TemperatureUnit_Impl.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/CelsiusUnit_Impl.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>
  #include <utilities/units/FahrenheitUnit_Impl.hpp>
%}

%import <utilities/units/Scale.i>

%define SWIG_UNIT_TEMPLATES(_name)
  %template(_name##Vector) std::vector<openstudio::_name>;
  %template(Optional##_name) boost::optional<openstudio::_name>;
%enddef

SWIG_UNIT_TEMPLATES(Unit);
// TemperatureUnit is abstract
%ignore std::vector<openstudio::TemperatureUnit>::vector(size_type);
%ignore std::vector<openstudio::TemperatureUnit>::resize(size_type);
SWIG_UNIT_TEMPLATES(TemperatureUnit);
SWIG_UNIT_TEMPLATES(CelsiusUnit);
SWIG_UNIT_TEMPLATES(FahrenheitUnit);
SWIG_UNIT_TEMPLATES(SIUnit);
SWIG_UNIT_TEMPLATES(IPUnit);
SWIG_UNIT_TEMPLATES(BTUUnit);
SWIG_UNIT_TEMPLATES(CFMUnit);
SWIG_UNIT_TEMPLATES(GPDUnit);
SWIG_UNIT_TEMPLATES(MPHUnit);
SWIG_UNIT_TEMPLATES(WhUnit);
SWIG_UNIT_TEMPLATES(ThermUnit);
SWIG_UNIT_TEMPLATES(Misc1Unit);


%define SWIG_UNIT(_name)
  %include <utilities/units/##_name##.hpp>
  
  %extend openstudio::Unit {
    %template(to_##_name) optionalCast<openstudio::##_name>;
  }
%enddef

SWIG_UNIT(Unit);
SWIG_UNIT(TemperatureUnit);
SWIG_UNIT(CelsiusUnit);
SWIG_UNIT(FahrenheitUnit);
SWIG_UNIT(SIUnit);
SWIG_UNIT(IPUnit);
SWIG_UNIT(BTUUnit);
SWIG_UNIT(CFMUnit);
SWIG_UNIT(GPDUnit);
SWIG_UNIT(MPHUnit);
SWIG_UNIT(WhUnit);
SWIG_UNIT(ThermUnit);
SWIG_UNIT(Misc1Unit);

// wrap operators
%extend openstudio::Unit {
  std::string __str__() const{
    std::stringstream ss;
    ss << *self;
    return ss.str();
  }
  openstudio::Unit operator*(const openstudio::Unit& rUnit) {
    return *self * rUnit;
  }
  openstudio::Unit operator/(const openstudio::Unit& rUnit) {
    return *self / rUnit;
  }
};

#endif // UTILITIES_UNITS_UNIT_I
