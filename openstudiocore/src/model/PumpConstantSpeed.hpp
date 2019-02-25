/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

  static std::vector<std::string> designPowerSizingMethodValues();

  /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Flow Rate" **/
  boost::optional<double> ratedFlowRate() const;

  OSOptionalQuantity getRatedFlowRate(bool returnIP=false) const;

  bool isRatedFlowRateAutosized() const;

  bool setRatedFlowRate(double ratedFlowRate);

  bool setRatedFlowRate(const Quantity& ratedFlowRate);

  void resetRatedFlowRate();

  void autosizeRatedFlowRate();

  /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Pump Head" **/
  double ratedPumpHead() const;

  Quantity getRatedPumpHead(bool returnIP=false) const;

  bool isRatedPumpHeadDefaulted() const;

  bool setRatedPumpHead(double ratedPumpHead);

  bool setRatedPumpHead(const Quantity& ratedPumpHead);

  void resetRatedPumpHead();

  /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Power Consumption" **/
  boost::optional<double> ratedPowerConsumption() const;

  OSOptionalQuantity getRatedPowerConsumption(bool returnIP=false) const;

  bool isRatedPowerConsumptionAutosized() const;

  bool setRatedPowerConsumption(double ratedPowerConsumption);

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

  bool setImpellerDiameter(double impellerDiameter);

  bool setImpellerDiameter(const Quantity& impellerDiameter);

  void resetImpellerDiameter();

  boost::optional<double> rotationalSpeed() const;

  OSOptionalQuantity getRotationalSpeed(bool returnIP=false) const;

  bool setRotationalSpeed(double rotationalSpeed);

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

  boost::optional<double> autosizedRatedFlowRate() const ;

  boost::optional<double> autosizedRatedPowerConsumption() const ;

  std::string designPowerSizingMethod() const;

  bool setDesignPowerSizingMethod(const std::string & designPowerSizingMethod);

  double designElectricPowerPerUnitFlowRate() const;

  bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

  double designShaftPowerPerUnitFlowRatePerUnitHead() const;

  bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

  std::string endUseSubcategory() const;

  bool setEndUseSubcategory(const std::string & endUseSubcategory);

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
