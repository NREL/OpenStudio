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

      virtual ~AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl() = default;

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
