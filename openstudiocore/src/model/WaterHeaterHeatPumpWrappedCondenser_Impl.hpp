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

#ifndef MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_IMPL_HPP
#define MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** WaterHeaterHeatPumpWrappedCondenser_Impl is a ZoneHVACComponent_Impl that is the implementation class for WaterHeaterHeatPumpWrappedCondenser.*/
  class MODEL_API WaterHeaterHeatPumpWrappedCondenser_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    WaterHeaterHeatPumpWrappedCondenser_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    WaterHeaterHeatPumpWrappedCondenser_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    WaterHeaterHeatPumpWrappedCondenser_Impl(const WaterHeaterHeatPumpWrappedCondenser_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~WaterHeaterHeatPumpWrappedCondenser_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    Schedule compressorSetpointTemperatureSchedule() const;

    double deadBandTemperatureDifference() const;

    double condenserBottomLocation() const;

    double condenserTopLocation() const;

    boost::optional<double> evaporatorAirFlowRate() const;

    bool isEvaporatorAirFlowRateAutocalculated() const;

    std::string inletAirConfiguration() const;

    boost::optional<Schedule> inletAirTemperatureSchedule() const;

    boost::optional<Schedule> inletAirHumiditySchedule() const;

    HVACComponent tank() const;

    ModelObject dXCoil() const;

    double minimumInletAirTemperatureforCompressorOperation() const;

    double maximumInletAirTemperatureforCompressorOperation() const;

    std::string compressorLocation() const;

    boost::optional<Schedule> compressorAmbientTemperatureSchedule() const;

    HVACComponent fan() const;

    std::string fanPlacement() const;

    double onCycleParasiticElectricLoad() const;

    double offCycleParasiticElectricLoad() const;

    std::string parasiticHeatRejectionLocation() const;

    boost::optional<Schedule> inletAirMixerSchedule() const;

    std::string tankElementControlLogic() const;

    boost::optional<double> controlSensor1HeightInStratifiedTank() const;

    double controlSensor1Weight() const;

    boost::optional<double> controlSensor2HeightInStratifiedTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCompressorSetpointTemperatureSchedule(Schedule& schedule);

    bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

    bool setCondenserBottomLocation(double condenserBottomLocation);

    bool setCondenserTopLocation(double condenserTopLocation);

    bool setEvaporatorAirFlowRate(boost::optional<double> evaporatorAirFlowRate);

    void autocalculateEvaporatorAirFlowRate();

    bool setInletAirConfiguration(std::string inletAirConfiguration);

    bool setInletAirTemperatureSchedule(Schedule& schedule);

    void resetInletAirTemperatureSchedule();

    bool setInletAirHumiditySchedule(Schedule& schedule);

    void resetInletAirHumiditySchedule();

    bool setTank(const HVACComponent& tank);

    bool setDXCoil(const ModelObject& coil);

    bool setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation);

    bool setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation);

    bool setCompressorLocation(std::string compressorLocation);

    bool setCompressorAmbientTemperatureSchedule(Schedule& schedule);

    void resetCompressorAmbientTemperatureSchedule();

    bool setFan(const HVACComponent& fan);

    bool setFanPlacement(std::string fanPlacement);

    bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    bool setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation);

    bool setInletAirMixerSchedule(Schedule& schedule);

    void resetInletAirMixerSchedule();

    bool setTankElementControlLogic(std::string tankElementControlLogic);

    bool setControlSensor1HeightInStratifiedTank(boost::optional<double> controlSensor1HeightInStratifiedTank);

    void resetControlSensor1HeightInStratifiedTank();

    bool setControlSensor1Weight(double controlSensor1Weight);

    bool setControlSensor2HeightInStratifiedTank(boost::optional<double> controlSensor2HeightInStratifiedTank);

    void resetControlSensor2HeightInStratifiedTank();

    //@}
    /** @name Other */
    //@{

    virtual unsigned inletPort() const;

    virtual unsigned outletPort() const;

    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;
    bool addToThermalZone(ThermalZone & thermalZone) override;
    std::vector<IdfObject> remove() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterHeatPumpWrappedCondenser");

    boost::optional<Schedule> optionalCompressorSetpointTemperatureSchedule() const;
    boost::optional<HVACComponent> optionalTank() const;
    boost::optional<ModelObject> optionalDXCoil() const;
    boost::optional<HVACComponent> optionalFan() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_IMPL_HPP

