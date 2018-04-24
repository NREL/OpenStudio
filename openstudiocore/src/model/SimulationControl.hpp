/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_SIMULATIONCONTROL_HPP
#define MODEL_SIMULATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

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
class MODEL_API SimulationControl : public ParentObject {
 public:
  virtual ~SimulationControl() {}


  //@}
  /** @name Static Methods */
  //@{
  static IddObjectType iddObjectType();

  static std::vector<std::string> validSolarDistributionValues();

  //@}
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

  Quantity getLoadsConvergenceToleranceValue(bool returnIP=false) const;

  bool isLoadsConvergenceToleranceValueDefaulted() const;

  double temperatureConvergenceToleranceValue() const;

  Quantity getTemperatureConvergenceToleranceValue(bool returnIP=false) const;

  bool isTemperatureConvergenceToleranceValueDefaulted() const;

  std::string solarDistribution() const;

  bool isSolarDistributionDefaulted() const;

  int maximumNumberofWarmupDays() const;

  bool isMaximumNumberofWarmupDaysDefaulted() const;

  int minimumNumberofWarmupDays() const;

  bool isMinimumNumberofWarmupDaysDefaulted() const;

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

  bool setLoadsConvergenceToleranceValue(const Quantity& loadsConvergenceToleranceValue);

  void resetLoadsConvergenceToleranceValue();

  bool setTemperatureConvergenceToleranceValue(double temperatureConvergenceToleranceValue);

  bool setTemperatureConvergenceToleranceValue(const Quantity& temperatureConvergenceToleranceValue);

  void resetTemperatureConvergenceToleranceValue();

  bool setSolarDistribution(std::string solarDistribution);

  void resetSolarDistribution();

  bool setMaximumNumberofWarmupDays(int maximumNumberofWarmupDays);

  void resetMaximumNumberofWarmupDays();

  bool setMinimumNumberofWarmupDays(int minimumNumberofWarmupDays);

  void resetMinimumNumberofWarmupDays();

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

  typedef detail::SimulationControl_Impl ImplType;

  explicit SimulationControl(std::shared_ptr<ImplType> impl);

 private:

  REGISTER_LOGGER("openstudio.model.SimulationControl");

  /// @endcond
};

/** \relates SimulationControl */
typedef boost::optional<SimulationControl> OptionalSimulationControl;

}
}

#endif
