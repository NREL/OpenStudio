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

#ifndef MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP
#define MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;

namespace detail {

  class DefaultSubSurfaceConstructions_Impl;

} // detail

/** DefaultSubSurfaceConstructions is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultSubSurfaceConstructions'. */
class MODEL_API DefaultSubSurfaceConstructions : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DefaultSubSurfaceConstructions(const Model& model);

  virtual ~DefaultSubSurfaceConstructions() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<ConstructionBase> fixedWindowConstruction() const;

  boost::optional<ConstructionBase> operableWindowConstruction() const;
  
  boost::optional<ConstructionBase> doorConstruction() const;

  boost::optional<ConstructionBase> glassDoorConstruction() const;

  boost::optional<ConstructionBase> overheadDoorConstruction() const;

  boost::optional<ConstructionBase> skylightConstruction() const;

  boost::optional<ConstructionBase> tubularDaylightDomeConstruction() const;

  boost::optional<ConstructionBase> tubularDaylightDiffuserConstruction() const;

  //@}
  /** @name Setters */
  //@{

  bool setFixedWindowConstruction(const ConstructionBase& construction);

  void resetFixedWindowConstruction();

  bool setOperableWindowConstruction(const ConstructionBase& construction);

  void resetOperableWindowConstruction();

  bool setDoorConstruction(const ConstructionBase& construction);

  void resetDoorConstruction();

  bool setGlassDoorConstruction(const ConstructionBase& construction);

  void resetGlassDoorConstruction();

  bool setOverheadDoorConstruction(const ConstructionBase& construction);

  void resetOverheadDoorConstruction();

  bool setSkylightConstruction(const ConstructionBase& construction);

  void resetSkylightConstruction();

  bool setTubularDaylightDomeConstruction(const ConstructionBase& construction);

  void resetTubularDaylightDomeConstruction();

  bool setTubularDaylightDiffuserConstruction(const ConstructionBase& construction);

  void resetTubularDaylightDiffuserConstruction();

  //@}

  /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
  void merge(const DefaultSubSurfaceConstructions& other);

 protected:
  /// @cond
  typedef detail::DefaultSubSurfaceConstructions_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit DefaultSubSurfaceConstructions(std::shared_ptr<detail::DefaultSubSurfaceConstructions_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DefaultSubSurfaceConstructions");
};

/** \relates DefaultSubSurfaceConstructions*/
typedef boost::optional<DefaultSubSurfaceConstructions> OptionalDefaultSubSurfaceConstructions;

/** \relates DefaultSubSurfaceConstructions*/
typedef std::vector<DefaultSubSurfaceConstructions> DefaultSubSurfaceConstructionsVector;

} // model
} // openstudio

#endif // MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP

