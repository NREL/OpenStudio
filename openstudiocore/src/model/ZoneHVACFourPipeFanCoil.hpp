/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

