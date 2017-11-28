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

#include "ChillerAbsorption.hpp"
#include "ChillerAbsorption_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Chiller_Absorption_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ChillerAbsorption_Impl::ChillerAbsorption_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ChillerAbsorption::iddObjectType());
  }

  ChillerAbsorption_Impl::ChillerAbsorption_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ChillerAbsorption::iddObjectType());
  }

  ChillerAbsorption_Impl::ChillerAbsorption_Impl(const ChillerAbsorption_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ChillerAbsorption_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ChillerAbsorption_Impl::iddObjectType() const {
    return ChillerAbsorption::iddObjectType();
  }

  boost::optional<double> ChillerAbsorption_Impl::nominalCapacity() const {
    return getDouble(OS_Chiller_AbsorptionFields::NominalCapacity,true);
  }

  bool ChillerAbsorption_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ChillerAbsorption_Impl::nominalPumpingPower() const {
    return getDouble(OS_Chiller_AbsorptionFields::NominalPumpingPower,true);
  }

  bool ChillerAbsorption_Impl::isNominalPumpingPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::NominalPumpingPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerAbsorption_Impl::minimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::MinimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::MaximumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::optimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::OptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::designCondenserInletTemperature() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::DesignCondenserInletTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ChillerAbsorption_Impl::designChilledWaterFlowRate() const {
    return getDouble(OS_Chiller_AbsorptionFields::DesignChilledWaterFlowRate,true);
  }

  bool ChillerAbsorption_Impl::isDesignChilledWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::DesignChilledWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ChillerAbsorption_Impl::designCondenserWaterFlowRate() const {
    return getDouble(OS_Chiller_AbsorptionFields::DesignCondenserWaterFlowRate,true);
  }

  bool ChillerAbsorption_Impl::isDesignCondenserWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::DesignCondenserWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerAbsorption_Impl::coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::Coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::Coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::Coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::coefficient1ofthePumpElectricUsePartLoadRatioCurve() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::Coefficient1ofthePumpElectricUsePartLoadRatioCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::coefficient2ofthePumpElectricUsePartLoadRatioCurve() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::Coefficient2ofthePumpElectricUsePartLoadRatioCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::coefficient3ofthePumpElectricUsePartLoadRatioCurve() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::Coefficient3ofthePumpElectricUsePartLoadRatioCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::chilledWaterOutletTemperatureLowerLimit() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::ChilledWaterOutletTemperatureLowerLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerAbsorption_Impl::chillerFlowMode() const {
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::ChillerFlowMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerAbsorption_Impl::generatorHeatSourceType() const {
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::GeneratorHeatSourceType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ChillerAbsorption_Impl::designGeneratorFluidFlowRate() const {
    return getDouble(OS_Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate,true);
  }

  bool ChillerAbsorption_Impl::isDesignGeneratorFluidFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerAbsorption_Impl::degreeofSubcoolinginSteamGenerator() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::DegreeofSubcoolinginSteamGenerator,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorption_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_Chiller_AbsorptionFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerAbsorption_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_Chiller_AbsorptionFields::NominalCapacity, nominalCapacity.get());
    }
    return result;
  }

  void ChillerAbsorption_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Chiller_AbsorptionFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorption_Impl::setNominalPumpingPower(boost::optional<double> nominalPumpingPower) {
    bool result(false);
    if (nominalPumpingPower) {
      result = setDouble(OS_Chiller_AbsorptionFields::NominalPumpingPower, nominalPumpingPower.get());
    }
    return result;
  }

  void ChillerAbsorption_Impl::autosizeNominalPumpingPower() {
    bool result = setString(OS_Chiller_AbsorptionFields::NominalPumpingPower, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorption_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    return result;
  }

  bool ChillerAbsorption_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    return result;
  }

  bool ChillerAbsorption_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::OptimumPartLoadRatio, optimumPartLoadRatio);
    return result;
  }

  void ChillerAbsorption_Impl::setDesignCondenserInletTemperature(double designCondenserInletTemperature) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::DesignCondenserInletTemperature, designCondenserInletTemperature);
    OS_ASSERT(result);
  }

  bool ChillerAbsorption_Impl::setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate) {
    bool result(false);
    if (designChilledWaterFlowRate) {
      result = setDouble(OS_Chiller_AbsorptionFields::DesignChilledWaterFlowRate, designChilledWaterFlowRate.get());
    }
    return result;
  }

  void ChillerAbsorption_Impl::autosizeDesignChilledWaterFlowRate() {
    bool result = setString(OS_Chiller_AbsorptionFields::DesignChilledWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorption_Impl::setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate) {
    bool result(false);
    if (designCondenserWaterFlowRate) {
      result = setDouble(OS_Chiller_AbsorptionFields::DesignCondenserWaterFlowRate, designCondenserWaterFlowRate.get());
    }
    return result;
  }

  void ChillerAbsorption_Impl::autosizeDesignCondenserWaterFlowRate() {
    bool result = setString(OS_Chiller_AbsorptionFields::DesignCondenserWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::Coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve, coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve);
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::Coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve, coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve);
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::Coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve, coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve);
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(double coefficient1ofthePumpElectricUsePartLoadRatioCurve) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::Coefficient1ofthePumpElectricUsePartLoadRatioCurve, coefficient1ofthePumpElectricUsePartLoadRatioCurve);
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(double coefficient2ofthePumpElectricUsePartLoadRatioCurve) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::Coefficient2ofthePumpElectricUsePartLoadRatioCurve, coefficient2ofthePumpElectricUsePartLoadRatioCurve);
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(double coefficient3ofthePumpElectricUsePartLoadRatioCurve) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::Coefficient3ofthePumpElectricUsePartLoadRatioCurve, coefficient3ofthePumpElectricUsePartLoadRatioCurve);
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::ChilledWaterOutletTemperatureLowerLimit, chilledWaterOutletTemperatureLowerLimit);
    OS_ASSERT(result);
  }

  bool ChillerAbsorption_Impl::setChillerFlowMode(std::string chillerFlowMode) {
    bool result = setString(OS_Chiller_AbsorptionFields::ChillerFlowMode, chillerFlowMode);
    return result;
  }

  bool ChillerAbsorption_Impl::setGeneratorHeatSourceType(std::string generatorHeatSourceType) {
    bool result = setString(OS_Chiller_AbsorptionFields::GeneratorHeatSourceType, generatorHeatSourceType);
    return result;
  }

  bool ChillerAbsorption_Impl::setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate) {
    bool result(false);
    if (designGeneratorFluidFlowRate) {
      result = setDouble(OS_Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate, designGeneratorFluidFlowRate.get());
    }
    return result;
  }

  void ChillerAbsorption_Impl::autosizeDesignGeneratorFluidFlowRate() {
    bool result = setString(OS_Chiller_AbsorptionFields::DesignGeneratorFluidFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void ChillerAbsorption_Impl::setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::DegreeofSubcoolinginSteamGenerator, degreeofSubcoolinginSteamGenerator);
    OS_ASSERT(result);
  }

  bool ChillerAbsorption_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_Chiller_AbsorptionFields::SizingFactor, sizingFactor);
    return result;
  }

  unsigned ChillerAbsorption_Impl::supplyInletPort() {
    return OS_Chiller_AbsorptionFields::ChilledWaterInletNodeName;
  }

  unsigned ChillerAbsorption_Impl::supplyOutletPort() {
    return OS_Chiller_AbsorptionFields::ChilledWaterOutletNodeName;
  }

  unsigned ChillerAbsorption_Impl::demandInletPort() {
    return OS_Chiller_AbsorptionFields::CondenserInletNodeName;
  }

  unsigned ChillerAbsorption_Impl::demandOutletPort() {
    return OS_Chiller_AbsorptionFields::CondenserOutletNodeName;
  }

} // detail

ChillerAbsorption::ChillerAbsorption(const Model& model)
  : WaterToWaterComponent(ChillerAbsorption::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ChillerAbsorption_Impl>());
  autosizeNominalCapacity();
  autosizeNominalPumpingPower();
  setMinimumPartLoadRatio(0.15);
  setMaximumPartLoadRatio(1.0);
  setOptimumPartLoadRatio(0.65);
  setDesignCondenserInletTemperature(35.0);
  autosizeDesignChilledWaterFlowRate();
  autosizeDesignCondenserWaterFlowRate();
  setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(0.03303);
  setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(0.6852);
  setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(0.2818);
  setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(1.0);
  setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(0);
  setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(0);
  setChilledWaterOutletTemperatureLowerLimit(5);
  setChillerFlowMode("NotModulated");
  setGeneratorHeatSourceType("Steam");
  autosizeDesignGeneratorFluidFlowRate();
  setDegreeofSubcoolinginSteamGenerator(1.0);
  setSizingFactor(1.0);
}

IddObjectType ChillerAbsorption::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Chiller_Absorption);
}

std::vector<std::string> ChillerAbsorption::chillerFlowModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Chiller_AbsorptionFields::ChillerFlowMode);
}

std::vector<std::string> ChillerAbsorption::generatorHeatSourceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Chiller_AbsorptionFields::GeneratorHeatSourceType);
}

