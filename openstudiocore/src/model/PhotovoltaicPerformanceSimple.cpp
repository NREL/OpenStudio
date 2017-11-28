/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  : PhotovoltaicPerformance(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

