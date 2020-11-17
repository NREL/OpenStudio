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
#include "../../model/FanOnOff.hpp"
#include "../../model/FanOnOff_Impl.hpp"
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFanOnOff(FanOnOff& modelObject) {
    //setup boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Node> node;
    OptionalModelObject mo;

    // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Fan_OnOff, modelObject);

    // Model Name
    //std::string baseName = modelObject.name().get();

    //  A3 ,Field Availability Schedule Name
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        idfObject.setString(Fan_OnOffFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    //  N1 ,Field Fan Efficiency

    value = modelObject.fanEfficiency();

    if (value) {
      idfObject.setDouble(Fan_OnOffFields::FanTotalEfficiency, *value);
    }

    //  N2 Pressure Rise

    value = modelObject.pressureRise();

    if (value) {
      idfObject.setDouble(Fan_OnOffFields::PressureRise, *value);
    }

    // N3,  Field Maximum Flow Rate

    value = modelObject.maximumFlowRate();
    if (value) {
      idfObject.setDouble(Fan_OnOffFields::MaximumFlowRate, value.get());
    } else {
      idfObject.setString(Fan_OnOffFields::MaximumFlowRate, "Autosize");
    }

    // N4,  Field Motor Efficiency

    value = modelObject.motorEfficiency();

    if (value) {
      idfObject.setDouble(Fan_OnOffFields::MotorEfficiency, *value);
    }

    //   N5, Field Motor In Airstream Fraction

    value = modelObject.motorInAirstreamFraction();

    if (value) {
      idfObject.setDouble(Fan_OnOffFields::MotorInAirstreamFraction, *value);
    }

    // A4 Air Inlet Node Name

    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        idfObject.setString(Fan_OnOffFields::AirInletNodeName, inletNode->name().get());
      }
    }

    // A5 , Field Air Outlet Node Name

    if (boost::optional<ModelObject> ouletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> ouletNode = ouletModelObject->optionalCast<Node>()) {
        idfObject.setString(Fan_OnOffFields::AirOutletNodeName, ouletNode->name().get());
      }
    }

    // A6 , Field Fan Power Ratio Function of Speed Ratio Curve Name

    boost::optional<Curve> fanPowerRatioFunctionofSpeedRatioCurve = modelObject.fanPowerRatioFunctionofSpeedRatioCurve();

    if (fanPowerRatioFunctionofSpeedRatioCurve) {
      boost::optional<IdfObject> _fanPowerRatioFunctionofSpeedRatioCurve = translateAndMapModelObject(fanPowerRatioFunctionofSpeedRatioCurve.get());

      if (_fanPowerRatioFunctionofSpeedRatioCurve && _fanPowerRatioFunctionofSpeedRatioCurve->name()) {
        idfObject.setString(Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName, _fanPowerRatioFunctionofSpeedRatioCurve->name().get());
      }
    }

    // A7 , Field Fan Efficiency Ratio Function of Speed Ratio Curve Name

    boost::optional<Curve> fanEfficiencyRatioFunctionofSpeedRatioCurve = modelObject.fanEfficiencyRatioFunctionofSpeedRatioCurve();

    if (fanEfficiencyRatioFunctionofSpeedRatioCurve) {
      boost::optional<IdfObject> _fanEfficiencyRatioFunctionofSpeedRatioCurve =
        translateAndMapModelObject(fanEfficiencyRatioFunctionofSpeedRatioCurve.get());

      if (_fanEfficiencyRatioFunctionofSpeedRatioCurve && _fanEfficiencyRatioFunctionofSpeedRatioCurve->name()) {
        idfObject.setString(Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName,
                            _fanEfficiencyRatioFunctionofSpeedRatioCurve->name().get());
      }
    }

    // A8 , Field End-Use Subcategory

    idfObject.setString(Fan_OnOffFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
