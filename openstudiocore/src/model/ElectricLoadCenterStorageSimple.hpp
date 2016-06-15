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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Zone;

namespace detail {

  class ElectricLoadCenterStorageSimple_Impl;

} // detail

/** ElectricLoadCenterStorageSimple is a ParentObject that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:Simple'. */
class MODEL_API ElectricLoadCenterStorageSimple : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricLoadCenterStorageSimple(const Model& model);

  virtual ~ElectricLoadCenterStorageSimple() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Zone.
  boost::optional<Zone> zone() const;

  double radiativeFractionforZoneHeatGains() const;

  bool isRadiativeFractionforZoneHeatGainsDefaulted() const;

  double nominalEnergeticEfficiencyforCharging() const;

  bool isNominalEnergeticEfficiencyforChargingDefaulted() const;

  double nominalDischargingEnergeticEfficiency() const;

  bool isNominalDischargingEnergeticEfficiencyDefaulted() const;

  double maximumStorageCapacity() const;

  double maximumPowerforDischarging() const;

  double maximumPowerforCharging() const;

  boost::optional<double> initialStateofCharge() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: Zone.
  bool setZone(const Zone& zone);

  void resetZone();

  bool setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains);

  void resetRadiativeFractionforZoneHeatGains();

  bool setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging);

  void resetNominalEnergeticEfficiencyforCharging();

  bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);

  void resetNominalDischargingEnergeticEfficiency();

  void setMaximumStorageCapacity(double maximumStorageCapacity);

  bool setMaximumPowerforDischarging(double maximumPowerforDischarging);

  bool setMaximumPowerforCharging(double maximumPowerforCharging);

  void setInitialStateofCharge(double initialStateofCharge);

  void resetInitialStateofCharge();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ElectricLoadCenterStorageSimple_Impl ImplType;

  explicit ElectricLoadCenterStorageSimple(std::shared_ptr<detail::ElectricLoadCenterStorageSimple_Impl> impl);

  friend class detail::ElectricLoadCenterStorageSimple_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageSimple");
};

/** \relates ElectricLoadCenterStorageSimple*/
typedef boost::optional<ElectricLoadCenterStorageSimple> OptionalElectricLoadCenterStorageSimple;

/** \relates ElectricLoadCenterStorageSimple*/
typedef std::vector<ElectricLoadCenterStorageSimple> ElectricLoadCenterStorageSimpleVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP

