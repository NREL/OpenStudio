/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BOILERHOTWATER_IMPL_HPP
#define MODEL_BOILERHOTWATER_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** BoilerHotWater_Impl is a StraightComponent_Impl that is the implementation class for BoilerHotWater.*/
    class MODEL_API BoilerHotWater_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      BoilerHotWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      BoilerHotWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      BoilerHotWater_Impl(const BoilerHotWater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~BoilerHotWater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string fuelType() const;

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      double nominalThermalEfficiency() const;

      boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable() const;

      boost::optional<Curve> normalizedBoilerEfficiencyCurve() const;

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      double minimumPartLoadRatio() const;

      bool isMinimumPartLoadRatioDefaulted() const;

      double maximumPartLoadRatio() const;

      bool isMaximumPartLoadRatioDefaulted() const;

      double optimumPartLoadRatio() const;

      bool isOptimumPartLoadRatioDefaulted() const;

      double waterOutletUpperTemperatureLimit() const;

      bool isWaterOutletUpperTemperatureLimitDefaulted() const;

      std::string boilerFlowMode() const;

      bool isBoilerFlowModeDefaulted() const;

      double onCycleParasiticElectricLoad() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      boost::optional<double> autosizedNominalCapacity() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      std::string endUseSubcategory() const;

      double offCycleParasiticFuelLoad() const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelType(const std::string& fuelType);

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      void autosizeNominalCapacity();

      bool setNominalThermalEfficiency(double nominalThermalEfficiency);

      bool setEfficiencyCurveTemperatureEvaluationVariable(boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable);

      void resetEfficiencyCurveTemperatureEvaluationVariable();

      bool setNormalizedBoilerEfficiencyCurve(const boost::optional<Curve>& normalizedBoilerEfficiencyCurve);

      void resetNormalizedBoilerEfficiencyCurve();

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void resetDesignWaterFlowRate();

      void autosizeDesignWaterFlowRate();

      bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

      void resetMinimumPartLoadRatio();

      bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

      void resetMaximumPartLoadRatio();

      bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

      void resetOptimumPartLoadRatio();

      bool setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit);

      void resetWaterOutletUpperTemperatureLimit();

      bool setBoilerFlowMode(const std::string& boilerFlowMode);

      void resetBoilerFlowMode();

      bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      bool setOffCycleParasiticFuelLoad(double offCycleParasiticFuelLoad);

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.BoilerHotWater");

      boost::optional<ModelObject> normalizedBoilerEfficiencyCurveAsModelObject() const;

      bool setNormalizedBoilerEfficiencyCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BOILERHOTWATER_IMPL_HPP
