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

#include <model/Space.hpp>
#include <model/Space_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/RenderingColor.hpp>
#include <model/RenderingColor_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/InteriorPartitionSurfaceGroup.hpp>
#include <model/InteriorPartitionSurfaceGroup_Impl.hpp>
#include <model/InteriorPartitionSurface.hpp>
#include <model/InteriorPartitionSurface_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/InternalMass.hpp>
#include <model/InternalMass_Impl.hpp>
#include <model/InternalMassDefinition.hpp>
#include <model/InternalMassDefinition_Impl.hpp>
#include <model/People.hpp>
#include <model/People_Impl.hpp>
#include <model/PeopleDefinition.hpp>
#include <model/PeopleDefinition_Impl.hpp>
#include <model/Lights.hpp>
#include <model/Lights_Impl.hpp>
#include <model/LightsDefinition.hpp>
#include <model/LightsDefinition_Impl.hpp>
#include <model/Luminaire.hpp>
#include <model/Luminaire_Impl.hpp>
#include <model/LuminaireDefinition.hpp>
#include <model/LuminaireDefinition_Impl.hpp>
#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipment_Impl.hpp>
#include <model/ElectricEquipmentDefinition.hpp>
#include <model/ElectricEquipmentDefinition_Impl.hpp>
#include <model/GasEquipment.hpp>
#include <model/GasEquipment_Impl.hpp>
#include <model/GasEquipmentDefinition.hpp>
#include <model/GasEquipmentDefinition_Impl.hpp>
#include <model/HotWaterEquipment.hpp>
#include <model/HotWaterEquipment_Impl.hpp>
#include <model/SteamEquipment.hpp>
#include <model/SteamEquipment_Impl.hpp>
#include <model/OtherEquipment.hpp>
#include <model/OtherEquipment_Impl.hpp>
#include <model/DaylightingControl.hpp>
#include <model/DaylightingControl_Impl.hpp>
#include <model/IlluminanceMap.hpp>
#include <model/IlluminanceMap_Impl.hpp>
#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/GlareSensor.hpp>
#include <model/GlareSensor_Impl.hpp>


#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>

#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Vector3d.hpp>
#include <utilities/geometry/EulerAngles.hpp>
#include <utilities/geometry/BoundingBox.hpp>

#include <utilities/core/Assert.hpp>

#undef BOOST_UBLAS_TYPE_CHECK
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

#include <cmath>

namespace openstudio {
namespace model {

namespace detail {

  Space_Impl::Space_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : PlanarSurfaceGroup_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == Space::iddObjectType());
  }

