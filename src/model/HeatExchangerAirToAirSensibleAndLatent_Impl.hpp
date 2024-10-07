/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP
#define MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Connection;
  class Curve;

  namespace detail {

    /** HeatExchangerAirToAirSensibleAndLatent_Impl is a AirToAirComponent_Impl that is the implementation class for HeatExchangerAirToAirSensibleAndLatent.*/
    class MODEL_API HeatExchangerAirToAirSensibleAndLatent_Impl : public AirToAirComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      HeatExchangerAirToAirSensibleAndLatent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatExchangerAirToAirSensibleAndLatent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatExchangerAirToAirSensibleAndLatent_Impl(const HeatExchangerAirToAirSensibleAndLatent_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatExchangerAirToAirSensibleAndLatent_Impl() override = default;

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

      virtual void autosize() override;

      virtual void applySizingValues() override;

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

      boost::optional<double> nominalSupplyAirFlowRate() const;

      bool isNominalSupplyAirFlowRateAutosized() const;

      double sensibleEffectivenessat100HeatingAirFlow() const;

      double latentEffectivenessat100HeatingAirFlow() const;

      double sensibleEffectivenessat100CoolingAirFlow() const;

      double latentEffectivenessat100CoolingAirFlow() const;

      double nominalElectricPower() const;

      bool supplyAirOutletTemperatureControl() const;

      std::string heatExchangerType() const;

      std::string frostControlType() const;

      double thresholdTemperature() const;

      bool isThresholdTemperatureDefaulted() const;

      boost::optional<double> initialDefrostTimeFraction() const;

      boost::optional<double> rateofDefrostTimeFractionIncrease() const;

      bool economizerLockout() const;

      boost::optional<Curve> sensibleEffectivenessofHeatingAirFlowCurve() const;

      boost::optional<Curve> latentEffectivenessofHeatingAirFlowCurve() const;

      boost::optional<Curve> sensibleEffectivenessofCoolingAirFlowCurve() const;

      boost::optional<Curve> latentEffectivenessofCoolingAirFlowCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setNominalSupplyAirFlowRate(boost::optional<double> nominalSupplyAirFlowRate);
      void autosizeNominalSupplyAirFlowRate();

      bool setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow);

      bool setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow);

      bool setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow);

      bool setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow);

      bool setNominalElectricPower(double nominalElectricPower);

      bool setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

      bool setHeatExchangerType(const std::string& heatExchangerType);

      bool setFrostControlType(const std::string& frostControlType);

      bool setThresholdTemperature(double thresholdTemperature);
      void resetThresholdTemperature();

      bool setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction);
      void resetInitialDefrostTimeFraction();

      bool setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease);
      void resetRateofDefrostTimeFractionIncrease();

      bool setEconomizerLockout(bool economizerLockout);

      bool setSensibleEffectivenessofHeatingAirFlowCurve(const Curve& sensibleEffectivenessofHeatingAirFlowCurve);
      void resetSensibleEffectivenessofHeatingAirFlowCurve();

      bool setLatentEffectivenessofHeatingAirFlowCurve(const Curve& latentEffectivenessofHeatingAirFlowCurve);
      void resetLatentEffectivenessofHeatingAirFlowCurve();

      bool setSensibleEffectivenessofCoolingAirFlowCurve(const Curve& sensibleEffectivenessofCoolingAirFlowCurve);
      void resetSensibleEffectivenessofCoolingAirFlowCurve();

      bool setLatentEffectivenessofCoolingAirFlowCurve(const Curve& latentEffectivenessofCoolingAirFlowCurve);
      void resetLatentEffectivenessofCoolingAirFlowCurve();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      boost::optional<double> autosizedNominalSupplyAirFlowRate() const;

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatExchangerAirToAirSensibleAndLatent");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      std::vector<std::string> heatExchangerTypeValues() const;
      std::vector<std::string> frostControlTypeValues() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_IMPL_HPP
