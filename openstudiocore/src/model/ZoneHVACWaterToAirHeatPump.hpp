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

#ifndef MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP
#define MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {


namespace model {

class Schedule;

class HVACComponent;

namespace detail {

  class ZoneHVACWaterToAirHeatPump_Impl;

} // detail

/** ZoneHVACWaterToAirHeatPump is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:WaterToAirHeatPump'. */
class MODEL_API ZoneHVACWaterToAirHeatPump : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  ZoneHVACWaterToAirHeatPump(const Model& model,   
                                    Schedule & availabilitySchedule,
                                    HVACComponent & supplyAirFan,
                                    HVACComponent & heatingCoil,
                                    HVACComponent & coolingCoil,
                                    HVACComponent & supplementalHeatingCoil );

  virtual ~ZoneHVACWaterToAirHeatPump() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fanPlacementValues();
  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;


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

  HVACComponent coolingCoil() const;

  double maximumCyclingRate() const;

  bool isMaximumCyclingRateDefaulted() const;

  double heatPumpTimeConstant() const;

  bool isHeatPumpTimeConstantDefaulted() const;

  double fractionofOnCyclePowerUse() const;

  bool isFractionofOnCyclePowerUseDefaulted() const;

  double heatPumpFanDelayTime() const;

  bool isHeatPumpFanDelayTimeDefaulted() const;

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

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

  void resetSupplyAirFlowRateDuringCoolingOperation();
  
  void autosizeSupplyAirFlowRateDuringCoolingOperation();

  bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);
  
  void resetSupplyAirFlowRateDuringHeatingOperation();
  
  void autosizeSupplyAirFlowRateDuringHeatingOperation();

  bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

  void resetOutdoorAirFlowRateDuringCoolingOperation();
  
  void autosizeOutdoorAirFlowRateDuringCoolingOperation();

  bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

  void resetOutdoorAirFlowRateDuringHeatingOperation();
  
  void autosizeOutdoorAirFlowRateDuringHeatingOperation();

  bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  bool setSupplyAirFan(HVACComponent& fansOnOff);

  bool setHeatingCoil(HVACComponent& heatingCoils);

  bool setCoolingCoil(HVACComponent& coolingCoils);

  bool setMaximumCyclingRate(boost::optional<double> maximumCyclingRate);

  void resetMaximumCyclingRate();

  bool setHeatPumpTimeConstant(boost::optional<double> heatPumpTimeConstant);

  void resetHeatPumpTimeConstant();

  bool setFractionofOnCyclePowerUse(boost::optional<double> fractionofOnCyclePowerUse);

  void resetFractionofOnCyclePowerUse();

  bool setHeatPumpFanDelayTime(boost::optional<double> heatPumpFanDelayTime);

  void resetHeatPumpFanDelayTime();

  bool setSupplementalHeatingCoil(HVACComponent& heatingCoilName);

  bool setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);
  
  void resetMaximumSupplyAirTemperaturefromSupplementalHeater();
  
  void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

  bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(boost::optional<double> maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

  void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

  // DLM: this appears to not be implemented
  //bool setOutdoorDryBulbTemperatureSensorNodeName(std::string outdoorDryBulbTemperatureSensorNodeName);

  bool setFanPlacement(std::string fanPlacement);

  void resetFanPlacement();
  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACWaterToAirHeatPump_Impl ImplType;

  explicit ZoneHVACWaterToAirHeatPump(std::shared_ptr<detail::ZoneHVACWaterToAirHeatPump_Impl> impl);

  friend class detail::ZoneHVACWaterToAirHeatPump_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACWaterToAirHeatPump");
};

/** \relates ZoneHVACWaterToAirHeatPump*/
typedef boost::optional<ZoneHVACWaterToAirHeatPump> OptionalZoneHVACWaterToAirHeatPump;

/** \relates ZoneHVACWaterToAirHeatPump*/
typedef std::vector<ZoneHVACWaterToAirHeatPump> ZoneHVACWaterToAirHeatPumpVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP

