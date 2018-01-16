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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

} // detail

/** CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit:SpeedData'. */
class MODEL_API CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model);

  explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
    const Model& model,
    const Curve& heatingCapacityFunctionofTemperature,
    const Curve& totalHeatingCapacityFunctionofAirFlowFraction,
    const Curve& heatingCapacityFunctionofWaterFlowFraction,
    const Curve& energyInputRatioFunctionofTemperature,
    const Curve& energyInputRatioFunctionofAirFlowFraction,
    const Curve& energyInputRatioFunctionofWaterFlowFraction,
    const Curve& wasteHeatFunctionofTemperature
  );

  virtual ~CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double referenceUnitGrossRatedHeatingCapacity() const;

  double referenceUnitGrossRatedHeatingCOP() const;

  double referenceUnitRatedAirFlow() const;

  double referenceUnitRatedWaterFlowRate() const;

  Curve heatingCapacityFunctionofTemperatureCurve() const;

  Curve totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

  Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

  Curve energyInputRatioFunctionofTemperatureCurve() const;

  Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

  Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

  double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

  Curve wasteHeatFunctionofTemperatureCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

  bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

  bool setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow);

  bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

  bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve);

  bool setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);

  bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl ImplType;

  explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl);

  friend class detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");
};

/** \relates CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
typedef boost::optional<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> OptionalCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;

/** \relates CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
typedef std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedDataVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

