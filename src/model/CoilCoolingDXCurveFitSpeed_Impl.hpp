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

      virtual ~CoilCoolingDXCurveFitSpeed_Impl() = default;

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
