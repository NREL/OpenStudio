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
      static const std::vector<std::string> result{
        "Heat Pump Part Load Ratio",
        "Heat Pump Cycling Ratio",
        "Heat Pump Load Side Heat Transfer Rate",
        "Heat Pump Load Side Heat Transfer Energy",
        "Heat Pump Source Side Heat Transfer Rate",
        "Heat Pump Source Side Heat Transfer Energy",
        "Heat Pump Load Side Inlet Temperature",
        "Heat Pump Load Side Outlet Temperature",
        "Heat Pump Source Side Inlet Temperature",
        "Heat Pump Source Side Outlet Temperature",
        "Heat Pump Electricity Rate",
        "Heat Pump Load Side Mass Flow Rate",
        "Heat Pump Source Side Mass Flow Rate",
        "Heat Pump Heat Recovery Heat Transfer Rate",
        "Heat Pump Heat Recovery Heat Transfer Energy",
        "Heat Pump Heat Recovery Inlet Temperature",
        "Heat Pump Heat Recovery Outlet Temperature",
        "Heat Pump Heat Recovery Mass Flow Rate",
        "Heat Pump Heat Recovery Operation Status",
        "Heat Pump Electricity Energy",
        "Thermosiphon Status",
      };
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

    unsigned HeatPumpPlantLoopEIRCooling_Impl::tertiaryInletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryInletNodeName;
    }

    unsigned HeatPumpPlantLoopEIRCooling_Impl::tertiaryOutletPort() const {
      return OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryOutletNodeName;
    }

    /** Convenience Function to return the Load Side Water Loop (HeatPump on supply side) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling_Impl::loadSideWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Source Side (Condenser) Water Loop (HeatPump on demand side) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling_Impl::sourceSideWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    /** Convenience Function to return the Heat Recovery Loop (HeatPump on demand side - tertiary) **/
    boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling_Impl::heatRecoveryLoop() const {
      return WaterToWaterComponent_Impl::tertiaryPlantLoop();
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

    boost::optional<Node> HeatPumpPlantLoopEIRCooling_Impl::heatRecoveryInletNode() const {
      boost::optional<Node> result;

      if (auto mo = tertiaryInletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    boost::optional<Node> HeatPumpPlantLoopEIRCooling_Impl::heatRecoveryOutletNode() const {
      boost::optional<Node> result;

      if (auto mo = tertiaryOutletModelObject()) {
        result = mo->optionalCast<Node>();
      }
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::addToNode(Node& node) {

      boost::optional<PlantLoop> t_plantLoop = node.plantLoop();

      // If trying to add to a node that is on the demand side of a plant loop
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // If there is already a source side water Plant Loop
          if (boost::optional<PlantLoop> ssLoop = this->sourceSideWaterLoop()) {
            // And it's not the same as the node's loop
            if (t_plantLoop.get() != ssLoop.get()) {
              // And if there is no generator loop (tertiary)
              if (!this->heatRecoveryLoop().is_initialized()) {
                // Then try to add it to the tertiary one
                LOG(Warn, "Calling addToTertiaryNode to connect it to the tertiary (=Heat Recovery Loop) loop for " << briefDescription());
                return this->addToTertiaryNode(node);
              }
            }
          }
        }
      }

      // All other cases, call the base class implementation
      // call the base class implementation to connect the component
      bool ok = WaterToWaterComponent_Impl::addToNode(node);

      // If there's a secondary plant loop, switch the condenser type to "WaterSource"
      if (this->sourceSideWaterLoop()) {
        this->setCondenserType("WaterSource");
      }
      return ok;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::addToTertiaryNode(Node& node) {

      auto t_plantLoop = node.plantLoop();

      // Only accept adding to a node that is on a demand side of a plant loop
      // Since tertiary here = heat recovery loop (heating)
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // Call base class method which accepts both supply and demand
          return WaterToWaterComponent_Impl::addToTertiaryNode(node);
        } else {
          LOG(Info,
              "Tertiary Loop (Heat Recovery Loop) connections can only be placed on the Demand side (of a Heating Loop), for " << briefDescription());
        }
      }
      return false;
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

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::heatRecoveryReferenceFlowRate() const {
      return getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryReferenceFlowRate, true);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::isHeatRecoveryReferenceFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryReferenceFlowRate, true);
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
      boost::optional<std::string> value = getString(OS_HeatPump_PlantLoop_EIR_CoolingFields::FlowMode, true);
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

    double HeatPumpPlantLoopEIRCooling_Impl::maximumSourceInletTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSourceInletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::minimumSupplyWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSupplyWaterTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::maximumSupplyWaterTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSupplyWaterTemperatureCurveName);
    }

    double HeatPumpPlantLoopEIRCooling_Impl::maximumHeatRecoveryOutletTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumHeatRecoveryOutletTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::heatRecoveryCapacityModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryCapacityModifierFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName);
    }

    boost::optional<Curve> HeatPumpPlantLoopEIRCooling_Impl::thermosiphonCapacityFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonCapacityFractionCurveName);
    }

    double HeatPumpPlantLoopEIRCooling_Impl::thermosiphonMinimumTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonMinimumTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
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

    bool HeatPumpPlantLoopEIRCooling_Impl::setHeatRecoveryReferenceFlowRate(double heatRecoveryReferenceFlowRate) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryReferenceFlowRate, heatRecoveryReferenceFlowRate);
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosizeHeatRecoveryReferenceFlowRate() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryReferenceFlowRate, "Autosize");
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

    bool HeatPumpPlantLoopEIRCooling_Impl::setFlowMode(const std::string& flowMode) {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::FlowMode, flowMode);
      return result;
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMinimumSourceInletTemperature(double minimumSourceInletTemperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSourceInletTemperature, minimumSourceInletTemperature);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMaximumSourceInletTemperature(double maximumSourceInletTemperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSourceInletTemperature, maximumSourceInletTemperature);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSupplyWaterTemperatureCurveName, minimumSupplyWaterTemperatureCurve.handle());
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::resetMinimumSupplyWaterTemperatureCurve() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::MinimumSupplyWaterTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSupplyWaterTemperatureCurveName, maximumSupplyWaterTemperatureCurve.handle());
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::resetMaximumSupplyWaterTemperatureCurve() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumSupplyWaterTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setMaximumHeatRecoveryOutletTemperature(double maximumHeatRecoveryOutletTemperature) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::MaximumHeatRecoveryOutletTemperature, maximumHeatRecoveryOutletTemperature);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(
      const Curve& heatRecoveryCapacityModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryCapacityModifierFunctionofTemperatureCurveName,
                               heatRecoveryCapacityModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::resetHeatRecoveryCapacityModifierFunctionofTemperatureCurve() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryCapacityModifierFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      const Curve& heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
      bool result = setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                               heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve.handle());
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::resetHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve() {
      bool result =
        setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::HeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setThermosiphonCapacityFractionCurve(const Curve& thermosiphonCapacityFractionCurve) {
      bool result =
        setPointer(OS_HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonCapacityFractionCurveName, thermosiphonCapacityFractionCurve.handle());
      return result;
    }

    void HeatPumpPlantLoopEIRCooling_Impl::resetThermosiphonCapacityFractionCurve() {
      bool result = setString(OS_HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonCapacityFractionCurveName, "");
      OS_ASSERT(result);
    }

    bool HeatPumpPlantLoopEIRCooling_Impl::setThermosiphonMinimumTemperatureDifference(double thermosiphonMinimumTemperatureDifference) {
      return setDouble(OS_HeatPump_PlantLoop_EIR_CoolingFields::ThermosiphonMinimumTemperatureDifference, thermosiphonMinimumTemperatureDifference);
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedLoadSideReferenceFlowRate() const {
      return getAutosizedValue("Design Size Load Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedSourceSideReferenceFlowRate() const {
      return getAutosizedValue("Design Size Source Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedHeatRecoveryReferenceFlowRate() const {
      return getAutosizedValue("Design Size Heat Recovery Side Volume Flow Rate", "m3/s");
    }

    boost::optional<double> HeatPumpPlantLoopEIRCooling_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void HeatPumpPlantLoopEIRCooling_Impl::autosize() {
      autosizeLoadSideReferenceFlowRate();
      autosizeSourceSideReferenceFlowRate();
      autosizeHeatRecoveryReferenceFlowRate();
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

      val = autosizedHeatRecoveryReferenceFlowRate();
      if (val) {
        setHeatRecoveryReferenceFlowRate(val.get());
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
    autosizeHeatRecoveryReferenceFlowRate();
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
    setMaximumHeatRecoveryOutletTemperature(60.0);
    setThermosiphonMinimumTemperatureDifference(0.0);
  }

  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(const Model& model)
    : WaterToWaterComponent(HeatPumpPlantLoopEIRCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>());

    autosizeLoadSideReferenceFlowRate();
    autosizeSourceSideReferenceFlowRate();
    autosizeHeatRecoveryReferenceFlowRate();
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
    setMaximumHeatRecoveryOutletTemperature(60.0);
    setThermosiphonMinimumTemperatureDifference(0.0);
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

  boost::optional<double> HeatPumpPlantLoopEIRCooling::heatRecoveryReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->heatRecoveryReferenceFlowRate();
  }

  bool HeatPumpPlantLoopEIRCooling::isHeatRecoveryReferenceFlowRateAutosized() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->isHeatRecoveryReferenceFlowRateAutosized();
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

  double HeatPumpPlantLoopEIRCooling::maximumSourceInletTemperature() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->maximumSourceInletTemperature();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::minimumSupplyWaterTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->minimumSupplyWaterTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::maximumSupplyWaterTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->maximumSupplyWaterTemperatureCurve();
  }

  double HeatPumpPlantLoopEIRCooling::maximumHeatRecoveryOutletTemperature() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->maximumHeatRecoveryOutletTemperature();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::heatRecoveryCapacityModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->heatRecoveryCapacityModifierFunctionofTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  }

  boost::optional<Curve> HeatPumpPlantLoopEIRCooling::thermosiphonCapacityFractionCurve() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->thermosiphonCapacityFractionCurve();
  }

  double HeatPumpPlantLoopEIRCooling::thermosiphonMinimumTemperatureDifference() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->thermosiphonMinimumTemperatureDifference();
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

  bool HeatPumpPlantLoopEIRCooling::setHeatRecoveryReferenceFlowRate(double heatRecoveryReferenceFlowRate) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setHeatRecoveryReferenceFlowRate(heatRecoveryReferenceFlowRate);
  }

  void HeatPumpPlantLoopEIRCooling::autosizeHeatRecoveryReferenceFlowRate() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizeHeatRecoveryReferenceFlowRate();
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

  bool HeatPumpPlantLoopEIRCooling::setMaximumSourceInletTemperature(double maximumSourceInletTemperature) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMaximumSourceInletTemperature(maximumSourceInletTemperature);
  }

  bool HeatPumpPlantLoopEIRCooling::setMinimumSupplyWaterTemperatureCurve(const Curve& minimumSupplyWaterTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMinimumSupplyWaterTemperatureCurve(minimumSupplyWaterTemperatureCurve);
  }

  void HeatPumpPlantLoopEIRCooling::resetMinimumSupplyWaterTemperatureCurve() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->resetMinimumSupplyWaterTemperatureCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setMaximumSupplyWaterTemperatureCurve(const Curve& maximumSupplyWaterTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMaximumSupplyWaterTemperatureCurve(maximumSupplyWaterTemperatureCurve);
  }

  void HeatPumpPlantLoopEIRCooling::resetMaximumSupplyWaterTemperatureCurve() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->resetMaximumSupplyWaterTemperatureCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setMaximumHeatRecoveryOutletTemperature(double maximumHeatRecoveryOutletTemperature) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setMaximumHeatRecoveryOutletTemperature(maximumHeatRecoveryOutletTemperature);
  }

  bool HeatPumpPlantLoopEIRCooling::setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(
    const Curve& heatRecoveryCapacityModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(
      heatRecoveryCapacityModifierFunctionofTemperatureCurve);
  }

  void HeatPumpPlantLoopEIRCooling::resetHeatRecoveryCapacityModifierFunctionofTemperatureCurve() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->resetHeatRecoveryCapacityModifierFunctionofTemperatureCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
    const Curve& heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(
      heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve);
  }

  void HeatPumpPlantLoopEIRCooling::resetHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->resetHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setThermosiphonCapacityFractionCurve(const Curve& thermosiphonCapacityFractionCurve) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setThermosiphonCapacityFractionCurve(thermosiphonCapacityFractionCurve);
  }

  void HeatPumpPlantLoopEIRCooling::resetThermosiphonCapacityFractionCurve() {
    getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->resetThermosiphonCapacityFractionCurve();
  }

  bool HeatPumpPlantLoopEIRCooling::setThermosiphonMinimumTemperatureDifference(double thermosiphonMinimumTemperatureDifference) {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->setThermosiphonMinimumTemperatureDifference(thermosiphonMinimumTemperatureDifference);
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedLoadSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedLoadSideReferenceFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedSourceSideReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedSourceSideReferenceFlowRate();
  }

  boost::optional<double> HeatPumpPlantLoopEIRCooling::autosizedHeatRecoveryReferenceFlowRate() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->autosizedHeatRecoveryReferenceFlowRate();
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

  boost::optional<PlantLoop> HeatPumpPlantLoopEIRCooling::heatRecoveryLoop() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->heatRecoveryLoop();
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

  boost::optional<Node> HeatPumpPlantLoopEIRCooling::heatRecoveryInletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->heatRecoveryInletNode();
  }

  boost::optional<Node> HeatPumpPlantLoopEIRCooling::heatRecoveryOutletNode() const {
    return getImpl<detail::HeatPumpPlantLoopEIRCooling_Impl>()->heatRecoveryOutletNode();
  }

  /// @cond
  HeatPumpPlantLoopEIRCooling::HeatPumpPlantLoopEIRCooling(std::shared_ptr<detail::HeatPumpPlantLoopEIRCooling_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
