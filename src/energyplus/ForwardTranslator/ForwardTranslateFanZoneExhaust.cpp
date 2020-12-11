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
#include "../../model/FanZoneExhaust.hpp"
#include "../../model/FanZoneExhaust_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/Fan_ZoneExhaust_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFanZoneExhaust(FanZoneExhaust& modelObject) {
    //setup boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> value;

    // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Fan_ZoneExhaust, modelObject);

    // Model Name (A2)
    //std::string baseName = modelObject.name().get();

    // A3, Field Availability Schedule Name
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        idfObject.setString(Fan_ZoneExhaustFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    // N1, Field Fan Efficiency

    value = modelObject.fanEfficiency();

    if (value) {
      idfObject.setDouble(Fan_ZoneExhaustFields::FanTotalEfficiency, *value);
    }

    // N2, Field Pressure Rise

    value = modelObject.pressureRise();

    if (value) {
      idfObject.setDouble(Fan_ZoneExhaustFields::PressureRise, *value);
    }

    // N3, Field Maximum Flow Rate

    value = modelObject.maximumFlowRate();
    if (value) {
      idfObject.setDouble(Fan_ZoneExhaustFields::MaximumFlowRate, value.get());
    }

    // A4, Field Air Inlet Node Name
    boost::optional<Node> inletNode = modelObject.inletNode();

    if (inletNode) {
      boost::optional<Node> _inletNode = modelObject.inletNode();

      if (_inletNode && _inletNode->name()) {
        idfObject.setString(Fan_ZoneExhaustFields::AirInletNodeName, _inletNode->name().get());
      }
    }

    // A5, Field Air Outlet Node Name
    boost::optional<Node> outletNode = modelObject.outletNode();

    if (outletNode) {
      boost::optional<Node> _outletNode = modelObject.outletNode();

      if (_outletNode && _outletNode->name()) {
        idfObject.setString(Fan_ZoneExhaustFields::AirOutletNodeName, _outletNode->name().get());
      }
    }

    // A6 , Field End-Use Subcategory
    s = modelObject.endUseSubcategory();
    if (s) {
      idfObject.setString(Fan_ZoneExhaustFields::EndUseSubcategory, *s);
    }

    // A7, Field Flow Fraction Schedule Name
    boost::optional<Schedule> flowFractionSchedule = modelObject.flowFractionSchedule();

    if (flowFractionSchedule) {
      boost::optional<IdfObject> _flowFractionSchedule = translateAndMapModelObject(flowFractionSchedule.get());

      if (_flowFractionSchedule && _flowFractionSchedule->name()) {
        idfObject.setString(Fan_ZoneExhaustFields::FlowFractionScheduleName, _flowFractionSchedule->name().get());
      }
    }

    // A8 , Field System Availability Manager Coupling Mode
    s = modelObject.systemAvailabilityManagerCouplingMode();

    if (s) {
      idfObject.setString(Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode, *s);
    }

    // A9, Field Minimum Zone Temperature Limit Schedule Name
    boost::optional<Schedule> minimumZoneTemperatureLimitSchedule = modelObject.minimumZoneTemperatureLimitSchedule();

    if (minimumZoneTemperatureLimitSchedule) {
      boost::optional<IdfObject> _minimumZoneTemperatureLimitSchedule = translateAndMapModelObject(minimumZoneTemperatureLimitSchedule.get());

      if (_minimumZoneTemperatureLimitSchedule && _minimumZoneTemperatureLimitSchedule->name()) {
        idfObject.setString(Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName, _minimumZoneTemperatureLimitSchedule->name().get());
      }
    }

    // A10, Field Balanced Exhaust Fraction Schedule Name
    boost::optional<Schedule> balancedExhaustFractionSchedule = modelObject.balancedExhaustFractionSchedule();

    if (balancedExhaustFractionSchedule) {
      boost::optional<IdfObject> _balancedExhaustFractionSchedule = translateAndMapModelObject(balancedExhaustFractionSchedule.get());

      if (_balancedExhaustFractionSchedule && _balancedExhaustFractionSchedule->name()) {
        idfObject.setString(Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName, _balancedExhaustFractionSchedule->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
