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

#ifndef MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_HPP
#define MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class DesignSpecificationOutdoorAir;

namespace detail {

  class ZoneHVACIdealLoadsAirSystem_Impl;

} // detail

/** ZoneHVACIdealLoadsAirSystem is a ZoneHVACComponent that wraps the OpenStudio IDD 
 *  object 'OS:ZoneHVAC:IdealLoadsAirSystem'. */
class MODEL_API ZoneHVACIdealLoadsAirSystem : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACIdealLoadsAirSystem(const Model& model);

  virtual ~ZoneHVACIdealLoadsAirSystem() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatingLimitValues();

  static std::vector<std::string> coolingLimitValues();

  static std::vector<std::string> dehumidificationControlTypeValues();

  static std::vector<std::string> humidificationControlTypeValues();

  static std::vector<std::string> demandControlledVentilationTypeValues();

  static std::vector<std::string> outdoorAirEconomizerTypeValues();

  static std::vector<std::string> heatRecoveryTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double maximumHeatingSupplyAirTemperature() const;

  Quantity getMaximumHeatingSupplyAirTemperature(bool returnIP=false) const;

  bool isMaximumHeatingSupplyAirTemperatureDefaulted() const;

  double minimumCoolingSupplyAirTemperature() const;

  Quantity getMinimumCoolingSupplyAirTemperature(bool returnIP=false) const;

  bool isMinimumCoolingSupplyAirTemperatureDefaulted() const;

  double maximumHeatingSupplyAirHumidityRatio() const;

  Quantity getMaximumHeatingSupplyAirHumidityRatio(bool returnIP=false) const;

  bool isMaximumHeatingSupplyAirHumidityRatioDefaulted() const;

  double minimumCoolingSupplyAirHumidityRatio() const;

  Quantity getMinimumCoolingSupplyAirHumidityRatio(bool returnIP=false) const;

  bool isMinimumCoolingSupplyAirHumidityRatioDefaulted() const;

  std::string heatingLimit() const;

  bool isHeatingLimitDefaulted() const;

  boost::optional<double> maximumHeatingAirFlowRate() const;

  OSOptionalQuantity getMaximumHeatingAirFlowRate(bool returnIP=false) const;

  bool isMaximumHeatingAirFlowRateAutosized() const;

  boost::optional<double> maximumSensibleHeatingCapacity() const;

  OSOptionalQuantity getMaximumSensibleHeatingCapacity(bool returnIP=false) const;

  bool isMaximumSensibleHeatingCapacityAutosized() const;

  std::string coolingLimit() const;

  bool isCoolingLimitDefaulted() const;

  boost::optional<double> maximumCoolingAirFlowRate() const;

  OSOptionalQuantity getMaximumCoolingAirFlowRate(bool returnIP=false) const;

  bool isMaximumCoolingAirFlowRateAutosized() const;

  boost::optional<double> maximumTotalCoolingCapacity() const;

  OSOptionalQuantity getMaximumTotalCoolingCapacity(bool returnIP=false) const;

  bool isMaximumTotalCoolingCapacityAutosized() const;

  boost::optional<Schedule> heatingAvailabilitySchedule() const;

  boost::optional<Schedule> coolingAvailabilitySchedule() const;

  std::string dehumidificationControlType() const;

  bool isDehumidificationControlTypeDefaulted() const;

  double coolingSensibleHeatRatio() const;

  Quantity getCoolingSensibleHeatRatio(bool returnIP=false) const;

  bool isCoolingSensibleHeatRatioDefaulted() const;

  std::string humidificationControlType() const;

  bool isHumidificationControlTypeDefaulted() const;

  boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

  std::string demandControlledVentilationType() const;

  bool isDemandControlledVentilationTypeDefaulted() const;

  std::string outdoorAirEconomizerType() const;

  bool isOutdoorAirEconomizerTypeDefaulted() const;

  std::string heatRecoveryType() const;

  bool isHeatRecoveryTypeDefaulted() const;

  double sensibleHeatRecoveryEffectiveness() const;

  Quantity getSensibleHeatRecoveryEffectiveness(bool returnIP=false) const;

  bool isSensibleHeatRecoveryEffectivenessDefaulted() const;

  double latentHeatRecoveryEffectiveness() const;

  Quantity getLatentHeatRecoveryEffectiveness(bool returnIP=false) const;

  bool isLatentHeatRecoveryEffectivenessDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& availabilitySchedule);

  void resetAvailabilitySchedule();

  bool setMaximumHeatingSupplyAirTemperature(double maximumHeatingSupplyAirTemperature);

  bool setMaximumHeatingSupplyAirTemperature(const Quantity& maximumHeatingSupplyAirTemperature);

  void resetMaximumHeatingSupplyAirTemperature();

  bool setMinimumCoolingSupplyAirTemperature(double minimumCoolingSupplyAirTemperature);

  bool setMinimumCoolingSupplyAirTemperature(const Quantity& minimumCoolingSupplyAirTemperature);

  void resetMinimumCoolingSupplyAirTemperature();

  bool setMaximumHeatingSupplyAirHumidityRatio(double maximumHeatingSupplyAirHumidityRatio);

  bool setMaximumHeatingSupplyAirHumidityRatio(const Quantity& maximumHeatingSupplyAirHumidityRatio);

  void resetMaximumHeatingSupplyAirHumidityRatio();

  bool setMinimumCoolingSupplyAirHumidityRatio(double minimumCoolingSupplyAirHumidityRatio);

  bool setMinimumCoolingSupplyAirHumidityRatio(const Quantity& minimumCoolingSupplyAirHumidityRatio);

  void resetMinimumCoolingSupplyAirHumidityRatio();

  bool setHeatingLimit(std::string heatingLimit);

  void resetHeatingLimit();

  bool setMaximumHeatingAirFlowRate(double maximumHeatingAirFlowRate);

  bool setMaximumHeatingAirFlowRate(const Quantity& maximumHeatingAirFlowRate);

  void resetMaximumHeatingAirFlowRate();

  void autosizeMaximumHeatingAirFlowRate();

  bool setMaximumSensibleHeatingCapacity(double maximumSensibleHeatingCapacity);

  bool setMaximumSensibleHeatingCapacity(const Quantity& maximumSensibleHeatingCapacity);

  void resetMaximumSensibleHeatingCapacity();

  void autosizeMaximumSensibleHeatingCapacity();

  bool setCoolingLimit(std::string coolingLimit);

  void resetCoolingLimit();

  bool setMaximumCoolingAirFlowRate(double maximumCoolingAirFlowRate);

  bool setMaximumCoolingAirFlowRate(const Quantity& maximumCoolingAirFlowRate);

  void resetMaximumCoolingAirFlowRate();

  void autosizeMaximumCoolingAirFlowRate();

  bool setMaximumTotalCoolingCapacity(double maximumTotalCoolingCapacity);

  bool setMaximumTotalCoolingCapacity(const Quantity& maximumTotalCoolingCapacity);

  void resetMaximumTotalCoolingCapacity();

  void autosizeMaximumTotalCoolingCapacity();

  bool setHeatingAvailabilitySchedule(Schedule& heatingAvailabilitySchedule);

  void resetHeatingAvailabilitySchedule();

  bool setCoolingAvailabilitySchedule(Schedule& coolingAvailabilitySchedule);

  void resetCoolingAvailabilitySchedule();

  bool setDehumidificationControlType(std::string dehumidificationControlType);

  void resetDehumidificationControlType();

  bool setCoolingSensibleHeatRatio(double coolingSensibleHeatRatio);

  bool setCoolingSensibleHeatRatio(const Quantity& coolingSensibleHeatRatio);

  void resetCoolingSensibleHeatRatio();

  bool setHumidificationControlType(std::string humidificationControlType);

  void resetHumidificationControlType();

  bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAirObject);

  void resetDesignSpecificationOutdoorAirObject();

  bool setDemandControlledVentilationType(std::string demandControlledVentilationType);

  void resetDemandControlledVentilationType();

  bool setOutdoorAirEconomizerType(std::string outdoorAirEconomizerType);

  void resetOutdoorAirEconomizerType();

  bool setHeatRecoveryType(std::string heatRecoveryType);

  void resetHeatRecoveryType();

  bool setSensibleHeatRecoveryEffectiveness(double sensibleHeatRecoveryEffectiveness);

  bool setSensibleHeatRecoveryEffectiveness(const Quantity& sensibleHeatRecoveryEffectiveness);

  void resetSensibleHeatRecoveryEffectiveness();

  bool setLatentHeatRecoveryEffectiveness(double latentHeatRecoveryEffectiveness);

  bool setLatentHeatRecoveryEffectiveness(const Quantity& latentHeatRecoveryEffectiveness);

  void resetLatentHeatRecoveryEffectiveness();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACIdealLoadsAirSystem_Impl ImplType;

  explicit ZoneHVACIdealLoadsAirSystem(std::shared_ptr<detail::ZoneHVACIdealLoadsAirSystem_Impl> impl);

  friend class detail::ZoneHVACIdealLoadsAirSystem_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACIdealLoadsAirSystem");
};

/** \relates ZoneHVACIdealLoadsAirSystem*/
typedef boost::optional<ZoneHVACIdealLoadsAirSystem> OptionalZoneHVACIdealLoadsAirSystem;

/** \relates ZoneHVACIdealLoadsAirSystem*/
typedef std::vector<ZoneHVACIdealLoadsAirSystem> ZoneHVACIdealLoadsAirSystemVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_HPP

