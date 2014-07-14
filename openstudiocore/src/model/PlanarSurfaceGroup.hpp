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

#ifndef MODEL_PLANARSURFACEGROUP_HPP
#define MODEL_PLANARSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Transformation;
class Point3d;
class BoundingBox;

namespace model {

namespace detail{
  class PlanarSurfaceGroup_Impl;
}

/** PlanarSurfaceGroup is an abstract class representing a group that holds many PlanarSurfaces.
 */
class MODEL_API PlanarSurfaceGroup : public ParentObject {
 public:
  virtual ~PlanarSurfaceGroup() {}

  /** @name Getters */
  //@{

  double directionofRelativeNorth() const;

  bool isDirectionofRelativeNorthDefaulted() const;

  double xOrigin() const;

  bool isXOriginDefaulted() const;

  double yOrigin() const;

  bool isYOriginDefaulted() const;

  double zOrigin() const;

  bool isZOriginDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setDirectionofRelativeNorth(double directionofRelativeNorth);

  void resetDirectionofRelativeNorth();

  void setXOrigin(double xOrigin);

  void resetXOrigin();

  void setYOrigin(double yOrigin);

  void resetYOrigin();

  void setZOrigin(double zOrigin);

  void resetZOrigin();

  //@}

  /** Returns the transformation from local coordinates to parent coordinates. */
  openstudio::Transformation transformation() const;

  /** Returns the transformation from local coordinates to building coordinates. */
  openstudio::Transformation buildingTransformation() const;

  /** Returns the transformation from local coordinates to site coordinates. */
  openstudio::Transformation siteTransformation() const;

  /** Sets the transformation from local coordinates to parent coordinates,
   *  this method can be used to move the group. */
  bool setTransformation(const openstudio::Transformation& transformation);

  /** Changes the transformation from local coordinates to parent coordinates,
   *  this method alter geometry of children relative to the
   *  group so that it stays in the same place with the new transformation. */
  bool changeTransformation(const openstudio::Transformation& transformation);

  /** Get the BoundingBox in local coordinates. */
  openstudio::BoundingBox boundingBox() const;

 protected:
  /** @name Constructors and Destructors */
  //@{

  explicit PlanarSurfaceGroup(IddObjectType iddObjectType, const Model& model);

  //@}

  /// @cond
  typedef detail::PlanarSurfaceGroup_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::PlanarSurfaceGroup_Impl;

  explicit PlanarSurfaceGroup(std::shared_ptr<detail::PlanarSurfaceGroup_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.PlanarSurfaceGroup");

};

/** \relates PlanarSurfaceGroup */
typedef boost::optional<PlanarSurfaceGroup> OptionalPlanarSurfaceGroup;

/** \relates PlanarSurfaceGroup */
typedef std::vector<PlanarSurfaceGroup> PlanarSurfaceGroupVector;

} // model
} // openstudio

#endif // MODEL_PLANARSURFACEGROUP_HPP
