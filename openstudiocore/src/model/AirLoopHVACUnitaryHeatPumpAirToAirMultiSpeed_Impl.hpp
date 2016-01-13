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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl is a StraightComponent_Impl that is the implementation class for AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.*/
  class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl(const IdfObject& idfObject,
                                                      Model_Impl* model,
                                                      bool keepHandle);

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                      Model_Impl* model,
                                                      bool keepHandle);

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl(const AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl& other,
                                                      Model_Impl* model,
                                                      bool keepHandle);

    virtual ~AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl() {}

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

    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    int numberofSpeedsforHeating() const;

    int numberofSpeedsforCooling() const;

    boost::optional<double> speed1SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> speed2SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> speed3SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> speed4SupplyAirFlowRateDuringHeatingOperation() const;

    bool isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> speed1SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    boost::optional<double> speed2SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    boost::optional<double> speed3SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    boost::optional<double> speed4SupplyAirFlowRateDuringCoolingOperation() const;

    bool isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setControllingZoneorThermostatLocation(const boost::optional<ThermalZone>& thermalZone);

    void resetControllingZoneorThermostatLocation();

    bool setSupplyAirFan(const HVACComponent& fan);

    bool setSupplyAirFanPlacement(std::string supplyAirFanPlacement);

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    bool setHeatingCoil(const HVACComponent& coil);

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    bool setCoolingCoil(const HVACComponent& coil);

    bool setSupplementalHeatingCoil(const HVACComponent& coil);

    void setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    bool setAuxiliaryOnCycleElectricPower(double auxiliaryOnCycleElectricPower);

    bool setAuxiliaryOffCycleElectricPower(double auxiliaryOffCycleElectricPower);

    bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

    bool setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery);

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setNumberofSpeedsforHeating(int numberofSpeedsforHeating);

    bool setNumberofSpeedsforCooling(int numberofSpeedsforCooling);

    bool setSpeed1SupplyAirFlowRateDuringHeatingOperation(boost::optional<double> speed1SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed2SupplyAirFlowRateDuringHeatingOperation(boost::optional<double> speed2SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed3SupplyAirFlowRateDuringHeatingOperation(boost::optional<double> speed3SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed4SupplyAirFlowRateDuringHeatingOperation(boost::optional<double> speed4SupplyAirFlowRateDuringHeatingOperation);

    void autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();

    bool setSpeed1SupplyAirFlowRateDuringCoolingOperation(boost::optional<double> speed1SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();

    bool setSpeed2SupplyAirFlowRateDuringCoolingOperation(boost::optional<double> speed2SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();

    bool setSpeed3SupplyAirFlowRateDuringCoolingOperation(boost::optional<double> speed3SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();

    bool setSpeed4SupplyAirFlowRateDuringCoolingOperation(boost::optional<double> speed4SupplyAirFlowRateDuringCoolingOperation);

    void autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;
    unsigned outletPort() override;
    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed");

    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIRMULTISPEED_IMPL_HPP

