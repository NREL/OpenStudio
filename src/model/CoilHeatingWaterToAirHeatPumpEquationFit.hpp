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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class CurveQuadLinear;

  namespace detail {

    class CoilHeatingWaterToAirHeatPumpEquationFit_Impl;

  }  // namespace detail

  /** CoilHeatingWaterToAirHeatPumpEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:WaterToAirHeatPump:EquationFit'. */
  class MODEL_API CoilHeatingWaterToAirHeatPumpEquationFit : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new CoilCoolingWaterToAirHeatPumpEquationFit with required curve objects */
    CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model, const CurveQuadLinear& heatingCapacityCurve,
                                             const CurveQuadLinear& heatingPowerConsumptionCurve);

    CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model);

    virtual ~CoilHeatingWaterToAirHeatPumpEquationFit() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWaterToAirHeatPumpEquationFit(const CoilHeatingWaterToAirHeatPumpEquationFit& other) = default;
    CoilHeatingWaterToAirHeatPumpEquationFit(CoilHeatingWaterToAirHeatPumpEquationFit&& other) = default;
    CoilHeatingWaterToAirHeatPumpEquationFit& operator=(const CoilHeatingWaterToAirHeatPumpEquationFit&) = default;
    CoilHeatingWaterToAirHeatPumpEquationFit& operator=(CoilHeatingWaterToAirHeatPumpEquationFit&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateDefaulted() const;

    bool isRatedAirFlowRateAutosized() const;

    boost::optional<double> ratedWaterFlowRate() const;

    bool isRatedWaterFlowRateDefaulted() const;

    bool isRatedWaterFlowRateAutosized() const;

    boost::optional<double> ratedHeatingCapacity() const;

    bool isRatedHeatingCapacityDefaulted() const;

    bool isRatedHeatingCapacityAutosized() const;

    double ratedHeatingCoefficientofPerformance() const;

    bool isRatedHeatingCoefficientofPerformanceDefaulted() const;

    double ratedEnteringWaterTemperature() const;

    double ratedEnteringAirDryBulbTemperature() const;

    double ratioofRatedHeatingCapacitytoRatedCoolingCapacity() const;

    CurveQuadLinear heatingCapacityCurve() const;

    OS_DEPRECATED double heatingCapacityCoefficient1() const;
    OS_DEPRECATED double heatingCapacityCoefficient2() const;
    OS_DEPRECATED double heatingCapacityCoefficient3() const;
    OS_DEPRECATED double heatingCapacityCoefficient4() const;
    OS_DEPRECATED double heatingCapacityCoefficient5() const;

    CurveQuadLinear heatingPowerConsumptionCurve() const;

    OS_DEPRECATED double heatingPowerConsumptionCoefficient1() const;
    OS_DEPRECATED double heatingPowerConsumptionCoefficient2() const;
    OS_DEPRECATED double heatingPowerConsumptionCoefficient3() const;
    OS_DEPRECATED double heatingPowerConsumptionCoefficient4() const;
    OS_DEPRECATED double heatingPowerConsumptionCoefficient5() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

    bool setRatedWaterFlowRate(double ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity);

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    void resetRatedHeatingCapacity();

    void autosizeRatedHeatingCapacity();

    bool setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance);

    void resetRatedHeatingCoefficientofPerformance();

    bool setRatedEnteringWaterTemperature(double ratedEnteringWaterTemperature);

    bool setRatedEnteringAirDryBulbTemperature(double ratedEnteringAirDryBulbTemperature);

    bool setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(double ratioofRatedHeatingCapacitytoRatedCoolingCapacity);

    bool setHeatingCapacityCurve(const CurveQuadLinear& heatingCapacityCurve);

    OS_DEPRECATED bool setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);
    OS_DEPRECATED bool setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);
    OS_DEPRECATED bool setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);
    OS_DEPRECATED bool setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);
    OS_DEPRECATED bool setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

    bool setHeatingPowerConsumptionCurve(const CurveQuadLinear& heatingPowerConsumptionCurve);

    OS_DEPRECATED bool setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1);
    OS_DEPRECATED bool setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2);
    OS_DEPRECATED bool setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3);
    OS_DEPRECATED bool setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4);
    OS_DEPRECATED bool setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedRatedWaterFlowRate() const;

    boost::optional<double> autosizedRatedHeatingCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl;

    explicit CoilHeatingWaterToAirHeatPumpEquationFit(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl> impl);

    friend class detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpEquationFit");
  };

  /** \relates CoilHeatingWaterToAirHeatPumpEquationFit*/
  using OptionalCoilHeatingWaterToAirHeatPumpEquationFit = boost::optional<CoilHeatingWaterToAirHeatPumpEquationFit>;

  /** \relates CoilHeatingWaterToAirHeatPumpEquationFit*/
  using CoilHeatingWaterToAirHeatPumpEquationFitVector = std::vector<CoilHeatingWaterToAirHeatPumpEquationFit>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
