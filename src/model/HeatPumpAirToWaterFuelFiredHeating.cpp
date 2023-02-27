/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "OutdoorAirNode.hpp"
#include "OutdoorAirNode_Impl.hpp"
#include "HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_AirToWater_FuelFired_Heating_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatPumpAirToWaterFuelFiredHeating_Impl::HeatPumpAirToWaterFuelFiredHeating_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatPumpAirToWaterFuelFiredHeating::iddObjectType());
  }

  HeatPumpAirToWaterFuelFiredHeating_Impl::HeatPumpAirToWaterFuelFiredHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatPumpAirToWaterFuelFiredHeating::iddObjectType());
  }

  HeatPumpAirToWaterFuelFiredHeating_Impl::HeatPumpAirToWaterFuelFiredHeating_Impl(const HeatPumpAirToWaterFuelFiredHeating_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatPumpAirToWaterFuelFiredHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatPumpAirToWaterFuelFiredHeating_Impl::iddObjectType() const {
    return HeatPumpAirToWaterFuelFiredHeating::iddObjectType();
  }

  Connection HeatPumpAirToWaterFuelFiredHeating_Impl::waterInletNode() const {
    boost::optional<Connection> value = optionalWaterInletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Water Inlet Node attached.");
    }
    return value.get();
  }

  Connection HeatPumpAirToWaterFuelFiredHeating_Impl::waterOutletNode() const {
    boost::optional<Connection> value = optionalWaterOutletNode();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Water Outlet Node attached.");
    }
    return value.get();
  }

  boost::optional<OutdoorAirNode> HeatPumpAirToWaterFuelFiredHeating_Impl::airSourceNode() const {
    return getObject<ModelObject>().getModelObjectTarget<OutdoorAirNode>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AirSourceNodeName);
  }

  boost::optional<HeatPumpAirToWaterFuelFiredCooling> HeatPumpAirToWaterFuelFiredHeating_Impl::companionCoolingHeatPump() const {
    return getObject<ModelObject>().getModelObjectTarget<HeatPumpAirToWaterFuelFiredCooling>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName);
  }

  std::string HeatPumpAirToWaterFuelFiredHeating_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredHeating_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory);
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::nominalHeatingCapacity() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity,true);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::isNominalHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> HeatPumpAirToWaterFuelFiredHeating_Impl::autosizedNominalHeatingCapacity() {
    return getAutosizedValue("TODO_CHECK_SQL Nominal Heating Capacity", "W");
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::nominalCOP() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::designFlowRate() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate,true);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::isDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> HeatPumpAirToWaterFuelFiredHeating_Impl::autosizedDesignFlowRate() {
    return getAutosizedValue("TODO_CHECK_SQL Design Flow Rate", "m3/s");
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::designSupplyTemperature() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignSupplyTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::designTemperatureLift() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift,true);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::isDesignTemperatureLiftAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> HeatPumpAirToWaterFuelFiredHeating_Impl::autosizedDesignTemperatureLift() {
    return getAutosizedValue("TODO_CHECK_SQL Design Temperature Lift", "deltaC");
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredHeating_Impl::flowMode() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredHeating_Impl::outdoorAirTemperatureCurveInputVariable() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredHeating_Impl::waterTemperatureCurveInputVariable() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  BivariateFunctions HeatPumpAirToWaterFuelFiredHeating_Impl::normalizedCapacityFunctionofTemperatureCurve() const {
    boost::optional<BivariateFunctions> value = optionalNormalizedCapacityFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Normalized Capacity Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  BivariateFunctions HeatPumpAirToWaterFuelFiredHeating_Impl::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
    boost::optional<BivariateFunctions> value = optionalFuelEnergyInputRatioFunctionofTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Functionof Temperature Curve attached.");
    }
    return value.get();
  }

  UnivariateFunctions HeatPumpAirToWaterFuelFiredHeating_Impl::fuelEnergyInputRatioFunctionofPLRCurve() const {
    boost::optional<UnivariateFunctions> value = optionalFuelEnergyInputRatioFunctionofPLRCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Functionof PLRCurve attached.");
    }
    return value.get();
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::minimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MinimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatPumpAirToWaterFuelFiredHeating_Impl::defrostControlType() const {
    boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::defrostOperationTimeFraction() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostOperationTimeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::fuelEnergyInputRatioDefrostAdjustmentCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName);
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::resistiveDefrostHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::ResistiveDefrostHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::cyclingRatioFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName);
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::nominalAuxiliaryElectricPower() const {
    return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower,true);
  }

  boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName);
  }

  double HeatPumpAirToWaterFuelFiredHeating_Impl::standbyElectricPower() const {
    boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::StandbyElectricPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setWaterInletNode(const Connection& connection) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName, connection.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setWaterOutletNode(const Connection& connection) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName, connection.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setAirSourceNode(const OutdoorAirNode& outdoorAirNode) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AirSourceNodeName, outdoorAirNode.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetAirSourceNode() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AirSourceNodeName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setCompanionCoolingHeatPump(const HeatPumpAirToWaterFuelFiredCooling& heatPumpAirToWaterFuelFiredCooling) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName, heatPumpAirToWaterFuelFiredCooling.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetCompanionCoolingHeatPump() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelType(const std::string& fuelType) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelType, fuelType);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setNominalHeatingCapacity(double nominalHeatingCapacity) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, nominalHeatingCapacity);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetNominalHeatingCapacity() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, "");
    OS_ASSERT(result);
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::autosizeNominalHeatingCapacity() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setNominalCOP(double nominalCOP) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalCOP, nominalCOP);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setDesignFlowRate(double designFlowRate) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, designFlowRate);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetDesignFlowRate() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, "");
    OS_ASSERT(result);
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::autosizeDesignFlowRate() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setDesignSupplyTemperature(double designSupplyTemperature) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignSupplyTemperature, designSupplyTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setDesignTemperatureLift(double designTemperatureLift) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, designTemperatureLift);
    OS_ASSERT(result);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::autosizeDesignTemperatureLift() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, "autosize");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::SizingFactor, sizingFactor);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFlowMode(const std::string& flowMode) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode, flowMode);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable, outdoorAirTemperatureCurveInputVariable);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setNormalizedCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName, bivariateFunctions.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName, bivariateFunctions.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName, univariateFunctions.handle());
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setDefrostControlType(const std::string& defrostControlType) {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostControlType, defrostControlType);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setDefrostOperationTimeFraction(double defrostOperationTimeFraction) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostOperationTimeFraction, defrostOperationTimeFraction);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelEnergyInputRatioDefrostAdjustmentCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName, univariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetFuelEnergyInputRatioDefrostAdjustmentCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, maximumOutdoorDrybulbTemperatureforDefrostOperation);
    return result;
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setCyclingRatioFactorCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName, univariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetCyclingRatioFactorCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower, nominalAuxiliaryElectricPower);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetNominalAuxiliaryElectricPower() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, bivariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, univariateFunctions.handle());
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
    bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, "");
    OS_ASSERT(result);
  }

  bool HeatPumpAirToWaterFuelFiredHeating_Impl::setStandbyElectricPower(double standbyElectricPower) {
    bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::StandbyElectricPower, standbyElectricPower);
    return result;
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::autosize() {
    autosizeNominalHeatingCapacity();
    autosizeDesignFlowRate();
    autosizeDesignTemperatureLift();
  }

  void HeatPumpAirToWaterFuelFiredHeating_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedNominalHeatingCapacity();
    if (val) {
      setNominalHeatingCapacity(val.get());
    }

    val = autosizedDesignFlowRate();
    if (val) {
      setDesignFlowRate(val.get());
    }

    val = autosizedDesignTemperatureLift();
    if (val) {
      setDesignTemperatureLift(val.get());
    }

  }

  boost::optional<Connection> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalWaterInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName);
  }

  boost::optional<Connection> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalWaterOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName);
  }

  boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalNormalizedCapacityFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalFuelEnergyInputRatioFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalFuelEnergyInputRatioFunctionofPLRCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName);
  }

} // detail

HeatPumpAirToWaterFuelFiredHeating::HeatPumpAirToWaterFuelFiredHeating(const Model& model)
  : StraightComponent(HeatPumpAirToWaterFuelFiredHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName
  //     OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName
  //     OS_HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName
  //     OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName
  //     OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName
  bool ok = true;
  // ok = setWaterInletNode();
  OS_ASSERT(ok);
  // ok = setWaterOutletNode();
  OS_ASSERT(ok);
  // ok = setFuelType();
  OS_ASSERT(ok);
  // ok = setNominalCOP();
  OS_ASSERT(ok);
  // setDesignSupplyTemperature();
  // setDesignTemperatureLift();
  // ok = setSizingFactor();
  OS_ASSERT(ok);
  // ok = setFlowMode();
  OS_ASSERT(ok);
  // ok = setOutdoorAirTemperatureCurveInputVariable();
  OS_ASSERT(ok);
  // ok = setWaterTemperatureCurveInputVariable();
  OS_ASSERT(ok);
  // ok = setNormalizedCapacityFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setFuelEnergyInputRatioFunctionofTemperatureCurve();
  OS_ASSERT(ok);
  // ok = setFuelEnergyInputRatioFunctionofPLRCurve();
  OS_ASSERT(ok);
  // ok = setMinimumPartLoadRatio();
  OS_ASSERT(ok);
  // ok = setMaximumPartLoadRatio();
  OS_ASSERT(ok);
  // ok = setDefrostControlType();
  OS_ASSERT(ok);
  // ok = setDefrostOperationTimeFraction();
  OS_ASSERT(ok);
  // ok = setResistiveDefrostHeaterCapacity();
  OS_ASSERT(ok);
  // ok = setMaximumOutdoorDrybulbTemperatureforDefrostOperation();
  OS_ASSERT(ok);
  // ok = setStandbyElectricPower();
  OS_ASSERT(ok);
}

IddObjectType HeatPumpAirToWaterFuelFiredHeating::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatPump_AirToWater_FuelFired_Heating);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelType);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::flowModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::outdoorAirTemperatureCurveInputVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::waterTemperatureCurveInputVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable);
}

std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::defrostControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostControlType);
}

Connection HeatPumpAirToWaterFuelFiredHeating::waterInletNode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->waterInletNode();
}

Connection HeatPumpAirToWaterFuelFiredHeating::waterOutletNode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->waterOutletNode();
}

boost::optional<OutdoorAirNode> HeatPumpAirToWaterFuelFiredHeating::airSourceNode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->airSourceNode();
}

boost::optional<HeatPumpAirToWaterFuelFiredCooling> HeatPumpAirToWaterFuelFiredHeating::companionCoolingHeatPump() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->companionCoolingHeatPump();
}

std::string HeatPumpAirToWaterFuelFiredHeating::fuelType() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->fuelType();
}

std::string HeatPumpAirToWaterFuelFiredHeating::endUseSubcategory() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->endUseSubcategory();
}

bool HeatPumpAirToWaterFuelFiredHeating::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->isEndUseSubcategoryDefaulted();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::nominalHeatingCapacity() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->nominalHeatingCapacity();
}

bool HeatPumpAirToWaterFuelFiredHeating::isNominalHeatingCapacityAutosized() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->isNominalHeatingCapacityAutosized();
}

  boost::optional <double> HeatPumpAirToWaterFuelFiredHeating::autosizedNominalHeatingCapacity() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->autosizedNominalHeatingCapacity();
  }

double HeatPumpAirToWaterFuelFiredHeating::nominalCOP() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->nominalCOP();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::designFlowRate() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->designFlowRate();
}

bool HeatPumpAirToWaterFuelFiredHeating::isDesignFlowRateAutosized() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->isDesignFlowRateAutosized();
}

  boost::optional <double> HeatPumpAirToWaterFuelFiredHeating::autosizedDesignFlowRate() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->autosizedDesignFlowRate();
  }

double HeatPumpAirToWaterFuelFiredHeating::designSupplyTemperature() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->designSupplyTemperature();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::designTemperatureLift() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->designTemperatureLift();
}

bool HeatPumpAirToWaterFuelFiredHeating::isDesignTemperatureLiftAutosized() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->isDesignTemperatureLiftAutosized();
}

  boost::optional <double> HeatPumpAirToWaterFuelFiredHeating::autosizedDesignTemperatureLift() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->autosizedDesignTemperatureLift();
  }

double HeatPumpAirToWaterFuelFiredHeating::sizingFactor() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->sizingFactor();
}

std::string HeatPumpAirToWaterFuelFiredHeating::flowMode() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->flowMode();
}

std::string HeatPumpAirToWaterFuelFiredHeating::outdoorAirTemperatureCurveInputVariable() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->outdoorAirTemperatureCurveInputVariable();
}

std::string HeatPumpAirToWaterFuelFiredHeating::waterTemperatureCurveInputVariable() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->waterTemperatureCurveInputVariable();
}

BivariateFunctions HeatPumpAirToWaterFuelFiredHeating::normalizedCapacityFunctionofTemperatureCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->normalizedCapacityFunctionofTemperatureCurve();
}

BivariateFunctions HeatPumpAirToWaterFuelFiredHeating::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->fuelEnergyInputRatioFunctionofTemperatureCurve();
}

UnivariateFunctions HeatPumpAirToWaterFuelFiredHeating::fuelEnergyInputRatioFunctionofPLRCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->fuelEnergyInputRatioFunctionofPLRCurve();
}

double HeatPumpAirToWaterFuelFiredHeating::minimumPartLoadRatio() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->minimumPartLoadRatio();
}

double HeatPumpAirToWaterFuelFiredHeating::maximumPartLoadRatio() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->maximumPartLoadRatio();
}

std::string HeatPumpAirToWaterFuelFiredHeating::defrostControlType() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->defrostControlType();
}

double HeatPumpAirToWaterFuelFiredHeating::defrostOperationTimeFraction() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->defrostOperationTimeFraction();
}

boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating::fuelEnergyInputRatioDefrostAdjustmentCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->fuelEnergyInputRatioDefrostAdjustmentCurve();
}

double HeatPumpAirToWaterFuelFiredHeating::resistiveDefrostHeaterCapacity() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resistiveDefrostHeaterCapacity();
}

double HeatPumpAirToWaterFuelFiredHeating::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->maximumOutdoorDrybulbTemperatureforDefrostOperation();
}

boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating::cyclingRatioFactorCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->cyclingRatioFactorCurve();
}

boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::nominalAuxiliaryElectricPower() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->nominalAuxiliaryElectricPower();
}

boost::optional<BivariateFunctions> HeatPumpAirToWaterFuelFiredHeating::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
}

boost::optional<UnivariateFunctions> HeatPumpAirToWaterFuelFiredHeating::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
}

double HeatPumpAirToWaterFuelFiredHeating::standbyElectricPower() const {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->standbyElectricPower();
}

bool HeatPumpAirToWaterFuelFiredHeating::setWaterInletNode(const Connection& connection) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setWaterInletNode(connection);
}

bool HeatPumpAirToWaterFuelFiredHeating::setWaterOutletNode(const Connection& connection) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setWaterOutletNode(connection);
}

bool HeatPumpAirToWaterFuelFiredHeating::setAirSourceNode(const OutdoorAirNode& outdoorAirNode) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setAirSourceNode(outdoorAirNode);
}

void HeatPumpAirToWaterFuelFiredHeating::resetAirSourceNode() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetAirSourceNode();
}

bool HeatPumpAirToWaterFuelFiredHeating::setCompanionCoolingHeatPump(const HeatPumpAirToWaterFuelFiredCooling& heatPumpAirToWaterFuelFiredCooling) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setCompanionCoolingHeatPump(heatPumpAirToWaterFuelFiredCooling);
}

void HeatPumpAirToWaterFuelFiredHeating::resetCompanionCoolingHeatPump() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetCompanionCoolingHeatPump();
}

bool HeatPumpAirToWaterFuelFiredHeating::setFuelType(const std::string& fuelType) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelType(fuelType);
}

bool HeatPumpAirToWaterFuelFiredHeating::setEndUseSubcategory(const std::string& endUseSubcategory) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void HeatPumpAirToWaterFuelFiredHeating::resetEndUseSubcategory() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetEndUseSubcategory();
}

bool HeatPumpAirToWaterFuelFiredHeating::setNominalHeatingCapacity(double nominalHeatingCapacity) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setNominalHeatingCapacity(nominalHeatingCapacity);
}

void HeatPumpAirToWaterFuelFiredHeating::resetNominalHeatingCapacity() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetNominalHeatingCapacity();
}

void HeatPumpAirToWaterFuelFiredHeating::autosizeNominalHeatingCapacity() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->autosizeNominalHeatingCapacity();
}

bool HeatPumpAirToWaterFuelFiredHeating::setNominalCOP(double nominalCOP) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setNominalCOP(nominalCOP);
}

bool HeatPumpAirToWaterFuelFiredHeating::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setDesignFlowRate(designFlowRate);
}

