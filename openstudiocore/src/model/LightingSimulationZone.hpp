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

#ifndef MODEL_LIGHTINGSIMULATIONZONE_HPP
#define MODEL_LIGHTINGSIMULATIONZONE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class RenderingColor;
class Space;

namespace detail {

  class LightingSimulationZone_Impl;

} // detail

// ETH@20120328 Should LightingSimulationZone be parented by Building, 
// similar to ThermalZone?

/** LightingSimulationZone is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingSimulationZone'. */
class MODEL_API LightingSimulationZone : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit LightingSimulationZone(const Model& model);

  virtual ~LightingSimulationZone() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  //@}
  /** @name Setters */
  //@{

  //@}

  /// Returns the rendering color.
  boost::optional<RenderingColor> renderingColor() const;

  /// Sets the rendering color.
  bool setRenderingColor(const RenderingColor& renderingColor);

  /// Resets the rendering color.
  void resetRenderingColor();

  /// Returns all spaces in this lighting simulation zone.
  std::vector<Space> spaces() const;

  /// Adds a spaces to this lighting simulation zone.
  bool addSpace(const Space& space);

  /// Resets spaces in this lighting simulation zone.
  void resetSpaces();

 protected:
  /// @cond
  typedef detail::LightingSimulationZone_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit LightingSimulationZone(std::shared_ptr<detail::LightingSimulationZone_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.LightingSimulationZone");
};

/** \relates LightingSimulationZone*/
typedef boost::optional<LightingSimulationZone> OptionalLightingSimulationZone;

/** \relates LightingSimulationZone*/
typedef std::vector<LightingSimulationZone> LightingSimulationZoneVector;

} // model
} // openstudio

#endif // MODEL_LIGHTINGSIMULATIONZONE_HPP

