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

#ifndef MODEL_SPACE_IMPL_HPP
#define MODEL_SPACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"

#include "../utilities/units/Quantity.hpp"

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

namespace openstudio {
namespace model {

// forward declarations
class SpaceType;
class ConstructionBase;
class DefaultConstructionSet;
class Schedule;
class DefaultScheduleType;
class DefaultScheduleSet;
class ThermalZone;
class BuildingStory;
class ShadingSurfaceGroup;
class InteriorPartitionSurfaceGroup;
class PlanarSurface;
class Surface;
class InternalMass;
class People;
class Lights;
class Luminaire;
class ElectricEquipment;
class GasEquipment;
class HotWaterEquipment;
class SteamEquipment;
class OtherEquipment;
class WaterUseEquipment;
class DaylightingControl;
class IlluminanceMap;
class GlareSensor;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class DesignSpecificationOutdoorAir;
class Space;

namespace detail {

  /** Space_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for Space.*/
  class MODEL_API Space_Impl : public PlanarSurfaceGroup_Impl {
    Q_OBJECT;

    Q_PROPERTY(double directionofRelativeNorth READ directionofRelativeNorth WRITE setDirectionofRelativeNorth RESET resetDirectionofRelativeNorth);
    Q_PROPERTY(openstudio::Quantity directionofRelativeNorth_SI READ directionofRelativeNorth_SI WRITE setDirectionofRelativeNorth RESET resetDirectionofRelativeNorth);
    Q_PROPERTY(openstudio::Quantity directionofRelativeNorth_IP READ directionofRelativeNorth_IP WRITE setDirectionofRelativeNorth RESET resetDirectionofRelativeNorth);
    Q_PROPERTY(bool isDirectionofRelativeNorthDefaulted READ isDirectionofRelativeNorthDefaulted);

    Q_PROPERTY(double xOrigin READ xOrigin WRITE setXOrigin RESET resetXOrigin);
    Q_PROPERTY(openstudio::Quantity xOrigin_SI READ xOrigin_SI WRITE setXOrigin RESET resetXOrigin);
    Q_PROPERTY(openstudio::Quantity xOrigin_IP READ xOrigin_IP WRITE setXOrigin RESET resetXOrigin);
    Q_PROPERTY(bool isXOriginDefaulted READ isXOriginDefaulted);

    Q_PROPERTY(double yOrigin READ yOrigin WRITE setYOrigin RESET resetYOrigin);
    Q_PROPERTY(openstudio::Quantity yOrigin_SI READ yOrigin_SI WRITE setYOrigin RESET resetYOrigin);
    Q_PROPERTY(openstudio::Quantity yOrigin_IP READ yOrigin_IP WRITE setYOrigin RESET resetYOrigin);
    Q_PROPERTY(bool isYOriginDefaulted READ isYOriginDefaulted);

    Q_PROPERTY(double zOrigin READ zOrigin WRITE setZOrigin RESET resetZOrigin);
    Q_PROPERTY(openstudio::Quantity zOrigin_SI READ xOrigin_SI WRITE setZOrigin RESET resetZOrigin);
    Q_PROPERTY(openstudio::Quantity zOrigin_IP READ xOrigin_IP WRITE setZOrigin RESET resetZOrigin);
    Q_PROPERTY(bool isZOriginDefaulted READ isZOriginDefaulted);

    Q_PROPERTY(bool partofTotalFloorArea READ partofTotalFloorArea WRITE setPartofTotalFloorArea RESET resetPartofTotalFloorArea);
    Q_PROPERTY(bool isPartofTotalFloorAreaDefaulted READ isPartofTotalFloorAreaDefaulted);

