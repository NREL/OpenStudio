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

#ifndef MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class AvailabilityManagerNightVentilation_Impl;

} // detail

/** AvailabilityManagerNightVentilation is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:NightVentilation'. */
class MODEL_API AvailabilityManagerNightVentilation : public AvailabilityManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AvailabilityManagerNightVentilation(const Model& model);

  virtual ~AvailabilityManagerNightVentilation() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule applicabilitySchedule() const;

  boost::optional<Schedule> ventilationTemperatureSchedule() const;

  double ventilationTemperatureDifference() const;

  double ventilationTemperatureLowLimit() const;

  double nightVentingFlowFraction() const;

  boost::optional<ThermalZone> controlZone() const;

  //@}
  /** @name Setters */
  //@{

  bool setApplicabilitySchedule(Schedule& schedule);

  bool setVentilationTemperatureSchedule(Schedule& schedule);

  void resetVentilationTemperatureSchedule();

  void setVentilationTemperatureDifference(double ventilationTemperatureDifference);

  void setVentilationTemperatureLowLimit(double ventilationTemperatureLowLimit);

  bool setNightVentingFlowFraction(double nightVentingFlowFraction);

  bool setControlZone(const ThermalZone& thermalZone);

  void resetControlZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AvailabilityManagerNightVentilation_Impl ImplType;

  explicit AvailabilityManagerNightVentilation(std::shared_ptr<detail::AvailabilityManagerNightVentilation_Impl> impl);

  friend class detail::AvailabilityManagerNightVentilation_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightVentilation");
};

/** \relates AvailabilityManagerNightVentilation*/
typedef boost::optional<AvailabilityManagerNightVentilation> OptionalAvailabilityManagerNightVentilation;

/** \relates AvailabilityManagerNightVentilation*/
typedef std::vector<AvailabilityManagerNightVentilation> AvailabilityManagerNightVentilationVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_HPP

