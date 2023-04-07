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
      virtual ~SimulationControl_Impl() = default;

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
