/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_IMPL_HPP
#define MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class CentralHeatPumpSystem;
  class CentralHeatPumpSystemModule;

  namespace detail {

    /** ChillerHeaterPerformanceElectricEIR_Impl is a ParentObject_Impl that is the implementation class for ChillerHeaterPerformanceElectricEIR.*/
    class MODEL_API ChillerHeaterPerformanceElectricEIR_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ChillerHeaterPerformanceElectricEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ChillerHeaterPerformanceElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ChillerHeaterPerformanceElectricEIR_Impl(const ChillerHeaterPerformanceElectricEIR_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ChillerHeaterPerformanceElectricEIR_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> referenceCoolingModeEvaporatorCapacity() const;

      bool isReferenceCoolingModeEvaporatorCapacityAutosized() const;

      double referenceCoolingModeCOP() const;

      double referenceCoolingModeLeavingChilledWaterTemperature() const;

      double referenceCoolingModeEnteringCondenserFluidTemperature() const;

      double referenceCoolingModeLeavingCondenserWaterTemperature() const;

      double referenceHeatingModeCoolingCapacityRatio() const;

      double referenceHeatingModeCoolingPowerInputRatio() const;

      double referenceHeatingModeLeavingChilledWaterTemperature() const;

      double referenceHeatingModeLeavingCondenserWaterTemperature() const;

      double referenceHeatingModeEnteringCondenserFluidTemperature() const;

      double heatingModeEnteringChilledWaterTemperatureLowLimit() const;

      std::string chilledWaterFlowModeType() const;

      boost::optional<double> designChilledWaterFlowRate() const;

      bool isDesignChilledWaterFlowRateAutosized() const;

      boost::optional<double> designCondenserWaterFlowRate() const;

      bool isDesignCondenserWaterFlowRateAutosized() const;

      double designHotWaterFlowRate() const;

      double compressorMotorEfficiency() const;

      std::string condenserType() const;

      std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable() const;

      Curve coolingModeCoolingCapacityFunctionOfTemperatureCurve() const;

      Curve coolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const;

      Curve coolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const;

      double coolingModeCoolingCapacityOptimumPartLoadRatio() const;

      std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable() const;

      Curve heatingModeCoolingCapacityFunctionOfTemperatureCurve() const;

      Curve heatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const;

      Curve heatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const;

      double heatingModeCoolingCapacityOptimumPartLoadRatio() const;

      double sizingFactor() const;

      boost::optional<double> autosizedReferenceCoolingModeEvaporatorCapacity() const;

      boost::optional<double> autosizedDesignChilledWaterFlowRate() const;

      boost::optional<double> autosizedDesignCondenserWaterFlowRate() const;

      void autosize();

      void applySizingValues();

      //@}
      /** @name Setters */
      //@{

      bool setReferenceCoolingModeEvaporatorCapacity(boost::optional<double> referenceCoolingModeEvaporatorCapacity);

      void autosizeReferenceCoolingModeEvaporatorCapacity();

      bool setReferenceCoolingModeCOP(double referenceCoolingModeCOP);

      bool setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature);

      bool setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature);

      bool setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature);

      bool setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio);

      bool setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio);

      bool setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature);

      bool setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature);

      bool setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature);

      bool setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit);

      bool setChilledWaterFlowModeType(const std::string& chilledWaterFlowModeType);

      bool setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate);

      void resetDesignChilledWaterFlowRate();

      void autosizeDesignChilledWaterFlowRate();

      bool setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate);

      void resetDesignCondenserWaterFlowRate();

      void autosizeDesignCondenserWaterFlowRate();

      bool setDesignHotWaterFlowRate(double designHotWaterFlowRate);

      bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

      bool setCondenserType(const std::string& condenserType);

      bool setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(
        const std::string& coolingModeTemperatureCurveCondenserWaterIndependentVariable);

      bool setCoolingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

      bool setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve);

      bool setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve);

      bool setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio);

      bool setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(
        const std::string& heatingModeTemperatureCurveCondenserWaterIndependentVariable);

      bool setHeatingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

      bool setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve);

      bool setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve);

      bool setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio);

      bool setSizingFactor(double sizingFactor);

      //@}
      /** @name Other */
      //@{

      // Reverse lookups
      // Convenience functions to return parent CentralHeatPumpSystem and CentralHeatPumpSystemModule
      // I've decided that multiple CentralHeatPumpSystemModules can reference the same ChillerHeaterPerformanceElectricEIR

      /* Returns an Array of CentralHeatPumpSystemModule that reference this object as chillerHeaterModulesPerformanceComponent */
      std::vector<CentralHeatPumpSystemModule> centralHeatPumpSystemModules() const;

      /* Returns an Array of CentralHeatPumpSystem that have a CentralHeatPumpSystemModume that reference this object
     * as chillerHeaterModulesPerformanceComponent */
      std::vector<CentralHeatPumpSystem> centralHeatPumpSystems() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ChillerHeaterPerformanceElectricEIR");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalCoolingModeCoolingCapacityFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const;
      boost::optional<Curve> optionalHeatingModeCoolingCapacityFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_IMPL_HPP
