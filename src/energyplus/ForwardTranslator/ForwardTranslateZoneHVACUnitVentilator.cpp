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
#include "../../model/ZoneHVACUnitVentilator.hpp"
#include "../../model/ZoneHVACUnitVentilator_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_UnitVentilator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACUnitVentilator(ZoneHVACUnitVentilator& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // Model Name
    std::string const baseName = modelObject.name().get();
    // Node Names
    std::string const mixedAirNodeName = baseName + " Mixed Air Node";
    std::string const fanOutletNodeName = baseName + " Fan Outlet Node";
    std::string const coolingCoilOutletNodeName = baseName + " Cooling Coil Outlet Node";
    std::string const reliefAirNodeName = baseName + " Relief Air Node";
    std::string const exhaustAirNodeName = baseName + " Exhaust Air Node";
    std::string const oaNodeName = baseName + " OA Node";

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_UnitVentilator, modelObject);

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // MaximumSupplyAirFlowRate
    if (modelObject.isMaximumSupplyAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate, "AutoSize");
    } else if ((value = modelObject.maximumSupplyAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate, value.get());
    }

    // OutdoorAirControlType
    if ((s = modelObject.outdoorAirControlType())) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::OutdoorAirControlType, s.get());
    }

    // MinimumOutdoorAirFlowRate
    if (modelObject.isMinimumOutdoorAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate, "AutoSize");
    } else if ((value = modelObject.minimumOutdoorAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate, value.get());
    }

    // MinimumOutdoorAirScheduleName
    {
      auto schedule = modelObject.minimumOutdoorAirSchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirScheduleName, _schedule->name().get());
      }
    }

    // MaximumOutdoorAirFlowRate
    if (modelObject.isMaximumOutdoorAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate, "AutoSize");
    } else if ((value = modelObject.maximumOutdoorAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate, value.get());
    }

    // MaximumOutdoorAirFractionorTemperatureScheduleName
    {
      auto schedule = modelObject.maximumOutdoorAirFractionorTemperatureSchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFractionorTemperatureScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName
    auto airInletNode = modelObject.inletNode();
    if (airInletNode) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::AirInletNodeName, airInletNode->name().get());
    }

    // AirOutletNodeName
    auto airOutletNode = modelObject.outletNode();
    if (airOutletNode) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::AirOutletNodeName, airOutletNode->name().get());
    }

    // OutdoorAirNodeName
    // ExhaustAirNodeName
    // MixedAirNodeName
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::MixedAirNodeName, mixedAirNodeName);
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::OutdoorAirNodeName, oaNodeName);
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::ExhaustAirNodeName, exhaustAirNodeName);
    IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
    _oaNodeList.setString(0, oaNodeName);
    m_idfObjects.push_back(_oaNodeList);

    // Supply Air Fan
    {
      auto supplyAirFan = modelObject.supplyAirFan();
      if (auto _supplyAirFan = translateAndMapModelObject(supplyAirFan)) {
        // SupplyAirFanObjectType
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanObjectType, _supplyAirFan->iddObject().name());

        // SupplyAirFanName
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanName, _supplyAirFan->name().get());
        // Supply Air Fan Inlet and Outlet Nodes
        if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName, mixedAirNodeName);
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName, fanOutletNodeName);
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff) {

          _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName, mixedAirNodeName);
          _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName, fanOutletNodeName);
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_VariableVolume) {
          _supplyAirFan->setString(Fan_VariableVolumeFields::AirInletNodeName, mixedAirNodeName);
          _supplyAirFan->setString(Fan_VariableVolumeFields::AirOutletNodeName, fanOutletNodeName);
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_SystemModel) {

          _supplyAirFan->setString(Fan_SystemModelFields::AirInletNodeName, mixedAirNodeName);
          _supplyAirFan->setString(Fan_SystemModelFields::AirOutletNodeName, fanOutletNodeName);
        }
      }
    }

    // CoilOption
    auto heatingCoil = modelObject.heatingCoil();
    auto coolingCoil = modelObject.coolingCoil();

    // Cooling Coil
    {
      if (coolingCoil && airOutletNode) {
        if (auto _coolingCoil = translateAndMapModelObject(coolingCoil.get())) {
          // CoolingCoilObjectType
          idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoolingCoilObjectType, _coolingCoil->iddObject().name());
          // CoolingCoilName
          idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoolingCoilName, _coolingCoil->name().get());
          // Cooling Coil Inlet and Outlet Nodes
          auto coolingCoilOutletName = heatingCoil ? coolingCoilOutletNodeName : airOutletNode->name().get();

          if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water) {
            _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName, fanOutletNodeName);
            _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName, coolingCoilOutletName);
          }
        }
      }
    }

    // Heating Coil
    {
      if (heatingCoil && airOutletNode) {
        if (auto _heatingCoil = translateAndMapModelObject(heatingCoil.get())) {
          // HeatingCoilObjectType
          idfObject.setString(ZoneHVAC_UnitVentilatorFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
          // HeatingCoilName
          idfObject.setString(ZoneHVAC_UnitVentilatorFields::HeatingCoilName, _heatingCoil->name().get());
          // Heating Coil Inlet and Outlet Nodes
          auto heatingCoilOutletName = coolingCoil ? coolingCoilOutletNodeName : fanOutletNodeName;

          if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
            _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, heatingCoilOutletName);
            _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, airOutletNode->name().get());
          } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
            _heatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, heatingCoilOutletName);
            _heatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, airOutletNode->name().get());
          } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
            _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, heatingCoilOutletName);
            _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, airOutletNode->name().get());
          }
        }
      }
    }

    if (heatingCoil && coolingCoil) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption, "HeatingAndCooling");
    } else if (heatingCoil) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption, "Heating");
    } else if (coolingCoil) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption, "Cooling");
    } else {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption, "None");
    }

    // SupplyAirFanOperatingModeScheduleName
    if ((temp = modelObject.supplyAirFanOperatingModeSchedule())) {
      if (auto _schedule = translateAndMapModelObject(temp.get())) {
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanOperatingModeScheduleName, _schedule->name().get());
      }
    }

    // HeatingConvergenceTolerance
    if ((value = modelObject.heatingConvergenceTolerance())) {
      idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::HeatingConvergenceTolerance, value.get());
    }

    // CoolingConvergenceTolerance
    if ((value = modelObject.coolingConvergenceTolerance())) {
      idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::CoolingConvergenceTolerance, value.get());
    }

    // AvailabilityManagerListName
    // DesignSpecificationZoneHVACSizingObjectName

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
