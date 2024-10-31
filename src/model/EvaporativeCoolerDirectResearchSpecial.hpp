/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP
#define MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {
    class EvaporativeCoolerDirectResearchSpecial_Impl;
  }  // namespace detail

  /** EvaporativeCoolerDirectResearchSpecial is a StraightComponent that wraps the
 *  IDD object named "OS:EvaporativeCooler:Direct:ResearchSpecial". */
  class MODEL_API EvaporativeCoolerDirectResearchSpecial : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new EvaporativeCoolerDirectResearchSpecial object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   */
    explicit EvaporativeCoolerDirectResearchSpecial(const Model& model, Schedule& schedule);

    virtual ~EvaporativeCoolerDirectResearchSpecial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EvaporativeCoolerDirectResearchSpecial(const EvaporativeCoolerDirectResearchSpecial& other) = default;
    EvaporativeCoolerDirectResearchSpecial(EvaporativeCoolerDirectResearchSpecial&& other) = default;
    EvaporativeCoolerDirectResearchSpecial& operator=(const EvaporativeCoolerDirectResearchSpecial&) = default;
    EvaporativeCoolerDirectResearchSpecial& operator=(EvaporativeCoolerDirectResearchSpecial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters and Setters */
    //@{

    /** Returns the Schedule referred to by the Availability Schedule field. **/
    Schedule availabilitySchedule() const;

    /** \deprecated */
    Schedule availableSchedule() const;

    /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
    bool setAvailabilitySchedule(Schedule& schedule);

    /** \deprecated */
    bool setAvailableSchedule(Schedule& schedule);

    /** Returns the value of the CoolerDesignEffectiveness field. **/
    double coolerDesignEffectiveness() const;

    /** Deprecated: In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooler Design Effectiveness" **/
    double coolerEffectiveness() const;

    /** Sets the value of the CoolerDesignEffectiveness field. **/
    bool setCoolerDesignEffectiveness(double value);

    /** Deprecated: In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooler Design Effectiveness" **/
    bool setCoolerEffectiveness(double value);

    /** Returns the value of the RecirculatingWaterPumpPowerConsumption field. **/
    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Recirculating Water Pump Design Power" **/
    boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

    /** Sets the value of the RecirculatingWaterPumpPowerConsumption field. **/
    bool setRecirculatingWaterPumpPowerConsumption(double value);

    void autosizeRecirculatingWaterPumpPowerConsumption();

    bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

    /** Returns the value of the PrimaryAirDesignFlowRate field. **/
    boost::optional<double> primaryAirDesignFlowRate() const;

    /** Sets the value of the PrimaryAirDesignFlowRate field. **/
    bool setPrimaryAirDesignFlowRate(double value);

    void autosizePrimaryAirDesignFlowRate();

    bool isPrimaryAirDesignFlowRateAutosized() const;

    /** Returns the Node referred to by the SensorNodeName field. **/
    boost::optional<Node> sensorNode() const;

    /** Sets the Node referred to by the SensorNodeName field. **/
    bool setSensorNode(const Node& node);

    /** Returns the value of the DriftLossFraction field. **/
    double driftLossFraction() const;

    /** Sets the value of the DriftLossFraction field. **/
    bool setDriftLossFraction(double value);

    /** Returns the value of the BlowdownConcentrationRatio field. **/
    double blowdownConcentrationRatio() const;

    /** Sets the value of the BlowdownConcentrationRatio field. **/
    bool setBlowdownConcentrationRatio(double value);

    boost::optional<Curve> effectivenessFlowRatioModifierCurve() const;

    bool setEffectivenessFlowRatioModifierCurve(const Curve& curve);

    void resetEffectivenessFlowRatioModifierCurve();

    double waterPumpPowerSizingFactor() const;

    bool setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

    boost::optional<Curve> waterPumpPowerModifierCurve() const;

    bool setWaterPumpPowerModifierCurve(const Curve& curve);

    void resetWaterPumpPowerModifierCurve();

    /** Sets the minimum and maximum operation temperatures **/

    /** Returns the value of the EvaporativeOperationMinimumDrybulbTemperature field. **/
    double evaporativeOperationMinimumDrybulbTemperature() const;
    /** Sets the value of the EvaporativeOperationMinimumDrybulbTemperature field. **/
    bool setEvaporativeOperationMinimumDrybulbTemperature(double evaporativeOperationMinimumDrybulbTemperature);

    /** Returns the value of the EvaporativeOperationMaximumLimitWetbulbTemperature field. **/
    double evaporativeOperationMaximumLimitWetbulbTemperature() const;
    /** Sets the value of the EvaporativeOperationMaximumLimitWetbulbTemperature field. **/
    bool setEvaporativeOperationMaximumLimitWetbulbTemperature(double evaporativeOperationMaximumLimitWetbulbTemperature);

    /** Returns the value of the EvaporativeOperationMaximumLimitDrybulbTemperature field. **/
    double evaporativeOperationMaximumLimitDrybulbTemperature() const;
    /** Sets the value of the EvaporativeOperationMaximumLimitDrybulbTemperature field. **/
    bool setEvaporativeOperationMaximumLimitDrybulbTemperature(double evaporativeOperationMaximumLimitDrybulbTemperature);

    boost::optional<double> autosizedRecirculatingWaterPumpPowerConsumption() const;

    boost::optional<double> autosizedPrimaryAirDesignFlowRate() const;

    //@}
   protected:
    friend class Model;
    friend class openstudio::IdfObject;

    /// @cond
    using ImplType = detail::EvaporativeCoolerDirectResearchSpecial_Impl;

    explicit EvaporativeCoolerDirectResearchSpecial(std::shared_ptr<detail::EvaporativeCoolerDirectResearchSpecial_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeCoolerDirectResearchSpecial");
    /// @endcond
  };

  /** \relates EvaporativeCoolerDirectResearchSpecial */
  using OptionalEvaporativeCoolerDirectResearchSpecial = boost::optional<EvaporativeCoolerDirectResearchSpecial>;

  /** \relates EvaporativeCoolerDirectResearchSpecial */
  using EvaporativeCoolerDirectResearchSpecialVector = std::vector<EvaporativeCoolerDirectResearchSpecial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP
