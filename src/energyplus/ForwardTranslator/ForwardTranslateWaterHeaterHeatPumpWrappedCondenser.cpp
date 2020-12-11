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
#include "../../model/WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "../../model/WaterHeaterHeatPumpWrappedCondenser_Impl.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpWrapped.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterHeater_HeatPump_WrappedCondenser_FieldEnums.hxx>
#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_Wrapped_FieldEnums.hxx>
#include <utilities/idd/WaterHeater_Mixed_FieldEnums.hxx>
#include <utilities/idd/WaterHeater_Stratified_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateWaterHeaterHeatPumpWrappedCondenser(WaterHeaterHeatPumpWrappedCondenser& modelObject) {
    IdfObject idfObject(IddObjectType::WaterHeater_HeatPump_WrappedCondenser);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    auto tank = modelObject.tank();
    auto stratifiedTank = tank.optionalCast<model::WaterHeaterStratified>();

    if (auto height = modelObject.controlSensor1HeightInStratifiedTank()) {
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1HeightInStratifiedTank, height.get());
    } else {
      if (stratifiedTank) {
        auto heater1Height = stratifiedTank->heater1Height();
        idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1HeightInStratifiedTank, heater1Height);
      }
    }

    {
      auto value = modelObject.controlSensor1Weight();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1Weight, value);
    }

    if (auto height = modelObject.controlSensor2HeightInStratifiedTank()) {
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor2HeightInStratifiedTank, height.get());
    } else {
      if (stratifiedTank) {
        auto heater2Height = stratifiedTank->heater2Height();
        idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor2HeightInStratifiedTank, heater2Height);
      }
    }

    if (auto schedule = modelObject.availabilitySchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::AvailabilityScheduleName, _schedule->nameString());
      }
    }

    {
      auto schedule = modelObject.compressorSetpointTemperatureSchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::CompressorSetpointTemperatureScheduleName, _schedule->nameString());
      }
    }

    {
      auto value = modelObject.deadBandTemperatureDifference();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::DeadBandTemperatureDifference, value);
    }

    {
      auto value = modelObject.condenserBottomLocation();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::CondenserBottomLocation, value);
    }

    {
      auto value = modelObject.condenserTopLocation();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::CondenserTopLocation, value);
    }

    {
      if (modelObject.isEvaporatorAirFlowRateAutocalculated()) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::EvaporatorAirFlowRate, "Autocalculate");
      } else if (auto value = modelObject.evaporatorAirFlowRate()) {
        idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::EvaporatorAirFlowRate, value.get());
      }
    }

    std::string airInletNodeName;
    std::string airOutletNodeName;
    std::string outdoorAirNodeName;
    std::string exhaustAirNodeName;
    std::string inletAirZoneName;
    std::string inletAirMixerNodeName;
    std::string outletAirSplitterNodeName;
    std::string fanInletNodeName;
    std::string fanOutletNodeName;
    std::string evapInletNodeName;
    std::string evapOutletNodeName;

    auto inletAirConfiguration = modelObject.inletAirConfiguration();
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::InletAirConfiguration, inletAirConfiguration);

    if (istringEqual(modelObject.fanPlacement(), "DrawThrough")) {
      idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::FanPlacement, "DrawThrough");

      if (istringEqual(inletAirConfiguration, "ZoneAirOnly")) {
        if (auto thermalZone = modelObject.thermalZone()) {
          auto inletNode = modelObject.inletNode();
          OS_ASSERT(inletNode);
          airInletNodeName = inletNode->name().get();

          auto outletNode = modelObject.outletNode();
          OS_ASSERT(outletNode);
          airOutletNodeName = outletNode->name().get();

          inletAirZoneName = thermalZone->name().get();
          fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
          fanOutletNodeName = airOutletNodeName;
          evapInletNodeName = airInletNodeName;
          evapOutletNodeName = fanInletNodeName;
        }
      } else if (istringEqual(inletAirConfiguration, "ZoneAndOutdoorAir")) {
        if (auto thermalZone = modelObject.thermalZone()) {
          auto inletNode = modelObject.inletNode();
          OS_ASSERT(inletNode);
          airInletNodeName = inletNode->name().get();

          auto outletNode = modelObject.outletNode();
          OS_ASSERT(outletNode);
          airOutletNodeName = outletNode->name().get();

          outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
          exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
          inletAirZoneName = thermalZone->name().get();
          inletAirMixerNodeName = modelObject.name().get() + " Mixer Outlet - Evap Inlet";
          outletAirSplitterNodeName = modelObject.name().get() + " Fan Outlet - Splitter Inlet";
          fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
          fanOutletNodeName = outletAirSplitterNodeName;
          evapInletNodeName = inletAirMixerNodeName;
          evapOutletNodeName = fanInletNodeName;
        }
      } else if (istringEqual(inletAirConfiguration, "OutdoorAirOnly")) {
        outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
        exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
        fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
        fanOutletNodeName = exhaustAirNodeName;
        evapInletNodeName = outdoorAirNodeName;
        evapOutletNodeName = fanInletNodeName;
      } else if (istringEqual(inletAirConfiguration, "Schedule")) {
        airInletNodeName = modelObject.name().get() + " Inlet";
        airOutletNodeName = modelObject.name().get() + " Outlet";
        fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
        fanOutletNodeName = airOutletNodeName;
        evapInletNodeName = airInletNodeName;
        evapOutletNodeName = fanInletNodeName;
      }

    } else {  // BlowThrough
      idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::FanPlacement, "BlowThrough");

      if (istringEqual(inletAirConfiguration, "ZoneAirOnly")) {
        if (auto thermalZone = modelObject.thermalZone()) {
          auto inletNode = modelObject.inletNode();
          OS_ASSERT(inletNode);
          airInletNodeName = inletNode->name().get();

          auto outletNode = modelObject.outletNode();
          OS_ASSERT(outletNode);
          airOutletNodeName = outletNode->name().get();

          inletAirZoneName = thermalZone->name().get();
          fanInletNodeName = airInletNodeName;
          fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
          evapInletNodeName = fanOutletNodeName;
          evapOutletNodeName = airOutletNodeName;
        }
      } else if (istringEqual(inletAirConfiguration, "ZoneAndOutdoorAir")) {
        if (auto thermalZone = modelObject.thermalZone()) {
          auto inletNode = modelObject.inletNode();
          OS_ASSERT(inletNode);
          airInletNodeName = inletNode->name().get();

          auto outletNode = modelObject.outletNode();
          OS_ASSERT(outletNode);
          airOutletNodeName = outletNode->name().get();

          outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
          exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
          inletAirZoneName = thermalZone->name().get();
          inletAirMixerNodeName = modelObject.name().get() + " Mixer Outlet - Fan Inlet";
          outletAirSplitterNodeName = modelObject.name().get() + " Evap Outlet - Splitter Inlet";
          fanInletNodeName = inletAirMixerNodeName;
          fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
          evapInletNodeName = fanOutletNodeName;
          evapOutletNodeName = outletAirSplitterNodeName;
        }
      } else if (istringEqual(inletAirConfiguration, "OutdoorAirOnly")) {
        outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
        exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
        fanInletNodeName = outdoorAirNodeName;
        fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
        evapInletNodeName = fanOutletNodeName;
        evapOutletNodeName = exhaustAirNodeName;
      } else if (istringEqual(inletAirConfiguration, "Schedule")) {
        airInletNodeName = modelObject.name().get() + " Inlet";
        airOutletNodeName = modelObject.name().get() + " Outlet";
        fanInletNodeName = airInletNodeName;
        fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
        evapInletNodeName = fanOutletNodeName;
        evapOutletNodeName = airOutletNodeName;
      }
    }

    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::AirInletNodeName, airInletNodeName);
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::AirOutletNodeName, airOutletNodeName);
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::OutdoorAirNodeName, outdoorAirNodeName);
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::ExhaustAirNodeName, exhaustAirNodeName);
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::InletAirZoneName, inletAirZoneName);
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::InletAirMixerNodeName, inletAirMixerNodeName);
    idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::OutletAirSplitterNodeName, outletAirSplitterNodeName);

    if (auto schedule = modelObject.inletAirTemperatureSchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::InletAirTemperatureScheduleName, _schedule->nameString());
      }
    }

    if (auto schedule = modelObject.inletAirHumiditySchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::InletAirHumidityScheduleName, _schedule->nameString());
      }
    }

    {
      auto mo = modelObject.tank();
      if (auto idf = translateAndMapModelObject(mo)) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankName, idf->name().get());
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankObjectType, idf->iddObject().name());
        if (mo.iddObjectType() == model::WaterHeaterMixed::iddObjectType()) {
          auto waterHeaterMixed = mo.cast<model::WaterHeaterMixed>();
          if (auto node = waterHeaterMixed.supplyInletModelObject()) {
            idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankUseSideInletNodeName, node->name().get());
          }
          if (auto node = waterHeaterMixed.supplyOutletModelObject()) {
            idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankUseSideOutletNodeName, node->name().get());
          }
        } else if (mo.iddObjectType() == model::WaterHeaterStratified::iddObjectType()) {
          auto waterHeaterStratified = mo.cast<model::WaterHeaterStratified>();
          if (auto node = waterHeaterStratified.supplyInletModelObject()) {
            idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankUseSideInletNodeName, node->name().get());
          }
          if (auto node = waterHeaterStratified.supplyOutletModelObject()) {
            idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankUseSideOutletNodeName, node->name().get());
          }
        } else {
          LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of tank: " << mo.briefDescription());
        }
      }
    }

    {
      auto mo = modelObject.dXCoil();
      if (auto idf = translateAndMapModelObject(mo)) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::DXCoilName, idf->name().get());
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::DXCoilObjectType, idf->iddObject().name());
        if (mo.iddObjectType() == model::CoilWaterHeatingAirToWaterHeatPumpWrapped::iddObjectType()) {
          idf->setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirInletNodeName, evapInletNodeName);
          idf->setString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirOutletNodeName, evapOutletNodeName);
        } else {
          LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of coil: " << mo.briefDescription());
        }
      }
    }

    {
      auto value = modelObject.minimumInletAirTemperatureforCompressorOperation();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::MinimumInletAirTemperatureforCompressorOperation, value);
    }

    {
      auto value = modelObject.maximumInletAirTemperatureforCompressorOperation();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::MaximumInletAirTemperatureforCompressorOperation, value);
    }

    {
      auto value = modelObject.compressorLocation();
      idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::CompressorLocation, value);
    }

    if (istringEqual("Schedule", modelObject.compressorLocation())) {
      if (auto schedule = modelObject.compressorAmbientTemperatureSchedule()) {
        if (auto _schedule = translateAndMapModelObject(schedule.get())) {
          idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::CompressorAmbientTemperatureScheduleName, _schedule->nameString());
        }
      }
    }

    {
      auto mo = modelObject.fan();
      if (auto idf = translateAndMapModelObject(mo)) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::FanName, idf->name().get());
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::FanObjectType, idf->iddObject().name());
        if (mo.iddObjectType() == IddObjectType::OS_Fan_OnOff) {
          idf->setString(Fan_OnOffFields::AirInletNodeName, fanInletNodeName);
          idf->setString(Fan_OnOffFields::AirOutletNodeName, fanOutletNodeName);
        } else if (mo.iddObjectType() == IddObjectType::OS_Fan_SystemModel) {
          idf->setString(Fan_SystemModelFields::AirInletNodeName, fanInletNodeName);
          idf->setString(Fan_SystemModelFields::AirOutletNodeName, fanOutletNodeName);
        } else {
          LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of fan: " << mo.briefDescription());
        }
      }
    }

    {
      auto value = modelObject.onCycleParasiticElectricLoad();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::OnCycleParasiticElectricLoad, value);
    }

    {
      auto value = modelObject.offCycleParasiticElectricLoad();
      idfObject.setDouble(WaterHeater_HeatPump_WrappedCondenserFields::OffCycleParasiticElectricLoad, value);
    }

    {
      auto value = modelObject.parasiticHeatRejectionLocation();
      idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::ParasiticHeatRejectionLocation, value);
    }

    if (auto schedule = modelObject.inletAirMixerSchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::InletAirMixerScheduleName, _schedule->nameString());
      }
    }

    {
      auto value = modelObject.tankElementControlLogic();
      if (istringEqual(value, "MutuallyExlcusive")) {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankElementControlLogic, "MutuallyExclusive");
      } else {
        idfObject.setString(WaterHeater_HeatPump_WrappedCondenserFields::TankElementControlLogic, value);
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
