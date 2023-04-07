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

#ifndef MODEL_COILCOOLINGDXCURVEFITSPEED_HPP
#define MODEL_COILCOOLINGDXCURVEFITSPEED_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class CoilCoolingDXCurveFitOperatingMode;

  namespace detail {

    class CoilCoolingDXCurveFitSpeed_Impl;

  }  // namespace detail

  /** CoilCoolingDXCurveFitSpeed is a ResourceObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:CurveFit:Speed'. */
  class MODEL_API CoilCoolingDXCurveFitSpeed : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXCurveFitSpeed(const Model& model);

    virtual ~CoilCoolingDXCurveFitSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXCurveFitSpeed(const CoilCoolingDXCurveFitSpeed& other) = default;
    CoilCoolingDXCurveFitSpeed(CoilCoolingDXCurveFitSpeed&& other) = default;
    CoilCoolingDXCurveFitSpeed& operator=(const CoilCoolingDXCurveFitSpeed&) = default;
    CoilCoolingDXCurveFitSpeed& operator=(CoilCoolingDXCurveFitSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double grossTotalCoolingCapacityFraction() const;

    double evaporatorAirFlowRateFraction() const;

    double condenserAirFlowRateFraction() const;

    boost::optional<double> grossSensibleHeatRatio() const;

    bool isGrossSensibleHeatRatioAutosized() const;

    double grossCoolingCOP() const;

    double activeFractionofCoilFaceArea() const;

    OS_DEPRECATED double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate2017() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate2023() const;

    double evaporativeCondenserPumpPowerFraction() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<Curve> totalCoolingCapacityModifierFunctionofTemperatureCurve() const;

    boost::optional<Curve> totalCoolingCapacityModifierFunctionofAirFlowFractionCurve() const;

    boost::optional<Curve> energyInputRatioModifierFunctionofTemperatureCurve() const;

    boost::optional<Curve> energyInputRatioModifierFunctionofAirFlowFractionCurve() const;

    boost::optional<Curve> partLoadFractionCorrelationCurve() const;

    boost::optional<Curve> wasteHeatModifierFunctionofTemperatureCurve() const;

    double ratedWasteHeatFractionofPowerInput() const;

    boost::optional<Curve> sensibleHeatRatioModifierFunctionofTemperatureCurve() const;

    boost::optional<Curve> sensibleHeatRatioModifierFunctionofFlowFractionCurve() const;

    std::vector<CoilCoolingDXCurveFitOperatingMode> coilCoolingDXCurveFitOperatingModes() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossTotalCoolingCapacityFraction(double grossTotalCoolingCapacityFraction);

    bool setEvaporatorAirFlowRateFraction(double evaporatorAirFlowRateFraction);

    bool setCondenserAirFlowRateFraction(double condenserAirFlowRateFraction);

    bool setGrossSensibleHeatRatio(double grossSensibleHeatRatio);

    void autosizeGrossSensibleHeatRatio();

    bool setGrossCoolingCOP(double grossCoolingCOP);

    bool setActiveFractionofCoilFaceArea(double activeFractionofCoilFaceArea);

    OS_DEPRECATED bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedEvaporatorFanPowerPerVolumeFlowRate2017);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedEvaporatorFanPowerPerVolumeFlowRate2023);

    bool setEvaporativeCondenserPumpPowerFraction(double evaporativeCondenserPumpPowerFraction);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setTotalCoolingCapacityModifierFunctionofTemperatureCurve(const Curve& curve);
    void resetTotalCoolingCapacityModifierFunctionofTemperatureCurve();

    bool setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(const Curve& curve);
    void resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve();

    bool setEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve);
    void resetEnergyInputRatioModifierFunctionofTemperatureCurve();

    bool setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(const Curve& curve);
    void resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve();

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);
    void resetPartLoadFractionCorrelationCurve();

    bool setWasteHeatModifierFunctionofTemperatureCurve(const Curve& curve);
    void resetWasteHeatModifierFunctionofTemperatureCurve();

    bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

    bool setSensibleHeatRatioModifierFunctionofTemperatureCurve(const Curve& curve);
    void resetSensibleHeatRatioModifierFunctionofTemperatureCurve();

    bool setSensibleHeatRatioModifierFunctionofFlowFractionCurve(const Curve& curve);
    void resetSensibleHeatRatioModifierFunctionofFlowFractionCurve();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedGrossSensibleHeatRatio() const;

    void autosize();

    void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXCurveFitSpeed_Impl;

    explicit CoilCoolingDXCurveFitSpeed(std::shared_ptr<detail::CoilCoolingDXCurveFitSpeed_Impl> impl);

    friend class detail::CoilCoolingDXCurveFitSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitSpeed");
  };

  /** \relates CoilCoolingDXCurveFitSpeed*/
  using OptionalCoilCoolingDXCurveFitSpeed = boost::optional<CoilCoolingDXCurveFitSpeed>;

  /** \relates CoilCoolingDXCurveFitSpeed*/
  using CoilCoolingDXCurveFitSpeedVector = std::vector<CoilCoolingDXCurveFitSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITSPEED_HPP
