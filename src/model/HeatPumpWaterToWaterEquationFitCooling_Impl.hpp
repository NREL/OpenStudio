/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class HeatPumpWaterToWaterEquationFitHeating;
  class CurveQuadLinear;

  namespace detail {

    /** HeatPumpWaterToWaterEquationFitCooling_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpWaterToWaterEquationFitCooling.*/
    class MODEL_API HeatPumpWaterToWaterEquationFitCooling_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatPumpWaterToWaterEquationFitCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatPumpWaterToWaterEquationFitCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatPumpWaterToWaterEquationFitCooling_Impl(const HeatPumpWaterToWaterEquationFitCooling_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatPumpWaterToWaterEquationFitCooling_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned supplyInletPort() const override;

      virtual unsigned supplyOutletPort() const override;

      virtual unsigned demandInletPort() const override;

      virtual unsigned demandOutletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> referenceLoadSideFlowRate() const;

      double ratedLoadSideFlowRate() const;

      bool isReferenceLoadSideFlowRateAutosized() const;

      boost::optional<double> referenceSourceSideFlowRate() const;

      double ratedSourceSideFlowRate() const;

      bool isReferenceSourceSideFlowRateAutosized() const;

      boost::optional<double> ratedCoolingCapacity() const;

      bool isRatedCoolingCapacityAutosized() const;

      boost::optional<double> ratedCoolingPowerConsumption() const;

      bool isRatedCoolingPowerConsumptionAutosized() const;

      CurveQuadLinear coolingCapacityCurve() const;

      CurveQuadLinear coolingCompressorPowerCurve() const;

      double referenceCoefficientofPerformance() const;

      double sizingFactor() const;

      boost::optional<HeatPumpWaterToWaterEquationFitHeating> companionHeatingHeatPump() const;

      boost::optional<double> autosizedReferenceLoadSideFlowRate() const;

      boost::optional<double> autosizedReferenceSourceSideFlowRate() const;

      boost::optional<double> autosizedRatedCoolingCapacity() const;

      boost::optional<double> autosizedRatedCoolingPowerConsumption() const;

      //@}
      /** @name Setters */
      //@{

      bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

      void autosizeReferenceLoadSideFlowRate();

      bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

      void autosizeReferenceSourceSideFlowRate();

      bool setRatedCoolingCapacity(double ratedCoolingCapacity);

      void autosizeRatedCoolingCapacity();

      bool setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption);

      void autosizeRatedCoolingPowerConsumption();

      bool setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve);

      bool setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve);

      bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

      bool setSizingFactor(double sizingFactor);

      bool setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitCooling");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_IMPL_HPP