boost::optional<double> ChillerAbsorption::nominalCapacity() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->nominalCapacity();
}

bool ChillerAbsorption::isNominalCapacityAutosized() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->isNominalCapacityAutosized();
}

boost::optional<double> ChillerAbsorption::nominalPumpingPower() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->nominalPumpingPower();
}

bool ChillerAbsorption::isNominalPumpingPowerAutosized() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->isNominalPumpingPowerAutosized();
}

double ChillerAbsorption::minimumPartLoadRatio() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->minimumPartLoadRatio();
}

double ChillerAbsorption::maximumPartLoadRatio() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->maximumPartLoadRatio();
}

double ChillerAbsorption::optimumPartLoadRatio() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->optimumPartLoadRatio();
}

double ChillerAbsorption::designCondenserInletTemperature() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->designCondenserInletTemperature();
}

boost::optional<double> ChillerAbsorption::designChilledWaterFlowRate() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->designChilledWaterFlowRate();
}

bool ChillerAbsorption::isDesignChilledWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->isDesignChilledWaterFlowRateAutosized();
}

boost::optional<double> ChillerAbsorption::designCondenserWaterFlowRate() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->designCondenserWaterFlowRate();
}

bool ChillerAbsorption::isDesignCondenserWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->isDesignCondenserWaterFlowRateAutosized();
}

double ChillerAbsorption::coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve();
}

double ChillerAbsorption::coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve();
}

double ChillerAbsorption::coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve();
}

double ChillerAbsorption::coefficient1ofthePumpElectricUsePartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->coefficient1ofthePumpElectricUsePartLoadRatioCurve();
}

double ChillerAbsorption::coefficient2ofthePumpElectricUsePartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->coefficient2ofthePumpElectricUsePartLoadRatioCurve();
}

double ChillerAbsorption::coefficient3ofthePumpElectricUsePartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->coefficient3ofthePumpElectricUsePartLoadRatioCurve();
}

double ChillerAbsorption::chilledWaterOutletTemperatureLowerLimit() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->chilledWaterOutletTemperatureLowerLimit();
}

std::string ChillerAbsorption::chillerFlowMode() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->chillerFlowMode();
}

std::string ChillerAbsorption::generatorHeatSourceType() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->generatorHeatSourceType();
}

boost::optional<double> ChillerAbsorption::designGeneratorFluidFlowRate() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->designGeneratorFluidFlowRate();
}

bool ChillerAbsorption::isDesignGeneratorFluidFlowRateAutosized() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->isDesignGeneratorFluidFlowRateAutosized();
}

double ChillerAbsorption::degreeofSubcoolinginSteamGenerator() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->degreeofSubcoolinginSteamGenerator();
}

double ChillerAbsorption::sizingFactor() const {
  return getImpl<detail::ChillerAbsorption_Impl>()->sizingFactor();
}

bool ChillerAbsorption::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setNominalCapacity(nominalCapacity);
}

void ChillerAbsorption::autosizeNominalCapacity() {
  getImpl<detail::ChillerAbsorption_Impl>()->autosizeNominalCapacity();
}

bool ChillerAbsorption::setNominalPumpingPower(double nominalPumpingPower) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setNominalPumpingPower(nominalPumpingPower);
}

void ChillerAbsorption::autosizeNominalPumpingPower() {
  getImpl<detail::ChillerAbsorption_Impl>()->autosizeNominalPumpingPower();
}

bool ChillerAbsorption::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

bool ChillerAbsorption::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

bool ChillerAbsorption::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
}

void ChillerAbsorption::setDesignCondenserInletTemperature(double designCondenserInletTemperature) {
  getImpl<detail::ChillerAbsorption_Impl>()->setDesignCondenserInletTemperature(designCondenserInletTemperature);
}

bool ChillerAbsorption::setDesignChilledWaterFlowRate(double designChilledWaterFlowRate) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setDesignChilledWaterFlowRate(designChilledWaterFlowRate);
}

void ChillerAbsorption::autosizeDesignChilledWaterFlowRate() {
  getImpl<detail::ChillerAbsorption_Impl>()->autosizeDesignChilledWaterFlowRate();
}

bool ChillerAbsorption::setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setDesignCondenserWaterFlowRate(designCondenserWaterFlowRate);
}

void ChillerAbsorption::autosizeDesignCondenserWaterFlowRate() {
  getImpl<detail::ChillerAbsorption_Impl>()->autosizeDesignCondenserWaterFlowRate();
}

void ChillerAbsorption::setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve) {
  getImpl<detail::ChillerAbsorption_Impl>()->setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve);
}

void ChillerAbsorption::setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve) {
  getImpl<detail::ChillerAbsorption_Impl>()->setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve);
}

void ChillerAbsorption::setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve) {
  getImpl<detail::ChillerAbsorption_Impl>()->setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve);
}

void ChillerAbsorption::setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(double coefficient1ofthePumpElectricUsePartLoadRatioCurve) {
  getImpl<detail::ChillerAbsorption_Impl>()->setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(coefficient1ofthePumpElectricUsePartLoadRatioCurve);
}

void ChillerAbsorption::setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(double coefficient2ofthePumpElectricUsePartLoadRatioCurve) {
  getImpl<detail::ChillerAbsorption_Impl>()->setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(coefficient2ofthePumpElectricUsePartLoadRatioCurve);
}

void ChillerAbsorption::setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(double coefficient3ofthePumpElectricUsePartLoadRatioCurve) {
  getImpl<detail::ChillerAbsorption_Impl>()->setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(coefficient3ofthePumpElectricUsePartLoadRatioCurve);
}

void ChillerAbsorption::setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit) {
  getImpl<detail::ChillerAbsorption_Impl>()->setChilledWaterOutletTemperatureLowerLimit(chilledWaterOutletTemperatureLowerLimit);
}

bool ChillerAbsorption::setChillerFlowMode(std::string chillerFlowMode) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setChillerFlowMode(chillerFlowMode);
}

bool ChillerAbsorption::setGeneratorHeatSourceType(std::string generatorHeatSourceType) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setGeneratorHeatSourceType(generatorHeatSourceType);
}

bool ChillerAbsorption::setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setDesignGeneratorFluidFlowRate(designGeneratorFluidFlowRate);
}

void ChillerAbsorption::autosizeDesignGeneratorFluidFlowRate() {
  getImpl<detail::ChillerAbsorption_Impl>()->autosizeDesignGeneratorFluidFlowRate();
}

void ChillerAbsorption::setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator) {
  getImpl<detail::ChillerAbsorption_Impl>()->setDegreeofSubcoolinginSteamGenerator(degreeofSubcoolinginSteamGenerator);
}

bool ChillerAbsorption::setSizingFactor(double sizingFactor) {
  return getImpl<detail::ChillerAbsorption_Impl>()->setSizingFactor(sizingFactor);
}

/// @cond
ChillerAbsorption::ChillerAbsorption(std::shared_ptr<detail::ChillerAbsorption_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

