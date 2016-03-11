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

#ifndef MODEL_SETPOINTMANAGERMULTIZONECOOLINGAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONECOOLINGAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerMultiZoneCoolingAverage_Impl;

} // detail

/** SetpointManagerMultiZoneCoolingAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:Cooling:Average'. */
class MODEL_API SetpointManagerMultiZoneCoolingAverage : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerMultiZoneCoolingAverage(const Model& model);

  virtual ~SetpointManagerMultiZoneCoolingAverage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double minimumSetpointTemperature() const;

  double maximumSetpointTemperature() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string& controlVariable);

  bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

  bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerMultiZoneCoolingAverage_Impl ImplType;

  explicit SetpointManagerMultiZoneCoolingAverage(std::shared_ptr<detail::SetpointManagerMultiZoneCoolingAverage_Impl> impl);

  friend class detail::SetpointManagerMultiZoneCoolingAverage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneCoolingAverage");
};

/** \relates SetpointManagerMultiZoneCoolingAverage*/
typedef boost::optional<SetpointManagerMultiZoneCoolingAverage> OptionalSetpointManagerMultiZoneCoolingAverage;

/** \relates SetpointManagerMultiZoneCoolingAverage*/
typedef std::vector<SetpointManagerMultiZoneCoolingAverage> SetpointManagerMultiZoneCoolingAverageVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERMULTIZONECOOLINGAVERAGE_HPP

