/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOW_IMPL_HPP
#define MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Connection;
  class HeatExchangerDesiccantBalancedFlowPerformanceDataType1;

  namespace detail {

    /** HeatExchangerDesiccantBalancedFlow_Impl is a AirToAirComponent_Impl that is the implementation class for HeatExchangerDesiccantBalancedFlow.*/
    class MODEL_API HeatExchangerDesiccantBalancedFlow_Impl : public AirToAirComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      HeatExchangerDesiccantBalancedFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatExchangerDesiccantBalancedFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatExchangerDesiccantBalancedFlow_Impl(const HeatExchangerDesiccantBalancedFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatExchangerDesiccantBalancedFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // Returns an eventual CoilSystemCooling Water/DX HeatExchangerAssisted
      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      /* Override disallows addToNode when part of a CoilSystemCooling Water/DX HeatExchangerAssisted
     * otherwise calls parent AirToAirComponent_Impl::addToNode
     */
      virtual bool addToNode(Node& node) override;

      virtual unsigned primaryAirInletPort() const override;

      virtual unsigned primaryAirOutletPort() const override;

      virtual unsigned secondaryAirInletPort() const override;

      virtual unsigned secondaryAirOutletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      HeatExchangerDesiccantBalancedFlowPerformanceDataType1 heatExchangerPerformance() const;

      bool economizerLockout() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatExchangerPerformance(const HeatExchangerDesiccantBalancedFlowPerformanceDataType1& heatExchangerPerformance);

      bool setEconomizerLockout(bool economizerLockout);

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatExchangerDesiccantBalancedFlow");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HeatExchangerDesiccantBalancedFlowPerformanceDataType1> optionalHeatExchangerPerformance() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOW_IMPL_HPP
