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

#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"

#include <utilities/idd/OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other,
                                                                                                                                           Model_Impl* model,
                                                                                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::iddObjectType() const {
    return CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitGrossRatedHeatingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedAirFlow() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitRatedWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::heatingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::energyInputRatioFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalWasteHeatFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCapacity, referenceUnitGrossRatedHeatingCapacity);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitGrossRatedHeatingCOP, referenceUnitGrossRatedHeatingCOP);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedAirFlow, referenceUnitRatedAirFlow);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitRatedWaterFlowRate, referenceUnitRatedWaterFlowRate);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::ReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions, referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
    return result;
  }

  bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::TotalHeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::HeatingCapacityFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::EnergyInputRatioFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedDataFields::WasteHeatFunctionofTemperatureCurveName);
  }

  ModelObject CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::clone(Model model) const {
    auto t_clone = ParentObject_Impl::clone(model).cast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>();

    return t_clone;
  }

  std::vector<ModelObject> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl::children() const {
    std::vector<ModelObject> children;
    children.push_back( heatingCapacityFunctionofTemperatureCurve() );
    children.push_back( totalHeatingCapacityFunctionofAirFlowFractionCurve() );
    children.push_back( heatingCapacityFunctionofWaterFlowFractionCurve() );
    children.push_back( energyInputRatioFunctionofTemperatureCurve() );
    children.push_back( energyInputRatioFunctionofAirFlowFractionCurve() );
    children.push_back( energyInputRatioFunctionofWaterFlowFractionCurve() );
    children.push_back( wasteHeatFunctionofTemperatureCurve() );
    return children;
  }

} // detail

CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model)
  : ParentObject(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>());

  CurveBiquadratic heating_curve_1(model);
  heating_curve_1.setCoefficient1Constant(0.617474);
  heating_curve_1.setCoefficient2x(-0.00245669);
  heating_curve_1.setCoefficient3xPOW2(-1.86550E-05);
  heating_curve_1.setCoefficient4y(0.0254921);
  heating_curve_1.setCoefficient5yPOW2(-1.00773E-04);
  heating_curve_1.setCoefficient6xTIMESY(-1.09447E-04);
  heating_curve_1.setMinimumValueofx(7.0);
  heating_curve_1.setMaximumValueofx(27.0);
  heating_curve_1.setMinimumValueofy(10.0);
  heating_curve_1.setMaximumValueofy(30.0);

  CurveQuadratic heating_curve_2(model);
  heating_curve_2.setCoefficient1Constant(1.0);
  heating_curve_2.setCoefficient2x(0.0);
  heating_curve_2.setCoefficient3xPOW2(0.0);
  heating_curve_2.setMinimumValueofx(0.0);
  heating_curve_2.setMaximumValueofx(1.0);

  CurveQuadratic heating_curve_3(model);
  heating_curve_3.setCoefficient1Constant(1.0);
  heating_curve_3.setCoefficient2x(0.0);
  heating_curve_3.setCoefficient3xPOW2(0.0);
  heating_curve_3.setMinimumValueofx(0.0);
  heating_curve_3.setMaximumValueofx(1.0);

  CurveBiquadratic heating_curve_4(model);
  heating_curve_4.setCoefficient1Constant(0.993257);
  heating_curve_4.setCoefficient2x(0.0201512);
  heating_curve_4.setCoefficient3xPOW2(7.72375E-05);
  heating_curve_4.setCoefficient4y(-0.0317207);
  heating_curve_4.setCoefficient5yPOW2(0.000740649);
  heating_curve_4.setCoefficient6xTIMESY(-3.03875E-04);
  heating_curve_4.setMinimumValueofx(7.0);
  heating_curve_4.setMaximumValueofx(27.0);
  heating_curve_4.setMinimumValueofy(10.0);
  heating_curve_4.setMaximumValueofy(30.0);

  CurveQuadratic heating_curve_5(model);
  heating_curve_5.setCoefficient1Constant(1.0);
  heating_curve_5.setCoefficient2x(0.0);
  heating_curve_5.setCoefficient3xPOW2(0.0);
  heating_curve_5.setMinimumValueofx(0.0);
  heating_curve_5.setMaximumValueofx(1.0);

  CurveQuadratic heating_curve_6(model);
  heating_curve_6.setCoefficient1Constant(1.0);
  heating_curve_6.setCoefficient2x(0.0);
  heating_curve_6.setCoefficient3xPOW2(0.0);
  heating_curve_6.setMinimumValueofx(0.0);
  heating_curve_6.setMaximumValueofx(1.0);

  CurveBiquadratic heating_curve_7(model);
  heating_curve_7.setCoefficient1Constant(1.0);
  heating_curve_7.setCoefficient2x(0.0);
  heating_curve_7.setCoefficient3xPOW2(0.0);
  heating_curve_7.setCoefficient4y(0.0);
  heating_curve_7.setCoefficient5yPOW2(0.0);
  heating_curve_7.setCoefficient6xTIMESY(0.0);
  heating_curve_7.setMinimumValueofx(7.0);
  heating_curve_7.setMaximumValueofx(27.0);
  heating_curve_7.setMinimumValueofy(10.0);
  heating_curve_7.setMaximumValueofy(30.0);

  bool ok = true;
  ok = setReferenceUnitGrossRatedHeatingCapacity(1838.7);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedHeatingCOP(5.0);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlow(0.1661088);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedWaterFlowRate(0.000381695);
  OS_ASSERT(ok);
  ok = setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(0.1);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofTemperatureCurve(heating_curve_1);
  OS_ASSERT(ok);
  ok = setTotalHeatingCapacityFunctionofAirFlowFractionCurve(heating_curve_2);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofWaterFlowFractionCurve(heating_curve_3);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(heating_curve_4);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofAirFlowFractionCurve(heating_curve_5);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofWaterFlowFractionCurve(heating_curve_6);
  OS_ASSERT(ok);
  ok = setWasteHeatFunctionofTemperatureCurve(heating_curve_7);
  OS_ASSERT(ok);
}

CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
  const Model& model,
  const Curve& heatingCapacityFunctionofTemperature,
  const Curve& totalHeatingCapacityFunctionofAirFlowFraction,
  const Curve& heatingCapacityFunctionofWaterFlowFraction,
  const Curve& energyInputRatioFunctionofTemperature,
  const Curve& energyInputRatioFunctionofAirFlowFraction,
  const Curve& energyInputRatioFunctionofWaterFlowFraction,
  const Curve& wasteHeatFunctionofTemperature)
  : ParentObject(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>());

  bool ok = true;
  ok = setReferenceUnitGrossRatedHeatingCapacity(1838.7);
  OS_ASSERT(ok);
  ok = setReferenceUnitGrossRatedHeatingCOP(5.0);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedAirFlow(0.1661088);
  OS_ASSERT(ok);
  ok = setReferenceUnitRatedWaterFlowRate(0.000381695);
  OS_ASSERT(ok);
  ok = setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(0.1);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofTemperatureCurve(heatingCapacityFunctionofTemperature);
  OS_ASSERT(ok);
  ok = setTotalHeatingCapacityFunctionofAirFlowFractionCurve(totalHeatingCapacityFunctionofAirFlowFraction);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofWaterFlowFractionCurve(heatingCapacityFunctionofWaterFlowFraction);
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

IddObjectType CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData);
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedHeatingCapacity() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedHeatingCapacity();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitGrossRatedHeatingCOP() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitGrossRatedHeatingCOP();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedAirFlow() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedAirFlow();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitRatedWaterFlowRate() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitRatedWaterFlowRate();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::totalHeatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->totalHeatingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::heatingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->heatingCapacityFunctionofWaterFlowFractionCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofAirFlowFractionCurve();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::energyInputRatioFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->energyInputRatioFunctionofWaterFlowFractionCurve();
}

double CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->referenceUnitWasteHeatFractionofInputPowerAtRatedConditions();
}

Curve CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedHeatingCapacity(referenceUnitGrossRatedHeatingCapacity);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitGrossRatedHeatingCOP(referenceUnitGrossRatedHeatingCOP);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedAirFlow(referenceUnitRatedAirFlow);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitRatedWaterFlowRate(referenceUnitRatedWaterFlowRate);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setTotalHeatingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setHeatingCapacityFunctionofWaterFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofAirFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setEnergyInputRatioFunctionofWaterFlowFractionCurve(curve);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);
}

bool CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curve);
}

/// @cond
CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

