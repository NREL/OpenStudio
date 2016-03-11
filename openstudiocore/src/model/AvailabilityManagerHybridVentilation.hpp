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

#ifndef MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP
#define MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

class ThermalZone;
class Schedule;
class Curve;

namespace detail {

  class AvailabilityManagerHybridVentilation_Impl;

} // detail

/** AvailabilityManagerHybridVentilation is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:HybridVentilation'. */
class MODEL_API AvailabilityManagerHybridVentilation : public AvailabilityManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  // Constructor for AvailabilityManagerHybridVentilation.
  // The required "ventilationControlModeSchedule" will be initialized with 
  // a new schedule having a constant value of one, meaning the availability manager
  // will operate with temperature control.
  // The required "minimumOutdoorVentilationAirSchedule" will be initialized with a 
  // schedule of constant value 0. This schedule is only applicable if control mode is 0,
  // but EnergyPlus requires it to operate.
  explicit AvailabilityManagerHybridVentilation(const Model& model);

  // Constructor for AvailabilityManagerHybridVentilation.
  // The required "ventilationControlModeSchedule" must be provided.
  // The required "minimumOutdoorVentilationAirSchedule" must be provided.
  explicit AvailabilityManagerHybridVentilation(const Model& model, Schedule& ventilationControlModeSchedule, Schedule& minimumOutdoorVentilationAirSchedule);

  virtual ~AvailabilityManagerHybridVentilation() {}

  //@}

  static IddObjectType iddObjectType();

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

  bool setControlledZone(const ThermalZone& thermalZone);

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

  bool setOpeningFactorFunctionofWindSpeedCurve(const Curve& curve);

  void resetOpeningFactorFunctionofWindSpeedCurve();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AvailabilityManagerHybridVentilation_Impl ImplType;

  explicit AvailabilityManagerHybridVentilation(std::shared_ptr<detail::AvailabilityManagerHybridVentilation_Impl> impl);

  friend class detail::AvailabilityManagerHybridVentilation_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerHybridVentilation");
};

/** \relates AvailabilityManagerHybridVentilation*/
typedef boost::optional<AvailabilityManagerHybridVentilation> OptionalAvailabilityManagerHybridVentilation;

/** \relates AvailabilityManagerHybridVentilation*/
typedef std::vector<AvailabilityManagerHybridVentilation> AvailabilityManagerHybridVentilationVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP

