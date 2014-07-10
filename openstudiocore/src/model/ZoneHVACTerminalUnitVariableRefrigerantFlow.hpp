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

#ifndef MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class HVACComponent;
class Schedule;
class CoilHeatingDXVariableRefrigerantFlow;
class CoilCoolingDXVariableRefrigerantFlow;

namespace detail {

  class ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;

} // detail

/** ZoneHVACTerminalUnitVariableRefrigerantFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow'. */
class MODEL_API ZoneHVACTerminalUnitVariableRefrigerantFlow : public ZoneHVACComponent {

 public:

  explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model);

  virtual ~ZoneHVACTerminalUnitVariableRefrigerantFlow() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> supplyAirFanplacementValues();

  Schedule terminalUnitAvailabilityschedule() const;

  bool setTerminalUnitAvailabilityschedule(Schedule& schedule);

  boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

  bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

  bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

  void autosizeSupplyAirFlowRateDuringCoolingOperation();

  boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const;

  bool setSupplyAirFlowRateWhenNoCoolingisNeeded(double supplyAirFlowRateWhenNoCoolingisNeeded);

  void autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

  boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

  bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

  bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

  void autosizeSupplyAirFlowRateDuringHeatingOperation();

  boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const;

  bool setSupplyAirFlowRateWhenNoHeatingisNeeded(double supplyAirFlowRateWhenNoHeatingisNeeded);

  void autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

  boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

  bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

  bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

  void autosizeOutdoorAirFlowRateDuringCoolingOperation();

  boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

  bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

  bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

  void autosizeOutdoorAirFlowRateDuringHeatingOperation();

  boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  Schedule supplyAirFanOperatingModeSchedule() const;

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  HVACComponent supplyAirFan() const;

  CoilCoolingDXVariableRefrigerantFlow coolingCoil() const;

  CoilHeatingDXVariableRefrigerantFlow heatingCoil() const;

  double zoneTerminalUnitOnParasiticElectricEnergyUse() const;

  bool setZoneTerminalUnitOnParasiticElectricEnergyUse(double zoneTerminalUnitOnParasiticElectricEnergyUse);

  double zoneTerminalUnitOffParasiticElectricEnergyUse() const;

  bool setZoneTerminalUnitOffParasiticElectricEnergyUse(double zoneTerminalUnitOffParasiticElectricEnergyUse);

  double ratedTotalHeatingCapacitySizingRatio() const;

  bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);

 protected:

  /// @cond
  typedef detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl ImplType;

  explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(std::shared_ptr<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl> impl);

  friend class detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACTerminalUnitVariableRefrigerantFlow");
};

/** \relates ZoneHVACTerminalUnitVariableRefrigerantFlow*/
typedef boost::optional<ZoneHVACTerminalUnitVariableRefrigerantFlow> OptionalZoneHVACTerminalUnitVariableRefrigerantFlow;

/** \relates ZoneHVACTerminalUnitVariableRefrigerantFlow*/
typedef std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> ZoneHVACTerminalUnitVariableRefrigerantFlowVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP

