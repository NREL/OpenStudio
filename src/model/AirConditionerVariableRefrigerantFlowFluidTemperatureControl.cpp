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

#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ModelObjectLists.hpp"
#include "ModelObjectLists_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::iddObjectType() const {
      return AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule);
    }

    ModelObjectLists AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::zoneTerminalUnitList() const {
      boost::optional<ModelObjectLists> value = optionalZoneTerminalUnitList();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Zone Terminal Unit List attached.");
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
      return getAutosizedValue("TODO_CHECK_SQL Rated Evaporative Capacity", "W");
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

    UnivariateFunctions
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
      boost::optional<UnivariateFunctions> value = optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Unit Evaporating Temperature Functionof Superheating Curve attached.");
      }
      return value.get();
    }

    UnivariateFunctions
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      boost::optional<UnivariateFunctions> value = optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Unit Condensing Temperature Functionof Subcooling Curve attached.");
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

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isNumberofCompressorsDefaulted() const {
      return isEmpty(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors);
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

    boost::optional<BivariateFunctions>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
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
      return getAutosizedValue("TODO_CHECK_SQL Resistive Defrost Heater Capacity", "W");
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::compressormaximumdeltaPressure() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressormaximumdeltaPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    int AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::numberofCompressorLoadingIndexEntries() const {
      boost::optional<int> value =
        getInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressorLoadingIndexEntries, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule", schedule);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setZoneTerminalUnitList(const ModelObjectLists& modelObjectLists) {
      bool result =
        setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList, modelObjectLists.handle());
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
      const UnivariateFunctions& univariateFunctions) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName,
        univariateFunctions.handle());
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(
      const UnivariateFunctions& univariateFunctions) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
        univariateFunctions.handle());
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

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetNumberofCompressors() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, "");
      OS_ASSERT(result);
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
      const BivariateFunctions& bivariateFunctions) {
      bool result = setPointer(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
        bivariateFunctions.handle());
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

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCompressormaximumdeltaPressure(double compressormaximumdeltaPressure) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressormaximumdeltaPressure,
                              compressormaximumdeltaPressure);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setNumberofCompressorLoadingIndexEntries(
      int numberofCompressorLoadingIndexEntries) {
      bool result = setInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressorLoadingIndexEntries,
                           numberofCompressorLoadingIndexEntries);
      return result;
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

    boost::optional<ModelObjectLists> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalZoneTerminalUnitList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectLists>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList);
    }

    boost::optional<UnivariateFunctions>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve()
        const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName);
    }

    boost::optional<UnivariateFunctions>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName);
    }

  }  // namespace detail

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl(const Model& model)
    : StraightComponent(AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    //     OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList
    //     OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName
    //     OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName
    bool ok = true;
    // ok = setZoneTerminalUnitList();
    OS_ASSERT(ok);
    // ok = setRefrigerantType();
    OS_ASSERT(ok);
    // ok = setRatedEvaporativeCapacity();
    OS_ASSERT(ok);
    // ok = setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity();
    OS_ASSERT(ok);
    // setMinimumOutdoorAirTemperatureinCoolingMode();
    // setMaximumOutdoorAirTemperatureinCoolingMode();
    // setMinimumOutdoorAirTemperatureinHeatingMode();
    // setMaximumOutdoorAirTemperatureinHeatingMode();
    // setReferenceOutdoorUnitSuperheating();
    // setReferenceOutdoorUnitSubcooling();
    // ok = setRefrigerantTemperatureControlAlgorithmforIndoorUnit();
    OS_ASSERT(ok);
    // setReferenceEvaporatingTemperatureforIndoorUnit();
    // setReferenceCondensingTemperatureforIndoorUnit();
    // setVariableEvaporatingTemperatureMinimumforIndoorUnit();
    // setVariableEvaporatingTemperatureMaximumforIndoorUnit();
    // setVariableCondensingTemperatureMinimumforIndoorUnit();
    // setVariableCondensingTemperatureMaximumforIndoorUnit();
    // ok = setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity();
    OS_ASSERT(ok);
    // ok = setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity();
    OS_ASSERT(ok);
    // ok = setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
    OS_ASSERT(ok);
    // ok = setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
    OS_ASSERT(ok);
    // ok = setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
    OS_ASSERT(ok);
    // ok = setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
    OS_ASSERT(ok);
    // ok = setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
    OS_ASSERT(ok);
    // setHeightDifferenceBetweenOutdoorUnitandIndoorUnits();
    // ok = setMainPipeInsulationThickness();
    OS_ASSERT(ok);
    // ok = setMainPipeInsulationThermalConductivity();
    OS_ASSERT(ok);
    // setCrankcaseHeaterPowerperCompressor();
    // setRatioofCompressorSizetoTotalCompressorCapacity();
    // setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater();
    // ok = setDefrostStrategy();
    OS_ASSERT(ok);
    // ok = setDefrostControl();
    OS_ASSERT(ok);
    // ok = setDefrostTimePeriodFraction();
    OS_ASSERT(ok);
    // ok = setResistiveDefrostHeaterCapacity();
    OS_ASSERT(ok);
    // setMaximumOutdoorDrybulbTemperatureforDefrostOperation();
    // ok = setCompressormaximumdeltaPressure();
    OS_ASSERT(ok);
    // ok = setNumberofCompressorLoadingIndexEntries();
    OS_ASSERT(ok);
  }

  IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::refrigerantTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::refrigerantTemperatureControlAlgorithmforIndoorUnitValues() {
    return getIddKeyNames(
      IddFactory::instance().getObject(iddObjectType()).get(),
      OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostStrategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl);
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::availabilitySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->availabilitySchedule();
  }

  ModelObjectLists AirConditionerVariableRefrigerantFlowFluidTemperatureControl::zoneTerminalUnitList() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->zoneTerminalUnitList();
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

  UnivariateFunctions
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  }

  UnivariateFunctions
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const {
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

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isNumberofCompressorsDefaulted() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isNumberofCompressorsDefaulted();
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

  boost::optional<BivariateFunctions>
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

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::compressormaximumdeltaPressure() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->compressormaximumdeltaPressure();
  }

  int AirConditionerVariableRefrigerantFlowFluidTemperatureControl::numberofCompressorLoadingIndexEntries() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->numberofCompressorLoadingIndexEntries();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setAvailabilitySchedule(schedule);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetAvailabilitySchedule() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetAvailabilitySchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setZoneTerminalUnitList(const ModelObjectLists& modelObjectLists) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setZoneTerminalUnitList(modelObjectLists);
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
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(univariateFunctions);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(univariateFunctions);
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

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetNumberofCompressors() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetNumberofCompressors();
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

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(bivariateFunctions);
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

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCompressormaximumdeltaPressure(double compressormaximumdeltaPressure) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCompressormaximumdeltaPressure(
      compressormaximumdeltaPressure);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setNumberofCompressorLoadingIndexEntries(
    int numberofCompressorLoadingIndexEntries) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setNumberofCompressorLoadingIndexEntries(
      numberofCompressorLoadingIndexEntries);
  }

  /// @cond
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl(
    std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
