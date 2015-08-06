/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"

#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_WaterHeating_AirToWaterHeatPump_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilWaterHeatingAirToWaterHeatPump_Impl::CoilWaterHeatingAirToWaterHeatPump_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilWaterHeatingAirToWaterHeatPump::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPump_Impl::CoilWaterHeatingAirToWaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilWaterHeatingAirToWaterHeatPump::iddObjectType());
  }

  CoilWaterHeatingAirToWaterHeatPump_Impl::CoilWaterHeatingAirToWaterHeatPump_Impl(const CoilWaterHeatingAirToWaterHeatPump_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
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

  unsigned CoilWaterHeatingAirToWaterHeatPump_Impl::airInletPort() {
    return OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirInletNodeName;
  }

  unsigned CoilWaterHeatingAirToWaterHeatPump_Impl::airOutletPort() {
    return OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirOutletNodeName;
  }

  unsigned CoilWaterHeatingAirToWaterHeatPump_Impl::waterInletPort() {
    return OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterInletNodeName;
  }

  unsigned CoilWaterHeatingAirToWaterHeatPump_Impl::waterOutletPort() {
    return OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterOutletNodeName;
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

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::isRatedEvaporatorAirFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  boost::optional<double> CoilWaterHeatingAirToWaterHeatPump_Impl::ratedCondenserWaterFlowRate() const {
    return getDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate,true);
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::isRatedCondenserWaterFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
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

  CurveBiquadratic CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCapacityFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalHeatingCapacityFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCapacityFunctionofWaterFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalHeatingCapacityFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveBiquadratic CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCOPFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalHeatingCOPFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCOPFunctionofAirFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalHeatingCOPFunctionofAirFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Air Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilWaterHeatingAirToWaterHeatPump_Impl::heatingCOPFunctionofWaterFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalHeatingCOPFunctionofWaterFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating COPFunctionof Water Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilWaterHeatingAirToWaterHeatPump_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<CurveQuadratic> value = optionalPartLoadFractionCorrelationCurve();
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

  void CoilWaterHeatingAirToWaterHeatPump_Impl::autocalculateRatedEvaporatorAirFlowRate() {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedEvaporatorAirFlowRate, "autocalculate");
    OS_ASSERT(result);
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate) {
    bool result(false);
    if (ratedCondenserWaterFlowRate) {
      result = setDouble(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate, ratedCondenserWaterFlowRate.get());
    }
    return result;
  }

  void CoilWaterHeatingAirToWaterHeatPump_Impl::autocalculateRatedCondenserWaterFlowRate() {
    bool result = setString(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::RatedCondenserWaterFlowRate, "autocalculate");
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

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofWaterFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCOPFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofTemperatureCurveName, curveBiquadratic.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCOPFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofAirFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setHeatingCOPFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofWaterFlowFractionCurveName, curveQuadratic.handle());
    return result;
  }

  bool CoilWaterHeatingAirToWaterHeatPump_Impl::setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::PartLoadFractionCorrelationCurveName, curveQuadratic.handle());
    return result;
  }

  boost::optional<CurveBiquadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCapacityFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<CurveBiquadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCOPFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofTemperatureCurveName);
  }

  boost::optional<CurveQuadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCOPFunctionofAirFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofAirFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalHeatingCOPFunctionofWaterFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofWaterFlowFractionCurveName);
  }

  boost::optional<CurveQuadratic> CoilWaterHeatingAirToWaterHeatPump_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_WaterHeating_AirToWaterHeatPumpFields::PartLoadFractionCorrelationCurveName);
  }

} // detail

