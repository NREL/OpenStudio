/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACE_IMPL_HPP
#define MODEL_SPACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Surface.hpp"

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

namespace openstudio {

class Polygon3d;
class Polyhedron;

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
  class BuildingUnit;
  class ShadingSurfaceGroup;
  class InteriorPartitionSurfaceGroup;
  class PlanarSurface;
  class Surface;
  class InternalMass;
  class People;
  class Lights;
  class Luminaire;
  class ElectricEquipment;
  class ElectricEquipmentITEAirCooled;
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
  class SpaceInfiltrationFlowCoefficient;
  class DesignSpecificationOutdoorAir;
  class Space;
  class ZoneMixing;

  namespace detail {

    /** Space_Impl is a PlanarSurfaceGroup_Impl that is the implementation class for Space.*/
    class MODEL_API Space_Impl : public PlanarSurfaceGroup_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Space_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Space_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Space_Impl(const Space_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Space_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual openstudio::Transformation buildingTransformation() const override;

      virtual bool changeTransformation(const openstudio::Transformation& transformation) override;

      virtual openstudio::BoundingBox boundingBox() const override;

      /** @name Getters */
      //@{

      virtual double directionofRelativeNorth() const override;

      virtual bool isDirectionofRelativeNorthDefaulted() const override;

      virtual double xOrigin() const override;

      virtual bool isXOriginDefaulted() const override;

      virtual double yOrigin() const override;

      virtual bool isYOriginDefaulted() const override;

      virtual double zOrigin() const override;

      virtual bool isZOriginDefaulted() const override;

      bool partofTotalFloorArea() const;

      bool isPartofTotalFloorAreaDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod = true) override;

      virtual void resetDirectionofRelativeNorth() override;

      virtual bool setXOrigin(double xOrigin, bool driverMethod = true) override;

      virtual void resetXOrigin() override;

      virtual bool setYOrigin(double yOrigin, bool driverMethod = true) override;

      virtual void resetYOrigin() override;

      virtual bool setZOrigin(double zOrigin, bool driverMethod = true) override;

      virtual void resetZOrigin() override;

      bool setPartofTotalFloorArea(bool partofTotalFloorArea);

      void resetPartofTotalFloorArea();

      bool setCeilingHeight(double ceilingHeight);

      void autocalculateCeilingHeight();

      void resetCeilingHeight();

      bool setVolume(double volume);

      void autocalculateVolume();

      void resetVolume();

      bool setFloorArea(double floorArea);

      void autocalculateFloorArea();

      void resetFloorArea();

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
      boost::optional<std::pair<ConstructionBase, int>> getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const;

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

      boost::optional<BuildingUnit> buildingUnit() const;

      bool setBuildingUnit(const BuildingUnit& buildingUnit);

      void resetBuildingUnit();

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

      /// Returns all ElectricEquipmentITEAirCooled in this space.
      std::vector<ElectricEquipmentITEAirCooled> electricEquipmentITEAirCooled() const;

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

      /// Returns all SpaceInfiltrationFlowCoefficient objects in this space.
      std::vector<SpaceInfiltrationFlowCoefficient> spaceInfiltrationFlowCoefficients() const;

      /// Returns DesignSpecificationOutdoorAir for this space, if not set directly on this space then
      /// the SpaceType's DesignSpecificationOutdoorAir will be returned.
      boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir() const;

      /// Returns true if DesignSpecificationOutdoorAir is not directly specified for this space.
      bool isDesignSpecificationOutdoorAirDefaulted() const;

      /// Sets DesignSpecificationOutdoorAir for this space.
      bool setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

      /// Resets DesignSpecificationOutdoorAir for this space.
      void resetDesignSpecificationOutdoorAir();

      /// Returns the multiplier for this space, comes from thermal zone, defaults to 1.
      int multiplier() const;

      double exteriorArea() const;

      double exteriorWallArea() const;

      double ceilingHeight() const;

      bool isCeilingHeightDefaulted() const;

      bool isCeilingHeightAutocalculated() const;

      double volume() const;

      bool isVolumeDefaulted() const;

      bool isVolumeAutocalculated() const;

      double floorArea() const;

      bool isFloorAreaDefaulted() const;

      bool isFloorAreaAutocalculated() const;

      double numberOfPeople() const;

      bool setNumberOfPeople(double numberOfPeople);

      bool setNumberOfPeople(double numberOfPeople, const boost::optional<People>& templatePeople);

      double peoplePerFloorArea() const;

      bool setPeoplePerFloorArea(double peoplePerFloorArea);

      bool setPeoplePerFloorArea(double peoplePerFloorArea, const boost::optional<People>& templatePeople);

      double floorAreaPerPerson() const;

      bool setFloorAreaPerPerson(double floorAreaPerPerson);

      bool setFloorAreaPerPerson(double floorAreaPerPerson, const boost::optional<People>& templatePeople);

      double lightingPower() const;

      bool setLightingPower(double lightingPower);

      bool setLightingPower(double lightingPower, const boost::optional<Lights>& templateLights);

      double lightingPowerPerFloorArea() const;

      bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea);

      bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, const boost::optional<Lights>& templateLights);

      double lightingPowerPerPerson() const;

      bool setLightingPowerPerPerson(double lightingPowerPerPerson);

      bool setLightingPowerPerPerson(double lightingPowerPerPerson, const boost::optional<Lights>& templateLights);

      double electricEquipmentPower() const;

      bool setElectricEquipmentPower(double electricEquipmentPower);

      bool setElectricEquipmentPower(double electricEquipmentPower, const boost::optional<ElectricEquipment>& templateElectricEquipment);

      double electricEquipmentPowerPerFloorArea() const;

      bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea);

      bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea,
                                                 const boost::optional<ElectricEquipment>& templateElectricEquipment);

      double electricEquipmentPowerPerPerson() const;

      bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson);

      bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson,
                                              const boost::optional<ElectricEquipment>& templateElectricEquipment);

      double electricEquipmentITEAirCooledPower() const;

      double electricEquipmentITEAirCooledPowerPerFloorArea() const;

      double gasEquipmentPower() const;

      bool setGasEquipmentPower(double gasEquipmentPower);

      bool setGasEquipmentPower(double gasEquipmentPower, const boost::optional<GasEquipment>& templateGasEquipment);

      double gasEquipmentPowerPerFloorArea() const;

      bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea);

      bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea, const boost::optional<GasEquipment>& templateGasEquipment);

      double gasEquipmentPowerPerPerson() const;

      bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson);

      bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson, const boost::optional<GasEquipment>& templateGasEquipment);

      /** Returns the infiltration design flow rate (m^3/s) in the space. Ignores
     *  SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. */
      /// Does not include space multiplier in calculation.
      double infiltrationDesignFlowRate() const;

      /** Returns the infiltration design flow per space floor area (m^3/m^2*s) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. */
      /// Does not include space multiplier in calculation.
      double infiltrationDesignFlowPerSpaceFloorArea() const;

      /** Returns the infiltration design flow per exterior surface area (m^3/m^2*s) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. */
      /// Does not include space multiplier in calculation.
      double infiltrationDesignFlowPerExteriorSurfaceArea() const;

      /** Returns the infiltration design flow per exterior wall area (m^3/m^2*s) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. */
      /// Does not include space multiplier in calculation.
      double infiltrationDesignFlowPerExteriorWallArea() const;

      /** Returns the infiltration design air changes per hour (1/h) in the space.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. */
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
      std::vector<Surface> findSurfaces(boost::optional<double> minDegreesFromNorth, boost::optional<double> maxDegreesFromNorth,
                                        boost::optional<double> minDegreesTilt, boost::optional<double> maxDegreesTilt, double tol);

      /** Returns the floor print of the space.
        Will return empty vector if all floors in space are not on the same x,y plane.
    */
      std::vector<Point3d> floorPrint() const;

      bool isPlenum() const;

      double exposedPerimeter(const Polygon3d& buildingPerimeter) const;

      Polyhedron polyhedron() const;
      bool isEnclosedVolume() const;

      // Find all surfaces where the outwardNormal does not point towards the outside of the Space
      std::vector<Surface> findSurfacesWithIncorrectOrientation() const;

      std::vector<Surface> findSurfacesWithIncorrectOrientationRaycasting() const;
      std::vector<Surface> findSurfacesWithIncorrectOrientationPolyhedron(const Polyhedron& volumePoly) const;

      // Checks the outwardNormal of every surface points towards the outside of the Space
      bool areAllSurfacesCorrectlyOriented() const;

      // Returns true if the orientation of any surface has been changed
      bool fixSurfacesWithIncorrectOrientation();

      /** This method will check the floorPrint of the space, if that can't be computed, it's not convex. If it can, it checks whether that resulting
        * Surface3d is convex. Note: having a floorPrint that's convex isn't sufficied to deem a space to be convex, the walls could still be non
        * convex, but it should suit most typical applications */
      bool isConvex() const;

      /** Checks if every Surface is convex, returns the Concave ones.
        * Note: having a non convex surface does not necesarilly mean that the Space is not convex
        * eg: a box with a wall that is split into two L s would return false, while the Space is actually still convex. */
      std::vector<Surface> findNonConvexSurfaces() const;

      void cacheGeometryDiagnostics();
      void resetCachedGeometryDiagnostics();

      std::vector<ZoneMixing> zoneMixing() const;
      std::vector<ZoneMixing> supplyZoneMixing() const;
      std::vector<ZoneMixing> exhaustZoneMixing() const;

     private:
      REGISTER_LOGGER("openstudio.model.Space");

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
      std::vector<ModelObject> electricEquipmentITEAirCooledAsModelObjects() const;
      std::vector<ModelObject> gasEquipmentAsModelObjects() const;
      std::vector<ModelObject> hotWaterEquipmentAsModelObjects() const;
      std::vector<ModelObject> steamEquipmentAsModelObjects() const;
      std::vector<ModelObject> otherEquipmentAsModelObjects() const;
      std::vector<ModelObject> daylightingControlsAsModelObjects() const;
      std::vector<ModelObject> illuminanceMapsAsModelObjects() const;
      std::vector<ModelObject> spaceInfiltrationDesignFlowRatesAsModelObjects() const;
      std::vector<ModelObject> spaceInfiltrationEffectiveLeakageAreasAsModelObjects() const;
      std::vector<ModelObject> spaceInfiltrationFlowCoefficientsAsModelObjects() const;
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

      mutable boost::optional<std::vector<Surface>> m_cachedNonConvexSurfaces;
      mutable boost::optional<bool> m_cachedIsConvex;
      mutable boost::optional<bool> m_cachedIsEnclosed;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACE_IMPL_HPP
