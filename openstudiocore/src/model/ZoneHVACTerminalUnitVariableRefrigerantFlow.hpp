/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class HVACComponent;
class Schedule;
class CoilHeatingDXVariableRefrigerantFlow;
class CoilCoolingDXVariableRefrigerantFlow;

namespace detail {

  class ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;

} // detail

/** ZoneHVACTerminalUnitVariableRefrigerantFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:TerminalUnit:VariableRefrigerantFlow'. */
class MODEL_API ZoneHVACTerminalUnitVariableRefrigerantFlow : public ZoneHVACComponent {

 public:

  explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model);

  explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model,
    const CoilCoolingDXVariableRefrigerantFlow & coolingCoil,
    const CoilHeatingDXVariableRefrigerantFlow & heatingCoil,
    const HVACComponent & fan);

  virtual ~ZoneHVACTerminalUnitVariableRefrigerantFlow() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> supplyAirFanplacementValues();

  Schedule terminalUnitAvailabilityschedule() const;

  bool setTerminalUnitAvailabilityschedule(Schedule& schedule);

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Supply Air Flow Rate" **/
  boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

  bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

  bool setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation);

  void autosizeSupplyAirFlowRateDuringCoolingOperation();

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Cooling Supply Air Flow Rate" **/
  boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const;

  bool setSupplyAirFlowRateWhenNoCoolingisNeeded(double supplyAirFlowRateWhenNoCoolingisNeeded);

  void autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Supply Air Flow Rate" **/
  boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

  bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

  bool setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation);

  void autosizeSupplyAirFlowRateDuringHeatingOperation();

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Heating Supply Air Flow Rate" **/
  boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded() const;

  bool isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const;

  bool setSupplyAirFlowRateWhenNoHeatingisNeeded(double supplyAirFlowRateWhenNoHeatingisNeeded);

  void autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooling Outdoor Air Flow Rate" **/
  boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

  bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

  bool setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation);

  void autosizeOutdoorAirFlowRateDuringCoolingOperation();

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Heating Outdoor Air Flow Rate" **/
  boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

  bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

  bool setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation);

  void autosizeOutdoorAirFlowRateDuringHeatingOperation();

  /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "No Load Outdoor Air Flow Rate" **/
  boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

  bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

  bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

  void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  Schedule supplyAirFanOperatingModeSchedule() const;

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  HVACComponent supplyAirFan() const;

  boost::optional<CoilCoolingDXVariableRefrigerantFlow> coolingCoil() const;

  bool setCoolingCoil(const CoilCoolingDXVariableRefrigerantFlow & coil);

  boost::optional<CoilHeatingDXVariableRefrigerantFlow> heatingCoil() const;

  bool setHeatingCoil(const CoilHeatingDXVariableRefrigerantFlow & coil);

  double zoneTerminalUnitOnParasiticElectricEnergyUse() const;

  bool setZoneTerminalUnitOnParasiticElectricEnergyUse(double zoneTerminalUnitOnParasiticElectricEnergyUse);

  double zoneTerminalUnitOffParasiticElectricEnergyUse() const;

  bool setZoneTerminalUnitOffParasiticElectricEnergyUse(double zoneTerminalUnitOffParasiticElectricEnergyUse);

  double ratedTotalHeatingCapacitySizingRatio() const;

  bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);

  boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const ;

  boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingisNeeded() const ;

  boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const ;

  boost::optional<double> autosizedSupplyAirFlowRateWhenNoHeatingisNeeded() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const ;



 protected:

  /// @cond
  typedef detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl ImplType;

  explicit ZoneHVACTerminalUnitVariableRefrigerantFlow(std::shared_ptr<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl> impl);

  friend class detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACTerminalUnitVariableRefrigerantFlow");
};

/** \relates ZoneHVACTerminalUnitVariableRefrigerantFlow*/
typedef boost::optional<ZoneHVACTerminalUnitVariableRefrigerantFlow> OptionalZoneHVACTerminalUnitVariableRefrigerantFlow;

/** \relates ZoneHVACTerminalUnitVariableRefrigerantFlow*/
typedef std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> ZoneHVACTerminalUnitVariableRefrigerantFlowVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_HPP

