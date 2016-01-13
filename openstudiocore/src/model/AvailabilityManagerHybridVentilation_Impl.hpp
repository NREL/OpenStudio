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

#ifndef MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;
class Schedule;
class Curve;

namespace detail {

  /** AvailabilityManagerHybridVentilation_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerHybridVentilation.*/
  class MODEL_API AvailabilityManagerHybridVentilation_Impl : public AvailabilityManager_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AvailabilityManagerHybridVentilation_Impl(const IdfObject& idfObject,
                                              Model_Impl* model,
                                              bool keepHandle);

    AvailabilityManagerHybridVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    AvailabilityManagerHybridVentilation_Impl(const AvailabilityManagerHybridVentilation_Impl& other,
                                              Model_Impl* model,
                                              bool keepHandle);

    virtual ~AvailabilityManagerHybridVentilation_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<ThermalZone> controlledZone() const;

    Schedule ventilationControlModeSchedule() const;

    bool useWeatherFileRainIndicators() const;

    double maximumWindSpeed() const;

    double minimumOutdoorTemperature() const;

    double maximumOutdoorTemperature() const;

    double minimumOutdoorEnthalpy() const;

    double maximumOutdoorEnthalpy() const;

    double minimumOutdoorDewpoint() const;

    double maximumOutdoorDewpoint() const;

    Schedule minimumOutdoorVentilationAirSchedule() const;

    boost::optional<Curve> openingFactorFunctionofWindSpeedCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlledZone(const boost::optional<ThermalZone>& thermalZone);

    void resetControlledZone();

    bool setVentilationControlModeSchedule(Schedule& schedule);

    void setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators);

    bool setMaximumWindSpeed(double maximumWindSpeed);

    bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

    bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

    bool setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy);

    bool setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy);

    bool setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint);

    bool setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint);

    bool setMinimumOutdoorVentilationAirSchedule(Schedule& schedule);

    bool setOpeningFactorFunctionofWindSpeedCurve(const boost::optional<Curve>& curve);

    void resetOpeningFactorFunctionofWindSpeedCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerHybridVentilation");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalVentilationControlModeSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP

