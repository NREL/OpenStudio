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

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Node;
class Schedule;

namespace detail {

  class SetpointManagerOutdoorAirReset_Impl;

} // detail

/** SetpointManagerOutdoorAirReset is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirReset'. */
class MODEL_API SetpointManagerOutdoorAirReset : public SetpointManager {
  public:

  explicit SetpointManagerOutdoorAirReset(const Model& model);

  virtual ~SetpointManagerOutdoorAirReset() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();


  boost::optional<Node> setpointNode() const;


  std::string controlVariable() const;

  bool isControlVariableDefaulted() const;

  bool setControlVariable(const std::string& controlVariable);

  void resetControlVariable();


  double setpointatOutdoorLowTemperature() const;

  Quantity getSetpointatOutdoorLowTemperature(bool returnIP=false) const;

  void setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

  bool setSetpointatOutdoorLowTemperature(const Quantity& setpointatOutdoorLowTemperature);


  double outdoorLowTemperature() const;

  Quantity getOutdoorLowTemperature(bool returnIP=false) const;

  void setOutdoorLowTemperature(double outdoorLowTemperature);

  bool setOutdoorLowTemperature(const Quantity& outdoorLowTemperature);


  double setpointatOutdoorHighTemperature() const;

  Quantity getSetpointatOutdoorHighTemperature(bool returnIP=false) const;

  void setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

  bool setSetpointatOutdoorHighTemperature(const Quantity& setpointatOutdoorHighTemperature);


  double outdoorHighTemperature() const;

  Quantity getOutdoorHighTemperature(bool returnIP=false) const;

  void setOutdoorHighTemperature(double outdoorHighTemperature);

  bool setOutdoorHighTemperature(const Quantity& outdoorHighTemperature);


  boost::optional<Schedule> schedule() const;

  bool setSchedule(Schedule& schedule);

  void resetSchedule();


  boost::optional<double> setpointatOutdoorLowTemperature2() const;

  OSOptionalQuantity getSetpointatOutdoorLowTemperature2(bool returnIP=false) const;

  void setSetpointatOutdoorLowTemperature2(double setpointatOutdoorLowTemperature2);

  bool setSetpointatOutdoorLowTemperature2(const Quantity& setpointatOutdoorLowTemperature2);

  void resetSetpointatOutdoorLowTemperature2();


  boost::optional<double> outdoorLowTemperature2() const;

  OSOptionalQuantity getOutdoorLowTemperature2(bool returnIP=false) const;

  void setOutdoorLowTemperature2(double outdoorLowTemperature2);

  bool setOutdoorLowTemperature2(const Quantity& outdoorLowTemperature2);

  void resetOutdoorLowTemperature2();


  boost::optional<double> setpointatOutdoorHighTemperature2() const;

  OSOptionalQuantity getSetpointatOutdoorHighTemperature2(bool returnIP=false) const;

  void setSetpointatOutdoorHighTemperature2(double setpointatOutdoorHighTemperature2);

  bool setSetpointatOutdoorHighTemperature2(const Quantity& setpointatOutdoorHighTemperature2);

  void resetSetpointatOutdoorHighTemperature2();


  boost::optional<double> outdoorHighTemperature2() const;

  OSOptionalQuantity getOutdoorHighTemperature2(bool returnIP=false) const;

  void setOutdoorHighTemperature2(double outdoorHighTemperature2);

  bool setOutdoorHighTemperature2(const Quantity& outdoorHighTemperature2);

  void resetOutdoorHighTemperature2();

 protected:
  
  /// @cond
  typedef detail::SetpointManagerOutdoorAirReset_Impl ImplType;

  explicit SetpointManagerOutdoorAirReset(std::shared_ptr<detail::SetpointManagerOutdoorAirReset_Impl> impl);

  friend class detail::SetpointManagerOutdoorAirReset_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirReset");
};

/** \relates SetpointManagerOutdoorAirReset*/
typedef boost::optional<SetpointManagerOutdoorAirReset> OptionalSetpointManagerOutdoorAirReset;

/** \relates SetpointManagerOutdoorAirReset*/
typedef std::vector<SetpointManagerOutdoorAirReset> SetpointManagerOutdoorAirResetVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP

