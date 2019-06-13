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

#ifndef MODEL_BUILDING_IMPL_HPP
#define MODEL_BUILDING_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"

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
class ThermalZone;
class Building;

namespace detail {

  /** Building_Impl is a ParentObject_Impl that is the implementation class for Building.*/
  class MODEL_API Building_Impl : public ParentObject_Impl {

  public:
    /** @name Constructors and Destructors */
    //@{

    Building_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Building_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    Building_Impl(const Building_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    virtual ~Building_Impl() {}

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

    std::vector<std::vector<Point3d> > generateSkylightPattern(double skylightToProjectedFloorRatio, double desiredWidth, double desiredHeight) const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.Building");

    openstudio::Quantity northAxis_SI() const;
    openstudio::Quantity northAxis_IP() const;
    bool setNorthAxis(const Quantity& northAxis);

    openstudio::Quantity nominalFloortoFloorHeight_SI() const;
    openstudio::Quantity nominalFloortoFloorHeight_IP() const;
    bool setNominalFloortoFloorHeight(const Quantity& nominalFloortoFloorHeight);

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

} // detail

} // model
} // openstudio

#endif // MODEL_BUILDING_IMPL_HPP
