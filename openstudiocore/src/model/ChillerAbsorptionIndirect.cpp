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

#include "ChillerAbsorptionIndirect.hpp"
#include "ChillerAbsorptionIndirect_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Chiller_Absorption_Indirect_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ChillerAbsorptionIndirect_Impl::ChillerAbsorptionIndirect_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ChillerAbsorptionIndirect::iddObjectType());
  }

  ChillerAbsorptionIndirect_Impl::ChillerAbsorptionIndirect_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ChillerAbsorptionIndirect::iddObjectType());
  }

  ChillerAbsorptionIndirect_Impl::ChillerAbsorptionIndirect_Impl(const ChillerAbsorptionIndirect_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ChillerAbsorptionIndirect_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ChillerAbsorptionIndirect_Impl::iddObjectType() const {
    return ChillerAbsorptionIndirect::iddObjectType();
  }

  boost::optional<double> ChillerAbsorptionIndirect_Impl::nominalCapacity() const {
    return getDouble(OS_Chiller_Absorption_IndirectFields::NominalCapacity,true);
  }

  bool ChillerAbsorptionIndirect_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ChillerAbsorptionIndirect_Impl::nominalPumpingPower() const {
    return getDouble(OS_Chiller_Absorption_IndirectFields::NominalPumpingPower,true);
  }

  bool ChillerAbsorptionIndirect_Impl::isNominalPumpingPowerAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::NominalPumpingPower, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerAbsorptionIndirect_Impl::minimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::MinimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::MaximumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::optimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::OptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::designCondenserInletTemperature() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserInletTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::condenserInletTemperatureLowerLimit() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::CondenserInletTemperatureLowerLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::chilledWaterOutletTemperatureLowerLimit() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::ChilledWaterOutletTemperatureLowerLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ChillerAbsorptionIndirect_Impl::designChilledWaterFlowRate() const {
    return getDouble(OS_Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate,true);
  }

  bool ChillerAbsorptionIndirect_Impl::isDesignChilledWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ChillerAbsorptionIndirect_Impl::designCondenserWaterFlowRate() const {
    return getDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate,true);
  }

  bool ChillerAbsorptionIndirect_Impl::isDesignCondenserWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string ChillerAbsorptionIndirect_Impl::chillerFlowMode() const {
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::ChillerFlowMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::generatorHeatInputFunctionofPartLoadRatioCurve() const {
    boost::optional<Curve> value = optionalGeneratorHeatInputFunctionofPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Generator Heat Input Functionof Part Load Ratio Curve attached.");
    }
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::pumpElectricInputFunctionofPartLoadRatioCurve() const {
    boost::optional<Curve> value = optionalPumpElectricInputFunctionofPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Pump Electric Input Functionof Part Load Ratio Curve attached.");
    }
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::capacityCorrectionFunctionofCondenserTemperatureCurve() const {
    boost::optional<Curve> value = optionalCapacityCorrectionFunctionofCondenserTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Capacity Correction Functionof Condenser Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::capacityCorrectionFunctionofChilledWaterTemperatureCurve() const {
    boost::optional<Curve> value = optionalCapacityCorrectionFunctionofChilledWaterTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Capacity Correction Functionof Chilled Water Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::capacityCorrectionFunctionofGeneratorTemperatureCurve() const {
    boost::optional<Curve> value = optionalCapacityCorrectionFunctionofGeneratorTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Capacity Correction Functionof Generator Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const {
    boost::optional<Curve> value = optionalGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Generator Heat Input Correction Functionof Condenser Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerAbsorptionIndirect_Impl::generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const {
    boost::optional<Curve> value = optionalGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Generator Heat Input Correction Functionof Chilled Water Temperature Curve attached.");
    }
    return value.get();
  }

  std::string ChillerAbsorptionIndirect_Impl::generatorHeatSourceType() const {
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::GeneratorHeatSourceType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ChillerAbsorptionIndirect_Impl::designGeneratorFluidFlowRate() const {
    return getDouble(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate,true);
  }

  bool ChillerAbsorptionIndirect_Impl::isDesignGeneratorFluidFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerAbsorptionIndirect_Impl::temperatureLowerLimitGeneratorInlet() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::TemperatureLowerLimitGeneratorInlet,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::degreeofSubcoolinginSteamGenerator() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamGenerator,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::degreeofSubcoolinginSteamCondensateLoop() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamCondensateLoop,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerAbsorptionIndirect_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerAbsorptionIndirect_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_Chiller_Absorption_IndirectFields::NominalCapacity, nominalCapacity.get());
    }
    return result;
  }

  void ChillerAbsorptionIndirect_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorptionIndirect_Impl::setNominalPumpingPower(boost::optional<double> nominalPumpingPower) {
    bool result(false);
    if (nominalPumpingPower) {
      result = setDouble(OS_Chiller_Absorption_IndirectFields::NominalPumpingPower, nominalPumpingPower.get());
    }
    return result;
  }

  void ChillerAbsorptionIndirect_Impl::autosizeNominalPumpingPower() {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::NominalPumpingPower, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorptionIndirect_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::OptimumPartLoadRatio, optimumPartLoadRatio);
    return result;
  }

  void ChillerAbsorptionIndirect_Impl::setDesignCondenserInletTemperature(double designCondenserInletTemperature) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserInletTemperature, designCondenserInletTemperature);
    OS_ASSERT(result);
  }

  void ChillerAbsorptionIndirect_Impl::setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::CondenserInletTemperatureLowerLimit, condenserInletTemperatureLowerLimit);
    OS_ASSERT(result);
  }

  void ChillerAbsorptionIndirect_Impl::setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::ChilledWaterOutletTemperatureLowerLimit, chilledWaterOutletTemperatureLowerLimit);
    OS_ASSERT(result);
  }

  bool ChillerAbsorptionIndirect_Impl::setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate) {
    bool result(false);
    if (designChilledWaterFlowRate) {
      result = setDouble(OS_Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate, designChilledWaterFlowRate.get());
    }
    return result;
  }

  void ChillerAbsorptionIndirect_Impl::autosizeDesignChilledWaterFlowRate() {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorptionIndirect_Impl::setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate) {
    bool result(false);
    if (designCondenserWaterFlowRate) {
      result = setDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate, designCondenserWaterFlowRate.get());
    }
    return result;
  }

  void ChillerAbsorptionIndirect_Impl::autosizeDesignCondenserWaterFlowRate() {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerAbsorptionIndirect_Impl::setChillerFlowMode(std::string chillerFlowMode) {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::ChillerFlowMode, chillerFlowMode);
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setGeneratorHeatInputFunctionofPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputFunctionofPartLoadRatioCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setPumpElectricInputFunctionofPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::PumpElectricInputFunctionofPartLoadRatioCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setCapacityCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofCondenserTemperatureCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofChilledWaterTemperatureCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setCapacityCorrectionFunctionofGeneratorTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofGeneratorTemperatureCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve, curve.handle());
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setGeneratorHeatSourceType(std::string generatorHeatSourceType) {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::GeneratorHeatSourceType, generatorHeatSourceType);
    return result;
  }

  void ChillerAbsorptionIndirect_Impl::setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate) {
    bool result(false);
    if (designGeneratorFluidFlowRate) {
      result = setDouble(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate, designGeneratorFluidFlowRate.get());
    }
    OS_ASSERT(result);
  }

  void ChillerAbsorptionIndirect_Impl::autosizeDesignGeneratorFluidFlowRate() {
    bool result = setString(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void ChillerAbsorptionIndirect_Impl::setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::TemperatureLowerLimitGeneratorInlet, temperatureLowerLimitGeneratorInlet);
    OS_ASSERT(result);
  }

  bool ChillerAbsorptionIndirect_Impl::setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamGenerator, degreeofSubcoolinginSteamGenerator);
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setDegreeofSubcoolinginSteamCondensateLoop(double degreeofSubcoolinginSteamCondensateLoop) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamCondensateLoop, degreeofSubcoolinginSteamCondensateLoop);
    return result;
  }

  bool ChillerAbsorptionIndirect_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_Chiller_Absorption_IndirectFields::SizingFactor, sizingFactor);
    return result;
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalGeneratorHeatInputFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputFunctionofPartLoadRatioCurve);
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalPumpElectricInputFunctionofPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::PumpElectricInputFunctionofPartLoadRatioCurve);
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalCapacityCorrectionFunctionofCondenserTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofCondenserTemperatureCurve);
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalCapacityCorrectionFunctionofChilledWaterTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofChilledWaterTemperatureCurve);
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalCapacityCorrectionFunctionofGeneratorTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofGeneratorTemperatureCurve);
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve);
  }

  boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve);
  }

  unsigned ChillerAbsorptionIndirect_Impl::supplyInletPort() {
    return OS_Chiller_Absorption_IndirectFields::ChilledWaterInletNodeName;
  }

  unsigned ChillerAbsorptionIndirect_Impl::supplyOutletPort() {
    return OS_Chiller_Absorption_IndirectFields::ChilledWaterOutletNodeName;
  }

  unsigned ChillerAbsorptionIndirect_Impl::demandInletPort() {
    return OS_Chiller_Absorption_IndirectFields::CondenserInletNodeName;
  }

  unsigned ChillerAbsorptionIndirect_Impl::demandOutletPort() {
    return OS_Chiller_Absorption_IndirectFields::CondenserOutletNodeName;
  }

  ModelObject ChillerAbsorptionIndirect_Impl::clone(Model model) const
  {
    auto newMo = WaterToWaterComponent_Impl::clone(model).cast<ChillerAbsorptionIndirect>();

    return newMo;
  }

  std::vector<ModelObject> ChillerAbsorptionIndirect_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(generatorHeatInputFunctionofPartLoadRatioCurve());
    result.push_back(pumpElectricInputFunctionofPartLoadRatioCurve());
    result.push_back(capacityCorrectionFunctionofCondenserTemperatureCurve());
    result.push_back(capacityCorrectionFunctionofChilledWaterTemperatureCurve());
    result.push_back(capacityCorrectionFunctionofGeneratorTemperatureCurve());
    result.push_back(generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve());
    result.push_back(generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve());

    return result;
  }

  unsigned ChillerAbsorptionIndirect_Impl::tertiaryInletPort() const {
    return OS_Chiller_Absorption_IndirectFields::GeneratorInletNode;
  }

  unsigned ChillerAbsorptionIndirect_Impl::tertiaryOutletPort() const {
    return OS_Chiller_Absorption_IndirectFields::GeneratorOutletNode;
  }

} // detail

ChillerAbsorptionIndirect::ChillerAbsorptionIndirect(const Model& model)
  : WaterToWaterComponent(ChillerAbsorptionIndirect::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ChillerAbsorptionIndirect_Impl>());

  autosizeNominalCapacity();
  autosizeNominalPumpingPower();
  setMinimumPartLoadRatio(0.15);
  setMaximumPartLoadRatio(1.0);
  setOptimumPartLoadRatio(0.65);
  setDesignCondenserInletTemperature(35.0);
  setCondenserInletTemperatureLowerLimit(10.0);
  setChilledWaterOutletTemperatureLowerLimit(5.0);
  autosizeDesignChilledWaterFlowRate();
  autosizeDesignCondenserWaterFlowRate();
  setChillerFlowMode("NotModulated");
  setGeneratorHeatSourceType("Steam");
  autosizeDesignGeneratorFluidFlowRate();
  setTemperatureLowerLimitGeneratorInlet(30.0);
  setDegreeofSubcoolinginSteamGenerator(2.0);
  setDegreeofSubcoolinginSteamCondensateLoop(12.0);
  setSizingFactor(1.0);

  {
    CurveCubic curve(model);
    curve.setCoefficient1Constant(0.18892);
    curve.setCoefficient2x(0.968044);
    curve.setCoefficient3xPOW2(1.119202);
    curve.setCoefficient4xPOW3(-0.5034);
    curve.setMinimumValueofx(0.15);
    curve.setMaximumValueofx(1.0);
    setGeneratorHeatInputFunctionofPartLoadRatioCurve(curve);
  }

  {
    CurveQuadratic curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setMinimumValueofx(0.15);
    curve.setMaximumValueofx(1.0);
    setPumpElectricInputFunctionofPartLoadRatioCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setCoefficient1Constant(0.245507);
    curve.setCoefficient2x(0.023614);
    curve.setCoefficient3xPOW2(0.0000278);
    curve.setCoefficient4xPOW3(0.000013);
    curve.setMinimumValueofx(7.0);
    curve.setMaximumValueofx(25.0);
    setCapacityCorrectionFunctionofCondenserTemperatureCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setCoefficient1Constant(1.0);
    curve.setCoefficient2x(0.0);
    curve.setCoefficient3xPOW2(0.0);
    curve.setCoefficient4xPOW3(0.0);
    curve.setMinimumValueofx(0.0);
    curve.setMaximumValueofx(100.0);
    setCapacityCorrectionFunctionofGeneratorTemperatureCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setCoefficient1Constant(0.690571);
    curve.setCoefficient2x(0.065571);
    curve.setCoefficient3xPOW2(-0.00289);
    curve.setCoefficient4xPOW3(0.0);
    curve.setMinimumValueofx(4.0);
    curve.setMaximumValueofx(10.0);
    setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setCoefficient1Constant(0.712019);
    curve.setCoefficient2x(-0.00478);
    curve.setCoefficient3xPOW2(0.000864);
    curve.setCoefficient4xPOW3(-0.000013);
    curve.setMinimumValueofx(7.0);
    curve.setMaximumValueofx(30.0);
    setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(curve);
  }

  {
    CurveCubic curve(model);
    curve.setCoefficient1Constant(0.995571);
    curve.setCoefficient2x(0.046821);
    curve.setCoefficient3xPOW2(-0.01099);
    curve.setCoefficient4xPOW3(0.000608);
    curve.setMinimumValueofx(4.0);
    curve.setMaximumValueofx(10.0);
    setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(curve);
  }
}

