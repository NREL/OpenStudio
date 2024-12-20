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

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/SystemAvailabilityManagerLists.hpp"
#include "../../model/SystemAvailabilityManagerLists_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/Fans.hpp"
#include "../../model/Fans_Impl.hpp"

#include "../../model/EvapCooler.hpp"
#include "../../model/EvapCooler_Impl.hpp"

#include "../../model/DesignSpecificationZoneHVACSizingName.hpp"
#include "../../model/DesignSpecificationZoneHVACSizingName_Impl.hpp"

#include <utilities/idd/ZoneHVAC_EvaporativeCoolerUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEvaporativeCoolerUnit( model::ZoneHVACEvaporativeCoolerUnit& modelObject ) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EvaporativeCoolerUnit, modelObject);

    // Availability Schedule Name: Optional Object
    if (boost::optional<Schedule> availabilitySchedule_ = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilitySchedule_.get()))  {
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName, wo_->nameString());
      }
    }

    // Availability Manager List Name: Optional Object
    if (boost::optional<SystemAvailabilityManagerLists> availabilityManagerList_ = modelObject.availabilityManagerList()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilityManagerList_.get()))  {
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityManagerListName, wo_->nameString());
      }
    }

    // Outdoor Air Inlet Node Name: Required Node
    Node outdoorAirInletNodeName = modelObject.inletNode();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(outdoorAirInletNodeName))  {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName, wo_->nameString());
    }

    // Cooler Outlet Node Name: Required Node
    Node coolerOutletNodeName = modelObject.outletNode();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolerOutletNodeName))  {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName, wo_->nameString());
    }

    // Zone Relief Air Node Name: Optional Node
    Node zoneReliefAirNodeName = modelObject.zoneReliefAirNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(zoneReliefAirNodeName))  {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName, wo_->nameString());
    }

    // Supply Air Fan Object Type: Required String
    const std::string supplyAirFanObjectType = modelObject.supplyAirFanObjectType();
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanObjectType, supplyAirFanObjectType);


    // Supply Air Fan Name: Required Object
    Fans supplyAirFan = modelObject.supplyAirFan();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(supplyAirFan))  {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName, wo_->nameString());
    }

    if (modelObject.isDesignSupplyAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, "Autosize");
    } else {
      // Design Supply Air Flow Rate: boost::optional<double>
      if (boost::optional<double> designSupplyAirFlowRate_ = modelObject.designSupplyAirFlowRate()) {
        idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, designSupplyAirFlowRate_.get());
      }
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
    idfObject.setDouble(ZoneHVAC_EvaporativeCoolerUnitFields::CoolingLoadControlThresholdHeatTransferRate, coolingLoadControlThresholdHeatTransferRate);


    // First Evaporative Cooler Object Type: Required String
    const std::string firstEvaporativeCoolerObjectType = modelObject.firstEvaporativeCoolerObjectType();
    idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectType, firstEvaporativeCoolerObjectType);


    // First Evaporative Cooler Object Name: Required Object
    EvapCooler firstEvaporativeCoolerObject = modelObject.firstEvaporativeCoolerObject();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(firstEvaporativeCoolerObject))  {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCoolerObjectName, wo_->nameString());
    }

    // Second Evaporative Cooler Object Type: boost::optional<std::string>
    if (boost::optional<std::string> secondEvaporativeCoolerObjectType_ = modelObject.secondEvaporativeCoolerObjectType()) {
      idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerObjectType, secondEvaporativeCoolerObjectType_.get());
    }

    // Second Evaporative Cooler Name: Optional Object
    if (boost::optional<EvapCooler> secondEvaporativeCooler_ = modelObject.secondEvaporativeCooler()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(secondEvaporativeCooler_.get()))  {
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName, wo_->nameString());
      }
    }

    // Design Specification ZoneHVAC Sizing Object Name: Optional Object
    if (boost::optional<DesignSpecificationZoneHVACSizingName> designSpecificationZoneHVACSizingObject_ = modelObject.designSpecificationZoneHVACSizingObject()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(designSpecificationZoneHVACSizingObject_.get()))  {
        idfObject.setString(ZoneHVAC_EvaporativeCoolerUnitFields::DesignSpecificationZoneHVACSizingObjectName, wo_->nameString());
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
