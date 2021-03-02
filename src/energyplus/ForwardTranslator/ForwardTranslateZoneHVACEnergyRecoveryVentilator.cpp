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
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilator.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilator_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EnergyRecoveryVentilator_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EnergyRecoveryVentilator_Controller_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEnergyRecoveryVentilator(ZoneHVACEnergyRecoveryVentilator& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // Model Name
    std::string const baseName = modelObject.name().get();
    // Node Names
    std::string const oaNodeName = baseName + " OA Node";
    std::string const supplyFanInletNodeName = baseName + " Supply Fan Inlet Node";
    std::string const exhaustFanInletNodeName = baseName + " Exhaust Fan Inlet Node";
    std::string const exhaustFanOutletNodeName = baseName + " Exhaust Fan Outlet Node";

    auto const zoneExhaustNode = modelObject.inletNode();
    auto const zoneInletNode = modelObject.outletNode();

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EnergyRecoveryVentilator, modelObject);

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // Heat Exchanger
    {
      auto heatExchanger = modelObject.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

      if (heatExchanger.supplyAirOutletTemperatureControl()) {
        LOG(Warn, "Overriding supplyAirOutletTemperatureControl to \"No\" for "
                    << heatExchanger.briefDescription() << " because the HX needs to control itself, not through Setpoint Manager.");
        heatExchanger.setSupplyAirOutletTemperatureControl(false);
      }

      if (auto _heatExchanger = translateAndMapModelObject(heatExchanger)) {
        // HeatExchangerName
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::HeatExchangerName, _heatExchanger->name().get());
        // Heat Exchanger Nodes
        if (_heatExchanger->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent) {
          _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, oaNodeName);
          _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName, supplyFanInletNodeName);
          _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName, zoneExhaustNode->name().get());
          _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, exhaustFanInletNodeName);

          IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
          _oaNodeList.setString(0, oaNodeName);
          m_idfObjects.push_back(_oaNodeList);
        }
      }
    }

    // SupplyAirFlowRate
    if (modelObject.isSupplyAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate, "AutoSize");
    } else if ((value = modelObject.supplyAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate, value.get());
    }

    // ExhaustAirFlowRate
    if (modelObject.isExhaustAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate, "AutoSize");
    } else if ((value = modelObject.exhaustAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate, value.get());
    }

    // Supply Air Fan
    {
      auto supplyAirFan = modelObject.supplyAirFan();
      if (auto _supplyAirFan = translateAndMapModelObject(supplyAirFan)) {
        // SupplyAirFanName
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFanName, _supplyAirFan->name().get());
        // Supply Air Fan Inlet and Outlet Nodes
        if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff) {
          _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName, supplyFanInletNodeName);
          _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName, zoneInletNode->name().get());
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_SystemModel) {
          _supplyAirFan->setString(Fan_SystemModelFields::AirInletNodeName, supplyFanInletNodeName);
          _supplyAirFan->setString(Fan_SystemModelFields::AirOutletNodeName, zoneInletNode->name().get());
        } else {
          LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of fan: " << supplyAirFan.briefDescription());
        }
      }
    }

    // Exhaust Air Fan
    {
      auto exhaustAirFan = modelObject.exhaustAirFan();
      if (auto _exhaustAirFan = translateAndMapModelObject(exhaustAirFan)) {
        // ExhaustAirFanName
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFanName, _exhaustAirFan->name().get());
        // Exhaust Air Fan Inlet and Outlet Nodes
        if (_exhaustAirFan->iddObject().type() == IddObjectType::Fan_OnOff) {
          _exhaustAirFan->setString(Fan_OnOffFields::AirInletNodeName, exhaustFanInletNodeName);
          _exhaustAirFan->setString(Fan_OnOffFields::AirOutletNodeName, exhaustFanOutletNodeName);
        } else if (_exhaustAirFan->iddObject().type() == IddObjectType::Fan_SystemModel) {
          _exhaustAirFan->setString(Fan_SystemModelFields::AirInletNodeName, exhaustFanInletNodeName);
          _exhaustAirFan->setString(Fan_SystemModelFields::AirOutletNodeName, exhaustFanOutletNodeName);
        } else {
          LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of fan: " << exhaustAirFan.briefDescription());
        }
      }
    }

    // ControllerName
    if (auto controller = modelObject.controller()) {
      if (auto _controller = translateAndMapModelObject(controller.get())) {
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ControllerName, _controller->name().get());

        // Need to set Humidistat Control Thermal Zone in Controller
        if (controller->highHumidityControlFlag()) {
          if (auto thermalZone = modelObject.thermalZone()) {
            if (auto _thermalZone = translateAndMapModelObject(thermalZone.get())) {
              _controller->setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HumidistatControlZoneName, _thermalZone->name().get());
            }
          }
        }
      }
    }

    // VentilationRateperUnitFloorArea
    if ((value = modelObject.ventilationRateperUnitFloorArea())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperUnitFloorArea, value.get());
    }

    // VentilationRateperOccupant
    if ((value = modelObject.ventilationRateperOccupant())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperOccupant, value.get());
    }

    // AvailabilityManagerListName

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
