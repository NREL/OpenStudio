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

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;
class ZoneHVACEnergyRecoveryVentilatorController;

namespace detail {

  class ZoneHVACEnergyRecoveryVentilator_Impl;

} // detail

/** ZoneHVACEnergyRecoveryVentilator is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EnergyRecoveryVentilator'. */
class MODEL_API ZoneHVACEnergyRecoveryVentilator : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACEnergyRecoveryVentilator(const Model& model);

  explicit ZoneHVACEnergyRecoveryVentilator(
  const Model& model,
  const HVACComponent& heatExchanger,
  const HVACComponent& supplyAirFan,
  const HVACComponent& exhaustAirFan
  );

  virtual ~ZoneHVACEnergyRecoveryVentilator() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  HVACComponent heatExchanger() const;

  boost::optional<double> supplyAirFlowRate() const;

  bool isSupplyAirFlowRateAutosized() const;

  boost::optional<double> exhaustAirFlowRate() const;

  bool isExhaustAirFlowRateAutosized() const;

  HVACComponent supplyAirFan() const;

  HVACComponent exhaustAirFan() const;

  boost::optional<ZoneHVACEnergyRecoveryVentilatorController> controller() const;

  double ventilationRateperUnitFloorArea() const;

  double ventilationRateperOccupant() const;

  // boost::optional<ModelObject> availabilityManagerList() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent);

  bool setSupplyAirFlowRate(double supplyAirFlowRate);

  void autosizeSupplyAirFlowRate();

  bool setExhaustAirFlowRate(double exhaustAirFlowRate);

  void autosizeExhaustAirFlowRate();

  bool setSupplyAirFan(const HVACComponent& supplyAirFan);

  bool setExhaustAirFan(const HVACComponent& exhaustAirFan);

  bool setController(const ZoneHVACEnergyRecoveryVentilatorController& zoneHVACEnergyRecoveryVentilatorController);

  void resetController();

  bool setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea);

  bool setVentilationRateperOccupant(double ventilationRateperOccupant);

  // bool setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists);

  // void resetAvailabilityManagerList();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACEnergyRecoveryVentilator_Impl ImplType;

  explicit ZoneHVACEnergyRecoveryVentilator(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilator_Impl> impl);

  friend class detail::ZoneHVACEnergyRecoveryVentilator_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilator");
};

/** \relates ZoneHVACEnergyRecoveryVentilator*/
typedef boost::optional<ZoneHVACEnergyRecoveryVentilator> OptionalZoneHVACEnergyRecoveryVentilator;

/** \relates ZoneHVACEnergyRecoveryVentilator*/
typedef std::vector<ZoneHVACEnergyRecoveryVentilator> ZoneHVACEnergyRecoveryVentilatorVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP

