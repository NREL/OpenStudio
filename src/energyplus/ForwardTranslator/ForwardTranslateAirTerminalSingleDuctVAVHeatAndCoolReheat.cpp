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
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAirTerminalSingleDuctVAVHeatAndCoolReheat(AirTerminalSingleDuctVAVHeatAndCoolReheat& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> value;

    std::string baseName = modelObject.name().get();

    IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit

    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat);
    idfObject.setName(baseName);

    m_idfObjects.push_back(_airDistributionUnit);
    m_idfObjects.push_back(idfObject);

    auto coil = modelObject.reheatCoil();
    auto _coil = translateAndMapModelObject(coil);

    boost::optional<std::string> inletNodeName;
    boost::optional<std::string> outletNodeName;

    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        inletNodeName = inletNode->name().get();
      }
    }

    if (boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>()) {
        outletNodeName = outletNode->name().get();
      }
    }

    std::string damperOutletNodeName = baseName + " Damper Outlet Node";

    if (outletNodeName && inletNodeName) {
      if (_coil) {
        if (_coil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
          _coil->setString(Coil_Heating_FuelFields::AirInletNodeName, damperOutletNodeName);
          _coil->setString(Coil_Heating_FuelFields::AirOutletNodeName, outletNodeName.get());
        } else if (_coil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
          _coil->setString(Coil_Heating_ElectricFields::AirInletNodeName, damperOutletNodeName);
          _coil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, outletNodeName.get());
        } else if (_coil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
          _coil->setString(Coil_Heating_WaterFields::AirInletNodeName, damperOutletNodeName);
          _coil->setString(Coil_Heating_WaterFields::AirOutletNodeName, outletNodeName.get());
        }
      }

      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirOutletNodeName, outletNodeName.get());
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirInletNodeName, inletNodeName.get());
    }

    // AvailabilityScheduleName
    if (auto schedule = modelObject.availabilitySchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // DamperAirOutletNodeName
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::DamperAirOutletNodeName, damperOutletNodeName);

    // AirInletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirInletNodeName, node->name().get());
    }

    // MaximumAirFlowRate
    if (modelObject.isMaximumAirFlowRateAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate, "AutoSize");
    } else if ((value = modelObject.maximumAirFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate, value.get());
    }

    // ZoneMinimumAirFlowFraction
    if ((value = modelObject.zoneMinimumAirFlowFraction())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ZoneMinimumAirFlowFraction, value.get());
    }

    if (_coil) {
      // ReheatCoilObjectType
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilObjectType, _coil->iddObject().name());

      // ReheatCoilName
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilName, _coil->name().get());
    }

    // MaximumHotWaterorSteamFlowRate
    if (modelObject.isMaximumHotWaterorSteamFlowRateAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate, "AutoSize");
    } else if ((value = modelObject.maximumHotWaterorSteamFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate, value.get());
    }

    // MinimumHotWaterorSteamFlowRate
    if ((value = modelObject.minimumHotWaterorSteamFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumHotWaterorSteamFlowRate, value.get());
    }

    // AirOutletNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirOutletNodeName, node->name().get());
    }

    // ConvergenceTolerance
    if ((value = modelObject.convergenceTolerance())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ConvergenceTolerance, value.get());
    }

    // MaximumReheatAirTemperature
    if ((value = modelObject.maximumReheatAirTemperature())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumReheatAirTemperature, value.get());
    }

    // Populate fields for AirDistributionUnit
    if (boost::optional<ModelObject> outletNode = modelObject.outletModelObject()) {
      _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName, outletNode->name().get());
    }
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType, idfObject.iddObject().name());
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName, idfObject.name().get());

    return _airDistributionUnit;
  }

}  // namespace energyplus

}  // namespace openstudio
