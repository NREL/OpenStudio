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

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class ZoneHVACBaseboardConvectiveElectric_Impl;

} // detail

/** ZoneHVACBaseboardConvectiveElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:Convective:Electric'. */
class MODEL_API ZoneHVACBaseboardConvectiveElectric : public ZoneHVACComponent {

 public:

  explicit ZoneHVACBaseboardConvectiveElectric(const Model& model);

  virtual ~ZoneHVACBaseboardConvectiveElectric() {}

  static IddObjectType iddObjectType();

  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);

  boost::optional<double> nominalCapacity() const;

  OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

  bool isNominalCapacityAutosized() const;

  void setNominalCapacity(double nominalCapacity);

  bool setNominalCapacity(const Quantity& nominalCapacity);

  void autosizeNominalCapacity();

  double efficiency() const;

  Quantity getEfficiency(bool returnIP=false) const;

  bool isEfficiencyDefaulted() const;

  bool setEfficiency(double efficiency);

  bool setEfficiency(const Quantity& efficiency);

  void resetEfficiency();
  
   /** Returns the optional ThermalZone that this ZoneHVACBaseboardConvectiveElectric is attached to
   **/
  boost::optional<ThermalZone> thermalZone();

  /** Adds this ZoneHVACBaseboardConvectiveElectric to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
  bool addToThermalZone(ThermalZone & thermalZone);

  /** Detaches this ZoneHVACBaseboardConvectiveElectric from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
  void removeFromThermalZone();

 protected:

  /// @cond
  typedef detail::ZoneHVACBaseboardConvectiveElectric_Impl ImplType;

  explicit ZoneHVACBaseboardConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardConvectiveElectric_Impl> impl);

  friend class detail::ZoneHVACBaseboardConvectiveElectric_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveElectric");
};

/** \relates ZoneHVACBaseboardConvectiveElectric*/
typedef boost::optional<ZoneHVACBaseboardConvectiveElectric> OptionalZoneHVACBaseboardConvectiveElectric;

/** \relates ZoneHVACBaseboardConvectiveElectric*/
typedef std::vector<ZoneHVACBaseboardConvectiveElectric> ZoneHVACBaseboardConvectiveElectricVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_HPP