  Space_Impl::Space_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : PlanarSurfaceGroup_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == Space::iddObjectType());
  }

  Space_Impl::Space_Impl(const Space_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : PlanarSurfaceGroup_Impl(other,model,keepHandle)
  {}

 boost::optional<ParentObject> Space_Impl::parent() const
  {
    return boost::optional<ParentObject>(this->model().building());
  }

  bool Space_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<Building>()){
      result = (this->model() == newParent.model());
    }
    return result;
  }

  std::vector<ModelObject> Space_Impl::children() const
  {
    std::vector<ModelObject> result;

    // shading groups
    ShadingSurfaceGroupVector shadingSurfaceGroups = this->shadingSurfaceGroups();
    result.insert(result.end(), shadingSurfaceGroups.begin(), shadingSurfaceGroups.end());

    // interior surface partition groups
    InteriorPartitionSurfaceGroupVector interiorPartitionSurfaceGroups = this->interiorPartitionSurfaceGroups();
    result.insert(result.end(), interiorPartitionSurfaceGroups.begin(), interiorPartitionSurfaceGroups.end());

    // surfaces
    SurfaceVector surfaces = this->surfaces();
    result.insert(result.end(), surfaces.begin(), surfaces.end());
    
    // internal mass
    InternalMassVector internalMass = this->internalMass();
    result.insert(result.end(), internalMass.begin(), internalMass.end());

    // lights
    LightsVector lights = this->lights();
    result.insert(result.end(), lights.begin(), lights.end());

    // luminaires
    LuminaireVector luminaires = this->luminaires();
    result.insert(result.end(), luminaires.begin(), luminaires.end());

    // people
    PeopleVector people = this->people();
    result.insert(result.end(), people.begin(), people.end());

    // electric equipment
    ElectricEquipmentVector electricEquipment = this->electricEquipment();
    result.insert(result.end(), electricEquipment.begin(), electricEquipment.end());

    // gas equipment
    GasEquipmentVector gasEquipment = this->gasEquipment();
    result.insert(result.end(), gasEquipment.begin(), gasEquipment.end());

    // hot water equipment
    HotWaterEquipmentVector hotWaterEquipment = this->hotWaterEquipment();
    result.insert(result.end(), hotWaterEquipment.begin(), hotWaterEquipment.end());

    // steam equipment
    SteamEquipmentVector steamEquipment = this->steamEquipment();
    result.insert(result.end(), steamEquipment.begin(), steamEquipment.end());

    // other equipment
    OtherEquipmentVector otherEquipment = this->otherEquipment();
    result.insert(result.end(), otherEquipment.begin(), otherEquipment.end());    
    
    // daylighting controls
    DaylightingControlVector daylightingControls = this->daylightingControls();
    result.insert(result.end(), daylightingControls.begin(), daylightingControls.end());

    //  illuminance map
    IlluminanceMapVector illuminanceMaps = this->illuminanceMaps();
    result.insert(result.end(), illuminanceMaps.begin(), illuminanceMaps.end());

   // glare sensor
    GlareSensorVector glareSensors = this->glareSensors();
    result.insert(result.end(), glareSensors.begin(), glareSensors.end());

    
    // SpaceInfiltration_DesignFlowRate
    SpaceInfiltrationDesignFlowRateVector spaceInfiltrationDesignFlowRates = this->spaceInfiltrationDesignFlowRates();
    result.insert(result.end(), spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end());

    // SpaceInfiltration_EffectiveLeakageArea
    SpaceInfiltrationEffectiveLeakageAreaVector spaceInfiltrationEffectiveLeakageAreas = this->spaceInfiltrationEffectiveLeakageAreas();
    result.insert(result.end(), spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end());

    return result;
  }

  std::vector<IddObjectType> Space_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_ShadingSurfaceGroup);
    result.push_back(IddObjectType::OS_InteriorPartitionSurfaceGroup);
    result.push_back(IddObjectType::OS_Surface);
    result.push_back(IddObjectType::OS_Lights);
    result.push_back(IddObjectType::OS_Luminaire);
    result.push_back(IddObjectType::OS_People);
    result.push_back(IddObjectType::OS_ElectricEquipment);
    result.push_back(IddObjectType::OS_GasEquipment);
    result.push_back(IddObjectType::OS_HotWaterEquipment);
    result.push_back(IddObjectType::OS_Daylighting_Control);
    result.push_back(IddObjectType::OS_IlluminanceMap);
    result.push_back(IddObjectType::OS_SpaceInfiltration_DesignFlowRate);
    result.push_back(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea);
    return result;
  }

  const std::vector<std::string>& Space_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Space_Impl::iddObjectType() const {
    return Space::iddObjectType();
  }

  Transformation Space_Impl::buildingTransformation() const
  {
    Transformation transformation;
    boost::optional<Building> building = this->model().building();
    if (building){
      transformation = building->transformation() * this->transformation();
    }else{
      transformation = this->transformation();
    }
    return transformation;
  }

  bool Space_Impl::changeTransformation(const openstudio::Transformation& transformation) {
    Transformation oldTransformation = this->transformation();
    if (!setTransformation(transformation)){
      return false;
    }

    // go through and update all child points and transformations so that:
    // (Tnew^-1 * Told) * xold = childTransformation * xold = xnew

    Transformation childTransformation = transformation.inverse() * oldTransformation;

    BOOST_FOREACH(Surface surface, this->surfaces()){
      bool test = surface.setVertices(childTransformation * surface.vertices());
      if(!test){
        LOG(Error, "Could not transform vertices for Surface '" << surface.name().get() << "'.");
      }
      BOOST_FOREACH(SubSurface subSurface, surface.subSurfaces()){
        test = subSurface.setVertices(childTransformation * subSurface.vertices());
        if(!test){
          LOG(Error, "Could not transform vertices for SubSurface '" << subSurface.name().get() << "'.");
        }
      }
    }

    BOOST_FOREACH(ShadingSurfaceGroup shadingSurfaceGroup, this->shadingSurfaceGroups()){
      bool test = shadingSurfaceGroup.setTransformation(childTransformation*shadingSurfaceGroup.transformation());
      BOOST_ASSERT(test);
    }

    BOOST_FOREACH(InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup, this->interiorPartitionSurfaceGroups()){
      bool test = interiorPartitionSurfaceGroup.setTransformation(childTransformation*interiorPartitionSurfaceGroup.transformation());
      BOOST_ASSERT(test);
    }

    BOOST_FOREACH(Luminaire luminaire, this->luminaires()){
      bool test = luminaire.setTransformation(childTransformation*luminaire.transformation());
      BOOST_ASSERT(test);
    }

    BOOST_FOREACH(DaylightingControl daylightingControl, this->daylightingControls()){
      bool test = daylightingControl.setTransformation(childTransformation*daylightingControl.transformation());
      BOOST_ASSERT(test);
    }

    BOOST_FOREACH(IlluminanceMap illuminanceMap, this->illuminanceMaps()){
      bool test = illuminanceMap.setTransformation(childTransformation*illuminanceMap.transformation());
      BOOST_ASSERT(test);
    }

    return true;
  }

  BoundingBox Space_Impl::boundingBox() const
  {
    BoundingBox result;

    BOOST_FOREACH(Surface surface, this->surfaces()){
      result.addPoints(surface.vertices());
    }

    BOOST_FOREACH(ShadingSurfaceGroup shadingSurfaceGroup, this->shadingSurfaceGroups()){
      result.addPoints(shadingSurfaceGroup.transformation() * shadingSurfaceGroup.boundingBox().corners());
    }

    BOOST_FOREACH(InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup, this->interiorPartitionSurfaceGroups()){
      result.addPoints(interiorPartitionSurfaceGroup.transformation() * interiorPartitionSurfaceGroup.boundingBox().corners());
    }

    BOOST_FOREACH(Luminaire luminaire, this->luminaires()){
      result.addPoint(luminaire.position());
    }

    BOOST_FOREACH(DaylightingControl daylightingControl, this->daylightingControls()){
      result.addPoint(daylightingControl.position());
    }

    BOOST_FOREACH(IlluminanceMap illuminanceMap, this->illuminanceMaps()){
      result.addPoints(illuminanceMap.transformation() * illuminanceMap.corners());
    }

    BOOST_FOREACH(GlareSensor glareSensor, this->glareSensors()){
      result.addPoint(glareSensor.position());
    }
  
    
    return result;
  }

  double Space_Impl::directionofRelativeNorth() const {
    boost::optional<double> value = getDouble(OS_SpaceFields::DirectionofRelativeNorth,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool Space_Impl::isDirectionofRelativeNorthDefaulted() const {
    return isEmpty(OS_SpaceFields::DirectionofRelativeNorth);
  }

  double Space_Impl::xOrigin() const {
    boost::optional<double> value = getDouble(OS_SpaceFields::XOrigin,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool Space_Impl::isXOriginDefaulted() const {
    return isEmpty(OS_SpaceFields::XOrigin);
  }

  double Space_Impl::yOrigin() const {
    boost::optional<double> value = getDouble(OS_SpaceFields::YOrigin,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool Space_Impl::isYOriginDefaulted() const {
    return isEmpty(OS_SpaceFields::YOrigin);
  }

  double Space_Impl::zOrigin() const {
    boost::optional<double> value = getDouble(OS_SpaceFields::ZOrigin,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool Space_Impl::isZOriginDefaulted() const {
    return isEmpty(OS_SpaceFields::ZOrigin);
  }

  bool Space_Impl::partofTotalFloorArea() const {
    boost::optional<std::string> value = getString(OS_SpaceFields::PartofTotalFloorArea,true);
    BOOST_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool Space_Impl::isPartofTotalFloorAreaDefaulted() const {
    return isEmpty(OS_SpaceFields::PartofTotalFloorArea);
  }

  void Space_Impl::setDirectionofRelativeNorth(double directionofRelativeNorth, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_SpaceFields::DirectionofRelativeNorth, directionofRelativeNorth, driverMethod);
    BOOST_ASSERT(result);
  }

  void Space_Impl::resetDirectionofRelativeNorth() {
    bool result = setString(OS_SpaceFields::DirectionofRelativeNorth, "");
    BOOST_ASSERT(result);
  }

  void Space_Impl::setXOrigin(double xOrigin, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_SpaceFields::XOrigin, xOrigin, driverMethod);
    BOOST_ASSERT(result);
  }

  void Space_Impl::resetXOrigin() {
    bool result = setString(OS_SpaceFields::XOrigin, "");
    BOOST_ASSERT(result);
  }

  void Space_Impl::setYOrigin(double yOrigin, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_SpaceFields::YOrigin, yOrigin, driverMethod);
    BOOST_ASSERT(result);
  }

  void Space_Impl::resetYOrigin() {
    bool result = setString(OS_SpaceFields::YOrigin, "");
    BOOST_ASSERT(result);
  }

  void Space_Impl::setZOrigin(double zOrigin, bool driverMethod) {
    bool result = false;
    result = setDouble(OS_SpaceFields::ZOrigin, zOrigin, driverMethod);
    BOOST_ASSERT(result);
  }

  void Space_Impl::resetZOrigin() {
    bool result = setString(OS_SpaceFields::ZOrigin, "");
    BOOST_ASSERT(result);
  }

  void Space_Impl::setPartofTotalFloorArea(bool partofTotalFloorArea) {
    bool result = false;
    if (partofTotalFloorArea) {
      result = setString(OS_SpaceFields::PartofTotalFloorArea, "Yes");
    } else {
      result = setString(OS_SpaceFields::PartofTotalFloorArea, "No");
    }
    BOOST_ASSERT(result);
  }

  void Space_Impl::resetPartofTotalFloorArea() {
    bool result = setString(OS_SpaceFields::PartofTotalFloorArea, "");
    BOOST_ASSERT(result);
  }

  boost::optional<SpaceType> Space_Impl::spaceType() const
  {
    boost::optional<SpaceType> result = getObject<ModelObject>().getModelObjectTarget<SpaceType>(OS_SpaceFields::SpaceTypeName);
    if (!result){
      boost::optional<Building> building = this->model().building();
      if (building){
        result = building->spaceType();
      }
    }
    return result;
  }

  bool Space_Impl::isSpaceTypeDefaulted() const
  {
    return isEmpty(OS_SpaceFields::SpaceTypeName);
  }

  bool Space_Impl::setSpaceType(const SpaceType& spaceType)
  {
    return setPointer(OS_SpaceFields::SpaceTypeName, spaceType.handle());
  }

  void Space_Impl::resetSpaceType()
  {
    setString(OS_SpaceFields::SpaceTypeName, "");
  }

  boost::optional<DefaultConstructionSet> Space_Impl::defaultConstructionSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultConstructionSet>(OS_SpaceFields::DefaultConstructionSetName);
  }

  boost::optional<ConstructionBase> Space_Impl::getDefaultConstruction(const PlanarSurface& planarSurface) const
  {
    boost::optional<std::pair<ConstructionBase, int> > result = this->getDefaultConstructionWithSearchDistance(planarSurface);
    if (result){
      return result->first;
    }
    return boost::none;
  }

  boost::optional<std::pair<ConstructionBase, int> > Space_Impl::getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const
  {
    boost::optional<ConstructionBase> result;
    boost::optional<DefaultConstructionSet> defaultConstructionSet;
    boost::optional<SpaceType> spaceType;
    boost::optional<BuildingStory> buildingStory;
    boost::optional<Building> building;

    // first check this object
    defaultConstructionSet = this->defaultConstructionSet();
    if (defaultConstructionSet){
      result = defaultConstructionSet->getDefaultConstruction(planarSurface);
      if (result){
        return std::make_pair<ConstructionBase, int>(*result, 1);
      }
    }

    // then check space type
    spaceType = this->spaceType();
    if (spaceType && !this->isSpaceTypeDefaulted()){
      defaultConstructionSet = spaceType->defaultConstructionSet();
      if (defaultConstructionSet){
        result = defaultConstructionSet->getDefaultConstruction(planarSurface);
        if (result){
          return std::make_pair<ConstructionBase, int>(*result, 2);
        }
      }
    }

    // then check building story
    buildingStory = this->buildingStory();
    if (buildingStory){
      defaultConstructionSet = buildingStory->defaultConstructionSet();
      if (defaultConstructionSet){
        result = defaultConstructionSet->getDefaultConstruction(planarSurface);
        if (result){
          return std::make_pair<ConstructionBase, int>(*result, 3);
        }
      }
    }

    // then check building
    building = this->model().building();
    if (building){
      defaultConstructionSet = building->defaultConstructionSet();
      if (defaultConstructionSet){
        result = defaultConstructionSet->getDefaultConstruction(planarSurface);
        if (result){
          return std::make_pair<ConstructionBase, int>(*result, 4);
        }
      }

      // then check building's space type
      spaceType = building->spaceType();
      if (spaceType){
        defaultConstructionSet = spaceType->defaultConstructionSet();
        if (defaultConstructionSet){
          result = defaultConstructionSet->getDefaultConstruction(planarSurface);
          if (result){
            return std::make_pair<ConstructionBase, int>(*result, 5);
          }
        }
      }
    }

    return boost::none;
  }

  bool Space_Impl::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
  {
    return setPointer(OS_SpaceFields::DefaultConstructionSetName, defaultConstructionSet.handle());
  }

  void Space_Impl::resetDefaultConstructionSet()
  {
    setString(OS_SpaceFields::DefaultConstructionSetName, "");
  }

  boost::optional<DefaultScheduleSet> Space_Impl::defaultScheduleSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultScheduleSet>(OS_SpaceFields::DefaultScheduleSetName);
  }

  boost::optional<Schedule> Space_Impl::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const
  {
    boost::optional<Schedule> result;
    boost::optional<DefaultScheduleSet> defaultScheduleSet;
    boost::optional<SpaceType> spaceType;
    boost::optional<BuildingStory> buildingStory;
    boost::optional<Building> building;

    // first check this object
    defaultScheduleSet = this->defaultScheduleSet();
    if (defaultScheduleSet){
      result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
      if (result){
        return result;
      }
    }

    // then check space type
    spaceType = this->spaceType();
    if (spaceType){
      defaultScheduleSet = spaceType->defaultScheduleSet();
      if (defaultScheduleSet){
        result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
        if (result){
          return result;
        }
      }
    }

    // then check building story
    buildingStory = this->buildingStory();
    if (buildingStory){
      defaultScheduleSet = buildingStory->defaultScheduleSet();
      if (defaultScheduleSet){
        result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
        if (result){
          return result;
        }
      }
    }

    // then check building
    building = this->model().building();
    if (building){
      defaultScheduleSet = building->defaultScheduleSet();
      if (defaultScheduleSet){
        result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
        if (result){
          return result;
        }
      }

      // then check building's space type
      spaceType = building->spaceType();
      if (spaceType){
        defaultScheduleSet = spaceType->defaultScheduleSet();
        if (defaultScheduleSet){
          result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
          if (result){
            return result;
          }
        }
      }
    }

    return boost::none;
  }

  bool Space_Impl::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
  {
    return setPointer(OS_SpaceFields::DefaultScheduleSetName, defaultScheduleSet.handle());
  }

  void Space_Impl::resetDefaultScheduleSet()
  {
    setString(OS_SpaceFields::DefaultScheduleSetName, "");
  }

  OptionalThermalZone Space_Impl::thermalZone() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SpaceFields::ThermalZoneName);
  }

  bool Space_Impl::setThermalZone(ThermalZone& thermalZone)
  {
    bool result = this->setPointer(OS_SpaceFields::ThermalZoneName, thermalZone.handle());
    if (result){
      thermalZone.checkDaylightingControlsAndIlluminanceMaps();
    }
    return result;
  }

  void Space_Impl::resetThermalZone()
  {
    bool result = this->setString(OS_SpaceFields::ThermalZoneName, "");
    BOOST_ASSERT(result);
  }

  OptionalBuildingStory Space_Impl::buildingStory() const
  {
    return getObject<ModelObject>().getModelObjectTarget<BuildingStory>(OS_SpaceFields::BuildingStoryName);
  }

  bool Space_Impl::setBuildingStory(const BuildingStory& buildingStory)
  {
    return this->setPointer(OS_SpaceFields::BuildingStoryName, buildingStory.handle());
  }

  void Space_Impl::resetBuildingStory()
  {
    bool result = this->setString(OS_SpaceFields::BuildingStoryName, "");
    BOOST_ASSERT(result);
  }

  ShadingSurfaceGroupVector Space_Impl::shadingSurfaceGroups() const
  {
    return getObject<ModelObject>().getModelObjectSources<ShadingSurfaceGroup>(
      ShadingSurfaceGroup::iddObjectType());
  }

  InteriorPartitionSurfaceGroupVector Space_Impl::interiorPartitionSurfaceGroups() const
  {
    return getObject<ModelObject>().getModelObjectSources<InteriorPartitionSurfaceGroup>(
      InteriorPartitionSurfaceGroup::iddObjectType());
  }

  SurfaceVector Space_Impl::surfaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<Surface>(Surface::iddObjectType());
  }

  std::vector<InternalMass> Space_Impl::internalMass() const {
    return getObject<Space>().getModelObjectSources<InternalMass>(InternalMass::iddObjectType());
  }

  PeopleVector Space_Impl::people() const
  {
    return getObject<ModelObject>().getModelObjectSources<People>(People::iddObjectType());
  }

  LightsVector Space_Impl::lights() const
  {
    return getObject<ModelObject>().getModelObjectSources<Lights>(Lights::iddObjectType());
  }

  LuminaireVector Space_Impl::luminaires() const
  {
    return getObject<ModelObject>().getModelObjectSources<Luminaire>(Luminaire::iddObjectType());
  }

  ElectricEquipmentVector Space_Impl::electricEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<ElectricEquipment>(
      ElectricEquipment::iddObjectType());
  }

  GasEquipmentVector Space_Impl::gasEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<GasEquipment>(
      GasEquipment::iddObjectType());
  }

  HotWaterEquipmentVector Space_Impl::hotWaterEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<HotWaterEquipment>(
      HotWaterEquipment::iddObjectType());
  }

  SteamEquipmentVector Space_Impl::steamEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<SteamEquipment>(
      SteamEquipment::iddObjectType());
  }

  OtherEquipmentVector Space_Impl::otherEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<OtherEquipment>(
      OtherEquipment::iddObjectType());
  }

  DaylightingControlVector Space_Impl::daylightingControls() const
  {
    return getObject<ModelObject>().getModelObjectSources<DaylightingControl>(DaylightingControl::iddObjectType());
  }

  IlluminanceMapVector Space_Impl::illuminanceMaps() const
  {
    return getObject<ModelObject>().getModelObjectSources<IlluminanceMap>(IlluminanceMap::iddObjectType());
  }

   GlareSensorVector Space_Impl::glareSensors() const
  {
    return getObject<ModelObject>().getModelObjectSources<GlareSensor>(GlareSensor::iddObjectType());
  } 
  
  SpaceInfiltrationDesignFlowRateVector Space_Impl::spaceInfiltrationDesignFlowRates() const
  {
    return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationDesignFlowRate>(
      SpaceInfiltrationDesignFlowRate::iddObjectType());
  }

  SpaceInfiltrationEffectiveLeakageAreaVector Space_Impl::spaceInfiltrationEffectiveLeakageAreas() const
  {
    return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationEffectiveLeakageArea>(
      SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
  }

  boost::optional<DesignSpecificationOutdoorAir> Space_Impl::designSpecificationOutdoorAir() const
  {
    boost::optional<DesignSpecificationOutdoorAir> result;

    result = getObject<ModelObject>().getModelObjectTarget<DesignSpecificationOutdoorAir>(
      OS_SpaceFields::DesignSpecificationOutdoorAirObjectName);

    if (!result){
      boost::optional<SpaceType> spaceType = this->spaceType();
      if (spaceType){
        result = spaceType->designSpecificationOutdoorAir();
      }
    }

    return result;
  }

  bool Space_Impl::isDesignSpecificationOutdoorAirDefaulted() const
  {
    return isEmpty(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName);
  }

  bool Space_Impl::setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir)
  {
    return setPointer(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir.handle());
  }

  void Space_Impl::resetDesignSpecificationOutdoorAir()
  {
    bool test = setString(OS_SpaceFields::DesignSpecificationOutdoorAirObjectName, "");
    BOOST_ASSERT(test);
  }

  int Space_Impl::multiplier() const
  {
    int result = 1;
    OptionalThermalZone thermalZone = this->thermalZone();
    if (thermalZone){
      result = thermalZone->multiplier();
    }
    return result;
  }

  double Space_Impl::floorArea() const
  {
    double result = 0;
    BOOST_FOREACH(const Surface& surface, this->surfaces()) {
      if (istringEqual(surface.surfaceType(), "Floor"))
      {
        result += surface.grossArea();
      }
    }
    return result;
  }

  double Space_Impl::exteriorArea() const {
    double result = 0;
    BOOST_FOREACH(const Surface& surface, this->surfaces()) {
      if (istringEqual(surface.outsideBoundaryCondition(), "Outdoors"))
      {
        result += surface.grossArea();
      }
    }
    return result;
  }

  double Space_Impl::volume() const {
    double result = 0;

    // TODO: need a better method
    double roofHeight = 0;
    int numRoof = 0;
    double floorHeight = 0;
    int numFloor = 0;
    BOOST_FOREACH(const Surface& surface, this->surfaces()) {
      if (istringEqual(surface.surfaceType(), "Floor")){
        BOOST_FOREACH(const Point3d& point, surface.vertices()) {
          floorHeight += point.z();
          ++numFloor;
        }
      }else if (istringEqual(surface.surfaceType(), "RoofCeiling")){
        BOOST_FOREACH(const Point3d& point, surface.vertices()) {
          roofHeight += point.z();
          ++numRoof;
        }
      }
    }

    if ((numRoof > 0) * (numFloor > 0)){
      roofHeight /= numRoof;
      floorHeight /= numFloor;
      result = (roofHeight - floorHeight) * this->floorArea();
    }

    return result;
  }

  double Space_Impl::numberOfPeople() const {
    double result = 0.0;
    double area = floorArea();

    BOOST_FOREACH(const People& person, this->people()) {
      result += person.getNumberOfPeople(area);
    }

    if (OptionalSpaceType st = spaceType()){
      BOOST_FOREACH(const People& person, st->people()) {
        result += person.getNumberOfPeople(area);
      }
    }

    return result;
  }

  bool Space_Impl::setNumberOfPeople(double numberOfPeople) {
    OptionalPeople templatePeople;
    PeopleVector myPeople = people();
    if (myPeople.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myPeople = st->people();
      }
    }
    if (!myPeople.empty()) {
      templatePeople = myPeople[0];
    }
    return setNumberOfPeople(numberOfPeople,templatePeople);
  }

  bool Space_Impl::setNumberOfPeople(double numberOfPeople, 
                                     const boost::optional<People>& templatePeople) 
  {
    if (numberOfPeople < 0.0) {
      LOG(Error,"Space cannot set numberOfPeople to " << numberOfPeople 
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalPeople myPeople = getMySpaceLoadInstance<People,PeopleDefinition>(templatePeople);
    if (!myPeople) {
      LOG(Error,"The templatePeople object must be in the same Model as this Space.");
      return false;
    }

    // set space and number of people
    bool ok(true);
    myPeople->makeUnique();
    ok = myPeople->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myPeople->peopleDefinition().setNumberofPeople(numberOfPeople); BOOST_ASSERT(ok);
    ok = myPeople->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other people from space
    PeopleVector allMyPeople = people();
    removeAllButOneSpaceLoadInstance<People>(allMyPeople,*myPeople);
    
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
        BOOST_FOREACH(People& peopleObject,allMyPeople) {
          peopleObject.remove();
        }
      }
    }

    return true;
  }


  double Space_Impl::peoplePerFloorArea() const {
    double result = 0.0;
    double area = floorArea();

    BOOST_FOREACH(const People& person, this->people()) {
      result += person.getPeoplePerFloorArea(area);
    }

    if (OptionalSpaceType st = spaceType()){
      BOOST_FOREACH(const People& person, st->people()) {
        result += person.getPeoplePerFloorArea(area);
      }
    }

    return result;
  }

  bool Space_Impl::setPeoplePerFloorArea(double peoplePerFloorArea) {
    OptionalPeople templatePeople;
    PeopleVector myPeople = people();
    if (myPeople.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myPeople = st->people();
      }
    }
    if (!myPeople.empty()) {
      templatePeople = myPeople[0];
    }
    return setPeoplePerFloorArea(peoplePerFloorArea,templatePeople);
  }

  bool Space_Impl::setPeoplePerFloorArea(double peoplePerFloorArea, 
                                         const boost::optional<People>& templatePeople) 
  {
    if (peoplePerFloorArea < 0.0) {
      LOG(Error,"Space cannot set peoplePerFloorArea to " << peoplePerFloorArea 
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalPeople myPeople = getMySpaceLoadInstance<People,PeopleDefinition>(templatePeople);
    if (!myPeople) {
      LOG(Error,"The templatePeople object must be in the same Model as this Space.");
      return false;
    }

    // set space and number of people
    bool ok(true);
    myPeople->makeUnique();
    ok = myPeople->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myPeople->peopleDefinition().setPeopleperSpaceFloorArea(peoplePerFloorArea); 
    BOOST_ASSERT(ok);
    ok = myPeople->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other people from space
    PeopleVector allMyPeople = people();
    removeAllButOneSpaceLoadInstance<People>(allMyPeople,*myPeople);
    
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
        BOOST_FOREACH(People& peopleObject,allMyPeople) {
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
      if (OptionalSpaceType st  = spaceType()) {
        myPeople = st->people();
      }
    }
    if (!myPeople.empty()) {
      templatePeople = myPeople[0];
    }
    return setFloorAreaPerPerson(floorAreaPerPerson,templatePeople);
  }

  bool Space_Impl::setFloorAreaPerPerson(double floorAreaPerPerson, 
                                         const boost::optional<People>& templatePeople)
  {
    if (floorAreaPerPerson < 0.0) {
      LOG(Error,"Space cannot set floorAreaPerPerson to " << floorAreaPerPerson 
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalPeople myPeople = getMySpaceLoadInstance<People,PeopleDefinition>(templatePeople);
    if (!myPeople) {
      LOG(Error,"The templatePeople object must be in the same Model as this Space.");
      return false;
    }

    // set space and number of people
    bool ok(true);
    myPeople->makeUnique();
    ok = myPeople->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myPeople->peopleDefinition().setSpaceFloorAreaperPerson(floorAreaPerPerson); 
    BOOST_ASSERT(ok);
    ok = myPeople->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other people from space
    PeopleVector allMyPeople = people();
    removeAllButOneSpaceLoadInstance<People>(allMyPeople,*myPeople);
    
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
        BOOST_FOREACH(People& peopleObject,allMyPeople) {
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

    BOOST_FOREACH(const Lights& light,lights()) {
      result += light.getLightingPower(area,numPeople);
    }
    BOOST_FOREACH(const Luminaire& luminaire, luminaires()) {
      result += luminaire.lightingPower();
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const Lights& light, spaceType->lights()) {
        result += light.getLightingPower(area,numPeople);
      }
      BOOST_FOREACH(const Luminaire& luminaire, spaceType->luminaires()) {
        result += luminaire.lightingPower();
      }
    }

    return result;
  }

  bool Space_Impl::setLightingPower(double lightingPower) {
    OptionalLights templateLights;
    LightsVector myLights = lights();
    if (myLights.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myLights = st->lights();
      }
    }
    if (!myLights.empty()) {
      templateLights = myLights[0];
    }
    return setLightingPower(lightingPower,templateLights);
  }

  bool Space_Impl::setLightingPower(double lightingPower, 
                                    const boost::optional<Lights>& templateLights)
  {
    if (lightingPower < 0.0) {
      LOG(Error,"Space cannot set lightingPower to " << lightingPower
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalLights myLights = getMySpaceLoadInstance<Lights,LightsDefinition>(templateLights);
    if (!myLights) {
      LOG(Error,"The templateLights object must be in the same Model as this Space.");
      return false;
    }

    // set space and lighting power
    bool ok(true);
    myLights->makeUnique();
    ok = myLights->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myLights->lightsDefinition().setLightingLevel(lightingPower); BOOST_ASSERT(ok);
    ok = myLights->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other lights from space
    LightsVector allMyLights = lights();
    removeAllButOneSpaceLoadInstance<Lights>(allMyLights,*myLights);
    // remove all luminaires from space
    LuminaireVector allMyLuminaires = luminaires();
    BOOST_FOREACH(Luminaire& luminaire, allMyLuminaires) {
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
        BOOST_FOREACH(Lights& light, allMyLights) {
          light.remove();
        }
        allMyLuminaires = spaceType->luminaires();
        BOOST_FOREACH(Luminaire& luminaire, allMyLuminaires) {
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

    BOOST_FOREACH(const Lights& light,lights()) {
      result += light.getPowerPerFloorArea(area,numPeople);
    }
    BOOST_FOREACH(const Luminaire& luminaire, luminaires()) {
      result += luminaire.getPowerPerFloorArea(area);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const Lights& light, spaceType->lights()) {
        result += light.getPowerPerFloorArea(area,numPeople);
      }
      BOOST_FOREACH(const Luminaire& luminaire, spaceType->luminaires()) {
        result += luminaire.getPowerPerFloorArea(area);
      }
    }

    return result;
  }

  bool Space_Impl::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea) {
    OptionalLights templateLights;
    LightsVector myLights = lights();
    if (myLights.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myLights = st->lights();
      }
    }
    if (!myLights.empty()) {
      templateLights = myLights[0];
    }
    return setLightingPowerPerFloorArea(lightingPowerPerFloorArea,templateLights);
  }

  bool Space_Impl::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, 
                                                const boost::optional<Lights>& templateLights)
  {
    if (lightingPowerPerFloorArea < 0.0) {
      LOG(Error,"Space cannot set lightingPowerPerFloorArea to " << lightingPowerPerFloorArea
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalLights myLights = getMySpaceLoadInstance<Lights,LightsDefinition>(templateLights);
    if (!myLights) {
      LOG(Error,"The templateLights object must be in the same Model as this Space.");
      return false;
    }

    // set space and lighting power
    bool ok(true);
    myLights->makeUnique();
    ok = myLights->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myLights->lightsDefinition().setWattsperSpaceFloorArea(lightingPowerPerFloorArea); 
    BOOST_ASSERT(ok);
    ok = myLights->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other lights from space
    LightsVector allMyLights = lights();
    removeAllButOneSpaceLoadInstance<Lights>(allMyLights,*myLights);
    // remove all luminaires from space
    LuminaireVector allMyLuminaires = luminaires();
    BOOST_FOREACH(Luminaire& luminaire, allMyLuminaires) {
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
        BOOST_FOREACH(Lights& light, allMyLights) {
          light.remove();
        }
        allMyLuminaires = spaceType->luminaires();
        BOOST_FOREACH(Luminaire& luminaire, allMyLuminaires) {
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

    BOOST_FOREACH(const Lights& light,lights()) {
      result += light.getPowerPerPerson(area,numPeople);
    }
    BOOST_FOREACH(const Luminaire& luminaire, luminaires()) {
      result += luminaire.getPowerPerPerson(numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const Lights& light, spaceType->lights()) {
        result += light.getPowerPerPerson(area,numPeople);
      }
      BOOST_FOREACH(const Luminaire& luminaire, spaceType->luminaires()) {
        result += luminaire.getPowerPerPerson(numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setLightingPowerPerPerson(double lightingPowerPerPerson) {
    OptionalLights templateLights;
    LightsVector myLights = lights();
    if (myLights.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myLights = st->lights();
      }
    }
    if (!myLights.empty()) {
      templateLights = myLights[0];
    }
    return setLightingPowerPerPerson(lightingPowerPerPerson,templateLights);
  }

  bool Space_Impl::setLightingPowerPerPerson(double lightingPowerPerPerson, 
                                             const boost::optional<Lights>& templateLights)
  {
    if (lightingPowerPerPerson < 0.0) {
      LOG(Error,"Space cannot set lightingPowerPerPerson to " << lightingPowerPerPerson
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalLights myLights = getMySpaceLoadInstance<Lights,LightsDefinition>(templateLights);
    if (!myLights) {
      LOG(Error,"The templateLights object must be in the same Model as this Space.");
      return false;
    }

    // set space and lighting power
    bool ok(true);
    myLights->makeUnique();
    ok = myLights->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myLights->lightsDefinition().setWattsperPerson(lightingPowerPerPerson); 
    BOOST_ASSERT(ok);
    ok = myLights->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other lights from space
    LightsVector allMyLights = lights();
    removeAllButOneSpaceLoadInstance<Lights>(allMyLights,*myLights);
    // remove all luminaires from space
    LuminaireVector allMyLuminaires = luminaires();
    BOOST_FOREACH(Luminaire& luminaire, allMyLuminaires) {
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
        BOOST_FOREACH(Lights& light, allMyLights) {
          light.remove();
        }
        allMyLuminaires = spaceType->luminaires();
        BOOST_FOREACH(Luminaire& luminaire, allMyLuminaires) {
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

    BOOST_FOREACH(const ElectricEquipment& equipment,electricEquipment()) {
      result += equipment.getDesignLevel(area,numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const ElectricEquipment& equipment,spaceType->electricEquipment()) {
        result += equipment.getDesignLevel(area,numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setElectricEquipmentPower(double electricEquipmentPower) {
    OptionalElectricEquipment templateEquipment;
    ElectricEquipmentVector myEquipment = electricEquipment();
    if (myEquipment.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myEquipment = st->electricEquipment();
      }
    }
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setElectricEquipmentPower(electricEquipmentPower,templateEquipment);
  }

  bool Space_Impl::setElectricEquipmentPower(
      double electricEquipmentPower, 
      const boost::optional<ElectricEquipment>& templateElectricEquipment)
  {
    if (electricEquipmentPower < 0.0) {
      LOG(Error,"Space cannot set electricEquipmentPower to " 
          << electricEquipmentPower << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalElectricEquipment myEquipment = 
        getMySpaceLoadInstance<ElectricEquipment,ElectricEquipmentDefinition>(templateElectricEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateElectricEquipment object must be in the same Model as this Space.");
      return false;
    }

    // set space and power level
    bool ok(true);
    myEquipment->makeUnique();
    ok = myEquipment->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myEquipment->electricEquipmentDefinition().setDesignLevel(electricEquipmentPower); 
    BOOST_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other electric equipment from space
    ElectricEquipmentVector allMyEquipment = electricEquipment();
    removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment,*myEquipment);
    
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
        BOOST_FOREACH(ElectricEquipment& equipment, allMyEquipment) {
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

    BOOST_FOREACH(const ElectricEquipment& equipment,electricEquipment()) {
      result += equipment.getPowerPerFloorArea(area,numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const ElectricEquipment& equipment,spaceType->electricEquipment()) {
        result += equipment.getPowerPerFloorArea(area,numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea)
  {
    OptionalElectricEquipment templateEquipment;
    ElectricEquipmentVector myEquipment = electricEquipment();
    if (myEquipment.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myEquipment = st->electricEquipment();
      }
    }
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea,templateEquipment);
  }

  bool Space_Impl::setElectricEquipmentPowerPerFloorArea(
      double electricEquipmentPowerPerFloorArea, 
      const boost::optional<ElectricEquipment>& templateElectricEquipment)
  {
    if (electricEquipmentPowerPerFloorArea < 0.0) {
      LOG(Error,"Space cannot set electricEquipmentPowerPerFloorArea to " 
          << electricEquipmentPowerPerFloorArea << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalElectricEquipment myEquipment = 
        getMySpaceLoadInstance<ElectricEquipment,ElectricEquipmentDefinition>(templateElectricEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateElectricEquipment object must be in the same Model as this Space.");
      return false;
    }

    // set space and power level
    bool ok(true);
    myEquipment->makeUnique();
    ok = myEquipment->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myEquipment->electricEquipmentDefinition().setWattsperSpaceFloorArea(electricEquipmentPowerPerFloorArea); 
    BOOST_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other electric equipment from space
    ElectricEquipmentVector allMyEquipment = electricEquipment();
    removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment,*myEquipment);
    
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
        BOOST_FOREACH(ElectricEquipment& equipment, allMyEquipment) {
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

    BOOST_FOREACH(const ElectricEquipment& equipment,electricEquipment()) {
      result += equipment.getPowerPerPerson(area,numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const ElectricEquipment& equipment,spaceType->electricEquipment()) {
        result += equipment.getPowerPerPerson(area,numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson) {
    OptionalElectricEquipment templateEquipment;
    ElectricEquipmentVector myEquipment = electricEquipment();
    if (myEquipment.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myEquipment = st->electricEquipment();
      }
    }
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson,templateEquipment);

  }

  bool Space_Impl::setElectricEquipmentPowerPerPerson(
      double electricEquipmentPowerPerPerson, 
      const boost::optional<ElectricEquipment>& templateElectricEquipment)
  {
    if (electricEquipmentPowerPerPerson < 0.0) {
      LOG(Error,"Space cannot set electricEquipmentPowerPerPerson to " 
          << electricEquipmentPowerPerPerson << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalElectricEquipment myEquipment = 
        getMySpaceLoadInstance<ElectricEquipment,ElectricEquipmentDefinition>(templateElectricEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateElectricEquipment object must be in the same Model as this Space.");
      return false;
    }

    // set space and power level
    bool ok(true);
    myEquipment->makeUnique();
    ok = myEquipment->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myEquipment->electricEquipmentDefinition().setWattsperPerson(electricEquipmentPowerPerPerson); 
    BOOST_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other electric equipment from space
    ElectricEquipmentVector allMyEquipment = electricEquipment();
    removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment,*myEquipment);
    
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
        BOOST_FOREACH(ElectricEquipment& equipment, allMyEquipment) {
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

    BOOST_FOREACH(const GasEquipment& equipment,gasEquipment()) {
      result += equipment.getDesignLevel(area,numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const GasEquipment& equipment,spaceType->gasEquipment()) {
        result += equipment.getDesignLevel(area,numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setGasEquipmentPower(double gasEquipmentPower) {
    OptionalGasEquipment templateEquipment;
    GasEquipmentVector myEquipment = gasEquipment();
    if (myEquipment.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myEquipment = st->gasEquipment();
      }
    }
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setGasEquipmentPower(gasEquipmentPower,templateEquipment);
  }

  bool Space_Impl::setGasEquipmentPower(
      double gasEquipmentPower, 
      const boost::optional<GasEquipment>& templateGasEquipment)
  {
    if (gasEquipmentPower < 0.0) {
      LOG(Error,"Space cannot set gasEquipmentPower to " 
          << gasEquipmentPower << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalGasEquipment myEquipment = 
        getMySpaceLoadInstance<GasEquipment,GasEquipmentDefinition>(templateGasEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateGasEquipment object must be in the same Model as this Space.");
      return false;
    }

    // set space and power level
    bool ok(true);
    myEquipment->makeUnique();
    ok = myEquipment->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myEquipment->gasEquipmentDefinition().setDesignLevel(gasEquipmentPower); 
    BOOST_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other gas equipment from space
    GasEquipmentVector allMyEquipment = gasEquipment();
    removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment,*myEquipment);
    
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
        BOOST_FOREACH(GasEquipment& equipment, allMyEquipment) {
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

    BOOST_FOREACH(const GasEquipment& equipment,gasEquipment()) {
      result += equipment.getPowerPerFloorArea(area,numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const GasEquipment& equipment,spaceType->gasEquipment()) {
        result += equipment.getPowerPerFloorArea(area,numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea)
  {
    OptionalGasEquipment templateEquipment;
    GasEquipmentVector myEquipment = gasEquipment();
    if (myEquipment.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myEquipment = st->gasEquipment();
      }
    }
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea,templateEquipment);
  }

  bool Space_Impl::setGasEquipmentPowerPerFloorArea(
      double gasEquipmentPowerPerFloorArea, 
      const boost::optional<GasEquipment>& templateGasEquipment)
  {
    if (gasEquipmentPowerPerFloorArea < 0.0) {
      LOG(Error,"Space cannot set gasEquipmentPowerPerFloorArea to " 
          << gasEquipmentPowerPerFloorArea << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalGasEquipment myEquipment = 
        getMySpaceLoadInstance<GasEquipment,GasEquipmentDefinition>(templateGasEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateGasEquipment object must be in the same Model as this Space.");
      return false;
    }

    // set space and power level
    bool ok(true);
    myEquipment->makeUnique();
    ok = myEquipment->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myEquipment->gasEquipmentDefinition().setWattsperSpaceFloorArea(gasEquipmentPowerPerFloorArea); 
    BOOST_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other gas equipment from space
    GasEquipmentVector allMyEquipment = gasEquipment();
    removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment,*myEquipment);
    
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
        BOOST_FOREACH(GasEquipment& equipment, allMyEquipment) {
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

    BOOST_FOREACH(const GasEquipment& equipment,gasEquipment()) {
      result += equipment.getPowerPerPerson(area,numPeople);
    }

    if (OptionalSpaceType spaceType = this->spaceType()) {
      BOOST_FOREACH(const GasEquipment& equipment,spaceType->gasEquipment()) {
        result += equipment.getPowerPerPerson(area,numPeople);
      }
    }

    return result;
  }

  bool Space_Impl::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson) {
    OptionalGasEquipment templateEquipment;
    GasEquipmentVector myEquipment = gasEquipment();
    if (myEquipment.empty()) {
      if (OptionalSpaceType st  = spaceType()) {
        myEquipment = st->gasEquipment();
      }
    }
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson,templateEquipment);

  }

  bool Space_Impl::setGasEquipmentPowerPerPerson(
      double gasEquipmentPowerPerPerson, 
      const boost::optional<GasEquipment>& templateGasEquipment)
  {
    if (gasEquipmentPowerPerPerson < 0.0) {
      LOG(Error,"Space cannot set gasEquipmentPowerPerPerson to " 
          << gasEquipmentPowerPerPerson << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify Lights and LightsDefinition object
    OptionalGasEquipment myEquipment = 
        getMySpaceLoadInstance<GasEquipment,GasEquipmentDefinition>(templateGasEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateGasEquipment object must be in the same Model as this Space.");
      return false;
    }

    // set space and power level
    bool ok(true);
    myEquipment->makeUnique();
    ok = myEquipment->setSpace(getObject<Space>()); BOOST_ASSERT(ok);
    ok = myEquipment->gasEquipmentDefinition().setWattsperPerson(gasEquipmentPowerPerPerson); 
    BOOST_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); BOOST_ASSERT(ok);

    // remove all other gas equipment from space
    GasEquipmentVector allMyEquipment = gasEquipment();
    removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment,*myEquipment);
    
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
        BOOST_FOREACH(GasEquipment& equipment, allMyEquipment) {
          equipment.remove();
        }
      }
    }

    return true;
  }

  void Space_Impl::hardApplySpaceType(bool hardSizeLoads)
  {
    Model model = this->model();
    Space space = this->getObject<Space>();

    boost::optional<SpaceType> spaceType = this->spaceType();
    if (spaceType){

      // Clone the space type if there is a space type
      spaceType = spaceType->clone(model).cast<SpaceType>();

      // Hard apply schedules to space type loads if there is a space type
      spaceType->hardApplySpaceLoadSchedules();

      // Move space loads from space type to space if there is a space type
      BOOST_FOREACH(ModelObject child, spaceType->children()){
        if (child.optionalCast<SpaceLoad>()){
          bool test = child.cast<SpaceLoad>().setSpace(space);
          BOOST_ASSERT(test);
        }
      }

      // Merge space type default constructions with space default constructions if there is a space type
      boost::optional<DefaultConstructionSet> spaceTypeDefaultConstructionSet = spaceType->defaultConstructionSet();
      if (spaceTypeDefaultConstructionSet){
        boost::optional<DefaultConstructionSet> defaultConstructionSet = this->defaultConstructionSet();
        if (defaultConstructionSet){
          defaultConstructionSet->merge(*spaceTypeDefaultConstructionSet);
        }else{
          this->setDefaultConstructionSet(*spaceTypeDefaultConstructionSet);
        }
      }

    }else{
      // create a new space type
      spaceType = SpaceType(model);
    }

    // always set space type to prevent picking up building level space type
    space.setSpaceType(*spaceType);

    // Hard apply schedules to space loads, do this before clearing space type default schedule set
    this->hardApplySpaceLoadSchedules();

    // If hard sizing loads, make each space load refer to unique definition and hard size it
    if (hardSizeLoads){
      BOOST_FOREACH(ModelObject child, this->children()){
        if (child.optionalCast<SpaceLoad>()){
          child.cast<SpaceLoad>().hardSize();
        }
      }
    }

    // clean the cloned space type
    spaceType->resetRenderingColor();
    spaceType->resetDefaultConstructionSet();
    spaceType->resetDefaultScheduleSet();
  }

  void Space_Impl::hardApplySpaceLoadSchedules()
  {
    BOOST_FOREACH(ModelObject child, this->children()){
      if (child.optionalCast<SpaceLoad>()){
        child.cast<SpaceLoad>().hardApplySchedules();
      }
    }
  }

  void Space_Impl::hardApplyConstructions() {

    BOOST_FOREACH(ShadingSurfaceGroup shadingSurfaceGroup, this->shadingSurfaceGroups()){
      BOOST_FOREACH(ShadingSurface shadingSurface, shadingSurfaceGroup.shadingSurfaces()){
        boost::optional<ConstructionBase> construction = shadingSurface.construction();
        if (construction){
          shadingSurface.setConstruction(*construction);
        }
      }
    }

    BOOST_FOREACH(InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup, this->interiorPartitionSurfaceGroups()){
      BOOST_FOREACH(InteriorPartitionSurface interiorPartitionSurface, interiorPartitionSurfaceGroup.interiorPartitionSurfaces()){
        boost::optional<ConstructionBase> construction = interiorPartitionSurface.construction();
        if (construction){
          interiorPartitionSurface.setConstruction(*construction);
        }
      }
    }

    BOOST_FOREACH(Surface surface, this->surfaces()){
      boost::optional<ConstructionBase> construction = surface.construction();
      if (construction){
        surface.setConstruction(*construction);
      }else{
        boost::optional<Surface> adjacentSurface = surface.adjacentSurface();
        if (adjacentSurface){
          construction = adjacentSurface->construction();
          if (construction){
            surface.setConstruction(*construction);
          }
        }
      }

      BOOST_FOREACH(SubSurface subSurface, surface.subSurfaces()){
        boost::optional<ConstructionBase> construction = subSurface.construction();
        if (construction){
          subSurface.setConstruction(*construction);
        }else{
          boost::optional<SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface();
          if (adjacentSubSurface){
            construction = adjacentSubSurface->construction();
            if (construction){
              subSurface.setConstruction(*construction);
            }
          }
        }
      }
    }
  }

  void Space_Impl::unmatchSurfaces()
  {
    BOOST_FOREACH(Surface surface, this->surfaces()){
      boost::optional<Surface> adjacentSurface = surface.adjacentSurface();
      if (adjacentSurface){
        surface.resetAdjacentSurface();
        adjacentSurface->resetAdjacentSurface();
      }

      BOOST_FOREACH(SubSurface subSurface, surface.subSurfaces()){
        boost::optional<SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface();
        if (adjacentSubSurface){
          subSurface.resetAdjacentSubSurface();
          adjacentSubSurface->resetAdjacentSubSurface();
        }
      }
    }
  }

  void Space_Impl::matchSurfaces(Space& other)
  {

    if (this->handle() == other.handle()){
      return;
    }

    // transform from other to this coordinates
    Transformation transformation = this->transformation().inverse()*other.transformation();

    BOOST_FOREACH(Surface surface, this->surfaces()){

      std::vector<Point3d> vertices = surface.vertices();

      boost::optional<Vector3d> outwardNormal = getOutwardNormal(vertices);
      if (!outwardNormal){
        continue;
      }

      BOOST_FOREACH(Surface otherSurface, other.surfaces()){

        std::vector<Point3d> otherVertices = transformation*otherSurface.vertices();

        boost::optional<Vector3d> otherOutwardNormal = getOutwardNormal(otherVertices);
        if (!otherOutwardNormal){
          continue;
        }

        double dot = outwardNormal->dot(*otherOutwardNormal);

        if (dot > -0.98){
          continue;
        }

        std::reverse(otherVertices.begin(), otherVertices.end());

        if (circularEqual(vertices, otherVertices, 0.001)){

          // TODO: check constructions?
          surface.setAdjacentSurface(otherSurface);
          otherSurface.setAdjacentSurface(surface);

          // once surfaces are matched, check subsurfaces
          BOOST_FOREACH(SubSurface subSurface, surface.subSurfaces()){

            vertices = subSurface.vertices();

            BOOST_FOREACH(SubSurface otherSubSurface, otherSurface.subSurfaces()){

              otherVertices = transformation*otherSubSurface.vertices();
              std::reverse(otherVertices.begin(), otherVertices.end());

              if (circularEqual(vertices, otherVertices, 0.001)){

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

  std::vector<Surface> Space_Impl::findSurfaces(boost::optional<double> minDegreesFromNorth,
                                                boost::optional<double> maxDegreesFromNorth,
                                                boost::optional<double> minDegreesTilt,
                                                boost::optional<double> maxDegreesTilt,
                                                double tol)
  {
    std::vector<Surface> result;
    std::vector<PlanarSurface> planarSurfaces;

    BOOST_FOREACH(const Surface& surface, this->surfaces()){
      planarSurfaces.push_back(surface);
    }

    planarSurfaces = PlanarSurface::findPlanarSurfaces(planarSurfaces, minDegreesFromNorth, maxDegreesFromNorth, minDegreesTilt, maxDegreesTilt, tol);

    BOOST_FOREACH(const PlanarSurface& planarSurface, planarSurfaces){
      result.push_back(planarSurface.cast<Surface>());
    }

    return result;
  }

  openstudio::Quantity Space_Impl::directionofRelativeNorth_SI() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::DirectionofRelativeNorth,true,false);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity Space_Impl::directionofRelativeNorth_IP() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::DirectionofRelativeNorth,true,true);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  bool Space_Impl::setDirectionofRelativeNorth(const Quantity& directionofRelativeNorth)
  {
    bool result = setQuantity(OS_SpaceFields::DirectionofRelativeNorth, directionofRelativeNorth);
    return result;
  }

  openstudio::Quantity Space_Impl::xOrigin_SI() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::XOrigin,true,false);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity Space_Impl::xOrigin_IP() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::XOrigin,true,true);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  bool Space_Impl::setXOrigin(const Quantity& xOrigin)
  {
    bool result = setQuantity(OS_SpaceFields::XOrigin, xOrigin);
    return result;
  }

  openstudio::Quantity Space_Impl::yOrigin_SI() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::YOrigin,true,false);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity Space_Impl::yOrigin_IP() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::YOrigin,true,true);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  bool Space_Impl::setYOrigin(const Quantity& yOrigin)
  {
    bool result = setQuantity(OS_SpaceFields::YOrigin, yOrigin);
    return result;
  }

  openstudio::Quantity Space_Impl::zOrigin_SI() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::ZOrigin,true,false);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity Space_Impl::zOrigin_IP() const
  {
    OSOptionalQuantity value = getQuantity(OS_SpaceFields::ZOrigin,true,true);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  bool Space_Impl::setZOrigin(const Quantity& zOrigin)
  {
    bool result = setQuantity(OS_SpaceFields::ZOrigin, zOrigin);
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

  bool Space_Impl::setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSpaceType intermediate = modelObject->optionalCast<SpaceType>();
      if (intermediate) {
        return setSpaceType(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetSpaceType();
    }
    return true;
  }

  bool Space_Impl::setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDefaultConstructionSet intermediate = modelObject->optionalCast<DefaultConstructionSet>();
      if (intermediate) {
        return setDefaultConstructionSet(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetDefaultConstructionSet();
    }
    return true;
  }

  bool Space_Impl::setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDefaultScheduleSet intermediate = modelObject->optionalCast<DefaultScheduleSet>();
      if (intermediate) {
        return setDefaultScheduleSet(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
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
  boost::optional<T> Space_Impl::getMySpaceLoadInstance(
      const boost::optional<T>& templateSpaceLoadInstance)
  {
    boost::optional<T> result;
    if (!templateSpaceLoadInstance) {
      TDef myDefinition(model());
      T myInstance(myDefinition);
      result = myInstance;
    }
    else {
      boost::optional<Space> templateSpace = templateSpaceLoadInstance->space();
      if (templateSpace && (templateSpace.get() == getObject<Space>()))
      {
        result = templateSpaceLoadInstance;
      }
      else if (templateSpaceLoadInstance->model() != model()) {
        return boost::none;
      }
      else if (!templateSpace && !templateSpaceLoadInstance->spaceType()) {
        result = templateSpaceLoadInstance;
      }
      else {
        T myInstance = templateSpaceLoadInstance->clone().template cast<T>();
        result = myInstance;
      }
    }
    return result;
  }

  template <typename T>
  void Space_Impl::removeAllButOneSpaceLoadInstance(std::vector<T>& instances, 
                                                    const T& instanceToKeep) 
  {
    int count(0);
    for (typename std::vector<T>::iterator it = instances.begin(), itEnd = instances.end();
         it != itEnd; ++it)
    {
      if (*it == instanceToKeep) {
        ++count;
        continue;
      }
      it->remove();
    }
    BOOST_ASSERT(count == 1);
  }

  std::vector<Point3d> Space_Impl::floorPrint() const
  {
    // get all floors
    double xmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::min();
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::min();
    boost::optional<double> z;
    std::vector<Surface> floors;
    BOOST_FOREACH(const Surface& surface, this->surfaces()){
      if (surface.vertices().size() < 3){
        LOG(Warn, "Skipping floor with fewer than 3 vertices");
        continue;
      }
      if (istringEqual("Floor", surface.surfaceType())){
        floors.push_back(surface);
        BOOST_FOREACH(const Point3d& point, surface.vertices()){
          if (!z){
            xmin = std::min(xmin, point.x());
            xmax = std::max(xmax, point.x());
            ymin = std::min(ymin, point.y());
            ymax = std::max(ymax, point.y());
            z = point.z();
          }else if (*z != point.z()){
            LOG(Error, "All floor surfaces must lie on the same x, y plane to compute space floor print");
            return std::vector<Point3d>();
          }
        }
      }
    }

    std::vector<Point3d> result;

    if (floors.empty()){
      LOG(Error, "No floor surfaces found to compute space floor print");
      return std::vector<Point3d>();

    }else if (floors.size() == 1){
      // just return this floors vertices
      result = floors[0].vertices();

      // remove colinear points
      result = removeColinear(result);

      // reorder the points 
      result = reorderULC(result);
      
      // remove additional colinear points that occur after reordering
      result = removeColinear(result);

    }else{

      BOOST_ASSERT(z);

      typedef boost::geometry::model::d2::point_xy<double> BoostPoint;
      typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
      typedef boost::geometry::model::ring<BoostPoint> BoostRing;
      typedef boost::geometry::model::multi_polygon<BoostPolygon> BoostMultiPolygon;

      BoostMultiPolygon boostResult;

      try{
        BOOST_FOREACH(const Surface& floor, floors){
          BoostPolygon boostPolygon;
          std::vector<Point3d> vertices = floor.vertices();
          BOOST_FOREACH(const Point3d& point, vertices){
            boost::geometry::append(boostPolygon, boost::make_tuple(point.x(), point.y()));
          }
          boost::geometry::append(boostPolygon, boost::make_tuple(vertices[0].x(), vertices[0].y()));

          BoostMultiPolygon tmpResult;
          boost::geometry::union_(boostResult, boostPolygon, tmpResult);
          boostResult = tmpResult;
        }
      }catch(const std::exception& e){
        LOG(Error, e.what());
        boostResult.clear();
      }

      if (boostResult.size() != 1){
        return std::vector<Point3d>();
      }

      BOOST_FOREACH(const BoostPoint& boostPoint, boostResult[0].outer()){
        Point3d point(boostPoint.x(), boostPoint.y(), *z);
        result.push_back(point);
      }
      result.pop_back();

      // remove colinear points
      result = removeColinear(result);

      // reorder the points 
      result = reorderULC(result);
      
      // remove additional colinear points that occur after reordering
      result = removeColinear(result);

      Point3d lastOuterVertex = result.back();

      BOOST_FOREACH(const BoostRing& boostInner, boostResult[0].inners()){

        std::vector<Point3d> innerLoop;
        BOOST_REVERSE_FOREACH(const BoostPoint& boostPoint, boostInner){
          Point3d point(boostPoint.x(), boostPoint.y(), *z);
          innerLoop.push_back(point);
        }

        // remove colinear points
        innerLoop = removeColinear(innerLoop);

        // reorder the points 
        innerLoop = reorderULC(innerLoop);
        
        // remove additional colinear points that occur after reordering
        innerLoop = removeColinear(innerLoop);

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

    // remove colinear points
    //result = removeColinear(result);

    // reorder the points 
    //result = reorderULC(result);
    
    // remove additional colinear points that occur after reordering
    //result = removeColinear(result);

    return result;
  }

} // detail

Space::Space(const Model& model)
  : PlanarSurfaceGroup(Space::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::Space_Impl>());
}

IddObjectType Space::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Space);
  return result;
}

boost::optional<Space> Space::fromFloorPrint(const std::vector<Point3d>& floorPrint, double floorHeight, Model& model)
{
  // check floor height
  if (floorHeight <= 0){
    LOG(Error, "Cannot create a space with floorHeight " << floorHeight << ".");
    return boost::none;
  }

  // check floor print
  unsigned numPoints = floorPrint.size();
  if (numPoints < 3){
    LOG(Error, "Cannot create a space for floorPrint of size " << floorPrint.size() << ".");
    return boost::none;
  }

  double z = floorPrint[0].z();
  double tol = 0.000001;
  BOOST_FOREACH(const Point3d& point, floorPrint){
    if (abs(point.z()-z) > tol){
      LOG(Error, "Inconsistent z height in floorPrint.");
      return boost::none;
    }
  }

  boost::optional<Vector3d> outwardNormal = getOutwardNormal(floorPrint);
  if (!outwardNormal){
    LOG(Error, "Cannot compute outwardNormal for floorPrint.");
    return boost::none;
  }

  if (outwardNormal->z() > -1 + tol){
    LOG(Error, "OutwardNormal of floorPrint must point down to create space.");
    return boost::none;
  }

  // we are good to go, create the space
  Space space(model);

  // create the floor
  std::vector<Point3d> points;
  for (std::vector<Point3d>::const_iterator it = floorPrint.begin(), itend = floorPrint.end(); it != itend; ++it){
    points.push_back(Point3d(it->x(), it->y(), z));
  }
  Surface floor(points, model);
  floor.setSpace(space);

  // create each wall
  for (unsigned i = 1; i <= numPoints; ++i){
    points.clear();
    points.push_back(Point3d(floorPrint[i % numPoints].x(), floorPrint[i % numPoints].y(), z + floorHeight));
    points.push_back(Point3d(floorPrint[i % numPoints].x(), floorPrint[i % numPoints].y(), z));
    points.push_back(Point3d(floorPrint[i - 1].x(), floorPrint[i - 1].y(), z));
    points.push_back(Point3d(floorPrint[i - 1].x(), floorPrint[i - 1].y(), z + floorHeight));

    Surface wall(points, model);
    wall.setSpace(space);
  }

  // create the roofCeiling
  points.clear();
  for (std::vector<Point3d>::const_reverse_iterator rit = floorPrint.rbegin(), ritend = floorPrint.rend(); rit != ritend; ++rit){
    points.push_back(Point3d(rit->x(), rit->y(), z + floorHeight));
  }
  Surface roofCeiling(points, model);
  roofCeiling.setSpace(space);

  return space;
}

bool Space::partofTotalFloorArea() const {
  return getImpl<detail::Space_Impl>()->partofTotalFloorArea();
}

bool Space::isPartofTotalFloorAreaDefaulted() const {
  return getImpl<detail::Space_Impl>()->isPartofTotalFloorAreaDefaulted();
}

void Space::setPartofTotalFloorArea(bool partofTotalFloorArea) {
  getImpl<detail::Space_Impl>()->setPartofTotalFloorArea(partofTotalFloorArea);
}

void Space::resetPartofTotalFloorArea() {
  getImpl<detail::Space_Impl>()->resetPartofTotalFloorArea();
}

boost::optional<SpaceType> Space::spaceType() const
{
  return getImpl<detail::Space_Impl>()->spaceType();
}

bool Space::isSpaceTypeDefaulted() const
{
  return getImpl<detail::Space_Impl>()->isSpaceTypeDefaulted();
}

bool Space::setSpaceType(const SpaceType& spaceType)
{
  return getImpl<detail::Space_Impl>()->setSpaceType(spaceType);
}

void Space::resetSpaceType()
{
  getImpl<detail::Space_Impl>()->resetSpaceType();
}

boost::optional<DefaultConstructionSet> Space::defaultConstructionSet() const
{
  return getImpl<detail::Space_Impl>()->defaultConstructionSet();
}

boost::optional<ConstructionBase> Space::getDefaultConstruction(const PlanarSurface& planarSurface) const
{
  return getImpl<detail::Space_Impl>()->getDefaultConstruction(planarSurface);
}

boost::optional<std::pair<ConstructionBase, int> > Space::getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const
{
  return getImpl<detail::Space_Impl>()->getDefaultConstructionWithSearchDistance(planarSurface);
}

bool Space::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
{
  return getImpl<detail::Space_Impl>()->setDefaultConstructionSet(defaultConstructionSet);
}

void Space::resetDefaultConstructionSet()
{
  getImpl<detail::Space_Impl>()->resetDefaultConstructionSet();
}

boost::optional<DefaultScheduleSet> Space::defaultScheduleSet() const
{
  return getImpl<detail::Space_Impl>()->defaultScheduleSet();
}

boost::optional<Schedule> Space::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const
{
  return getImpl<detail::Space_Impl>()->getDefaultSchedule(defaultScheduleType);
}

bool Space::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
{
  return getImpl<detail::Space_Impl>()->setDefaultScheduleSet(defaultScheduleSet);
}

void Space::resetDefaultScheduleSet()
{
  getImpl<detail::Space_Impl>()->resetDefaultScheduleSet();
}

boost::optional<ThermalZone> Space::thermalZone() const {
  return getImpl<detail::Space_Impl>()->thermalZone();
}

bool Space::setThermalZone(ThermalZone& thermalZone)
{
  return getImpl<detail::Space_Impl>()->setThermalZone(thermalZone);
}

void Space::resetThermalZone()
{
  getImpl<detail::Space_Impl>()->resetThermalZone();
}

boost::optional<BuildingStory> Space::buildingStory() const {
  return getImpl<detail::Space_Impl>()->buildingStory();
}

bool Space::setBuildingStory(const BuildingStory& buildingStory)
{
  return getImpl<detail::Space_Impl>()->setBuildingStory(buildingStory);
}

void Space::resetBuildingStory()
{
  getImpl<detail::Space_Impl>()->resetBuildingStory();
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

boost::optional<DesignSpecificationOutdoorAir> Space::designSpecificationOutdoorAir() const
{
  return getImpl<detail::Space_Impl>()->designSpecificationOutdoorAir();
}

bool Space::isDesignSpecificationOutdoorAirDefaulted() const
{
  return getImpl<detail::Space_Impl>()->isDesignSpecificationOutdoorAirDefaulted();
}

bool Space::setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir)
{
  return getImpl<detail::Space_Impl>()->setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
}

void Space::resetDesignSpecificationOutdoorAir()
{
  return getImpl<detail::Space_Impl>()->resetDesignSpecificationOutdoorAir();
}

int Space::multiplier() const {
  return getImpl<detail::Space_Impl>()->multiplier();
}

double Space::floorArea() const {
  return getImpl<detail::Space_Impl>()->floorArea();
}

double Space::exteriorArea() const {
  return getImpl<detail::Space_Impl>()->exteriorArea();
}

double Space::volume() const {
  return getImpl<detail::Space_Impl>()->volume();
}

double Space::numberOfPeople() const {
  return getImpl<detail::Space_Impl>()->numberOfPeople();
}

bool Space::setNumberOfPeople(double numberOfPeople) {
  return getImpl<detail::Space_Impl>()->setNumberOfPeople(numberOfPeople);
}

bool Space::setNumberOfPeople(double numberOfPeople, const People& templatePeople) {
  return getImpl<detail::Space_Impl>()->setNumberOfPeople(numberOfPeople,templatePeople);
}

double Space::peoplePerFloorArea() const {
  return getImpl<detail::Space_Impl>()->peoplePerFloorArea();
}

bool Space::setPeoplePerFloorArea(double peoplePerFloorArea) {
  return getImpl<detail::Space_Impl>()->setPeoplePerFloorArea(peoplePerFloorArea);
}

bool Space::setPeoplePerFloorArea(double peoplePerFloorArea, const People& templatePeople) {
  return getImpl<detail::Space_Impl>()->setPeoplePerFloorArea(peoplePerFloorArea,templatePeople);
}

double Space::floorAreaPerPerson() const {
  return getImpl<detail::Space_Impl>()->floorAreaPerPerson();
}

bool Space::setFloorAreaPerPerson(double floorAreaPerPerson) {
  return getImpl<detail::Space_Impl>()->setFloorAreaPerPerson(floorAreaPerPerson);
}

bool Space::setFloorAreaPerPerson(double floorAreaPerPerson, const People& templatePeople) {
  return getImpl<detail::Space_Impl>()->setFloorAreaPerPerson(floorAreaPerPerson,templatePeople);
}

double Space::lightingPower() const {
  return getImpl<detail::Space_Impl>()->lightingPower();
}

bool Space::setLightingPower(double lightingPower) {
  return getImpl<detail::Space_Impl>()->setLightingPower(lightingPower);
}

bool Space::setLightingPower(double lightingPower, const Lights& templateLights) {
  return getImpl<detail::Space_Impl>()->setLightingPower(lightingPower,templateLights);
}

double Space::lightingPowerPerFloorArea() const {
  return getImpl<detail::Space_Impl>()->lightingPowerPerFloorArea();
}

bool Space::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea) {
  return getImpl<detail::Space_Impl>()->setLightingPowerPerFloorArea(lightingPowerPerFloorArea);
}

bool Space::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, const Lights& templateLights) {
  return getImpl<detail::Space_Impl>()->setLightingPowerPerFloorArea(lightingPowerPerFloorArea,templateLights);
}

double Space::lightingPowerPerPerson() const {
  return getImpl<detail::Space_Impl>()->lightingPowerPerPerson();
}

bool Space::setLightingPowerPerPerson(double lightingPowerPerPerson) {
  return getImpl<detail::Space_Impl>()->setLightingPowerPerPerson(lightingPowerPerPerson);
}

bool Space::setLightingPowerPerPerson(double lightingPowerPerPerson, const Lights& templateLights) {
  return getImpl<detail::Space_Impl>()->setLightingPowerPerPerson(lightingPowerPerPerson,templateLights);
}

double Space::electricEquipmentPower() const {
  return getImpl<detail::Space_Impl>()->electricEquipmentPower();
}

bool Space::setElectricEquipmentPower(double electricEquipmentPower) {
  return getImpl<detail::Space_Impl>()->setElectricEquipmentPower(electricEquipmentPower);
}

bool Space::setElectricEquipmentPower(double electricEquipmentPower, 
                               const ElectricEquipment& templateElectricEquipment)
{
  return getImpl<detail::Space_Impl>()->setElectricEquipmentPower(electricEquipmentPower,templateElectricEquipment);
}

double Space::electricEquipmentPowerPerFloorArea() const {
  return getImpl<detail::Space_Impl>()->electricEquipmentPowerPerFloorArea();
}

bool Space::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea) {
  return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea);
}

bool Space::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea, 
                                           const ElectricEquipment& templateElectricEquipment)
{
  return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea,templateElectricEquipment);
}

double Space::electricEquipmentPowerPerPerson() const {
  return getImpl<detail::Space_Impl>()->electricEquipmentPowerPerPerson();
}

bool Space::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson) {
  return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson);
}

bool Space::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson, 
                                        const ElectricEquipment& templateElectricEquipment)
{
  return getImpl<detail::Space_Impl>()->setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson,templateElectricEquipment);
}

double Space::gasEquipmentPower() const {
  return getImpl<detail::Space_Impl>()->gasEquipmentPower();
}

bool Space::setGasEquipmentPower(double gasEquipmentPower) {
  return getImpl<detail::Space_Impl>()->setGasEquipmentPower(gasEquipmentPower);
}

bool Space::setGasEquipmentPower(double gasEquipmentPower, 
                          const GasEquipment& templateGasEquipment)
{
  return getImpl<detail::Space_Impl>()->setGasEquipmentPower(gasEquipmentPower,templateGasEquipment);
}

double Space::gasEquipmentPowerPerFloorArea() const {
  return getImpl<detail::Space_Impl>()->gasEquipmentPowerPerFloorArea();
}

bool Space::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea) {
  return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea);
}

bool Space::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea, 
                                      const GasEquipment& templateGasEquipment)
{
  return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea,templateGasEquipment);
}

double Space::gasEquipmentPowerPerPerson() const {
  return getImpl<detail::Space_Impl>()->gasEquipmentPowerPerPerson();
}

bool Space::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson) {
  return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson);
}

bool Space::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson, 
                                   const GasEquipment& templateGasEquipment)
{
  return getImpl<detail::Space_Impl>()->setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson,templateGasEquipment);
}

void Space::hardApplySpaceType(bool hardSizeLoads){
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

std::vector <Surface> Space::findSurfaces(boost::optional<double> minDegreesFromNorth,
                                          boost::optional<double> maxDegreesFromNorth,
                                          boost::optional<double> minDegreesTilt,
                                          boost::optional<double> maxDegreesTilt,
                                          double tol)
{
  return getImpl<detail::Space_Impl>()->findSurfaces(minDegreesFromNorth, maxDegreesFromNorth, minDegreesTilt, maxDegreesTilt, tol);
}

std::vector<Point3d> Space::floorPrint() const
{
  return getImpl<detail::Space_Impl>()->floorPrint();
}

/// @cond
Space::Space(boost::shared_ptr<detail::Space_Impl> impl)
  : PlanarSurfaceGroup(impl)
{}
/// @endcond

void matchSurfaces(std::vector<Space>& spaces)
{
  std::vector<BoundingBox> bounds;
  BOOST_FOREACH(const Space& space, spaces){
    bounds.push_back(space.transformation()*space.boundingBox());
  }

  for (unsigned i = 0; i < spaces.size(); ++i){
    for (unsigned j = i+1; j < spaces.size(); ++j){
      if (!bounds[i].intersects(bounds[j])){
        continue;
      }
      spaces[i].matchSurfaces(spaces[j]);
    }
  }
}

void unmatchSurfaces(std::vector<Space>& spaces)
{
  BOOST_FOREACH(Space& space, spaces){
    space.unmatchSurfaces();
  }
}

} // model
} // openstudio

