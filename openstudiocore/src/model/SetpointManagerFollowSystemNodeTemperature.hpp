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

#ifndef MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerFollowSystemNodeTemperature_Impl;

} // detail

/** SetpointManagerFollowSystemNodeTemperature is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:FollowSystemNodeTemperature'. */
class MODEL_API SetpointManagerFollowSystemNodeTemperature : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerFollowSystemNodeTemperature(const Model& model);

  virtual ~SetpointManagerFollowSystemNodeTemperature() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  static std::vector<std::string> referenceTemperatureTypeValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  boost::optional<Node> referenceNode() const;

  std::string referenceTemperatureType() const;

  double offsetTemperatureDifference() const;

  double maximumLimitSetpointTemperature() const;

  double minimumLimitSetpointTemperature() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string& controlVariable);

  bool setReferenceNode(const Node& node);

  void resetReferenceNode();

  bool setReferenceTemperatureType(const std::string& referenceTemperatureType);

  void setOffsetTemperatureDifference(double offsetTemperatureDifference);

  void setMaximumLimitSetpointTemperature(double maximumLimitSetpointTemperature);

  void setMinimumLimitSetpointTemperature(double minimumLimitSetpointTemperature);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerFollowSystemNodeTemperature_Impl ImplType;

  explicit SetpointManagerFollowSystemNodeTemperature(std::shared_ptr<detail::SetpointManagerFollowSystemNodeTemperature_Impl> impl);

  friend class detail::SetpointManagerFollowSystemNodeTemperature_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerFollowSystemNodeTemperature");
};

/** \relates SetpointManagerFollowSystemNodeTemperature*/
typedef boost::optional<SetpointManagerFollowSystemNodeTemperature> OptionalSetpointManagerFollowSystemNodeTemperature;

/** \relates SetpointManagerFollowSystemNodeTemperature*/
typedef std::vector<SetpointManagerFollowSystemNodeTemperature> SetpointManagerFollowSystemNodeTemperatureVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_HPP

