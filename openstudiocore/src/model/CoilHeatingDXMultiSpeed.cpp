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

#include "CoilHeatingDXMultiSpeed.hpp"
#include "CoilHeatingDXMultiSpeed_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "CoilHeatingDXMultiSpeedStageData.hpp"
#include "CoilHeatingDXMultiSpeedStageData_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Heating_DX_MultiSpeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDXMultiSpeed_Impl::CoilHeatingDXMultiSpeed_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXMultiSpeed::iddObjectType());
  }

  CoilHeatingDXMultiSpeed_Impl::CoilHeatingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXMultiSpeed::iddObjectType());
  }

  CoilHeatingDXMultiSpeed_Impl::CoilHeatingDXMultiSpeed_Impl(const CoilHeatingDXMultiSpeed_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXMultiSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXMultiSpeed_Impl::iddObjectType() const {
    return CoilHeatingDXMultiSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingDXMultiSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingDXMultiSpeed","Availability"));
    }
    return result;
  }

  unsigned CoilHeatingDXMultiSpeed_Impl::inletPort() {
    return OS_Coil_Heating_DX_MultiSpeedFields::AirInletNodeName;
  }

  unsigned CoilHeatingDXMultiSpeed_Impl::outletPort() {
    return OS_Coil_Heating_DX_MultiSpeedFields::AirOutletNodeName;
  }

  boost::optional<Schedule> CoilHeatingDXMultiSpeed_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName);
  }

  double CoilHeatingDXMultiSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXMultiSpeed_Impl::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor,true);
  }

  double CoilHeatingDXMultiSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeed_Impl::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName);
  }

  double CoilHeatingDXMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilHeatingDXMultiSpeed_Impl::defrostStrategy() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostStrategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilHeatingDXMultiSpeed_Impl::defrostControl() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXMultiSpeed_Impl::defrostTimePeriodFraction() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXMultiSpeed_Impl::resistiveDefrostHeaterCapacity() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity,true);
  }

  bool CoilHeatingDXMultiSpeed_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::applyPartLoadFractiontoSpeedsGreaterthan1() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  std::string CoilHeatingDXMultiSpeed_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeedFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  int CoilHeatingDXMultiSpeed_Impl::regionnumberforCalculatingHSPF() const {
    boost::optional<int> value = getInt(OS_Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingDXMultiSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName,
                              "CoilHeatingDXMultiSpeed",
                              "Availability",
                              schedule);
    return result;
  }

  void CoilHeatingDXMultiSpeed_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  void CoilHeatingDXMultiSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, minimumOutdoorDryBulbTemperatureforCompressorOperation);
    OS_ASSERT(result);
  }

  void CoilHeatingDXMultiSpeed_Impl::setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor) {
    bool result(false);
    if (outdoorDryBulbTemperaturetoTurnOnCompressor) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, outdoorDryBulbTemperaturetoTurnOnCompressor.get());
    }
    else {
      resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
      result = true;
    }
    OS_ASSERT(result);
  }

  void CoilHeatingDXMultiSpeed_Impl::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXMultiSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, curve.get().handle());
    }
    else {
      resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void CoilHeatingDXMultiSpeed_Impl::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, maximumOutdoorDryBulbTemperatureforDefrostOperation);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setDefrostStrategy(std::string defrostStrategy) {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostStrategy, defrostStrategy);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setDefrostControl(std::string defrostControl) {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::DefrostControl, defrostControl);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity) {
    bool result(false);
    if (resistiveDefrostHeaterCapacity) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeed_Impl::autosizeResistiveDefrostHeaterCapacity() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingDXMultiSpeed_Impl::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
    setBooleanFieldValue(OS_Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, applyPartLoadFractiontoSpeedsGreaterthan1);
  }

  bool CoilHeatingDXMultiSpeed_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::FuelType, fuelType);
    return result;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF) {
    bool result = setInt(OS_Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF, regionnumberforCalculatingHSPF);
    return result;
  }

  ModelObject CoilHeatingDXMultiSpeed_Impl::clone(Model model) const {
    auto t_clone = StraightComponent_Impl::clone(model).cast<CoilHeatingDXMultiSpeed>();

    if (auto stageDataList = this->stageDataList()) {
      auto stageDataListClone = stageDataList->clone(model).cast<ModelObjectList>();
      t_clone.getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setStageDataList(stageDataListClone);
    }

    return t_clone;
  }

  std::vector<ModelObject> CoilHeatingDXMultiSpeed_Impl::children() const {
    std::vector<ModelObject> children;

    if( auto const _stageDataList = stageDataList() ) {
      for( const auto & mo : _stageDataList->modelObjects() ) {
        children.push_back( mo );
      }
    }

    if ( auto const curve = defrostEnergyInputRatioFunctionofTemperatureCurve() ) {
      children.push_back( curve.get() );
    }
    return children;
  }

  boost::optional<ModelObjectList> CoilHeatingDXMultiSpeed_Impl::stageDataList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Coil_Heating_DX_MultiSpeedFields::StageDataList);
  }

  bool CoilHeatingDXMultiSpeed_Impl::addStage( const CoilHeatingDXMultiSpeedStageData & stage) {
    auto modelObjectList = stageDataList();
    if( modelObjectList ) {
      modelObjectList->addModelObject(stage);
    }
    return false;
  }

  void CoilHeatingDXMultiSpeed_Impl::removeStage( const CoilHeatingDXMultiSpeedStageData & stage) {
    auto modelObjectList = stageDataList();
    if( modelObjectList ) {
      modelObjectList->removeModelObject(stage);
    }
  }

  void CoilHeatingDXMultiSpeed_Impl::removeAllStages() {
    auto modelObjectList = stageDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilHeatingDXMultiSpeedStageData>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  std::vector<CoilHeatingDXMultiSpeedStageData> CoilHeatingDXMultiSpeed_Impl::stages() const {
    std::vector<CoilHeatingDXMultiSpeedStageData> result;
    auto const modelObjectList = stageDataList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CoilHeatingDXMultiSpeedStageData>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }
    return result;
  }

  boost::optional<HVACComponent> CoilHeatingDXMultiSpeed_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for( const auto & system : systems ) {
        auto heatingCoil = system.heatingCoil();
        if( heatingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }
    return boost::none;
  }

  bool CoilHeatingDXMultiSpeed_Impl::addToNode(Node & node)
  {
    return false;
  }

  bool CoilHeatingDXMultiSpeed_Impl::setStageDataList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Coil_Heating_DX_MultiSpeedFields::StageDataList, modelObjectList.get().handle());
    }
    else {
      resetStageDataList();
      result = true;
    }
    return result;
  }

  void CoilHeatingDXMultiSpeed_Impl::resetStageDataList() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeedFields::StageDataList, "");
    OS_ASSERT(result);
  }

  std::vector<IdfObject> CoilHeatingDXMultiSpeed_Impl::remove()
  {
    auto _stageDataList = stageDataList();
    auto result = StraightComponent_Impl::remove();
    if( (! result.empty()) && _stageDataList ) {
      _stageDataList->remove();
    }

    return result;
  }

} // detail

CoilHeatingDXMultiSpeed::CoilHeatingDXMultiSpeed(const Model& model)
  : StraightComponent(CoilHeatingDXMultiSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXMultiSpeed_Impl>());

  bool ok = true;
  auto always_on = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule( always_on );
  OS_ASSERT(ok);
  setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-8.0);
  ok = setCrankcaseHeaterCapacity(0.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(10.0);
  OS_ASSERT(ok);
  ok = setMaximumOutdoorDryBulbTemperatureforDefrostOperation(5.0);
  OS_ASSERT(ok);
  ok = setDefrostStrategy("Resistive");
  OS_ASSERT(ok);
  ok = setDefrostControl("OnDemand");
  OS_ASSERT(ok);
  ok = setDefrostTimePeriodFraction(0.058333);
  OS_ASSERT(ok);
  autosizeResistiveDefrostHeaterCapacity();
  setApplyPartLoadFractiontoSpeedsGreaterthan1(false);
  ok = setFuelType("NaturalGas");
  OS_ASSERT(ok);
  ok = setRegionnumberforCalculatingHSPF(4);
  OS_ASSERT(ok);

  auto stageDataList = ModelObjectList(model);
  stageDataList.setName(this->name().get() + " Stage Data List");
  ok = getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setStageDataList(stageDataList);
  OS_ASSERT(ok);

}

IddObjectType CoilHeatingDXMultiSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_MultiSpeed);
}

std::vector<std::string> CoilHeatingDXMultiSpeed::defrostStrategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_MultiSpeedFields::DefrostStrategy);
}

std::vector<std::string> CoilHeatingDXMultiSpeed::defrostControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_MultiSpeedFields::DefrostControl);
}

std::vector<std::string> CoilHeatingDXMultiSpeed::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_Heating_DX_MultiSpeedFields::FuelType);
}

boost::optional<Schedule> CoilHeatingDXMultiSpeed::availabilitySchedule() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->availabilitySchedule();
}

double CoilHeatingDXMultiSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
}

