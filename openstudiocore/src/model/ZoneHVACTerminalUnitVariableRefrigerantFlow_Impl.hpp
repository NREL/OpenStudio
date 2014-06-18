/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
    Q_OBJECT;
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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

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

    CoilCoolingDXVariableRefrigerantFlow coolingCoil() const;

    CoilHeatingDXVariableRefrigerantFlow heatingCoil() const;

    double zoneTerminalUnitOnParasiticElectricEnergyUse() const;

    double zoneTerminalUnitOffParasiticElectricEnergyUse() const;

    double ratedTotalHeatingCapacitySizingRatio() const;

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

    unsigned inletPort();

    unsigned outletPort();

    bool setSupplyAirFan(const HVACComponent & component);

    bool setCoolingCoil(const CoilCoolingDXVariableRefrigerantFlow & component);

    bool setHeatingCoil(const CoilHeatingDXVariableRefrigerantFlow & component);

    ModelObject clone(Model model) const;

    std::vector<ModelObject> children() const;

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

