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

#ifndef MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_HPP
#define MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

class Schedule;
class AirLoopHVAC;
class Model;
class Node;

namespace detail {

  class AirTerminalDualDuctConstantVolume_Impl;

} // detail

/** AirTerminalDualDuctConstantVolume is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:ConstantVolume'. */
class MODEL_API AirTerminalDualDuctConstantVolume : public Mixer {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalDualDuctConstantVolume(const Model& model);

  virtual ~AirTerminalDualDuctConstantVolume() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> maximumAirFlowRate() const;

  bool isMaximumAirFlowRateAutosized() const;

  //@}
  /** @name Setters */
  //@{

  // Note Schedules are passed by reference, not const reference.
  bool setAvailabilitySchedule(Schedule& schedule);

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  //@}
  /** @name Other */
  //@{

  /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
   * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
   * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
   * but here in the terminal interface we use the language for transparency with the idf.
   */
  boost::optional<Node> hotAirInletNode() const;

  /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
   * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
   */
  boost::optional<Node> coldAirInletNode() const;

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalDualDuctConstantVolume_Impl ImplType;

  explicit AirTerminalDualDuctConstantVolume(std::shared_ptr<detail::AirTerminalDualDuctConstantVolume_Impl> impl);

  friend class detail::AirTerminalDualDuctConstantVolume_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctConstantVolume");
};

/** \relates AirTerminalDualDuctConstantVolume*/
typedef boost::optional<AirTerminalDualDuctConstantVolume> OptionalAirTerminalDualDuctConstantVolume;

/** \relates AirTerminalDualDuctConstantVolume*/
typedef std::vector<AirTerminalDualDuctConstantVolume> AirTerminalDualDuctConstantVolumeVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTCONSTANTVOLUME_HPP

