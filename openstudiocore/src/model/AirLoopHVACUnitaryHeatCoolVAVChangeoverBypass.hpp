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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl;

} // detail

/** AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass'. */
class MODEL_API AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(const Model& model, const HVACComponent& fan, const HVACComponent& coolingCoil, const HVACComponent& heatingCoil);

  virtual ~AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass() {}

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

  bool setSupplyAirFanPlacement(std::string supplyAirFanPlacement);

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  bool setCoolingCoil(const HVACComponent& coolingCoilsDXMultiModeOrSingleSpeed);

  bool setHeatingCoil(const HVACComponent& heatingCoilName);

  bool setPriorityControlMode(std::string priorityControlMode);

  bool setMinimumOutletAirTemperatureDuringCoolingOperation(double minimumOutletAirTemperatureDuringCoolingOperation);

  bool setMaximumOutletAirTemperatureDuringHeatingOperation(double maximumOutletAirTemperatureDuringHeatingOperation);

  bool setDehumidificationControlType(std::string dehumidificationControlType);

  boost::optional<double> autosizedSystemAirFlowRateDuringCoolingOperation() const ;

  boost::optional<double> autosizedSystemAirFlowRateDuringHeatingOperation() const ;

  boost::optional<double> autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const ;



  //@}
  /** @name Other */
  //@{
  //@}
 protected:
  /// @cond
  typedef detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl ImplType;

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
typedef boost::optional<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> OptionalAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass;

/** \relates AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass*/
typedef std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypassVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_HPP

