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

#include "SizingZone.hpp"
#include "SizingZone_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SizingZone_Impl::SizingZone_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SizingZone::iddObjectType());
  }

  SizingZone_Impl::SizingZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SizingZone::iddObjectType());
  }

  SizingZone_Impl::SizingZone_Impl(const SizingZone_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SizingZone_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SizingZone_Impl::iddObjectType() const {
    return SizingZone::iddObjectType();
  }

  ThermalZone SizingZone_Impl::thermalZone() const {
    boost::optional<ThermalZone> value = optionalThermalZone();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Zone or Zone List attached.");
    }
    return value.get();
  }

  std::string SizingZone_Impl::zoneCoolingDesignSupplyAirTemperatureInputMethod() const {
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SizingZone_Impl::zoneCoolingDesignSupplyAirTemperature() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getZoneCoolingDesignSupplyAirTemperature(bool returnIP) const {
    OptionalDouble value = zoneCoolingDesignSupplyAirTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SizingZone_Impl::zoneCoolingDesignSupplyAirTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string SizingZone_Impl::zoneHeatingDesignSupplyAirTemperatureInputMethod() const {
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SizingZone_Impl::zoneHeatingDesignSupplyAirTemperature() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getZoneHeatingDesignSupplyAirTemperature(bool returnIP) const {
    OptionalDouble value = zoneHeatingDesignSupplyAirTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SizingZone_Impl::zoneHeatingDesignSupplyAirTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SizingZone_Impl::zoneCoolingDesignSupplyAirHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getZoneCoolingDesignSupplyAirHumidityRatio(bool returnIP) const {
    OptionalDouble value = zoneCoolingDesignSupplyAirHumidityRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SizingZone_Impl::zoneHeatingDesignSupplyAirHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getZoneHeatingDesignSupplyAirHumidityRatio(bool returnIP) const {
    OptionalDouble value = zoneHeatingDesignSupplyAirHumidityRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<double> SizingZone_Impl::zoneHeatingSizingFactor() const {
    return getDouble(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor,true);
  }

  OSOptionalQuantity SizingZone_Impl::getZoneHeatingSizingFactor(bool returnIP) const {
    OptionalDouble value = zoneHeatingSizingFactor();
    return getQuantityFromDouble(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor, value, returnIP);
  }

  boost::optional<double> SizingZone_Impl::zoneCoolingSizingFactor() const {
    return getDouble(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor,true);
  }

  OSOptionalQuantity SizingZone_Impl::getZoneCoolingSizingFactor(bool returnIP) const {
    OptionalDouble value = zoneCoolingSizingFactor();
    return getQuantityFromDouble(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor, value, returnIP);
  }

  std::string SizingZone_Impl::coolingDesignAirFlowMethod() const {
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SizingZone_Impl::isCoolingDesignAirFlowMethodDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod);
  }

  double SizingZone_Impl::coolingDesignAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getCoolingDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = coolingDesignAirFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isCoolingDesignAirFlowRateDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate);
  }

  double SizingZone_Impl::coolingMinimumAirFlowperZoneFloorArea() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getCoolingMinimumAirFlowperZoneFloorArea(bool returnIP) const {
    OptionalDouble value = coolingMinimumAirFlowperZoneFloorArea();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea);
  }

  double SizingZone_Impl::coolingMinimumAirFlow() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getCoolingMinimumAirFlow(bool returnIP) const {
    OptionalDouble value = coolingMinimumAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isCoolingMinimumAirFlowDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::CoolingMinimumAirFlow);
  }

  double SizingZone_Impl::coolingMinimumAirFlowFraction() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getCoolingMinimumAirFlowFraction(bool returnIP) const {
    OptionalDouble value = coolingMinimumAirFlowFraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isCoolingMinimumAirFlowFractionDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction);
  }

  std::string SizingZone_Impl::heatingDesignAirFlowMethod() const {
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SizingZone_Impl::isHeatingDesignAirFlowMethodDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod);
  }

  double SizingZone_Impl::heatingDesignAirFlowRate() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getHeatingDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = heatingDesignAirFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isHeatingDesignAirFlowRateDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate);
  }

  double SizingZone_Impl::heatingMaximumAirFlowperZoneFloorArea() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getHeatingMaximumAirFlowperZoneFloorArea(bool returnIP) const {
    OptionalDouble value = heatingMaximumAirFlowperZoneFloorArea();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea);
  }

  double SizingZone_Impl::heatingMaximumAirFlow() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getHeatingMaximumAirFlow(bool returnIP) const {
    OptionalDouble value = heatingMaximumAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isHeatingMaximumAirFlowDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::HeatingMaximumAirFlow);
  }

  double SizingZone_Impl::heatingMaximumAirFlowFraction() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getHeatingMaximumAirFlowFraction(bool returnIP) const {
    OptionalDouble value = heatingMaximumAirFlowFraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isHeatingMaximumAirFlowFractionDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction);
  }

  double SizingZone_Impl::designZoneAirDistributionEffectivenessinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getDesignZoneAirDistributionEffectivenessinCoolingMode(bool returnIP) const {
    OptionalDouble value = designZoneAirDistributionEffectivenessinCoolingMode();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode);
  }

  double SizingZone_Impl::designZoneAirDistributionEffectivenessinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SizingZone_Impl::getDesignZoneAirDistributionEffectivenessinHeatingMode(bool returnIP) const {
    OptionalDouble value = designZoneAirDistributionEffectivenessinHeatingMode();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SizingZone_Impl::isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const {
    return isEmpty(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode);
  }

  bool SizingZone_Impl::accountforDedicatedOutdoorAirSystem() const {
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  std::string SizingZone_Impl::dedicatedOutdoorAirSystemControlStrategy() const {
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> SizingZone_Impl::dedicatedOutdoorAirLowSetpointTemperatureforDesign() const {
    return getDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign,true);
  }

  bool SizingZone_Impl::isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> SizingZone_Impl::dedicatedOutdoorAirHighSetpointTemperatureforDesign() const {
    return getDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign,true);
  }

  bool SizingZone_Impl::isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  bool SizingZone_Impl::setThermalZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_Sizing_ZoneFields::ZoneorZoneListName, thermalZone.handle());
    return result;
  }

  void SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature) {
    bool result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, zoneCoolingDesignSupplyAirTemperature);
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperature(const Quantity& zoneCoolingDesignSupplyAirTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature,zoneCoolingDesignSupplyAirTemperature);
    if (!value) {
      return false;
    }
    setZoneCoolingDesignSupplyAirTemperature(value.get());
    return true;
  }

  void SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperatureDifference(double value) {
    bool result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, value);
    OS_ASSERT(result);
  }

  void SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature) {
    bool result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, zoneHeatingDesignSupplyAirTemperature);
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperature(const Quantity& zoneHeatingDesignSupplyAirTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature,zoneHeatingDesignSupplyAirTemperature);
    if (!value) {
      return false;
    }
    setZoneHeatingDesignSupplyAirTemperature(value.get());
    return true;
  }

  void SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperatureDifference(double value) {
    bool result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference, value);
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio) {
    bool result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, zoneCoolingDesignSupplyAirHumidityRatio);
    return result;
  }

  bool SizingZone_Impl::setZoneCoolingDesignSupplyAirHumidityRatio(const Quantity& zoneCoolingDesignSupplyAirHumidityRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio,zoneCoolingDesignSupplyAirHumidityRatio);
    if (!value) {
      return false;
    }
    return setZoneCoolingDesignSupplyAirHumidityRatio(value.get());
  }

  bool SizingZone_Impl::setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio) {
    bool result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, zoneHeatingDesignSupplyAirHumidityRatio);
    return result;
  }

  bool SizingZone_Impl::setZoneHeatingDesignSupplyAirHumidityRatio(const Quantity& zoneHeatingDesignSupplyAirHumidityRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio,zoneHeatingDesignSupplyAirHumidityRatio);
    if (!value) {
      return false;
    }
    return setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
  }

  bool SizingZone_Impl::setZoneHeatingSizingFactor(boost::optional<double> zoneHeatingSizingFactor) {
    bool result(false);
    if (zoneHeatingSizingFactor) {
      result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor, zoneHeatingSizingFactor.get());
    }
    else {
      resetZoneHeatingSizingFactor();
      result = true;
    }
    return result;
  }

  bool SizingZone_Impl::setZoneHeatingSizingFactor(const OSOptionalQuantity& zoneHeatingSizingFactor) {
    bool result(false);
    OptionalDouble value;
    if (zoneHeatingSizingFactor.isSet()) {
      value = getDoubleFromQuantity(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor,zoneHeatingSizingFactor.get());
      if (value) {
        result = setZoneHeatingSizingFactor(value);
      }
    }
    else {
      result = setZoneHeatingSizingFactor(value);
    }
    return result;
  }

  void SizingZone_Impl::resetZoneHeatingSizingFactor() {
    bool result = setString(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setZoneCoolingSizingFactor(boost::optional<double> zoneCoolingSizingFactor) {
    bool result(false);
    if (zoneCoolingSizingFactor) {
      result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor, zoneCoolingSizingFactor.get());
    }
    else {
      resetZoneCoolingSizingFactor();
      result = true;
    }
    return result;
  }

  bool SizingZone_Impl::setZoneCoolingSizingFactor(const OSOptionalQuantity& zoneCoolingSizingFactor) {
    bool result(false);
    OptionalDouble value;
    if (zoneCoolingSizingFactor.isSet()) {
      value = getDoubleFromQuantity(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor,zoneCoolingSizingFactor.get());
      if (value) {
        result = setZoneCoolingSizingFactor(value);
      }
    }
    else {
      result = setZoneCoolingSizingFactor(value);
    }
    return result;
  }

  void SizingZone_Impl::resetZoneCoolingSizingFactor() {
    bool result = setString(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setCoolingDesignAirFlowMethod(const std::string &coolingDesignAirFlowMethod) {
    bool result = setString(OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod, coolingDesignAirFlowMethod);
    return result;
  }

  void SizingZone_Impl::resetCoolingDesignAirFlowMethod() {
    bool result = setString(OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate) {
    bool result = setDouble(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate, coolingDesignAirFlowRate);
    return result;
  }

  bool SizingZone_Impl::setCoolingDesignAirFlowRate(const Quantity& coolingDesignAirFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate,coolingDesignAirFlowRate);
    if (!value) {
      return false;
    }
    return setCoolingDesignAirFlowRate(value.get());
  }

  void SizingZone_Impl::resetCoolingDesignAirFlowRate() {
    bool result = setString(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea) {
    bool result = setDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, coolingMinimumAirFlowperZoneFloorArea);
    return result;
  }

  bool SizingZone_Impl::setCoolingMinimumAirFlowperZoneFloorArea(const Quantity& coolingMinimumAirFlowperZoneFloorArea) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea,coolingMinimumAirFlowperZoneFloorArea);
    if (!value) {
      return false;
    }
    return setCoolingMinimumAirFlowperZoneFloorArea(value.get());
  }

  void SizingZone_Impl::resetCoolingMinimumAirFlowperZoneFloorArea() {
    bool result = setString(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setCoolingMinimumAirFlow(double coolingMinimumAirFlow) {
    bool result = setDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlow, coolingMinimumAirFlow);
    return result;
  }

  bool SizingZone_Impl::setCoolingMinimumAirFlow(const Quantity& coolingMinimumAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::CoolingMinimumAirFlow,coolingMinimumAirFlow);
    if (!value) {
      return false;
    }
    return setCoolingMinimumAirFlow(value.get());
  }

  void SizingZone_Impl::resetCoolingMinimumAirFlow() {
    bool result = setString(OS_Sizing_ZoneFields::CoolingMinimumAirFlow, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction) {
    bool result = setDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction, coolingMinimumAirFlowFraction);
    return result;
  }

  bool SizingZone_Impl::setCoolingMinimumAirFlowFraction(const Quantity& coolingMinimumAirFlowFraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction,coolingMinimumAirFlowFraction);
    if (!value) {
      return false;
    }
    return setCoolingMinimumAirFlowFraction(value.get());
  }

  void SizingZone_Impl::resetCoolingMinimumAirFlowFraction() {
    bool result = setString(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod) {
    bool result = setString(OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod, heatingDesignAirFlowMethod);
    return result;
  }

  void SizingZone_Impl::resetHeatingDesignAirFlowMethod() {
    bool result = setString(OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate) {
    bool result = setDouble(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate, heatingDesignAirFlowRate);
    return result;
  }

  bool SizingZone_Impl::setHeatingDesignAirFlowRate(const Quantity& heatingDesignAirFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate,heatingDesignAirFlowRate);
    if (!value) {
      return false;
    }
    return setHeatingDesignAirFlowRate(value.get());
  }

  void SizingZone_Impl::resetHeatingDesignAirFlowRate() {
    bool result = setString(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea) {
    bool result = setDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, heatingMaximumAirFlowperZoneFloorArea);
    return result;
  }

  bool SizingZone_Impl::setHeatingMaximumAirFlowperZoneFloorArea(const Quantity& heatingMaximumAirFlowperZoneFloorArea) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea,heatingMaximumAirFlowperZoneFloorArea);
    if (!value) {
      return false;
    }
    return setHeatingMaximumAirFlowperZoneFloorArea(value.get());
  }

  void SizingZone_Impl::resetHeatingMaximumAirFlowperZoneFloorArea() {
    bool result = setString(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setHeatingMaximumAirFlow(double heatingMaximumAirFlow) {
    bool result = setDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlow, heatingMaximumAirFlow);
    return result;
  }

  bool SizingZone_Impl::setHeatingMaximumAirFlow(const Quantity& heatingMaximumAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::HeatingMaximumAirFlow,heatingMaximumAirFlow);
    if (!value) {
      return false;
    }
    return setHeatingMaximumAirFlow(value.get());
  }

  void SizingZone_Impl::resetHeatingMaximumAirFlow() {
    bool result = setString(OS_Sizing_ZoneFields::HeatingMaximumAirFlow, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction) {
    bool result = setDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction, heatingMaximumAirFlowFraction);
    return result;
  }

  bool SizingZone_Impl::setHeatingMaximumAirFlowFraction(const Quantity& heatingMaximumAirFlowFraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction,heatingMaximumAirFlowFraction);
    if (!value) {
      return false;
    }
    return setHeatingMaximumAirFlowFraction(value.get());
  }

  void SizingZone_Impl::resetHeatingMaximumAirFlowFraction() {
    bool result = setString(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode) {
    bool result = setDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode, designZoneAirDistributionEffectivenessinCoolingMode);
    return result;
  }

  bool SizingZone_Impl::setDesignZoneAirDistributionEffectivenessinCoolingMode(const Quantity& designZoneAirDistributionEffectivenessinCoolingMode) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode,designZoneAirDistributionEffectivenessinCoolingMode);
    if (!value) {
      return false;
    }
    return setDesignZoneAirDistributionEffectivenessinCoolingMode(value.get());
  }

  void SizingZone_Impl::resetDesignZoneAirDistributionEffectivenessinCoolingMode() {
    bool result = setString(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode, "");
    OS_ASSERT(result);
  }

  bool SizingZone_Impl::setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode) {
    bool result = setDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode, designZoneAirDistributionEffectivenessinHeatingMode);
    return result;
  }

  bool SizingZone_Impl::setDesignZoneAirDistributionEffectivenessinHeatingMode(const Quantity& designZoneAirDistributionEffectivenessinHeatingMode) {
    OptionalDouble value = getDoubleFromQuantity(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode,designZoneAirDistributionEffectivenessinHeatingMode);
    if (!value) {
      return false;
    }
    return setDesignZoneAirDistributionEffectivenessinHeatingMode(value.get());
  }

  void SizingZone_Impl::resetDesignZoneAirDistributionEffectivenessinHeatingMode() {
    bool result = setString(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode, "");
    OS_ASSERT(result);
  }

  boost::optional<ThermalZone> SizingZone_Impl::optionalThermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Sizing_ZoneFields::ZoneorZoneListName);
  }

  openstudio::Quantity SizingZone_Impl::zoneCoolingDesignSupplyAirTemperature_SI() const {
    return getZoneCoolingDesignSupplyAirTemperature(false);
  }

  openstudio::Quantity SizingZone_Impl::zoneCoolingDesignSupplyAirTemperature_IP() const {
    return getZoneCoolingDesignSupplyAirTemperature(true);
  }

  openstudio::Quantity SizingZone_Impl::zoneHeatingDesignSupplyAirTemperature_SI() const {
    return getZoneHeatingDesignSupplyAirTemperature(false);
  }

  openstudio::Quantity SizingZone_Impl::zoneHeatingDesignSupplyAirTemperature_IP() const {
    return getZoneHeatingDesignSupplyAirTemperature(true);
  }

  openstudio::Quantity SizingZone_Impl::zoneCoolingDesignSupplyAirHumidityRatio_SI() const {
    return getZoneCoolingDesignSupplyAirHumidityRatio(false);
  }

  openstudio::Quantity SizingZone_Impl::zoneCoolingDesignSupplyAirHumidityRatio_IP() const {
    return getZoneCoolingDesignSupplyAirHumidityRatio(true);
  }

  openstudio::Quantity SizingZone_Impl::zoneHeatingDesignSupplyAirHumidityRatio_SI() const {
    return getZoneHeatingDesignSupplyAirHumidityRatio(false);
  }

  openstudio::Quantity SizingZone_Impl::zoneHeatingDesignSupplyAirHumidityRatio_IP() const {
    return getZoneHeatingDesignSupplyAirHumidityRatio(true);
  }

  openstudio::OSOptionalQuantity SizingZone_Impl::zoneHeatingSizingFactor_SI() const {
    return getZoneHeatingSizingFactor(false);
  }

  openstudio::OSOptionalQuantity SizingZone_Impl::zoneHeatingSizingFactor_IP() const {
    return getZoneHeatingSizingFactor(true);
  }

  openstudio::OSOptionalQuantity SizingZone_Impl::zoneCoolingSizingFactor_SI() const {
    return getZoneCoolingSizingFactor(false);
  }

  openstudio::OSOptionalQuantity SizingZone_Impl::zoneCoolingSizingFactor_IP() const {
    return getZoneCoolingSizingFactor(true);
  }

  std::vector<std::string> SizingZone_Impl::coolingDesignAirFlowMethodValues() const {
    return SizingZone::coolingDesignAirFlowMethodValues();
  }

  openstudio::Quantity SizingZone_Impl::coolingDesignAirFlowRate_SI() const {
    return getCoolingDesignAirFlowRate(false);
  }

  openstudio::Quantity SizingZone_Impl::coolingDesignAirFlowRate_IP() const {
    return getCoolingDesignAirFlowRate(true);
  }

  openstudio::Quantity SizingZone_Impl::coolingMinimumAirFlowperZoneFloorArea_SI() const {
    return getCoolingMinimumAirFlowperZoneFloorArea(false);
  }

  openstudio::Quantity SizingZone_Impl::coolingMinimumAirFlowperZoneFloorArea_IP() const {
    return getCoolingMinimumAirFlowperZoneFloorArea(true);
  }

  openstudio::Quantity SizingZone_Impl::coolingMinimumAirFlow_SI() const {
    return getCoolingMinimumAirFlow(false);
  }

  openstudio::Quantity SizingZone_Impl::coolingMinimumAirFlow_IP() const {
    return getCoolingMinimumAirFlow(true);
  }

  openstudio::Quantity SizingZone_Impl::coolingMinimumAirFlowFraction_SI() const {
    return getCoolingMinimumAirFlowFraction(false);
  }

  openstudio::Quantity SizingZone_Impl::coolingMinimumAirFlowFraction_IP() const {
    return getCoolingMinimumAirFlowFraction(true);
  }

  std::vector<std::string> SizingZone_Impl::heatingDesignAirFlowMethodValues() const {
    return SizingZone::heatingDesignAirFlowMethodValues();
  }

  openstudio::Quantity SizingZone_Impl::heatingDesignAirFlowRate_SI() const {
    return getHeatingDesignAirFlowRate(false);
  }

  openstudio::Quantity SizingZone_Impl::heatingDesignAirFlowRate_IP() const {
    return getHeatingDesignAirFlowRate(true);
  }

  openstudio::Quantity SizingZone_Impl::heatingMaximumAirFlowperZoneFloorArea_SI() const {
    return getHeatingMaximumAirFlowperZoneFloorArea(false);
  }

  openstudio::Quantity SizingZone_Impl::heatingMaximumAirFlowperZoneFloorArea_IP() const {
    return getHeatingMaximumAirFlowperZoneFloorArea(true);
  }

  openstudio::Quantity SizingZone_Impl::heatingMaximumAirFlow_SI() const {
    return getHeatingMaximumAirFlow(false);
  }

  openstudio::Quantity SizingZone_Impl::heatingMaximumAirFlow_IP() const {
    return getHeatingMaximumAirFlow(true);
  }

  openstudio::Quantity SizingZone_Impl::heatingMaximumAirFlowFraction_SI() const {
    return getHeatingMaximumAirFlowFraction(false);
  }

  openstudio::Quantity SizingZone_Impl::heatingMaximumAirFlowFraction_IP() const {
    return getHeatingMaximumAirFlowFraction(true);
  }

  openstudio::Quantity SizingZone_Impl::designZoneAirDistributionEffectivenessinCoolingMode_SI() const {
    return getDesignZoneAirDistributionEffectivenessinCoolingMode(false);
  }

  openstudio::Quantity SizingZone_Impl::designZoneAirDistributionEffectivenessinCoolingMode_IP() const {
    return getDesignZoneAirDistributionEffectivenessinCoolingMode(true);
  }

  openstudio::Quantity SizingZone_Impl::designZoneAirDistributionEffectivenessinHeatingMode_SI() const {
    return getDesignZoneAirDistributionEffectivenessinHeatingMode(false);
  }

  openstudio::Quantity SizingZone_Impl::designZoneAirDistributionEffectivenessinHeatingMode_IP() const {
    return getDesignZoneAirDistributionEffectivenessinHeatingMode(true);
  }

  boost::optional<ModelObject> SizingZone_Impl::thermalZoneAsModelObject() const {
    OptionalModelObject result = thermalZone();
    return result;
  }

  bool SizingZone_Impl::setThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
      if (intermediate) {
        ThermalZone thermalZone(*intermediate);
        return setThermalZone(thermalZone);
      }
    }
    return false;
  }

  bool SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string &value) {
    return setString(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, value);
  }

  bool SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string &value) {
    return setString(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, value);
  }

  void SizingZone_Impl::setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem) {
    setBooleanFieldValue(OS_Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, accountforDedicatedOutdoorAirSystem);
  }

  bool SizingZone_Impl::setDedicatedOutdoorAirSystemControlStrategy(std::string dedicatedOutdoorAirSystemControlStrategy) {
    bool result = setString(OS_Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy, dedicatedOutdoorAirSystemControlStrategy);
    return result;
  }

  void SizingZone_Impl::setDedicatedOutdoorAirLowSetpointTemperatureforDesign(boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign) {
    bool result(false);
    if (dedicatedOutdoorAirLowSetpointTemperatureforDesign) {
      result = setDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, dedicatedOutdoorAirLowSetpointTemperatureforDesign.get());
    }
    OS_ASSERT(result);
  }

  void SizingZone_Impl::autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign() {
    bool result = setString(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, "autosize");
    OS_ASSERT(result);
  }

  void SizingZone_Impl::setDedicatedOutdoorAirHighSetpointTemperatureforDesign(boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign) {
    bool result(false);
    if (dedicatedOutdoorAirHighSetpointTemperatureforDesign) {
      result = setDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, dedicatedOutdoorAirHighSetpointTemperatureforDesign.get());
    }
    OS_ASSERT(result);
  }

  void SizingZone_Impl::autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign() {
    bool result = setString(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, "autosize");
    OS_ASSERT(result);
  }
} // detail

SizingZone::SizingZone(const Model& model, const ThermalZone & thermalZone)
  : ModelObject(SizingZone::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SizingZone_Impl>());

  setThermalZone(thermalZone);

  setZoneCoolingDesignSupplyAirTemperatureInputMethod("SupplyAirTemperature");
  setZoneCoolingDesignSupplyAirTemperatureDifference(11.11);
  setZoneCoolingDesignSupplyAirTemperature(14.0);
  setZoneHeatingDesignSupplyAirTemperatureInputMethod("SupplyAirTemperature");
  setZoneHeatingDesignSupplyAirTemperatureDifference(11.11);
  setZoneHeatingDesignSupplyAirTemperature(40.0);
  setZoneCoolingDesignSupplyAirHumidityRatio(0.0085);
  setZoneHeatingDesignSupplyAirHumidityRatio(0.0080);
  setCoolingDesignAirFlowMethod("DesignDay");
  setHeatingDesignAirFlowMethod("DesignDay");
  setAccountforDedicatedOutdoorAirSystem(false);
  setDedicatedOutdoorAirSystemControlStrategy("NeutralSupplyAir");
  autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();
  autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();
}

IddObjectType SizingZone::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Sizing_Zone);
}

std::vector<std::string> SizingZone::coolingDesignAirFlowMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod);
}

std::vector<std::string> SizingZone::heatingDesignAirFlowMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod);
}

std::vector<std::string> SizingZone::zoneCoolingDesignSupplyAirTemperatureInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod);
}

std::vector<std::string> SizingZone::zoneHeatingDesignSupplyAirTemperatureInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod);
}

ThermalZone SizingZone::thermalZone() const {
  return getImpl<detail::SizingZone_Impl>()->thermalZone();
}

std::string SizingZone::zoneCoolingDesignSupplyAirTemperatureInputMethod() const {
  return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirTemperatureInputMethod();
}

double SizingZone::zoneCoolingDesignSupplyAirTemperature() const {
  return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirTemperature();
}

Quantity SizingZone::getZoneCoolingDesignSupplyAirTemperature(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getZoneCoolingDesignSupplyAirTemperature(returnIP);
}

double SizingZone::zoneCoolingDesignSupplyAirTemperatureDifference() const {
  return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirTemperatureDifference();
}

std::string SizingZone::zoneHeatingDesignSupplyAirTemperatureInputMethod() const {
  return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirTemperatureInputMethod();
}

double SizingZone::zoneHeatingDesignSupplyAirTemperature() const {
  return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirTemperature();
}

Quantity SizingZone::getZoneHeatingDesignSupplyAirTemperature(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getZoneHeatingDesignSupplyAirTemperature(returnIP);
}

double SizingZone::zoneHeatingDesignSupplyAirTemperatureDifference() const {
  return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirTemperatureDifference();
}

double SizingZone::zoneCoolingDesignSupplyAirHumidityRatio() const {
  return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirHumidityRatio();
}

Quantity SizingZone::getZoneCoolingDesignSupplyAirHumidityRatio(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getZoneCoolingDesignSupplyAirHumidityRatio(returnIP);
}

double SizingZone::zoneHeatingDesignSupplyAirHumidityRatio() const {
  return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirHumidityRatio();
}

Quantity SizingZone::getZoneHeatingDesignSupplyAirHumidityRatio(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getZoneHeatingDesignSupplyAirHumidityRatio(returnIP);
}

boost::optional<double> SizingZone::zoneHeatingSizingFactor() const {
  return getImpl<detail::SizingZone_Impl>()->zoneHeatingSizingFactor();
}

OSOptionalQuantity SizingZone::getZoneHeatingSizingFactor(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getZoneHeatingSizingFactor(returnIP);
}

boost::optional<double> SizingZone::zoneCoolingSizingFactor() const {
  return getImpl<detail::SizingZone_Impl>()->zoneCoolingSizingFactor();
}

OSOptionalQuantity SizingZone::getZoneCoolingSizingFactor(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getZoneCoolingSizingFactor(returnIP);
}

std::string SizingZone::coolingDesignAirFlowMethod() const {
  return getImpl<detail::SizingZone_Impl>()->coolingDesignAirFlowMethod();
}

bool SizingZone::isCoolingDesignAirFlowMethodDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isCoolingDesignAirFlowMethodDefaulted();
}

double SizingZone::coolingDesignAirFlowRate() const {
  return getImpl<detail::SizingZone_Impl>()->coolingDesignAirFlowRate();
}

Quantity SizingZone::getCoolingDesignAirFlowRate(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getCoolingDesignAirFlowRate(returnIP);
}

bool SizingZone::isCoolingDesignAirFlowRateDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isCoolingDesignAirFlowRateDefaulted();
}

double SizingZone::coolingMinimumAirFlowperZoneFloorArea() const {
  return getImpl<detail::SizingZone_Impl>()->coolingMinimumAirFlowperZoneFloorArea();
}

Quantity SizingZone::getCoolingMinimumAirFlowperZoneFloorArea(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getCoolingMinimumAirFlowperZoneFloorArea(returnIP);
}

bool SizingZone::isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isCoolingMinimumAirFlowperZoneFloorAreaDefaulted();
}

double SizingZone::coolingMinimumAirFlow() const {
  return getImpl<detail::SizingZone_Impl>()->coolingMinimumAirFlow();
}

Quantity SizingZone::getCoolingMinimumAirFlow(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getCoolingMinimumAirFlow(returnIP);
}

bool SizingZone::isCoolingMinimumAirFlowDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isCoolingMinimumAirFlowDefaulted();
}

double SizingZone::coolingMinimumAirFlowFraction() const {
  return getImpl<detail::SizingZone_Impl>()->coolingMinimumAirFlowFraction();
}

Quantity SizingZone::getCoolingMinimumAirFlowFraction(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getCoolingMinimumAirFlowFraction(returnIP);
}

bool SizingZone::isCoolingMinimumAirFlowFractionDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isCoolingMinimumAirFlowFractionDefaulted();
}

std::string SizingZone::heatingDesignAirFlowMethod() const {
  return getImpl<detail::SizingZone_Impl>()->heatingDesignAirFlowMethod();
}

bool SizingZone::isHeatingDesignAirFlowMethodDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isHeatingDesignAirFlowMethodDefaulted();
}

double SizingZone::heatingDesignAirFlowRate() const {
  return getImpl<detail::SizingZone_Impl>()->heatingDesignAirFlowRate();
}

Quantity SizingZone::getHeatingDesignAirFlowRate(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getHeatingDesignAirFlowRate(returnIP);
}

bool SizingZone::isHeatingDesignAirFlowRateDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isHeatingDesignAirFlowRateDefaulted();
}

double SizingZone::heatingMaximumAirFlowperZoneFloorArea() const {
  return getImpl<detail::SizingZone_Impl>()->heatingMaximumAirFlowperZoneFloorArea();
}

Quantity SizingZone::getHeatingMaximumAirFlowperZoneFloorArea(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getHeatingMaximumAirFlowperZoneFloorArea(returnIP);
}

bool SizingZone::isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isHeatingMaximumAirFlowperZoneFloorAreaDefaulted();
}

double SizingZone::heatingMaximumAirFlow() const {
  return getImpl<detail::SizingZone_Impl>()->heatingMaximumAirFlow();
}

Quantity SizingZone::getHeatingMaximumAirFlow(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getHeatingMaximumAirFlow(returnIP);
}

bool SizingZone::isHeatingMaximumAirFlowDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isHeatingMaximumAirFlowDefaulted();
}

double SizingZone::heatingMaximumAirFlowFraction() const {
  return getImpl<detail::SizingZone_Impl>()->heatingMaximumAirFlowFraction();
}

Quantity SizingZone::getHeatingMaximumAirFlowFraction(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getHeatingMaximumAirFlowFraction(returnIP);
}

bool SizingZone::isHeatingMaximumAirFlowFractionDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isHeatingMaximumAirFlowFractionDefaulted();
}

double SizingZone::designZoneAirDistributionEffectivenessinCoolingMode() const {
  return getImpl<detail::SizingZone_Impl>()->designZoneAirDistributionEffectivenessinCoolingMode();
}

Quantity SizingZone::getDesignZoneAirDistributionEffectivenessinCoolingMode(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getDesignZoneAirDistributionEffectivenessinCoolingMode(returnIP);
}

bool SizingZone::isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted();
}

double SizingZone::designZoneAirDistributionEffectivenessinHeatingMode() const {
  return getImpl<detail::SizingZone_Impl>()->designZoneAirDistributionEffectivenessinHeatingMode();
}

Quantity SizingZone::getDesignZoneAirDistributionEffectivenessinHeatingMode(bool returnIP) const {
  return getImpl<detail::SizingZone_Impl>()->getDesignZoneAirDistributionEffectivenessinHeatingMode(returnIP);
}

bool SizingZone::isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const {
  return getImpl<detail::SizingZone_Impl>()->isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted();
}

bool SizingZone::accountforDedicatedOutdoorAirSystem() const {
  return getImpl<detail::SizingZone_Impl>()->accountforDedicatedOutdoorAirSystem();
}

std::string SizingZone::dedicatedOutdoorAirSystemControlStrategy() const {
  return getImpl<detail::SizingZone_Impl>()->dedicatedOutdoorAirSystemControlStrategy();
}

boost::optional<double> SizingZone::dedicatedOutdoorAirLowSetpointTemperatureforDesign() const {
  return getImpl<detail::SizingZone_Impl>()->dedicatedOutdoorAirLowSetpointTemperatureforDesign();
}

bool SizingZone::isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized() const {
  return getImpl<detail::SizingZone_Impl>()->isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized();
}

boost::optional<double> SizingZone::dedicatedOutdoorAirHighSetpointTemperatureforDesign() const {
  return getImpl<detail::SizingZone_Impl>()->dedicatedOutdoorAirHighSetpointTemperatureforDesign();
}

bool SizingZone::isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized() const {
  return getImpl<detail::SizingZone_Impl>()->isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized();
}

bool SizingZone::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SizingZone_Impl>()->setThermalZone(thermalZone);
}

void SizingZone::setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature) {
  getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperature(zoneCoolingDesignSupplyAirTemperature);
}

bool SizingZone::setZoneCoolingDesignSupplyAirTemperature(const Quantity& zoneCoolingDesignSupplyAirTemperature) {
  return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperature(zoneCoolingDesignSupplyAirTemperature);
}

void SizingZone::setZoneCoolingDesignSupplyAirTemperatureDifference(double value) {
  getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperatureDifference(value);
}

void SizingZone::setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature) {
  getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperature(zoneHeatingDesignSupplyAirTemperature);
}

void SizingZone::setZoneHeatingDesignSupplyAirTemperatureDifference(double value) {
  getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperatureDifference(value);
}

bool SizingZone::setZoneHeatingDesignSupplyAirTemperature(const Quantity& zoneHeatingDesignSupplyAirTemperature) {
  return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperature(zoneHeatingDesignSupplyAirTemperature);
}

bool SizingZone::setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio) {
  return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirHumidityRatio(zoneCoolingDesignSupplyAirHumidityRatio);
}

bool SizingZone::setZoneCoolingDesignSupplyAirHumidityRatio(const Quantity& zoneCoolingDesignSupplyAirHumidityRatio) {
  return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirHumidityRatio(zoneCoolingDesignSupplyAirHumidityRatio);
}

bool SizingZone::setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio) {
  return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirHumidityRatio(zoneHeatingDesignSupplyAirHumidityRatio);
}

bool SizingZone::setZoneHeatingDesignSupplyAirHumidityRatio(const Quantity& zoneHeatingDesignSupplyAirHumidityRatio) {
  return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirHumidityRatio(zoneHeatingDesignSupplyAirHumidityRatio);
}

bool SizingZone::setZoneHeatingSizingFactor(double zoneHeatingSizingFactor) {
  return getImpl<detail::SizingZone_Impl>()->setZoneHeatingSizingFactor(zoneHeatingSizingFactor);
}

bool SizingZone::setZoneHeatingSizingFactor(const Quantity& zoneHeatingSizingFactor) {
  return getImpl<detail::SizingZone_Impl>()->setZoneHeatingSizingFactor(zoneHeatingSizingFactor);
}

void SizingZone::resetZoneHeatingSizingFactor() {
  getImpl<detail::SizingZone_Impl>()->resetZoneHeatingSizingFactor();
}

bool SizingZone::setZoneCoolingSizingFactor(double zoneCoolingSizingFactor) {
  return getImpl<detail::SizingZone_Impl>()->setZoneCoolingSizingFactor(zoneCoolingSizingFactor);
}

bool SizingZone::setZoneCoolingSizingFactor(const Quantity& zoneCoolingSizingFactor) {
  return getImpl<detail::SizingZone_Impl>()->setZoneCoolingSizingFactor(zoneCoolingSizingFactor);
}

void SizingZone::resetZoneCoolingSizingFactor() {
  getImpl<detail::SizingZone_Impl>()->resetZoneCoolingSizingFactor();
}

bool SizingZone::setCoolingDesignAirFlowMethod(const std::string &coolingDesignAirFlowMethod) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingDesignAirFlowMethod(coolingDesignAirFlowMethod);
}

void SizingZone::resetCoolingDesignAirFlowMethod() {
  getImpl<detail::SizingZone_Impl>()->resetCoolingDesignAirFlowMethod();
}

bool SizingZone::setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingDesignAirFlowRate(coolingDesignAirFlowRate);
}

bool SizingZone::setCoolingDesignAirFlowRate(const Quantity& coolingDesignAirFlowRate) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingDesignAirFlowRate(coolingDesignAirFlowRate);
}

void SizingZone::resetCoolingDesignAirFlowRate() {
  getImpl<detail::SizingZone_Impl>()->resetCoolingDesignAirFlowRate();
}

bool SizingZone::setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlowperZoneFloorArea(coolingMinimumAirFlowperZoneFloorArea);
}

bool SizingZone::setCoolingMinimumAirFlowperZoneFloorArea(const Quantity& coolingMinimumAirFlowperZoneFloorArea) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlowperZoneFloorArea(coolingMinimumAirFlowperZoneFloorArea);
}

void SizingZone::resetCoolingMinimumAirFlowperZoneFloorArea() {
  getImpl<detail::SizingZone_Impl>()->resetCoolingMinimumAirFlowperZoneFloorArea();
}

bool SizingZone::setCoolingMinimumAirFlow(double coolingMinimumAirFlow) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlow(coolingMinimumAirFlow);
}

bool SizingZone::setCoolingMinimumAirFlow(const Quantity& coolingMinimumAirFlow) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlow(coolingMinimumAirFlow);
}

void SizingZone::resetCoolingMinimumAirFlow() {
  getImpl<detail::SizingZone_Impl>()->resetCoolingMinimumAirFlow();
}

bool SizingZone::setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlowFraction(coolingMinimumAirFlowFraction);
}

bool SizingZone::setCoolingMinimumAirFlowFraction(const Quantity& coolingMinimumAirFlowFraction) {
  return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlowFraction(coolingMinimumAirFlowFraction);
}

void SizingZone::resetCoolingMinimumAirFlowFraction() {
  getImpl<detail::SizingZone_Impl>()->resetCoolingMinimumAirFlowFraction();
}

bool SizingZone::setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingDesignAirFlowMethod(heatingDesignAirFlowMethod);
}

void SizingZone::resetHeatingDesignAirFlowMethod() {
  getImpl<detail::SizingZone_Impl>()->resetHeatingDesignAirFlowMethod();
}

bool SizingZone::setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingDesignAirFlowRate(heatingDesignAirFlowRate);
}

bool SizingZone::setHeatingDesignAirFlowRate(const Quantity& heatingDesignAirFlowRate) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingDesignAirFlowRate(heatingDesignAirFlowRate);
}

void SizingZone::resetHeatingDesignAirFlowRate() {
  getImpl<detail::SizingZone_Impl>()->resetHeatingDesignAirFlowRate();
}

bool SizingZone::setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlowperZoneFloorArea(heatingMaximumAirFlowperZoneFloorArea);
}

bool SizingZone::setHeatingMaximumAirFlowperZoneFloorArea(const Quantity& heatingMaximumAirFlowperZoneFloorArea) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlowperZoneFloorArea(heatingMaximumAirFlowperZoneFloorArea);
}

void SizingZone::resetHeatingMaximumAirFlowperZoneFloorArea() {
  getImpl<detail::SizingZone_Impl>()->resetHeatingMaximumAirFlowperZoneFloorArea();
}

bool SizingZone::setHeatingMaximumAirFlow(double heatingMaximumAirFlow) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlow(heatingMaximumAirFlow);
}

bool SizingZone::setHeatingMaximumAirFlow(const Quantity& heatingMaximumAirFlow) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlow(heatingMaximumAirFlow);
}

void SizingZone::resetHeatingMaximumAirFlow() {
  getImpl<detail::SizingZone_Impl>()->resetHeatingMaximumAirFlow();
}

bool SizingZone::setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlowFraction(heatingMaximumAirFlowFraction);
}

bool SizingZone::setHeatingMaximumAirFlowFraction(const Quantity& heatingMaximumAirFlowFraction) {
  return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlowFraction(heatingMaximumAirFlowFraction);
}

void SizingZone::resetHeatingMaximumAirFlowFraction() {
  getImpl<detail::SizingZone_Impl>()->resetHeatingMaximumAirFlowFraction();
}

bool SizingZone::setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode) {
  return getImpl<detail::SizingZone_Impl>()->setDesignZoneAirDistributionEffectivenessinCoolingMode(designZoneAirDistributionEffectivenessinCoolingMode);
}

bool SizingZone::setDesignZoneAirDistributionEffectivenessinCoolingMode(const Quantity& designZoneAirDistributionEffectivenessinCoolingMode) {
  return getImpl<detail::SizingZone_Impl>()->setDesignZoneAirDistributionEffectivenessinCoolingMode(designZoneAirDistributionEffectivenessinCoolingMode);
}

void SizingZone::resetDesignZoneAirDistributionEffectivenessinCoolingMode() {
  getImpl<detail::SizingZone_Impl>()->resetDesignZoneAirDistributionEffectivenessinCoolingMode();
}

bool SizingZone::setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode) {
  return getImpl<detail::SizingZone_Impl>()->setDesignZoneAirDistributionEffectivenessinHeatingMode(designZoneAirDistributionEffectivenessinHeatingMode);
}

bool SizingZone::setDesignZoneAirDistributionEffectivenessinHeatingMode(const Quantity& designZoneAirDistributionEffectivenessinHeatingMode) {
  return getImpl<detail::SizingZone_Impl>()->setDesignZoneAirDistributionEffectivenessinHeatingMode(designZoneAirDistributionEffectivenessinHeatingMode);
}

void SizingZone::resetDesignZoneAirDistributionEffectivenessinHeatingMode() {
  getImpl<detail::SizingZone_Impl>()->resetDesignZoneAirDistributionEffectivenessinHeatingMode();
}

bool SizingZone::setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string &value) {
  return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperatureInputMethod(value);
}

bool SizingZone::setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string &value) {
  return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperatureInputMethod(value);
}

void SizingZone::setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem) {
  getImpl<detail::SizingZone_Impl>()->setAccountforDedicatedOutdoorAirSystem(accountforDedicatedOutdoorAirSystem);
}

bool SizingZone::setDedicatedOutdoorAirSystemControlStrategy(std::string dedicatedOutdoorAirSystemControlStrategy) {
  return getImpl<detail::SizingZone_Impl>()->setDedicatedOutdoorAirSystemControlStrategy(dedicatedOutdoorAirSystemControlStrategy);
}

void SizingZone::setDedicatedOutdoorAirLowSetpointTemperatureforDesign(double dedicatedOutdoorAirLowSetpointTemperatureforDesign) {
  getImpl<detail::SizingZone_Impl>()->setDedicatedOutdoorAirLowSetpointTemperatureforDesign(dedicatedOutdoorAirLowSetpointTemperatureforDesign);
}

void SizingZone::autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign() {
  getImpl<detail::SizingZone_Impl>()->autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();
}

void SizingZone::setDedicatedOutdoorAirHighSetpointTemperatureforDesign(double dedicatedOutdoorAirHighSetpointTemperatureforDesign) {
  getImpl<detail::SizingZone_Impl>()->setDedicatedOutdoorAirHighSetpointTemperatureforDesign(dedicatedOutdoorAirHighSetpointTemperatureforDesign);
}

void SizingZone::autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign() {
  getImpl<detail::SizingZone_Impl>()->autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();
}

/// @cond
SizingZone::SizingZone(std::shared_ptr<detail::SizingZone_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

