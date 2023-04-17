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

#ifndef MODEL_WATERHEATERHEATPUMP_HPP
#define MODEL_WATERHEATERHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class WaterHeaterHeatPump_Impl;

  }  // namespace detail

  /** WaterHeaterHeatPump is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:HeatPump'.
 *  As of EnergyPlus version 8.4.0 this object maps to WaterHeaterHeatPumpPumpedCondenser in idf format.
 */
  class MODEL_API WaterHeaterHeatPump : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit WaterHeaterHeatPump(const Model& model);

    explicit WaterHeaterHeatPump(const Model& model, const ModelObject& dxCoil, const HVACComponent& tank, const HVACComponent& fan,
                                 Schedule& compressorSetpointTemperatureSchedule, Schedule& inletAirMixerSchedule);

    virtual ~WaterHeaterHeatPump() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterHeaterHeatPump(const WaterHeaterHeatPump& other) = default;
    WaterHeaterHeatPump(WaterHeaterHeatPump&& other) = default;
    WaterHeaterHeatPump& operator=(const WaterHeaterHeatPump&) = default;
    WaterHeaterHeatPump& operator=(WaterHeaterHeatPump&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> inletAirConfigurationValues();

    static std::vector<std::string> compressorLocationValues();

    static std::vector<std::string> fanPlacementValues();

    static std::vector<std::string> parasiticHeatRejectionLocationValues();

    static std::vector<std::string> controlSensorLocationInStratifiedTankValues();

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

    double maximumInletAirTemperatureforCompressorOperation() const;

    std::string compressorLocation() const;

    boost::optional<Schedule> compressorAmbientTemperatureSchedule() const;

    HVACComponent fan() const;

    std::string fanPlacement() const;

    double onCycleParasiticElectricLoad() const;

    double offCycleParasiticElectricLoad() const;

    std::string parasiticHeatRejectionLocation() const;

    Schedule inletAirMixerSchedule() const;

    std::string controlSensorLocationInStratifiedTank() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCompressorSetpointTemperatureSchedule(Schedule& schedule);

    bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

    bool setCondenserWaterFlowRate(double condenserWaterFlowRate);

    void resetCondenserWaterFlowRate();

    void autosizeCondenserWaterFlowRate();

    bool setEvaporatorAirFlowRate(double evaporatorAirFlowRate);

    void resetEvaporatorAirFlowRate();

    void autosizeEvaporatorAirFlowRate();

    bool setInletAirConfiguration(const std::string& inletAirConfiguration);

    bool setInletAirTemperatureSchedule(Schedule& schedule);

    void resetInletAirTemperatureSchedule();

    bool setInletAirHumiditySchedule(Schedule& schedule);

    void resetInletAirHumiditySchedule();

    bool setTank(const HVACComponent& waterHeater);

    bool setDXCoil(const ModelObject& coil);

    bool setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation);

    bool setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation);

    bool setCompressorLocation(const std::string& compressorLocation);

    bool setCompressorAmbientTemperatureSchedule(Schedule& schedule);

    void resetCompressorAmbientTemperatureSchedule();

    bool setFan(const HVACComponent& fan);

    bool setFanPlacement(const std::string& fanPlacement);

    bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    bool setParasiticHeatRejectionLocation(const std::string& parasiticHeatRejectionLocation);

    bool setInletAirMixerSchedule(Schedule& schedule);

    bool setControlSensorLocationInStratifiedTank(const std::string& controlSensorLocationInStratifiedTank);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedCondenserWaterFlowRate() const;

    boost::optional<double> autosizedEvaporatorAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::WaterHeaterHeatPump_Impl;

    explicit WaterHeaterHeatPump(std::shared_ptr<detail::WaterHeaterHeatPump_Impl> impl);

    friend class detail::WaterHeaterHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterHeatPump");
  };

  /** \relates WaterHeaterHeatPump*/
  using OptionalWaterHeaterHeatPump = boost::optional<WaterHeaterHeatPump>;

  /** \relates WaterHeaterHeatPump*/
  using WaterHeaterHeatPumpVector = std::vector<WaterHeaterHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERHEATPUMP_HPP
