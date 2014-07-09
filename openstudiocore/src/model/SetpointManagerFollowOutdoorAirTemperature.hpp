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

#ifndef MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

class SetpointManagerFollowOutdoorAirTemperature_Impl;

}

/** SetpointManagerFollowOutdoorAirTemperature is an SetpointManager that wraps the IDD object
 *  named "OS:SetpointManager:FollowOutdoorAirTemperature"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManagerFollowOutdoorAirTemperature object.
 */
class MODEL_API SetpointManagerFollowOutdoorAirTemperature : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{  

  /** Constructs a new SetpointManagerFollowOutdoorAirTemperature object and places it inside the
   *  model. */
  explicit SetpointManagerFollowOutdoorAirTemperature(const Model& model);

  virtual ~SetpointManagerFollowOutdoorAirTemperature() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters and Setters */
  //@{

  /** Returns the Node referred to by the SetpointNodeName field. **/
  boost::optional<Node> setpointNode() const;

  /** Returns the Control Variable **/
  std::string controlVariable() const;

  /** Sets the Control Variable **/
  bool setControlVariable(const std::string & value);

  std::string referenceTemperatureType() const;

  void setReferenceTemperatureType(const std::string & value);

  double offsetTemperatureDifference() const;

  void setOffsetTemperatureDifference(double value);

  double maximumSetpointTemperature() const;

  void setMaximumSetpointTemperature(double value);

  double minimumSetpointTemperature() const;

  void setMinimumSetpointTemperature(double value);

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  friend class detail::SetpointManagerFollowOutdoorAirTemperature_Impl;

  /// @cond

  typedef detail::SetpointManagerFollowOutdoorAirTemperature_Impl ImplType;

  explicit SetpointManagerFollowOutdoorAirTemperature(std::shared_ptr<detail::SetpointManagerFollowOutdoorAirTemperature_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.SetpointManagerFollowOutdoorAirTemperature");

  /// @endcond
};

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP

