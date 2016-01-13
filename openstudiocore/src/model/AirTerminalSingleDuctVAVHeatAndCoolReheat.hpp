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

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl;

} // detail

/** AirTerminalSingleDuctVAVHeatAndCoolReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:VAV:HeatAndCool:Reheat'. */
class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctVAVHeatAndCoolReheat(const Model& model, const HVACComponent& reheatCoil);

  virtual ~AirTerminalSingleDuctVAVHeatAndCoolReheat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumAirFlowRate() const;

  bool isMaximumAirFlowRateAutosized() const;

  double zoneMinimumAirFlowFraction() const;

  HVACComponent reheatCoil() const;

  boost::optional<double> maximumHotWaterorSteamFlowRate() const;

  bool isMaximumHotWaterorSteamFlowRateAutosized() const;

  double minimumHotWaterorSteamFlowRate() const;

  double convergenceTolerance() const;

  double maximumReheatAirTemperature() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

  bool setReheatCoil(const HVACComponent& heatingCoilName);

  bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

  void autosizeMaximumHotWaterorSteamFlowRate();

  bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

  bool setConvergenceTolerance(double convergenceTolerance);

  bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl ImplType;

  explicit AirTerminalSingleDuctVAVHeatAndCoolReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl> impl);

  friend class detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolReheat");
};

/** \relates AirTerminalSingleDuctVAVHeatAndCoolReheat*/
typedef boost::optional<AirTerminalSingleDuctVAVHeatAndCoolReheat> OptionalAirTerminalSingleDuctVAVHeatAndCoolReheat;

/** \relates AirTerminalSingleDuctVAVHeatAndCoolReheat*/
typedef std::vector<AirTerminalSingleDuctVAVHeatAndCoolReheat> AirTerminalSingleDuctVAVHeatAndCoolReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLREHEAT_HPP

