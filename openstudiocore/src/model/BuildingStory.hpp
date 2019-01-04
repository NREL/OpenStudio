/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_BUILDINGSTORY_HPP
#define MODEL_BUILDINGSTORY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

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

  boost::optional<double> nominalFloortoFloorHeight() const;

  boost::optional<double> nominalFloortoCeilingHeight() const;

  //@}
  /** @name Setters */
  //@{

  bool setNominalZCoordinate(double nominalZCoordinate);
  void resetNominalZCoordinate();

  bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);
  void resetNominalFloortoFloorHeight();

  bool setNominalFloortoCeilingHeight(double nominalFloortoCeilingHeight);
  void resetNominalFloortoCeilingHeight();

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
