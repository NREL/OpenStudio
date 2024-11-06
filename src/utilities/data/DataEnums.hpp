/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_DATAENUMS_HPP
#define UTILITIES_DATA_DATAENUMS_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"

namespace openstudio {

// clang-format off

/** \class InstallLocationType
 *  \brief EnergyPlus meter install location.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(InstallLocationType,
  ((Facility))
  ((Building))
  ((HVAC))
  ((Zone))
  ((System))
  ((Plant))
);
 *  \endcode */
OPENSTUDIO_ENUM(InstallLocationType,
  ((Facility))
  ((Building))
  ((HVAC))
  ((Zone))
  ((System))
  ((Plant))
);

/** \class FuelType
 *  \brief EnergyPlus meterable fuel types.
 *  \details Use the valueDescription() for interfacing with EnergyPlus or setting an IDD field.
 *
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(FuelType,
  ((Electricity))
  ((Gas)(NaturalGas))
  ((Gasoline))
  ((Diesel))
  ((Coal))
  ((FuelOil_1)(FuelOilNo1))
  ((FuelOil_2)(FuelOilNo2))
  ((Propane))
  ((Water))
  ((Steam))
  ((DistrictCooling))
  ((DistrictHeating))
  ((OtherFuel_1)(OtherFuel1))
  ((OtherFuel_2)(OtherFuel2))
  ((EnergyTransfer))
  ((Solar))
  ((Geothermal))
);
 *  \endcode */
OPENSTUDIO_ENUM(FuelType,
  ((Electricity))
  ((Gas)(NaturalGas))
  ((Gasoline))
  ((Diesel))
  ((Coal))
  ((FuelOil_1)(FuelOilNo1))
  ((FuelOil_2)(FuelOilNo2))
  ((Propane))
  ((Water))
  ((Steam)(DistrictHeatingSteam))
  ((DistrictCooling))
  ((DistrictHeating)(DistrictHeatingWater))
  ((OtherFuel_1)(OtherFuel1))
  ((OtherFuel_2)(OtherFuel2))
  ((EnergyTransfer))
  ((Solar))
  ((Geothermal))
);

/** \relates FuelType */
using OptionalFuelType = boost::optional<FuelType>;

/** \relates FuelType */
using FuelTypeVector = std::vector<FuelType>;

/** \class EndUseFuelType
 *  \brief Fuel types reported by OpenStudio.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(EndUseFuelType,
  ((Electricity)(Electricity))
  ((Gas)(Natural Gas))
  ((Gasoline))
  ((Diesel))
  ((Coal))
  ((FuelOil_1)(Fuel Oil No 1))
  ((FuelOil_2)(Fuel Oil No 2))
  ((Propane))
  ((OtherFuel_1)(Other Fuel 1))
  ((OtherFuel_2)(Other Fuel 2))
  ((DistrictCooling)(District Cooling))
  ((DistrictHeating)(District Heating))
  ((Water))
);
 *  \endcode */
OPENSTUDIO_ENUM(EndUseFuelType,
  ((Electricity)(Electricity))
  ((Gas)(Natural Gas))
  ((Gasoline))
  ((Diesel))
  ((Coal))
  ((FuelOil_1)(Fuel Oil No 1))
  ((FuelOil_2)(Fuel Oil No 2))
  ((Propane))
  ((OtherFuel_1)(Other Fuel 1))
  ((OtherFuel_2)(Other Fuel 2))
  ((DistrictCooling)(District Cooling))
  ((DistrictHeating)(District Heating Water))
  ((DistrictHeatingSteam)(District Heating Steam))
  ((Water))
);

/** \class EndUseCategoryType
 *  \brief End use categories reported by OpenStudio.  These correspond to the "End Uses" table in the ABUPs report.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(EndUseCategoryType,
  ((Heating)(Heating))
  ((Cooling)(Cooling))
  ((InteriorLights)(Interior Lighting))
  ((ExteriorLights)(Exterior Lighting))
  ((InteriorEquipment)(Interior Equipment))
  ((ExteriorEquipment)(Exterior Equipment))
  ((Fans)(Fans))
  ((Pumps)(Pumps))
  ((HeatRejection)(Heat Rejection))
  ((Humidifier)(Humidification))
  ((HeatRecovery)(Heat Recovery))
  ((WaterSystems)(Water Systems))
  ((Refrigeration)(Refrigeration))
  ((Generators)(Generators))
);
 *  \endcode */
OPENSTUDIO_ENUM(EndUseCategoryType,
  ((Heating)(Heating))
  ((Cooling)(Cooling))
  ((InteriorLights)(Interior Lighting))
  ((ExteriorLights)(Exterior Lighting))
  ((InteriorEquipment)(Interior Equipment))
  ((ExteriorEquipment)(Exterior Equipment))
  ((Fans)(Fans))
  ((Pumps)(Pumps))
  ((HeatRejection)(Heat Rejection))
  ((Humidifier)(Humidification))
  ((HeatRecovery)(Heat Recovery))
  ((WaterSystems)(Water Systems))
  ((Refrigeration)(Refrigeration))
  ((Generators)(Generators))
);  // DLM: this references the Cogeneration meter

/** \class EndUseType
 *  \brief End uses that can be metered by EnergyPlus.  Some EndUseTypes correspond to EndUseCategoryTypes, others
 *  do not.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(EndUseType,
  ((InteriorLights))
  ((ExteriorLights))
  ((InteriorEquipment))
  ((ExteriorEquipment))
  ((Fans))
  ((Pumps))
  ((Heating))
  ((Cooling))
  ((HeatRejection))
  ((Humidifier))
  ((HeatRecovery))
  ((WaterSystems))
  ((Cogeneration)(Generators))
  ((Refrigeration))
  ((HeatingCoils))
  ((CoolingCoils))
  ((Boilers))
  ((Baseboard))
  ((HeatRecoveryForCooling))
  ((HeatRecoveryForHeating))
);
 *  \endcode */
OPENSTUDIO_ENUM(EndUseType,
  ((InteriorLights))
  ((ExteriorLights))
  ((InteriorEquipment))
  ((ExteriorEquipment))
  ((Fans))
  ((Pumps))
  ((Heating))
  ((Cooling))
  ((HeatRejection))
  ((Humidifier))
  ((HeatRecovery))
  ((WaterSystems))
  ((Cogeneration)(Generators))
  ((Refrigeration))
  ((HeatingCoils))
  ((CoolingCoils))
  ((Boilers))
  ((Baseboard))
  ((HeatRecoveryForCooling))
  ((HeatRecoveryForHeating))
);

/** \class BuildingSector
 *  \brief Commercial or residential.
 *  \details Used to differentiate between utility tariffs, building energy standards, etc.
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(BuildingSector,
  ((Commercial)(NonResidential))
  ((Residential))
);
 *  \endcode */
OPENSTUDIO_ENUM(BuildingSector,
  ((Commercial)(NonResidential))
  ((Residential))
);




/** \class AppGFuelType
 *  \brief heat for coil for Appendix G system type selections
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(AppGFuelType,
  ((District))
  ((Electric))
  ((Fuel))
  ((HeatPump))
  ((Other))
  ((Solar))
);
 *  \endcode */
OPENSTUDIO_ENUM(AppGFuelType,
  ((District))
  ((Electric))
  ((Fuel))
  ((Geothermal))
  ((HeatPump))
  ((Other))
  ((Solar))
);

/** \relates AppGFuelType */
using OptionalAppGFuelType = boost::optional<AppGFuelType>;

/** \relates AppGFuelType */
using AppGFuelTypeVector = std::vector<AppGFuelType>;

/** \class ComponentType
 *  \brief What an HVACComponent is meant to do: heating, cooling, both or none
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
 OPENSTUDIO_ENUM(ComponentType,
  ((Heating))
  ((Cooling))
  ((Both))
  ((None))
);
 *  \endcode */
OPENSTUDIO_ENUM(ComponentType,
  ((Heating))
  ((Cooling))
  ((Both))
  ((None))
);

/** \relates ComponentType */
using OptionalComponentType = boost::optional<ComponentType>;

/** \relates ComponentType */
using ComponentTypeVector = std::vector<ComponentType>;


OPENSTUDIO_ENUM(AlfalfaComponentCapability,
  ((Input))
  ((Output))
  ((Bidirectional))
)
/** \relates AlfalfaComponentCapability */
using OptionalAlfalfaComponentCapability = boost::optional<AlfalfaComponentCapability>;

/** \relates AlfalfaComponentCapability */
using AlfalfaComponentCapabilityVector = std::vector<AlfalfaComponentCapability>;


OPENSTUDIO_ENUM(AlfalfaComponentType,
  ((Actuator))
  ((Constant))
  ((Meter))
  ((OutputVariable))
  ((GlobalVariable))
)
/** \relates AlfalfaComponentType */
using OptionalAlfalfaComponentType = boost::optional<AlfalfaComponentType>;

/** \relates AlfalfaComponentType */
using AlfalfaComponentTypeVector = std::vector<AlfalfaComponentType>;

// clang-format on

inline UTILITIES_API AppGFuelType convertFuelTypeToAppG(FuelType fuelType) {

  if (fuelType == FuelType::Electricity) {
    return AppGFuelType::Electric;
  } else if ((fuelType == FuelType::Gas) || (fuelType == FuelType::Gasoline) || (fuelType == FuelType::Diesel) || (fuelType == FuelType::Coal)
             || (fuelType == FuelType::FuelOil_1) || (fuelType == FuelType::FuelOil_2) || (fuelType == FuelType::Propane)
             || (fuelType == FuelType::OtherFuel_1) || (fuelType == FuelType::OtherFuel_2)) {
    return AppGFuelType::Fuel;
  } else if ((fuelType == FuelType::DistrictCooling) || (fuelType == FuelType::DistrictHeating) || (fuelType == FuelType::Steam)) {
    return AppGFuelType::District;
  }

  // (fuelType == FuelType::Water)
  // (fuelType == FuelType::EnergyTransfer)

  return AppGFuelType::Other;
}

}  // namespace openstudio

#endif  // UTILITIES_DATA_DATAENUMS_HPP
