/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
  static std::vector<std::string> zoneCoolingDesignSupplyAirTemperatureInputMethodValues();
  static std::vector<std::string> zoneHeatingDesignSupplyAirTemperatureInputMethodValues();

  /** @name Getters */
  //@{

  ThermalZone thermalZone() const;

  std::string zoneCoolingDesignSupplyAirTemperatureInputMethod() const;

  double zoneCoolingDesignSupplyAirTemperature() const;

  Quantity getZoneCoolingDesignSupplyAirTemperature(bool returnIP=false) const;

  double zoneCoolingDesignSupplyAirTemperatureDifference() const;

  std::string zoneHeatingDesignSupplyAirTemperatureInputMethod() const;

  double zoneHeatingDesignSupplyAirTemperature() const;

  Quantity getZoneHeatingDesignSupplyAirTemperature(bool returnIP=false) const;

  double zoneHeatingDesignSupplyAirTemperatureDifference() const;

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

  bool accountforDedicatedOutdoorAirSystem() const;

  std::string dedicatedOutdoorAirSystemControlStrategy() const;

  boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign() const;

  bool isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() const;

  boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign() const;

  bool isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() const;

  //@}
  /** @name Setters */
  //@{

  bool setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string &value);

  bool setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature);

  bool setZoneCoolingDesignSupplyAirTemperature(const Quantity& zoneCoolingDesignSupplyAirTemperature);

  bool setZoneCoolingDesignSupplyAirTemperatureDifference(double value);

  bool setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string &value);

  bool setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature);

  bool setZoneHeatingDesignSupplyAirTemperature(const Quantity& zoneHeatingDesignSupplyAirTemperature);

  bool setZoneHeatingDesignSupplyAirTemperatureDifference(double value);

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

  bool setCoolingDesignAirFlowMethod(const std::string &coolingDesignAirFlowMethod);

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

  bool setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem);

  bool setDedicatedOutdoorAirSystemControlStrategy(std::string dedicatedOutdoorAirSystemControlStrategy);

  bool setDedicatedOutdoorAirLowSetpointTemperatureforDesign(double dedicatedOutdoorAirLowSetpointTemperatureforDesign);

  void autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();

  bool setDedicatedOutdoorAirHighSetpointTemperatureforDesign(double dedicatedOutdoorAirHighSetpointTemperatureforDesign);

  void autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign() const ;

  boost::optional<double> autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign() const ;

  void autosize();

  void applySizingValues();

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
