/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "SizingSystem.hpp"
#include "SizingSystem_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Sizing_System_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

SizingSystem_Impl::SizingSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ModelObject_Impl(idfObject,model,keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == SizingSystem::iddObjectType());
}

SizingSystem_Impl::SizingSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == SizingSystem::iddObjectType());
}

SizingSystem_Impl::SizingSystem_Impl(const SizingSystem_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{}

const std::vector<std::string>& SizingSystem_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType SizingSystem_Impl::iddObjectType() const {
  return SizingSystem::iddObjectType();
}

std::string SizingSystem_Impl::typeofLoadtoSizeOn() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::TypeofLoadtoSizeOn,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isTypeofLoadtoSizeOnDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::TypeofLoadtoSizeOn);
}

boost::optional<double> SizingSystem_Impl::designOutdoorAirFlowRate() const {
  return getDouble(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate,true);
}

bool SizingSystem_Impl::isDesignOutdoorAirFlowRateDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate);
}

bool SizingSystem_Impl::isDesignOutdoorAirFlowRateAutosized() const {
  bool result = false;
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autosize");
  }
  return result;
}

double SizingSystem_Impl::minimumSystemAirFlowRatio() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::MinimumSystemAirFlowRatio,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::preheatDesignTemperature() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::PreheatDesignTemperature,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::preheatDesignHumidityRatio() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::PreheatDesignHumidityRatio,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::precoolDesignTemperature() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::PrecoolDesignTemperature,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::precoolDesignHumidityRatio() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::PrecoolDesignHumidityRatio,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::centralCoolingDesignSupplyAirTemperature() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::centralHeatingDesignSupplyAirTemperature() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature,true);
  OS_ASSERT(value);
  return value.get();
}

std::string SizingSystem_Impl::sizingOption() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::SizingOption,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isSizingOptionDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::SizingOption);
}

bool SizingSystem_Impl::allOutdoorAirinCooling() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::AllOutdoorAirinCooling,true);
  OS_ASSERT(value);
  return openstudio::istringEqual(value.get(), "Yes");
}

bool SizingSystem_Impl::isAllOutdoorAirinCoolingDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::AllOutdoorAirinCooling);
}

bool SizingSystem_Impl::allOutdoorAirinHeating() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::AllOutdoorAirinHeating,true);
  OS_ASSERT(value);
  return openstudio::istringEqual(value.get(), "Yes");
}

bool SizingSystem_Impl::isAllOutdoorAirinHeatingDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::AllOutdoorAirinHeating);
}

double SizingSystem_Impl::centralCoolingDesignSupplyAirHumidityRatio() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isCentralCoolingDesignSupplyAirHumidityRatioDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio);
}

double SizingSystem_Impl::centralHeatingDesignSupplyAirHumidityRatio() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isCentralHeatingDesignSupplyAirHumidityRatioDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio);
}

std::string SizingSystem_Impl::coolingDesignAirFlowMethod() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::CoolingDesignAirFlowMethod,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isCoolingDesignAirFlowMethodDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::CoolingDesignAirFlowMethod);
}

double SizingSystem_Impl::coolingDesignAirFlowRate() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CoolingDesignAirFlowRate,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isCoolingDesignAirFlowRateDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::CoolingDesignAirFlowRate);
}

std::string SizingSystem_Impl::heatingDesignAirFlowMethod() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::HeatingDesignAirFlowMethod,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isHeatingDesignAirFlowMethodDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::HeatingDesignAirFlowMethod);
}

double SizingSystem_Impl::heatingDesignAirFlowRate() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::HeatingDesignAirFlowRate,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isHeatingDesignAirFlowRateDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::HeatingDesignAirFlowRate);
}

std::string SizingSystem_Impl::systemOutdoorAirMethod() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::SystemOutdoorAirMethod,true);
  OS_ASSERT(value);
  return value.get();
}

bool SizingSystem_Impl::isSystemOutdoorAirMethodDefaulted() const {
  return isEmpty(OS_Sizing_SystemFields::SystemOutdoorAirMethod);
}

bool SizingSystem_Impl::setTypeofLoadtoSizeOn(std::string typeofLoadtoSizeOn) {
  bool result = setString(OS_Sizing_SystemFields::TypeofLoadtoSizeOn, typeofLoadtoSizeOn);
  return result;
}

void SizingSystem_Impl::resetTypeofLoadtoSizeOn() {
  bool result = setString(OS_Sizing_SystemFields::TypeofLoadtoSizeOn, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setDesignOutdoorAirFlowRate(boost::optional<double> designOutdoorAirFlowRate) {
  bool result = false;
  if (designOutdoorAirFlowRate) {
    result = setDouble(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate, designOutdoorAirFlowRate.get());
  } else {
    result = setString(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate, "");
  }
  return result;
}

void SizingSystem_Impl::resetDesignOutdoorAirFlowRate() {
  bool result = setString(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate, "");
  OS_ASSERT(result);
}

void SizingSystem_Impl::autosizeDesignOutdoorAirFlowRate() {
  bool result = setString(OS_Sizing_SystemFields::DesignOutdoorAirFlowRate, "Autosize");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setMinimumSystemAirFlowRatio(double minimumSystemAirFlowRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::MinimumSystemAirFlowRatio, minimumSystemAirFlowRatio);
  return result;
}

void SizingSystem_Impl::setPreheatDesignTemperature(double preheatDesignTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::PreheatDesignTemperature, preheatDesignTemperature);
  OS_ASSERT(result);
}

void SizingSystem_Impl::setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::PreheatDesignHumidityRatio, preheatDesignHumidityRatio);
  OS_ASSERT(result);
}

void SizingSystem_Impl::setPrecoolDesignTemperature(double precoolDesignTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::PrecoolDesignTemperature, precoolDesignTemperature);
  OS_ASSERT(result);
}

void SizingSystem_Impl::setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::PrecoolDesignHumidityRatio, precoolDesignHumidityRatio);
  OS_ASSERT(result);
}

void SizingSystem_Impl::setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature, centralCoolingDesignSupplyAirTemperature);
  OS_ASSERT(result);
}

void SizingSystem_Impl::setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature, centralHeatingDesignSupplyAirTemperature);
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setSizingOption(std::string sizingOption) {
  bool result = setString(OS_Sizing_SystemFields::SizingOption, sizingOption);
  return result;
}

void SizingSystem_Impl::resetSizingOption() {
  bool result = setString(OS_Sizing_SystemFields::SizingOption, "");
  OS_ASSERT(result);
}

void SizingSystem_Impl::setAllOutdoorAirinCooling(bool allOutdoorAirinCooling) {
  bool result = false;
  if (allOutdoorAirinCooling) {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinCooling, "Yes");
  } else {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinCooling, "No");
  }
  OS_ASSERT(result);
}

void SizingSystem_Impl::resetAllOutdoorAirinCooling() {
  bool result = setString(OS_Sizing_SystemFields::AllOutdoorAirinCooling, "");
  OS_ASSERT(result);
}

void SizingSystem_Impl::setAllOutdoorAirinHeating(bool allOutdoorAirinHeating) {
  bool result = false;
  if (allOutdoorAirinHeating) {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinHeating, "Yes");
  } else {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinHeating, "No");
  }
  OS_ASSERT(result);
}

void SizingSystem_Impl::resetAllOutdoorAirinHeating() {
  bool result = setString(OS_Sizing_SystemFields::AllOutdoorAirinHeating, "");
  OS_ASSERT(result);
}

void SizingSystem_Impl::setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio, centralCoolingDesignSupplyAirHumidityRatio);
  OS_ASSERT(result);
}

void SizingSystem_Impl::resetCentralCoolingDesignSupplyAirHumidityRatio() {
  bool result = setString(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio, "");
  OS_ASSERT(result);
}

void SizingSystem_Impl::setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio, centralHeatingDesignSupplyAirHumidityRatio);
  OS_ASSERT(result);
}

void SizingSystem_Impl::resetCentralHeatingDesignSupplyAirHumidityRatio() {
  bool result = setString(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setCoolingDesignAirFlowMethod(std::string coolingDesignAirFlowMethod) {
  bool result = setString(OS_Sizing_SystemFields::CoolingDesignAirFlowMethod, coolingDesignAirFlowMethod);
  return result;
}

void SizingSystem_Impl::resetCoolingDesignAirFlowMethod() {
  bool result = setString(OS_Sizing_SystemFields::CoolingDesignAirFlowMethod, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate) {
  bool result = setDouble(OS_Sizing_SystemFields::CoolingDesignAirFlowRate, coolingDesignAirFlowRate);
  return result;
}

void SizingSystem_Impl::resetCoolingDesignAirFlowRate() {
  bool result = setString(OS_Sizing_SystemFields::CoolingDesignAirFlowRate, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod) {
  bool result = setString(OS_Sizing_SystemFields::HeatingDesignAirFlowMethod, heatingDesignAirFlowMethod);
  return result;
}

void SizingSystem_Impl::resetHeatingDesignAirFlowMethod() {
  bool result = setString(OS_Sizing_SystemFields::HeatingDesignAirFlowMethod, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate) {
  bool result = setDouble(OS_Sizing_SystemFields::HeatingDesignAirFlowRate, heatingDesignAirFlowRate);
  return result;
}

void SizingSystem_Impl::resetHeatingDesignAirFlowRate() {
  bool result = setString(OS_Sizing_SystemFields::HeatingDesignAirFlowRate, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod) {
  bool result = setString(OS_Sizing_SystemFields::SystemOutdoorAirMethod, systemOutdoorAirMethod);
  return result;
}

void SizingSystem_Impl::resetSystemOutdoorAirMethod() {
  bool result = setString(OS_Sizing_SystemFields::SystemOutdoorAirMethod, "");
  OS_ASSERT(result);
}

AirLoopHVAC SizingSystem_Impl::airLoopHVAC() const
{
  WorkspaceObject wo = getTarget(OS_Sizing_SystemFields::AirLoopName).get();

  return wo.cast<AirLoopHVAC>();
}

void SizingSystem_Impl::setAirLoopHVAC(const AirLoopHVAC & airLoopHVAC)
{
  if( model() != airLoopHVAC.model() )
  {
    return;
  }

  OS_ASSERT(this->setPointer(OS_Sizing_SystemFields::AirLoopName, airLoopHVAC.handle()));
}

} // detail

SizingSystem::SizingSystem(const Model& model, const AirLoopHVAC & airLoopHVAC)
  : ModelObject(SizingSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SizingSystem_Impl>());

  setAirLoopHVAC(airLoopHVAC);

  //these default sizing:system settings are suitable for a multizone VAV system.
  setTypeofLoadtoSizeOn("Sensible");
  autosizeDesignOutdoorAirFlowRate();
  setMinimumSystemAirFlowRatio(0.3);
  setPreheatDesignTemperature(7.0);
  setPreheatDesignHumidityRatio(0.008);
  setPrecoolDesignTemperature(12.8);
  setPrecoolDesignHumidityRatio(0.008);
  setCentralCoolingDesignSupplyAirTemperature(12.8);
  setCentralHeatingDesignSupplyAirTemperature(16.7);
  setSizingOption("NonCoincident");
  setAllOutdoorAirinCooling("No");
  setAllOutdoorAirinHeating("No");
  setCentralCoolingDesignSupplyAirHumidityRatio(0.0085);
  setCentralHeatingDesignSupplyAirHumidityRatio(0.0080);
  setCoolingDesignAirFlowMethod("DesignDay");
  setCoolingDesignAirFlowRate(0.0);
  setHeatingDesignAirFlowMethod("DesignDay");
  setHeatingDesignAirFlowRate(0.0);
  setSystemOutdoorAirMethod("ZoneSum");

}

IddObjectType SizingSystem::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Sizing_System);
  return result;
}

std::vector<std::string> SizingSystem::validTypeofLoadtoSizeOnValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_SystemFields::TypeofLoadtoSizeOn);
}

std::vector<std::string> SizingSystem::validSizingOptionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_SystemFields::SizingOption);
}

std::vector<std::string> SizingSystem::validCoolingDesignAirFlowMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_SystemFields::CoolingDesignAirFlowMethod);
}

std::vector<std::string> SizingSystem::validHeatingDesignAirFlowMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_SystemFields::HeatingDesignAirFlowMethod);
}

std::vector<std::string> SizingSystem::validSystemOutdoorAirMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Sizing_SystemFields::SystemOutdoorAirMethod);
}

std::string SizingSystem::typeofLoadtoSizeOn() const {
  return getImpl<detail::SizingSystem_Impl>()->typeofLoadtoSizeOn();
}

bool SizingSystem::isTypeofLoadtoSizeOnDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isTypeofLoadtoSizeOnDefaulted();
}

boost::optional<double> SizingSystem::designOutdoorAirFlowRate() const {
  return getImpl<detail::SizingSystem_Impl>()->designOutdoorAirFlowRate();
}

bool SizingSystem::isDesignOutdoorAirFlowRateDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isDesignOutdoorAirFlowRateDefaulted();
}

bool SizingSystem::isDesignOutdoorAirFlowRateAutosized() const {
  return getImpl<detail::SizingSystem_Impl>()->isDesignOutdoorAirFlowRateAutosized();
}

double SizingSystem::minimumSystemAirFlowRatio() const {
  return getImpl<detail::SizingSystem_Impl>()->minimumSystemAirFlowRatio();
}

double SizingSystem::preheatDesignTemperature() const {
  return getImpl<detail::SizingSystem_Impl>()->preheatDesignTemperature();
}

double SizingSystem::preheatDesignHumidityRatio() const {
  return getImpl<detail::SizingSystem_Impl>()->preheatDesignHumidityRatio();
}

double SizingSystem::precoolDesignTemperature() const {
  return getImpl<detail::SizingSystem_Impl>()->precoolDesignTemperature();
}

double SizingSystem::precoolDesignHumidityRatio() const {
  return getImpl<detail::SizingSystem_Impl>()->precoolDesignHumidityRatio();
}

double SizingSystem::centralCoolingDesignSupplyAirTemperature() const {
  return getImpl<detail::SizingSystem_Impl>()->centralCoolingDesignSupplyAirTemperature();
}

double SizingSystem::centralHeatingDesignSupplyAirTemperature() const {
  return getImpl<detail::SizingSystem_Impl>()->centralHeatingDesignSupplyAirTemperature();
}

std::string SizingSystem::sizingOption() const {
  return getImpl<detail::SizingSystem_Impl>()->sizingOption();
}

bool SizingSystem::isSizingOptionDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isSizingOptionDefaulted();
}

bool SizingSystem::allOutdoorAirinCooling() const {
  return getImpl<detail::SizingSystem_Impl>()->allOutdoorAirinCooling();
}

bool SizingSystem::isAllOutdoorAirinCoolingDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isAllOutdoorAirinCoolingDefaulted();
}

bool SizingSystem::allOutdoorAirinHeating() const {
  return getImpl<detail::SizingSystem_Impl>()->allOutdoorAirinHeating();
}

bool SizingSystem::isAllOutdoorAirinHeatingDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isAllOutdoorAirinHeatingDefaulted();
}

double SizingSystem::centralCoolingDesignSupplyAirHumidityRatio() const {
  return getImpl<detail::SizingSystem_Impl>()->centralCoolingDesignSupplyAirHumidityRatio();
}

bool SizingSystem::isCentralCoolingDesignSupplyAirHumidityRatioDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isCentralCoolingDesignSupplyAirHumidityRatioDefaulted();
}

double SizingSystem::centralHeatingDesignSupplyAirHumidityRatio() const {
  return getImpl<detail::SizingSystem_Impl>()->centralHeatingDesignSupplyAirHumidityRatio();
}

bool SizingSystem::isCentralHeatingDesignSupplyAirHumidityRatioDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isCentralHeatingDesignSupplyAirHumidityRatioDefaulted();
}

std::string SizingSystem::coolingDesignAirFlowMethod() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingDesignAirFlowMethod();
}

bool SizingSystem::isCoolingDesignAirFlowMethodDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isCoolingDesignAirFlowMethodDefaulted();
}

double SizingSystem::coolingDesignAirFlowRate() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingDesignAirFlowRate();
}

bool SizingSystem::isCoolingDesignAirFlowRateDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isCoolingDesignAirFlowRateDefaulted();
}

std::string SizingSystem::heatingDesignAirFlowMethod() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingDesignAirFlowMethod();
}

bool SizingSystem::isHeatingDesignAirFlowMethodDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isHeatingDesignAirFlowMethodDefaulted();
}

double SizingSystem::heatingDesignAirFlowRate() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingDesignAirFlowRate();
}

bool SizingSystem::isHeatingDesignAirFlowRateDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isHeatingDesignAirFlowRateDefaulted();
}

std::string SizingSystem::systemOutdoorAirMethod() const {
  return getImpl<detail::SizingSystem_Impl>()->systemOutdoorAirMethod();
}

bool SizingSystem::isSystemOutdoorAirMethodDefaulted() const {
  return getImpl<detail::SizingSystem_Impl>()->isSystemOutdoorAirMethodDefaulted();
}

bool SizingSystem::setTypeofLoadtoSizeOn(std::string typeofLoadtoSizeOn) {
  return getImpl<detail::SizingSystem_Impl>()->setTypeofLoadtoSizeOn(typeofLoadtoSizeOn);
}

void SizingSystem::resetTypeofLoadtoSizeOn() {
  getImpl<detail::SizingSystem_Impl>()->resetTypeofLoadtoSizeOn();
}

bool SizingSystem::setDesignOutdoorAirFlowRate(double designOutdoorAirFlowRate) {
  return getImpl<detail::SizingSystem_Impl>()->setDesignOutdoorAirFlowRate(designOutdoorAirFlowRate);
}

void SizingSystem::resetDesignOutdoorAirFlowRate() {
  getImpl<detail::SizingSystem_Impl>()->resetDesignOutdoorAirFlowRate();
}

void SizingSystem::autosizeDesignOutdoorAirFlowRate() {
  getImpl<detail::SizingSystem_Impl>()->autosizeDesignOutdoorAirFlowRate();
}

bool SizingSystem::setMinimumSystemAirFlowRatio(double minimumSystemAirFlowRatio) {
  return getImpl<detail::SizingSystem_Impl>()->setMinimumSystemAirFlowRatio(minimumSystemAirFlowRatio);
}

void SizingSystem::setPreheatDesignTemperature(double preheatDesignTemperature) {
  getImpl<detail::SizingSystem_Impl>()->setPreheatDesignTemperature(preheatDesignTemperature);
}

void SizingSystem::setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio) {
  getImpl<detail::SizingSystem_Impl>()->setPreheatDesignHumidityRatio(preheatDesignHumidityRatio);
}

void SizingSystem::setPrecoolDesignTemperature(double precoolDesignTemperature) {
  getImpl<detail::SizingSystem_Impl>()->setPrecoolDesignTemperature(precoolDesignTemperature);
}

void SizingSystem::setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio) {
  getImpl<detail::SizingSystem_Impl>()->setPrecoolDesignHumidityRatio(precoolDesignHumidityRatio);
}

void SizingSystem::setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature) {
  getImpl<detail::SizingSystem_Impl>()->setCentralCoolingDesignSupplyAirTemperature(centralCoolingDesignSupplyAirTemperature);
}

void SizingSystem::setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature) {
  getImpl<detail::SizingSystem_Impl>()->setCentralHeatingDesignSupplyAirTemperature(centralHeatingDesignSupplyAirTemperature);
}

bool SizingSystem::setSizingOption(std::string sizingOption) {
  return getImpl<detail::SizingSystem_Impl>()->setSizingOption(sizingOption);
}

void SizingSystem::resetSizingOption() {
  getImpl<detail::SizingSystem_Impl>()->resetSizingOption();
}

void SizingSystem::setAllOutdoorAirinCooling(bool allOutdoorAirinCooling) {
  getImpl<detail::SizingSystem_Impl>()->setAllOutdoorAirinCooling(allOutdoorAirinCooling);
}

void SizingSystem::resetAllOutdoorAirinCooling() {
  getImpl<detail::SizingSystem_Impl>()->resetAllOutdoorAirinCooling();
}

void SizingSystem::setAllOutdoorAirinHeating(bool allOutdoorAirinHeating) {
  getImpl<detail::SizingSystem_Impl>()->setAllOutdoorAirinHeating(allOutdoorAirinHeating);
}

void SizingSystem::resetAllOutdoorAirinHeating() {
  getImpl<detail::SizingSystem_Impl>()->resetAllOutdoorAirinHeating();
}

void SizingSystem::setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio) {
  getImpl<detail::SizingSystem_Impl>()->setCentralCoolingDesignSupplyAirHumidityRatio(centralCoolingDesignSupplyAirHumidityRatio);
}

void SizingSystem::resetCentralCoolingDesignSupplyAirHumidityRatio() {
  getImpl<detail::SizingSystem_Impl>()->resetCentralCoolingDesignSupplyAirHumidityRatio();
}

void SizingSystem::setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio) {
  getImpl<detail::SizingSystem_Impl>()->setCentralHeatingDesignSupplyAirHumidityRatio(centralHeatingDesignSupplyAirHumidityRatio);
}

void SizingSystem::resetCentralHeatingDesignSupplyAirHumidityRatio() {
  getImpl<detail::SizingSystem_Impl>()->resetCentralHeatingDesignSupplyAirHumidityRatio();
}

bool SizingSystem::setCoolingDesignAirFlowMethod(std::string coolingDesignAirFlowMethod) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingDesignAirFlowMethod(coolingDesignAirFlowMethod);
}

void SizingSystem::resetCoolingDesignAirFlowMethod() {
  getImpl<detail::SizingSystem_Impl>()->resetCoolingDesignAirFlowMethod();
}

bool SizingSystem::setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingDesignAirFlowRate(coolingDesignAirFlowRate);
}

void SizingSystem::resetCoolingDesignAirFlowRate() {
  getImpl<detail::SizingSystem_Impl>()->resetCoolingDesignAirFlowRate();
}

bool SizingSystem::setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingDesignAirFlowMethod(heatingDesignAirFlowMethod);
}

void SizingSystem::resetHeatingDesignAirFlowMethod() {
  getImpl<detail::SizingSystem_Impl>()->resetHeatingDesignAirFlowMethod();
}

bool SizingSystem::setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingDesignAirFlowRate(heatingDesignAirFlowRate);
}

void SizingSystem::resetHeatingDesignAirFlowRate() {
  getImpl<detail::SizingSystem_Impl>()->resetHeatingDesignAirFlowRate();
}

bool SizingSystem::setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod) {
  return getImpl<detail::SizingSystem_Impl>()->setSystemOutdoorAirMethod(systemOutdoorAirMethod);
}

void SizingSystem::resetSystemOutdoorAirMethod() {
  getImpl<detail::SizingSystem_Impl>()->resetSystemOutdoorAirMethod();
}

AirLoopHVAC SizingSystem::airLoopHVAC() const
{
  return getImpl<detail::SizingSystem_Impl>()->airLoopHVAC();
}

void SizingSystem::setAirLoopHVAC(const AirLoopHVAC & airLoopHVAC)
{
  getImpl<detail::SizingSystem_Impl>()->setAirLoopHVAC(airLoopHVAC);
}

/// @cond
SizingSystem::SizingSystem(std::shared_ptr<detail::SizingSystem_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model

} // openstudio

