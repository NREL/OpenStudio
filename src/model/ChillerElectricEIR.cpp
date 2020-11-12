/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Model.hpp"
#include "ChillerElectricEIR.hpp"
#include "ChillerElectricEIR_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Chiller_Electric_EIR_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ChillerElectricEIR_Impl::ChillerElectricEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ChillerElectricEIR::iddObjectType());
    }

    ChillerElectricEIR_Impl::ChillerElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ChillerElectricEIR::iddObjectType());
    }

    ChillerElectricEIR_Impl::ChillerElectricEIR_Impl(const ChillerElectricEIR_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ChillerElectricEIR_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Chiller Electricity Rate",
                                                   "Chiller Electricity Energy",
                                                   "Chiller Evaporator Cooling Rate",
                                                   "Chiller Evaporator Cooling Energy",
                                                   "Chiller Evaporator Inlet Temperature",
                                                   "Chiller Evaporator Outlet Temperature",
                                                   "Chiller Evaporator Mass Flow Rate",
                                                   "Chiller COP",
                                                   "Chiller Condenser Heat Transfer Rate",
                                                   "Chiller Condenser Heat Transfer Energy",
                                                   "Chiller Part Load Ratio",
                                                   "Chiller Cycling Ratio",
                                                   "Chiller False Load Heat Transfer Rate",
                                                   "Chiller False Load Heat Transfer Energy"};
      return result;
    }

    IddObjectType ChillerElectricEIR_Impl::iddObjectType() const {
      return ChillerElectricEIR::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ChillerElectricEIR_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ChillerElectricEIR", "Basin Heater Operating"));
      } else if (std::find(b, e, OS_Chiller_Electric_EIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ChillerElectricEIR", "Heat Recovery Inlet High Temperature Limit"));
      }
      return result;
    }

    boost::optional<double> ChillerElectricEIR_Impl::referenceCapacity() const {
      return getDouble(OS_Chiller_Electric_EIRFields::ReferenceCapacity, true);
    }

    bool ChillerElectricEIR_Impl::isReferenceCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ReferenceCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double ChillerElectricEIR_Impl::referenceCOP() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::ReferenceCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerElectricEIR_Impl::referenceLeavingChilledWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isReferenceLeavingChilledWaterTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature);
    }

    double ChillerElectricEIR_Impl::referenceEnteringCondenserFluidTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isReferenceEnteringCondenserFluidTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature);
    }

    boost::optional<double> ChillerElectricEIR_Impl::referenceChilledWaterFlowRate() const {
      return getDouble(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, true);
    }

    bool ChillerElectricEIR_Impl::isReferenceChilledWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ChillerElectricEIR_Impl::referenceCondenserFluidFlowRate() const {
      return getDouble(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, true);
    }

    bool ChillerElectricEIR_Impl::isReferenceCondenserFluidFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    CurveBiquadratic ChillerElectricEIR_Impl::coolingCapacityFunctionOfTemperature() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName).get();
      return wo.optionalCast<CurveBiquadratic>().get();
    }

    CurveBiquadratic ChillerElectricEIR_Impl::electricInputToCoolingOutputRatioFunctionOfTemperature() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName).get();
      return wo.optionalCast<CurveBiquadratic>().get();
    }

    CurveQuadratic ChillerElectricEIR_Impl::electricInputToCoolingOutputRatioFunctionOfPLR() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName).get();
      return wo.optionalCast<CurveQuadratic>().get();
    }

    double ChillerElectricEIR_Impl::minimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isMinimumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio);
    }

    double ChillerElectricEIR_Impl::maximumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isMaximumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio);
    }

    double ChillerElectricEIR_Impl::optimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isOptimumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio);
    }

    double ChillerElectricEIR_Impl::minimumUnloadingRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isMinimumUnloadingRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio);
    }

    std::string ChillerElectricEIR_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isCondenserTypeDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::CondenserType);
    }

    double ChillerElectricEIR_Impl::condenserFanPowerRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isCondenserFanPowerRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio);
    }

    double ChillerElectricEIR_Impl::fractionofCompressorElectricConsumptionRejectedbyCondenser() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser);
    }

    double ChillerElectricEIR_Impl::leavingChilledWaterLowerTemperatureLimit() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isLeavingChilledWaterLowerTemperatureLimitDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit);
    }

    std::string ChillerElectricEIR_Impl::chillerFlowMode() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ChillerFlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isChillerFlowModeDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::ChillerFlowMode);
    }

    boost::optional<double> ChillerElectricEIR_Impl::designHeatRecoveryWaterFlowRate() const {
      return getDouble(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, true);
    }

    bool ChillerElectricEIR_Impl::isDesignHeatRecoveryWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double ChillerElectricEIR_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isSizingFactorDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::SizingFactor);
    }

    double ChillerElectricEIR_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isBasinHeaterCapacityDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::BasinHeaterCapacity);
    }

    double ChillerElectricEIR_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::isBasinHeaterSetpointTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_EIRFields::BasinHeaterSetpointTemperature);
    }

    bool ChillerElectricEIR_Impl::setReferenceCapacity(boost::optional<double> referenceCapacity) {
      bool result = false;
      if (referenceCapacity) {
        result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceCapacity, referenceCapacity.get());
      }
      return result;
    }

    void ChillerElectricEIR_Impl::autosizeReferenceCapacity() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setReferenceCapacity(double referenceCapacity) {
      return setDouble(OS_Chiller_Electric_EIRFields::ReferenceCapacity, referenceCapacity);
    }

    bool ChillerElectricEIR_Impl::setReferenceCOP(double referenceCOP) {
      return setDouble(OS_Chiller_Electric_EIRFields::ReferenceCOP, referenceCOP);
    }

    bool ChillerElectricEIR_Impl::setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature) {
      bool result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature, referenceLeavingChilledWaterTemperature);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricEIR_Impl::resetReferenceLeavingChilledWaterTemperature() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature) {
      bool result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature, referenceEnteringCondenserFluidTemperature);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricEIR_Impl::resetReferenceEnteringCondenserFluidTemperature() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate) {
      bool result = false;
      if (referenceChilledWaterFlowRate) {
        result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, referenceChilledWaterFlowRate.get());
      }
      return result;
    }

    void ChillerElectricEIR_Impl::resetReferenceChilledWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, "");
      OS_ASSERT(result);
    }

    void ChillerElectricEIR_Impl::autosizeReferenceChilledWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate, referenceChilledWaterFlowRate);
      return result;
    }

    bool ChillerElectricEIR_Impl::setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate) {
      bool result = false;
      if (referenceCondenserFluidFlowRate) {
        result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, referenceCondenserFluidFlowRate.get());
      }
      return result;
    }

    void ChillerElectricEIR_Impl::resetReferenceCondenserFluidFlowRate() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, "");
      OS_ASSERT(result);
    }

    void ChillerElectricEIR_Impl::autosizeReferenceCondenserFluidFlowRate() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, referenceCondenserFluidFlowRate);
      return result;
    }

    bool ChillerElectricEIR_Impl::setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    }

    bool ChillerElectricEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, curve.handle());
    }

    bool ChillerElectricEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, curve.handle());
    }

    bool ChillerElectricEIR_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
      return setDouble(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    }

    void ChillerElectricEIR_Impl::resetMinimumPartLoadRatio() {
      bool result = setString(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
      return setDouble(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    }

    void ChillerElectricEIR_Impl::resetMaximumPartLoadRatio() {
      bool result = setString(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
      return setDouble(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio, optimumPartLoadRatio);
    }

    void ChillerElectricEIR_Impl::resetOptimumPartLoadRatio() {
      bool result = setString(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setMinimumUnloadingRatio(double minimumUnloadingRatio) {
      return setDouble(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio, minimumUnloadingRatio);
    }

    void ChillerElectricEIR_Impl::resetMinimumUnloadingRatio() {
      bool result = setString(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setCondenserType(std::string condenserType) {
      bool ok = false;
      if ((istringEqual("AirCooled", condenserType) || istringEqual("EvaporativelyCooled", condenserType)) && (this->secondaryPlantLoop())) {
        LOG(Warn,
            "Cannot set condenserType to AirCooled or EvaporativelyCooled, chiller '" << this->name() << "' is connected to a secondaryPlantLoop");
      } else if (istringEqual("WaterCooled", condenserType) && !(this->secondaryPlantLoop())) {
        LOG(Warn, "Cannot set condenserType to 'WaterCooled', chiller '" << this->name() << "' is not connected to a secondaryPlantLoop");
      } else {
        ok = setString(OS_Chiller_Electric_EIRFields::CondenserType, condenserType);
      }

      return ok;
    }

    void ChillerElectricEIR_Impl::resetCondenserType() {
      bool result = setString(OS_Chiller_Electric_EIRFields::CondenserType, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setCondenserFanPowerRatio(double condenserFanPowerRatio) {
      return setDouble(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio, condenserFanPowerRatio);
    }

    void ChillerElectricEIR_Impl::resetCondenserFanPowerRatio() {
      bool result = setString(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setFractionofCompressorElectricConsumptionRejectedbyCondenser(
      double fractionofCompressorElectricConsumptionRejectedbyCondenser) {
      return setDouble(OS_Chiller_Electric_EIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser,
                       fractionofCompressorElectricConsumptionRejectedbyCondenser);
    }

    void ChillerElectricEIR_Impl::resetFractionofCompressorElectricConsumptionRejectedbyCondenser() {
      bool result = setString(OS_Chiller_Electric_EIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
      bool result = setDouble(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit, leavingChilledWaterLowerTemperatureLimit);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricEIR_Impl::resetLeavingChilledWaterLowerTemperatureLimit() {
      bool result = setString(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setChillerFlowMode(std::string chillerFlowMode) {
      bool result = false;
      if (istringEqual(chillerFlowMode, "VariableFlow")) {
        // Support legacy key
        result = setString(OS_Chiller_Electric_EIRFields::ChillerFlowMode, "LeavingSetpointModulated");
      } else {
        result = setString(OS_Chiller_Electric_EIRFields::ChillerFlowMode, chillerFlowMode);
      }

      return result;
    }

    void ChillerElectricEIR_Impl::resetChillerFlowMode() {
      bool result = setString(OS_Chiller_Electric_EIRFields::ChillerFlowMode, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
      return setDouble(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, designHeatRecoveryWaterFlowRate);
    }

    void ChillerElectricEIR_Impl::autosizeDesignHeatRecoveryWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setSizingFactor(double sizingFactor) {
      return setDouble(OS_Chiller_Electric_EIRFields::SizingFactor, sizingFactor);
    }

    void ChillerElectricEIR_Impl::resetSizingFactor() {
      bool result = setString(OS_Chiller_Electric_EIRFields::SizingFactor, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      return setDouble(OS_Chiller_Electric_EIRFields::BasinHeaterCapacity, basinHeaterCapacity);
    }

    void ChillerElectricEIR_Impl::resetBasinHeaterCapacity() {
      bool result = setString(OS_Chiller_Electric_EIRFields::BasinHeaterCapacity, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricEIR_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      return setDouble(OS_Chiller_Electric_EIRFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    }

    void ChillerElectricEIR_Impl::resetBasinHeaterSetpointTemperature() {
      bool result = setString(OS_Chiller_Electric_EIRFields::BasinHeaterSetpointTemperature, "");
      OS_ASSERT(result);
    }

    unsigned ChillerElectricEIR_Impl::supplyInletPort() const {
      return OS_Chiller_Electric_EIRFields::ChilledWaterInletNodeName;
    }

    unsigned ChillerElectricEIR_Impl::supplyOutletPort() const {
      return OS_Chiller_Electric_EIRFields::ChilledWaterOutletNodeName;
    }

    unsigned ChillerElectricEIR_Impl::demandInletPort() const {
      return OS_Chiller_Electric_EIRFields::CondenserInletNodeName;
    }

    unsigned ChillerElectricEIR_Impl::demandOutletPort() const {
      return OS_Chiller_Electric_EIRFields::CondenserOutletNodeName;
    }

    unsigned ChillerElectricEIR_Impl::tertiaryInletPort() const {
      return OS_Chiller_Electric_EIRFields::HeatRecoveryInletNodeName;
    }

    unsigned ChillerElectricEIR_Impl::tertiaryOutletPort() const {
      return OS_Chiller_Electric_EIRFields::HeatRecoveryOutletNodeName;
    }

    ModelObject ChillerElectricEIR_Impl::clone(Model model) const {
      ChillerElectricEIR chiller = WaterToWaterComponent_Impl::clone(model).cast<ChillerElectricEIR>();

      return chiller;
    }

    bool ChillerElectricEIR_Impl::setBasinHeaterSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName, "ChillerElectricEIR", "Basin Heater Operating", schedule);
      return result;
    }

    void ChillerElectricEIR_Impl::resetBasinHeaterSchedule() {
      bool result = setString(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> ChillerElectricEIR_Impl::basinHeaterSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName);
    }

    std::vector<ModelObject> ChillerElectricEIR_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(coolingCapacityFunctionOfTemperature());
      result.push_back(electricInputToCoolingOutputRatioFunctionOfTemperature());
      result.push_back(electricInputToCoolingOutputRatioFunctionOfPLR());

      return result;
    }

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> ChillerElectricEIR_Impl::chilledWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> ChillerElectricEIR_Impl::condenserWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    /** Convenience Function to return the Heat Recovery Loop **/
    boost::optional<PlantLoop> ChillerElectricEIR_Impl::heatRecoveryLoop() const {
      return WaterToWaterComponent_Impl::tertiaryPlantLoop();
    }

    bool ChillerElectricEIR_Impl::addToNode(Node& node) {

      boost::optional<PlantLoop> t_plantLoop = node.plantLoop();

      // If trying to add to a node that is on the demand side of a plant loop
      if (t_plantLoop) {
        if (t_plantLoop->demandComponent(node.handle())) {
          // If there is already a condenser water Plant Loop
          if (boost::optional<PlantLoop> cndLoop = this->condenserWaterLoop()) {
            // And it's not the same as the node's loop
            if (t_plantLoop.get() != cndLoop.get()) {
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
      // Connect the component
      bool ok = WaterToWaterComponent_Impl::addToNode(node);

      // If there's a secondary plant loop, switch the condenser type to "WaterCooled"
      if (this->condenserWaterLoop()) {
        this->setCondenserType("WaterCooled");
      }
      return ok;
    }

    bool ChillerElectricEIR_Impl::addToTertiaryNode(Node& node) {
      auto _model = node.model();
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

    bool ChillerElectricEIR_Impl::removeFromSecondaryPlantLoop() {
      // Disconnect the component
      bool ok = WaterToWaterComponent_Impl::removeFromSecondaryPlantLoop();

      // Switch the condenser type to "AirCooled"
      this->setCondenserType("AirCooled");
      return ok;
    }

    boost::optional<ModelObject> ChillerElectricEIR_Impl::basinHeaterScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = basinHeaterSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool ChillerElectricEIR_Impl::setBasinHeaterScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setBasinHeaterSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetBasinHeaterSchedule();
      }
      return true;
    }

    boost::optional<double> ChillerElectricEIR_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Reference Capacity", "W");
    }

    boost::optional<double> ChillerElectricEIR_Impl::autosizedReferenceChilledWaterFlowRate() const {
      return getAutosizedValue("Design Size Reference Chilled Water Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricEIR_Impl::autosizedReferenceCondenserFluidFlowRate() const {
      return getAutosizedValue("Design Size Reference Condenser Fluid Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricEIR_Impl::autosizedDesignHeatRecoveryWaterFlowRate() const {
      return getAutosizedValue("Design Size Heat Recovery Water Flow Rate", "m3/s");
    }

    void ChillerElectricEIR_Impl::autosize() {
      autosizeReferenceCapacity();
      autosizeReferenceChilledWaterFlowRate();
      autosizeReferenceCondenserFluidFlowRate();
      autosizeDesignHeatRecoveryWaterFlowRate();
    }

    void ChillerElectricEIR_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedReferenceCapacity();
      if (val) {
        setReferenceCapacity(val.get());
      }

      val = autosizedReferenceChilledWaterFlowRate();
      if (val) {
        setReferenceChilledWaterFlowRate(val.get());
      }

      val = autosizedReferenceCondenserFluidFlowRate();
      if (val) {
        setReferenceCondenserFluidFlowRate(val.get());
      }

      val = autosizedDesignHeatRecoveryWaterFlowRate();
      if (val) {
        setDesignHeatRecoveryWaterFlowRate(val.get());
      }
    }

    std::string ChillerElectricEIR_Impl::endUseSubcategory() const {
      auto value = getString(OS_Chiller_Electric_EIRFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_Chiller_Electric_EIRFields::EndUseSubcategory, endUseSubcategory);
    }

    double ChillerElectricEIR_Impl::condenserHeatRecoveryRelativeCapacityFraction() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::CondenserHeatRecoveryRelativeCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricEIR_Impl::setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction) {
      bool result =
        setDouble(OS_Chiller_Electric_EIRFields::CondenserHeatRecoveryRelativeCapacityFraction, condenserHeatRecoveryRelativeCapacityFraction);
      // OS_ASSERT(result);
      return result;
    }

    boost::optional<Schedule> ChillerElectricEIR_Impl::heatRecoveryInletHighTemperatureLimitSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName);
    }

    // TODO: ScheduleTypeLimits
    bool ChillerElectricEIR_Impl::setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Chiller_Electric_EIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, "ChillerElectricEIR",
                                "Heat Recovery Inlet High Temperature Limit", schedule);
      return result;
    }

    void ChillerElectricEIR_Impl::resetHeatRecoveryInletHighTemperatureLimitSchedule() {
      bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Node> ChillerElectricEIR_Impl::heatRecoveryLeavingTemperatureSetpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Chiller_Electric_EIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName);
    }

    bool ChillerElectricEIR_Impl::setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node) {
      return setPointer(OS_Chiller_Electric_EIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, node.handle());
    }

    void ChillerElectricEIR_Impl::resetHeatRecoveryLeavingTemperatureSetpointNode() {
      bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ChillerElectricEIR::ChillerElectricEIR(const Model& model, const CurveBiquadratic& CCFofT, const CurveBiquadratic& EItoCORFofT,
                                         const CurveQuadratic& EItoCORFofPLR)
    : WaterToWaterComponent(ChillerElectricEIR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricEIR_Impl>());

    OS_ASSERT(setCoolingCapacityFunctionOfTemperature(CCFofT));
    OS_ASSERT(setElectricInputToCoolingOutputRatioFunctionOfTemperature(EItoCORFofT));
    OS_ASSERT(setElectricInputToCoolingOutputRatioFunctionOfPLR(EItoCORFofPLR));
    OS_ASSERT(setReferenceCOP(5.5f));

    autosizeReferenceCapacity();
    autosizeReferenceChilledWaterFlowRate();
    // autosizeReferenceCondenserFluidFlowRate();
    autosizeDesignHeatRecoveryWaterFlowRate();

    setSizingFactor(1.0);

    setBasinHeaterCapacity(0.0);

    setBasinHeaterSetpointTemperature(10.0);

    resetBasinHeaterSchedule();

    setCondenserHeatRecoveryRelativeCapacityFraction(1.0);
    resetHeatRecoveryLeavingTemperatureSetpointNode();
    resetHeatRecoveryInletHighTemperatureLimitSchedule();

    setEndUseSubcategory("General");
  }

  ChillerElectricEIR::ChillerElectricEIR(const Model& model) : WaterToWaterComponent(ChillerElectricEIR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricEIR_Impl>());

    CurveBiquadratic ccFofT(model);
    ccFofT.setCoefficient1Constant(1.0215158);
    ccFofT.setCoefficient2x(0.037035864);
    ccFofT.setCoefficient3xPOW2(0.0002332476);
    ccFofT.setCoefficient4y(-0.003894048);
    ccFofT.setCoefficient5yPOW2(-6.52536e-005);
    ccFofT.setCoefficient6xTIMESY(-0.0002680452);
    ccFofT.setMinimumValueofx(5.0);
    ccFofT.setMaximumValueofx(10.0);
    ccFofT.setMinimumValueofy(24.0);
    ccFofT.setMaximumValueofy(35.0);

    CurveBiquadratic eirToCorfOfT(model);
    eirToCorfOfT.setCoefficient1Constant(0.70176857);
    eirToCorfOfT.setCoefficient2x(-0.00452016);
    eirToCorfOfT.setCoefficient3xPOW2(0.0005331096);
    eirToCorfOfT.setCoefficient4y(-0.005498208);
    eirToCorfOfT.setCoefficient5yPOW2(0.0005445792);
    eirToCorfOfT.setCoefficient6xTIMESY(-0.0007290324);
    eirToCorfOfT.setMinimumValueofx(5.0);
    eirToCorfOfT.setMaximumValueofx(10.0);
    eirToCorfOfT.setMinimumValueofy(24.0);
    eirToCorfOfT.setMaximumValueofy(35.0);

    CurveQuadratic eirToCorfOfPlr(model);
    eirToCorfOfPlr.setCoefficient1Constant(0.06369119);
    eirToCorfOfPlr.setCoefficient2x(0.58488832);
    eirToCorfOfPlr.setCoefficient3xPOW2(0.35280274);
    eirToCorfOfPlr.setMinimumValueofx(0.0);
    eirToCorfOfPlr.setMaximumValueofx(1.0);

    setCoolingCapacityFunctionOfTemperature(ccFofT);
    setElectricInputToCoolingOutputRatioFunctionOfTemperature(eirToCorfOfT);
    setElectricInputToCoolingOutputRatioFunctionOfPLR(eirToCorfOfPlr);

    OS_ASSERT(setReferenceCOP(5.5f));

    autosizeReferenceCapacity();
    autosizeReferenceChilledWaterFlowRate();
    // autosizeReferenceCondenserFluidFlowRate();
    autosizeDesignHeatRecoveryWaterFlowRate();

    setSizingFactor(1.0);
    setBasinHeaterCapacity(0.0);
    setBasinHeaterSetpointTemperature(10.0);
    resetBasinHeaterSchedule();
    setCondenserHeatRecoveryRelativeCapacityFraction(1.0);
    resetHeatRecoveryLeavingTemperatureSetpointNode();
    resetHeatRecoveryInletHighTemperatureLimitSchedule();

    setEndUseSubcategory("General");
  }

  IddObjectType ChillerElectricEIR::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Chiller_Electric_EIR);
    return result;
  }

  std::vector<std::string> ChillerElectricEIR::validCondenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_EIRFields::CondenserType);
  }

  std::vector<std::string> ChillerElectricEIR::validChillerFlowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_EIRFields::ChillerFlowMode);
  }

  boost::optional<double> ChillerElectricEIR::referenceCapacity() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->referenceCapacity();
  }

  bool ChillerElectricEIR::isReferenceCapacityAutosized() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isReferenceCapacityAutosized();
  }

  double ChillerElectricEIR::referenceCOP() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->referenceCOP();
  }

  double ChillerElectricEIR::referenceLeavingChilledWaterTemperature() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->referenceLeavingChilledWaterTemperature();
  }

  bool ChillerElectricEIR::isReferenceLeavingChilledWaterTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isReferenceLeavingChilledWaterTemperatureDefaulted();
  }

  double ChillerElectricEIR::referenceEnteringCondenserFluidTemperature() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->referenceEnteringCondenserFluidTemperature();
  }

  bool ChillerElectricEIR::isReferenceEnteringCondenserFluidTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isReferenceEnteringCondenserFluidTemperatureDefaulted();
  }

  boost::optional<double> ChillerElectricEIR::referenceChilledWaterFlowRate() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->referenceChilledWaterFlowRate();
  }

  bool ChillerElectricEIR::isReferenceChilledWaterFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isReferenceChilledWaterFlowRateAutosized();
  }

  boost::optional<double> ChillerElectricEIR::referenceCondenserFluidFlowRate() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->referenceCondenserFluidFlowRate();
  }

  bool ChillerElectricEIR::isReferenceCondenserFluidFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isReferenceCondenserFluidFlowRateAutosized();
  }

  bool ChillerElectricEIR::setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& curve) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setCoolingCapacityFunctionOfTemperature(curve);
  }

  bool ChillerElectricEIR::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& curve) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfTemperature(curve);
  }

  bool ChillerElectricEIR::setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& curve) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfPLR(curve);
  }

  CurveBiquadratic ChillerElectricEIR::coolingCapacityFunctionOfTemperature() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->coolingCapacityFunctionOfTemperature();
  }

  CurveBiquadratic ChillerElectricEIR::electricInputToCoolingOutputRatioFunctionOfTemperature() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfTemperature();
  }

  CurveQuadratic ChillerElectricEIR::electricInputToCoolingOutputRatioFunctionOfPLR() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfPLR();
  }

  double ChillerElectricEIR::minimumPartLoadRatio() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->minimumPartLoadRatio();
  }

  bool ChillerElectricEIR::isMinimumPartLoadRatioDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isMinimumPartLoadRatioDefaulted();
  }

  double ChillerElectricEIR::maximumPartLoadRatio() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->maximumPartLoadRatio();
  }

  bool ChillerElectricEIR::isMaximumPartLoadRatioDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isMaximumPartLoadRatioDefaulted();
  }

  double ChillerElectricEIR::optimumPartLoadRatio() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->optimumPartLoadRatio();
  }

  bool ChillerElectricEIR::isOptimumPartLoadRatioDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isOptimumPartLoadRatioDefaulted();
  }

  double ChillerElectricEIR::minimumUnloadingRatio() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->minimumUnloadingRatio();
  }

  bool ChillerElectricEIR::isMinimumUnloadingRatioDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isMinimumUnloadingRatioDefaulted();
  }

  std::string ChillerElectricEIR::condenserType() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->condenserType();
  }

  bool ChillerElectricEIR::isCondenserTypeDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isCondenserTypeDefaulted();
  }

  double ChillerElectricEIR::condenserFanPowerRatio() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->condenserFanPowerRatio();
  }

  bool ChillerElectricEIR::isCondenserFanPowerRatioDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isCondenserFanPowerRatioDefaulted();
  }

  double ChillerElectricEIR::fractionofCompressorElectricConsumptionRejectedbyCondenser() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->fractionofCompressorElectricConsumptionRejectedbyCondenser();
  }

  bool ChillerElectricEIR::isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted();
  }

  double ChillerElectricEIR::leavingChilledWaterLowerTemperatureLimit() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->leavingChilledWaterLowerTemperatureLimit();
  }

  bool ChillerElectricEIR::isLeavingChilledWaterLowerTemperatureLimitDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isLeavingChilledWaterLowerTemperatureLimitDefaulted();
  }

  std::string ChillerElectricEIR::chillerFlowMode() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->chillerFlowMode();
  }

  bool ChillerElectricEIR::isChillerFlowModeDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isChillerFlowModeDefaulted();
  }

  boost::optional<double> ChillerElectricEIR::designHeatRecoveryWaterFlowRate() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->designHeatRecoveryWaterFlowRate();
  }

  bool ChillerElectricEIR::isDesignHeatRecoveryWaterFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isDesignHeatRecoveryWaterFlowRateAutosized();
  }

  double ChillerElectricEIR::sizingFactor() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->sizingFactor();
  }

  bool ChillerElectricEIR::isSizingFactorDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isSizingFactorDefaulted();
  }

  double ChillerElectricEIR::basinHeaterCapacity() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->basinHeaterCapacity();
  }

  bool ChillerElectricEIR::isBasinHeaterCapacityDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isBasinHeaterCapacityDefaulted();
  }

  double ChillerElectricEIR::basinHeaterSetpointTemperature() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->basinHeaterSetpointTemperature();
  }

  bool ChillerElectricEIR::isBasinHeaterSetpointTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->isBasinHeaterSetpointTemperatureDefaulted();
  }

  bool ChillerElectricEIR::setReferenceCapacity(boost::optional<double> referenceCapacity) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  bool ChillerElectricEIR::setReferenceCapacity(double referenceCapacity) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  void ChillerElectricEIR::autosizeReferenceCapacity() {
    getImpl<detail::ChillerElectricEIR_Impl>()->autosizeReferenceCapacity();
  }

  bool ChillerElectricEIR::setReferenceCOP(double referenceCOP) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceCOP(referenceCOP);
  }

  bool ChillerElectricEIR::setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceLeavingChilledWaterTemperature(referenceLeavingChilledWaterTemperature);
  }

  void ChillerElectricEIR::resetReferenceLeavingChilledWaterTemperature() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetReferenceLeavingChilledWaterTemperature();
  }

  bool ChillerElectricEIR::setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceEnteringCondenserFluidTemperature(referenceEnteringCondenserFluidTemperature);
  }

  void ChillerElectricEIR::resetReferenceEnteringCondenserFluidTemperature() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetReferenceEnteringCondenserFluidTemperature();
  }

  bool ChillerElectricEIR::setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceChilledWaterFlowRate(referenceChilledWaterFlowRate);
  }

  bool ChillerElectricEIR::setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceChilledWaterFlowRate(referenceChilledWaterFlowRate);
  }

  void ChillerElectricEIR::resetReferenceChilledWaterFlowRate() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetReferenceChilledWaterFlowRate();
  }

  void ChillerElectricEIR::autosizeReferenceChilledWaterFlowRate() {
    getImpl<detail::ChillerElectricEIR_Impl>()->autosizeReferenceChilledWaterFlowRate();
  }

  bool ChillerElectricEIR::setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceCondenserFluidFlowRate(referenceCondenserFluidFlowRate);
  }

  bool ChillerElectricEIR::setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceCondenserFluidFlowRate(referenceCondenserFluidFlowRate);
  }

  void ChillerElectricEIR::resetReferenceCondenserFluidFlowRate() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetReferenceCondenserFluidFlowRate();
  }

  void ChillerElectricEIR::autosizeReferenceCondenserFluidFlowRate() {
    getImpl<detail::ChillerElectricEIR_Impl>()->autosizeReferenceCondenserFluidFlowRate();
  }

  bool ChillerElectricEIR::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
  }

  void ChillerElectricEIR::resetMinimumPartLoadRatio() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetMinimumPartLoadRatio();
  }

  bool ChillerElectricEIR::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
  }

  void ChillerElectricEIR::resetMaximumPartLoadRatio() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetMaximumPartLoadRatio();
  }

  bool ChillerElectricEIR::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
  }

  void ChillerElectricEIR::resetOptimumPartLoadRatio() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetOptimumPartLoadRatio();
  }

  bool ChillerElectricEIR::setMinimumUnloadingRatio(double minimumUnloadingRatio) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setMinimumUnloadingRatio(minimumUnloadingRatio);
  }

  void ChillerElectricEIR::resetMinimumUnloadingRatio() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetMinimumUnloadingRatio();
  }

  bool ChillerElectricEIR::setCondenserType(std::string condenserType) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserType(condenserType);
  }

  void ChillerElectricEIR::resetCondenserType() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetCondenserType();
  }

  bool ChillerElectricEIR::setCondenserFanPowerRatio(double condenserFanPowerRatio) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserFanPowerRatio(condenserFanPowerRatio);
  }

  void ChillerElectricEIR::resetCondenserFanPowerRatio() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetCondenserFanPowerRatio();
  }

  bool ChillerElectricEIR::setFractionofCompressorElectricConsumptionRejectedbyCondenser(
    double fractionofCompressorElectricConsumptionRejectedbyCondenser) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setFractionofCompressorElectricConsumptionRejectedbyCondenser(
      fractionofCompressorElectricConsumptionRejectedbyCondenser);
  }

  void ChillerElectricEIR::resetFractionofCompressorElectricConsumptionRejectedbyCondenser() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetFractionofCompressorElectricConsumptionRejectedbyCondenser();
  }

  bool ChillerElectricEIR::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setLeavingChilledWaterLowerTemperatureLimit(leavingChilledWaterLowerTemperatureLimit);
  }

  void ChillerElectricEIR::resetLeavingChilledWaterLowerTemperatureLimit() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetLeavingChilledWaterLowerTemperatureLimit();
  }

  bool ChillerElectricEIR::setChillerFlowMode(std::string chillerFlowMode) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setChillerFlowMode(chillerFlowMode);
  }

  void ChillerElectricEIR::resetChillerFlowMode() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetChillerFlowMode();
  }

  bool ChillerElectricEIR::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setDesignHeatRecoveryWaterFlowRate(designHeatRecoveryWaterFlowRate);
  }

  void ChillerElectricEIR::autosizeDesignHeatRecoveryWaterFlowRate() {
    getImpl<detail::ChillerElectricEIR_Impl>()->autosizeDesignHeatRecoveryWaterFlowRate();
  }

  bool ChillerElectricEIR::setSizingFactor(double sizingFactor) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setSizingFactor(sizingFactor);
  }

  void ChillerElectricEIR::resetSizingFactor() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetSizingFactor();
  }

  bool ChillerElectricEIR::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  void ChillerElectricEIR::resetBasinHeaterCapacity() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetBasinHeaterCapacity();
  }

  bool ChillerElectricEIR::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
  }

  void ChillerElectricEIR::resetBasinHeaterSetpointTemperature() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetBasinHeaterSetpointTemperature();
  }

  bool ChillerElectricEIR::setBasinHeaterSchedule(Schedule& schedule) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setBasinHeaterSchedule(schedule);
  }

  void ChillerElectricEIR::resetBasinHeaterSchedule() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetBasinHeaterSchedule();
  }

  boost::optional<Schedule> ChillerElectricEIR::basinHeaterSchedule() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->basinHeaterSchedule();
  }

  std::string ChillerElectricEIR::endUseSubcategory() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->endUseSubcategory();
  }

  bool ChillerElectricEIR::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  // Convenience functions
  boost::optional<PlantLoop> ChillerElectricEIR::chilledWaterLoop() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->chilledWaterLoop();
  }

  boost::optional<PlantLoop> ChillerElectricEIR::condenserWaterLoop() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->condenserWaterLoop();
  }

  boost::optional<PlantLoop> ChillerElectricEIR::heatRecoveryLoop() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->heatRecoveryLoop();
  }

  double ChillerElectricEIR::condenserHeatRecoveryRelativeCapacityFraction() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->condenserHeatRecoveryRelativeCapacityFraction();
  }

  bool ChillerElectricEIR::setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserHeatRecoveryRelativeCapacityFraction(
      condenserHeatRecoveryRelativeCapacityFraction);
  }

  boost::optional<Schedule> ChillerElectricEIR::heatRecoveryInletHighTemperatureLimitSchedule() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->heatRecoveryInletHighTemperatureLimitSchedule();
  }

  bool ChillerElectricEIR::setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& s) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setHeatRecoveryInletHighTemperatureLimitSchedule(s);
  }

  void ChillerElectricEIR::resetHeatRecoveryInletHighTemperatureLimitSchedule() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetHeatRecoveryInletHighTemperatureLimitSchedule();
  }

  boost::optional<Node> ChillerElectricEIR::heatRecoveryLeavingTemperatureSetpointNode() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->heatRecoveryLeavingTemperatureSetpointNode();
  }

  bool ChillerElectricEIR::setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node) {
    return getImpl<detail::ChillerElectricEIR_Impl>()->setHeatRecoveryLeavingTemperatureSetpointNode(node);
  }
  void ChillerElectricEIR::resetHeatRecoveryLeavingTemperatureSetpointNode() {
    getImpl<detail::ChillerElectricEIR_Impl>()->resetHeatRecoveryLeavingTemperatureSetpointNode();
  }

  /// @cond
  ChillerElectricEIR::ChillerElectricEIR(std::shared_ptr<detail::ChillerElectricEIR_Impl> impl) : WaterToWaterComponent(std::move(impl)) {}

  /// @endcond

  boost::optional<double> ChillerElectricEIR::autosizedReferenceCapacity() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->autosizedReferenceCapacity();
  }

  boost::optional<double> ChillerElectricEIR::autosizedReferenceChilledWaterFlowRate() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->autosizedReferenceChilledWaterFlowRate();
  }

  boost::optional<double> ChillerElectricEIR::autosizedReferenceCondenserFluidFlowRate() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->autosizedReferenceCondenserFluidFlowRate();
  }

  boost::optional<double> ChillerElectricEIR::autosizedDesignHeatRecoveryWaterFlowRate() const {
    return getImpl<detail::ChillerElectricEIR_Impl>()->autosizedDesignHeatRecoveryWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
