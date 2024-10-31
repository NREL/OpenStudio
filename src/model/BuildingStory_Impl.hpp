/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BUILDINGSTORY_IMPL_HPP
#define MODEL_BUILDINGSTORY_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {

class BoundingBox;

namespace model {

  class SpaceType;
  class DefaultConstructionSet;
  class DefaultScheduleSet;
  class DefaultScheduleType;
  class Space;
  class RenderingColor;
  class BuildingStory;

  namespace detail {

    /** BuildingStory_Impl is a ModelObject_Impl that is the implementation class for BuildingStory.*/
    class MODEL_API BuildingStory_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      BuildingStory_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      BuildingStory_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      BuildingStory_Impl(const BuildingStory_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~BuildingStory_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<double> nominalZCoordinate() const;

      boost::optional<double> nominalFloortoFloorHeight() const;

      boost::optional<double> nominalFloortoCeilingHeight() const;

      openstudio::BoundingBox boundingBoxBuildingCoordinates() const;

      openstudio::BoundingBox boundingBoxSiteCoordinates() const;

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

      boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BUILDINGSTORY_IMPL_HPP
