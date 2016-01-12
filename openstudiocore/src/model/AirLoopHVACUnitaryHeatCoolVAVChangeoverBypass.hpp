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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl;

} // detail

/** AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass'. */
class MODEL_API AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(const Model& model, const HVACComponent& fan, const HVACComponent& coolingCoil, const HVACComponent& heatingCoil);

  virtual ~AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> supplyAirFanPlacementValues();

  static std::vector<std::string> priorityControlModeValues();

  static std::vector<std::string> dehumidificationControlTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
  boost::optional<double> systemAirFlowRateDuringCoolingOperation() const;

  bool isSystemAirFlowRateDuringCoolingOperationAutosized() const;

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
  boost::optional<double> systemAirFlowRateDuringHeatingOperation() const;

  bool isSystemAirFlowRateDuringHeatingOperationAutosized() const;

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate" **/
  boost::optional<double> systemAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Outdoor Air Flow Rate" **/
  boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

  bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Outdoor Air Flow Rate" **/
  boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

  bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Outdoor Air Flow Rate" **/
  boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  boost::optional<Schedule> outdoorAirFlowRateMultiplierSchedule() const;

  HVACComponent supplyAirFan() const;

  std::string supplyAirFanPlacement() const;

  boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

  HVACComponent coolingCoil() const;

  HVACComponent heatingCoil() const;

  std::string priorityControlMode() const;

  double minimumOutletAirTemperatureDuringCoolingOperation() const;

  double maximumOutletAirTemperatureDuringHeatingOperation() const;

  std::string dehumidificationControlType() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setSystemAirFlowRateDuringCoolingOperation(double systemAirFlowRateDuringCoolingOperation);

  void autosizeSystemAirFlowRateDuringCoolingOperation();

  bool setSystemAirFlowRateDuringHeatingOperation(double systemAirFlowRateDuringHeatingOperation);

  void autosizeSystemAirFlowRateDuringHeatingOperation();

  bool setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(double systemAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();

  bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

  void autosizeOutdoorAirFlowRateDuringCoolingOperation();

  bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

  void autosizeOutdoorAirFlowRateDuringHeatingOperation();

  bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  bool setOutdoorAirFlowRateMultiplierSchedule(Schedule& outdoorAirFlowRateMultiplierSchedule);

  void resetOutdoorAirFlowRateMultiplierSchedule();

  bool setSupplyAirFan(const HVACComponent& fansCVandOnOff);

  bool setSupplyAirFanPlacement(std::string supplyAirFanPlacement);

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  bool setCoolingCoil(const HVACComponent& coolingCoilsDXMultiModeOrSingleSpeed);

  bool setHeatingCoil(const HVACComponent& heatingCoilName);

  bool setPriorityControlMode(std::string priorityControlMode);

  bool setMinimumOutletAirTemperatureDuringCoolingOperation(double minimumOutletAirTemperatureDuringCoolingOperation);

  bool setMaximumOutletAirTemperatureDuringHeatingOperation(double maximumOutletAirTemperatureDuringHeatingOperation);

  bool setDehumidificationControlType(std::string dehumidificationControlType);

  //@}
  /** @name Other */
  //@{
  //@}
 protected:
  /// @cond
  typedef detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl ImplType;

  explicit AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(std::shared_ptr<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl> impl);

  friend class detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass");
};

/** \relates AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass*/
typedef boost::optional<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> OptionalAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass;

/** \relates AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass*/
typedef std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypassVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP

