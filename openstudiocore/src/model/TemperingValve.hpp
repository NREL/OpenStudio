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

#ifndef MODEL_TEMPERINGVALVE_HPP
#define MODEL_TEMPERINGVALVE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  class TemperingValve_Impl;

} // detail

/** TemperingValve is a StraightComponent that wraps the OpenStudio IDD object 'OS:TemperingValve'. */
class MODEL_API TemperingValve : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit TemperingValve(const Model& model);

  virtual ~TemperingValve() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Node> stream2SourceNode() const;

  boost::optional<Node> temperatureSetpointNode() const;

  boost::optional<Node> pumpOutletNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setStream2SourceNode(const Node & stream2SourceNode);

  void resetStream2SourceNode();

  bool setTemperatureSetpointNode(const Node & temperatureSetpointNode);

  void resetTemperatureSetpointNode();

  bool setPumpOutletNode(const Node & pumpOutletNode);

  void resetPumpOutletNode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::TemperingValve_Impl ImplType;

  explicit TemperingValve(std::shared_ptr<detail::TemperingValve_Impl> impl);

  friend class detail::TemperingValve_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.TemperingValve");
};

/** \relates TemperingValve*/
typedef boost::optional<TemperingValve> OptionalTemperingValve;

/** \relates TemperingValve*/
typedef std::vector<TemperingValve> TemperingValveVector;

} // model
} // openstudio

#endif // MODEL_TEMPERINGVALVE_HPP

