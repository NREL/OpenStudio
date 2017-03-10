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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class HeatPumpWaterToWaterEquationFitHeating_Impl;

} // detail

/** HeatPumpWaterToWaterEquationFitHeating is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:WaterToWater:EquationFit:Heating'. */
class MODEL_API HeatPumpWaterToWaterEquationFitHeating : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HeatPumpWaterToWaterEquationFitHeating(const Model& model);

  virtual ~HeatPumpWaterToWaterEquationFitHeating() {}

  //@}

  static IddObjectType iddObjectType();

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Load Side Flow Rate" **/
  double ratedLoadSideFlowRate() const;

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Source Side Flow Rate" **/
  double ratedSourceSideFlowRate() const;

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Capacity" **/
  double ratedHeatingCapacity() const;

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Power Consumption" **/
  double ratedHeatingPowerConsumption() const;

  double heatingCapacityCoefficient1() const;

  double heatingCapacityCoefficient2() const;

  double heatingCapacityCoefficient3() const;

  double heatingCapacityCoefficient4() const;

  double heatingCapacityCoefficient5() const;

  double heatingCompressorPowerCoefficient1() const;

  double heatingCompressorPowerCoefficient2() const;

  double heatingCompressorPowerCoefficient3() const;

  double heatingCompressorPowerCoefficient4() const;

  double heatingCompressorPowerCoefficient5() const;

  //@}
  /** @name Setters */
  //@{

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Load Side Flow Rate" **/
  bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Source Side Flow Rate" **/
  bool setRatedSourceSideFlowRate(double ratedSourceSideFlowRate);

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Capacity" **/
  bool setRatedHeatingCapacity(double ratedHeatingCapacity);

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Heating Power Consumption" **/
  bool setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption);

  void setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);

  void setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);

  void setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);

  void setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);

  void setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

  void setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1);

  void setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2);

  void setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3);

  void setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4);

  void setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::HeatPumpWaterToWaterEquationFitHeating_Impl ImplType;

  explicit HeatPumpWaterToWaterEquationFitHeating(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitHeating_Impl> impl);

  friend class detail::HeatPumpWaterToWaterEquationFitHeating_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitHeating");
};

/** \relates HeatPumpWaterToWaterEquationFitHeating*/
typedef boost::optional<HeatPumpWaterToWaterEquationFitHeating> OptionalHeatPumpWaterToWaterEquationFitHeating;

/** \relates HeatPumpWaterToWaterEquationFitHeating*/
typedef std::vector<HeatPumpWaterToWaterEquationFitHeating> HeatPumpWaterToWaterEquationFitHeatingVector;

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP

