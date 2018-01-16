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

#ifndef MODEL_WATERHEATERHEATPUMP_IMPL_HPP
#define MODEL_WATERHEATERHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** WaterHeaterHeatPump_Impl is a ZoneHVACComponent_Impl that is the implementation class for WaterHeaterHeatPump.*/
  class MODEL_API WaterHeaterHeatPump_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    WaterHeaterHeatPump_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle);

    WaterHeaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    WaterHeaterHeatPump_Impl(const WaterHeaterHeatPump_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~WaterHeaterHeatPump_Impl() {}

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

    Schedule compressorSetpointTemperatureSchedule() const;

    double deadBandTemperatureDifference() const;

    boost::optional<double> condenserWaterFlowRate() const;

    bool isCondenserWaterFlowRateAutosized() const;

    boost::optional<double> evaporatorAirFlowRate() const;

    bool isEvaporatorAirFlowRateAutosized() const;

    std::string inletAirConfiguration() const;

    boost::optional<Schedule> inletAirTemperatureSchedule() const;

    boost::optional<Schedule> inletAirHumiditySchedule() const;

    HVACComponent tank() const;

    ModelObject dXCoil() const;

    double minimumInletAirTemperatureforCompressorOperation() const;

    std::string compressorLocation() const;

    boost::optional<Schedule> compressorAmbientTemperatureSchedule() const;

    HVACComponent fan() const;

    std::string fanPlacement() const;

    double onCycleParasiticElectricLoad() const;

    double offCycleParasiticElectricLoad() const;

    std::string parasiticHeatRejectionLocation() const;

    Schedule inletAirMixerSchedule() const;

    std::string controlSensorLocationInStratifiedTank() const;

  boost::optional<double> autosizedCondenserWaterFlowRate() const ;

  boost::optional<double> autosizedEvaporatorAirFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCompressorSetpointTemperatureSchedule(Schedule& schedule);

    bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

    bool setCondenserWaterFlowRate(boost::optional<double> condenserWaterFlowRate);

    void resetCondenserWaterFlowRate();

    void autosizeCondenserWaterFlowRate();

    bool setEvaporatorAirFlowRate(boost::optional<double> evaporatorAirFlowRate);

    void resetEvaporatorAirFlowRate();

    void autosizeEvaporatorAirFlowRate();

    bool setInletAirConfiguration(std::string inletAirConfiguration);

    bool setInletAirTemperatureSchedule(Schedule& schedule);

    void resetInletAirTemperatureSchedule();

    bool setInletAirHumiditySchedule(Schedule& schedule);

    void resetInletAirHumiditySchedule();

    bool setTank(const HVACComponent& waterHeater);

    bool setDXCoil(const ModelObject& coil);

    bool setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation);

    bool setCompressorLocation(std::string compressorLocation);

    bool setCompressorAmbientTemperatureSchedule(Schedule& schedule);

    void resetCompressorAmbientTemperatureSchedule();

    bool setFan(const HVACComponent& fan);

    bool setFanPlacement(std::string fanPlacement);

    bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    bool setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation);

    bool setInletAirMixerSchedule(Schedule& schedule);

    bool setControlSensorLocationInStratifiedTank(std::string controlSensorLocationInStratifiedTank);

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() const override;
    unsigned outletPort() const override;
    std::vector<ModelObject> children() const override;
    ModelObject clone(Model model) const override;
    bool addToThermalZone(ThermalZone & thermalZone) override;
    std::vector<IdfObject> remove() override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterHeatPump");

    boost::optional<Schedule> optionalCompressorSetpointTemperatureSchedule() const;
    boost::optional<HVACComponent> optionalTank() const;
    boost::optional<ModelObject> optionalDXCoil() const;
    boost::optional<HVACComponent> optionalFan() const;
    boost::optional<Schedule> optionalInletAirMixerSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_WATERHEATERHEATPUMP_IMPL_HPP

