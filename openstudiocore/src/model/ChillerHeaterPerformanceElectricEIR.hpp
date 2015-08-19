/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_HPP
#define MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class BiquadraticCurves;
class BiquadraticCurves;
class BicubicCurves;
class BiquadraticCurves;
class BiquadraticCurves;
class BicubicCurves;

namespace detail {

  class ChillerHeaterPerformanceElectricEIR_Impl;

} // detail

/** ChillerHeaterPerformanceElectricEIR is a ParentObject that wraps the OpenStudio IDD object 'OS:ChillerHeaterPerformance:Electric:EIR'. */
class MODEL_API ChillerHeaterPerformanceElectricEIR : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ChillerHeaterPerformanceElectricEIR(const Model& model);

  virtual ~ChillerHeaterPerformanceElectricEIR() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> chilledWaterFlowModeTypeValues();

  static std::vector<std::string> condenserTypeValues();

  static std::vector<std::string> coolingModeTemperatureCurveCondenserWaterIndependentVariableValues();

  static std::vector<std::string> heatingModeTemperatureCurveCondenserWaterIndependentVariableValues();

  /** @name Getters */
  //@{

  boost::optional<double> referenceCoolingModeEvaporatorCapacity() const;

  bool isReferenceCoolingModeEvaporatorCapacityAutosized() const;

  double referenceCoolingModeCOP() const;

  double referenceCoolingModeLeavingChilledWaterTemperature() const;

  double referenceCoolingModeEnteringCondenserFluidTemperature() const;

  double referenceCoolingModeLeavingCondenserWaterTemperature() const;

  double referenceHeatingModeCoolingCapacityRatio() const;

  double referenceHeatingModeCoolingPowerInputRatio() const;

  double referenceHeatingModeLeavingChilledWaterTemperature() const;

  double referenceHeatingModeLeavingCondenserWaterTemperature() const;

  double referenceHeatingModeEnteringCondenserFluidTemperature() const;

  double heatingModeEnteringChilledWaterTemperatureLowLimit() const;

  std::string chilledWaterFlowModeType() const;

  boost::optional<double> designChilledWaterFlowRate() const;

  bool isDesignChilledWaterFlowRateAutosized() const;

  boost::optional<double> designCondenserWaterFlowRate() const;

  bool isDesignCondenserWaterFlowRateAutosized() const;

  double designHotWaterFlowRate() const;

  double compressorMotorEfficiency() const;

  std::string condenserType() const;

  std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable() const;

  // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  BiquadraticCurves coolingModeCoolingCapacityFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  BiquadraticCurves coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: BicubicCurves, QuadraticCurves, UniVariateTables, BiVariateTables.
  BicubicCurves coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const;

  double coolingModeCoolingCapacityOptimumPartLoadRatio() const;

  std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable() const;

  // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  BiquadraticCurves heatingModeCoolingCapacityFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  BiquadraticCurves heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: BicubicCurves, QuadraticCurves, UniVariateTables, BiVariateTables.
  BicubicCurves heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const;

  double heatingModeCoolingCapacityOptimumPartLoadRatio() const;

  double sizingFactor() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceCoolingModeEvaporatorCapacity(double referenceCoolingModeEvaporatorCapacity);

  void autosizeReferenceCoolingModeEvaporatorCapacity();

  bool setReferenceCoolingModeCOP(double referenceCoolingModeCOP);

  void setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature);

  void setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature);

  void setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature);

  void setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio);

  bool setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio);

  void setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature);

  void setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature);

  void setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature);

  void setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit);

  bool setChilledWaterFlowModeType(std::string chilledWaterFlowModeType);

  bool setDesignChilledWaterFlowRate(double designChilledWaterFlowRate);

  void resetDesignChilledWaterFlowRate();

  void autosizeDesignChilledWaterFlowRate();

  bool setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate);

  void resetDesignCondenserWaterFlowRate();

  void autosizeDesignCondenserWaterFlowRate();

  bool setDesignHotWaterFlowRate(double designHotWaterFlowRate);

  bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

  bool setCondenserType(std::string condenserType);

  bool setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable);

  // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  bool setCoolingModeCoolingCapacityFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves);

  // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  bool setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves);

  // TODO: Check argument type. From object lists, some candidates are: BicubicCurves, QuadraticCurves, UniVariateTables, BiVariateTables.
  bool setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const BicubicCurves& bicubicCurves);

  bool setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio);

  bool setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable);

  // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  bool setHeatingModeCoolingCapacityFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves);

  // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  bool setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const BiquadraticCurves& biquadraticCurves);

  // TODO: Check argument type. From object lists, some candidates are: BicubicCurves, QuadraticCurves, UniVariateTables, BiVariateTables.
  bool setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const BicubicCurves& bicubicCurves);

  bool setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio);

  bool setSizingFactor(double sizingFactor);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ChillerHeaterPerformanceElectricEIR_Impl ImplType;

  explicit ChillerHeaterPerformanceElectricEIR(std::shared_ptr<detail::ChillerHeaterPerformanceElectricEIR_Impl> impl);

  friend class detail::ChillerHeaterPerformanceElectricEIR_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ChillerHeaterPerformanceElectricEIR");
};

/** \relates ChillerHeaterPerformanceElectricEIR*/
typedef boost::optional<ChillerHeaterPerformanceElectricEIR> OptionalChillerHeaterPerformanceElectricEIR;

/** \relates ChillerHeaterPerformanceElectricEIR*/
typedef std::vector<ChillerHeaterPerformanceElectricEIR> ChillerHeaterPerformanceElectricEIRVector;

} // model
} // openstudio

#endif // MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_HPP

