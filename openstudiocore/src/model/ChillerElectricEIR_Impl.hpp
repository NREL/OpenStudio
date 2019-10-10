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

#ifndef MODEL_CHILLERELECTRICEIR_IMPL_HPP
#define MODEL_CHILLERELECTRICEIR_IMPL_HPP

#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class ChillerElectricEIR;
class CurveBiquadratic;
class  CurveQuadratic;
class Schedule;

namespace detail {

class MODEL_API ChillerElectricEIR_Impl : public WaterToWaterComponent_Impl
{

 public:

  ChillerElectricEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  ChillerElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

  ChillerElectricEIR_Impl(const ChillerElectricEIR_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

  virtual ~ChillerElectricEIR_Impl() {}

  /** @name Virtual Methods */
  //@{

  virtual ModelObject clone(Model model) const override;

  virtual std::vector<ModelObject> children() const override;

  virtual IddObjectType iddObjectType() const override;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

  virtual const std::vector<std::string> & outputVariableNames() const override;

  virtual bool addToNode(Node & node) override;

  virtual bool removeFromSecondaryPlantLoop() override;

  virtual unsigned supplyInletPort() const override;

  virtual unsigned supplyOutletPort() const override;

  virtual unsigned demandInletPort() const override;

  virtual unsigned demandOutletPort() const override;

  virtual unsigned tertiaryInletPort() const override;

  virtual unsigned tertiaryOutletPort() const override;

  virtual void autosize() override;

  virtual void applySizingValues() override;

  //@}
  boost::optional<double> referenceCapacity() const;

  bool isReferenceCapacityAutosized() const;

  double referenceCOP() const;

  double referenceLeavingChilledWaterTemperature() const;

  bool isReferenceLeavingChilledWaterTemperatureDefaulted() const;

  double referenceEnteringCondenserFluidTemperature() const;

  bool isReferenceEnteringCondenserFluidTemperatureDefaulted() const;

  boost::optional<double> referenceChilledWaterFlowRate() const;

  bool isReferenceChilledWaterFlowRateAutosized() const;

  boost::optional<double> referenceCondenserFluidFlowRate() const;

  bool isReferenceCondenserFluidFlowRateAutosized() const;

  CurveBiquadratic coolingCapacityFunctionOfTemperature() const;

  CurveBiquadratic  electricInputToCoolingOutputRatioFunctionOfTemperature() const;

  CurveQuadratic electricInputToCoolingOutputRatioFunctionOfPLR() const;

  double minimumPartLoadRatio() const;

  bool isMinimumPartLoadRatioDefaulted() const;

  double maximumPartLoadRatio() const;

  bool isMaximumPartLoadRatioDefaulted() const;

  double optimumPartLoadRatio() const;

  bool isOptimumPartLoadRatioDefaulted() const;

  double minimumUnloadingRatio() const;

  bool isMinimumUnloadingRatioDefaulted() const;

  std::string chilledWaterInletNodeName() const;

  std::string chilledWaterOutletNodeName() const;

  boost::optional<std::string> condenserInletNodeName() const;

  boost::optional<std::string> condenserOutletNodeName() const;

  std::string condenserType() const;

  bool isCondenserTypeDefaulted() const;

  double condenserFanPowerRatio() const;

  bool isCondenserFanPowerRatioDefaulted() const;

  double compressorMotorEfficiency() const;

  bool isCompressorMotorEfficiencyDefaulted() const;

  double leavingChilledWaterLowerTemperatureLimit() const;

  bool isLeavingChilledWaterLowerTemperatureLimitDefaulted() const;

  std::string chillerFlowMode() const;

  bool isChillerFlowModeDefaulted() const;

  double designHeatRecoveryWaterFlowRate() const;

  bool isDesignHeatRecoveryWaterFlowRateDefaulted() const;

  boost::optional<std::string> heatRecoveryInletNodeName() const;

  boost::optional<std::string> heatRecoveryOutletNodeName() const;

  double sizingFactor() const;

  bool isSizingFactorDefaulted() const;

  double basinHeaterCapacity() const;

  bool isBasinHeaterCapacityDefaulted() const;

  double basinHeaterSetpointTemperature() const;

  bool isBasinHeaterSetpointTemperatureDefaulted() const;

  boost::optional<Schedule> basinHeaterSchedule() const;

  boost::optional<double> autosizedReferenceCapacity() const ;

  boost::optional<double> autosizedReferenceChilledWaterFlowRate() const ;

  boost::optional<double> autosizedReferenceCondenserFluidFlowRate() const ;

  std::string endUseSubcategory() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceCapacity(boost::optional<double> referenceCapacity);

  bool setReferenceCapacity(double referenceCapacity);

  void autosizeReferenceCapacity();

  bool setReferenceCOP(double referenceCOP);

  bool setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature);

  void resetReferenceLeavingChilledWaterTemperature();

  bool setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature);

  void resetReferenceEnteringCondenserFluidTemperature();

  bool setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate);

  bool setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate);

  void resetReferenceChilledWaterFlowRate();

  void autosizeReferenceChilledWaterFlowRate();

  bool setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate);

  bool setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate);

  void resetReferenceCondenserFluidFlowRate();

  void autosizeReferenceCondenserFluidFlowRate();

  bool setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& );

  bool setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& );

  bool setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& );

  bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

  void resetMinimumPartLoadRatio();

  bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

  void resetMaximumPartLoadRatio();

  bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

  void resetOptimumPartLoadRatio();

  bool setMinimumUnloadingRatio(double minimumUnloadingRatio);

  void resetMinimumUnloadingRatio();

  bool setChilledWaterInletNodeName(std::string chilledWaterInletNodeName);

  bool setChilledWaterOutletNodeName(std::string chilledWaterOutletNodeName);

  bool setCondenserInletNodeName(boost::optional<std::string> condenserInletNodeName);

  bool setCondenserInletNodeName(std::string condenserInletNodeName);

  void resetCondenserInletNodeName();

  bool setCondenserOutletNodeName(boost::optional<std::string> condenserOutletNodeName);

  bool setCondenserOutletNodeName(std::string condenserOutletNodeName);

  void resetCondenserOutletNodeName();

  bool setCondenserType(std::string condenserType);

  void resetCondenserType();

  bool setCondenserFanPowerRatio(double condenserFanPowerRatio);

  void resetCondenserFanPowerRatio();

  bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

  void resetCompressorMotorEfficiency();

  bool setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit);

  void resetLeavingChilledWaterLowerTemperatureLimit();

  bool setChillerFlowMode(std::string chillerFlowMode);

  void resetChillerFlowMode();

  bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

  void resetDesignHeatRecoveryWaterFlowRate();

  bool setHeatRecoveryInletNodeName(boost::optional<std::string> heatRecoveryInletNodeName);

  bool setHeatRecoveryInletNodeName(std::string heatRecoveryInletNodeName);

  void resetHeatRecoveryInletNodeName();

  bool setHeatRecoveryOutletNodeName(boost::optional<std::string> heatRecoveryOutletNodeName);

  bool setHeatRecoveryOutletNodeName(std::string heatRecoveryOutletNodeName);

  void resetHeatRecoveryOutletNodeName();

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  void resetBasinHeaterCapacity();

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  void resetBasinHeaterSetpointTemperature();

  bool setBasinHeaterSchedule(Schedule & schedule );

  void resetBasinHeaterSchedule();

  bool setEndUseSubcategory(const std::string & endUseSubcategory);

  // TODO
  /*
   *N18, \field Condenser Heat Recovery Relative Capacity Fraction
   *     \note This optional field is the fraction of total rejected heat that can be recovered at full load
   *     \type real
   *     \minimum 0.0
   *     \maximum 1.0
   *A15, \field Heat Recovery Inlet High Temperature Limit Schedule Name
   *     \note This optional schedule of temperatures will turn off heat recovery if inlet exceeds the value
   *     \type object-list
   *     \object-list ScheduleNames
   *A16, \field Heat Recovery Leaving Temperature Setpoint Node Name
   */

  //@}
 protected:
 private:
  REGISTER_LOGGER("openstudio.model.ChillerElectricEIR");

  boost::optional<ModelObject> basinHeaterScheduleAsModelObject() const;

  bool setBasinHeaterScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
};

} // detail

} // model

} // openstudio

#endif // MODEL_CHILLERELECTRICEIR_IMPL_HPP
