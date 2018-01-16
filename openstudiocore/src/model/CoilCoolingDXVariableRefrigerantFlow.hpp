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

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;

namespace detail {

  class CoilCoolingDXVariableRefrigerantFlow_Impl;

} // detail

/** CoilCoolingDXVariableRefrigerantFlow is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableRefrigerantFlow'. */
class MODEL_API CoilCoolingDXVariableRefrigerantFlow : public HVACComponent {

 public:

  explicit CoilCoolingDXVariableRefrigerantFlow(const Model& model);

  virtual ~CoilCoolingDXVariableRefrigerantFlow() {}

  static IddObjectType iddObjectType();

  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);

  boost::optional<double> ratedTotalCoolingCapacity() const;

  bool isRatedTotalCoolingCapacityAutosized() const;

  bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

  void autosizeRatedTotalCoolingCapacity();

  boost::optional<double> ratedSensibleHeatRatio() const;

  bool isRatedSensibleHeatRatioAutosized() const;

  bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

  void autosizeRatedSensibleHeatRatio();

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  Curve coolingCapacityRatioModifierFunctionofTemperatureCurve() const;

  bool setCoolingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& lcurve);

  Curve coolingCapacityModifierCurveFunctionofFlowFraction() const;

  bool setCoolingCapacityModifierCurveFunctionofFlowFraction(const Curve& lcurve);

  boost::optional<double> autosizedRatedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedRatedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedAirFlowRate() const ;



 protected:

  /// @cond
  typedef detail::CoilCoolingDXVariableRefrigerantFlow_Impl ImplType;

  explicit CoilCoolingDXVariableRefrigerantFlow(std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlow_Impl> impl);

  friend class detail::CoilCoolingDXVariableRefrigerantFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlow");
};

/** \relates CoilCoolingDXVariableRefrigerantFlow*/
typedef boost::optional<CoilCoolingDXVariableRefrigerantFlow> OptionalCoilCoolingDXVariableRefrigerantFlow;

/** \relates CoilCoolingDXVariableRefrigerantFlow*/
typedef std::vector<CoilCoolingDXVariableRefrigerantFlow> CoilCoolingDXVariableRefrigerantFlowVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP

