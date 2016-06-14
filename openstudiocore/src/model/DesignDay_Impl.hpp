/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_DESIGNDAY_IMPL_HPP
#define MODEL_DESIGNDAY_IMPL_HPP

#include "SizingPeriod_Impl.hpp"

namespace openstudio {
namespace model {

class ScheduleDay;

namespace detail {

  class MODEL_API DesignDay_Impl : public SizingPeriod_Impl {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    
    
    
   public:

    // constructor
    DesignDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    DesignDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                   Model_Impl* model, 
                   bool keepHandle);

    // clone copy constructor
    DesignDay_Impl(const DesignDay_Impl& other,Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~DesignDay_Impl(){}

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    /** @name Getters */
    //@{

    double maximumDryBulbTemperature() const;

    bool isMaximumDryBulbTemperatureDefaulted() const;

    double dailyDryBulbTemperatureRange() const;

    bool isDailyDryBulbTemperatureRangeDefaulted() const;

    double humidityIndicatingConditionsAtMaximumDryBulb() const;

    bool isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted() const;

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

    std::string humidityIndicatingType() const;

    bool isHumidityIndicatingTypeDefaulted() const;

    boost::optional<ScheduleDay> humidityIndicatingDaySchedule() const;

    std::string dryBulbTemperatureRangeModifierType() const;

    bool isDryBulbTemperatureRangeModifierTypeDefaulted() const;

    boost::optional<ScheduleDay> dryBulbTemperatureRangeModifierSchedule() const;

    std::string solarModelIndicator() const;

    bool isSolarModelIndicatorDefaulted() const;

    boost::optional<ScheduleDay> beamSolarDaySchedule() const;

    boost::optional<ScheduleDay> diffuseSolarDaySchedule() const;

    double ashraeTaub() const;

    bool isAshraeTaubDefaulted() const;

    double ashraeTaud() const;

    bool isAshraeTaudDefaulted() const;

    boost::optional<double> dailyWetBulbTemperatureRange() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumDryBulbTemperature(double maximumDryBulbTemperature);

    void resetMaximumDryBulbTemperature();

    bool setDailyDryBulbTemperatureRange(double dailyDryBulbTemperatureRange);

    void resetDailyDryBulbTemperatureRange();

    void setHumidityIndicatingConditionsAtMaximumDryBulb(double humidityIndicatingConditionsAtMaximumDryBulb);

    void resetHumidityIndicatingConditionsAtMaximumDryBulb();

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

    bool setDayType(std::string dayType);

    void resetDayType();

    bool setDaylightSavingTimeIndicator(bool daylightSavingTimeIndicator);

    void resetDaylightSavingTimeIndicator();

    bool setHumidityIndicatingType(std::string humidityIndicatingType);

    void resetHumidityIndicatingType();

    bool setHumidityIndicatingDaySchedule(const ScheduleDay& schedule);

    void resetHumidityIndicatingDaySchedule();

    bool setDryBulbTemperatureRangeModifierType(std::string dryBulbTemperatureRangeModifierType);

    void resetDryBulbTemperatureRangeModifierType();

    bool setDryBulbTemperatureRangeModifierSchedule(const ScheduleDay & schedule);

    void resetDryBulbTemperatureRangeModifierSchedule();

    bool setSolarModelIndicator(std::string solarModelIndicator);

    void resetSolarModelIndicator();

    bool setBeamSolarDaySchedule(const ScheduleDay & schedule);

    void resetBeamSolarDaySchedule();

    bool setDiffuseSolarDaySchedule(const ScheduleDay & schedule);

    void resetDiffuseSolarDaySchedule();

    bool setAshraeTaub(double ashraeTaub);

    void resetAshraeTaub();

    bool setAshraeTaud(double ashraeTaud);

    void resetAshraeTaud();

    void setDailyWetBulbTemperatureRange(boost::optional<double> dailyWetBulbTemperatureRange);

    void resetDailyWetBulbTemperatureRange();

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays() override;

    //@}
  
   private:

    REGISTER_LOGGER("openstudio.model.DesignDay");

    boost::optional<ModelObject> humidityIndicatingDayScheduleAsModelObject() const;
    boost::optional<ModelObject> dryBulbTemperatureRangeModifierScheduleAsModelObject() const;
    boost::optional<ModelObject> beamSolarDayScheduleAsModelObject() const;
    boost::optional<ModelObject> diffuseSolarDayScheduleAsModelObject() const;

    bool setHumidityIndicatingDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDryBulbTemperatureRangeModifierScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBeamSolarDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDiffuseSolarDayScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DESIGNDAY_IMPL_HPP