    Q_PROPERTY(double floorArea READ floorArea);
    Q_PROPERTY(double exteriorArea READ exteriorArea);
    Q_PROPERTY(double exteriorWallArea READ exteriorWallArea);
    Q_PROPERTY(double volume READ volume);
    Q_PROPERTY(double numberOfPeople READ numberOfPeople WRITE setNumberOfPeople);
    Q_PROPERTY(double peoplePerFloorArea READ peoplePerFloorArea WRITE setPeoplePerFloorArea);
    Q_PROPERTY(double floorAreaPerPerson READ floorAreaPerPerson WRITE setFloorAreaPerPerson);
    Q_PROPERTY(double lightingPower READ lightingPower WRITE setLightingPower);
    Q_PROPERTY(double lightingPowerPerFloorArea READ lightingPowerPerFloorArea WRITE setLightingPowerPerFloorArea);
    Q_PROPERTY(double lightingPowerPerPerson READ lightingPowerPerPerson WRITE setLightingPowerPerPerson);
    Q_PROPERTY(double electricEquipmentPower READ electricEquipmentPower WRITE setElectricEquipmentPower);
    Q_PROPERTY(double electricEquipmentPowerPerFloorArea READ electricEquipmentPowerPerFloorArea WRITE setElectricEquipmentPowerPerFloorArea);
    Q_PROPERTY(double electricEquipmentPowerPerPerson READ electricEquipmentPowerPerPerson WRITE setElectricEquipmentPowerPerPerson);
    Q_PROPERTY(double gasEquipmentPower READ gasEquipmentPower WRITE setGasEquipmentPower);
    Q_PROPERTY(double gasEquipmentPowerPerFloorArea READ gasEquipmentPowerPerFloorArea WRITE setGasEquipmentPowerPerFloorArea);
    Q_PROPERTY(double gasEquipmentPowerPerPerson READ gasEquipmentPowerPerPerson WRITE setGasEquipmentPowerPerPerson);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> spaceType READ spaceTypeAsModelObject WRITE setSpaceTypeAsModelObject RESET resetSpaceType);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultConstructionSet READ defaultConstructionSetAsModelObject WRITE setDefaultConstructionSetAsModelObject RESET resetDefaultConstructionSet);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> defaultScheduleSet READ defaultScheduleSetAsModelObject WRITE setDefaultScheduleSetAsModelObject RESET resetDefaultScheduleSet);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> thermalZone READ thermalZoneAsModelObject WRITE setThermalZoneAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> buildingStory READ buildingStoryAsModelObject WRITE setBuildingStoryAsModelObject);

    Q_PROPERTY(std::vector<openstudio::model::ModelObject> shadingSurfaceGroups READ shadingSurfaceGroupsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> interiorPartitionSurfaceGroups READ interiorPartitionSurfaceGroupsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> surfaces READ surfacesAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> internalMass READ internalMassAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> people READ peopleAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> lights READ lightsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> luminaires READ luminairesAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> electricEquipment READ electricEquipmentAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> gasEquipment READ gasEquipmentAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> hotWaterEquipment READ hotWaterEquipmentAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> steamEquipment READ steamEquipmentAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> otherEquipment READ otherEquipmentAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> daylightingControls READ daylightingControlsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> illuminanceMaps READ illuminanceMapsAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> spaceInfiltrationDesignFlowRates READ spaceInfiltrationDesignFlowRatesAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> spaceInfiltrationEffectiveLeakageAreas READ spaceInfiltrationEffectiveLeakageAreasAsModelObjects);
  public:
    /** @name Constructors and Destructors */
    //@{

    Space_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Space_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    Space_Impl(const Space_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    virtual ~Space_Impl() {}

    //@}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual openstudio::Transformation buildingTransformation() const;

    virtual bool changeTransformation(const openstudio::Transformation& transformation);

    virtual openstudio::BoundingBox boundingBox() const;

    /** @name Getters */
    //@{

    virtual double directionofRelativeNorth() const;

    virtual bool isDirectionofRelativeNorthDefaulted() const;

    virtual double xOrigin() const;

    virtual bool isXOriginDefaulted() const;

    virtual double yOrigin() const;

    virtual bool isYOriginDefaulted() const;

    virtual double zOrigin() const;

    virtual bool isZOriginDefaulted() const;

    bool partofTotalFloorArea() const;

    bool isPartofTotalFloorAreaDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    virtual void setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true);

    virtual void resetDirectionofRelativeNorth();

    virtual void setXOrigin(double xOrigin, bool driverMethod = true);

    virtual void resetXOrigin();

    virtual void setYOrigin(double yOrigin, bool driverMethod = true);

    virtual void resetYOrigin();

    virtual void setZOrigin(double zOrigin, bool driverMethod = true);

    virtual void resetZOrigin();

    void setPartofTotalFloorArea(bool partofTotalFloorArea);

    void resetPartofTotalFloorArea();

    //@}

    /// Returns the space type for this space.
    boost::optional<SpaceType> spaceType() const;

    /// Returns true if space type for this space is inherited from the building.
    bool isSpaceTypeDefaulted() const;

    /// Sets the space type for this space.
    bool setSpaceType(const SpaceType& spaceType);

    /// Resets the space type for this space.
    void resetSpaceType();

    /// Returns the default construction set that this space references directly.
    boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

    /// Returns the default construction for this planar surface if available by searching (in order):
    /// This space's default construction set, search distance = 1 
    /// This space's space type's default construction set, search distance = 2
    /// This space's building story's default construction set, search distance = 3
    /// The building's default construction set, search distance = 4
    /// The building's space type's default construction set, search distance = 5
    boost::optional<ConstructionBase> getDefaultConstruction(const PlanarSurface& planarSurface) const;
    boost::optional<std::pair<ConstructionBase, int> > getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const;

    /// Sets the default construction set for this space directly.
    bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

    /// Resets the default construction set for this space.
    void resetDefaultConstructionSet();

    /// Returns the default schedule set.
    boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

    /// Returns the default schedule set for the specified type if available by searching (in order):
    /// This space's default schedule set
    /// This space's space type's default schedule set
    /// This space's building story's default schedule set
    /// This space's building's default schedule set
    /// This space's building's space type's default schedule set
    boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

    /// Sets the default schedule set.
    bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

    /// Resets the default schedule set.
    void resetDefaultScheduleSet();

    // returns the thermal zone if set
    boost::optional<ThermalZone> thermalZone() const;

    // sets the thermal zone
    bool setThermalZone(ThermalZone& thermalZone);

    // resets the thermal zone
    void resetThermalZone();

    // returns the building story if set
    boost::optional<BuildingStory> buildingStory() const;

    // sets the building story
    bool setBuildingStory(const BuildingStory& buildingStory);

    // resets the building story
    void resetBuildingStory();

    /// Returns all \link ShadingSurfaceGroup ShadingSurfaceGroups \endlink in this space.
    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

    /// Returns all \link InteriorPartitionSurfaceGroup InteriorPartitionSurfaceGroups \endlink in this space.
    std::vector<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups() const;

    /// Returns all \link Surface Surfaces \endlink in this space.
    std::vector<Surface> surfaces() const;

    /// Returns all InternalMass in this space.
    std::vector<InternalMass> internalMass() const;

    /// Returns all People in this space.
    std::vector<People> people() const;

    /// Returns all Lights in this space.
    std::vector<Lights> lights() const;

    /// Returns all \link Luminaire Luminaires \endlink in this space.
    std::vector<Luminaire> luminaires() const;

    /// Returns all ElectricEquipment in this space.
    std::vector<ElectricEquipment> electricEquipment() const;

    /// Returns all GasEquipment in this space.
    std::vector<GasEquipment> gasEquipment() const;

    /// Returns all HotWaterEquipment in this space.
    std::vector<HotWaterEquipment> hotWaterEquipment() const;

    /// Returns all SteamEquipment in this space.
    std::vector<SteamEquipment> steamEquipment() const;

    /// Returns all OtherEquipment in this space.
    std::vector<OtherEquipment> otherEquipment() const;  

    /// Returns all WaterUseEquipment in this space.
    std::vector<WaterUseEquipment> waterUseEquipment() const; 

    /// Returns all DaylightingControls in this space.
    std::vector<DaylightingControl> daylightingControls() const;

    /// Returns all IlluminanceMaps in this space.
    std::vector<IlluminanceMap> illuminanceMaps() const;

    /// Returns all GlareSensors in this space.
    std::vector<GlareSensor> glareSensors() const;  
    
    /// Returns all SpaceInfiltrationDesignFlowRate objects in this space.
    std::vector<SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates() const;

    /// Returns all SpaceInfiltrationEffectiveLeakageArea objects in this space.
    std::vector<SpaceInfiltrationEffectiveLeakageArea> spaceInfiltrationEffectiveLeakageAreas() const;

    /// Returns DesignSpecificationOutdoorAir for this space, if not set directly on this space then
    /// the SpaceType's DesignSpecificationOutdoorAir will be returned.
    boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir() const;

    /// Returns true if DesignSpecificationOutdoorAir is not directly specified for this space.
    bool isDesignSpecificationOutdoorAirDefaulted() const;

    /// Sets DesignSpecificationOutdoorAir for this space.
    bool setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) ;

    /// Resets DesignSpecificationOutdoorAir for this space.
    void resetDesignSpecificationOutdoorAir();

    /// Returns the multiplier for this space, comes from thermal zone, defaults to 1.
    int multiplier() const;

    double floorArea() const;

    double exteriorArea() const;

    double exteriorWallArea() const;

    double volume() const;

    double numberOfPeople() const;

    bool setNumberOfPeople(double numberOfPeople);

    bool setNumberOfPeople(double numberOfPeople, 
                           const boost::optional<People>& templatePeople);

    double peoplePerFloorArea() const;

    bool setPeoplePerFloorArea(double peoplePerFloorArea);

    bool setPeoplePerFloorArea(double peoplePerFloorArea, 
                               const boost::optional<People>& templatePeople);

    double floorAreaPerPerson() const;

    bool setFloorAreaPerPerson(double floorAreaPerPerson);

    bool setFloorAreaPerPerson(double floorAreaPerPerson, 
                               const boost::optional<People>& templatePeople);

    double lightingPower() const;

    bool setLightingPower(double lightingPower);

    bool setLightingPower(double lightingPower, const boost::optional<Lights>& templateLights);

    double lightingPowerPerFloorArea() const;

    bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea);

    bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, 
                                      const boost::optional<Lights>& templateLights);

    double lightingPowerPerPerson() const;

    bool setLightingPowerPerPerson(double lightingPowerPerPerson);

    bool setLightingPowerPerPerson(double lightingPowerPerPerson, 
                                   const boost::optional<Lights>& templateLights);

    double electricEquipmentPower() const;

    bool setElectricEquipmentPower(double electricEquipmentPower);

    bool setElectricEquipmentPower(
        double electricEquipmentPower, 
        const boost::optional<ElectricEquipment>& templateElectricEquipment);

    double electricEquipmentPowerPerFloorArea() const;

    bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea);

    bool setElectricEquipmentPowerPerFloorArea(
        double electricEquipmentPowerPerFloorArea, 
        const boost::optional<ElectricEquipment>& templateElectricEquipment);

    double electricEquipmentPowerPerPerson() const;

    bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson);

    bool setElectricEquipmentPowerPerPerson(
        double electricEquipmentPowerPerPerson, 
        const boost::optional<ElectricEquipment>& templateElectricEquipment);

    double gasEquipmentPower() const;

    bool setGasEquipmentPower(double gasEquipmentPower);

    bool setGasEquipmentPower(double gasEquipmentPower, 
                              const boost::optional<GasEquipment>& templateGasEquipment);

    double gasEquipmentPowerPerFloorArea() const;

    bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea);

    bool setGasEquipmentPowerPerFloorArea(
        double gasEquipmentPowerPerFloorArea, 
        const boost::optional<GasEquipment>& templateGasEquipment);

    double gasEquipmentPowerPerPerson() const;

    bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson);

    bool setGasEquipmentPowerPerPerson(
        double gasEquipmentPowerPerPerson, 
        const boost::optional<GasEquipment>& templateGasEquipment);

    /** Returns the infiltration design flow rate (m^3/s) in the space. Ignores
     *  SpaceInfiltrationEffectiveLeakageArea objects. */
    /// Does not include space multiplier in calculation.
    double infiltrationDesignFlowRate() const;

    /** Returns the infiltration design flow per space floor area (m^3/m^2*s) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
    /// Does not include space multiplier in calculation.
    double infiltrationDesignFlowPerSpaceFloorArea() const;

    /** Returns the infiltration design flow per exterior surface area (m^3/m^2*s) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
    /// Does not include space multiplier in calculation.
    double infiltrationDesignFlowPerExteriorSurfaceArea() const;

    /** Returns the infiltration design flow per exterior wall area (m^3/m^2*s) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
    /// Does not include space multiplier in calculation.
    double infiltrationDesignFlowPerExteriorWallArea() const;

    /** Returns the infiltration design air changes per hour (1/h) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
    /// Does not include space multiplier in calculation.
    double infiltrationDesignAirChangesPerHour() const;

    /** The following functionality is used by the EnergyPlus translator, the primary aim
     *  is to preserve information while putting the space into a state where it can easily
     *  be translated to EnergyPlus:
     *    Clone the space type if there is a space type
     *    Hard apply schedules to space type loads if there is a space type
     *    Move space loads from space type to space if there is a space type
     *    Merge space type default constructions with space default constructions if there is a space type
     *    Hard apply schedules to space loads
     *    If hard sizing loads, make each space load refer to unique definition and hard size it
     *    Remove the cloned space type if there is a space type
     */
    void hardApplySpaceType(bool hardSizeLoads);

    /** Set all schedules for child space loads to their default value if there is one. */
    void hardApplySpaceLoadSchedules();

    /** Set all constructions for child surfaces to their default value if there is one. */
    void hardApplyConstructions();

    /** Unmatch any matched surfaces and sub surfaces in this space. */
    void unmatchSurfaces();

    /** Match surfaces and sub surfaces in this space with those in the other. */
    void matchSurfaces(Space& other);

    /** Intersect surfaces in this space with those in the other. */
    void intersectSurfaces(Space& other);

    /** Find surfaces within angular range, specified in degrees and in the site coordinate system, an unset optional means no limit.
        Values for degrees from North are between 0 and 360 and for degrees tilt they are between 0 and 180.
        Note that maxDegreesFromNorth may be less than minDegreesFromNorth,
        e.g. to search between NorthWest and NorthEast set minDegreesFromNorth = 315 and maxDegreesFromNorth = 45.
        The value of maxDegreesTilt must be greater than or equal to minDegreesTilt.  A tolerance of one degree is applied by default.
    */
    std::vector <Surface> findSurfaces(boost::optional<double> minDegreesFromNorth,
                                       boost::optional<double> maxDegreesFromNorth,
                                       boost::optional<double> minDegreesTilt,
                                       boost::optional<double> maxDegreesTilt,
                                       double tol);

    /** Returns the floor print of the space.
        Will return empty vector if all floors in space are not on the same x,y plane.
    */
    std::vector<Point3d> floorPrint() const;

    bool isPlenum() const;

   private:
    REGISTER_LOGGER("openstudio.model.Space");

    openstudio::Quantity directionofRelativeNorth_SI() const;
    openstudio::Quantity directionofRelativeNorth_IP() const;
    bool setDirectionofRelativeNorth(const Quantity& directionofRelativeNorth);   

    openstudio::Quantity xOrigin_SI() const;
    openstudio::Quantity xOrigin_IP() const;
    bool setXOrigin(const Quantity& xOrigin);

    openstudio::Quantity yOrigin_SI() const;
    openstudio::Quantity yOrigin_IP() const;
    bool setYOrigin(const Quantity& yOrigin);

    openstudio::Quantity zOrigin_SI() const;
    openstudio::Quantity zOrigin_IP() const;
    bool setZOrigin(const Quantity& zOrigin);

    boost::optional<ModelObject> spaceTypeAsModelObject() const;
    boost::optional<ModelObject> defaultConstructionSetAsModelObject() const;
    boost::optional<ModelObject> defaultScheduleSetAsModelObject() const;
    boost::optional<ModelObject> thermalZoneAsModelObject() const;
    boost::optional<ModelObject> buildingStoryAsModelObject() const;
    std::vector<ModelObject> shadingSurfaceGroupsAsModelObjects() const;
    std::vector<ModelObject> interiorPartitionSurfaceGroupsAsModelObjects() const;
    std::vector<ModelObject> surfacesAsModelObjects() const;
    std::vector<ModelObject> internalMassAsModelObjects() const;
    std::vector<ModelObject> peopleAsModelObjects() const;
    std::vector<ModelObject> lightsAsModelObjects() const;
    std::vector<ModelObject> luminairesAsModelObjects() const;
    std::vector<ModelObject> electricEquipmentAsModelObjects() const;
    std::vector<ModelObject> gasEquipmentAsModelObjects() const;
    std::vector<ModelObject> hotWaterEquipmentAsModelObjects() const;
    std::vector<ModelObject> steamEquipmentAsModelObjects() const;
    std::vector<ModelObject> otherEquipmentAsModelObjects() const;
    std::vector<ModelObject> daylightingControlsAsModelObjects() const;
    std::vector<ModelObject> illuminanceMapsAsModelObjects() const;
    std::vector<ModelObject> spaceInfiltrationDesignFlowRatesAsModelObjects() const;
    std::vector<ModelObject> spaceInfiltrationEffectiveLeakageAreasAsModelObjects() const;
    std::vector<ModelObject> spaceVentilationDesignFlowRatesAsModelObjects() const;

    bool setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setBuildingStoryAsModelObject(const boost::optional<ModelObject>& modelObject);

    template <typename T, typename TDef> 
    boost::optional<T> getMySpaceLoadInstance(const boost::optional<T>& templateSpaceLoadInstance);

    template <typename T>
    void removeAllButOneSpaceLoadInstance(std::vector<T>& instances, const T& instanceToKeep);

    // helper function to get a boost polygon point from a Point3d
    boost::tuple<double, double> point3dToTuple(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol) const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACE_IMPL_HPP

