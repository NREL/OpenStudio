/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

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

  double PhotovoltaicPerformanceSimple_Impl::fractionOfSurfaceAreaWithActiveSolarCells() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceSimple_Impl::isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells);
  }

  std::string PhotovoltaicPerformanceSimple_Impl::conversionEfficiencyInputMode() const {
    boost::optional<std::string> result = getString(OS_PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode,true);
    OS_ASSERT(result);
    return result.get();
  }

  boost::optional<double> PhotovoltaicPerformanceSimple_Impl::fixedEfficiency() const {
    return getDouble(OS_PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed,true);
  }

  boost::optional<Schedule> PhotovoltaicPerformanceSimple_Impl::efficiencySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setFractionOfSurfaceAreaWithActiveSolarCells(double fractionOfSurfaceAreaWithActiveSolarCells) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells, fractionOfSurfaceAreaWithActiveSolarCells);
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetFractionOfSurfaceAreaWithActiveSolarCells() {
    bool result = setString(OS_PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setConversionEfficiencyInputMode(const std::string& conversionEfficiencyInputMode) {
    return setString(OS_PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode, conversionEfficiencyInputMode);
  }

  bool PhotovoltaicPerformanceSimple_Impl::setFixedEfficiency(double fixedEfficiency) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed, fixedEfficiency);
    if (result){
      result = setConversionEfficiencyInputMode("Fixed");
      OS_ASSERT(result);
      result = setString(OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName, "");
      OS_ASSERT(result);
    }
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetFixedEfficiency() {
    setDefaultFixedEfficiency();
  }

  bool PhotovoltaicPerformanceSimple_Impl::setEfficiencySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName,
                              "PhotovoltaicPerformanceSimple",
                              "Efficiency",
                              schedule);
    if (result) {
      result = setConversionEfficiencyInputMode("Scheduled");
      OS_ASSERT(result);
      result = setString(OS_PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed, "");
      OS_ASSERT(result);
    }
    return result;
  }

  void PhotovoltaicPerformanceSimple_Impl::resetEfficiencySchedule() {
    setDefaultFixedEfficiency();
  }

  void PhotovoltaicPerformanceSimple_Impl::setDefaultFixedEfficiency() {
    bool result = setFixedEfficiency(0.12);
    OS_ASSERT(result);
  }

} // detail

PhotovoltaicPerformanceSimple::PhotovoltaicPerformanceSimple(const Model& model)
  : PhotovoltaicPerformance(PhotovoltaicPerformanceSimple::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PhotovoltaicPerformanceSimple_Impl>());
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setDefaultFixedEfficiency();
}

IddObjectType PhotovoltaicPerformanceSimple::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PhotovoltaicPerformance_Simple);
}

double PhotovoltaicPerformanceSimple::fractionOfSurfaceAreaWithActiveSolarCells() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->fractionOfSurfaceAreaWithActiveSolarCells();
}

bool PhotovoltaicPerformanceSimple::isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted();
}

std::string PhotovoltaicPerformanceSimple::conversionEfficiencyInputMode() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->conversionEfficiencyInputMode();
}

boost::optional<double> PhotovoltaicPerformanceSimple::fixedEfficiency() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->fixedEfficiency();
}

boost::optional<Schedule> PhotovoltaicPerformanceSimple::efficiencySchedule() const {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->efficiencySchedule();
}

bool PhotovoltaicPerformanceSimple::setFractionOfSurfaceAreaWithActiveSolarCells(double fractionOfSurfaceAreaWithActiveSolarCells) {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setFractionOfSurfaceAreaWithActiveSolarCells(fractionOfSurfaceAreaWithActiveSolarCells);
}

void PhotovoltaicPerformanceSimple::resetFractionOfSurfaceAreaWithActiveSolarCells() {
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->resetFractionOfSurfaceAreaWithActiveSolarCells();
}

bool PhotovoltaicPerformanceSimple::setFixedEfficiency(double fixedEfficiency) {
  return getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->setFixedEfficiency(fixedEfficiency);
}

void PhotovoltaicPerformanceSimple::resetFixedEfficiency() {
  getImpl<detail::PhotovoltaicPerformanceSimple_Impl>()->resetFixedEfficiency();
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

