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
#include "CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  CoilHeatingDXVariableRefrigerantFlow_Impl::CoilHeatingDXVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : HVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXVariableRefrigerantFlow::iddObjectType());
  }

  CoilHeatingDXVariableRefrigerantFlow_Impl::CoilHeatingDXVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXVariableRefrigerantFlow::iddObjectType());
  }

  CoilHeatingDXVariableRefrigerantFlow_Impl::CoilHeatingDXVariableRefrigerantFlow_Impl(const CoilHeatingDXVariableRefrigerantFlow_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXVariableRefrigerantFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXVariableRefrigerantFlow_Impl::iddObjectType() const {
    return CoilHeatingDXVariableRefrigerantFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CoilHeatingDXVariableRefrigerantFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_DX_VariableRefrigerantFlowFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingDXVariableRefrigerantFlow","Availability Schedule"));
    }
    return result;
  }

  Schedule CoilHeatingDXVariableRefrigerantFlow_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  boost::optional<double> CoilHeatingDXVariableRefrigerantFlow_Impl::ratedTotalHeatingCapacity() const {
    return getDouble(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity,true);
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::isRatedTotalHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingDXVariableRefrigerantFlow_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedAirFlowRate,true);
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  Curve CoilHeatingDXVariableRefrigerantFlow_Impl::heatingCapacityRatioModifierFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityRatioModifierFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Ratio Modifier Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXVariableRefrigerantFlow_Impl::heatingCapacityModifierFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityModifierFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Modifier Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::AvailabilitySchedule,
                              "CoilHeatingDXVariableRefrigerantFlow",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity) {
    bool result(false);
    if (ratedTotalHeatingCapacity) {
      result = setDouble(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, ratedTotalHeatingCapacity.get());
    }
    return result;
  }

  void CoilHeatingDXVariableRefrigerantFlow_Impl::autosizeRatedTotalHeatingCapacity() {
    bool result = setString(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    return result;
  }

  void CoilHeatingDXVariableRefrigerantFlow_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::RatedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::setHeatingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilHeatingDXVariableRefrigerantFlow_Impl::setHeatingCapacityModifierFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::HeatingCapacityModifierFunctionofFlowFractionCurve, curve.handle());
    return result;
  }

  boost::optional<Schedule> CoilHeatingDXVariableRefrigerantFlow_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::AvailabilitySchedule);
  }

  boost::optional<Curve> CoilHeatingDXVariableRefrigerantFlow_Impl::optionalHeatingCapacityRatioModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilHeatingDXVariableRefrigerantFlow_Impl::optionalHeatingCapacityModifierFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_VariableRefrigerantFlowFields::HeatingCapacityModifierFunctionofFlowFractionCurve);
  }

  ModelObject CoilHeatingDXVariableRefrigerantFlow_Impl::clone(Model model) const
  {
    CoilHeatingDXVariableRefrigerantFlow objectClone = HVACComponent_Impl::clone(model).cast<CoilHeatingDXVariableRefrigerantFlow>();

    return objectClone;
  }

  std::vector<ModelObject> CoilHeatingDXVariableRefrigerantFlow_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(heatingCapacityRatioModifierFunctionofTemperatureCurve());
    result.push_back(heatingCapacityModifierFunctionofFlowFractionCurve());

    return result;
  }

} // detail

CoilHeatingDXVariableRefrigerantFlow::CoilHeatingDXVariableRefrigerantFlow(const Model& model)
  : HVACComponent(CoilHeatingDXVariableRefrigerantFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>());

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(schedule);

  autosizeRatedTotalHeatingCapacity();

  autosizeRatedAirFlowRate();

  CurveBiquadratic vrfTUHeatCAPFT(model);
  vrfTUHeatCAPFT.setName( name().get() + " VRFTUHeatCAPFT");
  vrfTUHeatCAPFT.setCoefficient1Constant(0.375443994956127);
  vrfTUHeatCAPFT.setCoefficient2x(6.68190645147821E-02);
  vrfTUHeatCAPFT.setCoefficient3xPOW2(-1.94171026482001E-03);
  vrfTUHeatCAPFT.setCoefficient4y(4.42618420640187E-02);
  vrfTUHeatCAPFT.setCoefficient5yPOW2(-0.0004009578);
  vrfTUHeatCAPFT.setCoefficient6xTIMESY(-0.0014819801);
  vrfTUHeatCAPFT.setMinimumValueofx(21.11);
  vrfTUHeatCAPFT.setMaximumValueofx(27.22);
  vrfTUHeatCAPFT.setMinimumValueofy(-15);
  vrfTUHeatCAPFT.setMaximumValueofy(18.33);
  vrfTUHeatCAPFT.setMinimumCurveOutput(0.6074);
  vrfTUHeatCAPFT.setMaximumCurveOutput(1.0);
  setHeatingCapacityRatioModifierFunctionofTemperatureCurve(vrfTUHeatCAPFT);

  CurveQuadratic vrfACCoolCapFFF(model);
  vrfACCoolCapFFF.setName( name().get() + " VRFACCoolCapFFF");
  vrfACCoolCapFFF.setCoefficient1Constant(0.8);
  vrfACCoolCapFFF.setCoefficient2x(0.2);
  vrfACCoolCapFFF.setCoefficient3xPOW2(0.0);
  vrfACCoolCapFFF.setMinimumValueofx(0.5);
  vrfACCoolCapFFF.setMaximumValueofx(1.5);
  setHeatingCapacityModifierFunctionofFlowFractionCurve(vrfACCoolCapFFF);
}

IddObjectType CoilHeatingDXVariableRefrigerantFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_VariableRefrigerantFlow);
}

Schedule CoilHeatingDXVariableRefrigerantFlow::availabilitySchedule() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->availabilitySchedule();
}

boost::optional<double> CoilHeatingDXVariableRefrigerantFlow::ratedTotalHeatingCapacity() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->ratedTotalHeatingCapacity();
}

bool CoilHeatingDXVariableRefrigerantFlow::isRatedTotalHeatingCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->isRatedTotalHeatingCapacityAutosized();
}

boost::optional<double> CoilHeatingDXVariableRefrigerantFlow::ratedAirFlowRate() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->ratedAirFlowRate();
}

bool CoilHeatingDXVariableRefrigerantFlow::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->isRatedAirFlowRateAutosized();
}

Curve CoilHeatingDXVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->heatingCapacityRatioModifierFunctionofTemperatureCurve();
}

Curve CoilHeatingDXVariableRefrigerantFlow::heatingCapacityModifierFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->heatingCapacityModifierFunctionofFlowFractionCurve();
}

bool CoilHeatingDXVariableRefrigerantFlow::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->setAvailabilitySchedule(schedule);
}

bool CoilHeatingDXVariableRefrigerantFlow::setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity) {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacity(ratedTotalHeatingCapacity);
}

void CoilHeatingDXVariableRefrigerantFlow::autosizeRatedTotalHeatingCapacity() {
  getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->autosizeRatedTotalHeatingCapacity();
}

bool CoilHeatingDXVariableRefrigerantFlow::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilHeatingDXVariableRefrigerantFlow::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->autosizeRatedAirFlowRate();
}

bool CoilHeatingDXVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioModifierFunctionofTemperatureCurve(curve);
}

bool CoilHeatingDXVariableRefrigerantFlow::setHeatingCapacityModifierFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXVariableRefrigerantFlow_Impl>()->setHeatingCapacityModifierFunctionofFlowFractionCurve(curve);
}

/// @cond
CoilHeatingDXVariableRefrigerantFlow::CoilHeatingDXVariableRefrigerantFlow(std::shared_ptr<detail::CoilHeatingDXVariableRefrigerantFlow_Impl> impl)
  : HVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

