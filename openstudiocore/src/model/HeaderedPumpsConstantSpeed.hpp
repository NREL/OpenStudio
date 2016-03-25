/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_HEADEREDPUMPSCONSTANTSPEED_HPP
#define MODEL_HEADEREDPUMPSCONSTANTSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class HeaderedPumpsConstantSpeed_Impl;

} // detail

/** HeaderedPumpsConstantSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:HeaderedPumps:ConstantSpeed'. */
class MODEL_API HeaderedPumpsConstantSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HeaderedPumpsConstantSpeed(const Model& model);

  virtual ~HeaderedPumpsConstantSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> flowSequencingControlSchemeValues();

  static std::vector<std::string> pumpControlTypeValues();

  /** @name Getters */
  //@{

  /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Total Design Flow Rate" **/
  boost::optional<double> totalRatedFlowRate() const;

  bool isTotalRatedFlowRateAutosized() const;

  int numberofPumpsinBank() const;

  std::string flowSequencingControlScheme() const;

  /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Pump Head" **/
  double ratedPumpHead() const;

  /** In EnergyPlus 8.5.0 and above this property maps to the EnergyPlus field "Design Power Consumption" **/
  boost::optional<double> ratedPowerConsumption() const;

  bool isRatedPowerConsumptionAutosized() const;

  double motorEfficiency() const;

  double fractionofMotorInefficienciestoFluidStream() const;

  std::string pumpControlType() const;

  boost::optional<Schedule> pumpFlowRateSchedule() const;

  boost::optional<ThermalZone> thermalZone() const;

  double skinLossRadiativeFraction() const;

  //@}
  /** @name Setters */
  //@{

  bool setTotalRatedFlowRate(double totalRatedFlowRate);

  void autosizeTotalRatedFlowRate();

  void setNumberofPumpsinBank(int numberofPumpsinBank);

  bool setFlowSequencingControlScheme(std::string flowSequencingControlScheme);

  void setRatedPumpHead(double ratedPumpHead);

  void setRatedPowerConsumption(double ratedPowerConsumption);

  void autosizeRatedPowerConsumption();

  bool setMotorEfficiency(double motorEfficiency);

  bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

  bool setPumpControlType(std::string pumpControlType);

  bool setPumpFlowRateSchedule(Schedule& schedule);

  void resetPumpFlowRateSchedule();

  bool setThermalZone(const ThermalZone& thermalZone);

  void resetThermalZone();

  bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::HeaderedPumpsConstantSpeed_Impl ImplType;

  explicit HeaderedPumpsConstantSpeed(std::shared_ptr<detail::HeaderedPumpsConstantSpeed_Impl> impl);

  friend class detail::HeaderedPumpsConstantSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HeaderedPumpsConstantSpeed");
};

/** \relates HeaderedPumpsConstantSpeed*/
typedef boost::optional<HeaderedPumpsConstantSpeed> OptionalHeaderedPumpsConstantSpeed;

/** \relates HeaderedPumpsConstantSpeed*/
typedef std::vector<HeaderedPumpsConstantSpeed> HeaderedPumpsConstantSpeedVector;

} // model
} // openstudio

#endif // MODEL_HEADEREDPUMPSCONSTANTSPEED_HPP

