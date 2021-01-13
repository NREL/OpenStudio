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
#include "ChillerElectricReformulatedEIR.hpp"
#include "ChillerElectricReformulatedEIR_Impl.hpp"
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

#include <utilities/idd/OS_Chiller_Electric_ReformulatedEIR_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ChillerElectricReformulatedEIR_Impl::ChillerElectricReformulatedEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ChillerElectricReformulatedEIR::iddObjectType());
    }

    ChillerElectricReformulatedEIR_Impl::ChillerElectricReformulatedEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ChillerElectricReformulatedEIR::iddObjectType());
    }

    ChillerElectricReformulatedEIR_Impl::ChillerElectricReformulatedEIR_Impl(const ChillerElectricReformulatedEIR_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ChillerElectricReformulatedEIR_Impl::outputVariableNames() const {
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

    IddObjectType ChillerElectricReformulatedEIR_Impl::iddObjectType() const {
      return ChillerElectricReformulatedEIR::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ChillerElectricReformulatedEIR_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterOperatingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ChillerElectricReformulatedEIR", "Basin Heater Operating"));
      } else if (std::find(b, e, OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName) != e) {
        result.push_back(ScheduleTypeKey("ChillerElectricReformulatedEIR", "Heat Recovery Inlet High Temperature Limit"));
      }
      return result;
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::referenceCapacity() const {
      return getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, true);
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double ChillerElectricReformulatedEIR_Impl::referenceCOP() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ChillerElectricReformulatedEIR_Impl::referenceLeavingChilledWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceLeavingChilledWaterTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature);
    }

    double ChillerElectricReformulatedEIR_Impl::referenceEnteringCondenserFluidTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceEnteringCondenserFluidTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceEnteringCondenserFluidTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceEnteringCondenserFluidTemperature);
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::referenceChilledWaterFlowRate() const {
      return getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, true);
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceChilledWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::referenceCondenserFluidFlowRate() const {
      return getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserFluidFlowRate, true);
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceCondenserFluidFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserFluidFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    CurveBiquadratic ChillerElectricReformulatedEIR_Impl::coolingCapacityFunctionOfTemperature() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_ReformulatedEIRFields::CoolingCapacityFunctionofTemperatureCurveName).get();
      return wo.optionalCast<CurveBiquadratic>().get();
    }

    CurveBiquadratic ChillerElectricReformulatedEIR_Impl::electricInputToCoolingOutputRatioFunctionOfTemperature() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName).get();
      return wo.optionalCast<CurveBiquadratic>().get();
    }

    CurveQuadratic ChillerElectricReformulatedEIR_Impl::electricInputToCoolingOutputRatioFunctionOfPLR() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName).get();
      return wo.optionalCast<CurveQuadratic>().get();
    }

    double ChillerElectricReformulatedEIR_Impl::minimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::MinimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isMinimumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::MinimumPartLoadRatio);
    }

    double ChillerElectricReformulatedEIR_Impl::maximumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::MaximumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isMaximumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::MaximumPartLoadRatio);
    }

    double ChillerElectricReformulatedEIR_Impl::optimumPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::OptimumPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isOptimumPartLoadRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::OptimumPartLoadRatio);
    }

    double ChillerElectricReformulatedEIR_Impl::minimumUnloadingRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::MinimumUnloadingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isMinimumUnloadingRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::MinimumUnloadingRatio);
    }

    std::string ChillerElectricReformulatedEIR_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isCondenserTypeDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::CondenserType);
    }

    double ChillerElectricReformulatedEIR_Impl::condenserFanPowerRatio() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserFanPowerRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isCondenserFanPowerRatioDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::CondenserFanPowerRatio);
    }

    double ChillerElectricReformulatedEIR_Impl::fractionofCompressorElectricConsumptionRejectedbyCondenser() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser);
    }

    double ChillerElectricReformulatedEIR_Impl::leavingChilledWaterLowerTemperatureLimit() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isLeavingChilledWaterLowerTemperatureLimitDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit);
    }

    std::string ChillerElectricReformulatedEIR_Impl::chillerFlowMode() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isChillerFlowModeDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode);
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::designHeatRecoveryWaterFlowRate() const {
      return getDouble(OS_Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate, true);
    }

    bool ChillerElectricReformulatedEIR_Impl::isDesignHeatRecoveryWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double ChillerElectricReformulatedEIR_Impl::sizingFactor() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::SizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isSizingFactorDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::SizingFactor);
    }

    double ChillerElectricReformulatedEIR_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isBasinHeaterCapacityDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterCapacity);
    }

    double ChillerElectricReformulatedEIR_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isBasinHeaterSetpointTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterSetpointTemperature);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCapacity(boost::optional<double> referenceCapacity) {
      bool result = false;
      if (referenceCapacity) {
        result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, referenceCapacity.get());
      }
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::autosizeReferenceCapacity() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCapacity(double referenceCapacity) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCapacity, referenceCapacity);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCOP(double referenceCOP) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCOP, referenceCOP);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature, referenceLeavingChilledWaterTemperature);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceLeavingChilledWaterTemperature() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceEnteringCondenserFluidTemperature, referenceEnteringCondenserFluidTemperature);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceEnteringCondenserFluidTemperature() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceEnteringCondenserFluidTemperature, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate) {
      bool result = false;
      if (referenceChilledWaterFlowRate) {
        result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, referenceChilledWaterFlowRate.get());
      }
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceChilledWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, "");
      OS_ASSERT(result);
    }

    void ChillerElectricReformulatedEIR_Impl::autosizeReferenceChilledWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceChilledWaterFlowRate, referenceChilledWaterFlowRate);
      return result;
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate) {
      bool result = false;
      if (referenceCondenserFluidFlowRate) {
        result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserFluidFlowRate, referenceCondenserFluidFlowRate.get());
      }
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceCondenserFluidFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserFluidFlowRate, "");
      OS_ASSERT(result);
    }

    void ChillerElectricReformulatedEIR_Impl::autosizeReferenceCondenserFluidFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserFluidFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserFluidFlowRate, referenceCondenserFluidFlowRate);
      return result;
    }

    bool ChillerElectricReformulatedEIR_Impl::setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_ReformulatedEIRFields::CoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    }

    bool ChillerElectricReformulatedEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, curve.handle());
    }

    bool ChillerElectricReformulatedEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, curve.handle());
    }

    bool ChillerElectricReformulatedEIR_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::MinimumPartLoadRatio, minimumPartLoadRatio);
    }

    void ChillerElectricReformulatedEIR_Impl::resetMinimumPartLoadRatio() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::MinimumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::MaximumPartLoadRatio, maximumPartLoadRatio);
    }

    void ChillerElectricReformulatedEIR_Impl::resetMaximumPartLoadRatio() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::MaximumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::OptimumPartLoadRatio, optimumPartLoadRatio);
    }

    void ChillerElectricReformulatedEIR_Impl::resetOptimumPartLoadRatio() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::OptimumPartLoadRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setMinimumUnloadingRatio(double minimumUnloadingRatio) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::MinimumUnloadingRatio, minimumUnloadingRatio);
    }

    void ChillerElectricReformulatedEIR_Impl::resetMinimumUnloadingRatio() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::MinimumUnloadingRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setCondenserType(std::string condenserType) {
      bool ok = false;
      if ((istringEqual("AirCooled", condenserType) || istringEqual("EvaporativelyCooled", condenserType)) && (this->secondaryPlantLoop())) {
        LOG(Warn,
            "Cannot set condenserType to AirCooled or EvaporativelyCooled, chiller '" << this->name() << "' is connected to a secondaryPlantLoop");
      } else if (istringEqual("WaterCooled", condenserType) && !(this->secondaryPlantLoop())) {
        LOG(Warn, "Cannot set condenserType to 'WaterCooled', chiller '" << this->name() << "' is not connected to a secondaryPlantLoop");
      } else {
        ok = setString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserType, condenserType);
      }

      return ok;
    }

    void ChillerElectricReformulatedEIR_Impl::resetCondenserType() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserType, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setCondenserFanPowerRatio(double condenserFanPowerRatio) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserFanPowerRatio, condenserFanPowerRatio);
    }

    void ChillerElectricReformulatedEIR_Impl::resetCondenserFanPowerRatio() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserFanPowerRatio, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setFractionofCompressorElectricConsumptionRejectedbyCondenser(
      double fractionofCompressorElectricConsumptionRejectedbyCondenser) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser,
                       fractionofCompressorElectricConsumptionRejectedbyCondenser);
    }

    void ChillerElectricReformulatedEIR_Impl::resetFractionofCompressorElectricConsumptionRejectedbyCondenser() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit, leavingChilledWaterLowerTemperatureLimit);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetLeavingChilledWaterLowerTemperatureLimit() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setChillerFlowMode(std::string chillerFlowMode) {
      bool result = false;
      if (istringEqual(chillerFlowMode, "VariableFlow")) {
        // Support legacy key
        result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode, "LeavingSetpointModulated");
      } else {
        result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode, chillerFlowMode);
      }

      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetChillerFlowMode() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate, designHeatRecoveryWaterFlowRate);
    }

    void ChillerElectricReformulatedEIR_Impl::autosizeDesignHeatRecoveryWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::DesignHeatRecoveryWaterFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setSizingFactor(double sizingFactor) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::SizingFactor, sizingFactor);
    }

    void ChillerElectricReformulatedEIR_Impl::resetSizingFactor() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::SizingFactor, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterCapacity, basinHeaterCapacity);
    }

    void ChillerElectricReformulatedEIR_Impl::resetBasinHeaterCapacity() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterCapacity, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      return setDouble(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    }

    void ChillerElectricReformulatedEIR_Impl::resetBasinHeaterSetpointTemperature() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterSetpointTemperature, "");
      OS_ASSERT(result);
    }

    unsigned ChillerElectricReformulatedEIR_Impl::supplyInletPort() const {
      return OS_Chiller_Electric_ReformulatedEIRFields::ChilledWaterInletNodeName;
    }

    unsigned ChillerElectricReformulatedEIR_Impl::supplyOutletPort() const {
      return OS_Chiller_Electric_ReformulatedEIRFields::ChilledWaterOutletNodeName;
    }

    unsigned ChillerElectricReformulatedEIR_Impl::demandInletPort() const {
      return OS_Chiller_Electric_ReformulatedEIRFields::CondenserInletNodeName;
    }

    unsigned ChillerElectricReformulatedEIR_Impl::demandOutletPort() const {
      return OS_Chiller_Electric_ReformulatedEIRFields::CondenserOutletNodeName;
    }

    unsigned ChillerElectricReformulatedEIR_Impl::tertiaryInletPort() const {
      return OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletNodeName;
    }

    unsigned ChillerElectricReformulatedEIR_Impl::tertiaryOutletPort() const {
      return OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryOutletNodeName;
    }

    ModelObject ChillerElectricReformulatedEIR_Impl::clone(Model model) const {
      ChillerElectricReformulatedEIR chiller = WaterToWaterComponent_Impl::clone(model).cast<ChillerElectricReformulatedEIR>();

      return chiller;
    }

    bool ChillerElectricReformulatedEIR_Impl::setBasinHeaterSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterOperatingScheduleName, "ChillerElectricReformulatedEIR", "Basin Heater Operating", schedule);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetBasinHeaterSchedule() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterOperatingScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> ChillerElectricReformulatedEIR_Impl::basinHeaterSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterOperatingScheduleName);
    }

    std::vector<ModelObject> ChillerElectricReformulatedEIR_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(coolingCapacityFunctionOfTemperature());
      result.push_back(electricInputToCoolingOutputRatioFunctionOfTemperature());
      result.push_back(electricInputToCoolingOutputRatioFunctionOfPLR());

      return result;
    }

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> ChillerElectricReformulatedEIR_Impl::chilledWaterLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> ChillerElectricReformulatedEIR_Impl::condenserWaterLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    /** Convenience Function to return the Heat Recovery Loop **/
    boost::optional<PlantLoop> ChillerElectricReformulatedEIR_Impl::heatRecoveryLoop() const {
      return WaterToWaterComponent_Impl::tertiaryPlantLoop();
    }

    bool ChillerElectricReformulatedEIR_Impl::addToNode(Node& node) {

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

    bool ChillerElectricReformulatedEIR_Impl::addToTertiaryNode(Node& node) {
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

    bool ChillerElectricReformulatedEIR_Impl::removeFromSecondaryPlantLoop() {
      // Disconnect the component
      bool ok = WaterToWaterComponent_Impl::removeFromSecondaryPlantLoop();

      // Switch the condenser type to "AirCooled"
      this->setCondenserType("AirCooled");
      return ok;
    }

    boost::optional<ModelObject> ChillerElectricReformulatedEIR_Impl::basinHeaterScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = basinHeaterSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool ChillerElectricReformulatedEIR_Impl::setBasinHeaterScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Reference Capacity", "W");
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedReferenceChilledWaterFlowRate() const {
      return getAutosizedValue("Design Size Reference Chilled Water Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedReferenceCondenserFluidFlowRate() const {
      return getAutosizedValue("Design Size Reference Condenser Fluid Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedDesignHeatRecoveryWaterFlowRate() const {
      return getAutosizedValue("Design Size Heat Recovery Water Flow Rate", "m3/s");
    }

    void ChillerElectricReformulatedEIR_Impl::autosize() {
      autosizeReferenceCapacity();
      autosizeReferenceChilledWaterFlowRate();
      autosizeReferenceCondenserFluidFlowRate();
      autosizeDesignHeatRecoveryWaterFlowRate();
    }

    void ChillerElectricReformulatedEIR_Impl::applySizingValues() {
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

    std::string ChillerElectricReformulatedEIR_Impl::endUseSubcategory() const {
      auto value = getString(OS_Chiller_Electric_ReformulatedEIRFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_Chiller_Electric_ReformulatedEIRFields::EndUseSubcategory, endUseSubcategory);
    }

    double ChillerElectricReformulatedEIR_Impl::condenserHeatRecoveryRelativeCapacityFraction() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserHeatRecoveryRelativeCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction) {
      bool result =
        setDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserHeatRecoveryRelativeCapacityFraction, condenserHeatRecoveryRelativeCapacityFraction);
      // OS_ASSERT(result);
      return result;
    }

    boost::optional<Schedule> ChillerElectricReformulatedEIR_Impl::heatRecoveryInletHighTemperatureLimitSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_ReformulatedEIRFields::BasinHeaterOperatingScheduleName);
    }

    // TODO: ScheduleTypeLimits
    bool ChillerElectricReformulatedEIR_Impl::setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, "ChillerElectricReformulatedEIR",
                                "Heat Recovery Inlet High Temperature Limit", schedule);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetHeatRecoveryInletHighTemperatureLimitSchedule() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::heatRecoveryLeavingTemperatureSetpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName);
    }

    bool ChillerElectricReformulatedEIR_Impl::setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node) {
      return setPointer(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, node.handle());
    }

    void ChillerElectricReformulatedEIR_Impl::resetHeatRecoveryLeavingTemperatureSetpointNode() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ChillerElectricReformulatedEIR::ChillerElectricReformulatedEIR(const Model& model, const CurveBiquadratic& CCFofT, const CurveBiquadratic& EItoCORFofT,
                                         const CurveQuadratic& EItoCORFofPLR)
    : WaterToWaterComponent(ChillerElectricReformulatedEIR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricReformulatedEIR_Impl>());

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

  ChillerElectricReformulatedEIR::ChillerElectricReformulatedEIR(const Model& model) : WaterToWaterComponent(ChillerElectricReformulatedEIR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricReformulatedEIR_Impl>());

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

  IddObjectType ChillerElectricReformulatedEIR::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Chiller_Electric_ReformulatedEIR);
    return result;
  }

  std::vector<std::string> ChillerElectricReformulatedEIR::validCondenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_ReformulatedEIRFields::CondenserType);
  }

  std::vector<std::string> ChillerElectricReformulatedEIR::validChillerFlowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode);
  }

  boost::optional<double> ChillerElectricReformulatedEIR::referenceCapacity() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceCapacity();
  }

  bool ChillerElectricReformulatedEIR::isReferenceCapacityAutosized() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceCapacityAutosized();
  }

  double ChillerElectricReformulatedEIR::referenceCOP() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceCOP();
  }

  double ChillerElectricReformulatedEIR::referenceLeavingChilledWaterTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceLeavingChilledWaterTemperature();
  }

  bool ChillerElectricReformulatedEIR::isReferenceLeavingChilledWaterTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceLeavingChilledWaterTemperatureDefaulted();
  }

  double ChillerElectricReformulatedEIR::referenceEnteringCondenserFluidTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceEnteringCondenserFluidTemperature();
  }

  bool ChillerElectricReformulatedEIR::isReferenceEnteringCondenserFluidTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceEnteringCondenserFluidTemperatureDefaulted();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::referenceChilledWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceChilledWaterFlowRate();
  }

  bool ChillerElectricReformulatedEIR::isReferenceChilledWaterFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceChilledWaterFlowRateAutosized();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::referenceCondenserFluidFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceCondenserFluidFlowRate();
  }

  bool ChillerElectricReformulatedEIR::isReferenceCondenserFluidFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceCondenserFluidFlowRateAutosized();
  }

  bool ChillerElectricReformulatedEIR::setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& curve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCoolingCapacityFunctionOfTemperature(curve);
  }

  bool ChillerElectricReformulatedEIR::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& curve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfTemperature(curve);
  }

  bool ChillerElectricReformulatedEIR::setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& curve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfPLR(curve);
  }

  CurveBiquadratic ChillerElectricReformulatedEIR::coolingCapacityFunctionOfTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->coolingCapacityFunctionOfTemperature();
  }

  CurveBiquadratic ChillerElectricReformulatedEIR::electricInputToCoolingOutputRatioFunctionOfTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfTemperature();
  }

  CurveQuadratic ChillerElectricReformulatedEIR::electricInputToCoolingOutputRatioFunctionOfPLR() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfPLR();
  }

  double ChillerElectricReformulatedEIR::minimumPartLoadRatio() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->minimumPartLoadRatio();
  }

  bool ChillerElectricReformulatedEIR::isMinimumPartLoadRatioDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isMinimumPartLoadRatioDefaulted();
  }

  double ChillerElectricReformulatedEIR::maximumPartLoadRatio() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->maximumPartLoadRatio();
  }

  bool ChillerElectricReformulatedEIR::isMaximumPartLoadRatioDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isMaximumPartLoadRatioDefaulted();
  }

  double ChillerElectricReformulatedEIR::optimumPartLoadRatio() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->optimumPartLoadRatio();
  }

  bool ChillerElectricReformulatedEIR::isOptimumPartLoadRatioDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isOptimumPartLoadRatioDefaulted();
  }

  double ChillerElectricReformulatedEIR::minimumUnloadingRatio() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->minimumUnloadingRatio();
  }

  bool ChillerElectricReformulatedEIR::isMinimumUnloadingRatioDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isMinimumUnloadingRatioDefaulted();
  }

  std::string ChillerElectricReformulatedEIR::condenserType() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserType();
  }

  bool ChillerElectricReformulatedEIR::isCondenserTypeDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isCondenserTypeDefaulted();
  }

  double ChillerElectricReformulatedEIR::condenserFanPowerRatio() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserFanPowerRatio();
  }

  bool ChillerElectricReformulatedEIR::isCondenserFanPowerRatioDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isCondenserFanPowerRatioDefaulted();
  }

  double ChillerElectricReformulatedEIR::fractionofCompressorElectricConsumptionRejectedbyCondenser() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->fractionofCompressorElectricConsumptionRejectedbyCondenser();
  }

  bool ChillerElectricReformulatedEIR::isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted();
  }

  double ChillerElectricReformulatedEIR::leavingChilledWaterLowerTemperatureLimit() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->leavingChilledWaterLowerTemperatureLimit();
  }

  bool ChillerElectricReformulatedEIR::isLeavingChilledWaterLowerTemperatureLimitDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isLeavingChilledWaterLowerTemperatureLimitDefaulted();
  }

  std::string ChillerElectricReformulatedEIR::chillerFlowMode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->chillerFlowMode();
  }

  bool ChillerElectricReformulatedEIR::isChillerFlowModeDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isChillerFlowModeDefaulted();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::designHeatRecoveryWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->designHeatRecoveryWaterFlowRate();
  }

  bool ChillerElectricReformulatedEIR::isDesignHeatRecoveryWaterFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isDesignHeatRecoveryWaterFlowRateAutosized();
  }

  double ChillerElectricReformulatedEIR::sizingFactor() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->sizingFactor();
  }

  bool ChillerElectricReformulatedEIR::isSizingFactorDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isSizingFactorDefaulted();
  }

  double ChillerElectricReformulatedEIR::basinHeaterCapacity() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->basinHeaterCapacity();
  }

  bool ChillerElectricReformulatedEIR::isBasinHeaterCapacityDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isBasinHeaterCapacityDefaulted();
  }

  double ChillerElectricReformulatedEIR::basinHeaterSetpointTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->basinHeaterSetpointTemperature();
  }

  bool ChillerElectricReformulatedEIR::isBasinHeaterSetpointTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isBasinHeaterSetpointTemperatureDefaulted();
  }

  bool ChillerElectricReformulatedEIR::setReferenceCapacity(boost::optional<double> referenceCapacity) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  bool ChillerElectricReformulatedEIR::setReferenceCapacity(double referenceCapacity) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCapacity(referenceCapacity);
  }

  void ChillerElectricReformulatedEIR::autosizeReferenceCapacity() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizeReferenceCapacity();
  }

  bool ChillerElectricReformulatedEIR::setReferenceCOP(double referenceCOP) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCOP(referenceCOP);
  }

  bool ChillerElectricReformulatedEIR::setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceLeavingChilledWaterTemperature(referenceLeavingChilledWaterTemperature);
  }

  void ChillerElectricReformulatedEIR::resetReferenceLeavingChilledWaterTemperature() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceLeavingChilledWaterTemperature();
  }

  bool ChillerElectricReformulatedEIR::setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceEnteringCondenserFluidTemperature(referenceEnteringCondenserFluidTemperature);
  }

  void ChillerElectricReformulatedEIR::resetReferenceEnteringCondenserFluidTemperature() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceEnteringCondenserFluidTemperature();
  }

  bool ChillerElectricReformulatedEIR::setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceChilledWaterFlowRate(referenceChilledWaterFlowRate);
  }

  bool ChillerElectricReformulatedEIR::setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceChilledWaterFlowRate(referenceChilledWaterFlowRate);
  }

  void ChillerElectricReformulatedEIR::resetReferenceChilledWaterFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceChilledWaterFlowRate();
  }

  void ChillerElectricReformulatedEIR::autosizeReferenceChilledWaterFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizeReferenceChilledWaterFlowRate();
  }

  bool ChillerElectricReformulatedEIR::setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCondenserFluidFlowRate(referenceCondenserFluidFlowRate);
  }

  bool ChillerElectricReformulatedEIR::setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCondenserFluidFlowRate(referenceCondenserFluidFlowRate);
  }

  void ChillerElectricReformulatedEIR::resetReferenceCondenserFluidFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceCondenserFluidFlowRate();
  }

  void ChillerElectricReformulatedEIR::autosizeReferenceCondenserFluidFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizeReferenceCondenserFluidFlowRate();
  }

  bool ChillerElectricReformulatedEIR::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
  }

  void ChillerElectricReformulatedEIR::resetMinimumPartLoadRatio() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetMinimumPartLoadRatio();
  }

  bool ChillerElectricReformulatedEIR::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
  }

  void ChillerElectricReformulatedEIR::resetMaximumPartLoadRatio() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetMaximumPartLoadRatio();
  }

  bool ChillerElectricReformulatedEIR::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
  }

  void ChillerElectricReformulatedEIR::resetOptimumPartLoadRatio() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetOptimumPartLoadRatio();
  }

  bool ChillerElectricReformulatedEIR::setMinimumUnloadingRatio(double minimumUnloadingRatio) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setMinimumUnloadingRatio(minimumUnloadingRatio);
  }

  void ChillerElectricReformulatedEIR::resetMinimumUnloadingRatio() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetMinimumUnloadingRatio();
  }

  bool ChillerElectricReformulatedEIR::setCondenserType(std::string condenserType) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCondenserType(condenserType);
  }

  void ChillerElectricReformulatedEIR::resetCondenserType() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetCondenserType();
  }

  bool ChillerElectricReformulatedEIR::setCondenserFanPowerRatio(double condenserFanPowerRatio) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCondenserFanPowerRatio(condenserFanPowerRatio);
  }

  void ChillerElectricReformulatedEIR::resetCondenserFanPowerRatio() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetCondenserFanPowerRatio();
  }

  bool ChillerElectricReformulatedEIR::setFractionofCompressorElectricConsumptionRejectedbyCondenser(
    double fractionofCompressorElectricConsumptionRejectedbyCondenser) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setFractionofCompressorElectricConsumptionRejectedbyCondenser(
      fractionofCompressorElectricConsumptionRejectedbyCondenser);
  }

  void ChillerElectricReformulatedEIR::resetFractionofCompressorElectricConsumptionRejectedbyCondenser() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetFractionofCompressorElectricConsumptionRejectedbyCondenser();
  }

  bool ChillerElectricReformulatedEIR::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setLeavingChilledWaterLowerTemperatureLimit(leavingChilledWaterLowerTemperatureLimit);
  }

  void ChillerElectricReformulatedEIR::resetLeavingChilledWaterLowerTemperatureLimit() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetLeavingChilledWaterLowerTemperatureLimit();
  }

  bool ChillerElectricReformulatedEIR::setChillerFlowMode(std::string chillerFlowMode) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setChillerFlowMode(chillerFlowMode);
  }

  void ChillerElectricReformulatedEIR::resetChillerFlowMode() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetChillerFlowMode();
  }

  bool ChillerElectricReformulatedEIR::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setDesignHeatRecoveryWaterFlowRate(designHeatRecoveryWaterFlowRate);
  }

  void ChillerElectricReformulatedEIR::autosizeDesignHeatRecoveryWaterFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizeDesignHeatRecoveryWaterFlowRate();
  }

  bool ChillerElectricReformulatedEIR::setSizingFactor(double sizingFactor) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setSizingFactor(sizingFactor);
  }

  void ChillerElectricReformulatedEIR::resetSizingFactor() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetSizingFactor();
  }

  bool ChillerElectricReformulatedEIR::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  void ChillerElectricReformulatedEIR::resetBasinHeaterCapacity() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetBasinHeaterCapacity();
  }

  bool ChillerElectricReformulatedEIR::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
  }

  void ChillerElectricReformulatedEIR::resetBasinHeaterSetpointTemperature() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetBasinHeaterSetpointTemperature();
  }

  bool ChillerElectricReformulatedEIR::setBasinHeaterSchedule(Schedule& schedule) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setBasinHeaterSchedule(schedule);
  }

  void ChillerElectricReformulatedEIR::resetBasinHeaterSchedule() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetBasinHeaterSchedule();
  }

  boost::optional<Schedule> ChillerElectricReformulatedEIR::basinHeaterSchedule() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->basinHeaterSchedule();
  }

  std::string ChillerElectricReformulatedEIR::endUseSubcategory() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->endUseSubcategory();
  }

  bool ChillerElectricReformulatedEIR::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  // Convenience functions
  boost::optional<PlantLoop> ChillerElectricReformulatedEIR::chilledWaterLoop() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->chilledWaterLoop();
  }

  boost::optional<PlantLoop> ChillerElectricReformulatedEIR::condenserWaterLoop() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserWaterLoop();
  }

  boost::optional<PlantLoop> ChillerElectricReformulatedEIR::heatRecoveryLoop() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->heatRecoveryLoop();
  }

  double ChillerElectricReformulatedEIR::condenserHeatRecoveryRelativeCapacityFraction() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserHeatRecoveryRelativeCapacityFraction();
  }

  bool ChillerElectricReformulatedEIR::setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCondenserHeatRecoveryRelativeCapacityFraction(
      condenserHeatRecoveryRelativeCapacityFraction);
  }

  boost::optional<Schedule> ChillerElectricReformulatedEIR::heatRecoveryInletHighTemperatureLimitSchedule() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->heatRecoveryInletHighTemperatureLimitSchedule();
  }

  bool ChillerElectricReformulatedEIR::setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& s) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setHeatRecoveryInletHighTemperatureLimitSchedule(s);
  }

  void ChillerElectricReformulatedEIR::resetHeatRecoveryInletHighTemperatureLimitSchedule() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetHeatRecoveryInletHighTemperatureLimitSchedule();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::heatRecoveryLeavingTemperatureSetpointNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->heatRecoveryLeavingTemperatureSetpointNode();
  }

  bool ChillerElectricReformulatedEIR::setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setHeatRecoveryLeavingTemperatureSetpointNode(node);
  }
  void ChillerElectricReformulatedEIR::resetHeatRecoveryLeavingTemperatureSetpointNode() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetHeatRecoveryLeavingTemperatureSetpointNode();
  }

  /// @cond
  ChillerElectricReformulatedEIR::ChillerElectricReformulatedEIR(std::shared_ptr<detail::ChillerElectricReformulatedEIR_Impl> impl) : WaterToWaterComponent(std::move(impl)) {}

  /// @endcond

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedReferenceCapacity() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedReferenceCapacity();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedReferenceChilledWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedReferenceChilledWaterFlowRate();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedReferenceCondenserFluidFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedReferenceCondenserFluidFlowRate();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedDesignHeatRecoveryWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedDesignHeatRecoveryWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
