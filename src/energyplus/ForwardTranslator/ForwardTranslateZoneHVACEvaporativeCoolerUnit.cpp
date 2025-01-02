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

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EvaporativeCoolerUnit, modelObject);

    // Availability Schedule Name: Required Object
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(modelObject.availabilitySchedule())) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName, wo_->nameString());
    }

    boost::optional<std::string> outdoorAirInletNodeName;
    boost::optional<std::string> coolerOutletNodeName;

    // Outdoor Air Inlet Node Name: Required Node
    if (boost::optional<Node> node = modelObject.inletNode()) {
      outdoorAirInletNodeName = node->name().get();
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName, node->name().get());
    }

    // Cooler Outlet Node Name: Required Node
    if (boost::optional<Node> node = modelObject.outletNode()) {
      coolerOutletNodeName = node->name().get();
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName, node->name().get());
    }

    // Supply Air Fan Object Type
    // Supply Air Fan Name
    boost::optional<IdfObject> fan_ = translateAndMapModelObject(modelObject.supplyAirFan().get());

    if (fan_ && fan_->name()) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanObjectType, fan_->iddObject().name());
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName, fan_->name().get());
    }

    if (modelObject.isDesignSupplyAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, "Autosize");
    } else {
      // Design Supply Air Flow Rate: boost::optional<double>
      idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, modelObject.designSupplyAirFlowRate().get());
    }

    // Fan Placement: Required String
    const std::string fanPlacement = modelObject.fanPlacement();
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
    HVACComponent firstEvaporativeCooler = modelObject.firstEvaporativeCooler();
    boost::optional<IdfObject> firstEvaporativeCooler_ = translateAndMapModelObject(firstEvaporativeCooler);
    if (firstEvaporativeCooler_) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectType, firstEvaporativeCooler_->iddObject().name());
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectName, firstEvaporativeCooler_->nameString());
    }

    // Second Evaporative Cooler Object Type: boost::optional<std::string>
    // Second Evaporative Cooler Name: Optional Object
    boost::optional<HVACComponent> _secondEvaporativeCooler = modelObject.secondEvaporativeCooler();
    boost::optional<IdfObject> secondEvaporativeCooler_;
    if (_secondEvaporativeCooler) {
      secondEvaporativeCooler_ = translateAndMapModelObject(_secondEvaporativeCooler.get());
      if (secondEvaporativeCooler_) {
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerObjectType, secondEvaporativeCooler_->iddObject().name());
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName, secondEvaporativeCooler_->nameString());
      }
    }

    // If BlowThrough:   o---- Fan ---- E1 ---- E2 ----o
    // If DrawThrough:   o---- E1 ---- E2 ---- Fan ----o
    std::string baseName = modelObject.name().get();
    if (fan_) {
      std::string outletNodeName;
      std::string inletNodeName = outdoorAirInletNodeName.get();
      if (istringEqual(fanPlacement, "BlowThrough")) {
        if (firstEvaporativeCooler_) {
          outletNodeName = baseName + " Fan - First Evaporative Cooler Node";
        } else if (secondEvaporativeCooler_) {
          outletNodeName = baseName + " Fan - Second Evaporative Cooler Node";
        } else {
          outletNodeName = coolerOutletNodeName.get();
        }
      } else {
        if (secondEvaporativeCooler_) {
          inletNodeName = baseName + " Second Evaporative Cooler - Fan Node";
        } else {
          inletNodeName = baseName + " First Evaporative Cooler - Fan Node";
        }
        outletNodeName = coolerOutletNodeName.get();
      }

      if (fan_->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        fan_->setString(Fan_ConstantVolumeFields::AirInletNodeName, inletNodeName);
        fan_->setString(Fan_ConstantVolumeFields::AirOutletNodeName, outletNodeName);
      } else if (fan_->iddObject().type() == IddObjectType::Fan_VariableVolume) {
        fan_->setString(Fan_VariableVolumeFields::AirInletNodeName, inletNodeName);
        fan_->setString(Fan_VariableVolumeFields::AirOutletNodeName, outletNodeName);
      } else if (fan_->iddObject().type() == IddObjectType::Fan_OnOff) {
        fan_->setString(Fan_OnOffFields::AirInletNodeName, inletNodeName);
        fan_->setString(Fan_OnOffFields::AirOutletNodeName, outletNodeName);
      } else if (fan_->iddObject().type() == IddObjectType::Fan_SystemModel) {
        fan_->setString(Fan_SystemModelFields::AirInletNodeName, inletNodeName);
        fan_->setString(Fan_SystemModelFields::AirOutletNodeName, outletNodeName);
      } else if (fan_->iddObject().type() == IddObjectType::Fan_ComponentModel) {
        fan_->setString(Fan_ComponentModelFields::AirInletNodeName, inletNodeName);
        fan_->setString(Fan_ComponentModelFields::AirOutletNodeName, outletNodeName);
      }
    }

    if (firstEvaporativeCooler_) {
      std::string outletNodeName;
      std::string inletNodeName;
      if (istringEqual(fanPlacement, "BlowThrough") && fan_) {
        inletNodeName = baseName + " Fan - First Evaporative Cooler Node";
      } else {
        inletNodeName = outdoorAirInletNodeName.get();
      }

      if (secondEvaporativeCooler_) {
        outletNodeName = baseName + " First Evaporative Cooler - Second Evaporative Cooler Node";
      } else if (istringEqual(fanPlacement, "BlowThrough")) {
        outletNodeName = coolerOutletNodeName.get();
      } else {
        outletNodeName = baseName + " First Evaporative Cooler - Fan Node";
      }

      if (firstEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Direct_ResearchSpecial) {
        firstEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName, inletNodeName);
        firstEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName, outletNodeName);
      } else if (firstEvaporativeCooler.iddObject().type() == IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial) {
        firstEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNodeName, inletNodeName);
        firstEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNodeName, outletNodeName);
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << firstEvaporativeCooler_->iddObject().type() << ".");
      }
    }

    if (secondEvaporativeCooler_) {
      std::string outletNodeName;
      std::string inletNodeName;
      if (firstEvaporativeCooler_) {
        inletNodeName = baseName + " First Evaporative Cooler - Second Evaporative Cooler Node";
      } else if (istringEqual(fanPlacement, "BlowThrough") && fan_) {
        inletNodeName = baseName + " Fan - Second Evaporative Cooler Node";
      } else {
        inletNodeName = outdoorAirInletNodeName.get();
      }

      if (istringEqual(fanPlacement, "DrawThrough") && fan_) {
        outletNodeName = baseName + " Second Evaporative Cooler - Fan Node";
      } else {
        outletNodeName = coolerOutletNodeName.get();
      }

      if (secondEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Direct_ResearchSpecial) {
        secondEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName, inletNodeName);
        secondEvaporativeCooler_->setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName, outletNodeName);
      } else if (secondEvaporativeCooler_->iddObject().type() == IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial) {
        secondEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNodeName, inletNodeName);
        secondEvaporativeCooler_->setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNodeName, outletNodeName);
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << secondEvaporativeCooler_->iddObject().type() << ".");
      }
    }

    // Shut Off Relative Humidity: boost::optional<double>
    if (boost::optional<double> shutOffRelativeHumidity_ = modelObject.shutOffRelativeHumidity()) {
      idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::ShutOffRelativeHumidity, shutOffRelativeHumidity_.get());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
