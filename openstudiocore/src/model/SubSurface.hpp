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

#ifndef MODEL_SUBSURFACE_HPP
#define MODEL_SUBSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

class Surface;
class ShadingSurface;
class ShadingControl;
class ShadingSurfaceGroup;
class DaylightingDeviceShelf;

namespace detail {

  class SubSurface_Impl;

} // detail

/** SubSurface is a PlanarSurface that wraps the OpenStudio IDD object 'OS_SubSurface'. */
class MODEL_API SubSurface : public PlanarSurface {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SubSurface(const std::vector<Point3d>& vertices, const Model& model);

  virtual ~SubSurface() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validSubSurfaceTypeValues();

  //@}
  /** @name Getters */
  //@{

  std::string subSurfaceType() const;

  boost::optional<double> viewFactortoGround() const;

  bool isViewFactortoGroundDefaulted() const;

  bool isViewFactortoGroundAutocalculated() const;

  bool allowShadingControl() const;

  boost::optional<ShadingControl> shadingControl() const;

  // TODO: Handle Non-Extensible IddField Frame and Divider Name.

  double multiplier() const;

  bool isMultiplierDefaulted() const;

  boost::optional<double> numberofVertices() const;

  bool isNumberofVerticesDefaulted() const;

  bool isNumberofVerticesAutocalculated() const;

  //@}
  /** @name Setters */
  //@{

  bool setSubSurfaceType(std::string subSurfaceType);

  bool setViewFactortoGround(boost::optional<double> viewFactortoGround);

  bool setViewFactortoGround(double viewFactortoGround);

  void resetViewFactortoGround();

  void autocalculateViewFactortoGround();

  bool setShadingControl(const ShadingControl& shadingControl);

  void resetShadingControl();

  // TODO: Handle Non-Extensible IddField Frame and Divider Name.

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  bool setNumberofVertices(boost::optional<double> numberofVertices);

  bool setNumberofVertices(double numberofVertices);

  void resetNumberofVertices();

  void autocalculateNumberofVertices();

  //@}

  // TODO: test that area is correct with multiplier
  
  /// get the surface
  boost::optional<Surface> surface() const;

  /// set the surface
  bool setSurface(const Surface& surface);

  /// get the adjacent subsurface
  boost::optional<SubSurface> adjacentSubSurface() const;

  /// set the adjacent subsurface, will fail unless both sub surfaces are parented by surfaces
  /// which are adjacent
  bool setAdjacentSubSurface(SubSurface& subSurface);

  /// reset the adjacent subsurface, will clear references on both this and adjacent sub surface 
  void resetAdjacentSubSurface();

  /** Assign default sub surface type based on vertices. */
  void assignDefaultSubSurfaceType();

  /** Return the surface()'s outsideBoundaryCondition, or an empty string. */
  std::string outsideBoundaryCondition() const;

  /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors. */
  // DLM: todo add argument for horizontal offset 
  boost::optional<ShadingSurface> addOverhang(double depth, double offset);

  /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors. 
   *  Offset is a fraction of the total window height, projection factor is based on height and offset. */
  // DLM: todo add argument for horizontal offset 
  boost::optional<ShadingSurface> addOverhangByProjectionFactor(double projectionFactor, double offsetFraction);

  // DLM: todo add methods to create fins

  /** Returns any shading surface groups associated with this sub surface. */
  std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

  /** Returns true if this sub surface allows the addition of a daylighting light shelf. */
  bool allowDaylightingDeviceShelf() const;

  /** Get the daylighting light shelf associated with this sub surface if there is one. */
  boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

  /** Add a daylighting light shelf associated with this sub surface.  Only succeeds if this is a fixed window, 
   * operable window, or glass door. Will return existing daylighting light shelf if there already is one. */
  boost::optional<DaylightingDeviceShelf> addDaylightingDeviceShelf() const;

  // DLM: todo add methods to create light shelves by projection factor

 protected:
  /// @cond
  typedef detail::SubSurface_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit SubSurface(std::shared_ptr<detail::SubSurface_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SubSurface");
};

/// Applys a skylight pattern to exterior roofs in selected spaces. 
/// Pattern should be in Building coordinates, on the z = 0 plane, with normal in positive z direction.
/// Returns new sub surfaces created.
MODEL_API std::vector<SubSurface> applySkylightPattern(const std::vector<std::vector<Point3d> >& pattern, const std::vector<Space>& spaces, const boost::optional<ConstructionBase>& construction);

/** \relates SubSurface*/
typedef boost::optional<SubSurface> OptionalSubSurface;

/** \relates SubSurface*/
typedef std::vector<SubSurface> SubSurfaceVector;

} // model
} // openstudio

#endif // MODEL_SUBSURFACE_HPP

