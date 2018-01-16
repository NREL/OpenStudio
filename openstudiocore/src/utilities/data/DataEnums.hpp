/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_DATA_DATAENUMS_HPP
#define UTILITIES_DATA_DATAENUMS_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"

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
