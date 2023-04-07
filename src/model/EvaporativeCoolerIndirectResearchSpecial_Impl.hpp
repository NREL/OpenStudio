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

#ifndef MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP
#define MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** EvaporativeCoolerIndirectResearchSpecial_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeCoolerIndirectResearchSpecial.*/
    class MODEL_API EvaporativeCoolerIndirectResearchSpecial_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EvaporativeCoolerIndirectResearchSpecial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EvaporativeCoolerIndirectResearchSpecial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EvaporativeCoolerIndirectResearchSpecial_Impl(const EvaporativeCoolerIndirectResearchSpecial_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EvaporativeCoolerIndirectResearchSpecial_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      // TODO: Check return type. From object lists, some candidates are: Schedule.
      boost::optional<Schedule> availabilitySchedule() const;

      double coolerMaximumEffectiveness() const;

      boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

      bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

      boost::optional<double> secondaryFanFlowRate() const;

      bool isSecondaryFanFlowRateAutosized() const;

      double secondaryFanTotalEfficiency() const;

      double secondaryFanDeltaPressure() const;

      double dewpointEffectivenessFactor() const;

      boost::optional<double> driftLossFraction() const;

      boost::optional<double> blowdownConcentrationRatio() const;

      boost::optional<Curve> wetbulbEffectivenessFlowRatioModifierCurve() const;

      boost::optional<double> coolerDrybulbDesignEffectiveness() const;

      boost::optional<Curve> drybulbEffectivenessFlowRatioModifierCurve() const;

      double waterPumpPowerSizingFactor() const;

      boost::optional<Curve> waterPumpPowerModifierCurve() const;

      double secondaryAirFlowScalingFactor() const;

      boost::optional<double> secondaryAirFanDesignPower() const;

      bool isSecondaryAirFanDesignPowerAutosized() const;

      boost::optional<Curve> secondaryAirFanPowerModifierCurve() const;

      boost::optional<double> primaryDesignAirFlowRate() const;

      bool isPrimaryDesignAirFlowRateAutosized() const;

      boost::optional<double> autosizedRecirculatingWaterPumpPowerConsumption() const;

      boost::optional<double> autosizedSecondaryFanFlowRate() const;

      boost::optional<double> autosizedSecondaryAirFanDesignPower() const;

      boost::optional<double> autosizedPrimaryDesignAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      // TODO: Water Supply Storage Tank Name

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setCoolerMaximumEffectiveness(double coolerMaximumEffectiveness);

      bool setRecirculatingWaterPumpPowerConsumption(double recirculatingWaterPumpPowerConsumption);

      void autosizeRecirculatingWaterPumpPowerConsumption();

      bool setSecondaryFanFlowRate(boost::optional<double> secondaryFanFlowRate);

      void resetSecondaryFanFlowRate();

      void autosizeSecondaryFanFlowRate();

      bool setSecondaryFanTotalEfficiency(double secondaryFanTotalEfficiency);

      bool setSecondaryFanDeltaPressure(double secondaryFanDeltaPressure);

      bool setDewpointEffectivenessFactor(double dewpointEffectivenessFactor);

      bool setDriftLossFraction(boost::optional<double> driftLossFraction);

      void resetDriftLossFraction();

      bool setBlowdownConcentrationRatio(boost::optional<double> blowdownConcentrationRatio);

      void resetBlowdownConcentrationRatio();

      bool setWetbulbEffectivenessFlowRatioModifierCurve(const boost::optional<Curve>& curve);

      void resetWetbulbEffectivenessFlowRatioModifierCurve();

      bool setCoolerDrybulbDesignEffectiveness(boost::optional<double> coolerDrybulbDesignEffectiveness);

      void resetCoolerDrybulbDesignEffectiveness();

      bool setDrybulbEffectivenessFlowRatioModifierCurve(const boost::optional<Curve>& curve);

      void resetDrybulbEffectivenessFlowRatioModifierCurve();

      bool setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

      bool setWaterPumpPowerModifierCurve(const boost::optional<Curve>& curve);

      void resetWaterPumpPowerModifierCurve();

      bool setSecondaryAirFlowScalingFactor(double secondaryAirFlowScalingFactor);

      bool setSecondaryAirFanDesignPower(boost::optional<double> secondaryAirFanDesignPower);

      void autosizeSecondaryAirFanDesignPower();

      bool setSecondaryAirFanPowerModifierCurve(const boost::optional<Curve>& curve);

      void resetSecondaryAirFanPowerModifierCurve();

      bool setPrimaryDesignAirFlowRate(boost::optional<double> primaryDesignAirFlowRate);

      void resetPrimaryDesignAirFlowRate();

      void autosizePrimaryDesignAirFlowRate();

      //@}
      /** @name Other */
      //@{
      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      bool addToNode(Node& node) override;

      bool setReliefAirInletNode(const Node& node);

      boost::optional<Node> reliefAirInletNode() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EvaporativeCoolerIndirectResearchSpecial");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP
