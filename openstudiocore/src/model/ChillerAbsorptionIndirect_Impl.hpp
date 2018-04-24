/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_CHILLERABSORPTIONINDIRECT_IMPL_HPP
#define MODEL_CHILLERABSORPTIONINDIRECT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** ChillerAbsorptionIndirect_Impl is a WaterToWaterComponent_Impl that is the implementation class for ChillerAbsorptionIndirect.*/
  class MODEL_API ChillerAbsorptionIndirect_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ChillerAbsorptionIndirect_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle);

    ChillerAbsorptionIndirect_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    ChillerAbsorptionIndirect_Impl(const ChillerAbsorptionIndirect_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~ChillerAbsorptionIndirect_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual unsigned supplyInletPort() override;
    virtual unsigned supplyOutletPort() override;
    virtual unsigned demandInletPort() override;
    virtual unsigned demandOutletPort() override;
    virtual unsigned tertiaryInletPort() const override;
    virtual unsigned tertiaryOutletPort() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    boost::optional<double> nominalPumpingPower() const;

    bool isNominalPumpingPowerAutosized() const;

    double minimumPartLoadRatio() const;

    double maximumPartLoadRatio() const;

    double optimumPartLoadRatio() const;

    double designCondenserInletTemperature() const;

    double condenserInletTemperatureLowerLimit() const;

    double chilledWaterOutletTemperatureLowerLimit() const;

    boost::optional<double> designChilledWaterFlowRate() const;

    bool isDesignChilledWaterFlowRateAutosized() const;

    boost::optional<double> designCondenserWaterFlowRate() const;

    bool isDesignCondenserWaterFlowRateAutosized() const;

    std::string chillerFlowMode() const;

    Curve generatorHeatInputFunctionofPartLoadRatioCurve() const;

    Curve pumpElectricInputFunctionofPartLoadRatioCurve() const;

    Curve capacityCorrectionFunctionofCondenserTemperatureCurve() const;

    Curve capacityCorrectionFunctionofChilledWaterTemperatureCurve() const;

    Curve capacityCorrectionFunctionofGeneratorTemperatureCurve() const;

    Curve generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const;

    Curve generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const;

    std::string generatorHeatSourceType() const;

    boost::optional<double> designGeneratorFluidFlowRate() const;

    bool isDesignGeneratorFluidFlowRateAutosized() const;

    double temperatureLowerLimitGeneratorInlet() const;

    double degreeofSubcoolinginSteamGenerator() const;

    double degreeofSubcoolinginSteamCondensateLoop() const;

    double sizingFactor() const;

  boost::optional<double> autosizedNominalCapacity() const ;

  boost::optional<double> autosizedNominalPumpingPower() const ;

  boost::optional<double> autosizedDesignChilledWaterFlowRate() const ;

  boost::optional<double> autosizedDesignCondenserWaterFlowRate() const ;

  boost::optional<double> autosizedDesignGeneratorFluidFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setNominalCapacity(boost::optional<double> nominalCapacity);

    void autosizeNominalCapacity();

    bool setNominalPumpingPower(boost::optional<double> nominalPumpingPower);

    void autosizeNominalPumpingPower();

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    bool setDesignCondenserInletTemperature(double designCondenserInletTemperature);

    bool setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit);

    bool setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

    bool setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate);

    void autosizeDesignChilledWaterFlowRate();

    bool setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate);

    void autosizeDesignCondenserWaterFlowRate();

    bool setChillerFlowMode(std::string chillerFlowMode);

    bool setGeneratorHeatInputFunctionofPartLoadRatioCurve(const Curve& curve);

    bool setPumpElectricInputFunctionofPartLoadRatioCurve(const Curve& curve);

    bool setCapacityCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve);

    bool setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve);

    bool setCapacityCorrectionFunctionofGeneratorTemperatureCurve(const Curve& curve);

    bool setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve);

    bool setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve);

    // TODO: This needs to be set automatically based on plant loop connected to it
    bool setGeneratorHeatSourceType(std::string generatorHeatSourceType);

    bool setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate);

    void autosizeDesignGeneratorFluidFlowRate();

    bool setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet);

    bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

    bool setDegreeofSubcoolinginSteamCondensateLoop(double degreeofSubcoolinginSteamCondensateLoop);

    bool setSizingFactor(double sizingFactor);

    //@}
    /** @name Other */
    //@{

    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ChillerAbsorptionIndirect");

    boost::optional<Curve> optionalGeneratorHeatInputFunctionofPartLoadRatioCurve() const;
    boost::optional<Curve> optionalPumpElectricInputFunctionofPartLoadRatioCurve() const;
    boost::optional<Curve> optionalCapacityCorrectionFunctionofCondenserTemperatureCurve() const;
    boost::optional<Curve> optionalCapacityCorrectionFunctionofChilledWaterTemperatureCurve() const;
    boost::optional<Curve> optionalCapacityCorrectionFunctionofGeneratorTemperatureCurve() const;
    boost::optional<Curve> optionalGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const;
    boost::optional<Curve> optionalGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CHILLERABSORPTIONINDIRECT_IMPL_HPP
