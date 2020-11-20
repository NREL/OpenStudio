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
#include "../../model/Model.hpp"
#include "../../model/EvaporativeFluidCoolerTwoSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerTwoSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeFluidCooler_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEvaporativeFluidCoolerTwoSpeed(EvaporativeFluidCoolerTwoSpeed& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    //Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::EvaporativeFluidCooler_TwoSpeed, modelObject);

    // WaterInletNodeName
    temp = modelObject.inletModelObject();
    if (temp) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::WaterInletNodeName, temp->name().get());
    }

    // WaterOutletNodeName
    temp = modelObject.outletModelObject();
    if (temp) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::WaterOutletNodeName, temp->name().get());
    }

    // HighFanSpeedAirFlowRate
    if (modelObject.isHighFanSpeedAirFlowRateAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, "Autosize");
    } else if ((d = modelObject.highFanSpeedAirFlowRate())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedAirFlowRate, d.get());
    }

    // HighFanSpeedFanPower
    if (modelObject.isHighFanSpeedFanPowerAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower, "Autosize");
    } else if ((d = modelObject.highFanSpeedFanPower())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedFanPower, d.get());
    }

    // LowFanSpeedAirFlowRate
    if (modelObject.isLowFanSpeedAirFlowRateAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, "Autocalculate");
    } else if ((d = modelObject.lowFanSpeedAirFlowRate())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRate, d.get());
    }

    // LowFanSpeedAirFlowRateSizingFactor
    if ((d = modelObject.lowFanSpeedAirFlowRateSizingFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedAirFlowRateSizingFactor, d.get());
    }

    // LowFanSpeedFanPower
    if (modelObject.isLowFanSpeedFanPowerAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower, "Autocalculate");
    } else if ((d = modelObject.lowFanSpeedFanPower())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPower, d.get());
    }

    // LowFanSpeedFanPowerSizingFactor
    if ((d = modelObject.lowFanSpeedFanPowerSizingFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedFanPowerSizingFactor, d.get());
    }

    // DesignSprayWaterFlowRate
    if ((d = modelObject.designSprayWaterFlowRate())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignSprayWaterFlowRate, d.get());
    }

    // PerformanceInputMethod
    if ((s = modelObject.performanceInputMethod())) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::PerformanceInputMethod, s.get());
    }

    // OutdoorAirInletNodeName
    idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::OutdoorAirInletNodeName, "");

    // HeatRejectionCapacityandNominalCapacitySizingRatio
    if ((d = modelObject.heatRejectionCapacityandNominalCapacitySizingRatio())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HeatRejectionCapacityandNominalCapacitySizingRatio, d.get());
    }

    // HighSpeedStandardDesignCapacity
    if ((d = modelObject.highSpeedStandardDesignCapacity())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighSpeedStandardDesignCapacity, d.get());
    }

    // LowSpeedStandardDesignCapacity
    if (modelObject.isLowSpeedStandardDesignCapacityAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity, "Autosize");
    } else if ((d = modelObject.lowSpeedStandardDesignCapacity())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardDesignCapacity, d.get());
    }

    // LowSpeedStandardCapacitySizingFactor
    if ((d = modelObject.lowSpeedStandardCapacitySizingFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedStandardCapacitySizingFactor, d.get());
    }

    // HighFanSpeedUfactorTimesAreaValue
    if (modelObject.isHighFanSpeedUfactorTimesAreaValueAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, "Autosize");
    } else if ((d = modelObject.highFanSpeedUfactorTimesAreaValue())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighFanSpeedUfactorTimesAreaValue, d.get());
    }

    // LowFanSpeedUfactorTimesAreaValue
    if (modelObject.isLowFanSpeedUfactorTimesAreaValueAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, "Autocalculate");
    } else if ((d = modelObject.lowFanSpeedUfactorTimesAreaValue())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUfactorTimesAreaValue, d.get());
    }

    // LowFanSpeedUFactorTimesAreaSizingFactor
    if ((d = modelObject.lowFanSpeedUFactorTimesAreaSizingFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowFanSpeedUFactorTimesAreaSizingFactor, d.get());
    }

    // DesignWaterFlowRate
    if (modelObject.isDesignWaterFlowRateAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate, "Autosize");
    } else if ((d = modelObject.designWaterFlowRate())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignWaterFlowRate, d.get());
    }

    // HighSpeedUserSpecifiedDesignCapacity
    if ((d = modelObject.highSpeedUserSpecifiedDesignCapacity())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighSpeedUserSpecifiedDesignCapacity, d.get());
    }

    // LowSpeedUserSpecifiedDesignCapacity
    if (modelObject.isLowSpeedUserSpecifiedDesignCapacityAutosized()) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity, "Autocalculate");
    } else if ((d = modelObject.lowSpeedUserSpecifiedDesignCapacity())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacity, d.get());
    }

    // LowSpeedUserSpecifiedDesignCapacitySizingFactor
    if ((d = modelObject.lowSpeedUserSpecifiedDesignCapacitySizingFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::LowSpeedUserSpecifiedDesignCapacitySizingFactor, d.get());
    }

    // DesignEnteringWaterTemperature
    if ((d = modelObject.designEnteringWaterTemperature())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringWaterTemperature, d.get());
    }

    // DesignEnteringAirTemperature
    if ((d = modelObject.designEnteringAirTemperature())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirTemperature, d.get());
    }

    // DesignEnteringAirWetbulbTemperature
    if ((d = modelObject.designEnteringAirWetbulbTemperature())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DesignEnteringAirWetbulbTemperature, d.get());
    }

    // HighSpeedSizingFactor
    if ((d = modelObject.highSpeedSizingFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::HighSpeedSizingFactor, d.get());
    }

    // EvaporationLossMode
    if ((s = modelObject.evaporationLossMode())) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossMode, s.get());
    }

    // EvaporationLossFactor
    if ((d = modelObject.evaporationLossFactor())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::EvaporationLossFactor, d.get());
    }

    // DriftLossPercent
    if ((d = modelObject.driftLossPercent())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::DriftLossPercent, d.get());
    }

    // BlowdownCalculationMode
    if ((s = modelObject.blowdownCalculationMode())) {
      idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::BlowdownCalculationMode, s.get());
    }

    // BlowdownConcentrationRatio
    if ((d = modelObject.blowdownConcentrationRatio())) {
      idfObject.setDouble(openstudio::EvaporativeFluidCooler_TwoSpeedFields::BlowdownConcentrationRatio, d.get());
    }

    // BlowdownMakeupWaterUsageScheduleName
    if ((temp = modelObject.blowdownMakeupWaterUsageSchedule())) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get())) {
        idfObject.setString(openstudio::EvaporativeFluidCooler_TwoSpeedFields::BlowdownMakeupWaterUsageScheduleName, _schedule->name().get());
      }
    }

    // SupplyWaterStorageTankName

    return idfObject;
  }
}  // namespace energyplus

}  // namespace openstudio
