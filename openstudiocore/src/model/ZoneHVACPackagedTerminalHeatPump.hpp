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

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

class ZoneHVACPackagedTerminalHeatPump_Impl;

} // detail

/** ZoneHVACPackagedTerminalHeatPump is a ZoneHVACComponent that wraps the
 *  OpenStudio IDD object 'OS:ZoneHVAC:PackagedTerminalHeatPump'. */
class MODEL_API ZoneHVACPackagedTerminalHeatPump : public ZoneHVACComponent 
{
  public:
  /** @name Constructors and Destructors */
  //@{

  ZoneHVACPackagedTerminalHeatPump( const Model& model,
                                    Schedule & availabilitySchedule,
                                    HVACComponent & supplyAirFan,
                                    HVACComponent & heatingCoil,
                                    HVACComponent & coolingCoil,
                                    HVACComponent & supplementalHeatingCoil );

  virtual ~ZoneHVACPackagedTerminalHeatPump() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> outdoorAirMixerObjectTypeValues();

  /** \deprecated */
  static std::vector<std::string> validOutdoorAirMixerObjectTypeValues();

  static std::vector<std::string> fanPlacementValues();

  /** \deprecated */
  static std::vector<std::string> validFanPlacementValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;  

  std::string outdoorAirMixerObjectType() const;

  std::string outdoorAirMixerName() const;

  boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

  bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

  bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

  bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

  bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  HVACComponent supplyAirFan() const;

  HVACComponent heatingCoil() const;

  double heatingConvergenceTolerance() const;

  bool isHeatingConvergenceToleranceDefaulted() const;

  double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

  bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

  HVACComponent coolingCoil() const;

  double coolingConvergenceTolerance() const;

  bool isCoolingConvergenceToleranceDefaulted() const;

  HVACComponent supplementalHeatingCoil() const;

  boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

  bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

  double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

  bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

  std::string fanPlacement() const;

  bool isFanPlacementDefaulted() const;

  boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule & schedule );

  bool setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType);

  void setOutdoorAirMixerName(std::string outdoorAirMixerName);

  bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

  void autosizeSupplyAirFlowRateDuringCoolingOperation();

  bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

  void autosizeSupplyAirFlowRateDuringHeatingOperation();

  bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

  void autosizeOutdoorAirFlowRateDuringCoolingOperation();

  bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

  void autosizeOutdoorAirFlowRateDuringHeatingOperation();

  bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void setSupplyAirFan( HVACComponent & hvacComponent );

  void setHeatingCoil( HVACComponent & hvacComponent );

  bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

  void resetHeatingConvergenceTolerance();

  bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

  void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

  void setCoolingCoil( HVACComponent & hvacComponent );

  bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

  void resetCoolingConvergenceTolerance();

  void setSupplementalHeatingCoil( HVACComponent & hvacComponent );

  void setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

  void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

  bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

  void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

  bool setFanPlacement(std::string fanPlacement);

  void resetFanPlacement();

  /** Sets the supply air fan operating mode schedule. Values of zero indicate that the fan 
   *  cycles on and off with the active (heating or cooling) coil. Other values operate the 
   *  fan continuously. */
  bool setSupplyAirFanOperatingModeSchedule(Schedule & schedule );

  void resetSupplyAirFanOperatingModeSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACPackagedTerminalHeatPump_Impl ImplType;

  explicit ZoneHVACPackagedTerminalHeatPump(std::shared_ptr<detail::ZoneHVACPackagedTerminalHeatPump_Impl> impl);

  friend class detail::ZoneHVACPackagedTerminalHeatPump_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalHeatPump");
};

/** \relates ZoneHVACPackagedTerminalHeatPump*/
typedef boost::optional<ZoneHVACPackagedTerminalHeatPump> OptionalZoneHVACPackagedTerminalHeatPump;

/** \relates ZoneHVACPackagedTerminalHeatPump*/
typedef std::vector<ZoneHVACPackagedTerminalHeatPump> ZoneHVACPackagedTerminalHeatPumpVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP

