/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~EvaporativeCoolerIndirectResearchSpecial_Impl() override = default;

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

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_IMPL_HPP
