/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPAIRTOWATERFUELFIREDCOOLING_IMPL_HPP
#define MODEL_HEATPUMPAIRTOWATERFUELFIREDCOOLING_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class HeatPumpAirToWaterFuelFiredHeating;
  class Curve;
  // class OutdoorAirNode;

  namespace detail {

    /** HeatPumpAirToWaterFuelFiredCooling_Impl is a StraightComponent_Impl that is the implementation class for HeatPumpAirToWaterFuelFiredCooling.*/
    class MODEL_API HeatPumpAirToWaterFuelFiredCooling_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatPumpAirToWaterFuelFiredCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatPumpAirToWaterFuelFiredCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatPumpAirToWaterFuelFiredCooling_Impl(const HeatPumpAirToWaterFuelFiredCooling_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatPumpAirToWaterFuelFiredCooling_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual bool addToNode(Node& node) override;

      // Resets the Companion Heat Pump
      virtual ModelObject clone(Model model) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      // boost::optional<OutdoorAirNode> airSourceNode() const;

      boost::optional<HeatPumpAirToWaterFuelFiredHeating> companionHeatingHeatPump() const;

      std::string fuelType() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      boost::optional<double> nominalCoolingCapacity() const;

      bool isNominalCoolingCapacityAutosized() const;

      boost::optional<double> autosizedNominalCoolingCapacity();

      double nominalCOP() const;

      boost::optional<double> designFlowRate() const;

      bool isDesignFlowRateAutosized() const;

      boost::optional<double> autosizedDesignFlowRate();

      double designSupplyTemperature() const;

      boost::optional<double> designTemperatureLift() const;

      bool isDesignTemperatureLiftAutosized() const;

      boost::optional<double> autosizedDesignTemperatureLift();

      double sizingFactor() const;

      std::string flowMode() const;

      std::string outdoorAirTemperatureCurveInputVariable() const;

      std::string waterTemperatureCurveInputVariable() const;

      Curve normalizedCapacityFunctionofTemperatureCurve() const;

      Curve fuelEnergyInputRatioFunctionofTemperatureCurve() const;

      Curve fuelEnergyInputRatioFunctionofPLRCurve() const;

      double minimumPartLoadRatio() const;

      double maximumPartLoadRatio() const;

      boost::optional<Curve> cyclingRatioFactorCurve() const;

      double nominalAuxiliaryElectricPower() const;

      boost::optional<Curve> auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const;

      boost::optional<Curve> auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const;

      double standbyElectricPower() const;

      //@}
      /** @name Setters */
      //@{

      // bool setAirSourceNode(const OutdoorAirNode& outdoorAirNode);

      // void resetAirSourceNode();

      bool setCompanionHeatingHeatPump(const HeatPumpAirToWaterFuelFiredHeating& heatPumpAirToWaterFuelFiredHeating);

      void resetCompanionHeatingHeatPump();

      bool setFuelType(const std::string& fuelType);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      bool setNominalCoolingCapacity(double nominalCoolingCapacity);

      void resetNominalCoolingCapacity();

      void autosizeNominalCoolingCapacity();

      bool setNominalCOP(double nominalCOP);

      bool setDesignFlowRate(double designFlowRate);

      void resetDesignFlowRate();

      void autosizeDesignFlowRate();

      bool setDesignSupplyTemperature(double designSupplyTemperature);

      bool setDesignTemperatureLift(double designTemperatureLift);

      void autosizeDesignTemperatureLift();

      bool setSizingFactor(double sizingFactor);

      bool setFlowMode(const std::string& flowMode);

      bool setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable);

      bool setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable);

      bool setNormalizedCapacityFunctionofTemperatureCurve(const Curve& normalizedCapacityFunctionofTemperatureCurve);

      bool setFuelEnergyInputRatioFunctionofTemperatureCurve(const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve);

      bool setFuelEnergyInputRatioFunctionofPLRCurve(const Curve& fuelEnergyInputRatioFunctionofPLRCurve);

      bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

      bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

      bool setCyclingRatioFactorCurve(const Curve& cyclingRatioFactorCurve);

      void resetCyclingRatioFactorCurve();

      bool setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower);

      bool setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(const Curve& auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve);

      void resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();

      bool setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(const Curve& auxiliaryElectricEnergyInputRatioFunctionofPLRCurve);

      void resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();

      bool setStandbyElectricPower(double standbyElectricPower);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatPumpAirToWaterFuelFiredCooling");

      boost::optional<Curve> optionalNormalizedCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalFuelEnergyInputRatioFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalFuelEnergyInputRatioFunctionofPLRCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPAIRTOWATERFUELFIREDCOOLING_IMPL_HPP
