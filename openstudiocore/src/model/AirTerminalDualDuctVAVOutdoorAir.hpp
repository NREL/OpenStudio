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

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP

#include <model/ModelAPI.hpp>
#include "Mixer.hpp"

namespace openstudio {

namespace model {

class Schedule;
class AirLoopHVAC;
class Model;
class Node;
class DesignSpecificationOutdoorAir;

namespace detail {

  class AirTerminalDualDuctVAVOutdoorAir_Impl;

} // detail

/** AirTerminalDualDuctVAVOutdoorAir is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:VAV:OutdoorAir'. */
class MODEL_API AirTerminalDualDuctVAVOutdoorAir : public Mixer {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalDualDuctVAVOutdoorAir(const Model& model);

  virtual ~AirTerminalDualDuctVAVOutdoorAir() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> perPersonVentilationRateModeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumTerminalAirFlowRate() const;

  bool isMaximumTerminalAirFlowRateAutosized() const;

  /*// TODO: Check return type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
  DesignSpecificationOutdoorAir designSpecificationOutdoorAirObject() const;
  */

  std::string perPersonVentilationRateMode() const;

  //@}
  /** @name Setters */
  //@{

  // Note Schedules are passed by reference, not const reference.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

  void autosizeMaximumTerminalAirFlowRate();

  bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

  bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

  boost::optional<Node> OutdoorAirInletNode() const;

  boost::optional<Node> RecirculatedAirInletNode() const;


  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalDualDuctVAVOutdoorAir_Impl ImplType;

  explicit AirTerminalDualDuctVAVOutdoorAir(std::shared_ptr<detail::AirTerminalDualDuctVAVOutdoorAir_Impl> impl);

  friend class detail::AirTerminalDualDuctVAVOutdoorAir_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");
};

/** \relates AirTerminalDualDuctVAVOutdoorAir*/
typedef boost::optional<AirTerminalDualDuctVAVOutdoorAir> OptionalAirTerminalDualDuctVAVOutdoorAir;

/** \relates AirTerminalDualDuctVAVOutdoorAir*/
typedef std::vector<AirTerminalDualDuctVAVOutdoorAir> AirTerminalDualDuctVAVOutdoorAirVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP

