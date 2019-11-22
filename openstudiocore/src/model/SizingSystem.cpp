/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "SizingSystem.hpp"
#include "SizingSystem_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Sizing_System_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/sql/SqlFile.hpp"

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
  return result;
}

IddObjectType SizingSystem_Impl::iddObjectType() const {
  return SizingSystem::iddObjectType();
}

bool SizingSystem_Impl::setParent(ParentObject& newParent)
{
  bool result = false;
  if( boost::optional<AirLoopHVAC> airLoopHVAC = newParent.optionalCast<AirLoopHVAC>()){
    result = this->setAirLoopHVAC(airLoopHVAC.get());
  }
  return result;
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

boost::optional<double> SizingSystem_Impl::centralHeatingMaximumSystemAirFlowRatio() const {
  return getDouble(OS_Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio,true);
}

bool SizingSystem_Impl::setCentralHeatingMaximumSystemAirFlowRatio(double centralHeatingMaximumSystemAirFlowRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, centralHeatingMaximumSystemAirFlowRatio);
  return result;
}

bool SizingSystem_Impl::isCentralHeatingMaximumSystemAirFlowRatioAutosized() const {
  bool result = false;
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autosize");
  }
  return result;
}

void SizingSystem_Impl::autosizeCentralHeatingMaximumSystemAirFlowRatio() {
  bool result = setString(OS_Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio, "Autosize");
  OS_ASSERT(result);
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

double SizingSystem_Impl::zoneMaximumOutdoorAirFraction() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::ZoneMaximumOutdoorAirFraction,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::coolingSupplyAirFlowRatePerFloorArea() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CoolingSupplyAirFlowRatePerFloorArea,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::coolingFractionofAutosizedCoolingSupplyAirFlowRate() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::coolingSupplyAirFlowRatePerUnitCoolingCapacity() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CoolingSupplyAirFlowRatePerUnitCoolingCapacity,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::heatingSupplyAirFlowRatePerFloorArea() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::HeatingSupplyAirFlowRatePerFloorArea,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::heatingFractionofAutosizedHeatingSupplyAirFlowRate() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::heatingFractionofAutosizedCoolingSupplyAirFlowRate() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::HeatingFractionofAutosizedCoolingSupplyAirFlowRate,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::heatingSupplyAirFlowRatePerUnitHeatingCapacity() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::HeatingSupplyAirFlowRatePerUnitHeatingCapacity,true);
  OS_ASSERT(value);
  return value.get();
}

std::string SizingSystem_Impl::coolingDesignCapacityMethod() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::CoolingDesignCapacityMethod,true);
  OS_ASSERT(value);
  return value.get();
}

boost::optional<double> SizingSystem_Impl::coolingDesignCapacity() const {
  return getDouble(OS_Sizing_SystemFields::CoolingDesignCapacity,true);
}

bool SizingSystem_Impl::isCoolingDesignCapacityAutosized() const {
  bool result = false;
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::CoolingDesignCapacity, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "autosize");
  }
  return result;
}

double SizingSystem_Impl::coolingDesignCapacityPerFloorArea() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::CoolingDesignCapacityPerFloorArea,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::fractionofAutosizedCoolingDesignCapacity() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::FractionofAutosizedCoolingDesignCapacity,true);
  OS_ASSERT(value);
  return value.get();
}

std::string SizingSystem_Impl::heatingDesignCapacityMethod() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::HeatingDesignCapacityMethod,true);
  OS_ASSERT(value);
  return value.get();
}

boost::optional<double> SizingSystem_Impl::heatingDesignCapacity() const {
  return getDouble(OS_Sizing_SystemFields::HeatingDesignCapacity,true);
}

bool SizingSystem_Impl::isHeatingDesignCapacityAutosized() const {
  bool result = false;
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::HeatingDesignCapacity, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "autosize");
  }
  return result;
}

double SizingSystem_Impl::heatingDesignCapacityPerFloorArea() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::HeatingDesignCapacityPerFloorArea,true);
  OS_ASSERT(value);
  return value.get();
}

double SizingSystem_Impl::fractionofAutosizedHeatingDesignCapacity() const {
  boost::optional<double> value = getDouble(OS_Sizing_SystemFields::FractionofAutosizedHeatingDesignCapacity,true);
  OS_ASSERT(value);
  return value.get();
}

std::string SizingSystem_Impl::centralCoolingCapacityControlMethod() const {
  boost::optional<std::string> value = getString(OS_Sizing_SystemFields::CentralCoolingCapacityControlMethod,true);
  OS_ASSERT(value);
  return value.get();
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

bool SizingSystem_Impl::setPreheatDesignTemperature(double preheatDesignTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::PreheatDesignTemperature, preheatDesignTemperature);
  OS_ASSERT(result);
  return result;
}

bool SizingSystem_Impl::setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::PreheatDesignHumidityRatio, preheatDesignHumidityRatio);
  OS_ASSERT(result);
  return result;
}

bool SizingSystem_Impl::setPrecoolDesignTemperature(double precoolDesignTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::PrecoolDesignTemperature, precoolDesignTemperature);
  OS_ASSERT(result);
  return result;
}

bool SizingSystem_Impl::setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::PrecoolDesignHumidityRatio, precoolDesignHumidityRatio);
  OS_ASSERT(result);
  return result;
}

bool SizingSystem_Impl::setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature, centralCoolingDesignSupplyAirTemperature);
  OS_ASSERT(result);
  return result;
}

bool SizingSystem_Impl::setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature, centralHeatingDesignSupplyAirTemperature);
  OS_ASSERT(result);
  return result;
}

bool SizingSystem_Impl::setSizingOption(std::string sizingOption) {
  bool result = setString(OS_Sizing_SystemFields::SizingOption, sizingOption);
  return result;
}

void SizingSystem_Impl::resetSizingOption() {
  bool result = setString(OS_Sizing_SystemFields::SizingOption, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setAllOutdoorAirinCooling(bool allOutdoorAirinCooling) {
  bool result = false;
  if (allOutdoorAirinCooling) {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinCooling, "Yes");
  } else {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinCooling, "No");
  }
  OS_ASSERT(result);
  return result;
}

void SizingSystem_Impl::resetAllOutdoorAirinCooling() {
  bool result = setString(OS_Sizing_SystemFields::AllOutdoorAirinCooling, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setAllOutdoorAirinHeating(bool allOutdoorAirinHeating) {
  bool result = false;
  if (allOutdoorAirinHeating) {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinHeating, "Yes");
  } else {
    result = setString(OS_Sizing_SystemFields::AllOutdoorAirinHeating, "No");
  }
  OS_ASSERT(result);
  return result;
}

void SizingSystem_Impl::resetAllOutdoorAirinHeating() {
  bool result = setString(OS_Sizing_SystemFields::AllOutdoorAirinHeating, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio, centralCoolingDesignSupplyAirHumidityRatio);
  OS_ASSERT(result);
  return result;
}

void SizingSystem_Impl::resetCentralCoolingDesignSupplyAirHumidityRatio() {
  bool result = setString(OS_Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio, "");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio) {
  bool result = setDouble(OS_Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio, centralHeatingDesignSupplyAirHumidityRatio);
  OS_ASSERT(result);
  return result;
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

bool SizingSystem_Impl::setZoneMaximumOutdoorAirFraction(double zoneMaximumOutdoorAirFraction) {
  bool result = setDouble(OS_Sizing_SystemFields::ZoneMaximumOutdoorAirFraction, zoneMaximumOutdoorAirFraction);
  return result;
}

bool SizingSystem_Impl::setCoolingSupplyAirFlowRatePerFloorArea(double coolingSupplyAirFlowRatePerFloorArea) {
  bool result = setDouble(OS_Sizing_SystemFields::CoolingSupplyAirFlowRatePerFloorArea, coolingSupplyAirFlowRatePerFloorArea);
  return result;
}

bool SizingSystem_Impl::setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(double coolingFractionofAutosizedCoolingSupplyAirFlowRate) {
  bool result = setDouble(OS_Sizing_SystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate, coolingFractionofAutosizedCoolingSupplyAirFlowRate);
  return result;
}

bool SizingSystem_Impl::setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(double coolingSupplyAirFlowRatePerUnitCoolingCapacity) {
  bool result = setDouble(OS_Sizing_SystemFields::CoolingSupplyAirFlowRatePerUnitCoolingCapacity, coolingSupplyAirFlowRatePerUnitCoolingCapacity);
  return result;
}

bool SizingSystem_Impl::setHeatingSupplyAirFlowRatePerFloorArea(double heatingSupplyAirFlowRatePerFloorArea) {
  bool result = setDouble(OS_Sizing_SystemFields::HeatingSupplyAirFlowRatePerFloorArea, heatingSupplyAirFlowRatePerFloorArea);
  return result;
}

bool SizingSystem_Impl::setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(double heatingFractionofAutosizedHeatingSupplyAirFlowRate) {
  bool result = setDouble(OS_Sizing_SystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate, heatingFractionofAutosizedHeatingSupplyAirFlowRate);
  return result;
}

bool SizingSystem_Impl::setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(double heatingFractionofAutosizedCoolingSupplyAirFlowRate) {
  bool result = setDouble(OS_Sizing_SystemFields::HeatingFractionofAutosizedCoolingSupplyAirFlowRate, heatingFractionofAutosizedCoolingSupplyAirFlowRate);
  return result;
}

bool SizingSystem_Impl::setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(double heatingSupplyAirFlowRatePerUnitHeatingCapacity) {
  bool result = setDouble(OS_Sizing_SystemFields::HeatingSupplyAirFlowRatePerUnitHeatingCapacity, heatingSupplyAirFlowRatePerUnitHeatingCapacity);
  return result;
}

bool SizingSystem_Impl::setCoolingDesignCapacityMethod(std::string coolingDesignCapacityMethod) {
  bool result = setString(OS_Sizing_SystemFields::CoolingDesignCapacityMethod, coolingDesignCapacityMethod);
  return result;
}

bool SizingSystem_Impl::setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity) {
  bool result(false);
  if (coolingDesignCapacity) {
    result = setDouble(OS_Sizing_SystemFields::CoolingDesignCapacity, coolingDesignCapacity.get());
  }
  return result;
}

void SizingSystem_Impl::autosizeCoolingDesignCapacity() {
  bool result = setString(OS_Sizing_SystemFields::CoolingDesignCapacity, "autosize");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea) {
  bool result = setDouble(OS_Sizing_SystemFields::CoolingDesignCapacityPerFloorArea, coolingDesignCapacityPerFloorArea);
  return result;
}

bool SizingSystem_Impl::setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity) {
  bool result = setDouble(OS_Sizing_SystemFields::FractionofAutosizedCoolingDesignCapacity, fractionofAutosizedCoolingDesignCapacity);
  return result;
}

bool SizingSystem_Impl::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
  bool result = setString(OS_Sizing_SystemFields::HeatingDesignCapacityMethod, heatingDesignCapacityMethod);
  return result;
}

bool SizingSystem_Impl::setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity) {
  bool result(false);
  if (heatingDesignCapacity) {
    result = setDouble(OS_Sizing_SystemFields::HeatingDesignCapacity, heatingDesignCapacity.get());
  }
  return result;
}

void SizingSystem_Impl::autosizeHeatingDesignCapacity() {
  bool result = setString(OS_Sizing_SystemFields::HeatingDesignCapacity, "autosize");
  OS_ASSERT(result);
}

bool SizingSystem_Impl::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
  bool result = setDouble(OS_Sizing_SystemFields::HeatingDesignCapacityPerFloorArea, heatingDesignCapacityPerFloorArea);
  return result;
}

bool SizingSystem_Impl::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
  bool result = setDouble(OS_Sizing_SystemFields::FractionofAutosizedHeatingDesignCapacity, fractionofAutosizedHeatingDesignCapacity);
  return result;
}

bool SizingSystem_Impl::setCentralCoolingCapacityControlMethod(std::string centralCoolingCapacityControlMethod) {
  bool result = setString(OS_Sizing_SystemFields::CentralCoolingCapacityControlMethod, centralCoolingCapacityControlMethod);
  return result;
}

AirLoopHVAC SizingSystem_Impl::airLoopHVAC() const
{
  WorkspaceObject wo = getTarget(OS_Sizing_SystemFields::AirLoopName).get();

  return wo.cast<AirLoopHVAC>();
}

bool SizingSystem_Impl::setAirLoopHVAC(const AirLoopHVAC & airLoopHVAC)
{
  if( model() != airLoopHVAC.model() )
  {
    LOG(Error, "Cannot set an AirLoopHVAC that isn't part of the same model for " << briefDescription());
    return false;
  }
  bool result = this->setPointer(OS_Sizing_SystemFields::AirLoopName, airLoopHVAC.handle());
  OS_ASSERT(result);
  return result;
}

  boost::optional<double> SizingSystem_Impl::autosizedDesignOutdoorAirFlowRate() const {
    boost::optional<double> result;

    // Get the parent AirLoopHVAC
    boost::optional<AirLoopHVAC> parAirLoop = airLoopHVAC();

    // Get the OA system
    boost::optional<AirLoopHVACOutdoorAirSystem> oaSys = parAirLoop->airLoopHVACOutdoorAirSystem();
    if (!oaSys) {
      LOG(Debug, "This object's parent AirLoopHVAC has no AirLoopHVACOutdoorAirSystem, cannot retrieve the autosizedDesignOutdoorAirFlowRate.");
      return result;
    }

    // Get the OA Controller
    ControllerOutdoorAir oaController = oaSys->getControllerOutdoorAir();

    return oaController.getAutosizedValue("Maximum Outdoor Air Flow Rate", "m3/s");
  }

  boost::optional<double> SizingSystem_Impl::autosizedCoolingDesignCapacity() const {
    boost::optional < double > result;

    std::string capacityType = "Cooling";

    // Get the parent AirLoopHVAC
    AirLoopHVAC parAirLoop = airLoopHVAC();

    // Get the name of the air loop
    if (!parAirLoop.name()) {
      LOG(Warn, "This object's parent AirLoopHVAC does not have a name, cannot retrieve the autosized " + capacityType + " Design Capacity.");
      return result;
    }

    // Get the object name and transform to the way it is recorded
    // in the sql file
    std::string sqlName = parAirLoop.name().get();
    boost::to_upper(sqlName);

    // Check that the model has a sql file
    if (!model().sqlFile()) {
      LOG(Warn, "This model has no sql file, cannot retrieve the autosized " + capacityType + " Design Capacity.");
      return result;
    }

    // Query the Intialization Summary -> System Sizing Information table to get
    // the row names that contains information for this component.
    std::stringstream rowsQuery;
    rowsQuery << "SELECT RowName ";
    rowsQuery << "FROM tabulardatawithstrings ";
    rowsQuery << "WHERE ReportName='Initialization Summary' ";
    rowsQuery << "AND ReportForString='Entire Facility' ";
    rowsQuery << "AND TableName = 'System Sizing Information' ";
    rowsQuery << "AND Value='" + sqlName + "'";

    boost::optional<std::vector<std::string>> rowNames = model().sqlFile().get().execAndReturnVectorOfString(rowsQuery.str());

    // Warn if the query failed
    if (!rowNames) {
      LOG(Warn, "Could not find a component called '" + sqlName + "' in any rows of the Initialization Summary System Sizing table.");
      return result;
    }

    // Query each row of the Intialization Summary -> System Sizing table
    // that contains this component to get the desired value.
    for (std::string rowName : rowNames.get()) {
      std::stringstream rowCheckQuery;
      rowCheckQuery << "SELECT Value ";
      rowCheckQuery << "FROM tabulardatawithstrings ";
      rowCheckQuery << "WHERE ReportName='Initialization Summary' ";
      rowCheckQuery << "AND ReportForString='Entire Facility' ";
      rowCheckQuery << "AND TableName = 'System Sizing Information' ";
      rowCheckQuery << "AND RowName='" << rowName << "' ";
      rowCheckQuery << "AND Value='" << capacityType << "'";
      boost::optional<std::string> rowValueName = model().sqlFile().get().execAndReturnFirstString(rowCheckQuery.str());
      // Check if the query succeeded
      if (!rowValueName) {
        continue;
      }
      // This is the right row
      std::stringstream valQuery;
      valQuery << "SELECT Value ";
      valQuery << "FROM tabulardatawithstrings ";
      valQuery << "WHERE ReportName='Initialization Summary' ";
      valQuery << "AND ReportForString='Entire Facility' ";
      valQuery << "AND TableName = 'System Sizing Information' ";
      valQuery << "AND ColumnName='User Design Capacity' ";
      valQuery << "AND RowName='" << rowName << "' ";
      boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery.str());
      // Check if the query succeeded
      if (val) {
        result = val.get();
        break;
      }
    }

    if (!result) {
      LOG(Debug, "The autosized value query for " + capacityType + " Design Capacity of " + sqlName + " returned no value.");
    }

    return result;

  }

  boost::optional<double> SizingSystem_Impl::autosizedHeatingDesignCapacity() const {
    boost::optional < double > result;

    std::string capacityType = "Heating";

    // Get the parent AirLoopHVAC
    AirLoopHVAC parAirLoop = airLoopHVAC();

    // Get the name of the air loop
    if (!parAirLoop.name()) {
      LOG(Debug, "This object's parent AirLoopHVAC does not have a name, cannot retrieve the autosized " + capacityType + " Design Capacity.");
      return result;
    }

    // Get the object name and transform to the way it is recorded
    // in the sql file
    std::string sqlName = parAirLoop.name().get();
    boost::to_upper(sqlName);

    // Check that the model has a sql file
    if (!model().sqlFile()) {
      LOG(Warn, "This model has no sql file, cannot retrieve the autosized " + capacityType + " Design Capacity.");
      return result;
    }

    // Query the Intialization Summary -> System Sizing Information table to get
    // the row names that contains information for this component.
    std::stringstream rowsQuery;
    rowsQuery << "SELECT RowName ";
    rowsQuery << "FROM tabulardatawithstrings ";
    rowsQuery << "WHERE ReportName='Initialization Summary' ";
    rowsQuery << "AND ReportForString='Entire Facility' ";
    rowsQuery << "AND TableName = 'System Sizing Information' ";
    rowsQuery << "AND Value='" + sqlName + "'";

    boost::optional<std::vector<std::string>> rowNames = model().sqlFile().get().execAndReturnVectorOfString(rowsQuery.str());

    // Warn if the query failed
    if (!rowNames) {
      LOG(Warn, "Could not find a component called '" + sqlName + "' in any rows of the Initialization Summary System Sizing table.");
      return result;
    }

    // Query each row of the Intialization Summary -> System Sizing table
    // that contains this component to get the desired value.
    for (std::string rowName : rowNames.get()) {
      std::stringstream rowCheckQuery;
      rowCheckQuery << "SELECT Value ";
      rowCheckQuery << "FROM tabulardatawithstrings ";
      rowCheckQuery << "WHERE ReportName='Initialization Summary' ";
      rowCheckQuery << "AND ReportForString='Entire Facility' ";
      rowCheckQuery << "AND TableName = 'System Sizing Information' ";
      rowCheckQuery << "AND RowName='" << rowName << "' ";
      rowCheckQuery << "AND Value='" << capacityType << "'";
      boost::optional<std::string> rowValueName = model().sqlFile().get().execAndReturnFirstString(rowCheckQuery.str());
      // Check if the query succeeded
      if (!rowValueName) {
        continue;
      }
      // This is the right row
      std::stringstream valQuery;
      valQuery << "SELECT Value ";
      valQuery << "FROM tabulardatawithstrings ";
      valQuery << "WHERE ReportName='Initialization Summary' ";
      valQuery << "AND ReportForString='Entire Facility' ";
      valQuery << "AND TableName = 'System Sizing Information' ";
      valQuery << "AND ColumnName='User Design Capacity' ";
      valQuery << "AND RowName='" << rowName << "' ";
      boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery.str());
      // Check if the query succeeded
      if (val) {
        result = val.get();
        break;
      }
    }

    if (!result) {
      LOG(Debug, "The autosized value query for " + capacityType + " Design Capacity of " + sqlName + " returned no value.");
    }

    return result;
  }

  boost::optional<double> SizingSystem_Impl::autosizedCentralHeatingMaximumSystemAirFlowRatio() const {
    boost::optional < double > result;

    // Get the parent AirLoopHVAC
    AirLoopHVAC parAirLoop = airLoopHVAC();

    // Get the name of the air loop
    if (!parAirLoop.name()) {
      LOG(Debug, "This object's parent AirLoopHVAC does not have a name, cannot retrieve the autosized "
          << "'Central Heating Maximum System Air Flow Ratio'.");
      return result;
    }

    // Get the object name and transform to the way it is recorded
    // in the sql file
    std::string sqlName = parAirLoop.name().get();
    boost::to_upper(sqlName);

    // Check that the model has a sql file
    if (!model().sqlFile()) {
      LOG(Warn, "This model has no sql file, cannot retrieve the autosized 'Central Heating Maximum System Air Flow Ratio'.");
      return result;
    }

    // Note JM 2018-09-10: It's not in the TabularDataWithStrings, so I look in the ComponentSizes
    std::stringstream valQuery;
    valQuery << "SELECT Value ";
    valQuery << "FROM ComponentSizes ";
    valQuery << "WHERE CompType='AirLoopHVAC' ";
    valQuery << "AND Description='User Heating Air Flow Ratio' ";
    valQuery << "AND Units='' ";
    valQuery << "AND CompName='" << sqlName << "' ";
    boost::optional<double> val = model().sqlFile().get().execAndReturnFirstDouble(valQuery.str());
    // Check if the query succeeded
    if (val) {
      result = val.get();
    }

    return result;
  }


  void SizingSystem_Impl::autosize() {
    autosizeDesignOutdoorAirFlowRate();
    autosizeCoolingDesignCapacity();
    autosizeHeatingDesignCapacity();
    autosizeCentralHeatingMaximumSystemAirFlowRatio();
  }

  void SizingSystem_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignOutdoorAirFlowRate();
    if (val) {
      setDesignOutdoorAirFlowRate(val.get());
    }

    val = autosizedCoolingDesignCapacity();
    if (val) {
      setCoolingDesignCapacity(val.get());
    }

    val = autosizedHeatingDesignCapacity();
    if (val) {
      setHeatingDesignCapacity(val.get());
    }

    val = autosizedCentralHeatingMaximumSystemAirFlowRatio();
    if (val) {
      setCentralHeatingMaximumSystemAirFlowRatio(val.get());
    }
  }

  std::vector<EMSActuatorNames> SizingSystem_Impl::emsActuatorNames() const {
    std::vector<EMSActuatorNames> actuators{{"Sizing:System", "Main Supply Volume Flow Rate"},
                                            {"Sizing:System", "Main Supply Coincident Peak Cooling Mass Flow Rate"},
                                            {"Sizing:System", "Main Supply Coincident Peak Heating Mass Flow Rate"},
                                            {"Sizing:System", "Main Supply Noncoincident Peak Cooling Mass Flow Rate"},
                                            {"Sizing:System", "Main Supply Noncoincident Peak Heating Mass Flow Rate"},
                                            {"Sizing:System", "Main Heating Volume Flow Rate"},
                                            {"Sizing:System", "Main Cooling Volume Flow Rate"}};
    return actuators;
  }

  std::vector<std::string> SizingSystem_Impl::emsInternalVariableNames() const {
    std::vector<std::string> types{"Intermediate Air System Main Supply Volume Flow Rate",
                                   "Intermediate Air System Coincident Peak Cooling Mass Flow Rate",
                                   "Intermediate Air System Coincident Peak Heating Mass Flow Rate",
                                   "Intermediate Air System Noncoincident Peak Cooling Mass Flow Rate",
                                   "Intermediate Air System Noncoincident Peak Heating Mass Flow Rate",
                                   "Intermediate Air System Heating Volume Flow Rate",
                                   "Intermediate Air System Cooling Volume Flow Rate"
                                   "Air System Cooling Design Sensible Capacity",
                                   "Air System Cooling Design Total Capacity",
                                   "Air System Heating Design Sensible Capacity",
                                   "Air System Preheating Design Sensible Capacity",
                                   "Air System Outdoor Air Design Volume Flow Rate",
                                   "Air System Cooling Design Mixed Air Temperature",
                                   "Air System Cooling Design Mixed Air Humidity Ratio",
                                   "Air System Cooling Design Return Air Temperature",
                                   "Air System Cooling Design Return Air Humidity Ratio",
                                   "Air System Cooling Design Outdoor Air Temperature",
                                   "Air System Cooling Design Outdoor Air Humidity Ratio",
                                   "Air System Heating Design Mixed Air Temperature",
                                   "Air System Heating Design Mixed Air Humidity Ratio",
                                   "Air System Heating Design Return Air Temperature",
                                   "Air System Heating Design Return Air Humidity Ratio",
                                   "Air System Heating Design Outdoor Air Temperature",
                                   "Air System Heating Design Outdoor Air Humidity Ratio"};
    return types;
  }
} // detail

SizingSystem::SizingSystem(const Model& model, const AirLoopHVAC & airLoopHVAC)
  : ModelObject(SizingSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SizingSystem_Impl>());

  setAirLoopHVAC(airLoopHVAC);

  setTypeofLoadtoSizeOn("Sensible");
  autosizeDesignOutdoorAirFlowRate();

  setCentralHeatingMaximumSystemAirFlowRatio(0.3);
  // TODO: should we autosize (E+ default) instead?
  // autosizeCentralHeatingMaximumSystemAirFlowRatio();

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
  setZoneMaximumOutdoorAirFraction(1.0);
  setCoolingSupplyAirFlowRatePerFloorArea(9.9676501E-3);
  setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(1.0);
  setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(3.9475456E-5);
  setHeatingSupplyAirFlowRatePerFloorArea(9.9676501E-3);
  setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(1.0);
  setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(1.0);
  setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(3.1588213E-5);
  setCoolingDesignCapacityMethod("CoolingDesignCapacity");
  autosizeCoolingDesignCapacity();
  setCoolingDesignCapacityPerFloorArea(234.7);
  setFractionofAutosizedCoolingDesignCapacity(1.0);
  setHeatingDesignCapacityMethod("HeatingDesignCapacity");
  autosizeHeatingDesignCapacity();
  setHeatingDesignCapacityPerFloorArea(157.0);
  setFractionofAutosizedHeatingDesignCapacity(1.0);
  setCentralCoolingCapacityControlMethod("OnOff");
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

double SizingSystem::zoneMaximumOutdoorAirFraction() const {
  return getImpl<detail::SizingSystem_Impl>()->zoneMaximumOutdoorAirFraction();
}

double SizingSystem::coolingSupplyAirFlowRatePerFloorArea() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingSupplyAirFlowRatePerFloorArea();
}

double SizingSystem::coolingFractionofAutosizedCoolingSupplyAirFlowRate() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingFractionofAutosizedCoolingSupplyAirFlowRate();
}

double SizingSystem::coolingSupplyAirFlowRatePerUnitCoolingCapacity() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingSupplyAirFlowRatePerUnitCoolingCapacity();
}

double SizingSystem::heatingSupplyAirFlowRatePerFloorArea() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingSupplyAirFlowRatePerFloorArea();
}

double SizingSystem::heatingFractionofAutosizedHeatingSupplyAirFlowRate() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingFractionofAutosizedHeatingSupplyAirFlowRate();
}

double SizingSystem::heatingFractionofAutosizedCoolingSupplyAirFlowRate() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingFractionofAutosizedCoolingSupplyAirFlowRate();
}

double SizingSystem::heatingSupplyAirFlowRatePerUnitHeatingCapacity() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingSupplyAirFlowRatePerUnitHeatingCapacity();
}

std::string SizingSystem::coolingDesignCapacityMethod() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingDesignCapacityMethod();
}

boost::optional<double> SizingSystem::coolingDesignCapacity() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingDesignCapacity();
}

bool SizingSystem::isCoolingDesignCapacityAutosized() const {
  return getImpl<detail::SizingSystem_Impl>()->isCoolingDesignCapacityAutosized();
}

double SizingSystem::coolingDesignCapacityPerFloorArea() const {
  return getImpl<detail::SizingSystem_Impl>()->coolingDesignCapacityPerFloorArea();
}

double SizingSystem::fractionofAutosizedCoolingDesignCapacity() const {
  return getImpl<detail::SizingSystem_Impl>()->fractionofAutosizedCoolingDesignCapacity();
}

std::string SizingSystem::heatingDesignCapacityMethod() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingDesignCapacityMethod();
}

boost::optional<double> SizingSystem::heatingDesignCapacity() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingDesignCapacity();
}

bool SizingSystem::isHeatingDesignCapacityAutosized() const {
  return getImpl<detail::SizingSystem_Impl>()->isHeatingDesignCapacityAutosized();
}

double SizingSystem::heatingDesignCapacityPerFloorArea() const {
  return getImpl<detail::SizingSystem_Impl>()->heatingDesignCapacityPerFloorArea();
}

double SizingSystem::fractionofAutosizedHeatingDesignCapacity() const {
  return getImpl<detail::SizingSystem_Impl>()->fractionofAutosizedHeatingDesignCapacity();
}

std::string SizingSystem::centralCoolingCapacityControlMethod() const {
  return getImpl<detail::SizingSystem_Impl>()->centralCoolingCapacityControlMethod();
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

bool SizingSystem::setPreheatDesignTemperature(double preheatDesignTemperature) {
  return getImpl<detail::SizingSystem_Impl>()->setPreheatDesignTemperature(preheatDesignTemperature);
}

bool SizingSystem::setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio) {
  return getImpl<detail::SizingSystem_Impl>()->setPreheatDesignHumidityRatio(preheatDesignHumidityRatio);
}

bool SizingSystem::setPrecoolDesignTemperature(double precoolDesignTemperature) {
  return getImpl<detail::SizingSystem_Impl>()->setPrecoolDesignTemperature(precoolDesignTemperature);
}

bool SizingSystem::setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio) {
  return getImpl<detail::SizingSystem_Impl>()->setPrecoolDesignHumidityRatio(precoolDesignHumidityRatio);
}

bool SizingSystem::setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature) {
  return getImpl<detail::SizingSystem_Impl>()->setCentralCoolingDesignSupplyAirTemperature(centralCoolingDesignSupplyAirTemperature);
}

bool SizingSystem::setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature) {
  return getImpl<detail::SizingSystem_Impl>()->setCentralHeatingDesignSupplyAirTemperature(centralHeatingDesignSupplyAirTemperature);
}

bool SizingSystem::setSizingOption(std::string sizingOption) {
  return getImpl<detail::SizingSystem_Impl>()->setSizingOption(sizingOption);
}

void SizingSystem::resetSizingOption() {
  getImpl<detail::SizingSystem_Impl>()->resetSizingOption();
}

bool SizingSystem::setAllOutdoorAirinCooling(bool allOutdoorAirinCooling) {
  return getImpl<detail::SizingSystem_Impl>()->setAllOutdoorAirinCooling(allOutdoorAirinCooling);
}

void SizingSystem::resetAllOutdoorAirinCooling() {
  getImpl<detail::SizingSystem_Impl>()->resetAllOutdoorAirinCooling();
}

bool SizingSystem::setAllOutdoorAirinHeating(bool allOutdoorAirinHeating) {
  return getImpl<detail::SizingSystem_Impl>()->setAllOutdoorAirinHeating(allOutdoorAirinHeating);
}

void SizingSystem::resetAllOutdoorAirinHeating() {
  getImpl<detail::SizingSystem_Impl>()->resetAllOutdoorAirinHeating();
}

bool SizingSystem::setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio) {
  return getImpl<detail::SizingSystem_Impl>()->setCentralCoolingDesignSupplyAirHumidityRatio(centralCoolingDesignSupplyAirHumidityRatio);
}

void SizingSystem::resetCentralCoolingDesignSupplyAirHumidityRatio() {
  getImpl<detail::SizingSystem_Impl>()->resetCentralCoolingDesignSupplyAirHumidityRatio();
}

bool SizingSystem::setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio) {
  return getImpl<detail::SizingSystem_Impl>()->setCentralHeatingDesignSupplyAirHumidityRatio(centralHeatingDesignSupplyAirHumidityRatio);
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

bool SizingSystem::setZoneMaximumOutdoorAirFraction(double zoneMaximumOutdoorAirFraction) {
  return getImpl<detail::SizingSystem_Impl>()->setZoneMaximumOutdoorAirFraction(zoneMaximumOutdoorAirFraction);
}

bool SizingSystem::setCoolingSupplyAirFlowRatePerFloorArea(double coolingSupplyAirFlowRatePerFloorArea) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingSupplyAirFlowRatePerFloorArea(coolingSupplyAirFlowRatePerFloorArea);
}

bool SizingSystem::setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(double coolingFractionofAutosizedCoolingSupplyAirFlowRate) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(coolingFractionofAutosizedCoolingSupplyAirFlowRate);
}

bool SizingSystem::setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(double coolingSupplyAirFlowRatePerUnitCoolingCapacity) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(coolingSupplyAirFlowRatePerUnitCoolingCapacity);
}

bool SizingSystem::setHeatingSupplyAirFlowRatePerFloorArea(double heatingSupplyAirFlowRatePerFloorArea) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingSupplyAirFlowRatePerFloorArea(heatingSupplyAirFlowRatePerFloorArea);
}

bool SizingSystem::setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(double heatingFractionofAutosizedHeatingSupplyAirFlowRate) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(heatingFractionofAutosizedHeatingSupplyAirFlowRate);
}

bool SizingSystem::setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(double heatingFractionofAutosizedCoolingSupplyAirFlowRate) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(heatingFractionofAutosizedCoolingSupplyAirFlowRate);
}

bool SizingSystem::setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(double heatingSupplyAirFlowRatePerUnitHeatingCapacity) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(heatingSupplyAirFlowRatePerUnitHeatingCapacity);
}

bool SizingSystem::setCoolingDesignCapacityMethod(std::string coolingDesignCapacityMethod) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingDesignCapacityMethod(coolingDesignCapacityMethod);
}

bool SizingSystem::setCoolingDesignCapacity(double coolingDesignCapacity) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingDesignCapacity(coolingDesignCapacity);
}

void SizingSystem::autosizeCoolingDesignCapacity() {
  getImpl<detail::SizingSystem_Impl>()->autosizeCoolingDesignCapacity();
}

bool SizingSystem::setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea) {
  return getImpl<detail::SizingSystem_Impl>()->setCoolingDesignCapacityPerFloorArea(coolingDesignCapacityPerFloorArea);
}

bool SizingSystem::setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity) {
  return getImpl<detail::SizingSystem_Impl>()->setFractionofAutosizedCoolingDesignCapacity(fractionofAutosizedCoolingDesignCapacity);
}

bool SizingSystem::setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingDesignCapacityMethod(heatingDesignCapacityMethod);
}

bool SizingSystem::setHeatingDesignCapacity(double heatingDesignCapacity) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingDesignCapacity(heatingDesignCapacity);
}

void SizingSystem::autosizeHeatingDesignCapacity() {
  getImpl<detail::SizingSystem_Impl>()->autosizeHeatingDesignCapacity();
}

bool SizingSystem::setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea) {
  return getImpl<detail::SizingSystem_Impl>()->setHeatingDesignCapacityPerFloorArea(heatingDesignCapacityPerFloorArea);
}

bool SizingSystem::setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity) {
  return getImpl<detail::SizingSystem_Impl>()->setFractionofAutosizedHeatingDesignCapacity(fractionofAutosizedHeatingDesignCapacity);
}

bool SizingSystem::setCentralCoolingCapacityControlMethod(std::string centralCoolingCapacityControlMethod) {
  return getImpl<detail::SizingSystem_Impl>()->setCentralCoolingCapacityControlMethod(centralCoolingCapacityControlMethod);
}

AirLoopHVAC SizingSystem::airLoopHVAC() const
{
  return getImpl<detail::SizingSystem_Impl>()->airLoopHVAC();
}

bool SizingSystem::setAirLoopHVAC(const AirLoopHVAC & airLoopHVAC)
{
  return getImpl<detail::SizingSystem_Impl>()->setAirLoopHVAC(airLoopHVAC);
}

/// @cond
SizingSystem::SizingSystem(std::shared_ptr<detail::SizingSystem_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

  boost::optional<double> SizingSystem::autosizedDesignOutdoorAirFlowRate() const {
    return getImpl<detail::SizingSystem_Impl>()->autosizedDesignOutdoorAirFlowRate();
  }

  boost::optional<double> SizingSystem::autosizedCoolingDesignCapacity() const {
    return getImpl<detail::SizingSystem_Impl>()->autosizedCoolingDesignCapacity();
  }

  boost::optional<double> SizingSystem::autosizedHeatingDesignCapacity() const {
    return getImpl<detail::SizingSystem_Impl>()->autosizedHeatingDesignCapacity();
  }

  void SizingSystem::autosize() {
    return getImpl<detail::SizingSystem_Impl>()->autosize();
  }

  void SizingSystem::applySizingValues() {
    return getImpl<detail::SizingSystem_Impl>()->applySizingValues();
  }


  boost::optional<double> SizingSystem::centralHeatingMaximumSystemAirFlowRatio() const {
    return getImpl<detail::SizingSystem_Impl>()->centralHeatingMaximumSystemAirFlowRatio();
  }

  bool SizingSystem::setCentralHeatingMaximumSystemAirFlowRatio(double centralHeatingMaximumSystemAirFlowRatio) {
    return getImpl<detail::SizingSystem_Impl>()->setCentralHeatingMaximumSystemAirFlowRatio(centralHeatingMaximumSystemAirFlowRatio);  }

  bool SizingSystem::isCentralHeatingMaximumSystemAirFlowRatioAutosized() const {
    return getImpl<detail::SizingSystem_Impl>()->isCentralHeatingMaximumSystemAirFlowRatioAutosized();
  }

  void SizingSystem::autosizeCentralHeatingMaximumSystemAirFlowRatio() {
    getImpl<detail::SizingSystem_Impl>()->autosizeCentralHeatingMaximumSystemAirFlowRatio();
  }

  boost::optional<double> SizingSystem::autosizedCentralHeatingMaximumSystemAirFlowRatio() const {
    return getImpl<detail::SizingSystem_Impl>()->autosizedCentralHeatingMaximumSystemAirFlowRatio();
  }



  // DEPRECATED: TODO REMOVE as soon as standards > 0.29.0 is relased
  boost::optional<double> SizingSystem::minimumSystemAirFlowRatio() const {
    return getImpl<detail::SizingSystem_Impl>()->centralHeatingMaximumSystemAirFlowRatio();
  }
  // DEPRECATED: TODO REMOVE as soon as standards > 0.29.0 is relased
  bool SizingSystem::setMinimumSystemAirFlowRatio(double centralHeatingMaximumSystemAirFlowRatio) {
    return getImpl<detail::SizingSystem_Impl>()->setCentralHeatingMaximumSystemAirFlowRatio(centralHeatingMaximumSystemAirFlowRatio);
  }


} // model

} // openstudio
