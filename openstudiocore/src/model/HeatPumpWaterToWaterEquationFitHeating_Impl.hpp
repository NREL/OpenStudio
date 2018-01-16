/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class HeatPumpWaterToWaterEquationFitCooling;

namespace detail {

  /** HeatPumpWaterToWaterEquationFitHeating_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpWaterToWaterEquationFitHeating.*/
  class MODEL_API HeatPumpWaterToWaterEquationFitHeating_Impl : public WaterToWaterComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    HeatPumpWaterToWaterEquationFitHeating_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    HeatPumpWaterToWaterEquationFitHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    HeatPumpWaterToWaterEquationFitHeating_Impl(const HeatPumpWaterToWaterEquationFitHeating_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~HeatPumpWaterToWaterEquationFitHeating_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> referenceLoadSideFlowRate() const;

    double ratedLoadSideFlowRate() const;

    bool isReferenceLoadSideFlowRateAutosized() const;

    boost::optional<double> referenceSourceSideFlowRate() const;

    double ratedSourceSideFlowRate() const;

    bool isReferenceSourceSideFlowRateAutosized() const;

    boost::optional<double> ratedHeatingCapacity() const;

    bool isRatedHeatingCapacityAutosized() const;

    boost::optional<double> ratedHeatingPowerConsumption() const;

    bool isRatedHeatingPowerConsumptionAutosized() const;

    double heatingCapacityCoefficient1() const;

    double heatingCapacityCoefficient2() const;

    double heatingCapacityCoefficient3() const;

    double heatingCapacityCoefficient4() const;

    double heatingCapacityCoefficient5() const;

    double heatingCompressorPowerCoefficient1() const;

    double heatingCompressorPowerCoefficient2() const;

    double heatingCompressorPowerCoefficient3() const;

    double heatingCompressorPowerCoefficient4() const;

    double heatingCompressorPowerCoefficient5() const;

    double referenceCoefficientofPerformance() const;

    double sizingFactor() const;

    boost::optional<HeatPumpWaterToWaterEquationFitCooling> companionCoolingHeatPump() const;

  boost::optional<double> autosizedReferenceLoadSideFlowRate() const ;

  boost::optional<double> autosizedReferenceSourceSideFlowRate() const ;

  boost::optional<double> autosizedRatedHeatingCapacity() const ;

  boost::optional<double> autosizedRatedHeatingPowerConsumption() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

    bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

    void autosizeReferenceLoadSideFlowRate();

    bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

    bool setRatedSourceSideFlowRate(double ratedLoadSideFlowRate);

    void autosizeReferenceSourceSideFlowRate();

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    void autosizeRatedHeatingCapacity();

    bool setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption);

    void autosizeRatedHeatingPowerConsumption();

    bool setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);

    bool setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);

    bool setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);

    bool setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);

    bool setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

    bool setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1);

    bool setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2);

    bool setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3);

    bool setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4);

    bool setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5);

    bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

    bool setSizingFactor(double sizingFactor);

    bool setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP);

    //@}
    /** @name Other */
    //@{

    virtual unsigned supplyInletPort() override;

    virtual unsigned supplyOutletPort() override;

    virtual unsigned demandInletPort() override;

    virtual unsigned demandOutletPort() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitHeating");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP
