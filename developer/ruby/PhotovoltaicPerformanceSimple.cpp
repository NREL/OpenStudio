/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "PhotovoltaicPerformanceSimple.hpp"
#include "PhotovoltaicPerformanceSimple_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PhotovoltaicPerformance_Simple_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PhotovoltaicPerformanceSimple_Impl::PhotovoltaicPerformanceSimple_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PhotovoltaicPerformance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PhotovoltaicPerformanceSimple::iddObjectType());
  }

  PhotovoltaicPerformanceSimple_Impl::PhotovoltaicPerformanceSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PhotovoltaicPerformance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PhotovoltaicPerformanceSimple::iddObjectType());
  }

  PhotovoltaicPerformanceSimple_Impl::PhotovoltaicPerformanceSimple_Impl(const PhotovoltaicPerformanceSimple_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PhotovoltaicPerformance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PhotovoltaicPerformanceSimple_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PhotovoltaicPerformanceSimple_Impl::iddObjectType() const {
    return PhotovoltaicPerformanceSimple::iddObjectType();
  }

  std::vector<ScheduleTypeKey> PhotovoltaicPerformanceSimple_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("PhotovoltaicPerformanceSimple","Efficiency"));
    }
    return result;
  }

  double PhotovoltaicPerformanceSimple_Impl::fractionofSurfaceAreawithActiveSolarCells() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceSimple_Impl::isFractionofSurfaceAreawithActiveSolarCellsDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells);
  }

  boost::optional<std::string> PhotovoltaicPerformanceSimple_Impl::conversionEfficiencyInputMode() const {
    return getString(OS_PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode,true);
  }

  boost::optional<double> PhotovoltaicPerformanceSimple_Impl::valueforCellEfficiencyifFixed() const {
    return getDouble(OS_PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed,true);
  }

  boost::optional<Schedule> PhotovoltaicPerformanceSimple_Impl::efficiencySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setFractionofSurfaceAreawithActiveSolarCells(double fractionofSurfaceAreawithActiveSolarCells) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells, fractionofSurfaceAreawithActiveSolarCells);
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetFractionofSurfaceAreawithActiveSolarCells() {
    bool result = setString(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setConversionEfficiencyInputMode(boost::optional<std::string> conversionEfficiencyInputMode) {
    bool result(false);
    if (conversionEfficiencyInputMode) {
      result = setString(OS_PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode, conversionEfficiencyInputMode.get());
    }
    else {
      resetConversionEfficiencyInputMode();
      result = true;
    }
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetConversionEfficiencyInputMode() {
    bool result = setString(OS_PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setValueforCellEfficiencyifFixed(boost::optional<double> valueforCellEfficiencyifFixed) {
    bool result(false);
    if (valueforCellEfficiencyifFixed) {
      result = setDouble(OS_PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed, valueforCellEfficiencyifFixed.get());
    }
    else {
      resetValueforCellEfficiencyifFixed();
      result = true;
    }
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetValueforCellEfficiencyifFixed() {
    bool result = setString(OS_PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setEfficiencySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName,
                              "PhotovoltaicPerformanceSimple",
                              "Efficiency",
                              schedule);
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetEfficiencySchedule() {
    bool result = setString(OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

PhotovoltaicPerformanceSimple::PhotovoltaicPerformanceSimple(const Model& model)
  : PhotovoltaicPerformance(PhotovoltaicPerformanceSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PhotovoltaicPerformanceSimple_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType PhotovoltaicPerformanceSimple::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PhotovoltaicPerformance_Simple);
}

std::vector<std::string> PhotovoltaicPerformanceSimple::conversionEfficiencyInputModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode);
}

double PhotovoltaicPerformanceSimple::fractionofSurfaceAreawithActiveSolarCells() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->fractionofSurfaceAreawithActiveSolarCells();
}

bool PhotovoltaicPerformanceSimple::isFractionofSurfaceAreawithActiveSolarCellsDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->isFractionofSurfaceAreawithActiveSolarCellsDefaulted();
}

boost::optional<std::string> PhotovoltaicPerformanceSimple::conversionEfficiencyInputMode() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->conversionEfficiencyInputMode();
}

boost::optional<double> PhotovoltaicPerformanceSimple::valueforCellEfficiencyifFixed() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->valueforCellEfficiencyifFixed();
}

boost::optional<Schedule> PhotovoltaicPerformanceSimple::efficiencySchedule() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->efficiencySchedule();
}

bool PhotovoltaicPerformanceSimple::setFractionofSurfaceAreawithActiveSolarCells(double fractionofSurfaceAreawithActiveSolarCells) {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setFractionofSurfaceAreawithActiveSolarCells(fractionofSurfaceAreawithActiveSolarCells);
}

void PhotovoltaicPerformanceSimple::resetFractionofSurfaceAreawithActiveSolarCells() {
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->resetFractionofSurfaceAreawithActiveSolarCells();
}

bool PhotovoltaicPerformanceSimple::setConversionEfficiencyInputMode(std::string conversionEfficiencyInputMode) {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setConversionEfficiencyInputMode(conversionEfficiencyInputMode);
}

void PhotovoltaicPerformanceSimple::resetConversionEfficiencyInputMode() {
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->resetConversionEfficiencyInputMode();
}

bool PhotovoltaicPerformanceSimple::setValueforCellEfficiencyifFixed(double valueforCellEfficiencyifFixed) {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setValueforCellEfficiencyifFixed(valueforCellEfficiencyifFixed);
}

void PhotovoltaicPerformanceSimple::resetValueforCellEfficiencyifFixed() {
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->resetValueforCellEfficiencyifFixed();
}

bool PhotovoltaicPerformanceSimple::setEfficiencySchedule(Schedule& schedule) {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setEfficiencySchedule(schedule);
}

void PhotovoltaicPerformanceSimple::resetEfficiencySchedule() {
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->resetEfficiencySchedule();
}

/// @cond
PhotovoltaicPerformanceSimple::PhotovoltaicPerformanceSimple(std::shared_ptr<detail::PhotovoltaicPerformanceSimple_Impl> impl)
  : PhotovoltaicPerformance(impl)
{}
/// @endcond

} // model
} // openstudio

