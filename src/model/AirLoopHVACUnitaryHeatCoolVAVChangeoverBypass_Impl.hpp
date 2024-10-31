/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Mixer;

  namespace detail {

    /** AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl is a StraightComponent_Impl that is the implementation class for AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.*/
    class MODEL_API AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl& other, Model_Impl* model,
                                                         bool keepHandle);

      virtual ~AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> systemAirFlowRateDuringCoolingOperation() const;

      bool isSystemAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> systemAirFlowRateDuringHeatingOperation() const;

      bool isSystemAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> systemAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      bool isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

      boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

      bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

      bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

      boost::optional<Schedule> outdoorAirFlowRateMultiplierSchedule() const;

      HVACComponent supplyAirFan() const;

      std::string supplyAirFanPlacement() const;

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

      HVACComponent coolingCoil() const;

      HVACComponent heatingCoil() const;

      std::string priorityControlMode() const;

      double minimumOutletAirTemperatureDuringCoolingOperation() const;

      double maximumOutletAirTemperatureDuringHeatingOperation() const;

      std::string dehumidificationControlType() const;

      double minimumRuntimeBeforeOperatingModeChange() const;

      unsigned plenumorMixerAirPort() const;
      Node plenumorMixerNode() const;
      boost::optional<Mixer> plenumorMixer() const;

      boost::optional<double> autosizedSystemAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedSystemAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setSystemAirFlowRateDuringCoolingOperation(boost::optional<double> systemAirFlowRateDuringCoolingOperation);

      void autosizeSystemAirFlowRateDuringCoolingOperation();

      bool setSystemAirFlowRateDuringHeatingOperation(boost::optional<double> systemAirFlowRateDuringHeatingOperation);

      void autosizeSystemAirFlowRateDuringHeatingOperation();

      bool setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> systemAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

      void autosizeOutdoorAirFlowRateDuringCoolingOperation();

      bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

      void autosizeOutdoorAirFlowRateDuringHeatingOperation();

      bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setOutdoorAirFlowRateMultiplierSchedule(Schedule& outdoorAirFlowRateMultiplierSchedule);

      void resetOutdoorAirFlowRateMultiplierSchedule();

      bool setSupplyAirFan(const HVACComponent& fansCVandOnOff);

      bool setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement);

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      void resetSupplyAirFanOperatingModeSchedule();

      bool setCoolingCoil(const HVACComponent& coolingCoilName);

      bool setHeatingCoil(const HVACComponent& heatingCoilName);

      bool setPriorityControlMode(const std::string& priorityControlMode);

      bool setMinimumOutletAirTemperatureDuringCoolingOperation(double minimumOutletAirTemperatureDuringCoolingOperation);

      bool setMaximumOutletAirTemperatureDuringHeatingOperation(double maximumOutletAirTemperatureDuringHeatingOperation);

      bool setDehumidificationControlType(const std::string& dehumidificationControlType);

      bool setMinimumRuntimeBeforeOperatingModeChange(double runtime);

      bool setPlenumorMixer(const Mixer& returnPathComponent);

      void resetPlenumorMixer();

      //@}
      /** @name Other */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass");

      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_IMPL_HPP
