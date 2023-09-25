/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
#include "LoadingIndex.hpp"
#include "LoadingIndex_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"

#include "ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include <algorithm>
#include <vector>

namespace openstudio {
namespace model {

  namespace detail {

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"VRF Heat Pump Total Cooling Rate",
                                                   "VRF Heat Pump Total Heating Rate",
                                                   "VRF Heat Pump Cooling Electricity Rate",
                                                   "VRF Heat Pump Cooling Electricity Energy",
                                                   "VRF Heat Pump Heating Electricity Rate",
                                                   "VRF Heat Pump Heating Electricity Energy",
                                                   "VRF Heat Pump Cooling COP",
                                                   "VRF Heat Pump Heating COP",
                                                   "VRF Heat Pump COP",
                                                   "VRF Heat Pump Compressor Electricity Rate",
                                                   "VRF Heat Pump Outdoor Unit Fan Power",
                                                   "VRF Heat Pump Compressor Rotating Speed",
                                                   "VRF Heat Pump Indoor Unit Evaporating Temperature",
                                                   "VRF Heat Pump Outdoor Unit Condensing Temperature",
                                                   "VRF Heat Pump Indoor Unit Condensing Temperature",
                                                   "VRF Heat Pump Outdoor Unit Evaporating Temperature",
                                                   "VRF Heat Pump Cooling Capacity at Max Compressor Speed",
                                                   "VRF Heat Pump Heating Capacity at Max Compressor Speed",
                                                   "VRF Heat Pump Indoor Unit Piping Correction for Cooling",
                                                   "VRF Heat Pump Indoor Unit Piping Correction for Heating",
                                                   "VRF Heat Pump Outdoor Unit Evaporator Heat Extract Rate",
                                                   "VRF Heat Pump Outdoor Unit Condenser Heat Release Rate",
                                                   "VRF Heat Pump Defrost Electricity Rate",
                                                   "VRF Heat Pump Defrost Electricity Energy",
                                                   "VRF Heat Pump Part Load Ratio",
                                                   "VRF Heat Pump Runtime Fraction",
                                                   "VRF Heat Pump Cycling Ratio",
                                                   "VRF Heat Pump Operating Mode",
                                                   "VRF Heat Pump Condenser Inlet Temperature",
                                                   "VRF Heat Pump Crankcase Heater Electricity Rate",
                                                   "VRF Heat Pump Crankcase Heater Electricity Energy",
                                                   "VRF Heat Pump Terminal Unit Cooling Load Rate",
                                                   "VRF Heat Pump Terminal Unit Heating Load Rate"};
      return result;
    }

    IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::iddObjectType() const {
      return AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(),
                    OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule)
          != fieldIndices.cend()) {
        result.emplace_back("AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule");
      }
      return result;
    }

    ModelObject AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::clone(Model model) const {
      auto airConditionerClone = HVACComponent_Impl::clone(model).cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();

      ModelObjectList vrfModelObjectList(model);
      airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVRFModelObjectList(
        vrfModelObjectList);

      // This will clone each LoadingIndex too
      ModelObjectList loadingIndexListClone = this->loadingIndexList().clone(model).cast<ModelObjectList>();
      airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setLoadingIndexList(
        loadingIndexListClone);

      return std::move(airConditionerClone);
    }

    std::vector<openstudio::IdfObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::remove() {
      vrfModelObjectList().remove();
      loadingIndexList().remove();

      return HVACComponent_Impl::remove();
    }

    std::vector<ModelObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::children() const {
      std::vector<ModelObject> result;

      std::vector<ModelObject> loadings;
      // children() is called during remove(), except we already trashed the moList...
      if (auto moList_ = optionalLoadingIndexList()) {
        loadings = moList_->modelObjects();
      }
      result.reserve(loadings.size() + 3);

      result.push_back(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve());
      result.push_back(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve());

      boost::optional<ModelObject> curve;
      curve = defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      for (auto&& loadingIndex : loadings) {
        result.emplace_back(std::move(loadingIndex));
      }

      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::addToNode(Node& node) {
      // Only accept the demand side of a PlantLoop
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return HVACComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosize() {
      autosizeRatedEvaporativeCapacity();
      autosizeResistiveDefrostHeaterCapacity();
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedRatedEvaporativeCapacity();
      if (val) {
        setRatedEvaporativeCapacity(val.get());
      }

      val = autosizedResistiveDefrostHeaterCapacity();
      if (val) {
        setResistiveDefrostHeaterCapacity(val.get());
      }
    }

    std::vector<EMSActuatorNames> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Variable Refrigerant Flow Heat Pump", "Operating Mode"}};
      return actuators;
    }

    std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::refrigerantType() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedEvaporativeCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isRatedEvaporativeCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedRatedEvaporativeCapacity() {
      // EPLUS-SQL-INCONSISTENCY + NOT IN SQL AT ALL
      return getAutosizedValue("Design Size Rated Evaporative Capacity", "W", "AirConditioner:VariableRefrigerantFlow");
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedCompressorPowerPerUnitofRatedEvaporativeCapacity() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::minimumOutdoorAirTemperatureinCoolingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorAirTemperatureinCoolingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::minimumOutdoorAirTemperatureinHeatingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorAirTemperatureinHeatingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::referenceOutdoorUnitSuperheating() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSuperheating, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::referenceOutdoorUnitSubcooling() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSubcooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::refrigerantTemperatureControlAlgorithmforIndoorUnit() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::referenceEvaporatingTemperatureforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceEvaporatingTemperatureforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::referenceCondensingTemperatureforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceCondensingTemperatureforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::variableEvaporatingTemperatureMinimumforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMinimumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::variableEvaporatingTemperatureMaximumforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMaximumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::variableCondensingTemperatureMinimumforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMinimumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::variableCondensingTemperatureMaximumforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMaximumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      boost::optional<Curve> value = optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Unit Evaporating Temperature Function of Superheating Curve attached.");
      }
      return value.get();
    }

    Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      boost::optional<Curve> value = optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Unit Condensing Temperature Function of Subcooling Curve attached.");
      }
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint()
      const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
        true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heightDifferenceBetweenOutdoorUnitandIndoorUnits() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::mainPipeInsulationThickness() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::mainPipeInsulationThermalConductivity() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::crankcaseHeaterPowerperCompressor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor, true);
      OS_ASSERT(value);
      return value.get();
    }

    int AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::numberofCompressors() const {
      boost::optional<int> value = getInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::ratioofCompressorSizetoTotalCompressorCapacity() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeater() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostStrategy() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostControl() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostTimePeriodFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resistiveDefrostHeaterCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedResistiveDefrostHeaterCapacity() {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Resistive Defrost Heater Capacity", "W", "AirConditioner:VariableRefrigerantFlow");
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::compressorMaximumDeltaPressure() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressorMaximumDeltaPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ModelObjectList> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalLoadingIndexList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LoadingIndexList);
    }

    ModelObjectList AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::loadingIndexList() const {
      boost::optional<ModelObjectList> moList_ = optionalLoadingIndexList();

      OS_ASSERT(moList_);

      return moList_.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::addLoadingIndex(LoadingIndex& loadingIndex) {
      return loadingIndexList().addModelObject(loadingIndex);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeLoadingIndex(LoadingIndex& loadingIndex) {
      loadingIndexList().removeModelObject(loadingIndex);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeAllLoadingIndexes() {
      loadingIndexList().removeAllModelObjects();
    }

    std::vector<LoadingIndex> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::loadingIndexes() const {
      return subsetCastVector<LoadingIndex>(loadingIndexList().modelObjects());
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setLoadingIndexList(const ModelObjectList& modelObjectList) {
      return setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LoadingIndexList, modelObjectList.handle());
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule", schedule);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRefrigerantType(const std::string& refrigerantType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType, refrigerantType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedEvaporativeCapacity(double ratedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, ratedEvaporativeCapacity);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeRatedEvaporativeCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(
      double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity,
                  ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMinimumOutdoorAirTemperatureinCoolingMode(
      double minimumOutdoorAirTemperatureinCoolingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinCoolingMode,
                              minimumOutdoorAirTemperatureinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorAirTemperatureinCoolingMode(
      double maximumOutdoorAirTemperatureinCoolingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinCoolingMode,
                              maximumOutdoorAirTemperatureinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMinimumOutdoorAirTemperatureinHeatingMode(
      double minimumOutdoorAirTemperatureinHeatingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinHeatingMode,
                              minimumOutdoorAirTemperatureinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorAirTemperatureinHeatingMode(
      double maximumOutdoorAirTemperatureinHeatingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinHeatingMode,
                              maximumOutdoorAirTemperatureinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setReferenceOutdoorUnitSuperheating(
      double referenceOutdoorUnitSuperheating) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSuperheating,
                              referenceOutdoorUnitSuperheating);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setReferenceOutdoorUnitSubcooling(double referenceOutdoorUnitSubcooling) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSubcooling,
                              referenceOutdoorUnitSubcooling);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRefrigerantTemperatureControlAlgorithmforIndoorUnit(
      const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit) {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit,
                  refrigerantTemperatureControlAlgorithmforIndoorUnit);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setReferenceEvaporatingTemperatureforIndoorUnit(
      double referenceEvaporatingTemperatureforIndoorUnit) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceEvaporatingTemperatureforIndoorUnit,
                              referenceEvaporatingTemperatureforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setReferenceCondensingTemperatureforIndoorUnit(
      double referenceCondensingTemperatureforIndoorUnit) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceCondensingTemperatureforIndoorUnit,
                              referenceCondensingTemperatureforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVariableEvaporatingTemperatureMinimumforIndoorUnit(
      double variableEvaporatingTemperatureMinimumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMinimumforIndoorUnit,
                  variableEvaporatingTemperatureMinimumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVariableEvaporatingTemperatureMaximumforIndoorUnit(
      double variableEvaporatingTemperatureMaximumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMaximumforIndoorUnit,
                  variableEvaporatingTemperatureMaximumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVariableCondensingTemperatureMinimumforIndoorUnit(
      double variableCondensingTemperatureMinimumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMinimumforIndoorUnit,
                  variableCondensingTemperatureMinimumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVariableCondensingTemperatureMaximumforIndoorUnit(
      double variableCondensingTemperatureMaximumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMaximumforIndoorUnit,
                  variableCondensingTemperatureMaximumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(
      double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity,
                  outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(
      double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity,
                  outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(
      const Curve& curve) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName,
        curve.handle());
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(
      const Curve& curve) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
        curve.handle());
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
      double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
      bool result = setDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
        diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
      double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
                  lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
      double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
      bool result = setDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
        equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(
      double heightDifferenceBetweenOutdoorUnitandIndoorUnits) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits,
                  heightDifferenceBetweenOutdoorUnitandIndoorUnits);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMainPipeInsulationThickness(double mainPipeInsulationThickness) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThickness, mainPipeInsulationThickness);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMainPipeInsulationThermalConductivity(
      double mainPipeInsulationThermalConductivity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThermalConductivity,
                              mainPipeInsulationThermalConductivity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCrankcaseHeaterPowerperCompressor(
      double crankcaseHeaterPowerperCompressor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor,
                              crankcaseHeaterPowerperCompressor);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setNumberofCompressors(int numberofCompressors) {
      bool result = setInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, numberofCompressors);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatioofCompressorSizetoTotalCompressorCapacity(
      double ratioofCompressorSizetoTotalCompressorCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity,
                              ratioofCompressorSizetoTotalCompressorCapacity);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(
      double maximumOutdoorDryBulbTemperatureforCrankcaseHeater) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater,
                  maximumOutdoorDryBulbTemperatureforCrankcaseHeater);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostStrategy(const std::string& defrostStrategy) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, defrostStrategy);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostControl(const std::string& defrostControl) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, defrostControl);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(
      const Curve& curve) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
        curve.handle());
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity,
                              resistiveDefrostHeaterCapacity);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeResistiveDefrostHeaterCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
      double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                  maximumOutdoorDrybulbTemperatureforDefrostOperation);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressorMaximumDeltaPressure,
                              compressorMaximumDeltaPressure);
      return result;
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve()
        const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName);
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule);
    }

    ModelObjectList AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::vrfModelObjectList() const {
      boost::optional<ModelObjectList> mo = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList);

      OS_ASSERT(mo);

      return mo.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVRFModelObjectList(const ModelObjectList& modelObjectList) {
      return setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList, modelObjectList.handle());
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      if (!vrf.isFluidTemperatureControl()) {
        LOG(Warn, "For " << briefDescription() << ", cannot add a terminal that uses non-FluidTemperatureControl coils: " << vrf.briefDescription());
        return false;
      }
      return vrfModelObjectList().addModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().removeModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeAllTerminals() {
      vrfModelObjectList().removeAllModelObjects();
    }

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::terminals() const {
      return subsetCastVector<ZoneHVACTerminalUnitVariableRefrigerantFlow>(vrfModelObjectList().modelObjects());
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedGrossRatedTotalCoolingCapacity() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Total Cooling Capacity (gross)", "W", "AirConditioner:VariableRefrigerantFlow");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedGrossRatedHeatingCapacity() const {
      // EPLUS-SQL-INCONSISTENCY
      return getAutosizedValue("Design Size Rated Total Heating Capacity", "W", "AirConditioner:VariableRefrigerantFlow");
    }

    ComponentType AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::componentType() const {
      return ComponentType::Both;
    }

    std::vector<FuelType> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<AppGFuelType> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::HeatPump};
    }

  }  // namespace detail

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl(const Model& model)
    : HVACComponent(AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>());

    bool ok = true;
    Schedule schedule = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(schedule);
    OS_ASSERT(ok);
    ok = setRefrigerantType("R410a");
    OS_ASSERT(ok);
    ok = setRatedEvaporativeCapacity(40000);
    OS_ASSERT(ok);
    ok = setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(0.35);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorAirTemperatureinCoolingMode(-6.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorAirTemperatureinCoolingMode(43.0);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorAirTemperatureinHeatingMode(-20.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorAirTemperatureinHeatingMode(16.0);
    OS_ASSERT(ok);
    ok = setReferenceOutdoorUnitSuperheating(3);
    OS_ASSERT(ok);
    ok = setReferenceOutdoorUnitSubcooling(5);
    OS_ASSERT(ok);
    ok = setRefrigerantTemperatureControlAlgorithmforIndoorUnit("VariableTemp");
    OS_ASSERT(ok);
    ok = setReferenceEvaporatingTemperatureforIndoorUnit(6.0);
    OS_ASSERT(ok);
    ok = setReferenceCondensingTemperatureforIndoorUnit(44.0);
    OS_ASSERT(ok);
    ok = setVariableEvaporatingTemperatureMinimumforIndoorUnit(4.0);
    OS_ASSERT(ok);
    ok = setVariableEvaporatingTemperatureMaximumforIndoorUnit(13.0);
    OS_ASSERT(ok);
    ok = setVariableCondensingTemperatureMinimumforIndoorUnit(42.0);
    OS_ASSERT(ok);
    ok = setVariableCondensingTemperatureMaximumforIndoorUnit(46.0);
    OS_ASSERT(ok);
    ok = setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(4.25E-3);
    OS_ASSERT(ok);
    ok = setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(7.50E-5);
    OS_ASSERT(ok);
    ok = setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(0.0762);
    OS_ASSERT(ok);
    ok = setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(30.0);
    OS_ASSERT(ok);
    ok = setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(36.0);
    OS_ASSERT(ok);
    ok = setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(5.0);
    OS_ASSERT(ok);
    ok = setMainPipeInsulationThickness(0.02);
    OS_ASSERT(ok);
    ok = setMainPipeInsulationThermalConductivity(0.032);
    OS_ASSERT(ok);
    ok = setCrankcaseHeaterPowerperCompressor(33.0);
    OS_ASSERT(ok);
    ok = setNumberofCompressors(2);
    OS_ASSERT(ok);
    ok = setRatioofCompressorSizetoTotalCompressorCapacity(0.5);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(5.0);
    OS_ASSERT(ok);
    ok = setDefrostStrategy("Resistive");
    OS_ASSERT(ok);
    ok = setDefrostControl("Timed");
    OS_ASSERT(ok);
    ok = setDefrostTimePeriodFraction(0.058333);
    OS_ASSERT(ok);
    ok = setResistiveDefrostHeaterCapacity(0.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorDrybulbTemperatureforDefrostOperation(5.0);
    OS_ASSERT(ok);
    ok = setCompressorMaximumDeltaPressure(4500000.0);
    OS_ASSERT(ok);

    {
      CurveQuadratic outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(model);
      outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setName(name().get() + " OUEvapTempCurve");
      outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient1Constant(0);
      outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient2x(6.05E-1);
      outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient3xPOW2(2.50E-2);
      outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setMinimumValueofx(0);
      outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setMaximumValueofx(15);
      ok = setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
      OS_ASSERT(ok);
    }

    {
      CurveQuadratic outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(model);
      outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setName(name().get() + " OUCondTempCurve");
      outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient1Constant(0);
      outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient2x(-2.91);
      outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient3xPOW2(1.180);
      outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setMinimumValueofx(0);
      outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setMaximumValueofx(5);
      ok = setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
      OS_ASSERT(ok);
    }

    //ok = setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
    //OS_ASSERT(ok);

    {
      ModelObjectList vrfModelObjectList(model);
      ok = getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVRFModelObjectList(vrfModelObjectList);
      OS_ASSERT(ok);
    }

    {
      ModelObjectList loadingIndexList(model);
      ok = getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setLoadingIndexList(loadingIndexList);
      OS_ASSERT(ok);
    }

    {
      CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve1(model);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setName(name().get() + " MinSpdCooling");
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setCoefficient1Constant(3.19E-01);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setCoefficient2x(-1.26E-03);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setCoefficient3xPOW2(-2.15E-05);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setCoefficient4y(1.20E-02);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setCoefficient5yPOW2(1.05E-04);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setCoefficient6xTIMESY(-8.66E-05);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setMinimumValueofx(15);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setMaximumValueofx(65);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setMinimumValueofy(-30);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve1.setMaximumValueofy(15);

      CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve1(model);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setName(name().get() + " MinSpdPower");
      compressorPowerMultiplierFunctionofTemperatureCurve1.setCoefficient1Constant(8.79E-02);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setCoefficient2x(-1.72E-04);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setCoefficient3xPOW2(6.93E-05);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setCoefficient4y(-3.38E-05);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setCoefficient5yPOW2(-8.10E-06);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setCoefficient6xTIMESY(-1.04E-05);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setMinimumValueofx(15);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setMaximumValueofx(65);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setMinimumValueofy(-30);
      compressorPowerMultiplierFunctionofTemperatureCurve1.setMaximumValueofy(15);

      LoadingIndex loadingIndex1(model, 1500, evaporativeCapacityMultiplierFunctionofTemperatureCurve1,
                                 compressorPowerMultiplierFunctionofTemperatureCurve1);
      OS_ASSERT(addLoadingIndex(loadingIndex1));
    }

    {
      CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve2(model);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setName(name().get() + " Spd1Cooling");
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setCoefficient1Constant(8.12E-01);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setCoefficient2x(-4.23E-03);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setCoefficient3xPOW2(-4.11E-05);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setCoefficient4y(2.97E-02);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setCoefficient5yPOW2(2.67E-04);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setCoefficient6xTIMESY(-2.23E-04);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setMinimumValueofx(15);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setMaximumValueofx(65);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setMinimumValueofy(-30);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve2.setMaximumValueofy(15);

      CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve2(model);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setName(name().get() + " Spd1Power");
      compressorPowerMultiplierFunctionofTemperatureCurve2.setCoefficient1Constant(3.26E-01);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setCoefficient2x(-2.20E-03);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setCoefficient3xPOW2(1.42E-04);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setCoefficient4y(2.82E-03);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setCoefficient5yPOW2(2.86E-05);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setCoefficient6xTIMESY(-3.50E-05);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setMinimumValueofx(15);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setMaximumValueofx(65);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setMinimumValueofy(-30);
      compressorPowerMultiplierFunctionofTemperatureCurve2.setMaximumValueofy(15);

      LoadingIndex loadingIndex2(model, 3600, evaporativeCapacityMultiplierFunctionofTemperatureCurve2,
                                 compressorPowerMultiplierFunctionofTemperatureCurve2);
      OS_ASSERT(addLoadingIndex(loadingIndex2));
    }

    {
      CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve3(model);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setName(name().get() + " Spd2Cooling");
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setCoefficient1Constant(1.32E+00);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setCoefficient2x(-6.20E-03);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setCoefficient3xPOW2(-7.10E-05);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setCoefficient4y(4.89E-02);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setCoefficient5yPOW2(4.59E-04);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setCoefficient6xTIMESY(-3.67E-04);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setMinimumValueofx(15);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setMaximumValueofx(65);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setMinimumValueofy(-30);
      evaporativeCapacityMultiplierFunctionofTemperatureCurve3.setMaximumValueofy(15);

      CurveBiquadratic compressorPowerMultiplierFunctionofTemperatureCurve3(model);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setName(name().get() + " Spd2Power");
      compressorPowerMultiplierFunctionofTemperatureCurve3.setCoefficient1Constant(6.56E-01);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setCoefficient2x(-3.71E-03);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setCoefficient3xPOW2(2.07E-04);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setCoefficient4y(1.05E-02);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setCoefficient5yPOW2(7.36E-05);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setCoefficient6xTIMESY(-1.57E-04);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setMinimumValueofx(15);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setMaximumValueofx(65);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setMinimumValueofy(-30);
      compressorPowerMultiplierFunctionofTemperatureCurve3.setMaximumValueofy(15);

      LoadingIndex loadingIndex3(model, 6000, evaporativeCapacityMultiplierFunctionofTemperatureCurve3,
                                 compressorPowerMultiplierFunctionofTemperatureCurve3);
      OS_ASSERT(addLoadingIndex(loadingIndex3));
    }
  }

  IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::refrigerantTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::validRefrigerantTypeValues() {
    return refrigerantTypeValues();
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::refrigerantTemperatureControlAlgorithmforIndoorUnitValues() {
    return getIddKeyNames(
      IddFactory::instance().getObject(iddObjectType()).get(),
      OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit);
  }

  std::vector<std::string>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::validRefrigerantTemperatureControlAlgorithmforIndoorUnitValues() {
    return refrigerantTemperatureControlAlgorithmforIndoorUnitValues();
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostStrategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::validDefrostStrategyValues() {
    return defrostStrategyValues();
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::validDefrostControlValues() {
    return defrostControlValues();
  }

  Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControl::availabilitySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->availabilitySchedule();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::refrigerantType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->refrigerantType();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedEvaporativeCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isRatedEvaporativeCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isRatedEvaporativeCapacityAutosized();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedEvaporativeCapacity() {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedRatedEvaporativeCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedCompressorPowerPerUnitofRatedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->ratedCompressorPowerPerUnitofRatedEvaporativeCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::minimumOutdoorAirTemperatureinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->minimumOutdoorAirTemperatureinCoolingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorAirTemperatureinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorAirTemperatureinCoolingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::minimumOutdoorAirTemperatureinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->minimumOutdoorAirTemperatureinHeatingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorAirTemperatureinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorAirTemperatureinHeatingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::referenceOutdoorUnitSuperheating() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->referenceOutdoorUnitSuperheating();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::referenceOutdoorUnitSubcooling() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->referenceOutdoorUnitSubcooling();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::refrigerantTemperatureControlAlgorithmforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->refrigerantTemperatureControlAlgorithmforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::referenceEvaporatingTemperatureforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->referenceEvaporatingTemperatureforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::referenceCondensingTemperatureforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->referenceCondensingTemperatureforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::variableEvaporatingTemperatureMinimumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->variableEvaporatingTemperatureMinimumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::variableEvaporatingTemperatureMaximumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->variableEvaporatingTemperatureMaximumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::variableCondensingTemperatureMinimumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->variableCondensingTemperatureMinimumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::variableCondensingTemperatureMaximumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->variableCondensingTemperatureMaximumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity();
  }

  Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControl::outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  }

  Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControl::outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heightDifferenceBetweenOutdoorUnitandIndoorUnits() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heightDifferenceBetweenOutdoorUnitandIndoorUnits();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::mainPipeInsulationThickness() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->mainPipeInsulationThickness();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::mainPipeInsulationThermalConductivity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->mainPipeInsulationThermalConductivity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::crankcaseHeaterPowerperCompressor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->crankcaseHeaterPowerperCompressor();
  }

  int AirConditionerVariableRefrigerantFlowFluidTemperatureControl::numberofCompressors() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->numberofCompressors();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratioofCompressorSizetoTotalCompressorCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratioofCompressorSizetoTotalCompressorCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorDryBulbTemperatureforCrankcaseHeater() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorDryBulbTemperatureforCrankcaseHeater();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostStrategy() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->defrostStrategy();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostControl() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->defrostControl();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostTimePeriodFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->defrostTimePeriodFraction();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isResistiveDefrostHeaterCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedResistiveDefrostHeaterCapacity() {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedResistiveDefrostHeaterCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->maximumOutdoorDrybulbTemperatureforDefrostOperation();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::compressorMaximumDeltaPressure() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->compressorMaximumDeltaPressure();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRefrigerantType(const std::string& refrigerantType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRefrigerantType(refrigerantType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedEvaporativeCapacity(double ratedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedEvaporativeCapacity(
      ratedEvaporativeCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeRatedEvaporativeCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeRatedEvaporativeCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(
    double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMinimumOutdoorAirTemperatureinCoolingMode(
    double minimumOutdoorAirTemperatureinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMinimumOutdoorAirTemperatureinCoolingMode(
      minimumOutdoorAirTemperatureinCoolingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorAirTemperatureinCoolingMode(
    double maximumOutdoorAirTemperatureinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMaximumOutdoorAirTemperatureinCoolingMode(
      maximumOutdoorAirTemperatureinCoolingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMinimumOutdoorAirTemperatureinHeatingMode(
    double minimumOutdoorAirTemperatureinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMinimumOutdoorAirTemperatureinHeatingMode(
      minimumOutdoorAirTemperatureinHeatingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorAirTemperatureinHeatingMode(
    double maximumOutdoorAirTemperatureinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMaximumOutdoorAirTemperatureinHeatingMode(
      maximumOutdoorAirTemperatureinHeatingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setReferenceOutdoorUnitSuperheating(double referenceOutdoorUnitSuperheating) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setReferenceOutdoorUnitSuperheating(
      referenceOutdoorUnitSuperheating);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setReferenceOutdoorUnitSubcooling(double referenceOutdoorUnitSubcooling) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setReferenceOutdoorUnitSubcooling(
      referenceOutdoorUnitSubcooling);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRefrigerantTemperatureControlAlgorithmforIndoorUnit(
    const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setRefrigerantTemperatureControlAlgorithmforIndoorUnit(refrigerantTemperatureControlAlgorithmforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setReferenceEvaporatingTemperatureforIndoorUnit(
    double referenceEvaporatingTemperatureforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setReferenceEvaporatingTemperatureforIndoorUnit(
      referenceEvaporatingTemperatureforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setReferenceCondensingTemperatureforIndoorUnit(
    double referenceCondensingTemperatureforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setReferenceCondensingTemperatureforIndoorUnit(
      referenceCondensingTemperatureforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setVariableEvaporatingTemperatureMinimumforIndoorUnit(
    double variableEvaporatingTemperatureMinimumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setVariableEvaporatingTemperatureMinimumforIndoorUnit(variableEvaporatingTemperatureMinimumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setVariableEvaporatingTemperatureMaximumforIndoorUnit(
    double variableEvaporatingTemperatureMaximumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setVariableEvaporatingTemperatureMaximumforIndoorUnit(variableEvaporatingTemperatureMaximumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setVariableCondensingTemperatureMinimumforIndoorUnit(
    double variableCondensingTemperatureMinimumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVariableCondensingTemperatureMinimumforIndoorUnit(
      variableCondensingTemperatureMinimumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setVariableCondensingTemperatureMaximumforIndoorUnit(
    double variableCondensingTemperatureMaximumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVariableCondensingTemperatureMaximumforIndoorUnit(
      variableCondensingTemperatureMaximumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(
    double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(
    double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
    double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
    double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
    double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(
    double heightDifferenceBetweenOutdoorUnitandIndoorUnits) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(
      heightDifferenceBetweenOutdoorUnitandIndoorUnits);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMainPipeInsulationThickness(double mainPipeInsulationThickness) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMainPipeInsulationThickness(
      mainPipeInsulationThickness);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMainPipeInsulationThermalConductivity(
    double mainPipeInsulationThermalConductivity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMainPipeInsulationThermalConductivity(
      mainPipeInsulationThermalConductivity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCrankcaseHeaterPowerperCompressor(
      crankcaseHeaterPowerperCompressor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setNumberofCompressors(int numberofCompressors) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setNumberofCompressors(numberofCompressors);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatioofCompressorSizetoTotalCompressorCapacity(
    double ratioofCompressorSizetoTotalCompressorCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatioofCompressorSizetoTotalCompressorCapacity(
      ratioofCompressorSizetoTotalCompressorCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeater) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(maximumOutdoorDryBulbTemperatureforCrankcaseHeater);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostStrategy(const std::string& defrostStrategy) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setDefrostStrategy(defrostStrategy);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostControl(const std::string& defrostControl) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setDefrostControl(defrostControl);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setDefrostTimePeriodFraction(
      defrostTimePeriodFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setResistiveDefrostHeaterCapacity(
      resistiveDefrostHeaterCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeResistiveDefrostHeaterCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeResistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
    double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCompressorMaximumDeltaPressure(
      compressorMaximumDeltaPressure);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->addTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->removeTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::removeAllTerminals() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->removeAllTerminals();
  }

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::terminals() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->terminals();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::addLoadingIndex(LoadingIndex& loadingIndex) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->addLoadingIndex(loadingIndex);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::removeLoadingIndex(LoadingIndex& loadingIndex) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->removeLoadingIndex(loadingIndex);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::removeAllLoadingIndexes() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->removeAllLoadingIndexes();
  }

  std::vector<LoadingIndex> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::loadingIndexes() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->loadingIndexes();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedGrossRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedGrossRatedTotalCoolingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedGrossRatedHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedGrossRatedHeatingCapacity();
  }

  /// @cond
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl(
    std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl> impl)
    : HVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
