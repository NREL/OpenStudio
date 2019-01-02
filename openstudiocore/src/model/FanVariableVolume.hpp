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

#ifndef MODEL_FANVARIABLEVOLUME_HPP
#define MODEL_FANVARIABLEVOLUME_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class AirflowNetworkFan;

namespace detail {

class FanVariableVolume_Impl;

} // detail

/** FanVariableVolume is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Fan:VariableVolume'. */
class MODEL_API FanVariableVolume : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  FanVariableVolume(const Model& model, Schedule & schedule);

  FanVariableVolume(const Model& model);

  virtual ~FanVariableVolume() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fanPowerMinimumFlowRateInputMethodValues();

  /** \deprecated */
  static std::vector<std::string> validFanPowerMinimumFlowRateInputMethodValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  double fanTotalEfficiency() const;
  double fanEfficiency() const;

  Quantity getFanTotalEfficiency(bool returnIP=false) const;
  Quantity getFanEfficiency(bool returnIP=false) const;

  bool isFanTotalEfficiencyDefaulted() const;
  bool isFanEfficiencyDefaulted() const;

  double pressureRise() const;

  Quantity getPressureRise(bool returnIP=false) const;

  boost::optional<double> maximumFlowRate() const;

  OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

  bool isMaximumFlowRateAutosized() const;

  std::string fanPowerMinimumFlowRateInputMethod() const;

  bool isFanPowerMinimumFlowRateInputMethodDefaulted() const;

  double fanPowerMinimumFlowFraction() const;

  Quantity getFanPowerMinimumFlowFraction(bool returnIP=false) const;

  bool isFanPowerMinimumFlowFractionDefaulted() const;

  boost::optional<double> fanPowerMinimumAirFlowRate() const;

  OSOptionalQuantity getFanPowerMinimumAirFlowRate(bool returnIP=false) const;

  double motorEfficiency() const;

  Quantity getMotorEfficiency(bool returnIP=false) const;

  bool isMotorEfficiencyDefaulted() const;

  double motorInAirstreamFraction() const;

  Quantity getMotorInAirstreamFraction(bool returnIP=false) const;

  bool isMotorInAirstreamFractionDefaulted() const;

  boost::optional<double> fanPowerCoefficient1() const;

  OSOptionalQuantity getFanPowerCoefficient1(bool returnIP=false) const;

  boost::optional<double> fanPowerCoefficient2() const;

  OSOptionalQuantity getFanPowerCoefficient2(bool returnIP=false) const;

  boost::optional<double> fanPowerCoefficient3() const;

  OSOptionalQuantity getFanPowerCoefficient3(bool returnIP=false) const;

  boost::optional<double> fanPowerCoefficient4() const;

  OSOptionalQuantity getFanPowerCoefficient4(bool returnIP=false) const;

  boost::optional<double> fanPowerCoefficient5() const;

  OSOptionalQuantity getFanPowerCoefficient5(bool returnIP=false) const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setFanTotalEfficiency(double fanTotalEfficiency);
  bool setFanTotalEfficiency(const Quantity& fanTotalEfficiency);
  void resetFanTotalEfficiency();

  bool setFanEfficiency(double fanTotalEfficiency);
  bool setFanEfficiency(const Quantity& fanTotalEfficiency);
  void resetFanEfficiency();

  bool setPressureRise(double pressureRise);

  bool setPressureRise(const Quantity& pressureRise);

  bool setMaximumFlowRate(double maximumFlowRate);

  bool setMaximumFlowRate(const Quantity& maximumFlowRate);

  void resetMaximumFlowRate();

  void autosizeMaximumFlowRate();

  bool setFanPowerMinimumFlowRateInputMethod(std::string fanPowerMinimumFlowRateInputMethod);

  void resetFanPowerMinimumFlowRateInputMethod();

  bool setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction);

  bool setFanPowerMinimumFlowFraction(const Quantity& fanPowerMinimumFlowFraction);

  void resetFanPowerMinimumFlowFraction();

  bool setFanPowerMinimumAirFlowRate(double fanPowerMinimumAirFlowRate);

  bool setFanPowerMinimumAirFlowRate(const Quantity& fanPowerMinimumAirFlowRate);

  void resetFanPowerMinimumAirFlowRate();

  bool setMotorEfficiency(double motorEfficiency);

  bool setMotorEfficiency(const Quantity& motorEfficiency);

  void resetMotorEfficiency();

  bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

  bool setMotorInAirstreamFraction(const Quantity& motorInAirstreamFraction);

  void resetMotorInAirstreamFraction();

  bool setFanPowerCoefficient1(double fanPowerCoefficient1);

  bool setFanPowerCoefficient1(const Quantity& fanPowerCoefficient1);

  void resetFanPowerCoefficient1();

  bool setFanPowerCoefficient2(double fanPowerCoefficient2);

  bool setFanPowerCoefficient2(const Quantity& fanPowerCoefficient2);

  void resetFanPowerCoefficient2();

  bool setFanPowerCoefficient3(double fanPowerCoefficient3);

  bool setFanPowerCoefficient3(const Quantity& fanPowerCoefficient3);

  void resetFanPowerCoefficient3();

  bool setFanPowerCoefficient4(double fanPowerCoefficient4);

  bool setFanPowerCoefficient4(const Quantity& fanPowerCoefficient4);

  void resetFanPowerCoefficient4();

  bool setFanPowerCoefficient5(double fanPowerCoefficient5);

  bool setFanPowerCoefficient5(const Quantity& fanPowerCoefficient5);

  void resetFanPowerCoefficient5();

  bool setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  AirflowNetworkFan getAirflowNetworkFan();
  boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

  boost::optional<double> autosizedMaximumFlowRate() const ;
  //@}
 protected:
  /// @cond
  typedef detail::FanVariableVolume_Impl ImplType;

  friend class detail::FanVariableVolume_Impl;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit FanVariableVolume(std::shared_ptr<detail::FanVariableVolume_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.FanVariableVolume");
};

/** \relates FanVariableVolume*/
typedef boost::optional<FanVariableVolume> OptionalFanVariableVolume;

/** \relates FanVariableVolume*/
typedef std::vector<FanVariableVolume> FanVariableVolumeVector;

} // model

} // openstudio

#endif // MODEL_FANVARIABLEVOLUME_HPP
