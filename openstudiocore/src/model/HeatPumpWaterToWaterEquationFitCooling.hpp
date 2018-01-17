/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class HeatPumpWaterToWaterEquationFitHeating;

namespace detail {

  class HeatPumpWaterToWaterEquationFitCooling_Impl;

} // detail

/** HeatPumpWaterToWaterEquationFitCooling is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:WaterToWater:EquationFit:Cooling'. */
class MODEL_API HeatPumpWaterToWaterEquationFitCooling : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HeatPumpWaterToWaterEquationFitCooling(const Model& model);

  virtual ~HeatPumpWaterToWaterEquationFitCooling() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> referenceLoadSideFlowRate() const;

  /** Prior to EnergyPlus 8.7.0 this field was not autosizeable. To preserve backwards compatibility this method will return -999.0 in autosized models.**/
  double ratedLoadSideFlowRate() const;

  bool isReferenceLoadSideFlowRateAutosized() const;

  boost::optional<double> referenceSourceSideFlowRate() const;

  /** Prior to EnergyPlus 8.7.0 this field was not autosizeable. To preserve backwards compatibility this method will return -999.0 in autosized models.**/
  double ratedSourceSideFlowRate() const;

  bool isReferenceSourceSideFlowRateAutosized() const;

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Capacity" **/
  boost::optional<double> ratedCoolingCapacity() const;

  bool isRatedCoolingCapacityAutosized() const;

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Power Consumption" **/
  boost::optional<double> ratedCoolingPowerConsumption() const;

  bool isRatedCoolingPowerConsumptionAutosized() const;

  double coolingCapacityCoefficient1() const;

  double coolingCapacityCoefficient2() const;

  double coolingCapacityCoefficient3() const;

  double coolingCapacityCoefficient4() const;

  double coolingCapacityCoefficient5() const;

  double coolingCompressorPowerCoefficient1() const;

  double coolingCompressorPowerCoefficient2() const;

  double coolingCompressorPowerCoefficient3() const;

  double coolingCompressorPowerCoefficient4() const;

  double coolingCompressorPowerCoefficient5() const;

  double referenceCoefficientofPerformance() const;

  double sizingFactor() const;

  boost::optional<HeatPumpWaterToWaterEquationFitHeating> companionHeatingHeatPump() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

  /** Synonym of setReferenceLoadSideFlowRate for backwards compatibility **/
  bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

  void autosizeReferenceLoadSideFlowRate();

  bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

  /** Synonym of setReferenceLoadSideFlowRate for backwards compatibility **/
  bool setRatedSourceSideFlowRate(double ratedSourceSideFlowRate);

  void autosizeReferenceSourceSideFlowRate();

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Capacity" **/
  bool setRatedCoolingCapacity(double ratedCoolingCapacity);

  void autosizeRatedCoolingCapacity();

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Power Consumption" **/
  bool setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption);

  void autosizeRatedCoolingPowerConsumption();

  bool setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1);

  bool setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2);

  bool setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3);

  bool setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4);

  bool setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5);

  bool setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1);

  bool setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2);

  bool setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3);

  bool setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4);

  bool setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5);

  bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

  bool setSizingFactor(double sizingFactor);

  bool setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP);

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedReferenceLoadSideFlowRate() const ;

  boost::optional<double> autosizedReferenceSourceSideFlowRate() const ;

  boost::optional<double> autosizedRatedCoolingCapacity() const ;

  boost::optional<double> autosizedRatedCoolingPowerConsumption() const ;



  //@}
 protected:
  /// @cond
  typedef detail::HeatPumpWaterToWaterEquationFitCooling_Impl ImplType;

  explicit HeatPumpWaterToWaterEquationFitCooling(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitCooling_Impl> impl);

  friend class detail::HeatPumpWaterToWaterEquationFitCooling_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitCooling");
};

/** \relates HeatPumpWaterToWaterEquationFitCooling*/
typedef boost::optional<HeatPumpWaterToWaterEquationFitCooling> OptionalHeatPumpWaterToWaterEquationFitCooling;

/** \relates HeatPumpWaterToWaterEquationFitCooling*/
typedef std::vector<HeatPumpWaterToWaterEquationFitCooling> HeatPumpWaterToWaterEquationFitCoolingVector;

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP
