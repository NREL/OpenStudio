/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPPLANTLOOPEIRCOOLING_HPP
#define MODEL_HEATPUMPPLANTLOOPEIRCOOLING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class HeatPumpPlantLoopEIRHeating;
  class Curve;

  namespace detail {

    class HeatPumpPlantLoopEIRCooling_Impl;

  }  // namespace detail

  /** HeatPumpPlantLoopEIRCooling is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:PlantLoop:EIR:Cooling'. */
  class MODEL_API HeatPumpPlantLoopEIRCooling : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpPlantLoopEIRCooling(const Model& model, const Curve& capacityModifierFunctionofTemperatureCurve,
                                         const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve,
                                         const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

    explicit HeatPumpPlantLoopEIRCooling(const Model& model);

    virtual ~HeatPumpPlantLoopEIRCooling() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatPumpPlantLoopEIRCooling(const HeatPumpPlantLoopEIRCooling& other) = default;
    HeatPumpPlantLoopEIRCooling(HeatPumpPlantLoopEIRCooling&& other) = default;
    HeatPumpPlantLoopEIRCooling& operator=(const HeatPumpPlantLoopEIRCooling&) = default;
    HeatPumpPlantLoopEIRCooling& operator=(HeatPumpPlantLoopEIRCooling&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validCondenserTypeValues();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    std::string condenserType() const;

    boost::optional<HeatPumpPlantLoopEIRHeating> companionHeatingHeatPump() const;

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

    bool setCompanionHeatingHeatPump(const HeatPumpPlantLoopEIRHeating& companionHP);

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
    using ImplType = detail::HeatPumpPlantLoopEIRCooling_Impl;

    explicit HeatPumpPlantLoopEIRCooling(std::shared_ptr<detail::HeatPumpPlantLoopEIRCooling_Impl> impl);

    friend class detail::HeatPumpPlantLoopEIRCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpPlantLoopEIRCooling");
  };

  /** \relates HeatPumpPlantLoopEIRCooling*/
  using OptionalHeatPumpPlantLoopEIRCooling = boost::optional<HeatPumpPlantLoopEIRCooling>;

  /** \relates HeatPumpPlantLoopEIRCooling*/
  using HeatPumpPlantLoopEIRCoolingVector = std::vector<HeatPumpPlantLoopEIRCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPPLANTLOOPEIRCOOLING_HPP
