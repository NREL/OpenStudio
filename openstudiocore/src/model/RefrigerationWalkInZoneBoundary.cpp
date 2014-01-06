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

#include <model/RefrigerationWalkInZoneBoundary.hpp>
#include <model/RefrigerationWalkInZoneBoundary_Impl.hpp>

#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Refrigeration_WalkIn_ZoneBoundary_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationWalkInZoneBoundary_Impl::RefrigerationWalkInZoneBoundary_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationWalkInZoneBoundary::iddObjectType());
  }

  RefrigerationWalkInZoneBoundary_Impl::RefrigerationWalkInZoneBoundary_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationWalkInZoneBoundary::iddObjectType());
  }

  RefrigerationWalkInZoneBoundary_Impl::RefrigerationWalkInZoneBoundary_Impl(const RefrigerationWalkInZoneBoundary_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationWalkInZoneBoundary_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationWalkInZoneBoundary_Impl::iddObjectType() const {
    return RefrigerationWalkInZoneBoundary::iddObjectType();
  }

  std::vector<ScheduleTypeKey> RefrigerationWalkInZoneBoundary_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorOpeningScheduleNameFacingZone) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkInZoneBoundary","Glass Reach In Door Opening  Facing Zone"));
    }
    if (std::find(b,e,OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningScheduleNameFacingZone) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationWalkInZoneBoundary","Stocking Door Opening  Facing Zone"));
    }
    return result;
  }

  ModelObject RefrigerationWalkInZoneBoundary_Impl::clone(Model model) const
  {
    RefrigerationWalkInZoneBoundary modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationWalkInZoneBoundary>();

    modelObjectClone.resetThermalZone();

    return modelObjectClone;
  }

  boost::optional<ThermalZone> RefrigerationWalkInZoneBoundary_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_WalkIn_ZoneBoundaryFields::ZoneName);
  }

  double RefrigerationWalkInZoneBoundary_Impl::totalInsulatedSurfaceAreaFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::TotalInsulatedSurfaceAreaFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationWalkInZoneBoundary_Impl::insulatedSurfaceUValueFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::InsulatedSurfaceUValueFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isInsulatedSurfaceUValueFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::InsulatedSurfaceUValueFacingZone);
  }

  double RefrigerationWalkInZoneBoundary_Impl::areaofGlassReachInDoorsFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofGlassReachInDoorsFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isAreaofGlassReachInDoorsFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofGlassReachInDoorsFacingZone);
  }

  double RefrigerationWalkInZoneBoundary_Impl::heightofGlassReachInDoorsFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofGlassReachInDoorsFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isHeightofGlassReachInDoorsFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofGlassReachInDoorsFacingZone);
  }

  double RefrigerationWalkInZoneBoundary_Impl::glassReachInDoorUValueFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorUValueFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isGlassReachInDoorUValueFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorUValueFacingZone);
  }

  boost::optional<Schedule> RefrigerationWalkInZoneBoundary_Impl::glassReachInDoorOpeningScheduleFacingZone() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorOpeningScheduleNameFacingZone);
  }

  double RefrigerationWalkInZoneBoundary_Impl::areaofStockingDoorsFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofStockingDoorsFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isAreaofStockingDoorsFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofStockingDoorsFacingZone);
  }

  double RefrigerationWalkInZoneBoundary_Impl::heightofStockingDoorsFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofStockingDoorsFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isHeightofStockingDoorsFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofStockingDoorsFacingZone);
  }

  double RefrigerationWalkInZoneBoundary_Impl::stockingDoorUValueFacingZone() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorUValueFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isStockingDoorUValueFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorUValueFacingZone);
  }

  boost::optional<Schedule> RefrigerationWalkInZoneBoundary_Impl::stockingDoorOpeningScheduleFacingZone() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningScheduleNameFacingZone);
  }

  std::string RefrigerationWalkInZoneBoundary_Impl::stockingDoorOpeningProtectionTypeFacingZone() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningProtectionTypeFacingZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationWalkInZoneBoundary_Impl::isStockingDoorOpeningProtectionTypeFacingZoneDefaulted() const {
    return isEmpty(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningProtectionTypeFacingZone);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setThermalZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_Refrigeration_WalkIn_ZoneBoundaryFields::ZoneName, thermalZone.handle());
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetThermalZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::ZoneName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setTotalInsulatedSurfaceAreaFacingZone(double totalInsulatedSurfaceAreaFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::TotalInsulatedSurfaceAreaFacingZone, totalInsulatedSurfaceAreaFacingZone);
    return result;
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setInsulatedSurfaceUValueFacingZone(double insulatedSurfaceUValueFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::InsulatedSurfaceUValueFacingZone, insulatedSurfaceUValueFacingZone);
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetInsulatedSurfaceUValueFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::InsulatedSurfaceUValueFacingZone, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::setAreaofGlassReachInDoorsFacingZone(double areaofGlassReachInDoorsFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofGlassReachInDoorsFacingZone, areaofGlassReachInDoorsFacingZone);
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetAreaofGlassReachInDoorsFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofGlassReachInDoorsFacingZone, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::setHeightofGlassReachInDoorsFacingZone(double heightofGlassReachInDoorsFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofGlassReachInDoorsFacingZone, heightofGlassReachInDoorsFacingZone);
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetHeightofGlassReachInDoorsFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofGlassReachInDoorsFacingZone, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setGlassReachInDoorUValueFacingZone(double glassReachInDoorUValueFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorUValueFacingZone, glassReachInDoorUValueFacingZone);
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetGlassReachInDoorUValueFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorUValueFacingZone, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setGlassReachInDoorOpeningScheduleFacingZone(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorOpeningScheduleNameFacingZone,
                              "RefrigerationWalkInZoneBoundary",
                              "Glass Reach In Door Opening  Facing Zone",
                              schedule);
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetGlassReachInDoorOpeningScheduleFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::GlassReachInDoorOpeningScheduleNameFacingZone, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::setAreaofStockingDoorsFacingZone(double areaofStockingDoorsFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofStockingDoorsFacingZone, areaofStockingDoorsFacingZone);
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetAreaofStockingDoorsFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::AreaofStockingDoorsFacingZone, "");
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::setHeightofStockingDoorsFacingZone(double heightofStockingDoorsFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofStockingDoorsFacingZone, heightofStockingDoorsFacingZone);
    OS_ASSERT(result);
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetHeightofStockingDoorsFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::HeightofStockingDoorsFacingZone, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setStockingDoorUValueFacingZone(double stockingDoorUValueFacingZone) {
    bool result = setDouble(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorUValueFacingZone, stockingDoorUValueFacingZone);
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetStockingDoorUValueFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorUValueFacingZone, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setStockingDoorOpeningScheduleFacingZone(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningScheduleNameFacingZone,
                              "RefrigerationWalkInZoneBoundary",
                              "Stocking Door Opening  Facing Zone",
                              schedule);
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetStockingDoorOpeningScheduleFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningScheduleNameFacingZone, "");
    OS_ASSERT(result);
  }

  bool RefrigerationWalkInZoneBoundary_Impl::setStockingDoorOpeningProtectionTypeFacingZone(std::string stockingDoorOpeningProtectionTypeFacingZone) {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningProtectionTypeFacingZone, stockingDoorOpeningProtectionTypeFacingZone);
    return result;
  }

  void RefrigerationWalkInZoneBoundary_Impl::resetStockingDoorOpeningProtectionTypeFacingZone() {
    bool result = setString(OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningProtectionTypeFacingZone, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationWalkInZoneBoundary::RefrigerationWalkInZoneBoundary(const Model& model)
  : ModelObject(RefrigerationWalkInZoneBoundary::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>());

  bool ok = true;
  ok = setTotalInsulatedSurfaceAreaFacingZone(43.4);
  OS_ASSERT(ok);
  ok = setInsulatedSurfaceUValueFacingZone(.235);
  OS_ASSERT(ok);
  setAreaofStockingDoorsFacingZone(2.0);
  setHeightofStockingDoorsFacingZone(2.0);
  ok = setStockingDoorOpeningProtectionTypeFacingZone("StripCurtain");
  OS_ASSERT(ok);
}

IddObjectType RefrigerationWalkInZoneBoundary::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_WalkIn_ZoneBoundary);
}

std::vector<std::string> RefrigerationWalkInZoneBoundary::stockingDoorOpeningProtectionTypeFacingZoneValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_WalkIn_ZoneBoundaryFields::StockingDoorOpeningProtectionTypeFacingZone);
}

boost::optional<ThermalZone> RefrigerationWalkInZoneBoundary::thermalZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->thermalZone();
}

double RefrigerationWalkInZoneBoundary::totalInsulatedSurfaceAreaFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->totalInsulatedSurfaceAreaFacingZone();
}

double RefrigerationWalkInZoneBoundary::insulatedSurfaceUValueFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->insulatedSurfaceUValueFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isInsulatedSurfaceUValueFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isInsulatedSurfaceUValueFacingZoneDefaulted();
}

double RefrigerationWalkInZoneBoundary::areaofGlassReachInDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->areaofGlassReachInDoorsFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isAreaofGlassReachInDoorsFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isAreaofGlassReachInDoorsFacingZoneDefaulted();
}

double RefrigerationWalkInZoneBoundary::heightofGlassReachInDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->heightofGlassReachInDoorsFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isHeightofGlassReachInDoorsFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isHeightofGlassReachInDoorsFacingZoneDefaulted();
}

double RefrigerationWalkInZoneBoundary::glassReachInDoorUValueFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->glassReachInDoorUValueFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isGlassReachInDoorUValueFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isGlassReachInDoorUValueFacingZoneDefaulted();
}

boost::optional<Schedule> RefrigerationWalkInZoneBoundary::glassReachInDoorOpeningScheduleFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->glassReachInDoorOpeningScheduleFacingZone();
}

double RefrigerationWalkInZoneBoundary::areaofStockingDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->areaofStockingDoorsFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isAreaofStockingDoorsFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isAreaofStockingDoorsFacingZoneDefaulted();
}

double RefrigerationWalkInZoneBoundary::heightofStockingDoorsFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->heightofStockingDoorsFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isHeightofStockingDoorsFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isHeightofStockingDoorsFacingZoneDefaulted();
}

double RefrigerationWalkInZoneBoundary::stockingDoorUValueFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->stockingDoorUValueFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isStockingDoorUValueFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isStockingDoorUValueFacingZoneDefaulted();
}

boost::optional<Schedule> RefrigerationWalkInZoneBoundary::stockingDoorOpeningScheduleFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->stockingDoorOpeningScheduleFacingZone();
}

std::string RefrigerationWalkInZoneBoundary::stockingDoorOpeningProtectionTypeFacingZone() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->stockingDoorOpeningProtectionTypeFacingZone();
}

bool RefrigerationWalkInZoneBoundary::isStockingDoorOpeningProtectionTypeFacingZoneDefaulted() const {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->isStockingDoorOpeningProtectionTypeFacingZoneDefaulted();
}

bool RefrigerationWalkInZoneBoundary::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setThermalZone(thermalZone);
}

void RefrigerationWalkInZoneBoundary::resetThermalZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetThermalZone();
}

bool RefrigerationWalkInZoneBoundary::setTotalInsulatedSurfaceAreaFacingZone(double totalInsulatedSurfaceAreaFacingZone) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setTotalInsulatedSurfaceAreaFacingZone(totalInsulatedSurfaceAreaFacingZone);
}

bool RefrigerationWalkInZoneBoundary::setInsulatedSurfaceUValueFacingZone(double insulatedSurfaceUValueFacingZone) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setInsulatedSurfaceUValueFacingZone(insulatedSurfaceUValueFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetInsulatedSurfaceUValueFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetInsulatedSurfaceUValueFacingZone();
}

void RefrigerationWalkInZoneBoundary::setAreaofGlassReachInDoorsFacingZone(double areaofGlassReachInDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setAreaofGlassReachInDoorsFacingZone(areaofGlassReachInDoorsFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetAreaofGlassReachInDoorsFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetAreaofGlassReachInDoorsFacingZone();
}

void RefrigerationWalkInZoneBoundary::setHeightofGlassReachInDoorsFacingZone(double heightofGlassReachInDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setHeightofGlassReachInDoorsFacingZone(heightofGlassReachInDoorsFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetHeightofGlassReachInDoorsFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetHeightofGlassReachInDoorsFacingZone();
}

bool RefrigerationWalkInZoneBoundary::setGlassReachInDoorUValueFacingZone(double glassReachInDoorUValueFacingZone) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setGlassReachInDoorUValueFacingZone(glassReachInDoorUValueFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetGlassReachInDoorUValueFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetGlassReachInDoorUValueFacingZone();
}

bool RefrigerationWalkInZoneBoundary::setGlassReachInDoorOpeningScheduleFacingZone(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setGlassReachInDoorOpeningScheduleFacingZone(schedule);
}

void RefrigerationWalkInZoneBoundary::resetGlassReachInDoorOpeningScheduleFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetGlassReachInDoorOpeningScheduleFacingZone();
}

void RefrigerationWalkInZoneBoundary::setAreaofStockingDoorsFacingZone(double areaofStockingDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setAreaofStockingDoorsFacingZone(areaofStockingDoorsFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetAreaofStockingDoorsFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetAreaofStockingDoorsFacingZone();
}

void RefrigerationWalkInZoneBoundary::setHeightofStockingDoorsFacingZone(double heightofStockingDoorsFacingZone) {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setHeightofStockingDoorsFacingZone(heightofStockingDoorsFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetHeightofStockingDoorsFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetHeightofStockingDoorsFacingZone();
}

bool RefrigerationWalkInZoneBoundary::setStockingDoorUValueFacingZone(double stockingDoorUValueFacingZone) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setStockingDoorUValueFacingZone(stockingDoorUValueFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetStockingDoorUValueFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetStockingDoorUValueFacingZone();
}

bool RefrigerationWalkInZoneBoundary::setStockingDoorOpeningScheduleFacingZone(Schedule& schedule) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setStockingDoorOpeningScheduleFacingZone(schedule);
}

void RefrigerationWalkInZoneBoundary::resetStockingDoorOpeningScheduleFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetStockingDoorOpeningScheduleFacingZone();
}

bool RefrigerationWalkInZoneBoundary::setStockingDoorOpeningProtectionTypeFacingZone(std::string stockingDoorOpeningProtectionTypeFacingZone) {
  return getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->setStockingDoorOpeningProtectionTypeFacingZone(stockingDoorOpeningProtectionTypeFacingZone);
}

void RefrigerationWalkInZoneBoundary::resetStockingDoorOpeningProtectionTypeFacingZone() {
  getImpl<detail::RefrigerationWalkInZoneBoundary_Impl>()->resetStockingDoorOpeningProtectionTypeFacingZone();
}

/// @cond
RefrigerationWalkInZoneBoundary::RefrigerationWalkInZoneBoundary(boost::shared_ptr<detail::RefrigerationWalkInZoneBoundary_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

