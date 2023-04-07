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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;

  namespace detail {

    class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;

  }  // namespace detail

  /** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit'. */
  class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model);

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model, const Curve& partLoadFraction);

    virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit& other) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit&& other) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit& operator=(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit&) = default;
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit& operator=(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

    bool useHotGasReheat() const;

    Curve energyPartLoadFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

    void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

    bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

    bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

    bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

    bool setUseHotGasReheat(bool useHotGasReheat);

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

    bool addSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedWaterFlowRateAtSelectedNominalSpeedLevel() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;

    explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(
      std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl);

    friend class detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit");
  };

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit*/
  using OptionalCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit = boost::optional<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>;

  /** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit*/
  using CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitVector = std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP
