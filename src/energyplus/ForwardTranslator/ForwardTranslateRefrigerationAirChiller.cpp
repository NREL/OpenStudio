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
#include "../../model/RefrigerationAirChiller.hpp"
#include "../../model/Schedule.hpp"
// #include <model/Curve.hpp>
// #include <model/Curve_Impl.hpp>
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/Refrigeration_AirChiller_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationAirChiller(RefrigerationAirChiller& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_AirChiller, modelObject);

    // AvailabilityScheduleName
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        object.setString(Refrigeration_AirChillerFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    //CapacityRatingType
    std::string capacityCorrectionCurveType;
    s = modelObject.capacityRatingType();
    if (s) {
      object.setString(Refrigeration_AirChillerFields::CapacityRatingType, s.get());
      if (istringEqual(s.get(), "UnitLoadFactorSensibleOnly")) {
        capacityCorrectionCurveType = "LinearSHR60";
      } else if (s.get().find("European") != std::string::npos) {
        capacityCorrectionCurveType = "European";
      } else {
        capacityCorrectionCurveType = "LinearSHR60";
      }
    }
    //RatedUnitLoadFactor
    d = modelObject.ratedUnitLoadFactor();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedUnitLoadFactor, d.get());
    }
    //RatedCapacity
    d = modelObject.ratedCapacity();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedCapacity, d.get());
    }
    //RatedRelativeHumidity
    d = modelObject.ratedRelativeHumidity();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedRelativeHumidity, d.get());
    }
    //RatedCoolingSourceTemperature
    d = modelObject.ratedCoolingSourceTemperature();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedCoolingSourceTemperature, d.get());
    }
    //RatedTemperatureDifferenceDT1
    d = modelObject.ratedTemperatureDifferenceDT1();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedTemperatureDifferenceDT1, d.get());
    }
    //MaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature
    d = modelObject.maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::MaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature, d.get());
    }
    //CoilMaterialCorrectionFactor
    d = modelObject.coilMaterialCorrectionFactor();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::CoilMaterialCorrectionFactor, d.get());
    }
    //RefrigerantCorrectionFactor
    d = modelObject.refrigerantCorrectionFactor();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RefrigerantCorrectionFactor, d.get());
    }
    //CapacityCorrectionCurveType
    // s = modelObject.capacityCorrectionCurveType();
    // if (s) {
    //   object.setString(Refrigeration_AirChillerFields::CapacityCorrectionCurveType,s.get());
    // }
    object.setString(Refrigeration_AirChillerFields::CapacityCorrectionCurveType, capacityCorrectionCurveType);
    //CapacityCorrectionCurveName
    // boost::optional<Curve> capacityCorrectionCurve = modelObject.capacityCorrectionCurve();

    // if( capacityCorrectionCurve )
    // {
    //   boost::optional<IdfObject> _capacityCorrectionCurve = translateAndMapModelObject(capacityCorrectionCurve.get());

    //   if( _capacityCorrectionCurve && _capacityCorrectionCurve->name() )
    //   {
    //     object.setString(Refrigeration_AirChillerFields::CapacityCorrectionCurveName,_capacityCorrectionCurve->name().get());
    //   }
    // }
    object.setString(Refrigeration_AirChillerFields::CapacityCorrectionCurveName, "");
    //SHR60CorrectionFactor
    d = modelObject.sHR60CorrectionFactor();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::SHR60CorrectionFactor, d.get());
    }
    //RatedTotalHeatingPower
    d = modelObject.ratedTotalHeatingPower();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedTotalHeatingPower, d.get());
    }
    //HeatingPowerScheduleName
    boost::optional<Schedule> heatingPowerSchedule = modelObject.heatingPowerSchedule();

    if (heatingPowerSchedule) {
      boost::optional<IdfObject> _heatingPowerSchedule = translateAndMapModelObject(heatingPowerSchedule.get());

      if (_heatingPowerSchedule && _heatingPowerSchedule->name()) {
        object.setString(Refrigeration_AirChillerFields::HeatingPowerScheduleName, _heatingPowerSchedule->name().get());
      }
    }
    //FanSpeedControlType
    s = modelObject.fanSpeedControlType();
    if (s) {
      object.setString(Refrigeration_AirChillerFields::FanSpeedControlType, s.get());
    }
    //RatedFanPower
    d = modelObject.ratedFanPower();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedFanPower, d.get());
    }
    //RatedAirFlow
    d = modelObject.ratedAirFlow();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::RatedAirFlow, d.get());
    }
    //MinimumFanAirFlowRatio
    d = modelObject.minimumFanAirFlowRatio();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::MinimumFanAirFlowRatio, d.get());
    }
    //DefrostType
    s = modelObject.defrostType();
    if (s) {
      object.setString(Refrigeration_AirChillerFields::DefrostType, s.get());
    }
    //DefrostControlType
    s = modelObject.defrostControlType();
    if (s) {
      object.setString(Refrigeration_AirChillerFields::DefrostControlType, s.get());
    }
    //DefrostScheduleName
    boost::optional<Schedule> defrostSchedule = modelObject.defrostSchedule();

    if (defrostSchedule) {
      boost::optional<IdfObject> _defrostSchedule = translateAndMapModelObject(defrostSchedule.get());

      if (_defrostSchedule && _defrostSchedule->name()) {
        object.setString(Refrigeration_AirChillerFields::DefrostScheduleName, _defrostSchedule->name().get());
      }
    }
    //DefrostDripDownScheduleName
    boost::optional<Schedule> defrostDripDownSchedule = modelObject.defrostDripDownSchedule();

    if (defrostDripDownSchedule) {
      boost::optional<IdfObject> _defrostDripDownSchedule = translateAndMapModelObject(defrostDripDownSchedule.get());

      if (_defrostDripDownSchedule && _defrostDripDownSchedule->name()) {
        object.setString(Refrigeration_AirChillerFields::DefrostDripDownScheduleName, _defrostDripDownSchedule->name().get());
      }
    }
    //DefrostPower
    d = modelObject.defrostPower();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::DefrostPower, d.get());
    }
    //TemperatureTerminationDefrostFractiontoIce
    d = modelObject.temperatureTerminationDefrostFractiontoIce();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::TemperatureTerminationDefrostFractiontoIce, d.get());
    }
    //VerticalLocation
    s = modelObject.verticalLocation();
    if (s) {
      object.setString(Refrigeration_AirChillerFields::VerticalLocation, s.get());
    }
    //AverageRefrigerantChargeInventory
    d = modelObject.averageRefrigerantChargeInventory();
    if (d) {
      object.setDouble(Refrigeration_AirChillerFields::AverageRefrigerantChargeInventory, d.get());
    }

    return object;
  }
}  // namespace energyplus
}  // namespace openstudio
