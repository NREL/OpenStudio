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

#ifndef MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

class AirTerminalSingleDuctParallelPIUReheat_Impl;

} // detail

/** AirTerminalSingleDuctParallelPIUReheat is a StraightComponent that wraps the
 *  OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ParallelPIU:Reheat'. */
class MODEL_API AirTerminalSingleDuctParallelPIUReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctParallelPIUReheat(const Model& model,
                                                  Schedule & schedule,
                                                  HVACComponent & fan,
                                                  HVACComponent & reheatCoil );

  virtual ~AirTerminalSingleDuctParallelPIUReheat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> maximumPrimaryAirFlowRate() const;

  bool isMaximumPrimaryAirFlowRateAutosized() const;

  boost::optional<double> maximumSecondaryAirFlowRate() const;

  bool isMaximumSecondaryAirFlowRateAutosized() const;

  boost::optional<double> minimumPrimaryAirFlowFraction() const;

  bool isMinimumPrimaryAirFlowFractionAutosized() const;

  boost::optional<double> fanOnFlowFraction() const;

  bool isFanOnFlowFractionAutosized() const;

  boost::optional<Node> secondaryAirInletNode() const;

  unsigned secondaryAirInletPort() const;

  HVACComponent fan() const;

  HVACComponent reheatCoil() const;

  boost::optional<double> maximumHotWaterorSteamFlowRate() const;

  bool isMaximumHotWaterorSteamFlowRateAutosized() const;

  double minimumHotWaterorSteamFlowRate() const;

  bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

  double convergenceTolerance() const;

  bool isConvergenceToleranceDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule & schedule);

  void setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate);

  void autosizeMaximumPrimaryAirFlowRate();

  void setMaximumSecondaryAirFlowRate(double maximumSecondaryAirFlowRate);

  void autosizeMaximumSecondaryAirFlowRate();

  void setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction);

  void autosizeMinimumPrimaryAirFlowFraction();

  void setFanOnFlowFraction(double fanOnFlowFraction);

  void autosizeFanOnFlowFraction();

  void setFan( HVACComponent & hvacComponent );

  void setReheatCoil( HVACComponent & hvacComponent );

  void setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

  void resetMaximumHotWaterorSteamFlowRate();

  void autosizeMaximumHotWaterorSteamFlowRate();

  bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

  void resetMinimumHotWaterorSteamFlowRate();

  bool setConvergenceTolerance(double convergenceTolerance);

  void resetConvergenceTolerance();

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctParallelPIUReheat_Impl ImplType;

  friend class Model;

  friend class openstudio::IdfObject;

  friend class detail::AirTerminalSingleDuctParallelPIUReheat_Impl;

  explicit AirTerminalSingleDuctParallelPIUReheat(std::shared_ptr<detail::AirTerminalSingleDuctParallelPIUReheat_Impl> impl);
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctParallelPIUReheat");
};

/** \relates AirTerminalSingleDuctParallelPIUReheat*/
typedef boost::optional<AirTerminalSingleDuctParallelPIUReheat> OptionalAirTerminalSingleDuctParallelPIUReheat;

/** \relates AirTerminalSingleDuctParallelPIUReheat*/
typedef std::vector<AirTerminalSingleDuctParallelPIUReheat> AirTerminalSingleDuctParallelPIUReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTPARALLELPIUREHEAT_HPP

