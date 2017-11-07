/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_CHILLERELECTRICEIR_HPP
#define MODEL_CHILLERELECTRICEIR_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class CurveQuadratic;
class CurveBiquadratic;
class Schedule;

namespace detail {

class ChillerElectricEIR_Impl;

} // detail

/** ChillerElectricEIR is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Chiller:Electric:EIR'. */
class MODEL_API ChillerElectricEIR : public WaterToWaterComponent {
 public:

  /**
   *
   *\param model the Model to put the chiller in
   *\param CCFofT Cooling Capacity as a Function of Temp
   *\param EItoCORFofT Electric Input to Cooling Output Ratio as a Function of Temp
   *\param EItoCORFofPLR Electric Input to Cooling Output Ratio as a Function of Partial Load Ratio
   */
  explicit ChillerElectricEIR(const Model& model,
                              const CurveBiquadratic& CCFofT,
                              const CurveBiquadratic& EItoCORFofT,
                              const CurveQuadratic& EItoCORFofPLR );

  explicit ChillerElectricEIR(const Model& model);

  virtual ~ChillerElectricEIR() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validCondenserTypeValues();

  static std::vector<std::string> validCompressorTypeValues();

  static std::vector<std::string> validChillerFlowModeValues();

  /** @name Getters */
  //@{

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

  std::string compressorType() const;

  bool isCondenserTypeDefaulted() const;

  bool isCompressorTypeDefaulted() const;

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

  //@}
  /** @name Setters */
  //@{

  bool setReferenceCapacity(boost::optional<double> referenceCapacity);

  bool setReferenceCapacity(double referenceCapacity);

  void autosizeReferenceCapacity();

  bool setReferenceCOP(double referenceCOP);

  void setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature);

  void resetReferenceLeavingChilledWaterTemperature();

  void setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature);

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

  void setChilledWaterInletNodeName(std::string chilledWaterInletNodeName);

  void setChilledWaterOutletNodeName(std::string chilledWaterOutletNodeName);

  void setCondenserInletNodeName(boost::optional<std::string> condenserInletNodeName);

  void setCondenserInletNodeName(std::string condenserInletNodeName);

  void resetCondenserInletNodeName();

  void setCondenserOutletNodeName(boost::optional<std::string> condenserOutletNodeName);

  void setCondenserOutletNodeName(std::string condenserOutletNodeName);

  void resetCondenserOutletNodeName();

  bool setCondenserType(std::string condenserType);

  void resetCondenserType();

  bool setCompressorType(std::string compressorType);

  void resetCompressorType();

  bool setCondenserFanPowerRatio(double condenserFanPowerRatio);

  void resetCondenserFanPowerRatio();

  bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

  void resetCompressorMotorEfficiency();

  void setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit);

  void resetLeavingChilledWaterLowerTemperatureLimit();

  bool setChillerFlowMode(std::string chillerFlowMode);

  void resetChillerFlowMode();

  bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

  void resetDesignHeatRecoveryWaterFlowRate();

  void setHeatRecoveryInletNodeName(boost::optional<std::string> heatRecoveryInletNodeName);

  void setHeatRecoveryInletNodeName(std::string heatRecoveryInletNodeName);

  void resetHeatRecoveryInletNodeName();

  void setHeatRecoveryOutletNodeName(boost::optional<std::string> heatRecoveryOutletNodeName);

  void setHeatRecoveryOutletNodeName(std::string heatRecoveryOutletNodeName);

  void resetHeatRecoveryOutletNodeName();

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  void resetBasinHeaterCapacity();

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  void resetBasinHeaterSetpointTemperature();

  bool setBasinHeaterSchedule(Schedule & s);

  void resetBasinHeaterSchedule();
  //@}
 protected:


  /// @cond
  typedef detail::ChillerElectricEIR_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit ChillerElectricEIR(std::shared_ptr<detail::ChillerElectricEIR_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ChillerElectricEIR");
};

/** \relates ChillerElectricEIR*/
typedef boost::optional<ChillerElectricEIR> OptionalChillerElectricEIR;

/** \relates ChillerElectricEIR*/
typedef std::vector<ChillerElectricEIR> ChillerElectricEIRVector;

} // model

} // openstudio

#endif // MODEL_CHILLERELECTRICEIR_HPP

