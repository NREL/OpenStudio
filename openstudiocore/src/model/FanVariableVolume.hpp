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

#ifndef MODEL_FANVARIABLEVOLUME_HPP
#define MODEL_FANVARIABLEVOLUME_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;

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

  virtual ~FanVariableVolume() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fanPowerMinimumFlowRateInputMethodValues();

  /** \deprecated */
  static std::vector<std::string> validFanPowerMinimumFlowRateInputMethodValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  double fanEfficiency() const;

  Quantity getFanEfficiency(bool returnIP=false) const;

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

  bool setFanEfficiency(double fanEfficiency);

  bool setFanEfficiency(const Quantity& fanEfficiency);

  void resetFanEfficiency();

  void setPressureRise(double pressureRise);

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

  void setFanPowerCoefficient1(double fanPowerCoefficient1);

  bool setFanPowerCoefficient1(const Quantity& fanPowerCoefficient1);

  void resetFanPowerCoefficient1();

  void setFanPowerCoefficient2(double fanPowerCoefficient2);

  bool setFanPowerCoefficient2(const Quantity& fanPowerCoefficient2);

  void resetFanPowerCoefficient2();

  void setFanPowerCoefficient3(double fanPowerCoefficient3);

  bool setFanPowerCoefficient3(const Quantity& fanPowerCoefficient3);

  void resetFanPowerCoefficient3();

  void setFanPowerCoefficient4(double fanPowerCoefficient4);

  bool setFanPowerCoefficient4(const Quantity& fanPowerCoefficient4);

  void resetFanPowerCoefficient4();

  void setFanPowerCoefficient5(double fanPowerCoefficient5);

  bool setFanPowerCoefficient5(const Quantity& fanPowerCoefficient5);

  void resetFanPowerCoefficient5();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

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

