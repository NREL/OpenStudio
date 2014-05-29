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

#ifndef MODEL_BUILDING_IMPL_HPP
#define MODEL_BUILDING_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

class Facility;
class Meter;
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
    Q_OBJECT;

    Q_PROPERTY(double northAxis READ northAxis WRITE setNorthAxis RESET resetNorthAxis);
    Q_PROPERTY(openstudio::Quantity northAxis_SI READ northAxis_SI WRITE setNorthAxis RESET resetNorthAxis);
    Q_PROPERTY(openstudio::Quantity northAxis_IP READ northAxis_IP WRITE setNorthAxis RESET resetNorthAxis);
    Q_PROPERTY(bool isNorthAxisDefaulted READ isNorthAxisDefaulted);

    Q_PROPERTY(double nominalFloortoFloorHeight READ nominalFloortoFloorHeight WRITE setNominalFloortoFloorHeight RESET resetNominalFloortoFloorHeight);
    Q_PROPERTY(openstudio::Quantity nominalFloortoFloorHeight_SI READ nominalFloortoFloorHeight_SI WRITE setNominalFloortoFloorHeight RESET resetNominalFloortoFloorHeight);
    Q_PROPERTY(openstudio::Quantity nominalFloortoFloorHeight_IP READ nominalFloortoFloorHeight_IP WRITE setNominalFloortoFloorHeight RESET resetNominalFloortoFloorHeight);
    Q_PROPERTY(bool isNominalFloortoFloorHeightDefaulted READ isNominalFloortoFloorHeightDefaulted);

    Q_PROPERTY(double floorArea READ floorArea);
    Q_PROPERTY(boost::optional<double> conditionedFloorArea READ conditionedFloorArea);
    Q_PROPERTY(double numberOfPeople READ numberOfPeople);
    Q_PROPERTY(double peoplePerFloorArea READ peoplePerFloorArea);
    Q_PROPERTY(double floorAreaPerPerson READ floorAreaPerPerson);
    Q_PROPERTY(double lightingPower READ lightingPower);
    Q_PROPERTY(double lightingPowerPerFloorArea READ lightingPowerPerFloorArea);
    Q_PROPERTY(double lightingPowerPerPerson READ lightingPowerPerPerson);
    Q_PROPERTY(double electricEquipmentPower READ electricEquipmentPower);
    Q_PROPERTY(double electricEquipmentPowerPerFloorArea READ electricEquipmentPowerPerFloorArea);
    Q_PROPERTY(double electricEquipmentPowerPerPerson READ electricEquipmentPowerPerPerson);
    Q_PROPERTY(double gasEquipmentPower READ gasEquipmentPower);
    Q_PROPERTY(double gasEquipmentPowerPerFloorArea READ gasEquipmentPowerPerFloorArea);
    Q_PROPERTY(double gasEquipmentPowerPerPerson READ gasEquipmentPowerPerPerson);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> spaceType READ spaceTypeAsModelObject WRITE setSpaceTypeAsModelObject RESET resetSpaceType);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultConstructionSet READ defaultConstructionSetAsModelObject WRITE setDefaultConstructionSetAsModelObject RESET resetDefaultConstructionSet);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultScheduleSet READ defaultScheduleSetAsModelObject WRITE setDefaultScheduleSetAsModelObject RESET resetDefaultScheduleSet);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> meters READ metersAsModelObjects);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> facility READ facilityAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> spaces READ spacesAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> shadingSurfaceGroups READ shadingSurfaceGroupsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> thermalZones READ thermalZonesAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> exteriorWalls READ exteriorWallsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> roofs READ roofsAsModelObjects);
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

    virtual boost::optional<ParentObject> parent() const;

    virtual std::vector<ModelObject> children() const;

    virtual bool setParent(ParentObject& newParent);

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    double northAxis() const;

    bool isNorthAxisDefaulted() const;

    double nominalFloortoFloorHeight() const;

    bool isNominalFloortoFloorHeightDefaulted() const;

    boost::optional<int> standardsNumberOfStories() const;

    boost::optional<int> standardsNumberOfAboveGroundStories() const;

    boost::optional<std::string> standardsBuildingType() const;

    std::vector<std::string> suggestedStandardsBuildingTypes() const;

    //@}
    /** @name Setters */
    //@{

    void setNorthAxis(double northAxis);

    void resetNorthAxis();

    bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);

    void resetNominalFloortoFloorHeight();

    bool setStandardsNumberOfStories(int value);
    void resetStandardsNumberOfStories();

    bool setStandardsNumberOfAboveGroundStories(int value);
    void resetStandardsNumberOfAboveGroundStories();

    bool setStandardsBuildingType(const std::string& standardsBuildingType);
    void resetStandardsBuildingType();

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

    std::vector<Meter> meters() const;

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

