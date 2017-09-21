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

#include "CoilWaterHeatingDesuperheater.hpp"
#include "CoilWaterHeatingDesuperheater_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "WaterHeaterMixed.hpp"
#include "WaterHeaterMixed_Impl.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/OS_Coil_WaterHeating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilWaterHeatingDesuperheater_Impl::CoilWaterHeatingDesuperheater_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingDesuperheater::iddObjectType());
  }

  CoilWaterHeatingDesuperheater_Impl::CoilWaterHeatingDesuperheater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilWaterHeatingDesuperheater::iddObjectType());
  }

  CoilWaterHeatingDesuperheater_Impl::CoilWaterHeatingDesuperheater_Impl(const CoilWaterHeatingDesuperheater_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilWaterHeatingDesuperheater_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilWaterHeatingDesuperheater_Impl::iddObjectType() const {
    return CoilWaterHeatingDesuperheater::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilWaterHeatingDesuperheater_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_WaterHeating_DesuperheaterFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilWaterHeatingDesuperheater","Availability"));
    }
    if (std::find(b,e,OS_Coil_WaterHeating_DesuperheaterFields::SetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilWaterHeatingDesuperheater","Setpoint Temperature"));
    }
    return result;
  }

  unsigned CoilWaterHeatingDesuperheater_Impl::inletPort()
  {
    return OS_Coil_WaterHeating_DesuperheaterFields::WaterInletNodeName;
  }

  unsigned CoilWaterHeatingDesuperheater_Impl::outletPort()
  {
    return OS_Coil_WaterHeating_DesuperheaterFields::WaterOutletNodeName;
  }

  ModelObject CoilWaterHeatingDesuperheater_Impl::clone(Model model) const {
    CoilWaterHeatingDesuperheater modelObjectClone = ModelObject_Impl::clone(model).cast<CoilWaterHeatingDesuperheater>();

    modelObjectClone.resetHeatingSource();

    return modelObjectClone;
  }

  std::vector<IddObjectType> CoilWaterHeatingDesuperheater_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Biquadratic);
    return result;
  }

  std::vector<ModelObject> CoilWaterHeatingDesuperheater_Impl::children() const
  {
    std::vector<ModelObject> result;
    boost::optional<CurveBiquadratic> intermediate;
    if ( (intermediate = heatReclaimEfficiencyFunctionofTemperatureCurve()) ) {
      result.push_back(*intermediate);
    }
    return result;
  }

  bool CoilWaterHeatingDesuperheater_Impl::addToNode(Node & node) {
    return false;
  }

  std::vector<IdfObject> CoilWaterHeatingDesuperheater_Impl::remove() {
    removeFromHeatRejectionTarget();
    return StraightComponent_Impl::remove();
  }

  boost::optional<ModelObject> CoilWaterHeatingDesuperheater_Impl::heatRejectionTarget() const
  {
    boost::optional<ModelObject> inletHeatRejectionTarget;
    boost::optional<ModelObject> outletHeatRejectionTarget;
    StraightComponent desuperheater = this->getObject<ModelObject>().cast<StraightComponent>();
    if ( boost::optional<ModelObject> outletModelObject = desuperheater.outletModelObject() )
    {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>() )
      {
        if ( boost::optional<ModelObject> heatRejectionTarget = outletNode->outletModelObject() )
        {
          outletHeatRejectionTarget = heatRejectionTarget;
        }
      }
    }
    if ( boost::optional<ModelObject> inletModelObject = desuperheater.inletModelObject() )
    {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>() )
      {
        if ( boost::optional<ModelObject> heatRejectionTarget = inletNode->inletModelObject() )
        {
          inletHeatRejectionTarget = heatRejectionTarget;
        }
      }
    }
    if (inletHeatRejectionTarget && outletHeatRejectionTarget && (inletHeatRejectionTarget.get() == outletHeatRejectionTarget.get()) )
    {
      return inletHeatRejectionTarget;
    }
    return boost::none;
  }

  bool CoilWaterHeatingDesuperheater_Impl::addToHeatRejectionTarget(const ModelObject& heatRejectionTarget)
  {
    bool validChoice = false;
    if( heatRejectionTarget.iddObjectType() == openstudio::IddObjectType::OS_WaterHeater_Mixed )
    {
      validChoice = true;
    }

    if (!validChoice) return false;

    if ( boost::optional<WaterToWaterComponent> _heatRejectionTarget = heatRejectionTarget.optionalCast<WaterToWaterComponent>() )
    {
      Model _model = this->model();
      Node node1(_model);
      Node node2(_model);

      _model.connect(this->getObject<ModelObject>(),this->outletPort(),node1,node1.inletPort());
      _model.connect(node1,node1.outletPort(),*_heatRejectionTarget,_heatRejectionTarget->demandInletPort());

      _model.connect(*_heatRejectionTarget,_heatRejectionTarget->demandOutletPort(),node2,node2.inletPort());
      _model.connect(node2,node2.outletPort(),this->getObject<ModelObject>(),this->inletPort());

      return true;
    }
    return false;
  }

  void CoilWaterHeatingDesuperheater_Impl::removeFromHeatRejectionTarget()
  {
    Model _model = this->model();
    if ( boost::optional<ModelObject> outletModelObject = this->outletModelObject() )
    {
      if ( boost::optional<Node> node = outletModelObject->optionalCast<Node>() )
      {
        _model.disconnect(*node,node->outletPort());
        _model.disconnect(*node,node->inletPort());
        node->remove();
      }
    }
    if ( boost::optional<ModelObject> inletModelObject = this->inletModelObject() )
    {
      if ( boost::optional<Node> node = inletModelObject->optionalCast<Node>() )
      {
        _model.disconnect(*node,node->outletPort());
        _model.disconnect(*node,node->inletPort());
        node->remove();
      }
    }
  }

  Schedule CoilWaterHeatingDesuperheater_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_WaterHeating_DesuperheaterFields::AvailabilityScheduleName);
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  Schedule CoilWaterHeatingDesuperheater_Impl::setpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalSetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double CoilWaterHeatingDesuperheater_Impl::deadBandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::DeadBandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilWaterHeatingDesuperheater_Impl::isDeadBandTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Coil_WaterHeating_DesuperheaterFields::DeadBandTemperatureDifference);
  }

  boost::optional<double> CoilWaterHeatingDesuperheater_Impl::ratedHeatReclaimRecoveryEfficiency() const {
    return getDouble(OS_Coil_WaterHeating_DesuperheaterFields::RatedHeatReclaimRecoveryEfficiency,true);
  }

  double CoilWaterHeatingDesuperheater_Impl::ratedInletWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::RatedInletWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingDesuperheater_Impl::ratedOutdoorAirTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::RatedOutdoorAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingDesuperheater_Impl::maximumInletWaterTemperatureforHeatReclaim() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::MaximumInletWaterTemperatureforHeatReclaim,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> CoilWaterHeatingDesuperheater_Impl::heatReclaimEfficiencyFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_WaterHeating_DesuperheaterFields::HeatReclaimEfficiencyFunctionofTemperatureCurveName);
  }

  // boost::optional<HVACComponent> CoilWaterHeatingDesuperheater_Impl::tank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_Coil_WaterHeating_DesuperheaterFields::TankName);
    // boost::optional<HVACComponent> value = optionalTank();
    // if (!value) {
    //   LOG_AND_THROW(briefDescription() << " does not have an Tank attached.");
    // }
    // return value.get();
  // }

  boost::optional<ModelObject> CoilWaterHeatingDesuperheater_Impl::heatingSource() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_Coil_WaterHeating_DesuperheaterFields::HeatingSourceName);
    // boost::optional<ModelObject> value = optionalHeatingSource();
    // if (!value) {
    //   LOG_AND_THROW(briefDescription() << " does not have an Heating Source attached.");
    // }
    // return value.get();
  }

  double CoilWaterHeatingDesuperheater_Impl::waterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::WaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingDesuperheater_Impl::waterPumpPower() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::WaterPumpPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilWaterHeatingDesuperheater_Impl::isWaterPumpPowerDefaulted() const {
    return isEmpty(OS_Coil_WaterHeating_DesuperheaterFields::WaterPumpPower);
  }

  double CoilWaterHeatingDesuperheater_Impl::fractionofPumpHeattoWater() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::FractionofPumpHeattoWater,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilWaterHeatingDesuperheater_Impl::isFractionofPumpHeattoWaterDefaulted() const {
    return isEmpty(OS_Coil_WaterHeating_DesuperheaterFields::FractionofPumpHeattoWater);
  }

  double CoilWaterHeatingDesuperheater_Impl::onCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::OnCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilWaterHeatingDesuperheater_Impl::isOnCycleParasiticElectricLoadDefaulted() const {
    return isEmpty(OS_Coil_WaterHeating_DesuperheaterFields::OnCycleParasiticElectricLoad);
  }

  double CoilWaterHeatingDesuperheater_Impl::offCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_DesuperheaterFields::OffCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilWaterHeatingDesuperheater_Impl::isOffCycleParasiticElectricLoadDefaulted() const {
    return isEmpty(OS_Coil_WaterHeating_DesuperheaterFields::OffCycleParasiticElectricLoad);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_WaterHeating_DesuperheaterFields::AvailabilityScheduleName,
                              "CoilWaterHeatingDesuperheater",
                              "Availability",
                              schedule);
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_WaterHeating_DesuperheaterFields::SetpointTemperatureScheduleName,
                              "CoilWaterHeatingDesuperheater",
                              "Setpoint Temperature",
                              schedule);
    return result;
  }

  bool CoilWaterHeatingDesuperheater_Impl::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::DeadBandTemperatureDifference, deadBandTemperatureDifference);
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetDeadBandTemperatureDifference() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::DeadBandTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setRatedHeatReclaimRecoveryEfficiency(boost::optional<double> ratedHeatReclaimRecoveryEfficiency) {
    bool result(false);
    if (ratedHeatReclaimRecoveryEfficiency) {
      result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::RatedHeatReclaimRecoveryEfficiency, ratedHeatReclaimRecoveryEfficiency.get());
    }
    else {
      resetRatedHeatReclaimRecoveryEfficiency();
      result = true;
    }
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetRatedHeatReclaimRecoveryEfficiency() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::RatedHeatReclaimRecoveryEfficiency, "");
    OS_ASSERT(result);
  }

  void CoilWaterHeatingDesuperheater_Impl::setRatedInletWaterTemperature(double ratedInletWaterTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::RatedInletWaterTemperature, ratedInletWaterTemperature);
    OS_ASSERT(result);
  }

  void CoilWaterHeatingDesuperheater_Impl::setRatedOutdoorAirTemperature(double ratedOutdoorAirTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::RatedOutdoorAirTemperature, ratedOutdoorAirTemperature);
    OS_ASSERT(result);
  }

  void CoilWaterHeatingDesuperheater_Impl::setMaximumInletWaterTemperatureforHeatReclaim(double maximumInletWaterTemperatureforHeatReclaim) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::MaximumInletWaterTemperatureforHeatReclaim, maximumInletWaterTemperatureforHeatReclaim);
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setHeatReclaimEfficiencyFunctionofTemperatureCurve(const boost::optional<CurveBiquadratic>& curveBiquadratic) {
    bool result(false);
    if (curveBiquadratic) {
      result = setPointer(OS_Coil_WaterHeating_DesuperheaterFields::HeatReclaimEfficiencyFunctionofTemperatureCurveName, curveBiquadratic.get().handle());
    }
    else {
      resetHeatReclaimEfficiencyFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetHeatReclaimEfficiencyFunctionofTemperatureCurve() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::HeatReclaimEfficiencyFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  // bool CoilWaterHeatingDesuperheater_Impl::setTank(const HVACComponent& waterHeater) {
  //   bool result = setPointer(OS_Coil_WaterHeating_DesuperheaterFields::TankName, waterHeater.handle());
  //   if (result) addToWaterHeater(waterHeater);
  //   return result;
  // }

  bool CoilWaterHeatingDesuperheater_Impl::setHeatingSource(const boost::optional<ModelObject>& heatingSource) {
      bool result(false);
      if (heatingSource) {
          result = setPointer(OS_Coil_WaterHeating_DesuperheaterFields::HeatingSourceName, heatingSource.get().handle());
      }
      else {
          resetHeatingSource();
          result = true;
      }
      return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetHeatingSource() {
      bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::HeatingSourceName, "");
      OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setWaterFlowRate(double waterFlowRate) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::WaterFlowRate, waterFlowRate);
    return result;
  }

  bool CoilWaterHeatingDesuperheater_Impl::setWaterPumpPower(double waterPumpPower) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::WaterPumpPower, waterPumpPower);
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetWaterPumpPower() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::WaterPumpPower, "");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setFractionofPumpHeattoWater(double fractionofPumpHeattoWater) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::FractionofPumpHeattoWater, fractionofPumpHeattoWater);
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetFractionofPumpHeattoWater() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::FractionofPumpHeattoWater, "");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::OnCycleParasiticElectricLoad, onCycleParasiticElectricLoad);
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetOnCycleParasiticElectricLoad() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::OnCycleParasiticElectricLoad, "");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingDesuperheater_Impl::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
    bool result = setDouble(OS_Coil_WaterHeating_DesuperheaterFields::OffCycleParasiticElectricLoad, offCycleParasiticElectricLoad);
    return result;
  }

  void CoilWaterHeatingDesuperheater_Impl::resetOffCycleParasiticElectricLoad() {
    bool result = setString(OS_Coil_WaterHeating_DesuperheaterFields::OffCycleParasiticElectricLoad, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> CoilWaterHeatingDesuperheater_Impl::optionalSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_WaterHeating_DesuperheaterFields::SetpointTemperatureScheduleName);
  }

  // boost::optional<HVACComponent> CoilWaterHeatingDesuperheater_Impl::optionalTank() const {
  //   return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_Coil_WaterHeating_DesuperheaterFields::TankName);
  // }

  // boost::optional<ModelObject> CoilWaterHeatingDesuperheater_Impl::optionalHeatingSource() const {
  //   return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_Coil_WaterHeating_DesuperheaterFields::HeatingSourceName);
  // }

} // detail

CoilWaterHeatingDesuperheater::CoilWaterHeatingDesuperheater(const Model& model, Schedule& setpointTemperatureSchedule)
  : StraightComponent(CoilWaterHeatingDesuperheater::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilWaterHeatingDesuperheater_Impl>());

  Schedule availabilitySchedule = model.alwaysOnDiscreteSchedule();
  bool ok = setAvailabilitySchedule(availabilitySchedule);
  OS_ASSERT(ok);
  ok = setSetpointTemperatureSchedule(setpointTemperatureSchedule);
  OS_ASSERT(ok);
  ok = setDeadBandTemperatureDifference(5.0);
  OS_ASSERT(ok);
  ok = setRatedHeatReclaimRecoveryEfficiency(0.8);
  OS_ASSERT(ok);
  setRatedInletWaterTemperature(50.0);
  setRatedOutdoorAirTemperature(35.0);
  setMaximumInletWaterTemperatureforHeatReclaim(60.0);
  ok = setWaterFlowRate(0.0001);
  OS_ASSERT(ok);
  ok = setWaterPumpPower(100.0);
  OS_ASSERT(ok);
  ok = setFractionofPumpHeattoWater(0.2);
  OS_ASSERT(ok);
  ok = setOnCycleParasiticElectricLoad(0.0);
  OS_ASSERT(ok);
  ok = setOffCycleParasiticElectricLoad(0.0);
  OS_ASSERT(ok);
}

