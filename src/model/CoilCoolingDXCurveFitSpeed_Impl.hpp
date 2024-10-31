/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXCURVEFITSPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXCURVEFITSPEED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class CoilCoolingDXCurveFitOperatingMode;

  namespace detail {

    /** CoilCoolingDXCurveFitSpeed_Impl is a ResourceObject_Impl that is the implementation class for CoilCoolingDXCurveFitSpeed.*/
    class MODEL_API CoilCoolingDXCurveFitSpeed_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXCurveFitSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitSpeed_Impl(const CoilCoolingDXCurveFitSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXCurveFitSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is used by any CoilCoolingDXCurveFitOperatingMode, remove the corresponding extensible group to avoid having 'blanks'
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      double grossTotalCoolingCapacityFraction() const;

      double evaporatorAirFlowRateFraction() const;

      double condenserAirFlowRateFraction() const;

      boost::optional<double> grossSensibleHeatRatio() const;

      bool isGrossSensibleHeatRatioAutosized() const;

      double grossCoolingCOP() const;

      double activeFractionofCoilFaceArea() const;

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
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITSPEED_IMPL_HPP
