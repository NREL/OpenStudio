/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALAIRCONDITIONER_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALAIRCONDITIONER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACPackagedTerminalAirConditioner_Impl;

  }  // namespace detail

  /** ZoneHVACPackagedTerminalAirConditioner is a ZoneHVACComponent that wraps the
 *  OpenStudio IDD object 'OS:ZoneHVAC:PackagedTerminalAirConditioner'. */
  class MODEL_API ZoneHVACPackagedTerminalAirConditioner : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACPackagedTerminalAirConditioner(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan,
                                           HVACComponent& heatingCoil, HVACComponent& coolingCoil);

    virtual ~ZoneHVACPackagedTerminalAirConditioner() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACPackagedTerminalAirConditioner(const ZoneHVACPackagedTerminalAirConditioner& other) = default;
    ZoneHVACPackagedTerminalAirConditioner(ZoneHVACPackagedTerminalAirConditioner&& other) = default;
    ZoneHVACPackagedTerminalAirConditioner& operator=(const ZoneHVACPackagedTerminalAirConditioner&) = default;
    ZoneHVACPackagedTerminalAirConditioner& operator=(ZoneHVACPackagedTerminalAirConditioner&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> outdoorAirMixerObjectTypeValues();

    /** \deprecated */
    static std::vector<std::string> validOutdoorAirMixerObjectTypeValues();

    static std::vector<std::string> fanPlacementValues();

    /** \deprecated */
    static std::vector<std::string> validFanPlacementValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string outdoorAirMixerObjectType() const;

    std::string outdoorAirMixerName() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

    bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

    bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    bool noLoadSupplyAirFlowRateControlSetToLowSpeed() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

    bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

    bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Outdoor Air Flow Rate" **/
    boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    HVACComponent supplyAirFan() const;

    HVACComponent heatingCoil() const;

    HVACComponent coolingCoil() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    Schedule supplyAirFanOperatingModeSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setOutdoorAirMixerObjectType(const std::string& outdoorAirMixerObjectType);

    bool setOutdoorAirMixerName(const std::string& outdoorAirMixerName);

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setNoLoadSupplyAirFlowRateControlSetToLowSpeed(bool noLoadSupplyAirFlowRateControlSetToLowSpeed);

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setSupplyAirFan(HVACComponent& fan);

    bool setHeatingCoil(HVACComponent& heatingCoil);

    bool setCoolingCoil(HVACComponent& coolingCoil);

    bool setFanPlacement(const std::string& fanPlacement);

    void resetFanPlacement();

    /** Sets the supply air fan operating mode schedule. Values of zero indicate that the fan
   *  cycles on and off with the active (heating or cooling) coil. Other values operate the
   *  fan continuously. */
    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACPackagedTerminalAirConditioner_Impl;

    explicit ZoneHVACPackagedTerminalAirConditioner(std::shared_ptr<detail::ZoneHVACPackagedTerminalAirConditioner_Impl> impl);

    friend class detail::ZoneHVACPackagedTerminalAirConditioner_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalAirConditioner");
  };

  /** \relates ZoneHVACPackagedTerminalAirConditioner*/
  using OptionalZoneHVACPackagedTerminalAirConditioner = boost::optional<ZoneHVACPackagedTerminalAirConditioner>;

  /** \relates ZoneHVACPackagedTerminalAirConditioner*/
  using ZoneHVACPackagedTerminalAirConditionerVector = std::vector<ZoneHVACPackagedTerminalAirConditioner>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACPACKAGEDTERMINALAIRCONDITIONER_HPP
