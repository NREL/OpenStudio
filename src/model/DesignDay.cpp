/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DesignDay.hpp"
#include "DesignDay_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/OS_SizingPeriod_DesignDay_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    DesignDay_Impl::DesignDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : SizingPeriod_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DesignDay::iddObjectType());
    }

    DesignDay_Impl::DesignDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SizingPeriod_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DesignDay::iddObjectType());
    }

    DesignDay_Impl::DesignDay_Impl(const DesignDay_Impl& other, Model_Impl* model, bool keepHandle) : SizingPeriod_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& DesignDay_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType DesignDay_Impl::iddObjectType() const {
      return DesignDay::iddObjectType();
    }

    double DesignDay_Impl::maximumDryBulbTemperature() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::MaximumDryBulbTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isMaximumDryBulbTemperatureDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::MaximumDryBulbTemperature);
    }

    double DesignDay_Impl::dailyDryBulbTemperatureRange() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isDailyDryBulbTemperatureRangeDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange);
    }

    double DesignDay_Impl::barometricPressure() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::BarometricPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isBarometricPressureDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::BarometricPressure);
    }

    double DesignDay_Impl::windSpeed() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::WindSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isWindSpeedDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::WindSpeed);
    }

    double DesignDay_Impl::windDirection() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::WindDirection, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isWindDirectionDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::WindDirection);
    }

    double DesignDay_Impl::skyClearness() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::SkyClearness, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isSkyClearnessDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::SkyClearness);
    }

    bool DesignDay_Impl::rainIndicator() const {
      bool result(false);
      std::string i = getString(OS_SizingPeriod_DesignDayFields::RainIndicator, true).get();
      if (i == "Yes") {
        result = true;
      }
      return result;
    }

    bool DesignDay_Impl::isRainIndicatorDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::RainIndicator);
    }

    bool DesignDay_Impl::snowIndicator() const {
      bool result(false);
      std::string i = getString(OS_SizingPeriod_DesignDayFields::SnowIndicator, true).get();
      if (i == "Yes") {
        result = true;
      }
      return result;
    }

    bool DesignDay_Impl::isSnowIndicatorDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::SnowIndicator);
    }

    int DesignDay_Impl::dayOfMonth() const {
      boost::optional<int> value = getInt(OS_SizingPeriod_DesignDayFields::DayofMonth, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isDayOfMonthDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::DayofMonth);
    }

    int DesignDay_Impl::month() const {
      boost::optional<int> value = getInt(OS_SizingPeriod_DesignDayFields::Month, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isMonthDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::Month);
    }

    std::string DesignDay_Impl::dayType() const {
      boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::DayType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isDayTypeDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::DayType);
    }

    bool DesignDay_Impl::daylightSavingTimeIndicator() const {
      bool result(false);
      std::string i = getString(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, true).get();
      if (i == "Yes") {
        result = true;
      }
      return result;
    }

    bool DesignDay_Impl::isDaylightSavingTimeIndicatorDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator);
    }

    std::string DesignDay_Impl::humidityConditionType() const {
      boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::HumidityConditionType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isHumidityConditionTypeDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::HumidityConditionType);
    }

    boost::optional<ScheduleDay> DesignDay_Impl::humidityConditionDaySchedule() const {
      return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(OS_SizingPeriod_DesignDayFields::HumidityConditionDayScheduleName);
    }

    boost::optional<double> DesignDay_Impl::wetBulbOrDewPointAtMaximumDryBulb() const {
      return getDouble(OS_SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb, true);
    }

    boost::optional<double> DesignDay_Impl::humidityRatioAtMaximumDryBulb() const {
      return getDouble(OS_SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb, true);
    }

    boost::optional<double> DesignDay_Impl::enthalpyAtMaximumDryBulb() const {
      return getDouble(OS_SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb, true);
    }

    std::string DesignDay_Impl::dryBulbTemperatureRangeModifierType() const {
      boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isDryBulbTemperatureRangeModifierTypeDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType);
    }

    boost::optional<ScheduleDay> DesignDay_Impl::dryBulbTemperatureRangeModifierDaySchedule() const {
      return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(
        OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierDayScheduleName);
    }

    std::string DesignDay_Impl::solarModelIndicator() const {
      boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::SolarModelIndicator, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isSolarModelIndicatorDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::SolarModelIndicator);
    }

    boost::optional<ScheduleDay> DesignDay_Impl::beamSolarDaySchedule() const {
      return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(OS_SizingPeriod_DesignDayFields::BeamSolarDayScheduleName);
    }

    boost::optional<ScheduleDay> DesignDay_Impl::diffuseSolarDaySchedule() const {
      return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(OS_SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName);
    }

    double DesignDay_Impl::ashraeClearSkyOpticalDepthForBeamIrradiance() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforBeamIrradiance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforBeamIrradiance);
    }

    double DesignDay_Impl::ashraeClearSkyOpticalDepthForDiffuseIrradiance() const {
      boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforDiffuseIrradiance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforDiffuseIrradiance);
    }

    boost::optional<double> DesignDay_Impl::dailyWetBulbTemperatureRange() const {
      return getDouble(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, true);
    }

    boost::optional<int> DesignDay_Impl::maximumNumberWarmupDays() const {
      return getInt(OS_SizingPeriod_DesignDayFields::MaximumNumberWarmupDays, true);
    }

    std::string DesignDay_Impl::beginEnvironmentResetMode() const {
      boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::BeginEnvironmentResetMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DesignDay_Impl::isBeginEnvironmentResetModeDefaulted() const {
      return isEmpty(OS_SizingPeriod_DesignDayFields::BeginEnvironmentResetMode);
    }

    bool DesignDay_Impl::setMaximumDryBulbTemperature(double maximumDryBulbTemperature) {
      bool result = setDouble(OS_SizingPeriod_DesignDayFields::MaximumDryBulbTemperature, maximumDryBulbTemperature);
      return result;
    }

    void DesignDay_Impl::resetMaximumDryBulbTemperature() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::MaximumDryBulbTemperature, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDailyDryBulbTemperatureRange(double dailyDryBulbTemperatureRange) {
      bool result = setDouble(OS_SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange, dailyDryBulbTemperatureRange);
      return result;
    }

    void DesignDay_Impl::resetDailyDryBulbTemperatureRange() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setBarometricPressure(double barometricPressure) {
      bool result = setDouble(OS_SizingPeriod_DesignDayFields::BarometricPressure, barometricPressure);
      return result;
    }

    void DesignDay_Impl::resetBarometricPressure() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::BarometricPressure, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setWindSpeed(double windSpeed) {
      bool result = setDouble(OS_SizingPeriod_DesignDayFields::WindSpeed, windSpeed);
      return result;
    }

    void DesignDay_Impl::resetWindSpeed() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::WindSpeed, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setWindDirection(double windDirection) {
      bool result = setDouble(OS_SizingPeriod_DesignDayFields::WindDirection, windDirection);
      return result;
    }

    void DesignDay_Impl::resetWindDirection() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::WindDirection, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setSkyClearness(double skyClearness) {
      bool result = setDouble(OS_SizingPeriod_DesignDayFields::SkyClearness, skyClearness);
      return result;
    }

    void DesignDay_Impl::resetSkyClearness() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::SkyClearness, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setRainIndicator(bool rainIndicator) {
      if (rainIndicator) {
        return this->setString(OS_SizingPeriod_DesignDayFields::RainIndicator, "Yes");
      } else {
        return this->setString(OS_SizingPeriod_DesignDayFields::RainIndicator, "No");
      }
      return false;
    }

    void DesignDay_Impl::resetRainIndicator() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::RainIndicator, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setSnowIndicator(bool snowIndicator) {
      if (snowIndicator) {
        return this->setString(OS_SizingPeriod_DesignDayFields::SnowIndicator, "Yes");
      } else {
        return this->setString(OS_SizingPeriod_DesignDayFields::SnowIndicator, "No");
      }
      return false;
    }

    void DesignDay_Impl::resetSnowIndicator() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::SnowIndicator, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDayOfMonth(int dayOfMonth) {
      bool result = setInt(OS_SizingPeriod_DesignDayFields::DayofMonth, dayOfMonth);
      return result;
    }

    void DesignDay_Impl::resetDayOfMonth() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DayofMonth, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setMonth(int month) {
      bool result = setInt(OS_SizingPeriod_DesignDayFields::Month, month);
      return result;
    }

    void DesignDay_Impl::resetMonth() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::Month, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDayType(const std::string& dayType) {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DayType, dayType);
      return result;
    }

    void DesignDay_Impl::resetDayType() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DayType, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDaylightSavingTimeIndicator(bool daylightSavingTimeIndicator) {
      if (daylightSavingTimeIndicator) {
        return this->setString(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, "Yes");
      } else {
        return this->setString(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, "No");
      }
      return false;
    }

    void DesignDay_Impl::resetDaylightSavingTimeIndicator() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setHumidityConditionType(const std::string& humidityConditionType) {
      bool result = setString(OS_SizingPeriod_DesignDayFields::HumidityConditionType, humidityConditionType);
      return result;
    }

    void DesignDay_Impl::resetHumidityConditionType() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::HumidityConditionType, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setHumidityConditionDaySchedule(const ScheduleDay& schedule) {
      return this->getObject<ModelObject>().setPointer(OS_SizingPeriod_DesignDayFields::HumidityConditionDayScheduleName, schedule.handle());
    }

    void DesignDay_Impl::resetHumidityConditionDaySchedule() {
      this->setString(OS_SizingPeriod_DesignDayFields::HumidityConditionDayScheduleName, "");
    }

    bool DesignDay_Impl::setWetBulbOrDewPointAtMaximumDryBulb(boost::optional<double> wetBulbOrDewPointAtMaximumDryBulb) {
      bool result = false;
      if (wetBulbOrDewPointAtMaximumDryBulb) {
        result = setDouble(OS_SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb, wetBulbOrDewPointAtMaximumDryBulb.get());
      } else {
        result = setString(OS_SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void DesignDay_Impl::resetWetBulbOrDewPointAtMaximumDryBulb() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setHumidityRatioAtMaximumDryBulb(boost::optional<double> humidityRatioAtMaximumDryBulb) {
      bool result = false;
      if (humidityRatioAtMaximumDryBulb) {
        result = setDouble(OS_SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb, humidityRatioAtMaximumDryBulb.get());
      } else {
        result = setString(OS_SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void DesignDay_Impl::resetHumidityRatioAtMaximumDryBulb() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setEnthalpyAtMaximumDryBulb(boost::optional<double> enthalpyAtMaximumDryBulb) {
      bool result = false;
      if (enthalpyAtMaximumDryBulb) {
        result = setDouble(OS_SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb, enthalpyAtMaximumDryBulb.get());
      } else {
        result = setString(OS_SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void DesignDay_Impl::resetEnthalpyAtMaximumDryBulb() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDryBulbTemperatureRangeModifierType(const std::string& dryBulbTemperatureRangeModifierType) {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, dryBulbTemperatureRangeModifierType);
      return result;
    }

    void DesignDay_Impl::resetDryBulbTemperatureRangeModifierType() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDryBulbTemperatureRangeModifierDaySchedule(const ScheduleDay& schedule) {
      return this->getObject<ModelObject>().setPointer(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierDayScheduleName,
                                                       schedule.handle());
    }

    void DesignDay_Impl::resetDryBulbTemperatureRangeModifierDaySchedule() {
      this->setString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierDayScheduleName, "");
    }

    bool DesignDay_Impl::setSolarModelIndicator(const std::string& solarModelIndicator) {
      bool result = setString(OS_SizingPeriod_DesignDayFields::SolarModelIndicator, solarModelIndicator);
      return result;
    }

    void DesignDay_Impl::resetSolarModelIndicator() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::SolarModelIndicator, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setBeamSolarDaySchedule(const ScheduleDay& schedule) {
      return this->setPointer(OS_SizingPeriod_DesignDayFields::BeamSolarDayScheduleName, schedule.handle());
    }

    void DesignDay_Impl::resetBeamSolarDaySchedule() {
      this->setString(OS_SizingPeriod_DesignDayFields::BeamSolarDayScheduleName, "");
    }

    bool DesignDay_Impl::setDiffuseSolarDaySchedule(const ScheduleDay& schedule) {
      return this->setPointer(OS_SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName, schedule.handle());
    }

    void DesignDay_Impl::resetDiffuseSolarDaySchedule() {
      this->setString(OS_SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName, "");
    }

    boost::optional<ModelObject> DesignDay_Impl::humidityConditionDayScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalScheduleDay schedule = humidityConditionDaySchedule();
      if (schedule) {
        result = *schedule;
      }
      return result;
    }

    boost::optional<ModelObject> DesignDay_Impl::dryBulbTemperatureRangeModifierDayScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalScheduleDay schedule = dryBulbTemperatureRangeModifierDaySchedule();
      if (schedule) {
        result = *schedule;
      }
      return result;
    }

    boost::optional<ModelObject> DesignDay_Impl::beamSolarDayScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalScheduleDay schedule = beamSolarDaySchedule();
      if (schedule) {
        result = *schedule;
      }
      return result;
    }

    boost::optional<ModelObject> DesignDay_Impl::diffuseSolarDayScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalScheduleDay schedule = diffuseSolarDaySchedule();
      if (schedule) {
        result = *schedule;
      }
      return result;
    }

    bool DesignDay_Impl::setHumidityConditionDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
        if (schedule) {
          return setHumidityConditionDaySchedule(*schedule);
        }
        return false;
      } else {
        resetHumidityConditionDaySchedule();
      }
      return true;
    }

    bool DesignDay_Impl::setDryBulbTemperatureRangeModifierDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
        if (schedule) {
          return setDryBulbTemperatureRangeModifierDaySchedule(*schedule);
        }
        return false;
      } else {
        resetDryBulbTemperatureRangeModifierDaySchedule();
      }
      return true;
    }

    bool DesignDay_Impl::setBeamSolarDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
        if (schedule) {
          return setBeamSolarDaySchedule(*schedule);
        }
        return false;
      } else {
        resetBeamSolarDaySchedule();
      }
      return true;
    }

    bool DesignDay_Impl::setDiffuseSolarDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
        if (schedule) {
          return setDiffuseSolarDaySchedule(*schedule);
        }
        return false;
      } else {
        resetDiffuseSolarDaySchedule();
      }
      return true;
    }

    bool DesignDay_Impl::setAshraeClearSkyOpticalDepthForBeamIrradiance(double ashraeClearSkyOpticalDepthForBeamIrradiance) {
      bool result =
        setDouble(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforBeamIrradiance, ashraeClearSkyOpticalDepthForBeamIrradiance);
      return result;
    }

    void DesignDay_Impl::resetAshraeClearSkyOpticalDepthForBeamIrradiance() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforBeamIrradiance, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setAshraeClearSkyOpticalDepthForDiffuseIrradiance(double ashraeClearSkyOpticalDepthForDiffuseIrradiance) {
      bool result =
        setDouble(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforDiffuseIrradiance, ashraeClearSkyOpticalDepthForDiffuseIrradiance);
      return result;
    }

    void DesignDay_Impl::resetAshraeClearSkyOpticalDepthForDiffuseIrradiance() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforDiffuseIrradiance, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setDailyWetBulbTemperatureRange(boost::optional<double> dailyWetBulbTemperatureRange) {
      bool result = false;
      if (dailyWetBulbTemperatureRange) {
        result = setDouble(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, dailyWetBulbTemperatureRange.get());
      } else {
        result = setString(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void DesignDay_Impl::resetDailyWetBulbTemperatureRange() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, "");
      OS_ASSERT(result);
    }

    void DesignDay_Impl::ensureNoLeapDays() {
      boost::optional<int> month_ = getInt(OS_SizingPeriod_DesignDayFields::Month);
      if (month_ && (*month_ == 2)) {
        boost::optional<int> day_ = this->getInt(OS_SizingPeriod_DesignDayFields::DayofMonth);
        if (day_ && (*day_ == 29)) {
          this->setInt(OS_SizingPeriod_DesignDayFields::DayofMonth, 28);
        }
      }
    }

    bool DesignDay_Impl::setMaximumNumberWarmupDays(boost::optional<int> maximumNumberWarmupDays) {
      bool result = false;
      if (maximumNumberWarmupDays) {
        result = setInt(OS_SizingPeriod_DesignDayFields::MaximumNumberWarmupDays, maximumNumberWarmupDays.get());
      } else {
        result = setString(OS_SizingPeriod_DesignDayFields::MaximumNumberWarmupDays, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void DesignDay_Impl::resetMaximumNumberWarmupDays() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::MaximumNumberWarmupDays, "");
      OS_ASSERT(result);
    }

    bool DesignDay_Impl::setBeginEnvironmentResetMode(const std::string& beginEnvironmentResetMode) {
      bool result = setString(OS_SizingPeriod_DesignDayFields::BeginEnvironmentResetMode, beginEnvironmentResetMode);
      return result;
    }

    void DesignDay_Impl::resetBeginEnvironmentResetMode() {
      bool result = setString(OS_SizingPeriod_DesignDayFields::BeginEnvironmentResetMode, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  /// constructor
  DesignDay::DesignDay(const Model& model) : SizingPeriod(DesignDay::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DesignDay_Impl>());

    setWetBulbOrDewPointAtMaximumDryBulb(23.0);
  }

  // constructor
  DesignDay::DesignDay(std::shared_ptr<detail::DesignDay_Impl> impl) : SizingPeriod(std::move(impl)) {}

  IddObjectType DesignDay::iddObjectType() {
    IddObjectType result(IddObjectType::OS_SizingPeriod_DesignDay);
    return result;
  }

  std::vector<std::string> DesignDay::validDayTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SizingPeriod_DesignDayFields::DayType);
  }

  // Deprecated
  std::vector<std::string> DesignDay::validHumidityIndicatingTypeValues() {
    DEPRECATED_AT_MSG(3, 3, 0, "Use validHumidityConditionTypeValues instead.");
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SizingPeriod_DesignDayFields::HumidityConditionType);
  }

  std::vector<std::string> DesignDay::validHumidityConditionTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SizingPeriod_DesignDayFields::HumidityConditionType);
  }

  std::vector<std::string> DesignDay::validDryBulbTemperatureRangeModifierTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType);
  }

  std::vector<std::string> DesignDay::validSolarModelIndicatorValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SizingPeriod_DesignDayFields::SolarModelIndicator);
  }

  std::vector<std::string> DesignDay::validBeginEnvironmentResetModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SizingPeriod_DesignDayFields::BeginEnvironmentResetMode);
  }

  double DesignDay::maximumDryBulbTemperature() const {
    return getImpl<detail::DesignDay_Impl>()->maximumDryBulbTemperature();
  }

  bool DesignDay::isMaximumDryBulbTemperatureDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isMaximumDryBulbTemperatureDefaulted();
  }

  double DesignDay::dailyDryBulbTemperatureRange() const {
    return getImpl<detail::DesignDay_Impl>()->dailyDryBulbTemperatureRange();
  }

  bool DesignDay::isDailyDryBulbTemperatureRangeDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isDailyDryBulbTemperatureRangeDefaulted();
  }

  double DesignDay::humidityIndicatingConditionsAtMaximumDryBulb() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use wetBulbOrDewPointAtMaximumDryBulb, humidityRatioAtMaximumDryBulb, or enthalpyAtMaximumDryBulb instead.");
    boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::HumidityConditionType, true);
    OS_ASSERT(value);
    std::string humidityConditionType = value.get();
    if (istringEqual(humidityConditionType, "WetBulb") || istringEqual(humidityConditionType, "DewPoint")
        || istringEqual(humidityConditionType, "WetBulbProfileMultiplierSchedule")
        || istringEqual(humidityConditionType, "WetBulbProfileDifferenceSchedule")
        || istringEqual(humidityConditionType, "WetBulbProfileDefaultMultipliers")) {
      return getImpl<detail::DesignDay_Impl>()->wetBulbOrDewPointAtMaximumDryBulb().get();
    } else if (istringEqual(humidityConditionType, "HumidityRatio")) {
      return getImpl<detail::DesignDay_Impl>()->humidityRatioAtMaximumDryBulb().get();
    } else if (istringEqual(humidityConditionType, "Enthalpy")) {
      return getImpl<detail::DesignDay_Impl>()->enthalpyAtMaximumDryBulb().get();
    }
    return 0.0;
  }

  bool DesignDay::isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted() const {
    DEPRECATED_AT_MSG(
      3, 3, 0,
      "Check that None of these have a value: wetBulbOrDewPointAtMaximumDryBulb,humidityRatioAtMaximumDryBulb, enthalpyAtMaximumDryBulb instead.");
    bool wetBulbOrDewPointAtMaximumDryBulb = isEmpty(OS_SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb);
    bool humidityRatioAtMaximumDryBulb = isEmpty(OS_SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb);
    bool enthalpyAtMaximumDryBulb = isEmpty(OS_SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb);
    return wetBulbOrDewPointAtMaximumDryBulb && humidityRatioAtMaximumDryBulb && enthalpyAtMaximumDryBulb;
  }

  double DesignDay::barometricPressure() const {
    return getImpl<detail::DesignDay_Impl>()->barometricPressure();
  }

  bool DesignDay::isBarometricPressureDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isBarometricPressureDefaulted();
  }

  double DesignDay::windSpeed() const {
    return getImpl<detail::DesignDay_Impl>()->windSpeed();
  }

  bool DesignDay::isWindSpeedDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isWindSpeedDefaulted();
  }

  double DesignDay::windDirection() const {
    return getImpl<detail::DesignDay_Impl>()->windDirection();
  }

  bool DesignDay::isWindDirectionDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isWindDirectionDefaulted();
  }

  double DesignDay::skyClearness() const {
    return getImpl<detail::DesignDay_Impl>()->skyClearness();
  }

  bool DesignDay::isSkyClearnessDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isSkyClearnessDefaulted();
  }

  bool DesignDay::rainIndicator() const {
    return getImpl<detail::DesignDay_Impl>()->rainIndicator();
  }

  bool DesignDay::isRainIndicatorDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isRainIndicatorDefaulted();
  }

  bool DesignDay::snowIndicator() const {
    return getImpl<detail::DesignDay_Impl>()->snowIndicator();
  }

  bool DesignDay::isSnowIndicatorDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isSnowIndicatorDefaulted();
  }

  int DesignDay::dayOfMonth() const {
    return getImpl<detail::DesignDay_Impl>()->dayOfMonth();
  }

  bool DesignDay::isDayOfMonthDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isDayOfMonthDefaulted();
  }

  int DesignDay::month() const {
    return getImpl<detail::DesignDay_Impl>()->month();
  }

  bool DesignDay::isMonthDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isMonthDefaulted();
  }

  std::string DesignDay::dayType() const {
    return getImpl<detail::DesignDay_Impl>()->dayType();
  }

  bool DesignDay::isDayTypeDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isDayTypeDefaulted();
  }

  bool DesignDay::daylightSavingTimeIndicator() const {
    return getImpl<detail::DesignDay_Impl>()->daylightSavingTimeIndicator();
  }

  bool DesignDay::isDaylightSavingTimeIndicatorDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isDaylightSavingTimeIndicatorDefaulted();
  }

  std::string DesignDay::humidityIndicatingType() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use humidityConditionType instead.");
    return getImpl<detail::DesignDay_Impl>()->humidityConditionType();
  }

  std::string DesignDay::humidityConditionType() const {
    return getImpl<detail::DesignDay_Impl>()->humidityConditionType();
  }

  bool DesignDay::isHumidityIndicatingTypeDefaulted() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use isHumidityConditionTypeDefaulted instead.");
    return getImpl<detail::DesignDay_Impl>()->isHumidityConditionTypeDefaulted();
  }

  bool DesignDay::isHumidityConditionTypeDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isHumidityConditionTypeDefaulted();
  }

  boost::optional<ScheduleDay> DesignDay::humidityIndicatingDaySchedule() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use humidityConditionDaySchedule instead.");
    return getImpl<detail::DesignDay_Impl>()->humidityConditionDaySchedule();
  }

  boost::optional<ScheduleDay> DesignDay::humidityConditionDaySchedule() const {
    return getImpl<detail::DesignDay_Impl>()->humidityConditionDaySchedule();
  }

  boost::optional<double> DesignDay::wetBulbOrDewPointAtMaximumDryBulb() const {
    return getImpl<detail::DesignDay_Impl>()->wetBulbOrDewPointAtMaximumDryBulb();
  }

  boost::optional<double> DesignDay::humidityRatioAtMaximumDryBulb() const {
    return getImpl<detail::DesignDay_Impl>()->humidityRatioAtMaximumDryBulb();
  }

  boost::optional<double> DesignDay::enthalpyAtMaximumDryBulb() const {
    return getImpl<detail::DesignDay_Impl>()->enthalpyAtMaximumDryBulb();
  }

  std::string DesignDay::dryBulbTemperatureRangeModifierType() const {
    return getImpl<detail::DesignDay_Impl>()->dryBulbTemperatureRangeModifierType();
  }

  bool DesignDay::isDryBulbTemperatureRangeModifierTypeDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isDryBulbTemperatureRangeModifierTypeDefaulted();
  }

  boost::optional<ScheduleDay> DesignDay::dryBulbTemperatureRangeModifierSchedule() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use dryBulbTemperatureRangeModifierDaySchedule instead.");
    return getImpl<detail::DesignDay_Impl>()->dryBulbTemperatureRangeModifierDaySchedule();
  }

  boost::optional<ScheduleDay> DesignDay::dryBulbTemperatureRangeModifierDaySchedule() const {
    return getImpl<detail::DesignDay_Impl>()->dryBulbTemperatureRangeModifierDaySchedule();
  }

  std::string DesignDay::solarModelIndicator() const {
    return getImpl<detail::DesignDay_Impl>()->solarModelIndicator();
  }

  bool DesignDay::isSolarModelIndicatorDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isSolarModelIndicatorDefaulted();
  }

  boost::optional<ScheduleDay> DesignDay::beamSolarDaySchedule() const {
    return getImpl<detail::DesignDay_Impl>()->beamSolarDaySchedule();
  }

  boost::optional<ScheduleDay> DesignDay::diffuseSolarDaySchedule() const {
    return getImpl<detail::DesignDay_Impl>()->diffuseSolarDaySchedule();
  }

  double DesignDay::ashraeTaub() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use ashraeClearSkyOpticalDepthForBeamIrradiance instead.");
    return getImpl<detail::DesignDay_Impl>()->ashraeClearSkyOpticalDepthForBeamIrradiance();
  }

  double DesignDay::ashraeClearSkyOpticalDepthForBeamIrradiance() const {
    return getImpl<detail::DesignDay_Impl>()->ashraeClearSkyOpticalDepthForBeamIrradiance();
  }

  bool DesignDay::isAshraeTaubDefaulted() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted instead.");
    return getImpl<detail::DesignDay_Impl>()->isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted();
  }

  bool DesignDay::isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted();
  }

  double DesignDay::ashraeTaud() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use ashraeClearSkyOpticalDepthForDiffuseIrradiance instead.");
    return getImpl<detail::DesignDay_Impl>()->ashraeClearSkyOpticalDepthForDiffuseIrradiance();
  }

  double DesignDay::ashraeClearSkyOpticalDepthForDiffuseIrradiance() const {
    return getImpl<detail::DesignDay_Impl>()->ashraeClearSkyOpticalDepthForDiffuseIrradiance();
  }

  bool DesignDay::isAshraeTaudDefaulted() const {
    DEPRECATED_AT_MSG(3, 3, 0, "Use isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted instead.");
    return getImpl<detail::DesignDay_Impl>()->isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted();
  }

  bool DesignDay::isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted();
  }

  boost::optional<double> DesignDay::dailyWetBulbTemperatureRange() const {
    return getImpl<detail::DesignDay_Impl>()->dailyWetBulbTemperatureRange();
  }

  boost::optional<int> DesignDay::maximumNumberWarmupDays() const {
    return getImpl<detail::DesignDay_Impl>()->maximumNumberWarmupDays();
  }

  std::string DesignDay::beginEnvironmentResetMode() const {
    return getImpl<detail::DesignDay_Impl>()->beginEnvironmentResetMode();
  }

  bool DesignDay::isBeginEnvironmentResetModeDefaulted() const {
    return getImpl<detail::DesignDay_Impl>()->isBeginEnvironmentResetModeDefaulted();
  }

  bool DesignDay::setMaximumDryBulbTemperature(double maximumDryBulbTemperature) {
    return getImpl<detail::DesignDay_Impl>()->setMaximumDryBulbTemperature(maximumDryBulbTemperature);
  }

  void DesignDay::resetMaximumDryBulbTemperature() {
    getImpl<detail::DesignDay_Impl>()->resetMaximumDryBulbTemperature();
  }

  bool DesignDay::setDailyDryBulbTemperatureRange(double dailyDryBulbTemperatureRange) {
    return getImpl<detail::DesignDay_Impl>()->setDailyDryBulbTemperatureRange(dailyDryBulbTemperatureRange);
  }

  void DesignDay::resetDailyDryBulbTemperatureRange() {
    getImpl<detail::DesignDay_Impl>()->resetDailyDryBulbTemperatureRange();
  }

  bool DesignDay::setBarometricPressure(double barometricPressure) {
    return getImpl<detail::DesignDay_Impl>()->setBarometricPressure(barometricPressure);
  }

  void DesignDay::resetBarometricPressure() {
    getImpl<detail::DesignDay_Impl>()->resetBarometricPressure();
  }

  bool DesignDay::setWindSpeed(double windSpeed) {
    return getImpl<detail::DesignDay_Impl>()->setWindSpeed(windSpeed);
  }

  void DesignDay::resetWindSpeed() {
    getImpl<detail::DesignDay_Impl>()->resetWindSpeed();
  }

  bool DesignDay::setWindDirection(double windDirection) {
    return getImpl<detail::DesignDay_Impl>()->setWindDirection(windDirection);
  }

  void DesignDay::resetWindDirection() {
    getImpl<detail::DesignDay_Impl>()->resetWindDirection();
  }

  bool DesignDay::setSkyClearness(double skyClearness) {
    return getImpl<detail::DesignDay_Impl>()->setSkyClearness(skyClearness);
  }

  void DesignDay::resetSkyClearness() {
    getImpl<detail::DesignDay_Impl>()->resetSkyClearness();
  }

  bool DesignDay::setRainIndicator(bool rainIndicator) {
    return getImpl<detail::DesignDay_Impl>()->setRainIndicator(rainIndicator);
  }

  void DesignDay::resetRainIndicator() {
    getImpl<detail::DesignDay_Impl>()->resetRainIndicator();
  }

  bool DesignDay::setSnowIndicator(bool snowIndicator) {
    return getImpl<detail::DesignDay_Impl>()->setSnowIndicator(snowIndicator);
  }

  void DesignDay::resetSnowIndicator() {
    getImpl<detail::DesignDay_Impl>()->resetSnowIndicator();
  }

  bool DesignDay::setDayOfMonth(int dayOfMonth) {
    return getImpl<detail::DesignDay_Impl>()->setDayOfMonth(dayOfMonth);
  }

  void DesignDay::resetDayOfMonth() {
    getImpl<detail::DesignDay_Impl>()->resetDayOfMonth();
  }

  bool DesignDay::setMonth(int month) {
    return getImpl<detail::DesignDay_Impl>()->setMonth(month);
  }

  void DesignDay::resetMonth() {
    getImpl<detail::DesignDay_Impl>()->resetMonth();
  }

  bool DesignDay::setDayType(const std::string& dayType) {
    return getImpl<detail::DesignDay_Impl>()->setDayType(dayType);
  }

  void DesignDay::resetDayType() {
    getImpl<detail::DesignDay_Impl>()->resetDayType();
  }

  bool DesignDay::setDaylightSavingTimeIndicator(bool daylightSavingTimeIndicator) {
    return getImpl<detail::DesignDay_Impl>()->setDaylightSavingTimeIndicator(daylightSavingTimeIndicator);
  }

  void DesignDay::resetDaylightSavingTimeIndicator() {
    getImpl<detail::DesignDay_Impl>()->resetDaylightSavingTimeIndicator();
  }

  bool DesignDay::setHumidityIndicatingType(const std::string& humidityIndicatingType) {
    DEPRECATED_AT_MSG(3, 3, 0, "Use setHumidityConditionType instead.");
    return getImpl<detail::DesignDay_Impl>()->setHumidityConditionType(humidityIndicatingType);
  }

  bool DesignDay::setHumidityConditionType(const std::string& humidityConditionType) {
    return getImpl<detail::DesignDay_Impl>()->setHumidityConditionType(humidityConditionType);
  }

  void DesignDay::resetHumidityIndicatingType() {
    DEPRECATED_AT_MSG(3, 3, 0, "Use resetHumidityConditionType instead.");
    getImpl<detail::DesignDay_Impl>()->resetHumidityConditionType();
  }

  void DesignDay::resetHumidityConditionType() {
    getImpl<detail::DesignDay_Impl>()->resetHumidityConditionType();
  }

  bool DesignDay::setHumidityIndicatingDaySchedule(const ScheduleDay& schedule) {
    DEPRECATED_AT_MSG(3, 3, 0, "Use setHumidityConditionDaySchedule instead.");
    return getImpl<detail::DesignDay_Impl>()->setHumidityConditionDaySchedule(schedule);
  }

  bool DesignDay::setHumidityConditionDaySchedule(const ScheduleDay& schedule) {
    return getImpl<detail::DesignDay_Impl>()->setHumidityConditionDaySchedule(schedule);
  }

  void DesignDay::resetHumidityIndicatingDaySchedule() {
    DEPRECATED_AT_MSG(3, 3, 0, "Use resetHumidityConditionDaySchedule instead.");
    getImpl<detail::DesignDay_Impl>()->resetHumidityConditionDaySchedule();
  }

  void DesignDay::resetHumidityConditionDaySchedule() {
    getImpl<detail::DesignDay_Impl>()->resetHumidityConditionDaySchedule();
  }

  bool DesignDay::setHumidityIndicatingConditionsAtMaximumDryBulb(double humidityIndicatingConditionsAtMaximumDryBulb) {
    DEPRECATED_AT_MSG(3, 3, 0, "Use setWetBulbOrDewPointAtMaximumDryBulb, setHumidityRatioAtMaximumDryBulb, or setEnthalpyAtMaximumDryBulb instead.");
    boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::HumidityConditionType, true);
    OS_ASSERT(value);
    std::string humidityConditionType = value.get();
    if (istringEqual(humidityConditionType, "WetBulb") || istringEqual(humidityConditionType, "DewPoint")
        || istringEqual(humidityConditionType, "WetBulbProfileMultiplierSchedule")
        || istringEqual(humidityConditionType, "WetBulbProfileDifferenceSchedule")
        || istringEqual(humidityConditionType, "WetBulbProfileDefaultMultipliers")) {
      return getImpl<detail::DesignDay_Impl>()->setWetBulbOrDewPointAtMaximumDryBulb(humidityIndicatingConditionsAtMaximumDryBulb);
    } else if (istringEqual(humidityConditionType, "HumidityRatio")) {
      return getImpl<detail::DesignDay_Impl>()->setHumidityRatioAtMaximumDryBulb(humidityIndicatingConditionsAtMaximumDryBulb);
    } else if (istringEqual(humidityConditionType, "Enthalpy")) {
      return getImpl<detail::DesignDay_Impl>()->setEnthalpyAtMaximumDryBulb(humidityIndicatingConditionsAtMaximumDryBulb);
    }
    return false;
  }

  void DesignDay::resetHumidityIndicatingConditionsAtMaximumDryBulb() {
    DEPRECATED_AT_MSG(3, 3, 0,
                      "Use resetWetBulbOrDewPointAtMaximumDryBulb, resetHumidityRatioAtMaximumDryBulb, and resetEnthalpyAtMaximumDryBulb instead.");
    getImpl<detail::DesignDay_Impl>()->resetWetBulbOrDewPointAtMaximumDryBulb();
    getImpl<detail::DesignDay_Impl>()->resetHumidityRatioAtMaximumDryBulb();
    getImpl<detail::DesignDay_Impl>()->resetEnthalpyAtMaximumDryBulb();
  }

  bool DesignDay::setWetBulbOrDewPointAtMaximumDryBulb(double wetBulbOrDewPointAtMaximumDryBulb) {
    return getImpl<detail::DesignDay_Impl>()->setWetBulbOrDewPointAtMaximumDryBulb(wetBulbOrDewPointAtMaximumDryBulb);
  }

  void DesignDay::resetWetBulbOrDewPointAtMaximumDryBulb() {
    getImpl<detail::DesignDay_Impl>()->resetWetBulbOrDewPointAtMaximumDryBulb();
  }

  bool DesignDay::setHumidityRatioAtMaximumDryBulb(double humidityRatioAtMaximumDryBulb) {
    return getImpl<detail::DesignDay_Impl>()->setHumidityRatioAtMaximumDryBulb(humidityRatioAtMaximumDryBulb);
  }

  void DesignDay::resetHumidityRatioAtMaximumDryBulb() {
    getImpl<detail::DesignDay_Impl>()->resetHumidityRatioAtMaximumDryBulb();
  }

  bool DesignDay::setEnthalpyAtMaximumDryBulb(double enthalpyAtMaximumDryBulb) {
    return getImpl<detail::DesignDay_Impl>()->setEnthalpyAtMaximumDryBulb(enthalpyAtMaximumDryBulb);
  }

  void DesignDay::resetEnthalpyAtMaximumDryBulb() {
    getImpl<detail::DesignDay_Impl>()->resetEnthalpyAtMaximumDryBulb();
  }

  bool DesignDay::setDryBulbTemperatureRangeModifierType(const std::string& dryBulbTemperatureRangeModifierType) {
    return getImpl<detail::DesignDay_Impl>()->setDryBulbTemperatureRangeModifierType(dryBulbTemperatureRangeModifierType);
  }

  void DesignDay::resetDryBulbTemperatureRangeModifierType() {
    getImpl<detail::DesignDay_Impl>()->resetDryBulbTemperatureRangeModifierType();
  }

  bool DesignDay::setDryBulbTemperatureRangeModifierSchedule(const ScheduleDay& schedule) {
    DEPRECATED_AT_MSG(3, 3, 0, "Use setDryBulbTemperatureRangeModifierDaySchedule instead.");
    return getImpl<detail::DesignDay_Impl>()->setDryBulbTemperatureRangeModifierDaySchedule(schedule);
  }

  bool DesignDay::setDryBulbTemperatureRangeModifierDaySchedule(const ScheduleDay& schedule) {
    return getImpl<detail::DesignDay_Impl>()->setDryBulbTemperatureRangeModifierDaySchedule(schedule);
  }

  void DesignDay::resetDryBulbTemperatureRangeModifierSchedule() {
    DEPRECATED_AT_MSG(3, 3, 0, "Use resetDryBulbTemperatureRangeModifierDaySchedule instead.");
    getImpl<detail::DesignDay_Impl>()->resetDryBulbTemperatureRangeModifierDaySchedule();
  }

  void DesignDay::resetDryBulbTemperatureRangeModifierDaySchedule() {
    getImpl<detail::DesignDay_Impl>()->resetDryBulbTemperatureRangeModifierDaySchedule();
  }

  bool DesignDay::setSolarModelIndicator(const std::string& solarModelIndicator) {
    return getImpl<detail::DesignDay_Impl>()->setSolarModelIndicator(solarModelIndicator);
  }

  void DesignDay::resetSolarModelIndicator() {
    getImpl<detail::DesignDay_Impl>()->resetSolarModelIndicator();
  }

  bool DesignDay::setBeamSolarDaySchedule(const ScheduleDay& schedule) {
    return getImpl<detail::DesignDay_Impl>()->setBeamSolarDaySchedule(schedule);
  }

  void DesignDay::resetBeamSolarDaySchedule() {
    getImpl<detail::DesignDay_Impl>()->resetBeamSolarDaySchedule();
  }

  bool DesignDay::setDiffuseSolarDaySchedule(const ScheduleDay& schedule) {
    return getImpl<detail::DesignDay_Impl>()->setDiffuseSolarDaySchedule(schedule);
  }

  void DesignDay::resetDiffuseSolarDaySchedule() {
    getImpl<detail::DesignDay_Impl>()->resetDiffuseSolarDaySchedule();
  }

  bool DesignDay::setAshraeTaub(double ashraeTaub) {
    DEPRECATED_AT_MSG(3, 3, 0, "Use setAshraeClearSkyOpticalDepthForBeamIrradiance instead.");
    return getImpl<detail::DesignDay_Impl>()->setAshraeClearSkyOpticalDepthForBeamIrradiance(ashraeTaub);
  }

  bool DesignDay::setAshraeClearSkyOpticalDepthForBeamIrradiance(double ashraeClearSkyOpticalDepthForBeamIrradiance) {
    return getImpl<detail::DesignDay_Impl>()->setAshraeClearSkyOpticalDepthForBeamIrradiance(ashraeClearSkyOpticalDepthForBeamIrradiance);
  }

  void DesignDay::resetAshraeTaub() {
    DEPRECATED_AT_MSG(3, 3, 0, "Use resetAshraeClearSkyOpticalDepthForBeamIrradiance instead.");
    getImpl<detail::DesignDay_Impl>()->resetAshraeClearSkyOpticalDepthForBeamIrradiance();
  }

  void DesignDay::resetAshraeClearSkyOpticalDepthForBeamIrradiance() {
    getImpl<detail::DesignDay_Impl>()->resetAshraeClearSkyOpticalDepthForBeamIrradiance();
  }

  bool DesignDay::setAshraeTaud(double ashraeTaud) {
    DEPRECATED_AT_MSG(3, 3, 0, "Use setAshraeClearSkyOpticalDepthForDiffuseIrradiance instead.");
    return getImpl<detail::DesignDay_Impl>()->setAshraeClearSkyOpticalDepthForDiffuseIrradiance(ashraeTaud);
  }

  bool DesignDay::setAshraeClearSkyOpticalDepthForDiffuseIrradiance(double ashraeClearSkyOpticalDepthForDiffuseIrradiance) {
    return getImpl<detail::DesignDay_Impl>()->setAshraeClearSkyOpticalDepthForDiffuseIrradiance(ashraeClearSkyOpticalDepthForDiffuseIrradiance);
  }

  void DesignDay::resetAshraeTaud() {
    DEPRECATED_AT_MSG(3, 3, 0, "Use resetAshraeClearSkyOpticalDepthForDiffuseIrradiance instead.");
    getImpl<detail::DesignDay_Impl>()->resetAshraeClearSkyOpticalDepthForDiffuseIrradiance();
  }

  void DesignDay::resetAshraeClearSkyOpticalDepthForDiffuseIrradiance() {
    getImpl<detail::DesignDay_Impl>()->resetAshraeClearSkyOpticalDepthForDiffuseIrradiance();
  }

  bool DesignDay::setDailyWetBulbTemperatureRange(double dailyWetBulbTemperatureRange) {
    return getImpl<detail::DesignDay_Impl>()->setDailyWetBulbTemperatureRange(dailyWetBulbTemperatureRange);
  }

  void DesignDay::resetDailyWetBulbTemperatureRange() {
    getImpl<detail::DesignDay_Impl>()->resetDailyWetBulbTemperatureRange();
  }

  bool DesignDay::setMaximumNumberWarmupDays(int maximumNumberWarmupDays) {
    return getImpl<detail::DesignDay_Impl>()->setMaximumNumberWarmupDays(maximumNumberWarmupDays);
  }

  void DesignDay::resetMaximumNumberWarmupDays() {
    getImpl<detail::DesignDay_Impl>()->resetMaximumNumberWarmupDays();
  }

  bool DesignDay::setBeginEnvironmentResetMode(const std::string& beginEnvironmentResetMode) {
    return getImpl<detail::DesignDay_Impl>()->setBeginEnvironmentResetMode(beginEnvironmentResetMode);
  }

  void DesignDay::resetBeginEnvironmentResetMode() {
    getImpl<detail::DesignDay_Impl>()->resetBeginEnvironmentResetMode();
  }

}  // namespace model
}  // namespace openstudio
