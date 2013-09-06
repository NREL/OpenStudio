/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_DATA_DATAENUMS_HPP
#define UTILITIES_DATA_DATAENUMS_HPP

#include "../UtilitiesAPI.hpp"
#include <utilities/core/Enum.hpp>

namespace openstudio {

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
  ((Plant)));
 *  \endcode */
OPENSTUDIO_ENUM(InstallLocationType,
  ((Facility))
  ((Building))
  ((HVAC))
  ((Zone))
  ((System))
  ((Plant)));

/** \class FuelType
 *  \brief EnergyPlus meterable fuel types
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(FuelType,
  ((Electricity))
  ((Gas)(NaturalGas))
  ((Gasoline))
  ((Diesel))
  ((FuelOil_1))
  ((FuelOil_2))
  ((Propane))
  ((Water))
  ((Steam))
  ((DistrictCooling))
  ((DistrictHeating))
  ((EnergyTransfer)));
 *  \endcode */
OPENSTUDIO_ENUM(FuelType,
  ((Electricity))
  ((Gas)(NaturalGas))
  ((Gasoline))
  ((Diesel))
  ((FuelOil_1))
  ((FuelOil_2))
  ((Propane))
  ((Water))
  ((Steam))
  ((DistrictCooling))
  ((DistrictHeating))
  ((EnergyTransfer)));

/** \relates FuelType */
typedef boost::optional<FuelType> OptionalFuelType;

/** \relates FuelType */
typedef std::vector<FuelType> FuelTypeVector;

/** \class EndUseFuelType
 *  \brief Fuel types reported by OpenStudio.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(EndUseFuelType,
  ((Electricity)(Electricity))
  ((Gas)(Natural Gas))
  ((OtherFuel)(Other Fuel))
  ((DistrictCooling)(District Cooling))
  ((DistrictHeating)(District Heating))
  ((Water)(Water)));
 *  \endcode */
OPENSTUDIO_ENUM(EndUseFuelType,
  ((Electricity)(Electricity))
  ((Gas)(Natural Gas))
  ((AdditionalFuel)(Additional Fuel))
  ((DistrictCooling)(District Cooling))
  ((DistrictHeating)(District Heating))
  ((Water)(Water)));

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
  ((Generators)(Generators)));
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
  ((Generators)(Generators)));  // DLM: this references the Cogeneration meter

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
  ((HeatRecoveryForHeating)));
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
  ((HeatRecoveryForHeating)));

/** \class BuildingSector
 *  \brief Commercial or residential. 
 *  \details Used to differentiate between utility tariffs, building energy standards, etc.
 *  See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro call is: 
 *  \code
OPENSTUDIO_ENUM(BuildingSector,
  ((Commercial)(NonResidential))
  ((Residential)));
 *  \endcode */
OPENSTUDIO_ENUM(BuildingSector,
  ((Commercial)(NonResidential))
  ((Residential)));

} // openstudio

#endif // UTILITIES_DATA_DATAENUMS_HPP
