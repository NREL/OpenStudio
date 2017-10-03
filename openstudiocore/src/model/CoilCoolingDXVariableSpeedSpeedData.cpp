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

#include "CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"

#include <utilities/idd/OS_Coil_Cooling_DX_VariableSpeed_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingDXVariableSpeedSpeedData_Impl::CoilCoolingDXVariableSpeedSpeedData_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXVariableSpeedSpeedData::iddObjectType());
  }

  CoilCoolingDXVariableSpeedSpeedData_Impl::CoilCoolingDXVariableSpeedSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingDXVariableSpeedSpeedData::iddObjectType());
  }

  CoilCoolingDXVariableSpeedSpeedData_Impl::CoilCoolingDXVariableSpeedSpeedData_Impl(const CoilCoolingDXVariableSpeedSpeedData_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingDXVariableSpeedSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingDXVariableSpeedSpeedData_Impl::iddObjectType() const {
    return CoilCoolingDXVariableSpeedSpeedData::iddObjectType();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedTotalCoolingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitGrossRatedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitRatedCondenserAirFlowRate() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedCondenserAirFlowRate,true);
  }

  boost::optional<double> CoilCoolingDXVariableSpeedSpeedData_Impl::referenceUnitRatedPadEffectivenessofEvapPrecooling() const {
    return getDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedPadEffectivenessofEvapPrecooling,true);
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingDXVariableSpeedSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity, referenceUnitGrossRatedTotalCoolingCapacity);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio, referenceUnitGrossRatedSensibleHeatRatio);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP, referenceUnitGrossRatedCoolingCOP);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedAirFlowRate, referenceUnitRatedAirFlowRate);
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedCondenserAirFlowRate(boost::optional<double> referenceUnitRatedCondenserAirFlowRate) {
    bool result(false);
    if (referenceUnitRatedCondenserAirFlowRate) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedCondenserAirFlowRate, referenceUnitRatedCondenserAirFlowRate.get());
    }
    else {
      resetReferenceUnitRatedCondenserAirFlowRate();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXVariableSpeedSpeedData_Impl::resetReferenceUnitRatedCondenserAirFlowRate() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedCondenserAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setReferenceUnitRatedPadEffectivenessofEvapPrecooling(boost::optional<double> referenceUnitRatedPadEffectivenessofEvapPrecooling) {
    bool result(false);
    if (referenceUnitRatedPadEffectivenessofEvapPrecooling) {
      result = setDouble(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedPadEffectivenessofEvapPrecooling, referenceUnitRatedPadEffectivenessofEvapPrecooling.get());
    }
    else {
      resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();
      result = true;
    }
    return result;
  }

  void CoilCoolingDXVariableSpeedSpeedData_Impl::resetReferenceUnitRatedPadEffectivenessofEvapPrecooling() {
    bool result = setString(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::ReferenceUnitRatedPadEffectivenessofEvapPrecooling, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingDXVariableSpeedSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilCoolingDXVariableSpeedSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_VariableSpeed_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  ModelObject CoilCoolingDXVariableSpeedSpeedData_Impl::clone(Model model) const {
    auto t_clone = ParentObject_Impl::clone(model).cast<CoilCoolingDXVariableSpeedSpeedData>();

    return t_clone;
  }

  std::vector<ModelObject> CoilCoolingDXVariableSpeedSpeedData_Impl::children() const {
    std::vector<ModelObject> children;
    children.push_back( totalCoolingCapacityFunctionofTemperatureCurve() );
    children.push_back( totalCoolingCapacityFunctionofAirFlowFractionCurve() );
    children.push_back( energyInputRatioFunctionofTemperatureCurve() );
    children.push_back( energyInputRatioFunctionofAirFlowFractionCurve() );
    return children;
  }

} // detail

CoilCoolingDXVariableSpeedSpeedData::CoilCoolingDXVariableSpeedSpeedData(const Model& model)
  : ParentObject(CoilCoolingDXVariableSpeedSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>());

  CurveBiquadratic cooling_curve_1(model);
  cooling_curve_1.setCoefficient1Constant(1.6253449506);
  cooling_curve_1.setCoefficient2x(-0.0786550838);
  cooling_curve_1.setCoefficient3xPOW2(0.0030679776);
  cooling_curve_1.setCoefficient4y(0.0008002088);
  cooling_curve_1.setCoefficient5yPOW2(-0.0000354060);
  cooling_curve_1.setCoefficient6xTIMESY(-0.0003534409);
  cooling_curve_1.setMinimumValueofx(13.89);
  cooling_curve_1.setMaximumValueofx(22.22);
  cooling_curve_1.setMinimumValueofy(12.78);
  cooling_curve_1.setMaximumValueofy(51.67);

  CurveQuadratic cooling_curve_2(model);
  cooling_curve_2.setCoefficient1Constant(1.0);
  cooling_curve_2.setCoefficient2x(0.0);
  cooling_curve_2.setCoefficient3xPOW2(0.0);
  cooling_curve_2.setMinimumValueofx(0.5);
  cooling_curve_2.setMaximumValueofx(1.5);

  CurveBiquadratic cooling_curve_3(model);
  cooling_curve_3.setCoefficient1Constant(1.4240389306);
  cooling_curve_3.setCoefficient2x(-0.0593310687);
  cooling_curve_3.setCoefficient3xPOW2(0.0026068070);
  cooling_curve_3.setCoefficient4y(0.0008867551);
  cooling_curve_3.setCoefficient5yPOW2(-0.0000369191);
  cooling_curve_3.setCoefficient6xTIMESY(-0.0003552805);
  cooling_curve_3.setMinimumValueofx(13.89);
  cooling_curve_3.setMaximumValueofx(22.22);
  cooling_curve_3.setMinimumValueofy(12.78);
  cooling_curve_3.setMaximumValueofy(51.67);

  CurveQuadratic cooling_curve_4(model);
  cooling_curve_4.setCoefficient1Constant(1.0);
  cooling_curve_4.setCoefficient2x(0.0);
  cooling_curve_4.setCoefficient3xPOW2(0.0);
  cooling_curve_4.setMinimumValueofx(0.5);
  cooling_curve_4.setMaximumValueofx(1.5);

  bool ok = true;
  ok = setReferenceUnitGrossRatedTotalCoolingCapacity(8914.185229);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedSensibleHeatRatio(0.79);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedCoolingCOP(3.980488789);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlowRate(0.944);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofTemperatureCurve(cooling_curve_1);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofAirFlowFractionCurve(cooling_curve_2);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(cooling_curve_3);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(cooling_curve_4);
  OS_ASSERT(ok);

}

CoilCoolingDXVariableSpeedSpeedData::CoilCoolingDXVariableSpeedSpeedData(const Model& model,
                                                                         const Curve& totalCoolingCapacityFunctionofTemperatureCurve,
                                                                         const Curve& totalCoolingCapacityFunctionofAirFlowFractionCurve,
                                                                         const Curve& energyInputRatioFunctionofTemperatureCurve,
                                                                         const Curve& energyInputRatioFunctionofAirFlowFractionCurve)
  : ParentObject(CoilCoolingDXVariableSpeedSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>());

  bool ok = true;
  ok = setReferenceUnitGrossRatedTotalCoolingCapacity(8914.185229);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedSensibleHeatRatio(0.79);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedCoolingCOP(3.980488789);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlowRate(0.944);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofTemperatureCurve(totalCoolingCapacityFunctionofTemperatureCurve);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofAirFlowFractionCurve(totalCoolingCapacityFunctionofAirFlowFractionCurve);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperatureCurve);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(energyInputRatioFunctionofAirFlowFractionCurve);
  OS_ASSERT(ok);

}

IddObjectType CoilCoolingDXVariableSpeedSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_DX_VariableSpeed_SpeedData);
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitGrossRatedTotalCoolingCapacity() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedTotalCoolingCapacity();
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitGrossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedSensibleHeatRatio();
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitGrossRatedCoolingCOP() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitGrossRatedCoolingCOP();
}

double CoilCoolingDXVariableSpeedSpeedData::referenceUnitRatedAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
}

boost::optional<double> CoilCoolingDXVariableSpeedSpeedData::referenceUnitRatedCondenserAirFlowRate() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedCondenserAirFlowRate();
}

boost::optional<double> CoilCoolingDXVariableSpeedSpeedData::referenceUnitRatedPadEffectivenessofEvapPrecooling() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->referenceUnitRatedPadEffectivenessofEvapPrecooling();
}

Curve CoilCoolingDXVariableSpeedSpeedData::totalCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
}

Curve CoilCoolingDXVariableSpeedSpeedData::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->totalCoolingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilCoolingDXVariableSpeedSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilCoolingDXVariableSpeedSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedTotalCoolingCapacity(referenceUnitGrossRatedTotalCoolingCapacity);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedSensibleHeatRatio(referenceUnitGrossRatedSensibleHeatRatio);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitGrossRatedCoolingCOP(referenceUnitGrossRatedCoolingCOP);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(referenceUnitRatedAirFlowRate);
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitRatedCondenserAirFlowRate(double referenceUnitRatedCondenserAirFlowRate) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedCondenserAirFlowRate(referenceUnitRatedCondenserAirFlowRate);
}

void CoilCoolingDXVariableSpeedSpeedData::resetReferenceUnitRatedCondenserAirFlowRate() {
  getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->resetReferenceUnitRatedCondenserAirFlowRate();
}

bool CoilCoolingDXVariableSpeedSpeedData::setReferenceUnitRatedPadEffectivenessofEvapPrecooling(double referenceUnitRatedPadEffectivenessofEvapPrecooling) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setReferenceUnitRatedPadEffectivenessofEvapPrecooling(referenceUnitRatedPadEffectivenessofEvapPrecooling);
}

void CoilCoolingDXVariableSpeedSpeedData::resetReferenceUnitRatedPadEffectivenessofEvapPrecooling() {
  getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();
}

bool CoilCoolingDXVariableSpeedSpeedData::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXVariableSpeedSpeedData::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setTotalCoolingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilCoolingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilCoolingDXVariableSpeedSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingDXVariableSpeedSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curve);
}

/// @cond
CoilCoolingDXVariableSpeedSpeedData::CoilCoolingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilCoolingDXVariableSpeedSpeedData_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

