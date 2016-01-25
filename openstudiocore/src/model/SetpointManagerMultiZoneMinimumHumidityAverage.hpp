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

#ifndef MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerMultiZoneMinimumHumidityAverage_Impl;

} // detail

/** SetpointManagerMultiZoneMinimumHumidityAverage is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:MultiZone:MinimumHumidity:Average'. */
class MODEL_API SetpointManagerMultiZoneMinimumHumidityAverage : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerMultiZoneMinimumHumidityAverage(const Model& model);

  virtual ~SetpointManagerMultiZoneMinimumHumidityAverage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  double minimumSetpointHumidityRatio() const;

  bool isMinimumSetpointHumidityRatioDefaulted() const;

  double maximumSetpointHumidityRatio() const;

  bool isMaximumSetpointHumidityRatioDefaulted() const;

  boost::optional<Node> setpointNode() const;

  std::string controlVariable() const;

  //@}
  /** @name Setters */
  //@{

  bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

  void resetMinimumSetpointHumidityRatio();

  bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

  void resetMaximumSetpointHumidityRatio();

  bool setControlVariable(const std::string& controlVariable);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl ImplType;

  explicit SetpointManagerMultiZoneMinimumHumidityAverage(std::shared_ptr<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl> impl);

  friend class detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerMultiZoneMinimumHumidityAverage");
};

/** \relates SetpointManagerMultiZoneMinimumHumidityAverage*/
typedef boost::optional<SetpointManagerMultiZoneMinimumHumidityAverage> OptionalSetpointManagerMultiZoneMinimumHumidityAverage;

/** \relates SetpointManagerMultiZoneMinimumHumidityAverage*/
typedef std::vector<SetpointManagerMultiZoneMinimumHumidityAverage> SetpointManagerMultiZoneMinimumHumidityAverageVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP

