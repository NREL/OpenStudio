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

#ifndef MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP
#define MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;

namespace detail {

  class DefaultSurfaceConstructions_Impl;

} // detail

/** DefaultSurfaceConstructions is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultSurfaceConstructions'. */
class MODEL_API DefaultSurfaceConstructions : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DefaultSurfaceConstructions(const Model& model);

  virtual ~DefaultSurfaceConstructions() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<ConstructionBase> floorConstruction() const;

  boost::optional<ConstructionBase> wallConstruction() const;

  boost::optional<ConstructionBase> roofCeilingConstruction() const;

  //@}
  /** @name Setters */
  //@{

  bool setFloorConstruction(const ConstructionBase& construction);

  void resetFloorConstruction();

  bool setWallConstruction(const ConstructionBase& construction);

  void resetWallConstruction();

  bool setRoofCeilingConstruction(const ConstructionBase& construction);

  void resetRoofCeilingConstruction();

  //@}

  /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
  void merge(const DefaultSurfaceConstructions& other);

 protected:
  /// @cond
  typedef detail::DefaultSurfaceConstructions_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit DefaultSurfaceConstructions(std::shared_ptr<detail::DefaultSurfaceConstructions_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DefaultSurfaceConstructions");
};

/** \relates DefaultSurfaceConstructions*/
typedef boost::optional<DefaultSurfaceConstructions> OptionalDefaultSurfaceConstructions;

/** \relates DefaultSurfaceConstructions*/
typedef std::vector<DefaultSurfaceConstructions> DefaultSurfaceConstructionsVector;

} // model
} // openstudio

#endif // MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP

