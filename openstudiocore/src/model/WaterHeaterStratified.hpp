/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_WATERHEATERSTRATIFIED_HPP
#define MODEL_WATERHEATERSTRATIFIED_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  class WaterHeaterStratified_Impl;

} // detail

/** WaterHeaterStratified is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:Stratified'. */
class MODEL_API WaterHeaterStratified : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WaterHeaterStratified(const Model& model);

  virtual ~WaterHeaterStratified() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> tankShapeValues();

  static std::vector<std::string> heaterPriorityControlValues();

  static std::vector<std::string> heaterFuelTypeValues();

  static std::vector<std::string> offCycleParasiticFuelTypeValues();

  static std::vector<std::string> onCycleParasiticFuelTypeValues();

  static std::vector<std::string> ambientTemperatureIndicatorValues();

  static std::vector<std::string> inletModeValues();

  static std::vector<std::string> sourceSideFlowControlModeValues();

  /** @name Getters */
  //@{

  std::string endUseSubcategory() const;

  boost::optional<double> tankVolume() const;

  bool isTankVolumeAutosized() const;

  boost::optional<double> tankHeight() const;

  bool isTankHeightAutosized() const;

  std::string tankShape() const;

  boost::optional<double> tankPerimeter() const;

  double maximumTemperatureLimit() const;

  std::string heaterPriorityControl() const;

  Schedule heater1SetpointTemperatureSchedule() const;

  double heater1DeadbandTemperatureDifference() const;

  boost::optional<double> heater1Capacity() const;

  bool isHeater1CapacityAutosized() const;

  double heater1Height() const;

  Schedule heater2SetpointTemperatureSchedule() const;

  double heater2DeadbandTemperatureDifference() const;

  double heater2Capacity() const;

  double heater2Height() const;

  std::string heaterFuelType() const;

  double heaterThermalEfficiency() const;

  double offCycleParasiticFuelConsumptionRate() const;

  std::string offCycleParasiticFuelType() const;

  double offCycleParasiticHeatFractiontoTank() const;

  double offCycleParasiticHeight() const;

  double onCycleParasiticFuelConsumptionRate() const;

  std::string onCycleParasiticFuelType() const;

  double onCycleParasiticHeatFractiontoTank() const;

  double onCycleParasiticHeight() const;

  std::string ambientTemperatureIndicator() const;

  boost::optional<Schedule> ambientTemperatureSchedule() const;

  boost::optional<ThermalZone> ambientTemperatureThermalZone() const;

  boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

  boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const;

  double skinLossFractiontoZone() const;

  boost::optional<double> offCycleFlueLossCoefficienttoAmbientTemperature() const;

  double offCycleFlueLossFractiontoZone() const;

  boost::optional<double> peakUseFlowRate() const;

  boost::optional<Schedule> useFlowRateFractionSchedule() const;

  boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

  double useSideEffectiveness() const;

  double useSideInletHeight() const;

  boost::optional<double> useSideOutletHeight() const;

  bool isUseSideOutletHeightAutocalculated() const;

  double sourceSideEffectiveness() const;

  boost::optional<double> sourceSideInletHeight() const;

  bool isSourceSideInletHeightAutocalculated() const;

  double sourceSideOutletHeight() const;

  std::string inletMode() const;

  boost::optional<double> useSideDesignFlowRate() const;

  bool isUseSideDesignFlowRateAutosized() const;

  boost::optional<double> sourceSideDesignFlowRate() const;

  bool isSourceSideDesignFlowRateAutosized() const;

  double indirectWaterHeatingRecoveryTime() const;

  int numberofNodes() const;

  double additionalDestratificationConductivity() const;

  double node1AdditionalLossCoefficient() const;

  double node2AdditionalLossCoefficient() const;

  double node3AdditionalLossCoefficient() const;

  double node4AdditionalLossCoefficient() const;

  double node5AdditionalLossCoefficient() const;

  double node6AdditionalLossCoefficient() const;

  double node7AdditionalLossCoefficient() const;

  double node8AdditionalLossCoefficient() const;

  double node9AdditionalLossCoefficient() const;

  double node10AdditionalLossCoefficient() const;

  double node11AdditionalLossCoefficient() const;

  double node12AdditionalLossCoefficient() const;

  std::string sourceSideFlowControlMode() const;

  boost::optional<Schedule> indirectAlternateSetpointTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setEndUseSubcategory(std::string endUseSubcategory);

  bool setTankVolume(double tankVolume);

  void autosizeTankVolume();

  bool setTankHeight(double tankHeight);

  void autosizeTankHeight();

  bool setTankShape(std::string tankShape);

  bool setTankPerimeter(double tankPerimeter);

  void resetTankPerimeter();

  bool setMaximumTemperatureLimit(double maximumTemperatureLimit);

  bool setHeaterPriorityControl(std::string heaterPriorityControl);

  bool setHeater1SetpointTemperatureSchedule(Schedule& schedule);

  bool setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference);

  bool setHeater1Capacity(double heater1Capacity);

  void autosizeHeater1Capacity();

  bool setHeater1Height(double heater1Height);

  bool setHeater2SetpointTemperatureSchedule(Schedule& schedule);

  bool setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference);

  bool setHeater2Capacity(double heater2Capacity);

  bool setHeater2Height(double heater2Height);

  bool setHeaterFuelType(std::string heaterFuelType);

  bool setHeaterThermalEfficiency(double heaterThermalEfficiency);

  bool setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate);

  bool setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType);

  bool setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank);

  bool setOffCycleParasiticHeight(double offCycleParasiticHeight);

  bool setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate);

  bool setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType);

  bool setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank);

  bool setOnCycleParasiticHeight(double onCycleParasiticHeight);

  bool setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator);

  bool setAmbientTemperatureSchedule(Schedule& schedule);

  void resetAmbientTemperatureSchedule();

  bool setAmbientTemperatureThermalZone(const ThermalZone& thermalZone);

  void resetAmbientTemperatureThermalZone();

  bool setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName);

  void resetAmbientTemperatureOutdoorAirNodeName();

  bool setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(double uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);

  void resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();

  bool setSkinLossFractiontoZone(double skinLossFractiontoZone);

  bool setOffCycleFlueLossCoefficienttoAmbientTemperature(double offCycleFlueLossCoefficienttoAmbientTemperature);

  void resetOffCycleFlueLossCoefficienttoAmbientTemperature();

  bool setOffCycleFlueLossFractiontoZone(double offCycleFlueLossFractiontoZone);

  bool setPeakUseFlowRate(double peakUseFlowRate);

  void resetPeakUseFlowRate();

  bool setUseFlowRateFractionSchedule(Schedule& schedule);

  void resetUseFlowRateFractionSchedule();

  bool setColdWaterSupplyTemperatureSchedule(Schedule& schedule);

  void resetColdWaterSupplyTemperatureSchedule();

  bool setUseSideEffectiveness(double useSideEffectiveness);

  bool setUseSideInletHeight(double useSideInletHeight);

  bool setUseSideOutletHeight(double useSideOutletHeight);

  void autocalculateUseSideOutletHeight();

  bool setSourceSideEffectiveness(double sourceSideEffectiveness);

  bool setSourceSideInletHeight(double sourceSideInletHeight);

  void autocalculateSourceSideInletHeight();

  bool setSourceSideOutletHeight(double sourceSideOutletHeight);

  bool setInletMode(std::string inletMode);

  bool setUseSideDesignFlowRate(double useSideDesignFlowRate);

  void autosizeUseSideDesignFlowRate();

  bool setSourceSideDesignFlowRate(double sourceSideDesignFlowRate);

  void autosizeSourceSideDesignFlowRate();

  bool setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime);

  bool setNumberofNodes(int numberofNodes);

  bool setAdditionalDestratificationConductivity(double additionalDestratificationConductivity);

  bool setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient);

  bool setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient);

  bool setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient);

  bool setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient);

  bool setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient);

  bool setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient);

  bool setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient);

  bool setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient);

  bool setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient);

  bool setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient);

  bool setNode11AdditionalLossCoefficient(double node11AdditionalLossCoefficient);

  bool setNode12AdditionalLossCoefficient(double node12AdditionalLossCoefficient);

  bool setSourceSideFlowControlMode(std::string sourceSideFlowControlMode);

  bool setIndirectAlternateSetpointTemperatureSchedule(Schedule& schedule);

  void resetIndirectAlternateSetpointTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedTankVolume() const ;

  boost::optional<double> autosizedTankHeight() const ;

  boost::optional<double> autosizedHeater1Capacity() const ;

  boost::optional<double> autosizedUseSideDesignFlowRate() const ;

  boost::optional<double> autosizedSourceSideDesignFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::WaterHeaterStratified_Impl ImplType;

  explicit WaterHeaterStratified(std::shared_ptr<detail::WaterHeaterStratified_Impl> impl);

  friend class detail::WaterHeaterStratified_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.WaterHeaterStratified");
};

/** \relates WaterHeaterStratified*/
typedef boost::optional<WaterHeaterStratified> OptionalWaterHeaterStratified;

/** \relates WaterHeaterStratified*/
typedef std::vector<WaterHeaterStratified> WaterHeaterStratifiedVector;

} // model
} // openstudio

#endif // MODEL_WATERHEATERSTRATIFIED_HPP
