/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SizingZone.hpp"
#include "SizingZone_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/sql/SqlFile.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SizingZone_Impl::SizingZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SizingZone::iddObjectType());
    }

    SizingZone_Impl::SizingZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SizingZone::iddObjectType());
    }

    SizingZone_Impl::SizingZone_Impl(const SizingZone_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SizingZone_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SizingZone_Impl::iddObjectType() const {
      return SizingZone::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SizingZone_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName) != e) {
        result.emplace_back("SizingZone", "Zone Humidistat Dehumidification Set Point");
      }
      if (std::find(b, e, OS_Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName) != e) {
        result.emplace_back("SizingZone", "Zone Humidistat Humidification Set Point");
      }
      return result;
    }

    ThermalZone SizingZone_Impl::thermalZone() const {
      boost::optional<ThermalZone> value = optionalThermalZone();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Zone or Zone List attached.");
      }
      return value.get();
    }

    std::string SizingZone_Impl::zoneCoolingDesignSupplyAirTemperatureInputMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingZone_Impl::zoneCoolingDesignSupplyAirTemperature() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingZone_Impl::zoneCoolingDesignSupplyAirTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string SizingZone_Impl::zoneHeatingDesignSupplyAirTemperatureInputMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingZone_Impl::zoneHeatingDesignSupplyAirTemperature() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingZone_Impl::zoneHeatingDesignSupplyAirTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingZone_Impl::zoneCoolingDesignSupplyAirHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SizingZone_Impl::zoneHeatingDesignSupplyAirHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SizingZone_Impl::zoneHeatingSizingFactor() const {
      return getDouble(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor, true);
    }

    boost::optional<double> SizingZone_Impl::zoneCoolingSizingFactor() const {
      return getDouble(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor, true);
    }

    std::string SizingZone_Impl::coolingDesignAirFlowMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isCoolingDesignAirFlowMethodDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod);
    }

    double SizingZone_Impl::coolingDesignAirFlowRate() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isCoolingDesignAirFlowRateDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate);
    }

    double SizingZone_Impl::coolingMinimumAirFlowperZoneFloorArea() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea);
    }

    double SizingZone_Impl::coolingMinimumAirFlow() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isCoolingMinimumAirFlowDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::CoolingMinimumAirFlow);
    }

    double SizingZone_Impl::coolingMinimumAirFlowFraction() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isCoolingMinimumAirFlowFractionDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction);
    }

    std::string SizingZone_Impl::heatingDesignAirFlowMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isHeatingDesignAirFlowMethodDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod);
    }

    double SizingZone_Impl::heatingDesignAirFlowRate() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isHeatingDesignAirFlowRateDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate);
    }

    double SizingZone_Impl::heatingMaximumAirFlowperZoneFloorArea() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea);
    }

    double SizingZone_Impl::heatingMaximumAirFlow() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isHeatingMaximumAirFlowDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::HeatingMaximumAirFlow);
    }

    double SizingZone_Impl::heatingMaximumAirFlowFraction() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isHeatingMaximumAirFlowFractionDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction);
    }

    double SizingZone_Impl::designZoneAirDistributionEffectivenessinCoolingMode() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode);
    }

    double SizingZone_Impl::designZoneAirDistributionEffectivenessinHeatingMode() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode);
    }

    double SizingZone_Impl::designZoneSecondaryRecirculationFraction() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::DesignZoneSecondaryRecirculationFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isDesignZoneSecondaryRecirculationFractionDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::DesignZoneSecondaryRecirculationFraction);
    }

    double SizingZone_Impl::designMinimumZoneVentilationEfficiency() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::DesignMinimumZoneVentilationEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::isDesignMinimumZoneVentilationEfficiencyDefaulted() const {
      return isEmpty(OS_Sizing_ZoneFields::DesignMinimumZoneVentilationEfficiency);
    }

    bool SizingZone_Impl::accountforDedicatedOutdoorAirSystem() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    std::string SizingZone_Impl::dedicatedOutdoorAirSystemControlStrategy() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> SizingZone_Impl::dedicatedOutdoorAirLowSetpointTemperatureforDesign() const {
      return getDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, true);
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
      return getDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, true);
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

    bool SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, zoneCoolingDesignSupplyAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperatureDifference(double value) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, value);
      OS_ASSERT(result);
      return result;
    }

    bool SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, zoneHeatingDesignSupplyAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperatureDifference(double value) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference, value);
      OS_ASSERT(result);
      return result;
    }

    bool SizingZone_Impl::setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, zoneCoolingDesignSupplyAirHumidityRatio);
      return result;
    }

    bool SizingZone_Impl::setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, zoneHeatingDesignSupplyAirHumidityRatio);
      return result;
    }

    bool SizingZone_Impl::setZoneHeatingSizingFactor(boost::optional<double> zoneHeatingSizingFactor) {
      bool result(false);
      if (zoneHeatingSizingFactor) {
        result = setDouble(OS_Sizing_ZoneFields::ZoneHeatingSizingFactor, zoneHeatingSizingFactor.get());
      } else {
        resetZoneHeatingSizingFactor();
        result = true;
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
      } else {
        resetZoneCoolingSizingFactor();
        result = true;
      }
      return result;
    }

    void SizingZone_Impl::resetZoneCoolingSizingFactor() {
      bool result = setString(OS_Sizing_ZoneFields::ZoneCoolingSizingFactor, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod) {
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

    void SizingZone_Impl::resetCoolingDesignAirFlowRate() {
      bool result = setString(OS_Sizing_ZoneFields::CoolingDesignAirFlowRate, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea) {
      bool result = setDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, coolingMinimumAirFlowperZoneFloorArea);
      return result;
    }

    void SizingZone_Impl::resetCoolingMinimumAirFlowperZoneFloorArea() {
      bool result = setString(OS_Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setCoolingMinimumAirFlow(double coolingMinimumAirFlow) {
      bool result = setDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlow, coolingMinimumAirFlow);
      return result;
    }

    void SizingZone_Impl::resetCoolingMinimumAirFlow() {
      bool result = setString(OS_Sizing_ZoneFields::CoolingMinimumAirFlow, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction) {
      bool result = setDouble(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction, coolingMinimumAirFlowFraction);
      return result;
    }

    void SizingZone_Impl::resetCoolingMinimumAirFlowFraction() {
      bool result = setString(OS_Sizing_ZoneFields::CoolingMinimumAirFlowFraction, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod) {
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

    void SizingZone_Impl::resetHeatingDesignAirFlowRate() {
      bool result = setString(OS_Sizing_ZoneFields::HeatingDesignAirFlowRate, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea) {
      bool result = setDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, heatingMaximumAirFlowperZoneFloorArea);
      return result;
    }

    void SizingZone_Impl::resetHeatingMaximumAirFlowperZoneFloorArea() {
      bool result = setString(OS_Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setHeatingMaximumAirFlow(double heatingMaximumAirFlow) {
      bool result = setDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlow, heatingMaximumAirFlow);
      return result;
    }

    void SizingZone_Impl::resetHeatingMaximumAirFlow() {
      bool result = setString(OS_Sizing_ZoneFields::HeatingMaximumAirFlow, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction) {
      bool result = setDouble(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction, heatingMaximumAirFlowFraction);
      return result;
    }

    void SizingZone_Impl::resetHeatingMaximumAirFlowFraction() {
      bool result = setString(OS_Sizing_ZoneFields::HeatingMaximumAirFlowFraction, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode) {
      bool result =
        setDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode, designZoneAirDistributionEffectivenessinCoolingMode);
      return result;
    }

    void SizingZone_Impl::resetDesignZoneAirDistributionEffectivenessinCoolingMode() {
      bool result = setString(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinCoolingMode, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode) {
      bool result =
        setDouble(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode, designZoneAirDistributionEffectivenessinHeatingMode);
      return result;
    }

    void SizingZone_Impl::resetDesignZoneAirDistributionEffectivenessinHeatingMode() {
      bool result = setString(OS_Sizing_ZoneFields::DesignZoneAirDistributionEffectivenessinHeatingMode, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setDesignZoneSecondaryRecirculationFraction(double designZoneSecondaryRecirculationFraction) {
      bool result = setDouble(OS_Sizing_ZoneFields::DesignZoneSecondaryRecirculationFraction, designZoneSecondaryRecirculationFraction);
      return result;
    }

    void SizingZone_Impl::resetDesignZoneSecondaryRecirculationFraction() {
      bool result = setString(OS_Sizing_ZoneFields::DesignZoneSecondaryRecirculationFraction, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setDesignMinimumZoneVentilationEfficiency(double designMinimumZoneVentilationEfficiency) {
      bool result = setDouble(OS_Sizing_ZoneFields::DesignMinimumZoneVentilationEfficiency, designMinimumZoneVentilationEfficiency);
      return result;
    }

    void SizingZone_Impl::resetDesignMinimumZoneVentilationEfficiency() {
      bool result = setString(OS_Sizing_ZoneFields::DesignMinimumZoneVentilationEfficiency, "");
      OS_ASSERT(result);
    }

    boost::optional<ThermalZone> SizingZone_Impl::optionalThermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Sizing_ZoneFields::ZoneorZoneListName);
    }

    std::vector<std::string> SizingZone_Impl::coolingDesignAirFlowMethodValues() const {
      return SizingZone::coolingDesignAirFlowMethodValues();
    }

    std::vector<std::string> SizingZone_Impl::heatingDesignAirFlowMethodValues() const {
      return SizingZone::heatingDesignAirFlowMethodValues();
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

    bool SizingZone_Impl::setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string& value) {
      return setString(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, value);
    }

    bool SizingZone_Impl::setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string& value) {
      return setString(OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, value);
    }

    bool SizingZone_Impl::setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem) {
      return setBooleanFieldValue(OS_Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, accountforDedicatedOutdoorAirSystem);
      ;
    }

    bool SizingZone_Impl::setDedicatedOutdoorAirSystemControlStrategy(const std::string& dedicatedOutdoorAirSystemControlStrategy) {
      bool result = setString(OS_Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy, dedicatedOutdoorAirSystemControlStrategy);
      return result;
    }

    std::string SizingZone_Impl::zoneLoadSizingMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneLoadSizingMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::setZoneLoadSizingMethod(const std::string& zoneLoadSizingMethod) {
      bool result = setString(OS_Sizing_ZoneFields::ZoneLoadSizingMethod, zoneLoadSizingMethod);
      return result;
    }

    std::string SizingZone_Impl::zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod(
      const std::string& zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod) {
      bool result = setString(OS_Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod,
                              zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod);
      return result;
    }

    boost::optional<double> SizingZone_Impl::zoneDehumidificationDesignSupplyAirHumidityRatio() const {
      return getDouble(OS_Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio, true);
    }

    bool SizingZone_Impl::setZoneDehumidificationDesignSupplyAirHumidityRatio(double zoneDehumidificationDesignSupplyAirHumidityRatio) {
      bool result =
        setDouble(OS_Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio, zoneDehumidificationDesignSupplyAirHumidityRatio);
      return result;
    }

    void SizingZone_Impl::resetZoneDehumidificationDesignSupplyAirHumidityRatio() {
      bool result = setString(OS_Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio, "");
      OS_ASSERT(result);
    }

    double SizingZone_Impl::zoneCoolingDesignSupplyAirHumidityRatioDifference() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::setZoneCoolingDesignSupplyAirHumidityRatioDifference(double zoneCoolingDesignSupplyAirHumidityRatioDifference) {
      bool result =
        setDouble(OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference, zoneCoolingDesignSupplyAirHumidityRatioDifference);
      return result;
    }

    std::string SizingZone_Impl::zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod(
      const std::string& zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod) {
      bool result = setString(OS_Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod,
                              zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod);
      return result;
    }

    boost::optional<double> SizingZone_Impl::zoneHumidificationDesignSupplyAirHumidityRatio() const {
      return getDouble(OS_Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio, true);
    }

    bool SizingZone_Impl::setZoneHumidificationDesignSupplyAirHumidityRatio(double zoneHumidificationDesignSupplyAirHumidityRatio) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio, zoneHumidificationDesignSupplyAirHumidityRatio);
      return result;
    }

    void SizingZone_Impl::resetZoneHumidificationDesignSupplyAirHumidityRatio() {
      bool result = setString(OS_Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio, "");
      OS_ASSERT(result);
    }

    double SizingZone_Impl::zoneHumidificationDesignSupplyAirHumidityRatioDifference() const {
      boost::optional<double> value = getDouble(OS_Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool
      SizingZone_Impl::setZoneHumidificationDesignSupplyAirHumidityRatioDifference(double zoneHumidificationDesignSupplyAirHumidityRatioDifference) {
      bool result = setDouble(OS_Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference,
                              zoneHumidificationDesignSupplyAirHumidityRatioDifference);
      return result;
    }

    boost::optional<Schedule> SizingZone_Impl::zoneHumidistatDehumidificationSetPointSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName);
    }

    bool SizingZone_Impl::setZoneHumidistatDehumidificationSetPointSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName, "SizingZone",
                                "Zone Humidistat Dehumidification Set Point", schedule);
      return result;
    }

    void SizingZone_Impl::resetZoneHumidistatDehumidificationSetPointSchedule() {
      bool result = setString(OS_Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> SizingZone_Impl::zoneHumidistatHumidificationSetPointSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName);
    }

    bool SizingZone_Impl::setZoneHumidistatHumidificationSetPointSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName, "SizingZone",
                                "Zone Humidistat Humidification Set Point", schedule);
      return result;
    }

    void SizingZone_Impl::resetZoneHumidistatHumidificationSetPointSchedule() {
      bool result = setString(OS_Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName, "");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setDedicatedOutdoorAirLowSetpointTemperatureforDesign(
      boost::optional<double> dedicatedOutdoorAirLowSetpointTemperatureforDesign) {
      bool result(false);
      if (dedicatedOutdoorAirLowSetpointTemperatureforDesign) {
        result = setDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign,
                           dedicatedOutdoorAirLowSetpointTemperatureforDesign.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void SizingZone_Impl::autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign() {
      bool result = setString(OS_Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, "autosize");
      OS_ASSERT(result);
    }

    bool SizingZone_Impl::setDedicatedOutdoorAirHighSetpointTemperatureforDesign(
      boost::optional<double> dedicatedOutdoorAirHighSetpointTemperatureforDesign) {
      bool result(false);
      if (dedicatedOutdoorAirHighSetpointTemperatureforDesign) {
        result = setDouble(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign,
                           dedicatedOutdoorAirHighSetpointTemperatureforDesign.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void SizingZone_Impl::autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign() {
      bool result = setString(OS_Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, "autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> SizingZone_Impl::autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign() const {
      boost::optional<double> result;

      std::string setpointType = "Low";
      std::string setpointParam = "DOAS Design " + setpointType + " Setpoint Temperature {C}";

      // Get the parent ThermalZone
      ThermalZone parZone = thermalZone();

      // Get the name of the thermal zone
      if (!parZone.name()) {
        LOG(Warn, "This object's parent ThermalZone does not have a name, cannot retrieve the autosized Dedicated Outdoor Air" + setpointType
                    + " Setpoint Temperature.");
        return result;
      }

      // Get the object name and transform to the way it is recorded
      // in the sql file
      std::string sqlName = parZone.name().get();
      boost::to_upper(sqlName);

      // Check that the model has a sql file
      if (!model().sqlFile()) {
        LOG(Warn, "This model has no sql file, cannot retrieve the autosized Dedicated Outdoor Air" + setpointType + " Setpoint Temperature.");
        return result;
      }

      // Query the InitializationSummary -> Zone Sizing DOAS Inputs Information table to get
      // the row names that contains information for this component.
      std::string rowsQuery = R"(
      SELECT RowName FROM TabularDataWithStrings
        WHERE ReportName = 'InitializationSummary'
        AND ReportForString = 'Entire Facility'
        AND TableName = 'Zone Sizing DOAS Inputs'
        AND Value = ?;)";

      boost::optional<std::vector<std::string>> rowNames = model().sqlFile().get().execAndReturnVectorOfString(rowsQuery,
                                                                                                               // Bind args
                                                                                                               sqlName);

      // Warn if the query failed
      if (!rowNames) {
        LOG(Debug, "Could not find a component called '" + sqlName + "' in any rows of the InitializationSummary Zone Sizing DOAS Inputs table.");
        return result;
      }

      // Query each row of the InitializationSummary -> Zone Sizing DOAS Inputs table
      // that contains this component to get the desired value.
      for (const std::string& rowName : rowNames.get()) {
        std::string valQuery = R"(
        SELECT Value FROM TabularDataWithStrings
          WHERE ReportName = 'InitializationSummary'
          AND ReportForString = 'Entire Facility'
          AND TableName = 'Zone Sizing DOAS Inputs'
          AND RowName = ?
          AND ColumnName = ?;)";
        boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery, rowName, setpointParam);

        // Check if the query succeeded
        if (val) {
          result = val.get();
          break;
        }
      }

      if (!result) {
        LOG(Debug,
            "The autosized value query for Dedicated Outdoor Air " + setpointType + " Setpoint Temperature for " + sqlName + " returned no value.");
      }

      return result;
    }

    boost::optional<double> SizingZone_Impl::autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign() const {
      boost::optional<double> result;

      std::string setpointType = "High";
      std::string setpointParam = "DOAS Design " + setpointType + " Setpoint Temperature {C}";

      // Get the parent ThermalZone
      ThermalZone parZone = thermalZone();

      // Get the name of the thermal zone
      if (!parZone.name()) {
        LOG(Warn, "This object's parent ThermalZone does not have a name, cannot retrieve the autosized Dedicated Outdoor Air" + setpointType
                    + " Setpoint Temperature.");
        return result;
      }

      // Get the object name and transform to the way it is recorded
      // in the sql file
      std::string sqlName = parZone.name().get();
      boost::to_upper(sqlName);

      // Check that the model has a sql file
      if (!model().sqlFile()) {
        LOG(Warn, "This model has no sql file, cannot retrieve the autosized Dedicated Outdoor Air" + setpointType + " Setpoint Temperature.");
        return result;
      }

      // Query the InitializationSummary -> Zone Sizing DOAS Inputs Information table to get
      // the row names that contains information for this component.
      std::string rowsQuery = R"(
      SELECT RowName FROM TabularDataWithStrings
        WHERE ReportName = 'InitializationSummary'
        AND ReportForString = 'Entire Facility'
        AND TableName = 'Zone Sizing DOAS Inputs'
        AND Value = ?;)";

      boost::optional<std::vector<std::string>> rowNames = model().sqlFile().get().execAndReturnVectorOfString(rowsQuery,
                                                                                                               // Bind args
                                                                                                               sqlName);

      // Warn if the query failed
      if (!rowNames) {
        LOG(Debug, "Could not find a component called '" + sqlName + "' in any rows of the InitializationSummary Zone Sizing DOAS Inputs table.");
        return result;
      }

      // Query each row of the InitializationSummary -> Zone Sizing DOAS Inputs table
      // that contains this component to get the desired value.
      for (const std::string& rowName : rowNames.get()) {
        std::string valQuery = R"(
        SELECT Value FROM TabularDataWithStrings
          WHERE ReportName = 'InitializationSummary'
          AND ReportForString = 'Entire Facility'
          AND TableName = 'Zone Sizing DOAS Inputs'
          AND RowName = ?
          AND ColumnName = ?;)";
        boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery, rowName, setpointParam);

        // Check if the query succeeded
        if (val) {
          result = val.get();
          break;
        }
      }

      if (!result) {
        LOG(Debug,
            "The autosized value query for Dedicated Outdoor Air " + setpointType + " Setpoint Temperature for " + sqlName + " returned no value.");
      }

      return result;
    }

    std::string SizingZone_Impl::sizingOption() const {
      boost::optional<std::string> value = getString(OS_Sizing_ZoneFields::SizingOption, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SizingZone_Impl::setSizingOption(const std::string& sizingOption) {
      bool result = setString(OS_Sizing_ZoneFields::SizingOption, sizingOption);
      return result;
    }

    void SizingZone_Impl::autosize() {
      autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();
      autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();
    }

    void SizingZone_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign();
      if (val) {
        setDedicatedOutdoorAirLowSetpointTemperatureforDesign(val.get());
      }

      val = autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign();
      if (val) {
        setDedicatedOutdoorAirHighSetpointTemperatureforDesign(val.get());
      }
    }

    std::vector<EMSActuatorNames> SizingZone_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Sizing:Zone", "Zone Design Heating Air Mass Flow Rate"},
                                              {"Sizing:Zone", "Zone Design Cooling Air Mass Flow Rate"},
                                              {"Sizing:Zone", "Zone Design Heating Load"},
                                              {"Sizing:Zone", "Zone Design Cooling Load"},
                                              {"Sizing:Zone", "Zone Design Heating Vol Flow"},
                                              {"Sizing:Zone", "Zone Design Cooling Vol Flow"}};
      return actuators;
    }

    std::vector<std::string> SizingZone_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Final Zone Design Heating Air Mass Flow Rate",
                                     "Intermediate Zone Design Heating Air Mass Flow Rate",
                                     "Final Zone Design Cooling Air Mass Flow Rate",
                                     "Intermediate Zone Design Cooling Air Mass Flow Rate",
                                     "Final Zone Design Heating Load",
                                     "Intermediate Zone Design Heating Load",
                                     "Final Zone Design Cooling Load",
                                     "Intermediate Zone Design Cooling Load",
                                     "Final Zone Design Heating Air Density",
                                     "Intermediate Zone Design Heating Air Density",
                                     "Final Zone Design Cooling Air Density",
                                     "Intermediate Zone Design Cooling Air Density",
                                     "Final Zone Design Heating Volume Flow",
                                     "Intermediate Zone Design Heating Volume Flow",
                                     "Final Zone Design Cooling Volume Flow",
                                     "Intermediate Zone Design Cooling Volume Flow",
                                     "Zone Outdoor Air Design Volume Flow Rate"};
      return types;
    }
  }  // namespace detail

  SizingZone::SizingZone(const Model& model, const ThermalZone& thermalZone) : ModelObject(SizingZone::iddObjectType(), model) {
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

    // New E+ 22.2.0 fields, IDD defaults
    setZoneLoadSizingMethod("Sensible Load Only No Latent Load");
    setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod("HumidityRatioDifference");
    setZoneCoolingDesignSupplyAirHumidityRatioDifference(0.005);
    setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod("HumidityRatioDifference");
    setZoneHumidificationDesignSupplyAirHumidityRatioDifference(0.005);

    // New E+ 24.2.0 fields, IDD defaults
    setSizingOption("Coincident");
  }

  IddObjectType SizingZone::iddObjectType() {
    return {IddObjectType::OS_Sizing_Zone};
  }

  std::vector<std::string> SizingZone::coolingDesignAirFlowMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_ZoneFields::CoolingDesignAirFlowMethod);
  }

  std::vector<std::string> SizingZone::heatingDesignAirFlowMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_ZoneFields::HeatingDesignAirFlowMethod);
  }

  std::vector<std::string> SizingZone::zoneCoolingDesignSupplyAirTemperatureInputMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod);
  }

  std::vector<std::string> SizingZone::zoneHeatingDesignSupplyAirTemperatureInputMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod);
  }

  std::vector<std::string> SizingZone::zoneLoadSizingMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_ZoneFields::ZoneLoadSizingMethod);
  }

  std::vector<std::string> SizingZone::zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod);
  }
  std::vector<std::string> SizingZone::zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod);
  }

  std::vector<std::string> SizingZone::validSizingOptionValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Sizing_ZoneFields::SizingOption);
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

  double SizingZone::zoneCoolingDesignSupplyAirTemperatureDifference() const {
    return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirTemperatureDifference();
  }

  std::string SizingZone::zoneHeatingDesignSupplyAirTemperatureInputMethod() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirTemperatureInputMethod();
  }

  double SizingZone::zoneHeatingDesignSupplyAirTemperature() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirTemperature();
  }

  double SizingZone::zoneHeatingDesignSupplyAirTemperatureDifference() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirTemperatureDifference();
  }

  double SizingZone::zoneCoolingDesignSupplyAirHumidityRatio() const {
    return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirHumidityRatio();
  }

  double SizingZone::zoneHeatingDesignSupplyAirHumidityRatio() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHeatingDesignSupplyAirHumidityRatio();
  }

  boost::optional<double> SizingZone::zoneHeatingSizingFactor() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHeatingSizingFactor();
  }

  boost::optional<double> SizingZone::zoneCoolingSizingFactor() const {
    return getImpl<detail::SizingZone_Impl>()->zoneCoolingSizingFactor();
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

  bool SizingZone::isCoolingDesignAirFlowRateDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isCoolingDesignAirFlowRateDefaulted();
  }

  double SizingZone::coolingMinimumAirFlowperZoneFloorArea() const {
    return getImpl<detail::SizingZone_Impl>()->coolingMinimumAirFlowperZoneFloorArea();
  }

  bool SizingZone::isCoolingMinimumAirFlowperZoneFloorAreaDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isCoolingMinimumAirFlowperZoneFloorAreaDefaulted();
  }

  double SizingZone::coolingMinimumAirFlow() const {
    return getImpl<detail::SizingZone_Impl>()->coolingMinimumAirFlow();
  }

  bool SizingZone::isCoolingMinimumAirFlowDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isCoolingMinimumAirFlowDefaulted();
  }

  double SizingZone::coolingMinimumAirFlowFraction() const {
    return getImpl<detail::SizingZone_Impl>()->coolingMinimumAirFlowFraction();
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

  bool SizingZone::isHeatingDesignAirFlowRateDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isHeatingDesignAirFlowRateDefaulted();
  }

  double SizingZone::heatingMaximumAirFlowperZoneFloorArea() const {
    return getImpl<detail::SizingZone_Impl>()->heatingMaximumAirFlowperZoneFloorArea();
  }

  bool SizingZone::isHeatingMaximumAirFlowperZoneFloorAreaDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isHeatingMaximumAirFlowperZoneFloorAreaDefaulted();
  }

  double SizingZone::heatingMaximumAirFlow() const {
    return getImpl<detail::SizingZone_Impl>()->heatingMaximumAirFlow();
  }

  bool SizingZone::isHeatingMaximumAirFlowDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isHeatingMaximumAirFlowDefaulted();
  }

  double SizingZone::heatingMaximumAirFlowFraction() const {
    return getImpl<detail::SizingZone_Impl>()->heatingMaximumAirFlowFraction();
  }

  bool SizingZone::isHeatingMaximumAirFlowFractionDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isHeatingMaximumAirFlowFractionDefaulted();
  }

  double SizingZone::designZoneAirDistributionEffectivenessinCoolingMode() const {
    return getImpl<detail::SizingZone_Impl>()->designZoneAirDistributionEffectivenessinCoolingMode();
  }

  bool SizingZone::isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted();
  }

  double SizingZone::designZoneAirDistributionEffectivenessinHeatingMode() const {
    return getImpl<detail::SizingZone_Impl>()->designZoneAirDistributionEffectivenessinHeatingMode();
  }

  bool SizingZone::isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted();
  }

  double SizingZone::designZoneSecondaryRecirculationFraction() const {
    return getImpl<detail::SizingZone_Impl>()->designZoneSecondaryRecirculationFraction();
  }

  bool SizingZone::isDesignZoneSecondaryRecirculationFractionDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isDesignZoneSecondaryRecirculationFractionDefaulted();
  }

  double SizingZone::designMinimumZoneVentilationEfficiency() const {
    return getImpl<detail::SizingZone_Impl>()->designMinimumZoneVentilationEfficiency();
  }

  bool SizingZone::isDesignMinimumZoneVentilationEfficiencyDefaulted() const {
    return getImpl<detail::SizingZone_Impl>()->isDesignMinimumZoneVentilationEfficiencyDefaulted();
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

  bool SizingZone::setZoneCoolingDesignSupplyAirTemperature(double zoneCoolingDesignSupplyAirTemperature) {
    return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperature(zoneCoolingDesignSupplyAirTemperature);
  }

  bool SizingZone::setZoneCoolingDesignSupplyAirTemperatureDifference(double value) {
    return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperatureDifference(value);
  }

  bool SizingZone::setZoneHeatingDesignSupplyAirTemperature(double zoneHeatingDesignSupplyAirTemperature) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperature(zoneHeatingDesignSupplyAirTemperature);
  }

  bool SizingZone::setZoneHeatingDesignSupplyAirTemperatureDifference(double value) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperatureDifference(value);
  }

  bool SizingZone::setZoneCoolingDesignSupplyAirHumidityRatio(double zoneCoolingDesignSupplyAirHumidityRatio) {
    return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirHumidityRatio(zoneCoolingDesignSupplyAirHumidityRatio);
  }

  bool SizingZone::setZoneHeatingDesignSupplyAirHumidityRatio(double zoneHeatingDesignSupplyAirHumidityRatio) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirHumidityRatio(zoneHeatingDesignSupplyAirHumidityRatio);
  }

  bool SizingZone::setZoneHeatingSizingFactor(double zoneHeatingSizingFactor) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHeatingSizingFactor(zoneHeatingSizingFactor);
  }

  void SizingZone::resetZoneHeatingSizingFactor() {
    getImpl<detail::SizingZone_Impl>()->resetZoneHeatingSizingFactor();
  }

  bool SizingZone::setZoneCoolingSizingFactor(double zoneCoolingSizingFactor) {
    return getImpl<detail::SizingZone_Impl>()->setZoneCoolingSizingFactor(zoneCoolingSizingFactor);
  }

  void SizingZone::resetZoneCoolingSizingFactor() {
    getImpl<detail::SizingZone_Impl>()->resetZoneCoolingSizingFactor();
  }

  bool SizingZone::setCoolingDesignAirFlowMethod(const std::string& coolingDesignAirFlowMethod) {
    return getImpl<detail::SizingZone_Impl>()->setCoolingDesignAirFlowMethod(coolingDesignAirFlowMethod);
  }

  void SizingZone::resetCoolingDesignAirFlowMethod() {
    getImpl<detail::SizingZone_Impl>()->resetCoolingDesignAirFlowMethod();
  }

  bool SizingZone::setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate) {
    return getImpl<detail::SizingZone_Impl>()->setCoolingDesignAirFlowRate(coolingDesignAirFlowRate);
  }

  void SizingZone::resetCoolingDesignAirFlowRate() {
    getImpl<detail::SizingZone_Impl>()->resetCoolingDesignAirFlowRate();
  }

  bool SizingZone::setCoolingMinimumAirFlowperZoneFloorArea(double coolingMinimumAirFlowperZoneFloorArea) {
    return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlowperZoneFloorArea(coolingMinimumAirFlowperZoneFloorArea);
  }

  void SizingZone::resetCoolingMinimumAirFlowperZoneFloorArea() {
    getImpl<detail::SizingZone_Impl>()->resetCoolingMinimumAirFlowperZoneFloorArea();
  }

  bool SizingZone::setCoolingMinimumAirFlow(double coolingMinimumAirFlow) {
    return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlow(coolingMinimumAirFlow);
  }

  void SizingZone::resetCoolingMinimumAirFlow() {
    getImpl<detail::SizingZone_Impl>()->resetCoolingMinimumAirFlow();
  }

  bool SizingZone::setCoolingMinimumAirFlowFraction(double coolingMinimumAirFlowFraction) {
    return getImpl<detail::SizingZone_Impl>()->setCoolingMinimumAirFlowFraction(coolingMinimumAirFlowFraction);
  }

  void SizingZone::resetCoolingMinimumAirFlowFraction() {
    getImpl<detail::SizingZone_Impl>()->resetCoolingMinimumAirFlowFraction();
  }

  bool SizingZone::setHeatingDesignAirFlowMethod(const std::string& heatingDesignAirFlowMethod) {
    return getImpl<detail::SizingZone_Impl>()->setHeatingDesignAirFlowMethod(heatingDesignAirFlowMethod);
  }

  void SizingZone::resetHeatingDesignAirFlowMethod() {
    getImpl<detail::SizingZone_Impl>()->resetHeatingDesignAirFlowMethod();
  }

  bool SizingZone::setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate) {
    return getImpl<detail::SizingZone_Impl>()->setHeatingDesignAirFlowRate(heatingDesignAirFlowRate);
  }

  void SizingZone::resetHeatingDesignAirFlowRate() {
    getImpl<detail::SizingZone_Impl>()->resetHeatingDesignAirFlowRate();
  }

  bool SizingZone::setHeatingMaximumAirFlowperZoneFloorArea(double heatingMaximumAirFlowperZoneFloorArea) {
    return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlowperZoneFloorArea(heatingMaximumAirFlowperZoneFloorArea);
  }

  void SizingZone::resetHeatingMaximumAirFlowperZoneFloorArea() {
    getImpl<detail::SizingZone_Impl>()->resetHeatingMaximumAirFlowperZoneFloorArea();
  }

  bool SizingZone::setHeatingMaximumAirFlow(double heatingMaximumAirFlow) {
    return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlow(heatingMaximumAirFlow);
  }

  void SizingZone::resetHeatingMaximumAirFlow() {
    getImpl<detail::SizingZone_Impl>()->resetHeatingMaximumAirFlow();
  }

  bool SizingZone::setHeatingMaximumAirFlowFraction(double heatingMaximumAirFlowFraction) {
    return getImpl<detail::SizingZone_Impl>()->setHeatingMaximumAirFlowFraction(heatingMaximumAirFlowFraction);
  }

  void SizingZone::resetHeatingMaximumAirFlowFraction() {
    getImpl<detail::SizingZone_Impl>()->resetHeatingMaximumAirFlowFraction();
  }

  bool SizingZone::setDesignZoneAirDistributionEffectivenessinCoolingMode(double designZoneAirDistributionEffectivenessinCoolingMode) {
    return getImpl<detail::SizingZone_Impl>()->setDesignZoneAirDistributionEffectivenessinCoolingMode(
      designZoneAirDistributionEffectivenessinCoolingMode);
  }

  void SizingZone::resetDesignZoneAirDistributionEffectivenessinCoolingMode() {
    getImpl<detail::SizingZone_Impl>()->resetDesignZoneAirDistributionEffectivenessinCoolingMode();
  }

  bool SizingZone::setDesignZoneAirDistributionEffectivenessinHeatingMode(double designZoneAirDistributionEffectivenessinHeatingMode) {
    return getImpl<detail::SizingZone_Impl>()->setDesignZoneAirDistributionEffectivenessinHeatingMode(
      designZoneAirDistributionEffectivenessinHeatingMode);
  }

  void SizingZone::resetDesignZoneAirDistributionEffectivenessinHeatingMode() {
    getImpl<detail::SizingZone_Impl>()->resetDesignZoneAirDistributionEffectivenessinHeatingMode();
  }

  bool SizingZone::setDesignZoneSecondaryRecirculationFraction(double designZoneSecondaryRecirculationFraction) {
    return getImpl<detail::SizingZone_Impl>()->setDesignZoneSecondaryRecirculationFraction(designZoneSecondaryRecirculationFraction);
  }

  void SizingZone::resetDesignZoneSecondaryRecirculationFraction() {
    getImpl<detail::SizingZone_Impl>()->resetDesignZoneSecondaryRecirculationFraction();
  }

  bool SizingZone::setDesignMinimumZoneVentilationEfficiency(double designMinimumZoneVentilationEfficiency) {
    return getImpl<detail::SizingZone_Impl>()->setDesignMinimumZoneVentilationEfficiency(designMinimumZoneVentilationEfficiency);
  }

  void SizingZone::resetDesignMinimumZoneVentilationEfficiency() {
    getImpl<detail::SizingZone_Impl>()->resetDesignMinimumZoneVentilationEfficiency();
  }

  bool SizingZone::setZoneCoolingDesignSupplyAirTemperatureInputMethod(const std::string& value) {
    return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirTemperatureInputMethod(value);
  }

  bool SizingZone::setZoneHeatingDesignSupplyAirTemperatureInputMethod(const std::string& value) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHeatingDesignSupplyAirTemperatureInputMethod(value);
  }

  bool SizingZone::setAccountforDedicatedOutdoorAirSystem(bool accountforDedicatedOutdoorAirSystem) {
    return getImpl<detail::SizingZone_Impl>()->setAccountforDedicatedOutdoorAirSystem(accountforDedicatedOutdoorAirSystem);
  }

  bool SizingZone::setDedicatedOutdoorAirSystemControlStrategy(const std::string& dedicatedOutdoorAirSystemControlStrategy) {
    return getImpl<detail::SizingZone_Impl>()->setDedicatedOutdoorAirSystemControlStrategy(dedicatedOutdoorAirSystemControlStrategy);
  }

  bool SizingZone::setDedicatedOutdoorAirLowSetpointTemperatureforDesign(double dedicatedOutdoorAirLowSetpointTemperatureforDesign) {
    return getImpl<detail::SizingZone_Impl>()->setDedicatedOutdoorAirLowSetpointTemperatureforDesign(
      dedicatedOutdoorAirLowSetpointTemperatureforDesign);
  }

  void SizingZone::autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign() {
    getImpl<detail::SizingZone_Impl>()->autosizeDedicatedOutdoorAirLowSetpointTemperatureforDesign();
  }

  bool SizingZone::setDedicatedOutdoorAirHighSetpointTemperatureforDesign(double dedicatedOutdoorAirHighSetpointTemperatureforDesign) {
    return getImpl<detail::SizingZone_Impl>()->setDedicatedOutdoorAirHighSetpointTemperatureforDesign(
      dedicatedOutdoorAirHighSetpointTemperatureforDesign);
  }

  void SizingZone::autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign() {
    getImpl<detail::SizingZone_Impl>()->autosizeDedicatedOutdoorAirHighSetpointTemperatureforDesign();
  }

  std::string SizingZone::zoneLoadSizingMethod() const {
    return getImpl<detail::SizingZone_Impl>()->zoneLoadSizingMethod();
  }

  bool SizingZone::setZoneLoadSizingMethod(const std::string& zoneLoadSizingMethod) {
    return getImpl<detail::SizingZone_Impl>()->setZoneLoadSizingMethod(zoneLoadSizingMethod);
  }

  std::string SizingZone::zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod() const {
    return getImpl<detail::SizingZone_Impl>()->zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod();
  }

  bool SizingZone::setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod(
    const std::string& zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod) {
    return getImpl<detail::SizingZone_Impl>()->setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod(
      zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod);
  }

  boost::optional<double> SizingZone::zoneDehumidificationDesignSupplyAirHumidityRatio() const {
    return getImpl<detail::SizingZone_Impl>()->zoneDehumidificationDesignSupplyAirHumidityRatio();
  }

  bool SizingZone::setZoneDehumidificationDesignSupplyAirHumidityRatio(double zoneDehumidificationDesignSupplyAirHumidityRatio) {
    return getImpl<detail::SizingZone_Impl>()->setZoneDehumidificationDesignSupplyAirHumidityRatio(zoneDehumidificationDesignSupplyAirHumidityRatio);
  }

  void SizingZone::resetZoneDehumidificationDesignSupplyAirHumidityRatio() {
    getImpl<detail::SizingZone_Impl>()->resetZoneDehumidificationDesignSupplyAirHumidityRatio();
  }

  double SizingZone::zoneCoolingDesignSupplyAirHumidityRatioDifference() const {
    return getImpl<detail::SizingZone_Impl>()->zoneCoolingDesignSupplyAirHumidityRatioDifference();
  }

  bool SizingZone::setZoneCoolingDesignSupplyAirHumidityRatioDifference(double zoneCoolingDesignSupplyAirHumidityRatioDifference) {
    return getImpl<detail::SizingZone_Impl>()->setZoneCoolingDesignSupplyAirHumidityRatioDifference(
      zoneCoolingDesignSupplyAirHumidityRatioDifference);
  }

  std::string SizingZone::zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod() const {
    return getImpl<detail::SizingZone_Impl>()->zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod();
  }

  bool SizingZone::setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod(
    const std::string& zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod) {
    return getImpl<detail::SizingZone_Impl>()->setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod(
      zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod);
  }

  boost::optional<double> SizingZone::zoneHumidificationDesignSupplyAirHumidityRatio() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHumidificationDesignSupplyAirHumidityRatio();
  }

  bool SizingZone::setZoneHumidificationDesignSupplyAirHumidityRatio(double zoneHumidificationDesignSupplyAirHumidityRatio) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHumidificationDesignSupplyAirHumidityRatio(zoneHumidificationDesignSupplyAirHumidityRatio);
  }

  void SizingZone::resetZoneHumidificationDesignSupplyAirHumidityRatio() {
    getImpl<detail::SizingZone_Impl>()->resetZoneHumidificationDesignSupplyAirHumidityRatio();
  }

  double SizingZone::zoneHumidificationDesignSupplyAirHumidityRatioDifference() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHumidificationDesignSupplyAirHumidityRatioDifference();
  }

  bool SizingZone::setZoneHumidificationDesignSupplyAirHumidityRatioDifference(double zoneHumidificationDesignSupplyAirHumidityRatioDifference) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHumidificationDesignSupplyAirHumidityRatioDifference(
      zoneHumidificationDesignSupplyAirHumidityRatioDifference);
  }

  boost::optional<Schedule> SizingZone::zoneHumidistatDehumidificationSetPointSchedule() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHumidistatDehumidificationSetPointSchedule();
  }

  bool SizingZone::setZoneHumidistatDehumidificationSetPointSchedule(Schedule& schedule) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHumidistatDehumidificationSetPointSchedule(schedule);
  }

  void SizingZone::resetZoneHumidistatDehumidificationSetPointSchedule() {
    getImpl<detail::SizingZone_Impl>()->resetZoneHumidistatDehumidificationSetPointSchedule();
  }

  boost::optional<Schedule> SizingZone::zoneHumidistatHumidificationSetPointSchedule() const {
    return getImpl<detail::SizingZone_Impl>()->zoneHumidistatHumidificationSetPointSchedule();
  }

  bool SizingZone::setZoneHumidistatHumidificationSetPointSchedule(Schedule& schedule) {
    return getImpl<detail::SizingZone_Impl>()->setZoneHumidistatHumidificationSetPointSchedule(schedule);
  }

  void SizingZone::resetZoneHumidistatHumidificationSetPointSchedule() {
    getImpl<detail::SizingZone_Impl>()->resetZoneHumidistatHumidificationSetPointSchedule();
  }

  /// @cond
  SizingZone::SizingZone(std::shared_ptr<detail::SizingZone_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

  boost::optional<double> SizingZone::autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign() const {
    return getImpl<detail::SizingZone_Impl>()->autosizedDedicatedOutdoorAirLowSetpointTemperatureforDesign();
  }

  boost::optional<double> SizingZone::autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign() const {
    return getImpl<detail::SizingZone_Impl>()->autosizedDedicatedOutdoorAirHighSetpointTemperatureforDesign();
  }

  std::string SizingZone::sizingOption() const {
    return getImpl<detail::SizingZone_Impl>()->sizingOption();
  }

  bool SizingZone::setSizingOption(const std::string& sizingOption) {
    return getImpl<detail::SizingZone_Impl>()->setSizingOption(sizingOption);
  }

  void SizingZone::autosize() {
    return getImpl<detail::SizingZone_Impl>()->autosize();
  }

  void SizingZone::applySizingValues() {
    return getImpl<detail::SizingZone_Impl>()->applySizingValues();
  }

}  // namespace model
}  // namespace openstudio
