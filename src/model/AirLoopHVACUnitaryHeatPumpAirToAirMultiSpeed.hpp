/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl;

  }  // namespace detail

  /** AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatPump:AirToAir:MultiSpeed'. */
  class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(const Model& model, const HVACComponent& fan, const HVACComponent& heatingCoil,
                                                          const HVACComponent& coolingCoil, const HVACComponent& supplementalHeatingCoil);

    virtual ~AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(const AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed& other) = default;
    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed&& other) = default;
    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed& operator=(const AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed&) = default;
    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed& operator=(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> supplyAirFanPlacementValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<ThermalZone> controllingZoneorThermostatLocation() const;

    HVACComponent supplyAirFan() const;

    std::string supplyAirFanPlacement() const;

    Schedule supplyAirFanOperatingModeSchedule() const;

    HVACComponent heatingCoil() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    HVACComponent coolingCoil() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    double auxiliaryOnCycleElectricPower() const;

    double auxiliaryOffCycleElectricPower() const;

    double designHeatRecoveryWaterFlowRate() const;

    double maximumTemperatureforHeatRecovery() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Supply Air Flow Rate" **/
    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    int numberofSpeedsforHeating() const;

    int numberofSpeedsforCooling() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Speed 1 Supply Air Flow Rate" **/
    boost::optional<double> speed1SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Speed 2 Supply Air Flow Rate" **/
    boost::optional<double> speed2SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Speed 3 Supply Air Flow Rate" **/
    boost::optional<double> speed3SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Speed 4 Supply Air Flow Rate" **/
    boost::optional<double> speed4SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Speed 1 Supply Air Flow Rate" **/
    boost::optional<double> speed1SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Speed 2 Supply Air Flow Rate" **/
    boost::optional<double> speed2SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Speed 3 Supply Air Flow Rate" **/
    boost::optional<double> speed3SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Speed 4 Supply Air Flow Rate" **/
    boost::optional<double> speed4SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setControllingZoneorThermostatLocation(const ThermalZone& thermalZone);

    void resetControllingZoneorThermostatLocation();

    bool setSupplyAirFan(const HVACComponent& fan);

    bool setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement);

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    bool setHeatingCoil(const HVACComponent& coil);

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    bool setCoolingCoil(const HVACComponent& coil);

    bool setSupplementalHeatingCoil(const HVACComponent& coil);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    bool setAuxiliaryOnCycleElectricPower(double auxiliaryOnCycleElectricPower);

    bool setAuxiliaryOffCycleElectricPower(double auxiliaryOffCycleElectricPower);

    bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

    bool setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery);

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setNumberofSpeedsforHeating(int numberofSpeedsforHeating);

    bool setNumberofSpeedsforCooling(int numberofSpeedsforCooling);

    bool setSpeed1SupplyAirFlowRateDuringHeatingOperation(double speed1SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed2SupplyAirFlowRateDuringHeatingOperation(double speed2SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed3SupplyAirFlowRateDuringHeatingOperation(double speed3SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed4SupplyAirFlowRateDuringHeatingOperation(double speed4SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed1SupplyAirFlowRateDuringCoolingOperation(double speed1SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();

    bool setSpeed2SupplyAirFlowRateDuringCoolingOperation(double speed2SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();

    bool setSpeed3SupplyAirFlowRateDuringCoolingOperation(double speed3SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();

    bool setSpeed4SupplyAirFlowRateDuringCoolingOperation(double speed4SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

    boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    boost::optional<double> autosizedSpeed1SupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSpeed2SupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSpeed3SupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSpeed4SupplyAirFlowRateDuringHeatingOperation() const;

    boost::optional<double> autosizedSpeed1SupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSpeed2SupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSpeed3SupplyAirFlowRateDuringCoolingOperation() const;

    boost::optional<double> autosizedSpeed4SupplyAirFlowRateDuringCoolingOperation() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl;

    explicit AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl> impl);

    friend class detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed");
  };

  /** \relates AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed*/
  using OptionalAirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed = boost::optional<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>;

  /** \relates AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed*/
  using AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeedVector = std::vector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_HPP
