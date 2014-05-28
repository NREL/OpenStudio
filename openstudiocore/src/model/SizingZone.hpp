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

#ifndef MODEL_SIZINGZONE_HPP
#define MODEL_SIZINGZONE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class ThermalZone;

namespace detail {

  class SizingZone_Impl;

} // detail

/** SizingZone is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Zone'. */
class MODEL_API SizingZone : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SizingZone(const Model& model, const ThermalZone & thermalZone);

  virtual ~SizingZone() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> coolingDesignAirFlowMethodValues();

  static std::vector<std::string> heatingDesignAirFlowMethodValues();

  /** @name Getters */
  //@{

  ThermalZone thermalZone() const;

  double zoneCoolingDesignSupplyAirTemperature() const;

  Quantity getZoneCoolingDesignSupplyAirTemperature(bool returnIP=false) const;

  double zoneHeatingDesignSupplyAirTemperature() const;

  Quantity getZoneHeatingDesignSupplyAirTemperature(bool returnIP=false) const;

  double zoneCoolingDesignSupplyAirHumidityRatio() const;

  Quantity getZoneCoolingDesignSupplyAirHumidityRatio(bool returnIP=false) const;

  double zoneHeatingDesignSupplyAirHumidityRatio() const;

  Quantity getZoneHeatingDesignSupplyAirHumidityRatio(bool returnIP=false) const;

  boost::optional<double> zoneHeatingSizingFactor() const;

  OSOptionalQuantity getZoneHeatingSizingFactor(bool returnIP=false) const;

  boost::optional<double> zoneCoolingSizingFactor() const;

  OSOptionalQuantity getZoneCoolingSizingFactor(bool returnIP=false) const;

  std::string coolingDesignAirFlowMethod() const;

  bool isCoolingDesignAirFlowMethodDefaulted() const;

  double coolingDesignAirFlowRate() const;

  Quantity getCoolingDesignAirFlowRate(bool returnIP=false) const;

  bool isCoolingDesignAirFlowRateDefaulted() const;

  double coolingMinimumAirFlowperZoneFloorArea() const;

  Quantity getCoolingMinimumAirFlowperZoneFloorArea(bool returnIP=false) const;

  bool isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const;

  double coolingMinimumAirFlow() const;

  Quantity getCoolingMinimumAirFlow(bool returnIP=false) const;

  bool isCoolingMinimumAirFlowDefaulted() const;

  double coolingMinimumAirFlowFraction() const;

  Quantity getCoolingMinimumAirFlowFraction(bool returnIP=false) const;

  bool isCoolingMinimumAirFlowFractionDefaulted() const;

  std::string heatingDesignAirFlowMethod() const;

  bool isHeatingDesignAirFlowMethodDefaulted() const;

  double heatingDesignAirFlowRate() const;

  Quantity getHeatingDesignAirFlowRate(bool returnIP=false) const;

  bool isHeatingDesignAirFlowRateDefaulted() const;

  double heatingMaximumAirFlowperZoneFloorArea() const;

  Quantity getHeatingMaximumAirFlowperZoneFloorArea(bool returnIP=false) const;

  bool isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const;

  double heatingMaximumAirFlow() const;

  Quantity getHeatingMaximumAirFlow(bool returnIP=false) const;

  bool isHeatingMaximumAirFlowDefaulted() const;

  double heatingMaximumAirFlowFraction() const;

  Quantity getHeatingMaximumAirFlowFraction(bool returnIP=false) const;

  bool isHeatingMaximumAirFlowFractionDefaulted() const;

  double designZoneAirDistributionEffectivenessinCoolingMode() const;

  Quantity getDesignZoneAirDistributionEffectivenessinCoolingMode(bool returnIP=false) const;

  bool isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const;

  double designZoneAirDistributionEffectivenessinHeatingMode() const;

  Quantity getDesignZoneAirDistributionEffectivenessinHeatingMode(bool returnIP=false) const;

  bool isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature);

  bool setZoneCoolingDesignSupplyAirTemperature(const Quantity& zoneCoolingDesignSupplyAirTemperature);

  void setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature);

  bool setZoneHeatingDesignSupplyAirTemperature(const Quantity& zoneHeatingDesignSupplyAirTemperature);

  bool setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio);

  bool setZoneCoolingDesignSupplyAirHumidityRatio(const Quantity& zoneCoolingDesignSupplyAirHumidityRatio);

  bool setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio);

  bool setZoneHeatingDesignSupplyAirHumidityRatio(const Quantity& zoneHeatingDesignSupplyAirHumidityRatio);

  bool setZoneHeatingSizingFactor(double zoneHeatingSizingFactor);

  bool setZoneHeatingSizingFactor(const Quantity& zoneHeatingSizingFactor);

  void resetZoneHeatingSizingFactor();

  bool setZoneCoolingSizingFactor(double zoneCoolingSizingFactor);

  bool setZoneCoolingSizingFactor(const Quantity& zoneCoolingSizingFactor);

  void resetZoneCoolingSizingFactor();

  bool setCoolingDesignAirFlowMethod(std::string coolingDesignAirFlowMethod);

  void resetCoolingDesignAirFlowMethod();

  bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

  bool setCoolingDesignAirFlowRate(const Quantity& coolingDesignAirFlowRate);

  void resetCoolingDesignAirFlowRate();

  bool setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea);

  bool setCoolingMinimumAirFlowperZoneFloorArea(const Quantity& coolingMinimumAirFlowperZoneFloorArea);

  void resetCoolingMinimumAirFlowperZoneFloorArea();

  bool setCoolingMinimumAirFlow(double coolingMinimumAirFlow);

  bool setCoolingMinimumAirFlow(const Quantity& coolingMinimumAirFlow);

  void resetCoolingMinimumAirFlow();

  bool setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction);

  bool setCoolingMinimumAirFlowFraction(const Quantity& coolingMinimumAirFlowFraction);

  void resetCoolingMinimumAirFlowFraction();

  bool setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod);

  void resetHeatingDesignAirFlowMethod();

  bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

  bool setHeatingDesignAirFlowRate(const Quantity& heatingDesignAirFlowRate);

  void resetHeatingDesignAirFlowRate();

  bool setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea);

  bool setHeatingMaximumAirFlowperZoneFloorArea(const Quantity& heatingMaximumAirFlowperZoneFloorArea);

  void resetHeatingMaximumAirFlowperZoneFloorArea();

  bool setHeatingMaximumAirFlow(double heatingMaximumAirFlow);

  bool setHeatingMaximumAirFlow(const Quantity& heatingMaximumAirFlow);

  void resetHeatingMaximumAirFlow();

  bool setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction);

  bool setHeatingMaximumAirFlowFraction(const Quantity& heatingMaximumAirFlowFraction);

  void resetHeatingMaximumAirFlowFraction();

  bool setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode);

  bool setDesignZoneAirDistributionEffectivenessinCoolingMode(const Quantity& designZoneAirDistributionEffectivenessinCoolingMode);

  void resetDesignZoneAirDistributionEffectivenessinCoolingMode();

  bool setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode);

  bool setDesignZoneAirDistributionEffectivenessinHeatingMode(const Quantity& designZoneAirDistributionEffectivenessinHeatingMode);

  void resetDesignZoneAirDistributionEffectivenessinHeatingMode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:

  bool setThermalZone(const ThermalZone & thermalZone);

  /// @cond
  typedef detail::SizingZone_Impl ImplType;

  explicit SizingZone(std::shared_ptr<detail::SizingZone_Impl> impl);

  friend class detail::SizingZone_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class ThermalZone;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SizingZone");
};

/** \relates SizingZone*/
typedef boost::optional<SizingZone> OptionalSizingZone;

/** \relates SizingZone*/
typedef std::vector<SizingZone> SizingZoneVector;

} // model
} // openstudio

#endif // MODEL_SIZINGZONE_HPP

