/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOW_HPP
#define MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOW_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class AirflowNetworkEquivalentDuct;
  class HeatExchangerDesiccantBalancedFlowPerformanceDataType1;

  namespace detail {

    class HeatExchangerDesiccantBalancedFlow_Impl;

  }  // namespace detail

  /** HeatExchangerDesiccantBalancedFlow is a AirToAirComponent that wraps the OpenStudio IDD object 'OS:HeatExchanger:AirToAir:SensibleAndLatent'. */
  class MODEL_API HeatExchangerDesiccantBalancedFlow : public AirToAirComponent
  {

   public:
    explicit HeatExchangerDesiccantBalancedFlow(const Model& model);

    explicit HeatExchangerDesiccantBalancedFlow(const Model& model,
                                                const HeatExchangerDesiccantBalancedFlowPerformanceDataType1& heatExchangerPerformance);

    virtual ~HeatExchangerDesiccantBalancedFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatExchangerDesiccantBalancedFlow(const HeatExchangerDesiccantBalancedFlow& other) = default;
    HeatExchangerDesiccantBalancedFlow(HeatExchangerDesiccantBalancedFlow&& other) = default;
    HeatExchangerDesiccantBalancedFlow& operator=(const HeatExchangerDesiccantBalancedFlow&) = default;
    HeatExchangerDesiccantBalancedFlow& operator=(HeatExchangerDesiccantBalancedFlow&&) = default;

    static IddObjectType iddObjectType();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1 heatExchangerPerformance() const;

    bool setHeatExchangerPerformance(const HeatExchangerDesiccantBalancedFlowPerformanceDataType1& heatExchangerPerformance);

    bool economizerLockout() const;

    bool setEconomizerLockout(bool economizerLockout);

    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

   protected:
    /// @cond
    using ImplType = detail::HeatExchangerDesiccantBalancedFlow_Impl;

    explicit HeatExchangerDesiccantBalancedFlow(std::shared_ptr<detail::HeatExchangerDesiccantBalancedFlow_Impl> impl);

    friend class detail::HeatExchangerDesiccantBalancedFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.HeatExchangerDesiccantBalancedFlow");
  };

  /** \relates HeatExchangerDesiccantBalancedFlow*/
  using OptionalHeatExchangerDesiccantBalancedFlow = boost::optional<HeatExchangerDesiccantBalancedFlow>;

  /** \relates HeatExchangerDesiccantBalancedFlow*/
  using HeatExchangerDesiccantBalancedFlowVector = std::vector<HeatExchangerDesiccantBalancedFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOW_HPP
