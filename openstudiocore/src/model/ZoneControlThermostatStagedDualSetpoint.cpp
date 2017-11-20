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

#include "ZoneControlThermostatStagedDualSetpoint.hpp"
#include "ZoneControlThermostatStagedDualSetpoint_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_ZoneControl_Thermostat_StagedDualSetpoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneControlThermostatStagedDualSetpoint_Impl::ZoneControlThermostatStagedDualSetpoint_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : Thermostat_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneControlThermostatStagedDualSetpoint::iddObjectType());
  }

  ZoneControlThermostatStagedDualSetpoint_Impl::ZoneControlThermostatStagedDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : Thermostat_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneControlThermostatStagedDualSetpoint::iddObjectType());
  }

  ZoneControlThermostatStagedDualSetpoint_Impl::ZoneControlThermostatStagedDualSetpoint_Impl(const ZoneControlThermostatStagedDualSetpoint_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : Thermostat_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneControlThermostatStagedDualSetpoint_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneControlThermostatStagedDualSetpoint_Impl::iddObjectType() const {
    return ZoneControlThermostatStagedDualSetpoint::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneControlThermostatStagedDualSetpoint_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneControl_Thermostat_StagedDualSetpointFields::HeatingTemperatureSetpointSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlThermostatStagedDualSetpoint","Heating Temperature Setpoint Schedule"));
    }
    if (std::find(b,e,OS_ZoneControl_Thermostat_StagedDualSetpointFields::CoolingTemperatureSetpointBaseSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlThermostatStagedDualSetpoint","Cooling Temperature Setpoint Base Schedule"));
    }
    return result;
  }

  int ZoneControlThermostatStagedDualSetpoint_Impl::numberofHeatingStages() const {
    boost::optional<int> value = getInt(OS_ZoneControl_Thermostat_StagedDualSetpointFields::NumberofHeatingStages,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneControlThermostatStagedDualSetpoint_Impl::heatingTemperatureSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_Thermostat_StagedDualSetpointFields::HeatingTemperatureSetpointSchedule);
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::heatingThrottlingTemperatureRange() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::HeatingThrottlingTemperatureRange,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage1HeatingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage1HeatingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage2HeatingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage2HeatingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage3HeatingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage3HeatingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage4HeatingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage4HeatingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  int ZoneControlThermostatStagedDualSetpoint_Impl::numberofCoolingStages() const {
    boost::optional<int> value = getInt(OS_ZoneControl_Thermostat_StagedDualSetpointFields::NumberofCoolingStages,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneControlThermostatStagedDualSetpoint_Impl::coolingTemperatureSetpointBaseSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_Thermostat_StagedDualSetpointFields::CoolingTemperatureSetpointBaseSchedule);
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::coolingThrottlingTemperatureRange() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::CoolingThrottlingTemperatureRange,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage1CoolingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage1CoolingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage2CoolingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage2CoolingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage3CoolingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage3CoolingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneControlThermostatStagedDualSetpoint_Impl::stage4CoolingTemperatureOffset() const {
    boost::optional<double> value = getDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage4CoolingTemperatureOffset,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setNumberofHeatingStages(int numberofHeatingStages) {
    bool result = setInt(OS_ZoneControl_Thermostat_StagedDualSetpointFields::NumberofHeatingStages, numberofHeatingStages);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setHeatingTemperatureSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_Thermostat_StagedDualSetpointFields::HeatingTemperatureSetpointSchedule,
                              "ZoneControlThermostatStagedDualSetpoint",
                              "Heating Temperature Setpoint Schedule",
                              schedule);
    return result;
  }

  void ZoneControlThermostatStagedDualSetpoint_Impl::resetHeatingTemperatureSetpointSchedule() {
    bool result = setString(OS_ZoneControl_Thermostat_StagedDualSetpointFields::HeatingTemperatureSetpointSchedule, "");
    OS_ASSERT(result);
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setHeatingThrottlingTemperatureRange(double heatingThrottlingTemperatureRange) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::HeatingThrottlingTemperatureRange, heatingThrottlingTemperatureRange);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage1HeatingTemperatureOffset(double stage1HeatingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage1HeatingTemperatureOffset, stage1HeatingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage2HeatingTemperatureOffset(double stage2HeatingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage2HeatingTemperatureOffset, stage2HeatingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage3HeatingTemperatureOffset(double stage3HeatingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage3HeatingTemperatureOffset, stage3HeatingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage4HeatingTemperatureOffset(double stage4HeatingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage4HeatingTemperatureOffset, stage4HeatingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setNumberofCoolingStages(int numberofCoolingStages) {
    bool result = setInt(OS_ZoneControl_Thermostat_StagedDualSetpointFields::NumberofCoolingStages, numberofCoolingStages);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setCoolingTemperatureSetpointBaseSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_Thermostat_StagedDualSetpointFields::CoolingTemperatureSetpointBaseSchedule,
                              "ZoneControlThermostatStagedDualSetpoint",
                              "Cooling Temperature Setpoint Base Schedule",
                              schedule);
    return result;
  }

  void ZoneControlThermostatStagedDualSetpoint_Impl::resetCoolingTemperatureSetpointBaseSchedule() {
    bool result = setString(OS_ZoneControl_Thermostat_StagedDualSetpointFields::CoolingTemperatureSetpointBaseSchedule, "");
    OS_ASSERT(result);
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setCoolingThrottlingTemperatureRange(double coolingThrottlingTemperatureRange) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::CoolingThrottlingTemperatureRange, coolingThrottlingTemperatureRange);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage1CoolingTemperatureOffset(double stage1CoolingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage1CoolingTemperatureOffset, stage1CoolingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage2CoolingTemperatureOffset(double stage2CoolingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage2CoolingTemperatureOffset, stage2CoolingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage3CoolingTemperatureOffset(double stage3CoolingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage3CoolingTemperatureOffset, stage3CoolingTemperatureOffset);
    return result;
  }

  bool ZoneControlThermostatStagedDualSetpoint_Impl::setStage4CoolingTemperatureOffset(double stage4CoolingTemperatureOffset) {
    bool result = setDouble(OS_ZoneControl_Thermostat_StagedDualSetpointFields::Stage4CoolingTemperatureOffset, stage4CoolingTemperatureOffset);
    return result;
  }

} // detail

ZoneControlThermostatStagedDualSetpoint::ZoneControlThermostatStagedDualSetpoint(const Model& model)
  : Thermostat(ZoneControlThermostatStagedDualSetpoint::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>());

  setNumberofHeatingStages(2);
  setHeatingThrottlingTemperatureRange(1.1);
  setStage1HeatingTemperatureOffset(0.0);
  setStage2HeatingTemperatureOffset(-0.5);
  setStage3HeatingTemperatureOffset(-1.0);
  setStage4HeatingTemperatureOffset(-1.5);
  setNumberofCoolingStages(4);
  setCoolingThrottlingTemperatureRange(1.1);
  setStage1CoolingTemperatureOffset(0.0);
  setStage2CoolingTemperatureOffset(1.0);
  setStage3CoolingTemperatureOffset(2.0);
  setStage4CoolingTemperatureOffset(3.0);
}

IddObjectType ZoneControlThermostatStagedDualSetpoint::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneControl_Thermostat_StagedDualSetpoint);
}

int ZoneControlThermostatStagedDualSetpoint::numberofHeatingStages() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->numberofHeatingStages();
}

boost::optional<Schedule> ZoneControlThermostatStagedDualSetpoint::heatingTemperatureSetpointSchedule() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->heatingTemperatureSetpointSchedule();
}

double ZoneControlThermostatStagedDualSetpoint::heatingThrottlingTemperatureRange() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->heatingThrottlingTemperatureRange();
}

double ZoneControlThermostatStagedDualSetpoint::stage1HeatingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage1HeatingTemperatureOffset();
}

double ZoneControlThermostatStagedDualSetpoint::stage2HeatingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage2HeatingTemperatureOffset();
}

double ZoneControlThermostatStagedDualSetpoint::stage3HeatingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage3HeatingTemperatureOffset();
}

double ZoneControlThermostatStagedDualSetpoint::stage4HeatingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage4HeatingTemperatureOffset();
}

int ZoneControlThermostatStagedDualSetpoint::numberofCoolingStages() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->numberofCoolingStages();
}

boost::optional<Schedule> ZoneControlThermostatStagedDualSetpoint::coolingTemperatureSetpointBaseSchedule() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->coolingTemperatureSetpointBaseSchedule();
}

double ZoneControlThermostatStagedDualSetpoint::coolingThrottlingTemperatureRange() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->coolingThrottlingTemperatureRange();
}

double ZoneControlThermostatStagedDualSetpoint::stage1CoolingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage1CoolingTemperatureOffset();
}

double ZoneControlThermostatStagedDualSetpoint::stage2CoolingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage2CoolingTemperatureOffset();
}

double ZoneControlThermostatStagedDualSetpoint::stage3CoolingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage3CoolingTemperatureOffset();
}

double ZoneControlThermostatStagedDualSetpoint::stage4CoolingTemperatureOffset() const {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->stage4CoolingTemperatureOffset();
}

bool ZoneControlThermostatStagedDualSetpoint::setNumberofHeatingStages(int numberofHeatingStages) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setNumberofHeatingStages(numberofHeatingStages);
}

bool ZoneControlThermostatStagedDualSetpoint::setHeatingTemperatureSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setHeatingTemperatureSetpointSchedule(schedule);
}

void ZoneControlThermostatStagedDualSetpoint::resetHeatingTemperatureSetpointSchedule() {
  getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->resetHeatingTemperatureSetpointSchedule();
}

bool ZoneControlThermostatStagedDualSetpoint::setHeatingThrottlingTemperatureRange(double heatingThrottlingTemperatureRange) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setHeatingThrottlingTemperatureRange(heatingThrottlingTemperatureRange);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage1HeatingTemperatureOffset(double stage1HeatingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage1HeatingTemperatureOffset(stage1HeatingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage2HeatingTemperatureOffset(double stage2HeatingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage2HeatingTemperatureOffset(stage2HeatingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage3HeatingTemperatureOffset(double stage3HeatingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage3HeatingTemperatureOffset(stage3HeatingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage4HeatingTemperatureOffset(double stage4HeatingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage4HeatingTemperatureOffset(stage4HeatingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setNumberofCoolingStages(int numberofCoolingStages) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setNumberofCoolingStages(numberofCoolingStages);
}

bool ZoneControlThermostatStagedDualSetpoint::setCoolingTemperatureSetpointBaseSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setCoolingTemperatureSetpointBaseSchedule(schedule);
}

void ZoneControlThermostatStagedDualSetpoint::resetCoolingTemperatureSetpointBaseSchedule() {
  getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->resetCoolingTemperatureSetpointBaseSchedule();
}

bool ZoneControlThermostatStagedDualSetpoint::setCoolingThrottlingTemperatureRange(double coolingThrottlingTemperatureRange) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setCoolingThrottlingTemperatureRange(coolingThrottlingTemperatureRange);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage1CoolingTemperatureOffset(double stage1CoolingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage1CoolingTemperatureOffset(stage1CoolingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage2CoolingTemperatureOffset(double stage2CoolingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage2CoolingTemperatureOffset(stage2CoolingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage3CoolingTemperatureOffset(double stage3CoolingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage3CoolingTemperatureOffset(stage3CoolingTemperatureOffset);
}

bool ZoneControlThermostatStagedDualSetpoint::setStage4CoolingTemperatureOffset(double stage4CoolingTemperatureOffset) {
  return getImpl<detail::ZoneControlThermostatStagedDualSetpoint_Impl>()->setStage4CoolingTemperatureOffset(stage4CoolingTemperatureOffset);
}

/// @cond
ZoneControlThermostatStagedDualSetpoint::ZoneControlThermostatStagedDualSetpoint(std::shared_ptr<detail::ZoneControlThermostatStagedDualSetpoint_Impl> impl)
  : Thermostat(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

