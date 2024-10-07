/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** CoilHeatingWaterToAirHeatPumpEquationFit_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilHeatingWaterToAirHeatPumpEquationFit.*/
    class MODEL_API CoilHeatingWaterToAirHeatPumpEquationFit_Impl : public WaterToAirComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpEquationFit_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWaterToAirHeatPumpEquationFit_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      boost::optional<double> ratedHeatingCapacity() const;

      bool isRatedHeatingCapacityDefaulted() const;

      bool isRatedHeatingCapacityAutosized() const;

      double ratedHeatingCoefficientofPerformance() const;

      bool isRatedHeatingCoefficientofPerformanceDefaulted() const;

      double ratedEnteringWaterTemperature() const;

      double ratedEnteringAirDryBulbTemperature() const;

      double ratioofRatedHeatingCapacitytoRatedCoolingCapacity() const;

      Curve heatingCapacityCurve() const;

      Curve heatingPowerConsumptionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedRatedWaterFlowRate() const;

      boost::optional<double> autosizedRatedHeatingCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      bool setRatedAirFlowRate(double ratedAirFlowRate);

      void resetRatedAirFlowRate();

      void autosizeRatedAirFlowRate();

      bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

      bool setRatedWaterFlowRate(double ratedWaterFlowRate);

      void resetRatedWaterFlowRate();

      void autosizeRatedWaterFlowRate();

      bool setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity);

      bool setRatedHeatingCapacity(double ratedHeatingCapacity);

      void resetRatedHeatingCapacity();

      void autosizeRatedHeatingCapacity();

      bool setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance);

      void resetRatedHeatingCoefficientofPerformance();

      bool setRatedEnteringWaterTemperature(double ratedEnteringWaterTemperature);

      bool setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature);

      bool setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(double ratioofRatedHeatingCapacitytoRatedCoolingCapacity);

      bool setHeatingCapacityCurve(const Curve& heatingCapacityCurve);

      bool setHeatingPowerConsumptionCurve(const Curve& heatingPowerConsumptionCurve);

      bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpEquationFit");

      boost::optional<Curve> optionalHeatingCapacityCurve() const;
      boost::optional<Curve> optionalHeatingPowerConsumptionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
