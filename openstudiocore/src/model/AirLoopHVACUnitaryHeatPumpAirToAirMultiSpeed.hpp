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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl;

} // detail

/** AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatPump:AirToAir:MultiSpeed'. */
class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(const Model& model, 
    const HVACComponent& fan, 
    const HVACComponent& heatingCoil, 
    const HVACComponent& supplementalHeatingCoil,
    const HVACComponent& coolingCoil);

  virtual ~AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> supplyAirFanPlacementValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<ThermalZone> controllingZoneorThermostatLocation() const;

  HVACComponent supplyAirFan() const;

  std::string supplyAirFanPlacement() const;

  Schedule supplyAirFanOperatingModeSchedule() const;

  HVACComponent heatingCoil() const;

  double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

  HVACComponent coolingCoil() const;

  HVACComponent supplementalHeatingCoil() const;

  boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

  bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

  double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

  double auxiliaryOnCycleElectricPower() const;

  double auxiliaryOffCycleElectricPower() const;

  double designHeatRecoveryWaterFlowRate() const;

  double maximumTemperatureforHeatRecovery() const;

  boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  int numberofSpeedsforHeating() const;

  int numberofSpeedsforCooling() const;

  boost::optional<double> speed1SupplyAirFlowRateDuringHeatingOperation() const;

  bool isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> speed2SupplyAirFlowRateDuringHeatingOperation() const;

  bool isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> speed3SupplyAirFlowRateDuringHeatingOperation() const;

  bool isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> speed4SupplyAirFlowRateDuringHeatingOperation() const;

  bool isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> speed1SupplyAirFlowRateDuringCoolingOperation() const;

  bool isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> speed2SupplyAirFlowRateDuringCoolingOperation() const;

  bool isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> speed3SupplyAirFlowRateDuringCoolingOperation() const;

  bool isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> speed4SupplyAirFlowRateDuringCoolingOperation() const;

  bool isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setControllingZoneorThermostatLocation(const ThermalZone& thermalZone);

  void resetControllingZoneorThermostatLocation();

  bool setSupplyAirFan(const HVACComponent& fan);

  bool setSupplyAirFanPlacement(std::string supplyAirFanPlacement);

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  bool setHeatingCoil(const HVACComponent& coil);

  bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

  bool setCoolingCoil(const HVACComponent& coil);

  bool setSupplementalHeatingCoil(const HVACComponent& coil);

  void setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

  void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

  bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

  bool setAuxiliaryOnCycleElectricPower(double auxiliaryOnCycleElectricPower);

  bool setAuxiliaryOffCycleElectricPower(double auxiliaryOffCycleElectricPower);

  bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

  bool setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery);

  bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  bool setNumberofSpeedsforHeating(int numberofSpeedsforHeating);

  bool setNumberofSpeedsforCooling(int numberofSpeedsforCooling);

  bool setSpeed1SupplyAirFlowRateDuringHeatingOperation(double speed1SupplyAirFlowRateDuringHeatingOperation);

  void autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();

  bool setSpeed2SupplyAirFlowRateDuringHeatingOperation(double speed2SupplyAirFlowRateDuringHeatingOperation);

  void autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();

  bool setSpeed3SupplyAirFlowRateDuringHeatingOperation(double speed3SupplyAirFlowRateDuringHeatingOperation);

  void autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();

  bool setSpeed4SupplyAirFlowRateDuringHeatingOperation(double speed4SupplyAirFlowRateDuringHeatingOperation);

  void autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();

  bool setSpeed1SupplyAirFlowRateDuringCoolingOperation(double speed1SupplyAirFlowRateDuringCoolingOperation);

  void autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();

  bool setSpeed2SupplyAirFlowRateDuringCoolingOperation(double speed2SupplyAirFlowRateDuringCoolingOperation);

  void autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();

  bool setSpeed3SupplyAirFlowRateDuringCoolingOperation(double speed3SupplyAirFlowRateDuringCoolingOperation);

  void autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();

  bool setSpeed4SupplyAirFlowRateDuringCoolingOperation(double speed4SupplyAirFlowRateDuringCoolingOperation);

  void autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl ImplType;

  explicit AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl> impl);

  friend class detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed");
};

/** \relates AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed*/
typedef boost::optional<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed> OptionalAirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed;

/** \relates AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed*/
typedef std::vector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeedVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_HPP

