/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class AvailabilityManagerNightCycle_Impl;

} // detail

/** AvailabilityManagerNightCycle is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:NightCycle'. */
class MODEL_API AvailabilityManagerNightCycle : public ModelObject {
 public:

  explicit AvailabilityManagerNightCycle(const Model& model);

  virtual ~AvailabilityManagerNightCycle() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlTypeValues();


  std::string controlType() const;

  bool setControlType(std::string controlType);

  void resetControlType();

  bool isControlTypeDefaulted() const;


  double thermostatTolerance() const;

  Quantity getThermostatTolerance(bool returnIP=false) const;

  bool setThermostatTolerance(const Quantity& thermostatTolerance);

  void setThermostatTolerance(double thermostatTolerance);

  bool isThermostatToleranceDefaulted() const;

  void resetThermostatTolerance();


  double cyclingRunTime() const;

  void setCyclingRunTime(double cyclingRunTime);

  bool setCyclingRunTime(const Quantity& cyclingRunTime);

  void resetCyclingRunTime();

  Quantity getCyclingRunTime(bool returnIP=false) const;

  bool isCyclingRunTimeDefaulted() const;


  boost::optional<ThermalZone> controlThermalZone() const;

  bool setControlThermalZone(const ThermalZone& thermalZone);

  void resetControlThermalZone();

 protected:

  /// @cond
  typedef detail::AvailabilityManagerNightCycle_Impl ImplType;

  explicit AvailabilityManagerNightCycle(std::shared_ptr<detail::AvailabilityManagerNightCycle_Impl> impl);

  friend class detail::AvailabilityManagerNightCycle_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");
};

/** \relates AvailabilityManagerNightCycle*/
typedef boost::optional<AvailabilityManagerNightCycle> OptionalAvailabilityManagerNightCycle;

/** \relates AvailabilityManagerNightCycle*/
typedef std::vector<AvailabilityManagerNightCycle> AvailabilityManagerNightCycleVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP

