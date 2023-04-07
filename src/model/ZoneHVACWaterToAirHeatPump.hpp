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

#ifndef MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP
#define MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class HVACComponent;

  namespace detail {

    class ZoneHVACWaterToAirHeatPump_Impl;

  }  // namespace detail

  /** ZoneHVACWaterToAirHeatPump is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:WaterToAirHeatPump'. */
  class MODEL_API ZoneHVACWaterToAirHeatPump : public ZoneHVACComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACWaterToAirHeatPump(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan, HVACComponent& heatingCoil,
                               HVACComponent& coolingCoil, HVACComponent& supplementalHeatingCoil);

    virtual ~ZoneHVACWaterToAirHeatPump() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACWaterToAirHeatPump(const ZoneHVACWaterToAirHeatPump& other) = default;
    ZoneHVACWaterToAirHeatPump(ZoneHVACWaterToAirHeatPump&& other) = default;
    ZoneHVACWaterToAirHeatPump& operator=(const ZoneHVACWaterToAirHeatPump&) = default;
    ZoneHVACWaterToAirHeatPump& operator=(ZoneHVACWaterToAirHeatPump&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fanPlacementValues();

    static std::vector<std::string> heatPumpCoilWaterFlowModeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

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

    HVACComponent coolingCoil() const;

    double maximumCyclingRate() const;

    bool isMaximumCyclingRateDefaulted() const;

    double heatPumpTimeConstant() const;

    bool isHeatPumpTimeConstantDefaulted() const;

    double fractionofOnCyclePowerUse() const;

    bool isFractionofOnCyclePowerUseDefaulted() const;

    double heatPumpFanDelayTime() const;

    bool isHeatPumpFanDelayTimeDefaulted() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    std::string heatPumpCoilWaterFlowMode() const;

    bool isHeatPumpCoilWaterFlowModeDefaulted() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    // TODO: field 'Availability Manager List Name' isn't implemented

    // TODO: field 'Design Specification ZoneHVAC Sizing' isn't implemented since the object isn't wrapped in SDK

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

    bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

    void resetSupplyAirFlowRateDuringCoolingOperation();

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);

    bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

    void resetSupplyAirFlowRateDuringHeatingOperation();

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

    bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

    void resetOutdoorAirFlowRateDuringCoolingOperation();

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

    bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

    void resetOutdoorAirFlowRateDuringHeatingOperation();

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setSupplyAirFan(HVACComponent& fansOnOff);

    bool setHeatingCoil(HVACComponent& heatingCoils);

    bool setCoolingCoil(HVACComponent& coolingCoils);

    bool setMaximumCyclingRate(boost::optional<double> maximumCyclingRate);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    void resetMaximumCyclingRate();

    bool setHeatPumpTimeConstant(boost::optional<double> heatPumpTimeConstant);

    bool setHeatPumpTimeConstant(double heatPumpTimeConstant);

    void resetHeatPumpTimeConstant();

    bool setFractionofOnCyclePowerUse(boost::optional<double> fractionofOnCyclePowerUse);

    bool setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse);

    void resetFractionofOnCyclePowerUse();

    bool setHeatPumpFanDelayTime(boost::optional<double> heatPumpFanDelayTime);

    bool setHeatPumpFanDelayTime(double heatPumpFanDelayTime);

    void resetHeatPumpFanDelayTime();

    bool setSupplementalHeatingCoil(HVACComponent& heatingCoilName);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

    bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);

    void resetMaximumSupplyAirTemperaturefromSupplementalHeater();

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      boost::optional<double> maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    // DLM: this appears to not be implemented
    //bool setOutdoorDryBulbTemperatureSensorNodeName(const std::string& outdoorDryBulbTemperatureSensorNodeName);

    bool setFanPlacement(const std::string& fanPlacement);

    void resetFanPlacement();

    bool setHeatPumpCoilWaterFlowMode(const std::string& heatPumpCoilWaterFlowMode);

    void resetHeatPumpCoilWaterFlowMode();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

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
    using ImplType = detail::ZoneHVACWaterToAirHeatPump_Impl;

    explicit ZoneHVACWaterToAirHeatPump(std::shared_ptr<detail::ZoneHVACWaterToAirHeatPump_Impl> impl);

    friend class detail::ZoneHVACWaterToAirHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACWaterToAirHeatPump");
  };

  /** \relates ZoneHVACWaterToAirHeatPump*/
  using OptionalZoneHVACWaterToAirHeatPump = boost::optional<ZoneHVACWaterToAirHeatPump>;

  /** \relates ZoneHVACWaterToAirHeatPump*/
  using ZoneHVACWaterToAirHeatPumpVector = std::vector<ZoneHVACWaterToAirHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACWATERTOAIRHEATPUMP_HPP
