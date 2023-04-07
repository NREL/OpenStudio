/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_LiIonNMCBattery_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateElectricLoadCenterStorageLiIonNMCBattery(model::ElectricLoadCenterStorageLiIonNMCBattery& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Storage_LiIonNMCBattery, modelObject);

    // Name
    if (modelObject.name()) {
      idfObject.setString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::Name, modelObject.name().get());
    }

    // Availability Schedule Name
    Schedule schedule = modelObject.availabilitySchedule();
    boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule);
    if (_schedule) {
      idfObject.setString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName, _schedule->name().get());
    }

    // Zone Name
    if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
      idfObject.setString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName, modelObject.thermalZone().get().name().get());
    }

    // Radiative Fraction
    {
      double value = modelObject.radiativeFraction();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFraction, value);
    }

    // Lifetime Model
    {
      std::string value = modelObject.lifetimeModel();
      idfObject.setString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::LifetimeModel, value);
    }

    // Number of Cells in Series
    {
      int value = modelObject.numberofCellsinSeries();
      idfObject.setInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofCellsinSeries, value);
    }

    // Number of Strings in Parallel
    {
      int value = modelObject.numberofStringsinParallel();
      idfObject.setInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofStringsinParallel, value);
    }

    // Initial Fractional State of Charge
    {
      double value = modelObject.initialFractionalStateofCharge();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialFractionalStateofCharge, value);
    }

    // DC to DC Charging Efficiency
    {
      double value = modelObject.dctoDCChargingEfficiency();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DCtoDCChargingEfficiency, value);
    }

    // Battery Mass
    {
      double value = modelObject.batteryMass();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryMass, value);
    }

    // Battery Surface Area
    {
      double value = modelObject.batterySurfaceArea();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySurfaceArea, value);
    }

    // Battery Specific Heat Capacity
    {
      double value = modelObject.batterySpecificHeatCapacity();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySpecificHeatCapacity, value);
    }

    // Heat Transfer Coefficient Between Battery and Ambient
    {
      double value = modelObject.heatTransferCoefficientBetweenBatteryandAmbient();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::HeatTransferCoefficientBetweenBatteryandAmbient, value);
    }

    // Fully Charged Cell Voltage
    {
      double value = modelObject.fullyChargedCellVoltage();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellVoltage, value);
    }

    // Cell Voltage at End of Exponential Zone
    {
      double value = modelObject.cellVoltageatEndofExponentialZone();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofExponentialZone, value);
    }

    // Cell Voltage at End of Nominal Zone
    {
      double value = modelObject.cellVoltageatEndofNominalZone();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofNominalZone, value);
    }

    // Default Nominal Cell Voltage
    {
      double value = modelObject.defaultNominalCellVoltage();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DefaultNominalCellVoltage, value);
    }

    // Fully Charged Cell Capacity
    {
      double value = modelObject.fullyChargedCellCapacity();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellCapacity, value);
    }

    // Fraction of Cell Capacity Removed at the End of Exponential Zone
    {
      double value = modelObject.fractionofCellCapacityRemovedattheEndofExponentialZone();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofExponentialZone, value);
    }

    // Fraction of Cell Capacity Removed at the End of Nominal Zone
    {
      double value = modelObject.fractionofCellCapacityRemovedattheEndofNominalZone();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofNominalZone, value);
    }

    // Charge Rate at Which Voltage vs Capacity Cruve Was Generated
    {
      double value = modelObject.chargeRateatWhichVoltagevsCapacityCurveWasGenerated();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ChargeRateatWhichVoltagevsCapacityCurveWasGenerated, value);
    }

    // Battery Cell Internal Electrical Resistance
    {
      double value = modelObject.batteryCellInternalElectricalResistance();
      idfObject.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryCellInternalElectricalResistance, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
