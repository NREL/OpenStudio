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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed:SpeedData'. */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(const Model& model);

    virtual ~CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData&& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& operator=(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData&) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& operator=(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double ratedWaterHeatingCapacity() const;

    double ratedWaterHeatingCOP() const;

    double ratedSensibleHeatRatio() const;

    double referenceUnitRatedAirFlowRate() const;

    double referenceUnitRatedWaterFlowRate() const;

    double referenceUnitWaterPumpInputPowerAtRatedConditions() const;

    Curve totalWaterHeatingCapacityFunctionofTemperatureCurve() const;

    Curve totalWaterHeatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve copFunctionofTemperatureCurve() const;

    Curve copFunctionofAirFlowFractionCurve() const;

    Curve copFunctionofWaterFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity);

    bool setRatedWaterHeatingCOP(double ratedWaterHeatingCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

    bool setReferenceUnitWaterPumpInputPowerAtRatedConditions(double referenceUnitWaterPumpInputPowerAtRatedConditions);

    bool setTotalWaterHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setCOPFunctionofTemperatureCurve(const Curve& curve);

    bool setCOPFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData(
      std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData*/
  using OptionalCoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData = boost::optional<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>;

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData*/
  using CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedDataVector = std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEEDSPEEDDATA_HPP
