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

#ifndef MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

class HVACComponent;
class Schedule;
class CoilHeatingDXVariableRefrigerantFlow;
class CoilCoolingDXVariableRefrigerantFlow;

namespace detail {

  /** ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACTerminalUnitVariableRefrigerantFlow.*/
  class MODEL_API ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl : public ZoneHVACComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    virtual ~ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

    HVACComponent supplyAirFan() const;

    boost::optional<CoilCoolingDXVariableRefrigerantFlow> coolingCoil() const;

    boost::optional<CoilHeatingDXVariableRefrigerantFlow> heatingCoil() const;

    double zoneTerminalUnitOnParasiticElectricEnergyUse() const;

    double zoneTerminalUnitOffParasiticElectricEnergyUse() const;

    double ratedTotalHeatingCapacitySizingRatio() const;

  boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const ;

  boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingisNeeded() const ;

  boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const ;

  boost::optional<double> autosizedSupplyAirFlowRateWhenNoHeatingisNeeded() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateDuringCoolingOperation() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateDuringHeatingOperation() const ;

  boost::optional<double> autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

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

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() const override;

    unsigned outletPort() const override;

    bool setSupplyAirFan(const HVACComponent & component);

    bool setCoolingCoil(const CoilCoolingDXVariableRefrigerantFlow & component);

    bool setHeatingCoil(const CoilHeatingDXVariableRefrigerantFlow & component);

    ModelObject clone(Model model) const override;

    std::vector<ModelObject> children() const override;

    //@}
   protected:

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACTerminalUnitVariableRefrigerantFlow");

    boost::optional<Schedule> optionalTerminalUnitAvailabilityschedule() const;
    boost::optional<Schedule> optionalSupplyAirFanOperatingModeSchedule() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACTERMINALUNITVARIABLEREFRIGERANTFLOW_IMPL_HPP

