/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "ChillerElectricReformulatedEIR.hpp"
#include "ChillerElectricReformulatedEIR_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveBicubic.hpp"
#include "CurveBicubic_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Chiller_Electric_ReformulatedEIR_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    ChillerElectricReformulatedEIR_Impl::ChillerElectricReformulatedEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ChillerElectricReformulatedEIR::iddObjectType());
    }

    ChillerElectricReformulatedEIR_Impl::ChillerElectricReformulatedEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ChillerElectricReformulatedEIR::iddObjectType());
    }

    ChillerElectricReformulatedEIR_Impl::ChillerElectricReformulatedEIR_Impl(const ChillerElectricReformulatedEIR_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ChillerElectricReformulatedEIR_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Chiller Part Load Ratio",
        "Chiller Cycling Ratio",
        "Chiller Electricity Rate",
        "Chiller Electricity Energy",
        "Chiller Evaporator Cooling Rate",
        "Chiller Evaporator Cooling Energy",
        "Chiller False Load Heat Transfer Rate",
        "Chiller False Load Heat Transfer Energy",
        "Chiller Evaporator Inlet Temperature",
        "Chiller Evaporator Outlet Temperature",
        "Chiller Evaporator Mass Flow Rate",
        "Chiller Condenser Heat Transfer Rate",
        "Chiller Condenser Heat Transfer Energy",
        "Chiller COP",
        "Chiller Capacity Temperature Modifier Multiplier",
        "Chiller EIR Temperature Modifier Multiplier",
        "Chiller EIR Part Load Modifier Multiplier",
        "Chiller Condenser Inlet Temperature",
        "Chiller Condenser Outlet Temperature",
        "Chiller Condenser Mass Flow Rate",
        "Chiller Effective Heat Rejection Temperature",
        "Chiller Heat Recovery Inlet Temperature",
        "Chiller Heat Recovery Mass Flow Rate",
        "Chiller Heat Recovery Outlet Temperature",
        "Chiller Total Recovered Heat Energy",
        "Chiller Total Recovered Heat Rate",
        "Thermosiphon Status",
      };
      return result;
    }

    IddObjectType ChillerElectricReformulatedEIR_Impl::iddObjectType() const {
      return ChillerElectricReformulatedEIR::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ChillerElectricReformulatedEIR_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName) != e) {
        result.emplace_back("ChillerElectricReformulatedEIR", "Heat Recovery Inlet High Temperature Limit");
      } else if (std::find(b, e, OS_Chiller_Electric_ReformulatedEIRFields::TemperatureDifferenceAcrossCondenserScheduleName) != e) {
        result.emplace_back("ChillerElectricReformulatedEIR", "Temperature Difference Across Condenser");
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

    double ChillerElectricReformulatedEIR_Impl::referenceLeavingCondenserWaterTemperature() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingCondenserWaterTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceLeavingCondenserWaterTemperatureDefaulted() const {
      return isEmpty(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingCondenserWaterTemperature);
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

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::referenceCondenserWaterFlowRate() const {
      return getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, true);
    }

    bool ChillerElectricReformulatedEIR_Impl::isReferenceCondenserWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    Curve ChillerElectricReformulatedEIR_Impl::coolingCapacityFunctionOfTemperature() const {
      WorkspaceObject wo = getTarget(OS_Chiller_Electric_ReformulatedEIRFields::CoolingCapacityFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    Curve ChillerElectricReformulatedEIR_Impl::electricInputToCoolingOutputRatioFunctionOfTemperature() const {
      WorkspaceObject wo =
        getTarget(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName).get();
      return wo.optionalCast<Curve>().get();
    }

    std::string ChillerElectricReformulatedEIR_Impl::electricInputToCoolingOutputRatioFunctionOfPLRType() const {
      boost::optional<std::string> value =
        getString(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType, true);
      OS_ASSERT(value);
      return value.get();
    }

    Curve ChillerElectricReformulatedEIR_Impl::electricInputToCoolingOutputRatioFunctionOfPLR() const {
      WorkspaceObject wo =
        getTarget(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName).get();
      return wo.optionalCast<Curve>().get();
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

    double ChillerElectricReformulatedEIR_Impl::fractionofCompressorElectricConsumptionRejectedbyCondenser() const {
      boost::optional<double> value =
        getDouble(OS_Chiller_Electric_ReformulatedEIRFields::FractionofCompressorElectricConsumptionRejectedbyCondenser, true);
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
      bool result =
        setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature, referenceLeavingChilledWaterTemperature);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceLeavingChilledWaterTemperature() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingChilledWaterTemperature, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceLeavingCondenserWaterTemperature(double referenceLeavingCondenserWaterTemperature) {
      bool result =
        setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingCondenserWaterTemperature, referenceLeavingCondenserWaterTemperature);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceLeavingCondenserWaterTemperature() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceLeavingCondenserWaterTemperature, "");
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

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCondenserWaterFlowRate(boost::optional<double> referenceCondenserWaterFlowRate) {
      bool result = false;
      if (referenceCondenserWaterFlowRate) {
        result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, referenceCondenserWaterFlowRate.get());
      }
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetReferenceCondenserWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, "");
      OS_ASSERT(result);
    }

    void ChillerElectricReformulatedEIR_Impl::autosizeReferenceCondenserWaterFlowRate() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setReferenceCondenserWaterFlowRate(double referenceCondenserWaterFlowRate) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ReferenceCondenserWaterFlowRate, referenceCondenserWaterFlowRate);
      return result;
    }

    bool ChillerElectricReformulatedEIR_Impl::setCoolingCapacityFunctionOfTemperature(const Curve& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_ReformulatedEIRFields::CoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    }

    bool ChillerElectricReformulatedEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const Curve& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName,
                              curve.handle());
    }

    bool ChillerElectricReformulatedEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfPLRType(
      const std::string& electricInputToCoolingOutputRatioFunctionOfPLRType) {
      return setString(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType,
                       electricInputToCoolingOutputRatioFunctionOfPLRType);
    }

    void ChillerElectricReformulatedEIR_Impl::resetElectricInputToCoolingOutputRatioFunctionOfPLRType() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfPLR(const Curve& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName,
                              curve.handle());
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
      bool result =
        setDouble(OS_Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit, leavingChilledWaterLowerTemperatureLimit);
      OS_ASSERT(result);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetLeavingChilledWaterLowerTemperatureLimit() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::LeavingChilledWaterLowerTemperatureLimit, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setChillerFlowMode(const std::string& chillerFlowMode) {
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
      auto chiller = WaterToWaterComponent_Impl::clone(model).cast<ChillerElectricReformulatedEIR>();

      return std::move(chiller);
    }

    std::vector<ModelObject> ChillerElectricReformulatedEIR_Impl::children() const {
      std::vector<ModelObject> result;
      result.reserve(5);

      result.emplace_back(coolingCapacityFunctionOfTemperature());
      result.emplace_back(electricInputToCoolingOutputRatioFunctionOfTemperature());
      result.emplace_back(electricInputToCoolingOutputRatioFunctionOfPLR());
      if (auto curve_ = condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve()) {
        result.emplace_back(std::move(*curve_));
      }
      if (auto curve_ = thermosiphonCapacityFractionCurve()) {
        result.emplace_back(std::move(*curve_));
      }
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

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::chilledWaterInletNode() const {
      if (auto mo_ = supplyInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::chilledWaterOutletNode() const {
      if (auto mo_ = supplyOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::condenserInletNode() const {
      if (auto mo_ = demandInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::condenserOutletNode() const {
      if (auto mo_ = demandOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::heatRecoveryInletNode() const {
      if (auto mo_ = tertiaryInletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::heatRecoveryOutletNode() const {
      if (auto mo_ = tertiaryOutletModelObject()) {
        return mo_->optionalCast<Node>();
      }
      return boost::none;
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

      return ok;
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedReferenceCapacity() const {
      return getAutosizedValue("Design Size Reference Capacity", "W");
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedReferenceChilledWaterFlowRate() const {
      return getAutosizedValue("Design Size Reference Chilled Water Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedReferenceCondenserWaterFlowRate() const {
      return getAutosizedValue("Design Size Reference Condenser Water Flow Rate", "m3/s");
    }

    boost::optional<double> ChillerElectricReformulatedEIR_Impl::autosizedDesignHeatRecoveryWaterFlowRate() const {
      return getAutosizedValue("Design Size Design Heat Recovery Fluid Flow Rate", "m3/s");
    }

    void ChillerElectricReformulatedEIR_Impl::autosize() {
      autosizeReferenceCapacity();
      autosizeReferenceChilledWaterFlowRate();
      autosizeReferenceCondenserWaterFlowRate();
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

      val = autosizedReferenceCondenserWaterFlowRate();
      if (val) {
        setReferenceCondenserWaterFlowRate(val.get());
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
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserHeatRecoveryRelativeCapacityFraction,
                              condenserHeatRecoveryRelativeCapacityFraction);
      // OS_ASSERT(result);
      return result;
    }

    boost::optional<Schedule> ChillerElectricReformulatedEIR_Impl::heatRecoveryInletHighTemperatureLimitSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName);
    }

    bool ChillerElectricReformulatedEIR_Impl::setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName,
                                "ChillerElectricReformulatedEIR", "Heat Recovery Inlet High Temperature Limit", schedule);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetHeatRecoveryInletHighTemperatureLimitSchedule() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryInletHighTemperatureLimitScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<Node> ChillerElectricReformulatedEIR_Impl::heatRecoveryLeavingTemperatureSetpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(
        OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName);
    }

    bool ChillerElectricReformulatedEIR_Impl::setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node) {
      return setPointer(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, node.handle());
    }

    void ChillerElectricReformulatedEIR_Impl::resetHeatRecoveryLeavingTemperatureSetpointNode() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::HeatRecoveryLeavingTemperatureSetpointNodeName, "");
      OS_ASSERT(result);
    }

    std::string ChillerElectricReformulatedEIR_Impl::condenserFlowControl() const {
      boost::optional<std::string> value = getString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserFlowControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> ChillerElectricReformulatedEIR_Impl::condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Chiller_Electric_ReformulatedEIRFields::CondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurveName);
    }

    boost::optional<Schedule> ChillerElectricReformulatedEIR_Impl::temperatureDifferenceAcrossCondenserSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Chiller_Electric_ReformulatedEIRFields::TemperatureDifferenceAcrossCondenserScheduleName);
    }

    double ChillerElectricReformulatedEIR_Impl::condenserMinimumFlowFraction() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserMinimumFlowFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> ChillerElectricReformulatedEIR_Impl::thermosiphonCapacityFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Chiller_Electric_ReformulatedEIRFields::ThermosiphonCapacityFractionCurveName);
    }

    double ChillerElectricReformulatedEIR_Impl::thermosiphonMinimumTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Chiller_Electric_ReformulatedEIRFields::ThermosiphonMinimumTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ChillerElectricReformulatedEIR_Impl::setCondenserFlowControl(const std::string& condenserFlowControl) {
      return setString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserFlowControl, condenserFlowControl);
    }

    bool ChillerElectricReformulatedEIR_Impl::setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(
      const Curve& condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve) {
      bool result = setPointer(OS_Chiller_Electric_ReformulatedEIRFields::CondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurveName,
                               condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve.handle());
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::CondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurveName, "");
      OS_ASSERT(result);
    }

    bool
      ChillerElectricReformulatedEIR_Impl::setTemperatureDifferenceAcrossCondenserSchedule(Schedule& temperatureDifferenceAcrossCondenserSchedule) {
      bool result =
        setSchedule(OS_Chiller_Electric_ReformulatedEIRFields::TemperatureDifferenceAcrossCondenserScheduleName, "ChillerElectricReformulatedEIR",
                    "Temperature Difference Across Condenser", temperatureDifferenceAcrossCondenserSchedule);
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetTemperatureDifferenceAcrossCondenserSchedule() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::TemperatureDifferenceAcrossCondenserScheduleName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setCondenserMinimumFlowFraction(double condenserMinimumFlowFraction) {
      bool result = setDouble(OS_Chiller_Electric_ReformulatedEIRFields::CondenserMinimumFlowFraction, condenserMinimumFlowFraction);
      return result;
    }

    bool ChillerElectricReformulatedEIR_Impl::setThermosiphonCapacityFractionCurve(const Curve& thermosiphonCapacityFractionCurve) {
      bool result =
        setPointer(OS_Chiller_Electric_ReformulatedEIRFields::ThermosiphonCapacityFractionCurveName, thermosiphonCapacityFractionCurve.handle());
      return result;
    }

    void ChillerElectricReformulatedEIR_Impl::resetThermosiphonCapacityFractionCurve() {
      bool result = setString(OS_Chiller_Electric_ReformulatedEIRFields::ThermosiphonCapacityFractionCurveName, "");
      OS_ASSERT(result);
    }

    bool ChillerElectricReformulatedEIR_Impl::setThermosiphonMinimumTemperatureDifference(double thermosiphonMinimumTemperatureDifference) {
      bool result =
        setDouble(OS_Chiller_Electric_ReformulatedEIRFields::ThermosiphonMinimumTemperatureDifference, thermosiphonMinimumTemperatureDifference);
      return result;
    }

    ComponentType ChillerElectricReformulatedEIR_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> ChillerElectricReformulatedEIR_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      result.insert(FuelType::Electricity);
      if (auto p_ = condenserWaterLoop()) {
        for (auto ft : p_->coolingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> ChillerElectricReformulatedEIR_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> ChillerElectricReformulatedEIR_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  ChillerElectricReformulatedEIR::ChillerElectricReformulatedEIR(const Model& model, const Curve& CCFofT, const Curve& EItoCORFofT,
                                                                 const Curve& EItoCORFofPLR)
    : WaterToWaterComponent(ChillerElectricReformulatedEIR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricReformulatedEIR_Impl>());

    OS_ASSERT(setCoolingCapacityFunctionOfTemperature(CCFofT));
    OS_ASSERT(setElectricInputToCoolingOutputRatioFunctionOfTemperature(EItoCORFofT));
    OS_ASSERT(setElectricInputToCoolingOutputRatioFunctionOfPLR(EItoCORFofPLR));
    OS_ASSERT(setReferenceCOP(3.99));

    autosizeReferenceCapacity();
    autosizeReferenceChilledWaterFlowRate();
    // autosizeReferenceCondenserWaterFlowRate();
    autosizeDesignHeatRecoveryWaterFlowRate();

    // setSizingFactor(1.0);

    setCondenserHeatRecoveryRelativeCapacityFraction(1.0);
    resetHeatRecoveryLeavingTemperatureSetpointNode();
    resetHeatRecoveryInletHighTemperatureLimitSchedule();
    setEndUseSubcategory("General");
    setCondenserFlowControl("ConstantFlow");
    setCondenserMinimumFlowFraction(0.2);
    setThermosiphonMinimumTemperatureDifference(0.0);
  }

  ChillerElectricReformulatedEIR::ChillerElectricReformulatedEIR(const Model& model)
    : WaterToWaterComponent(ChillerElectricReformulatedEIR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ChillerElectricReformulatedEIR_Impl>());

    CurveBiquadratic ccFofT(model);
    ccFofT.setCoefficient1Constant(0.958546443);
    ccFofT.setCoefficient2x(0.035168695);
    ccFofT.setCoefficient3xPOW2(0.000124662);
    ccFofT.setCoefficient4y(-0.00274551);
    ccFofT.setCoefficient5yPOW2(-0.00005000);
    ccFofT.setCoefficient6xTIMESY(-0.00017234);
    ccFofT.setMinimumValueofx(5.0);
    ccFofT.setMaximumValueofx(10.0);
    ccFofT.setMinimumValueofy(20.0);
    ccFofT.setMaximumValueofy(40.94);

    CurveBiquadratic eirToCorfOfT(model);
    eirToCorfOfT.setCoefficient1Constant(0.732700123);
    eirToCorfOfT.setCoefficient2x(-0.00834360);
    eirToCorfOfT.setCoefficient3xPOW2(0.000638530);
    eirToCorfOfT.setCoefficient4y(-0.00303753);
    eirToCorfOfT.setCoefficient5yPOW2(0.000484952);
    eirToCorfOfT.setCoefficient6xTIMESY(-0.00083584);
    eirToCorfOfT.setMinimumValueofx(5.0);
    eirToCorfOfT.setMaximumValueofx(10.0);
    eirToCorfOfT.setMinimumValueofy(20.0);
    eirToCorfOfT.setMaximumValueofy(40.94);

    CurveBicubic eirToCorfOfPlr(model);
    eirToCorfOfPlr.setCoefficient1Constant(0.070862846);
    eirToCorfOfPlr.setCoefficient2x(0.002787560);
    eirToCorfOfPlr.setCoefficient3xPOW2(-0.00000891);
    eirToCorfOfPlr.setCoefficient4y(0.230973399);
    eirToCorfOfPlr.setCoefficient5yPOW2(1.250442176);
    eirToCorfOfPlr.setCoefficient6xTIMESY(-0.00216102);
    eirToCorfOfPlr.setCoefficient7xPOW3(0.000000);
    eirToCorfOfPlr.setCoefficient8yPOW3(-0.56300936);
    eirToCorfOfPlr.setCoefficient9xPOW2TIMESY(0.000000);
    eirToCorfOfPlr.setCoefficient10xTIMESYPOW2(0.000000);
    eirToCorfOfPlr.setMinimumValueofx(20.0);
    eirToCorfOfPlr.setMaximumValueofx(40.94);
    eirToCorfOfPlr.setMinimumValueofy(0.01);
    eirToCorfOfPlr.setMaximumValueofy(1.0);

    setCoolingCapacityFunctionOfTemperature(ccFofT);
    setElectricInputToCoolingOutputRatioFunctionOfTemperature(eirToCorfOfT);
    setElectricInputToCoolingOutputRatioFunctionOfPLR(eirToCorfOfPlr);
    OS_ASSERT(setReferenceCOP(3.99));

    autosizeReferenceCapacity();
    autosizeReferenceChilledWaterFlowRate();
    // autosizeReferenceCondenserWaterFlowRate();
    autosizeDesignHeatRecoveryWaterFlowRate();

    // setSizingFactor(1.0);

    setCondenserHeatRecoveryRelativeCapacityFraction(1.0);
    resetHeatRecoveryLeavingTemperatureSetpointNode();
    resetHeatRecoveryInletHighTemperatureLimitSchedule();
    setEndUseSubcategory("General");
    setCondenserFlowControl("ConstantFlow");
    setCondenserMinimumFlowFraction(0.2);
    setThermosiphonMinimumTemperatureDifference(0.0);
  }

  IddObjectType ChillerElectricReformulatedEIR::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Chiller_Electric_ReformulatedEIR);
    return result;
  }

  std::vector<std::string> ChillerElectricReformulatedEIR::validChillerFlowModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_ReformulatedEIRFields::ChillerFlowMode);
  }

  std::vector<std::string> ChillerElectricReformulatedEIR::validCondenserFlowControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Chiller_Electric_ReformulatedEIRFields::CondenserFlowControl);
  }

  std::vector<std::string> ChillerElectricReformulatedEIR::validElectricInputToCoolingOutputRatioFunctionOfPLRTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Chiller_Electric_ReformulatedEIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveType);
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

  double ChillerElectricReformulatedEIR::referenceLeavingCondenserWaterTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceLeavingCondenserWaterTemperature();
  }

  bool ChillerElectricReformulatedEIR::isReferenceLeavingCondenserWaterTemperatureDefaulted() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceLeavingCondenserWaterTemperatureDefaulted();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::referenceChilledWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceChilledWaterFlowRate();
  }

  bool ChillerElectricReformulatedEIR::isReferenceChilledWaterFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceChilledWaterFlowRateAutosized();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::referenceCondenserWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->referenceCondenserWaterFlowRate();
  }

  bool ChillerElectricReformulatedEIR::isReferenceCondenserWaterFlowRateAutosized() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->isReferenceCondenserWaterFlowRateAutosized();
  }

  bool ChillerElectricReformulatedEIR::setCoolingCapacityFunctionOfTemperature(const Curve& curve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCoolingCapacityFunctionOfTemperature(curve);
  }

  bool ChillerElectricReformulatedEIR::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const Curve& curve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfTemperature(curve);
  }

  bool ChillerElectricReformulatedEIR::setElectricInputToCoolingOutputRatioFunctionOfPLRType(
    const std::string& electricInputToCoolingOutputRatioFunctionOfPLRType) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfPLRType(
      electricInputToCoolingOutputRatioFunctionOfPLRType);
  }

  void ChillerElectricReformulatedEIR::resetElectricInputToCoolingOutputRatioFunctionOfPLRType() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetElectricInputToCoolingOutputRatioFunctionOfPLRType();
  }

  bool ChillerElectricReformulatedEIR::setElectricInputToCoolingOutputRatioFunctionOfPLR(const Curve& curve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfPLR(curve);
  }

  Curve ChillerElectricReformulatedEIR::coolingCapacityFunctionOfTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->coolingCapacityFunctionOfTemperature();
  }

  Curve ChillerElectricReformulatedEIR::electricInputToCoolingOutputRatioFunctionOfTemperature() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfTemperature();
  }

  std::string ChillerElectricReformulatedEIR::electricInputToCoolingOutputRatioFunctionOfPLRType() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfPLRType();
  }

  Curve ChillerElectricReformulatedEIR::electricInputToCoolingOutputRatioFunctionOfPLR() const {
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
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceLeavingChilledWaterTemperature(
      referenceLeavingChilledWaterTemperature);
  }

  void ChillerElectricReformulatedEIR::resetReferenceLeavingChilledWaterTemperature() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceLeavingChilledWaterTemperature();
  }

  bool ChillerElectricReformulatedEIR::setReferenceLeavingCondenserWaterTemperature(double referenceLeavingCondenserWaterTemperature) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceLeavingCondenserWaterTemperature(
      referenceLeavingCondenserWaterTemperature);
  }

  void ChillerElectricReformulatedEIR::resetReferenceLeavingCondenserWaterTemperature() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceLeavingCondenserWaterTemperature();
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

  bool ChillerElectricReformulatedEIR::setReferenceCondenserWaterFlowRate(boost::optional<double> referenceCondenserWaterFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCondenserWaterFlowRate(referenceCondenserWaterFlowRate);
  }

  bool ChillerElectricReformulatedEIR::setReferenceCondenserWaterFlowRate(double referenceCondenserWaterFlowRate) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setReferenceCondenserWaterFlowRate(referenceCondenserWaterFlowRate);
  }

  void ChillerElectricReformulatedEIR::resetReferenceCondenserWaterFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetReferenceCondenserWaterFlowRate();
  }

  void ChillerElectricReformulatedEIR::autosizeReferenceCondenserWaterFlowRate() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizeReferenceCondenserWaterFlowRate();
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

  bool ChillerElectricReformulatedEIR::setFractionofCompressorElectricConsumptionRejectedbyCondenser(
    double fractionofCompressorElectricConsumptionRejectedbyCondenser) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setFractionofCompressorElectricConsumptionRejectedbyCondenser(
      fractionofCompressorElectricConsumptionRejectedbyCondenser);
  }

  void ChillerElectricReformulatedEIR::resetFractionofCompressorElectricConsumptionRejectedbyCondenser() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetFractionofCompressorElectricConsumptionRejectedbyCondenser();
  }

  bool ChillerElectricReformulatedEIR::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setLeavingChilledWaterLowerTemperatureLimit(
      leavingChilledWaterLowerTemperatureLimit);
  }

  void ChillerElectricReformulatedEIR::resetLeavingChilledWaterLowerTemperatureLimit() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetLeavingChilledWaterLowerTemperatureLimit();
  }

  bool ChillerElectricReformulatedEIR::setChillerFlowMode(const std::string& chillerFlowMode) {
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

  std::string ChillerElectricReformulatedEIR::endUseSubcategory() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->endUseSubcategory();
  }

  bool ChillerElectricReformulatedEIR::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setEndUseSubcategory(endUseSubcategory);
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

  std::string ChillerElectricReformulatedEIR::condenserFlowControl() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserFlowControl();
  }

  boost::optional<Curve> ChillerElectricReformulatedEIR::condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve();
  }

  boost::optional<Schedule> ChillerElectricReformulatedEIR::temperatureDifferenceAcrossCondenserSchedule() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->temperatureDifferenceAcrossCondenserSchedule();
  }

  double ChillerElectricReformulatedEIR::condenserMinimumFlowFraction() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserMinimumFlowFraction();
  }

  boost::optional<Curve> ChillerElectricReformulatedEIR::thermosiphonCapacityFractionCurve() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->thermosiphonCapacityFractionCurve();
  }

  double ChillerElectricReformulatedEIR::thermosiphonMinimumTemperatureDifference() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->thermosiphonMinimumTemperatureDifference();
  }

  bool ChillerElectricReformulatedEIR::setCondenserFlowControl(const std::string& condenserFlowControl) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCondenserFlowControl(condenserFlowControl);
  }

  bool ChillerElectricReformulatedEIR::setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(
    const Curve& condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(
      condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve);
  }

  void ChillerElectricReformulatedEIR::resetCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve();
  }

  bool ChillerElectricReformulatedEIR::setTemperatureDifferenceAcrossCondenserSchedule(Schedule& temperatureDifferenceAcrossCondenserSchedule) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setTemperatureDifferenceAcrossCondenserSchedule(
      temperatureDifferenceAcrossCondenserSchedule);
  }

  void ChillerElectricReformulatedEIR::resetTemperatureDifferenceAcrossCondenserSchedule() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetTemperatureDifferenceAcrossCondenserSchedule();
  }

  bool ChillerElectricReformulatedEIR::setCondenserMinimumFlowFraction(double condenserMinimumFlowFraction) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setCondenserMinimumFlowFraction(condenserMinimumFlowFraction);
  }

  bool ChillerElectricReformulatedEIR::setThermosiphonCapacityFractionCurve(const Curve& thermosiphonCapacityFractionCurve) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setThermosiphonCapacityFractionCurve(thermosiphonCapacityFractionCurve);
  }

  void ChillerElectricReformulatedEIR::resetThermosiphonCapacityFractionCurve() {
    getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->resetThermosiphonCapacityFractionCurve();
  }

  bool ChillerElectricReformulatedEIR::setThermosiphonMinimumTemperatureDifference(double thermosiphonMinimumTemperatureDifference) {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->setThermosiphonMinimumTemperatureDifference(
      thermosiphonMinimumTemperatureDifference);
  }

  /// @cond
  ChillerElectricReformulatedEIR::ChillerElectricReformulatedEIR(std::shared_ptr<detail::ChillerElectricReformulatedEIR_Impl> impl)
    : WaterToWaterComponent(std::move(impl)) {}

  /// @endcond

  // Convenience functions
  boost::optional<PlantLoop> ChillerElectricReformulatedEIR::chilledWaterLoop() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->chilledWaterLoop();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::chilledWaterInletNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->chilledWaterInletNode();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::chilledWaterOutletNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->chilledWaterOutletNode();
  }

  boost::optional<PlantLoop> ChillerElectricReformulatedEIR::condenserWaterLoop() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserWaterLoop();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::condenserInletNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserInletNode();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::condenserOutletNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->condenserOutletNode();
  }

  boost::optional<PlantLoop> ChillerElectricReformulatedEIR::heatRecoveryLoop() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->heatRecoveryLoop();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::heatRecoveryInletNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->heatRecoveryInletNode();
  }

  boost::optional<Node> ChillerElectricReformulatedEIR::heatRecoveryOutletNode() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->heatRecoveryOutletNode();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedReferenceCapacity() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedReferenceCapacity();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedReferenceChilledWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedReferenceChilledWaterFlowRate();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedReferenceCondenserWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedReferenceCondenserWaterFlowRate();
  }

  boost::optional<double> ChillerElectricReformulatedEIR::autosizedDesignHeatRecoveryWaterFlowRate() const {
    return getImpl<detail::ChillerElectricReformulatedEIR_Impl>()->autosizedDesignHeatRecoveryWaterFlowRate();
  }

}  // namespace model
}  // namespace openstudio
