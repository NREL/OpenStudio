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

#ifndef MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class AirTerminalSingleDuctSeriesPIUReheat_Impl;

} // detail

/** AirTerminalSingleDuctSeriesPIUReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:SeriesPIU:Reheat'. */
class MODEL_API AirTerminalSingleDuctSeriesPIUReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctSeriesPIUReheat(const Model& model,
                                                HVACComponent & fan, 
                                                HVACComponent & reheatCoil);

  virtual ~AirTerminalSingleDuctSeriesPIUReheat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumAirFlowRate() const;

  bool isMaximumAirFlowRateAutosized() const;

  boost::optional<double> maximumPrimaryAirFlowRate() const;

  bool isMaximumPrimaryAirFlowRateAutosized() const;

  boost::optional<double> minimumPrimaryAirFlowFraction() const;

  bool isMinimumPrimaryAirFlowFractionAutosized() const;

  HVACComponent fan() const;

  HVACComponent reheatCoil() const;

  boost::optional<double> maximumHotWaterorSteamFlowRate() const;

  bool isMaximumHotWaterorSteamFlowRateAutosized() const;

  double minimumHotWaterorSteamFlowRate() const;

  double convergenceTolerance() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  bool setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate);

  void autosizeMaximumPrimaryAirFlowRate();

  bool setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction);

  void autosizeMinimumPrimaryAirFlowFraction();

  bool setFan(const HVACComponent& fan);

  bool setReheatCoil(const HVACComponent& coil);

  void setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

  void resetMaximumHotWaterorSteamFlowRate();

  void autosizeMaximumHotWaterorSteamFlowRate();

  bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

  bool setConvergenceTolerance(double convergenceTolerance);

  //@}
  /** @name Other */
  //@{

  unsigned secondaryAirInletPort() const;
  boost::optional<Node> secondaryAirInletNode() const;

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctSeriesPIUReheat_Impl ImplType;

  explicit AirTerminalSingleDuctSeriesPIUReheat(std::shared_ptr<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl> impl);

  friend class detail::AirTerminalSingleDuctSeriesPIUReheat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctSeriesPIUReheat");
};

/** \relates AirTerminalSingleDuctSeriesPIUReheat*/
typedef boost::optional<AirTerminalSingleDuctSeriesPIUReheat> OptionalAirTerminalSingleDuctSeriesPIUReheat;

/** \relates AirTerminalSingleDuctSeriesPIUReheat*/
typedef std::vector<AirTerminalSingleDuctSeriesPIUReheat> AirTerminalSingleDuctSeriesPIUReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP

