/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIMULATIONCONTROL_IMPL_HPP
#define MODEL_SIMULATIONCONTROL_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ModelObject_Impl.hpp"

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

    class MODEL_API SimulationControl_Impl : public ParentObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      SimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      SimulationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      SimulationControl_Impl(const SimulationControl_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~SimulationControl_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      bool doZoneSizingCalculation() const;

      bool isDoZoneSizingCalculationDefaulted() const;

      bool doSystemSizingCalculation() const;

      bool isDoSystemSizingCalculationDefaulted() const;

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

      bool doHVACSizingSimulationforSizingPeriods() const;
      bool isDoHVACSizingSimulationforSizingPeriodsDefaulted() const;

      int maximumNumberofHVACSizingSimulationPasses() const;
      bool isMaximumNumberofHVACSizingSimulationPassesDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setDoZoneSizingCalculation(bool doZoneSizingCalculation);

      void resetDoZoneSizingCalculation();

      bool setDoSystemSizingCalculation(bool doSystemSizingCalculation);

      void resetDoSystemSizingCalculation();

      bool setDoPlantSizingCalculation(bool doPlantSizingCalculation);

      void resetDoPlantSizingCalculation();

      bool setRunSimulationforSizingPeriods(bool runSimulationforSizingPeriods);

      void resetRunSimulationforSizingPeriods();

      bool setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

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
      void resetDoHVACSizingSimulationforSizingPeriods();

      bool setMaximumNumberofHVACSizingSimulationPasses(int maximumNumberofHVACSizingSimulationPasses);
      void resetMaximumNumberofHVACSizingSimulationPasses();

      //@}

      boost::optional<ConvergenceLimits> convergenceLimits() const;

      boost::optional<HeatBalanceAlgorithm> heatBalanceAlgorithm() const;

      boost::optional<InsideSurfaceConvectionAlgorithm> insideSurfaceConvectionAlgorithm() const;

      boost::optional<OutsideSurfaceConvectionAlgorithm> outsideSurfaceConvectionAlgorithm() const;

      std::vector<RunPeriod> runPeriods() const;

      boost::optional<ShadowCalculation> shadowCalculation() const;

      boost::optional<SizingParameters> sizingParameters() const;

      boost::optional<Timestep> timestep() const;

      boost::optional<ZoneAirContaminantBalance> zoneAirContaminantBalance() const;

      boost::optional<ZoneAirHeatBalanceAlgorithm> zoneAirHeatBalanceAlgorithm() const;

      boost::optional<ZoneAirMassFlowConservation> zoneAirMassFlowConservation() const;

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

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.SimulationControl");

      std::vector<std::string> solarDistributionValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIMULATIONCONTROL_IMPL_HPP
