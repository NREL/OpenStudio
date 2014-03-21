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

#include <model/RefrigerationWalkIn.hpp>
#include <model/RefrigerationWalkIn_Impl.hpp>
#include <model/RefrigerationWalkInZoneBoundary.hpp>
#include <model/RefrigerationWalkInZoneBoundary_Impl.hpp>

#include <model/RefrigerationSystem_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idf/WorkspaceExtensibleGroup.hpp>
#include <utilities/idf/WorkspaceObject.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Refrigeration_WalkIn_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationWalkIn_Impl::RefrigerationWalkIn_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationWalkIn::iddObjectType());
  }

  RefrigerationWalkIn_Impl::RefrigerationWalkIn_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationWalkIn::iddObjectType());
  }

  RefrigerationWalkIn_Impl::RefrigerationWalkIn_Impl(const RefrigerationWalkIn_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationWalkIn_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationWalkIn_Impl::iddObjectType() const {
    return RefrigerationWalkIn::iddObjectType();
  }

  std::vector<ScheduleTypeKey> RefrigerationWalkIn_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Refrigeration_WalkInFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkIn","Availability"));
    }
    if (std::find(b,e,OS_Refrigeration_WalkInFields::HeatingPowerScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkIn","Heating Power"));
    }
    if (std::find(b,e,OS_Refrigeration_WalkInFields::LightingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkIn","Lighting"));
    }
    if (std::find(b,e,OS_Refrigeration_WalkInFields::DefrostScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkIn","Defrost"));
    }
    if (std::find(b,e,OS_Refrigeration_WalkInFields::DefrostDripDownScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkIn","Defrost Drip-Down"));
    }
    if (std::find(b,e,OS_Refrigeration_WalkInFields::RestockingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkIn","Restocking"));
    }
    return result;
  }

  std::vector<IdfObject> RefrigerationWalkIn_Impl::remove()
  {
    std::vector<IdfObject> result;

    boost::shared_ptr<Model_Impl> t_model = model().getImpl<Model_Impl>();
    t_model->blockSignals(true);

    std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries = this->zoneBoundaries();
    for( std::vector<RefrigerationWalkInZoneBoundary>::iterator it = zoneBoundaries.begin();
         it != zoneBoundaries.end();
         ++it )
    {
      std::vector<IdfObject> removedZoneBoundaries = it->remove();
      result.insert(result.end(), removedZoneBoundaries.begin(), removedZoneBoundaries.end());
    }      

    t_model->blockSignals(false);

    std::vector<IdfObject> removedRRefrigerationWalkIn = ModelObject_Impl::remove();
    result.insert(result.end(), removedRRefrigerationWalkIn.begin(), removedRRefrigerationWalkIn.end());

    return result;
  }

  ModelObject RefrigerationWalkIn_Impl::clone(Model model) const
  {
    RefrigerationWalkIn modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationWalkIn>();

    modelObjectClone.removeAllZoneBoundaries();

    std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries = this->zoneBoundaries();
    for( std::vector<RefrigerationWalkInZoneBoundary>::iterator it = zoneBoundaries.begin();
         it != zoneBoundaries.end();
         ++it )
    {
      RefrigerationWalkInZoneBoundary zoneBoundaryClone = it->clone(model).cast<RefrigerationWalkInZoneBoundary>();
      modelObjectClone.addZoneBoundary(zoneBoundaryClone);
    }

    return modelObjectClone;
  }

  bool RefrigerationWalkIn_Impl::addZoneBoundary(const RefrigerationWalkInZoneBoundary& refrigerationWalkInZoneBoundary)
  {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    bool temp = eg.setPointer(OS_Refrigeration_WalkInExtensibleFields::WalkInZoneBoundary, refrigerationWalkInZoneBoundary.handle());
    if( !temp ) {
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      return temp;
    }
    return temp;
  }

  void RefrigerationWalkIn_Impl::removeZoneBoundary(unsigned index)
  {
    unsigned numberofDataPairs = numExtensibleGroups();
    if(index < numberofDataPairs) {
      getObject<ModelObject>().eraseExtensibleGroup(index);
    }
  }

  void RefrigerationWalkIn_Impl::removeAllZoneBoundaries() 
  {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  std::vector<RefrigerationWalkInZoneBoundary> RefrigerationWalkIn_Impl::zoneBoundaries() const
  {
    std::vector<RefrigerationWalkInZoneBoundary> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
         it != groups.end();
         ++it )
    {
      if( boost::optional<WorkspaceObject> wo = it->cast<WorkspaceExtensibleGroup>().getTarget(OS_Refrigeration_WalkInExtensibleFields::WalkInZoneBoundary) )
      {
        if(boost::optional<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundary = 
            wo->optionalCast<RefrigerationWalkInZoneBoundary>())
        {
          if( refrigerationWalkInZoneBoundary )
          {
            result.push_back( refrigerationWalkInZoneBoundary.get() );
          }
        }
      }
    }

    return result;
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkInFields::AvailabilityScheduleName);
  }

  double RefrigerationWalkIn_Impl::ratedCoilCoolingCapacity() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::RatedCoilCoolingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationWalkIn_Impl::operatingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::OperatingTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationWalkIn_Impl::ratedCoolingSourceTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::RatedCoolingSourceTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationWalkIn_Impl::ratedTotalHeatingPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::RatedTotalHeatingPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::heatingPowerSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkInFields::HeatingPowerScheduleName);
  }

  double RefrigerationWalkIn_Impl::ratedCoolingCoilFanPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::RatedCoolingCoilFanPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkIn_Impl::isRatedCoolingCoilFanPowerDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkInFields::RatedCoolingCoilFanPower);
  }

  double RefrigerationWalkIn_Impl::ratedCirculationFanPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::RatedCirculationFanPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkIn_Impl::isRatedCirculationFanPowerDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkInFields::RatedCirculationFanPower);
  }

  double RefrigerationWalkIn_Impl::ratedTotalLightingPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::RatedTotalLightingPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::lightingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkInFields::LightingScheduleName);
  }

  std::string RefrigerationWalkIn_Impl::defrostType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_WalkInFields::DefrostType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkIn_Impl::isDefrostTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkInFields::DefrostType);
  }

  std::string RefrigerationWalkIn_Impl::defrostControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_WalkInFields::DefrostControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkIn_Impl::isDefrostControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkInFields::DefrostControlType);
  }

  Schedule RefrigerationWalkIn_Impl::defrostSchedule() const {
    boost::optional<Schedule> value = optionalDefrostSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Defrost Schedule attached.");
    }
    return value.get();
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::defrostDripDownSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkInFields::DefrostDripDownScheduleName);
  }

  boost::optional<double> RefrigerationWalkIn_Impl::defrostPower() const {
    return getDouble(OS_Refrigeration_WalkInFields::DefrostPower,true);
  }

  boost::optional<double> RefrigerationWalkIn_Impl::temperatureTerminationDefrostFractiontoIce() const {
    return getDouble(OS_Refrigeration_WalkInFields::TemperatureTerminationDefrostFractiontoIce,true);
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::restockingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkInFields::RestockingScheduleName);
  }

  double RefrigerationWalkIn_Impl::averageRefrigerantChargeInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::AverageRefrigerantChargeInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkIn_Impl::isAverageRefrigerantChargeInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkInFields::AverageRefrigerantChargeInventory);
  }

  double RefrigerationWalkIn_Impl::insulatedFloorSurfaceArea() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::InsulatedFloorSurfaceArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationWalkIn_Impl::insulatedFloorUValue() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkInFields::InsulatedFloorUValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkIn_Impl::isInsulatedFloorUValueDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkInFields::InsulatedFloorUValue);
  }

  boost::optional<RefrigerationSystem> RefrigerationWalkIn_Impl::system() const {
    std::vector<RefrigerationSystem> refrigerationSystems = this->model().getConcreteModelObjects<RefrigerationSystem>();
    RefrigerationWalkIn refrigerationWalkIn = this->getObject<RefrigerationWalkIn>();
    BOOST_FOREACH(RefrigerationSystem refrigerationSystem, refrigerationSystems) {
      RefrigerationWalkInVector refrigerationWalkIns = refrigerationSystem.walkins();
      if ( !refrigerationWalkIns.empty() && std::find(refrigerationWalkIns.begin(), refrigerationWalkIns.end(), refrigerationWalkIn) != refrigerationWalkIns.end() ) {
        return refrigerationSystem;
      }
    }
    return boost::none;
  }

  boost::optional<ThermalZone> RefrigerationWalkIn_Impl::zoneBoundaryThermalZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->thermalZone();
    }
    return boost::none;    
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryTotalInsulatedSurfaceAreaFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->totalInsulatedSurfaceAreaFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryAreaofGlassReachInDoorsFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->areaofGlassReachInDoorsFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryHeightofGlassReachInDoorsFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->heightofGlassReachInDoorsFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryAreaofStockingDoorsFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->areaofStockingDoorsFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryHeightofStockingDoorsFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->heightofStockingDoorsFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryInsulatedSurfaceUValueFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->insulatedSurfaceUValueFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryGlassReachInDoorUValueFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->glassReachInDoorUValueFacingZone();
    }
    return boost::none;
  }

  boost::optional<double> RefrigerationWalkIn_Impl::zoneBoundaryStockingDoorUValueFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->stockingDoorUValueFacingZone();
    }
    return boost::none;
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::zoneBoundaryStockingDoorOpeningScheduleFacingZone() const {
    if(boost::optional<RefrigerationWalkInZoneBoundary> t_zoneBoundary = zoneBoundary()){
      return t_zoneBoundary->stockingDoorOpeningScheduleFacingZone();
    }
    return boost::none;
  }

  bool RefrigerationWalkIn_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkInFields::AvailabilityScheduleName,
                              "RefrigerationWalkIn",
                              "Availability",
                              schedule);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Refrigeration_WalkInFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkIn_Impl::setRatedCoilCoolingCapacity(double ratedCoilCoolingCapacity) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::RatedCoilCoolingCapacity, ratedCoilCoolingCapacity);
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setOperatingTemperature(double operatingTemperature) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::OperatingTemperature, operatingTemperature);
    return result;
  }

  bool RefrigerationWalkIn_Impl::setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::RatedCoolingSourceTemperature, ratedCoolingSourceTemperature);
    return result;
  }

  void RefrigerationWalkIn_Impl::setRatedTotalHeatingPower(double ratedTotalHeatingPower) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::RatedTotalHeatingPower, ratedTotalHeatingPower);
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setHeatingPowerSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkInFields::HeatingPowerScheduleName,
                              "RefrigerationWalkIn",
                              "Heating Power",
                              schedule);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetHeatingPowerSchedule() {
    bool result = setString(OS_Refrigeration_WalkInFields::HeatingPowerScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setRatedCoolingCoilFanPower(double ratedCoolingCoilFanPower) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::RatedCoolingCoilFanPower, ratedCoolingCoilFanPower);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetRatedCoolingCoilFanPower() {
    bool result = setString(OS_Refrigeration_WalkInFields::RatedCoolingCoilFanPower, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setRatedCirculationFanPower(double ratedCirculationFanPower) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::RatedCirculationFanPower, ratedCirculationFanPower);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetRatedCirculationFanPower() {
    bool result = setString(OS_Refrigeration_WalkInFields::RatedCirculationFanPower, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkIn_Impl::setRatedTotalLightingPower(double ratedTotalLightingPower) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::RatedTotalLightingPower, ratedTotalLightingPower);
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setLightingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkInFields::LightingScheduleName,
                              "RefrigerationWalkIn",
                              "Lighting",
                              schedule);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetLightingSchedule() {
    bool result = setString(OS_Refrigeration_WalkInFields::LightingScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setDefrostType(std::string defrostType) {
    bool result = setString(OS_Refrigeration_WalkInFields::DefrostType, defrostType);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetDefrostType() {
    bool result = setString(OS_Refrigeration_WalkInFields::DefrostType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setDefrostControlType(std::string defrostControlType) {
    bool result = setString(OS_Refrigeration_WalkInFields::DefrostControlType, defrostControlType);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetDefrostControlType() {
    bool result = setString(OS_Refrigeration_WalkInFields::DefrostControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setDefrostSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkInFields::DefrostScheduleName,
                              "RefrigerationWalkIn",
                              "Defrost",
                              schedule);
    return result;
  }

  bool RefrigerationWalkIn_Impl::setDefrostDripDownSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkInFields::DefrostDripDownScheduleName,
                              "RefrigerationWalkIn",
                              "Defrost Drip-Down",
                              schedule);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetDefrostDripDownSchedule() {
    bool result = setString(OS_Refrigeration_WalkInFields::DefrostDripDownScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setDefrostPower(boost::optional<double> defrostPower) {
    bool result(false);
    if (defrostPower) {
      result = setDouble(OS_Refrigeration_WalkInFields::DefrostPower, defrostPower.get());
    }
    else {
      resetDefrostPower();
      result = true;
    }
    return result;
  }

  void RefrigerationWalkIn_Impl::resetDefrostPower() {
    bool result = setString(OS_Refrigeration_WalkInFields::DefrostPower, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setTemperatureTerminationDefrostFractiontoIce(boost::optional<double> temperatureTerminationDefrostFractiontoIce) {
    bool result(false);
    if (temperatureTerminationDefrostFractiontoIce) {
      result = setDouble(OS_Refrigeration_WalkInFields::TemperatureTerminationDefrostFractiontoIce, temperatureTerminationDefrostFractiontoIce.get());
    }
    else {
      resetTemperatureTerminationDefrostFractiontoIce();
      result = true;
    }
    return result;
  }

  void RefrigerationWalkIn_Impl::resetTemperatureTerminationDefrostFractiontoIce() {
    bool result = setString(OS_Refrigeration_WalkInFields::TemperatureTerminationDefrostFractiontoIce, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setRestockingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkInFields::RestockingScheduleName,
                              "RefrigerationWalkIn",
                              "Restocking",
                              schedule);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetRestockingSchedule() {
    bool result = setString(OS_Refrigeration_WalkInFields::RestockingScheduleName, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkIn_Impl::setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::AverageRefrigerantChargeInventory, averageRefrigerantChargeInventory);
    OS_ASSERT(result);
  }

  void RefrigerationWalkIn_Impl::resetAverageRefrigerantChargeInventory() {
    bool result = setString(OS_Refrigeration_WalkInFields::AverageRefrigerantChargeInventory, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkIn_Impl::setInsulatedFloorSurfaceArea(double insulatedFloorSurfaceArea) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::InsulatedFloorSurfaceArea, insulatedFloorSurfaceArea);
    return result;
  }

  bool RefrigerationWalkIn_Impl::setInsulatedFloorUValue(double insulatedFloorUValue) {
    bool result = setDouble(OS_Refrigeration_WalkInFields::InsulatedFloorUValue, insulatedFloorUValue);
    return result;
  }

  void RefrigerationWalkIn_Impl::resetInsulatedFloorUValue() {
    bool result = setString(OS_Refrigeration_WalkInFields::InsulatedFloorUValue, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> RefrigerationWalkIn_Impl::optionalDefrostSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkInFields::DefrostScheduleName);
  }

  bool RefrigerationWalkIn_Impl::addToSystem(RefrigerationSystem & system) {
    return system.addWalkin(this->getObject<RefrigerationWalkIn>());
  }

  void RefrigerationWalkIn_Impl::removeFromSystem() {
    boost::optional<RefrigerationSystem> refrigerationSystem = system();
    if(refrigerationSystem){
      refrigerationSystem.get().removeWalkin(this->getObject<RefrigerationWalkIn>());
    }
  }

  bool RefrigerationWalkIn_Impl::setZoneBoundaryThermalZone(const ThermalZone& zoneBoundaryThermalZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    return zoneBoundary.setThermalZone(zoneBoundaryThermalZone);
  }

  void RefrigerationWalkIn_Impl::resetZoneBoundaryThermalZone() {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    zoneBoundary.resetThermalZone();
  }

  bool RefrigerationWalkIn_Impl::setZoneBoundaryTotalInsulatedSurfaceAreaFacingZone(double zoneBoundaryTotalInsulatedSurfaceAreaFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    return zoneBoundary.setTotalInsulatedSurfaceAreaFacingZone(zoneBoundaryTotalInsulatedSurfaceAreaFacingZone);
  }

  void RefrigerationWalkIn_Impl::setZoneBoundaryAreaofGlassReachInDoorsFacingZone(double zoneBoundaryAreaofGlassReachInDoorsFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    zoneBoundary.setAreaofGlassReachInDoorsFacingZone(zoneBoundaryAreaofGlassReachInDoorsFacingZone);
  }

  void RefrigerationWalkIn_Impl::setZoneBoundaryHeightofGlassReachInDoorsFacingZone(double zoneBoundaryHeightofGlassReachInDoorsFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    zoneBoundary.setHeightofGlassReachInDoorsFacingZone(zoneBoundaryHeightofGlassReachInDoorsFacingZone);
  }

  void RefrigerationWalkIn_Impl::setZoneBoundaryAreaofStockingDoorsFacingZone(double zoneBoundaryAreaofStockingDoorsFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    zoneBoundary.setAreaofStockingDoorsFacingZone(zoneBoundaryAreaofStockingDoorsFacingZone);
  }

  void RefrigerationWalkIn_Impl::setZoneBoundaryHeightofStockingDoorsFacingZone(double zoneBoundaryHeightofStockingDoorsFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    zoneBoundary.setHeightofStockingDoorsFacingZone(zoneBoundaryHeightofStockingDoorsFacingZone);
  }

  bool RefrigerationWalkIn_Impl::setZoneBoundaryInsulatedSurfaceUValueFacingZone(double zoneBoundaryInsulatedSurfaceUValueFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    return zoneBoundary.setInsulatedSurfaceUValueFacingZone(zoneBoundaryInsulatedSurfaceUValueFacingZone);
  }

  bool RefrigerationWalkIn_Impl::setZoneBoundaryGlassReachInDoorUValueFacingZone(double zoneBoundaryGlassReachInDoorUValueFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    return zoneBoundary.setGlassReachInDoorUValueFacingZone(zoneBoundaryGlassReachInDoorUValueFacingZone);
  }

  bool RefrigerationWalkIn_Impl::setZoneBoundaryStockingDoorUValueFacingZone(double zoneBoundaryStockingDoorUValueFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    return zoneBoundary.setStockingDoorUValueFacingZone(zoneBoundaryStockingDoorUValueFacingZone);
  }

  bool RefrigerationWalkIn_Impl::setZoneBoundaryStockingDoorOpeningScheduleFacingZone(Schedule& zoneBoundaryStockingDoorOpeningScheduleFacingZone) {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    return zoneBoundary.setStockingDoorOpeningScheduleFacingZone(zoneBoundaryStockingDoorOpeningScheduleFacingZone);
  }

  void RefrigerationWalkIn_Impl::resetZoneBoundaryStockingDoorOpeningScheduleFacingZone() {
    RefrigerationWalkInZoneBoundary zoneBoundary = frontZoneBoundary();
    zoneBoundary.resetStockingDoorOpeningScheduleFacingZone();
  }
  
  RefrigerationWalkInZoneBoundary RefrigerationWalkIn_Impl::frontZoneBoundary() {
    if(zoneBoundaries().empty()){
      RefrigerationWalkInZoneBoundary refrigerationWalkInZoneBoundary(model());
      bool success = addZoneBoundary(refrigerationWalkInZoneBoundary);
      OS_ASSERT(success);
    }
    return zoneBoundaries().front();
  }

  boost::optional<RefrigerationWalkInZoneBoundary> RefrigerationWalkIn_Impl::zoneBoundary() const
  {
    if(! zoneBoundaries().empty()){
      return zoneBoundaries().front();
    } else {
      return boost::none;
    } 
  }

} // detail

RefrigerationWalkIn::RefrigerationWalkIn(const Model& model, Schedule& walkinDefrostSchedule)
  : ModelObject(RefrigerationWalkIn::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationWalkIn_Impl>());

  setRatedCoilCoolingCapacity(4690.0);
  bool ok = setOperatingTemperature(-2.22);
  OS_ASSERT(ok);
  ok = setRatedCoolingSourceTemperature(-6.67);
  OS_ASSERT(ok);
  setRatedTotalHeatingPower(0.0);
  ok = setRatedCoolingCoilFanPower(735.0);
  OS_ASSERT(ok);
  ok = setRatedCirculationFanPower(0.0);
  OS_ASSERT(ok);
  setRatedTotalLightingPower(120.0);
  ok = setDefrostType("Electric");
  OS_ASSERT(ok);
  ok = setDefrostControlType("TimeSchedule");
  OS_ASSERT(ok);
  ok = setDefrostSchedule(walkinDefrostSchedule);
  OS_ASSERT(ok);
  ok = setDefrostPower(5512.0);
  OS_ASSERT(ok);
  ok = setInsulatedFloorSurfaceArea(13.0);
  OS_ASSERT(ok);
  ok = setInsulatedFloorUValue(0.207);
  OS_ASSERT(ok);
  RefrigerationWalkInZoneBoundary zoneBoundary = RefrigerationWalkInZoneBoundary(model);
  ok = addZoneBoundary(zoneBoundary);
  OS_ASSERT(ok);
}

IddObjectType RefrigerationWalkIn::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_WalkIn);
}

std::vector<std::string> RefrigerationWalkIn::defrostTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_WalkInFields::DefrostType);
}

std::vector<std::string> RefrigerationWalkIn::defrostControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_WalkInFields::DefrostControlType);
}

bool RefrigerationWalkIn::addZoneBoundary(const RefrigerationWalkInZoneBoundary& refrigerationWalkInZoneBoundary){
  return getImpl<detail::RefrigerationWalkIn_Impl>()->addZoneBoundary(refrigerationWalkInZoneBoundary);
}

void RefrigerationWalkIn::removeZoneBoundary(unsigned index){
  return getImpl<detail::RefrigerationWalkIn_Impl>()->removeZoneBoundary(index);
}

void RefrigerationWalkIn::removeAllZoneBoundaries(){
  return getImpl<detail::RefrigerationWalkIn_Impl>()->removeAllZoneBoundaries();
}

std::vector<RefrigerationWalkInZoneBoundary> RefrigerationWalkIn::zoneBoundaries() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaries();
}

boost::optional<Schedule> RefrigerationWalkIn::availabilitySchedule() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->availabilitySchedule();
}

double RefrigerationWalkIn::ratedCoilCoolingCapacity() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->ratedCoilCoolingCapacity();
}

double RefrigerationWalkIn::operatingTemperature() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->operatingTemperature();
}

double RefrigerationWalkIn::ratedCoolingSourceTemperature() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->ratedCoolingSourceTemperature();
}

double RefrigerationWalkIn::ratedTotalHeatingPower() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->ratedTotalHeatingPower();
}

boost::optional<Schedule> RefrigerationWalkIn::heatingPowerSchedule() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->heatingPowerSchedule();
}

double RefrigerationWalkIn::ratedCoolingCoilFanPower() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->ratedCoolingCoilFanPower();
}

bool RefrigerationWalkIn::isRatedCoolingCoilFanPowerDefaulted() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->isRatedCoolingCoilFanPowerDefaulted();
}

double RefrigerationWalkIn::ratedCirculationFanPower() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->ratedCirculationFanPower();
}

bool RefrigerationWalkIn::isRatedCirculationFanPowerDefaulted() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->isRatedCirculationFanPowerDefaulted();
}

double RefrigerationWalkIn::ratedTotalLightingPower() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->ratedTotalLightingPower();
}

boost::optional<Schedule> RefrigerationWalkIn::lightingSchedule() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->lightingSchedule();
}

std::string RefrigerationWalkIn::defrostType() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->defrostType();
}

bool RefrigerationWalkIn::isDefrostTypeDefaulted() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->isDefrostTypeDefaulted();
}

std::string RefrigerationWalkIn::defrostControlType() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->defrostControlType();
}

bool RefrigerationWalkIn::isDefrostControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->isDefrostControlTypeDefaulted();
}

Schedule RefrigerationWalkIn::defrostSchedule() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->defrostSchedule();
}

boost::optional<Schedule> RefrigerationWalkIn::defrostDripDownSchedule() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->defrostDripDownSchedule();
}

boost::optional<double> RefrigerationWalkIn::defrostPower() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->defrostPower();
}

boost::optional<double> RefrigerationWalkIn::temperatureTerminationDefrostFractiontoIce() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->temperatureTerminationDefrostFractiontoIce();
}

boost::optional<Schedule> RefrigerationWalkIn::restockingSchedule() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->restockingSchedule();
}

double RefrigerationWalkIn::averageRefrigerantChargeInventory() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->averageRefrigerantChargeInventory();
}

bool RefrigerationWalkIn::isAverageRefrigerantChargeInventoryDefaulted() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->isAverageRefrigerantChargeInventoryDefaulted();
}

double RefrigerationWalkIn::insulatedFloorSurfaceArea() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->insulatedFloorSurfaceArea();
}

double RefrigerationWalkIn::insulatedFloorUValue() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->insulatedFloorUValue();
}

bool RefrigerationWalkIn::isInsulatedFloorUValueDefaulted() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->isInsulatedFloorUValueDefaulted();
}

boost::optional<RefrigerationSystem> RefrigerationWalkIn::system() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->system();
}

boost::optional<ThermalZone> RefrigerationWalkIn::zoneBoundaryThermalZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryThermalZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryTotalInsulatedSurfaceAreaFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryTotalInsulatedSurfaceAreaFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryAreaofGlassReachInDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryAreaofGlassReachInDoorsFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryHeightofGlassReachInDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryHeightofGlassReachInDoorsFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryAreaofStockingDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryAreaofStockingDoorsFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryHeightofStockingDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryHeightofStockingDoorsFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryInsulatedSurfaceUValueFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryInsulatedSurfaceUValueFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryGlassReachInDoorUValueFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryGlassReachInDoorUValueFacingZone();
}

boost::optional<double> RefrigerationWalkIn::zoneBoundaryStockingDoorUValueFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryStockingDoorUValueFacingZone();
}

boost::optional<Schedule> RefrigerationWalkIn::zoneBoundaryStockingDoorOpeningScheduleFacingZone() const {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->zoneBoundaryStockingDoorOpeningScheduleFacingZone();
}

bool RefrigerationWalkIn::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setAvailabilitySchedule(schedule);
}

void RefrigerationWalkIn::resetAvailabilitySchedule() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetAvailabilitySchedule();
}

void RefrigerationWalkIn::setRatedCoilCoolingCapacity(double ratedCoilCoolingCapacity) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setRatedCoilCoolingCapacity(ratedCoilCoolingCapacity);
}

bool RefrigerationWalkIn::setOperatingTemperature(double operatingTemperature) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setOperatingTemperature(operatingTemperature);
}

bool RefrigerationWalkIn::setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setRatedCoolingSourceTemperature(ratedCoolingSourceTemperature);
}

void RefrigerationWalkIn::setRatedTotalHeatingPower(double ratedTotalHeatingPower) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setRatedTotalHeatingPower(ratedTotalHeatingPower);
}

bool RefrigerationWalkIn::setHeatingPowerSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setHeatingPowerSchedule(schedule);
}

void RefrigerationWalkIn::resetHeatingPowerSchedule() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetHeatingPowerSchedule();
}

bool RefrigerationWalkIn::setRatedCoolingCoilFanPower(double ratedCoolingCoilFanPower) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setRatedCoolingCoilFanPower(ratedCoolingCoilFanPower);
}

void RefrigerationWalkIn::resetRatedCoolingCoilFanPower() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetRatedCoolingCoilFanPower();
}

