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

#ifndef MODEL_BOILERHOTWATER_IMPL_HPP
#define MODEL_BOILERHOTWATER_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** BoilerHotWater_Impl is a StraightComponent_Impl that is the implementation class for BoilerHotWater.*/
  class MODEL_API BoilerHotWater_Impl : public StraightComponent_Impl {

   public:

    /** @name Constructors and Destructors */
    //@{

    BoilerHotWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    BoilerHotWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    BoilerHotWater_Impl(const BoilerHotWater_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~BoilerHotWater_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual void autosize() override;

    virtual void applySizingValues() override;

    virtual bool addToNode(Node & node) override;

    virtual ModelObject clone(Model model) const override;

    //@}
    /** @name Getters */
    //@{

    std::string fuelType() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    double nominalThermalEfficiency() const;

    boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable() const;

    boost::optional<Curve> normalizedBoilerEfficiencyCurve() const;

    boost::optional<double> designWaterOutletTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    double minimumPartLoadRatio() const;

    bool isMinimumPartLoadRatioDefaulted() const;

    double maximumPartLoadRatio() const;

    bool isMaximumPartLoadRatioDefaulted() const;

    double optimumPartLoadRatio() const;

    bool isOptimumPartLoadRatioDefaulted() const;

    // TODO: Handle Non-Extensible IddField Boiler Water Inlet Node Name.

    // TODO: Handle Non-Extensible IddField Boiler Water Outlet Node Name.

    double waterOutletUpperTemperatureLimit() const;

    bool isWaterOutletUpperTemperatureLimitDefaulted() const;

    std::string boilerFlowMode() const;

    bool isBoilerFlowModeDefaulted() const;

    boost::optional<double> parasiticElectricLoad() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    boost::optional<double> autosizedNominalCapacity() const ;

    boost::optional<double> autosizedDesignWaterFlowRate() const ;

    std::string endUseSubcategory() const;


    //@}
    /** @name Setters */
    //@{

    bool setFuelType(std::string fuelType);

    bool setNominalCapacity(boost::optional<double> nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    bool setNominalThermalEfficiency(double nominalThermalEfficiency);

    bool setEfficiencyCurveTemperatureEvaluationVariable(boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable);

    void resetEfficiencyCurveTemperatureEvaluationVariable();

    bool setNormalizedBoilerEfficiencyCurve(const boost::optional<Curve>& normalizedBoilerEfficiencyCurve);

    void resetNormalizedBoilerEfficiencyCurve();

    bool setDesignWaterOutletTemperature(boost::optional<double> designWaterOutletTemperature);

    void resetDesignWaterOutletTemperature();

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    void resetMinimumPartLoadRatio();

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    void resetMaximumPartLoadRatio();

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    void resetOptimumPartLoadRatio();

    // TODO: Handle Non-Extensible IddField Boiler Water Inlet Node Name.

    // TODO: Handle Non-Extensible IddField Boiler Water Outlet Node Name.

    bool setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit);

    void resetWaterOutletUpperTemperatureLimit();

    bool setBoilerFlowMode(std::string boilerFlowMode);

    void resetBoilerFlowMode();

    bool setParasiticElectricLoad(boost::optional<double> parasiticElectricLoad);

    void resetParasiticElectricLoad();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    bool setEndUseSubcategory(const std::string & endUseSubcategory);

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.BoilerHotWater");

    boost::optional<ModelObject> normalizedBoilerEfficiencyCurveAsModelObject() const;

    bool setNormalizedBoilerEfficiencyCurveAsModelObject(const boost::optional<ModelObject>& modelObject);

  };

} // detail

} // model
} // openstudio

#endif // MODEL_BOILERHOTWATER_IMPL_HPP
