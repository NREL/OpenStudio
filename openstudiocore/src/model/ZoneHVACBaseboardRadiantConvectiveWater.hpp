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

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ZoneHVACBaseboardRadiantConvectiveWater_Impl;

} // detail

/** ZoneHVACBaseboardRadiantConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Water'. */
class MODEL_API ZoneHVACBaseboardRadiantConvectiveWater : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACBaseboardRadiantConvectiveWater(const Model& model);

  virtual ~ZoneHVACBaseboardRadiantConvectiveWater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  double fractionRadiant() const;

  double fractionofRadiantEnergyIncidentonPeople() const;

  HVACComponent heatingCoil() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setFractionRadiant(double fractionRadiant);

  bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

  bool setHeatingCoil( const HVACComponent& heatingCoil );

  //@}
  /** @name Other */
  //@{

  /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveWater is attached to
   **/
  boost::optional<ThermalZone> thermalZone();

  /** Adds this ZoneHVACBaseboardRadiantConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
  bool addToThermalZone(ThermalZone & thermalZone);

  /** Detaches this ZoneHVACBaseboardRadiantConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
  void removeFromThermalZone();

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl ImplType;

  explicit ZoneHVACBaseboardRadiantConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl> impl);

  friend class detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveWater");
};

/** \relates ZoneHVACBaseboardRadiantConvectiveWater*/
typedef boost::optional<ZoneHVACBaseboardRadiantConvectiveWater> OptionalZoneHVACBaseboardRadiantConvectiveWater;

/** \relates ZoneHVACBaseboardRadiantConvectiveWater*/
typedef std::vector<ZoneHVACBaseboardRadiantConvectiveWater> ZoneHVACBaseboardRadiantConvectiveWaterVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP

