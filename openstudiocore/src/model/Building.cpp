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

#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/BuildingStandardsInformation.hpp>
#include <model/BuildingStandardsInformation_Impl.hpp>
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

    OptionalBuildingStandardsInformation stdsInfo = model().getOptionalUniqueModelObject<BuildingStandardsInformation>();
    if (stdsInfo) {
      result.push_back(*stdsInfo);
    }

    // meters
    MeterVector meters = this->meters();
    result.insert(result.end(),meters.begin(),meters.end());

    // building stories
    BuildingStoryVector stories = model().getModelObjects<BuildingStory>();
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

  std::string Building_Impl::buildingType() const {
    boost::optional<std::string> result = getString(OS_BuildingFields::BuildingType,true);
    OS_ASSERT(result);
    return *result;
  }

  bool Building_Impl::isBuildingTypeDefaulted() const {
    return isEmpty(OS_BuildingFields::BuildingType);
  }

  std::vector<std::string> Building_Impl::buildingTypeValues() const {
    return Building::validBuildingTypeValues();
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

  bool Building_Impl::setBuildingType(const std::string& buildingType) {
    bool result = false;
    result = setString(OS_BuildingFields::BuildingType, buildingType);
    return result;
  }

  void Building_Impl::resetBuildingType() {
    bool result = setString(OS_BuildingFields::BuildingType, "");
    OS_ASSERT(result);
  }

  void Building_Impl::setNorthAxis(double northAxis) {
    bool result = false;
    result = setDouble(OS_BuildingFields::NorthAxis, northAxis);
    OS_ASSERT(result);
  }

  void Building_Impl::resetNorthAxis() {
    bool result = setString(OS_BuildingFields::NorthAxis, "");
    OS_ASSERT(result);
  }

  bool Building_Impl::setNominalFloortoFloorHeight(double nominalFloortoFloorHeight) {
    bool result = false;
    result = setDouble(OS_BuildingFields::NominalFloortoFloorHeight, nominalFloortoFloorHeight);
    return result;
  }

  void Building_Impl::resetNominalFloortoFloorHeight() {
    bool result = setString(OS_BuildingFields::NominalFloortoFloorHeight, "");
    OS_ASSERT(result);
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
    MeterVector meters = this->model().getModelObjects<Meter>();
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
    return this->model().getModelObjects<Space>();
  }

  ShadingSurfaceGroupVector Building_Impl::shadingSurfaceGroups() const
  {
    ShadingSurfaceGroupVector result;
    BOOST_FOREACH(ShadingSurfaceGroup shadingGroup, this->model().getModelObjects<ShadingSurfaceGroup>()){
      if (istringEqual(shadingGroup.shadingSurfaceType(), "Building")){
        result.push_back(shadingGroup);
      }
    }
    return result;
  }

  std::vector<ThermalZone> Building_Impl::thermalZones() const
  {
    // all thermal zones in workspace implicitly belong to building
    return this->model().getModelObjects<ThermalZone>();
  }

  std::vector<Surface> Building_Impl::exteriorWalls() const {
    SurfaceVector result;
    SurfaceVector candidates = model().getModelObjects<Surface>();
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
    SurfaceVector candidates = model().getModelObjects<Surface>();
    BOOST_FOREACH(const Surface& candidate,candidates) {
      std::string surfaceType = candidate.surfaceType();
      std::string outsideBoundaryCondition = candidate.outsideBoundaryCondition();
      if (openstudio::istringEqual(surfaceType, "RoofCeiling") && openstudio::istringEqual(outsideBoundaryCondition, "Outdoors")) {
        result.push_back(candidate);
      }
    }
    return result;
  }

  BuildingStandardsInformation Building_Impl::standardsInformation() const {
    return model().getUniqueModelObject<BuildingStandardsInformation>();
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

  boost::optional<int> Building_Impl::numberOfStories() const {
    OptionalBuildingStandardsInformation standardsInformation = this->standardsInformation();
    if (standardsInformation) {
      return standardsInformation->numberOfStories();
    }
    return boost::none;
  }

  boost::optional<int> Building_Impl::numberOfAboveGroundStories() const {
    OptionalBuildingStandardsInformation standardsInformation = this->standardsInformation();
    if (standardsInformation) {
      return standardsInformation->numberOfAboveGroundStories();
    }
    return boost::none;
  }

  bool Building_Impl::setNumberOfStories(boost::optional<int> value) {
    BuildingStandardsInformation standardsInformation = model().getUniqueModelObject<BuildingStandardsInformation>();
    return standardsInformation.setNumberOfStories(value);
  }

  bool Building_Impl::setNumberOfAboveGroundStories(boost::optional<int> value) {
    BuildingStandardsInformation standardsInformation = model().getUniqueModelObject<BuildingStandardsInformation>();
    return standardsInformation.setNumberOfAboveGroundStories(value);
  }

  Transformation Building_Impl::transformation() const
  {
    // rotate negative amount around the z axis, EnergyPlus defines rotation clockwise
    return Transformation::rotation(Vector3d(0,0,1), -degToRad(this->northAxis()));
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
    bool result = false;
    result = setQuantity(OS_BuildingFields::NorthAxis, northAxis);
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
    bool result = false;
    result = setQuantity(OS_BuildingFields::NominalFloortoFloorHeight, nominalFloortoFloorHeight);
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

  boost::optional<ModelObject> Building_Impl::standardsInformationAsModelObject() const {
    OptionalModelObject result = standardsInformation();
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

std::vector<std::string> Building::validBuildingTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_BuildingFields::BuildingType);
}

std::string Building::buildingType() const {
  return getImpl<detail::Building_Impl>()->buildingType();
}

bool Building::isBuildingTypeDefaulted() const {
  return getImpl<detail::Building_Impl>()->isBuildingTypeDefaulted();
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

bool Building::setBuildingType(const std::string& buildingType) {
  return getImpl<detail::Building_Impl>()->setBuildingType(buildingType);
}

void Building::resetBuildingType() {
  getImpl<detail::Building_Impl>()->resetBuildingType();
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

BuildingStandardsInformation Building::standardsInformation() const {
  return getImpl<detail::Building_Impl>()->standardsInformation();
}

double Building::floorArea() const
{
  return getImpl<detail::Building_Impl>()->floorArea();
}

boost::optional<double> Building::conditionedFloorArea() const
{
  return getImpl<detail::Building_Impl>()->conditionedFloorArea();
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

boost::optional<int> Building::numberOfStories() const {
  return getImpl<detail::Building_Impl>()->numberOfStories();
}

boost::optional<int> Building::numberOfAboveGroundStories() const {
  return getImpl<detail::Building_Impl>()->numberOfAboveGroundStories();
}

bool Building::setNumberOfStories(boost::optional<int> value) {
  return getImpl<detail::Building_Impl>()->setNumberOfStories(value);
}

bool Building::setNumberOfAboveGroundStories(boost::optional<int> value) {
  return getImpl<detail::Building_Impl>()->setNumberOfAboveGroundStories(value);
}

Transformation Building::transformation() const
{
  return getImpl<detail::Building_Impl>()->transformation();
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

