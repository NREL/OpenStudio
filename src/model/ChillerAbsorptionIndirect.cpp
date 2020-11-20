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
#include "Node.hpp"
#include "PlantLoop.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Chiller_Absorption_Indirect_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ChillerAbsorptionIndirect_Impl::ChillerAbsorptionIndirect_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ChillerAbsorptionIndirect::iddObjectType());
    }

    ChillerAbsorptionIndirect_Impl::ChillerAbsorptionIndirect_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ChillerAbsorptionIndirect::iddObjectType());
    }

    ChillerAbsorptionIndirect_Impl::ChillerAbsorptionIndirect_Impl(const ChillerAbsorptionIndirect_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ChillerAbsorptionIndirect_Impl::outputVariableNames() const {
      // static until ModelObject return type is changed for outputvariableNames
      static const std::vector<std::string> result{

        // Common variables
        "Chiller Electricity Rate", "Chiller Electricity Energy", "Chiller Evaporator Cooling Rate", "Chiller Evaporator Cooling Energy",
        "Chiller Evaporator Inlet Temperature", "Chiller Evaporator Outlet Temperature", "Chiller Evaporator Mass Flow Rate",
        "Chiller Condenser Heat Transfer Rate", "Chiller Condenser Heat Transfer Energy", "Chiller Condenser Inlet Temperature",
        "Chiller Condenser Outlet Temperature", "Chiller Condenser Mass Flow Rate", "Chiller COP", "Chiller Part-Load Ratio", "Chiller Cycling Ratio",

        // TODO: add proper tests once the ModelObject return type is changed.
        // Generator = Hot Water
        //if (this->generatorHeatSourceType() == "HotWater")
        //{
        "Chiller Source Hot Water Rate", "Chiller Source Hot Water Energy", "Chiller Hot Water Mass Flow Rate",
        //}
        //
        // Generator = Steam
        // generatorHeatSourceType == 'Steam'
        //if (this->generatorHeatSourceType() == "Steam")
        //{
        "Chiller Source Steam Rate", "Chiller Source Steam Energy", "Chiller Steam Mass Flow Rate", "Chiller Steam Heat Loss Rate"

      };
      return result;
    }

    IddObjectType ChillerAbsorptionIndirect_Impl::iddObjectType() const {
      return ChillerAbsorptionIndirect::iddObjectType();
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::nominalCapacity() const {
      return getDouble(OS_Chiller_Absorption_IndirectFields::NominalCapacity, true);
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
      return getDouble(OS_Chiller_Absorption_IndirectFields::NominalPumpingPower, true);
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
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::maximumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::MaximumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::optimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::OptimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::designCondenserInletTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserInletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::condenserInletTemperatureLowerLimit() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::CondenserInletTemperatureLowerLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::chilledWaterOutletTemperatureLowerLimit() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::ChilledWaterOutletTemperatureLowerLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::designChilledWaterFlowRate() const {
      return getDouble(OS_Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate, true);
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
      return getDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate, true);
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
      boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::ChillerFlowMode, true);
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
      boost::optional<std::string> value = getString(OS_Chiller_Absorption_IndirectFields::GeneratorHeatSourceType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::designGeneratorFluidFlowRate() const {
      return getDouble(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate, true);
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
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::TemperatureLowerLimitGeneratorInlet, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::degreeofSubcoolinginSteamGenerator() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamGenerator, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::degreeofSubcoolinginSteamCondensateLoop() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamCondensateLoop, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerAbsorptionIndirect_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Chiller_Absorption_IndirectFields::SizingFactor, true);
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

    bool ChillerAbsorptionIndirect_Impl::setDesignCondenserInletTemperature(double designCondenserInletTemperature) {
      bool result = setDouble(OS_Chiller_Absorption_IndirectFields::DesignCondenserInletTemperature, designCondenserInletTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool ChillerAbsorptionIndirect_Impl::setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit) {
      bool result = setDouble(OS_Chiller_Absorption_IndirectFields::CondenserInletTemperatureLowerLimit, condenserInletTemperatureLowerLimit);
      OS_ASSERT(result);
      return result;
    }

    bool ChillerAbsorptionIndirect_Impl::setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit) {
      bool result = setDouble(OS_Chiller_Absorption_IndirectFields::ChilledWaterOutletTemperatureLowerLimit, chilledWaterOutletTemperatureLowerLimit);
      OS_ASSERT(result);
      return result;
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
      bool result =
        setPointer(OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve, curve.handle());
      return result;
    }

    bool ChillerAbsorptionIndirect_Impl::setGeneratorHeatSourceType(std::string generatorHeatSourceType) {
      bool ok = false;
      if (istringEqual("Steam", generatorHeatSourceType) && (this->generatorLoop())) {
        // We don't support Steam loops in OS right now
        LOG(Warn, "Cannot set generatorHeatSourceType to 'Steam' as chiller '" << this->name() << "' is connected to a generatorLoop");
      } else {
        ok = setString(OS_Chiller_Absorption_IndirectFields::GeneratorHeatSourceType, generatorHeatSourceType);
      }
      return ok;
    }

    bool ChillerAbsorptionIndirect_Impl::setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate) {
      bool result(false);
      if (designGeneratorFluidFlowRate) {
        result = setDouble(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate, designGeneratorFluidFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void ChillerAbsorptionIndirect_Impl::autosizeDesignGeneratorFluidFlowRate() {
      bool result = setString(OS_Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool ChillerAbsorptionIndirect_Impl::setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet) {
      bool result = setDouble(OS_Chiller_Absorption_IndirectFields::TemperatureLowerLimitGeneratorInlet, temperatureLowerLimitGeneratorInlet);
      OS_ASSERT(result);
      return result;
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
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputFunctionofPartLoadRatioCurve);
    }

    boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalPumpElectricInputFunctionofPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::PumpElectricInputFunctionofPartLoadRatioCurve);
    }

    boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalCapacityCorrectionFunctionofCondenserTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofCondenserTemperatureCurve);
    }

    boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalCapacityCorrectionFunctionofChilledWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofChilledWaterTemperatureCurve);
    }

    boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalCapacityCorrectionFunctionofGeneratorTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofGeneratorTemperatureCurve);
    }

    boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve);
    }

    boost::optional<Curve> ChillerAbsorptionIndirect_Impl::optionalGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve);
    }

    unsigned ChillerAbsorptionIndirect_Impl::supplyInletPort() const {
      return OS_Chiller_Absorption_IndirectFields::ChilledWaterInletNodeName;
    }

    unsigned ChillerAbsorptionIndirect_Impl::supplyOutletPort() const {
      return OS_Chiller_Absorption_IndirectFields::ChilledWaterOutletNodeName;
    }

    unsigned ChillerAbsorptionIndirect_Impl::demandInletPort() const {
      return OS_Chiller_Absorption_IndirectFields::CondenserInletNodeName;
    }

    unsigned ChillerAbsorptionIndirect_Impl::demandOutletPort() const {
      return OS_Chiller_Absorption_IndirectFields::CondenserOutletNodeName;
    }

    ModelObject ChillerAbsorptionIndirect_Impl::clone(Model model) const {
      auto newMo = WaterToWaterComponent_Impl::clone(model).cast<ChillerAbsorptionIndirect>();

      return newMo;
    }

    std::vector<ModelObject> ChillerAbsorptionIndirect_Impl::children() const {
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

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> ChillerAbsorptionIndirect_Impl::chilledWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> ChillerAbsorptionIndirect_Impl::condenserWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    /** Convenience Function to return the Heat Recovery Loop **/
    boost::optional<PlantLoop> ChillerAbsorptionIndirect_Impl::generatorLoop() const {
      return WaterToWaterComponent_Impl::tertiaryPlantLoop();
    }

    bool ChillerAbsorptionIndirect_Impl::addToNode(Node& node) {

      boost::optional<PlantLoop> t_plantLoop = node.plantLoop();

      // If trying to add to a node that is on the demand side of a plant loop
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // If there is already a condenser water Plant Loop
          if (boost::optional<PlantLoop> cndLoop = this->condenserWaterLoop()) {
            // And it's not the same as the node's loop
            if (t_plantLoop.get() != cndLoop.get()) {
              // And if there is no generator loop (tertiary)
              if (!this->generatorLoop().is_initialized()) {
                // Then try to add it to the tertiary one
                LOG(Warn, "Calling addToTertiaryNode to connect it to the tertiary (=Generator Loop) loop for " << briefDescription());
                return this->addToTertiaryNode(node);
              }
            }
          }
        }
      }

      // All other cases, call the base class implementation
      return WaterToWaterComponent_Impl::addToNode(node);
    }

    bool ChillerAbsorptionIndirect_Impl::addToTertiaryNode(Node& node) {
      auto _model = node.model();
      auto t_plantLoop = node.plantLoop();

      // Only accept adding to a node that is on a demand side of a plant loop
      // Since tertiary here = generator loop (heating)
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // Call base class method which accepts both supply and demand
          bool ok = WaterToWaterComponent_Impl::addToTertiaryNode(node);
          if (ok) {
            LOG(Info, "Setting Generator Heat Source Type to 'HotWater' for " << briefDescription());
            this->setGeneratorHeatSourceType("HotWater");
            return true;
          }
        } else {
          LOG(Info,
              "Tertiary Loop (Generator Loop) connections can only be placed on the Demand side (of a Heating Loop), for " << briefDescription());
        }
      }
      return false;
    }

    bool ChillerAbsorptionIndirect_Impl::removeFromTertiaryPlantLoop() {
      // Disconnect the component
      bool ok = WaterToWaterComponent_Impl::removeFromTertiaryPlantLoop();
      if (ok) {
        // Switch the Generator Heat Source Type to "Steam"
        this->setGeneratorHeatSourceType("Steam");
      }
      return ok;
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::autosizedNominalPumpingPower() const {
      return getAutosizedValue("Design Size Nominal Pumping Power", "W");
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::autosizedDesignChilledWaterFlowRate() const {
      return getAutosizedValue("Design Size Design Chilled Water Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::autosizedDesignCondenserWaterFlowRate() const {
      return getAutosizedValue("Design Size Design Condenser Water Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerAbsorptionIndirect_Impl::autosizedDesignGeneratorFluidFlowRate() const {
      return getAutosizedValue("Design Size Design Generator Fluid Flow Rate", "m3/s");
    }

    void ChillerAbsorptionIndirect_Impl::autosize() {
      autosizeNominalCapacity();
      autosizeNominalPumpingPower();
      autosizeDesignChilledWaterFlowRate();
      autosizeDesignCondenserWaterFlowRate();
      autosizeDesignGeneratorFluidFlowRate();
    }

    void ChillerAbsorptionIndirect_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }

      val = autosizedNominalPumpingPower();
      if (val) {
        setNominalPumpingPower(val.get());
      }

      val = autosizedDesignChilledWaterFlowRate();
      if (val) {
        setDesignChilledWaterFlowRate(val.get());
      }

      val = autosizedDesignCondenserWaterFlowRate();
      if (val) {
        setDesignCondenserWaterFlowRate(val.get());
      }

      val = autosizedDesignGeneratorFluidFlowRate();
      if (val) {
        setDesignGeneratorFluidFlowRate(val.get());
      }
    }

  }  // namespace detail

  ChillerAbsorptionIndirect::ChillerAbsorptionIndirect(const Model& model)
    : WaterToWaterComponent(ChillerAbsorptionIndirect::iddObjectType(), model) {
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
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Absorption_IndirectFields::ChillerFlowMode);
  }

  std::vector<std::string> ChillerAbsorptionIndirect::generatorHeatSourceTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Absorption_IndirectFields::GeneratorHeatSourceType);
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

  bool ChillerAbsorptionIndirect::setDesignCondenserInletTemperature(double designCondenserInletTemperature) {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDesignCondenserInletTemperature(designCondenserInletTemperature);
  }

  bool ChillerAbsorptionIndirect::setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit) {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setCondenserInletTemperatureLowerLimit(condenserInletTemperatureLowerLimit);
  }

  bool ChillerAbsorptionIndirect::setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit) {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setChilledWaterOutletTemperatureLowerLimit(chilledWaterOutletTemperatureLowerLimit);
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

  bool ChillerAbsorptionIndirect::setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate) {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setDesignGeneratorFluidFlowRate(designGeneratorFluidFlowRate);
  }

  void ChillerAbsorptionIndirect::autosizeDesignGeneratorFluidFlowRate() {
    getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizeDesignGeneratorFluidFlowRate();
  }

  bool ChillerAbsorptionIndirect::setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet) {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->setTemperatureLowerLimitGeneratorInlet(temperatureLowerLimitGeneratorInlet);
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

  // Convenience functions
  boost::optional<PlantLoop> ChillerAbsorptionIndirect::chilledWaterLoop() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->chilledWaterLoop();
  }

  boost::optional<PlantLoop> ChillerAbsorptionIndirect::condenserWaterLoop() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->condenserWaterLoop();
  }

  boost::optional<PlantLoop> ChillerAbsorptionIndirect::generatorLoop() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->generatorLoop();
  }

  /// @cond
  ChillerAbsorptionIndirect::ChillerAbsorptionIndirect(std::shared_ptr<detail::ChillerAbsorptionIndirect_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> ChillerAbsorptionIndirect::autosizedNominalCapacity() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizedNominalCapacity();
  }

  boost::optional<double> ChillerAbsorptionIndirect::autosizedNominalPumpingPower() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizedNominalPumpingPower();
  }

  boost::optional<double> ChillerAbsorptionIndirect::autosizedDesignChilledWaterFlowRate() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizedDesignChilledWaterFlowRate();
  }

  boost::optional<double> ChillerAbsorptionIndirect::autosizedDesignCondenserWaterFlowRate() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizedDesignCondenserWaterFlowRate();
  }

  boost::optional<double> ChillerAbsorptionIndirect::autosizedDesignGeneratorFluidFlowRate() const {
    return getImpl<detail::ChillerAbsorptionIndirect_Impl>()->autosizedDesignGeneratorFluidFlowRate();
  }

}  // namespace model
}  // namespace openstudio
