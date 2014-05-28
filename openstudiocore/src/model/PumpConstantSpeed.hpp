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

#ifndef MODEL_PUMPCONSTANTSPEED_HPP
#define MODEL_PUMPCONSTANTSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class Curve;
class ThermalZone;

namespace detail {

  class PumpConstantSpeed_Impl;

} // detail

/** PumpConstantSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Pump:ConstantSpeed'. */
class MODEL_API PumpConstantSpeed : public StraightComponent {
 public:

  explicit PumpConstantSpeed(const Model& model);

  virtual ~PumpConstantSpeed() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> pumpControlTypeValues();

  boost::optional<double> ratedFlowRate() const;

  OSOptionalQuantity getRatedFlowRate(bool returnIP=false) const;

  bool isRatedFlowRateAutosized() const;

  void setRatedFlowRate(double ratedFlowRate);

  bool setRatedFlowRate(const Quantity& ratedFlowRate);

  void resetRatedFlowRate();

  void autosizeRatedFlowRate();

  double ratedPumpHead() const;

  Quantity getRatedPumpHead(bool returnIP=false) const;

  bool isRatedPumpHeadDefaulted() const;

  void setRatedPumpHead(double ratedPumpHead);

  bool setRatedPumpHead(const Quantity& ratedPumpHead);

  void resetRatedPumpHead();

  boost::optional<double> ratedPowerConsumption() const;

  OSOptionalQuantity getRatedPowerConsumption(bool returnIP=false) const;

  bool isRatedPowerConsumptionAutosized() const;

  void setRatedPowerConsumption(double ratedPowerConsumption);

  bool setRatedPowerConsumption(const Quantity& ratedPowerConsumption);

  void resetRatedPowerConsumption();

  void autosizeRatedPowerConsumption();

  double motorEfficiency() const;

  Quantity getMotorEfficiency(bool returnIP=false) const;

  bool isMotorEfficiencyDefaulted() const;

  bool setMotorEfficiency(double motorEfficiency);

  bool setMotorEfficiency(const Quantity& motorEfficiency);

  void resetMotorEfficiency();

  double fractionofMotorInefficienciestoFluidStream() const;

  Quantity getFractionofMotorInefficienciestoFluidStream(bool returnIP=false) const;

  bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

  bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

  bool setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream);

  void resetFractionofMotorInefficienciestoFluidStream();

  std::string pumpControlType() const;

  bool isPumpControlTypeDefaulted() const;

  bool setPumpControlType(std::string pumpControlType);

  void resetPumpControlType();

  boost::optional<Schedule> pumpFlowRateSchedule() const;

  bool setPumpFlowRateSchedule(Schedule& schedule);

  void resetPumpFlowRateSchedule();

  boost::optional<Curve> pumpCurve() const;

  bool setPumpCurve(const Curve& curve);

  void resetPumpCurve();

  boost::optional<double> impellerDiameter() const;

  OSOptionalQuantity getImpellerDiameter(bool returnIP=false) const;

  void setImpellerDiameter(double impellerDiameter);

  bool setImpellerDiameter(const Quantity& impellerDiameter);

  void resetImpellerDiameter();

  boost::optional<double> rotationalSpeed() const;

  OSOptionalQuantity getRotationalSpeed(bool returnIP=false) const;

  void setRotationalSpeed(double rotationalSpeed);

  bool setRotationalSpeed(const Quantity& rotationalSpeed);

  void resetRotationalSpeed();

  boost::optional<ThermalZone> zone() const;

  boost::optional<double> skinLossRadiativeFraction() const;

  OSOptionalQuantity getSkinLossRadiativeFraction(bool returnIP=false) const;

  bool setZone(const ThermalZone& thermalZone);

  void resetZone();

  bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

  bool setSkinLossRadiativeFraction(const Quantity& skinLossRadiativeFraction);

  void resetSkinLossRadiativeFraction();

 protected:

  /// @cond
  typedef detail::PumpConstantSpeed_Impl ImplType;

  explicit PumpConstantSpeed(std::shared_ptr<detail::PumpConstantSpeed_Impl> impl);

  friend class detail::PumpConstantSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.PumpConstantSpeed");
};

/** \relates PumpConstantSpeed*/
typedef boost::optional<PumpConstantSpeed> OptionalPumpConstantSpeed;

/** \relates PumpConstantSpeed*/
typedef std::vector<PumpConstantSpeed> PumpConstantSpeedVector;

} // model
} // openstudio

#endif // MODEL_PUMPCONSTANTSPEED_HPP

