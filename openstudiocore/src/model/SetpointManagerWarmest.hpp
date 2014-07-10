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

#ifndef MODEL_SETPOINTMANAGERWARMEST_HPP
#define MODEL_SETPOINTMANAGERWARMEST_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerWarmest_Impl;

} // detail

/** SetpointManagerWarmest is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:Warmest'. */
class MODEL_API SetpointManagerWarmest : public SetpointManager {
 public:

  explicit SetpointManagerWarmest(const Model& model);

  virtual ~SetpointManagerWarmest() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  static std::vector<std::string> strategyValues();

  std::string controlVariable() const;

  bool setControlVariable(const std::string& controlVariable);

  double minimumSetpointTemperature() const;

  bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

  double maximumSetpointTemperature() const;

  bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

  std::string strategy() const;

  bool setStrategy(const std::string& strategy);

  boost::optional<Node> setpointNode() const;

 protected:

  /// @cond
  typedef detail::SetpointManagerWarmest_Impl ImplType;

  explicit SetpointManagerWarmest(std::shared_ptr<detail::SetpointManagerWarmest_Impl> impl);

  friend class detail::SetpointManagerWarmest_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.SetpointManagerWarmest");
};

/** \relates SetpointManagerWarmest*/
typedef boost::optional<SetpointManagerWarmest> OptionalSetpointManagerWarmest;

/** \relates SetpointManagerWarmest*/
typedef std::vector<SetpointManagerWarmest> SetpointManagerWarmestVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERWARMEST_HPP

