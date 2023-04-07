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

#ifndef MODEL_HEATPUMPPLANTLOOPEIRHEATING_HPP
#define MODEL_HEATPUMPPLANTLOOPEIRHEATING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class HeatPumpPlantLoopEIRCooling;
  class Curve;

  namespace detail {

    class HeatPumpPlantLoopEIRHeating_Impl;

  }  // namespace detail

  /** HeatPumpPlantLoopEIRHeating is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:PlantLoop:EIR:Heating'. */
  class MODEL_API HeatPumpPlantLoopEIRHeating : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpPlantLoopEIRHeating(const Model& model, const Curve& capacityModifierFunctionofTemperatureCurve,
                                         const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve,
                                         const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

    explicit HeatPumpPlantLoopEIRHeating(const Model& model);

    virtual ~HeatPumpPlantLoopEIRHeating() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatPumpPlantLoopEIRHeating(const HeatPumpPlantLoopEIRHeating& other) = default;
    HeatPumpPlantLoopEIRHeating(HeatPumpPlantLoopEIRHeating&& other) = default;
    HeatPumpPlantLoopEIRHeating& operator=(const HeatPumpPlantLoopEIRHeating&) = default;
    HeatPumpPlantLoopEIRHeating& operator=(HeatPumpPlantLoopEIRHeating&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validCondenserTypeValues();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    std::string condenserType() const;

    boost::optional<HeatPumpPlantLoopEIRCooling> companionCoolingHeatPump() const;

    boost::optional<double> loadSideReferenceFlowRate() const;

    bool isLoadSideReferenceFlowRateAutosized() const;

    boost::optional<double> sourceSideReferenceFlowRate() const;

    bool isSourceSideReferenceFlowRateAutosized() const;

    boost::optional<double> referenceCapacity() const;

    bool isReferenceCapacityAutosized() const;

    double referenceCoefficientofPerformance() const;

    double sizingFactor() const;

    Curve capacityModifierFunctionofTemperatureCurve() const;

    Curve electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const;

    Curve electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setCondenserType(const std::string& condenserType);

    bool setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP);

    bool setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate);

    void autosizeLoadSideReferenceFlowRate();

    bool setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate);

    void autosizeSourceSideReferenceFlowRate();

    bool setReferenceCapacity(double referenceCapacity);

    void autosizeReferenceCapacity();

    bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

    bool setSizingFactor(double sizingFactor);

    bool setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve);

    bool setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve);

    bool
      setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedLoadSideReferenceFlowRate() const;

    boost::optional<double> autosizedSourceSideReferenceFlowRate() const;

    boost::optional<double> autosizedReferenceCapacity() const;

    /** Convenience Function to return the Load Side Water Loop (HeatPump on supply side) **/
    boost::optional<PlantLoop> loadSideWaterLoop() const;

    /** Convenience Function to return the Source Side (Condenser) Water Loop (HeatPump on demand side) **/
    boost::optional<PlantLoop> sourceSideWaterLoop() const;

    // Convenience function to return the inletNode on the Source Side
    boost::optional<Node> sourceSideWaterInletNode() const;
    // Convenience function to return the outletNode on the Source Side
    boost::optional<Node> sourceSideWaterOutletNode() const;

    // Convenience function to return the inletNode on the Load Side
    boost::optional<Node> loadSideWaterInletNode() const;
    // Convenience function to return the outletNode on the Load Side
    boost::optional<Node> loadSideWaterOutletNode() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeatPumpPlantLoopEIRHeating_Impl;

    explicit HeatPumpPlantLoopEIRHeating(std::shared_ptr<detail::HeatPumpPlantLoopEIRHeating_Impl> impl);

    friend class detail::HeatPumpPlantLoopEIRHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpPlantLoopEIRHeating");
  };

  /** \relates HeatPumpPlantLoopEIRHeating*/
  using OptionalHeatPumpPlantLoopEIRHeating = boost::optional<HeatPumpPlantLoopEIRHeating>;

  /** \relates HeatPumpPlantLoopEIRHeating*/
  using HeatPumpPlantLoopEIRHeatingVector = std::vector<HeatPumpPlantLoopEIRHeating>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPPLANTLOOPEIRHEATING_HPP
