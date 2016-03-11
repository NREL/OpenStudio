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

#ifndef MODEL_ZONEHVACUNITVENTILATOR_HPP
#define MODEL_ZONEHVACUNITVENTILATOR_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACUnitVentilator_Impl;

} // detail

/** ZoneHVACUnitVentilator is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:UnitVentilator'. */
class MODEL_API ZoneHVACUnitVentilator : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACUnitVentilator(const Model& model);

  explicit ZoneHVACUnitVentilator(
    const Model& model,
    const HVACComponent& supplyAirFan
  );

  virtual ~ZoneHVACUnitVentilator() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> outdoorAirControlTypeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> maximumSupplyAirFlowRate() const;

  bool isMaximumSupplyAirFlowRateAutosized() const;

  std::string outdoorAirControlType() const;

  boost::optional<double> minimumOutdoorAirFlowRate() const;

  bool isMinimumOutdoorAirFlowRateAutosized() const;

  Schedule minimumOutdoorAirSchedule() const;

  boost::optional<double> maximumOutdoorAirFlowRate() const;

  bool isMaximumOutdoorAirFlowRateAutosized() const;

  Schedule maximumOutdoorAirFractionorTemperatureSchedule() const;

  HVACComponent supplyAirFan() const;

  boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

  boost::optional<HVACComponent> heatingCoil() const;

  double heatingConvergenceTolerance() const;

  boost::optional<HVACComponent> coolingCoil() const;

  double coolingConvergenceTolerance() const;

  // boost::optional<ModelObject> availabilityManagerList() const;

  // boost::optional<ModelObject> designSpecificationZoneHVACSizingObject() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate);

  void autosizeMaximumSupplyAirFlowRate();

  bool setOutdoorAirControlType(std::string outdoorAirControlType);

  bool setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate);

  void autosizeMinimumOutdoorAirFlowRate();

  bool setMinimumOutdoorAirSchedule(Schedule& schedule);

  bool setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate);

  void autosizeMaximumOutdoorAirFlowRate();

  bool setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule);

  bool setSupplyAirFan(const HVACComponent& supplyAirFan);

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  bool setHeatingCoil(const HVACComponent& heatingCoil);

  void resetHeatingCoil();

  bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

  bool setCoolingCoil(const HVACComponent& coolingCoil);

  void resetCoolingCoil();

  bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

  // bool setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists);

  // void resetAvailabilityManagerList();

  // bool setDesignSpecificationZoneHVACSizingObject(const ModelObject& designSpecificationZoneHVACSizingName);

  // void resetDesignSpecificationZoneHVACSizingObject();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACUnitVentilator_Impl ImplType;

  explicit ZoneHVACUnitVentilator(std::shared_ptr<detail::ZoneHVACUnitVentilator_Impl> impl);

  friend class detail::ZoneHVACUnitVentilator_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACUnitVentilator");
};

/** \relates ZoneHVACUnitVentilator*/
typedef boost::optional<ZoneHVACUnitVentilator> OptionalZoneHVACUnitVentilator;

/** \relates ZoneHVACUnitVentilator*/
typedef std::vector<ZoneHVACUnitVentilator> ZoneHVACUnitVentilatorVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACUNITVENTILATOR_HPP

