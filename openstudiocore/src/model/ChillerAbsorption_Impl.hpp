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

#ifndef MODEL_CHILLERABSORPTION_IMPL_HPP
#define MODEL_CHILLERABSORPTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** ChillerAbsorption_Impl is a WaterToWaterComponent_Impl that is the implementation class for ChillerAbsorption.*/
  class MODEL_API ChillerAbsorption_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ChillerAbsorption_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

    ChillerAbsorption_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    ChillerAbsorption_Impl(const ChillerAbsorption_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~ChillerAbsorption_Impl() {}

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

    boost::optional<double> designChilledWaterFlowRate() const;

    bool isDesignChilledWaterFlowRateAutosized() const;

    boost::optional<double> designCondenserWaterFlowRate() const;

    bool isDesignCondenserWaterFlowRateAutosized() const;

    double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve() const;

    double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve() const;

    double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve() const;

    double coefficient1ofthePumpElectricUsePartLoadRatioCurve() const;

    double coefficient2ofthePumpElectricUsePartLoadRatioCurve() const;

    double coefficient3ofthePumpElectricUsePartLoadRatioCurve() const;

    double chilledWaterOutletTemperatureLowerLimit() const;

    std::string chillerFlowMode() const;

    std::string generatorHeatSourceType() const;

    boost::optional<double> designGeneratorFluidFlowRate() const;

    bool isDesignGeneratorFluidFlowRateAutosized() const;

    double degreeofSubcoolinginSteamGenerator() const;

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

    bool setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate);

    void autosizeDesignChilledWaterFlowRate();

    bool setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate);

    void autosizeDesignCondenserWaterFlowRate();

    void setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve);

    void setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve);

    void setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve);

    void setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(double coefficient1ofthePumpElectricUsePartLoadRatioCurve);

    void setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(double coefficient2ofthePumpElectricUsePartLoadRatioCurve);

    void setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(double coefficient3ofthePumpElectricUsePartLoadRatioCurve);

    void setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

    bool setChillerFlowMode(std::string chillerFlowMode);

    bool setGeneratorHeatSourceType(std::string generatorHeatSourceType);

    bool setDesignGeneratorFluidFlowRate(boost::optional<double> designGeneratorFluidFlowRate);

    void autosizeDesignGeneratorFluidFlowRate();

    void setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

    bool setSizingFactor(double sizingFactor);

    //@}
    /** @name Other */
    //@{

    unsigned supplyInletPort() override;
    unsigned supplyOutletPort() override;
    unsigned demandInletPort() override;
    unsigned demandOutletPort() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ChillerAbsorption");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_CHILLERABSORPTION_IMPL_HPP

