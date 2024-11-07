#ifndef UTILITIES_DATA_DATA_I
#define UTILITIES_DATA_DATA_I

%{
  #include <boost/function.hpp>
  #include <utilities/data/DataEnums.hpp>
  #include <utilities/data/Variant.hpp>

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
%}

%template(InstallLocationTypeVector) std::vector<openstudio::InstallLocationType>;
%template(OptionalInstallLocationType) boost::optional<openstudio::InstallLocationType>;
%template(FuelTypeVector) std::vector<openstudio::FuelType>;
%template(OptionalFuelType) boost::optional<openstudio::FuelType>;
%template(EndUseFuelTypeVector) std::vector<openstudio::EndUseFuelType>;
%template(OptionalEndUseFuelType) boost::optional<openstudio::EndUseFuelType>;
%template(EndUseCategoryTypeVector) std::vector<openstudio::EndUseCategoryType>;
%template(OptionalEndUseCategoryType) boost::optional<openstudio::EndUseCategoryType>;
%template(EndUseTypeVector) std::vector<openstudio::EndUseType>;
%template(OptionalEndUseType) boost::optional<openstudio::EndUseType>;
%template(BuildingSectorVector) std::vector<openstudio::BuildingSector>;
%template(OptionalBuildingSector) boost::optional<openstudio::BuildingSector>;
%template(AppGFuelTypeVector) std::vector<openstudio::AppGFuelType>;
%template(OptionalAppGFuelType) boost::optional<openstudio::AppGFuelType>;
%template(ComponentTypeVector) std::vector<openstudio::ComponentType>;
%template(OptionalComponentType) boost::optional<openstudio::ComponentType>;
%template(AlfalfaComponentTypeVector) std::vector<openstudio::AlfalfaComponentType>;
%template(OptionalAlfalfaComponentType) boost::optional<openstudio::AlfalfaComponentType>;
%template(AlfalfaComponentCapabilityVector) std::vector<openstudio::AlfalfaComponentCapability>;
%template(OptionalAlfalfaComponentCapability) boost::optional<openstudio::AlfalfaComponentCapability>;

%include <utilities/data/DataEnums.hpp>
%include <utilities/data/Attribute.i>
%include <utilities/data/Variant.i>
%include <utilities/data/CalibrationResult.i>
%include <utilities/data/EndUses.i>
%include <utilities/data/Vector.i>
%include <utilities/data/Matrix.i>
%include <utilities/data/TimeSeries.i>

#endif //UTILITIES_DATA_DATA_I
