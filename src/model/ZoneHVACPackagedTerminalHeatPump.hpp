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

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACPackagedTerminalHeatPump_Impl;

  }  // namespace detail

  /** ZoneHVACPackagedTerminalHeatPump is a ZoneHVACComponent that wraps the
 *  OpenStudio IDD object 'OS:ZoneHVAC:PackagedTerminalHeatPump'. */
  class MODEL_API ZoneHVACPackagedTerminalHeatPump : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACPackagedTerminalHeatPump(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan, HVACComponent& heatingCoil,
                                     HVACComponent& coolingCoil, HVACComponent& supplementalHeatingCoil);

    virtual ~ZoneHVACPackagedTerminalHeatPump() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACPackagedTerminalHeatPump(const ZoneHVACPackagedTerminalHeatPump& other) = default;
    ZoneHVACPackagedTerminalHeatPump(ZoneHVACPackagedTerminalHeatPump&& other) = default;
    ZoneHVACPackagedTerminalHeatPump& operator=(const ZoneHVACPackagedTerminalHeatPump&) = default;
    ZoneHVACPackagedTerminalHeatPump& operator=(ZoneHVACPackagedTerminalHeatPump&&) = default;

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

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    /** Not used as of EnergyPlus 8.8.0 and above **/
    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

    HVACComponent coolingCoil() const;

    double coolingConvergenceTolerance() const;

    bool isCoolingConvergenceToleranceDefaulted() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

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

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setSupplyAirFan(HVACComponent& hvacComponent);

    bool setHeatingCoil(HVACComponent& hvacComponent);

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    /** Not used as of EnergyPlus 8.8.0 and above **/
    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

    bool setCoolingCoil(HVACComponent& hvacComponent);

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    void resetCoolingConvergenceTolerance();

    bool setSupplementalHeatingCoil(HVACComponent& hvacComponent);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

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

    boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACPackagedTerminalHeatPump_Impl;

    explicit ZoneHVACPackagedTerminalHeatPump(std::shared_ptr<detail::ZoneHVACPackagedTerminalHeatPump_Impl> impl);

    friend class detail::ZoneHVACPackagedTerminalHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalHeatPump");
  };

  /** \relates ZoneHVACPackagedTerminalHeatPump*/
  using OptionalZoneHVACPackagedTerminalHeatPump = boost::optional<ZoneHVACPackagedTerminalHeatPump>;

  /** \relates ZoneHVACPackagedTerminalHeatPump*/
  using ZoneHVACPackagedTerminalHeatPumpVector = std::vector<ZoneHVACPackagedTerminalHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_HPP