void HeatPumpAirToWaterFuelFiredHeating::resetDesignFlowRate() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetDesignFlowRate();
}

void HeatPumpAirToWaterFuelFiredHeating::autosizeDesignFlowRate() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->autosizeDesignFlowRate();
}

bool HeatPumpAirToWaterFuelFiredHeating::setDesignSupplyTemperature(double designSupplyTemperature) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setDesignSupplyTemperature(designSupplyTemperature);
}

bool HeatPumpAirToWaterFuelFiredHeating::setDesignTemperatureLift(double designTemperatureLift) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setDesignTemperatureLift(designTemperatureLift);
}

void HeatPumpAirToWaterFuelFiredHeating::autosizeDesignTemperatureLift() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->autosizeDesignTemperatureLift();
}

bool HeatPumpAirToWaterFuelFiredHeating::setSizingFactor(double sizingFactor) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setSizingFactor(sizingFactor);
}

bool HeatPumpAirToWaterFuelFiredHeating::setFlowMode(const std::string& flowMode) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFlowMode(flowMode);
}

bool HeatPumpAirToWaterFuelFiredHeating::setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setOutdoorAirTemperatureCurveInputVariable(outdoorAirTemperatureCurveInputVariable);
}

bool HeatPumpAirToWaterFuelFiredHeating::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setWaterTemperatureCurveInputVariable(waterTemperatureCurveInputVariable);
}

bool HeatPumpAirToWaterFuelFiredHeating::setNormalizedCapacityFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setNormalizedCapacityFunctionofTemperatureCurve(bivariateFunctions);
}

bool HeatPumpAirToWaterFuelFiredHeating::setFuelEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelEnergyInputRatioFunctionofTemperatureCurve(bivariateFunctions);
}

bool HeatPumpAirToWaterFuelFiredHeating::setFuelEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelEnergyInputRatioFunctionofPLRCurve(univariateFunctions);
}

bool HeatPumpAirToWaterFuelFiredHeating::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

bool HeatPumpAirToWaterFuelFiredHeating::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

bool HeatPumpAirToWaterFuelFiredHeating::setDefrostControlType(const std::string& defrostControlType) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setDefrostControlType(defrostControlType);
}

bool HeatPumpAirToWaterFuelFiredHeating::setDefrostOperationTimeFraction(double defrostOperationTimeFraction) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setDefrostOperationTimeFraction(defrostOperationTimeFraction);
}

bool HeatPumpAirToWaterFuelFiredHeating::setFuelEnergyInputRatioDefrostAdjustmentCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelEnergyInputRatioDefrostAdjustmentCurve(univariateFunctions);
}

void HeatPumpAirToWaterFuelFiredHeating::resetFuelEnergyInputRatioDefrostAdjustmentCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetFuelEnergyInputRatioDefrostAdjustmentCurve();
}

bool HeatPumpAirToWaterFuelFiredHeating::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
}

bool HeatPumpAirToWaterFuelFiredHeating::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
}

bool HeatPumpAirToWaterFuelFiredHeating::setCyclingRatioFactorCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setCyclingRatioFactorCurve(univariateFunctions);
}

void HeatPumpAirToWaterFuelFiredHeating::resetCyclingRatioFactorCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetCyclingRatioFactorCurve();
}

bool HeatPumpAirToWaterFuelFiredHeating::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setNominalAuxiliaryElectricPower(nominalAuxiliaryElectricPower);
}

void HeatPumpAirToWaterFuelFiredHeating::resetNominalAuxiliaryElectricPower() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetNominalAuxiliaryElectricPower();
}

bool HeatPumpAirToWaterFuelFiredHeating::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(const BivariateFunctions& bivariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(bivariateFunctions);
}

void HeatPumpAirToWaterFuelFiredHeating::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
}

bool HeatPumpAirToWaterFuelFiredHeating::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(univariateFunctions);
}

void HeatPumpAirToWaterFuelFiredHeating::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
  getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
}

bool HeatPumpAirToWaterFuelFiredHeating::setStandbyElectricPower(double standbyElectricPower) {
  return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setStandbyElectricPower(standbyElectricPower);
}

/// @cond
HeatPumpAirToWaterFuelFiredHeating::HeatPumpAirToWaterFuelFiredHeating(std::shared_ptr<detail::HeatPumpAirToWaterFuelFiredHeating_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

