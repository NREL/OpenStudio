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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

class ThermalZone;

class HVACComponent;

namespace detail {

class AirLoopHVACUnitaryHeatPumpAirToAir_Impl;

} // detail

/** AirLoopHVACUnitaryHeatPumpAirToAir is a StraightComponent that wraps the
 *  OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatPump:AirToAir'. */
class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAir : public StraightComponent 
{
  public:
  /** @name Constructors and Destructors */
  //@{

  AirLoopHVACUnitaryHeatPumpAirToAir( const Model & model,
                                      Schedule & availabilitySchedule,
                                      HVACComponent & supplyFan,
                                      HVACComponent & heatingCoil,
                                      HVACComponent & coolingCoil,
                                      HVACComponent & supplementalHeatingCoil );

  virtual ~AirLoopHVACUnitaryHeatPumpAirToAir() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validFanPlacementValues();

  static std::vector<std::string> validDehumidificationControlTypeValues();

  /** @name Getters */
  //@{
  
  Schedule availabilitySchedule() const;
  
  boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

  bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

  boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

  bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

  boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  boost::optional<ThermalZone> controllingZone() const;

  HVACComponent supplyAirFan() const;

  HVACComponent heatingCoil() const;

  HVACComponent coolingCoil() const;

  HVACComponent supplementalHeatingCoil() const;
  
  boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

  bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

  double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

  bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

  std::string fanPlacement() const;

  bool isFanPlacementDefaulted() const;

  /** Returns the supply air fan operating mode schedule, if present. In this schedule, values
   *  of zero correspond to AUTO fan operation (fan cycles on and off with coils to meet load).
   *  Other values indicate that the supply fan is to run continuously. */
  boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;
  
  std::string dehumidificationControlType() const;

  bool isDehumidificationControlTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);
  
  bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

  void autosizeSupplyAirFlowRateDuringCoolingOperation();

  bool setSupplyAirFlowRateDuringHeatingOperation(double n2);

  void autosizeSupplyAirFlowRateDuringHeatingOperation();

  bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

  void setControllingZone( ThermalZone & zone );

  void resetControllingZone();

  void setSupplyAirFan( HVACComponent & hvacComponent );

  void setHeatingCoil( HVACComponent & hvacComponent );

  void setCoolingCoil( HVACComponent & hvacComponent );

  void setSupplementalHeatingCoil( HVACComponent & hvacComponent );
    
  void setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

  void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

  bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

  void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

  bool setFanPlacement(std::string fanPlacement);

  void resetFanPlacement();
 
  void setSupplyAirFanOperatingModeSchedule(Schedule & schedule);

  void resetSupplyAirFanOperatingModeSchedule();
  
  bool setDehumidificationControlType(std::string dehumidificationControlType);

  void resetDehumidificationControlType();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond

  typedef detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl ImplType;

  explicit AirLoopHVACUnitaryHeatPumpAirToAir(std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl> impl);

  friend class detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAir");
};

/** \relates AirLoopHVACUnitaryHeatPumpAirToAir*/
typedef boost::optional<AirLoopHVACUnitaryHeatPumpAirToAir> OptionalAirLoopHVACUnitaryHeatPumpAirToAir;

/** \relates AirLoopHVACUnitaryHeatPumpAirToAir*/
typedef std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> AirLoopHVACUnitaryHeatPumpAirToAirVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_HPP

