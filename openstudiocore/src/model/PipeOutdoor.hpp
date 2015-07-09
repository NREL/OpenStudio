/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_PIPEOUTDOOR_HPP
#define MODEL_PIPEOUTDOOR_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Construction;
class Connection;
class Connection;
class Node;

namespace detail {

  class PipeOutdoor_Impl;

} // detail

/** PipeOutdoor is a StraightComponent that wraps the OpenStudio IDD object 'OS:Pipe:Outdoor'. */
class MODEL_API PipeOutdoor : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PipeOutdoor(const Model& model);

  virtual ~PipeOutdoor() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Construction.
  boost::optional<Construction> construction() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> fluidInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> fluidOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Node.
  boost::optional<Node> ambientTemperatureOutdoorAirNode() const;

  double pipeInsideDiameter() const;

  double pipeLength() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Construction.
  bool setConstruction(const Construction& construction);

  void resetConstruction();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setFluidInletNode(const Connection& connection);

  void resetFluidInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setFluidOutletNode(const Connection& connection);

  void resetFluidOutletNode();

  // TODO: Check argument type. From object lists, some candidates are: Node.
  bool setAmbientTemperatureOutdoorAirNode(const Node& node);

  void resetAmbientTemperatureOutdoorAirNode();

  bool setPipeInsideDiameter(double pipeInsideDiameter);

  bool setPipeLength(double pipeLength);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::PipeOutdoor_Impl ImplType;

  explicit PipeOutdoor(std::shared_ptr<detail::PipeOutdoor_Impl> impl);

  friend class detail::PipeOutdoor_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PipeOutdoor");
};

/** \relates PipeOutdoor*/
typedef boost::optional<PipeOutdoor> OptionalPipeOutdoor;

/** \relates PipeOutdoor*/
typedef std::vector<PipeOutdoor> PipeOutdoorVector;

} // model
} // openstudio

#endif // MODEL_PIPEOUTDOOR_HPP

