/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Mixer;

  namespace detail {

    class AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl;

  }  // namespace detail

  /** AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass'. */
  class MODEL_API AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(const Model& model, const HVACComponent& fan, const HVACComponent& coolingCoil,
                                                           const HVACComponent& heatingCoil);

    virtual ~AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(const AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass& other) = default;
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass&& other) = default;
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass& operator=(const AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass&) = default;
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass& operator=(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> supplyAirFanPlacementValues();

    static std::vector<std::string> priorityControlModeValues();

    static std::vector<std::string> dehumidificationControlTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
    boost::optional<double> systemAirFlowRateDuringCoolingOperation() const;

    bool isSystemAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
    boost::optional<double> systemAirFlowRateDuringHeatingOperation() const;

    bool isSystemAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate" **/
    boost::optional<double> systemAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

    bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

    bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Outdoor Air Flow Rate" **/
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

    /** This Node always exists for connecting "Plenum or Mixer Inlet Node", it will be translated only if actually connected to an
   * AirLoopHVAC:ReturnPlenum or an AirLoopHVAC:ZoneMixer */
    Node plenumorMixerNode() const;

    /** Convenience method to get the optional linked Mixer object (AirLoopHVAC:ReturnPlenum or AirLoopHVAC:ZoneMixer) */
    boost::optional<Mixer> plenumorMixer() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setSystemAirFlowRateDuringCoolingOperation(double systemAirFlowRateDuringCoolingOperation);

    void autosizeSystemAirFlowRateDuringCoolingOperation();

    bool setSystemAirFlowRateDuringHeatingOperation(double systemAirFlowRateDuringHeatingOperation);

    void autosizeSystemAirFlowRateDuringHeatingOperation();

    bool setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(double systemAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

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

    /** Connect the bypass air duct to an AirLoopHVAC:ReturnPlenum or an AirLoopHVAC:ZoneMixer that must
   * be on the same AirLoopHVAC as this Unitary System */
    bool setPlenumorMixer(const Mixer& returnPathComponent);

    void resetPlenumorMixer();

    boost::optional<double> autosizedSystemAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSystemAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    //@}
    /** @name Other */
    //@{
    //@}
   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl;

    explicit AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(std::shared_ptr<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl> impl);

    friend class detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass");
  };

  /** \relates AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass*/
  using OptionalAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass = boost::optional<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>;

  /** \relates AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass*/
  using AirLoopHVACUnitaryHeatCoolVAVChangeoverBypassVector = std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP
