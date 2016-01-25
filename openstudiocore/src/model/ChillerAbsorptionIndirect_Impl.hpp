/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

    void setDesignCondenserInletTemperature(double designCondenserInletTemperature);

    void setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit);

    void setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

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

    bool setGeneratorHeatSourceType(std::string generatorHeatSourceType);

    void setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate);

    void autosizeDesignGeneratorFluidFlowRate();

    void setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet);

    bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

    bool setDegreeofSubcoolinginSteamCondensateLoop(double degreeofSubcoolinginSteamCondensateLoop);

    bool setSizingFactor(double sizingFactor);

    //@}
    /** @name Other */
    //@{

    unsigned supplyInletPort() override;
    unsigned supplyOutletPort() override;
    unsigned demandInletPort() override;
    unsigned demandOutletPort() override;
    unsigned tertiaryInletPort() const override;
    unsigned tertiaryOutletPort() const override;

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

