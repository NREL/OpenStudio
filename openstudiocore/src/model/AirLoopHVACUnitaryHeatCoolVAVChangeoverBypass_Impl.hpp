/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl is a StraightComponent_Impl that is the implementation class for AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.*/
  class MODEL_API AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const IdfObject& idfObject,
                                                       Model_Impl* model,
                                                       bool keepHandle);

    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle);

    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle);

    virtual ~AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

    bool setSupplyAirFanPlacement(std::string supplyAirFanPlacement);

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    bool setCoolingCoil(const HVACComponent& coolingCoilsDXMultiModeOrSingleSpeed);

    bool setHeatingCoil(const HVACComponent& heatingCoilName);

    bool setPriorityControlMode(std::string priorityControlMode);

    bool setMinimumOutletAirTemperatureDuringCoolingOperation(double minimumOutletAirTemperatureDuringCoolingOperation);

    bool setMaximumOutletAirTemperatureDuringHeatingOperation(double maximumOutletAirTemperatureDuringHeatingOperation);

    bool setDehumidificationControlType(std::string dehumidificationControlType);

    //@}
    /** @name Other */
    //@{

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    ModelObject clone(Model model) const override;

    std::vector<IdfObject> remove() override;

    std::vector<ModelObject> children() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass");

    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATCOOLVAVCHANGEOVERBYPASS_IMPL_HPP

