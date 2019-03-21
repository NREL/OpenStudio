/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ChillerHeaterPerformanceElectricEIR.hpp"
#include "ChillerHeaterPerformanceElectricEIR_Impl.hpp"

// Needed for the convenience functions to return parents
#include "CentralHeatPumpSystem.hpp"
#include "CentralHeatPumpSystem_Impl.hpp"
#include "CentralHeatPumpSystemModule.hpp"
#include "CentralHeatPumpSystemModule_Impl.hpp"
#include "Model.hpp"
//#include "Model_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBicubic.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ChillerHeaterPerformance_Electric_EIR_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ChillerHeaterPerformanceElectricEIR_Impl::ChillerHeaterPerformanceElectricEIR_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ChillerHeaterPerformanceElectricEIR::iddObjectType());
  }

  ChillerHeaterPerformanceElectricEIR_Impl::ChillerHeaterPerformanceElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ChillerHeaterPerformanceElectricEIR::iddObjectType());
  }

  ChillerHeaterPerformanceElectricEIR_Impl::ChillerHeaterPerformanceElectricEIR_Impl(const ChillerHeaterPerformanceElectricEIR_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ChillerHeaterPerformanceElectricEIR_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
      // TODO: Implement? Problem is that the output for this is extremely weird/unusual
      // You actually have to go to the CentralHeatPumpSystem to find the Number of Chiller Heater Modules XXX
      // result.push_back("Chiller Heater Operation Mode Unit <x>");
    return result;
  }

  IddObjectType ChillerHeaterPerformanceElectricEIR_Impl::iddObjectType() const {
    return ChillerHeaterPerformanceElectricEIR::iddObjectType();
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeEvaporatorCapacity() const {
    return getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity,true);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::isReferenceCoolingModeEvaporatorCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeCOP() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeLeavingChilledWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingChilledWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeEnteringCondenserFluidTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEnteringCondenserFluidTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceCoolingModeLeavingCondenserWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingCondenserWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeCoolingCapacityRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingCapacityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeCoolingPowerInputRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingPowerInputRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeLeavingChilledWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingChilledWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeLeavingCondenserWaterTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingCondenserWaterTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::referenceHeatingModeEnteringCondenserFluidTemperature() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeEnteringCondenserFluidTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::heatingModeEnteringChilledWaterTemperatureLowLimit() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeEnteringChilledWaterTemperatureLowLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::chilledWaterFlowModeType() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::designChilledWaterFlowRate() const {
    return getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate,true);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::isDesignChilledWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::designCondenserWaterFlowRate() const {
    return getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate,true);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::isDesignCondenserWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::designHotWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignHotWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::compressorMotorEfficiency() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CompressorMotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::condenserType() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::CondenserType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::coolingModeTemperatureCurveCondenserWaterIndependentVariable() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve ChillerHeaterPerformanceElectricEIR_Impl::coolingModeCoolingCapacityFunctionOfTemperatureCurve() const {
    boost::optional<Curve> value = optionalCoolingModeCoolingCapacityFunctionOfTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Mode Cooling Capacity Function of Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerHeaterPerformanceElectricEIR_Impl::coolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const {
    boost::optional<Curve> value = optionalCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Mode Electric Input to Cooling Output Ratio Function of Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerHeaterPerformanceElectricEIR_Impl::coolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const {
    boost::optional<Curve> value = optionalCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Mode Electric Input to Cooling Output Ratio Function of Part Load Ratio Curve attached.");
    }
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::coolingModeCoolingCapacityOptimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityOptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ChillerHeaterPerformanceElectricEIR_Impl::heatingModeTemperatureCurveCondenserWaterIndependentVariable() const {
    boost::optional<std::string> value = getString(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  Curve ChillerHeaterPerformanceElectricEIR_Impl::heatingModeCoolingCapacityFunctionOfTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingModeCoolingCapacityFunctionOfTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Mode Cooling Capacity Function of Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerHeaterPerformanceElectricEIR_Impl::heatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const {
    boost::optional<Curve> value = optionalHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Mode Electric Input to Cooling Output Ratio Function of Temperature Curve attached.");
    }
    return value.get();
  }

  Curve ChillerHeaterPerformanceElectricEIR_Impl::heatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const {
    boost::optional<Curve> value = optionalHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Mode Electric Input to Cooling Output Ratio Function of Part Load Ratio Curve attached.");
    }
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::heatingModeCoolingCapacityOptimumPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityOptimumPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ChillerHeaterPerformanceElectricEIR_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeEvaporatorCapacity(boost::optional<double> referenceCoolingModeEvaporatorCapacity) {
    bool result(false);
    if (referenceCoolingModeEvaporatorCapacity) {
      result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity, referenceCoolingModeEvaporatorCapacity.get());
    }
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosizeReferenceCoolingModeEvaporatorCapacity() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEvaporatorCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeCOP(double referenceCoolingModeCOP) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeCOP, referenceCoolingModeCOP);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingChilledWaterTemperature, referenceCoolingModeLeavingChilledWaterTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeEnteringCondenserFluidTemperature, referenceCoolingModeEnteringCondenserFluidTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceCoolingModeLeavingCondenserWaterTemperature, referenceCoolingModeLeavingCondenserWaterTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingCapacityRatio, referenceHeatingModeCoolingCapacityRatio);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeCoolingPowerInputRatio, referenceHeatingModeCoolingPowerInputRatio);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingChilledWaterTemperature, referenceHeatingModeLeavingChilledWaterTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeLeavingCondenserWaterTemperature, referenceHeatingModeLeavingCondenserWaterTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::ReferenceHeatingModeEnteringCondenserFluidTemperature, referenceHeatingModeEnteringCondenserFluidTemperature);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeEnteringChilledWaterTemperatureLowLimit, heatingModeEnteringChilledWaterTemperatureLowLimit);
    OS_ASSERT(result);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setChilledWaterFlowModeType(std::string const & chilledWaterFlowModeType) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType, chilledWaterFlowModeType);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate) {
    bool result(false);
    if (designChilledWaterFlowRate) {
      result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, designChilledWaterFlowRate.get());
    }
    else {
      resetDesignChilledWaterFlowRate();
      result = true;
    }
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::resetDesignChilledWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosizeDesignChilledWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignChilledWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate) {
    bool result(false);
    if (designCondenserWaterFlowRate) {
      result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, designCondenserWaterFlowRate.get());
    }
    else {
      resetDesignCondenserWaterFlowRate();
      result = true;
    }
    return result;
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::resetDesignCondenserWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosizeDesignCondenserWaterFlowRate() {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignCondenserWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setDesignHotWaterFlowRate(double designHotWaterFlowRate) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::DesignHotWaterFlowRate, designHotWaterFlowRate);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCompressorMotorEfficiency(double compressorMotorEfficiency) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CompressorMotorEfficiency, compressorMotorEfficiency);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCondenserType(std::string const & condenserType) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::CondenserType, condenserType);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(std::string const & coolingModeTemperatureCurveCondenserWaterIndependentVariable) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable, coolingModeTemperatureCurveCondenserWaterIndependentVariable);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityOptimumPartLoadRatio, coolingModeCoolingCapacityOptimumPartLoadRatio);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(std::string const & heatingModeTemperatureCurveCondenserWaterIndependentVariable) {
    bool result = setString(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable, heatingModeTemperatureCurveCondenserWaterIndependentVariable);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName, curve.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve) {
    bool result = setPointer(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName, curve.handle());
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityOptimumPartLoadRatio, heatingModeCoolingCapacityOptimumPartLoadRatio);
    return result;
  }

  bool ChillerHeaterPerformanceElectricEIR_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_ChillerHeaterPerformance_Electric_EIRFields::SizingFactor, sizingFactor);
    return result;
  }

  boost::optional<Curve> ChillerHeaterPerformanceElectricEIR_Impl::optionalCoolingModeCoolingCapacityFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> ChillerHeaterPerformanceElectricEIR_Impl::optionalCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> ChillerHeaterPerformanceElectricEIR_Impl::optionalCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName);
  }

  boost::optional<Curve> ChillerHeaterPerformanceElectricEIR_Impl::optionalHeatingModeCoolingCapacityFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeCoolingCapacityFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> ChillerHeaterPerformanceElectricEIR_Impl::optionalHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurveName);
  }

  boost::optional<Curve> ChillerHeaterPerformanceElectricEIR_Impl::optionalHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurveName);
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::autosizedReferenceCoolingModeEvaporatorCapacity() const {
    return getAutosizedValue("Design Size Reference Cooling Mode Evaporator Capacity", "W");
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::autosizedDesignChilledWaterFlowRate() const {
    return getAutosizedValue("Design Size Design Chilled Water Flow Rate", "m3/s");
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR_Impl::autosizedDesignCondenserWaterFlowRate() const {
    return getAutosizedValue("Design Size Design Condenser Water Flow Rate", "m3/s");
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::autosize() {
    autosizeReferenceCoolingModeEvaporatorCapacity();
    autosizeDesignChilledWaterFlowRate();
    autosizeDesignCondenserWaterFlowRate();
  }

  void ChillerHeaterPerformanceElectricEIR_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedReferenceCoolingModeEvaporatorCapacity();
    if (val) {
      setReferenceCoolingModeEvaporatorCapacity(val.get());
    }

    val = autosizedDesignChilledWaterFlowRate();
    if (val) {
      setDesignChilledWaterFlowRate(val.get());
    }

    val = autosizedDesignCondenserWaterFlowRate();
    if (val) {
      setDesignCondenserWaterFlowRate(val.get());
    }
  }

  // Convenience functions to return parent CentralHeatPumpSystem and CentralHeatPumpSystemModule
/*
 *  boost::optional<CentralHeatPumpSystemModule> ChillerHeaterPerformanceElectricEIR_Impl::centralHeatPumpSystemModule() const {
 *
 *    boost::optional<CentralHeatPumpSystemModule> result;
 *    for ( const CentralHeatPumpSystemModule& central_hp_module : this->model().getConcreteModelObjects<CentralHeatPumpSystemModule>() )
 *    {
 *      ChillerHeaterPerformanceElectricEIR chiller = central_hp_module.chillerHeaterModulesPerformanceComponent();
 *        if (chiller.handle() == this->handle())
 *        {
 *          result = central_hp_module;
 *        }
 *    }
 *    return result;
 *  }
 */

/*
 *  boost::optional<CentralHeatPumpSystem> ChillerHeaterPerformanceElectricEIR_Impl::centralHeatPumpSystem() const {
 *
 *    boost::optional<CentralHeatPumpSystem> result;
 *
 *    // Check if this chiller has a CentralHeatPumpSystemModule
 *    if ( boost::optional<CentralHeatPumpSystemModule> this_central_hp_module = this->centralHeatPumpSystemModule() )
 *    {
 *      // TODO: use the CentralHeatPumpSystemModule::centralHeatPumpSystem() method instead
 *      if ( boost::optional<CentralHeatPumpSystem> central_hp = this_central_hp_module->centralHeatPumpSystem() )
 *      {
 *        result = central_hp.get();
 *      }
 *
 *      // loop on all CentralHeatPumpSystems in the model
 *      //for ( const CentralHeatPumpSystem& central_hp : this->model().getConcreteModelObjects<CentralHeatPumpSystem>() )
 *      //{
 *        //// Loop on each CentralHeatPumpSystemModules
 *        //for (const CentralHeatPumpSystemModule& central_hp_module : central_hp.modules() )
 *        //{
 *          //if ( central_hp_module.handle() == this_central_hp_module->handle() )
 *          //{
 *              //result = central_hp;
 *          //}
 *        //}
 *      //}
 *    }
 *    return result;
 *  }
 */


  // Convenience functions to return a vector of parent CentralHeatPumpSystemModules
  std::vector<CentralHeatPumpSystemModule> ChillerHeaterPerformanceElectricEIR_Impl::centralHeatPumpSystemModules() const {

    std::vector<CentralHeatPumpSystemModule> result;
    // Loop on all CentralHeatPumpSystemModules
    for ( const CentralHeatPumpSystemModule& central_hp_module : this->model().getConcreteModelObjects<CentralHeatPumpSystemModule>() )
    {
        // If the ChillerHeaterPerformanceElectricEIR is the same as this one, add it to the result
        ChillerHeaterPerformanceElectricEIR chiller = central_hp_module.chillerHeaterModulesPerformanceComponent();
        if (chiller.handle() == this->handle())
        {
          result.push_back(central_hp_module);
        }
    }
    return result;
  }


  std::vector<CentralHeatPumpSystem> ChillerHeaterPerformanceElectricEIR_Impl::centralHeatPumpSystems() const {

    std::vector<CentralHeatPumpSystem> result;

    // TODO: cleanup
    //EITHER
    //
    // Loop on all parent CentralHeatPumpSystemModules
    /*
     *for (const CentralHeatPumpSystemModule& mod : this->centralHeatPumpSystemModules() )
     *{
     *  // Use the CentralHeatPumpSystemModule::centralHeatPumpSystem() method instead
     *  // If there's one, add that to result vector
     *  if ( boost::optional<CentralHeatPumpSystem> central_hp = mod.centralHeatPumpSystem() )
     *  {
     *    result.push_back(central_hp.get()):
     *  }
     *}
     */

    // OR
    //
    // loop on all CentralHeatPumpSystems in the model
    for ( const CentralHeatPumpSystem& central_hp : this->model().getConcreteModelObjects<CentralHeatPumpSystem>() )
    {
      // flag to avoid pushing it several times (a CentralHP can have multiple Mods that reference this chillerPerf)
      bool is_added = false;

      // Loop on each CentralHeatPumpSystemModules
      for (const CentralHeatPumpSystemModule& central_hp_module : central_hp.modules() )
      {
        if (!is_added) {

          ChillerHeaterPerformanceElectricEIR chiller = central_hp_module.chillerHeaterModulesPerformanceComponent();

          if ( chiller.handle() == this->handle() ) {
            result.push_back(central_hp);
            is_added = true;
          }
        }
      }
    }

    return result;
  }

} // detail

ChillerHeaterPerformanceElectricEIR::ChillerHeaterPerformanceElectricEIR(const Model& model)
  : ParentObject(ChillerHeaterPerformanceElectricEIR::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>());

  CurveBiquadratic chillerHeaterClgCapFT(model);
  chillerHeaterClgCapFT.setCoefficient1Constant(0.950829);
  chillerHeaterClgCapFT.setCoefficient2x(3.419327E-02);
  chillerHeaterClgCapFT.setCoefficient3xPOW2(2.66642E-04);
  chillerHeaterClgCapFT.setCoefficient4y(-1.733397E-03);
  chillerHeaterClgCapFT.setCoefficient5yPOW2(-1.762417E-04);
  chillerHeaterClgCapFT.setCoefficient6xTIMESY(-3.69198E-05);
  chillerHeaterClgCapFT.setMinimumValueofx(4.44);
  chillerHeaterClgCapFT.setMaximumValueofx(12.78);
  chillerHeaterClgCapFT.setMinimumValueofy(12.78);
  chillerHeaterClgCapFT.setMaximumValueofy(29.44);
  chillerHeaterClgCapFT.setInputUnitTypeforX("Temperature");
  chillerHeaterClgCapFT.setInputUnitTypeforY("Temperature");
  chillerHeaterClgCapFT.setOutputUnitType("Dimensionless");

  CurveBiquadratic chillerHeaterClgEIRFT(model);
  chillerHeaterClgEIRFT.setCoefficient1Constant(0.7362431);
  chillerHeaterClgEIRFT.setCoefficient2x(2.136491E-02);
  chillerHeaterClgEIRFT.setCoefficient3xPOW2(3.638909E-04);
  chillerHeaterClgEIRFT.setCoefficient4y(-4.284947E-03);
  chillerHeaterClgEIRFT.setCoefficient5yPOW2(3.389817E-04);
  chillerHeaterClgEIRFT.setCoefficient6xTIMESY(-3.632396E-04);
  chillerHeaterClgEIRFT.setMinimumValueofx(4.44);
  chillerHeaterClgEIRFT.setMaximumValueofx(12.78);
  chillerHeaterClgEIRFT.setMinimumValueofy(12.78);
  chillerHeaterClgEIRFT.setMaximumValueofy(29.44);
  chillerHeaterClgEIRFT.setInputUnitTypeforX("Temperature");
  chillerHeaterClgEIRFT.setInputUnitTypeforY("Temperature");
  chillerHeaterClgEIRFT.setOutputUnitType("Dimensionless");

  CurveBiquadratic chillerHeaterHtgCapFT(model);
  chillerHeaterHtgCapFT.setCoefficient1Constant(0.9415266);
  chillerHeaterHtgCapFT.setCoefficient2x(5.527431E-02);
  chillerHeaterHtgCapFT.setCoefficient3xPOW2(3.573558E-04);
  chillerHeaterHtgCapFT.setCoefficient4y(1.258391E-03);
  chillerHeaterHtgCapFT.setCoefficient5yPOW2(-6.420546E-05);
  chillerHeaterHtgCapFT.setCoefficient6xTIMESY(-5.350989E-04);
  chillerHeaterHtgCapFT.setMinimumValueofx(4.44);
  chillerHeaterHtgCapFT.setMaximumValueofx(15.56);
  chillerHeaterHtgCapFT.setMinimumValueofy(25);
  chillerHeaterHtgCapFT.setMaximumValueofy(57.22);
  chillerHeaterHtgCapFT.setInputUnitTypeforX("Temperature");
  chillerHeaterHtgCapFT.setInputUnitTypeforY("Temperature");
  chillerHeaterHtgCapFT.setOutputUnitType("Dimensionless");

  CurveBiquadratic chillerHeaterHtgEIRFT(model);
  chillerHeaterHtgEIRFT.setCoefficient1Constant(0.2286246);
  chillerHeaterHtgEIRFT.setCoefficient2x(2.498714E-02);
  chillerHeaterHtgEIRFT.setCoefficient3xPOW2(-1.267106E-05);
  chillerHeaterHtgEIRFT.setCoefficient4y(9.327184E-03);
  chillerHeaterHtgEIRFT.setCoefficient5yPOW2(5.892037E-05);
  chillerHeaterHtgEIRFT.setCoefficient6xTIMESY(-3.268512E-04);
  chillerHeaterHtgEIRFT.setMinimumValueofx(4.44);
  chillerHeaterHtgEIRFT.setMaximumValueofx(15.56);
  chillerHeaterHtgEIRFT.setMinimumValueofy(25);
  chillerHeaterHtgEIRFT.setMaximumValueofy(57.22);
  chillerHeaterHtgEIRFT.setInputUnitTypeforX("Temperature");
  chillerHeaterHtgEIRFT.setInputUnitTypeforY("Temperature");
  chillerHeaterHtgEIRFT.setOutputUnitType("Dimensionless");

  CurveBicubic chillerHeaterClgEIRFPLR(model);
  chillerHeaterClgEIRFPLR.setCoefficient1Constant(0.0);
  chillerHeaterClgEIRFPLR.setCoefficient2x(1.22895);
  chillerHeaterClgEIRFPLR.setCoefficient3xPOW2(-0.751383);
  chillerHeaterClgEIRFPLR.setCoefficient7xPOW3(0.517396);
  chillerHeaterClgEIRFPLR.setMinimumValueofx(0.2);
  chillerHeaterClgEIRFPLR.setMaximumValueofx(1);

  CurveBicubic chillerHeaterHtgEIRFPLR(model);
  chillerHeaterHtgEIRFPLR.setCoefficient1Constant(0.0);
  chillerHeaterHtgEIRFPLR.setCoefficient2x(1.12853);
  chillerHeaterHtgEIRFPLR.setCoefficient3xPOW2(-0.0264962);
  chillerHeaterHtgEIRFPLR.setCoefficient7xPOW3(-0.103811);
  chillerHeaterHtgEIRFPLR.setMinimumValueofx(0.3);
  chillerHeaterHtgEIRFPLR.setMaximumValueofx(1);

  bool ok = true;
  autosizeReferenceCoolingModeEvaporatorCapacity();
  autosizeDesignCondenserWaterFlowRate();
  autosizeDesignChilledWaterFlowRate();
  ok = setReferenceCoolingModeCOP( 1.5 );
  OS_ASSERT(ok);
  setReferenceCoolingModeLeavingChilledWaterTemperature( 6.67 );
  setReferenceCoolingModeEnteringCondenserFluidTemperature( 29.4 );
  setReferenceCoolingModeLeavingCondenserWaterTemperature( 35.0 );
  setReferenceHeatingModeCoolingCapacityRatio( 0.74 );
  ok = setReferenceHeatingModeCoolingPowerInputRatio( 1.38 );
  OS_ASSERT(ok);
  setReferenceHeatingModeLeavingChilledWaterTemperature( 6.67 );
  setReferenceHeatingModeLeavingCondenserWaterTemperature( 60 );
  setReferenceHeatingModeEnteringCondenserFluidTemperature( 29.44 );
  setHeatingModeEnteringChilledWaterTemperatureLowLimit( 5 );
  ok = setChilledWaterFlowModeType( "VariableFlow" );
  OS_ASSERT(ok);
  ok = setDesignHotWaterFlowRate( 0.0003525 );
  OS_ASSERT(ok);
  ok = setCompressorMotorEfficiency( 1 );
  OS_ASSERT(ok);
  ok = setCondenserType( "WaterCooled" );
  OS_ASSERT(ok);
  ok = setCoolingModeTemperatureCurveCondenserWaterIndependentVariable( "EnteringCondenser" );
  OS_ASSERT(ok);
  ok = setCoolingModeCoolingCapacityFunctionOfTemperatureCurve( chillerHeaterClgCapFT );
  OS_ASSERT(ok);
  ok = setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve( chillerHeaterClgEIRFT );
  OS_ASSERT(ok);
  ok = setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve( chillerHeaterClgEIRFPLR );
  OS_ASSERT(ok);
  ok = setCoolingModeCoolingCapacityOptimumPartLoadRatio( 1 );
  OS_ASSERT(ok);
  ok = setHeatingModeTemperatureCurveCondenserWaterIndependentVariable( "LeavingCondenser" );
  OS_ASSERT(ok);
  ok = setHeatingModeCoolingCapacityFunctionOfTemperatureCurve( chillerHeaterHtgCapFT );
  OS_ASSERT(ok);
  ok = setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve( chillerHeaterHtgEIRFT );
  OS_ASSERT(ok);
  ok = setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve( chillerHeaterHtgEIRFPLR );
  OS_ASSERT(ok);
  ok = setHeatingModeCoolingCapacityOptimumPartLoadRatio( 1 );
  OS_ASSERT(ok);
  ok = setSizingFactor( 1 );
  OS_ASSERT(ok);
}

ChillerHeaterPerformanceElectricEIR::ChillerHeaterPerformanceElectricEIR(
  const Model& model,
  const Curve& chillerHeaterClgCapFT,
  const Curve& chillerHeaterClgEIRFT,
  const Curve& chillerHeaterClgEIRFPLR,
  const Curve& chillerHeaterHtgCapFT,
  const Curve& chillerHeaterHtgEIRFT,
  const Curve& chillerHeaterHtgEIRFPLR
  )
  : ParentObject(ChillerHeaterPerformanceElectricEIR::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>());

  bool ok = true;
  autosizeReferenceCoolingModeEvaporatorCapacity();
  autosizeDesignCondenserWaterFlowRate();
  autosizeDesignChilledWaterFlowRate();
  ok = setReferenceCoolingModeCOP( 1.5 );
  OS_ASSERT(ok);
  setReferenceCoolingModeLeavingChilledWaterTemperature( 6.67 );
  setReferenceCoolingModeEnteringCondenserFluidTemperature( 29.4 );
  setReferenceCoolingModeLeavingCondenserWaterTemperature( 35.0 );
  setReferenceHeatingModeCoolingCapacityRatio( 0.74 );
  ok = setReferenceHeatingModeCoolingPowerInputRatio( 1.38 );
  OS_ASSERT(ok);
  setReferenceHeatingModeLeavingChilledWaterTemperature( 6.67 );
  setReferenceHeatingModeLeavingCondenserWaterTemperature( 60 );
  setReferenceHeatingModeEnteringCondenserFluidTemperature( 29.44 );
  setHeatingModeEnteringChilledWaterTemperatureLowLimit( 5 );
  ok = setChilledWaterFlowModeType( "VariableFlow" );
  OS_ASSERT(ok);
  ok = setDesignHotWaterFlowRate( 0.0003525 );
  OS_ASSERT(ok);
  ok = setCompressorMotorEfficiency( 1 );
  OS_ASSERT(ok);
  ok = setCondenserType( "WaterCooled" );
  OS_ASSERT(ok);
  ok = setCoolingModeTemperatureCurveCondenserWaterIndependentVariable( "EnteringCondenser" );
  OS_ASSERT(ok);
  ok = setCoolingModeCoolingCapacityFunctionOfTemperatureCurve( chillerHeaterClgCapFT );
  OS_ASSERT(ok);
  ok = setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve( chillerHeaterClgEIRFT );
  OS_ASSERT(ok);
  ok = setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve( chillerHeaterClgEIRFPLR );
  OS_ASSERT(ok);
  ok = setCoolingModeCoolingCapacityOptimumPartLoadRatio( 1 );
  OS_ASSERT(ok);
  ok = setHeatingModeTemperatureCurveCondenserWaterIndependentVariable( "LeavingCondenser" );
  OS_ASSERT(ok);
  ok = setHeatingModeCoolingCapacityFunctionOfTemperatureCurve( chillerHeaterHtgCapFT );
  OS_ASSERT(ok);
  ok = setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve( chillerHeaterHtgEIRFT );
  OS_ASSERT(ok);
  ok = setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve( chillerHeaterHtgEIRFPLR );
  OS_ASSERT(ok);
  ok = setHeatingModeCoolingCapacityOptimumPartLoadRatio( 1 );
  OS_ASSERT(ok);
  ok = setSizingFactor( 1 );
  OS_ASSERT(ok);
}

IddObjectType ChillerHeaterPerformanceElectricEIR::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ChillerHeaterPerformance_Electric_EIR);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::chilledWaterFlowModeTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::ChilledWaterFlowModeType);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::condenserTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::CondenserType);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::coolingModeTemperatureCurveCondenserWaterIndependentVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::CoolingModeTemperatureCurveCondenserWaterIndependentVariable);
}

std::vector<std::string> ChillerHeaterPerformanceElectricEIR::heatingModeTemperatureCurveCondenserWaterIndependentVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ChillerHeaterPerformance_Electric_EIRFields::HeatingModeTemperatureCurveCondenserWaterIndependentVariable);
}

boost::optional<double> ChillerHeaterPerformanceElectricEIR::referenceCoolingModeEvaporatorCapacity() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeEvaporatorCapacity();
}

bool ChillerHeaterPerformanceElectricEIR::isReferenceCoolingModeEvaporatorCapacityAutosized() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->isReferenceCoolingModeEvaporatorCapacityAutosized();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeCOP() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeCOP();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeLeavingChilledWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeLeavingChilledWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeEnteringCondenserFluidTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeEnteringCondenserFluidTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceCoolingModeLeavingCondenserWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceCoolingModeLeavingCondenserWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeCoolingCapacityRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeCoolingCapacityRatio();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeCoolingPowerInputRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeCoolingPowerInputRatio();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeLeavingChilledWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeLeavingChilledWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeLeavingCondenserWaterTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeLeavingCondenserWaterTemperature();
}

double ChillerHeaterPerformanceElectricEIR::referenceHeatingModeEnteringCondenserFluidTemperature() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->referenceHeatingModeEnteringCondenserFluidTemperature();
}

double ChillerHeaterPerformanceElectricEIR::heatingModeEnteringChilledWaterTemperatureLowLimit() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeEnteringChilledWaterTemperatureLowLimit();
}

std::string ChillerHeaterPerformanceElectricEIR::chilledWaterFlowModeType() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->chilledWaterFlowModeType();
}

boost::optional<double> ChillerHeaterPerformanceElectricEIR::designChilledWaterFlowRate() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->designChilledWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::isDesignChilledWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->isDesignChilledWaterFlowRateAutosized();
}

boost::optional<double> ChillerHeaterPerformanceElectricEIR::designCondenserWaterFlowRate() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->designCondenserWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::isDesignCondenserWaterFlowRateAutosized() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->isDesignCondenserWaterFlowRateAutosized();
}

double ChillerHeaterPerformanceElectricEIR::designHotWaterFlowRate() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->designHotWaterFlowRate();
}

double ChillerHeaterPerformanceElectricEIR::compressorMotorEfficiency() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->compressorMotorEfficiency();
}

std::string ChillerHeaterPerformanceElectricEIR::condenserType() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->condenserType();
}

std::string ChillerHeaterPerformanceElectricEIR::coolingModeTemperatureCurveCondenserWaterIndependentVariable() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeTemperatureCurveCondenserWaterIndependentVariable();
}

Curve ChillerHeaterPerformanceElectricEIR::coolingModeCoolingCapacityFunctionOfTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeCoolingCapacityFunctionOfTemperatureCurve();
}

Curve ChillerHeaterPerformanceElectricEIR::coolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve();
}

Curve ChillerHeaterPerformanceElectricEIR::coolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve();
}

double ChillerHeaterPerformanceElectricEIR::coolingModeCoolingCapacityOptimumPartLoadRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->coolingModeCoolingCapacityOptimumPartLoadRatio();
}

std::string ChillerHeaterPerformanceElectricEIR::heatingModeTemperatureCurveCondenserWaterIndependentVariable() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeTemperatureCurveCondenserWaterIndependentVariable();
}

Curve ChillerHeaterPerformanceElectricEIR::heatingModeCoolingCapacityFunctionOfTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeCoolingCapacityFunctionOfTemperatureCurve();
}

Curve ChillerHeaterPerformanceElectricEIR::heatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve();
}

Curve ChillerHeaterPerformanceElectricEIR::heatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve();
}

double ChillerHeaterPerformanceElectricEIR::heatingModeCoolingCapacityOptimumPartLoadRatio() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->heatingModeCoolingCapacityOptimumPartLoadRatio();
}

double ChillerHeaterPerformanceElectricEIR::sizingFactor() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->sizingFactor();
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeEvaporatorCapacity(double referenceCoolingModeEvaporatorCapacity) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeEvaporatorCapacity(referenceCoolingModeEvaporatorCapacity);
}

void ChillerHeaterPerformanceElectricEIR::autosizeReferenceCoolingModeEvaporatorCapacity() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizeReferenceCoolingModeEvaporatorCapacity();
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeCOP(double referenceCoolingModeCOP) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeCOP(referenceCoolingModeCOP);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeLeavingChilledWaterTemperature(referenceCoolingModeLeavingChilledWaterTemperature);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeEnteringCondenserFluidTemperature(referenceCoolingModeEnteringCondenserFluidTemperature);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceCoolingModeLeavingCondenserWaterTemperature(referenceCoolingModeLeavingCondenserWaterTemperature);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeCoolingCapacityRatio(referenceHeatingModeCoolingCapacityRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeCoolingPowerInputRatio(referenceHeatingModeCoolingPowerInputRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeLeavingChilledWaterTemperature(referenceHeatingModeLeavingChilledWaterTemperature);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeLeavingCondenserWaterTemperature(referenceHeatingModeLeavingCondenserWaterTemperature);
}

bool ChillerHeaterPerformanceElectricEIR::setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setReferenceHeatingModeEnteringCondenserFluidTemperature(referenceHeatingModeEnteringCondenserFluidTemperature);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeEnteringChilledWaterTemperatureLowLimit(heatingModeEnteringChilledWaterTemperatureLowLimit);
}

bool ChillerHeaterPerformanceElectricEIR::setChilledWaterFlowModeType(const std::string& chilledWaterFlowModeType) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setChilledWaterFlowModeType(chilledWaterFlowModeType);
}

bool ChillerHeaterPerformanceElectricEIR::setDesignChilledWaterFlowRate(double designChilledWaterFlowRate) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setDesignChilledWaterFlowRate(designChilledWaterFlowRate);
}

void ChillerHeaterPerformanceElectricEIR::resetDesignChilledWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->resetDesignChilledWaterFlowRate();
}

void ChillerHeaterPerformanceElectricEIR::autosizeDesignChilledWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizeDesignChilledWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setDesignCondenserWaterFlowRate(designCondenserWaterFlowRate);
}

void ChillerHeaterPerformanceElectricEIR::resetDesignCondenserWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->resetDesignCondenserWaterFlowRate();
}

void ChillerHeaterPerformanceElectricEIR::autosizeDesignCondenserWaterFlowRate() {
  getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizeDesignCondenserWaterFlowRate();
}

bool ChillerHeaterPerformanceElectricEIR::setDesignHotWaterFlowRate(double designHotWaterFlowRate) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setDesignHotWaterFlowRate(designHotWaterFlowRate);
}

bool ChillerHeaterPerformanceElectricEIR::setCompressorMotorEfficiency(double compressorMotorEfficiency) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCompressorMotorEfficiency(compressorMotorEfficiency);
}

bool ChillerHeaterPerformanceElectricEIR::setCondenserType(const std::string& condenserType) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCondenserType(condenserType);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(const std::string& coolingModeTemperatureCurveCondenserWaterIndependentVariable) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(coolingModeTemperatureCurveCondenserWaterIndependentVariable);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeCoolingCapacityFunctionOfTemperatureCurve(curve);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(curve);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(curve);
}

bool ChillerHeaterPerformanceElectricEIR::setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setCoolingModeCoolingCapacityOptimumPartLoadRatio(coolingModeCoolingCapacityOptimumPartLoadRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(const std::string& heatingModeTemperatureCurveCondenserWaterIndependentVariable) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(heatingModeTemperatureCurveCondenserWaterIndependentVariable);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeCoolingCapacityFunctionOfTemperatureCurve(curve);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(curve);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(curve);
}

bool ChillerHeaterPerformanceElectricEIR::setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setHeatingModeCoolingCapacityOptimumPartLoadRatio(heatingModeCoolingCapacityOptimumPartLoadRatio);
}

bool ChillerHeaterPerformanceElectricEIR::setSizingFactor(double sizingFactor) {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->setSizingFactor(sizingFactor);
}

// Reverse lookups
std::vector<CentralHeatPumpSystemModule> ChillerHeaterPerformanceElectricEIR::centralHeatPumpSystemModules() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->centralHeatPumpSystemModules();
}

std::vector<CentralHeatPumpSystem> ChillerHeaterPerformanceElectricEIR::centralHeatPumpSystems() const {
  return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->centralHeatPumpSystems();
}

/// @cond
ChillerHeaterPerformanceElectricEIR::ChillerHeaterPerformanceElectricEIR(std::shared_ptr<detail::ChillerHeaterPerformanceElectricEIR_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

  boost::optional<double> ChillerHeaterPerformanceElectricEIR::autosizedReferenceCoolingModeEvaporatorCapacity() const {
    return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizedReferenceCoolingModeEvaporatorCapacity();
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR::autosizedDesignChilledWaterFlowRate() const {
    return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizedDesignChilledWaterFlowRate();
  }

  boost::optional<double> ChillerHeaterPerformanceElectricEIR::autosizedDesignCondenserWaterFlowRate() const {
    return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosizedDesignCondenserWaterFlowRate();
  }

  void ChillerHeaterPerformanceElectricEIR::autosize() {
    return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->autosize();
  }

  void ChillerHeaterPerformanceElectricEIR::applySizingValues() {
    return getImpl<detail::ChillerHeaterPerformanceElectricEIR_Impl>()->applySizingValues();
  }

} // model
} // openstudio
