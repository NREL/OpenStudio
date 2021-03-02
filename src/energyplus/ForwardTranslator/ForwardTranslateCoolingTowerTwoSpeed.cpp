/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/CoolingTowerTwoSpeed.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/CoolingTower_TwoSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoolingTowerTwoSpeed(CoolingTowerTwoSpeed& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalInt i;
    OptionalModelObject temp;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::CoolingTower_TwoSpeed, modelObject);

    // Water Inlet Node Name
    temp = modelObject.inletModelObject();
    if (temp) {
      idfObject.setString(CoolingTower_TwoSpeedFields::WaterInletNodeName, temp->name().get());
    }

    // Water Outlet Node Name
    temp = modelObject.outletModelObject();
    if (temp) {
      idfObject.setString(CoolingTower_TwoSpeedFields::WaterOutletNodeName, temp->name().get());
    }

    // Design Water Flow Rate
    if ((d = modelObject.designWaterFlowRate())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::DesignWaterFlowRate, d.get());
    } else if (modelObject.isDesignWaterFlowRateAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::DesignWaterFlowRate, "Autosize");
    }

    // High Fan Speed Air Flow Rate
    if ((d = modelObject.highFanSpeedAirFlowRate())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::HighFanSpeedAirFlowRate, d.get());
    } else if (modelObject.isHighFanSpeedAirFlowRateAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::HighFanSpeedAirFlowRate, "Autosize");
    }

    // High Fan Speed Fan Power
    if ((d = modelObject.highFanSpeedFanPower())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::HighFanSpeedFanPower, d.get());
    } else if (modelObject.isHighFanSpeedFanPowerAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::HighFanSpeedFanPower, "Autosize");
    }

    // High Fan Speed U-Factor Times Area Value
    if ((d = modelObject.highFanSpeedUFactorTimesAreaValue())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue, d.get());
    } else if (modelObject.isHighFanSpeedUFactorTimesAreaValueAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::HighFanSpeedUFactorTimesAreaValue, "Autosize");
    }

    // Low Fan Speed Air Flow Rate
    if ((d = modelObject.lowFanSpeedAirFlowRate())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRate, d.get());
    } else if (modelObject.isLowFanSpeedAirFlowRateAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRate, "Autocalculate");
    }

    // Low Fan Speed Air Flow Rate Sizing Factor
    if ((d = modelObject.lowFanSpeedAirFlowRateSizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, d.get());
    }

    // Low Fan Speed Fan Power
    if ((d = modelObject.lowFanSpeedFanPower())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowFanSpeedFanPower, d.get());
    } else if (modelObject.isLowFanSpeedFanPowerAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::LowFanSpeedFanPower, "Autocalculate");
    }

    // Low Fan Speed Fan Power Sizing Factor
    if ((d = modelObject.lowFanSpeedFanPowerSizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, d.get());
    }

    // Low Fan Speed U-Factor Times Area Value
    if ((d = modelObject.lowFanSpeedUFactorTimesAreaValue())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue, d.get());
    } else if (modelObject.isLowFanSpeedUFactorTimesAreaValueAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaValue, "Autocalculate");
    }

    // Low Fan Speed U-Factor Times Area Sizing Factor
    if ((d = modelObject.lowFanSpeedUFactorTimesAreaSizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, d.get());
    }

    // Free Convection Regime Air Flow Rate
    if ((d = modelObject.freeConvectionRegimeAirFlowRate())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate, d.get());
    } else if (modelObject.isFreeConvectionRegimeAirFlowRateAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRate, "Autocalculate");
    }

    // Free Convection Regime Air Flow Rate Sizing Factor
    if ((d = modelObject.freeConvectionRegimeAirFlowRateSizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::FreeConvectionRegimeAirFlowRateSizingFactor, d.get());
    }

    // Free Convection Regime U-Factor Times Area Value
    if ((d = modelObject.freeConvectionRegimeUFactorTimesAreaValue())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue, d.get());
    } else if (modelObject.isFreeConvectionRegimeUFactorTimesAreaValueAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::FreeConvectionRegimeUFactorTimesAreaValue, "Autocalculate");
    }

    // Free Convection U-Factor Times Area Value Sizing Factor
    if ((d = modelObject.freeConvectionUFactorTimesAreaValueSizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::FreeConvectionUFactorTimesAreaValueSizingFactor, d.get());
    }

    // Performance Input Method
    if ((s = modelObject.performanceInputMethod())) {
      idfObject.setString(CoolingTower_TwoSpeedFields::PerformanceInputMethod, s.get());
    }

    // Heat Rejection Capacity and Nominal Capacity Sizing Ratio
    if ((d = modelObject.heatRejectionCapacityandNominalCapacitySizingRatio())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio, d.get());
    }

    // High Speed Nominal Capacity
    if ((d = modelObject.highSpeedNominalCapacity())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::HighSpeedNominalCapacity, d.get());
    }

    // Low Speed Nominal Capacity
    if ((d = modelObject.lowSpeedNominalCapacity())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity, d.get());
    } else if (modelObject.isLowSpeedNominalCapacityAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::LowSpeedNominalCapacity, "Autocalculate");
    }

    // Low Speed Nominal Capacity Sizing Factor
    if ((d = modelObject.lowSpeedNominalCapacitySizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::LowSpeedNominalCapacitySizingFactor, d.get());
    }

    // Free Convection Nominal Capacity
    if ((d = modelObject.freeConvectionNominalCapacity())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity, d.get());
    } else if (modelObject.isFreeConvectionNominalCapacityAutosized()) {
      idfObject.setString(CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacity, "Autocalculate");
    }

    // Free Convection Nominal Capacity Sizing Factor
    if ((d = modelObject.freeConvectionNominalCapacitySizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::FreeConvectionNominalCapacitySizingFactor, d.get());
    }

    // Basin Heater Capacity
    if ((d = modelObject.basinHeaterCapacity())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::BasinHeaterCapacity, d.get());
    }

    // Basin Heater Setpoint Temperature
    if ((d = modelObject.basinHeaterSetpointTemperature())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::BasinHeaterSetpointTemperature, d.get());
    }

    // Basin Heater Operating Schedule Name
    if ((temp = modelObject.basinHeaterOperatingSchedule())) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get())) {
        idfObject.setString(CoolingTower_TwoSpeedFields::BasinHeaterOperatingScheduleName, _schedule->name().get());
      }
    }

    // Evaporation Loss Mode
    if ((s = modelObject.evaporationLossMode())) {
      idfObject.setString(CoolingTower_TwoSpeedFields::EvaporationLossMode, s.get());
    }

    // Evaporation Loss Factor
    if ((d = modelObject.evaporationLossFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::EvaporationLossFactor, d.get());
    }

    // Drift Loss Percent
    if ((d = modelObject.driftLossPercent())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::DriftLossPercent, d.get());
    }

    // Blowdown Calculation Mode
    if ((s = modelObject.blowdownCalculationMode())) {
      idfObject.setString(CoolingTower_TwoSpeedFields::BlowdownCalculationMode, s.get());
    }

    // Blowdown Concentration Ratio
    if ((d = modelObject.blowdownConcentrationRatio())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::BlowdownConcentrationRatio, d.get());
    }

    // Blowdown Makeup Water Usage Schedule Name
    if ((temp = modelObject.blowdownMakeupWaterUsageSchedule())) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get())) {
        idfObject.setString(CoolingTower_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName, _schedule->name().get());
      }
    }

    // Supply Water Storage Tank Name
    // not used currently

    // Outdoor Air Inlet Node Name
    // not used currently

    // Number of Cells
    if ((i = modelObject.numberofCells())) {
      idfObject.setInt(CoolingTower_TwoSpeedFields::NumberofCells, i.get());
    }

    // Cell Control
    if ((s = modelObject.cellControl())) {
      idfObject.setString(CoolingTower_TwoSpeedFields::CellControl, s.get());
    }

    // Cell Minimum  Water Flow Rate Fractio
    if ((d = modelObject.cellMinimumWaterFlowRateFraction())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::CellMinimumWaterFlowRateFraction, d.get());
    }

    // Cell Maximum Water Flow Rate Fraction
    if ((d = modelObject.cellMaximumWaterFlowRateFraction())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::CellMaximumWaterFlowRateFraction, d.get());
    }

    // Sizing Factor
    if ((d = modelObject.sizingFactor())) {
      idfObject.setDouble(CoolingTower_TwoSpeedFields::SizingFactor, d.get());
    }

    if ((d = modelObject.designInletAirDryBulbTemperature())) {
      idfObject.setDouble(openstudio::CoolingTower_TwoSpeedFields::DesignInletAirDryBulbTemperature, d.get());
    }

    if ((d = modelObject.designInletAirWetBulbTemperature())) {
      idfObject.setDouble(openstudio::CoolingTower_TwoSpeedFields::DesignInletAirWetBulbTemperature, d.get());
    }

    if (modelObject.isDesignApproachTemperatureAutosized()) {
      idfObject.setString(openstudio::CoolingTower_TwoSpeedFields::DesignApproachTemperature, "Autosize");
    } else if ((d = modelObject.designApproachTemperature())) {
      idfObject.setDouble(openstudio::CoolingTower_TwoSpeedFields::DesignApproachTemperature, d.get());
    }

    if (modelObject.isDesignRangeTemperatureAutosized()) {
      idfObject.setString(openstudio::CoolingTower_TwoSpeedFields::DesignRangeTemperature, "Autosize");
    } else if ((d = modelObject.designApproachTemperature())) {
      idfObject.setDouble(openstudio::CoolingTower_TwoSpeedFields::DesignRangeTemperature, d.get());
    }

    if ((s = modelObject.endUseSubcategory())) {
      idfObject.setString(openstudio::CoolingTower_TwoSpeedFields::EndUseSubcategory, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
