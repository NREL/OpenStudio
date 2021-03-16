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

#include "../../model/FanSystemModel.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFanSystemModel(FanSystemModel& modelObject) {
    //setup boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Node> node;
    OptionalModelObject mo;

    // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Fan_SystemModel, modelObject);

    // Model Name
    //std::string baseName = modelObject.name().get();

    // Availability Schedule Name: Required Object
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();
    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        idfObject.setString(Fan_SystemModelFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    // Air Inlet Node Name: Optional Object
    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        idfObject.setString(Fan_SystemModelFields::AirInletNodeName, inletNode->name().get());
      }
    }

    // Air Outlet Node Name: Optional Object
    if (boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>()) {
        idfObject.setString(Fan_SystemModelFields::AirOutletNodeName, outletNode->name().get());
      }
    }

    // Design Maximum Air Flow Rate: Autosizable Double
    value = modelObject.designMaximumAirFlowRate();
    if (value) {
      idfObject.setDouble(Fan_SystemModelFields::DesignMaximumAirFlowRate, value.get());
    } else {
      idfObject.setString(Fan_SystemModelFields::DesignMaximumAirFlowRate, "Autosize");
    }

    // Speed Control Method: Required String
    idfObject.setString(Fan_SystemModelFields::SpeedControlMethod, modelObject.speedControlMethod());

    // Electric Power Minimum Flow Rate Fraction: Required Double
    idfObject.setDouble(Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction, modelObject.electricPowerMinimumFlowRateFraction());

    // Design Pressure Rise: Required Double
    idfObject.setDouble(Fan_SystemModelFields::DesignPressureRise, modelObject.designPressureRise());

    // Motor Efficiency: Required Double
    idfObject.setDouble(Fan_SystemModelFields::MotorEfficiency, modelObject.motorEfficiency());

    // Motor In Air Stream Fraction: Required Double
    idfObject.setDouble(Fan_SystemModelFields::MotorInAirStreamFraction, modelObject.motorInAirStreamFraction());

    // Design Electric Power Consumption: Autosizable
    value = modelObject.designElectricPowerConsumption();
    if (value) {
      idfObject.setDouble(Fan_SystemModelFields::DesignElectricPowerConsumption, value.get());
    } else {
      idfObject.setString(Fan_SystemModelFields::DesignElectricPowerConsumption, "Autosize");
    }

    // Design Power Sizing Method: Required String
    idfObject.setString(Fan_SystemModelFields::DesignPowerSizingMethod, modelObject.designPowerSizingMethod());

    // Electric Power Per Unit Flow Rate: Required Double
    idfObject.setDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRate, modelObject.electricPowerPerUnitFlowRate());

    // Electric Power Per Unit Flow Rate Per Unit Pressure: Required Double
    idfObject.setDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure,
                        modelObject.electricPowerPerUnitFlowRatePerUnitPressure());

    // Fan Total Efficiency: Required Double
    idfObject.setDouble(Fan_SystemModelFields::FanTotalEfficiency, modelObject.fanTotalEfficiency());

    // Electric Power Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<Curve> _curve = modelObject.electricPowerFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> _idf_curve = translateAndMapModelObject(_curve.get())) {
        idfObject.setString(Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, _idf_curve->name().get());
      }
    }

    // Night Ventilation Mode Pressure Rise: Optional Double
    value = modelObject.nightVentilationModePressureRise();
    if (value) {
      idfObject.setDouble(Fan_SystemModelFields::NightVentilationModePressureRise, value.get());
    }

    // Night Ventilation Mode Flow Fraction: Optional Double
    value = modelObject.nightVentilationModeFlowFraction();
    if (value) {
      idfObject.setDouble(Fan_SystemModelFields::NightVentilationModeFlowFraction, value.get());
    }

    // Motor Loss Zone Name: Optional Object
    if (boost::optional<ThermalZone> _z = modelObject.motorLossZone()) {
      if (boost::optional<IdfObject> _idf_z = translateAndMapModelObject(_z.get())) {
        idfObject.setString(Fan_SystemModelFields::MotorLossZoneName, _idf_z->nameString());
      }
    }

    // Motor Loss Radiative Fraction: Optional Double
    value = modelObject.motorLossRadiativeFraction();
    if (value) {
      idfObject.setDouble(Fan_SystemModelFields::MotorLossRadiativeFraction, value.get());
    }

    // End-Use Subcategory: Required String
    idfObject.setString(Fan_SystemModelFields::EndUseSubcategory, modelObject.endUseSubcategory());

    // Speeds
    std::vector<FanSystemModelSpeed> speeds = modelObject.speeds();
    if (!speeds.empty()) {
      for (const FanSystemModelSpeed& speed : speeds) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction, speed.flowFraction());
        eg.setDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction, speed.electricPowerFraction());
      }
    }

    // Number of Speeds: Required Integer
    idfObject.setInt(Fan_SystemModelFields::NumberofSpeeds, modelObject.numberofSpeeds());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
