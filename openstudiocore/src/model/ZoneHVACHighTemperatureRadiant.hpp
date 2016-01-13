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

#ifndef MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP
#define MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Surface;

namespace detail {

  class ZoneHVACHighTemperatureRadiant_Impl;

} // detail

/** ZoneHVACHighTemperatureRadiant is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:HighTemperatureRadiant'. */
class MODEL_API ZoneHVACHighTemperatureRadiant : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACHighTemperatureRadiant(const Model& model);

  virtual ~ZoneHVACHighTemperatureRadiant() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fuelTypeValues();

  static std::vector<std::string> temperatureControlTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  /** In EnergyPlus 8.2.0 and above this property maps to the EnergyPlus field "Heating Design Capacity" **/
  boost::optional<double> maximumPowerInput() const;

  bool isMaximumPowerInputAutosized() const;

  std::string fuelType() const;

  double combustionEfficiency() const;

  double fractionofInputConvertedtoRadiantEnergy() const;

  double fractionofInputConvertedtoLatentEnergy() const;

  double fractionofInputthatIsLost() const;

  std::string temperatureControlType() const;

  double heatingThrottlingRange() const;

  boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

  double fractionofRadiantEnergyIncidentonPeople() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumPowerInput(double maximumPowerInput);

  void autosizeMaximumPowerInput();

  bool setFuelType(std::string fuelType);

  bool setCombustionEfficiency(double combustionEfficiency);

  bool setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy);

  bool setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy);

  bool setFractionofInputthatIsLost(double fractionofInputthatIsLost);

  bool setTemperatureControlType(std::string temperatureControlType);

  bool setHeatingThrottlingRange(double heatingThrottlingRange);

  bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

  void resetHeatingSetpointTemperatureSchedule();

  bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

  //@}
  /** @name Other */
  //@{

  boost::optional<ThermalZone> thermalZone() const;

  bool addToThermalZone(ThermalZone & thermalZone);

  void removeFromThermalZone();
  //@}

 protected:

  /// @cond
  typedef detail::ZoneHVACHighTemperatureRadiant_Impl ImplType;

  explicit ZoneHVACHighTemperatureRadiant(std::shared_ptr<detail::ZoneHVACHighTemperatureRadiant_Impl> impl);

  friend class detail::ZoneHVACHighTemperatureRadiant_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACHighTemperatureRadiant");
};

/** \relates ZoneHVACHighTemperatureRadiant*/
typedef boost::optional<ZoneHVACHighTemperatureRadiant> OptionalZoneHVACHighTemperatureRadiant;

/** \relates ZoneHVACHighTemperatureRadiant*/
typedef std::vector<ZoneHVACHighTemperatureRadiant> ZoneHVACHighTemperatureRadiantVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACHIGHTEMPERATURERADIANT_HPP

