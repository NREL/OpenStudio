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

#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"

#include <utilities/idd/OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::iddObjectType() const {
    return CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedTotalCoolingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalCoolingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Cooling Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalWasteHeatFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedTotalCoolingCapacity, referenceUnitGrossRatedTotalCoolingCapacity);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedSensibleHeatRatio, referenceUnitGrossRatedSensibleHeatRatio);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedCoolingCOP, referenceUnitGrossRatedCoolingCOP);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlowRate, referenceUnitRatedAirFlowRate);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, referenceUnitRatedWaterFlowRate);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions, referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
    return result;
  }

  bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalCoolingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalCoolingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalCoolingCapacityFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<Curve> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName);
  }

  ModelObject CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::clone(Model model) const {
    auto t_clone = ParentObject_Impl::clone(model).cast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>();

    return t_clone;
  }

  std::vector<ModelObject> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::children() const {
    std::vector<ModelObject> children;
    children.push_back( totalCoolingCapacityFunctionofTemperatureCurve() );
    children.push_back( totalCoolingCapacityFunctionofAirFlowFractionCurve() );
    children.push_back( totalCoolingCapacityFunctionofWaterFlowFractionCurve() );
    children.push_back( energyInputRatioFunctionofTemperatureCurve() );
    children.push_back( energyInputRatioFunctionofAirFlowFractionCurve() );
    children.push_back( energyInputRatioFunctionofWaterFlowFractionCurve() );
    children.push_back( wasteHeatFunctionofTemperatureCurve() );
    return children;
  }

} // detail

CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model)
  : ParentObject(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>());

  CurveBiquadratic cooling_curve_1(model);
  cooling_curve_1.setCoefficient1Constant(1.43085);
  cooling_curve_1.setCoefficient2x(-0.0453653);
  cooling_curve_1.setCoefficient3xPOW2(0.00199378);
  cooling_curve_1.setCoefficient4y(-0.00805944);
  cooling_curve_1.setCoefficient5yPOW2(3.93414E-05);
  cooling_curve_1.setCoefficient6xTIMESY(-1.81187E-04);
  cooling_curve_1.setMinimumValueofx(10.0);
  cooling_curve_1.setMaximumValueofx(25.6);
  cooling_curve_1.setMinimumValueofy(7.2);
  cooling_curve_1.setMaximumValueofy(48.9);

  CurveQuadratic cooling_curve_2(model);
  cooling_curve_2.setCoefficient1Constant(1.0);
  cooling_curve_2.setCoefficient2x(0.0);
  cooling_curve_2.setCoefficient3xPOW2(0.0);
  cooling_curve_2.setMinimumValueofx(0.0);
  cooling_curve_2.setMaximumValueofx(1.0);

  CurveQuadratic cooling_curve_3(model);
  cooling_curve_3.setCoefficient1Constant(1.0);
  cooling_curve_3.setCoefficient2x(0.0);
  cooling_curve_3.setCoefficient3xPOW2(0.0);
  cooling_curve_3.setMinimumValueofx(0.0);
  cooling_curve_3.setMaximumValueofx(1.0);

  CurveBiquadratic cooling_curve_4(model);
  cooling_curve_4.setCoefficient1Constant(0.138848);
  cooling_curve_4.setCoefficient2x(0.0457985);
  cooling_curve_4.setCoefficient3xPOW2(-0.00138661);
  cooling_curve_4.setCoefficient4y(0.0141485);
  cooling_curve_4.setCoefficient5yPOW2(0.000386055);
  cooling_curve_4.setCoefficient6xTIMESY(-4.38194E-04);
  cooling_curve_4.setMinimumValueofx(10.0);
  cooling_curve_4.setMaximumValueofx(25.6);
  cooling_curve_4.setMinimumValueofy(7.2);
  cooling_curve_4.setMaximumValueofy(48.9);

  CurveQuadratic cooling_curve_5(model);
  cooling_curve_5.setCoefficient1Constant(1.0);
  cooling_curve_5.setCoefficient2x(0.0);
  cooling_curve_5.setCoefficient3xPOW2(0.0);
  cooling_curve_5.setMinimumValueofx(0.0);
  cooling_curve_5.setMaximumValueofx(1.0);

  CurveQuadratic cooling_curve_6(model);
  cooling_curve_6.setCoefficient1Constant(1.0);
  cooling_curve_6.setCoefficient2x(0.0);
  cooling_curve_6.setCoefficient3xPOW2(0.0);
  cooling_curve_6.setMinimumValueofx(0.0);
  cooling_curve_6.setMaximumValueofx(1.0);

  CurveBiquadratic cooling_curve_7(model);
  cooling_curve_7.setCoefficient1Constant(1.0);
  cooling_curve_7.setCoefficient2x(0.0);
  cooling_curve_7.setCoefficient3xPOW2(0.0);
  cooling_curve_7.setCoefficient4y(0.0);
  cooling_curve_7.setCoefficient5yPOW2(0.0);
  cooling_curve_7.setCoefficient6xTIMESY(0.0);
  cooling_curve_7.setMinimumValueofx(10.0);
  cooling_curve_7.setMaximumValueofx(25.6);
  cooling_curve_7.setMinimumValueofy(7.2);
  cooling_curve_7.setMaximumValueofy(48.9);

  bool ok = true;
  ok = setReferenceUnitGrossRatedTotalCoolingCapacity(1524.1);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedSensibleHeatRatio(0.75);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedCoolingCOP(4.0);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlowRate(0.1359072);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedWaterFlowRate(0.000381695);
  OS_ASSERT(ok);
  ok = setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(0.1);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofTemperatureCurve(cooling_curve_1);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofAirFlowFractionCurve(cooling_curve_2);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(cooling_curve_3);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(cooling_curve_4);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(cooling_curve_5);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofWaterFlowFractionCurve(cooling_curve_6);
  OS_ASSERT(ok);
  ok = setWasteHeatFunctionofTemperatureCurve(cooling_curve_7);
  OS_ASSERT(ok);
}

CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
  const Model& model,
  const Curve& totalCoolingCapacityFunctionofTemperature,
  const Curve& totalCoolingCapacityFunctionofAirFlowFraction,
  const Curve& totalCoolingCapacityFunctionofWaterFlowFraction,
  const Curve& energyInputRatioFunctionofTemperature,
  const Curve& energyInputRatioFunctionofAirFlowFraction,
  const Curve& energyInputRatioFunctionofWaterFlowFraction,
  const Curve& wasteHeatFunctionofTemperature)
  : ParentObject(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>());

  bool ok = true;
  ok = setReferenceUnitGrossRatedTotalCoolingCapacity(1524.1);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedSensibleHeatRatio(0.75);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedCoolingCOP(4.0);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlowRate(0.1359072);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedWaterFlowRate(0.000381695);
  OS_ASSERT(ok);
  ok = setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(0.1);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofTemperatureCurve(totalCoolingCapacityFunctionofTemperature);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofAirFlowFractionCurve(totalCoolingCapacityFunctionofAirFlowFraction);
  OS_ASSERT(ok);
  ok = setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(totalCoolingCapacityFunctionofWaterFlowFraction);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperature);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(energyInputRatioFunctionofAirFlowFraction);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofWaterFlowFractionCurve(energyInputRatioFunctionofWaterFlowFraction);
  OS_ASSERT(ok);
  ok = setWasteHeatFunctionofTemperatureCurve(wasteHeatFunctionofTemperature);
  OS_ASSERT(ok);
}

IddObjectType CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData);
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedTotalCoolingCapacity() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedTotalCoolingCapacity();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedSensibleHeatRatio();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedCoolingCOP() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedCoolingCOP();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedAirFlowRate() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedAirFlowRate();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedWaterFlowRate() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedWaterFlowRate();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalCoolingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalCoolingCapacityFunctionofTemperatureCurve();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalCoolingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalCoolingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalCoolingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalCoolingCapacityFunctionofWaterFlowFractionCurve();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofWaterFlowFractionCurve();
}

double CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitWasteHeatFractionofInputPowerAtRatedConditions();
}

Curve CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedTotalCoolingCapacity(referenceUnitGrossRatedTotalCoolingCapacity);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedSensibleHeatRatio(referenceUnitGrossRatedSensibleHeatRatio);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedCoolingCOP(referenceUnitGrossRatedCoolingCOP);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedAirFlowRate(referenceUnitRatedAirFlowRate);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedWaterFlowRate(referenceUnitRatedWaterFlowRate);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalCoolingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalCoolingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(curve);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curve);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofWaterFlowFractionCurve(curve);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
}

bool CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curve);
}

/// @cond
CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

