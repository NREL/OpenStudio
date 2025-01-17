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
#include "../../model/ZoneHVACExhaustControl.hpp"
#include "../../model/ZoneHVACExhaustControl_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/ZoneHVAC_ExhaustControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACExhaustControl(model::ZoneHVACExhaustControl& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_ExhaustControl, modelObject);

    // Availability Schedule Name: Required Object
    Schedule availabilitySchedule_ = modelObject.availabilitySchedule();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilitySchedule_)) {
      idfObject.setString(ZoneHVAC_ExhaustControlFields::AvailabilityScheduleName, wo_->nameString());
    }

    // Zone Name: Required Object
    boost::optional<std::string> thermalZoneName;
    if (boost::optional<ThermalZone> zone = modelObject.thermalZone()) {
      if ((s = zone->name())) {
        thermalZoneName = s;

        idfObject.setString(ZoneHVAC_ExhaustControlFields::ZoneName, thermalZoneName.get());
      }
    }

    // InletNodeName
    if (auto node = modelObject.inletNode()) {
      idfObject.setString(ZoneHVAC_ExhaustControlFields::InletNodeName, node->name().get());
    }

    // OutletNodeName
    if (auto node = modelObject.outletNode()) {
      idfObject.setString(ZoneHVAC_ExhaustControlFields::OutletNodeName, node->name().get());
    }

    if (modelObject.isDesignExhaustFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate, "Autosize");
    } else {
      // Design Exhaust Flow Rate: boost::optional<double>
      if (boost::optional<double> designExhaustFlowRate_ = modelObject.designExhaustFlowRate()) {
        idfObject.setDouble(ZoneHVAC_ExhaustControlFields::DesignExhaustFlowRate, designExhaustFlowRate_.get());
      }
    }

    // Flow Control Type: Optional String
    const std::string flowControlType = modelObject.flowControlType();
    idfObject.setString(ZoneHVAC_ExhaustControlFields::FlowControlType, flowControlType);

    // Exhaust Flow Fraction Schedule Name: Optional Object
    if (boost::optional<Schedule> exhaustFlowFractionSchedule_ = modelObject.exhaustFlowFractionSchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(exhaustFlowFractionSchedule_.get())) {
        idfObject.setString(ZoneHVAC_ExhaustControlFields::ExhaustFlowFractionScheduleName, wo_->nameString());
      }
    }

    // Minimum Zone Temperature Limit Schedule Name: Optional Object
    if (boost::optional<Schedule> minimumZoneTemperatureLimitSchedule_ = modelObject.minimumZoneTemperatureLimitSchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(minimumZoneTemperatureLimitSchedule_.get())) {
        idfObject.setString(ZoneHVAC_ExhaustControlFields::MinimumZoneTemperatureLimitScheduleName, wo_->nameString());
      }
    }

    // Minimum Exhaust Flow Fraction Schedule Name: Optional Object
    if (boost::optional<Schedule> minimumExhaustFlowFractionSchedule_ = modelObject.minimumExhaustFlowFractionSchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(minimumExhaustFlowFractionSchedule_.get())) {
        idfObject.setString(ZoneHVAC_ExhaustControlFields::MinimumExhaustFlowFractionScheduleName, wo_->nameString());
      }
    }

    // Balanced Exhaust Fraction Schedule Name: Optional Object
    if (boost::optional<Schedule> balancedExhaustFractionSchedule_ = modelObject.balancedExhaustFractionSchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(balancedExhaustFractionSchedule_.get())) {
        idfObject.setString(ZoneHVAC_ExhaustControlFields::BalancedExhaustFractionScheduleName, wo_->nameString());
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
