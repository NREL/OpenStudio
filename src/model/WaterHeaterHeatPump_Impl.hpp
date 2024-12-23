/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API WaterHeaterHeatPump_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      WaterHeaterHeatPump_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterHeaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterHeaterHeatPump_Impl(const WaterHeaterHeatPump_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterHeaterHeatPump_Impl() override = default;

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

      std::string tankElementControlLogic() const;

      std::string controlSensorLocationInStratifiedTank() const;

      boost::optional<double> autosizedCondenserWaterFlowRate() const;

      boost::optional<double> autosizedEvaporatorAirFlowRate() const;

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

      bool setTankElementControlLogic(const std::string& tankElementControlLogic);

      bool setControlSensorLocationInStratifiedTank(const std::string& controlSensorLocationInStratifiedTank);

      //@}
      /** @name Other */
      //@{

      unsigned inletPort() const override;
      unsigned outletPort() const override;
      std::vector<ModelObject> children() const override;
      ModelObject clone(Model model) const override;
      bool addToThermalZone(ThermalZone& thermalZone) override;
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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERHEATPUMP_IMPL_HPP
