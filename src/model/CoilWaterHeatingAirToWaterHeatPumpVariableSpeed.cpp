/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result{
      // TODO: make sure this is right (cooling, not heating)
      // Taken directly from the I/O for E+ 8.7
      "Cooling Coil Total Cooling Rate",
      "Cooling Coil Total Cooling Energy",
      "Cooling Coil Sensible Cooling Rate",
      "Cooling Coil Sensible Cooling Energy",
      "Cooling Coil Latent Cooling Rate",
      "Cooling Coil Latent Cooling Energy",
      "Cooling Coil Runtime Fraction",
      "DX Cooling Coil Crankcase Heater Electricity Rate",
      "Cooling Coil Crankcase Heater Electricity Energy",
      "Cooling Coil Total Water Heating Rate",
      "Cooling Coil Total Water Heating Energy",
      "Cooling Coil Water Heating Electricity Rate",
      "Cooling Coil Water Heating Electricity Energy"
    };
    return result;
  }

  IddObjectType CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::iddObjectType() const {
    return CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedEvaporatorInletAirDryBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedEvaporatorInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedCondenserWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::ratedEvaporatorAirFlowRate() const {
    return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate,true);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::isRatedEvaporatorAirFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::evaporatorFanPowerIncludedinRatedCOP() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::evaporatorAirTemperatureTypeforCurveObjects() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::heatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::heatingCOPFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::heatingCOPFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedHeatingCapacity, ratedHeatingCapacity);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedCOP(double ratedCOP) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCOP, ratedCOP);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature, ratedEvaporatorInletAirDryBulbTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature, ratedEvaporatorInletAirWetBulbTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterTemperature, ratedCondenserWaterTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate) {
    bool result(false);
    if (ratedEvaporatorAirFlowRate) {
      result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate.get());
    }
    return result;
  }

  void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::autocalculateRatedEvaporatorAirFlowRate() {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, "autocalculate");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP) {
    return setBooleanFieldValue(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP, evaporatorFanPowerIncludedinRatedCOP);;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, maximumAmbientTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects) {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects, evaporatorAirTemperatureTypeforCurveObjects);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setHeatingCOPFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCOPFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCOPFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::optionalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::optionalHeatingCOPFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCOPFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::optionalHeatingCOPFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::HeatingCOPFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName);
  }

} // detail

CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(const Model& model)
  : ModelObject(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>());

  setRatedHeatingCapacity(2349.6);
  setRatedCOP(2.4);
  setRatedSensibleHeatRatio(0.981);
  setRatedEvaporatorInletAirDryBulbTemperature(19.72);
  setRatedEvaporatorInletAirWetBulbTemperature(13.5);
  setRatedCondenserWaterTemperature(48.89);
  setRatedEvaporatorAirFlowRate(0.189);
  setEvaporatorFanPowerIncludedinRatedCOP(true);
  setCrankcaseHeaterCapacity(0.0);
  setMaximumAmbientTemperatureforCrankcaseHeaterOperation(10.0);
  setEvaporatorAirTemperatureTypeforCurveObjects("WetBulbTemperature");

  {
    CurveBiquadratic curve(model);
    curve.setName("HPWH-Htg-Cap-fT");
    curve.setCoefficient1Constant(0.563);
    curve.setCoefficient2x(0.0437);
    curve.setCoefficient3xPOW2(0.000039);
    curve.setCoefficient4y(0.0055);
    curve.setCoefficient5yPOW2(-.000148);
    curve.setCoefficient6xTIMESY(-.000145);
    curve.setMinimumValueofx(0);
    curve.setMaximumValueofx(100);
    curve.setMinimumValueofy(0);
    curve.setMaximumValueofy(100);
    curve.setMinimumCurveOutput(0);

    setHeatingCapacityFunctionofTemperatureCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setName("HPWH-Htg-Cap-FF");
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setCoefficient4xPOW3(0.0);
    curve.setMinimumValueofx(0);
    curve.setMaximumValueofx(100);

    setHeatingCapacityFunctionofAirFlowFractionCurve(curve);
  }

  {
    CurveBiquadratic curve(model);
    curve.setName("HPWH-Htg-COP-fT");
    curve.setCoefficient1Constant(1.1332);
    curve.setCoefficient2x(0.063);
    curve.setCoefficient3xPOW2(-.0000979);
    curve.setCoefficient4y(-.00972);
    curve.setCoefficient5yPOW2(-.0000214);
    curve.setCoefficient6xTIMESY(-.000686);
    curve.setMinimumValueofx(0);
    curve.setMaximumValueofx(100);
    curve.setMinimumValueofy(0);
    curve.setMaximumValueofy(100);

    setHeatingCOPFunctionofTemperatureCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setName("HPWH-Htg-COP-FF");
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setCoefficient4xPOW3(0.0);
    curve.setMinimumValueofx(0);
    curve.setMaximumValueofx(100);

    setHeatingCOPFunctionofAirFlowFractionCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setName("HPWH-COP-fPLR");
    curve.setCoefficient1Constant(1);
    curve.setCoefficient2x(0);
    curve.setCoefficient3xPOW2(0);
    curve.setCoefficient4xPOW3(0);
    curve.setMinimumValueofx(0);
    curve.setMaximumValueofx(1);

    setPartLoadFractionCorrelationCurve(curve);
  }
}

IddObjectType CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed);
}

std::vector<std::string> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::evaporatorAirTemperatureTypeforCurveObjectsValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects);
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedHeatingCapacity() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedHeatingCapacity();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedCOP();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedSensibleHeatRatio() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedSensibleHeatRatio();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedEvaporatorInletAirDryBulbTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedEvaporatorInletAirDryBulbTemperature();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedEvaporatorInletAirWetBulbTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedEvaporatorInletAirWetBulbTemperature();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedCondenserWaterTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedCondenserWaterTemperature();
}

boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::ratedEvaporatorAirFlowRate() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->ratedEvaporatorAirFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::isRatedEvaporatorAirFlowRateAutocalculated() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->isRatedEvaporatorAirFlowRateAutocalculated();
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::evaporatorFanPowerIncludedinRatedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->evaporatorFanPowerIncludedinRatedCOP();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->crankcaseHeaterCapacity();
}

double CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->maximumAmbientTemperatureforCrankcaseHeaterOperation();
}

std::string CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::evaporatorAirTemperatureTypeforCurveObjects() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->evaporatorAirTemperatureTypeforCurveObjects();
}

Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::heatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->heatingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::heatingCOPFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->heatingCOPFunctionofTemperatureCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::heatingCOPFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->heatingCOPFunctionofAirFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->partLoadFractionCorrelationCurve();
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedHeatingCapacity(double ratedHeatingCapacity) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedCOP(double ratedCOP) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedCOP(ratedCOP);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedEvaporatorInletAirDryBulbTemperature(ratedEvaporatorInletAirDryBulbTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedEvaporatorInletAirWetBulbTemperature(ratedEvaporatorInletAirWetBulbTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedCondenserWaterTemperature(ratedCondenserWaterTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setRatedEvaporatorAirFlowRate(ratedEvaporatorAirFlowRate);
}

void CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::autocalculateRatedEvaporatorAirFlowRate() {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->autocalculateRatedEvaporatorAirFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setEvaporatorFanPowerIncludedinRatedCOP(evaporatorFanPowerIncludedinRatedCOP);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setMaximumAmbientTemperatureforCrankcaseHeaterOperation(maximumAmbientTemperatureforCrankcaseHeaterOperation);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setEvaporatorAirTemperatureTypeforCurveObjects(evaporatorAirTemperatureTypeforCurveObjects);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setHeatingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setHeatingCOPFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setHeatingCOPFunctionofTemperatureCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setHeatingCOPFunctionofAirFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

/// @cond
CoilWaterHeatingAirToWaterHeatPumpVariableSpeed::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
