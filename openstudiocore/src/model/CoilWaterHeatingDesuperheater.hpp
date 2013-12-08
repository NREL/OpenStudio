/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILWATERHEATINGDESUPERHEATER_HPP
#define MODEL_COILWATERHEATINGDESUPERHEATER_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class CurveBiquadratic;
class HVACComponent;

namespace detail {

  class CoilWaterHeatingDesuperheater_Impl;

} // detail

/** CoilWaterHeatingDesuperheater is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:Desuperheater'. */
class MODEL_API CoilWaterHeatingDesuperheater : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilWaterHeatingDesuperheater(const Model& model);

  virtual ~CoilWaterHeatingDesuperheater() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule setpointTemperatureSchedule() const;

  double deadBandTemperatureDifference() const;

  bool isDeadBandTemperatureDifferenceDefaulted() const;

  boost::optional<double> ratedHeatReclaimRecoveryEfficiency() const;

  double ratedInletWaterTemperature() const;

  double ratedOutdoorAirTemperature() const;

  double maximumInletWaterTemperatureforHeatReclaim() const;

  // TODO: Check return type. From object lists, some candidates are: CurveBiquadratic.
  boost::optional<CurveBiquadratic> heatReclaimEfficiencyFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // Connection waterInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // Connection waterOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: HVACComponent.
  boost::optional<HVACComponent> tank() const;

  // TODO: Check return type. From object lists, some candidates are: DesuperHeatingCoilSources.
  boost::optional<ModelObject> heatingSource() const;

  double waterFlowRate() const;

  double waterPumpPower() const;

  bool isWaterPumpPowerDefaulted() const;

  double fractionofPumpHeattoWater() const;

  bool isFractionofPumpHeattoWaterDefaulted() const;

  double onCycleParasiticElectricLoad() const;

  bool isOnCycleParasiticElectricLoadDefaulted() const;

  double offCycleParasiticElectricLoad() const;

  bool isOffCycleParasiticElectricLoadDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setSetpointTemperatureSchedule(Schedule& schedule);

  bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

  void resetDeadBandTemperatureDifference();

  bool setRatedHeatReclaimRecoveryEfficiency(double ratedHeatReclaimRecoveryEfficiency);

  void resetRatedHeatReclaimRecoveryEfficiency();

  void setRatedInletWaterTemperature(double ratedInletWaterTemperature);

  void setRatedOutdoorAirTemperature(double ratedOutdoorAirTemperature);

  void setMaximumInletWaterTemperatureforHeatReclaim(double maximumInletWaterTemperatureforHeatReclaim);

  // TODO: Check argument type. From object lists, some candidates are: CurveBiquadratic.
  bool setHeatReclaimEfficiencyFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

  void resetHeatReclaimEfficiencyFunctionofTemperatureCurve();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  // bool setWaterInletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  // bool setWaterOutletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: HVACComponent.
  bool setTank(const HVACComponent& waterHeater);

  // TODO: Check argument type. From object lists, some candidates are: DesuperHeatingCoilSources.
  bool setHeatingSource(const ModelObject& heatingSource);

  bool setWaterFlowRate(double waterFlowRate);

  bool setWaterPumpPower(double waterPumpPower);

  void resetWaterPumpPower();

  bool setFractionofPumpHeattoWater(double fractionofPumpHeattoWater);

  void resetFractionofPumpHeattoWater();

  bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

  void resetOnCycleParasiticElectricLoad();

  bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

  void resetOffCycleParasiticElectricLoad();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilWaterHeatingDesuperheater_Impl ImplType;

  explicit CoilWaterHeatingDesuperheater(boost::shared_ptr<detail::CoilWaterHeatingDesuperheater_Impl> impl);

  friend class detail::CoilWaterHeatingDesuperheater_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilWaterHeatingDesuperheater");
};

/** \relates CoilWaterHeatingDesuperheater*/
typedef boost::optional<CoilWaterHeatingDesuperheater> OptionalCoilWaterHeatingDesuperheater;

/** \relates CoilWaterHeatingDesuperheater*/
typedef std::vector<CoilWaterHeatingDesuperheater> CoilWaterHeatingDesuperheaterVector;

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGDESUPERHEATER_HPP

