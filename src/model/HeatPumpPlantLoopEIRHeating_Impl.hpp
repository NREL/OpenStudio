/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPPLANTLOOPEIRHEATING_IMPL_HPP
#define MODEL_HEATPUMPPLANTLOOPEIRHEATING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class HeatPumpPlantLoopEIRCooling;
  class Curve;

  namespace detail {

    /** HeatPumpPlantLoopEIRHeating_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpPlantLoopEIRHeating.*/
    class MODEL_API HeatPumpPlantLoopEIRHeating_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatPumpPlantLoopEIRHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatPumpPlantLoopEIRHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatPumpPlantLoopEIRHeating_Impl(const HeatPumpPlantLoopEIRHeating_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatPumpPlantLoopEIRHeating_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned supplyInletPort() const override;

      virtual unsigned supplyOutletPort() const override;

      virtual unsigned demandInletPort() const override;

      virtual unsigned demandOutletPort() const override;

      virtual unsigned tertiaryInletPort() const override;

      virtual unsigned tertiaryOutletPort() const override;

      /* This function will call the base class' method WaterToWaterComponent_Impl::addToNode()
       * If this is connecting to the demand side of a loop, will set the condenserType to 'WaterSource'
       */
      virtual bool addToNode(Node& node) override;

      /* Restricts addToTertiaryNode to a node that is on the demand side of a plant loop (tertiary = Heat Recovery Loop) */
      virtual bool addToTertiaryNode(Node& node) override;

      /** Override to switch the condenser type to 'AirSource' **/
      virtual bool removeFromSecondaryPlantLoop() override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string condenserType() const;

      boost::optional<HeatPumpPlantLoopEIRCooling> companionCoolingHeatPump() const;

      boost::optional<double> loadSideReferenceFlowRate() const;

      bool isLoadSideReferenceFlowRateAutosized() const;

      boost::optional<double> sourceSideReferenceFlowRate() const;

      bool isSourceSideReferenceFlowRateAutosized() const;

      boost::optional<double> heatRecoveryReferenceFlowRate() const;

      bool isHeatRecoveryReferenceFlowRateAutosized() const;

      boost::optional<double> referenceCapacity() const;

      bool isReferenceCapacityAutosized() const;

      double referenceCoefficientofPerformance() const;

      double sizingFactor() const;

      Curve capacityModifierFunctionofTemperatureCurve() const;

      Curve electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const;

      Curve electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const;

      double heatingToCoolingCapacitySizingRatio() const;

      std::string heatPumpSizingMethod() const;

      std::string controlType() const;

      std::string flowMode() const;

      double minimumPartLoadRatio() const;

      double minimumSourceInletTemperature() const;

      double maximumSourceInletTemperature() const;

      boost::optional<Curve> minimumSupplyWaterTemperatureCurve() const;

      boost::optional<Curve> maximumSupplyWaterTemperatureCurve() const;

      boost::optional<Curve> dryOutdoorCorrectionFactorCurve() const;

      double maximumOutdoorDryBulbTemperatureForDefrostOperation() const;

      std::string heatPumpDefrostControl() const;

      double heatPumpDefrostTimePeriodFraction() const;

      boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

      boost::optional<Curve> timedEmpiricalDefrostFrequencyCurve() const;

      boost::optional<Curve> timedEmpiricalDefrostHeatLoadPenaltyCurve() const;

      boost::optional<Curve> timedEmpiricalDefrostHeatInputEnergyFractionCurve() const;

      double minimumHeatRecoveryOutletTemperature() const;

      boost::optional<Curve> heatRecoveryCapacityModifierFunctionofTemperatureCurve() const;

      boost::optional<Curve> heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setCondenserType(const std::string& condenserType);

      bool setCompanionCoolingHeatPump(const HeatPumpPlantLoopEIRCooling& companionHP);

      bool setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate);

      void autosizeLoadSideReferenceFlowRate();

      bool setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate);

      void autosizeSourceSideReferenceFlowRate();

      bool setHeatRecoveryReferenceFlowRate(double heatRecoveryReferenceFlowRate);

      void autosizeHeatRecoveryReferenceFlowRate();

      bool setReferenceCapacity(double referenceCapacity);

      void autosizeReferenceCapacity();

      bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

      bool setSizingFactor(double sizingFactor);

      bool setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve);

      bool setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve);

      bool setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
        const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);

      bool setHeatingToCoolingCapacitySizingRatio(double heatingToCoolingCapacitySizingRatio);

      bool setHeatPumpSizingMethod(const std::string& heatPumpSizingMethod);

      bool setControlType(const std::string& controlType);

      bool setFlowMode(const std::string& flowMode);

      bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

      bool setMinimumSourceInletTemperature(double minimumSourceInletTemperature);

      bool setMaximumSourceInletTemperature(double maximumSourceInletTemperature);

      bool setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve);

      void resetMinimumSupplyWaterTemperatureCurve();

      bool setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve);

      void resetMaximumSupplyWaterTemperatureCurve();

      bool setDryOutdoorCorrectionFactorCurve(const Curve& dryOutdoorCorrectionFactorCurve);

      void resetDryOutdoorCorrectionFactorCurve();

      bool setMaximumOutdoorDryBulbTemperatureForDefrostOperation(double maximumOutdoorDryBulbTemperatureForDefrostOperation);

      bool setHeatPumpDefrostControl(const std::string& heatPumpDefrostControl);

      bool setHeatPumpDefrostTimePeriodFraction(double heatPumpDefrostTimePeriodFraction);

      bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& defrostEnergyInputRatioFunctionofTemperatureCurve);

      void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

      bool setTimedEmpiricalDefrostFrequencyCurve(const Curve& timedEmpiricalDefrostFrequencyCurve);

      void resetTimedEmpiricalDefrostFrequencyCurve();

      bool setTimedEmpiricalDefrostHeatLoadPenaltyCurve(const Curve& timedEmpiricalDefrostHeatLoadPenaltyCurve);

      void resetTimedEmpiricalDefrostHeatLoadPenaltyCurve();

      bool setTimedEmpiricalDefrostHeatInputEnergyFractionCurve(const Curve& timedEmpiricalDefrostHeatInputEnergyFractionCurve);

      void resetTimedEmpiricalDefrostHeatInputEnergyFractionCurve();

      bool setMinimumHeatRecoveryOutletTemperature(double minimumHeatRecoveryOutletTemperature);

      bool setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(const Curve& heatRecoveryCapacityModifierFunctionofTemperatureCurve);

      void resetHeatRecoveryCapacityModifierFunctionofTemperatureCurve();

      bool setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
        const Curve& heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve);

      void resetHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedLoadSideReferenceFlowRate() const;

      boost::optional<double> autosizedSourceSideReferenceFlowRate() const;

      boost::optional<double> autosizedHeatRecoveryReferenceFlowRate() const;

      boost::optional<double> autosizedReferenceCapacity() const;

      /** Convenience Function to return the Load Side Water Loop (HeatPump on supply side) **/
      boost::optional<PlantLoop> loadSideWaterLoop() const;

      /** Convenience Function to return the Source Side (Condenser) Water Loop (HeatPump on demand side) **/
      boost::optional<PlantLoop> sourceSideWaterLoop() const;

      /** Convenience Function to return the Heat Recovery Loop (HeatPump on demand side - tertiary) **/
      boost::optional<PlantLoop> heatRecoveryLoop() const;

      // Convenience function to return the inletNode on the Source Side
      boost::optional<Node> sourceSideWaterInletNode() const;
      // Convenience function to return the outletNode on the Source Side
      boost::optional<Node> sourceSideWaterOutletNode() const;

      // Convenience function to return the inletNode on the Load Side
      boost::optional<Node> loadSideWaterInletNode() const;
      // Convenience function to return the outletNode on the Load Side
      boost::optional<Node> loadSideWaterOutletNode() const;

      // Convenience function to return the inletNode on the Heat Recovery
      boost::optional<Node> heatRecoveryInletNode() const;
      // Convenience function to return the outletNode on the Heat Recovery
      boost::optional<Node> heatRecoveryOutletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatPumpPlantLoopEIRHeating");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPPLANTLOOPEIRHEATING_IMPL_HPP
