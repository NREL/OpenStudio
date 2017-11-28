/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_ZONEHVACFOURPIPEFANCOIL_HPP
#define MODEL_ZONEHVACFOURPIPEFANCOIL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACFourPipeFanCoil_Impl;

} // detail

/** ZoneHVACFourPipeFanCoil is a ZoneHVACComponent that wraps the
 *  OpenStudio IDD object 'OS:ZoneHVAC:FourPipeFanCoil'.
 *  If fan type is OS:Fan:ConstantVolume, all availability schedule values must be greater than zero
 */
class MODEL_API ZoneHVACFourPipeFanCoil : public ZoneHVACComponent
{
  public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct new ZoneHVACFourPipeFanCoil. capacityControlMethod is set based on the IddObjectType of supplyAirFan:
   *  \li OS:Fan:ConstantVolume results in capacityControlMethod == "ConstantFanVariableFlow"
   *  \li OS:Fan:OnOff results in capacityControlMethod == "CyclingFan"
   *  \li OS:Fan:VariableVolume results in capacityControlMethod == "VariableFanVariableFlow"
   */
  ZoneHVACFourPipeFanCoil( const Model& model,
                                          Schedule & availabilitySchedule,
                                          HVACComponent & supplyAirFan,
                                          HVACComponent & coolingCoil,
                                          HVACComponent & heatingCoil );

  virtual ~ZoneHVACFourPipeFanCoil() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> capacityControlMethodValues();

  static std::vector<std::string> outdoorAirMixerObjectTypeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  std::string capacityControlMethod() const;

  boost::optional<double> maximumSupplyAirFlowRate() const;

  OSOptionalQuantity getMaximumSupplyAirFlowRate(bool returnIP=false) const;

  bool isMaximumSupplyAirFlowRateAutosized() const;

  double lowSpeedSupplyAirFlowRatio() const;

  Quantity getLowSpeedSupplyAirFlowRatio(bool returnIP=false) const;

  bool isLowSpeedSupplyAirFlowRatioDefaulted() const;

  double mediumSpeedSupplyAirFlowRatio() const;

  Quantity getMediumSpeedSupplyAirFlowRatio(bool returnIP=false) const;

  bool isMediumSpeedSupplyAirFlowRatioDefaulted() const;

  boost::optional<double> maximumOutdoorAirFlowRate() const;

  OSOptionalQuantity getMaximumOutdoorAirFlowRate(bool returnIP=false) const;

  bool isMaximumOutdoorAirFlowRateAutosized() const;

  boost::optional<Schedule> outdoorAirSchedule() const;

  std::string outdoorAirMixerObjectType() const;

  std::string outdoorAirMixerName() const;

  HVACComponent supplyAirFan() const;

  HVACComponent coolingCoil() const;

  boost::optional<double> maximumColdWaterFlowRate() const;

  OSOptionalQuantity getMaximumColdWaterFlowRate(bool returnIP=false) const;

  bool isMaximumColdWaterFlowRateAutosized() const;

  double minimumColdWaterFlowRate() const;

  Quantity getMinimumColdWaterFlowRate(bool returnIP=false) const;

  bool isMinimumColdWaterFlowRateDefaulted() const;

  double coolingConvergenceTolerance() const;

  Quantity getCoolingConvergenceTolerance(bool returnIP=false) const;

  bool isCoolingConvergenceToleranceDefaulted() const;

  HVACComponent heatingCoil() const;

  boost::optional<double> maximumHotWaterFlowRate() const;

  OSOptionalQuantity getMaximumHotWaterFlowRate(bool returnIP=false) const;

  bool isMaximumHotWaterFlowRateAutosized() const;

  double minimumHotWaterFlowRate() const;

  Quantity getMinimumHotWaterFlowRate(bool returnIP=false) const;

  bool isMinimumHotWaterFlowRateDefaulted() const;

  double heatingConvergenceTolerance() const;

  Quantity getHeatingConvergenceTolerance(bool returnIP=false) const;

  bool isHeatingConvergenceToleranceDefaulted() const;

  boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

  bool isMinimumSupplyAirTemperatureInCoolingModeAutosized() const;

  boost::optional<double> minimumSupplyAirTemperatureInCoolingMode() const;
  
  bool isMaximumSupplyAirTemperatureInHeatingModeAutosized() const;

  boost::optional<double> maximumSupplyAirTemperatureInHeatingMode() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setCapacityControlMethod(std::string capacityControlMethod);

  void setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate);

  bool setMaximumSupplyAirFlowRate(const Quantity& maximumSupplyAirFlowRate);

  void autosizeMaximumSupplyAirFlowRate();

  bool setLowSpeedSupplyAirFlowRatio(double lowSpeedSupplyAirFlowRatio);

  bool setLowSpeedSupplyAirFlowRatio(const Quantity& lowSpeedSupplyAirFlowRatio);

  void resetLowSpeedSupplyAirFlowRatio();

  bool setMediumSpeedSupplyAirFlowRatio(double mediumSpeedSupplyAirFlowRatio);

  bool setMediumSpeedSupplyAirFlowRatio(const Quantity& mediumSpeedSupplyAirFlowRatio);

  void resetMediumSpeedSupplyAirFlowRatio();

  void setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate);

  bool setMaximumOutdoorAirFlowRate(const Quantity& maximumOutdoorAirFlowRate);

  void autosizeMaximumOutdoorAirFlowRate();

  bool setOutdoorAirSchedule(Schedule& schedule);

  void resetOutdoorAirSchedule();

  bool setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType);

  void setOutdoorAirMixerName(std::string outdoorAirMixerName);

  bool setSupplyAirFan( HVACComponent & fan );

  bool setCoolingCoil( HVACComponent & coolingCoil );

  void setMaximumColdWaterFlowRate(double maximumColdWaterFlowRate);

  bool setMaximumColdWaterFlowRate(const Quantity& maximumColdWaterFlowRate);

  void autosizeMaximumColdWaterFlowRate();

  void setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

  bool setMinimumColdWaterFlowRate(const Quantity& minimumColdWaterFlowRate);

  void resetMinimumColdWaterFlowRate();

  bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

  bool setCoolingConvergenceTolerance(const Quantity& coolingConvergenceTolerance);

  void resetCoolingConvergenceTolerance();

  bool setHeatingCoil( HVACComponent & heatingCoil );

  void setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate);

  bool setMaximumHotWaterFlowRate(const Quantity& maximumHotWaterFlowRate);

  void autosizeMaximumHotWaterFlowRate();

  void setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

  bool setMinimumHotWaterFlowRate(const Quantity& minimumHotWaterFlowRate);

  void resetMinimumHotWaterFlowRate();

  bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

  bool setHeatingConvergenceTolerance(const Quantity& heatingConvergenceTolerance);

  void resetHeatingConvergenceTolerance();

  /** Sets the supply air fan operating mode schedule. Values of zero indicate that the fan 
   *  cycles on and off with the active (heating or cooling) coil. Other values operate the 
   *  fan continuously. */
  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  void autosizeMinimumSupplyAirTemperatureInCoolingMode();

  bool setMinimumSupplyAirTemperatureInCoolingMode(double minimumSupplyAirTemperatureInCoolingMode);

  void autosizeMaximumSupplyAirTemperatureInHeatingMode();

  bool setMaximumSupplyAirTemperatureInHeatingMode(double maximumSupplyAirTemperatureInHeatingMode);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACFourPipeFanCoil_Impl ImplType;

  explicit ZoneHVACFourPipeFanCoil(std::shared_ptr<detail::ZoneHVACFourPipeFanCoil_Impl> impl);

  friend class detail::ZoneHVACFourPipeFanCoil_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ZoneHVACFourPipeFanCoil");
};

/** \relates ZoneHVACFourPipeFanCoil*/
typedef boost::optional<ZoneHVACFourPipeFanCoil> OptionalZoneHVACFourPipeFanCoil;

/** \relates ZoneHVACFourPipeFanCoil*/
typedef std::vector<ZoneHVACFourPipeFanCoil> ZoneHVACFourPipeFanCoilVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACFOURPIPEFANCOIL_HPP

