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

#include <vector>
#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR.hpp"
#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl.hpp"
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

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
      const AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::iddObjectType() const {
      return AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Availability Schedule"));
      }
      return result;
    }

    unsigned AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::inletPort() const {
      return 0;
    }

    unsigned AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outletPort() const {
      return 0;
    }

    ModelObject AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::clone(Model model) const {
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR airConditionerClone =
        StraightComponent_Impl::clone(model).cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>();

      ModelObjectList modelObjectList(model);
      airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setVRFModelObjectList(
        modelObjectList);

      if (boost::optional<Curve> curve = outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(clone);
      }

      if (boost::optional<Curve> curve = outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(clone);
      }

      if (auto curve = defrostEnergyInputRatioModifierFunctionofTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(clone);
      }

      ModelObjectList loadingIndexList(model);
      airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setLoadingIndexList(
        loadingIndexList);

      return airConditionerClone;
    }

    std::vector<openstudio::IdfObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::remove() {
      vrfModelObjectList().remove();
      loadingIndexList().remove();
      auto result = StraightComponent_Impl::remove();

      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::addToNode(Node& node) {
      // Only accept the demand side of a PlantLoop
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          // bool success = StraightComponent_Impl::addToNode(node);
          // if (success) {
          // // If everything went well, then switch the condenser type
          //   setCondenserType("WaterCooled");
          //   return success;
          // }
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    // This override is rendered moot now.
    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeFromLoop() {
      // Disconnect the component
      bool ok = StraightComponent_Impl::removeFromLoop();

      // Don't Switch the condenser type to "AirCooled"
      // this->setCondenserType("AirCooled");
      return ok;
    }

    std::vector<ModelObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::children() const {
      std::vector<ModelObject> result;

      boost::optional<ModelObject> curve;

      curve = outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      for (const auto& mo : loadingIndexList().modelObjects()) {
        result.push_back(mo);
      }

      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosize() {
      autosizeRatedEvaporativeCapacity();
      autosizeResistiveDefrostHeaterCapacity();
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::applySizingValues() {
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

    std::vector<EMSActuatorNames> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Variable Refrigerant Flow Heat Pump", "Operating Mode"}};
      return actuators;
    }

    std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::refrigerantType() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::ratedEvaporativeCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isRatedEvaporativeCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedRatedEvaporativeCapacity() {
      return getAutosizedValue("TODO_CHECK_SQL Rated Evaporative Capacity", "W");
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::ratedCompressorPowerPerUnitofRatedEvaporativeCapacity() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumOutdoorAirTemperatureinCoolingOnlyMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinCoolingOnlyMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorAirTemperatureinCoolingOnlyMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinCoolingOnlyMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumOutdoorAirTemperatureinHeatingOnlyMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinHeatingOnlyMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorAirTemperatureinHeatingOnlyMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinHeatingOnlyMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumOutdoorTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorTemperatureinHeatRecoveryMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorTemperatureinHeatRecoveryMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::refrigerantTemperatureControlAlgorithmforIndoorUnit() const {
      boost::optional<std::string> value = getString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::referenceEvaporatingTemperatureforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceEvaporatingTemperatureforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::referenceCondensingTemperatureforIndoorUnit() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceCondensingTemperatureforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::variableEvaporatingTemperatureMinimumforIndoorUnit() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMinimumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::variableEvaporatingTemperatureMaximumforIndoorUnit() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMaximumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::variableCondensingTemperatureMinimumforIndoorUnit() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMinimumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::variableCondensingTemperatureMaximumforIndoorUnit() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMaximumforIndoorUnit, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitEvaporatorReferenceSuperheating() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorReferenceSuperheating, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitCondenserReferenceSubcooling() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserReferenceSubcooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitEvaporatorRatedBypassFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorRatedBypassFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitCondenserRatedBypassFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserRatedBypassFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::
      differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::
                                                  DifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode,
                                                true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitHeatExchangerCapacityRatio() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitHeatExchangerCapacityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      boost::optional<Curve> value = optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Unit Evaporating Temperature Functionof Superheating Curve attached.");
      }
      return value.get();
    }

    Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      boost::optional<Curve> value = optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Unit Condensing Temperature Functionof Subcooling Curve attached.");
      }
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::diameterofMainPipeforSuctionGas() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforSuctionGas, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::diameterofMainPipeforDischargeGas() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforDischargeGas, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint()
      const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::
                                                  EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
                                                true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heightDifferenceBetweenOutdoorUnitandIndoorUnits() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::mainPipeInsulationThickness() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThickness, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::mainPipeInsulationThermalConductivity() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThermalConductivity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::crankcaseHeaterPowerperCompressor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CrankcaseHeaterPowerperCompressor, true);
      OS_ASSERT(value);
      return value.get();
    }

    int AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::numberofCompressors() const {
      boost::optional<int> value = getInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::NumberofCompressors, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::ratioofCompressorSizetoTotalCompressorCapacity() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatioofCompressorSizetoTotalCompressorCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorDryBulbTemperatureforCrankcaseHeater() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostStrategy() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostStrategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostControl() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostTimePeriodFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostTimePeriodFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resistiveDefrostHeaterCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedResistiveDefrostHeaterCapacity() {
      return getAutosizedValue("TODO_CHECK_SQL Resistive Defrost Heater Capacity", "W");
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryCoolingCapacityFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryCoolingCapacityTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingCapacityTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryCoolingEnergyFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingEnergyFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryCoolingEnergyTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingEnergyTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryHeatingCapacityFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryHeatingCapacityTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingCapacityTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryHeatingEnergyFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingEnergyFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryHeatingEnergyTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingEnergyTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::compressorMaximumDeltaPressure() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorMaximumDeltaPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::compressorInverterEfficiency() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorInverterEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::compressorEvaporativeCapacityCorrectionFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorEvaporativeCapacityCorrectionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    ModelObjectList AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::loadingIndexList() const {
      boost::optional<ModelObjectList> mo = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::LoadingIndexList);

      OS_ASSERT(mo);

      return mo.get();
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::addLoadingIndex(LoadingIndex& loadingIndex) {
      loadingIndexList().addModelObject(loadingIndex);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeLoadingIndex(LoadingIndex& loadingIndex) {
      loadingIndexList().removeModelObject(loadingIndex);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeAllLoadingIndexes() {
      loadingIndexList().removeAllModelObjects();
    }

    std::vector<LoadingIndex> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::loadingIndexes() const {
      return subsetCastVector<LoadingIndex>(loadingIndexList().modelObjects());
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setLoadingIndexList(const ModelObjectList& modelObjectList) {
      return setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::LoadingIndexList, modelObjectList.handle());
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::AvailabilitySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Availability Schedule", schedule);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRefrigerantType(const std::string& refrigerantType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantType, refrigerantType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRatedEvaporativeCapacity(double ratedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, ratedEvaporativeCapacity);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeRatedEvaporativeCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(
      double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity,
                  ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumOutdoorAirTemperatureinCoolingOnlyMode(
      double minimumOutdoorAirTemperatureinCoolingOnlyMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinCoolingOnlyMode,
                  minimumOutdoorAirTemperatureinCoolingOnlyMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorAirTemperatureinCoolingOnlyMode(
      double maximumOutdoorAirTemperatureinCoolingOnlyMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinCoolingOnlyMode,
                  maximumOutdoorAirTemperatureinCoolingOnlyMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumOutdoorAirTemperatureinHeatingOnlyMode(
      double minimumOutdoorAirTemperatureinHeatingOnlyMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinHeatingOnlyMode,
                  minimumOutdoorAirTemperatureinHeatingOnlyMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorAirTemperatureinHeatingOnlyMode(
      double maximumOutdoorAirTemperatureinHeatingOnlyMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinHeatingOnlyMode,
                  maximumOutdoorAirTemperatureinHeatingOnlyMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumOutdoorTemperatureinHeatRecoveryMode(
      double minimumOutdoorTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorTemperatureinHeatRecoveryMode,
                              minimumOutdoorTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorTemperatureinHeatRecoveryMode(
      double maximumOutdoorTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorTemperatureinHeatRecoveryMode,
                              maximumOutdoorTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRefrigerantTemperatureControlAlgorithmforIndoorUnit(
      const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit) {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit,
                  refrigerantTemperatureControlAlgorithmforIndoorUnit);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setReferenceEvaporatingTemperatureforIndoorUnit(
      double referenceEvaporatingTemperatureforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceEvaporatingTemperatureforIndoorUnit,
                  referenceEvaporatingTemperatureforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setReferenceCondensingTemperatureforIndoorUnit(
      double referenceCondensingTemperatureforIndoorUnit) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceCondensingTemperatureforIndoorUnit,
                              referenceCondensingTemperatureforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVariableEvaporatingTemperatureMinimumforIndoorUnit(
      double variableEvaporatingTemperatureMinimumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMinimumforIndoorUnit,
                  variableEvaporatingTemperatureMinimumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVariableEvaporatingTemperatureMaximumforIndoorUnit(
      double variableEvaporatingTemperatureMaximumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMaximumforIndoorUnit,
                  variableEvaporatingTemperatureMaximumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVariableCondensingTemperatureMinimumforIndoorUnit(
      double variableCondensingTemperatureMinimumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMinimumforIndoorUnit,
                  variableCondensingTemperatureMinimumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVariableCondensingTemperatureMaximumforIndoorUnit(
      double variableCondensingTemperatureMaximumforIndoorUnit) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMaximumforIndoorUnit,
                  variableCondensingTemperatureMaximumforIndoorUnit);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitEvaporatorReferenceSuperheating(
      double outdoorUnitEvaporatorReferenceSuperheating) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorReferenceSuperheating,
                              outdoorUnitEvaporatorReferenceSuperheating);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitCondenserReferenceSubcooling(
      double outdoorUnitCondenserReferenceSubcooling) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserReferenceSubcooling,
                              outdoorUnitCondenserReferenceSubcooling);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitEvaporatorRatedBypassFactor(
      double outdoorUnitEvaporatorRatedBypassFactor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorRatedBypassFactor,
                              outdoorUnitEvaporatorRatedBypassFactor);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitCondenserRatedBypassFactor(
      double outdoorUnitCondenserRatedBypassFactor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserRatedBypassFactor,
                              outdoorUnitCondenserRatedBypassFactor);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::
      setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(
        double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::
                                DifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode,
                              differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitHeatExchangerCapacityRatio(
      double outdoorUnitHeatExchangerCapacityRatio) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitHeatExchangerCapacityRatio,
                              outdoorUnitHeatExchangerCapacityRatio);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(
      double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity,
                  outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(
      double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity,
                  outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::
                              OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName,
                            curve.get().handle());
      } else {
        result = true;
      }
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
          curve.get().handle());
      } else {
        result = true;
      }
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDiameterofMainPipeforSuctionGas(
      double diameterofMainPipeforSuctionGas) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforSuctionGas,
                              diameterofMainPipeforSuctionGas);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDiameterofMainPipeforDischargeGas(
      double diameterofMainPipeforDischargeGas) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforDischargeGas,
                              diameterofMainPipeforDischargeGas);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
      double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
      bool result = setDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
        lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
      double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::
                                EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
                              equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(
      double heightDifferenceBetweenOutdoorUnitandIndoorUnits) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits,
                  heightDifferenceBetweenOutdoorUnitandIndoorUnits);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMainPipeInsulationThickness(double mainPipeInsulationThickness) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThickness,
                              mainPipeInsulationThickness);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMainPipeInsulationThermalConductivity(
      double mainPipeInsulationThermalConductivity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThermalConductivity,
                              mainPipeInsulationThermalConductivity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCrankcaseHeaterPowerperCompressor(
      double crankcaseHeaterPowerperCompressor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CrankcaseHeaterPowerperCompressor,
                              crankcaseHeaterPowerperCompressor);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setNumberofCompressors(int numberofCompressors) {
      bool result = setInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::NumberofCompressors, numberofCompressors);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRatioofCompressorSizetoTotalCompressorCapacity(
      double ratioofCompressorSizetoTotalCompressorCapacity) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatioofCompressorSizetoTotalCompressorCapacity,
                  ratioofCompressorSizetoTotalCompressorCapacity);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(
      double maximumOutdoorDryBulbTemperatureforCrankcaseHeater) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater,
                  maximumOutdoorDryBulbTemperatureforCrankcaseHeater);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostStrategy(const std::string& defrostStrategy) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostStrategy, defrostStrategy);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostControl(const std::string& defrostControl) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostControl, defrostControl);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
          curve.get().handle());
      } else {
        resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
      return result;
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity,
                              resistiveDefrostHeaterCapacity);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeResistiveDefrostHeaterCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
      double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                  maximumOutdoorDrybulbTemperatureforDefrostOperation);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryCoolingCapacityFraction(
      double initialHeatRecoveryCoolingCapacityFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingCapacityFraction,
                              initialHeatRecoveryCoolingCapacityFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryCoolingCapacityTimeConstant(
      double heatRecoveryCoolingCapacityTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingCapacityTimeConstant,
                              heatRecoveryCoolingCapacityTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryCoolingEnergyFraction(
      double initialHeatRecoveryCoolingEnergyFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingEnergyFraction,
                              initialHeatRecoveryCoolingEnergyFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryCoolingEnergyTimeConstant(
      double heatRecoveryCoolingEnergyTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingEnergyTimeConstant,
                              heatRecoveryCoolingEnergyTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryHeatingCapacityFraction(
      double initialHeatRecoveryHeatingCapacityFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingCapacityFraction,
                              initialHeatRecoveryHeatingCapacityFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryHeatingCapacityTimeConstant(
      double heatRecoveryHeatingCapacityTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingCapacityTimeConstant,
                              heatRecoveryHeatingCapacityTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryHeatingEnergyFraction(
      double initialHeatRecoveryHeatingEnergyFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingEnergyFraction,
                              initialHeatRecoveryHeatingEnergyFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryHeatingEnergyTimeConstant(
      double heatRecoveryHeatingEnergyTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingEnergyTimeConstant,
                              heatRecoveryHeatingEnergyTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorMaximumDeltaPressure,
                              compressorMaximumDeltaPressure);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCompressorInverterEfficiency(double compressorInverterEfficiency) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorInverterEfficiency,
                              compressorInverterEfficiency);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCompressorEvaporativeCapacityCorrectionFactor(
      double compressorEvaporativeCapacityCorrectionFactor) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorEvaporativeCapacityCorrectionFactor,
                  compressorEvaporativeCapacityCorrectionFactor);
      return result;
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve()
        const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName);
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::AvailabilitySchedule);
    }

    ModelObjectList AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::vrfModelObjectList() const {
      boost::optional<ModelObjectList> mo = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ZoneTerminalUnitList);

      OS_ASSERT(mo);

      return mo.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVRFModelObjectList(const ModelObjectList& modelObjectList) {
      return setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ZoneTerminalUnitList, modelObjectList.handle());
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().addModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().removeModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeAllTerminals() {
      vrfModelObjectList().removeAllModelObjects();
    }

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::terminals() const {
      return subsetCastVector<ZoneHVACTerminalUnitVariableRefrigerantFlow>(vrfModelObjectList().modelObjects());
    }

  }  // namespace detail

  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(const Model& model)
    : StraightComponent(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>());

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
    ok = setMinimumOutdoorAirTemperatureinCoolingOnlyMode(-6.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorAirTemperatureinCoolingOnlyMode(43.0);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorAirTemperatureinHeatingOnlyMode(-20.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorAirTemperatureinHeatingOnlyMode(16.0);
    OS_ASSERT(ok);
    ok = setMinimumOutdoorTemperatureinHeatRecoveryMode(-20.0);
    OS_ASSERT(ok);
    ok = setMaximumOutdoorTemperatureinHeatRecoveryMode(43.0);
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
    ok = setOutdoorUnitEvaporatorReferenceSuperheating(3);
    OS_ASSERT(ok);
    ok = setOutdoorUnitCondenserReferenceSubcooling(5);
    OS_ASSERT(ok);
    ok = setOutdoorUnitEvaporatorRatedBypassFactor(0.4);
    OS_ASSERT(ok);
    ok = setOutdoorUnitCondenserRatedBypassFactor(0.2);
    OS_ASSERT(ok);
    ok = setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(5);
    OS_ASSERT(ok);
    ok = setOutdoorUnitHeatExchangerCapacityRatio(0.3);
    OS_ASSERT(ok);
    ok = setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(4.25E-3);
    OS_ASSERT(ok);
    ok = setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(7.50E-5);
    OS_ASSERT(ok);
    ok = setDiameterofMainPipeforSuctionGas(0.0762);
    OS_ASSERT(ok);
    ok = setDiameterofMainPipeforDischargeGas(0.0762);
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
    ok = setInitialHeatRecoveryCoolingCapacityFraction(1);
    OS_ASSERT(ok);
    ok = setHeatRecoveryCoolingCapacityTimeConstant(0);
    OS_ASSERT(ok);
    ok = setInitialHeatRecoveryCoolingEnergyFraction(1);
    OS_ASSERT(ok);
    ok = setHeatRecoveryCoolingEnergyTimeConstant(0);
    OS_ASSERT(ok);
    ok = setInitialHeatRecoveryHeatingCapacityFraction(1);
    OS_ASSERT(ok);
    ok = setHeatRecoveryHeatingCapacityTimeConstant(0);
    OS_ASSERT(ok);
    ok = setInitialHeatRecoveryHeatingEnergyFraction(1);
    OS_ASSERT(ok);
    ok = setHeatRecoveryHeatingEnergyTimeConstant(0);
    OS_ASSERT(ok);
    ok = setCompressorMaximumDeltaPressure(4500000.0);
    OS_ASSERT(ok);
    ok = setCompressorInverterEfficiency(0.95);
    OS_ASSERT(ok);
    ok = setCompressorEvaporativeCapacityCorrectionFactor(1.0);
    OS_ASSERT(ok);

    CurveQuadratic outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(model);
    outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient1Constant(0);
    outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient2x(6.05E-1);
    outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setCoefficient3xPOW2(2.50E-2);
    outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setMinimumValueofx(0);
    outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve.setMaximumValueofx(15);
    ok = setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
    OS_ASSERT(ok);

    CurveQuadratic outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(model);
    outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient1Constant(0);
    outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient2x(-2.91);
    outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setCoefficient3xPOW2(1.180);
    outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setMinimumValueofx(0);
    outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve.setMaximumValueofx(5);
    ok = setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
    OS_ASSERT(ok);

    //ok = setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
    //OS_ASSERT(ok);

    ModelObjectList vrfModelObjectList(model);
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setVRFModelObjectList(vrfModelObjectList);
    OS_ASSERT(ok);

    ModelObjectList loadingIndexList(model);
    ok = getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setLoadingIndexList(loadingIndexList);
    OS_ASSERT(ok);

    CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve1(model);
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
    addLoadingIndex(loadingIndex1);

    CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve2(model);
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
    addLoadingIndex(loadingIndex2);

    CurveBiquadratic evaporativeCapacityMultiplierFunctionofTemperatureCurve3(model);
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
    addLoadingIndex(loadingIndex3);
  }

  IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::refrigerantTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantType);
  }

  std::vector<std::string>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::refrigerantTemperatureControlAlgorithmforIndoorUnitValues() {
    return getIddKeyNames(
      IddFactory::instance().getObject(iddObjectType()).get(),
      OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostStrategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostStrategy);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostControl);
  }

  Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::availabilitySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->availabilitySchedule();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::refrigerantType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->refrigerantType();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->ratedEvaporativeCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isRatedEvaporativeCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isRatedEvaporativeCapacityAutosized();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedRatedEvaporativeCapacity() {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedRatedEvaporativeCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedCompressorPowerPerUnitofRatedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->ratedCompressorPowerPerUnitofRatedEvaporativeCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumOutdoorAirTemperatureinCoolingOnlyMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumOutdoorAirTemperatureinCoolingOnlyMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorAirTemperatureinCoolingOnlyMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->maximumOutdoorAirTemperatureinCoolingOnlyMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumOutdoorAirTemperatureinHeatingOnlyMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumOutdoorAirTemperatureinHeatingOnlyMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorAirTemperatureinHeatingOnlyMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->maximumOutdoorAirTemperatureinHeatingOnlyMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumOutdoorTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumOutdoorTemperatureinHeatRecoveryMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->maximumOutdoorTemperatureinHeatRecoveryMode();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::refrigerantTemperatureControlAlgorithmforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->refrigerantTemperatureControlAlgorithmforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::referenceEvaporatingTemperatureforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->referenceEvaporatingTemperatureforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::referenceCondensingTemperatureforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->referenceCondensingTemperatureforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::variableEvaporatingTemperatureMinimumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->variableEvaporatingTemperatureMinimumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::variableEvaporatingTemperatureMaximumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->variableEvaporatingTemperatureMaximumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::variableCondensingTemperatureMinimumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->variableCondensingTemperatureMinimumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::variableCondensingTemperatureMaximumforIndoorUnit() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->variableCondensingTemperatureMaximumforIndoorUnit();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitEvaporatorReferenceSuperheating() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->outdoorUnitEvaporatorReferenceSuperheating();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitCondenserReferenceSubcooling() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->outdoorUnitCondenserReferenceSubcooling();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitEvaporatorRatedBypassFactor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->outdoorUnitEvaporatorRatedBypassFactor();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitCondenserRatedBypassFactor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->outdoorUnitCondenserRatedBypassFactor();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::
    differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitHeatExchangerCapacityRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->outdoorUnitHeatExchangerCapacityRatio();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity();
  }

  Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  }

  Curve AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::diameterofMainPipeforSuctionGas() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->diameterofMainPipeforSuctionGas();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::diameterofMainPipeforDischargeGas() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->diameterofMainPipeforDischargeGas();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  }

  double
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heightDifferenceBetweenOutdoorUnitandIndoorUnits() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heightDifferenceBetweenOutdoorUnitandIndoorUnits();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::mainPipeInsulationThickness() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->mainPipeInsulationThickness();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::mainPipeInsulationThermalConductivity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->mainPipeInsulationThermalConductivity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::crankcaseHeaterPowerperCompressor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->crankcaseHeaterPowerperCompressor();
  }

  int AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::numberofCompressors() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->numberofCompressors();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratioofCompressorSizetoTotalCompressorCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->ratioofCompressorSizetoTotalCompressorCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorDryBulbTemperatureforCrankcaseHeater() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->maximumOutdoorDryBulbTemperatureforCrankcaseHeater();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostStrategy() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->defrostStrategy();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostControl() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->defrostControl();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostTimePeriodFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->defrostTimePeriodFraction();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isResistiveDefrostHeaterCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedResistiveDefrostHeaterCapacity() {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedResistiveDefrostHeaterCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->maximumOutdoorDrybulbTemperatureforDefrostOperation();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryCoolingCapacityFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryCoolingCapacityFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryCoolingCapacityTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryCoolingCapacityTimeConstant();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryCoolingEnergyFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryCoolingEnergyFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryCoolingEnergyTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryCoolingEnergyTimeConstant();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryHeatingCapacityFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryHeatingCapacityFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryHeatingCapacityTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryHeatingCapacityTimeConstant();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryHeatingEnergyFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryHeatingEnergyFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryHeatingEnergyTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryHeatingEnergyTimeConstant();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::compressorMaximumDeltaPressure() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->compressorMaximumDeltaPressure();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::compressorInverterEfficiency() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->compressorInverterEfficiency();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::compressorEvaporativeCapacityCorrectionFactor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->compressorEvaporativeCapacityCorrectionFactor();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRefrigerantType(const std::string& refrigerantType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRefrigerantType(refrigerantType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedEvaporativeCapacity(double ratedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRatedEvaporativeCapacity(
      ratedEvaporativeCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeRatedEvaporativeCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeRatedEvaporativeCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(
    double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumOutdoorAirTemperatureinCoolingOnlyMode(
    double minimumOutdoorAirTemperatureinCoolingOnlyMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumOutdoorAirTemperatureinCoolingOnlyMode(
      minimumOutdoorAirTemperatureinCoolingOnlyMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorAirTemperatureinCoolingOnlyMode(
    double maximumOutdoorAirTemperatureinCoolingOnlyMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMaximumOutdoorAirTemperatureinCoolingOnlyMode(
      maximumOutdoorAirTemperatureinCoolingOnlyMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumOutdoorAirTemperatureinHeatingOnlyMode(
    double minimumOutdoorAirTemperatureinHeatingOnlyMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumOutdoorAirTemperatureinHeatingOnlyMode(
      minimumOutdoorAirTemperatureinHeatingOnlyMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorAirTemperatureinHeatingOnlyMode(
    double maximumOutdoorAirTemperatureinHeatingOnlyMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMaximumOutdoorAirTemperatureinHeatingOnlyMode(
      maximumOutdoorAirTemperatureinHeatingOnlyMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumOutdoorTemperatureinHeatRecoveryMode(
    double minimumOutdoorTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumOutdoorTemperatureinHeatRecoveryMode(
      minimumOutdoorTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorTemperatureinHeatRecoveryMode(
    double maximumOutdoorTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMaximumOutdoorTemperatureinHeatRecoveryMode(
      maximumOutdoorTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRefrigerantTemperatureControlAlgorithmforIndoorUnit(
    const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setRefrigerantTemperatureControlAlgorithmforIndoorUnit(refrigerantTemperatureControlAlgorithmforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setReferenceEvaporatingTemperatureforIndoorUnit(
    double referenceEvaporatingTemperatureforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setReferenceEvaporatingTemperatureforIndoorUnit(
      referenceEvaporatingTemperatureforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setReferenceCondensingTemperatureforIndoorUnit(
    double referenceCondensingTemperatureforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setReferenceCondensingTemperatureforIndoorUnit(
      referenceCondensingTemperatureforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setVariableEvaporatingTemperatureMinimumforIndoorUnit(
    double variableEvaporatingTemperatureMinimumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setVariableEvaporatingTemperatureMinimumforIndoorUnit(variableEvaporatingTemperatureMinimumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setVariableEvaporatingTemperatureMaximumforIndoorUnit(
    double variableEvaporatingTemperatureMaximumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setVariableEvaporatingTemperatureMaximumforIndoorUnit(variableEvaporatingTemperatureMaximumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setVariableCondensingTemperatureMinimumforIndoorUnit(
    double variableCondensingTemperatureMinimumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setVariableCondensingTemperatureMinimumforIndoorUnit(variableCondensingTemperatureMinimumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setVariableCondensingTemperatureMaximumforIndoorUnit(
    double variableCondensingTemperatureMaximumforIndoorUnit) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setVariableCondensingTemperatureMaximumforIndoorUnit(variableCondensingTemperatureMaximumforIndoorUnit);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitEvaporatorReferenceSuperheating(
    double outdoorUnitEvaporatorReferenceSuperheating) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setOutdoorUnitEvaporatorReferenceSuperheating(
      outdoorUnitEvaporatorReferenceSuperheating);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitCondenserReferenceSubcooling(
    double outdoorUnitCondenserReferenceSubcooling) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setOutdoorUnitCondenserReferenceSubcooling(
      outdoorUnitCondenserReferenceSubcooling);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitEvaporatorRatedBypassFactor(
    double outdoorUnitEvaporatorRatedBypassFactor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setOutdoorUnitEvaporatorRatedBypassFactor(
      outdoorUnitEvaporatorRatedBypassFactor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitCondenserRatedBypassFactor(
    double outdoorUnitCondenserRatedBypassFactor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setOutdoorUnitCondenserRatedBypassFactor(
      outdoorUnitCondenserRatedBypassFactor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::
    setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(
      double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(
        differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitHeatExchangerCapacityRatio(
    double outdoorUnitHeatExchangerCapacityRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setOutdoorUnitHeatExchangerCapacityRatio(
      outdoorUnitHeatExchangerCapacityRatio);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(
    double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(
    double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDiameterofMainPipeforSuctionGas(double diameterofMainPipeforSuctionGas) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDiameterofMainPipeforSuctionGas(
      diameterofMainPipeforSuctionGas);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDiameterofMainPipeforDischargeGas(double diameterofMainPipeforDischargeGas) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDiameterofMainPipeforDischargeGas(
      diameterofMainPipeforDischargeGas);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
    double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
    double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(
    double heightDifferenceBetweenOutdoorUnitandIndoorUnits) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(heightDifferenceBetweenOutdoorUnitandIndoorUnits);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMainPipeInsulationThickness(double mainPipeInsulationThickness) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMainPipeInsulationThickness(
      mainPipeInsulationThickness);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMainPipeInsulationThermalConductivity(
    double mainPipeInsulationThermalConductivity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMainPipeInsulationThermalConductivity(
      mainPipeInsulationThermalConductivity);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCrankcaseHeaterPowerperCompressor(
      crankcaseHeaterPowerperCompressor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setNumberofCompressors(int numberofCompressors) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setNumberofCompressors(numberofCompressors);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatioofCompressorSizetoTotalCompressorCapacity(
    double ratioofCompressorSizetoTotalCompressorCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRatioofCompressorSizetoTotalCompressorCapacity(
      ratioofCompressorSizetoTotalCompressorCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeater) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(maximumOutdoorDryBulbTemperatureforCrankcaseHeater);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostStrategy(const std::string& defrostStrategy) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDefrostStrategy(defrostStrategy);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostControl(const std::string& defrostControl) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDefrostControl(defrostControl);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDefrostTimePeriodFraction(
      defrostTimePeriodFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setResistiveDefrostHeaterCapacity(
      resistiveDefrostHeaterCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeResistiveDefrostHeaterCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeResistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
    double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryCoolingCapacityFraction(
    double initialHeatRecoveryCoolingCapacityFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryCoolingCapacityFraction(
      initialHeatRecoveryCoolingCapacityFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryCoolingCapacityTimeConstant(
    double heatRecoveryCoolingCapacityTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryCoolingCapacityTimeConstant(
      heatRecoveryCoolingCapacityTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryCoolingEnergyFraction(
    double initialHeatRecoveryCoolingEnergyFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryCoolingEnergyFraction(
      initialHeatRecoveryCoolingEnergyFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryCoolingEnergyTimeConstant(
    double heatRecoveryCoolingEnergyTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryCoolingEnergyTimeConstant(
      heatRecoveryCoolingEnergyTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryHeatingCapacityFraction(
    double initialHeatRecoveryHeatingCapacityFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryHeatingCapacityFraction(
      initialHeatRecoveryHeatingCapacityFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryHeatingCapacityTimeConstant(
    double heatRecoveryHeatingCapacityTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryHeatingCapacityTimeConstant(
      heatRecoveryHeatingCapacityTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryHeatingEnergyFraction(
    double initialHeatRecoveryHeatingEnergyFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryHeatingEnergyFraction(
      initialHeatRecoveryHeatingEnergyFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryHeatingEnergyTimeConstant(
    double heatRecoveryHeatingEnergyTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryHeatingEnergyTimeConstant(
      heatRecoveryHeatingEnergyTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCompressorMaximumDeltaPressure(
      compressorMaximumDeltaPressure);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCompressorInverterEfficiency(double compressorInverterEfficiency) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCompressorInverterEfficiency(
      compressorInverterEfficiency);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCompressorEvaporativeCapacityCorrectionFactor(
    double compressorEvaporativeCapacityCorrectionFactor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCompressorEvaporativeCapacityCorrectionFactor(
      compressorEvaporativeCapacityCorrectionFactor);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->addTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->removeTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::removeAllTerminals() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->removeAllTerminals();
  }

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::terminals() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->terminals();
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::addLoadingIndex(LoadingIndex& loadingIndex) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->addLoadingIndex(loadingIndex);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::removeLoadingIndex(LoadingIndex& loadingIndex) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->removeLoadingIndex(loadingIndex);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::removeAllLoadingIndexes() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->removeAllLoadingIndexes();
  }

  std::vector<LoadingIndex> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::loadingIndexes() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->loadingIndexes();
  }

  /// @cond
  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(
    std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
