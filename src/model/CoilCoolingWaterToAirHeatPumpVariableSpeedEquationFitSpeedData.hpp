/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

  }  // namespace detail

  /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit:SpeedData'. */
  class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model);

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
      const Model& model, const Curve& totalCoolingCapacityFunctionofTemperature, const Curve& totalCoolingCapacityFunctionofAirFlowFraction,
      const Curve& totalCoolingCapacityFunctionofWaterFlowFraction, const Curve& energyInputRatioFunctionofTemperature,
      const Curve& energyInputRatioFunctionofAirFlowFraction, const Curve& energyInputRatioFunctionofWaterFlowFraction,
      const Curve& wasteHeatFunctionofTemperature);

    virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& other) =
      default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&& other) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&
      operator=(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&
      operator=(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double referenceUnitGrossRatedTotalCoolingCapacity() const;

    double referenceUnitGrossRatedSensibleHeatRatio() const;

    double referenceUnitGrossRatedCoolingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    double referenceUnitRatedWaterFlowRate() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

    Curve totalCoolingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

    Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

    double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

    Curve wasteHeatFunctionofTemperatureCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

    bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

    bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

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
    using ImplType = detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
      std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl);

    friend class detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");
  };

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
  using OptionalCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData =
    boost::optional<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>;

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
  using CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedDataVector =
    std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
