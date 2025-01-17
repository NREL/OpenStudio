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
#include "../../model/ZoneHVACEvaporativeCoolerUnit.hpp"
#include "../../model/ZoneHVACEvaporativeCoolerUnit_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/ZoneHVAC_EvaporativeCoolerUnit_FieldEnums.hxx>
#include <utilities/idd/Fan_ComponentModel_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/EvaporativeCooler_Indirect_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEvaporativeCoolerUnit(model::ZoneHVACEvaporativeCoolerUnit& modelObject) {

    boost::optional<IdfObject> i_firstEvaporativeCooler_;
    {
      HVACComponent firstEvaporativeCooler = modelObject.firstEvaporativeCooler();
      i_firstEvaporativeCooler_ = translateAndMapModelObject(firstEvaporativeCooler);
      if (!i_firstEvaporativeCooler_) {
        LOG(Error, "ZoneHVACEvaporativeCoolerUnit '" << modelObject.nameString() << "', could not translate required First Evaporative Cooler:"
                                                     << firstEvaporativeCooler.briefDescription());
        return boost::none;
      }
    }

    boost::optional<IdfObject> i_fan_;
    {
      HVACComponent supplyAirFan = modelObject.supplyAirFan();
      i_fan_ = translateAndMapModelObject(supplyAirFan);
      if (!i_fan_) {
        LOG(Error, "ZoneHVACEvaporativeCoolerUnit '" << modelObject.nameString()
                                                     << "', could not translate required Supply Air Fan:" << supplyAirFan.briefDescription());
        return boost::none;
      }
    }

    // i_firstEvaporativeCooler_ and i_fan_ are always initialized now

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EvaporativeCoolerUnit, modelObject);

    // Availability Schedule Name: Required Object
    Schedule availabilitySchedule_ = modelObject.availabilitySchedule();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilitySchedule_)) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName, wo_->nameString());
    }

    const auto outdoorAirInletNodeName = modelObject.nameString() + " Outdoor Air Node";
    std::string coolerOutletNodeName;

    // Outdoor Air Inlet Node Name: Required Node - This is an Outdoor Air Node
    {
      IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
      oaNodeListIdf.setString(0, outdoorAirInletNodeName);
      m_idfObjects.emplace_back(std::move(oaNodeListIdf));
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName, outdoorAirInletNodeName);
    }

    // Technically outletNode and inletNode are always initialized, since they are connected via addToThermalZone and this FT routine is called by
    // thermalZone::equipment

    // Cooler Outlet Node Name: Required Node, a zone air inlet node
    if (boost::optional<Node> node = modelObject.outletNode()) {
      coolerOutletNodeName = node->nameString();
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName, node->nameString());
    }

    // Zone Relief Air Node Name: optional Node, filled to a Zone Exhaust Node if the flow is being balanced here and not elsewhere
    // NOTE: we always assume the airflow is balanced
    if (boost::optional<Node> node = modelObject.inletNode()) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName, node->nameString());
    }

    // Supply Air Fan Object Type
    // Supply Air Fan Name
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanObjectType, i_fan_->iddObject().name());
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName, i_fan_->nameString());

    if (modelObject.isDesignSupplyAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, "Autosize");
    } else {
      // Design Supply Air Flow Rate: boost::optional<double>
      idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, modelObject.designSupplyAirFlowRate().get());
    }

    // Fan Placement: Required String
    const std::string fanPlacement = modelObject.fanPlacement();
    const bool blowThroughFan = istringEqual(fanPlacement, "BlowThrough");
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FanPlacement, fanPlacement);

    // Cooler Unit Control Method: Required String
    const std::string coolerUnitControlMethod = modelObject.coolerUnitControlMethod();
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerUnitControlMethod, coolerUnitControlMethod);

    // Throttling Range Temperature Difference: Optional Double
    const double throttlingRangeTemperatureDifference = modelObject.throttlingRangeTemperatureDifference();
    idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ThrottlingRangeTemperatureDifference, throttlingRangeTemperatureDifference);

    // Cooling Load Control Threshold Heat Transfer Rate: Optional Double
    const double coolingLoadControlThresholdHeatTransferRate = modelObject.coolingLoadControlThresholdHeatTransferRate();
    idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::CoolingLoadControlThresholdHeatTransferRate,
                        coolingLoadControlThresholdHeatTransferRate);

    // First Evaporative Cooler Object Type: Required String
    // First Evaporative Cooler Object Name: Required Object
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectType, i_firstEvaporativeCooler_->iddObject().name());
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectName, i_firstEvaporativeCooler_->nameString());

    // Second Evaporative Cooler Object Type: boost::optional<std::string>
    // Second Evaporative Cooler Name: Optional Object
    boost::optional<IdfObject> i_secondEvaporativeCooler_;
    if (boost::optional<HVACComponent> secondEvaporativeCooler_ = modelObject.secondEvaporativeCooler()) {
      i_secondEvaporativeCooler_ = translateAndMapModelObject(*secondEvaporativeCooler_);
      if (i_secondEvaporativeCooler_) {
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerObjectType, i_secondEvaporativeCooler_->iddObject().name());
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName, i_secondEvaporativeCooler_->nameString());
      }
    }

    // If BlowThrough:   o---- Fan ---- E1 (---- E2) ----o
    // If DrawThrough:   o---- E1 (---- E2) ---- Fan ----o
    std::string baseName = modelObject.nameString();

    // if (i_fan_) is always true
    {
      std::string inletNodeName;
      std::string outletNodeName;
      if (blowThroughFan) {
        inletNodeName = outdoorAirInletNodeName;
        outletNodeName = baseName + " Fan - First Evaporative Cooler Node";
      } else {
        if (i_secondEvaporativeCooler_) {
          inletNodeName = baseName + " Second Evaporative Cooler - Fan Node";
        } else {
          inletNodeName = baseName + " First Evaporative Cooler - Fan Node";
        }
        outletNodeName = coolerOutletNodeName;
      }

      if (i_fan_->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        i_fan_->setString(Fan_ConstantVolumeFields::AirInletNodeName, inletNodeName);
        i_fan_->setString(Fan_ConstantVolumeFields::AirOutletNodeName, outletNodeName);
      } else if (i_fan_->iddObject().type() == IddObjectType::Fan_VariableVolume) {
        i_fan_->setString(Fan_VariableVolumeFields::AirInletNodeName, inletNodeName);
        i_fan_->setString(Fan_VariableVolumeFields::AirOutletNodeName, outletNodeName);
      } else if (i_fan_->iddObject().type() == IddObjectType::Fan_OnOff) {
        i_fan_->setString(Fan_OnOffFields::AirInletNodeName, inletNodeName);
        i_fan_->setString(Fan_OnOffFields::AirOutletNodeName, outletNodeName);
      } else if (i_fan_->iddObject().type() == IddObjectType::Fan_SystemModel) {
        i_fan_->setString(Fan_SystemModelFields::AirInletNodeName, inletNodeName);
        i_fan_->setString(Fan_SystemModelFields::AirOutletNodeName, outletNodeName);
      } else if (i_fan_->iddObject().type() == IddObjectType::Fan_ComponentModel) {
        i_fan_->setString(Fan_ComponentModelFields::AirInletNodeName, inletNodeName);
        i_fan_->setString(Fan_ComponentModelFields::AirOutletNodeName, outletNodeName);
      }
    }

    // if (i_firstEvaporativeCooler_) is always true
    {
      std::string inletNodeName;
      std::string outletNodeName;
      if (blowThroughFan) {
        inletNodeName = baseName + " Fan - First Evaporative Cooler Node";
        if (i_secondEvaporativeCooler_) {
          outletNodeName = baseName + " First Evaporative Cooler - Second Evaporative Cooler Node";
        } else {
          outletNodeName = coolerOutletNodeName;
        }
      } else {
        inletNodeName = outdoorAirInletNodeName;
        if (i_secondEvaporativeCooler_) {
          outletNodeName = baseName + " First Evaporative Cooler - Second Evaporative Cooler Node";
        } else {
          outletNodeName = baseName + " First Evaporative Cooler - Fan Node";
        }
      }

      if (i_firstEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Direct_ResearchSpecial) {
        i_firstEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName, inletNodeName);
        i_firstEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName, outletNodeName);
        i_firstEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName, coolerOutletNodeName);
      } else if (i_firstEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial) {
        i_firstEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNodeName, inletNodeName);
        i_firstEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNodeName, outletNodeName);
        i_firstEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SensorNodeName, coolerOutletNodeName);
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << i_firstEvaporativeCooler_->iddObject().type() << ".");
      }
    }

    if (i_secondEvaporativeCooler_) {
      const std::string inletNodeName = baseName + " First Evaporative Cooler - Second Evaporative Cooler Node";
      std::string outletNodeName;
      if (blowThroughFan) {
        outletNodeName = coolerOutletNodeName;
      } else {
        outletNodeName = baseName + " Second Evaporative Cooler - Fan Node";
      }

      if (i_secondEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Direct_ResearchSpecial) {
        i_secondEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName, inletNodeName);
        i_secondEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName, outletNodeName);
        i_secondEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName, coolerOutletNodeName);
      } else if (i_secondEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial) {
        i_secondEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNodeName, inletNodeName);
        i_secondEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNodeName, outletNodeName);
        i_secondEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SensorNodeName, coolerOutletNodeName);
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << i_secondEvaporativeCooler_->iddObject().type() << ".");
      }
    }

    // Shut Off Relative Humidity: boost::optional<double>
    idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ShutOffRelativeHumidity, modelObject.shutOffRelativeHumidity());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
