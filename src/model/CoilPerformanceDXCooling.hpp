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

#ifndef MODEL_COILPERFORMANCEDXCOOLING_HPP
#define MODEL_COILPERFORMANCEDXCOOLING_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Node;

  namespace detail {

    class CoilPerformanceDXCooling_Impl;

  }  // namespace detail

  /** CoilPerformanceDXCooling is a ModelObject that wraps the OpenStudio IDD object 'OS:CoilPerformance:DX:Cooling'. */
  class MODEL_API CoilPerformanceDXCooling : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilPerformanceDXCooling(const Model& model);

    explicit CoilPerformanceDXCooling(const Model& model, const Curve& coolingCapacityFunctionofTemperature,
                                      const Curve& coolingCapacityFunctionofFlowFraction, const Curve& energyInputRatioFunctionofTemperature,
                                      const Curve& energyInputRatioFunctionofFlowFraction, const Curve& partLoadFractionCorrelation);

    virtual ~CoilPerformanceDXCooling() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilPerformanceDXCooling(const CoilPerformanceDXCooling& other) = default;
    CoilPerformanceDXCooling(CoilPerformanceDXCooling&& other) = default;
    CoilPerformanceDXCooling& operator=(const CoilPerformanceDXCooling&) = default;
    CoilPerformanceDXCooling& operator=(CoilPerformanceDXCooling&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> grossRatedTotalCoolingCapacity() const;

    bool isGrossRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> grossRatedSensibleHeatRatio() const;

    bool isGrossRatedSensibleHeatRatioAutosized() const;

    double grossRatedCoolingCOP() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    double fractionofAirFlowBypassedAroundCoil() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    boost::optional<Node> condenserAirInletNode() const;

    std::string condenserType() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserAirFlowRate() const;

    bool isEvaporativeCondenserAirFlowRateAutosized() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    boost::optional<Curve> sensibleHeatRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> sensibleHeatRatioFunctionofFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity);

    void autosizeGrossRatedTotalCoolingCapacity();

    bool setGrossRatedSensibleHeatRatio(double grossRatedSensibleHeatRatio);

    void autosizeGrossRatedSensibleHeatRatio();

    bool setGrossRatedCoolingCOP(double grossRatedCoolingCOP);

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool setFractionofAirFlowBypassedAroundCoil(double fractionofAirFlowBypassedAroundCoil);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    bool
      setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setCondenserAirInletNode(const Node& node);

    void resetCondenserAirInletNode();

    bool setCondenserType(const std::string& condenserType);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate);

    void autosizeEvaporativeCondenserAirFlowRate();

    bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setSensibleHeatRatioFunctionofTemperatureCurve(const Curve& curve);

    void resetSensibleHeatRatioFunctionofTemperatureCurve();

    bool setSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& curve);

    void resetSensibleHeatRatioFunctionofFlowFractionCurve();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedGrossRatedSensibleHeatRatio() const;

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

    void autosize();

    void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilPerformanceDXCooling_Impl;

    explicit CoilPerformanceDXCooling(std::shared_ptr<detail::CoilPerformanceDXCooling_Impl> impl);

    friend class detail::CoilPerformanceDXCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilPerformanceDXCooling");
  };

  /** \relates CoilPerformanceDXCooling*/
  using OptionalCoilPerformanceDXCooling = boost::optional<CoilPerformanceDXCooling>;

  /** \relates CoilPerformanceDXCooling*/
  using CoilPerformanceDXCoolingVector = std::vector<CoilPerformanceDXCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILPERFORMANCEDXCOOLING_HPP
