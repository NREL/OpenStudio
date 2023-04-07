/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class HeatPumpWaterToWaterEquationFitCooling;
  class CurveQuadLinear;

  namespace detail {

    class HeatPumpWaterToWaterEquationFitHeating_Impl;

  }  // namespace detail

  /** HeatPumpWaterToWaterEquationFitHeating is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:WaterToWater:EquationFit:Heating'. */
  class MODEL_API HeatPumpWaterToWaterEquationFitHeating : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpWaterToWaterEquationFitHeating(const Model& model, const CurveQuadLinear& heatingCapacityCurve,
                                                    const CurveQuadLinear& heatingCompressorPowerCurve);

    explicit HeatPumpWaterToWaterEquationFitHeating(const Model& model);

    virtual ~HeatPumpWaterToWaterEquationFitHeating() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatPumpWaterToWaterEquationFitHeating(const HeatPumpWaterToWaterEquationFitHeating& other) = default;
    HeatPumpWaterToWaterEquationFitHeating(HeatPumpWaterToWaterEquationFitHeating&& other) = default;
    HeatPumpWaterToWaterEquationFitHeating& operator=(const HeatPumpWaterToWaterEquationFitHeating&) = default;
    HeatPumpWaterToWaterEquationFitHeating& operator=(HeatPumpWaterToWaterEquationFitHeating&&) = default;

    //@}

    static IddObjectType iddObjectType();

    boost::optional<double> referenceLoadSideFlowRate() const;

    /** Prior to EnergyPlus 8.7.0 this field was not autosizeable. To preserve backwards compatibility this method will return -999.0 in autosized models.**/
    double ratedLoadSideFlowRate() const;

    bool isReferenceLoadSideFlowRateAutosized() const;

    boost::optional<double> referenceSourceSideFlowRate() const;

    /** Prior to EnergyPlus 8.7.0 this field was not autosizeable. To preserve backwards compatibility this method will return -999.0 in autosized models.**/
    double ratedSourceSideFlowRate() const;

    bool isReferenceSourceSideFlowRateAutosized() const;

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Capacity" **/
    boost::optional<double> ratedHeatingCapacity() const;

    bool isRatedHeatingCapacityAutosized() const;

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Power Consumption" **/
    boost::optional<double> ratedHeatingPowerConsumption() const;

    bool isRatedHeatingPowerConsumptionAutosized() const;

    CurveQuadLinear heatingCapacityCurve() const;
    OS_DEPRECATED double heatingCapacityCoefficient1() const;
    OS_DEPRECATED double heatingCapacityCoefficient2() const;
    OS_DEPRECATED double heatingCapacityCoefficient3() const;
    OS_DEPRECATED double heatingCapacityCoefficient4() const;
    OS_DEPRECATED double heatingCapacityCoefficient5() const;

    CurveQuadLinear heatingCompressorPowerCurve() const;
    OS_DEPRECATED double heatingCompressorPowerCoefficient1() const;
    OS_DEPRECATED double heatingCompressorPowerCoefficient2() const;
    OS_DEPRECATED double heatingCompressorPowerCoefficient3() const;
    OS_DEPRECATED double heatingCompressorPowerCoefficient4() const;
    OS_DEPRECATED double heatingCompressorPowerCoefficient5() const;

    double referenceCoefficientofPerformance() const;

    double sizingFactor() const;

    boost::optional<HeatPumpWaterToWaterEquationFitCooling> companionCoolingHeatPump() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

    /** Synonym of setReferenceLoadSideFlowRate for backwards compatibility **/
    bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

    void autosizeReferenceLoadSideFlowRate();

    bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

    /** Synonym of setReferenceSourceSideFlowRate for backwards compatibility **/
    bool setRatedSourceSideFlowRate(double ratedLoadSideFlowRate);

    void autosizeReferenceSourceSideFlowRate();

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Capacity" **/
    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    void autosizeRatedHeatingCapacity();

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Power Consumption" **/
    bool setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption);

    void autosizeRatedHeatingPowerConsumption();

    bool setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve);

    OS_DEPRECATED bool setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);
    OS_DEPRECATED bool setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);
    OS_DEPRECATED bool setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);
    OS_DEPRECATED bool setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);
    OS_DEPRECATED bool setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

    bool setHeatingCompressorPowerCurve(const CurveQuadLinear& heatingCompressorPowerCurve);

    OS_DEPRECATED bool setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1);
    OS_DEPRECATED bool setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2);
    OS_DEPRECATED bool setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3);
    OS_DEPRECATED bool setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4);
    OS_DEPRECATED bool setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5);

    bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

    bool setSizingFactor(double sizingFactor);

    bool setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedReferenceLoadSideFlowRate() const;

    boost::optional<double> autosizedReferenceSourceSideFlowRate() const;

    boost::optional<double> autosizedRatedHeatingCapacity() const;

    boost::optional<double> autosizedRatedHeatingPowerConsumption() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeatPumpWaterToWaterEquationFitHeating_Impl;

    explicit HeatPumpWaterToWaterEquationFitHeating(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitHeating_Impl> impl);

    friend class detail::HeatPumpWaterToWaterEquationFitHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitHeating");
  };

  /** \relates HeatPumpWaterToWaterEquationFitHeating*/
  using OptionalHeatPumpWaterToWaterEquationFitHeating = boost::optional<HeatPumpWaterToWaterEquationFitHeating>;

  /** \relates HeatPumpWaterToWaterEquationFitHeating*/
  using HeatPumpWaterToWaterEquationFitHeatingVector = std::vector<HeatPumpWaterToWaterEquationFitHeating>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP
