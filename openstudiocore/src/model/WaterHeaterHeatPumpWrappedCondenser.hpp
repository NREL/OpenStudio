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

#ifndef MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_HPP
#define MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_HPP

#include <model/ModelAPI.hpp>
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class WaterHeaterHeatPumpWrappedCondenser_Impl;

} // detail

/** WaterHeaterHeatPumpWrappedCondenser is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:HeatPump:WrappedCondenser'. */
class MODEL_API WaterHeaterHeatPumpWrappedCondenser : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WaterHeaterHeatPumpWrappedCondenser(const Model& model);

  explicit WaterHeaterHeatPumpWrappedCondenser(const Model& model,
    const ModelObject & dxCoil,
    const HVACComponent & tank,
    const HVACComponent & fan,
    Schedule & compressorSetpointTemperatureSchedule,
    Schedule & inletAirMixerSchedule);

  virtual ~WaterHeaterHeatPumpWrappedCondenser() {}

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

  bool setInletAirConfiguration(std::string inletAirConfiguration);

  bool setInletAirTemperatureSchedule(Schedule& schedule);

  void resetInletAirTemperatureSchedule();

  bool setInletAirHumiditySchedule(Schedule& schedule);

  void resetInletAirHumiditySchedule();

  bool setTank(const HVACComponent& waterHeaterStratified);

  bool setDXCoil(const ModelObject& heatPumpWaterHeaterDXCoilsWrapped);

  bool setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation);

  bool setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation);

  bool setCompressorLocation(std::string compressorLocation);

  bool setCompressorAmbientTemperatureSchedule(Schedule& schedule);

  void resetCompressorAmbientTemperatureSchedule();

  bool setFan(const HVACComponent& fansOnOff);

  bool setFanPlacement(std::string fanPlacement);

  bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

  bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

  bool setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation);

  bool setInletAirMixerSchedule(Schedule& schedule);

  void resetInletAirMixerSchedule();

  bool setTankElementControlLogic(std::string tankElementControlLogic);

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
  typedef detail::WaterHeaterHeatPumpWrappedCondenser_Impl ImplType;

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
typedef boost::optional<WaterHeaterHeatPumpWrappedCondenser> OptionalWaterHeaterHeatPumpWrappedCondenser;

/** \relates WaterHeaterHeatPumpWrappedCondenser*/
typedef std::vector<WaterHeaterHeatPumpWrappedCondenser> WaterHeaterHeatPumpWrappedCondenserVector;

} // model
} // openstudio

#endif // MODEL_WATERHEATERHEATPUMPWRAPPEDCONDENSER_HPP

