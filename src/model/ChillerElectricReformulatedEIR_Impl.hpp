/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERELECTRICREFORMULATEDEIR_IMPL_HPP
#define MODEL_CHILLERELECTRICREFORMULATEDEIR_IMPL_HPP

#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Schedule;
  class Node;

  namespace detail {

    class MODEL_API ChillerElectricReformulatedEIR_Impl : public WaterToWaterComponent_Impl
    {

     public:
      ChillerElectricReformulatedEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ChillerElectricReformulatedEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ChillerElectricReformulatedEIR_Impl(const ChillerElectricReformulatedEIR_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ChillerElectricReformulatedEIR_Impl() override = default;

      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      // chilledWaterLoop
      virtual unsigned supplyInletPort() const override;
      virtual unsigned supplyOutletPort() const override;

      // condenserWaterLoop
      virtual unsigned demandInletPort() const override;
      virtual unsigned demandOutletPort() const override;

      // heatRecoveryLoop
      virtual unsigned tertiaryInletPort() const override;
      virtual unsigned tertiaryOutletPort() const override;

      /* This function will perform a check if trying to add it to a node that is on the demand side of a plant loop.
   * If:
   *     - the node is on the demand side of a loop
   *     - the node isn't on the current condenser water loop itself
   *     - the chiller doesn't already have a heat recovery (tertiary) loop,
   * then it tries to add it to the Tertiary loop.
   * In all other cases, it will call the base class' method WaterToWaterComponent_Impl::addToNode()
   */
      virtual bool addToNode(Node& node) override;

      /* Restricts addToTertiaryNode to a node that is on the demand side of a plant loop (tertiary = Heat Recovery Loop) */
      virtual bool addToTertiaryNode(Node& node) override;

      /** Override to switch the condenser type to 'AirCooled' **/
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

      boost::optional<double> referenceCapacity() const;

      bool isReferenceCapacityAutosized() const;

      double referenceCOP() const;

      double referenceLeavingChilledWaterTemperature() const;

      bool isReferenceLeavingChilledWaterTemperatureDefaulted() const;

      double referenceLeavingCondenserWaterTemperature() const;

      bool isReferenceLeavingCondenserWaterTemperatureDefaulted() const;

      boost::optional<double> referenceChilledWaterFlowRate() const;

      bool isReferenceChilledWaterFlowRateAutosized() const;

      boost::optional<double> referenceCondenserWaterFlowRate() const;

      bool isReferenceCondenserWaterFlowRateAutosized() const;

      Curve coolingCapacityFunctionOfTemperature() const;

      Curve electricInputToCoolingOutputRatioFunctionOfTemperature() const;

      std::string electricInputToCoolingOutputRatioFunctionOfPLRType() const;

      Curve electricInputToCoolingOutputRatioFunctionOfPLR() const;

      double minimumPartLoadRatio() const;

      bool isMinimumPartLoadRatioDefaulted() const;

      double maximumPartLoadRatio() const;

      bool isMaximumPartLoadRatioDefaulted() const;

      double optimumPartLoadRatio() const;

      bool isOptimumPartLoadRatioDefaulted() const;

      double minimumUnloadingRatio() const;

      bool isMinimumUnloadingRatioDefaulted() const;

      double fractionofCompressorElectricConsumptionRejectedbyCondenser() const;

      bool isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted() const;

      double leavingChilledWaterLowerTemperatureLimit() const;

      bool isLeavingChilledWaterLowerTemperatureLimitDefaulted() const;

      std::string chillerFlowMode() const;

      bool isChillerFlowModeDefaulted() const;

      boost::optional<double> designHeatRecoveryWaterFlowRate() const;

      bool isDesignHeatRecoveryWaterFlowRateAutosized() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      double condenserHeatRecoveryRelativeCapacityFraction() const;

      boost::optional<Schedule> heatRecoveryInletHighTemperatureLimitSchedule() const;

      boost::optional<Node> heatRecoveryLeavingTemperatureSetpointNode() const;

      std::string endUseSubcategory() const;

      std::string condenserFlowControl() const;

      boost::optional<Curve> condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve() const;

      boost::optional<Schedule> temperatureDifferenceAcrossCondenserSchedule() const;

      double condenserMinimumFlowFraction() const;

      boost::optional<Curve> thermosiphonCapacityFractionCurve() const;

      double thermosiphonMinimumTemperatureDifference() const;

      //@}
      /** @name Setters */
      //@{

      bool setReferenceCapacity(boost::optional<double> referenceCapacity);

      bool setReferenceCapacity(double referenceCapacity);

      void autosizeReferenceCapacity();

      bool setReferenceCOP(double referenceCOP);

      bool setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature);

      void resetReferenceLeavingChilledWaterTemperature();

      bool setReferenceLeavingCondenserWaterTemperature(double referenceLeavingCondenserWaterTemperature);

      void resetReferenceLeavingCondenserWaterTemperature();

      bool setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate);

      bool setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate);

      void resetReferenceChilledWaterFlowRate();

      void autosizeReferenceChilledWaterFlowRate();

      bool setReferenceCondenserWaterFlowRate(boost::optional<double> referenceCondenserWaterFlowRate);

      bool setReferenceCondenserWaterFlowRate(double referenceCondenserWaterFlowRate);

      void resetReferenceCondenserWaterFlowRate();

      void autosizeReferenceCondenserWaterFlowRate();

      bool setCoolingCapacityFunctionOfTemperature(const Curve&);

      bool setElectricInputToCoolingOutputRatioFunctionOfTemperature(const Curve&);

      bool setElectricInputToCoolingOutputRatioFunctionOfPLRType(const std::string& electricInputToCoolingOutputRatioFunctionOfPLRType);

      void resetElectricInputToCoolingOutputRatioFunctionOfPLRType();

      bool setElectricInputToCoolingOutputRatioFunctionOfPLR(const Curve&);

      bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

      void resetMinimumPartLoadRatio();

      bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

      void resetMaximumPartLoadRatio();

      bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

      void resetOptimumPartLoadRatio();

      bool setMinimumUnloadingRatio(double minimumUnloadingRatio);

      void resetMinimumUnloadingRatio();

      bool setFractionofCompressorElectricConsumptionRejectedbyCondenser(double fractionofCompressorElectricConsumptionRejectedbyCondenser);

      void resetFractionofCompressorElectricConsumptionRejectedbyCondenser();

      bool setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit);

      void resetLeavingChilledWaterLowerTemperatureLimit();

      bool setChillerFlowMode(const std::string& chillerFlowMode);

      void resetChillerFlowMode();

      bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

      void autosizeDesignHeatRecoveryWaterFlowRate();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction);

      bool setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& s);
      void resetHeatRecoveryInletHighTemperatureLimitSchedule();

      bool setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node);
      void resetHeatRecoveryLeavingTemperatureSetpointNode();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      bool setCondenserFlowControl(const std::string& condenserFlowControl);

      bool
        setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(const Curve& condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve);
      void resetCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve();

      bool setTemperatureDifferenceAcrossCondenserSchedule(Schedule& temperatureDifferenceAcrossCondenserSchedule);
      void resetTemperatureDifferenceAcrossCondenserSchedule();

      bool setCondenserMinimumFlowFraction(double condenserMinimumFlowFraction);

      bool setThermosiphonCapacityFractionCurve(const Curve& thermosiphonCapacityFractionCurve);
      void resetThermosiphonCapacityFractionCurve();

      bool setThermosiphonMinimumTemperatureDifference(double thermosiphonMinimumTemperatureDifference);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedReferenceCapacity() const;

      boost::optional<double> autosizedReferenceChilledWaterFlowRate() const;

      boost::optional<double> autosizedReferenceCondenserWaterFlowRate() const;

      boost::optional<double> autosizedDesignHeatRecoveryWaterFlowRate() const;

      /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
      boost::optional<PlantLoop> chilledWaterLoop() const;
      // Same as supplyInletModelObject, but cast to a Node
      boost::optional<Node> chilledWaterInletNode() const;
      boost::optional<Node> chilledWaterOutletNode() const;

      /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
      boost::optional<PlantLoop> condenserWaterLoop() const;
      boost::optional<Node> condenserInletNode() const;
      boost::optional<Node> condenserOutletNode() const;

      /** Convenience Function to return the Heat Recovery Loop (chiller on demand side - tertiary) **/
      boost::optional<PlantLoop> heatRecoveryLoop() const;
      boost::optional<Node> heatRecoveryInletNode() const;
      boost::optional<Node> heatRecoveryOutletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ChillerElectricReformulatedEIR");

      boost::optional<ModelObject> basinHeaterScheduleAsModelObject() const;

      bool setBasinHeaterScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICREFORMULATEDEIR_IMPL_HPP
