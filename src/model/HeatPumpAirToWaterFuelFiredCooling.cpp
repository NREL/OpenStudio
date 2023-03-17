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

#include "HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

#include "Model.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_AirToWater_FuelFired_Cooling_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpAirToWaterFuelFiredCooling_Impl::HeatPumpAirToWaterFuelFiredCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpAirToWaterFuelFiredCooling::iddObjectType());
    }

    HeatPumpAirToWaterFuelFiredCooling_Impl::HeatPumpAirToWaterFuelFiredCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpAirToWaterFuelFiredCooling::iddObjectType());
    }

    HeatPumpAirToWaterFuelFiredCooling_Impl::HeatPumpAirToWaterFuelFiredCooling_Impl(const HeatPumpAirToWaterFuelFiredCooling_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpAirToWaterFuelFiredCooling_Impl::outputVariableNames() const {
      static std::vector<std::string> result{"Fuel-fired Absorption HeatPump Load Side Heat Transfer Rate",
                                             "Fuel-fired Absorption HeatPump Load Side Heat Transfer Energy",
                                             "Fuel-fired Absorption HeatPump Inlet Temperature",
                                             "Fuel-fired Absorption HeatPump Outlet Temperature",
                                             "Fuel-fired Absorption HeatPump Fuel Rate",
                                             "Fuel-fired Absorption HeatPump Electricity Rate",
                                             "Fuel-fired Absorption HeatPump Fuel Energy",
                                             "Fuel-fired Absorption HeatPump Electricity Energy",
                                             "Fuel-fired Absorption HeatPump Mass Flow Rate",
                                             "Fuel-fired Absorption HeatPump Volumetric Flow Rate"};
      return result;
    }

    IddObjectType HeatPumpAirToWaterFuelFiredCooling_Impl::iddObjectType() const {
      return HeatPumpAirToWaterFuelFiredCooling::iddObjectType();
    }

    unsigned HeatPumpAirToWaterFuelFiredCooling_Impl::inletPort() const {
      return OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterInletNodeName;
    }

    unsigned HeatPumpAirToWaterFuelFiredCooling_Impl::outletPort() const {
      return OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterOutletNodeName;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    ModelObject HeatPumpAirToWaterFuelFiredCooling_Impl::clone(Model model) const {
      auto moClone = StraightComponent_Impl::clone(model);
      moClone.setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName, "");
      return moClone;
    }

    boost::optional<HeatPumpAirToWaterFuelFiredHeating> HeatPumpAirToWaterFuelFiredCooling_Impl::companionHeatingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpAirToWaterFuelFiredHeating>(
        OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName);
    }

    std::string HeatPumpAirToWaterFuelFiredCooling_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredCooling_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory);
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::nominalCoolingCapacity() const {
      return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, true);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::isNominalCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::autosizedNominalCoolingCapacity() {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::nominalCOP() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::designFlowRate() const {
      return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, true);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::isDesignFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::autosizedDesignFlowRate() {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::designSupplyTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignSupplyTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::designTemperatureLift() const {
      return getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, true);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::isDesignTemperatureLiftAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredCooling_Impl::autosizedDesignTemperatureLift() {
      // TODO: This entire field is actually never used in E+ as of 23.1.0-IOFreeze
      return getAutosizedValue("TODO_CHECK_SQL Design Temperature Lift", "deltaC");
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredCooling_Impl::flowMode() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredCooling_Impl::outdoorAirTemperatureCurveInputVariable() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::OutdoorAirTemperatureCurveInputVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredCooling_Impl::waterTemperatureCurveInputVariable() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterTemperatureCurveInputVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve HeatPumpAirToWaterFuelFiredCooling_Impl::normalizedCapacityFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalNormalizedCapacityFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Normalized Capacity Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve HeatPumpAirToWaterFuelFiredCooling_Impl::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalFuelEnergyInputRatioFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve HeatPumpAirToWaterFuelFiredCooling_Impl::fuelEnergyInputRatioFunctionofPLRCurve() const {
      boost::optional<Curve> value = optionalFuelEnergyInputRatioFunctionofPLRCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Function of PLR Curve attached.");
      }
      return value.get();
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::minimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::maximumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MaximumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling_Impl::cyclingRatioFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CyclingRatioFactorCurveName);
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::nominalAuxiliaryElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalAuxiliaryElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling_Impl::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling_Impl::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName);
    }

    double HeatPumpAirToWaterFuelFiredCooling_Impl::standbyElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::StandbyElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setCompanionHeatingHeatPump(
      const HeatPumpAirToWaterFuelFiredHeating& heatPumpAirToWaterFuelFiredHeating) {
      bool result =
        setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName, heatPumpAirToWaterFuelFiredHeating.handle());
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetCompanionHeatingHeatPump() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CompanionHeatingHeatPumpName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFuelType(const std::string& fuelType) {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelType, fuelType);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNominalCoolingCapacity(double nominalCoolingCapacity) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, nominalCoolingCapacity);
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetNominalCoolingCapacity() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, "");
      OS_ASSERT(result);
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::autosizeNominalCoolingCapacity() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNominalCOP(double nominalCOP) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalCOP, nominalCOP);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setDesignFlowRate(double designFlowRate) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, designFlowRate);
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetDesignFlowRate() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, "");
      OS_ASSERT(result);
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::autosizeDesignFlowRate() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setDesignSupplyTemperature(double designSupplyTemperature) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignSupplyTemperature, designSupplyTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setDesignTemperatureLift(double designTemperatureLift) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, designTemperatureLift);
      OS_ASSERT(result);
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::autosizeDesignTemperatureLift() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::DesignTemperatureLift, "autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setSizingFactor(double sizingFactor) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::SizingFactor, sizingFactor);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFlowMode(const std::string& flowMode) {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FlowMode, flowMode);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setOutdoorAirTemperatureCurveInputVariable(
      const std::string& outdoorAirTemperatureCurveInputVariable) {
      bool result =
        setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::OutdoorAirTemperatureCurveInputVariable, outdoorAirTemperatureCurveInputVariable);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNormalizedCapacityFunctionofTemperatureCurve(
      const Curve& normalizedCapacityFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NormalizedCapacityFunctionofTemperatureCurveName,
                               normalizedCapacityFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFuelEnergyInputRatioFunctionofTemperatureCurve(
      const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName,
                               fuelEnergyInputRatioFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setFuelEnergyInputRatioFunctionofPLRCurve(const Curve& fuelEnergyInputRatioFunctionofPLRCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofPLRCurveName,
                               fuelEnergyInputRatioFunctionofPLRCurve.handle());
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MinimumPartLoadRatio, minimumPartLoadRatio);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::MaximumPartLoadRatio, maximumPartLoadRatio);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setCyclingRatioFactorCurve(const Curve& cyclingRatioFactorCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CyclingRatioFactorCurveName, cyclingRatioFactorCurve.handle());
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetCyclingRatioFactorCurve() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::CyclingRatioFactorCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::NominalAuxiliaryElectricPower, nominalAuxiliaryElectricPower);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(
      const Curve& auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName,
                               auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve.handle());
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(
      const Curve& auxiliaryElectricEnergyInputRatioFunctionofPLRCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName,
                               auxiliaryElectricEnergyInputRatioFunctionofPLRCurve.handle());
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_CoolingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredCooling_Impl::setStandbyElectricPower(double standbyElectricPower) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_CoolingFields::StandbyElectricPower, standbyElectricPower);
      return result;
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::autosize() {
      autosizeNominalCoolingCapacity();
      autosizeDesignFlowRate();
      autosizeDesignTemperatureLift();
    }

    void HeatPumpAirToWaterFuelFiredCooling_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCoolingCapacity();
      if (val) {
        setNominalCoolingCapacity(val.get());
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

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalNormalizedCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_CoolingFields::NormalizedCapacityFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalFuelEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling_Impl::optionalFuelEnergyInputRatioFunctionofPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelEnergyInputRatioFunctionofPLRCurveName);
    }

  }  // namespace detail

  HeatPumpAirToWaterFuelFiredCooling::HeatPumpAirToWaterFuelFiredCooling(const Model& model)
    : StraightComponent(HeatPumpAirToWaterFuelFiredCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>());

    bool ok = setFuelType("NaturalGas");
    OS_ASSERT(ok);
    autosizeNominalCoolingCapacity();
    ok = setNominalCOP(1.0);
    OS_ASSERT(ok);
    autosizeDesignFlowRate();
    ok = setDesignSupplyTemperature(7.0);
    OS_ASSERT(ok);
    ok = setDesignTemperatureLift(11.1);
    OS_ASSERT(ok);
    ok = setSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setFlowMode("NotModulated");
    OS_ASSERT(ok);
    ok = setOutdoorAirTemperatureCurveInputVariable("DryBulb");
    OS_ASSERT(ok);
    ok = setWaterTemperatureCurveInputVariable("EnteringEvaporator");
    OS_ASSERT(ok);

    // From PlantLoopHeatPump_Fuel-Fired.idf

    {
      CurveBiquadratic normalizedCapacityFunctionofTemperatureCurve(model);
      normalizedCapacityFunctionofTemperatureCurve.setName("GAHP Cooling CapFT");
      normalizedCapacityFunctionofTemperatureCurve.setCoefficient1Constant(1);
      normalizedCapacityFunctionofTemperatureCurve.setCoefficient2x(0);
      normalizedCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
      normalizedCapacityFunctionofTemperatureCurve.setCoefficient4y(0);
      normalizedCapacityFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
      normalizedCapacityFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
      normalizedCapacityFunctionofTemperatureCurve.setMinimumValueofx(5);
      normalizedCapacityFunctionofTemperatureCurve.setMaximumValueofx(10);
      normalizedCapacityFunctionofTemperatureCurve.setMinimumValueofy(24);
      normalizedCapacityFunctionofTemperatureCurve.setMaximumValueofy(35);
      normalizedCapacityFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
      normalizedCapacityFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
      ok = setNormalizedCapacityFunctionofTemperatureCurve(normalizedCapacityFunctionofTemperatureCurve);
      OS_ASSERT(ok);
    }

    {
      CurveBiquadratic fuelEnergyInputRatioFunctionofTemperatureCurve(model);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setName("GAHP Cooling EIRFT");
      fuelEnergyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setCoefficient2x(0);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setCoefficient4y(0);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(5);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(10);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setMinimumValueofy(24);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setMaximumValueofy(35);
      fuelEnergyInputRatioFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
      fuelEnergyInputRatioFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
      ok = setFuelEnergyInputRatioFunctionofTemperatureCurve(fuelEnergyInputRatioFunctionofTemperatureCurve);
      OS_ASSERT(ok);
    }

    {
      CurveQuadratic fuelEnergyInputRatioFunctionofPLRCurve(model);
      fuelEnergyInputRatioFunctionofPLRCurve.setName("GAHP Cooling EIRFPLR");
      fuelEnergyInputRatioFunctionofPLRCurve.setCoefficient1Constant(1);
      fuelEnergyInputRatioFunctionofPLRCurve.setCoefficient2x(0);
      fuelEnergyInputRatioFunctionofPLRCurve.setCoefficient3xPOW2(0);
      fuelEnergyInputRatioFunctionofPLRCurve.setMinimumValueofx(0);
      fuelEnergyInputRatioFunctionofPLRCurve.setMaximumValueofx(1);
      ok = setFuelEnergyInputRatioFunctionofPLRCurve(fuelEnergyInputRatioFunctionofPLRCurve);
      OS_ASSERT(ok);
    }

    ok = setMinimumPartLoadRatio(0.1);
    OS_ASSERT(ok);
    ok = setMaximumPartLoadRatio(1.0);
    OS_ASSERT(ok);
    ok = setNominalAuxiliaryElectricPower(0);
    OS_ASSERT(ok);
    ok = setStandbyElectricPower(0);
    OS_ASSERT(ok);
  }

  HeatPumpAirToWaterFuelFiredCooling::HeatPumpAirToWaterFuelFiredCooling(const Model& model,
                                                                         const Curve& normalizedCapacityFunctionofTemperatureCurve,
                                                                         const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve,
                                                                         const Curve& fuelEnergyInputRatioFunctionofPLRCurve)
    : StraightComponent(HeatPumpAirToWaterFuelFiredCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>());

    bool ok = true;
    ok = setFuelType("NaturalGas");
    OS_ASSERT(ok);
    autosizeNominalCoolingCapacity();
    ok = setNominalCOP(1.0);
    OS_ASSERT(ok);
    autosizeDesignFlowRate();
    ok = setDesignSupplyTemperature(7.0);
    OS_ASSERT(ok);
    ok = setDesignTemperatureLift(11.1);
    OS_ASSERT(ok);
    ok = setSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setFlowMode("NotModulated");
    OS_ASSERT(ok);
    ok = setOutdoorAirTemperatureCurveInputVariable("DryBulb");
    OS_ASSERT(ok);
    ok = setWaterTemperatureCurveInputVariable("EnteringEvaporator");
    OS_ASSERT(ok);

    ok = setNormalizedCapacityFunctionofTemperatureCurve(normalizedCapacityFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s normalized capacity modifier function of temperature curve to "
                                     << normalizedCapacityFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setFuelEnergyInputRatioFunctionofTemperatureCurve(fuelEnergyInputRatioFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s fuel energy input ratio function of temperature curve to "
                                     << fuelEnergyInputRatioFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setFuelEnergyInputRatioFunctionofPLRCurve(fuelEnergyInputRatioFunctionofPLRCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s fuel energy input ratio function of PLR curve to "
                                     << fuelEnergyInputRatioFunctionofPLRCurve.briefDescription() << ".");
    }

    ok = setMinimumPartLoadRatio(0.1);
    OS_ASSERT(ok);
    ok = setMaximumPartLoadRatio(1.0);
    OS_ASSERT(ok);
    ok = setNominalAuxiliaryElectricPower(0);
    OS_ASSERT(ok);
    ok = setStandbyElectricPower(0);
    OS_ASSERT(ok);
  }

  IddObjectType HeatPumpAirToWaterFuelFiredCooling::iddObjectType() {
    return {IddObjectType::OS_HeatPump_AirToWater_FuelFired_Cooling};
  }

  std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_AirToWater_FuelFired_CoolingFields::FuelType);
  }

  std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::flowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_AirToWater_FuelFired_CoolingFields::FlowMode);
  }

  std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::outdoorAirTemperatureCurveInputVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_HeatPump_AirToWater_FuelFired_CoolingFields::OutdoorAirTemperatureCurveInputVariable);
  }

  std::vector<std::string> HeatPumpAirToWaterFuelFiredCooling::waterTemperatureCurveInputVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_HeatPump_AirToWater_FuelFired_CoolingFields::WaterTemperatureCurveInputVariable);
  }

  boost::optional<HeatPumpAirToWaterFuelFiredHeating> HeatPumpAirToWaterFuelFiredCooling::companionHeatingHeatPump() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->companionHeatingHeatPump();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling::fuelType() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->fuelType();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling::endUseSubcategory() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->endUseSubcategory();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isEndUseSubcategoryDefaulted();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::nominalCoolingCapacity() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->nominalCoolingCapacity();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::isNominalCoolingCapacityAutosized() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isNominalCoolingCapacityAutosized();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::autosizedNominalCoolingCapacity() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizedNominalCoolingCapacity();
  }

  double HeatPumpAirToWaterFuelFiredCooling::nominalCOP() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->nominalCOP();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::designFlowRate() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->designFlowRate();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::isDesignFlowRateAutosized() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isDesignFlowRateAutosized();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::autosizedDesignFlowRate() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizedDesignFlowRate();
  }

  double HeatPumpAirToWaterFuelFiredCooling::designSupplyTemperature() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->designSupplyTemperature();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::designTemperatureLift() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->designTemperatureLift();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::isDesignTemperatureLiftAutosized() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->isDesignTemperatureLiftAutosized();
  }

  boost::optional<double> HeatPumpAirToWaterFuelFiredCooling::autosizedDesignTemperatureLift() {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizedDesignTemperatureLift();
  }

  double HeatPumpAirToWaterFuelFiredCooling::sizingFactor() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->sizingFactor();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling::flowMode() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->flowMode();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling::outdoorAirTemperatureCurveInputVariable() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->outdoorAirTemperatureCurveInputVariable();
  }

  std::string HeatPumpAirToWaterFuelFiredCooling::waterTemperatureCurveInputVariable() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->waterTemperatureCurveInputVariable();
  }

  Curve HeatPumpAirToWaterFuelFiredCooling::normalizedCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->normalizedCapacityFunctionofTemperatureCurve();
  }

  Curve HeatPumpAirToWaterFuelFiredCooling::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->fuelEnergyInputRatioFunctionofTemperatureCurve();
  }

  Curve HeatPumpAirToWaterFuelFiredCooling::fuelEnergyInputRatioFunctionofPLRCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->fuelEnergyInputRatioFunctionofPLRCurve();
  }

  double HeatPumpAirToWaterFuelFiredCooling::minimumPartLoadRatio() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->minimumPartLoadRatio();
  }

  double HeatPumpAirToWaterFuelFiredCooling::maximumPartLoadRatio() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->maximumPartLoadRatio();
  }

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling::cyclingRatioFactorCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->cyclingRatioFactorCurve();
  }

  double HeatPumpAirToWaterFuelFiredCooling::nominalAuxiliaryElectricPower() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->nominalAuxiliaryElectricPower();
  }

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredCooling::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
  }

  double HeatPumpAirToWaterFuelFiredCooling::standbyElectricPower() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->standbyElectricPower();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setCompanionHeatingHeatPump(const HeatPumpAirToWaterFuelFiredHeating& heatPumpAirToWaterFuelFiredHeating) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setCompanionHeatingHeatPump(heatPumpAirToWaterFuelFiredHeating);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetCompanionHeatingHeatPump() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetCompanionHeatingHeatPump();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setFuelType(const std::string& fuelType) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFuelType(fuelType);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetEndUseSubcategory() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetEndUseSubcategory();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setNominalCoolingCapacity(double nominalCoolingCapacity) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNominalCoolingCapacity(nominalCoolingCapacity);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetNominalCoolingCapacity() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetNominalCoolingCapacity();
  }

  void HeatPumpAirToWaterFuelFiredCooling::autosizeNominalCoolingCapacity() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizeNominalCoolingCapacity();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setNominalCOP(double nominalCOP) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNominalCOP(nominalCOP);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setDesignFlowRate(double designFlowRate) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setDesignFlowRate(designFlowRate);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetDesignFlowRate() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetDesignFlowRate();
  }

  void HeatPumpAirToWaterFuelFiredCooling::autosizeDesignFlowRate() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizeDesignFlowRate();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setDesignSupplyTemperature(double designSupplyTemperature) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setDesignSupplyTemperature(designSupplyTemperature);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setDesignTemperatureLift(double designTemperatureLift) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setDesignTemperatureLift(designTemperatureLift);
  }

  void HeatPumpAirToWaterFuelFiredCooling::autosizeDesignTemperatureLift() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->autosizeDesignTemperatureLift();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setFlowMode(const std::string& flowMode) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFlowMode(flowMode);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setOutdoorAirTemperatureCurveInputVariable(
      outdoorAirTemperatureCurveInputVariable);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setWaterTemperatureCurveInputVariable(waterTemperatureCurveInputVariable);
  }

  bool
    HeatPumpAirToWaterFuelFiredCooling::setNormalizedCapacityFunctionofTemperatureCurve(const Curve& normalizedCapacityFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNormalizedCapacityFunctionofTemperatureCurve(
      normalizedCapacityFunctionofTemperatureCurve);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setFuelEnergyInputRatioFunctionofTemperatureCurve(
    const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFuelEnergyInputRatioFunctionofTemperatureCurve(
      fuelEnergyInputRatioFunctionofTemperatureCurve);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setFuelEnergyInputRatioFunctionofPLRCurve(const Curve& fuelEnergyInputRatioFunctionofPLRCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setFuelEnergyInputRatioFunctionofPLRCurve(
      fuelEnergyInputRatioFunctionofPLRCurve);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setCyclingRatioFactorCurve(const Curve& cyclingRatioFactorCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setCyclingRatioFactorCurve(cyclingRatioFactorCurve);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetCyclingRatioFactorCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetCyclingRatioFactorCurve();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setNominalAuxiliaryElectricPower(nominalAuxiliaryElectricPower);
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(
    const Curve& auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(
      auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(
    const Curve& auxiliaryElectricEnergyInputRatioFunctionofPLRCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(
      auxiliaryElectricEnergyInputRatioFunctionofPLRCurve);
  }

  void HeatPumpAirToWaterFuelFiredCooling::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
  }

  bool HeatPumpAirToWaterFuelFiredCooling::setStandbyElectricPower(double standbyElectricPower) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredCooling_Impl>()->setStandbyElectricPower(standbyElectricPower);
  }

  /// @cond
  HeatPumpAirToWaterFuelFiredCooling::HeatPumpAirToWaterFuelFiredCooling(std::shared_ptr<detail::HeatPumpAirToWaterFuelFiredCooling_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
