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

#ifndef MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_HPP
#define MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerWarmestTemperatureFlow_Impl;

} // detail

/** SetpointManagerWarmestTemperatureFlow is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:WarmestTemperatureFlow'. */
class MODEL_API SetpointManagerWarmestTemperatureFlow : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerWarmestTemperatureFlow(const Model& model);

  virtual ~SetpointManagerWarmestTemperatureFlow() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  static std::vector<std::string> strategyValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double minimumSetpointTemperature() const;

  double maximumSetpointTemperature() const;

  std::string strategy() const;

  double minimumTurndownRatio() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string & controlVariable);

  bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

  bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

  bool setStrategy(std::string strategy);

  bool setMinimumTurndownRatio(double minimumTurndownRatio);

  //@}
  /** @name Other */
  //@{
  boost::optional<Node> setpointNode() const;

  bool setSetpointNode( const Node & node );

  void resetSetpointNode();

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerWarmestTemperatureFlow_Impl ImplType;

  explicit SetpointManagerWarmestTemperatureFlow(std::shared_ptr<detail::SetpointManagerWarmestTemperatureFlow_Impl> impl);

  friend class detail::SetpointManagerWarmestTemperatureFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerWarmestTemperatureFlow");
};

/** \relates SetpointManagerWarmestTemperatureFlow*/
typedef boost::optional<SetpointManagerWarmestTemperatureFlow> OptionalSetpointManagerWarmestTemperatureFlow;

/** \relates SetpointManagerWarmestTemperatureFlow*/
typedef std::vector<SetpointManagerWarmestTemperatureFlow> SetpointManagerWarmestTemperatureFlowVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERWARMESTTEMPERATUREFLOW_HPP

