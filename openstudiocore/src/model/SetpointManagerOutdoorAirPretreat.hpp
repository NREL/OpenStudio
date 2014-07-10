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

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager.hpp>

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class SetpointManagerOutdoorAirPretreat_Impl;

} // detail

/** SetpointManagerOutdoorAirPretreat is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirPretreat'. */
class MODEL_API SetpointManagerOutdoorAirPretreat : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerOutdoorAirPretreat(const Model& model);

  virtual ~SetpointManagerOutdoorAirPretreat() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double minimumSetpointTemperature() const;

  bool isMinimumSetpointTemperatureDefaulted() const;

  double maximumSetpointTemperature() const;

  bool isMaximumSetpointTemperatureDefaulted() const;

  double minimumSetpointHumidityRatio() const;

  bool isMinimumSetpointHumidityRatioDefaulted() const;

  double maximumSetpointHumidityRatio() const;

  bool isMaximumSetpointHumidityRatioDefaulted() const;

  boost::optional<Node> referenceSetpointNode() const;

  boost::optional<Node> mixedAirStreamNode() const;

  boost::optional<Node> outdoorAirStreamNode() const;

  boost::optional<Node> returnAirStreamNode() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(const std::string& controlVariable);

  void resetControlVariable();

  bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

  void resetMinimumSetpointTemperature();

  bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

  void resetMaximumSetpointTemperature();

  bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

  void resetMinimumSetpointHumidityRatio();

  bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

  void resetMaximumSetpointHumidityRatio();

  bool setReferenceSetpointNode(const Node& node);

  void resetReferenceSetpointNode();

  bool setMixedAirStreamNode(const Node& node);

  void resetMixedAirStreamNode();

  bool setOutdoorAirStreamNode(const Node& node);

  void resetOutdoorAirStreamNode();

  bool setReturnAirStreamNode(const Node& node);

  void resetReturnAirStreamNode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerOutdoorAirPretreat_Impl ImplType;

  explicit SetpointManagerOutdoorAirPretreat(std::shared_ptr<detail::SetpointManagerOutdoorAirPretreat_Impl> impl);

  friend class detail::SetpointManagerOutdoorAirPretreat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirPretreat");
};

/** \relates SetpointManagerOutdoorAirPretreat*/
typedef boost::optional<SetpointManagerOutdoorAirPretreat> OptionalSetpointManagerOutdoorAirPretreat;

/** \relates SetpointManagerOutdoorAirPretreat*/
typedef std::vector<SetpointManagerOutdoorAirPretreat> SetpointManagerOutdoorAirPretreatVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP

