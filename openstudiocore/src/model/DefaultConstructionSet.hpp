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

#ifndef MODEL_DEFAULTCONSTRUCTIONSET_HPP
#define MODEL_DEFAULTCONSTRUCTIONSET_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class PlanarSurface;
class ConstructionBase;
class DefaultSubSurfaceConstructions;
class DefaultSurfaceConstructions;

namespace detail {

  class DefaultConstructionSet_Impl;

} // detail

/** DefaultConstructionSet is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultConstructionSet'. */
class MODEL_API DefaultConstructionSet : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DefaultConstructionSet(const Model& model);

  virtual ~DefaultConstructionSet() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<DefaultSurfaceConstructions> defaultExteriorSurfaceConstructions() const;

  boost::optional<DefaultSurfaceConstructions> defaultInteriorSurfaceConstructions() const;

  boost::optional<DefaultSurfaceConstructions> defaultGroundContactSurfaceConstructions() const;

  boost::optional<DefaultSubSurfaceConstructions> defaultExteriorSubSurfaceConstructions() const;

  boost::optional<DefaultSubSurfaceConstructions> defaultInteriorSubSurfaceConstructions() const;

  boost::optional<ConstructionBase> interiorPartitionConstruction() const;

  boost::optional<ConstructionBase> spaceShadingConstruction() const;

  boost::optional<ConstructionBase> buildingShadingConstruction() const;

  boost::optional<ConstructionBase> siteShadingConstruction() const;

  //@}
  /** @name Setters */
  //@{

  bool setDefaultExteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

  void resetDefaultExteriorSurfaceConstructions();

  bool setDefaultInteriorSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

  void resetDefaultInteriorSurfaceConstructions();

  bool setDefaultGroundContactSurfaceConstructions(const DefaultSurfaceConstructions& defaultSurfaceConstructions);

  void resetDefaultGroundContactSurfaceConstructions();

  bool setDefaultExteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

  void resetDefaultExteriorSubSurfaceConstructions();

  bool setDefaultInteriorSubSurfaceConstructions(const DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);

  void resetDefaultInteriorSubSurfaceConstructions();

  bool setInteriorPartitionConstruction(const ConstructionBase& construction);

  void resetInteriorPartitionConstruction();

  bool setSpaceShadingConstruction(const ConstructionBase& construction);

  void resetSpaceShadingConstruction();

  bool setBuildingShadingConstruction(const ConstructionBase& construction);

  void resetBuildingShadingConstruction();

  bool setSiteShadingConstruction(const ConstructionBase& construction);

  void resetSiteShadingConstruction();

  //@}

  /// Returns the default construction for this planar surface if available.
  boost::optional<ConstructionBase> getDefaultConstruction(const PlanarSurface& planarSurface) const;

  /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
  void merge(const DefaultConstructionSet& other);

 protected:

  /// @cond
  typedef detail::DefaultConstructionSet_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit DefaultConstructionSet(std::shared_ptr<detail::DefaultConstructionSet_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DefaultConstructionSet");
};

/** \relates DefaultConstructionSet*/
typedef boost::optional<DefaultConstructionSet> OptionalDefaultConstructionSet;

/** \relates DefaultConstructionSet*/
typedef std::vector<DefaultConstructionSet> DefaultConstructionSetVector;

} // model
} // openstudio

#endif // MODEL_DEFAULTCONSTRUCTIONSET_HPP