IddObjectType ChillerAbsorptionIndirect::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Chiller_Absorption_Indirect);
}

std::vector<std::string> ChillerAbsorptionIndirect::chillerFlowModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Chiller_Absorption_IndirectFields::ChillerFlowMode);
}

std::vector<std::string> ChillerAbsorptionIndirect::generatorHeatSourceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Chiller_Absorption_IndirectFields::GeneratorHeatSourceType);
}

boost::optional<double> ChillerAbsorptionIndirect::nominalCapacity() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->nominalCapacity();
}

bool ChillerAbsorptionIndirect::isNominalCapacityAutosized() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->isNominalCapacityAutosized();
}

boost::optional<double> ChillerAbsorptionIndirect::nominalPumpingPower() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->nominalPumpingPower();
}

bool ChillerAbsorptionIndirect::isNominalPumpingPowerAutosized() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->isNominalPumpingPowerAutosized();
}

double ChillerAbsorptionIndirect::minimumPartLoadRatio() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->minimumPartLoadRatio();
}

double ChillerAbsorptionIndirect::maximumPartLoadRatio() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->maximumPartLoadRatio();
}

double ChillerAbsorptionIndirect::optimumPartLoadRatio() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->optimumPartLoadRatio();
}

double ChillerAbsorptionIndirect::designCondenserInletTemperature() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->designCondenserInletTemperature();
}

double ChillerAbsorptionIndirect::condenserInletTemperatureLowerLimit() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->condenserInletTemperatureLowerLimit();
}

double ChillerAbsorptionIndirect::chilledWaterOutletTemperatureLowerLimit() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->chilledWaterOutletTemperatureLowerLimit();
}

boost::optional<double> ChillerAbsorptionIndirect::designChilledWaterFlowRate() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->designChilledWaterFlowRate();
}

bool ChillerAbsorptionIndirect::isDesignChilledWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->isDesignChilledWaterFlowRateAutosized();
}

boost::optional<double> ChillerAbsorptionIndirect::designCondenserWaterFlowRate() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->designCondenserWaterFlowRate();
}

bool ChillerAbsorptionIndirect::isDesignCondenserWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->isDesignCondenserWaterFlowRateAutosized();
}

std::string ChillerAbsorptionIndirect::chillerFlowMode() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->chillerFlowMode();
}

Curve ChillerAbsorptionIndirect::generatorHeatInputFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->generatorHeatInputFunctionofPartLoadRatioCurve();
}

Curve ChillerAbsorptionIndirect::pumpElectricInputFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->pumpElectricInputFunctionofPartLoadRatioCurve();
}

Curve ChillerAbsorptionIndirect::capacityCorrectionFunctionofCondenserTemperatureCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->capacityCorrectionFunctionofCondenserTemperatureCurve();
}

Curve ChillerAbsorptionIndirect::capacityCorrectionFunctionofChilledWaterTemperatureCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->capacityCorrectionFunctionofChilledWaterTemperatureCurve();
}

Curve ChillerAbsorptionIndirect::capacityCorrectionFunctionofGeneratorTemperatureCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->capacityCorrectionFunctionofGeneratorTemperatureCurve();
}

Curve ChillerAbsorptionIndirect::generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve();
}

Curve ChillerAbsorptionIndirect::generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve();
}

std::string ChillerAbsorptionIndirect::generatorHeatSourceType() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->generatorHeatSourceType();
}

boost::optional<double> ChillerAbsorptionIndirect::designGeneratorFluidFlowRate() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->designGeneratorFluidFlowRate();
}

bool ChillerAbsorptionIndirect::isDesignGeneratorFluidFlowRateAutosized() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->isDesignGeneratorFluidFlowRateAutosized();
}

double ChillerAbsorptionIndirect::temperatureLowerLimitGeneratorInlet() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->temperatureLowerLimitGeneratorInlet();
}

