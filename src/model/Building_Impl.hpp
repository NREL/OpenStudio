/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BUILDING_IMPL_HPP
#define MODEL_BUILDING_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  class Facility;
  class OutputMeter;
  class BuildingStory;
  class ShadingSurfaceGroup;
  class Surface;
  class Space;
  class SpaceType;
  class DefaultConstructionSet;
  class DefaultScheduleSet;
  class DefaultScheduleType;
  class ThermalZone;
  class Building;

  namespace detail {

    /** Building_Impl is a ParentObject_Impl that is the implementation class for Building.*/
    class MODEL_API Building_Impl : public ParentObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Building_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Building_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Building_Impl(const Building_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Building_Impl() override = default;

      //@}

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<ModelObject> children() const override;

      virtual ModelObject clone(Model model) const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      double northAxis() const;
      bool isNorthAxisDefaulted() const;

      boost::optional<double> nominalFloortoFloorHeight() const;

      boost::optional<int> standardsNumberOfStories() const;

      boost::optional<int> standardsNumberOfAboveGroundStories() const;

      boost::optional<int> standardsNumberOfLivingUnits() const;

      boost::optional<double> nominalFloortoCeilingHeight() const;

      boost::optional<std::string> standardsTemplate() const;
      std::vector<std::string> suggestedStandardsTemplates() const;

      boost::optional<std::string> standardsBuildingType() const;
      std::vector<std::string> suggestedStandardsBuildingTypes() const;

      bool relocatable() const;
      bool isRelocatableDefaulted() const;

      boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

      //@}
      /** @name Setters */
      //@{

      bool setNorthAxis(double northAxis);

      void resetNorthAxis();

      bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);

      void resetNominalFloortoFloorHeight();

      bool setStandardsNumberOfStories(int value);
      void resetStandardsNumberOfStories();

      bool setStandardsNumberOfAboveGroundStories(int value);
      void resetStandardsNumberOfAboveGroundStories();

      bool setStandardsNumberOfLivingUnits(int value);
      void resetStandardsNumberOfLivingUnits();

      bool setNominalFloortoCeilingHeight(double nominalFloortoCeilingHeight);
      void resetNominalFloortoCeilingHeight();

      bool setStandardsTemplate(const std::string& standardsTemplate);
      void resetStandardsTemplate();

      bool setStandardsBuildingType(const std::string& standardsBuildingType);
      void resetStandardsBuildingType();

      bool setRelocatable(bool isRelocatable);
      void resetRelocatable();

      //@}

      boost::optional<SpaceType> spaceType() const;

      bool setSpaceType(const SpaceType& spaceType);

      void resetSpaceType();

      boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

      bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

      void resetDefaultConstructionSet();

      boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

      bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

      void resetDefaultScheduleSet();

      std::vector<OutputMeter> meters() const;

      std::vector<BuildingStory> buildingStories() const;

      boost::optional<Facility> facility() const;

      std::vector<Space> spaces() const;

      std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

      std::vector<ThermalZone> thermalZones() const;

      std::vector<Surface> exteriorWalls() const;

      std::vector<Surface> roofs() const;

      double floorArea() const;

      boost::optional<double> conditionedFloorArea() const;

      double exteriorSurfaceArea() const;

      double exteriorWallArea() const;

      double airVolume() const;

      double numberOfPeople() const;

      double peoplePerFloorArea() const;

      double floorAreaPerPerson() const;

      double lightingPower() const;

      double lightingPowerPerFloorArea() const;

      double lightingPowerPerPerson() const;

      double electricEquipmentPower() const;

      double electricEquipmentPowerPerFloorArea() const;

      double electricEquipmentPowerPerPerson() const;

      double gasEquipmentPower() const;

      double gasEquipmentPowerPerFloorArea() const;

      double gasEquipmentPowerPerPerson() const;

      double infiltrationDesignFlowRate() const;

      double infiltrationDesignFlowPerSpaceFloorArea() const;

      double infiltrationDesignFlowPerExteriorSurfaceArea() const;

      double infiltrationDesignFlowPerExteriorWallArea() const;

      double infiltrationDesignAirChangesPerHour() const;

      Transformation transformation() const;

      double exteriorPerimeter() const;

      std::vector<std::vector<Point3d>> generateSkylightPattern(double skylightToProjectedFloorRatio, double desiredWidth,
                                                                double desiredHeight) const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.Building");

      boost::optional<ModelObject> spaceTypeAsModelObject() const;
      boost::optional<ModelObject> defaultConstructionSetAsModelObject() const;
      boost::optional<ModelObject> defaultScheduleSetAsModelObject() const;
      std::vector<ModelObject> metersAsModelObjects() const;
      std::vector<ModelObject> buildingStoriesAsModelObjects() const;
      boost::optional<ModelObject> facilityAsModelObject() const;
      std::vector<ModelObject> spacesAsModelObjects() const;
      std::vector<ModelObject> shadingSurfaceGroupsAsModelObjects() const;
      std::vector<ModelObject> thermalZonesAsModelObjects() const;
      std::vector<ModelObject> exteriorWallsAsModelObjects() const;
      std::vector<ModelObject> roofsAsModelObjects() const;

      bool setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BUILDING_IMPL_HPP
