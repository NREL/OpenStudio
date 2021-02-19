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
#include "../../model/CoilChillerAirSourceVariableSpeed.hpp"
#include "../../model/CoilChillerAirSourceVariableSpeedSpeedData.hpp"
#include "../../model/Node.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/Coil_Chiller_AirSource_VariableSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilChillerAirSourceVariableSpeed(CoilChillerAirSourceVariableSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    if (modelObject.speeds().size() == 0) {
      // TODO: if it has zero speed data, Log and don't translate
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Chiller_AirSource_VariableSpeed, modelObject);

    // Nominal Speed Level
    if (auto speedLevel = modelObject.nominalSpeedLevel()) {
      idfObject.setInt(Coil_Chiller_AirSource_VariableSpeedFields::NominalSpeedLevel, speedLevel);
    }

    // Rated Chilled Water Capacity
    if (modelObject.isRatedChilledWaterCapacityAutosized()) {
      idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, "autosize");
    } else if ((value = modelObject.ratedChilledWaterCapacity())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::RatedChilledWaterCapacity, value.get());
    }

    // Rated Evaporator Inlet Water Temperature
    if ((value = modelObject.ratedEvaporatorInletWaterTemperature())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorInletWaterTemperature, value.get());
    }

    // Rated Condenser Inlet Air Temperature
    if ((value = modelObject.ratedCondenserInletAirTemperature())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::RatedCondenserInletAirTemperature, value.get());
    }

    // Rated Evaporator Water Flow Rate
    if (modelObject.isRatedEvaporatorWaterFlowRateAutocalculated()) {
      idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, "autocalculate");
    } else if ((value = modelObject.ratedEvaporatorWaterFlowRate())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::RatedEvaporatorWaterFlowRate, value.get());
    }

    // Evaporator Pump Power Included in Rated COP
    if ((s = modelObject.evaporatorPumpPowerIncludedinRatedCOP())) {
      idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpPowerIncludedinRatedCOP, s.get());
    }

    // Evaporator Pump Heat Included in Rated Cooling Capacity and Rated COP
    if ((s = modelObject.evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP())) {
      idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP, s.get());
    }

    // Fraction of Evaporator Pump Heat to Water
    if ((value = modelObject.fractionofEvaporatorPumpHeattoWater())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::FractionofEvaporatorPumpHeattoWater, value.get());
    }

    // Evaporator Water Inlet Node Name
    /*     if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterInletNodeName, node->name().get());
    } */
    idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterInletNodeName, "ChillerInletNode");

    // Evaporator Water Outlet Node Name
    /*     if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterOutletNodeName, node->name().get());
    } */
    idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::EvaporatorWaterOutletNodeName, "ChillerWaterOutletNode");

    // Crankcase Heater Capacity
    if ((value = modelObject.crankcaseHeaterCapacity())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::CrankcaseHeaterCapacity, value.get());
    }

    // Maximum Ambient Temperature for Crankcase Heater Operation
    if ((value = modelObject.maximumAmbientTemperatureforCrankcaseHeaterOperation())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, value.get());
    }

    // Part Load Fraction Correlation Curve Name
    if (boost::optional<model::Curve> curve = modelObject.partLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    // Grid Signal Schedule Name
    if (auto schedule = modelObject.gridSignalSchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Chiller_AirSource_VariableSpeedFields::GridSignalScheduleName, _schedule->name().get());
      }
    }

    // Lower Bound to Apply Grid Responsive Control
    if ((value = modelObject.lowerBoundToApplyGridResponsiveControl())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::LowerBoundtoApplyGridResponsiveControl, value.get());
    }

    // Upper Bound to Apply Grid Responsive Control
    if ((value = modelObject.upperBoundToApplyGridResponsiveControl())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::UpperBoundtoApplyGridResponsiveControl, value.get());
    }

    // Max Speed Level During Grid-Responsive Control
    if ((value = modelObject.maxSpeedLevelDuringGridResponsiveControl())) {
      idfObject.setDouble(Coil_Chiller_AirSource_VariableSpeedFields::MaxSpeedLevelDuringGridResponsiveControl, value.get());
    }

    auto const speeds = modelObject.speeds();

    // Number of Speeds
    if (auto num = speeds.size()) {
      idfObject.setInt(Coil_Chiller_AirSource_VariableSpeedFields::NumberofSpeeds, num);
    }

    for (auto const& speed : speeds) {
      auto eg = idfObject.pushExtensibleGroup();

      // Rated Water Cooling Capacity at Speed
      if ((value = speed.ratedWaterCoolingCapacity())) {
        eg.setDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::RatedWaterCoolingCapacityatSpeed, value.get());
      }

      // Rated Water Cooling COP at Speed
      if ((value = speed.ratedWaterCoolingCOP())) {
        eg.setDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::RatedWaterCoolingCOPatSpeed, value.get());
      }

      // Speed Reference Unit Rated Air Flow Rate
      if ((value = speed.referenceUnitRatedAirFlowRate())) {
        eg.setDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate, value.get());
      }

      // Speed Reference Unit Rated Water Flow Rate
      if ((value = speed.referenceUnitRatedWaterFlowRate())) {
        eg.setDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate, value.get());
      }

      // Speed Reference Unit Water Pump Input Power At Rated Conditions
      if ((value = speed.referenceUnitWaterPumpInputPowerAtRatedConditions())) {
        eg.setDouble(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedReferenceUnitWaterPumpInputPowerAtRatedConditions, value.get());
      }

      // Speed Total Cooling Capacity Function of Temperature Curve Name
      {
        auto curve = speed.totalCoolingCapacityFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName,
                       _curve->name().get());
        }
      }

      // Speed Total Cooling Capacity Function of Water Flow Fraction Curve Name
      {
        auto curve = speed.totalCoolingCapacityFunctionofWaterFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofWaterFlowFractionCurveName,
                       _curve->name().get());
        }
      }

      // Speed EIR Function of Temperature Curve Name
      {
        auto curve = speed.energyInputRatioFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedEIRFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // Speed EIR Function of Water Flow Fraction Curve Name
      {
        auto curve = speed.energyInputRatioFunctionofWaterFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Chiller_AirSource_VariableSpeedExtensibleFields::SpeedEIRFunctionofWaterFlowFractionCurveName, _curve->name().get());
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
