/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FLUIDCOOLERSINGLESPEED_IMPL_HPP
#define MODEL_FLUIDCOOLERSINGLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    /** FluidCoolerSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for FluidCoolerSingleSpeed.*/
    class MODEL_API FluidCoolerSingleSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FluidCoolerSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FluidCoolerSingleSpeed_Impl(const FluidCoolerSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FluidCoolerSingleSpeed_Impl() override = default;

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

      boost::optional<double> designAirFlowRateUfactorTimesAreaValue() const;

      bool isDesignAirFlowRateUfactorTimesAreaValueAutosized() const;

      double nominalCapacity() const;

      double designEnteringWaterTemperature() const;

      double designEnteringAirTemperature() const;

      double designEnteringAirWetbulbTemperature() const;

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      boost::optional<double> designAirFlowRate() const;

      bool isDesignAirFlowRateAutosized() const;

      boost::optional<double> designAirFlowRateFanPower() const;

      bool isDesignAirFlowRateFanPowerAutosized() const;

      boost::optional<Node> outdoorAirInletNode() const;

      boost::optional<double> autosizedDesignAirFlowRateUfactorTimesAreaValue() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      boost::optional<double> autosizedDesignAirFlowRate() const;

      boost::optional<double> autosizedDesignAirFlowRateFanPower() const;

      //@}
      /** @name Setters */
      //@{

      bool setPerformanceInputMethod(const std::string& performanceInputMethod);

      bool setDesignAirFlowRateUfactorTimesAreaValue(boost::optional<double> designAirFlowRateUfactorTimesAreaValue);

      void autosizeDesignAirFlowRateUfactorTimesAreaValue();

      bool setNominalCapacity(double nominalCapacity);

      bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

      bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

      bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void autosizeDesignWaterFlowRate();

      bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

      void autosizeDesignAirFlowRate();

      bool setDesignAirFlowRateFanPower(boost::optional<double> designAirFlowRateFanPower);

      void autosizeDesignAirFlowRateFanPower();

      bool setOutdoorAirInletNode(const boost::optional<Node>& node);

      void resetOutdoorAirInletNode();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FluidCoolerSingleSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FLUIDCOOLERSINGLESPEED_IMPL_HPP
