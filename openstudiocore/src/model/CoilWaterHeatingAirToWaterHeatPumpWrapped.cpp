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

#include "CoilWaterHeatingAirToWaterHeatPumpWrapped.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_WaterHeating_AirToWaterHeatPump_Wrapped_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const IdfObject& idfObject,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpWrapped::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilWaterHeatingAirToWaterHeatPumpWrapped::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::iddObjectType() const {
    return CoilWaterHeatingAirToWaterHeatPumpWrapped::iddObjectType();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedEvaporatorInletAirDryBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirDryBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedEvaporatorInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedCondenserWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCondenserWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::ratedEvaporatorAirFlowRate() const {
    return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate,true);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::isRatedEvaporatorAirFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::evaporatorFanPowerIncludedinRatedCOP() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorFanPowerIncludedinRatedCOP,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::evaporatorAirTemperatureTypeforCurveObjects() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirTemperatureTypeforCurveObjects,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::heatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::heatingCOPFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::heatingCOPFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedHeatingCapacity, ratedHeatingCapacity);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedCOP(double ratedCOP) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCOP, ratedCOP);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirDryBulbTemperature, ratedEvaporatorInletAirDryBulbTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirWetBulbTemperature, ratedEvaporatorInletAirWetBulbTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCondenserWaterTemperature, ratedCondenserWaterTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate) {
    bool result(false);
    if (ratedEvaporatorAirFlowRate) {
      result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate.get());
    }
    return result;
  }

  void CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::autocalculateRatedEvaporatorAirFlowRate() {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate, "autocalculate");
    OS_ASSERT(result);
  }

  void CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP) {
    setBooleanFieldValue(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorFanPowerIncludedinRatedCOP, evaporatorFanPowerIncludedinRatedCOP);
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, maximumAmbientTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects) {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirTemperatureTypeforCurveObjects, evaporatorAirTemperatureTypeforCurveObjects);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setHeatingCOPFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofAirFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::PartLoadFractionCorrelationCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::optionalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::optionalHeatingCOPFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::optionalHeatingCOPFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofAirFlowFractionCurveName);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::PartLoadFractionCorrelationCurveName);
  }

} // detail

CoilWaterHeatingAirToWaterHeatPumpWrapped::CoilWaterHeatingAirToWaterHeatPumpWrapped(const Model& model)
  : ModelObject(CoilWaterHeatingAirToWaterHeatPumpWrapped::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>());

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

IddObjectType CoilWaterHeatingAirToWaterHeatPumpWrapped::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_Wrapped);
}

std::vector<std::string> CoilWaterHeatingAirToWaterHeatPumpWrapped::evaporatorAirTemperatureTypeforCurveObjectsValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirTemperatureTypeforCurveObjects);
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedHeatingCapacity() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedHeatingCapacity();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedCOP();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedSensibleHeatRatio() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedSensibleHeatRatio();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedEvaporatorInletAirDryBulbTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedEvaporatorInletAirDryBulbTemperature();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedEvaporatorInletAirWetBulbTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedEvaporatorInletAirWetBulbTemperature();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedCondenserWaterTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedCondenserWaterTemperature();
}

boost::optional<double> CoilWaterHeatingAirToWaterHeatPumpWrapped::ratedEvaporatorAirFlowRate() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->ratedEvaporatorAirFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::isRatedEvaporatorAirFlowRateAutocalculated() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->isRatedEvaporatorAirFlowRateAutocalculated();
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::evaporatorFanPowerIncludedinRatedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->evaporatorFanPowerIncludedinRatedCOP();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->crankcaseHeaterCapacity();
}

double CoilWaterHeatingAirToWaterHeatPumpWrapped::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->maximumAmbientTemperatureforCrankcaseHeaterOperation();
}

std::string CoilWaterHeatingAirToWaterHeatPumpWrapped::evaporatorAirTemperatureTypeforCurveObjects() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->evaporatorAirTemperatureTypeforCurveObjects();
}

Curve CoilWaterHeatingAirToWaterHeatPumpWrapped::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpWrapped::heatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->heatingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpWrapped::heatingCOPFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->heatingCOPFunctionofTemperatureCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpWrapped::heatingCOPFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->heatingCOPFunctionofAirFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPumpWrapped::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->partLoadFractionCorrelationCurve();
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedHeatingCapacity(double ratedHeatingCapacity) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedCOP(double ratedCOP) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedCOP(ratedCOP);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedEvaporatorInletAirDryBulbTemperature(ratedEvaporatorInletAirDryBulbTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedEvaporatorInletAirWetBulbTemperature(ratedEvaporatorInletAirWetBulbTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedCondenserWaterTemperature(ratedCondenserWaterTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setRatedEvaporatorAirFlowRate(ratedEvaporatorAirFlowRate);
}

void CoilWaterHeatingAirToWaterHeatPumpWrapped::autocalculateRatedEvaporatorAirFlowRate() {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->autocalculateRatedEvaporatorAirFlowRate();
}

void CoilWaterHeatingAirToWaterHeatPumpWrapped::setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP) {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setEvaporatorFanPowerIncludedinRatedCOP(evaporatorFanPowerIncludedinRatedCOP);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setMaximumAmbientTemperatureforCrankcaseHeaterOperation(maximumAmbientTemperatureforCrankcaseHeaterOperation);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setEvaporatorAirTemperatureTypeforCurveObjects(evaporatorAirTemperatureTypeforCurveObjects);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setHeatingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setHeatingCOPFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setHeatingCOPFunctionofTemperatureCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setHeatingCOPFunctionofAirFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPumpWrapped::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

/// @cond
CoilWaterHeatingAirToWaterHeatPumpWrapped::CoilWaterHeatingAirToWaterHeatPumpWrapped(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