double ChillerAbsorptionIndirect::degreeofSubcoolinginSteamGenerator() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->degreeofSubcoolinginSteamGenerator();
}

double ChillerAbsorptionIndirect::degreeofSubcoolinginSteamCondensateLoop() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->degreeofSubcoolinginSteamCondensateLoop();
}

double ChillerAbsorptionIndirect::sizingFactor() const {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->sizingFactor();
}

bool ChillerAbsorptionIndirect::setNominalCapacity(double nominalCapacity) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setNominalCapacity(nominalCapacity);
}

void ChillerAbsorptionIndirect::autosizeNominalCapacity() {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizeNominalCapacity();
}

bool ChillerAbsorptionIndirect::setNominalPumpingPower(double nominalPumpingPower) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setNominalPumpingPower(nominalPumpingPower);
}

void ChillerAbsorptionIndirect::autosizeNominalPumpingPower() {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizeNominalPumpingPower();
}

bool ChillerAbsorptionIndirect::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

bool ChillerAbsorptionIndirect::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

bool ChillerAbsorptionIndirect::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
}

void ChillerAbsorptionIndirect::setDesignCondenserInletTemperature(double designCondenserInletTemperature) {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDesignCondenserInletTemperature(designCondenserInletTemperature);
}

void ChillerAbsorptionIndirect::setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit) {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setCondenserInletTemperatureLowerLimit(condenserInletTemperatureLowerLimit);
}

void ChillerAbsorptionIndirect::setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit) {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setChilledWaterOutletTemperatureLowerLimit(chilledWaterOutletTemperatureLowerLimit);
}

bool ChillerAbsorptionIndirect::setDesignChilledWaterFlowRate(double designChilledWaterFlowRate) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDesignChilledWaterFlowRate(designChilledWaterFlowRate);
}

void ChillerAbsorptionIndirect::autosizeDesignChilledWaterFlowRate() {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizeDesignChilledWaterFlowRate();
}

bool ChillerAbsorptionIndirect::setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDesignCondenserWaterFlowRate(designCondenserWaterFlowRate);
}

void ChillerAbsorptionIndirect::autosizeDesignCondenserWaterFlowRate() {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizeDesignCondenserWaterFlowRate();
}

bool ChillerAbsorptionIndirect::setChillerFlowMode(std::string chillerFlowMode) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setChillerFlowMode(chillerFlowMode);
}

bool ChillerAbsorptionIndirect::setGeneratorHeatInputFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setGeneratorHeatInputFunctionofPartLoadRatioCurve(curve);
}

bool ChillerAbsorptionIndirect::setPumpElectricInputFunctionofPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setPumpElectricInputFunctionofPartLoadRatioCurve(curve);
}

bool ChillerAbsorptionIndirect::setCapacityCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setCapacityCorrectionFunctionofCondenserTemperatureCurve(curve);
}

bool ChillerAbsorptionIndirect::setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(curve);
}

bool ChillerAbsorptionIndirect::setCapacityCorrectionFunctionofGeneratorTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setCapacityCorrectionFunctionofGeneratorTemperatureCurve(curve);
}

bool ChillerAbsorptionIndirect::setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(curve);
}

bool ChillerAbsorptionIndirect::setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(curve);
}

bool ChillerAbsorptionIndirect::setGeneratorHeatSourceType(std::string generatorHeatSourceType) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setGeneratorHeatSourceType(generatorHeatSourceType);
}

void ChillerAbsorptionIndirect::setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate) {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDesignGeneratorFluidFlowRate(designGeneratorFluidFlowRate);
}

void ChillerAbsorptionIndirect::autosizeDesignGeneratorFluidFlowRate() {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizeDesignGeneratorFluidFlowRate();
}

void ChillerAbsorptionIndirect::setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet) {
  getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setTemperatureLowerLimitGeneratorInlet(temperatureLowerLimitGeneratorInlet);
}

bool ChillerAbsorptionIndirect::setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDegreeofSubcoolinginSteamGenerator(degreeofSubcoolinginSteamGenerator);
}

bool ChillerAbsorptionIndirect::setDegreeofSubcoolinginSteamCondensateLoop(double degreeofSubcoolinginSteamCondensateLoop) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDegreeofSubcoolinginSteamCondensateLoop(degreeofSubcoolinginSteamCondensateLoop);
}

bool ChillerAbsorptionIndirect::setSizingFactor(double sizingFactor) {
  return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setSizingFactor(sizingFactor);
}

/// @cond
ChillerAbsorptionIndirect::ChillerAbsorptionIndirect(std::shared_ptr<detail::ChillerAbsorptionIndirect_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

