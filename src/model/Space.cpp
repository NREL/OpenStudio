/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Space.hpp"
#include "Space_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"
#include "InternalMassDefinition.hpp"
#include "InternalMassDefinition_Impl.hpp"
#include "People.hpp"
#include "People_Impl.hpp"
#include "PeopleDefinition.hpp"
#include "PeopleDefinition_Impl.hpp"
#include "Lights.hpp"
#include "Lights_Impl.hpp"
#include "LightsDefinition.hpp"
#include "LightsDefinition_Impl.hpp"
#include "Luminaire.hpp"
#include "Luminaire_Impl.hpp"
#include "LuminaireDefinition.hpp"
#include "LuminaireDefinition_Impl.hpp"
#include "ElectricEquipment.hpp"
#include "ElectricEquipment_Impl.hpp"
#include "ElectricEquipmentDefinition.hpp"
#include "ElectricEquipmentDefinition_Impl.hpp"
#include "ElectricEquipmentITEAirCooled.hpp"
#include "ElectricEquipmentITEAirCooled_Impl.hpp"
#include "ElectricEquipmentITEAirCooledDefinition.hpp"
#include "ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
#include "GasEquipment.hpp"
#include "GasEquipment_Impl.hpp"
#include "GasEquipmentDefinition.hpp"
#include "GasEquipmentDefinition_Impl.hpp"
#include "HotWaterEquipment.hpp"
#include "HotWaterEquipment_Impl.hpp"
#include "SteamEquipment.hpp"
#include "SteamEquipment_Impl.hpp"
#include "OtherEquipment.hpp"
#include "OtherEquipment_Impl.hpp"
#include "WaterUseEquipment.hpp"
#include "WaterUseEquipment_Impl.hpp"
#include "DaylightingControl.hpp"
#include "DaylightingControl_Impl.hpp"
#include "IlluminanceMap.hpp"
#include "IlluminanceMap_Impl.hpp"
#include "SpaceInfiltrationDesignFlowRate.hpp"
#include "SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "SpaceInfiltrationFlowCoefficient.hpp"
#include "SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"
#include "GlareSensor.hpp"
#include "GlareSensor_Impl.hpp"
#include "ZoneMixing.hpp"
#include "ZoneMixing_Impl.hpp"

#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"
#include "../utilities/geometry/EulerAngles.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Polygon3d.hpp"
#include "../utilities/geometry/Polyhedron.hpp"

#include "../utilities/core/ContainersMove.hpp"

#include "../utilities/core/Assert.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4244)
#endif
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#include <algorithm>
#include <cmath>
#include <iterator>

#include <fmt/core.h>

namespace openstudio {
namespace model {

  namespace detail {

    Space_Impl::Space_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : PlanarSurfaceGroup_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Space::iddObjectType());
    }

    Space_Impl::Space_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : PlanarSurfaceGroup_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Space::iddObjectType());
    }

    Space_Impl::Space_Impl(const Space_Impl& other, Model_Impl* model, bool keepHandle) : PlanarSurfaceGroup_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> Space_Impl::parent() const {
      return boost::optional<ParentObject>(this->model().building());
    }

    bool Space_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (newParent.optionalCast<Building>()) {
        result = (this->model() == newParent.model());
      }
      return result;
    }

    std::vector<ModelObject> Space_Impl::children() const {
      return concat<ModelObject>(this->shadingSurfaceGroups(), this->interiorPartitionSurfaceGroups(), this->surfaces(), this->internalMass(),
                                 this->lights(), this->luminaires(), this->people(), this->electricEquipment(), this->electricEquipmentITEAirCooled(),
                                 this->gasEquipment(), this->hotWaterEquipment(), this->steamEquipment(), this->otherEquipment(),
                                 this->waterUseEquipment(), this->daylightingControls(), this->illuminanceMaps(), this->glareSensors(),
                                 this->spaceInfiltrationDesignFlowRates(), this->spaceInfiltrationEffectiveLeakageAreas(),
                                 this->spaceInfiltrationFlowCoefficients(), this->supplyZoneMixing());
    }

    std::vector<IddObjectType> Space_Impl::allowableChildTypes() const {
      return {
        IddObjectType::OS_ShadingSurfaceGroup,
        IddObjectType::OS_InteriorPartitionSurfaceGroup,
        IddObjectType::OS_Surface,
        IddObjectType::OS_Lights,
        IddObjectType::OS_Luminaire,
        IddObjectType::OS_People,
        IddObjectType::OS_ElectricEquipment,
        IddObjectType::OS_ElectricEquipment_ITE_AirCooled,
        IddObjectType::OS_GasEquipment,
        IddObjectType::OS_HotWaterEquipment,
        IddObjectType::OS_Daylighting_Control,
        IddObjectType::OS_IlluminanceMap,
        IddObjectType::OS_SpaceInfiltration_DesignFlowRate,
        IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea,
        IddObjectType::OS_SpaceInfiltration_FlowCoefficient,
      };
    }

    const std::vector<std::string>& Space_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Space_Impl::iddObjectType() const {
      return Space::iddObjectType();
    }

    Transformation Space_Impl::buildingTransformation() const {
      return this->transformation();
    }

    bool Space_Impl::changeTransformation(const openstudio::Transformation& transformation) {
      Transformation oldTransformation = this->transformation();
      if (!setTransformation(transformation)) {
        return false;
      }

      // go through and update all child points and transformations so that:
      // (Tnew^-1 * Told) * xold = childTransformation * xold = xnew

      Transformation childTransformation = transformation.inverse() * oldTransformation;

      for (Surface& surface : this->surfaces()) {
        bool test = surface.setVertices(childTransformation * surface.vertices());
        if (!test) {
          LOG(Error, "Could not transform vertices for Surface '" << surface.name().get() << "'.");
        }
        for (SubSurface& subSurface : surface.subSurfaces()) {
          test = subSurface.setVertices(childTransformation * subSurface.vertices());
          if (!test) {
            LOG(Error, "Could not transform vertices for SubSurface '" << subSurface.name().get() << "'.");
          }
        }
      }

      for (ShadingSurfaceGroup& shadingSurfaceGroup : this->shadingSurfaceGroups()) {
        bool test = shadingSurfaceGroup.setTransformation(childTransformation * shadingSurfaceGroup.transformation());
        OS_ASSERT(test);
      }

      for (InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup : this->interiorPartitionSurfaceGroups()) {
        bool test = interiorPartitionSurfaceGroup.setTransformation(childTransformation * interiorPartitionSurfaceGroup.transformation());
        OS_ASSERT(test);
      }

      for (Luminaire& luminaire : this->luminaires()) {
        bool test = luminaire.setTransformation(childTransformation * luminaire.transformation());
        OS_ASSERT(test);
      }

      for (DaylightingControl& daylightingControl : this->daylightingControls()) {
        bool test = daylightingControl.setTransformation(childTransformation * daylightingControl.transformation());
        OS_ASSERT(test);
      }

      for (IlluminanceMap& illuminanceMap : this->illuminanceMaps()) {
        bool test = illuminanceMap.setTransformation(childTransformation * illuminanceMap.transformation());
        OS_ASSERT(test);
      }

      return true;
    }

    BoundingBox Space_Impl::boundingBox() const {
      BoundingBox result;

      for (Surface& surface : this->surfaces()) {
        result.addPoints(surface.vertices());
      }

      for (ShadingSurfaceGroup& shadingSurfaceGroup : this->shadingSurfaceGroups()) {
        result.addPoints(shadingSurfaceGroup.transformation() * shadingSurfaceGroup.boundingBox().corners());
      }

      for (InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup : this->interiorPartitionSurfaceGroups()) {
        result.addPoints(interiorPartitionSurfaceGroup.transformation() * interiorPartitionSurfaceGroup.boundingBox().corners());
      }

      for (Luminaire& luminaire : this->luminaires()) {
        result.addPoint(luminaire.position());
      }

      for (DaylightingControl& daylightingControl : this->daylightingControls()) {
        result.addPoint(daylightingControl.position());
      }

      for (IlluminanceMap& illuminanceMap : this->illuminanceMaps()) {
        result.addPoints(illuminanceMap.transformation() * illuminanceMap.corners());
      }

      for (GlareSensor& glareSensor : this->glareSensors()) {
        result.addPoint(glareSensor.position());
      }

      return result;
    }

    double Space_Impl::directionofRelativeNorth() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::DirectionofRelativeNorth, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Space_Impl::isDirectionofRelativeNorthDefaulted() const {
      return isEmpty(OS_SpaceFields::DirectionofRelativeNorth);
    }

    double Space_Impl::xOrigin() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::XOrigin, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Space_Impl::isXOriginDefaulted() const {
      return isEmpty(OS_SpaceFields::XOrigin);
    }

    double Space_Impl::yOrigin() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::YOrigin, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Space_Impl::isYOriginDefaulted() const {
      return isEmpty(OS_SpaceFields::YOrigin);
    }

    double Space_Impl::zOrigin() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::ZOrigin, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Space_Impl::isZOriginDefaulted() const {
      return isEmpty(OS_SpaceFields::ZOrigin);
    }

    bool Space_Impl::partofTotalFloorArea() const {
      boost::optional<std::string> value = getString(OS_SpaceFields::PartofTotalFloorArea, false, true);
      if (!value) {
        if (this->isPlenum()) {
          return false;
        }
        value = getString(OS_SpaceFields::PartofTotalFloorArea, true);
      }
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool Space_Impl::isPartofTotalFloorAreaDefaulted() const {
      return isEmpty(OS_SpaceFields::PartofTotalFloorArea);
    }

    bool Space_Impl::setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod) {
      bool result = setDouble(OS_SpaceFields::DirectionofRelativeNorth, directionofRelativeNorth, driverMethod);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::resetDirectionofRelativeNorth() {
      bool result = setString(OS_SpaceFields::DirectionofRelativeNorth, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setXOrigin(double xOrigin, bool driverMethod) {
      bool result = setDouble(OS_SpaceFields::XOrigin, xOrigin, driverMethod);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::resetXOrigin() {
      bool result = setString(OS_SpaceFields::XOrigin, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setYOrigin(double yOrigin, bool driverMethod) {
      bool result = setDouble(OS_SpaceFields::YOrigin, yOrigin, driverMethod);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::resetYOrigin() {
      bool result = setString(OS_SpaceFields::YOrigin, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setZOrigin(double zOrigin, bool driverMethod) {
      bool result = setDouble(OS_SpaceFields::ZOrigin, zOrigin, driverMethod);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::resetZOrigin() {
      bool result = setString(OS_SpaceFields::ZOrigin, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setPartofTotalFloorArea(bool partofTotalFloorArea) {
      bool result = false;
      if (partofTotalFloorArea) {
        result = setString(OS_SpaceFields::PartofTotalFloorArea, "Yes");
      } else {
        result = setString(OS_SpaceFields::PartofTotalFloorArea, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::resetPartofTotalFloorArea() {
      bool result = setString(OS_SpaceFields::PartofTotalFloorArea, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setCeilingHeight(double ceilingHeight) {
      bool result = setDouble(OS_SpaceFields::CeilingHeight, ceilingHeight);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::autocalculateCeilingHeight() {
      bool result = setString(OS_SpaceFields::CeilingHeight, "Autocalculate");
      OS_ASSERT(result);
    }

    void Space_Impl::resetCeilingHeight() {
      bool result = setString(OS_SpaceFields::CeilingHeight, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setVolume(double volume) {
      bool result = setDouble(OS_SpaceFields::Volume, volume);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::autocalculateVolume() {
      bool result = setString(OS_SpaceFields::Volume, "Autocalculate");
      OS_ASSERT(result);
    }

    void Space_Impl::resetVolume() {
      bool result = setString(OS_SpaceFields::Volume, "");
      OS_ASSERT(result);
    }

    bool Space_Impl::setFloorArea(double floorArea) {
      bool result = setDouble(OS_SpaceFields::FloorArea, floorArea);
      OS_ASSERT(result);
      return result;
    }

    void Space_Impl::autocalculateFloorArea() {
      bool result = setString(OS_SpaceFields::FloorArea, "Autocalculate");
      OS_ASSERT(result);
    }

    void Space_Impl::resetFloorArea() {
      bool result = setString(OS_SpaceFields::FloorArea, "");
      OS_ASSERT(result);
    }

    boost::optional<SpaceType> Space_Impl::spaceType() const {
      boost::optional<SpaceType> result = getObject<ModelObject>().getModelObjectTarget<SpaceType>(OS_SpaceFields::SpaceTypeName);
      if (!result) {
        if (this->isPlenum()) {
          result = this->model().plenumSpaceType();
        } else {
          boost::optional<Building> building = this->model().building();
          if (building) {
            result = building->spaceType();
          }
        }
      }
      return result;
    }

    bool Space_Impl::isSpaceTypeDefaulted() const {
      return isEmpty(OS_SpaceFields::SpaceTypeName);
    }

    bool Space_Impl::setSpaceType(const SpaceType& spaceType) {
      return setPointer(OS_SpaceFields::SpaceTypeName, spaceType.handle());
    }

    void Space_Impl::resetSpaceType() {
      setString(OS_SpaceFields::SpaceTypeName, "");
    }

    boost::optional<DefaultConstructionSet> Space_Impl::defaultConstructionSet() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultConstructionSet>(OS_SpaceFields::DefaultConstructionSetName);
    }

    boost::optional<ConstructionBase> Space_Impl::getDefaultConstruction(const PlanarSurface& planarSurface) const {
      boost::optional<std::pair<ConstructionBase, int>> result = this->getDefaultConstructionWithSearchDistance(planarSurface);
      if (result) {
        return result->first;
      }
      return boost::none;
    }

    boost::optional<std::pair<ConstructionBase, int>> Space_Impl::getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const {
      boost::optional<ConstructionBase> result;
      boost::optional<DefaultConstructionSet> defaultConstructionSet;
      boost::optional<SpaceType> spaceType;
      boost::optional<BuildingStory> buildingStory;
      boost::optional<Building> building;

      // first check this object
      defaultConstructionSet = this->defaultConstructionSet();
      if (defaultConstructionSet) {
        result = defaultConstructionSet->getDefaultConstruction(planarSurface);
        if (result) {
          return std::make_pair(*result, 1);
        }
      }

      // then check space type
      spaceType = this->spaceType();
      if (spaceType && !this->isSpaceTypeDefaulted()) {
        defaultConstructionSet = spaceType->defaultConstructionSet();
        if (defaultConstructionSet) {
          result = defaultConstructionSet->getDefaultConstruction(planarSurface);
          if (result) {
            return std::make_pair(*result, 2);
          }
        }
      }

      // then check building story
      buildingStory = this->buildingStory();
      if (buildingStory) {
        defaultConstructionSet = buildingStory->defaultConstructionSet();
        if (defaultConstructionSet) {
          result = defaultConstructionSet->getDefaultConstruction(planarSurface);
          if (result) {
            return std::make_pair(*result, 3);
          }
        }
      }

      // then check building
      building = this->model().building();
      if (building) {
        defaultConstructionSet = building->defaultConstructionSet();
        if (defaultConstructionSet) {
          result = defaultConstructionSet->getDefaultConstruction(planarSurface);
          if (result) {
            return std::make_pair(*result, 4);
          }
        }

        // then check building's space type
        spaceType = building->spaceType();
        if (spaceType) {
          defaultConstructionSet = spaceType->defaultConstructionSet();
          if (defaultConstructionSet) {
            result = defaultConstructionSet->getDefaultConstruction(planarSurface);
            if (result) {
              return std::make_pair(*result, 5);
            }
          }
        }
      }

      return boost::none;
    }

    bool Space_Impl::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet) {
      return setPointer(OS_SpaceFields::DefaultConstructionSetName, defaultConstructionSet.handle());
    }

    void Space_Impl::resetDefaultConstructionSet() {
      setString(OS_SpaceFields::DefaultConstructionSetName, "");
    }

    boost::optional<DefaultScheduleSet> Space_Impl::defaultScheduleSet() const {
      return getObject<ModelObject>().getModelObjectTarget<DefaultScheduleSet>(OS_SpaceFields::DefaultScheduleSetName);
    }

    boost::optional<Schedule> Space_Impl::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const {
      boost::optional<Schedule> result;
      boost::optional<DefaultScheduleSet> defaultScheduleSet;
      boost::optional<SpaceType> spaceType;
      boost::optional<BuildingStory> buildingStory;
      boost::optional<Building> building;

      // first check this object
      defaultScheduleSet = this->defaultScheduleSet();
      if (defaultScheduleSet) {
        result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
        if (result) {
          return result;
        }
      }

      // then check space type
      spaceType = this->spaceType();
      if (spaceType) {
        defaultScheduleSet = spaceType->defaultScheduleSet();
        if (defaultScheduleSet) {
          result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
          if (result) {
            return result;
          }
        }
      }

      // then check building story
      buildingStory = this->buildingStory();
      if (buildingStory) {
        defaultScheduleSet = buildingStory->defaultScheduleSet();
        if (defaultScheduleSet) {
          result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
          if (result) {
            return result;
          }
        }
      }

      // then check building
      building = this->model().building();
      if (building) {
        defaultScheduleSet = building->defaultScheduleSet();
        if (defaultScheduleSet) {
          result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
          if (result) {
            return result;
          }
        }

        // then check building's space type
        spaceType = building->spaceType();
        if (spaceType) {
          defaultScheduleSet = spaceType->defaultScheduleSet();
          if (defaultScheduleSet) {
            result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
            if (result) {
              return result;
            }
          }
        }
      }

      return boost::none;
    }

    bool Space_Impl::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet) {
      return setPointer(OS_SpaceFields::DefaultScheduleSetName, defaultScheduleSet.handle());
    }

    void Space_Impl::resetDefaultScheduleSet() {
      setString(OS_SpaceFields::DefaultScheduleSetName, "");
    }

    OptionalThermalZone Space_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SpaceFields::ThermalZoneName);
    }

    bool Space_Impl::setThermalZone(ThermalZone& thermalZone) {
      bool result = this->setPointer(OS_SpaceFields::ThermalZoneName, thermalZone.handle());
      if (result) {
        thermalZone.checkDaylightingControlsAndIlluminanceMaps();
      }
      return result;
    }

    void Space_Impl::resetThermalZone() {
      bool result = this->setString(OS_SpaceFields::ThermalZoneName, "");
      OS_ASSERT(result);
    }

    OptionalBuildingStory Space_Impl::buildingStory() const {
      return getObject<ModelObject>().getModelObjectTarget<BuildingStory>(OS_SpaceFields::BuildingStoryName);
    }

    bool Space_Impl::setBuildingStory(const BuildingStory& buildingStory) {
      return this->setPointer(OS_SpaceFields::BuildingStoryName, buildingStory.handle());
    }

    void Space_Impl::resetBuildingStory() {
      bool result = this->setString(OS_SpaceFields::BuildingStoryName, "");
      OS_ASSERT(result);
    }

    boost::optional<BuildingUnit> Space_Impl::buildingUnit() const {
      return getObject<ModelObject>().getModelObjectTarget<BuildingUnit>(OS_SpaceFields::BuildingUnitName);
    }

    bool Space_Impl::setBuildingUnit(const BuildingUnit& buildingUnit) {
      return this->setPointer(OS_SpaceFields::BuildingUnitName, buildingUnit.handle());
    }

    void Space_Impl::resetBuildingUnit() {
      bool result = this->setString(OS_SpaceFields::BuildingUnitName, "");
      OS_ASSERT(result);
    }

    ShadingSurfaceGroupVector Space_Impl::shadingSurfaceGroups() const {
      return getObject<ModelObject>().getModelObjectSources<ShadingSurfaceGroup>(ShadingSurfaceGroup::iddObjectType());
    }

    InteriorPartitionSurfaceGroupVector Space_Impl::interiorPartitionSurfaceGroups() const {
      return getObject<ModelObject>().getModelObjectSources<InteriorPartitionSurfaceGroup>(InteriorPartitionSurfaceGroup::iddObjectType());
    }

    SurfaceVector Space_Impl::surfaces() const {
      return getObject<ModelObject>().getModelObjectSources<Surface>(Surface::iddObjectType());
    }

    std::vector<InternalMass> Space_Impl::internalMass() const {
      return getObject<Space>().getModelObjectSources<InternalMass>(InternalMass::iddObjectType());
    }

    PeopleVector Space_Impl::people() const {
      return getObject<ModelObject>().getModelObjectSources<People>(People::iddObjectType());
    }

    LightsVector Space_Impl::lights() const {
      return getObject<ModelObject>().getModelObjectSources<Lights>(Lights::iddObjectType());
    }

    LuminaireVector Space_Impl::luminaires() const {
      return getObject<ModelObject>().getModelObjectSources<Luminaire>(Luminaire::iddObjectType());
    }

    ElectricEquipmentVector Space_Impl::electricEquipment() const {
      return getObject<ModelObject>().getModelObjectSources<ElectricEquipment>(ElectricEquipment::iddObjectType());
    }

    ElectricEquipmentITEAirCooledVector Space_Impl::electricEquipmentITEAirCooled() const {
      return getObject<ModelObject>().getModelObjectSources<ElectricEquipmentITEAirCooled>(ElectricEquipmentITEAirCooled::iddObjectType());
    }

    GasEquipmentVector Space_Impl::gasEquipment() const {
      return getObject<ModelObject>().getModelObjectSources<GasEquipment>(GasEquipment::iddObjectType());
    }

    HotWaterEquipmentVector Space_Impl::hotWaterEquipment() const {
      return getObject<ModelObject>().getModelObjectSources<HotWaterEquipment>(HotWaterEquipment::iddObjectType());
    }

    SteamEquipmentVector Space_Impl::steamEquipment() const {
      return getObject<ModelObject>().getModelObjectSources<SteamEquipment>(SteamEquipment::iddObjectType());
    }

    OtherEquipmentVector Space_Impl::otherEquipment() const {
      return getObject<ModelObject>().getModelObjectSources<OtherEquipment>(OtherEquipment::iddObjectType());
    }

    WaterUseEquipmentVector Space_Impl::waterUseEquipment() const {
      return getObject<ModelObject>().getModelObjectSources<WaterUseEquipment>(WaterUseEquipment::iddObjectType());
    }

    DaylightingControlVector Space_Impl::daylightingControls() const {
      return getObject<ModelObject>().getModelObjectSources<DaylightingControl>(DaylightingControl::iddObjectType());
    }

    IlluminanceMapVector Space_Impl::illuminanceMaps() const {
      return getObject<ModelObject>().getModelObjectSources<IlluminanceMap>(IlluminanceMap::iddObjectType());
    }

    GlareSensorVector Space_Impl::glareSensors() const {
      return getObject<ModelObject>().getModelObjectSources<GlareSensor>(GlareSensor::iddObjectType());
    }

    SpaceInfiltrationDesignFlowRateVector Space_Impl::spaceInfiltrationDesignFlowRates() const {
      return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationDesignFlowRate>(SpaceInfiltrationDesignFlowRate::iddObjectType());
    }

    SpaceInfiltrationEffectiveLeakageAreaVector Space_Impl::spaceInfiltrationEffectiveLeakageAreas() const {
      return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationEffectiveLeakageArea>(
        SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
    }

    SpaceInfiltrationFlowCoefficientVector Space_Impl::spaceInfiltrationFlowCoefficients() const {
      return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationFlowCoefficient>(SpaceInfiltrationFlowCoefficient::iddObjectType());
    }

    boost::optional<DesignSpecificationOutdoorAir> Space_Impl::designSpecificationOutdoorAir() const {
      boost::optional<DesignSpecificationOutdoorAir> result;

      result = getObject<ModelObject>().getModelObjectTarget<DesignSpecificationOutdoorAir>(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName);

      if (!result) {
        boost::optional<SpaceType> spaceType = this->spaceType();
        if (spaceType) {
          result = spaceType->designSpecificationOutdoorAir();
        }
      }

      return result;
    }

    bool Space_Impl::isDesignSpecificationOutdoorAirDefaulted() const {
      return isEmpty(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName);
    }

    bool Space_Impl::setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) {
      return setPointer(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir.handle());
    }

    void Space_Impl::resetDesignSpecificationOutdoorAir() {
      bool test = setString(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName, "");
      OS_ASSERT(test);
    }

    int Space_Impl::multiplier() const {
      int result = 1;
      OptionalThermalZone thermalZone = this->thermalZone();
      if (thermalZone) {
        result = thermalZone->multiplier();
      }
      return result;
    }

    double Space_Impl::exteriorArea() const {
      double result = 0;
      for (const Surface& surface : this->surfaces()) {
        if (istringEqual(surface.outsideBoundaryCondition(), "Outdoors")) {
          result += surface.grossArea();
        }
      }
      return result;
    }

    double Space_Impl::exteriorWallArea() const {
      double result = 0;
      for (const Surface& surface : this->surfaces()) {
        if (istringEqual(surface.outsideBoundaryCondition(), "Outdoors")) {
          if (istringEqual(surface.surfaceType(), "Wall")) {
            result += surface.grossArea();
          }
        }
      }
      return result;
    }

    Polyhedron Space_Impl::polyhedron() const {
      auto sfs = surfaces();
      std::vector<Surface3d> surface3ds;
      surface3ds.reserve(sfs.size());
      for (size_t surfNum = 0; auto& surface : sfs) {
        surface3ds.emplace_back(surface.vertices(), surface.nameString(), surfNum++);
      }
      return {surface3ds};
    }

    bool Space_Impl::isEnclosedVolume() const {
      if (m_cachedIsEnclosed.has_value()) {
        return m_cachedIsEnclosed.get();
      }
      auto volumePoly = this->polyhedron();
      auto isVolEnclosed = volumePoly.isEnclosedVolume();
      if (!isVolEnclosed) {
        const auto edgesNot2 = volumePoly.edgesNotTwo();
        LOG(Warn, briefDescription() << " is not enclosed, there are " << edgesNot2.size() << " edges that aren't used exactly twice");
        for (const auto& edge : edgesNot2) {
          LOG(Debug, edge);
        }
      }
      return isVolEnclosed;
    }

    double Space_Impl::ceilingHeight() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::CeilingHeight, true);
      if (value) {
        return value.get();
      }

      double result = 0;

      // TODO: need a better method
      double roofHeight = 0;
      int numRoof = 0;
      double floorHeight = 0;
      int numFloor = 0;
      for (const Surface& surface : this->surfaces()) {
        if (istringEqual(surface.surfaceType(), "Floor")) {
          for (const Point3d& point : surface.vertices()) {
            floorHeight += point.z();
            ++numFloor;
          }
        } else if (istringEqual(surface.surfaceType(), "RoofCeiling")) {
          for (const Point3d& point : surface.vertices()) {
            roofHeight += point.z();
            ++numRoof;
          }
        }
      }

      if ((numRoof > 0) && (numFloor > 0)) {
        roofHeight /= numRoof;
        floorHeight /= numFloor;
        result = roofHeight - floorHeight;
      }

      return result;
    }

    bool Space_Impl::isCeilingHeightDefaulted() const {
      return isEmpty(OS_SpaceFields::CeilingHeight);
    }

    bool Space_Impl::isCeilingHeightAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SpaceFields::CeilingHeight, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    std::vector<Surface> Space_Impl::findSurfacesWithIncorrectOrientationRaycasting() const {
      std::vector<Surface> result;

      const Point3d p0{};

      auto surfaces = this->surfaces();
      for (const auto& surface : surfaces) {

        auto outwardNormal = surface.outwardNormal();
        auto inwardNormal = outwardNormal.reverseVector();

        auto surfacePoint = surface.centroid();
        bool found = false;
        for (const auto& anotherSurface : surfaces) {
          if (surface == anotherSurface) {
            continue;
          }
          auto anotherPlane = anotherSurface.plane();
          const double vd = anotherPlane.outwardNormal().dot(outwardNormal);

          auto reversedPlane = anotherPlane.reversePlane();
          const double vd2 = reversedPlane.outwardNormal().dot(inwardNormal);
          OS_ASSERT(vd == vd2);

          // If this isn't orthogonal
          if (std::abs(vd) < 0.001) {
            LOG(Debug, "Surface " << surface.nameString() << " is orthogonal to " << anotherSurface.nameString());
            continue;
          }
          if (vd < 0) {
            LOG(Debug, "Surfaces '" << surface.nameString() << "' and '" << anotherSurface.nameString()
                                    << "' are seemingly pointing in the opposite outward direction, which is a good sign.");
          } else {
            LOG(Debug, "Surfaces '" << surface.nameString() << "' and '" << anotherSurface.nameString()
                                    << "' are seemingly pointing in the same outward direction, potentially one is in the wrong direction.");
          }

          const double v0 = anotherPlane.outwardNormal().dot(surfacePoint - p0) + anotherPlane.d();
          const double v02 = -(reversedPlane.outwardNormal().dot(surfacePoint - p0) + reversedPlane.d());
          OS_ASSERT(v0 == v02);
          const double t = v0 / vd;
          if (t > 0.0) {
            LOG(Debug, "Surface '" << surface.nameString() << "' raycasts to " << anotherSurface.nameString());
            found = true;
            break;
          } else {
            LOG(Debug, "Surface '" << surface.nameString() << "' does not raycast to " << anotherSurface.nameString());
          }
        }
        if (!found) {
          result.push_back(surface);
        }
      }

      return result;
    }

    std::vector<Surface> Space_Impl::findSurfacesWithIncorrectOrientationPolyhedron(const Polyhedron& volumePoly) const {
      auto sf3ds = volumePoly.findSurfacesWithIncorrectOrientation();
      if (sf3ds.empty()) {
        return {};
      }
      std::vector<std::string> sfNames;
      sfNames.reserve(sf3ds.size());
      std::transform(sf3ds.cbegin(), sf3ds.cend(), std::back_inserter(sfNames), [](const auto& sf3d) { return sf3d.name; });
      std::vector<Surface> surfaces = this->surfaces();
      surfaces.erase(std::remove_if(surfaces.begin(), surfaces.end(),
                                    [&sfNames](const auto& surface) {
                                      return std::find(sfNames.cbegin(), sfNames.cend(), surface.nameString()) == sfNames.cend();
                                    }),
                     surfaces.end());
      return surfaces;
    }

    std::vector<Surface> Space_Impl::findSurfacesWithIncorrectOrientation() const {
      if (m_cachedNonConvexSurfaces.has_value()) {
        return m_cachedNonConvexSurfaces.get();
      }
      auto volumePoly = this->polyhedron();
      // Actually, its perfectly fine to lookup incorrect orientations even if the polyhedron isn't enclosed...
      // If a Box space is missing one wall for eg, the opposite wall will be deemed incorrectly oriented if using ray casting.
      return findSurfacesWithIncorrectOrientationPolyhedron(volumePoly);
      // if (volumePoly.isEnclosedVolume()) {
      //   return findSurfacesWithIncorrectOrientationPolyhedron(volumePoly);
      // }
      // LOG(Warn, "Can't lookup surfaces with incorrect orientations for a non-enclosed Polyhedron, falling back to the Raycasting method. "
      //           "This will produce false-negatives for non-convex spaces such as H-shaped spaces.");
      // return findSurfacesWithIncorrectOrientationRaycasting();
    }

    bool Space_Impl::areAllSurfacesCorrectlyOriented() const {
      auto surfaces = findSurfacesWithIncorrectOrientation();
      for (const auto& surface : surfaces) {
        LOG(Error,
            "In Space '" << nameString() << "', Surface '" << surface.nameString() << "' has an outward normal pointing in the wrong direction.");
      }

      return surfaces.empty();
    }

    bool Space_Impl::fixSurfacesWithIncorrectOrientation() {

      auto surfaces = findSurfacesWithIncorrectOrientation();
      if (surfaces.empty()) {
        return false;
      }

      for (auto& surface : surfaces) {
        auto vertices = surface.vertices();

        LOG(Error, "In Space '" << nameString() << "', Surface '" << surface.nameString()
                                << "' has an outward normal pointing in the wrong direction. Flipping it.");
        std::reverse(vertices.begin(), vertices.end());
        surface.setVertices(openstudio::reorderULC(vertices));
      }
      return true;
    }

    double Space_Impl::volume() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::Volume, true);
      if (value) {
        return value.get();
      }

      auto volumePoly = this->polyhedron();

      if (volumePoly.isEnclosedVolume()) {
        if (volumePoly.isCompletelyInsideOut()) {
          const double volume = volumePoly.polyhedronVolume();
          LOG(Error, briefDescription() << " has all of its Surfaces that are inside-out. Call Space::fixSurfacesWithIncorrectOrientation().");
          return -volume;
        } else if (!volumePoly.hasAnySurfaceWithIncorrectOrientation()) {
          return volumePoly.polyhedronVolume();
        } else {
          LOG(Warn, briefDescription() << " has some Surfaces with incorrection orientation. Call Space::fixSurfacesWithIncorrectOrientation(). "
                                          "Falling back to ceilingHeight * floorArea. Volume calculation will be potentially inaccurate.");
        }
      } else {
        LOG(Warn, briefDescription() << " is not enclosed, there are " << volumePoly.edgesNotTwo().size()
                                     << " edges that aren't used exactly twice. "
                                        "Falling back to ceilingHeight * floorArea. Volume calculation will be potentially inaccurate.");
      }

      const double result = this->ceilingHeight() * this->floorArea();

      return result;
    }

    bool Space_Impl::isVolumeDefaulted() const {
      return isEmpty(OS_SpaceFields::Volume);
    }

    bool Space_Impl::isVolumeAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SpaceFields::Volume, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    double Space_Impl::floorArea() const {
      boost::optional<double> value = getDouble(OS_SpaceFields::FloorArea, true);
      if (value) {
        return value.get();
      }

      double result = 0;
      for (const Surface& surface : this->surfaces()) {
        if (istringEqual(surface.surfaceType(), "Floor")) {
          if (surface.isAirWall()) {
            continue;
          }
          result += surface.grossArea();
        }
      }
      return result;
    }

    bool Space_Impl::isFloorAreaDefaulted() const {
      return isEmpty(OS_SpaceFields::FloorArea);
    }

    bool Space_Impl::isFloorAreaAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_SpaceFields::FloorArea, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    double Space_Impl::numberOfPeople() const {
      double result = 0.0;
      const double area = floorArea();

      for (const People& person : this->people()) {
        result += person.getNumberOfPeople(area);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const People& person : st->people()) {
          result += person.getNumberOfPeople(area);
        }
      }

      return result;
    }

    bool Space_Impl::setNumberOfPeople(double numberOfPeople) {
      OptionalPeople templatePeople;
      PeopleVector myPeople = people();
      if (myPeople.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myPeople = st->people();
        }
      }
      if (!myPeople.empty()) {
        templatePeople = myPeople[0];
      }
      return setNumberOfPeople(numberOfPeople, templatePeople);
    }

    bool Space_Impl::setNumberOfPeople(double numberOfPeople, const boost::optional<People>& templatePeople) {
      if (numberOfPeople < 0.0) {
        LOG(Error, "Space cannot set numberOfPeople to " << numberOfPeople << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify People and PeopleDefinition object
      OptionalPeople myPeople = getMySpaceLoadInstance<People, PeopleDefinition>(templatePeople);
      if (!myPeople) {
        LOG(Error, "The templatePeople object must be in the same Model as this Space.");
        return false;
      }

      // set space and number of people
      bool ok(true);
      myPeople->makeUnique();
      ok = myPeople->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myPeople->peopleDefinition().setNumberofPeople(numberOfPeople);
      OS_ASSERT(ok);
      ok = myPeople->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other people from space
      PeopleVector allMyPeople = people();
      removeAllButOneSpaceLoadInstance<People>(allMyPeople, *myPeople);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->people().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyPeople = spaceType->people();
          for (People& peopleObject : allMyPeople) {
            peopleObject.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::peoplePerFloorArea() const {
      double result = 0.0;
      double area = floorArea();

      for (const People& person : this->people()) {
        result += person.getPeoplePerFloorArea(area);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const People& person : st->people()) {
          result += person.getPeoplePerFloorArea(area);
        }
      }

      return result;
    }

    bool Space_Impl::setPeoplePerFloorArea(double peoplePerFloorArea) {
      OptionalPeople templatePeople;
      PeopleVector myPeople = people();
      if (myPeople.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myPeople = st->people();
        }
      }
      if (!myPeople.empty()) {
        templatePeople = myPeople[0];
      }
      return setPeoplePerFloorArea(peoplePerFloorArea, templatePeople);
    }

    bool Space_Impl::setPeoplePerFloorArea(double peoplePerFloorArea, const boost::optional<People>& templatePeople) {
      if (peoplePerFloorArea < 0.0) {
        LOG(Error, "Space cannot set peoplePerFloorArea to " << peoplePerFloorArea << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify People and PeopleDefinition object
      OptionalPeople myPeople = getMySpaceLoadInstance<People, PeopleDefinition>(templatePeople);
      if (!myPeople) {
        LOG(Error, "The templatePeople object must be in the same Model as this Space.");
        return false;
      }

      // set space and number of people
      bool ok(true);
      myPeople->makeUnique();
      ok = myPeople->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myPeople->peopleDefinition().setPeopleperSpaceFloorArea(peoplePerFloorArea);
      OS_ASSERT(ok);
      ok = myPeople->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other people from space
      PeopleVector allMyPeople = people();
      removeAllButOneSpaceLoadInstance<People>(allMyPeople, *myPeople);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->people().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyPeople = spaceType->people();
          for (People& peopleObject : allMyPeople) {
            peopleObject.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::floorAreaPerPerson() const {
      return 1.0 / peoplePerFloorArea();
    }

    bool Space_Impl::setFloorAreaPerPerson(double floorAreaPerPerson) {
      OptionalPeople templatePeople;
      PeopleVector myPeople = people();
      if (myPeople.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myPeople = st->people();
        }
      }
      if (!myPeople.empty()) {
        templatePeople = myPeople[0];
      }
      return setFloorAreaPerPerson(floorAreaPerPerson, templatePeople);
    }

    bool Space_Impl::setFloorAreaPerPerson(double floorAreaPerPerson, const boost::optional<People>& templatePeople) {
      if (floorAreaPerPerson < 0.0) {
        LOG(Error, "Space cannot set floorAreaPerPerson to " << floorAreaPerPerson << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify People and PeopleDefinition object
      OptionalPeople myPeople = getMySpaceLoadInstance<People, PeopleDefinition>(templatePeople);
      if (!myPeople) {
        LOG(Error, "The templatePeople object must be in the same Model as this Space.");
        return false;
      }

      // set space and number of people
      bool ok(true);
      myPeople->makeUnique();
      ok = myPeople->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myPeople->peopleDefinition().setSpaceFloorAreaperPerson(floorAreaPerPerson);
      OS_ASSERT(ok);
      ok = myPeople->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other people from space
      PeopleVector allMyPeople = people();
      removeAllButOneSpaceLoadInstance<People>(allMyPeople, *myPeople);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->people().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyPeople = spaceType->people();
          for (People& peopleObject : allMyPeople) {
            peopleObject.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::lightingPower() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const Lights& light : lights()) {
        result += light.getLightingPower(area, numPeople);
      }
      for (const Luminaire& luminaire : luminaires()) {
        result += luminaire.lightingPower();
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const Lights& light : spaceType->lights()) {
          result += light.getLightingPower(area, numPeople);
        }
        for (const Luminaire& luminaire : spaceType->luminaires()) {
          result += luminaire.lightingPower();
        }
      }

      return result;
    }

    bool Space_Impl::setLightingPower(double lightingPower) {
      OptionalLights templateLights;
      LightsVector myLights = lights();
      if (myLights.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myLights = st->lights();
        }
      }
      if (!myLights.empty()) {
        templateLights = myLights[0];
      }
      return setLightingPower(lightingPower, templateLights);
    }

    bool Space_Impl::setLightingPower(double lightingPower, const boost::optional<Lights>& templateLights) {
      if (lightingPower < 0.0) {
        LOG(Error, "Space cannot set lightingPower to " << lightingPower << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalLights myLights = getMySpaceLoadInstance<Lights, LightsDefinition>(templateLights);
      if (!myLights) {
        LOG(Error, "The templateLights object must be in the same Model as this Space.");
        return false;
      }

      // set space and lighting power
      bool ok(true);
      myLights->makeUnique();
      ok = myLights->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myLights->lightsDefinition().setLightingLevel(lightingPower);
      OS_ASSERT(ok);
      ok = myLights->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other lights from space
      LightsVector allMyLights = lights();
      removeAllButOneSpaceLoadInstance<Lights>(allMyLights, *myLights);
      // remove all luminaires from space
      LuminaireVector allMyLuminaires = luminaires();
      for (Luminaire& luminaire : allMyLuminaires) {
        luminaire.remove();
      }

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->lights().empty() || !spaceType->luminaires().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyLights = spaceType->lights();
          for (Lights& light : allMyLights) {
            light.remove();
          }
          allMyLuminaires = spaceType->luminaires();
          for (Luminaire& luminaire : allMyLuminaires) {
            luminaire.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::lightingPowerPerFloorArea() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const Lights& light : lights()) {
        result += light.getPowerPerFloorArea(area, numPeople);
      }
      for (const Luminaire& luminaire : luminaires()) {
        result += luminaire.getPowerPerFloorArea(area);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const Lights& light : spaceType->lights()) {
          result += light.getPowerPerFloorArea(area, numPeople);
        }
        for (const Luminaire& luminaire : spaceType->luminaires()) {
          result += luminaire.getPowerPerFloorArea(area);
        }
      }

      return result;
    }

    bool Space_Impl::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea) {
      OptionalLights templateLights;
      LightsVector myLights = lights();
      if (myLights.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myLights = st->lights();
        }
      }
      if (!myLights.empty()) {
        templateLights = myLights[0];
      }
      return setLightingPowerPerFloorArea(lightingPowerPerFloorArea, templateLights);
    }

    bool Space_Impl::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, const boost::optional<Lights>& templateLights) {
      if (lightingPowerPerFloorArea < 0.0) {
        LOG(Error, "Space cannot set lightingPowerPerFloorArea to " << lightingPowerPerFloorArea << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalLights myLights = getMySpaceLoadInstance<Lights, LightsDefinition>(templateLights);
      if (!myLights) {
        LOG(Error, "The templateLights object must be in the same Model as this Space.");
        return false;
      }

      // set space and lighting power
      bool ok(true);
      myLights->makeUnique();
      ok = myLights->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myLights->lightsDefinition().setWattsperSpaceFloorArea(lightingPowerPerFloorArea);
      OS_ASSERT(ok);
      ok = myLights->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other lights from space
      LightsVector allMyLights = lights();
      removeAllButOneSpaceLoadInstance<Lights>(allMyLights, *myLights);
      // remove all luminaires from space
      LuminaireVector allMyLuminaires = luminaires();
      for (Luminaire& luminaire : allMyLuminaires) {
        luminaire.remove();
      }

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->lights().empty() || !spaceType->luminaires().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyLights = spaceType->lights();
          for (Lights& light : allMyLights) {
            light.remove();
          }
          allMyLuminaires = spaceType->luminaires();
          for (Luminaire& luminaire : allMyLuminaires) {
            luminaire.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::lightingPowerPerPerson() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const Lights& light : lights()) {
        result += light.getPowerPerPerson(area, numPeople);
      }
      for (const Luminaire& luminaire : luminaires()) {
        result += luminaire.getPowerPerPerson(numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const Lights& light : spaceType->lights()) {
          result += light.getPowerPerPerson(area, numPeople);
        }
        for (const Luminaire& luminaire : spaceType->luminaires()) {
          result += luminaire.getPowerPerPerson(numPeople);
        }
      }

      return result;
    }

    bool Space_Impl::setLightingPowerPerPerson(double lightingPowerPerPerson) {
      OptionalLights templateLights;
      LightsVector myLights = lights();
      if (myLights.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myLights = st->lights();
        }
      }
      if (!myLights.empty()) {
        templateLights = myLights[0];
      }
      return setLightingPowerPerPerson(lightingPowerPerPerson, templateLights);
    }

    bool Space_Impl::setLightingPowerPerPerson(double lightingPowerPerPerson, const boost::optional<Lights>& templateLights) {
      if (lightingPowerPerPerson < 0.0) {
        LOG(Error, "Space cannot set lightingPowerPerPerson to " << lightingPowerPerPerson << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalLights myLights = getMySpaceLoadInstance<Lights, LightsDefinition>(templateLights);
      if (!myLights) {
        LOG(Error, "The templateLights object must be in the same Model as this Space.");
        return false;
      }

      // set space and lighting power
      bool ok(true);
      myLights->makeUnique();
      ok = myLights->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myLights->lightsDefinition().setWattsperPerson(lightingPowerPerPerson);
      OS_ASSERT(ok);
      ok = myLights->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other lights from space
      LightsVector allMyLights = lights();
      removeAllButOneSpaceLoadInstance<Lights>(allMyLights, *myLights);
      // remove all luminaires from space
      LuminaireVector allMyLuminaires = luminaires();
      for (Luminaire& luminaire : allMyLuminaires) {
        luminaire.remove();
      }

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->lights().empty() || !spaceType->luminaires().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyLights = spaceType->lights();
          for (Lights& light : allMyLights) {
            light.remove();
          }
          allMyLuminaires = spaceType->luminaires();
          for (Luminaire& luminaire : allMyLuminaires) {
            luminaire.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::electricEquipmentPower() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const ElectricEquipment& equipment : electricEquipment()) {
        result += equipment.getDesignLevel(area, numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const ElectricEquipment& equipment : spaceType->electricEquipment()) {
          result += equipment.getDesignLevel(area, numPeople);
        }
      }

      return result;
    }

    double Space_Impl::electricEquipmentITEAirCooledPower() const {
      double result(0.0);
      double area = floorArea();

      for (const ElectricEquipmentITEAirCooled& iTequipment : electricEquipmentITEAirCooled()) {
        result += iTequipment.getDesignLevel(area);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const ElectricEquipmentITEAirCooled& iTequipment : spaceType->electricEquipmentITEAirCooled()) {
          result += iTequipment.getDesignLevel(area);
        }
      }

      return result;
    }

    bool Space_Impl::setElectricEquipmentPower(double electricEquipmentPower) {
      OptionalElectricEquipment templateEquipment;
      ElectricEquipmentVector myEquipment = electricEquipment();
      if (myEquipment.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myEquipment = st->electricEquipment();
        }
      }
      if (!myEquipment.empty()) {
        templateEquipment = myEquipment[0];
      }
      return setElectricEquipmentPower(electricEquipmentPower, templateEquipment);
    }

    bool Space_Impl::setElectricEquipmentPower(double electricEquipmentPower, const boost::optional<ElectricEquipment>& templateElectricEquipment) {
      if (electricEquipmentPower < 0.0) {
        LOG(Error, "Space cannot set electricEquipmentPower to " << electricEquipmentPower << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalElectricEquipment myEquipment = getMySpaceLoadInstance<ElectricEquipment, ElectricEquipmentDefinition>(templateElectricEquipment);
      if (!myEquipment) {
        LOG(Error, "The templateElectricEquipment object must be in the same Model as this Space.");
        return false;
      }

      // set space and power level
      bool ok(true);
      myEquipment->makeUnique();
      ok = myEquipment->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myEquipment->electricEquipmentDefinition().setDesignLevel(electricEquipmentPower);
      OS_ASSERT(ok);
      ok = myEquipment->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other electric equipment from space
      ElectricEquipmentVector allMyEquipment = electricEquipment();
      removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment, *myEquipment);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->electricEquipment().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyEquipment = spaceType->electricEquipment();
          for (ElectricEquipment& equipment : allMyEquipment) {
            equipment.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::electricEquipmentPowerPerFloorArea() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const ElectricEquipment& equipment : electricEquipment()) {
        result += equipment.getPowerPerFloorArea(area, numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const ElectricEquipment& equipment : spaceType->electricEquipment()) {
          result += equipment.getPowerPerFloorArea(area, numPeople);
        }
      }

      return result;
    }

    double Space_Impl::electricEquipmentITEAirCooledPowerPerFloorArea() const {
      double result(0.0);
      double area = floorArea();

      for (const ElectricEquipmentITEAirCooled& iTequipment : electricEquipmentITEAirCooled()) {
        result += iTequipment.getPowerPerFloorArea(area);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const ElectricEquipmentITEAirCooled& iTequipment : spaceType->electricEquipmentITEAirCooled()) {
          result += iTequipment.getPowerPerFloorArea(area);
        }
      }

      return result;
    }

    bool Space_Impl::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea) {
      OptionalElectricEquipment templateEquipment;
      ElectricEquipmentVector myEquipment = electricEquipment();
      if (myEquipment.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myEquipment = st->electricEquipment();
        }
      }
      if (!myEquipment.empty()) {
        templateEquipment = myEquipment[0];
      }
      return setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea, templateEquipment);
    }

    bool Space_Impl::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea,
                                                           const boost::optional<ElectricEquipment>& templateElectricEquipment) {
      if (electricEquipmentPowerPerFloorArea < 0.0) {
        LOG(Error, "Space cannot set electricEquipmentPowerPerFloorArea to " << electricEquipmentPowerPerFloorArea << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalElectricEquipment myEquipment = getMySpaceLoadInstance<ElectricEquipment, ElectricEquipmentDefinition>(templateElectricEquipment);
      if (!myEquipment) {
        LOG(Error, "The templateElectricEquipment object must be in the same Model as this Space.");
        return false;
      }

      // set space and power level
      bool ok(true);
      myEquipment->makeUnique();
      ok = myEquipment->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myEquipment->electricEquipmentDefinition().setWattsperSpaceFloorArea(electricEquipmentPowerPerFloorArea);
      OS_ASSERT(ok);
      ok = myEquipment->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other electric equipment from space
      ElectricEquipmentVector allMyEquipment = electricEquipment();
      removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment, *myEquipment);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->electricEquipment().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyEquipment = spaceType->electricEquipment();
          for (ElectricEquipment& equipment : allMyEquipment) {
            equipment.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::electricEquipmentPowerPerPerson() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const ElectricEquipment& equipment : electricEquipment()) {
        result += equipment.getPowerPerPerson(area, numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const ElectricEquipment& equipment : spaceType->electricEquipment()) {
          result += equipment.getPowerPerPerson(area, numPeople);
        }
      }

      return result;
    }

    bool Space_Impl::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson) {
      OptionalElectricEquipment templateEquipment;
      ElectricEquipmentVector myEquipment = electricEquipment();
      if (myEquipment.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myEquipment = st->electricEquipment();
        }
      }
      if (!myEquipment.empty()) {
        templateEquipment = myEquipment[0];
      }
      return setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson, templateEquipment);
    }

    bool Space_Impl::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson,
                                                        const boost::optional<ElectricEquipment>& templateElectricEquipment) {
      if (electricEquipmentPowerPerPerson < 0.0) {
        LOG(Error, "Space cannot set electricEquipmentPowerPerPerson to " << electricEquipmentPowerPerPerson << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalElectricEquipment myEquipment = getMySpaceLoadInstance<ElectricEquipment, ElectricEquipmentDefinition>(templateElectricEquipment);
      if (!myEquipment) {
        LOG(Error, "The templateElectricEquipment object must be in the same Model as this Space.");
        return false;
      }

      // set space and power level
      bool ok(true);
      myEquipment->makeUnique();
      ok = myEquipment->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myEquipment->electricEquipmentDefinition().setWattsperPerson(electricEquipmentPowerPerPerson);
      OS_ASSERT(ok);
      ok = myEquipment->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other electric equipment from space
      ElectricEquipmentVector allMyEquipment = electricEquipment();
      removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment, *myEquipment);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->electricEquipment().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyEquipment = spaceType->electricEquipment();
          for (ElectricEquipment& equipment : allMyEquipment) {
            equipment.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::gasEquipmentPower() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const GasEquipment& equipment : gasEquipment()) {
        result += equipment.getDesignLevel(area, numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const GasEquipment& equipment : spaceType->gasEquipment()) {
          result += equipment.getDesignLevel(area, numPeople);
        }
      }

      return result;
    }

    bool Space_Impl::setGasEquipmentPower(double gasEquipmentPower) {
      OptionalGasEquipment templateEquipment;
      GasEquipmentVector myEquipment = gasEquipment();
      if (myEquipment.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myEquipment = st->gasEquipment();
        }
      }
      if (!myEquipment.empty()) {
        templateEquipment = myEquipment[0];
      }
      return setGasEquipmentPower(gasEquipmentPower, templateEquipment);
    }

    bool Space_Impl::setGasEquipmentPower(double gasEquipmentPower, const boost::optional<GasEquipment>& templateGasEquipment) {
      if (gasEquipmentPower < 0.0) {
        LOG(Error, "Space cannot set gasEquipmentPower to " << gasEquipmentPower << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalGasEquipment myEquipment = getMySpaceLoadInstance<GasEquipment, GasEquipmentDefinition>(templateGasEquipment);
      if (!myEquipment) {
        LOG(Error, "The templateGasEquipment object must be in the same Model as this Space.");
        return false;
      }

      // set space and power level
      bool ok(true);
      myEquipment->makeUnique();
      ok = myEquipment->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myEquipment->gasEquipmentDefinition().setDesignLevel(gasEquipmentPower);
      OS_ASSERT(ok);
      ok = myEquipment->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other gas equipment from space
      GasEquipmentVector allMyEquipment = gasEquipment();
      removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment, *myEquipment);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->gasEquipment().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyEquipment = spaceType->gasEquipment();
          for (GasEquipment& equipment : allMyEquipment) {
            equipment.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::gasEquipmentPowerPerFloorArea() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const GasEquipment& equipment : gasEquipment()) {
        result += equipment.getPowerPerFloorArea(area, numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const GasEquipment& equipment : spaceType->gasEquipment()) {
          result += equipment.getPowerPerFloorArea(area, numPeople);
        }
      }

      return result;
    }

    bool Space_Impl::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea) {
      OptionalGasEquipment templateEquipment;
      GasEquipmentVector myEquipment = gasEquipment();
      if (myEquipment.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myEquipment = st->gasEquipment();
        }
      }
      if (!myEquipment.empty()) {
        templateEquipment = myEquipment[0];
      }
      return setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea, templateEquipment);
    }

    bool Space_Impl::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea,
                                                      const boost::optional<GasEquipment>& templateGasEquipment) {
      if (gasEquipmentPowerPerFloorArea < 0.0) {
        LOG(Error, "Space cannot set gasEquipmentPowerPerFloorArea to " << gasEquipmentPowerPerFloorArea << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalGasEquipment myEquipment = getMySpaceLoadInstance<GasEquipment, GasEquipmentDefinition>(templateGasEquipment);
      if (!myEquipment) {
        LOG(Error, "The templateGasEquipment object must be in the same Model as this Space.");
        return false;
      }

      // set space and power level
      bool ok(true);
      myEquipment->makeUnique();
      ok = myEquipment->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myEquipment->gasEquipmentDefinition().setWattsperSpaceFloorArea(gasEquipmentPowerPerFloorArea);
      OS_ASSERT(ok);
      ok = myEquipment->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other gas equipment from space
      GasEquipmentVector allMyEquipment = gasEquipment();
      removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment, *myEquipment);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->gasEquipment().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyEquipment = spaceType->gasEquipment();
          for (GasEquipment& equipment : allMyEquipment) {
            equipment.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::gasEquipmentPowerPerPerson() const {
      double result(0.0);
      double area = floorArea();
      double numPeople = numberOfPeople();

      for (const GasEquipment& equipment : gasEquipment()) {
        result += equipment.getPowerPerPerson(area, numPeople);
      }

      if (OptionalSpaceType spaceType = this->spaceType()) {
        for (const GasEquipment& equipment : spaceType->gasEquipment()) {
          result += equipment.getPowerPerPerson(area, numPeople);
        }
      }

      return result;
    }

    bool Space_Impl::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson) {
      OptionalGasEquipment templateEquipment;
      GasEquipmentVector myEquipment = gasEquipment();
      if (myEquipment.empty()) {
        if (OptionalSpaceType st = spaceType()) {
          myEquipment = st->gasEquipment();
        }
      }
      if (!myEquipment.empty()) {
        templateEquipment = myEquipment[0];
      }
      return setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson, templateEquipment);
    }

    bool Space_Impl::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson, const boost::optional<GasEquipment>& templateGasEquipment) {
      if (gasEquipmentPowerPerPerson < 0.0) {
        LOG(Error, "Space cannot set gasEquipmentPowerPerPerson to " << gasEquipmentPowerPerPerson << ", the value must be >= 0.0.");
        return false;
      }

      // create or modify Lights and LightsDefinition object
      OptionalGasEquipment myEquipment = getMySpaceLoadInstance<GasEquipment, GasEquipmentDefinition>(templateGasEquipment);
      if (!myEquipment) {
        LOG(Error, "The templateGasEquipment object must be in the same Model as this Space.");
        return false;
      }

      // set space and power level
      bool ok(true);
      myEquipment->makeUnique();
      ok = myEquipment->setSpace(getObject<Space>());
      OS_ASSERT(ok);
      ok = myEquipment->gasEquipmentDefinition().setWattsperPerson(gasEquipmentPowerPerPerson);
      OS_ASSERT(ok);
      ok = myEquipment->setMultiplier(1);
      OS_ASSERT(ok);

      // remove all other gas equipment from space
      GasEquipmentVector allMyEquipment = gasEquipment();
      removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment, *myEquipment);

      // handle space type
      if (OptionalSpaceType spaceType = this->spaceType()) {
        if (!spaceType->gasEquipment().empty()) {
          // if spaceType is used by multiple ParentObjects (includes Space, Building, excludes
          // SpaceLoadInstances), create and use clone instead of original
          if (spaceType->getModelObjectSources<ParentObject>().size() > 1u) {
            spaceType = spaceType->clone().cast<SpaceType>();
            setSpaceType(*spaceType);
          }
          allMyEquipment = spaceType->gasEquipment();
          for (GasEquipment& equipment : allMyEquipment) {
            equipment.remove();
          }
        }
      }

      return true;
    }

    double Space_Impl::infiltrationDesignFlowRate() const {
      double result(0.0);
      double floorArea = this->floorArea();
      double exteriorSurfaceArea = this->exteriorArea();
      double exteriorWallArea = this->exteriorWallArea();
      double airVolume = volume();

      for (const SpaceInfiltrationDesignFlowRate& idfr : spaceInfiltrationDesignFlowRates()) {
        result += idfr.getDesignFlowRate(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const SpaceInfiltrationDesignFlowRate& idfr : st->spaceInfiltrationDesignFlowRates()) {
          result += idfr.getDesignFlowRate(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
        }
      }

      return result;
    }

    double Space_Impl::infiltrationDesignFlowPerSpaceFloorArea() const {
      double result(0.0);
      double floorArea = this->floorArea();
      double exteriorSurfaceArea = this->exteriorArea();
      double exteriorWallArea = this->exteriorWallArea();
      double airVolume = volume();

      for (const SpaceInfiltrationDesignFlowRate& idfr : spaceInfiltrationDesignFlowRates()) {
        result += idfr.getFlowPerSpaceFloorArea(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const SpaceInfiltrationDesignFlowRate& idfr : st->spaceInfiltrationDesignFlowRates()) {
          result += idfr.getFlowPerSpaceFloorArea(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
        }
      }

      return result;
    }

    double Space_Impl::infiltrationDesignFlowPerExteriorSurfaceArea() const {
      double result(0.0);
      double floorArea = this->floorArea();
      double exteriorSurfaceArea = this->exteriorArea();
      double exteriorWallArea = this->exteriorWallArea();
      double airVolume = volume();

      for (const SpaceInfiltrationDesignFlowRate& idfr : spaceInfiltrationDesignFlowRates()) {
        result += idfr.getFlowPerExteriorSurfaceArea(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const SpaceInfiltrationDesignFlowRate& idfr : st->spaceInfiltrationDesignFlowRates()) {
          result += idfr.getFlowPerExteriorSurfaceArea(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
        }
      }

      return result;
    }

    double Space_Impl::infiltrationDesignFlowPerExteriorWallArea() const {
      double result(0.0);
      double floorArea = this->floorArea();
      double exteriorSurfaceArea = this->exteriorArea();
      double exteriorWallArea = this->exteriorWallArea();
      double airVolume = volume();

      for (const SpaceInfiltrationDesignFlowRate& idfr : spaceInfiltrationDesignFlowRates()) {
        result += idfr.getFlowPerExteriorWallArea(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const SpaceInfiltrationDesignFlowRate& idfr : st->spaceInfiltrationDesignFlowRates()) {
          result += idfr.getFlowPerExteriorWallArea(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
        }
      }

      return result;
    }

    double Space_Impl::infiltrationDesignAirChangesPerHour() const {
      double result(0.0);
      double floorArea = this->floorArea();
      double exteriorSurfaceArea = this->exteriorArea();
      double exteriorWallArea = this->exteriorWallArea();
      double airVolume = volume();

      for (const SpaceInfiltrationDesignFlowRate& idfr : spaceInfiltrationDesignFlowRates()) {
        result += idfr.getAirChangesPerHour(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
      }

      if (OptionalSpaceType st = spaceType()) {
        for (const SpaceInfiltrationDesignFlowRate& idfr : st->spaceInfiltrationDesignFlowRates()) {
          result += idfr.getAirChangesPerHour(floorArea, exteriorSurfaceArea, exteriorWallArea, airVolume);
        }
      }

      return result;
    }

    void Space_Impl::hardApplySpaceType(bool hardSizeLoads) {
      Model model = this->model();
      auto space = this->getObject<Space>();

      std::string plenumSpaceTypeName = model.plenumSpaceTypeName();

      boost::optional<SpaceType> spaceType = this->spaceType();
      if (spaceType) {

        // Clone the space type if there is a space type
        spaceType = spaceType->clone(model).cast<SpaceType>();

        // Hard apply schedules to space type loads if there is a space type
        spaceType->hardApplySpaceLoadSchedules();

        // Move space loads from space type to space if there is a space type
        for (const ModelObject& child : spaceType->children()) {
          if (auto load_ = child.optionalCast<SpaceLoad>()) {
            bool test = load_->setSpace(space);
            OS_ASSERT(test);
          }
        }

        // Merge space type default constructions with space default constructions if there is a space type
        boost::optional<DefaultConstructionSet> spaceTypeDefaultConstructionSet = spaceType->defaultConstructionSet();
        if (spaceTypeDefaultConstructionSet) {
          boost::optional<DefaultConstructionSet> defaultConstructionSet = this->defaultConstructionSet();
          if (defaultConstructionSet) {
            defaultConstructionSet->merge(*spaceTypeDefaultConstructionSet);
          } else {
            this->setDefaultConstructionSet(*spaceTypeDefaultConstructionSet);
          }
        }

      } else {
        // create a new space type
        spaceType = SpaceType(model);
      }

      // always set space type to prevent picking up building level space type
      space.setSpaceType(*spaceType);

      // Hard apply schedules to space loads, do this before clearing space type default schedule set
      this->hardApplySpaceLoadSchedules();

      // If hard sizing loads, make each space load refer to unique definition and hard size it
      if (hardSizeLoads) {
        for (const ModelObject& child : this->children()) {
          if (auto load_ = child.optionalCast<SpaceLoad>()) {
            load_->hardSize();
          }
        }
      }

      // clean the cloned space type
      spaceType->resetRenderingColor();
      spaceType->resetDefaultConstructionSet();
      spaceType->resetDefaultScheduleSet();
    }

    void Space_Impl::hardApplySpaceLoadSchedules() {
      for (const ModelObject& child : this->children()) {
        if (auto load_ = child.optionalCast<SpaceLoad>()) {
          load_->hardApplySchedules();
        }
      }
    }

    void Space_Impl::hardApplyConstructions() {

      for (const ShadingSurfaceGroup& shadingSurfaceGroup : this->shadingSurfaceGroups()) {
        for (ShadingSurface& shadingSurface : shadingSurfaceGroup.shadingSurfaces()) {
          if (boost::optional<ConstructionBase> construction = shadingSurface.construction()) {
            shadingSurface.setConstruction(*construction);
          }
        }
      }

      for (const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup : this->interiorPartitionSurfaceGroups()) {
        for (InteriorPartitionSurface& interiorPartitionSurface : interiorPartitionSurfaceGroup.interiorPartitionSurfaces()) {
          if (boost::optional<ConstructionBase> construction = interiorPartitionSurface.construction()) {
            interiorPartitionSurface.setConstruction(*construction);
          }
        }
      }

      for (Surface& surface : this->surfaces()) {
        boost::optional<ConstructionBase> construction = surface.construction();
        if (construction) {
          surface.setConstruction(*construction);
        } else {
          if (boost::optional<Surface> adjacentSurface = surface.adjacentSurface()) {
            construction = adjacentSurface->construction();
            if (construction) {
              surface.setConstruction(*construction);
            }
          }
        }

        for (SubSurface& subSurface : surface.subSurfaces()) {
          boost::optional<ConstructionBase> construction = subSurface.construction();
          if (construction) {
            subSurface.setConstruction(*construction);
          } else {
            if (boost::optional<SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface()) {
              construction = adjacentSubSurface->construction();
              if (construction) {
                subSurface.setConstruction(*construction);
              }
            }
          }
        }
      }
    }

    void Space_Impl::unmatchSurfaces() {
      for (Surface& surface : this->surfaces()) {
        boost::optional<Surface> adjacentSurface = surface.adjacentSurface();
        if (adjacentSurface) {
          surface.resetAdjacentSurface();
          adjacentSurface->resetAdjacentSurface();
        }

        for (SubSurface& subSurface : surface.subSurfaces()) {
          boost::optional<SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface();
          if (adjacentSubSurface) {
            subSurface.resetAdjacentSubSurface();
            adjacentSubSurface->resetAdjacentSubSurface();
          }
        }
      }
    }

    void Space_Impl::matchSurfaces(Space& other) {
      double tol = 0.01;

      if (this->handle() == other.handle()) {
        return;
      }

      // transform from other to this coordinates
      Transformation transformation = this->transformation().inverse() * other.transformation();

      for (Surface& surface : this->surfaces()) {
        if (surface.adjacentSurface()) {
          continue;
        }
        std::vector<Point3d> vertices = surface.vertices();
        boost::optional<Vector3d> outwardNormal = getOutwardNormal(vertices);
        if (!outwardNormal) {
          continue;
        }

        for (Surface& otherSurface : other.surfaces()) {
          if (otherSurface.adjacentSurface()) {
            continue;
          }
          std::vector<Point3d> otherVertices = transformation * otherSurface.vertices();
          boost::optional<Vector3d> otherOutwardNormal = getOutwardNormal(otherVertices);
          if (!otherOutwardNormal) {
            continue;
          }

          double dot = outwardNormal->dot(*otherOutwardNormal);

          if (dot > -0.98) {
            continue;
          }

          std::reverse(otherVertices.begin(), otherVertices.end());

          if (circularEqual(vertices, otherVertices, tol)) {

            // TODO: check constructions?
            surface.setAdjacentSurface(otherSurface);
            otherSurface.setAdjacentSurface(surface);

            // once surfaces are matched, check subsurfaces
            for (SubSurface& subSurface : surface.subSurfaces()) {

              vertices = removeCollinear(subSurface.vertices());

              for (SubSurface& otherSubSurface : otherSurface.subSurfaces()) {

                otherVertices = removeCollinear(transformation * otherSubSurface.vertices());
                std::reverse(otherVertices.begin(), otherVertices.end());

                if (circularEqual(vertices, otherVertices, tol)) {

                  // TODO: check constructions?
                  subSurface.setAdjacentSubSurface(otherSubSurface);
                  otherSubSurface.setAdjacentSubSurface(subSurface);
                }
              }
            }
          }
        }
      }
    }

    void Space_Impl::intersectSurfaces(Space& other) {
      if (this->handle() == other.handle()) {
        return;
      }

      std::string name = nameString();
      std::string otherName = other.nameString();
      LOG(Debug, "Intersecting space " << name << " with space " << otherName);

      std::vector<Surface> surfaces = this->surfaces();
      std::vector<Surface> otherSurfaces = other.surfaces();

      std::sort(surfaces.begin(), surfaces.end(), [](const Surface& a, const Surface& b) -> bool { return a.grossArea() > b.grossArea(); });
      std::sort(otherSurfaces.begin(), otherSurfaces.end(), [](const Surface& a, const Surface& b) -> bool { return a.grossArea() > b.grossArea(); });

      std::map<std::string, bool> hasSubSurfaceMap;
      std::map<std::string, bool> hasAdjacentSurfaceMap;
      std::set<std::string> completedIntersections;

      bool anyNewSurfaces = true;
      while (anyNewSurfaces) {

        anyNewSurfaces = false;
        std::vector<Surface> newSurfaces;
        std::vector<Surface> newOtherSurfaces;

        for (Surface& surface : surfaces) {
          std::string surfaceHandle = toString(surface.handle());

          if (hasSubSurfaceMap.find(surfaceHandle) == hasSubSurfaceMap.end()) {
            hasSubSurfaceMap[surfaceHandle] = !surface.subSurfaces().empty();
            hasAdjacentSurfaceMap[surfaceHandle] = surface.adjacentSurface().has_value();
          }

          if (hasSubSurfaceMap[surfaceHandle] || hasAdjacentSurfaceMap[surfaceHandle]) {
            continue;
          }

          for (Surface& otherSurface : otherSurfaces) {
            std::string otherSurfaceHandle = toString(otherSurface.handle());

            if (hasSubSurfaceMap.find(otherSurfaceHandle) == hasSubSurfaceMap.end()) {
              hasSubSurfaceMap[otherSurfaceHandle] = !otherSurface.subSurfaces().empty();
              hasAdjacentSurfaceMap[otherSurfaceHandle] = otherSurface.adjacentSurface().has_value();
            }

            if (hasSubSurfaceMap[otherSurfaceHandle] || hasAdjacentSurfaceMap[otherSurfaceHandle]) {
              continue;
            }

            // see if we have already tested these for intersection,
            // surfaces that previously did not intersect will not intersect if vertices change
            // surfaces that previously did intersect will intersect exactly
            std::string intersectionKey = surfaceHandle + otherSurfaceHandle;
            if (completedIntersections.find(intersectionKey) != completedIntersections.end()) {
              continue;
            }
            completedIntersections.insert(intersectionKey);

            // number of surfaces in each space will only increase in intersect
            boost::optional<SurfaceIntersection> intersection = surface.computeIntersection(otherSurface);
            if (intersection) {
              std::vector<Surface> newSurfaces1 = intersection->newSurfaces1();
              std::vector<Surface> newSurfaces2 = intersection->newSurfaces2();

              // surfaces involved in this intersection are ineligible to be re-intersected with other surfaces in this intersection
              std::vector<Surface> ineligibleSurfaces;
              ineligibleSurfaces.reserve(newSurfaces1.size() + 1);
              ineligibleSurfaces.push_back(surface);
              ineligibleSurfaces.insert(ineligibleSurfaces.end(), newSurfaces1.begin(), newSurfaces1.end());

              std::vector<Surface> ineligibleOtherSurfaces;
              ineligibleOtherSurfaces.reserve(newSurfaces2.size() + 1);
              ineligibleOtherSurfaces.push_back(otherSurface);
              ineligibleOtherSurfaces.insert(ineligibleOtherSurfaces.end(), newSurfaces2.begin(), newSurfaces2.end());
              for (Surface& ineligibleSurface : ineligibleSurfaces) {
                for (Surface& ineligibleOtherSurface : ineligibleOtherSurfaces) {
                  std::string ineligibleIntersectionKey = toString(ineligibleSurface.handle()) + toString(ineligibleOtherSurface.handle());
                  completedIntersections.insert(ineligibleIntersectionKey);
                }
              }

              newSurfaces.reserve(newSurfaces.size() + newSurfaces1.size());
              newSurfaces.insert(newSurfaces.end(), std::make_move_iterator(newSurfaces1.begin()), std::make_move_iterator(newSurfaces1.end()));
              newOtherSurfaces.reserve(newOtherSurfaces.size() + newSurfaces2.size());
              newOtherSurfaces.insert(newOtherSurfaces.end(), std::make_move_iterator(newSurfaces2.begin()),
                                      std::make_move_iterator(newSurfaces2.end()));
            }
          }
        }

        if (!newSurfaces.empty()) {
          surfaces.insert(surfaces.end(), newSurfaces.begin(), newSurfaces.end());
          anyNewSurfaces = true;
        }
        if (!newOtherSurfaces.empty()) {
          otherSurfaces.insert(otherSurfaces.end(), newOtherSurfaces.begin(), newOtherSurfaces.end());
          anyNewSurfaces = true;
        }
      }
    }

    std::vector<Surface> Space_Impl::findSurfaces(boost::optional<double> minDegreesFromNorth, boost::optional<double> maxDegreesFromNorth,
                                                  boost::optional<double> minDegreesTilt, boost::optional<double> maxDegreesTilt, double tol) {

      std::vector<PlanarSurface> planarSurfaces;
      auto thisSurfaces = this->surfaces();
      planarSurfaces.reserve(thisSurfaces.size());
      planarSurfaces.insert(planarSurfaces.end(), std::make_move_iterator(thisSurfaces.begin()), std::make_move_iterator(thisSurfaces.end()));

      planarSurfaces =
        PlanarSurface::findPlanarSurfaces(planarSurfaces, minDegreesFromNorth, maxDegreesFromNorth, minDegreesTilt, maxDegreesTilt, tol);

      std::vector<Surface> result;
      result.reserve(planarSurfaces.size());
      for (const PlanarSurface& planarSurface : planarSurfaces) {
        result.emplace_back(planarSurface.cast<Surface>());
      }

      return result;
    }

    boost::optional<ModelObject> Space_Impl::spaceTypeAsModelObject() const {
      OptionalModelObject result;
      OptionalSpaceType intermediate = spaceType();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> Space_Impl::defaultConstructionSetAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultConstructionSet intermediate = defaultConstructionSet();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> Space_Impl::defaultScheduleSetAsModelObject() const {
      OptionalModelObject result;
      OptionalDefaultScheduleSet intermediate = defaultScheduleSet();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> Space_Impl::thermalZoneAsModelObject() const {
      OptionalModelObject result;
      OptionalThermalZone intermediate = thermalZone();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> Space_Impl::buildingStoryAsModelObject() const {
      OptionalModelObject result;
      OptionalBuildingStory intermediate = buildingStory();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    std::vector<ModelObject> Space_Impl::shadingSurfaceGroupsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(shadingSurfaceGroups());
      return result;
    }

    std::vector<ModelObject> Space_Impl::interiorPartitionSurfaceGroupsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(interiorPartitionSurfaceGroups());
      return result;
    }

    std::vector<ModelObject> Space_Impl::surfacesAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(surfaces());
      return result;
    }

    std::vector<ModelObject> Space_Impl::internalMassAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(internalMass());
      return result;
    }

    std::vector<ModelObject> Space_Impl::peopleAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(people());
      return result;
    }

    std::vector<ModelObject> Space_Impl::lightsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(lights());
      return result;
    }

    std::vector<ModelObject> Space_Impl::luminairesAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(luminaires());
      return result;
    }

    std::vector<ModelObject> Space_Impl::electricEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(electricEquipment());
      return result;
    }

    std::vector<ModelObject> Space_Impl::electricEquipmentITEAirCooledAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(electricEquipmentITEAirCooled());
      return result;
    }

    std::vector<ModelObject> Space_Impl::gasEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(gasEquipment());
      return result;
    }

    std::vector<ModelObject> Space_Impl::hotWaterEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(hotWaterEquipment());
      return result;
    }

    std::vector<ModelObject> Space_Impl::steamEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(steamEquipment());
      return result;
    }

    std::vector<ModelObject> Space_Impl::otherEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(otherEquipment());
      return result;
    }

    std::vector<ModelObject> Space_Impl::daylightingControlsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(daylightingControls());
      return result;
    }

    std::vector<ModelObject> Space_Impl::illuminanceMapsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(illuminanceMaps());
      return result;
    }

    std::vector<ModelObject> Space_Impl::spaceInfiltrationDesignFlowRatesAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(spaceInfiltrationDesignFlowRates());
      return result;
    }

    std::vector<ModelObject> Space_Impl::spaceInfiltrationEffectiveLeakageAreasAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(spaceInfiltrationEffectiveLeakageAreas());
      return result;
    }

    std::vector<ModelObject> Space_Impl::spaceInfiltrationFlowCoefficientsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(spaceInfiltrationFlowCoefficients());
      return result;
    }

    bool Space_Impl::setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSpaceType intermediate = modelObject->optionalCast<SpaceType>();
        if (intermediate) {
          return setSpaceType(*intermediate);
        } else {
          return false;
        }
      } else {
        resetSpaceType();
      }
      return true;
    }

    bool Space_Impl::setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultConstructionSet intermediate = modelObject->optionalCast<DefaultConstructionSet>();
        if (intermediate) {
          return setDefaultConstructionSet(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultConstructionSet();
      }
      return true;
    }

    bool Space_Impl::setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalDefaultScheduleSet intermediate = modelObject->optionalCast<DefaultScheduleSet>();
        if (intermediate) {
          return setDefaultScheduleSet(*intermediate);
        } else {
          return false;
        }
      } else {
        resetDefaultScheduleSet();
      }
      return true;
    }

    bool Space_Impl::setThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
        if (intermediate) {
          return setThermalZone(*intermediate);
        }
      }
      return false;
    }

    bool Space_Impl::setBuildingStoryAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalBuildingStory intermediate = modelObject->optionalCast<BuildingStory>();
        if (intermediate) {
          return setBuildingStory(*intermediate);
        }
      }
      return false;
    }

    template <typename T, typename TDef>
    boost::optional<T> Space_Impl::getMySpaceLoadInstance(const boost::optional<T>& templateSpaceLoadInstance) {
      boost::optional<T> result;
      if (!templateSpaceLoadInstance) {
        TDef myDefinition(model());
        T myInstance(myDefinition);
        result = myInstance;
      } else {
        boost::optional<Space> templateSpace = templateSpaceLoadInstance->space();
        if (templateSpace && (templateSpace.get() == getObject<Space>())) {
          result = templateSpaceLoadInstance;
        } else if (templateSpaceLoadInstance->model() != model()) {
          return boost::none;
        } else if (!templateSpace && !templateSpaceLoadInstance->spaceType()) {
          result = templateSpaceLoadInstance;
        } else {
          T myInstance = templateSpaceLoadInstance->clone().template cast<T>();
          result = myInstance;
        }
      }
      return result;
    }

    template <typename T>
    void Space_Impl::removeAllButOneSpaceLoadInstance(std::vector<T>& instances, const T& instanceToKeep) {
      int count(0);
      for (auto& instance : instances) {
        if (instance == instanceToKeep) {
          ++count;
          continue;
        }
        instance.remove();
      }
      OS_ASSERT(count == 1);
    }

    std::vector<Point3d> Space_Impl::floorPrint() const {
      double tol = 0.01;  // 1 cm tolerance

      // get all surfaces, sort so results are repeatable
      std::vector<Surface> surfaces = this->surfaces();
      std::sort(surfaces.begin(), surfaces.end(), IdfObjectNameLess());

      // find all floors
      boost::optional<double> z;
      std::vector<Surface> floors;
      for (const Surface& surface : surfaces) {
        if (surface.vertices().size() < 3) {
          LOG(Warn, "Skipping floor with fewer than 3 vertices");
          continue;
        }
        if (istringEqual("Floor", surface.surfaceType())) {
          floors.push_back(surface);
          for (const Point3d& point : surface.vertices()) {
            if (!z) {
              z = point.z();
            } else if (std::abs(z.get() - point.z()) > tol) {
              LOG(Error, "All floor surfaces must lie on the same x, y plane to compute space floor print");
              return {};
            }
          }
        }
      }

      std::vector<Point3d> result;
      std::vector<Point3d> allPoints;

      if (floors.empty()) {
        LOG(Error, "No floor surfaces found to compute space floor print");
        return {};

      } else if (floors.size() == 1) {
        // just return this floors vertices
        result = floors[0].vertices();

        // remove collinear points
        result = removeCollinearLegacy(result);

        // reorder the points
        result = reorderULC(result);

        // remove additional collinear points that occur after reordering
        result = removeCollinearLegacy(result);

      } else {

        OS_ASSERT(z);

        using BoostPoint = boost::geometry::model::d2::point_xy<double>;
        using BoostPolygon = boost::geometry::model::polygon<BoostPoint>;
        using BoostRing = boost::geometry::model::ring<BoostPoint>;
        using BoostMultiPolygon = boost::geometry::model::multi_polygon<BoostPolygon>;

        BoostMultiPolygon boostResult;

        try {
          for (const Surface& floor : floors) {
            BoostPolygon boostPolygon;
            std::vector<Point3d> vertices = floor.vertices();
            for (const Point3d& point : vertices) {
              boost::geometry::append(boostPolygon, point3dToTuple(point, allPoints, tol));
            }
            boost::geometry::append(boostPolygon, point3dToTuple(vertices[0], allPoints, tol));

            BoostMultiPolygon tmpResult;
            boost::geometry::union_(boostResult, boostPolygon, tmpResult);
            boostResult = tmpResult;
          }
        } catch (const std::exception& e) {
          LOG(Error, e.what());
          boostResult.clear();
        }

        if (boostResult.size() != 1) {
          return {};
        }

        for (const BoostPoint& boostPoint : boostResult[0].outer()) {
          Point3d point(boostPoint.x(), boostPoint.y(), *z);
          result.push_back(point);
        }
        result.pop_back();

        // remove collinear points
        result = removeCollinearLegacy(result);

        // reorder the points
        result = reorderULC(result);

        // remove additional collinear points that occur after reordering
        result = removeCollinearLegacy(result);

        // if result is now empty just quit
        if (result.size() < 3) {
          return {};
        }

        Point3d lastOuterVertex = result.back();

        for (const BoostRing& boostInner : boostResult[0].inners()) {

          std::vector<Point3d> innerLoop;
          for (auto boostPoint = boostInner.rbegin(); boostPoint != boostInner.rend(); ++boostPoint) {
            Point3d point(boostPoint->x(), boostPoint->y(), *z);
            innerLoop.push_back(point);
          }

          // remove collinear points
          innerLoop = removeCollinearLegacy(innerLoop);

          // reorder the points
          innerLoop = reorderULC(innerLoop);

          // remove additional collinear points that occur after reordering
          innerLoop = removeCollinearLegacy(innerLoop);

          // if inner loop is now empty just ignore it
          if (innerLoop.size() < 3) {
            continue;
          }

          // reverse the inner loop
          std::reverse(innerLoop.begin(), innerLoop.end());

          // close the inner loop
          innerLoop.push_back(innerLoop.front());

          // insert the inner loop
          result.insert(result.end(), innerLoop.begin(), innerLoop.end());

          // add the last point back in
          result.push_back(lastOuterVertex);
        }
      }

      // remove collinear points
      result = removeCollinearLegacy(result);

      // reorder the points
      result = reorderULC(result);

      // remove additional collinear points that occur after reordering
      result = removeCollinearLegacy(result);

      // if result is now empty just quit
      if (result.size() < 3) {
        return {};
      }

      return result;
    }

    bool Space_Impl::isConvex() const {
      if (m_cachedIsConvex.has_value()) {
        return m_cachedIsConvex.get();
      }
      auto points = floorPrint();
      if (points.empty()) {
        LOG(Warn, "Can't compute a floorPrint for " << briefDescription());
        return false;
      }
      Surface3d sf3d(points, fmt::format("{} floorPrint", nameString()), 0);
      return sf3d.isConvex();
    }

    std::vector<Surface> Space_Impl::findNonConvexSurfaces() const {
      auto surfaces = this->surfaces();
      surfaces.erase(std::remove_if(surfaces.begin(), surfaces.end(), [](const auto& surface) { return surface.isConvex(); }), surfaces.end());
      return surfaces;
    }

    void Space_Impl::cacheGeometryDiagnostics() {
      m_cachedIsConvex = isConvex();
      auto volumePoly = this->polyhedron();
      m_cachedIsEnclosed = volumePoly.isEnclosedVolume();
      // Actually, its perfectly fine to lookup incorrect orientations even if the polyhedron isn't enclosed...
      // If a Box space is missing one wall for eg, the opposite wall will be deemed incorrectly oriented if using ray casting.
      m_cachedNonConvexSurfaces = findSurfacesWithIncorrectOrientationPolyhedron(volumePoly);
    }

    void Space_Impl::resetCachedGeometryDiagnostics() {
      m_cachedNonConvexSurfaces.reset();
      m_cachedIsConvex.reset();
      m_cachedIsEnclosed.reset();
    }

    bool Space_Impl::isPlenum() const {
      bool result = false;
      boost::optional<ThermalZone> thermalZone = this->thermalZone();
      if (thermalZone) {
        result = thermalZone->isPlenum();
      }
      return result;
    }

    double Space_Impl::exposedPerimeter(const Polygon3d& buildingPerimeter) const {
      Transformation tr = transformation();

      double perimeter = 0;
      for (const auto& surface : surfaces()) {
        if (surface.surfaceType() == "Floor" && surface.outsideBoundaryCondition() == "Ground") {
          auto vertices = surface.vertices();
          if (!vertices.empty() && vertices[0].z() == 0.0) {
            vertices = tr * vertices;
            for (size_t i = 0; i < vertices.size(); i++) {
              Point3dVector line;
              line.push_back(vertices[i]);
              line.push_back(vertices[(i + 1) % vertices.size()]);
              Point3dVectorVector overlaps = buildingPerimeter.overlap(line);
              for (const auto& overlap : overlaps) {
                perimeter += openstudio::getDistance(overlap[0], overlap[1]);
              }
            }
          }
        }
      }
      return perimeter;
    }

    // helper function to get a boost polygon point from a Point3d
    boost::tuple<double, double> Space_Impl::point3dToTuple(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol) const {
      // simple method
      //return boost::make_tuple(point3d.x(), point3d.y());

      // detailed method, try to combine points within tolerance
      for (const Point3d& otherPoint : allPoints) {
        if (std::sqrt(std::pow(point3d.x() - otherPoint.x(), 2) + std::pow(point3d.y() - otherPoint.y(), 2)) < tol) {
          return boost::make_tuple(otherPoint.x(), otherPoint.y());
        }
      }
      allPoints.push_back(point3d);
      return boost::make_tuple(point3d.x(), point3d.y());
    }

    std::vector<ZoneMixing> Space_Impl::zoneMixing() const {
      return getObject<ModelObject>().getModelObjectSources<ZoneMixing>();
    }

    std::vector<ZoneMixing> Space_Impl::supplyZoneMixing() const {
      std::vector<ZoneMixing> result = this->zoneMixing();

      Handle handle = this->handle();
      auto new_end =
        std::remove_if(result.begin(), result.end(), [&](const ZoneMixing& mixing) { return (mixing.zoneOrSpace().handle() != handle); });

      result.erase(new_end, result.end());
      return result;
    }

    std::vector<ZoneMixing> Space_Impl::exhaustZoneMixing() const {
      std::vector<ZoneMixing> result = this->zoneMixing();

      Handle handle = this->handle();
      auto new_end = std::remove_if(result.begin(), result.end(), [&](const ZoneMixing& mixing) {
        return (!mixing.sourceZoneOrSpace() || (mixing.sourceZoneOrSpace()->handle() != handle));
      });

      result.erase(new_end, result.end());
      return result;
    }

  }  // namespace detail

  Space::Space(const Model& model) : PlanarSurfaceGroup(Space::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Space_Impl>());
  }

  IddObjectType Space::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Space);
    return result;
  }

  boost::optional<Space> Space::fromFloorPrint(const std::vector<Point3d>& floorPrint, double floorHeight, Model& model,
                                               const std::string& spaceName) {
    // check floor height
    if (floorHeight <= 0) {
      LOG(Error, "Cannot create a space with floorHeight " << floorHeight << ".");
      return boost::none;
    }

    // check floor print
    unsigned numPoints = floorPrint.size();
    if (numPoints < 3) {
      LOG(Error, "Cannot create a space for floorPrint of size " << floorPrint.size() << ".");
      return boost::none;
    }

    double z = floorPrint[0].z();
    double tol = 0.000001;
    for (const Point3d& point : floorPrint) {
      if (std::abs(point.z() - z) > tol) {
        LOG(Error, "Inconsistent z height in floorPrint.");
        return boost::none;
      }
    }

    // Enforce the same z
    std::vector<Point3d> reorderedFloorPrint;
    reorderedFloorPrint.reserve(floorPrint.size());
    std::transform(floorPrint.cbegin(), floorPrint.cend(), std::back_inserter(reorderedFloorPrint),
                   [&z](const auto& pt) { return Point3d{pt.x(), pt.y(), z}; });

    boost::optional<Vector3d> outwardNormal = getOutwardNormal(reorderedFloorPrint);
    if (!outwardNormal) {
      LOG(Error, "Cannot compute outwardNormal for floorPrint.");
      return boost::none;
    }

    if (outwardNormal->z() > -1 + tol) {
      LOG(Error, "OutwardNormal of floorPrint must point down to create space.");
      return boost::none;
    }

    // reorderedFloorPrint = openstudio::reorderULC(reorderedFloorPrint);

    // we are good to go, create the space
    Space space(model);

    // create the floor
    Surface floor(reorderedFloorPrint, model);
    if (!spaceName.empty()) {
      space.setName(spaceName);
      floor.setName(fmt::format("{} Floor", space.nameString()));
    }
    floor.setSpace(space);

    double zCeiling = z + floorHeight;
    std::vector<Point3d> points;
    points.reserve(4);
    // create each wall
    for (unsigned i = 1; i <= numPoints; ++i) {
      // Counter clockwise, Upper Left Corner convention
      points = {
        {reorderedFloorPrint[i - 1].x(), reorderedFloorPrint[i - 1].y(), zCeiling},                  // Upper Left Corner
        {reorderedFloorPrint[i % numPoints].x(), reorderedFloorPrint[i % numPoints].y(), zCeiling},  // Upper Right Corner
        {reorderedFloorPrint[i % numPoints].x(), reorderedFloorPrint[i % numPoints].y(), z},         // Lower Right Corner
        {reorderedFloorPrint[i - 1].x(), reorderedFloorPrint[i - 1].y(), z},                         // Lower Left Corner
      };

      Surface wall(points, model);
      if (!spaceName.empty()) {
        wall.setName(fmt::format("{} Wall {}", space.nameString(), i));
      }

      wall.setSpace(space);
    }

    // create the roofCeiling
    std::vector<Point3d> ceilingPoints;
    ceilingPoints.reserve(reorderedFloorPrint.size());
    std::transform(reorderedFloorPrint.crbegin(), reorderedFloorPrint.crend(), std::back_inserter(ceilingPoints),
                   [zCeiling](const auto& pt) { return Point3d{pt.x(), pt.y(), zCeiling}; });
    Surface roofCeiling(ceilingPoints, model);
    if (!spaceName.empty()) {
      roofCeiling.setName(fmt::format("{} RoofCeiling", space.nameString()));
    }
    roofCeiling.setSpace(space);

    return space;
  }

  bool Space::partofTotalFloorArea() const {
    return getImpl<detail::Space_Impl>()->partofTotalFloorArea();
  }

  bool Space::isPartofTotalFloorAreaDefaulted() const {
    return getImpl<detail::Space_Impl>()->isPartofTotalFloorAreaDefaulted();
  }

  bool Space::setPartofTotalFloorArea(bool partofTotalFloorArea) {
    return getImpl<detail::Space_Impl>()->setPartofTotalFloorArea(partofTotalFloorArea);
  }

  void Space::resetPartofTotalFloorArea() {
    getImpl<detail::Space_Impl>()->resetPartofTotalFloorArea();
  }

  bool Space::setCeilingHeight(double ceilingHeight) {
    return getImpl<detail::Space_Impl>()->setCeilingHeight(ceilingHeight);
  }

  void Space::autocalculateCeilingHeight() {
    getImpl<detail::Space_Impl>()->autocalculateCeilingHeight();
  }

  void Space::resetCeilingHeight() {
    getImpl<detail::Space_Impl>()->resetCeilingHeight();
  }

  bool Space::setVolume(double volume) {
    return getImpl<detail::Space_Impl>()->setVolume(volume);
  }

  void Space::autocalculateVolume() {
    getImpl<detail::Space_Impl>()->autocalculateVolume();
  }

  void Space::resetVolume() {
    getImpl<detail::Space_Impl>()->resetVolume();
  }

  bool Space::setFloorArea(double floorArea) {
    return getImpl<detail::Space_Impl>()->setFloorArea(floorArea);
  }

  void Space::autocalculateFloorArea() {
    getImpl<detail::Space_Impl>()->autocalculateFloorArea();
  }

  void Space::resetFloorArea() {
    getImpl<detail::Space_Impl>()->resetFloorArea();
  }

  boost::optional<SpaceType> Space::spaceType() const {
    return getImpl<detail::Space_Impl>()->spaceType();
  }

  bool Space::isSpaceTypeDefaulted() const {
    return getImpl<detail::Space_Impl>()->isSpaceTypeDefaulted();
  }

  bool Space::setSpaceType(const SpaceType& spaceType) {
    return getImpl<detail::Space_Impl>()->setSpaceType(spaceType);
  }

  void Space::resetSpaceType() {
    getImpl<detail::Space_Impl>()->resetSpaceType();
  }

  boost::optional<DefaultConstructionSet> Space::defaultConstructionSet() const {
    return getImpl<detail::Space_Impl>()->defaultConstructionSet();
  }

  boost::optional<ConstructionBase> Space::getDefaultConstruction(const PlanarSurface& planarSurface) const {
    return getImpl<detail::Space_Impl>()->getDefaultConstruction(planarSurface);
  }

  boost::optional<std::pair<ConstructionBase, int>> Space::getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const {
    return getImpl<detail::Space_Impl>()->getDefaultConstructionWithSearchDistance(planarSurface);
  }

  bool Space::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet) {
    return getImpl<detail::Space_Impl>()->setDefaultConstructionSet(defaultConstructionSet);
  }

  void Space::resetDefaultConstructionSet() {
    getImpl<detail::Space_Impl>()->resetDefaultConstructionSet();
  }

  boost::optional<DefaultScheduleSet> Space::defaultScheduleSet() const {
    return getImpl<detail::Space_Impl>()->defaultScheduleSet();
  }

  boost::optional<Schedule> Space::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const {
    return getImpl<detail::Space_Impl>()->getDefaultSchedule(defaultScheduleType);
  }

  bool Space::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet) {
    return getImpl<detail::Space_Impl>()->setDefaultScheduleSet(defaultScheduleSet);
  }

  void Space::resetDefaultScheduleSet() {
    getImpl<detail::Space_Impl>()->resetDefaultScheduleSet();
  }

  boost::optional<ThermalZone> Space::thermalZone() const {
    return getImpl<detail::Space_Impl>()->thermalZone();
  }

  bool Space::setThermalZone(ThermalZone& thermalZone) {
    return getImpl<detail::Space_Impl>()->setThermalZone(thermalZone);
  }

  void Space::resetThermalZone() {
    getImpl<detail::Space_Impl>()->resetThermalZone();
  }

  boost::optional<BuildingStory> Space::buildingStory() const {
    return getImpl<detail::Space_Impl>()->buildingStory();
  }

  bool Space::setBuildingStory(const BuildingStory& buildingStory) {
    return getImpl<detail::Space_Impl>()->setBuildingStory(buildingStory);
  }

  void Space::resetBuildingStory() {
    getImpl<detail::Space_Impl>()->resetBuildingStory();
  }

  boost::optional<BuildingUnit> Space::buildingUnit() const {
    return getImpl<detail::Space_Impl>()->buildingUnit();
  }

  bool Space::setBuildingUnit(const BuildingUnit& buildingUnit) {
    return getImpl<detail::Space_Impl>()->setBuildingUnit(buildingUnit);
  }

  void Space::resetBuildingUnit() {
    getImpl<detail::Space_Impl>()->resetBuildingUnit();
  }

  std::vector<ShadingSurfaceGroup> Space::shadingSurfaceGroups() const {
    return getImpl<detail::Space_Impl>()->shadingSurfaceGroups();
  }

  std::vector<InteriorPartitionSurfaceGroup> Space::interiorPartitionSurfaceGroups() const {
    return getImpl<detail::Space_Impl>()->interiorPartitionSurfaceGroups();
  }

  std::vector<Surface> Space::surfaces() const {
    return getImpl<detail::Space_Impl>()->surfaces();
  }

  std::vector<InternalMass> Space::internalMass() const {
    return getImpl<detail::Space_Impl>()->internalMass();
  }

  std::vector<People> Space::people() const {
    return getImpl<detail::Space_Impl>()->people();
  }

  std::vector<Lights> Space::lights() const {
    return getImpl<detail::Space_Impl>()->lights();
  }

  std::vector<Luminaire> Space::luminaires() const {
    return getImpl<detail::Space_Impl>()->luminaires();
  }

  std::vector<ElectricEquipment> Space::electricEquipment() const {
    return getImpl<detail::Space_Impl>()->electricEquipment();
  }

  std::vector<ElectricEquipmentITEAirCooled> Space::electricEquipmentITEAirCooled() const {
    return getImpl<detail::Space_Impl>()->electricEquipmentITEAirCooled();
  }

  std::vector<GasEquipment> Space::gasEquipment() const {
    return getImpl<detail::Space_Impl>()->gasEquipment();
  }

  std::vector<HotWaterEquipment> Space::hotWaterEquipment() const {
    return getImpl<detail::Space_Impl>()->hotWaterEquipment();
  }

  std::vector<SteamEquipment> Space::steamEquipment() const {
    return getImpl<detail::Space_Impl>()->steamEquipment();
  }

  std::vector<OtherEquipment> Space::otherEquipment() const {
    return getImpl<detail::Space_Impl>()->otherEquipment();
  }

  std::vector<WaterUseEquipment> Space::waterUseEquipment() const {
    return getImpl<detail::Space_Impl>()->waterUseEquipment();
  }

  std::vector<DaylightingControl> Space::daylightingControls() const {
    return getImpl<detail::Space_Impl>()->daylightingControls();
  }

  std::vector<IlluminanceMap> Space::illuminanceMaps() const {
    return getImpl<detail::Space_Impl>()->illuminanceMaps();
  }

  std::vector<GlareSensor> Space::glareSensors() const {
    return getImpl<detail::Space_Impl>()->glareSensors();
  }

  std::vector<SpaceInfiltrationDesignFlowRate> Space::spaceInfiltrationDesignFlowRates() const {
    return getImpl<detail::Space_Impl>()->spaceInfiltrationDesignFlowRates();
  }

  std::vector<SpaceInfiltrationEffectiveLeakageArea> Space::spaceInfiltrationEffectiveLeakageAreas() const {
    return getImpl<detail::Space_Impl>()->spaceInfiltrationEffectiveLeakageAreas();
  }

  std::vector<SpaceInfiltrationFlowCoefficient> Space::spaceInfiltrationFlowCoefficients() const {
    return getImpl<detail::Space_Impl>()->spaceInfiltrationFlowCoefficients();
  }

  boost::optional<DesignSpecificationOutdoorAir> Space::designSpecificationOutdoorAir() const {
    return getImpl<detail::Space_Impl>()->designSpecificationOutdoorAir();
  }

  bool Space::isDesignSpecificationOutdoorAirDefaulted() const {
    return getImpl<detail::Space_Impl>()->isDesignSpecificationOutdoorAirDefaulted();
  }

  bool Space::setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) {
    return getImpl<detail::Space_Impl>()->setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
  }

  void Space::resetDesignSpecificationOutdoorAir() {
    return getImpl<detail::Space_Impl>()->resetDesignSpecificationOutdoorAir();
  }

  int Space::multiplier() const {
    return getImpl<detail::Space_Impl>()->multiplier();
  }

  double Space::exteriorArea() const {
    return getImpl<detail::Space_Impl>()->exteriorArea();
  }

  double Space::exteriorWallArea() const {
    return getImpl<detail::Space_Impl>()->exteriorWallArea();
  }

  double Space::ceilingHeight() const {
    return getImpl<detail::Space_Impl>()->ceilingHeight();
  }

  bool Space::isCeilingHeightDefaulted() const {
    return getImpl<detail::Space_Impl>()->isCeilingHeightDefaulted();
  }

  bool Space::isCeilingHeightAutocalculated() const {
    return getImpl<detail::Space_Impl>()->isCeilingHeightAutocalculated();
  }

  double Space::volume() const {
    return getImpl<detail::Space_Impl>()->volume();
  }

  bool Space::isVolumeDefaulted() const {
    return getImpl<detail::Space_Impl>()->isVolumeDefaulted();
  }

  bool Space::isVolumeAutocalculated() const {
    return getImpl<detail::Space_Impl>()->isVolumeAutocalculated();
  }

  double Space::floorArea() const {
    return getImpl<detail::Space_Impl>()->floorArea();
  }

  bool Space::isFloorAreaDefaulted() const {
    return getImpl<detail::Space_Impl>()->isFloorAreaDefaulted();
  }

  bool Space::isFloorAreaAutocalculated() const {
    return getImpl<detail::Space_Impl>()->isFloorAreaAutocalculated();
  }

  double Space::numberOfPeople() const {
    return getImpl<detail::Space_Impl>()->numberOfPeople();
  }

  bool Space::setNumberOfPeople(double numberOfPeople) {
    return getImpl<detail::Space_Impl>()->setNumberOfPeople(numberOfPeople);
  }

  bool Space::setNumberOfPeople(double numberOfPeople, const People& templatePeople) {
    return getImpl<detail::Space_Impl>()->setNumberOfPeople(numberOfPeople, templatePeople);
  }

  double Space::peoplePerFloorArea() const {
    return getImpl<detail::Space_Impl>()->peoplePerFloorArea();
  }

  bool Space::setPeoplePerFloorArea(double peoplePerFloorArea) {
    return getImpl<detail::Space_Impl>()->setPeoplePerFloorArea(peoplePerFloorArea);
  }

  bool Space::setPeoplePerFloorArea(double peoplePerFloorArea, const People& templatePeople) {
    return getImpl<detail::Space_Impl>()->setPeoplePerFloorArea(peoplePerFloorArea, templatePeople);
  }

  double Space::floorAreaPerPerson() const {
    return getImpl<detail::Space_Impl>()->floorAreaPerPerson();
  }

  bool Space::setFloorAreaPerPerson(double floorAreaPerPerson) {
    return getImpl<detail::Space_Impl>()->setFloorAreaPerPerson(floorAreaPerPerson);
  }

  bool Space::setFloorAreaPerPerson(double floorAreaPerPerson, const People& templatePeople) {
    return getImpl<detail::Space_Impl>()->setFloorAreaPerPerson(floorAreaPerPerson, templatePeople);
  }

  double Space::lightingPower() const {
    return getImpl<detail::Space_Impl>()->lightingPower();
  }

  bool Space::setLightingPower(double lightingPower) {
    return getImpl<detail::Space_Impl>()->setLightingPower(lightingPower);
  }

  bool Space::setLightingPower(double lightingPower, const Lights& templateLights) {
    return getImpl<detail::Space_Impl>()->setLightingPower(lightingPower, templateLights);
  }

  double Space::lightingPowerPerFloorArea() const {
    return getImpl<detail::Space_Impl>()->lightingPowerPerFloorArea();
  }

  bool Space::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea) {
    return getImpl<detail::Space_Impl>()->setLightingPowerPerFloorArea(lightingPowerPerFloorArea);
  }

  bool Space::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, const Lights& templateLights) {
    return getImpl<detail::Space_Impl>()->setLightingPowerPerFloorArea(lightingPowerPerFloorArea, templateLights);
  }

  double Space::lightingPowerPerPerson() const {
    return getImpl<detail::Space_Impl>()->lightingPowerPerPerson();
  }

  bool Space::setLightingPowerPerPerson(double lightingPowerPerPerson) {
    return getImpl<detail::Space_Impl>()->setLightingPowerPerPerson(lightingPowerPerPerson);
  }

  bool Space::setLightingPowerPerPerson(double lightingPowerPerPerson, const Lights& templateLights) {
    return getImpl<detail::Space_Impl>()->setLightingPowerPerPerson(lightingPowerPerPerson, templateLights);
  }

  double Space::electricEquipmentPower() const {
    return getImpl<detail::Space_Impl>()->electricEquipmentPower();
  }

  double Space::electricEquipmentITEAirCooledPower() const {
    return getImpl<detail::Space_Impl>()->electricEquipmentITEAirCooledPower();
  }

  bool Space::setElectricEquipmentPower(double electricEquipmentPower) {
    return getImpl<detail::Space_Impl>()->setElectricEquipmentPower(electricEquipmentPower);
  }

  bool Space::setElectricEquipmentPower(double electricEquipmentPower, const ElectricEquipment& templateElectricEquipment) {
    return getImpl<detail::Space_Impl>()->setElectricEquipmentPower(electricEquipmentPower, templateElectricEquipment);
  }

  double Space::electricEquipmentPowerPerFloorArea() const {
    return getImpl<detail::Space_Impl>()->electricEquipmentPowerPerFloorArea();
  }

  double Space::electricEquipmentITEAirCooledPowerPerFloorArea() const {
    return getImpl<detail::Space_Impl>()->electricEquipmentITEAirCooledPowerPerFloorArea();
  }

  bool Space::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea) {
    return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea);
  }

  bool Space::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea, const ElectricEquipment& templateElectricEquipment) {
    return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea, templateElectricEquipment);
  }

  double Space::electricEquipmentPowerPerPerson() const {
    return getImpl<detail::Space_Impl>()->electricEquipmentPowerPerPerson();
  }

  bool Space::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson) {
    return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson);
  }

  bool Space::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson, const ElectricEquipment& templateElectricEquipment) {
    return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson, templateElectricEquipment);
  }

  double Space::gasEquipmentPower() const {
    return getImpl<detail::Space_Impl>()->gasEquipmentPower();
  }

  bool Space::setGasEquipmentPower(double gasEquipmentPower) {
    return getImpl<detail::Space_Impl>()->setGasEquipmentPower(gasEquipmentPower);
  }

  bool Space::setGasEquipmentPower(double gasEquipmentPower, const GasEquipment& templateGasEquipment) {
    return getImpl<detail::Space_Impl>()->setGasEquipmentPower(gasEquipmentPower, templateGasEquipment);
  }

  double Space::gasEquipmentPowerPerFloorArea() const {
    return getImpl<detail::Space_Impl>()->gasEquipmentPowerPerFloorArea();
  }

  bool Space::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea) {
    return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea);
  }

  bool Space::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea, const GasEquipment& templateGasEquipment) {
    return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea, templateGasEquipment);
  }

  double Space::gasEquipmentPowerPerPerson() const {
    return getImpl<detail::Space_Impl>()->gasEquipmentPowerPerPerson();
  }

  bool Space::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson) {
    return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson);
  }

  bool Space::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson, const GasEquipment& templateGasEquipment) {
    return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson, templateGasEquipment);
  }

  double Space::infiltrationDesignFlowRate() const {
    return getImpl<detail::Space_Impl>()->infiltrationDesignFlowRate();
  }

  double Space::infiltrationDesignFlowPerSpaceFloorArea() const {
    return getImpl<detail::Space_Impl>()->infiltrationDesignFlowPerSpaceFloorArea();
  }

  double Space::infiltrationDesignFlowPerExteriorSurfaceArea() const {
    return getImpl<detail::Space_Impl>()->infiltrationDesignFlowPerExteriorSurfaceArea();
  }

  double Space::infiltrationDesignFlowPerExteriorWallArea() const {
    return getImpl<detail::Space_Impl>()->infiltrationDesignFlowPerExteriorWallArea();
  }

  double Space::infiltrationDesignAirChangesPerHour() const {
    return getImpl<detail::Space_Impl>()->infiltrationDesignAirChangesPerHour();
  }

  void Space::hardApplySpaceType(bool hardSizeLoads) {
    getImpl<detail::Space_Impl>()->hardApplySpaceType(hardSizeLoads);
  }

  void Space::hardApplySpaceLoadSchedules() {
    getImpl<detail::Space_Impl>()->hardApplySpaceLoadSchedules();
  }

  void Space::hardApplyConstructions() {
    getImpl<detail::Space_Impl>()->hardApplyConstructions();
  }

  void Space::unmatchSurfaces() {
    getImpl<detail::Space_Impl>()->unmatchSurfaces();
  }

  void Space::matchSurfaces(Space& space) {
    getImpl<detail::Space_Impl>()->matchSurfaces(space);
  }

  void Space::intersectSurfaces(Space& space) {
    getImpl<detail::Space_Impl>()->intersectSurfaces(space);
  }

  std::vector<Surface> Space::findSurfaces(boost::optional<double> minDegreesFromNorth, boost::optional<double> maxDegreesFromNorth,
                                           boost::optional<double> minDegreesTilt, boost::optional<double> maxDegreesTilt, double tol) {
    return getImpl<detail::Space_Impl>()->findSurfaces(minDegreesFromNorth, maxDegreesFromNorth, minDegreesTilt, maxDegreesTilt, tol);
  }

  std::vector<Point3d> Space::floorPrint() const {
    return getImpl<detail::Space_Impl>()->floorPrint();
  }

  bool Space::isPlenum() const {
    return getImpl<detail::Space_Impl>()->isPlenum();
  }

  double Space::exposedPerimeter(const Polygon3d& buildingPerimeter) const {
    return getImpl<detail::Space_Impl>()->exposedPerimeter(buildingPerimeter);
  }

  Polyhedron Space::polyhedron() const {
    return getImpl<detail::Space_Impl>()->polyhedron();
  }

  bool Space::isEnclosedVolume() const {
    return getImpl<detail::Space_Impl>()->isEnclosedVolume();
  }

  std::vector<ZoneMixing> Space::zoneMixing() const {
    return getImpl<detail::Space_Impl>()->zoneMixing();
  }

  std::vector<ZoneMixing> Space::supplyZoneMixing() const {
    return getImpl<detail::Space_Impl>()->supplyZoneMixing();
  }

  std::vector<ZoneMixing> Space::exhaustZoneMixing() const {
    return getImpl<detail::Space_Impl>()->exhaustZoneMixing();
  }

  std::vector<Surface> Space::findSurfacesWithIncorrectOrientation() const {
    return getImpl<detail::Space_Impl>()->findSurfacesWithIncorrectOrientation();
  }

  bool Space::areAllSurfacesCorrectlyOriented() const {
    return getImpl<detail::Space_Impl>()->areAllSurfacesCorrectlyOriented();
  }

  bool Space::fixSurfacesWithIncorrectOrientation() {
    return getImpl<detail::Space_Impl>()->fixSurfacesWithIncorrectOrientation();
  }

  bool Space::isConvex() const {
    return getImpl<detail::Space_Impl>()->isConvex();
  }

  std::vector<Surface> Space::findNonConvexSurfaces() const {
    return getImpl<detail::Space_Impl>()->findNonConvexSurfaces();
  }

  /// @cond
  void Space::cacheGeometryDiagnostics() {
    getImpl<detail::Space_Impl>()->cacheGeometryDiagnostics();
  }
  void Space::resetCachedGeometryDiagnostics() {
    getImpl<detail::Space_Impl>()->resetCachedGeometryDiagnostics();
  }
  /// @endcond

  /// @cond
  Space::Space(std::shared_ptr<detail::Space_Impl> impl) : PlanarSurfaceGroup(std::move(impl)) {}
  /// @endcond

  void intersectSurfaces(std::vector<Space>& t_spaces) {
    std::vector<Space> spaces(t_spaces);
    std::sort(spaces.begin(), spaces.end(), [](const Space& a, const Space& b) -> bool { return a.floorArea() < b.floorArea(); });

    std::vector<BoundingBox> bounds;
    for (const Space& space : spaces) {
      bounds.push_back(space.transformation() * space.boundingBox());
    }

    for (unsigned i = 0; i < spaces.size(); ++i) {
      for (unsigned j = i + 1; j < spaces.size(); ++j) {
        if (!bounds[i].intersects(bounds[j])) {
          continue;
        }
        spaces[i].intersectSurfaces(spaces[j]);
      }
    }
  }

  void matchSurfaces(std::vector<Space>& spaces) {
    std::vector<BoundingBox> bounds;
    for (const Space& space : spaces) {
      bounds.push_back(space.transformation() * space.boundingBox());
    }

    for (unsigned i = 0; i < spaces.size(); ++i) {
      for (unsigned j = i + 1; j < spaces.size(); ++j) {
        if (!bounds[i].intersects(bounds[j])) {
          continue;
        }
        spaces[i].matchSurfaces(spaces[j]);
      }
    }
  }

  void unmatchSurfaces(std::vector<Space>& spaces) {
    for (Space& space : spaces) {
      space.unmatchSurfaces();
    }
  }

  std::vector<std::vector<Point3d>> generateSkylightPattern(const std::vector<Space>& spaces, double directionOfRelativeNorth,
                                                            double skylightToProjectedFloorRatio, double desiredWidth, double desiredHeight) {
    std::vector<std::vector<Point3d>> result;

    if (skylightToProjectedFloorRatio <= 0.0) {
      return result;
    } else if (skylightToProjectedFloorRatio >= 1.0) {
      return result;
    }

    if (desiredWidth <= 0) {
      return result;
    }

    if (desiredHeight <= 0) {
      return result;
    }

    if (spaces.empty()) {
      return result;
    }

    // rotate negative amount around the z axis, EnergyPlus defines rotation clockwise
    Transformation buildingToGridTransformation = Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(directionOfRelativeNorth));

    // rotate positive amount around the z axis, EnergyPlus defines rotation clockwise
    Transformation gridToBuildingTransformation = buildingToGridTransformation.inverse();

    // find extents in grid coordinate system
    double xmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::min();
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::min();
    for (const Space& space : spaces) {
      Transformation spaceToBuildingTransformation = space.buildingTransformation();
      Transformation transformation = buildingToGridTransformation * spaceToBuildingTransformation;
      for (const Surface& surface : space.surfaces()) {
        if (istringEqual("RoofCeiling", surface.surfaceType()) && istringEqual("Outdoors", surface.outsideBoundaryCondition())) {
          std::vector<Point3d> vertices = transformation * surface.vertices();
          for (const Point3d& vertex : vertices) {
            xmin = std::min(xmin, vertex.x());
            xmax = std::max(xmax, vertex.x());
            ymin = std::min(ymin, vertex.y());
            ymax = std::max(ymax, vertex.y());
          }
        }
      }
    }
    if ((xmin > xmax) || (ymin > ymax)) {
      return result;
    }

    double floorPrintWidth = (xmax - xmin);
    double floorPrintHeight = (ymax - ymin);

    double numSkylightsX = std::sqrt(skylightToProjectedFloorRatio) * floorPrintWidth / desiredWidth;
    double numSkylightsY = std::sqrt(skylightToProjectedFloorRatio) * floorPrintHeight / desiredHeight;

    // space is distance from end of one skylight to beginning of next
    double xSpace = (floorPrintWidth - numSkylightsX * desiredWidth) / (std::ceil(numSkylightsX));
    double ySpace = (floorPrintHeight - numSkylightsY * desiredHeight) / (std::ceil(numSkylightsY));

    if ((xSpace <= 0.0) || (ySpace <= 0.0)) {
      return result;
    }

    for (double x = xmin + xSpace / 2.0; x < xmax - xSpace / 2.0; x += desiredWidth + xSpace) {
      for (double y = ymin + ySpace / 2.0; y < ymax - ySpace / 2.0; y += desiredHeight + ySpace) {

        double x2 = std::min(x + desiredWidth, xmax - xSpace / 2.0);
        double y2 = std::min(y + desiredHeight, ymax - ySpace / 2.0);

        // skylight in grid coordinates
        std::vector<Point3d> skylight{
          {x, y, 0},
          {x2, y, 0},
          {x2, y2, 0},
          {x, y2, 0},
        };

        // put results into building coordinates
        result.push_back(gridToBuildingTransformation * skylight);
      }
    }

    return result;
  }

}  // namespace model
}  // namespace openstudio
