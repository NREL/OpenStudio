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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double ratedLoadSideFlowRate() const;

    double ratedSourceSideFlowRate() const;

    double ratedHeatingCapacity() const;

    double ratedHeatingPowerConsumption() const;

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

    //@}
    /** @name Setters */
    //@{

    bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

    bool setRatedSourceSideFlowRate(double ratedSourceSideFlowRate);

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    bool setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption);

    void setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);

    void setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);

    void setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);

    void setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);

    void setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

    void setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1);

    void setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2);

    void setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3);

    void setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4);

    void setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5);

    //@}
    /** @name Other */
    //@{

    virtual unsigned supplyInletPort();

    virtual unsigned supplyOutletPort();

    virtual unsigned demandInletPort();

    virtual unsigned demandOutletPort();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitHeating");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP

