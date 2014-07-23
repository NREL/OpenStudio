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

#ifndef MODEL_INTERIORPARTITIONSURFACE_HPP
#define MODEL_INTERIORPARTITIONSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

class InteriorPartitionSurfaceGroup;
class DaylightingDeviceShelf;

namespace detail {

  class InteriorPartitionSurface_Impl;

} // detail

/** InteriorPartitionSurface is a PlanarSurface that wraps the OpenStudio IDD object 'OS_InteriorPartitionSurface'. */
class MODEL_API InteriorPartitionSurface : public PlanarSurface {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit InteriorPartitionSurface(const std::vector<Point3d>& vertices, const Model& model);

  virtual ~InteriorPartitionSurface() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  bool converttoInternalMass() const;

  bool isConverttoInternalMassDefaulted() const;

  boost::optional<double> surfaceArea() const;

  boost::optional<double> numberofVertices() const;

  bool isNumberofVerticesDefaulted() const;

  bool isNumberofVerticesAutocalculated() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  void setConverttoInternalMass(bool converttoInternalMass);

  void resetConverttoInternalMass();

  bool setSurfaceArea(boost::optional<double> surfaceArea);

  bool setSurfaceArea(double surfaceArea);

  void resetSurfaceArea();

  bool setNumberofVertices(boost::optional<double> numberofVertices);

  bool setNumberofVertices(double numberofVertices);

  void resetNumberofVertices();

  void autocalculateNumberofVertices();

  // TODO: Handle this object's extensible fields.

  //@}

  /// get the interior partition surface group
  boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup() const;
  
  /// set the interior partition surface group
  bool setInteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup);

  void resetInteriorPartitionSurfaceGroup();

  boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

 protected:
  /// @cond
  typedef detail::InteriorPartitionSurface_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit InteriorPartitionSurface(std::shared_ptr<detail::InteriorPartitionSurface_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.InteriorPartitionSurface");
};

/** \relates InteriorPartitionSurface*/
typedef boost::optional<InteriorPartitionSurface> OptionalInteriorPartitionSurface;

/** \relates InteriorPartitionSurface*/
typedef std::vector<InteriorPartitionSurface> InteriorPartitionSurfaceVector;

} // model
} // openstudio

#endif // MODEL_INTERIORPARTITIONSURFACE_HPP

