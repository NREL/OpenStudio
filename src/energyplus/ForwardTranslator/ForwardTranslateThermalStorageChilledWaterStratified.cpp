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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalStorageChilledWaterStratified.hpp"
#include "../../model/ThermalStorageChilledWaterStratified_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/ThermalStorage_ChilledWater_Stratified_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermalStorageChilledWaterStratified(ThermalStorageChilledWaterStratified& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Schedule> schedule;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ThermalStorage_ChilledWater_Stratified, modelObject);

    // Tank Volume
    value = modelObject.tankVolume();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::TankVolume, value.get());
    }

    // Tank Height
    value = modelObject.tankHeight();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::TankHeight, value.get());
    }

    // Tank Shape
    s = modelObject.tankShape();
    if (s) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::TankShape, s.get());
    }

    // Tank Perimeter
    value = modelObject.tankPerimeter();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::TankPerimeter, value.get());
    }

    // SetpointTemperatureScheduleName
    schedule = modelObject.setpointTemperatureSchedule();
    if (schedule) {
      auto _schedule = translateAndMapModelObject(schedule.get());

      if (_schedule) {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::SetpointTemperatureScheduleName, _schedule->name().get());
      }
    }

    // DeadbandTemperatureDifference
    value = modelObject.deadbandTemperatureDifference();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::DeadbandTemperatureDifference, value.get());
    }

    // TemperatureSensorHeight
    value = modelObject.temperatureSensorHeight();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::TemperatureSensorHeight, value.get());
    }

    // Minimum Temperature Limit
    value = modelObject.minimumTemperatureLimit();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::MinimumTemperatureLimit, value.get());
    }

    // NominalCoolingCapacity
    value = modelObject.nominalCoolingCapacity();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::NominalCoolingCapacity, value.get());
    }

    // Ambient Temperature Indicator
    s = modelObject.ambientTemperatureIndicator();
    if (s) {
      if (istringEqual(s.get(), "ThermalZone")) {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureIndicator, "Zone");
      } else {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureIndicator, s.get());
      }
    }

    // Ambient Temperature Schedule Name
    schedule = modelObject.ambientTemperatureSchedule();
    if (schedule) {
      auto _schedule = translateAndMapModelObject(schedule.get());

      if (_schedule) {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureScheduleName, _schedule->name().get());
      }
    }

    // Ambient Temperature Zone Name
    if (auto zone = modelObject.ambientTemperatureThermalZone()) {
      auto _zone = translateAndMapModelObject(zone.get());

      if (_zone) {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureZoneName, _zone->name().get());
      }
    }

    // Ambient Temperature Outdoor Air Node Name
    s = modelObject.ambientTemperatureOutdoorAirNodeName();
    if (s && (!s->empty())) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName, s.get());
    } else if (istringEqual(modelObject.ambientTemperatureIndicator(), "Outdoors")) {
      auto name = modelObject.name().get() + " OA Node";
      IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
      oaNodeListIdf.setString(0, name);
      m_idfObjects.push_back(oaNodeListIdf);

      idfObject.setString(openstudio::ThermalStorage_ChilledWater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName, name);
    }

    // Uniform Skin Loss Coefficient per Unit Area to Ambient Temperature
    value = modelObject.uniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature, value.get());
    }

    // Use Side Inlet Node Name
    if (auto mo = modelObject.supplyInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::UseSideInletNodeName, node->name().get());
      }
    }

    // Use Side Outlet Node Name
    if (auto mo = modelObject.supplyOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::UseSideOutletNodeName, node->name().get());
      }
    }

    // UseSideHeatTransferEffectiveness
    value = modelObject.useSideHeatTransferEffectiveness();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::UseSideHeatTransferEffectiveness, value.get());
    }

    // UseSideAvailabilityScheduleName
    if ((schedule = modelObject.useSideAvailabilitySchedule())) {
      auto _schedule = translateAndMapModelObject(schedule.get());

      if (_schedule) {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::UseSideAvailabilityScheduleName, _schedule->name().get());
      }
    }

    // Use Side Outlet Height
    value = modelObject.useSideOutletHeight();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::UseSideOutletHeight, value.get());
    }

    // Use Side Inlet Height
    if (modelObject.isUseSideInletHeightAutocalculated()) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::UseSideInletHeight, "Autocalculate");
    } else {
      value = modelObject.useSideInletHeight();

      if (value) {
        idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::UseSideInletHeight, value.get());
      }
    }

    // Use Side Design Flow Rate
    if (modelObject.isUseSideDesignFlowRateAutosized()) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::UseSideDesignFlowRate, "Autosize");
    } else {
      value = modelObject.useSideDesignFlowRate();

      if (value) {
        idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::UseSideDesignFlowRate, value.get());
      }
    }

    // Source Side Inlet Node Name
    if (auto mo = modelObject.demandInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::SourceSideInletNodeName, node->name().get());
      }
    }

    // Source Side Outlet Node Name
    if (auto mo = modelObject.demandOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        translateAndMapModelObject(node.get());

        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletNodeName, node->name().get());
      }
    }

    // SourceSideHeatTransferEffectiveness
    value = modelObject.sourceSideHeatTransferEffectiveness();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::SourceSideHeatTransferEffectiveness, value.get());
    }

    // SourceSideAvailabilityScheduleName
    if ((schedule = modelObject.sourceSideAvailabilitySchedule())) {
      auto _schedule = translateAndMapModelObject(schedule.get());

      if (_schedule) {
        idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::SourceSideAvailabilityScheduleName, _schedule->name().get());
      }
    }

    // Source Side Outlet Height
    if (modelObject.isSourceSideOutletHeightAutocalculated()) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletHeight, "Autocalculate");
    } else {
      value = modelObject.sourceSideOutletHeight();

      if (value) {
        idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::SourceSideOutletHeight, value.get());
      }
    }

    // Source Side Inlet Height
    value = modelObject.sourceSideInletHeight();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::SourceSideInletHeight, value.get());
    }

    // Source Side Design Flow Rate
    if (modelObject.isSourceSideDesignFlowRateAutosized()) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::SourceSideDesignFlowRate, "Autosize");
    } else {
      value = modelObject.sourceSideDesignFlowRate();

      if (value) {
        idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::SourceSideDesignFlowRate, value.get());
      }
    }

    // TankRecoveryTime
    value = modelObject.tankRecoveryTime();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::TankRecoveryTime, value.get());
    }

    // Inlet Mode
    s = modelObject.inletMode();
    if (s) {
      idfObject.setString(ThermalStorage_ChilledWater_StratifiedFields::InletMode, s.get());
    }

    // Number of Nodes
    auto num_nodes = modelObject.numberofNodes();
    idfObject.setInt(ThermalStorage_ChilledWater_StratifiedFields::NumberofNodes, num_nodes);

    // Additional Destratification Conductivity
    value = modelObject.additionalDestratificationConductivity();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::AdditionalDestratificationConductivity, value.get());
    }

    // Node 1 Additional Loss Coefficient
    value = modelObject.node1AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node1AdditionalLossCoefficient, value.get());
    }

    // Node 2 Additional Loss Coefficient
    value = modelObject.node2AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node2AdditionalLossCoefficient, value.get());
    }

    // Node 3 Additional Loss Coefficient
    value = modelObject.node3AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node3AdditionalLossCoefficient, value.get());
    }

    // Node 4 Additional Loss Coefficient
    value = modelObject.node4AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node4AdditionalLossCoefficient, value.get());
    }

    // Node 5 Additional Loss Coefficient
    value = modelObject.node5AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node5AdditionalLossCoefficient, value.get());
    }

    // Node 6 Additional Loss Coefficient
    value = modelObject.node6AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node6AdditionalLossCoefficient, value.get());
    }

    // Node 7 Additional Loss Coefficient
    value = modelObject.node7AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node7AdditionalLossCoefficient, value.get());
    }

    // Node 8 Additional Loss Coefficient
    value = modelObject.node8AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node8AdditionalLossCoefficient, value.get());
    }

    // Node 9 Additional Loss Coefficient
    value = modelObject.node9AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node9AdditionalLossCoefficient, value.get());
    }

    // Node 10 Additional Loss Coefficient
    value = modelObject.node10AdditionalLossCoefficient();
    if (value) {
      idfObject.setDouble(ThermalStorage_ChilledWater_StratifiedFields::Node10AdditionalLossCoefficient, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
