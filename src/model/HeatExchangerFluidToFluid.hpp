/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERFLUIDTOFLUID_HPP
#define MODEL_HEATEXCHANGERFLUIDTOFLUID_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;

  namespace detail {

    class HeatExchangerFluidToFluid_Impl;

  }  // namespace detail

  /** HeatExchangerFluidToFluid is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatExchanger:FluidToFluid'. */
  class MODEL_API HeatExchangerFluidToFluid : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatExchangerFluidToFluid(const Model& model);

    virtual ~HeatExchangerFluidToFluid() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatExchangerFluidToFluid(const HeatExchangerFluidToFluid& other) = default;
    HeatExchangerFluidToFluid(HeatExchangerFluidToFluid&& other) = default;
    HeatExchangerFluidToFluid& operator=(const HeatExchangerFluidToFluid&) = default;
    HeatExchangerFluidToFluid& operator=(HeatExchangerFluidToFluid&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatExchangeModelTypeValues();

    static std::vector<std::string> controlTypeValues();

    static std::vector<std::string> heatTransferMeteringEndUseTypeValues();

    static std::vector<std::string> componentOverrideCoolingControlTemperatureModeValues();

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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setLoopDemandSideDesignFlowRate(double loopDemandSideDesignFlowRate);

    void autosizeLoopDemandSideDesignFlowRate();

    bool setLoopSupplySideDesignFlowRate(double loopSupplySideDesignFlowRate);

    void autosizeLoopSupplySideDesignFlowRate();

    bool setHeatExchangeModelType(const std::string& heatExchangeModelType);

    void resetHeatExchangeModelType();

    bool setHeatExchangerUFactorTimesAreaValue(double heatExchangerUFactorTimesAreaValue);

    void autosizeHeatExchangerUFactorTimesAreaValue();

    bool setControlType(const std::string& controlType);

    void resetControlType();

    bool setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger);

    void resetMinimumTemperatureDifferencetoActivateHeatExchanger();

    bool setHeatTransferMeteringEndUseType(const std::string& heatTransferMeteringEndUseType);

    void resetHeatTransferMeteringEndUseType();

    bool setComponentOverrideLoopSupplySideInletNode(const Node& node);

    void resetComponentOverrideLoopSupplySideInletNode();

    bool setComponentOverrideLoopDemandSideInletNode(const Node& node);

    void resetComponentOverrideLoopDemandSideInletNode();

    bool setComponentOverrideCoolingControlTemperatureMode(const std::string& componentOverrideCoolingControlTemperatureMode);

    void resetComponentOverrideCoolingControlTemperatureMode();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    bool setOperationMinimumTemperatureLimit(double operationMinimumTemperatureLimit);

    void resetOperationMinimumTemperatureLimit();

    bool setOperationMaximumTemperatureLimit(double operationMaximumTemperatureLimit);

    void resetOperationMaximumTemperatureLimit();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedLoopDemandSideDesignFlowRate() const;

    boost::optional<double> autosizedLoopSupplySideDesignFlowRate() const;

    boost::optional<double> autosizedHeatExchangerUFactorTimesAreaValue() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeatExchangerFluidToFluid_Impl;

    explicit HeatExchangerFluidToFluid(std::shared_ptr<detail::HeatExchangerFluidToFluid_Impl> impl);

    friend class detail::HeatExchangerFluidToFluid_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatExchangerFluidToFluid");
  };

  /** \relates HeatExchangerFluidToFluid*/
  using OptionalHeatExchangerFluidToFluid = boost::optional<HeatExchangerFluidToFluid>;

  /** \relates HeatExchangerFluidToFluid*/
  using HeatExchangerFluidToFluidVector = std::vector<HeatExchangerFluidToFluid>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERFLUIDTOFLUID_HPP
