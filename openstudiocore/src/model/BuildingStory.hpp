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

#ifndef MODEL_BUILDINGSTORY_HPP
#define MODEL_BUILDINGSTORY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {  

class OSOptionalQuantity;

namespace model {

class SpaceType;
class DefaultConstructionSet;
class DefaultScheduleSet;
class Space;
class RenderingColor;

namespace detail {

  class BuildingStory_Impl;

} // detail

/** BuildingStory is a ModelObject that wraps the OpenStudio IDD object 'OS_BuildingStory'. */
class MODEL_API BuildingStory : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit BuildingStory(const Model& model);

  virtual ~BuildingStory() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> nominalZCoordinate() const;
  
  OSOptionalQuantity getNominalZCoordinate(bool returnIP=false) const;

  double nominalFloortoFloorHeight() const;
  
  Quantity getNominalFloortoFloorHeight(bool returnIP=false) const;

  bool isNominalFloortoFloorHeightDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setNominalZCoordinate(boost::optional<double> nominalZCoordinate);

  void setNominalZCoordinate(double nominalZCoordinate);
  
  bool setNominalZCoordinate(const Quantity& nominalZCoordinate);

  void resetNominalZCoordinate();

  bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);
  
  bool setNominalFloortoFloorHeight(const Quantity& nominalFloortoFloorHeight);

  void resetNominalFloortoFloorHeight();

  //@}

  /// Gets all spaces on this building story.
  std::vector<Space> spaces() const;

  /// Returns the default construction set that this space references directly.
  boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

  /// Sets the default construction set for this space directly.
  bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

  /// Resets the default construction set for this space.
  void resetDefaultConstructionSet();

  /// Returns the default schedule set that this space references directly.
  boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

  /// Sets the default schedule set for this space directly.
  bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

  /// Resets the default schedule set for this space.
  void resetDefaultScheduleSet();

  /// Returns the rendering color.
  boost::optional<RenderingColor> renderingColor() const;

  /// Sets the rendering color.
  bool setRenderingColor(const RenderingColor& renderingColor);

  /// Resets the rendering color.
  void resetRenderingColor();

 protected:
  /// @cond
  typedef detail::BuildingStory_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit BuildingStory(std::shared_ptr<detail::BuildingStory_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.BuildingStory");
};

/** \relates BuildingStory*/
typedef boost::optional<BuildingStory> OptionalBuildingStory;

/** \relates BuildingStory*/
typedef std::vector<BuildingStory> BuildingStoryVector;

} // model
} // openstudio

#endif // MODEL_BUILDINGSTORY_HPP

