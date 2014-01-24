/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <model/Model.hpp>
#include <model/ChillerElectricEIR.hpp>
#include <model/ChillerElectricEIR_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Chiller_Electric_EIR_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ChillerElectricEIR_Impl::ChillerElectricEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ChillerElectricEIR::iddObjectType());
  }

  ChillerElectricEIR_Impl::ChillerElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ChillerElectricEIR::iddObjectType());
  }

  ChillerElectricEIR_Impl::ChillerElectricEIR_Impl(const ChillerElectricEIR_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  IddObjectType ChillerElectricEIR_Impl::iddObjectType() const {
    return ChillerElectricEIR::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ChillerElectricEIR_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ChillerElectricEIR","Basin Heater"));
    }
    return result;
  }

  boost::optional<double> ChillerElectricEIR_Impl::referenceCapacity() const {
    return getDouble(OS_Chiller_Electric_EIRFields::ReferenceCapacity,true);
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
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::ReferenceCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerElectricEIR_Impl::referenceLeavingChilledWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isReferenceLeavingChilledWaterTemperatureDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature);
  }

  double ChillerElectricEIR_Impl::referenceEnteringCondenserFluidTemperature() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isReferenceEnteringCondenserFluidTemperatureDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature);
  }

  boost::optional<double> ChillerElectricEIR_Impl::referenceChilledWaterFlowRate() const {
    return getDouble(OS_Chiller_Electric_EIRFields::ReferenceChilledWaterFlowRate,true);
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
    return getDouble(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate,true);
  }

  bool ChillerElectricEIR_Impl::isReferenceCondenserFluidFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ReferenceCondenserFluidFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  CurveBiquadratic ChillerElectricEIR_Impl::coolingCapacityFunctionOfTemperature() const
  {
    WorkspaceObject wo = getTarget(OS_Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName).get();
    return wo.optionalCast<CurveBiquadratic>().get();
  }

  CurveBiquadratic  ChillerElectricEIR_Impl::electricInputToCoolingOutputRatioFunctionOfTemperature() const
  {
    WorkspaceObject wo = getTarget(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName).get();
    return wo.optionalCast<CurveBiquadratic>().get();
  }

  CurveQuadratic ChillerElectricEIR_Impl::electricInputToCoolingOutputRatioFunctionOfPLR() const
  {
    WorkspaceObject wo = getTarget(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName).get();
    return wo.optionalCast<CurveQuadratic>().get();
  }

  double ChillerElectricEIR_Impl::minimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isMinimumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio);
  }

  double ChillerElectricEIR_Impl::maximumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isMaximumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio);
  }

  double ChillerElectricEIR_Impl::optimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isOptimumPartLoadRatioDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio);
  }

  double ChillerElectricEIR_Impl::minimumUnloadingRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isMinimumUnloadingRatioDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio);
  }

  std::string ChillerElectricEIR_Impl::chilledWaterInletNodeName() const {
    boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ChilledWaterInletNodeName,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerElectricEIR_Impl::chilledWaterOutletNodeName() const {
    boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ChilledWaterOutletNodeName,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> ChillerElectricEIR_Impl::condenserInletNodeName() const {
    return getString(OS_Chiller_Electric_EIRFields::CondenserInletNodeName,true);
  }

  boost::optional<std::string> ChillerElectricEIR_Impl::condenserOutletNodeName() const {
    return getString(OS_Chiller_Electric_EIRFields::CondenserOutletNodeName,true);
  }

  std::string ChillerElectricEIR_Impl::condenserType() const {
    boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::CondenserType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isCondenserTypeDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::CondenserType);
  }

  double ChillerElectricEIR_Impl::condenserFanPowerRatio() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isCondenserFanPowerRatioDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio);
  }

  double ChillerElectricEIR_Impl::compressorMotorEfficiency() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::CompressorMotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isCompressorMotorEfficiencyDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::CompressorMotorEfficiency);
  }

  double ChillerElectricEIR_Impl::leavingChilledWaterLowerTemperatureLimit() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isLeavingChilledWaterLowerTemperatureLimitDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit);
  }

  std::string ChillerElectricEIR_Impl::chillerFlowMode() const {
    boost::optional<std::string> value = getString(OS_Chiller_Electric_EIRFields::ChillerFlowMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isChillerFlowModeDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::ChillerFlowMode);
  }

  double ChillerElectricEIR_Impl::designHeatRecoveryWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isDesignHeatRecoveryWaterFlowRateDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate);
  }

  boost::optional<std::string> ChillerElectricEIR_Impl::heatRecoveryInletNodeName() const {
    return getString(OS_Chiller_Electric_EIRFields::HeatRecoveryInletNodeName,true);
  }

  boost::optional<std::string> ChillerElectricEIR_Impl::heatRecoveryOutletNodeName() const {
    return getString(OS_Chiller_Electric_EIRFields::HeatRecoveryOutletNodeName,true);
  }

  double ChillerElectricEIR_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::SizingFactor);
  }

  double ChillerElectricEIR_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::BasinHeaterCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerElectricEIR_Impl::isBasinHeaterCapacityDefaulted() const {
    return isEmpty(OS_Chiller_Electric_EIRFields::BasinHeaterCapacity);
  }

  double ChillerElectricEIR_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_Chiller_Electric_EIRFields::BasinHeaterSetpointTemperature,true);
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

  bool ChillerElectricEIR_Impl::setReferenceCapacity(double referenceCapacity)
  {
    return setDouble(OS_Chiller_Electric_EIRFields::ReferenceCapacity, referenceCapacity);
  }

  bool ChillerElectricEIR_Impl::setReferenceCOP(double referenceCOP)
  {
    return setDouble(OS_Chiller_Electric_EIRFields::ReferenceCOP, referenceCOP);
  }

  void ChillerElectricEIR_Impl::setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature) {
    bool result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature, referenceLeavingChilledWaterTemperature);
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::resetReferenceLeavingChilledWaterTemperature() {
    bool result = setString(OS_Chiller_Electric_EIRFields::ReferenceLeavingChilledWaterTemperature, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature) {
    bool result = setDouble(OS_Chiller_Electric_EIRFields::ReferenceEnteringCondenserFluidTemperature, referenceEnteringCondenserFluidTemperature);
    OS_ASSERT(result);
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

  bool ChillerElectricEIR_Impl::setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& curve)
  {
    if(model() != curve.model())
    {
      return false;
    }
    return this->setPointer(OS_Chiller_Electric_EIRFields::CoolingCapacityFunctionofTemperatureCurveName, curve.handle());
  }

  bool ChillerElectricEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& curve)
  {
    if(model() != curve.model())
    {
      return false;
    }
    return this->setPointer(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, curve.handle());
  }

  bool ChillerElectricEIR_Impl::setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& curve)
  {
    if(model() != curve.model())
    {
      return false;
    }
    return this->setPointer(OS_Chiller_Electric_EIRFields::ElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, curve.handle());
  }

  bool ChillerElectricEIR_Impl::setMinimumPartLoadRatio(double minimumPartLoadRatio)
  {
   return setDouble(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio, minimumPartLoadRatio);
  }

  void ChillerElectricEIR_Impl::resetMinimumPartLoadRatio()
  {
    bool result = setString(OS_Chiller_Electric_EIRFields::MinimumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool ChillerElectricEIR_Impl::setMaximumPartLoadRatio(double maximumPartLoadRatio)
  {
    return setDouble(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio, maximumPartLoadRatio);
  }

  void ChillerElectricEIR_Impl::resetMaximumPartLoadRatio() {
    bool result = setString(OS_Chiller_Electric_EIRFields::MaximumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool ChillerElectricEIR_Impl::setOptimumPartLoadRatio(double optimumPartLoadRatio)
  {
    return setDouble(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio, optimumPartLoadRatio);
  }

  void ChillerElectricEIR_Impl::resetOptimumPartLoadRatio() {
    bool result = setString(OS_Chiller_Electric_EIRFields::OptimumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool ChillerElectricEIR_Impl::setMinimumUnloadingRatio(double minimumUnloadingRatio)
  {
   return setDouble(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio, minimumUnloadingRatio);
  }

  void ChillerElectricEIR_Impl::resetMinimumUnloadingRatio() {
    bool result = setString(OS_Chiller_Electric_EIRFields::MinimumUnloadingRatio, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setChilledWaterInletNodeName(std::string chilledWaterInletNodeName) {
    bool result = setString(OS_Chiller_Electric_EIRFields::ChilledWaterInletNodeName, chilledWaterInletNodeName);
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setChilledWaterOutletNodeName(std::string chilledWaterOutletNodeName) {
    bool result = setString(OS_Chiller_Electric_EIRFields::ChilledWaterOutletNodeName, chilledWaterOutletNodeName);
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setCondenserInletNodeName(boost::optional<std::string> condenserInletNodeName) {
    if (condenserInletNodeName) {
      bool result = setString(OS_Chiller_Electric_EIRFields::CondenserInletNodeName, condenserInletNodeName.get());
      OS_ASSERT(result);
    }
  }

  void ChillerElectricEIR_Impl::resetCondenserInletNodeName() {
    bool result = setString(OS_Chiller_Electric_EIRFields::CondenserInletNodeName, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setCondenserInletNodeName(std::string condenserInletNodeName) {
    bool result = setString(OS_Chiller_Electric_EIRFields::CondenserInletNodeName, condenserInletNodeName);
    OS_ASSERT(result);
  }


  void ChillerElectricEIR_Impl::setCondenserOutletNodeName(boost::optional<std::string> condenserOutletNodeName) {
    if (condenserOutletNodeName) {
      bool result = setString(OS_Chiller_Electric_EIRFields::CondenserOutletNodeName, condenserOutletNodeName.get());
      OS_ASSERT(result);
    }
  }

  void ChillerElectricEIR_Impl::resetCondenserOutletNodeName() {
    bool result = setString(OS_Chiller_Electric_EIRFields::CondenserOutletNodeName, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setCondenserOutletNodeName(std::string condenserOutletNodeName) {
    bool result = setString(OS_Chiller_Electric_EIRFields::CondenserOutletNodeName, condenserOutletNodeName);
    OS_ASSERT(result);
  }



  bool ChillerElectricEIR_Impl::setCondenserType(std::string condenserType)
  {
    return setString(OS_Chiller_Electric_EIRFields::CondenserType, condenserType);
  }

  void ChillerElectricEIR_Impl::resetCondenserType() {
    bool result = setString(OS_Chiller_Electric_EIRFields::CondenserType, "");
    OS_ASSERT(result);
  }

  bool ChillerElectricEIR_Impl::setCondenserFanPowerRatio(double condenserFanPowerRatio)
  {
    return setDouble(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio, condenserFanPowerRatio);
  }

  void ChillerElectricEIR_Impl::resetCondenserFanPowerRatio() {
    bool result = setString(OS_Chiller_Electric_EIRFields::CondenserFanPowerRatio, "");
    OS_ASSERT(result);
  }

  bool ChillerElectricEIR_Impl::setCompressorMotorEfficiency(double compressorMotorEfficiency)
  {
    return setDouble(OS_Chiller_Electric_EIRFields::CompressorMotorEfficiency, compressorMotorEfficiency);
  }

  void ChillerElectricEIR_Impl::resetCompressorMotorEfficiency() {
    bool result = setString(OS_Chiller_Electric_EIRFields::CompressorMotorEfficiency, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
    bool result = setDouble(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit, leavingChilledWaterLowerTemperatureLimit);
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::resetLeavingChilledWaterLowerTemperatureLimit() {
    bool result = setString(OS_Chiller_Electric_EIRFields::LeavingChilledWaterLowerTemperatureLimit, "");
    OS_ASSERT(result);
  }

  bool ChillerElectricEIR_Impl::setChillerFlowMode(std::string chillerFlowMode) {
    bool result = false;
    if(istringEqual(chillerFlowMode,"VariableFlow"))
    {
      // Support legacy key
      result = setString(OS_Chiller_Electric_EIRFields::ChillerFlowMode, "LeavingSetpointModulated");
    }
    else
    {
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

  void ChillerElectricEIR_Impl::resetDesignHeatRecoveryWaterFlowRate() {
    bool result = setString(OS_Chiller_Electric_EIRFields::DesignHeatRecoveryWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setHeatRecoveryInletNodeName(boost::optional<std::string> heatRecoveryInletNodeName) {
    if (heatRecoveryInletNodeName) {
      bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryInletNodeName, heatRecoveryInletNodeName.get());
      OS_ASSERT(result);
    }
  }

  void ChillerElectricEIR_Impl::resetHeatRecoveryInletNodeName() {
    bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryInletNodeName, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setHeatRecoveryInletNodeName(std::string heatRecoveryInletNodeName) {
    bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryInletNodeName, heatRecoveryInletNodeName);
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setHeatRecoveryOutletNodeName(boost::optional<std::string> heatRecoveryOutletNodeName) {
    if (heatRecoveryOutletNodeName) {
      bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryOutletNodeName, heatRecoveryOutletNodeName.get());
      OS_ASSERT(result);
    }
  }

  void ChillerElectricEIR_Impl::resetHeatRecoveryOutletNodeName() {
    bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryOutletNodeName, "");
    OS_ASSERT(result);
  }

  void ChillerElectricEIR_Impl::setHeatRecoveryOutletNodeName(std::string heatRecoveryOutletNodeName) {
    bool result = setString(OS_Chiller_Electric_EIRFields::HeatRecoveryOutletNodeName, heatRecoveryOutletNodeName);
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

  const std::vector<std::string> & ChillerElectricEIR_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;

    return result; 
  }

  unsigned ChillerElectricEIR_Impl::supplyInletPort()
  {
    return OS_Chiller_Electric_EIRFields::ChilledWaterInletNodeName;
  }

  unsigned ChillerElectricEIR_Impl::supplyOutletPort()
  {
    return OS_Chiller_Electric_EIRFields::ChilledWaterOutletNodeName;
  }

  unsigned ChillerElectricEIR_Impl::demandInletPort()
  {
    return OS_Chiller_Electric_EIRFields::CondenserInletNodeName;
  }

  unsigned ChillerElectricEIR_Impl::demandOutletPort()
  {
    return OS_Chiller_Electric_EIRFields::CondenserOutletNodeName;
  }

  ModelObject ChillerElectricEIR_Impl::clone(Model model) const
  {
    ChillerElectricEIR chiller = WaterToWaterComponent_Impl::clone(model).cast<ChillerElectricEIR>();

    CurveBiquadratic biQuadCurve = this->coolingCapacityFunctionOfTemperature().clone(model).cast<CurveBiquadratic>();

    chiller.setCoolingCapacityFunctionOfTemperature(biQuadCurve);

    biQuadCurve = this->electricInputToCoolingOutputRatioFunctionOfTemperature().clone(model).cast<CurveBiquadratic>();

    chiller.setElectricInputToCoolingOutputRatioFunctionOfTemperature(biQuadCurve);

    CurveQuadratic curve = this->electricInputToCoolingOutputRatioFunctionOfPLR().clone(model).cast<CurveQuadratic>();

    chiller.setElectricInputToCoolingOutputRatioFunctionOfPLR(curve);

    return chiller;
  }

  bool ChillerElectricEIR_Impl::setBasinHeaterSchedule(Schedule & schedule ) {
    bool result = setSchedule(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName,
                              "ChillerElectricEIR",
                              "Basin Heater Operating",
                              schedule);
    return result;
  }
  
  void ChillerElectricEIR_Impl::resetBasinHeaterSchedule()
  {
    bool result = setString(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName, "");
    OS_ASSERT(result);
  }
  
  boost::optional<Schedule> ChillerElectricEIR_Impl::basinHeaterSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Chiller_Electric_EIRFields::BasinHeaterOperatingScheduleName);
  }

  std::vector<ModelObject> ChillerElectricEIR_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(coolingCapacityFunctionOfTemperature());
    result.push_back(electricInputToCoolingOutputRatioFunctionOfTemperature());
    result.push_back(electricInputToCoolingOutputRatioFunctionOfPLR());

    return result;
  }

  bool ChillerElectricEIR_Impl::addToNode(Node & node)
  {
    return WaterToWaterComponent_Impl::addToNode(node);
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
      }
      else {
        return false;
      }
    }
    else {
      resetBasinHeaterSchedule();
    }
    return true;
  }

} // detail

ChillerElectricEIR::ChillerElectricEIR(const Model& model,
                                       const CurveBiquadratic& CCFofT,
                                       const CurveBiquadratic& EItoCORFofT,
                                       const CurveQuadratic& EItoCORFofPLR)
  : WaterToWaterComponent(ChillerElectricEIR::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ChillerElectricEIR_Impl>());

  OS_ASSERT(setCoolingCapacityFunctionOfTemperature(CCFofT));
  OS_ASSERT(setElectricInputToCoolingOutputRatioFunctionOfTemperature(EItoCORFofT));
  OS_ASSERT(setElectricInputToCoolingOutputRatioFunctionOfPLR(EItoCORFofPLR));
  autosizeReferenceCapacity();
  OS_ASSERT(setReferenceCOP(5.5f));

  setDesignHeatRecoveryWaterFlowRate(0.0);

  resetHeatRecoveryInletNodeName();

  resetHeatRecoveryOutletNodeName();

  setSizingFactor(1.0);

  setBasinHeaterCapacity(0.0);

  setBasinHeaterSetpointTemperature(10.0);

  resetBasinHeaterSchedule();
}

IddObjectType ChillerElectricEIR::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Chiller_Electric_EIR);
  return result;
}

std::vector<std::string> ChillerElectricEIR::validCondenserTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Chiller_Electric_EIRFields::CondenserType);
}

std::vector<std::string> ChillerElectricEIR::validChillerFlowModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Chiller_Electric_EIRFields::ChillerFlowMode);
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

bool ChillerElectricEIR::setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& curve)
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->setCoolingCapacityFunctionOfTemperature(curve);
}

bool ChillerElectricEIR::setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& curve)
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfTemperature(curve);
}

bool ChillerElectricEIR::setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& curve)
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->setElectricInputToCoolingOutputRatioFunctionOfPLR(curve);
}

CurveBiquadratic ChillerElectricEIR::coolingCapacityFunctionOfTemperature() const
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->coolingCapacityFunctionOfTemperature();
}

CurveBiquadratic ChillerElectricEIR::electricInputToCoolingOutputRatioFunctionOfTemperature() const
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->electricInputToCoolingOutputRatioFunctionOfTemperature();
}

CurveQuadratic ChillerElectricEIR::electricInputToCoolingOutputRatioFunctionOfPLR() const
{
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

std::string ChillerElectricEIR::chilledWaterInletNodeName() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->chilledWaterInletNodeName();
}

std::string ChillerElectricEIR::chilledWaterOutletNodeName() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->chilledWaterOutletNodeName();
}

boost::optional<std::string> ChillerElectricEIR::condenserInletNodeName() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->condenserInletNodeName();
}

boost::optional<std::string> ChillerElectricEIR::condenserOutletNodeName() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->condenserOutletNodeName();
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

double ChillerElectricEIR::compressorMotorEfficiency() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->compressorMotorEfficiency();
}

bool ChillerElectricEIR::isCompressorMotorEfficiencyDefaulted() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->isCompressorMotorEfficiencyDefaulted();
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

double ChillerElectricEIR::designHeatRecoveryWaterFlowRate() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->designHeatRecoveryWaterFlowRate();
}

bool ChillerElectricEIR::isDesignHeatRecoveryWaterFlowRateDefaulted() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->isDesignHeatRecoveryWaterFlowRateDefaulted();
}

boost::optional<std::string> ChillerElectricEIR::heatRecoveryInletNodeName() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->heatRecoveryInletNodeName();
}

boost::optional<std::string> ChillerElectricEIR::heatRecoveryOutletNodeName() const {
  return getImpl<detail::ChillerElectricEIR_Impl>()->heatRecoveryOutletNodeName();
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

void ChillerElectricEIR::setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceLeavingChilledWaterTemperature(referenceLeavingChilledWaterTemperature);
}

void ChillerElectricEIR::resetReferenceLeavingChilledWaterTemperature() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetReferenceLeavingChilledWaterTemperature();
}

void ChillerElectricEIR::setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setReferenceEnteringCondenserFluidTemperature(referenceEnteringCondenserFluidTemperature);
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

void ChillerElectricEIR::setChilledWaterInletNodeName(std::string chilledWaterInletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setChilledWaterInletNodeName(chilledWaterInletNodeName);
}

void ChillerElectricEIR::setChilledWaterOutletNodeName(std::string chilledWaterOutletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setChilledWaterOutletNodeName(chilledWaterOutletNodeName);
}

void ChillerElectricEIR::setCondenserInletNodeName(boost::optional<std::string> condenserInletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserInletNodeName(condenserInletNodeName);
}

void ChillerElectricEIR::setCondenserInletNodeName(std::string condenserInletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserInletNodeName(condenserInletNodeName);
}

void ChillerElectricEIR::resetCondenserInletNodeName() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetCondenserInletNodeName();
}

void ChillerElectricEIR::setCondenserOutletNodeName(boost::optional<std::string> condenserOutletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserOutletNodeName(condenserOutletNodeName);
}

void ChillerElectricEIR::setCondenserOutletNodeName(std::string condenserOutletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setCondenserOutletNodeName(condenserOutletNodeName);
}

void ChillerElectricEIR::resetCondenserOutletNodeName() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetCondenserOutletNodeName();
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

bool ChillerElectricEIR::setCompressorMotorEfficiency(double compressorMotorEfficiency) {
  return getImpl<detail::ChillerElectricEIR_Impl>()->setCompressorMotorEfficiency(compressorMotorEfficiency);
}

void ChillerElectricEIR::resetCompressorMotorEfficiency() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetCompressorMotorEfficiency();
}

void ChillerElectricEIR::setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setLeavingChilledWaterLowerTemperatureLimit(leavingChilledWaterLowerTemperatureLimit);
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

void ChillerElectricEIR::resetDesignHeatRecoveryWaterFlowRate() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetDesignHeatRecoveryWaterFlowRate();
}

void ChillerElectricEIR::setHeatRecoveryInletNodeName(boost::optional<std::string> heatRecoveryInletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setHeatRecoveryInletNodeName(heatRecoveryInletNodeName);
}

void ChillerElectricEIR::setHeatRecoveryInletNodeName(std::string heatRecoveryInletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setHeatRecoveryInletNodeName(heatRecoveryInletNodeName);
}

void ChillerElectricEIR::resetHeatRecoveryInletNodeName() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetHeatRecoveryInletNodeName();
}

void ChillerElectricEIR::setHeatRecoveryOutletNodeName(boost::optional<std::string> heatRecoveryOutletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setHeatRecoveryOutletNodeName(heatRecoveryOutletNodeName);
}

void ChillerElectricEIR::setHeatRecoveryOutletNodeName(std::string heatRecoveryOutletNodeName) {
  getImpl<detail::ChillerElectricEIR_Impl>()->setHeatRecoveryOutletNodeName(heatRecoveryOutletNodeName);
}

void ChillerElectricEIR::resetHeatRecoveryOutletNodeName() {
  getImpl<detail::ChillerElectricEIR_Impl>()->resetHeatRecoveryOutletNodeName();
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

bool ChillerElectricEIR::setBasinHeaterSchedule(Schedule & schedule )
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->setBasinHeaterSchedule(schedule);
}

void ChillerElectricEIR::resetBasinHeaterSchedule()
{
  getImpl<detail::ChillerElectricEIR_Impl>()->resetBasinHeaterSchedule();
}

boost::optional<Schedule> ChillerElectricEIR::basinHeaterSchedule() const
{
  return getImpl<detail::ChillerElectricEIR_Impl>()->basinHeaterSchedule();
}

/// @cond
ChillerElectricEIR::ChillerElectricEIR(boost::shared_ptr<detail::ChillerElectricEIR_Impl> impl)
  : WaterToWaterComponent(impl)
{}

/// @endcond

} // model
} // openstudio

