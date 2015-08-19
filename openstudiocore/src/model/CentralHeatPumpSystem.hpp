/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Connection;
class Connection;
class Connection;
class Connection;
class Schedule;

namespace detail {

  class CentralHeatPumpSystem_Impl;

} // detail

/** CentralHeatPumpSystem is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:CentralHeatPumpSystem'. */
class MODEL_API CentralHeatPumpSystem : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CentralHeatPumpSystem(const Model& model);

  virtual ~CentralHeatPumpSystem() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlMethodValues();

  /** @name Getters */
  //@{

  std::string controlMethod() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection coolingLoopInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection coolingLoopOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection sourceLoopInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection sourceLoopOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection heatingLoopInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection heatingLoopOutletNode() const;

  double ancillaryPower() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> ancillaryOperationSchedule() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  bool setControlMethod(std::string controlMethod);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setCoolingLoopInletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setCoolingLoopOutletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setSourceLoopInletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setSourceLoopOutletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setHeatingLoopInletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setHeatingLoopOutletNode(const Connection& connection);

  bool setAncillaryPower(double ancillaryPower);

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAncillaryOperationSchedule(Schedule& schedule);

  void resetAncillaryOperationSchedule();

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CentralHeatPumpSystem_Impl ImplType;

  explicit CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl);

  friend class detail::CentralHeatPumpSystem_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");
};

/** \relates CentralHeatPumpSystem*/
typedef boost::optional<CentralHeatPumpSystem> OptionalCentralHeatPumpSystem;

/** \relates CentralHeatPumpSystem*/
typedef std::vector<CentralHeatPumpSystem> CentralHeatPumpSystemVector;

} // model
} // openstudio

#endif // MODEL_CENTRALHEATPUMPSYSTEM_HPP

