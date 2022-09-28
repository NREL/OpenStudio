/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ReverseTranslator.hpp"

#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_LiIonNMCBattery_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateElectricLoadCenterStorageLiIonNMCBattery(const WorkspaceObject& workspaceObject) {

    OptionalDouble d;
    boost::optional<WorkspaceObject> owo;
    OptionalString optS;
    OptionalInt i;

    // Number of Cells in Series
    OptionalInt nSeries;
    nSeries = workspaceObject.getInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofCellsinSeries);
    if (!nSeries) {
      LOG(Error, workspaceObject.briefDescription() << " is missing required field 'Number of Cells in Series'. It will not be translated.");
      return boost::none;
    }

    // Number of Strings in Parallel
    OptionalInt nParallel = workspaceObject.getInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofStringsinParallel);
    if (!nParallel) {
      LOG(Error, workspaceObject.briefDescription() << " is missing required field 'Number of Strings in Parallel'. It will not be translated.");
      return boost::none;
    }

    // Battery Mass
    OptionalDouble mass = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryMass);
    if (!mass) {
      LOG(Error, workspaceObject.briefDescription() << " is missing required field 'Battery Mass'. It will not be translated.");
      return boost::none;
    }

    // Battery Surface Area
    OptionalDouble surfaceArea = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySurfaceArea);
    if (!surfaceArea) {
      LOG(Error, workspaceObject.briefDescription() << " is missing required field 'Battery Surface Area'. It will not be translated.");
      return boost::none;
    }

    openstudio::model::ElectricLoadCenterStorageLiIonNMCBattery elcStorLiIonNMCBattery(m_model, *nSeries, *nParallel, *mass, *surfaceArea);

    // Name
    optS = workspaceObject.name();
    if (optS) {
      elcStorLiIonNMCBattery.setName(*optS);
    }

    // AvailabilityScheduleName
    if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName))) {
      if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get())) {
        if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
          elcStorLiIonNMCBattery.setAvailabilitySchedule(schedule.get());
        }
      }
    }

    // ZoneName
    if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName))) {
      if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get())) {
        // Zone is translated, and a Space is returned instead
        if (boost::optional<Space> space_ = mo->optionalCast<Space>()) {
          if (auto z_ = space_->thermalZone()) {
            elcStorLiIonNMCBattery.setThermalZone(z_.get());
          }
        }
      }
    }

    // Radiative Fraction
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFraction);
    if (d) {
      elcStorLiIonNMCBattery.setRadiativeFraction(*d);
    }

    // Lifetime Model
    optS = workspaceObject.getString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::LifetimeModel);
    if (optS) {
      elcStorLiIonNMCBattery.setLifetimeModel(*optS);
    }

    // Initial Fractional State of Charge
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialFractionalStateofCharge);
    if (d) {
      elcStorLiIonNMCBattery.setInitialFractionalStateofCharge(*d);
    }

    // DC to DC Charging Efficiency
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DCtoDCChargingEfficiency);
    if (d) {
      elcStorLiIonNMCBattery.setDCtoDCChargingEfficiency(*d);
    }

    // Battery Specific Heat Capacity
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySpecificHeatCapacity);
    if (d) {
      elcStorLiIonNMCBattery.setBatterySpecificHeatCapacity(*d);
    }

    // Heat Transfer Coefficient Between Battery and Ambient
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::HeatTransferCoefficientBetweenBatteryandAmbient);
    if (d) {
      elcStorLiIonNMCBattery.setHeatTransferCoefficientBetweenBatteryandAmbient(*d);
    }

    // Fully Charged Cell Voltage
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellVoltage);
    if (d) {
      elcStorLiIonNMCBattery.setFullyChargedCellVoltage(*d);
    }

    // Cell Voltage at End of Exponential Zone
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofExponentialZone);
    if (d) {
      elcStorLiIonNMCBattery.setCellVoltageatEndofExponentialZone(*d);
    }

    // Cell Voltage at End of Nominal Zone
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofNominalZone);
    if (d) {
      elcStorLiIonNMCBattery.setCellVoltageatEndofNominalZone(*d);
    }

    // Default Nominal Cell Voltage
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DefaultNominalCellVoltage);
    if (d) {
      elcStorLiIonNMCBattery.setDefaultNominalCellVoltage(*d);
    }

    // Fully Charged Cell Capacity
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellCapacity);
    if (d) {
      elcStorLiIonNMCBattery.setFullyChargedCellCapacity(*d);
    }

    // Fraction of Cell Capacity Removed at the End of Exponential Zone
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofExponentialZone);
    if (d) {
      elcStorLiIonNMCBattery.setFractionofCellCapacityRemovedattheEndofExponentialZone(*d);
    }

    // Fraction of Cell Capacity Removed at the End of Nominal Zone
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofNominalZone);
    if (d) {
      elcStorLiIonNMCBattery.setFractionofCellCapacityRemovedattheEndofNominalZone(*d);
    }

    // Charge Rate at Which Voltage vs Capacity Curve Was Generated
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ChargeRateatWhichVoltagevsCapacityCurveWasGenerated);
    if (d) {
      elcStorLiIonNMCBattery.setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(*d);
    }

    // Battery Cell Internal Electrical Resistance
    d = workspaceObject.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryCellInternalElectricalResistance);
    if (d) {
      elcStorLiIonNMCBattery.setBatteryCellInternalElectricalResistance(*d);
    }

    return elcStorLiIonNMCBattery;
  }

}  // namespace energyplus

}  // namespace openstudio
