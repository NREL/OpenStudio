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

#ifndef MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP
#define MODEL_SETPOINTMANAGERMULTIZONEMINIMUMHUMIDITYAVERAGE_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
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

  // TODO: Check return type. From object lists, some candidates are: Node.
  Node setpointNodeorNodeList() const;

  std::string controlVariable() const;

  bool isControlVariableDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

  void resetMinimumSetpointHumidityRatio();

  bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

  void resetMaximumSetpointHumidityRatio();

  // TODO: Check argument type. From object lists, some candidates are: Node.
  bool setSetpointNodeorNodeList(const Node& node);

  bool setControlVariable(std::string controlVariable);

  void resetControlVariable();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl ImplType;

  explicit SetpointManagerMultiZoneMinimumHumidityAverage(boost::shared_ptr<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl> impl);

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

