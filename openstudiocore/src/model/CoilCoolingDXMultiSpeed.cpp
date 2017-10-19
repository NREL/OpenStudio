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

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CoilCoolingDXMultiSpeed.hpp"
#include "CoilCoolingDXMultiSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilCoolingDXMultiSpeedStageData.hpp"
#include "CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXMultiSpeed_Impl::CoilCoolingDXMultiSpeed_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXMultiSpeed::iddObjectType());
  }

  CoilCoolingDXMultiSpeed_Impl::CoilCoolingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXMultiSpeed::iddObjectType());
  }

  CoilCoolingDXMultiSpeed_Impl::CoilCoolingDXMultiSpeed_Impl(const CoilCoolingDXMultiSpeed_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXMultiSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXMultiSpeed_Impl::iddObjectType() const {
    return CoilCoolingDXMultiSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilCoolingDXMultiSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXMultiSpeed","Availability Schedule"));
    }
    if (std::find(b,e,OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilCoolingDXMultiSpeed","Basin Heater Operating Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> CoilCoolingDXMultiSpeed_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule);
  }

  std::string CoilCoolingDXMultiSpeed_Impl::condenserType() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::CondenserType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingDXMultiSpeed_Impl::applyPartLoadFractiontoSpeedsGreaterthan1() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool CoilCoolingDXMultiSpeed_Impl::applyLatentDegradationtoSpeedsGreaterthan1() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool CoilCoolingDXMultiSpeed_Impl::isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const {
    return isEmpty(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1);
  }

  double CoilCoolingDXMultiSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeed_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXMultiSpeed_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> CoilCoolingDXMultiSpeed_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule);
  }

  std::string CoilCoolingDXMultiSpeed_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_MultiSpeedFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingDXMultiSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule,
                              "CoilCoolingDXMultiSpeed",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void CoilCoolingDXMultiSpeed_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeed_Impl::setCondenserType(std::string condenserType) {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::CondenserType, condenserType);
    return result;
  }

  void CoilCoolingDXMultiSpeed_Impl::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
    if (applyPartLoadFractiontoSpeedsGreaterthan1) {
      setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "Yes");
    } else {
      setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "No");
    }
  }

  void CoilCoolingDXMultiSpeed_Impl::setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1) {
    if (applyLatentDegradationtoSpeedsGreaterthan1) {
      setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "Yes");
    } else {
      setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "No");
    }
  }

  void CoilCoolingDXMultiSpeed_Impl::resetApplyLatentDegradationtoSpeedsGreaterthan1() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilCoolingDXMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilCoolingDXMultiSpeed_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  bool CoilCoolingDXMultiSpeed_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  bool CoilCoolingDXMultiSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule,
                              "CoilCoolingDXMultiSpeed",
                              "Basin Heater Operating Schedule",
                              schedule);
    return result;
  }

  void CoilCoolingDXMultiSpeed_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingSchedule, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXMultiSpeed_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_Coil_Cooling_DX_MultiSpeedFields::FuelType, fuelType);
    return result;
  }

  unsigned CoilCoolingDXMultiSpeed_Impl::inletPort() {
    return OS_Coil_Cooling_DX_MultiSpeedFields::AirInletNode;
  }

  unsigned CoilCoolingDXMultiSpeed_Impl::outletPort() {
    return OS_Coil_Cooling_DX_MultiSpeedFields::AirOutletNode;
  }

  ModelObject CoilCoolingDXMultiSpeed_Impl::clone(Model model) const {
    auto t_clone = StraightComponent_Impl::clone(model).cast<CoilCoolingDXMultiSpeed>();

    auto t_stages = stages();
    for( auto stage: t_stages ) {
      auto stageClone = stage.clone(model).cast<CoilCoolingDXMultiSpeedStageData>();
      t_clone.addStage(stageClone);
    }
    return t_clone;
  }

  std::vector<ModelObject> CoilCoolingDXMultiSpeed_Impl::children() const {
    return subsetCastVector<ModelObject>(stages());
  }

  std::vector<CoilCoolingDXMultiSpeedStageData> CoilCoolingDXMultiSpeed_Impl::stages() const {
    std::vector<CoilCoolingDXMultiSpeedStageData> result;
    auto groups = extensibleGroups();
    for( auto group: groups ) {
      auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_Coil_Cooling_DX_MultiSpeedExtensibleFields::Stage);
      if( target ) {
        if( auto stage = target->optionalCast<CoilCoolingDXMultiSpeedStageData>() ) {
          result.push_back(stage.get());
        }
      }
    }
    return result;
  }

  void CoilCoolingDXMultiSpeed_Impl::addStage(CoilCoolingDXMultiSpeedStageData& stage) {
    auto group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    OS_ASSERT(! group.empty());
    group.setPointer(OS_Coil_Cooling_DX_MultiSpeedExtensibleFields::Stage,stage.handle());
  }

  boost::optional<HVACComponent> CoilCoolingDXMultiSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for( const auto & system : systems ) {
        auto coolingCoil = system.coolingCoil();
        if( coolingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }
    return boost::none;
  }

  bool CoilCoolingDXMultiSpeed_Impl::addToNode(Node & node)
  {
    return false;
  }

} // detail

CoilCoolingDXMultiSpeed::CoilCoolingDXMultiSpeed(const Model& model)
  : StraightComponent(CoilCoolingDXMultiSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXMultiSpeed_Impl>());

  setCondenserType("AirCooled");
  setApplyPartLoadFractiontoSpeedsGreaterthan1(false);
  setCrankcaseHeaterCapacity(0.0);
  setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  setBasinHeaterCapacity(0.0);
  setBasinHeaterSetpointTemperature(2.0);
  setFuelType("NaturalGas");
}

IddObjectType CoilCoolingDXMultiSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_MultiSpeed);
}

std::vector<std::string> CoilCoolingDXMultiSpeed::condenserTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Cooling_DX_MultiSpeedFields::CondenserType);
}

std::vector<std::string> CoilCoolingDXMultiSpeed::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Cooling_DX_MultiSpeedFields::FuelType);
}

boost::optional<Schedule> CoilCoolingDXMultiSpeed::availabilitySchedule() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->availabilitySchedule();
}

std::string CoilCoolingDXMultiSpeed::condenserType() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->condenserType();
}

bool CoilCoolingDXMultiSpeed::applyPartLoadFractiontoSpeedsGreaterthan1() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->applyPartLoadFractiontoSpeedsGreaterthan1();
}

bool CoilCoolingDXMultiSpeed::applyLatentDegradationtoSpeedsGreaterthan1() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->applyLatentDegradationtoSpeedsGreaterthan1();
}

bool CoilCoolingDXMultiSpeed::isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted();
}

double CoilCoolingDXMultiSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->crankcaseHeaterCapacity();
}

double CoilCoolingDXMultiSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

double CoilCoolingDXMultiSpeed::basinHeaterCapacity() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->basinHeaterCapacity();
}

double CoilCoolingDXMultiSpeed::basinHeaterSetpointTemperature() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->basinHeaterSetpointTemperature();
}

boost::optional<Schedule> CoilCoolingDXMultiSpeed::basinHeaterOperatingSchedule() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->basinHeaterOperatingSchedule();
}

std::string CoilCoolingDXMultiSpeed::fuelType() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->fuelType();
}

bool CoilCoolingDXMultiSpeed::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilCoolingDXMultiSpeed::resetAvailabilitySchedule() {
  getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->resetAvailabilitySchedule();
}

bool CoilCoolingDXMultiSpeed::setCondenserType(std::string condenserType) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setCondenserType(condenserType);
}

void CoilCoolingDXMultiSpeed::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
  getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setApplyPartLoadFractiontoSpeedsGreaterthan1(applyPartLoadFractiontoSpeedsGreaterthan1);
}

void CoilCoolingDXMultiSpeed::setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1) {
  getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setApplyLatentDegradationtoSpeedsGreaterthan1(applyLatentDegradationtoSpeedsGreaterthan1);
}

void CoilCoolingDXMultiSpeed::resetApplyLatentDegradationtoSpeedsGreaterthan1() {
  getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->resetApplyLatentDegradationtoSpeedsGreaterthan1();
}

bool CoilCoolingDXMultiSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilCoolingDXMultiSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

bool CoilCoolingDXMultiSpeed::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

bool CoilCoolingDXMultiSpeed::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

bool CoilCoolingDXMultiSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
}

void CoilCoolingDXMultiSpeed::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
}

bool CoilCoolingDXMultiSpeed::setFuelType(std::string fuelType) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->setFuelType(fuelType);
}

std::vector<CoilCoolingDXMultiSpeedStageData> CoilCoolingDXMultiSpeed::stages() const {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->stages();
}

void CoilCoolingDXMultiSpeed::addStage(CoilCoolingDXMultiSpeedStageData& stage) {
  return getImpl<detail::CoilCoolingDXMultiSpeed_Impl>()->addStage(stage);
}

/// @cond
CoilCoolingDXMultiSpeed::CoilCoolingDXMultiSpeed(std::shared_ptr<detail::CoilCoolingDXMultiSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

