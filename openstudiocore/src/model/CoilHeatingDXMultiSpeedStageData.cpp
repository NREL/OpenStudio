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

#include "CoilHeatingDXMultiSpeedStageData.hpp"
#include "CoilHeatingDXMultiSpeedStageData_Impl.hpp"

#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/OS_Coil_Heating_DX_MultiSpeed_StageData_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingDXMultiSpeedStageData_Impl::CoilHeatingDXMultiSpeedStageData_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingDXMultiSpeedStageData::iddObjectType());
  }

  CoilHeatingDXMultiSpeedStageData_Impl::CoilHeatingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingDXMultiSpeedStageData::iddObjectType());
  }

  CoilHeatingDXMultiSpeedStageData_Impl::CoilHeatingDXMultiSpeedStageData_Impl(const CoilHeatingDXMultiSpeedStageData_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingDXMultiSpeedStageData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingDXMultiSpeedStageData_Impl::iddObjectType() const {
    return CoilHeatingDXMultiSpeedStageData::iddObjectType();
  }

  boost::optional<double> CoilHeatingDXMultiSpeedStageData_Impl::grossRatedTotalHeatingCapacity() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedTotalHeatingCapacity,true);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::isGrossRatedTotalHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedTotalHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingDXMultiSpeedStageData_Impl::grossRatedSensibleHeatRatio() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio,true);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::isGrossRatedSensibleHeatRatioAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::grossRatedHeatingCOP() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedHeatingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXMultiSpeedStageData_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedAirFlowRate,true);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedEvaporatorFanPowerPerVolumeFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilHeatingDXMultiSpeedStageData_Impl::totalHeatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalTotalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingDXMultiSpeedStageData_Impl::totalHeatingCapacityFunctionofFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalTotalHeatingCapacityFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Total Heating Capacity Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveBiquadratic CoilHeatingDXMultiSpeedStageData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingDXMultiSpeedStageData_Impl::energyInputRatioFunctionofFlowFractionCurve() const {
    boost::optional<CurveQuadratic> value = optionalEnergyInputRatioFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  CurveQuadratic CoilHeatingDXMultiSpeedStageData_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<CurveQuadratic> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::nominalTimeforCondensateRemovaltoBegin() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::NominalTimeforCondensateRemovaltoBegin,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::maximumCyclingRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::MaximumCyclingRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::latentCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::LatentCapacityTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::ratedWasteHeatFractionofPowerInput() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedWasteHeatFractionofPowerInput,true);
    OS_ASSERT(value);
    return value.get();
  }

  CurveBiquadratic CoilHeatingDXMultiSpeedStageData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<CurveBiquadratic> value = optionalWasteHeatFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::evaporativeCondenserEffectiveness() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EvaporativeCondenserEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingDXMultiSpeedStageData_Impl::evaporativeCondenserAirFlowRate() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate,true);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingDXMultiSpeedStageData_Impl::ratedEvaporativeCondenserPumpPowerConsumption() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption,true);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setGrossRatedTotalHeatingCapacity(boost::optional<double> grossRatedTotalHeatingCapacity) {
    bool result(false);
    if (grossRatedTotalHeatingCapacity) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedTotalHeatingCapacity, grossRatedTotalHeatingCapacity.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeedStageData_Impl::autosizeGrossRatedTotalHeatingCapacity() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedTotalHeatingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setGrossRatedSensibleHeatRatio(boost::optional<double> grossRatedSensibleHeatRatio) {
    bool result(false);
    if (grossRatedSensibleHeatRatio) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio, grossRatedSensibleHeatRatio.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeedStageData_Impl::autosizeGrossRatedSensibleHeatRatio() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedSensibleHeatRatio, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setGrossRatedHeatingCOP(double grossRatedHeatingCOP) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedHeatingCOP, grossRatedHeatingCOP);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeedStageData_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedEvaporatorFanPowerPerVolumeFlowRate, ratedEvaporatorFanPowerPerVolumeFlowRate);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setTotalHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::TotalHeatingCapacityFunctionofTemperatureCurve, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setTotalHeatingCapacityFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::TotalHeatingCapacityFunctionofFlowFractionCurve, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurve, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setEnergyInputRatioFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurve, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurve, curveQuadratic.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::NominalTimeforCondensateRemovaltoBegin, nominalTimeforCondensateRemovaltoBegin);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setMaximumCyclingRate(double maximumCyclingRate) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::MaximumCyclingRate, maximumCyclingRate);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setLatentCapacityTimeConstant(double latentCapacityTimeConstant) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::LatentCapacityTimeConstant, latentCapacityTimeConstant);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedWasteHeatFractionofPowerInput, ratedWasteHeatFractionofPowerInput);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurve, curveBiquadratic.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate) {
    bool result(false);
    if (evaporativeCondenserAirFlowRate) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate, evaporativeCondenserAirFlowRate.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeedStageData_Impl::autosizeEvaporativeCondenserAirFlowRate() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EvaporativeCondenserAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatedEvaporativeCondenserPumpPowerConsumption(boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption) {
    bool result(false);
    if (ratedEvaporativeCondenserPumpPowerConsumption) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption, ratedEvaporativeCondenserPumpPowerConsumption.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeedStageData_Impl::autosizeRatedEvaporativeCondenserPumpPowerConsumption() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedEvaporativeCondenserPumpPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<CurveBiquadratic> CoilHeatingDXMultiSpeedStageData_Impl::optionalTotalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::TotalHeatingCapacityFunctionofTemperatureCurve);
  }

  boost::optional<CurveQuadratic> CoilHeatingDXMultiSpeedStageData_Impl::optionalTotalHeatingCapacityFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::TotalHeatingCapacityFunctionofFlowFractionCurve);
  }

  boost::optional<CurveBiquadratic> CoilHeatingDXMultiSpeedStageData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurve);
  }

  boost::optional<CurveQuadratic> CoilHeatingDXMultiSpeedStageData_Impl::optionalEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurve);
  }

  boost::optional<CurveQuadratic> CoilHeatingDXMultiSpeedStageData_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurve);
  }

  boost::optional<CurveBiquadratic> CoilHeatingDXMultiSpeedStageData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurve);
  }

} // detail

CoilHeatingDXMultiSpeedStageData::CoilHeatingDXMultiSpeedStageData(const Model& model)
  : ParentObject(CoilHeatingDXMultiSpeedStageData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Coil_Heating_DX_MultiSpeed_StageDataFields::TotalHeatingCapacityFunctionofTemperatureCurve
  //     OS_Coil_Heating_DX_MultiSpeed_StageDataFields::TotalHeatingCapacityFunctionofFlowFractionCurve
  //     OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurve
  //     OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurve
  //     OS_Coil_Heating_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurve
  //     OS_Coil_Heating_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurve
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setGrossRatedTotalHeatingCapacity();
  OS_ASSERT(ok);
  // ok = setGrossRatedSensibleHeatRatio();
  OS_ASSERT(ok);
  // ok = setGrossRatedHeatingCOP();
  OS_ASSERT(ok);
  // ok = setRatedAirFlowRate();
  OS_ASSERT(ok);
  // ok = setRatedEvaporatorFanPowerPerVolumeFlowRate();
  OS_ASSERT(ok);
  // ok = setTotalHeatingCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setTotalHeatingCapacityFunctionofFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setEnergyInputRatioFunctionofFlowFractionCurve();
  OS_ASSERT(ok);
  // ok = setPartLoadFractionCorrelationCurve();
  OS_ASSERT(ok);
  // ok = setNominalTimeforCondensateRemovaltoBegin();
  OS_ASSERT(ok);
  // ok = setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  OS_ASSERT(ok);
  // ok = setMaximumCyclingRate();
  OS_ASSERT(ok);
  // ok = setLatentCapacityTimeConstant();
  OS_ASSERT(ok);
  // ok = setRatedWasteHeatFractionofPowerInput();
  OS_ASSERT(ok);
  // ok = setWasteHeatFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setEvaporativeCondenserEffectiveness();
  OS_ASSERT(ok);
  // ok = setEvaporativeCondenserAirFlowRate();
  OS_ASSERT(ok);
  // ok = setRatedEvaporativeCondenserPumpPowerConsumption();
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingDXMultiSpeedStageData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_MultiSpeed_StageData);
}

boost::optional<double> CoilHeatingDXMultiSpeedStageData::grossRatedTotalHeatingCapacity() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->grossRatedTotalHeatingCapacity();
}

bool CoilHeatingDXMultiSpeedStageData::isGrossRatedTotalHeatingCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->isGrossRatedTotalHeatingCapacityAutosized();
}

boost::optional<double> CoilHeatingDXMultiSpeedStageData::grossRatedSensibleHeatRatio() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->grossRatedSensibleHeatRatio();
}

bool CoilHeatingDXMultiSpeedStageData::isGrossRatedSensibleHeatRatioAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->isGrossRatedSensibleHeatRatioAutosized();
}

double CoilHeatingDXMultiSpeedStageData::grossRatedHeatingCOP() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->grossRatedHeatingCOP();
}

boost::optional<double> CoilHeatingDXMultiSpeedStageData::ratedAirFlowRate() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratedAirFlowRate();
}

bool CoilHeatingDXMultiSpeedStageData::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->isRatedAirFlowRateAutosized();
}

double CoilHeatingDXMultiSpeedStageData::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate();
}

CurveBiquadratic CoilHeatingDXMultiSpeedStageData::totalHeatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->totalHeatingCapacityFunctionofTemperatureCurve();
}

CurveQuadratic CoilHeatingDXMultiSpeedStageData::totalHeatingCapacityFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->totalHeatingCapacityFunctionofFlowFractionCurve();
}

CurveBiquadratic CoilHeatingDXMultiSpeedStageData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

CurveQuadratic CoilHeatingDXMultiSpeedStageData::energyInputRatioFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->energyInputRatioFunctionofFlowFractionCurve();
}

CurveQuadratic CoilHeatingDXMultiSpeedStageData::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->partLoadFractionCorrelationCurve();
}

double CoilHeatingDXMultiSpeedStageData::nominalTimeforCondensateRemovaltoBegin() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->nominalTimeforCondensateRemovaltoBegin();
}

double CoilHeatingDXMultiSpeedStageData::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
}

double CoilHeatingDXMultiSpeedStageData::maximumCyclingRate() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->maximumCyclingRate();
}

double CoilHeatingDXMultiSpeedStageData::latentCapacityTimeConstant() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->latentCapacityTimeConstant();
}

double CoilHeatingDXMultiSpeedStageData::ratedWasteHeatFractionofPowerInput() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratedWasteHeatFractionofPowerInput();
}

CurveBiquadratic CoilHeatingDXMultiSpeedStageData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

double CoilHeatingDXMultiSpeedStageData::evaporativeCondenserEffectiveness() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->evaporativeCondenserEffectiveness();
}

boost::optional<double> CoilHeatingDXMultiSpeedStageData::evaporativeCondenserAirFlowRate() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->evaporativeCondenserAirFlowRate();
}

bool CoilHeatingDXMultiSpeedStageData::isEvaporativeCondenserAirFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
}

boost::optional<double> CoilHeatingDXMultiSpeedStageData::ratedEvaporativeCondenserPumpPowerConsumption() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratedEvaporativeCondenserPumpPowerConsumption();
}

bool CoilHeatingDXMultiSpeedStageData::isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->isRatedEvaporativeCondenserPumpPowerConsumptionAutosized();
}

bool CoilHeatingDXMultiSpeedStageData::setGrossRatedTotalHeatingCapacity(double grossRatedTotalHeatingCapacity) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setGrossRatedTotalHeatingCapacity(grossRatedTotalHeatingCapacity);
}

void CoilHeatingDXMultiSpeedStageData::autosizeGrossRatedTotalHeatingCapacity() {
  getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->autosizeGrossRatedTotalHeatingCapacity();
}

bool CoilHeatingDXMultiSpeedStageData::setGrossRatedSensibleHeatRatio(double grossRatedSensibleHeatRatio) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setGrossRatedSensibleHeatRatio(grossRatedSensibleHeatRatio);
}

void CoilHeatingDXMultiSpeedStageData::autosizeGrossRatedSensibleHeatRatio() {
  getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->autosizeGrossRatedSensibleHeatRatio();
}

bool CoilHeatingDXMultiSpeedStageData::setGrossRatedHeatingCOP(double grossRatedHeatingCOP) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setGrossRatedHeatingCOP(grossRatedHeatingCOP);
}

bool CoilHeatingDXMultiSpeedStageData::setRatedAirFlowRate(double ratedAirFlowRate) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilHeatingDXMultiSpeedStageData::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->autosizeRatedAirFlowRate();
}

bool CoilHeatingDXMultiSpeedStageData::setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate(ratedEvaporatorFanPowerPerVolumeFlowRate);
}

bool CoilHeatingDXMultiSpeedStageData::setTotalHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setTotalHeatingCapacityFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingDXMultiSpeedStageData::setTotalHeatingCapacityFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setTotalHeatingCapacityFunctionofFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingDXMultiSpeedStageData::setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingDXMultiSpeedStageData::setEnergyInputRatioFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setEnergyInputRatioFunctionofFlowFractionCurve(curveQuadratic);
}

bool CoilHeatingDXMultiSpeedStageData::setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setPartLoadFractionCorrelationCurve(curveQuadratic);
}

bool CoilHeatingDXMultiSpeedStageData::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setNominalTimeforCondensateRemovaltoBegin(nominalTimeforCondensateRemovaltoBegin);
}

bool CoilHeatingDXMultiSpeedStageData::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
}

bool CoilHeatingDXMultiSpeedStageData::setMaximumCyclingRate(double maximumCyclingRate) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setMaximumCyclingRate(maximumCyclingRate);
}

bool CoilHeatingDXMultiSpeedStageData::setLatentCapacityTimeConstant(double latentCapacityTimeConstant) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setLatentCapacityTimeConstant(latentCapacityTimeConstant);
}

bool CoilHeatingDXMultiSpeedStageData::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatedWasteHeatFractionofPowerInput(ratedWasteHeatFractionofPowerInput);
}

bool CoilHeatingDXMultiSpeedStageData::setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curveBiquadratic);
}

bool CoilHeatingDXMultiSpeedStageData::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
}

bool CoilHeatingDXMultiSpeedStageData::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setEvaporativeCondenserAirFlowRate(evaporativeCondenserAirFlowRate);
}

void CoilHeatingDXMultiSpeedStageData::autosizeEvaporativeCondenserAirFlowRate() {
  getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
}

bool CoilHeatingDXMultiSpeedStageData::setRatedEvaporativeCondenserPumpPowerConsumption(double ratedEvaporativeCondenserPumpPowerConsumption) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatedEvaporativeCondenserPumpPowerConsumption(ratedEvaporativeCondenserPumpPowerConsumption);
}

void CoilHeatingDXMultiSpeedStageData::autosizeRatedEvaporativeCondenserPumpPowerConsumption() {
  getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->autosizeRatedEvaporativeCondenserPumpPowerConsumption();
}

/// @cond
CoilHeatingDXMultiSpeedStageData::CoilHeatingDXMultiSpeedStageData(std::shared_ptr<detail::CoilHeatingDXMultiSpeedStageData_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

