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

#ifndef MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class HVACComponent;
  class Schedule;
  class ThermalZone;

  namespace detail {

    /** ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACTerminalUnitVariableRefrigerantFlow.*/
    class MODEL_API ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl : public ZoneHVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl& other, Model_Impl* model,
                                                       bool keepHandle);

      virtual ~ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule terminalUnitAvailabilityschedule() const;

      boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

      bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded() const;

      bool isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const;

      boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

      bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded() const;

      bool isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const;

      boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

      bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

      bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

      Schedule supplyAirFanOperatingModeSchedule() const;

      boost::optional<HVACComponent> supplyAirFan() const;

      boost::optional<HVACComponent> coolingCoil() const;

      boost::optional<HVACComponent> heatingCoil() const;

      double zoneTerminalUnitOnParasiticElectricEnergyUse() const;

      double zoneTerminalUnitOffParasiticElectricEnergyUse() const;

      double ratedTotalHeatingCapacitySizingRatio() const;

      // Supplemental Heating Coil Name
      boost::optional<HVACComponent> supplementalHeatingCoil() const;
      bool setSupplementalHeatingCoil(const HVACComponent& coil);
      void resetSupplementalHeatingCoil();

      // Maximum Supply Air Temperature from Supplemental Heater (autosized)
      boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;
      bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;
      bool setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater);
      void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

      // Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation (default 21C)
      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;
      bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

      // Controlling Zone or Thermostat Location
      boost::optional<ThermalZone> controllingZoneorThermostatLocation() const;
      bool setControllingZoneorThermostatLocation(const ThermalZone& thermalZone);
      void resetControllingZoneorThermostatLocation();

      // Supply Air Fan Placement
      std::string supplyAirFanPlacement() const;
      bool setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement);

      boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingisNeeded() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateWhenNoHeatingisNeeded() const;

      boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

      //@}
      /** @name Setters */
      //@{

      bool setTerminalUnitAvailabilityschedule(Schedule& schedule);

      bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

      void autosizeSupplyAirFlowRateDuringCoolingOperation();

      bool setSupplyAirFlowRateWhenNoCoolingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded);

      void autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

      bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);

      void autosizeSupplyAirFlowRateDuringHeatingOperation();

      bool setSupplyAirFlowRateWhenNoHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded);

      void autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

      bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

      void autosizeOutdoorAirFlowRateDuringCoolingOperation();

      bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

      void autosizeOutdoorAirFlowRateDuringHeatingOperation();

      bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      bool setZoneTerminalUnitOnParasiticElectricEnergyUse(double zoneTerminalUnitOnParasiticElectricEnergyUse);

      bool setZoneTerminalUnitOffParasiticElectricEnergyUse(double zoneTerminalUnitOffParasiticElectricEnergyUse);

      bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);

      bool setSupplyAirFan(const HVACComponent& component);
      void resetSupplyAirFan();

      bool setCoolingCoil(const HVACComponent& component);

      bool setHeatingCoil(const HVACComponent& component);

      //@}
      /** @name Other */
      //@{

      // Returns true if the Cooling and Heating Coils are of the FluidTemperatureControl type
      bool isFluidTemperatureControl() const;

      boost::optional<HVACComponent> vrfSystem() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACTerminalUnitVariableRefrigerantFlow");

      boost::optional<Schedule> optionalTerminalUnitAvailabilityschedule() const;
      boost::optional<Schedule> optionalSupplyAirFanOperatingModeSchedule() const;

      boost::optional<ModelObject> supplementalHeatingCoilAsModelObject() const;
      bool setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_IMPL_HPP
