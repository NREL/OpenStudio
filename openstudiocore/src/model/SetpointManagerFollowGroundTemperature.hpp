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

#ifndef MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerFollowGroundTemperature_Impl;

} // detail

/** SetpointManagerFollowGroundTemperature is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:FollowGroundTemperature'. */
class MODEL_API SetpointManagerFollowGroundTemperature : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerFollowGroundTemperature(const Model& model);

  virtual ~SetpointManagerFollowGroundTemperature() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double offsetTemperatureDifference() const;

  double maximumSetpointTemperature() const;

  double minimumSetpointTemperature() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string& controlVariable);

  void setOffsetTemperatureDifference(double offsetTemperatureDifference);

  void setMaximumSetpointTemperature(double maximumSetpointTemperature);

  void setMinimumSetpointTemperature(double minimumSetpointTemperature);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerFollowGroundTemperature_Impl ImplType;

  explicit SetpointManagerFollowGroundTemperature(std::shared_ptr<detail::SetpointManagerFollowGroundTemperature_Impl> impl);

  friend class detail::SetpointManagerFollowGroundTemperature_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerFollowGroundTemperature");
};

/** \relates SetpointManagerFollowGroundTemperature*/
typedef boost::optional<SetpointManagerFollowGroundTemperature> OptionalSetpointManagerFollowGroundTemperature;

/** \relates SetpointManagerFollowGroundTemperature*/
typedef std::vector<SetpointManagerFollowGroundTemperature> SetpointManagerFollowGroundTemperatureVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP

