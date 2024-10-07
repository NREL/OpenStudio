/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DESIGNDAY_IMPL_HPP
#define MODEL_DESIGNDAY_IMPL_HPP

#include "SizingPeriod_Impl.hpp"

namespace openstudio {
namespace model {

  class ScheduleDay;

  namespace detail {

    class MODEL_API DesignDay_Impl : public SizingPeriod_Impl
    {

     public:
      // constructor
      DesignDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      DesignDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      DesignDay_Impl(const DesignDay_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~DesignDay_Impl() override = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double maximumDryBulbTemperature() const;

      bool isMaximumDryBulbTemperatureDefaulted() const;

      double dailyDryBulbTemperatureRange() const;

      bool isDailyDryBulbTemperatureRangeDefaulted() const;

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

      std::string humidityConditionType() const;

      bool isHumidityConditionTypeDefaulted() const;

      boost::optional<ScheduleDay> humidityConditionDaySchedule() const;

      boost::optional<double> wetBulbOrDewPointAtMaximumDryBulb() const;

      boost::optional<double> humidityRatioAtMaximumDryBulb() const;

      boost::optional<double> enthalpyAtMaximumDryBulb() const;

      std::string dryBulbTemperatureRangeModifierType() const;

      bool isDryBulbTemperatureRangeModifierTypeDefaulted() const;

      boost::optional<ScheduleDay> dryBulbTemperatureRangeModifierDaySchedule() const;

      std::string solarModelIndicator() const;

      bool isSolarModelIndicatorDefaulted() const;

      boost::optional<ScheduleDay> beamSolarDaySchedule() const;

      boost::optional<ScheduleDay> diffuseSolarDaySchedule() const;

      double ashraeClearSkyOpticalDepthForBeamIrradiance() const;

      bool isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted() const;

      double ashraeClearSkyOpticalDepthForDiffuseIrradiance() const;

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

      bool setHumidityConditionType(const std::string& humidityConditionType);

      void resetHumidityConditionType();

      bool setHumidityConditionDaySchedule(const ScheduleDay& schedule);

      void resetHumidityConditionDaySchedule();

      bool setWetBulbOrDewPointAtMaximumDryBulb(boost::optional<double> wetBulbOrDewPointAtMaximumDryBulb);

      void resetWetBulbOrDewPointAtMaximumDryBulb();

      bool setHumidityRatioAtMaximumDryBulb(boost::optional<double> humidityRatioAtMaximumDryBulb);

      void resetHumidityRatioAtMaximumDryBulb();

      bool setEnthalpyAtMaximumDryBulb(boost::optional<double> enthalpyAtMaximumDryBulb);

      void resetEnthalpyAtMaximumDryBulb();

      bool setDryBulbTemperatureRangeModifierType(const std::string& dryBulbTemperatureRangeModifierType);

      void resetDryBulbTemperatureRangeModifierType();

      bool setDryBulbTemperatureRangeModifierDaySchedule(const ScheduleDay& schedule);

      void resetDryBulbTemperatureRangeModifierDaySchedule();

      bool setSolarModelIndicator(const std::string& solarModelIndicator);

      void resetSolarModelIndicator();

      bool setBeamSolarDaySchedule(const ScheduleDay& schedule);

      void resetBeamSolarDaySchedule();

      bool setDiffuseSolarDaySchedule(const ScheduleDay& schedule);

      void resetDiffuseSolarDaySchedule();

      bool setAshraeClearSkyOpticalDepthForBeamIrradiance(double ashraeClearSkyOpticalDepthForBeamIrradiance);

      void resetAshraeClearSkyOpticalDepthForBeamIrradiance();

      bool setAshraeClearSkyOpticalDepthForDiffuseIrradiance(double ashraeClearSkyOpticalDepthForDiffuseIrradiance);

      void resetAshraeClearSkyOpticalDepthForDiffuseIrradiance();

      bool setDailyWetBulbTemperatureRange(boost::optional<double> dailyWetBulbTemperatureRange);

      void resetDailyWetBulbTemperatureRange();

      bool setMaximumNumberWarmupDays(boost::optional<int> maximumNumberWarmupDays);

      void resetMaximumNumberWarmupDays();

      bool setBeginEnvironmentResetMode(const std::string& beginEnvironmentResetMode);

      void resetBeginEnvironmentResetMode();

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.DesignDay");

      boost::optional<ModelObject> humidityConditionDayScheduleAsModelObject() const;
      boost::optional<ModelObject> dryBulbTemperatureRangeModifierDayScheduleAsModelObject() const;
      boost::optional<ModelObject> beamSolarDayScheduleAsModelObject() const;
      boost::optional<ModelObject> diffuseSolarDayScheduleAsModelObject() const;

      bool setHumidityConditionDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDryBulbTemperatureRangeModifierDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setBeamSolarDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDiffuseSolarDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNDAY_IMPL_HPP
