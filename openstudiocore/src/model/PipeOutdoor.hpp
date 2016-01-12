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

#ifndef MODEL_PIPEOUTDOOR_HPP
#define MODEL_PIPEOUTDOOR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Construction;
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

  boost::optional<Construction> construction() const;

  boost::optional<Node> ambientTemperatureOutdoorAirNode() const;

  double pipeInsideDiameter() const;

  double pipeLength() const;

  //@}
  /** @name Setters */
  //@{

  bool setConstruction(const Construction& construction);

  void resetConstruction();

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

