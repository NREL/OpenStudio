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

#ifndef MODEL_SHADINGSURFACEGROUP_HPP
#define MODEL_SHADINGSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup.hpp"

namespace openstudio {

class Transformation;

namespace model {
  
class Space;
class ShadingSurface;
class Surface;
class SubSurface;

namespace detail {

  class ShadingSurfaceGroup_Impl;

} // detail

/** ShadingSurfaceGroup is a PlanarSurfaceGroup that wraps the OpenStudio IDD object 'OS_ShadingSurfaceGroup'. */
class MODEL_API ShadingSurfaceGroup : public PlanarSurfaceGroup {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ShadingSurfaceGroup(const Model& model);

  virtual ~ShadingSurfaceGroup() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validShadingSurfaceTypeValues();

  //@}
  /** @name Getters */
  //@{

  std::string shadingSurfaceType() const;

  //@}
  /** @name Setters */
  //@{

  /// this will fail if "Space" and space is not set, use setSpace instead
  bool setShadingSurfaceType(std::string shadingSurfaceType);

  //@}

  /// Returns the parent Space.
  boost::optional<Space> space() const;

  /// Sets the parent Space.
  bool setSpace(const Space& space);

  /** If (space()), calls setShadingSurfaceType("Building") to remove the relationship. */
  void resetSpace();

  /// Returns child shading surfaces.
  std::vector<ShadingSurface> shadingSurfaces() const;

  /** Returns the specific sub surface shaded by this group if specified. */
  boost::optional<SubSurface> shadedSubSurface() const;

  /** Returns the surface shaded by this group if specified. */
  boost::optional<Surface> shadedSurface() const;

  /** Sets the specifc sub surface shaded by this group, sub surface must be in the same parent space. */
  bool setShadedSubSurface(const SubSurface& subSurface);

  /** Sets the surface shaded by this group, surface must be in the same parent space. */
  bool setShadedSurface(const Surface& surface);

  /** Resets the sub surface shaded by this group. */
  void resetShadedObject();

 protected:
  /// @cond
  typedef detail::ShadingSurfaceGroup_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit ShadingSurfaceGroup(std::shared_ptr<detail::ShadingSurfaceGroup_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ShadingSurfaceGroup");
};

/** \relates ShadingSurfaceGroup*/
typedef boost::optional<ShadingSurfaceGroup> OptionalShadingSurfaceGroup;

/** \relates ShadingSurfaceGroup*/
typedef std::vector<ShadingSurfaceGroup> ShadingSurfaceGroupVector;

} // model
} // openstudio

#endif // MODEL_SHADINGSURFACEGROUP_HPP

