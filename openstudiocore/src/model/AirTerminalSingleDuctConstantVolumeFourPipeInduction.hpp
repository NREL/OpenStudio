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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Node;

namespace detail {

  class AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl;

} // detail

/** AirTerminalSingleDuctConstantVolumeFourPipeInduction is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:FourPipeInduction'. */
class MODEL_API AirTerminalSingleDuctConstantVolumeFourPipeInduction : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctConstantVolumeFourPipeInduction(const Model& model, HVACComponent& heatingCoil);

  virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInduction() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumTotalAirFlowRate() const;

  bool isMaximumTotalAirFlowRateAutosized() const;

  double inductionRatio() const;

  bool isInductionRatioDefaulted() const;

  HVACComponent heatingCoil() const;

  boost::optional<double> maximumHotWaterFlowRate() const;

  bool isMaximumHotWaterFlowRateAutosized() const;

  double minimumHotWaterFlowRate() const;

  bool isMinimumHotWaterFlowRateDefaulted() const;

  double heatingConvergenceTolerance() const;

  bool isHeatingConvergenceToleranceDefaulted() const;

  boost::optional<HVACComponent> coolingCoil() const;

  boost::optional<double> maximumColdWaterFlowRate() const;

  bool isMaximumColdWaterFlowRateAutosized() const;

  double minimumColdWaterFlowRate() const;

  bool isMinimumColdWaterFlowRateDefaulted() const;

  double coolingConvergenceTolerance() const;

  bool isCoolingConvergenceToleranceDefaulted() const;

  boost::optional<Node> inducedAirInletNode() const;

  unsigned inducedAirInletPort() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumTotalAirFlowRate(double maximumTotalAirFlowRate);

  void autosizeMaximumTotalAirFlowRate();

  bool setInductionRatio(double inductionRatio);

  void resetInductionRatio();

  bool setHeatingCoil(const HVACComponent& heatingCoil);

  void setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate);

  void resetMaximumHotWaterFlowRate();

  void autosizeMaximumHotWaterFlowRate();

  bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

  void resetMinimumHotWaterFlowRate();

  bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

  void resetHeatingConvergenceTolerance();

  bool setCoolingCoil(const HVACComponent& coolingCoil);

  void resetCoolingCoil();

  void setMaximumColdWaterFlowRate(double maximumColdWaterFlowRate);

  void resetMaximumColdWaterFlowRate();

  void autosizeMaximumColdWaterFlowRate();

  bool setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

  void resetMinimumColdWaterFlowRate();

  bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

  void resetCoolingConvergenceTolerance();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl ImplType;

  explicit AirTerminalSingleDuctConstantVolumeFourPipeInduction(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl> impl);

  friend class detail::AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeFourPipeInduction");
};

/** \relates AirTerminalSingleDuctConstantVolumeFourPipeInduction*/
typedef boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeInduction> OptionalAirTerminalSingleDuctConstantVolumeFourPipeInduction;

/** \relates AirTerminalSingleDuctConstantVolumeFourPipeInduction*/
typedef std::vector<AirTerminalSingleDuctConstantVolumeFourPipeInduction> AirTerminalSingleDuctConstantVolumeFourPipeInductionVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEFOURPIPEINDUCTION_HPP

