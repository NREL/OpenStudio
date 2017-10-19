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

#include "CoilHeatingDXMultiSpeedStageData.hpp"
#include "CoilHeatingDXMultiSpeedStageData_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveQuadratic.hpp"

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

  boost::optional<double> CoilHeatingDXMultiSpeedStageData_Impl::grossRatedHeatingCapacity() const {
    return getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedHeatingCapacity,true);
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::isGrossRatedHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedHeatingCapacity, true);
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

  double CoilHeatingDXMultiSpeedStageData_Impl::ratedSupplyAirFanPowerPerVolumeFlowRate() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedSupplyAirFanPowerPerVolumeFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilHeatingDXMultiSpeedStageData_Impl::heatingCapacityFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXMultiSpeedStageData_Impl::heatingCapacityFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalHeatingCapacityFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Capacity Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXMultiSpeedStageData_Impl::energyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXMultiSpeedStageData_Impl::energyInputRatioFunctionofFlowFractionCurve() const {
    boost::optional<Curve> value = optionalEnergyInputRatioFunctionofFlowFractionCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Energy Input Ratio Functionof Flow Fraction Curve attached.");
    }
    return value.get();
  }

  Curve CoilHeatingDXMultiSpeedStageData_Impl::partLoadFractionCorrelationCurve() const {
    boost::optional<Curve> value = optionalPartLoadFractionCorrelationCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Part Load Fraction Correlation Curve attached.");
    }
    return value.get();
  }

  double CoilHeatingDXMultiSpeedStageData_Impl::ratedWasteHeatFractionofPowerInput() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedWasteHeatFractionofPowerInput,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve CoilHeatingDXMultiSpeedStageData_Impl::wasteHeatFunctionofTemperatureCurve() const {
    boost::optional<Curve> value = optionalWasteHeatFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Waste Heat Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setGrossRatedHeatingCapacity(boost::optional<double> grossRatedHeatingCapacity) {
    bool result(false);
    if (grossRatedHeatingCapacity) {
      result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedHeatingCapacity, grossRatedHeatingCapacity.get());
    }
    return result;
  }

  void CoilHeatingDXMultiSpeedStageData_Impl::autosizeGrossRatedHeatingCapacity() {
    bool result = setString(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::GrossRatedHeatingCapacity, "autosize");
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

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatedSupplyAirFanPowerPerVolumeFlowRate(double ratedSupplyAirFanPowerPerVolumeFlowRate) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedSupplyAirFanPowerPerVolumeFlowRate, ratedSupplyAirFanPowerPerVolumeFlowRate);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::HeatingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::HeatingCapacityFunctionofFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurveName, curve.handle());
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
    bool result = setDouble(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::RatedWasteHeatFractionofPowerInput, ratedWasteHeatFractionofPowerInput);
    return result;
  }

  bool CoilHeatingDXMultiSpeedStageData_Impl::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeedStageData_Impl::optionalHeatingCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::HeatingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeedStageData_Impl::optionalHeatingCapacityFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::HeatingCapacityFunctionofFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeedStageData_Impl::optionalEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeedStageData_Impl::optionalEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::EnergyInputRatioFunctionofFlowFractionCurveName);
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeedStageData_Impl::optionalPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::PartLoadFractionCorrelationCurveName);
  }

  boost::optional<Curve> CoilHeatingDXMultiSpeedStageData_Impl::optionalWasteHeatFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Heating_DX_MultiSpeed_StageDataFields::WasteHeatFunctionofTemperatureCurveName);
  }

  std::vector<ModelObject> CoilHeatingDXMultiSpeedStageData_Impl::children() const {
    std::vector<ModelObject> result;

    result.push_back(heatingCapacityFunctionofTemperatureCurve());
    result.push_back(heatingCapacityFunctionofFlowFractionCurve());
    result.push_back(energyInputRatioFunctionofTemperatureCurve());
    result.push_back(energyInputRatioFunctionofFlowFractionCurve());
    result.push_back(partLoadFractionCorrelationCurve());
    result.push_back(wasteHeatFunctionofTemperatureCurve());

    return result;
  }

  ModelObject CoilHeatingDXMultiSpeedStageData_Impl::clone(Model model) const {
    auto t_clone = ModelObject_Impl::clone(model).cast<CoilHeatingDXMultiSpeedStageData>();

    return t_clone;
  }

} // detail

CoilHeatingDXMultiSpeedStageData::CoilHeatingDXMultiSpeedStageData(const Model& model)
  : ParentObject(CoilHeatingDXMultiSpeedStageData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>());

  CurveBiquadratic heating_curve_1(model);
  heating_curve_1.setCoefficient1Constant(0.760523473);
  heating_curve_1.setCoefficient2x(0.007531702);
  heating_curve_1.setCoefficient3xPOW2(-0.000288398);
  heating_curve_1.setCoefficient4y(0.021653033);
  heating_curve_1.setCoefficient5yPOW2(3.29133E-05);
  heating_curve_1.setCoefficient6xTIMESY(0.00014862);
  heating_curve_1.setMinimumValueofx(18.33333);
  heating_curve_1.setMaximumValueofx(23.88889);
  heating_curve_1.setMinimumValueofy(-13.88889);
  heating_curve_1.setMaximumValueofy(19.44444);

  CurveQuadratic heating_curve_2(model);
  heating_curve_2.setCoefficient1Constant(0.752130847);
  heating_curve_2.setCoefficient2x(0.437552514);
  heating_curve_2.setCoefficient3xPOW2(-0.189683361);
  heating_curve_2.setMinimumValueofx(0.7);
  heating_curve_2.setMaximumValueofx(1.1);

  CurveBiquadratic heating_curve_3(model);
  heating_curve_3.setCoefficient1Constant(0.750128109);
  heating_curve_3.setCoefficient2x(0.002298944);
  heating_curve_3.setCoefficient3xPOW2(0.000953927);
  heating_curve_3.setCoefficient4y(-0.008327043);
  heating_curve_3.setCoefficient5yPOW2(0.001016058);
  heating_curve_3.setCoefficient6xTIMESY(-0.001433855);
  heating_curve_3.setMinimumValueofx(18.33333);
  heating_curve_3.setMaximumValueofx(23.88889);
  heating_curve_3.setMinimumValueofy(-13.88889);
  heating_curve_3.setMaximumValueofy(19.44444);

  CurveQuadratic heating_curve_4(model);
  heating_curve_4.setCoefficient1Constant(1.853909301);
  heating_curve_4.setCoefficient2x(-1.411203778);
  heating_curve_4.setCoefficient3xPOW2(0.557294478);
  heating_curve_4.setMinimumValueofx(0.7);
  heating_curve_4.setMaximumValueofx(1.1);

  CurveQuadratic heating_curve_5(model);
  heating_curve_5.setCoefficient1Constant(0.85);
  heating_curve_5.setCoefficient2x(0.15);
  heating_curve_5.setCoefficient3xPOW2(0.0);
  heating_curve_5.setMinimumValueofx(0.0);
  heating_curve_5.setMaximumValueofx(1.0);

  CurveBiquadratic heating_curve_6(model);
  heating_curve_6.setCoefficient1Constant(1.0);
  heating_curve_6.setCoefficient2x(0.0);
  heating_curve_6.setCoefficient3xPOW2(0.0);
  heating_curve_6.setCoefficient4y(0.0);
  heating_curve_6.setCoefficient5yPOW2(0.0);
  heating_curve_6.setCoefficient6xTIMESY(0.0);
  heating_curve_6.setMinimumValueofx(0);
  heating_curve_6.setMaximumValueofx(50);
  heating_curve_6.setMinimumValueofy(0);
  heating_curve_6.setMaximumValueofy(50);

  bool ok = true;
  autosizeGrossRatedHeatingCapacity();
  ok = setGrossRatedHeatingCOP(2.75);
  OS_ASSERT(ok);
  autosizeRatedAirFlowRate();
  ok = setRatedSupplyAirFanPowerPerVolumeFlowRate(773.3);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofTemperatureCurve(heating_curve_1);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofFlowFractionCurve(heating_curve_2);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(heating_curve_3);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofFlowFractionCurve(heating_curve_4);
  OS_ASSERT(ok);
  ok = setPartLoadFractionCorrelationCurve(heating_curve_5);
  OS_ASSERT(ok);
  ok = setRatedWasteHeatFractionofPowerInput(0.2);
  OS_ASSERT(ok);
  ok = setWasteHeatFunctionofTemperatureCurve(heating_curve_6);
  OS_ASSERT(ok);
}

CoilHeatingDXMultiSpeedStageData::CoilHeatingDXMultiSpeedStageData(const Model& model,
  const Curve& heatingCapacityFunctionofTemperature,
  const Curve& heatingCapacityFuncionofFlowFraction,
  const Curve& energyInputRatioFunctionofTemperature,
  const Curve& energyInputRatioFunctionofFlowFraction,
  const Curve& partLoadFractionCorrelation,
  const Curve& wasteHeatFunctionofTemperature)
  : ParentObject(CoilHeatingDXMultiSpeedStageData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>());

  bool ok = true;
  autosizeGrossRatedHeatingCapacity();
  ok = setGrossRatedHeatingCOP(2.75);
  OS_ASSERT(ok);
  autosizeRatedAirFlowRate();
  ok = setRatedSupplyAirFanPowerPerVolumeFlowRate(773.3);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofTemperatureCurve(heatingCapacityFunctionofTemperature);
  OS_ASSERT(ok);
  ok = setHeatingCapacityFunctionofFlowFractionCurve(heatingCapacityFuncionofFlowFraction);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofTemperatureCurve(energyInputRatioFunctionofTemperature);
  OS_ASSERT(ok);
  ok = setEnergyInputRatioFunctionofFlowFractionCurve(energyInputRatioFunctionofFlowFraction);
  OS_ASSERT(ok);
  ok = setPartLoadFractionCorrelationCurve(partLoadFractionCorrelation);
  OS_ASSERT(ok);
  ok = setRatedWasteHeatFractionofPowerInput(0.2);
  OS_ASSERT(ok);
  ok = setWasteHeatFunctionofTemperatureCurve(wasteHeatFunctionofTemperature);
  OS_ASSERT(ok);
}

IddObjectType CoilHeatingDXMultiSpeedStageData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_DX_MultiSpeed_StageData);
}

boost::optional<double> CoilHeatingDXMultiSpeedStageData::grossRatedHeatingCapacity() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->grossRatedHeatingCapacity();
}

bool CoilHeatingDXMultiSpeedStageData::isGrossRatedHeatingCapacityAutosized() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->isGrossRatedHeatingCapacityAutosized();
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

double CoilHeatingDXMultiSpeedStageData::ratedSupplyAirFanPowerPerVolumeFlowRate() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratedSupplyAirFanPowerPerVolumeFlowRate();
}

Curve CoilHeatingDXMultiSpeedStageData::heatingCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->heatingCapacityFunctionofTemperatureCurve();
}

Curve CoilHeatingDXMultiSpeedStageData::heatingCapacityFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->heatingCapacityFunctionofFlowFractionCurve();
}

Curve CoilHeatingDXMultiSpeedStageData::energyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->energyInputRatioFunctionofTemperatureCurve();
}

Curve CoilHeatingDXMultiSpeedStageData::energyInputRatioFunctionofFlowFractionCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->energyInputRatioFunctionofFlowFractionCurve();
}

Curve CoilHeatingDXMultiSpeedStageData::partLoadFractionCorrelationCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->partLoadFractionCorrelationCurve();
}

double CoilHeatingDXMultiSpeedStageData::ratedWasteHeatFractionofPowerInput() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->ratedWasteHeatFractionofPowerInput();
}

Curve CoilHeatingDXMultiSpeedStageData::wasteHeatFunctionofTemperatureCurve() const {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->wasteHeatFunctionofTemperatureCurve();
}

bool CoilHeatingDXMultiSpeedStageData::setGrossRatedHeatingCapacity(double grossRatedHeatingCapacity) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setGrossRatedHeatingCapacity(grossRatedHeatingCapacity);
}

void CoilHeatingDXMultiSpeedStageData::autosizeGrossRatedHeatingCapacity() {
  getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->autosizeGrossRatedHeatingCapacity();
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

bool CoilHeatingDXMultiSpeedStageData::setRatedSupplyAirFanPowerPerVolumeFlowRate(double ratedSupplyAirFanPowerPerVolumeFlowRate) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatedSupplyAirFanPowerPerVolumeFlowRate(ratedSupplyAirFanPowerPerVolumeFlowRate);
}

bool CoilHeatingDXMultiSpeedStageData::setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setHeatingCapacityFunctionofTemperatureCurve(curve);
}

bool CoilHeatingDXMultiSpeedStageData::setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setHeatingCapacityFunctionofFlowFractionCurve(curve);
}

bool CoilHeatingDXMultiSpeedStageData::setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setEnergyInputRatioFunctionofTemperatureCurve(curve);
}

bool CoilHeatingDXMultiSpeedStageData::setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setEnergyInputRatioFunctionofFlowFractionCurve(curve);
}

bool CoilHeatingDXMultiSpeedStageData::setPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

bool CoilHeatingDXMultiSpeedStageData::setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setRatedWasteHeatFractionofPowerInput(ratedWasteHeatFractionofPowerInput);
}

bool CoilHeatingDXMultiSpeedStageData::setWasteHeatFunctionofTemperatureCurve(const Curve& curve) {
  return getImpl<detail::CoilHeatingDXMultiSpeedStageData_Impl>()->setWasteHeatFunctionofTemperatureCurve(curve);
}

/// @cond
CoilHeatingDXMultiSpeedStageData::CoilHeatingDXMultiSpeedStageData(std::shared_ptr<detail::CoilHeatingDXMultiSpeedStageData_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

