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
#include "../../model/DesignDay.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/SizingPeriod_DesignDay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDesignDay(DesignDay& modelObject) {

    IdfObject idfObject(openstudio::IddObjectType::SizingPeriod_DesignDay);

    // Name
    idfObject.setName(modelObject.name().get());

    // Month
    idfObject.setInt(SizingPeriod_DesignDayFields::Month, modelObject.month());

    // Day of Month
    idfObject.setInt(SizingPeriod_DesignDayFields::DayofMonth, modelObject.dayOfMonth());

    // Day Type
    idfObject.setString(SizingPeriod_DesignDayFields::DayType, modelObject.dayType());

    // Maximum Dry-Bulb Temperature
    idfObject.setDouble(SizingPeriod_DesignDayFields::MaximumDryBulbTemperature, modelObject.maximumDryBulbTemperature());

    // Dry-Bulb Temperature Range Modifier Type
    std::string dryBulbTemperatureRangeModifierType = modelObject.dryBulbTemperatureRangeModifierType();
    idfObject.setString(SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, dryBulbTemperatureRangeModifierType);

    // Daily Dry-Bulb Temperature Range
    if (!istringEqual(dryBulbTemperatureRangeModifierType, "DifferenceSchedule")) {
      idfObject.setDouble(SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange, modelObject.dailyDryBulbTemperatureRange());
    }

    // Dry-Bulb Temperature Range Modifier Day Schedule Name
    if (istringEqual(dryBulbTemperatureRangeModifierType, "MultiplierSchedule")
        || istringEqual(dryBulbTemperatureRangeModifierType, "DifferenceSchedule")) {
      boost::optional<IdfObject> idfSchedule;
      if (boost::optional<ScheduleDay> schedule = modelObject.dryBulbTemperatureRangeModifierSchedule()) {
        idfSchedule = translateAndMapModelObject(*schedule);
      }
      if (idfSchedule) {
        idfObject.setString(SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierDayScheduleName, idfSchedule->name().get());
      } else {
        LOG(Error, "Dry Bulb Temperature Range Modifier Day Schedule Name field is required but not found");
      }
    }

    // Humidity Condition Type
    std::string humidityIndicatingType = modelObject.humidityIndicatingType();
    if (istringEqual(humidityIndicatingType, "Schedule")) {
      humidityIndicatingType = "RelativeHumiditySchedule";
    }
    idfObject.setString(SizingPeriod_DesignDayFields::HumidityConditionType, humidityIndicatingType);

    // Wetbulb or DewPoint at Maximum Dry-Bulb
    if (istringEqual(humidityIndicatingType, "Wetbulb") || istringEqual(humidityIndicatingType, "Dewpoint")
        || istringEqual(humidityIndicatingType, "WetBulbProfileMultiplierSchedule")
        || istringEqual(humidityIndicatingType, "WetBulbProfileDifferenceSchedule")
        || istringEqual(humidityIndicatingType, "WetBulbProfileDefaultMultipliers")) {
      // units for this field are C
      idfObject.setDouble(SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb,
                          modelObject.humidityIndicatingConditionsAtMaximumDryBulb());
    }

    // Humidity Condition Day Schedule Name
    if (istringEqual(humidityIndicatingType, "RelativeHumiditySchedule") || istringEqual(humidityIndicatingType, "WetBulbProfileMultiplierSchedule")
        || istringEqual(humidityIndicatingType, "WetBulbProfileDifferenceSchedule")
        || istringEqual(humidityIndicatingType, "RelativeHumiditySchedule")) {
      if (boost::optional<ScheduleDay> schedule = modelObject.humidityIndicatingDaySchedule()) {
        idfObject.setString(SizingPeriod_DesignDayFields::HumidityConditionDayScheduleName, schedule->name().get());
      } else {
        LOG(Error, "Humidity Condition Day Schedule Name field is required but not found");
      }
    }

    // Humidity Ratio at Maximum Dry-Bulb
    if (istringEqual(humidityIndicatingType, "HumidityRatio")) {
      // units for this field are kgWater/kgDryAir
      idfObject.setDouble(SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb, modelObject.humidityIndicatingConditionsAtMaximumDryBulb());
    }

    // Enthalpy at Maximum Dry-Bulb
    if (istringEqual(humidityIndicatingType, "Enthalpy")) {
      // units for this field are J/kg
      idfObject.setDouble(SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb, modelObject.humidityIndicatingConditionsAtMaximumDryBulb());
    }

    // Daily Wet-Bulb Temperature Range
    if (istringEqual(humidityIndicatingType, "WetbulbProfileMultiplierSchedule")
        || istringEqual(humidityIndicatingType, "WetBulbProfileDefaultMultipliers")) {
      if (OptionalDouble d = modelObject.dailyWetBulbTemperatureRange()) {
        idfObject.setDouble(SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, *d);
      } else {
        LOG(Error, "Daily Wet Bulb Temperature Range field is required but not found");
      }
    }

    // Barometric Pressure
    idfObject.setDouble(SizingPeriod_DesignDayFields::BarometricPressure, modelObject.barometricPressure());

    // Site Wind Speed
    idfObject.setDouble(SizingPeriod_DesignDayFields::WindSpeed, modelObject.windSpeed());

    // Site Wind Direction
    idfObject.setDouble(SizingPeriod_DesignDayFields::WindDirection, modelObject.windDirection());

    // Rain Indicator
    if (modelObject.rainIndicator()) {
      idfObject.setString(SizingPeriod_DesignDayFields::RainIndicator, "Yes");
    } else {
      idfObject.setString(SizingPeriod_DesignDayFields::RainIndicator, "No");
    }

    // Snow Indicator
    if (modelObject.snowIndicator()) {
      idfObject.setString(SizingPeriod_DesignDayFields::SnowIndicator, "Yes");
    } else {
      idfObject.setString(SizingPeriod_DesignDayFields::SnowIndicator, "No");
    }

    // Site Daylight Saving Time Status
    if (modelObject.daylightSavingTimeIndicator()) {
      idfObject.setString(SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, "Yes");
    } else {
      idfObject.setString(SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, "No");
    }

    // Solar Model Indicator
    std::string solarModelIndicator = modelObject.solarModelIndicator();
    idfObject.setString(SizingPeriod_DesignDayFields::SolarModelIndicator, solarModelIndicator);

    // Beam Solar Day Schedule Name and Site Diffuse Solar Radiation Rate per Area Day Schedule Name
    if (istringEqual(solarModelIndicator, "Schedule")) {

      boost::optional<IdfObject> idfSchedule;

      // Beam Solar Day Schedule Name
      if (boost::optional<ScheduleDay> schedule = modelObject.beamSolarDaySchedule()) {
        idfSchedule = translateAndMapModelObject(*schedule);
      }
      if (idfSchedule) {
        idfObject.setString(SizingPeriod_DesignDayFields::BeamSolarDayScheduleName, idfSchedule->name().get());
      } else {
        LOG(Error, "Beam Solar Day Schedule Name field is required but not found");
      }

      idfSchedule.reset();

      // Site Diffuse Solar Radiation Rate per Area Day Schedule Name
      if (boost::optional<ScheduleDay> schedule = modelObject.diffuseSolarDaySchedule()) {
        idfSchedule = translateAndMapModelObject(*schedule);
      }
      if (idfSchedule) {
        idfObject.setString(SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName, idfSchedule->name().get());
      } else {
        LOG(Error, "Site Diffuse Solar Radiation Rate per Area Day Schedule Name field is required but not found");
      }
    }

    if (istringEqual(solarModelIndicator, "ASHRAETau")) {
      // ASHRAE Clear Sky Optical Depth for Beam Irradiance (taub)
      idfObject.setDouble(SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforBeamIrradiance_taub_, modelObject.ashraeTaub());

      // ASHRAE Clear Sky Optical Depth for Diffuse Irradiance (taud)
      idfObject.setDouble(SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforDiffuseIrradiance_taud_, modelObject.ashraeTaud());
    }

    // Sky Clearness
    if (istringEqual(solarModelIndicator, "ASHRAEClearSky") || istringEqual(solarModelIndicator, "ZhangHuang")) {
      idfObject.setDouble(SizingPeriod_DesignDayFields::SkyClearness, modelObject.skyClearness());
    }

    m_idfObjects.push_back(idfObject);

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
