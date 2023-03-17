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

#include "Model.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_AirToWater_FuelFired_Heating_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpAirToWaterFuelFiredHeating_Impl::HeatPumpAirToWaterFuelFiredHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpAirToWaterFuelFiredHeating::iddObjectType());
    }

    HeatPumpAirToWaterFuelFiredHeating_Impl::HeatPumpAirToWaterFuelFiredHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpAirToWaterFuelFiredHeating::iddObjectType());
    }

    HeatPumpAirToWaterFuelFiredHeating_Impl::HeatPumpAirToWaterFuelFiredHeating_Impl(const HeatPumpAirToWaterFuelFiredHeating_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpAirToWaterFuelFiredHeating_Impl::outputVariableNames() const {
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

    IddObjectType HeatPumpAirToWaterFuelFiredHeating_Impl::iddObjectType() const {
      return HeatPumpAirToWaterFuelFiredHeating::iddObjectType();
    }

    unsigned HeatPumpAirToWaterFuelFiredHeating_Impl::inletPort() const {
      return OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterInletNodeName;
    }

    unsigned HeatPumpAirToWaterFuelFiredHeating_Impl::outletPort() const {
      return OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterOutletNodeName;
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    ModelObject HeatPumpAirToWaterFuelFiredHeating_Impl::clone(Model model) const {
      auto moClone = StraightComponent_Impl::clone(model);
      moClone.setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName, "");
      return moClone;
    }

    boost::optional<HeatPumpAirToWaterFuelFiredCooling> HeatPumpAirToWaterFuelFiredHeating_Impl::companionCoolingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpAirToWaterFuelFiredCooling>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName);
    }

    std::string HeatPumpAirToWaterFuelFiredHeating_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredHeating_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_HeatPump_AirToWater_FuelFired_HeatingFields::EndUseSubcategory);
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::nominalHeatingCapacity() const {
      return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, true);
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::isNominalHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::autosizedNominalHeatingCapacity() {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::nominalCOP() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::designFlowRate() const {
      return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, true);
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::isDesignFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::autosizedDesignFlowRate() {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::designSupplyTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignSupplyTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::designTemperatureLift() const {
      return getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, true);
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::isDesignTemperatureLiftAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DesignTemperatureLift, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpAirToWaterFuelFiredHeating_Impl::autosizedDesignTemperatureLift() {
      // TODO: This entire field is actually never used in E+ as of 23.1.0-IOFreeze
      return getAutosizedValue("TODO_CHECK_SQL Design Temperature Lift", "deltaC");
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredHeating_Impl::flowMode() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredHeating_Impl::outdoorAirTemperatureCurveInputVariable() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredHeating_Impl::waterTemperatureCurveInputVariable() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve HeatPumpAirToWaterFuelFiredHeating_Impl::normalizedCapacityFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalNormalizedCapacityFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Normalized Capacity Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve HeatPumpAirToWaterFuelFiredHeating_Impl::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
      boost::optional<Curve> value = optionalFuelEnergyInputRatioFunctionofTemperatureCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Function of Temperature Curve attached.");
      }
      return value.get();
    }

    Curve HeatPumpAirToWaterFuelFiredHeating_Impl::fuelEnergyInputRatioFunctionofPLRCurve() const {
      boost::optional<Curve> value = optionalFuelEnergyInputRatioFunctionofPLRCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fuel Energy Input Ratio Function of PLR Curve attached.");
      }
      return value.get();
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::minimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::maximumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpAirToWaterFuelFiredHeating_Impl::defrostControlType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::defrostOperationTimeFraction() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::DefrostOperationTimeFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::fuelEnergyInputRatioDefrostAdjustmentCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName);
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::resistiveDefrostHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::ResistiveDefrostHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
      boost::optional<double> value =
        getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::cyclingRatioFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName);
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::nominalAuxiliaryElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NominalAuxiliaryElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName);
    }

    double HeatPumpAirToWaterFuelFiredHeating_Impl::standbyElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::StandbyElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setCompanionCoolingHeatPump(
      const HeatPumpAirToWaterFuelFiredCooling& heatPumpAirToWaterFuelFiredCooling) {
      bool result =
        setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CompanionCoolingHeatPumpName, heatPumpAirToWaterFuelFiredCooling.handle());
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

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setOutdoorAirTemperatureCurveInputVariable(
      const std::string& outdoorAirTemperatureCurveInputVariable) {
      bool result =
        setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::OutdoorAirTemperatureCurveInputVariable, outdoorAirTemperatureCurveInputVariable);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::WaterTemperatureCurveInputVariable, waterTemperatureCurveInputVariable);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setNormalizedCapacityFunctionofTemperatureCurve(
      const Curve& normalizedCapacityFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName,
                               normalizedCapacityFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelEnergyInputRatioFunctionofTemperatureCurve(
      const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName,
                               fuelEnergyInputRatioFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelEnergyInputRatioFunctionofPLRCurve(const Curve& fuelEnergyInputRatioFunctionofPLRCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName,
                               fuelEnergyInputRatioFunctionofPLRCurve.handle());
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

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setFuelEnergyInputRatioDefrostAdjustmentCurve(
      const Curve& fuelEnergyInputRatioDefrostAdjustmentCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioDefrostAdjustmentCurveName,
                               fuelEnergyInputRatioDefrostAdjustmentCurve.handle());
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

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
      double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
      bool result = setDouble(OS_HeatPump_AirToWater_FuelFired_HeatingFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                              maximumOutdoorDrybulbTemperatureforDefrostOperation);
      return result;
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setCyclingRatioFactorCurve(const Curve& cyclingRatioFactorCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::CyclingRatioFactorCurveName, cyclingRatioFactorCurve.handle());
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

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(
      const Curve& auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName,
                               auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve.handle());
      return result;
    }

    void HeatPumpAirToWaterFuelFiredHeating_Impl::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
      bool result = setString(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpAirToWaterFuelFiredHeating_Impl::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(
      const Curve& auxiliaryElectricEnergyInputRatioFunctionofPLRCurve) {
      bool result = setPointer(OS_HeatPump_AirToWater_FuelFired_HeatingFields::AuxiliaryElectricEnergyInputRatioFunctionofPLRCurveName,
                               auxiliaryElectricEnergyInputRatioFunctionofPLRCurve.handle());
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

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalNormalizedCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::NormalizedCapacityFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalFuelEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating_Impl::optionalFuelEnergyInputRatioFunctionofPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelEnergyInputRatioFunctionofPLRCurveName);
    }

  }  // namespace detail

  HeatPumpAirToWaterFuelFiredHeating::HeatPumpAirToWaterFuelFiredHeating(const Model& model)
    : StraightComponent(HeatPumpAirToWaterFuelFiredHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>());

    bool ok = true;
    ok = setFuelType("NaturalGas");
    OS_ASSERT(ok);
    autosizeNominalHeatingCapacity();
    ok = setNominalCOP(1.0);
    OS_ASSERT(ok);
    autosizeDesignFlowRate();
    ok = setDesignSupplyTemperature(60);
    OS_ASSERT(ok);
    ok = setDesignTemperatureLift(11.1);
    OS_ASSERT(ok);
    ok = setSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setFlowMode("NotModulated");
    OS_ASSERT(ok);
    ok = setOutdoorAirTemperatureCurveInputVariable("DryBulb");
    OS_ASSERT(ok);
    ok = setWaterTemperatureCurveInputVariable("EnteringCondenser");
    OS_ASSERT(ok);

    // From PlantLoopHeatPump_Fuel-Fired.idf

    {
      CurveBiquadratic normalizedCapacityFunctionofTemperatureCurve(model);
      normalizedCapacityFunctionofTemperatureCurve.setName("GAHP Heating CapFT");
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
      fuelEnergyInputRatioFunctionofTemperatureCurve.setName("GAHP Heating EIRFT");
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
      fuelEnergyInputRatioFunctionofPLRCurve.setName("GAHP Heating EIRFPLR");
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
    ok = setDefrostControlType("Timed");
    OS_ASSERT(ok);
    ok = setDefrostOperationTimeFraction(0);
    OS_ASSERT(ok);
    ok = setResistiveDefrostHeaterCapacity(0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDrybulbTemperatureforDefrostOperation(5);
    OS_ASSERT(ok);
    ok = setNominalAuxiliaryElectricPower(0);
    OS_ASSERT(ok);
    ok = setStandbyElectricPower(0);
    OS_ASSERT(ok);
  }

  HeatPumpAirToWaterFuelFiredHeating::HeatPumpAirToWaterFuelFiredHeating(const Model& model,
                                                                         const Curve& normalizedCapacityFunctionofTemperatureCurve,
                                                                         const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve,
                                                                         const Curve& fuelEnergyInputRatioFunctionofPLRCurve)
    : StraightComponent(HeatPumpAirToWaterFuelFiredHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>());

    bool ok = true;
    ok = setFuelType("NaturalGas");
    OS_ASSERT(ok);
    autosizeNominalHeatingCapacity();
    ok = setNominalCOP(1.0);
    OS_ASSERT(ok);
    autosizeDesignFlowRate();
    ok = setDesignSupplyTemperature(60);
    OS_ASSERT(ok);
    ok = setDesignTemperatureLift(11.1);
    OS_ASSERT(ok);
    ok = setSizingFactor(1.0);
    OS_ASSERT(ok);
    ok = setFlowMode("NotModulated");
    OS_ASSERT(ok);
    ok = setOutdoorAirTemperatureCurveInputVariable("DryBulb");
    OS_ASSERT(ok);
    ok = setWaterTemperatureCurveInputVariable("EnteringCondenser");
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
    ok = setDefrostControlType("Timed");
    OS_ASSERT(ok);
    ok = setDefrostOperationTimeFraction(0);
    OS_ASSERT(ok);
    ok = setResistiveDefrostHeaterCapacity(0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDrybulbTemperatureforDefrostOperation(5);
    OS_ASSERT(ok);
    ok = setNominalAuxiliaryElectricPower(0);
    OS_ASSERT(ok);
    ok = setStandbyElectricPower(0);
    OS_ASSERT(ok);
  }

  IddObjectType HeatPumpAirToWaterFuelFiredHeating::iddObjectType() {
    return {IddObjectType::OS_HeatPump_AirToWater_FuelFired_Heating};
  }

  std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_AirToWater_FuelFired_HeatingFields::FuelType);
  }

  std::vector<std::string> HeatPumpAirToWaterFuelFiredHeating::flowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_AirToWater_FuelFired_HeatingFields::FlowMode);
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

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::autosizedNominalHeatingCapacity() {
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

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::autosizedDesignFlowRate() {
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

  boost::optional<double> HeatPumpAirToWaterFuelFiredHeating::autosizedDesignTemperatureLift() {
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

  Curve HeatPumpAirToWaterFuelFiredHeating::normalizedCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->normalizedCapacityFunctionofTemperatureCurve();
  }

  Curve HeatPumpAirToWaterFuelFiredHeating::fuelEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->fuelEnergyInputRatioFunctionofTemperatureCurve();
  }

  Curve HeatPumpAirToWaterFuelFiredHeating::fuelEnergyInputRatioFunctionofPLRCurve() const {
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

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating::fuelEnergyInputRatioDefrostAdjustmentCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->fuelEnergyInputRatioDefrostAdjustmentCurve();
  }

  double HeatPumpAirToWaterFuelFiredHeating::resistiveDefrostHeaterCapacity() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resistiveDefrostHeaterCapacity();
  }

  double HeatPumpAirToWaterFuelFiredHeating::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->maximumOutdoorDrybulbTemperatureforDefrostOperation();
  }

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating::cyclingRatioFactorCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->cyclingRatioFactorCurve();
  }

  double HeatPumpAirToWaterFuelFiredHeating::nominalAuxiliaryElectricPower() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->nominalAuxiliaryElectricPower();
  }

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating::auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpAirToWaterFuelFiredHeating::auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->auxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
  }

  double HeatPumpAirToWaterFuelFiredHeating::standbyElectricPower() const {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->standbyElectricPower();
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
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setOutdoorAirTemperatureCurveInputVariable(
      outdoorAirTemperatureCurveInputVariable);
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setWaterTemperatureCurveInputVariable(waterTemperatureCurveInputVariable);
  }

  bool
    HeatPumpAirToWaterFuelFiredHeating::setNormalizedCapacityFunctionofTemperatureCurve(const Curve& normalizedCapacityFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setNormalizedCapacityFunctionofTemperatureCurve(
      normalizedCapacityFunctionofTemperatureCurve);
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setFuelEnergyInputRatioFunctionofTemperatureCurve(
    const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelEnergyInputRatioFunctionofTemperatureCurve(
      fuelEnergyInputRatioFunctionofTemperatureCurve);
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setFuelEnergyInputRatioFunctionofPLRCurve(const Curve& fuelEnergyInputRatioFunctionofPLRCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelEnergyInputRatioFunctionofPLRCurve(
      fuelEnergyInputRatioFunctionofPLRCurve);
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

  bool HeatPumpAirToWaterFuelFiredHeating::setFuelEnergyInputRatioDefrostAdjustmentCurve(const Curve& fuelEnergyInputRatioDefrostAdjustmentCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setFuelEnergyInputRatioDefrostAdjustmentCurve(
      fuelEnergyInputRatioDefrostAdjustmentCurve);
  }

  void HeatPumpAirToWaterFuelFiredHeating::resetFuelEnergyInputRatioDefrostAdjustmentCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetFuelEnergyInputRatioDefrostAdjustmentCurve();
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
    double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
      maximumOutdoorDrybulbTemperatureforDefrostOperation);
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setCyclingRatioFactorCurve(const Curve& cyclingRatioFactorCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setCyclingRatioFactorCurve(cyclingRatioFactorCurve);
  }

  void HeatPumpAirToWaterFuelFiredHeating::resetCyclingRatioFactorCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetCyclingRatioFactorCurve();
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setNominalAuxiliaryElectricPower(nominalAuxiliaryElectricPower);
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(
    const Curve& auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(
      auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve);
  }

  void HeatPumpAirToWaterFuelFiredHeating::resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(
    const Curve& auxiliaryElectricEnergyInputRatioFunctionofPLRCurve) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(
      auxiliaryElectricEnergyInputRatioFunctionofPLRCurve);
  }

  void HeatPumpAirToWaterFuelFiredHeating::resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve() {
    getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();
  }

  bool HeatPumpAirToWaterFuelFiredHeating::setStandbyElectricPower(double standbyElectricPower) {
    return getImpl<detail::HeatPumpAirToWaterFuelFiredHeating_Impl>()->setStandbyElectricPower(standbyElectricPower);
  }

  /// @cond
  HeatPumpAirToWaterFuelFiredHeating::HeatPumpAirToWaterFuelFiredHeating(std::shared_ptr<detail::HeatPumpAirToWaterFuelFiredHeating_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