CoilWaterHeatingAirToWaterHeatPump::CoilWaterHeatingAirToWaterHeatPump(const Model& model)
  : WaterToAirComponent(CoilWaterHeatingAirToWaterHeatPump::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirInletNodeName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirOutletNodeName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterInletNodeName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterOutletNodeName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofTemperatureCurveName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofAirFlowFractionCurveName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCapacityFunctionofWaterFlowFractionCurveName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofTemperatureCurveName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofAirFlowFractionCurveName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::HeatingCOPFunctionofWaterFlowFractionCurveName
  //     OS_Coil_WaterHeating_AirToWaterHeatPumpFields::PartLoadFractionCorrelationCurveName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setRatedHeatingCapacity();
  OS_ASSERT(ok);
  // ok = setRatedCOP();
  OS_ASSERT(ok);
  // ok = setRatedSensibleHeatRatio();
  OS_ASSERT(ok);
  // ok = setRatedEvaporatorInletAirDryBulbTemperature();
  OS_ASSERT(ok);
  // ok = setRatedEvaporatorInletAirWetBulbTemperature();
  OS_ASSERT(ok);
  // ok = setRatedCondenserInletWaterTemperature();
  OS_ASSERT(ok);
  // ok = setRatedEvaporatorAirFlowRate();
  OS_ASSERT(ok);
  // ok = setRatedCondenserWaterFlowRate();
  OS_ASSERT(ok);
  // setEvaporatorFanPowerIncludedinRatedCOP();
  // setCondenserPumpPowerIncludedinRatedCOP();
  // setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP();
  // ok = setCondenserWaterPumpPower();
  OS_ASSERT(ok);
  // ok = setFractionofCondenserPumpHeattoWater();
  OS_ASSERT(ok);
  // ok = setEvaporatorAirInletNode();
  OS_ASSERT(ok);
  // ok = setEvaporatorAirOutletNode();
  OS_ASSERT(ok);
  // ok = setCondenserWaterInletNode();
  OS_ASSERT(ok);
  // ok = setCondenserWaterOutletNode();
  OS_ASSERT(ok);
  // ok = setCrankcaseHeaterCapacity();
  OS_ASSERT(ok);
  // ok = setMaximumAmbientTemperatureforCrankcaseHeaterOperation();
  OS_ASSERT(ok);
  // ok = setEvaporatorAirTemperatureTypeforCurveObjects();
  OS_ASSERT(ok);
  // ok = setHeatingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setHeatingCapacityFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setHeatingCapacityFunctionofWaterFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setHeatingCOPFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setHeatingCOPFunctionofAirFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setHeatingCOPFunctionofWaterFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setPartLoadFractionCorrelationCurve();
  OS_ASSERT(ok);
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

bool CoilWaterHeatingAirToWaterHeatPump::isRatedEvaporatorAirFlowRateAutocalculated() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->isRatedEvaporatorAirFlowRateAutocalculated();
}

boost::optional<double> CoilWaterHeatingAirToWaterHeatPump::ratedCondenserWaterFlowRate() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->ratedCondenserWaterFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPump::isRatedCondenserWaterFlowRateAutocalculated() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->isRatedCondenserWaterFlowRateAutocalculated();
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

CurveBiquadratic CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

CurveQuadratic CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCapacityFunctionofAirFlowFractionCurve();
}

CurveQuadratic CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCapacityFunctionofWaterFlowFractionCurve();
}

CurveBiquadratic CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCOPFunctionofTemperatureCurve();
}

CurveQuadratic CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofAirFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCOPFunctionofAirFlowFractionCurve();
}

CurveQuadratic CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofWaterFlowFractionCurve() const {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->heatingCOPFunctionofWaterFlowFractionCurve();
}

CurveQuadratic CoilWaterHeatingAirToWaterHeatPump::partLoadFractionCorrelationCurve() const {
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

void CoilWaterHeatingAirToWaterHeatPump::autocalculateRatedEvaporatorAirFlowRate() {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->autocalculateRatedEvaporatorAirFlowRate();
}

bool CoilWaterHeatingAirToWaterHeatPump::setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setRatedCondenserWaterFlowRate(ratedCondenserWaterFlowRate);
}

void CoilWaterHeatingAirToWaterHeatPump::autocalculateRatedCondenserWaterFlowRate() {
  getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->autocalculateRatedCondenserWaterFlowRate();
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

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCapacityFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCapacityFunctionofWaterFlowFractionCurve(curveQuadratic);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCOPFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofAirFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCOPFunctionofAirFlowFractionCurve(curveQuadratic);
}

bool CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofWaterFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setHeatingCOPFunctionofWaterFlowFractionCurve(curveQuadratic);
}

bool CoilWaterHeatingAirToWaterHeatPump::setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilWaterHeatingAirToWaterHeatPump_Impl>()->setPartLoadFractionCorrelationCurve(curveQuadratic);
}

/// @cond
CoilWaterHeatingAirToWaterHeatPump::CoilWaterHeatingAirToWaterHeatPump(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPump_Impl> impl)
  : WaterToAirComponent(impl)
{}
/// @endcond

} // model
} // openstudio

