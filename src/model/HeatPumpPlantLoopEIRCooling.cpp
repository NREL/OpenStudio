/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HeatPumpPlantLoopEIRCooling.hpp"
#include "HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "Model.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include "HeatPumpPlantLoopEIRHeating.hpp"
#include "HeatPumpPlantLoopEIRHeating_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_PlantLoop_EIR_Cooling_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    HeatPumpPlantLoopEIRCooling_Impl::HeatPumpPlantLoopEIRCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatPumpPlantLoopEIRCooling::iddObjectType());
    }

    HeatPumpPlantLoopEIRCooling_Impl::HeatPumpPlantLoopEIRCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatPumpPlantLoopEIRCooling::iddObjectType());
    }

    HeatPumpPlantLoopEIRCooling_Impl::HeatPumpPlantLoopEIRCooling_Impl(const HeatPumpPlantLoopEIRCooling_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatPumpPlantLoopEIRCooling_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heat Pump Electricity Energy",
                                                   "Heat Pump Load Side Heat Transfer Energy",
                                                   "Heat Pump Source Side Heat Transfer Energy",
                                                   "Heat Pump Electricity Rate",
                                                   "Heat Pump Load Side Heat Transfer Rate",
                                                   "Heat Pump Source Side Heat Transfer Rate",
                                                   "Heat Pump Load Side Outlet Temperature",
                                                   "Heat Pump Load Side Inlet Temperature",
                                                   "Heat Pump Source Side Outlet Temperature",
                                                   "Heat Pump Source Side Inlet Temperature",
                                                   "Heat Pump Load Side Mass Flow Rate",
                                                   "Heat Pump Source Side Mass Flow Rate"};
      return result;
    }

    IddObjectType HeatPumpPlantLoopEIRCooling_Impl::iddObjectType() const {
      return HeatPumpPlantLoopEIRCooling::iddObjectType();
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::supplyInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::supplyOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::demandInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::demandOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName;
    }

    /** Convenience Function to return the Load Side Water Loop (HeatPump on supply side) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling_Impl::loadSideWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Source Side (Condenser) Water Loop (HeatPump on demand side) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling_Impl::sourceSideWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    boost::optional<Node> HeatPumpPlantLoopEIRCooling_Impl::sourceSideWaterInletNode() const {
      boost::optional<Node> result;

      if (auto mo = demandInletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRCooling_Impl::sourceSideWaterOutletNode() const {
      boost::optional<Node> result;

      if (auto mo = demandOutletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRCooling_Impl::loadSideWaterInletNode() const {
      boost::optional<Node> result;

      if (auto mo = supplyInletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRCooling_Impl::loadSideWaterOutletNode() const {
      boost::optional<Node> result;

      if (auto mo = supplyOutletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::addToNode(Node& node) {

      // call the base class implementation to connect the component
      bool ok = WaterToWaterComponent_Impl::addToNode(node);

      // If there's a secondary plant loop, switch the condenser type to "WaterCooled"
      if (this->sourceSideWaterLoop()) {
        this->setCondenserType("WaterSource");
      }
      return ok;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::removeFromSecondaryPlantLoop() {
      // Disconnect the component
      bool ok = WaterToWaterComponent_Impl::removeFromSecondaryPlantLoop();

      // Switch the condenser type back to "AirSource"
      this->setCondenserType("AirSource");
      return ok;
    }

    std::string HeatPumpPlantLoopEIRCooling_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setCondenserType(const std::string& condenserType) {
      bool ok = false;
      if (openstudio::istringEqual("AirSource", condenserType) && (this->secondaryPlantLoop())) {
        LOG(Warn, "Cannot set condenserType to AirSource, HeatPumpPlantLoopEIRCooling '"
                    << this->name() << "' is connected to a Source Side Loop. Use removeFromSecondaryPlantLoop() instead.");
      } else if (istringEqual("WaterSource", condenserType) && !(this->secondaryPlantLoop())) {
        LOG(Warn, "Cannot set condenserType to 'WaterSource', HeatPumpPlantLoopEIRCooling '"
                    << this->name() << "' is not connected to a Source Side Loop. Use addToNode(PlantLoop&) instead.");
      } else {
        ok = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, condenserType);
      }

      return ok;
    }

    boost::optional<HeatPumpPlantLoopEIRHeating> HeatPumpPlantLoopEIRCooling_Impl::companionHeatingHeatPump() const {
      return getObject<ModelObject>().getModelObjectTarget<HeatPumpPlantLoopEIRHeating>(
        OS_HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName);
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::loadSideReferenceFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isLoadSideReferenceFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::sourceSideReferenceFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isSourceSideReferenceFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::referenceCapacity() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, true);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isReferenceCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double HeatPumpPlantLoopEIRCooling_Impl::referenceCoefficientofPerformance() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRCooling_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve HeatPumpPlantLoopEIRCooling_Impl::capacityModifierFunctionofTemperatureCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve HeatPumpPlantLoopEIRCooling_Impl::electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
      WorkspaceObject wo = getTarget(OS_HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve HeatPumpPlantLoopEIRCooling_Impl::electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const {
      WorkspaceObject wo =
        getTarget(OS_HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    std::string HeatPumpPlantLoopEIRCooling_Impl::controlType() const {
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatPumpPlantLoopEIRCooling_Impl::flowMode() const {
      boost::optional<double> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::FlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRCooling_Impl::minimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRCooling_Impl::minimumSourceInletTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSourceInletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatPumpPlantLoopEIRCooling_Impl::maximumSourceInletTemeperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSourceInletTemeperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::minimumSupplyWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSupplyWaterTemperatureCurve);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::maximumSupplyWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSupplyWaterTemperatureCurve);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setCompanionHeatingHeatPump(const HeatPumpPlantLoopEIRHeating& companionHP) {
      return this->setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName, companionHP.handle());
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, loadSideReferenceFlowRate);
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeLoadSideReferenceFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, sourceSideReferenceFlowRate);
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeSourceSideReferenceFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setReferenceCapacity(double referenceCapacity) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, referenceCapacity);
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeReferenceCapacity() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, referenceCoefficientofPerformance);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setSizingFactor(double sizingFactor) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, sizingFactor);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName,
                               capacityModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                               electricInputtoOutputRatioModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName,
                               electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setControlType(const std::string& controlType) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ControlType, controlType);
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setFlowMode(double flowMode) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::FlowMode, flowMode);
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMinimumSourceInletTemperature(double minimumSourceInletTemperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSourceInletTemperature, minimumSourceInletTemperature);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMaximumSourceInletTemperature(double maximumSourceInletTemeperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSourceInletTemeperature, maximumSourceInletTemeperature);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSupplyWaterTemperatureCurve, minimumSupplyWaterTemperatureCurve.handle());
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSupplyWaterTemperatureCurve, maximumSupplyWaterTemperatureCurve.handle());
      return result;
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedLoadSideReferenceFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedSourceSideReferenceFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosize() {
      autosizeLoadSideReferenceFlowRate();
      autosizeSourceSideReferenceFlowRate();
      autosizeReferenceCapacity();
    }

    void HeatPumpPlantLoopEIRCooling_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedLoadSideReferenceFlowRate();
      if (val) {
        setLoadSideReferenceFlowRate(val.get());
      }

      val = autosizedSourceSideReferenceFlowRate();
      if (val) {
        setSourceSideReferenceFlowRate(val.get());
      }

      val = autosizedReferenceCapacity();
      if (val) {
        setReferenceCapacity(val.get());
      }
    }

    ComponentType HeatPumpPlantLoopEIRCooling_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> HeatPumpPlantLoopEIRCooling_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> HeatPumpPlantLoopEIRCooling_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> HeatPumpPlantLoopEIRCooling_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(const Model& model, const Curve& capacityModifierFunctionofTemperatureCurve,
                                                           const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve,
                                                           const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>());

    autosizeLoadSideReferenceFlowRate();
    autosizeSourceSideReferenceFlowRate();
    autosizeReferenceCapacity();

    bool ok = setCapacityModifierFunctionofTemperatureCurve(capacityModifierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s capacity modifier function of temperature curve to "
                                     << capacityModifierFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s electric input to output ratio modifier function of temperature curve to "
                                     << electricInputtoOutputRatioModifierFunctionofTemperatureCurve.briefDescription() << ".");
    }

    ok = setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s electric input to output ratio modifier function of part load ratio curve to "
                                     << electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.briefDescription() << ".");
    }

    // Bypass the check by calling setString directly
    setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, "AirSource");

    setReferenceCoefficientofPerformance(7.5);  // IDD default
    setSizingFactor(1.0);
    setControlType("Load");
    setFlowMode("ConstantFlow");
    setMinimumPartLoadRatio(0.0);
    setMinimumSourceInletTemperature(-100.0);
    setMaximumSourceInletTemperature(100.0);
  }

  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(const Model& model)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>());

    autosizeLoadSideReferenceFlowRate();
    autosizeSourceSideReferenceFlowRate();
    autosizeReferenceCapacity();

    // Note: The default HAS to be AirSource (since it's not connected to a plantLoop...)
    // But in both PlantLoopHeatPump_EIR_WaterSource.idf and PlantLoopHeatPump_EIR_AirSource.idf, the curves are flatlines
    // The only difference is the Source Side Flow Rate (m3/s), but here it's autosized anyways

    CurveBiquadratic capacityModifierFunctionofTemperatureCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    capacityModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    capacityModifierFunctionofTemperatureCurve.setCoefficient2x(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient4y(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
    capacityModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
    capacityModifierFunctionofTemperatureCurve.setMinimumValueofx(5);
    capacityModifierFunctionofTemperatureCurve.setMaximumValueofx(10);
    capacityModifierFunctionofTemperatureCurve.setMinimumValueofy(24);
    capacityModifierFunctionofTemperatureCurve.setMaximumValueofy(35);
    capacityModifierFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
    capacityModifierFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
    bool ok = setCapacityModifierFunctionofTemperatureCurve(capacityModifierFunctionofTemperatureCurve);
    OS_ASSERT(ok);

    CurveBiquadratic electricInputtoOutputRatioModifierFunctionofTemperatureCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient1Constant(1);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient2x(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient3xPOW2(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient4y(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient5yPOW2(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setCoefficient6xTIMESY(0);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMinimumValueofx(5);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMaximumValueofx(10);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMinimumValueofy(24);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setMaximumValueofy(35);
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setInputUnitTypeforX("Temperature");
    electricInputtoOutputRatioModifierFunctionofTemperatureCurve.setInputUnitTypeforY("Temperature");
    ok = setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
    OS_ASSERT(ok);

    CurveQuadratic electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(model);  // PlantLoopHeatPump_EIR_WaterSource.idf
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient1Constant(1);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient2x(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setCoefficient3xPOW2(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setMinimumValueofx(0);
    electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve.setMaximumValueofx(1);
    ok = setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
    OS_ASSERT(ok);

    // Bypass the check by calling setString directly
    setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, "AirSource");

    setReferenceCoefficientofPerformance(7.5);
    setSizingFactor(1.0);
    setControlType("Load");
    setFlowMode("ConstantFlow");
    setMinimumPartLoadRatio(0.0);
    setMinimumSourceInletTemperature(-100.0);
    setMaximumSourceInletTemperature(100.0);
  }

  IddObjectType HeatPumpPlantLoopEIRCooling::iddObjectType() {
    return {IddObjectType::OS_HeatPump_PlantLoop_EIR_Cooling};
  }

  std::vector<std::string> HeatPumpPlantLoopEIRCooling::condenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_HeatPump_PlantLoop_EIR_CoolingFields::CondenserType);
  }

  std::vector<std::string> HeatPumpPlantLoopEIRCooling::validCondenserTypeValues() {
    return HeatPumpPlantLoopEIRCooling::condenserTypeValues();
  }

  std::string HeatPumpPlantLoopEIRCooling::condenserType() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->condenserType();
  }

  boost::optional<HeatPumpPlantLoopEIRHeating> HeatPumpPlantLoopEIRCooling::companionHeatingHeatPump() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->companionHeatingHeatPump();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::loadSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->loadSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::isLoadSideReferenceFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isLoadSideReferenceFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::sourceSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->sourceSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::isSourceSideReferenceFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isSourceSideReferenceFlowRateAutosized();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::referenceCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->referenceCapacity();
  }

  bool HeatPumpPlantLoopEIRCooling::isReferenceCapacityAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isReferenceCapacityAutosized();
  }

  double HeatPumpPlantLoopEIRCooling::referenceCoefficientofPerformance() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->referenceCoefficientofPerformance();
  }

  double HeatPumpPlantLoopEIRCooling::sizingFactor() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->sizingFactor();
  }

  Curve HeatPumpPlantLoopEIRCooling::capacityModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->capacityModifierFunctionofTemperatureCurve();
  }

  Curve HeatPumpPlantLoopEIRCooling::electricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  }

  Curve HeatPumpPlantLoopEIRCooling::electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
  }

  std::string HeatPumpPlantLoopEIRCooling::controlType() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->controlType();
  }

  std::string HeatPumpPlantLoopEIRCooling::flowMode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->flowMode();
  }

  double HeatPumpPlantLoopEIRCooling::minimumPartLoadRatio() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->minimumPartLoadRatio();
  }

  double HeatPumpPlantLoopEIRCooling::minimumSourceInletTemperature() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->minimumSourceInletTemperature();
  }

  double HeatPumpPlantLoopEIRCooling::maximumSourceInletTemeperature() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->maximumSourceInletTemeperature();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::minimumSupplyWaterTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->minimumSupplyWaterTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::maximumSupplyWaterTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->maximumSupplyWaterTemperatureCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setCondenserType(condenserType);
  }

  bool HeatPumpPlantLoopEIRCooling::setCompanionHeatingHeatPump(const HeatPumpPlantLoopEIRHeating& companionHP) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setCompanionHeatingHeatPump(companionHP);
  }

  bool HeatPumpPlantLoopEIRCooling::setLoadSideReferenceFlowRate(double loadSideReferenceFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setLoadSideReferenceFlowRate(loadSideReferenceFlowRate);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeLoadSideReferenceFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeLoadSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::setSourceSideReferenceFlowRate(double sourceSideReferenceFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setSourceSideReferenceFlowRate(sourceSideReferenceFlowRate);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeSourceSideReferenceFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeSourceSideReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::setReferenceCapacity(double referenceCapacity) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeReferenceCapacity() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeReferenceCapacity();
  }

  bool HeatPumpPlantLoopEIRCooling::setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setReferenceCoefficientofPerformance(referenceCoefficientofPerformance);
  }

  bool HeatPumpPlantLoopEIRCooling::setSizingFactor(double sizingFactor) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setSizingFactor(sizingFactor);
  }

  bool HeatPumpPlantLoopEIRCooling::setCapacityModifierFunctionofTemperatureCurve(const Curve& capacityModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setCapacityModifierFunctionofTemperatureCurve(
      capacityModifierFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRCooling::setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
    const Curve& electricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRCooling::setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
    const Curve& electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(
      electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
  }

  bool HeatPumpPlantLoopEIRCooling::setControlType(const std::string& controlType) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setControlType(controlType);
  }

  bool HeatPumpPlantLoopEIRCooling::setFlowMode(const std::string& flowMode) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setFlowMode(flowMode);
  }

  bool HeatPumpPlantLoopEIRCooling::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
  }

  bool HeatPumpPlantLoopEIRCooling::setMinimumSourceInletTemperature(double minimumSourceInletTemperature) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMinimumSourceInletTemperature(minimumSourceInletTemperature);
  }

  bool HeatPumpPlantLoopEIRCooling::setMaximumSourceInletTemperature(double maximumSourceInletTemeperature) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMaximumSourceInletTemperature(maximumSourceInletTemeperature);
  }

  bool HeatPumpPlantLoopEIRCooling::setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMinimumSupplyWaterTemperatureCurve(minimumSupplyWaterTemperatureCurve);
  }

  bool HeatPumpPlantLoopEIRCooling::setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMaximumSupplyWaterTemperatureCurve(maximumSupplyWaterTemperatureCurve);
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedLoadSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedLoadSideReferenceFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedSourceSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedSourceSideReferenceFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedReferenceCapacity() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedReferenceCapacity();
  }

  // Convenience functions
  boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling::loadSideWaterLoop() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->loadSideWaterLoop();
  }

  boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling::sourceSideWaterLoop() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->sourceSideWaterLoop();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRCooling::sourceSideWaterInletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->sourceSideWaterInletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRCooling::sourceSideWaterOutletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->sourceSideWaterOutletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRCooling::loadSideWaterInletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->loadSideWaterInletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRCooling::loadSideWaterOutletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->loadSideWaterOutletNode();
  }

  /// @cond
  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(std::shared_ptr<detail::HeatPumpPlantLoopEIRCooling_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
