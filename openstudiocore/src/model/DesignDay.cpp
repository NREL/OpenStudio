/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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


namespace openstudio {

namespace model  {

namespace detail {

  DesignDay_Impl::DesignDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SizingPeriod_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DesignDay::iddObjectType());
  }

  DesignDay_Impl::DesignDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DesignDay::iddObjectType());
  }

  DesignDay_Impl::DesignDay_Impl(const DesignDay_Impl& other,Model_Impl* model,bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& DesignDay_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DesignDay_Impl::iddObjectType() const {
    return DesignDay::iddObjectType();
  }

  double DesignDay_Impl::maximumDryBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::MaximumDryBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isMaximumDryBulbTemperatureDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::MaximumDryBulbTemperature);
  }

  double DesignDay_Impl::dailyDryBulbTemperatureRange() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isDailyDryBulbTemperatureRangeDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange);
  }

  double DesignDay_Impl::humidityIndicatingConditionsAtMaximumDryBulb() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::HumidityIndicatingConditionsatMaximumDryBulb,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::HumidityIndicatingConditionsatMaximumDryBulb);
  }

  double DesignDay_Impl::barometricPressure() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::BarometricPressure,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isBarometricPressureDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::BarometricPressure);
  }

  double DesignDay_Impl::windSpeed() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::WindSpeed,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isWindSpeedDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::WindSpeed);
  }

  double DesignDay_Impl::windDirection() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::WindDirection,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isWindDirectionDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::WindDirection);
  }

  double DesignDay_Impl::skyClearness() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::SkyClearness,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isSkyClearnessDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::SkyClearness);
  }

  bool DesignDay_Impl::rainIndicator() const {
    bool result(false);
    int i = getInt(OS_SizingPeriod_DesignDayFields::RainIndicator,true).get();
    if( i == 1 ) {
      result = true;
    }
    return result;
  }

  bool DesignDay_Impl::isRainIndicatorDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::RainIndicator);
  }

  bool DesignDay_Impl::snowIndicator() const {
    bool result(false);
    int i = getInt(OS_SizingPeriod_DesignDayFields::SnowIndicator,true).get();
    if( i == 1 ) {
      result = true;
    }
    return result;
  }

  bool DesignDay_Impl::isSnowIndicatorDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::SnowIndicator);
  }

  int DesignDay_Impl::dayOfMonth() const {
    boost::optional<int> value = getInt(OS_SizingPeriod_DesignDayFields::DayofMonth,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isDayOfMonthDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::DayofMonth);
  }

  int DesignDay_Impl::month() const {
    boost::optional<int> value = getInt(OS_SizingPeriod_DesignDayFields::Month,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isMonthDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::Month);
  }

  std::string DesignDay_Impl::dayType() const {
    boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::DayType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isDayTypeDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::DayType);
  }

  bool DesignDay_Impl::daylightSavingTimeIndicator() const {
    bool result(false);
    int i = getInt(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator,true).get();
    if( i == 1 ) {
      result = true;
    }
    return result;
  }

  bool DesignDay_Impl::isDaylightSavingTimeIndicatorDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator);
  }

  std::string DesignDay_Impl::humidityIndicatingType() const {
    boost::optional<std::string> value = getString(
        OS_SizingPeriod_DesignDayFields::HumidityIndicatingType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isHumidityIndicatingTypeDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::HumidityIndicatingType);
  }

  boost::optional<ScheduleDay> DesignDay_Impl::humidityIndicatingDaySchedule() const {
    return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(
        OS_SizingPeriod_DesignDayFields::HumidityIndicatingDayScheduleName);
  }

  std::string DesignDay_Impl::dryBulbTemperatureRangeModifierType() const {
    boost::optional<std::string> value = getString(
        OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isDryBulbTemperatureRangeModifierTypeDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType);
  }

  boost::optional<ScheduleDay> DesignDay_Impl::dryBulbTemperatureRangeModifierSchedule() const
  {
    return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierScheduleName);
  }

  std::string DesignDay_Impl::solarModelIndicator() const {
    boost::optional<std::string> value = getString(OS_SizingPeriod_DesignDayFields::SolarModelIndicator,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isSolarModelIndicatorDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::SolarModelIndicator);
  }

  boost::optional<ScheduleDay> DesignDay_Impl::beamSolarDaySchedule() const
  {
    return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(OS_SizingPeriod_DesignDayFields::BeamSolarDayScheduleName);
  }

  boost::optional<ScheduleDay> DesignDay_Impl::diffuseSolarDaySchedule() const
  {
    return this->getObject<ModelObject>().getModelObjectTarget<ScheduleDay>(OS_SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName);
  }

  double DesignDay_Impl::ashraeTaub() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::ASHRAETaub,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isAshraeTaubDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::ASHRAETaub);
  }

  double DesignDay_Impl::ashraeTaud() const {
    boost::optional<double> value = getDouble(OS_SizingPeriod_DesignDayFields::ASHRAETaud,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool DesignDay_Impl::isAshraeTaudDefaulted() const {
    return isEmpty(OS_SizingPeriod_DesignDayFields::ASHRAETaud);
  }

  boost::optional<double> DesignDay_Impl::dailyWetBulbTemperatureRange() const {
    return getDouble(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange,true);
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

  void DesignDay_Impl::setHumidityIndicatingConditionsAtMaximumDryBulb(double humidityIndicatingConditionsAtMaximumDryBulb) {
    bool result = setDouble(OS_SizingPeriod_DesignDayFields::HumidityIndicatingConditionsatMaximumDryBulb, humidityIndicatingConditionsAtMaximumDryBulb);
    OS_ASSERT(result);
  }

  void DesignDay_Impl::resetHumidityIndicatingConditionsAtMaximumDryBulb() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::HumidityIndicatingConditionsatMaximumDryBulb, "");
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
      return this->setInt(OS_SizingPeriod_DesignDayFields::RainIndicator,1);
    }
    else {
      return this->setInt(OS_SizingPeriod_DesignDayFields::RainIndicator,0);
    }
    return false;
  }

  void DesignDay_Impl::resetRainIndicator() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::RainIndicator, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setSnowIndicator(bool snowIndicator) {
    if (snowIndicator) {
      return this->setInt(OS_SizingPeriod_DesignDayFields::SnowIndicator,1);
    }
    else {
      return this->setInt(OS_SizingPeriod_DesignDayFields::SnowIndicator,0);
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

  bool DesignDay_Impl::setDayType(std::string dayType) {
    bool result = setString(OS_SizingPeriod_DesignDayFields::DayType, dayType);
    return result;
  }

  void DesignDay_Impl::resetDayType() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::DayType, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setDaylightSavingTimeIndicator(bool daylightSavingTimeIndicator) {
    if (daylightSavingTimeIndicator) {
      return this->setInt(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator,1);
    }
    else {
      return this->setInt(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator,0);
    }
    return false;
  }

  void DesignDay_Impl::resetDaylightSavingTimeIndicator() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setHumidityIndicatingType(std::string humidityIndicatingType) {
    bool result = setString(OS_SizingPeriod_DesignDayFields::HumidityIndicatingType, humidityIndicatingType);
    return result;
  }

  void DesignDay_Impl::resetHumidityIndicatingType() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::HumidityIndicatingType, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setHumidityIndicatingDaySchedule(const ScheduleDay& schedule) {
    return this->getObject<ModelObject>().setPointer(
        OS_SizingPeriod_DesignDayFields::HumidityIndicatingDayScheduleName,schedule.handle());
  }

  void DesignDay_Impl::resetHumidityIndicatingDaySchedule() {
    this->setString(OS_SizingPeriod_DesignDayFields::HumidityIndicatingDayScheduleName,"");
  }

  bool DesignDay_Impl::setDryBulbTemperatureRangeModifierType(std::string dryBulbTemperatureRangeModifierType) {
    bool result = setString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, dryBulbTemperatureRangeModifierType);
    return result;
  }

  void DesignDay_Impl::resetDryBulbTemperatureRangeModifierType() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setDryBulbTemperatureRangeModifierSchedule(const ScheduleDay& schedule) {
    return this->getObject<ModelObject>().setPointer(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierScheduleName,schedule.handle());
  }

  void DesignDay_Impl::resetDryBulbTemperatureRangeModifierSchedule() {
    this->setString(OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierScheduleName,"");
  }

  bool DesignDay_Impl::setSolarModelIndicator(std::string solarModelIndicator) {
    bool result = setString(OS_SizingPeriod_DesignDayFields::SolarModelIndicator, solarModelIndicator);
    return result;
  }

  void DesignDay_Impl::resetSolarModelIndicator() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::SolarModelIndicator, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setBeamSolarDaySchedule(const ScheduleDay & schedule) {
    return this->setPointer(OS_SizingPeriod_DesignDayFields::BeamSolarDayScheduleName,schedule.handle());
  }

  void DesignDay_Impl::resetBeamSolarDaySchedule() {
    this->setString(OS_SizingPeriod_DesignDayFields::BeamSolarDayScheduleName,"");
  }

  bool DesignDay_Impl::setDiffuseSolarDaySchedule(const ScheduleDay & schedule) {
    return this->setPointer(OS_SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName,schedule.handle());
  }

  void DesignDay_Impl::resetDiffuseSolarDaySchedule() {
    this->setString(OS_SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName,"");
  }

  boost::optional<ModelObject> DesignDay_Impl::humidityIndicatingDayScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalScheduleDay schedule = humidityIndicatingDaySchedule();
    if (schedule) {
      result = *schedule;
    }
    return result;
  }

  boost::optional<ModelObject>
  DesignDay_Impl::dryBulbTemperatureRangeModifierScheduleAsModelObject() const
  {
    OptionalModelObject result;
    OptionalScheduleDay schedule = dryBulbTemperatureRangeModifierSchedule();
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

  bool DesignDay_Impl::setHumidityIndicatingDayScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject)
  {
    if (modelObject) {
      OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
      if (schedule) {
        return setHumidityIndicatingDaySchedule(*schedule);
      }
      return false;
    }
    else {
      resetHumidityIndicatingDaySchedule();
    }
    return true;
  }

  bool DesignDay_Impl::setDryBulbTemperatureRangeModifierScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject)
  {
    if (modelObject) {
      OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
      if (schedule) {
        return setDryBulbTemperatureRangeModifierSchedule(*schedule);
      }
      return false;
    }
    else {
      resetDryBulbTemperatureRangeModifierSchedule();
    }
    return true;
  }

  bool DesignDay_Impl::setBeamSolarDayScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject)
  {
    if (modelObject) {
      OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
      if (schedule) {
        return setBeamSolarDaySchedule(*schedule);
      }
      return false;
    }
    else {
      resetBeamSolarDaySchedule();
    }
    return true;
  }

  bool DesignDay_Impl::setDiffuseSolarDayScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject)
  {
    if (modelObject) {
      OptionalScheduleDay schedule = modelObject->optionalCast<ScheduleDay>();
      if (schedule) {
        return setDiffuseSolarDaySchedule(*schedule);
      }
      return false;
    }
    else {
      resetDiffuseSolarDaySchedule();
    }
    return true;
  }

  bool DesignDay_Impl::setAshraeTaub(double ashraeTaub) {
    bool result = setDouble(OS_SizingPeriod_DesignDayFields::ASHRAETaub, ashraeTaub);
    return result;
  }

  void DesignDay_Impl::resetAshraeTaub() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::ASHRAETaub, "");
    OS_ASSERT(result);
  }

  bool DesignDay_Impl::setAshraeTaud(double ashraeTaud) {
    bool result = setDouble(OS_SizingPeriod_DesignDayFields::ASHRAETaud, ashraeTaud);
    return result;
  }

  void DesignDay_Impl::resetAshraeTaud() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::ASHRAETaud, "");
    OS_ASSERT(result);
  }

  void DesignDay_Impl::setDailyWetBulbTemperatureRange(boost::optional<double> dailyWetBulbTemperatureRange) {
    bool result = false;
    if (dailyWetBulbTemperatureRange) {
      result = setDouble(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, dailyWetBulbTemperatureRange.get());
    } else {
      result = setString(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, "");
    }
    OS_ASSERT(result);
  }

  void DesignDay_Impl::resetDailyWetBulbTemperatureRange() {
    bool result = setString(OS_SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, "");
    OS_ASSERT(result);
  }

  void DesignDay_Impl::ensureNoLeapDays()
  {
    boost::optional<int> month;
    boost::optional<int> day;

    month = getInt(OS_SizingPeriod_DesignDayFields::Month);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_SizingPeriod_DesignDayFields::DayofMonth);
      if (day && (day.get() == 29)){
        this->setInt(OS_SizingPeriod_DesignDayFields::DayofMonth, 28);
      }
    }
  }

} // detail

/// constructor
DesignDay::DesignDay(const Model& model)
  : SizingPeriod(DesignDay::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DesignDay_Impl>());

  setMaximumDryBulbTemperature(23.0);
  setDailyDryBulbTemperatureRange(0.0);
  setHumidityIndicatingConditionsAtMaximumDryBulb(23.0);
  setBarometricPressure(31000);
  setWindSpeed(0.0);
  setWindDirection(0.0);
  setSkyClearness(0.0);
  setSnowIndicator(0);
  setDayOfMonth(1);
  setMonth(1);
  setDayType("SummerDesignDay");
  setDaylightSavingTimeIndicator(false);
  setHumidityIndicatingType("WetBulb");
  setDryBulbTemperatureRangeModifierType("DefaultMultipliers");
  setSolarModelIndicator("ASHRAEClearSky");
}

// constructor
DesignDay::DesignDay(std::shared_ptr<detail::DesignDay_Impl> impl)
  : SizingPeriod(std::move(impl))
{}

IddObjectType DesignDay::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SizingPeriod_DesignDay);
  return result;
}

std::vector<std::string> DesignDay::validDayTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SizingPeriod_DesignDayFields::DayType);
}

std::vector<std::string> DesignDay::validHumidityIndicatingTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SizingPeriod_DesignDayFields::HumidityIndicatingType);
}

std::vector<std::string> DesignDay::validDryBulbTemperatureRangeModifierTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType);
}

std::vector<std::string> DesignDay::validSolarModelIndicatorValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SizingPeriod_DesignDayFields::SolarModelIndicator);
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
  return getImpl<detail::DesignDay_Impl>()->humidityIndicatingConditionsAtMaximumDryBulb();
}

bool DesignDay::isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted() const {
  return getImpl<detail::DesignDay_Impl>()->isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted();
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
  return getImpl<detail::DesignDay_Impl>()->humidityIndicatingType();
}

bool DesignDay::isHumidityIndicatingTypeDefaulted() const {
  return getImpl<detail::DesignDay_Impl>()->isHumidityIndicatingTypeDefaulted();
}

boost::optional<ScheduleDay> DesignDay::humidityIndicatingDaySchedule() const {
  return getImpl<detail::DesignDay_Impl>()->humidityIndicatingDaySchedule();
}

std::string DesignDay::dryBulbTemperatureRangeModifierType() const {
  return getImpl<detail::DesignDay_Impl>()->dryBulbTemperatureRangeModifierType();
}

bool DesignDay::isDryBulbTemperatureRangeModifierTypeDefaulted() const {
  return getImpl<detail::DesignDay_Impl>()->isDryBulbTemperatureRangeModifierTypeDefaulted();
}

boost::optional<ScheduleDay> DesignDay::dryBulbTemperatureRangeModifierSchedule() const {
  return getImpl<detail::DesignDay_Impl>()->dryBulbTemperatureRangeModifierSchedule();
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
  return getImpl<detail::DesignDay_Impl>()->ashraeTaub();
}

bool DesignDay::isAshraeTaubDefaulted() const {
  return getImpl<detail::DesignDay_Impl>()->isAshraeTaubDefaulted();
}

double DesignDay::ashraeTaud() const {
  return getImpl<detail::DesignDay_Impl>()->ashraeTaud();
}

bool DesignDay::isAshraeTaudDefaulted() const {
  return getImpl<detail::DesignDay_Impl>()->isAshraeTaudDefaulted();
}

boost::optional<double> DesignDay::dailyWetBulbTemperatureRange() const {
  return getImpl<detail::DesignDay_Impl>()->dailyWetBulbTemperatureRange();
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

void DesignDay::setHumidityIndicatingConditionsAtMaximumDryBulb(double humidityIndicatingConditionsAtMaximumDryBulb)
{
  getImpl<detail::DesignDay_Impl>()->setHumidityIndicatingConditionsAtMaximumDryBulb(humidityIndicatingConditionsAtMaximumDryBulb);
}

void DesignDay::resetHumidityIndicatingConditionsAtMaximumDryBulb() {
  getImpl<detail::DesignDay_Impl>()->resetHumidityIndicatingConditionsAtMaximumDryBulb();
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

bool DesignDay::setDayType(std::string dayType) {
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

bool DesignDay::setHumidityIndicatingType(std::string humidityIndicatingType) {
  return getImpl<detail::DesignDay_Impl>()->setHumidityIndicatingType(humidityIndicatingType);
}

void DesignDay::resetHumidityIndicatingType() {
  getImpl<detail::DesignDay_Impl>()->resetHumidityIndicatingType();
}

bool DesignDay::setHumidityIndicatingDaySchedule(const ScheduleDay& schedule) {
  return getImpl<detail::DesignDay_Impl>()->setHumidityIndicatingDaySchedule(schedule);
}

void DesignDay::resetHumidityIndicatingDaySchedule() {
  getImpl<detail::DesignDay_Impl>()->resetHumidityIndicatingDaySchedule();
}

bool DesignDay::setDryBulbTemperatureRangeModifierType(std::string dryBulbTemperatureRangeModifierType) {
  return getImpl<detail::DesignDay_Impl>()->setDryBulbTemperatureRangeModifierType(dryBulbTemperatureRangeModifierType);
}

void DesignDay::resetDryBulbTemperatureRangeModifierType() {
  getImpl<detail::DesignDay_Impl>()->resetDryBulbTemperatureRangeModifierType();
}

bool DesignDay::setDryBulbTemperatureRangeModifierSchedule(const ScheduleDay& schedule) {
  return getImpl<detail::DesignDay_Impl>()->setDryBulbTemperatureRangeModifierSchedule(schedule);
}

void DesignDay::resetDryBulbTemperatureRangeModifierSchedule() {
  getImpl<detail::DesignDay_Impl>()->resetDryBulbTemperatureRangeModifierSchedule();
}

bool DesignDay::setSolarModelIndicator(std::string solarModelIndicator) {
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
  return getImpl<detail::DesignDay_Impl>()->setAshraeTaub(ashraeTaub);
}

void DesignDay::resetAshraeTaub() {
  getImpl<detail::DesignDay_Impl>()->resetAshraeTaub();
}

bool DesignDay::setAshraeTaud(double ashraeTaud) {
  return getImpl<detail::DesignDay_Impl>()->setAshraeTaud(ashraeTaud);
}

void DesignDay::resetAshraeTaud() {
  getImpl<detail::DesignDay_Impl>()->resetAshraeTaud();
}

void DesignDay::setDailyWetBulbTemperatureRange(double dailyWetBulbTemperatureRange) {
  getImpl<detail::DesignDay_Impl>()->setDailyWetBulbTemperatureRange(dailyWetBulbTemperatureRange);
}

void DesignDay::resetDailyWetBulbTemperatureRange() {
  getImpl<detail::DesignDay_Impl>()->resetDailyWetBulbTemperatureRange();
}

} // model
} // openstudio
