/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIMULATIONCONTROL_HPP
#define MODEL_SIMULATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class ConvergenceLimits;
  class HeatBalanceAlgorithm;
  class InsideSurfaceConvectionAlgorithm;
  class OutsideSurfaceConvectionAlgorithm;
  class RunPeriod;
  class ShadowCalculation;
  class SizingParameters;
  class Timestep;
  class ZoneAirContaminantBalance;
  class ZoneAirHeatBalanceAlgorithm;
  class ZoneAirMassFlowConservation;
  class ZoneCapacitanceMultiplierResearchSpecial;

  namespace detail {
    class SimulationControl_Impl;
  }

  /** SimulationControl derives from ParentObject and is an interface to the OpenStudio IDD object named "SimulationControl".
 *
 *  SimulationControl defines what types of sizing simulations are performed for an EnergyPlus simulation,
 *  it also controls whether or not to report results from sizing and weather file simulations.
 *  SimulationControl parents several other ModelObject types that configure simulation parameters for EnergyPlus.
 *  SimulationControl does not have a public constructor because it is a unique ModelObject.
 *  To get the SimulationControl object for a Model or create one if it does not yet exist use model.getUniqueObject<SimulationControl>().
 *  To get the SimulationControl object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<SimulationControl>().
 */
  class MODEL_API SimulationControl : public ParentObject
  {
   public:
    virtual ~SimulationControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SimulationControl(const SimulationControl& other) = default;
    SimulationControl(SimulationControl&& other) = default;
    SimulationControl& operator=(const SimulationControl&) = default;
    SimulationControl& operator=(SimulationControl&&) = default;

    //@}
    /** @name Static Methods */
    //@{
    static IddObjectType iddObjectType();

    static std::vector<std::string> validSolarDistributionValues();

    //@}
    /** @name Getters */
    //@{

    // When defaulted, this will return true if you have DesignDays in your model and any zone with a Zonal HVAC equipment, false otherwise
    bool doZoneSizingCalculation() const;

    bool isDoZoneSizingCalculationDefaulted() const;

    // When defaulted, this will return true only if you have DesignDays and at least one AirLoopHVAC in your model
    bool doSystemSizingCalculation() const;

    bool isDoSystemSizingCalculationDefaulted() const;

    // When defaulted, this will return true only if you have DesignDays and at least one PlantLoop in your model
    bool doPlantSizingCalculation() const;

    bool isDoPlantSizingCalculationDefaulted() const;

    bool runSimulationforSizingPeriods() const;

    bool isRunSimulationforSizingPeriodsDefaulted() const;

    bool runSimulationforWeatherFileRunPeriods() const;

    bool isRunSimulationforWeatherFileRunPeriodsDefaulted() const;

    double loadsConvergenceToleranceValue() const;

    bool isLoadsConvergenceToleranceValueDefaulted() const;

    double temperatureConvergenceToleranceValue() const;

    bool isTemperatureConvergenceToleranceValueDefaulted() const;

    std::string solarDistribution() const;

    bool isSolarDistributionDefaulted() const;

    int maximumNumberofWarmupDays() const;

    bool isMaximumNumberofWarmupDaysDefaulted() const;

    int minimumNumberofWarmupDays() const;

    bool isMinimumNumberofWarmupDaysDefaulted() const;

    // When defaulted, this will return true only if you have DesignDays and at least one PlantLoop with a Sizing Plant having a 'Coincident' Sizing Option
    bool doHVACSizingSimulationforSizingPeriods() const;
    bool isDoHVACSizingSimulationforSizingPeriodsDefaulted() const;

    int maximumNumberofHVACSizingSimulationPasses() const;
    bool isMaximumNumberofHVACSizingSimulationPassesDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDoZoneSizingCalculation(bool doZoneSizingCalculation);

    void setDoZoneSizingCalculationNoFail(bool doZoneSizingCalculation);

    void resetDoZoneSizingCalculation();

    bool setDoSystemSizingCalculation(bool doSystemSizingCalculation);

    void setDoSystemSizingCalculationNoFail(bool doSystemSizingCalculation);

    void resetDoSystemSizingCalculation();

    bool setDoPlantSizingCalculation(bool doPlantSizingCalculation);

    void setDoPlantSizingCalculationNoFail(bool doPlantSizingCalculation);

    void resetDoPlantSizingCalculation();

    bool setRunSimulationforSizingPeriods(bool runSimulationforSizingPeriods);

    void setRunSimulationforSizingPeriodsNoFail(bool runSimulationforSizingPeriods);

    void resetRunSimulationforSizingPeriods();

    bool setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

    void setRunSimulationforWeatherFileRunPeriodsNoFail(bool runSimulationforWeatherFileRunPeriods);

    void resetRunSimulationforWeatherFileRunPeriods();

    bool setLoadsConvergenceToleranceValue(double loadsConvergenceToleranceValue);

    void resetLoadsConvergenceToleranceValue();

    bool setTemperatureConvergenceToleranceValue(double temperatureConvergenceToleranceValue);

    void resetTemperatureConvergenceToleranceValue();

    bool setSolarDistribution(const std::string& solarDistribution);

    void resetSolarDistribution();

    bool setMaximumNumberofWarmupDays(int maximumNumberofWarmupDays);

    void resetMaximumNumberofWarmupDays();

    bool setMinimumNumberofWarmupDays(int minimumNumberofWarmupDays);

    void resetMinimumNumberofWarmupDays();

    bool setDoHVACSizingSimulationforSizingPeriods(bool doHVACSizingSimulationforSizingPeriods);
    void setDoHVACSizingSimulationforSizingPeriodsNoFail(bool doHVACSizingSimulationforSizingPeriods);
    void resetDoHVACSizingSimulationforSizingPeriods();

    bool setMaximumNumberofHVACSizingSimulationPasses(int maximumNumberofHVACSizingSimulationPasses);
    void resetMaximumNumberofHVACSizingSimulationPasses();

    //@}

    /// Returns child ConvergenceLimits if it exists.
    boost::optional<ConvergenceLimits> convergenceLimits() const;

    /// Returns child HeatBalanceAlgorithm if it exists.
    boost::optional<HeatBalanceAlgorithm> heatBalanceAlgorithm() const;

    /// Returns child InsideSurfaceConvectionAlgorithm if it exists.
    boost::optional<InsideSurfaceConvectionAlgorithm> insideSurfaceConvectionAlgorithm() const;

    /// Returns child OutsideSurfaceConvectionAlgorithm if it exists.
    boost::optional<OutsideSurfaceConvectionAlgorithm> outsideSurfaceConvectionAlgorithm() const;

    /// Returns all children of type RunPeriod.
    std::vector<RunPeriod> runPeriods() const;

    /// Returns child ShadowCalculation if it exists.
    boost::optional<ShadowCalculation> shadowCalculation() const;

    /** Returns the SizingParameters object if it exists. */
    boost::optional<SizingParameters> sizingParameters() const;

    /// Returns child Timestep if it exists.
    boost::optional<Timestep> timestep() const;

    /// Returns child ZoneAirContaminantBalance if it exists.
    boost::optional<ZoneAirContaminantBalance> zoneAirContaminantBalance() const;

    /// Returns child ZoneAirHeatBalanceAlgorithm if it exists.
    boost::optional<ZoneAirHeatBalanceAlgorithm> zoneAirHeatBalanceAlgorithm() const;

    /// Returns child ZoneAirMassFlowConservation object if it exists.
    boost::optional<ZoneAirMassFlowConservation> zoneAirMassFlowConservation() const;

    /// Returns child ZoneCapacitanceMultiplierResearchSpecial if it exists.
    boost::optional<ZoneCapacitanceMultiplierResearchSpecial> zoneCapacitanceMultiplierResearchSpecial() const;

    /** Returns the SqlFile environment period strings that correspond to sizing periods. */
    std::vector<std::string> sizingEnvironmentPeriods() const;

    /** Returns the SqlFile environment period strings that correspond to single-year annual
   *  simulations. */
    std::vector<std::string> annualSimulationEnvironmentPeriods() const;

    /** Returns the SqlFile environment period strings that correspond to partial-year,
   *  non-sizing simulations. */
    std::vector<std::string> partialYearEnvironmentPeriods() const;

    /** Returns the SqlFile environment period strings that correspond to non-sizing simulations
   *  with repeated run periods. (Multi-year, or repeated partial years.) */
    std::vector<std::string> repeatedIntervalEnvironmentPeriods() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new SimulationControl object in the model.
    explicit SimulationControl(const Model& model);

    //@}

    /// @cond

    friend class detail::SimulationControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    using ImplType = detail::SimulationControl_Impl;

    explicit SimulationControl(std::shared_ptr<ImplType> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SimulationControl");

    /// @endcond
  };

  /** \relates SimulationControl */
  using OptionalSimulationControl = boost::optional<SimulationControl>;

}  // namespace model
}  // namespace openstudio

#endif
