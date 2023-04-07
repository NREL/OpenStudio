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

#ifndef MODEL_DESIGNDAY_HPP
#define MODEL_DESIGNDAY_HPP

#include "ModelAPI.hpp"
#include "SizingPeriod.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  class ScheduleDay;

  namespace detail {
    class DesignDay_Impl;
  }  // namespace detail

  class MODEL_API DesignDay : public SizingPeriod
  {
   public:
    // constructor
    explicit DesignDay(const Model& model);

    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDayTypeValues();

    OS_DEPRECATED static std::vector<std::string> validHumidityIndicatingTypeValues();

    static std::vector<std::string> validHumidityConditionTypeValues();

    static std::vector<std::string> validDryBulbTemperatureRangeModifierTypeValues();

    static std::vector<std::string> validSolarModelIndicatorValues();

    static std::vector<std::string> validBeginEnvironmentResetModeValues();

    //@}
    /** @name Getters */
    //@{

    double maximumDryBulbTemperature() const;

    bool isMaximumDryBulbTemperatureDefaulted() const;

    double dailyDryBulbTemperatureRange() const;

    bool isDailyDryBulbTemperatureRangeDefaulted() const;

    OS_DEPRECATED double humidityIndicatingConditionsAtMaximumDryBulb() const;

    OS_DEPRECATED bool isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted() const;

    double barometricPressure() const;

    bool isBarometricPressureDefaulted() const;

    double windSpeed() const;

    bool isWindSpeedDefaulted() const;

    double windDirection() const;

    bool isWindDirectionDefaulted() const;

    double skyClearness() const;

    bool isSkyClearnessDefaulted() const;

    bool rainIndicator() const;

    bool isRainIndicatorDefaulted() const;

    bool snowIndicator() const;

    bool isSnowIndicatorDefaulted() const;

    int dayOfMonth() const;

    bool isDayOfMonthDefaulted() const;

    int month() const;

    bool isMonthDefaulted() const;

    std::string dayType() const;

    bool isDayTypeDefaulted() const;

    bool daylightSavingTimeIndicator() const;

    bool isDaylightSavingTimeIndicatorDefaulted() const;

    OS_DEPRECATED std::string humidityIndicatingType() const;

    std::string humidityConditionType() const;

    OS_DEPRECATED bool isHumidityIndicatingTypeDefaulted() const;

    bool isHumidityConditionTypeDefaulted() const;

    OS_DEPRECATED boost::optional<ScheduleDay> humidityIndicatingDaySchedule() const;

    boost::optional<ScheduleDay> humidityConditionDaySchedule() const;

    boost::optional<double> wetBulbOrDewPointAtMaximumDryBulb() const;

    boost::optional<double> humidityRatioAtMaximumDryBulb() const;

    boost::optional<double> enthalpyAtMaximumDryBulb() const;

    std::string dryBulbTemperatureRangeModifierType() const;

    bool isDryBulbTemperatureRangeModifierTypeDefaulted() const;

    OS_DEPRECATED boost::optional<ScheduleDay> dryBulbTemperatureRangeModifierSchedule() const;

    boost::optional<ScheduleDay> dryBulbTemperatureRangeModifierDaySchedule() const;

    std::string solarModelIndicator() const;

    bool isSolarModelIndicatorDefaulted() const;

    boost::optional<ScheduleDay> beamSolarDaySchedule() const;

    boost::optional<ScheduleDay> diffuseSolarDaySchedule() const;

    OS_DEPRECATED double ashraeTaub() const;

    double ashraeClearSkyOpticalDepthForBeamIrradiance() const;

    OS_DEPRECATED bool isAshraeTaubDefaulted() const;

    bool isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted() const;

    OS_DEPRECATED double ashraeTaud() const;

    double ashraeClearSkyOpticalDepthForDiffuseIrradiance() const;

    OS_DEPRECATED bool isAshraeTaudDefaulted() const;

    bool isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted() const;

    boost::optional<double> dailyWetBulbTemperatureRange() const;

    boost::optional<int> maximumNumberWarmupDays() const;

    std::string beginEnvironmentResetMode() const;

    bool isBeginEnvironmentResetModeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumDryBulbTemperature(double maximumDryBulbTemperature);

    void resetMaximumDryBulbTemperature();

    bool setDailyDryBulbTemperatureRange(double dailyDryBulbTemperatureRange);

    void resetDailyDryBulbTemperatureRange();

    bool setBarometricPressure(double barometricPressure);

    void resetBarometricPressure();

    bool setWindSpeed(double windSpeed);

    void resetWindSpeed();

    bool setWindDirection(double windDirection);

    void resetWindDirection();

    bool setSkyClearness(double skyClearness);

    void resetSkyClearness();

    bool setRainIndicator(bool rainIndicator);

    void resetRainIndicator();

    bool setSnowIndicator(bool snowIndicator);

    void resetSnowIndicator();

    bool setDayOfMonth(int dayOfMonth);

    void resetDayOfMonth();

    bool setMonth(int month);

    void resetMonth();

    bool setDayType(const std::string& dayType);

    void resetDayType();

    bool setDaylightSavingTimeIndicator(bool daylightSavingTimeIndicator);

    void resetDaylightSavingTimeIndicator();

    OS_DEPRECATED bool setHumidityIndicatingType(const std::string& humidityIndicatingType);

    bool setHumidityConditionType(const std::string& humidityConditionType);

    OS_DEPRECATED void resetHumidityIndicatingType();

    void resetHumidityConditionType();

    /** Sets the humidity indicating day schedule. The appropriate ScheduleTypeLimits varies
   *  depending on the humidity indicating type. Please see the EnergyPlus input-output reference
   *  for details before using this feature. */
    OS_DEPRECATED bool setHumidityIndicatingDaySchedule(const ScheduleDay& schedule);

    bool setHumidityConditionDaySchedule(const ScheduleDay& schedule);

    OS_DEPRECATED void resetHumidityIndicatingDaySchedule();

    void resetHumidityConditionDaySchedule();

    OS_DEPRECATED bool setHumidityIndicatingConditionsAtMaximumDryBulb(double humidityIndicatingConditionsAtMaximumDryBulb);

    OS_DEPRECATED void resetHumidityIndicatingConditionsAtMaximumDryBulb();

    bool setWetBulbOrDewPointAtMaximumDryBulb(double wetBulbOrDewPointAtMaximumDryBulb);

    void resetWetBulbOrDewPointAtMaximumDryBulb();

    bool setHumidityRatioAtMaximumDryBulb(double humidityRatioAtMaximumDryBulb);

    void resetHumidityRatioAtMaximumDryBulb();

    bool setEnthalpyAtMaximumDryBulb(double enthalpyAtMaximumDryBulb);

    void resetEnthalpyAtMaximumDryBulb();

    bool setDryBulbTemperatureRangeModifierType(const std::string& dryBulbTemperatureRangeModifierType);

    void resetDryBulbTemperatureRangeModifierType();

    /** Sets the dry bulb temperature range modifier day schedule. The appropriate
   *  ScheduleTypeLimits varies depending on the dry bulb temperature range modifier type. Please
   *  see the EnergyPlus input-output reference for details before using this feature. */
    OS_DEPRECATED bool setDryBulbTemperatureRangeModifierSchedule(const ScheduleDay& schedule);

    bool setDryBulbTemperatureRangeModifierDaySchedule(const ScheduleDay& schedule);

    OS_DEPRECATED void resetDryBulbTemperatureRangeModifierSchedule();

    void resetDryBulbTemperatureRangeModifierDaySchedule();

    bool setSolarModelIndicator(const std::string& solarModelIndicator);

    void resetSolarModelIndicator();

    bool setBeamSolarDaySchedule(const ScheduleDay& schedule);

    void resetBeamSolarDaySchedule();

    bool setDiffuseSolarDaySchedule(const ScheduleDay& schedule);

    void resetDiffuseSolarDaySchedule();

    OS_DEPRECATED bool setAshraeTaub(double ashraeTaub);

    bool setAshraeClearSkyOpticalDepthForBeamIrradiance(double ashraeClearSkyOpticalDepthForBeamIrradiance);

    OS_DEPRECATED void resetAshraeTaub();

    void resetAshraeClearSkyOpticalDepthForBeamIrradiance();

    OS_DEPRECATED bool setAshraeTaud(double ashraeTaud);

    bool setAshraeClearSkyOpticalDepthForDiffuseIrradiance(double ashraeClearSkyOpticalDepthForDiffuseIrradiance);

    OS_DEPRECATED void resetAshraeTaud();

    void resetAshraeClearSkyOpticalDepthForDiffuseIrradiance();

    bool setDailyWetBulbTemperatureRange(double dailyWetBulbTemperatureRange);

    void resetDailyWetBulbTemperatureRange();

    bool setMaximumNumberWarmupDays(int maximumNumberWarmupDays);

    void resetMaximumNumberWarmupDays();

    bool setBeginEnvironmentResetMode(const std::string& beginEnvironmentResetMode);

    void resetBeginEnvironmentResetMode();

    //@}

    virtual ~DesignDay() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DesignDay(const DesignDay& other) = default;
    DesignDay(DesignDay&& other) = default;
    DesignDay& operator=(const DesignDay&) = default;
    DesignDay& operator=(DesignDay&&) = default;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    using ImplType = detail::DesignDay_Impl;

    // constructor
    explicit DesignDay(std::shared_ptr<detail::DesignDay_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.DesignDay");
  };

  /** \relates DesignDay */
  using OptionalDesignDay = boost::optional<DesignDay>;

  /** \relates DesignDay */
  using DesignDayVector = std::vector<DesignDay>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNDAY_HPP