bool RefrigerationWalkIn::setRatedCirculationFanPower(double ratedCirculationFanPower) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setRatedCirculationFanPower(ratedCirculationFanPower);
}

void RefrigerationWalkIn::resetRatedCirculationFanPower() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetRatedCirculationFanPower();
}

void RefrigerationWalkIn::setRatedTotalLightingPower(double ratedTotalLightingPower) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setRatedTotalLightingPower(ratedTotalLightingPower);
}

bool RefrigerationWalkIn::setLightingSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setLightingSchedule(schedule);
}

void RefrigerationWalkIn::resetLightingSchedule() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetLightingSchedule();
}

bool RefrigerationWalkIn::setDefrostType(std::string defrostType) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setDefrostType(defrostType);
}

void RefrigerationWalkIn::resetDefrostType() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetDefrostType();
}

bool RefrigerationWalkIn::setDefrostControlType(std::string defrostControlType) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setDefrostControlType(defrostControlType);
}

void RefrigerationWalkIn::resetDefrostControlType() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetDefrostControlType();
}

bool RefrigerationWalkIn::setDefrostSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setDefrostSchedule(schedule);
}

bool RefrigerationWalkIn::setDefrostDripDownSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setDefrostDripDownSchedule(schedule);
}

void RefrigerationWalkIn::resetDefrostDripDownSchedule() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetDefrostDripDownSchedule();
}

bool RefrigerationWalkIn::setDefrostPower(double defrostPower) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setDefrostPower(defrostPower);
}

void RefrigerationWalkIn::resetDefrostPower() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetDefrostPower();
}

bool RefrigerationWalkIn::setTemperatureTerminationDefrostFractiontoIce(double temperatureTerminationDefrostFractiontoIce) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setTemperatureTerminationDefrostFractiontoIce(temperatureTerminationDefrostFractiontoIce);
}

void RefrigerationWalkIn::resetTemperatureTerminationDefrostFractiontoIce() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetTemperatureTerminationDefrostFractiontoIce();
}

bool RefrigerationWalkIn::setRestockingSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setRestockingSchedule(schedule);
}

void RefrigerationWalkIn::resetRestockingSchedule() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetRestockingSchedule();
}

void RefrigerationWalkIn::setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setAverageRefrigerantChargeInventory(averageRefrigerantChargeInventory);
}

void RefrigerationWalkIn::resetAverageRefrigerantChargeInventory() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetAverageRefrigerantChargeInventory();
}

bool RefrigerationWalkIn::setInsulatedFloorSurfaceArea(double insulatedFloorSurfaceArea) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setInsulatedFloorSurfaceArea(insulatedFloorSurfaceArea);
}

bool RefrigerationWalkIn::setInsulatedFloorUValue(double insulatedFloorUValue) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setInsulatedFloorUValue(insulatedFloorUValue);
}

void RefrigerationWalkIn::resetInsulatedFloorUValue() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetInsulatedFloorUValue();
}

bool RefrigerationWalkIn::addToSystem(RefrigerationSystem & system) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->addToSystem(system);
}

void RefrigerationWalkIn::removeFromSystem() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->removeFromSystem();
}

bool RefrigerationWalkIn::setZoneBoundaryThermalZone(const ThermalZone& zoneBoundaryThermalZone) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryThermalZone(zoneBoundaryThermalZone);
}

void RefrigerationWalkIn::resetZoneBoundaryThermalZone() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetZoneBoundaryThermalZone();
}

bool RefrigerationWalkIn::setZoneBoundaryTotalInsulatedSurfaceAreaFacingZone(double zoneBoundaryTotalInsulatedSurfaceAreaFacingZone) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryTotalInsulatedSurfaceAreaFacingZone(zoneBoundaryTotalInsulatedSurfaceAreaFacingZone);
}

void RefrigerationWalkIn::setZoneBoundaryAreaofGlassReachInDoorsFacingZone(double zoneBoundaryAreaofGlassReachInDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryAreaofGlassReachInDoorsFacingZone(zoneBoundaryAreaofGlassReachInDoorsFacingZone);
}

void RefrigerationWalkIn::setZoneBoundaryHeightofGlassReachInDoorsFacingZone(double zoneBoundaryHeightofGlassReachInDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryHeightofGlassReachInDoorsFacingZone(zoneBoundaryHeightofGlassReachInDoorsFacingZone);
}

void RefrigerationWalkIn::setZoneBoundaryAreaofStockingDoorsFacingZone(double zoneBoundaryAreaofStockingDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryAreaofStockingDoorsFacingZone(zoneBoundaryAreaofStockingDoorsFacingZone);
}

void RefrigerationWalkIn::setZoneBoundaryHeightofStockingDoorsFacingZone(double zoneBoundaryHeightofStockingDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryHeightofStockingDoorsFacingZone(zoneBoundaryHeightofStockingDoorsFacingZone);
}

bool RefrigerationWalkIn::setZoneBoundaryInsulatedSurfaceUValueFacingZone(double zoneBoundaryInsulatedSurfaceUValueFacingZone) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryInsulatedSurfaceUValueFacingZone(zoneBoundaryInsulatedSurfaceUValueFacingZone);
}

bool RefrigerationWalkIn::setZoneBoundaryGlassReachInDoorUValueFacingZone(double zoneBoundaryGlassReachInDoorUValueFacingZone) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryGlassReachInDoorUValueFacingZone(zoneBoundaryGlassReachInDoorUValueFacingZone);
}

bool RefrigerationWalkIn::setZoneBoundaryStockingDoorUValueFacingZone(double zoneBoundaryStockingDoorUValueFacingZone) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryStockingDoorUValueFacingZone(zoneBoundaryStockingDoorUValueFacingZone);
}

bool RefrigerationWalkIn::setZoneBoundaryStockingDoorOpeningScheduleFacingZone(Schedule& zoneBoundaryStockingDoorOpeningScheduleFacingZone) {
  return getImpl<detail::RefrigerationWalkIn_Impl>()->setZoneBoundaryStockingDoorOpeningScheduleFacingZone(zoneBoundaryStockingDoorOpeningScheduleFacingZone);
}

void RefrigerationWalkIn::resetZoneBoundaryStockingDoorOpeningScheduleFacingZone() {
  getImpl<detail::RefrigerationWalkIn_Impl>()->resetZoneBoundaryStockingDoorOpeningScheduleFacingZone();
}

/// @cond
RefrigerationWalkIn::RefrigerationWalkIn(boost::shared_ptr<detail::RefrigerationWalkIn_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

