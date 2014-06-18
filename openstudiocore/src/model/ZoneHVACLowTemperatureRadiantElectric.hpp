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

#ifndef MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP
#define MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACLowTemperatureRadiantElectric_Impl;

} // detail

/** ZoneHVACLowTemperatureRadiantElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:Electric'. */
class MODEL_API ZoneHVACLowTemperatureRadiantElectric : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACLowTemperatureRadiantElectric(const Model& model, Schedule & availabilitySchedule, Schedule & heatingTemperatureSchedule);

  virtual ~ZoneHVACLowTemperatureRadiantElectric() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> radiantSurfaceTypeValues();

  static std::vector<std::string> temperatureControlTypeValues();

  /** @name Getters */

  Schedule availabilitySchedule() const;
  
  Schedule heatingSetpointTemperatureSchedule() const; 

  boost::optional<std::string> radiantSurfaceType() const;

  std::vector<Surface> surfaces() const;

  boost::optional<double> maximumElectricalPowertoPanel() const;

  bool isMaximumElectricalPowertoPanelDefaulted() const;

  bool isMaximumElectricalPowertoPanelAutosized() const;

  std::string temperatureControlType() const;

  bool isTemperatureControlTypeDefaulted() const;

  double heatingThrottlingRange() const;

  bool isHeatingThrottlingRangeDefaulted() const;

  /** @name Setters */

  bool setAvailabilitySchedule(Schedule& schedule);
  
  bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

  bool setRadiantSurfaceType(std::string radiantSurfaceType);

  void resetRadiantSurfaceType();

  bool setMaximumElectricalPowertoPanel(double maximumElectricalPowertoPanel);

  void resetMaximumElectricalPowertoPanel();

  void autosizeMaximumElectricalPowertoPanel();

  bool setTemperatureControlType(std::string temperatureControlType);

  void resetTemperatureControlType();

  bool setHeatingThrottlingRange(double heatingThrottlingRange);

  void resetHeatingThrottlingRange();
  
  boost::optional<ThermalZone> thermalZone() const;

  bool addToThermalZone(ThermalZone & thermalZone);

  void removeFromThermalZone();


  /** @name Other */

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACLowTemperatureRadiantElectric_Impl ImplType;

  explicit ZoneHVACLowTemperatureRadiantElectric(std::shared_ptr<detail::ZoneHVACLowTemperatureRadiantElectric_Impl> impl);

  friend class detail::ZoneHVACLowTemperatureRadiantElectric_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACLowTemperatureRadiantElectric");
};

/** \relates ZoneHVACLowTemperatureRadiantElectric*/
typedef boost::optional<ZoneHVACLowTemperatureRadiantElectric> OptionalZoneHVACLowTemperatureRadiantElectric;

/** \relates ZoneHVACLowTemperatureRadiantElectric*/
typedef std::vector<ZoneHVACLowTemperatureRadiantElectric> ZoneHVACLowTemperatureRadiantElectricVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP

