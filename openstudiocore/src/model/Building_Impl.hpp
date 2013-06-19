/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ParentObject_Impl.hpp>

#include <utilities/units/Quantity.hpp>

namespace openstudio {

class Transformation;

namespace model {

class BuildingStandardsInformation;
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
    Q_PROPERTY(std::string buildingType READ buildingType WRITE setBuildingType RESET resetBuildingType);
    Q_PROPERTY(bool isBuildingTypeDefaulted READ isBuildingTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> buildingTypeValues READ buildingTypeValues);

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
    Q_PROPERTY(boost::optional<int> numberOfStories READ numberOfStories WRITE setNumberOfStories);
    Q_PROPERTY(boost::optional<int> numberOfAboveGroundStories READ numberOfAboveGroundStories WRITE setNumberOfAboveGroundStories);

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
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> standardsInformation READ standardsInformationAsModelObject);
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

    
    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    std::string buildingType() const;

    bool isBuildingTypeDefaulted() const;

    std::vector<std::string> buildingTypeValues() const;

    double northAxis() const;

    bool isNorthAxisDefaulted() const;

    double nominalFloortoFloorHeight() const;

    bool isNominalFloortoFloorHeightDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setBuildingType(const std::string& buildingType);

    void resetBuildingType();

    void setNorthAxis(double northAxis);

    void resetNorthAxis();

    bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);

    void resetNominalFloortoFloorHeight();

    //@}

    /// Returns the building's space type.
    boost::optional<SpaceType> spaceType() const;

    /// Sets the building's space type.
    bool setSpaceType(const SpaceType& spaceType);

    /// Resets the building's space type.
    void resetSpaceType();
  
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

    /// get meter requests for the building
    std::vector<Meter> meters() const;

    // get facility
    boost::optional<Facility> facility() const;

    // get all spaces
    std::vector<Space> spaces() const;

    // get all building level exterior shading groups
    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

    // get all thermal zones in the building
    std::vector<ThermalZone> thermalZones() const;

    /** Returns all exterior wall surfaces. */
    std::vector<Surface> exteriorWalls() const;

    /** Returns all roof surfaces. */
    std::vector<Surface> roofs() const;

    /** Get the standards information associated with the overall building. */
    BuildingStandardsInformation standardsInformation() const;

    // get total floor area
    /// Includes only spaces marked as included in floor area.
    /// Includes space multipliers in calculation.
    double floorArea() const;

    // get total conditioned floor area, requires sql file for now
    boost::optional<double> conditionedFloorArea() const;

    /** Returns the number of people in the building. */
    double numberOfPeople() const;

    /** Returns the number of people per floor area (people/m^2). */
    double peoplePerFloorArea() const;

    /** Returns the total floor area per person in this building (m^2/person). */
    double floorAreaPerPerson() const;

    /** Returns the lighting power (W) in this building. */
    double lightingPower() const;
    
    /** Returns the lighting power density (W/m^2) of this building. */
    double lightingPowerPerFloorArea() const;

    /** Returns the lighting power per person (W/person) of this building. */
    double lightingPowerPerPerson() const;

    /** Returns the electric equipment power (W) of this building. */
    double electricEquipmentPower() const;

    /** Returns the electric equipment power density (W/m^2) of this building. */
    double electricEquipmentPowerPerFloorArea() const;

    /** Returns the electric equipment power per person (W/person) of this building. */
    double electricEquipmentPowerPerPerson() const;

    /** Returns the gas equipment power (W) of this building. */
    double gasEquipmentPower() const;

    /** Returns the gas equipment power density (W/m^2) of this building. */
    double gasEquipmentPowerPerFloorArea() const;

    /** Returns the gas equipment power per person (W/person) of this building. */
    double gasEquipmentPowerPerPerson() const;

    /** Returns the number of stories in this building. This is a user-set standards attribute. 
     *  It is not inferred from underlying data. */
    boost::optional<int> numberOfStories() const;

    /** Returns the number of above ground stories in this building. This is a user-set standards
     *  attribute. It is not inferred from underlying data. */
    boost::optional<int> numberOfAboveGroundStories() const;

    /** Sets the number of stories in this building. This is a user-set standards attribute. It 
     *  is not inferred from underlying data. */
    bool setNumberOfStories(boost::optional<int> value);

    /** Sets the number of above ground stories in this building. This is a user-set standards
     *  attribute. It is not inferred from underlying data. */
    bool setNumberOfAboveGroundStories(boost::optional<int> value);

    // get the transformation from building coordinates to world coordinates
    Transformation transformation() const;
  
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
    boost::optional<ModelObject> standardsInformationAsModelObject() const;

    bool setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_BUILDING_IMPL_HPP

