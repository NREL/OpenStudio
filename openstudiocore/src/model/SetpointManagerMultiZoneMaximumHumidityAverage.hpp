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

#ifndef MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerMultiZoneMaximumHumidityAverage_Impl;

} // detail

/** SetpointManagerMultiZoneMaximumHumidityAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:MaximumHumidity:Average'. */
class MODEL_API SetpointManagerMultiZoneMaximumHumidityAverage : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerMultiZoneMaximumHumidityAverage(const Model& model);

  virtual ~SetpointManagerMultiZoneMaximumHumidityAverage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double minimumSetpointHumidityRatio() const;

  double maximumSetpointHumidityRatio() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string& controlVariable);

  bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

  bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl ImplType;

  explicit SetpointManagerMultiZoneMaximumHumidityAverage(std::shared_ptr<detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl> impl);

  friend class detail::SetpointManagerMultiZoneMaximumHumidityAverage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneMaximumHumidityAverage");
};

/** \relates SetpointManagerMultiZoneMaximumHumidityAverage*/
typedef boost::optional<SetpointManagerMultiZoneMaximumHumidityAverage> OptionalSetpointManagerMultiZoneMaximumHumidityAverage;

/** \relates SetpointManagerMultiZoneMaximumHumidityAverage*/
typedef std::vector<SetpointManagerMultiZoneMaximumHumidityAverage> SetpointManagerMultiZoneMaximumHumidityAverageVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERMULTIZONEMAXIMUMHUMIDITYAVERAGE_HPP

