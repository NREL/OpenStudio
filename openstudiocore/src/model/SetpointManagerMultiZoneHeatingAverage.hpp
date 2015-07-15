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

#ifndef MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Node;

namespace detail {

  class SetpointManagerMultiZoneHeatingAverage_Impl;

} // detail

/** SetpointManagerMultiZoneHeatingAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:Heating:Average'. */
class MODEL_API SetpointManagerMultiZoneHeatingAverage : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerMultiZoneHeatingAverage(const Model& model);

  virtual ~SetpointManagerMultiZoneHeatingAverage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double minimumSetpointTemperature() const;

  double maximumSetpointTemperature() const;

  // TODO: Check return type. From object lists, some candidates are: Node.
  boost::optional<Node> setpointNodeorNodeList() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(std::string controlVariable);

  bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

  bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

  // TODO: Check argument type. From object lists, some candidates are: Node.
  bool setSetpointNodeorNodeList(const Node& node);

  void resetSetpointNodeorNodeList();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerMultiZoneHeatingAverage_Impl ImplType;

  explicit SetpointManagerMultiZoneHeatingAverage(std::shared_ptr<detail::SetpointManagerMultiZoneHeatingAverage_Impl> impl);

  friend class detail::SetpointManagerMultiZoneHeatingAverage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneHeatingAverage");
};

/** \relates SetpointManagerMultiZoneHeatingAverage*/
typedef boost::optional<SetpointManagerMultiZoneHeatingAverage> OptionalSetpointManagerMultiZoneHeatingAverage;

/** \relates SetpointManagerMultiZoneHeatingAverage*/
typedef std::vector<SetpointManagerMultiZoneHeatingAverage> SetpointManagerMultiZoneHeatingAverageVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERMULTIZONEHEATINGAVERAGE_HPP