boost::optional<double> CoilHeatingDXMultiSpeed::outdoorDryBulbTemperaturetoTurnOnCompressor() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->outdoorDryBulbTemperaturetoTurnOnCompressor();
}

double CoilHeatingDXMultiSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->crankcaseHeaterCapacity();
}

double CoilHeatingDXMultiSpeed::maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
}

boost::optional<Curve> CoilHeatingDXMultiSpeed::defrostEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostEnergyInputRatioFunctionofTemperatureCurve();
}

double CoilHeatingDXMultiSpeed::maximumOutdoorDryBulbTemperatureforDefrostOperation() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforDefrostOperation();
}

std::string CoilHeatingDXMultiSpeed::defrostStrategy() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostStrategy();
}

std::string CoilHeatingDXMultiSpeed::defrostControl() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostControl();
}

double CoilHeatingDXMultiSpeed::defrostTimePeriodFraction() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->defrostTimePeriodFraction();
}

boost::optional<double> CoilHeatingDXMultiSpeed::resistiveDefrostHeaterCapacity() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resistiveDefrostHeaterCapacity();
}

bool CoilHeatingDXMultiSpeed::isResistiveDefrostHeaterCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
}

bool CoilHeatingDXMultiSpeed::applyPartLoadFractiontoSpeedsGreaterthan1() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->applyPartLoadFractiontoSpeedsGreaterthan1();
}

std::string CoilHeatingDXMultiSpeed::fuelType() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->fuelType();
}

int CoilHeatingDXMultiSpeed::regionnumberforCalculatingHSPF() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->regionnumberforCalculatingHSPF();
}

bool CoilHeatingDXMultiSpeed::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilHeatingDXMultiSpeed::resetAvailabilitySchedule() {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resetAvailabilitySchedule();
}

void CoilHeatingDXMultiSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(minimumOutdoorDryBulbTemperatureforCompressorOperation);
}

void CoilHeatingDXMultiSpeed::setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor) {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setOutdoorDryBulbTemperaturetoTurnOnCompressor(outdoorDryBulbTemperaturetoTurnOnCompressor);
}

void CoilHeatingDXMultiSpeed::resetOutdoorDryBulbTemperaturetoTurnOnCompressor() {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
}

bool CoilHeatingDXMultiSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilHeatingDXMultiSpeed::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
}

bool CoilHeatingDXMultiSpeed::setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostEnergyInputRatioFunctionofTemperatureCurve(curve);
}

void CoilHeatingDXMultiSpeed::resetDefrostEnergyInputRatioFunctionofTemperatureCurve() {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->resetDefrostEnergyInputRatioFunctionofTemperatureCurve();
}

bool CoilHeatingDXMultiSpeed::setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforDefrostOperation(maximumOutdoorDryBulbTemperatureforDefrostOperation);
}

bool CoilHeatingDXMultiSpeed::setDefrostStrategy(std::string defrostStrategy) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostStrategy(defrostStrategy);
}

bool CoilHeatingDXMultiSpeed::setDefrostControl(std::string defrostControl) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostControl(defrostControl);
}

bool CoilHeatingDXMultiSpeed::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setDefrostTimePeriodFraction(defrostTimePeriodFraction);
}

bool CoilHeatingDXMultiSpeed::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
}

void CoilHeatingDXMultiSpeed::autosizeResistiveDefrostHeaterCapacity() {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->autosizeResistiveDefrostHeaterCapacity();
}

void CoilHeatingDXMultiSpeed::setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1) {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setApplyPartLoadFractiontoSpeedsGreaterthan1(applyPartLoadFractiontoSpeedsGreaterthan1);
}

bool CoilHeatingDXMultiSpeed::setFuelType(std::string fuelType) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setFuelType(fuelType);
}

bool CoilHeatingDXMultiSpeed::setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->setRegionnumberforCalculatingHSPF(regionnumberforCalculatingHSPF);
}

std::vector<CoilHeatingDXMultiSpeedStageData> CoilHeatingDXMultiSpeed::stages() const {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->stages();
}

bool CoilHeatingDXMultiSpeed::addStage(const CoilHeatingDXMultiSpeedStageData& stage) {
  return getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->addStage(stage);
}

void CoilHeatingDXMultiSpeed::removeStage(const CoilHeatingDXMultiSpeedStageData& stage) {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->removeStage(stage);
}

void CoilHeatingDXMultiSpeed::removeAllStages() {
  getImpl<detail::CoilHeatingDXMultiSpeed_Impl>()->removeAllStages();
}

/// @cond
CoilHeatingDXMultiSpeed::CoilHeatingDXMultiSpeed(std::shared_ptr<detail::CoilHeatingDXMultiSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

