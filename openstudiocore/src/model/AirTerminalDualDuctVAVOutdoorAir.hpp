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

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class Connection;
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

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> outdoorAirInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> recirculatedAirInletNode() const;

  boost::optional<double> maximumTerminalAirFlowRate() const;

  bool isMaximumTerminalAirFlowRateAutosized() const;

  // TODO: Check return type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
  DesignSpecificationOutdoorAir designSpecificationOutdoorAirObject() const;

  std::string perPersonVentilationRateMode() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirOutletNode(const Connection& connection);

  void resetAirOutletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setOutdoorAirInletNode(const Connection& connection);

  void resetOutdoorAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setRecirculatedAirInletNode(const Connection& connection);

  void resetRecirculatedAirInletNode();

  bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

  void autosizeMaximumTerminalAirFlowRate();

  // TODO: Check argument type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
  bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

  bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

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

