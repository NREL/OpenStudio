/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~CoilCoolingDXCurveFitSpeed() override = default;
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

    OS_DEPRECATED(3, 5, 0) double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

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

    OS_DEPRECATED(3, 5, 0) bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

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
