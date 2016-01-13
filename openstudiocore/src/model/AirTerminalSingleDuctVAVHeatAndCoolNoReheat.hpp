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

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl;

} // detail

/** AirTerminalSingleDuctVAVHeatAndCoolNoReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:VAV:HeatAndCool:NoReheat'. */
class MODEL_API AirTerminalSingleDuctVAVHeatAndCoolNoReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctVAVHeatAndCoolNoReheat(const Model& model);

  virtual ~AirTerminalSingleDuctVAVHeatAndCoolNoReheat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumAirFlowRate() const;

  bool isMaximumAirFlowRateAutosized() const;

  double zoneMinimumAirFlowFraction() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  bool setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl ImplType;

  explicit AirTerminalSingleDuctVAVHeatAndCoolNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl> impl);

  friend class detail::AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVHeatAndCoolNoReheat");
};

/** \relates AirTerminalSingleDuctVAVHeatAndCoolNoReheat*/
typedef boost::optional<AirTerminalSingleDuctVAVHeatAndCoolNoReheat> OptionalAirTerminalSingleDuctVAVHeatAndCoolNoReheat;

/** \relates AirTerminalSingleDuctVAVHeatAndCoolNoReheat*/
typedef std::vector<AirTerminalSingleDuctVAVHeatAndCoolNoReheat> AirTerminalSingleDuctVAVHeatAndCoolNoReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVHEATANDCOOLNOREHEAT_HPP

