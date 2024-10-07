/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_HPP
#define MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class EvaporativeCoolerIndirectResearchSpecial_Impl;

  }  // namespace detail

  /** EvaporativeCoolerIndirectResearchSpecial is a StraightComponent that wraps the OpenStudio IDD object 'OS:EvaporativeCooler:Indirect:ResearchSpecial'. */
  class MODEL_API EvaporativeCoolerIndirectResearchSpecial : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EvaporativeCoolerIndirectResearchSpecial(const Model& model);

    virtual ~EvaporativeCoolerIndirectResearchSpecial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EvaporativeCoolerIndirectResearchSpecial(const EvaporativeCoolerIndirectResearchSpecial& other) = default;
    EvaporativeCoolerIndirectResearchSpecial(EvaporativeCoolerIndirectResearchSpecial&& other) = default;
    EvaporativeCoolerIndirectResearchSpecial& operator=(const EvaporativeCoolerIndirectResearchSpecial&) = default;
    EvaporativeCoolerIndirectResearchSpecial& operator=(EvaporativeCoolerIndirectResearchSpecial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooler Wetbulb Design Effectiveness" **/
    double coolerMaximumEffectiveness() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Recirculating Water Pump Design Power" **/
    boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

    bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Secondary Air Design Flow Rate" **/
    boost::optional<double> secondaryFanFlowRate() const;

    bool isSecondaryFanFlowRateAutosized() const;

    /** In EnergyPlus 8.3.0 and above this property and Secondary Fan Delta Pressure no longer exist.
    * They are replaced by the single property Secondary Fan Design Power.
    * To maintain the stability of the OpenStudio API, we retain this property and populate the new EnergyPlus property
    * Secondary Fan Sizing Specific Power with the calculation secondaryFanDeltaPressure() / secondaryFanTotalEfficiency(). **/
    double secondaryFanTotalEfficiency() const;

    /** In EnergyPlus 8.3.0 and above this property and Secondary Fan Total Efficiency no longer exist.
    * They are replaced by the single property Secondary Fan Design Power.
    * To maintain the stability of the OpenStudio API, we retain this property and populate the new EnergyPlus property
    * Secondary Fan Sizing Specific Power with the calculation secondaryFanDeltaPressure() / secondaryFanTotalEfficiency(). **/
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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCoolerMaximumEffectiveness(double coolerMaximumEffectiveness);

    bool setRecirculatingWaterPumpPowerConsumption(double recirculatingWaterPumpPowerConsumption);

    void autosizeRecirculatingWaterPumpPowerConsumption();

    bool setSecondaryFanFlowRate(double secondaryFanFlowRate);

    void resetSecondaryFanFlowRate();

    void autosizeSecondaryFanFlowRate();

    bool setSecondaryFanTotalEfficiency(double secondaryFanTotalEfficiency);

    bool setSecondaryFanDeltaPressure(double secondaryFanDeltaPressure);

    bool setDewpointEffectivenessFactor(double dewpointEffectivenessFactor);

    bool setDriftLossFraction(double driftLossFraction);

    void resetDriftLossFraction();

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    bool setWetbulbEffectivenessFlowRatioModifierCurve(const Curve& curve);

    void resetWetbulbEffectivenessFlowRatioModifierCurve();

    bool setCoolerDrybulbDesignEffectiveness(double coolerDrybulbDesignEffectiveness);

    void resetCoolerDrybulbDesignEffectiveness();

    bool setDrybulbEffectivenessFlowRatioModifierCurve(const Curve& curve);

    void resetDrybulbEffectivenessFlowRatioModifierCurve();

    bool setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

    bool setWaterPumpPowerModifierCurve(const Curve& curve);

    void resetWaterPumpPowerModifierCurve();

    bool setSecondaryAirFlowScalingFactor(double secondaryAirFlowScalingFactor);

    bool setSecondaryAirFanDesignPower(double secondaryAirFanDesignPower);

    void autosizeSecondaryAirFanDesignPower();

    bool setSecondaryAirFanPowerModifierCurve(const Curve& curve);

    void resetSecondaryAirFanPowerModifierCurve();

    bool setPrimaryDesignAirFlowRate(double primaryDesignAirFlowRate);

    void resetPrimaryDesignAirFlowRate();

    void autosizePrimaryDesignAirFlowRate();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRecirculatingWaterPumpPowerConsumption() const;

    boost::optional<double> autosizedSecondaryFanFlowRate() const;

    boost::optional<double> autosizedSecondaryAirFanDesignPower() const;

    boost::optional<double> autosizedPrimaryDesignAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::EvaporativeCoolerIndirectResearchSpecial_Impl;

    explicit EvaporativeCoolerIndirectResearchSpecial(std::shared_ptr<detail::EvaporativeCoolerIndirectResearchSpecial_Impl> impl);

    friend class detail::EvaporativeCoolerIndirectResearchSpecial_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeCoolerIndirectResearchSpecial");
  };

  /** \relates EvaporativeCoolerIndirectResearchSpecial*/
  using OptionalEvaporativeCoolerIndirectResearchSpecial = boost::optional<EvaporativeCoolerIndirectResearchSpecial>;

  /** \relates EvaporativeCoolerIndirectResearchSpecial*/
  using EvaporativeCoolerIndirectResearchSpecialVector = std::vector<EvaporativeCoolerIndirectResearchSpecial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_HPP
