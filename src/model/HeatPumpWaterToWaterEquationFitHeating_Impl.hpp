/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class HeatPumpWaterToWaterEquationFitCooling;
  class CurveQuadLinear;

  namespace detail {

    /** HeatPumpWaterToWaterEquationFitHeating_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatPumpWaterToWaterEquationFitHeating.*/
    class MODEL_API HeatPumpWaterToWaterEquationFitHeating_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatPumpWaterToWaterEquationFitHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatPumpWaterToWaterEquationFitHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatPumpWaterToWaterEquationFitHeating_Impl(const HeatPumpWaterToWaterEquationFitHeating_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatPumpWaterToWaterEquationFitHeating_Impl() override = default;

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

      boost::optional<double> ratedHeatingCapacity() const;

      bool isRatedHeatingCapacityAutosized() const;

      boost::optional<double> ratedHeatingPowerConsumption() const;

      bool isRatedHeatingPowerConsumptionAutosized() const;

      CurveQuadLinear heatingCapacityCurve() const;

      CurveQuadLinear heatingCompressorPowerCurve() const;

      double referenceCoefficientofPerformance() const;

      double sizingFactor() const;

      boost::optional<HeatPumpWaterToWaterEquationFitCooling> companionCoolingHeatPump() const;

      boost::optional<double> autosizedReferenceLoadSideFlowRate() const;

      boost::optional<double> autosizedReferenceSourceSideFlowRate() const;

      boost::optional<double> autosizedRatedHeatingCapacity() const;

      boost::optional<double> autosizedRatedHeatingPowerConsumption() const;

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

      bool setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve);

      bool setHeatingCompressorPowerCurve(const CurveQuadLinear& heatingCompressorPowerCurve);

      bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

      bool setSizingFactor(double sizingFactor);

      bool setCompanionCoolingHeatPump(const HeatPumpWaterToWaterEquationFitCooling& companionHP);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitHeating");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_IMPL_HPP
