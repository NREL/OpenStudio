/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP
#define MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;

  namespace detail {

    /** HeatExchangerFluidToFluid_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatExchangerFluidToFluid.*/
    class MODEL_API HeatExchangerFluidToFluid_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatExchangerFluidToFluid_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatExchangerFluidToFluid_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatExchangerFluidToFluid_Impl(const HeatExchangerFluidToFluid_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatExchangerFluidToFluid_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> loopDemandSideDesignFlowRate() const;

      bool isLoopDemandSideDesignFlowRateAutosized() const;

      boost::optional<double> loopSupplySideDesignFlowRate() const;

      bool isLoopSupplySideDesignFlowRateAutosized() const;

      std::string heatExchangeModelType() const;

      bool isHeatExchangeModelTypeDefaulted() const;

      boost::optional<double> heatExchangerUFactorTimesAreaValue() const;

      bool isHeatExchangerUFactorTimesAreaValueAutosized() const;

      std::string controlType() const;

      bool isControlTypeDefaulted() const;

      double minimumTemperatureDifferencetoActivateHeatExchanger() const;

      bool isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted() const;

      std::string heatTransferMeteringEndUseType() const;

      bool isHeatTransferMeteringEndUseTypeDefaulted() const;

      boost::optional<Node> componentOverrideLoopSupplySideInletNode() const;

      boost::optional<Node> componentOverrideLoopDemandSideInletNode() const;

      std::string componentOverrideCoolingControlTemperatureMode() const;

      bool isComponentOverrideCoolingControlTemperatureModeDefaulted() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      boost::optional<double> operationMinimumTemperatureLimit() const;

      boost::optional<double> operationMaximumTemperatureLimit() const;

      boost::optional<double> autosizedLoopDemandSideDesignFlowRate() const;

      boost::optional<double> autosizedLoopSupplySideDesignFlowRate() const;

      boost::optional<double> autosizedHeatExchangerUFactorTimesAreaValue() const;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Schedule.
      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setLoopDemandSideDesignFlowRate(boost::optional<double> loopDemandSideDesignFlowRate);

      void autosizeLoopDemandSideDesignFlowRate();

      bool setLoopSupplySideDesignFlowRate(boost::optional<double> loopSupplySideDesignFlowRate);

      void autosizeLoopSupplySideDesignFlowRate();

      bool setHeatExchangeModelType(const std::string& heatExchangeModelType);

      void resetHeatExchangeModelType();

      bool setHeatExchangerUFactorTimesAreaValue(boost::optional<double> heatExchangerUFactorTimesAreaValue);

      void autosizeHeatExchangerUFactorTimesAreaValue();

      bool setControlType(const std::string& controlType);

      void resetControlType();

      bool setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger);

      void resetMinimumTemperatureDifferencetoActivateHeatExchanger();

      bool setHeatTransferMeteringEndUseType(const std::string& heatTransferMeteringEndUseType);

      void resetHeatTransferMeteringEndUseType();

      bool setComponentOverrideLoopSupplySideInletNode(const boost::optional<Node>& node);

      void resetComponentOverrideLoopSupplySideInletNode();

      bool setComponentOverrideLoopDemandSideInletNode(const boost::optional<Node>& node);

      void resetComponentOverrideLoopDemandSideInletNode();

      bool setComponentOverrideCoolingControlTemperatureMode(const std::string& componentOverrideCoolingControlTemperatureMode);

      void resetComponentOverrideCoolingControlTemperatureMode();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setOperationMinimumTemperatureLimit(boost::optional<double> operationMinimumTemperatureLimit);

      void resetOperationMinimumTemperatureLimit();

      bool setOperationMaximumTemperatureLimit(boost::optional<double> operationMaximumTemperatureLimit);

      void resetOperationMaximumTemperatureLimit();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatExchangerFluidToFluid");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP
