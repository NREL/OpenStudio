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

} // detail

/** WaterHeaterHeatPump is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:HeatPump'.
 *  As of EnergyPlus version 8.4.0 this object maps to WaterHeaterHeatPumpPumpedCondenser in idf format.
 */
class MODEL_API WaterHeaterHeatPump : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WaterHeaterHeatPump(const Model& model);

  explicit WaterHeaterHeatPump(const Model& model,
    const ModelObject & dxCoil,
    const HVACComponent & tank,
    const HVACComponent & fan,
    Schedule & compressorSetpointTemperatureSchedule,
    Schedule & inletAirMixerSchedule);

  virtual ~WaterHeaterHeatPump() {}

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

  //@}
 protected:
  /// @cond
  typedef detail::WaterHeaterHeatPump_Impl ImplType;

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
typedef boost::optional<WaterHeaterHeatPump> OptionalWaterHeaterHeatPump;

/** \relates WaterHeaterHeatPump*/
typedef std::vector<WaterHeaterHeatPump> WaterHeaterHeatPumpVector;

} // model
} // openstudio

#endif // MODEL_WATERHEATERHEATPUMP_HPP

