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

#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/Meter.hpp>
#include <model/Meter_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>

#include <utilities/math/FloatCompare.hpp>
#include <utilities/data/DataEnums.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Transformation.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/units/QuantityConverter.hpp>

#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model {

namespace detail {

  Building_Impl::Building_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Building::iddObjectType());
  }

  Building_Impl::Building_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Building::iddObjectType());
  }

  Building_Impl::Building_Impl(const Building_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> Building_Impl::parent() const
  {
    return boost::optional<ParentObject>(this->facility());
  }

  std::vector<ModelObject> Building_Impl::children() const
  {
    std::vector<ModelObject> result;

    // meters
    MeterVector meters = this->meters();
    result.insert(result.end(),meters.begin(),meters.end());

    // building stories
    BuildingStoryVector stories = model().getConcreteModelObjects<BuildingStory>();
    result.insert(result.end(),stories.begin(),stories.end());

    // exterior shading groups
    ShadingSurfaceGroupVector shadingSurfaceGroups = this->shadingSurfaceGroups();
    result.insert(result.end(), shadingSurfaceGroups.begin(), shadingSurfaceGroups.end());

    // thermal zones
    ThermalZoneVector thermalZones = this->thermalZones();
    result.insert(result.end(), thermalZones.begin(), thermalZones.end());

    // spaces
    SpaceVector spaces = this->spaces();
    result.insert(result.end(), spaces.begin(), spaces.end());

    return result;
  }

  bool Building_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<Facility>()){
      return true;
    }
    return false;
  }

  std::vector<IddObjectType> Building_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Space);
    result.push_back(IddObjectType::OS_ShadingSurfaceGroup);
    result.push_back(IddObjectType::OS_ThermalZone);
    return result;
  }

  const std::vector<std::string>& Building_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Building_Impl::iddObjectType() const {
    return Building::iddObjectType();
  }

  double Building_Impl::northAxis() const {
    boost::optional<double> value = getDouble(OS_BuildingFields::NorthAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Building_Impl::isNorthAxisDefaulted() const {
    return isEmpty(OS_BuildingFields::NorthAxis);
  }

  double Building_Impl::nominalFloortoFloorHeight() const {
    boost::optional<double> value = getDouble(OS_BuildingFields::NominalFloortoFloorHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Building_Impl::isNominalFloortoFloorHeightDefaulted() const {
    return isEmpty(OS_BuildingFields::NominalFloortoFloorHeight);
  }

  boost::optional<int> Building_Impl::standardsNumberOfStories() const
  {
    boost::optional<int> value = getInt(OS_BuildingFields::StandardsNumberofStories, false);
    return value;
  }

  boost::optional<int> Building_Impl::standardsNumberOfAboveGroundStories() const
  {
    boost::optional<int> value = getInt(OS_BuildingFields::StandardsNumberofAboveGroundStories, false);
    return value;
  }

  boost::optional<std::string> Building_Impl::standardsBuildingType() const
  {
    return getString(OS_BuildingFields::StandardsBuildingType, false, true);
  }

  std::vector<std::string> Building_Impl::suggestedStandardsBuildingTypes() const
  {
    std::vector<std::string> result;
  
    boost::optional<std::string> standardsBuildingType = this->standardsBuildingType();

    // DLM: temp code, eventually get from StandardsLibrary
    Model tempModel;
    SpaceType tempSpaceType(tempModel);
    std::vector<std::string> tempSuggestions = tempSpaceType.suggestedStandardsBuildingTypes();
    BOOST_FOREACH(const std::string& suggestion, tempSuggestions){
      result.push_back(suggestion);
    }

    // include values from model
    BOOST_FOREACH(const SpaceType& other, this->model().getConcreteModelObjects<SpaceType>()){
      boost::optional<std::string> otherBuildingType = other.standardsBuildingType();
      if (otherBuildingType){
        result.push_back(*otherBuildingType);
      }
    }

    // remove standardsBuildingType
    IstringFind finder;
    if (standardsBuildingType){
      finder.addTarget(*standardsBuildingType);
    }
    std::vector<std::string>::iterator it = std::remove_if(result.begin(), result.end(), finder); 
    result.resize( std::distance(result.begin(),it) ); 

    // sort
    std::sort(result.begin(), result.end(), IstringCompare());

    // make unique
    // DLM: have to sort before calling unique, unique only works on consecutive elements
    it = std::unique(result.begin(), result.end(), IstringEqual()); 
    result.resize( std::distance(result.begin(),it) ); 

    // add current to front
    if (standardsBuildingType){
      result.insert(result.begin(), *standardsBuildingType);
    }

    return result;
  }

  void Building_Impl::setNorthAxis(double northAxis) {
    bool result = setDouble(OS_BuildingFields::NorthAxis, northAxis);
    OS_ASSERT(result);
  }

  void Building_Impl::resetNorthAxis() {
    bool result = setString(OS_BuildingFields::NorthAxis, "");
    OS_ASSERT(result);
  }

  bool Building_Impl::setNominalFloortoFloorHeight(double nominalFloortoFloorHeight) {
    bool result = setDouble(OS_BuildingFields::NominalFloortoFloorHeight, nominalFloortoFloorHeight);
    return result;
  }

  void Building_Impl::resetNominalFloortoFloorHeight() {
    bool result = setString(OS_BuildingFields::NominalFloortoFloorHeight, "");
    OS_ASSERT(result);
  }

  bool Building_Impl::setStandardsNumberOfStories(int value)
  {
    bool test = setInt(OS_BuildingFields::StandardsNumberofStories, value);
    return test;
  }

  void Building_Impl::resetStandardsNumberOfStories()
  {
    bool test = setString(OS_BuildingFields::StandardsNumberofStories, "");
    OS_ASSERT(test);
  }

  bool Building_Impl::setStandardsNumberOfAboveGroundStories(int value)
  {
    bool test = setInt(OS_BuildingFields::StandardsNumberofAboveGroundStories, value);
    return test;
  }

  void Building_Impl::resetStandardsNumberOfAboveGroundStories()
  {
    bool test = setString(OS_BuildingFields::StandardsNumberofAboveGroundStories, "");
    OS_ASSERT(test);
  }

  bool Building_Impl::setStandardsBuildingType(const std::string& standardsBuildingType)
  {
    bool result = setString(OS_BuildingFields::StandardsBuildingType, standardsBuildingType);
    OS_ASSERT(result);
    return result;
  }

  void Building_Impl::resetStandardsBuildingType()
  {
    bool test = setString(OS_BuildingFields::StandardsBuildingType, "");
    OS_ASSERT(test);
  }

  boost::optional<SpaceType> Building_Impl::spaceType() const
  {
    return getObject<ModelObject>().getModelObjectTarget<SpaceType>(OS_BuildingFields::SpaceTypeName);
  }

  bool Building_Impl::setSpaceType(const SpaceType& spaceType)
  {
    return setPointer(OS_BuildingFields::SpaceTypeName, spaceType.handle());
  }

  void Building_Impl::resetSpaceType()
  {
    bool test = setString(OS_BuildingFields::SpaceTypeName, "");
    OS_ASSERT(test);
  }

  boost::optional<DefaultConstructionSet> Building_Impl::defaultConstructionSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultConstructionSet>(OS_BuildingFields::DefaultConstructionSetName);
  }

  bool Building_Impl::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
  {
    return setPointer(OS_BuildingFields::DefaultConstructionSetName, defaultConstructionSet.handle());
  }

  void Building_Impl::resetDefaultConstructionSet()
  {
    setString(OS_BuildingFields::DefaultConstructionSetName, "");
  }

  boost::optional<DefaultScheduleSet> Building_Impl::defaultScheduleSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultScheduleSet>(OS_BuildingFields::DefaultScheduleSetName);
  }

  bool Building_Impl::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
  {
    return setPointer(OS_BuildingFields::DefaultScheduleSetName, defaultScheduleSet.handle());
  }

  void Building_Impl::resetDefaultScheduleSet()
  {
    setString(OS_BuildingFields::DefaultScheduleSetName, "");
  }

  MeterVector Building_Impl::meters() const
  {
    MeterVector result;
    MeterVector meters = this->model().getConcreteModelObjects<Meter>();
    BOOST_FOREACH(const Meter& meter, meters){
      if (meter.installLocationType() && (InstallLocationType::Building == meter.installLocationType().get().value())){
        result.push_back(meter);
      }
    }
    return result;
  }

  OptionalFacility Building_Impl::facility() const
  {
    return this->model().getOptionalUniqueModelObject<Facility>();
  }

  std::vector<Space> Building_Impl::spaces() const
  {
    // all spaces in workspace implicitly belong to building
    return this->model().getConcreteModelObjects<Space>();
  }

  ShadingSurfaceGroupVector Building_Impl::shadingSurfaceGroups() const
  {
    ShadingSurfaceGroupVector result;
    BOOST_FOREACH(ShadingSurfaceGroup shadingGroup, this->model().getConcreteModelObjects<ShadingSurfaceGroup>()){
      if (istringEqual(shadingGroup.shadingSurfaceType(), "Building")){
        result.push_back(shadingGroup);
      }
    }
    return result;
  }

  std::vector<ThermalZone> Building_Impl::thermalZones() const
  {
    // all thermal zones in workspace implicitly belong to building
    return this->model().getConcreteModelObjects<ThermalZone>();
  }

  std::vector<Surface> Building_Impl::exteriorWalls() const {
    SurfaceVector result;
    SurfaceVector candidates = model().getConcreteModelObjects<Surface>();
    BOOST_FOREACH(const Surface& candidate,candidates) {
      std::string surfaceType = candidate.surfaceType();
      std::string outsideBoundaryCondition = candidate.outsideBoundaryCondition();
      if (openstudio::istringEqual(surfaceType, "Wall") && openstudio::istringEqual(outsideBoundaryCondition, "Outdoors")) {
        result.push_back(candidate);
      }
    }
    return result;
  }

  std::vector<Surface> Building_Impl::roofs() const {
    SurfaceVector result;
    SurfaceVector candidates = model().getConcreteModelObjects<Surface>();
    BOOST_FOREACH(const Surface& candidate,candidates) {
      std::string surfaceType = candidate.surfaceType();
      std::string outsideBoundaryCondition = candidate.outsideBoundaryCondition();
      if (openstudio::istringEqual(surfaceType, "RoofCeiling") && openstudio::istringEqual(outsideBoundaryCondition, "Outdoors")) {
        result.push_back(candidate);
      }
    }
    return result;
  }

  double Building_Impl::floorArea() const
  {
    double result = 0;
    BOOST_FOREACH(const Space& space, spaces()){
      bool partofTotalFloorArea = space.partofTotalFloorArea();
      if (partofTotalFloorArea) {
        result += space.multiplier() * space.floorArea();
      }
    }
    return result;
  }

  boost::optional<double> Building_Impl::conditionedFloorArea() const
  {
    boost::optional<double> result;

    BOOST_FOREACH(const ThermalZone& thermalZone, thermalZones()){
      boost::optional<std::string> isConditioned = thermalZone.isConditioned();

      if (isConditioned) {

        if (!result){
          result = 0;
        }

        if (istringEqual("Yes", *isConditioned)){
          BOOST_FOREACH(const Space& space, thermalZone.spaces()){
            bool partofTotalFloorArea = space.partofTotalFloorArea();
            if (partofTotalFloorArea) {
              result = *result + space.multiplier() * space.floorArea();
            }
          }
        }
      }
    }

    return result;
  }

  double Building_Impl::exteriorSurfaceArea() const {
    double result(0.0);
    BOOST_FOREACH(const Surface& surface,model().getModelObjects<Surface>()) {
      OptionalSpace space = surface.space();
      std::string outsideBoundaryCondition = surface.outsideBoundaryCondition();
      if (space && openstudio::istringEqual(outsideBoundaryCondition, "Outdoors")) {
        result += surface.grossArea() * space->multiplier();
      }
    }
    return result;
  }

  double Building_Impl::exteriorWallArea() const {
    double result(0.0);
    BOOST_FOREACH(const Surface& exteriorWall,exteriorWalls()) {
      if (OptionalSpace space = exteriorWall.space()) {
        result += exteriorWall.grossArea() * space->multiplier();
      }
    }
    return result;
  }

  double Building_Impl::airVolume() const {
    double result(0.0);
    BOOST_FOREACH(const Space& space, spaces()) {
      result += space.volume() * space.multiplier();
    }
    return result;
  }

  double Building_Impl::numberOfPeople() const {
    double result(0.0);
    BOOST_FOREACH(const Space& space, spaces()) {
      result += space.numberOfPeople() * space.multiplier();
    }
    return result;
  }

  double Building_Impl::peoplePerFloorArea() const {
    double area = floorArea();
    double np = numberOfPeople();
    if (equal(area,0.0)) {
      if (equal(np,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].peoplePerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return np / area;
  }

  double Building_Impl::floorAreaPerPerson() const {
    double area = floorArea();
    double np = numberOfPeople();
    if (equal(np,0.0)) {
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return area / np;
  }
  
  double Building_Impl::lightingPower() const {
    double result(0.0);
    BOOST_FOREACH(const Space& space, spaces()){
      result += space.multiplier() * space.lightingPower();
    }
    return result;
  }

  double Building_Impl::lightingPowerPerFloorArea() const {
    double area = floorArea();
    double lp = lightingPower();
    if (equal(area,0.0)) {
      if (equal(lp,0.0)) {
        return 0.0;
      }
      else if (spaces().size() == 1u) {
        return spaces()[0].lightingPowerPerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return lp / area;
  }

  double Building_Impl::lightingPowerPerPerson() const {
    double np = numberOfPeople();
    double lp = lightingPower();
    if (equal(np,0.0)) {
      if (equal(lp,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].lightingPowerPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return lp / np;
  }

  double Building_Impl::electricEquipmentPower() const {
    double result(0.0);
    BOOST_FOREACH(const Space& space, spaces()){
      result += space.multiplier() * space.electricEquipmentPower();
    }
    return result;
  }

  double Building_Impl::electricEquipmentPowerPerFloorArea() const {
    double area = floorArea();
    double ep = electricEquipmentPower();
    if (equal(area,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].electricEquipmentPowerPerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / area;
  }

  double Building_Impl::electricEquipmentPowerPerPerson() const {
    double np = numberOfPeople();
    double ep = electricEquipmentPower();
    if (equal(np,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].electricEquipmentPowerPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / np;
  }

  double Building_Impl::gasEquipmentPower() const {
    double result(0.0);
    BOOST_FOREACH(const Space& space, spaces()){
      result += space.multiplier() * space.gasEquipmentPower();
    }
    return result;
  }

  double Building_Impl::gasEquipmentPowerPerFloorArea() const {
    double area = floorArea();
    double ep = gasEquipmentPower();
    if (equal(area,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].gasEquipmentPowerPerFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / area;
  }

  double Building_Impl::gasEquipmentPowerPerPerson() const {
    double np = numberOfPeople();
    double ep = gasEquipmentPower();
    if (equal(np,0.0)) {
      if (equal(ep,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].gasEquipmentPowerPerPerson();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return ep / np;
  }

  double Building_Impl::infiltrationDesignFlowRate() const {
    double result(0.0);
    BOOST_FOREACH(const Space& space, spaces()){
      result += space.multiplier() * space.infiltrationDesignFlowRate();
    }
    return result;
  }

  double Building_Impl::infiltrationDesignFlowPerSpaceFloorArea() const {
    double area = floorArea();
    double idfr = infiltrationDesignFlowRate();
    if (equal(area,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignFlowPerSpaceFloorArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return idfr/area;
  }

  double Building_Impl::infiltrationDesignFlowPerExteriorSurfaceArea() const {
    double area = exteriorSurfaceArea();
    double idfr = infiltrationDesignFlowRate();
    if (equal(area,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignFlowPerExteriorSurfaceArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return idfr/area;
  }

  double Building_Impl::infiltrationDesignFlowPerExteriorWallArea() const {
    double area = exteriorWallArea();
    double idfr = infiltrationDesignFlowRate();
    if (equal(area,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignFlowPerExteriorWallArea();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return idfr/area;
  }

  double Building_Impl::infiltrationDesignAirChangesPerHour() const {
    double volume = airVolume();
    double idfr = infiltrationDesignFlowRate();
    if (equal(volume,0.0)) {
      if (equal(idfr,0.0)) {
        return 0.0;
      }
      if (spaces().size() == 1u) {
        return spaces()[0].infiltrationDesignAirChangesPerHour();
      }
      LOG_AND_THROW("Calculation would require division by 0.");
    }
    return convert(idfr/volume,"1/s","1/h").get();
  }

  Transformation Building_Impl::transformation() const
  {
    // rotate negative amount around the z axis, EnergyPlus defines rotation clockwise
    return Transformation::rotation(Vector3d(0,0,1), -degToRad(this->northAxis()));
  }

  std::vector<std::vector<Point3d> > Building_Impl::generateSkylightPattern(double skylightToProjectedFloorRatio, double desiredWidth, double desiredHeight) const
  {
    return openstudio::model::generateSkylightPattern(this->spaces(), 0.0, skylightToProjectedFloorRatio, desiredWidth, desiredHeight);
  }

  openstudio::Quantity Building_Impl::northAxis_SI() const
  {
    OSOptionalQuantity value = getQuantity(OS_BuildingFields::NorthAxis,true,false);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity Building_Impl::northAxis_IP() const
  {
    OSOptionalQuantity value = getQuantity(OS_BuildingFields::NorthAxis,true,true);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  bool Building_Impl::setNorthAxis(const Quantity& northAxis)
  {
    bool result = setQuantity(OS_BuildingFields::NorthAxis, northAxis);
    return result;
  }

  openstudio::Quantity Building_Impl::nominalFloortoFloorHeight_SI() const
  {
    OSOptionalQuantity value = getQuantity(OS_BuildingFields::NominalFloortoFloorHeight,true,false);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  openstudio::Quantity Building_Impl::nominalFloortoFloorHeight_IP() const
  {
    OSOptionalQuantity value = getQuantity(OS_BuildingFields::NominalFloortoFloorHeight,true,true);
    OS_ASSERT(value.isSet());
    return value.get();
  }

  bool Building_Impl::setNominalFloortoFloorHeight(const Quantity& nominalFloortoFloorHeight)
  {
    bool result = setQuantity(OS_BuildingFields::NominalFloortoFloorHeight, nominalFloortoFloorHeight);
    return result;
  }

  boost::optional<ModelObject> Building_Impl::spaceTypeAsModelObject() const {
    OptionalModelObject result;
    OptionalSpaceType intermediate = spaceType();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> Building_Impl::defaultConstructionSetAsModelObject() const {
    OptionalModelObject result;
    OptionalDefaultConstructionSet intermediate = defaultConstructionSet();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> Building_Impl::defaultScheduleSetAsModelObject() const {
    OptionalModelObject result;
    OptionalDefaultScheduleSet intermediate = defaultScheduleSet();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> Building_Impl::metersAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(meters());
    return result;
  }

  boost::optional<ModelObject> Building_Impl::facilityAsModelObject() const {
    OptionalModelObject result;
    OptionalFacility intermediate = facility();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> Building_Impl::spacesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(spaces());
    return result;
  }

  std::vector<ModelObject> Building_Impl::shadingSurfaceGroupsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(shadingSurfaceGroups());
    return result;
  }

  std::vector<ModelObject> Building_Impl::thermalZonesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(thermalZones());
    return result;
  }

  std::vector<ModelObject> Building_Impl::exteriorWallsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(exteriorWalls());
    return result;
  }

  std::vector<ModelObject> Building_Impl::roofsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(roofs());
    return result;
  }

  bool Building_Impl::setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool Building_Impl::setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool Building_Impl::setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

} // detail

IddObjectType Building::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Building);
  return result;
}

double Building::northAxis() const {
  return getImpl<detail::Building_Impl>()->northAxis();
}

bool Building::isNorthAxisDefaulted() const {
  return getImpl<detail::Building_Impl>()->isNorthAxisDefaulted();
}

double Building::nominalFloortoFloorHeight() const {
  return getImpl<detail::Building_Impl>()->nominalFloortoFloorHeight();
}

bool Building::isNominalFloortoFloorHeightDefaulted() const {
  return getImpl<detail::Building_Impl>()->isNominalFloortoFloorHeightDefaulted();
}

boost::optional<int> Building::standardsNumberOfStories() const{
  return getImpl<detail::Building_Impl>()->standardsNumberOfStories();
}

boost::optional<int> Building::standardsNumberOfAboveGroundStories() const{
  return getImpl<detail::Building_Impl>()->standardsNumberOfAboveGroundStories();
}

boost::optional<std::string> Building::standardsBuildingType() const{
  return getImpl<detail::Building_Impl>()->standardsBuildingType();
}

std::vector<std::string> Building::suggestedStandardsBuildingTypes() const{
  return getImpl<detail::Building_Impl>()->suggestedStandardsBuildingTypes();
}

void Building::setNorthAxis(double northAxis) {
  getImpl<detail::Building_Impl>()->setNorthAxis(northAxis);
}

void Building::resetNorthAxis() {
  getImpl<detail::Building_Impl>()->resetNorthAxis();
}

bool Building::setNominalFloortoFloorHeight(double nominalFloortoFloorHeight) {
  return getImpl<detail::Building_Impl>()->setNominalFloortoFloorHeight(nominalFloortoFloorHeight);
}

void Building::resetNominalFloortoFloorHeight() {
  getImpl<detail::Building_Impl>()->resetNominalFloortoFloorHeight();
}

bool Building::setStandardsNumberOfStories(int value){
  return getImpl<detail::Building_Impl>()->setStandardsNumberOfStories(value);
}

void Building::resetStandardsNumberOfStories(){
  getImpl<detail::Building_Impl>()->resetStandardsNumberOfStories();
}

bool Building::setStandardsNumberOfAboveGroundStories(int value){
  return getImpl<detail::Building_Impl>()->setStandardsNumberOfAboveGroundStories(value);
}

void Building::resetStandardsNumberOfAboveGroundStories(){
  getImpl<detail::Building_Impl>()->resetStandardsNumberOfAboveGroundStories();
}

bool Building::setStandardsBuildingType(const std::string& standardsBuildingType){
  return getImpl<detail::Building_Impl>()->setStandardsBuildingType(standardsBuildingType);
}

void Building::resetStandardsBuildingType(){
  getImpl<detail::Building_Impl>()->resetStandardsBuildingType();
}

boost::optional<SpaceType> Building::spaceType() const
{
  return getImpl<detail::Building_Impl>()->spaceType();
}

bool Building::setSpaceType(const SpaceType& spaceType)
{
  return getImpl<detail::Building_Impl>()->setSpaceType(spaceType);
}

void Building::resetSpaceType()
{
  getImpl<detail::Building_Impl>()->resetSpaceType();
}

boost::optional<DefaultConstructionSet> Building::defaultConstructionSet() const
{
  return getImpl<detail::Building_Impl>()->defaultConstructionSet();
}

bool Building::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
{
  return getImpl<detail::Building_Impl>()->setDefaultConstructionSet(defaultConstructionSet);
}

void Building::resetDefaultConstructionSet()
{
  getImpl<detail::Building_Impl>()->resetDefaultConstructionSet();
}

boost::optional<DefaultScheduleSet> Building::defaultScheduleSet() const
{
  return getImpl<detail::Building_Impl>()->defaultScheduleSet();
}

bool Building::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
{
  return getImpl<detail::Building_Impl>()->setDefaultScheduleSet(defaultScheduleSet);
}

void Building::resetDefaultScheduleSet()
{
  getImpl<detail::Building_Impl>()->resetDefaultScheduleSet();
}

MeterVector Building::meters() const
{
  return getImpl<detail::Building_Impl>()->meters();
}

OptionalFacility Building::facility() const
{
  return getImpl<detail::Building_Impl>()->facility();
}

SpaceVector Building::spaces() const
{
  return getImpl<detail::Building_Impl>()->spaces();
}

ShadingSurfaceGroupVector Building::shadingSurfaceGroups() const
{
  return getImpl<detail::Building_Impl>()->shadingSurfaceGroups();
}

std::vector<ThermalZone> Building::thermalZones() const
{
  return getImpl<detail::Building_Impl>()->thermalZones();
}

std::vector<Surface> Building::exteriorWalls() const {
  return getImpl<detail::Building_Impl>()->exteriorWalls();
}

std::vector<Surface> Building::roofs() const {
  return getImpl<detail::Building_Impl>()->roofs();
}

double Building::floorArea() const
{
  return getImpl<detail::Building_Impl>()->floorArea();
}

boost::optional<double> Building::conditionedFloorArea() const
{
  return getImpl<detail::Building_Impl>()->conditionedFloorArea();
}

double Building::exteriorSurfaceArea() const {
  return getImpl<detail::Building_Impl>()->exteriorSurfaceArea();
}

double Building::exteriorWallArea() const {
  return getImpl<detail::Building_Impl>()->exteriorWallArea();
}

double Building::airVolume() const {
  return getImpl<detail::Building_Impl>()->airVolume();
}

double Building::numberOfPeople() const {
  return getImpl<detail::Building_Impl>()->numberOfPeople();
}

double Building::peoplePerFloorArea() const {
  return getImpl<detail::Building_Impl>()->peoplePerFloorArea();
}

double Building::floorAreaPerPerson() const {
  return getImpl<detail::Building_Impl>()->floorAreaPerPerson();
}

double Building::lightingPower() const {
  return getImpl<detail::Building_Impl>()->lightingPower();
}

double Building::lightingPowerPerFloorArea() const {
  return getImpl<detail::Building_Impl>()->lightingPowerPerFloorArea();
}

double Building::lightingPowerPerPerson() const {
  return getImpl<detail::Building_Impl>()->lightingPowerPerPerson();
}

double Building::electricEquipmentPower() const {
  return getImpl<detail::Building_Impl>()->electricEquipmentPower();
}

double Building::electricEquipmentPowerPerFloorArea() const {
  return getImpl<detail::Building_Impl>()->electricEquipmentPowerPerFloorArea();
}

double Building::electricEquipmentPowerPerPerson() const {
  return getImpl<detail::Building_Impl>()->electricEquipmentPowerPerPerson();
}

double Building::gasEquipmentPower() const {
  return getImpl<detail::Building_Impl>()->gasEquipmentPower();
}

double Building::gasEquipmentPowerPerFloorArea() const {
  return getImpl<detail::Building_Impl>()->gasEquipmentPowerPerFloorArea();
}

double Building::gasEquipmentPowerPerPerson() const {
  return getImpl<detail::Building_Impl>()->gasEquipmentPowerPerPerson();
}

double Building::infiltrationDesignFlowRate() const {
  return getImpl<detail::Building_Impl>()->infiltrationDesignFlowRate();
}

double Building::infiltrationDesignFlowPerSpaceFloorArea() const {
  return getImpl<detail::Building_Impl>()->infiltrationDesignFlowPerSpaceFloorArea();
}

double Building::infiltrationDesignFlowPerExteriorSurfaceArea() const {
  return getImpl<detail::Building_Impl>()->infiltrationDesignFlowPerExteriorSurfaceArea();
}

double Building::infiltrationDesignFlowPerExteriorWallArea() const {
  return getImpl<detail::Building_Impl>()->infiltrationDesignFlowPerExteriorWallArea();
}

double Building::infiltrationDesignAirChangesPerHour() const {
  return getImpl<detail::Building_Impl>()->infiltrationDesignAirChangesPerHour();
}

Transformation Building::transformation() const
{
  return getImpl<detail::Building_Impl>()->transformation();
}

std::vector<std::vector<Point3d> > Building::generateSkylightPattern(double skylightToProjectedFloorRatio, double desiredWidth, double desiredHeight) const
{
  return getImpl<detail::Building_Impl>()->generateSkylightPattern(skylightToProjectedFloorRatio, desiredWidth, desiredHeight);
}

/// @cond
Building::Building(boost::shared_ptr<detail::Building_Impl> impl)
  : ParentObject(impl)
{}

Building::Building(Model& model)
  : ParentObject(Building::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

