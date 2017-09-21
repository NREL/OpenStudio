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

#include "CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_WaterHeating_AirToWaterHeatPump_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilWaterHeatingAirToWaterHeatPump_Impl::CoilWaterHeatingAirToWaterHeatPump_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : HVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingAirToWaterHeatPump::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPump_Impl::CoilWaterHeatingAirToWaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilWaterHeatingAirToWaterHeatPump::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPump_Impl::CoilWaterHeatingAirToWaterHeatPump_Impl(const CoilWaterHeatingAirToWaterHeatPump_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilWaterHeatingAirToWaterHeatPump_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilWaterHeatingAirToWaterHeatPump_Impl::iddObjectType() const {
    return CoilWaterHeatingAirToWaterHeatPump::iddObjectType();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::ratedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::ratedCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::ratedSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::ratedEvaporatorInletAirDryBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorInletAirDryBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::ratedEvaporatorInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::ratedCondenserInletWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserInletWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPump_Impl::ratedEvaporatorAirFlowRate() const {
    return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate,true);
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::isRatedEvaporatorAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPump_Impl::ratedCondenserWaterFlowRate() const {
    return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate,true);
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::isRatedCondenserWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::evaporatorFanPowerIncludedinRatedCOP() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorFanPowerIncludedinRatedCOP,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::condenserPumpPowerIncludedinRatedCOP() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpPowerIncludedinRatedCOP,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::condenserWaterPumpPower() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterPumpPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::fractionofCondenserPumpHeattoWater() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::FractionofCondenserPumpHeattoWater,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::crankcaseHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CrankcaseHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilWaterHeatingAirToWaterHeatPump_Impl::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string CoilWaterHeatingAirToWaterHeatPump_Impl::evaporatorAirTemperatureTypeforCurveObjects() const {
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirTemperatureTypeforCurveObjects,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCOPFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCOPFunctionofAirFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCOPFunctionofWaterFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCOPFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilWaterHeatingAirToWaterHeatPump_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedHeatingCapacity, ratedHeatingCapacity);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedCOP(double ratedCOP) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCOP, ratedCOP);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorInletAirDryBulbTemperature, ratedEvaporatorInletAirDryBulbTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorInletAirWetBulbTemperature, ratedEvaporatorInletAirWetBulbTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserInletWaterTemperature, ratedCondenserInletWaterTemperature);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate) {
    bool result(false);
    if (ratedEvaporatorAirFlowRate) {
      result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate.get());
    }
    return result;
  }

  void CoilWaterHeatingAirToWaterHeatPump_Impl::autosizeRatedEvaporatorAirFlowRate() {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate) {
    bool result(false);
    if (ratedCondenserWaterFlowRate) {
      result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate, ratedCondenserWaterFlowRate.get());
    }
    return result;
  }

  void CoilWaterHeatingAirToWaterHeatPump_Impl::autosizeRatedCondenserWaterFlowRate() {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void CoilWaterHeatingAirToWaterHeatPump_Impl::setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP) {
    setBooleanFieldValue(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorFanPowerIncludedinRatedCOP, evaporatorFanPowerIncludedinRatedCOP);
  }

  void CoilWaterHeatingAirToWaterHeatPump_Impl::setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP) {
    setBooleanFieldValue(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpPowerIncludedinRatedCOP, condenserPumpPowerIncludedinRatedCOP);
  }

  void CoilWaterHeatingAirToWaterHeatPump_Impl::setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP) {
    setBooleanFieldValue(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP, condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setCondenserWaterPumpPower(double condenserWaterPumpPower) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterPumpPower, condenserWaterPumpPower);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::FractionofCondenserPumpHeattoWater, fractionofCondenserPumpHeattoWater);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CrankcaseHeaterCapacity, crankcaseHeaterCapacity);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
    bool result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation, maximumAmbientTemperatureforCrankcaseHeaterOperation);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects) {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirTemperatureTypeforCurveObjects, evaporatorAirTemperatureTypeforCurveObjects);
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofAirFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofWaterFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCOPFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofTemperatureCurve, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofAirFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofWaterFlowFractionCurve, curve.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::PartLoadFractionCorrelationCurve, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofAirFlowFractionCurve);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofWaterFlowFractionCurve);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCOPFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofTemperatureCurve);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCOPFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofAirFlowFractionCurve);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCOPFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofWaterFlowFractionCurve);
  }

  boost::optional<Curve> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::PartLoadFractionCorrelationCurve);
  }

  ModelObject CoilWaterHeatingAirToWaterHeatPump_Impl::clone(Model model) const
  {
    auto newCoil = ModelObject_Impl::clone(model).cast<CoilWaterHeatingAirToWaterHeatPump>();

    return newCoil;
  }

  std::vector<ModelObject> CoilWaterHeatingAirToWaterHeatPump_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(heatingCapacityFunctionofTemperatureCurve());
    result.push_back(heatingCapacityFunctionofAirFlowFractionCurve());
    result.push_back(heatingCapacityFunctionofWaterFlowFractionCurve());
    result.push_back(heatingCOPFunctionofTemperatureCurve());
    result.push_back(heatingCOPFunctionofAirFlowFractionCurve());
    result.push_back(heatingCOPFunctionofWaterFlowFractionCurve());
    result.push_back(partLoadFractionCorrelationCurve());

    return result;
  }

} // detail

CoilWaterHeatingAirToWaterHeatPump::CoilWaterHeatingAirToWaterHeatPump(const Model& model)
  : HVACComponent(CoilWaterHeatingAirToWaterHeatPump::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>());

  setRatedHeatingCapacity(4000.0);
  setRatedCOP(3.2);
  setRatedSensibleHeatRatio(0.6956);
  setRatedEvaporatorInletAirDryBulbTemperature(29.44);
  setRatedEvaporatorInletAirWetBulbTemperature(22.22);
  setRatedCondenserInletWaterTemperature(55.72);
  autosizeRatedEvaporatorAirFlowRate();
  autosizeRatedCondenserWaterFlowRate();
  setEvaporatorFanPowerIncludedinRatedCOP(false);
  setCondenserPumpPowerIncludedinRatedCOP(false);
  setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(false);
  setCondenserWaterPumpPower(150.0);
  setFractionofCondenserPumpHeattoWater(0.1);
  setCrankcaseHeaterCapacity(100.0);
  setMaximumAmbientTemperatureforCrankcaseHeaterOperation(5.0);
  setEvaporatorAirTemperatureTypeforCurveObjects("WetBulbTemperature");

  {
    CurveBiquadratic curve(model);
    curve.setCoefficient1Constant(0.369827);
    curve.setCoefficient2x(0.043341);
    curve.setCoefficient3xPOW2(-0.00023);
    curve.setCoefficient4y(0.000466);
    curve.setCoefficient5yPOW2(0.000026);
    curve.setCoefficient6xTIMESY(-0.00027);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(40.0);
    curve.setMinimumValueofy(20.0);
    curve.setMaximumValueofy(90.0);
    curve.setInputUnitTypeforX("Temperature");
    curve.setInputUnitTypeforY("Temperature");
    curve.setOutputUnitType("Dimensionless");
    setHeatingCapacityFunctionofTemperatureCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    setHeatingCapacityFunctionofAirFlowFractionCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    setHeatingCapacityFunctionofWaterFlowFractionCurve(curve);
  }

  {
    CurveBiquadratic curve(model);
    curve.setCoefficient1Constant(1.19713);
    curve.setCoefficient2x(0.077849);
    curve.setCoefficient3xPOW2(-0.0000016);
    curve.setCoefficient4y(-0.02675);
    curve.setCoefficient5yPOW2(0.000296);
    curve.setCoefficient6xTIMESY(-0.00112);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(40.0);
    curve.setMinimumValueofy(20.0);
    curve.setMaximumValueofy(90.0);
    curve.setInputUnitTypeforX("Temperature");
    curve.setInputUnitTypeforY("Temperature");
    curve.setOutputUnitType("Dimensionless");
    setHeatingCOPFunctionofTemperatureCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    setHeatingCOPFunctionofAirFlowFractionCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    setHeatingCOPFunctionofWaterFlowFractionCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(0.75);
    curve.setCoefficient2x(0.25);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(1.0);
    setPartLoadFractionCorrelationCurve(curve);
  }

}

CoilWaterHeatingAirToWaterHeatPump::CoilWaterHeatingAirToWaterHeatPump(const Model& model,
  Curve & heatingCapacityFunctionofTemperatureCurve,
  Curve & heatingCapacityFunctionofAirFlowFractionCurve,
  Curve & heatingCapacityFunctionofWaterFlowFractionCurve,
  Curve & heatingCOPFunctionofTemperatureCurve,
  Curve & heatingCOPFunctionofAirFlowFractionCurve,
  Curve & heatingCOPFunctionofWaterFlowFractionCurve,
  Curve & partLoadFractionCorrelationCurve)
  : HVACComponent(CoilWaterHeatingAirToWaterHeatPump::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>());

  setRatedHeatingCapacity(4000.0);
  setRatedCOP(3.2);
  setRatedSensibleHeatRatio(0.6956);
  setRatedEvaporatorInletAirDryBulbTemperature(29.44);
  setRatedEvaporatorInletAirWetBulbTemperature(22.22);
  setRatedCondenserInletWaterTemperature(55.72);
  autosizeRatedEvaporatorAirFlowRate();
  autosizeRatedCondenserWaterFlowRate();
  setEvaporatorFanPowerIncludedinRatedCOP(false);
  setCondenserPumpPowerIncludedinRatedCOP(false);
  setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(false);
  setCondenserWaterPumpPower(150.0);
  setFractionofCondenserPumpHeattoWater(0.1);
  setCrankcaseHeaterCapacity(100.0);
  setMaximumAmbientTemperatureforCrankcaseHeaterOperation(5.0);
  setEvaporatorAirTemperatureTypeforCurveObjects("WetBulbTemperature");

  setHeatingCapacityFunctionofTemperatureCurve(heatingCapacityFunctionofTemperatureCurve);
  setHeatingCapacityFunctionofAirFlowFractionCurve(heatingCapacityFunctionofAirFlowFractionCurve);
  setHeatingCapacityFunctionofWaterFlowFractionCurve(heatingCapacityFunctionofWaterFlowFractionCurve);
  setHeatingCOPFunctionofTemperatureCurve(heatingCOPFunctionofTemperatureCurve);
  setHeatingCOPFunctionofAirFlowFractionCurve(heatingCOPFunctionofAirFlowFractionCurve);
  setHeatingCOPFunctionofWaterFlowFractionCurve(heatingCOPFunctionofWaterFlowFractionCurve);
  setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve);
}

IddObjectType CoilWaterHeatingAirToWaterHeatPump::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump);
}

std::vector<std::string> CoilWaterHeatingAirToWaterHeatPump::evaporatorAirTemperatureTypeforCurveObjectsValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirTemperatureTypeforCurveObjects);
}

double CoilWaterHeatingAirToWaterHeatPump::ratedHeatingCapacity() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedHeatingCapacity();
}

double CoilWaterHeatingAirToWaterHeatPump::ratedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedCOP();
}

double CoilWaterHeatingAirToWaterHeatPump::ratedSensibleHeatRatio() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedSensibleHeatRatio();
}

double CoilWaterHeatingAirToWaterHeatPump::ratedEvaporatorInletAirDryBulbTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedEvaporatorInletAirDryBulbTemperature();
}

double CoilWaterHeatingAirToWaterHeatPump::ratedEvaporatorInletAirWetBulbTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedEvaporatorInletAirWetBulbTemperature();
}

double CoilWaterHeatingAirToWaterHeatPump::ratedCondenserInletWaterTemperature() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedCondenserInletWaterTemperature();
}

boost::optional<double> CoilWaterHeatingAirToWaterHeatPump::ratedEvaporatorAirFlowRate() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedEvaporatorAirFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPump::isRatedEvaporatorAirFlowRateAutosized() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->isRatedEvaporatorAirFlowRateAutosized();
}

boost::optional<double> CoilWaterHeatingAirToWaterHeatPump::ratedCondenserWaterFlowRate() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedCondenserWaterFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPump::isRatedCondenserWaterFlowRateAutosized() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->isRatedCondenserWaterFlowRateAutosized();
}

bool CoilWaterHeatingAirToWaterHeatPump::evaporatorFanPowerIncludedinRatedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->evaporatorFanPowerIncludedinRatedCOP();
}

bool CoilWaterHeatingAirToWaterHeatPump::condenserPumpPowerIncludedinRatedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->condenserPumpPowerIncludedinRatedCOP();
}

bool CoilWaterHeatingAirToWaterHeatPump::condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP();
}

double CoilWaterHeatingAirToWaterHeatPump::condenserWaterPumpPower() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->condenserWaterPumpPower();
}

double CoilWaterHeatingAirToWaterHeatPump::fractionofCondenserPumpHeattoWater() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->fractionofCondenserPumpHeattoWater();
}

double CoilWaterHeatingAirToWaterHeatPump::crankcaseHeaterCapacity() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->crankcaseHeaterCapacity();
}

double CoilWaterHeatingAirToWaterHeatPump::maximumAmbientTemperatureforCrankcaseHeaterOperation() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->maximumAmbientTemperatureforCrankcaseHeaterOperation();
}

std::string CoilWaterHeatingAirToWaterHeatPump::evaporatorAirTemperatureTypeforCurveObjects() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->evaporatorAirTemperatureTypeforCurveObjects();
}

Curve CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCapacityFunctionofAirFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCapacityFunctionofWaterFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCOPFunctionofTemperatureCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCOPFunctionofAirFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCOPFunctionofWaterFlowFractionCurve();
}

Curve CoilWaterHeatingAirToWaterHeatPump::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->partLoadFractionCorrelationCurve();
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedHeatingCapacity(double ratedHeatingCapacity) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedCOP(double ratedCOP) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedCOP(ratedCOP);
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedEvaporatorInletAirDryBulbTemperature(ratedEvaporatorInletAirDryBulbTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedEvaporatorInletAirWetBulbTemperature(ratedEvaporatorInletAirWetBulbTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedCondenserInletWaterTemperature(ratedCondenserInletWaterTemperature);
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedEvaporatorAirFlowRate(ratedEvaporatorAirFlowRate);
}

void CoilWaterHeatingAirToWaterHeatPump::autosizeRatedEvaporatorAirFlowRate() {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->autosizeRatedEvaporatorAirFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedCondenserWaterFlowRate(ratedCondenserWaterFlowRate);
}

void CoilWaterHeatingAirToWaterHeatPump::autosizeRatedCondenserWaterFlowRate() {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->autosizeRatedCondenserWaterFlowRate();
}

void CoilWaterHeatingAirToWaterHeatPump::setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP) {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setEvaporatorFanPowerIncludedinRatedCOP(evaporatorFanPowerIncludedinRatedCOP);
}

void CoilWaterHeatingAirToWaterHeatPump::setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP) {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setCondenserPumpPowerIncludedinRatedCOP(condenserPumpPowerIncludedinRatedCOP);
}

void CoilWaterHeatingAirToWaterHeatPump::setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP) {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);
}

bool CoilWaterHeatingAirToWaterHeatPump::setCondenserWaterPumpPower(double condenserWaterPumpPower) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setCondenserWaterPumpPower(condenserWaterPumpPower);
}

bool CoilWaterHeatingAirToWaterHeatPump::setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setFractionofCondenserPumpHeattoWater(fractionofCondenserPumpHeattoWater);
}

bool CoilWaterHeatingAirToWaterHeatPump::setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setCrankcaseHeaterCapacity(crankcaseHeaterCapacity);
}

bool CoilWaterHeatingAirToWaterHeatPump::setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setMaximumAmbientTemperatureforCrankcaseHeaterOperation(maximumAmbientTemperatureforCrankcaseHeaterOperation);
}

bool CoilWaterHeatingAirToWaterHeatPump::setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setEvaporatorAirTemperatureTypeforCurveObjects(evaporatorAirTemperatureTypeforCurveObjects);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCapacityFunctionofAirFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCapacityFunctionofWaterFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCOPFunctionofTemperatureCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCOPFunctionofAirFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCOPFunctionofWaterFlowFractionCurve(curve);
}

bool CoilWaterHeatingAirToWaterHeatPump::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

/// @cond
CoilWaterHeatingAirToWaterHeatPump::CoilWaterHeatingAirToWaterHeatPump(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPump_Impl> impl)
  : HVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

