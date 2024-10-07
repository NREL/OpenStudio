/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FLUIDCOOLERTWOSPEED_IMPL_HPP
#define MODEL_FLUIDCOOLERTWOSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    /** FluidCoolerTwoSpeed_Impl is a StraightComponent_Impl that is the implementation class for FluidCoolerTwoSpeed.*/
    class MODEL_API FluidCoolerTwoSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FluidCoolerTwoSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FluidCoolerTwoSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FluidCoolerTwoSpeed_Impl(const FluidCoolerTwoSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FluidCoolerTwoSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string performanceInputMethod() const;

      boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

      bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

      boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

      bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

      double lowFanSpeedUFactorTimesAreaSizingFactor() const;

      double highSpeedNominalCapacity() const;

      boost::optional<double> lowSpeedNominalCapacity() const;

      bool isLowSpeedNominalCapacityAutosized() const;

      double lowSpeedNominalCapacitySizingFactor() const;

      double designEnteringWaterTemperature() const;

      double designEnteringAirTemperature() const;

      double designEnteringAirWetbulbTemperature() const;

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      boost::optional<double> highFanSpeedAirFlowRate() const;

      bool isHighFanSpeedAirFlowRateAutosized() const;

      boost::optional<double> highFanSpeedFanPower() const;

      bool isHighFanSpeedFanPowerAutosized() const;

      boost::optional<double> lowFanSpeedAirFlowRate() const;

      bool isLowFanSpeedAirFlowRateAutosized() const;

      double lowFanSpeedAirFlowRateSizingFactor() const;

      boost::optional<double> lowFanSpeedFanPower() const;

      bool isLowFanSpeedFanPowerAutosized() const;

      double lowFanSpeedFanPowerSizingFactor() const;

      boost::optional<Node> outdoorAirInletNode() const;

      boost::optional<double> autosizedHighFanSpeedUfactorTimesAreaValue() const;

      boost::optional<double> autosizedLowFanSpeedUfactorTimesAreaValue() const;

      boost::optional<double> autosizedLowSpeedNominalCapacity() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      boost::optional<double> autosizedHighFanSpeedAirFlowRate() const;

      boost::optional<double> autosizedHighFanSpeedFanPower() const;

      boost::optional<double> autosizedLowFanSpeedAirFlowRate() const;

      boost::optional<double> autosizedLowFanSpeedFanPower() const;

      //@}
      /** @name Setters */
      //@{

      bool setPerformanceInputMethod(const std::string& performanceInputMethod);

      bool setHighFanSpeedUfactorTimesAreaValue(boost::optional<double> highFanSpeedUfactorTimesAreaValue);

      void autosizeHighFanSpeedUfactorTimesAreaValue();

      bool setLowFanSpeedUfactorTimesAreaValue(boost::optional<double> lowFanSpeedUfactorTimesAreaValue);

      void autosizeLowFanSpeedUfactorTimesAreaValue();

      bool setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

      bool setHighSpeedNominalCapacity(double highSpeedNominalCapacity);

      bool setLowSpeedNominalCapacity(boost::optional<double> lowSpeedNominalCapacity);

      void autosizeLowSpeedNominalCapacity();

      bool setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor);

      bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

      bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

      bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void autosizeDesignWaterFlowRate();

      bool setHighFanSpeedAirFlowRate(boost::optional<double> highFanSpeedAirFlowRate);

      void autosizeHighFanSpeedAirFlowRate();

      bool setHighFanSpeedFanPower(boost::optional<double> highFanSpeedFanPower);

      void autosizeHighFanSpeedFanPower();

      bool setLowFanSpeedAirFlowRate(boost::optional<double> lowFanSpeedAirFlowRate);

      void autosizeLowFanSpeedAirFlowRate();

      bool setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

      bool setLowFanSpeedFanPower(boost::optional<double> lowFanSpeedFanPower);

      void autosizeLowFanSpeedFanPower();

      bool setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

      bool setOutdoorAirInletNode(const boost::optional<Node>& node);

      void resetOutdoorAirInletNode();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FluidCoolerTwoSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FLUIDCOOLERTWOSPEED_IMPL_HPP
