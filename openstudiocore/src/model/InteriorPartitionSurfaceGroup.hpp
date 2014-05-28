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

#ifndef MODEL_INTERIORPARTITIONSURFACEGROUP_HPP
#define MODEL_INTERIORPARTITIONSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup.hpp"

namespace openstudio {

class Transformation;

namespace model {

class Space;
class InteriorPartitionSurface;

namespace detail {

  class InteriorPartitionSurfaceGroup_Impl;

} // detail

/** InteriorPartitionSurfaceGroup is a PlanarSurfaceGroup that wraps the OpenStudio IDD object 'OS_InteriorPartitionSurfaceGroup'. */
class MODEL_API InteriorPartitionSurfaceGroup : public PlanarSurfaceGroup {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit InteriorPartitionSurfaceGroup(const Model& model);

  virtual ~InteriorPartitionSurfaceGroup() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  int multiplier() const;

  bool isMultiplierDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setMultiplier(int multiplier);

  void resetMultiplier();

  //@}

  /// Returns the parent Space.
  boost::optional<Space> space() const;

  /// Sets the parent Space.
  bool setSpace(const Space& space);

  void resetSpace();

  /// Returns all \link InteriorPartitionSurface InteriorPartitionSurfaces \endlink in this group.
  std::vector<InteriorPartitionSurface> interiorPartitionSurfaces() const;

 protected:
  /// @cond
  typedef detail::InteriorPartitionSurfaceGroup_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit InteriorPartitionSurfaceGroup(std::shared_ptr<detail::InteriorPartitionSurfaceGroup_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.InteriorPartitionSurfaceGroup");
};

/** \relates InteriorPartitionSurfaceGroup*/
typedef boost::optional<InteriorPartitionSurfaceGroup> OptionalInteriorPartitionSurfaceGroup;

/** \relates InteriorPartitionSurfaceGroup*/
typedef std::vector<InteriorPartitionSurfaceGroup> InteriorPartitionSurfaceGroupVector;

} // model
} // openstudio

#endif // MODEL_INTERIORPARTITIONSURFACEGROUP_HPP