IddObjectType CoilWaterHeatingDesuperheater::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_WaterHeating_Desuperheater);
}

boost::optional<ModelObject> CoilWaterHeatingDesuperheater::heatRejectionTarget() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->heatRejectionTarget();
}

bool CoilWaterHeatingDesuperheater::addToHeatRejectionTarget(const ModelObject& heatRejectionTarget) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->addToHeatRejectionTarget(heatRejectionTarget);
}

void CoilWaterHeatingDesuperheater::removeFromHeatRejectionTarget() {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->removeFromHeatRejectionTarget();
}

Schedule CoilWaterHeatingDesuperheater::availabilitySchedule() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->availabilitySchedule();
}

Schedule CoilWaterHeatingDesuperheater::setpointTemperatureSchedule() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setpointTemperatureSchedule();
}

double CoilWaterHeatingDesuperheater::deadBandTemperatureDifference() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->deadBandTemperatureDifference();
}

bool CoilWaterHeatingDesuperheater::isDeadBandTemperatureDifferenceDefaulted() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->isDeadBandTemperatureDifferenceDefaulted();
}

boost::optional<double> CoilWaterHeatingDesuperheater::ratedHeatReclaimRecoveryEfficiency() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->ratedHeatReclaimRecoveryEfficiency();
}

double CoilWaterHeatingDesuperheater::ratedInletWaterTemperature() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->ratedInletWaterTemperature();
}

double CoilWaterHeatingDesuperheater::ratedOutdoorAirTemperature() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->ratedOutdoorAirTemperature();
}

double CoilWaterHeatingDesuperheater::maximumInletWaterTemperatureforHeatReclaim() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->maximumInletWaterTemperatureforHeatReclaim();
}

boost::optional<CurveBiquadratic> CoilWaterHeatingDesuperheater::heatReclaimEfficiencyFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->heatReclaimEfficiencyFunctionofTemperatureCurve();
}

// boost::optional<HVACComponent> CoilWaterHeatingDesuperheater::tank() const {
//   return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->tank();
// }

boost::optional<ModelObject> CoilWaterHeatingDesuperheater::heatingSource() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->heatingSource();
}

double CoilWaterHeatingDesuperheater::waterFlowRate() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->waterFlowRate();
}

double CoilWaterHeatingDesuperheater::waterPumpPower() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->waterPumpPower();
}

bool CoilWaterHeatingDesuperheater::isWaterPumpPowerDefaulted() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->isWaterPumpPowerDefaulted();
}

double CoilWaterHeatingDesuperheater::fractionofPumpHeattoWater() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->fractionofPumpHeattoWater();
}

bool CoilWaterHeatingDesuperheater::isFractionofPumpHeattoWaterDefaulted() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->isFractionofPumpHeattoWaterDefaulted();
}

double CoilWaterHeatingDesuperheater::onCycleParasiticElectricLoad() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->onCycleParasiticElectricLoad();
}

bool CoilWaterHeatingDesuperheater::isOnCycleParasiticElectricLoadDefaulted() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->isOnCycleParasiticElectricLoadDefaulted();
}

double CoilWaterHeatingDesuperheater::offCycleParasiticElectricLoad() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->offCycleParasiticElectricLoad();
}

bool CoilWaterHeatingDesuperheater::isOffCycleParasiticElectricLoadDefaulted() const {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->isOffCycleParasiticElectricLoadDefaulted();
}

bool CoilWaterHeatingDesuperheater::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setAvailabilitySchedule(schedule);
}

void CoilWaterHeatingDesuperheater::resetAvailabilitySchedule() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetAvailabilitySchedule();
}

bool CoilWaterHeatingDesuperheater::setSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setSetpointTemperatureSchedule(schedule);
}

bool CoilWaterHeatingDesuperheater::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setDeadBandTemperatureDifference(deadBandTemperatureDifference);
}

void CoilWaterHeatingDesuperheater::resetDeadBandTemperatureDifference() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetDeadBandTemperatureDifference();
}

bool CoilWaterHeatingDesuperheater::setRatedHeatReclaimRecoveryEfficiency(double ratedHeatReclaimRecoveryEfficiency) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setRatedHeatReclaimRecoveryEfficiency(ratedHeatReclaimRecoveryEfficiency);
}

void CoilWaterHeatingDesuperheater::resetRatedHeatReclaimRecoveryEfficiency() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetRatedHeatReclaimRecoveryEfficiency();
}

void CoilWaterHeatingDesuperheater::setRatedInletWaterTemperature(double ratedInletWaterTemperature) {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setRatedInletWaterTemperature(ratedInletWaterTemperature);
}

void CoilWaterHeatingDesuperheater::setRatedOutdoorAirTemperature(double ratedOutdoorAirTemperature) {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setRatedOutdoorAirTemperature(ratedOutdoorAirTemperature);
}

void CoilWaterHeatingDesuperheater::setMaximumInletWaterTemperatureforHeatReclaim(double maximumInletWaterTemperatureforHeatReclaim) {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setMaximumInletWaterTemperatureforHeatReclaim(maximumInletWaterTemperatureforHeatReclaim);
}

bool CoilWaterHeatingDesuperheater::setHeatReclaimEfficiencyFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setHeatReclaimEfficiencyFunctionofTemperatureCurve(curveBiquadratic);
}

void CoilWaterHeatingDesuperheater::resetHeatReclaimEfficiencyFunctionofTemperatureCurve() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetHeatReclaimEfficiencyFunctionofTemperatureCurve();
}

// bool CoilWaterHeatingDesuperheater::setTank(const HVACComponent& waterHeater) {
//   return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setTank(waterHeater);
// }

bool CoilWaterHeatingDesuperheater::setHeatingSource(const ModelObject& heatingSource) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setHeatingSource(heatingSource);
}

void CoilWaterHeatingDesuperheater::resetHeatingSource() {
    getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetHeatingSource();
}

bool CoilWaterHeatingDesuperheater::setWaterFlowRate(double waterFlowRate) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setWaterFlowRate(waterFlowRate);
}

bool CoilWaterHeatingDesuperheater::setWaterPumpPower(double waterPumpPower) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setWaterPumpPower(waterPumpPower);
}

void CoilWaterHeatingDesuperheater::resetWaterPumpPower() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetWaterPumpPower();
}

bool CoilWaterHeatingDesuperheater::setFractionofPumpHeattoWater(double fractionofPumpHeattoWater) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setFractionofPumpHeattoWater(fractionofPumpHeattoWater);
}

void CoilWaterHeatingDesuperheater::resetFractionofPumpHeattoWater() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetFractionofPumpHeattoWater();
}

bool CoilWaterHeatingDesuperheater::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setOnCycleParasiticElectricLoad(onCycleParasiticElectricLoad);
}

void CoilWaterHeatingDesuperheater::resetOnCycleParasiticElectricLoad() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetOnCycleParasiticElectricLoad();
}

bool CoilWaterHeatingDesuperheater::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
  return getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->setOffCycleParasiticElectricLoad(offCycleParasiticElectricLoad);
}

void CoilWaterHeatingDesuperheater::resetOffCycleParasiticElectricLoad() {
  getImpl<detail::CoilWaterHeatingDesuperheater_Impl>()->resetOffCycleParasiticElectricLoad();
}

/// @cond
CoilWaterHeatingDesuperheater::CoilWaterHeatingDesuperheater(std::shared_ptr<detail::CoilWaterHeatingDesuperheater_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

