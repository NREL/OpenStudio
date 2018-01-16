/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_BUILDINGSTORY_IMPL_HPP
#define MODEL_BUILDINGSTORY_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class SpaceType;
class DefaultConstructionSet;
class DefaultScheduleSet;
class Space;
class RenderingColor;
class BuildingStory;

namespace detail {

  /** BuildingStory_Impl is a ModelObject_Impl that is the implementation class for BuildingStory.*/
  class MODEL_API BuildingStory_Impl : public ModelObject_Impl {





   public:
    /** @name Constructors and Destructors */
    //@{

    BuildingStory_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    BuildingStory_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    BuildingStory_Impl(const BuildingStory_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    virtual ~BuildingStory_Impl() {}

    //@}

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

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
   private:
    REGISTER_LOGGER("openstudio.model.BuildingStory");

    boost::optional<ModelObject> defaultConstructionSetAsModelObject() const;
    boost::optional<ModelObject> defaultScheduleSetAsModelObject() const;
    boost::optional<ModelObject> renderingColorAsModelObject() const;

    bool setDefaultConstructionSetAsModelObject(boost::optional<ModelObject> modelObject);
    bool setDefaultScheduleSetAsModelObject(boost::optional<ModelObject> modelObject);
    bool setRenderingColorAsModelObject(boost::optional<ModelObject> modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_BUILDINGSTORY_IMPL_HPP
