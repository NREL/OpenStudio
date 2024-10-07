/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_HPP
#define MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class WaterHeaterHeatPumpWrappedCondenser_Impl;

  }  // namespace detail

  /** WaterHeaterHeatPumpWrappedCondenser is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:HeatPump:WrappedCondenser'. */
  class MODEL_API WaterHeaterHeatPumpWrappedCondenser : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit WaterHeaterHeatPumpWrappedCondenser(const Model& model);

    explicit WaterHeaterHeatPumpWrappedCondenser(const Model& model, const ModelObject& dxCoil, const HVACComponent& tank, const HVACComponent& fan,
                                                 Schedule& compressorSetpointTemperatureSchedule, Schedule& inletAirMixerSchedule);

    virtual ~WaterHeaterHeatPumpWrappedCondenser() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterHeaterHeatPumpWrappedCondenser(const WaterHeaterHeatPumpWrappedCondenser& other) = default;
    WaterHeaterHeatPumpWrappedCondenser(WaterHeaterHeatPumpWrappedCondenser&& other) = default;
    WaterHeaterHeatPumpWrappedCondenser& operator=(const WaterHeaterHeatPumpWrappedCondenser&) = default;
    WaterHeaterHeatPumpWrappedCondenser& operator=(WaterHeaterHeatPumpWrappedCondenser&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> inletAirConfigurationValues();

    static std::vector<std::string> compressorLocationValues();

    static std::vector<std::string> fanPlacementValues();

    static std::vector<std::string> parasiticHeatRejectionLocationValues();

    static std::vector<std::string> tankElementControlLogicValues();

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

    bool setEvaporatorAirFlowRate(double evaporatorAirFlowRate);

    void autocalculateEvaporatorAirFlowRate();

    bool setInletAirConfiguration(const std::string& inletAirConfiguration);

    bool setInletAirTemperatureSchedule(Schedule& schedule);

    void resetInletAirTemperatureSchedule();

    bool setInletAirHumiditySchedule(Schedule& schedule);

    void resetInletAirHumiditySchedule();

    bool setTank(const HVACComponent& waterHeaterStratified);

    bool setDXCoil(const ModelObject& heatPumpWaterHeaterDXCoilsWrapped);

    bool setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation);

    bool setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation);

    bool setCompressorLocation(const std::string& compressorLocation);

    bool setCompressorAmbientTemperatureSchedule(Schedule& schedule);

    void resetCompressorAmbientTemperatureSchedule();

    bool setFan(const HVACComponent& fansOnOff);

    bool setFanPlacement(const std::string& fanPlacement);

    bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    bool setParasiticHeatRejectionLocation(const std::string& parasiticHeatRejectionLocation);

    bool setInletAirMixerSchedule(Schedule& schedule);

    void resetInletAirMixerSchedule();

    bool setTankElementControlLogic(const std::string& tankElementControlLogic);

    bool setControlSensor1HeightInStratifiedTank(double controlSensor1HeightInStratifiedTank);

    void resetControlSensor1HeightInStratifiedTank();

    bool setControlSensor1Weight(double controlSensor1Weight);

    bool setControlSensor2HeightInStratifiedTank(double controlSensor2HeightInStratifiedTank);

    void resetControlSensor2HeightInStratifiedTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::WaterHeaterHeatPumpWrappedCondenser_Impl;

    explicit WaterHeaterHeatPumpWrappedCondenser(std::shared_ptr<detail::WaterHeaterHeatPumpWrappedCondenser_Impl> impl);

    friend class detail::WaterHeaterHeatPumpWrappedCondenser_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterHeatPumpWrappedCondenser");
  };

  /** \relates WaterHeaterHeatPumpWrappedCondenser*/
  using OptionalWaterHeaterHeatPumpWrappedCondenser = boost::optional<WaterHeaterHeatPumpWrappedCondenser>;

  /** \relates WaterHeaterHeatPumpWrappedCondenser*/
  using WaterHeaterHeatPumpWrappedCondenserVector = std::vector<WaterHeaterHeatPumpWrappedCondenser>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_HPP
