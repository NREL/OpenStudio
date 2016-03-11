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

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ZoneHVACBaseboardRadiantConvectiveElectric_Impl;

} // detail

/** ZoneHVACBaseboardRadiantConvectiveElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Electric'. */
class MODEL_API ZoneHVACBaseboardRadiantConvectiveElectric : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACBaseboardRadiantConvectiveElectric(const Model& model);

  virtual ~ZoneHVACBaseboardRadiantConvectiveElectric() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatingDesignCapacityMethodValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  std::string heatingDesignCapacityMethod() const;

  boost::optional<double> heatingDesignCapacity() const;

  bool isHeatingDesignCapacityAutosized() const;

  double heatingDesignCapacityPerFloorArea() const;

  double fractionofAutosizedHeatingDesignCapacity() const;

  double efficiency() const;

  double fractionRadiant() const;

  double fractionofRadiantEnergyIncidentonPeople() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod);

  bool setHeatingDesignCapacity(double heatingDesignCapacity);

  void autosizeHeatingDesignCapacity();

  bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

  bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

  bool setEfficiency(double efficiency);

  bool setFractionRadiant(double fractionRadiant);

  bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

  //@}
  /** @name Other */
  //@{

  /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveElectric is attached to
   **/
  boost::optional<ThermalZone> thermalZone();

  /** Adds this ZoneHVACBaseboardRadiantConvectiveElectric to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
  bool addToThermalZone(ThermalZone & thermalZone);

  /** Detaches this ZoneHVACBaseboardRadiantConvectiveElectric from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
  void removeFromThermalZone();

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl ImplType;

  explicit ZoneHVACBaseboardRadiantConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl> impl);

  friend class detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveElectric");
};

/** \relates ZoneHVACBaseboardRadiantConvectiveElectric*/
typedef boost::optional<ZoneHVACBaseboardRadiantConvectiveElectric> OptionalZoneHVACBaseboardRadiantConvectiveElectric;

/** \relates ZoneHVACBaseboardRadiantConvectiveElectric*/
typedef std::vector<ZoneHVACBaseboardRadiantConvectiveElectric> ZoneHVACBaseboardRadiantConvectiveElectricVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP

