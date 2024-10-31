/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    class MODEL_API CoilCoolingWaterToAirHeatPumpEquationFit_Impl : public WaterToAirComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */

      CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpEquationFit_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingWaterToAirHeatPumpEquationFit_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //virtual bool addToNode(Node & node);

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateDefaulted() const;

      bool isRatedAirFlowRateAutosized() const;

      boost::optional<double> ratedWaterFlowRate() const;

      bool isRatedWaterFlowRateDefaulted() const;

      bool isRatedWaterFlowRateAutosized() const;

      boost::optional<double> ratedTotalCoolingCapacity() const;

      bool isRatedTotalCoolingCapacityDefaulted() const;

      bool isRatedTotalCoolingCapacityAutosized() const;

      boost::optional<double> ratedSensibleCoolingCapacity() const;

      bool isRatedSensibleCoolingCapacityDefaulted() const;

      bool isRatedSensibleCoolingCapacityAutosized() const;

      double ratedCoolingCoefficientofPerformance() const;

      bool isRatedCoolingCoefficientofPerformanceDefaulted() const;

      double ratedEnteringWaterTemperature() const;

      double ratedEnteringAirDryBulbTemperature() const;

      double ratedEnteringAirWetBulbTemperature() const;

      Curve totalCoolingCapacityCurve() const;

      Curve sensibleCoolingCapacityCurve() const;

      Curve coolingPowerConsumptionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      double nominalTimeforCondensateRemovaltoBegin() const;

      bool isNominalTimeforCondensateRemovaltoBeginDefaulted() const;

      double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

      bool isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const;

      double maximumCyclingRate() const;
      double latentCapacityTimeConstant() const;
      double fanDelayTime() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedRatedWaterFlowRate() const;

      boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

      boost::optional<double> autosizedRatedSensibleCoolingCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void resetRatedAirFlowRate();

      void autosizeRatedAirFlowRate();

      bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

      void resetRatedWaterFlowRate();

      void autosizeRatedWaterFlowRate();

      bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

      void resetRatedTotalCoolingCapacity();

      void autosizeRatedTotalCoolingCapacity();

      bool setRatedSensibleCoolingCapacity(boost::optional<double> ratedSensibleCoolingCapacity);

      void resetRatedSensibleCoolingCapacity();

      void autosizeRatedSensibleCoolingCapacity();

      bool setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance);

      bool setRatedEnteringWaterTemperature(double ratedEnteringAirDryBulbTemperature);

      bool setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature);

      bool setRatedEnteringAirWetBulbTemperature(double ratedEnteringAirWetBulbTemperature);

      bool setTotalCoolingCapacityCurve(const Curve& totalCoolingCapacityCurve);

      bool setSensibleCoolingCapacityCurve(const Curve& sensibleCoolingCapacityCurve);

      bool setCoolingPowerConsumptionCurve(const Curve& coolingPowerConsumptionCurve);

      bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

      void resetNominalTimeforCondensateRemovaltoBegin();

      bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

      bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
        double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

      void resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

      bool setMaximumCyclingRate(double maximumCyclingRate);
      bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);
      bool setFanDelayTime(double fanDelayTime);

      //@}
      /** @name Other */
      //@{

      void setConstructorSharedDefaults(const Model& model);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpEquationFit");

      boost::optional<Curve> optionalTotalCoolingCapacityCurve() const;
      boost::optional<Curve> optionalSensibleCoolingCapacityCurve() const;
      boost::optional<Curve> optionalCoolingPowerConsumptionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
