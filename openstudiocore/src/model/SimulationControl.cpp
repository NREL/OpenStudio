/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"

#include "Model.hpp"
#include "ConvergenceLimits.hpp"
#include "ConvergenceLimits_Impl.hpp"
#include "HeatBalanceAlgorithm.hpp"
#include "HeatBalanceAlgorithm_Impl.hpp"
#include "InsideSurfaceConvectionAlgorithm.hpp"
#include "InsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "OutsideSurfaceConvectionAlgorithm.hpp"
#include "OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "ShadowCalculation.hpp"
#include "ShadowCalculation_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"
#include "SizingParameters.hpp"
#include "SizingParameters_Impl.hpp"
#include "Timestep.hpp"
#include "Timestep_Impl.hpp"
#include "WeatherFile.hpp"
#include "WeatherFile_Impl.hpp"
#include "ZoneAirContaminantBalance.hpp"
#include "ZoneAirContaminantBalance_Impl.hpp"
#include "ZoneAirHeatBalanceAlgorithm.hpp"
#include "ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "ZoneAirMassFlowConservation.hpp"
#include "ZoneAirMassFlowConservation_Impl.hpp"
#include "ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/units/Unit.hpp"

namespace openstudio{
namespace model{

namespace detail{

  SimulationControl_Impl::SimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SimulationControl::iddObjectType());
  }

  SimulationControl_Impl::SimulationControl_Impl(
        const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SimulationControl::iddObjectType());
  }

  SimulationControl_Impl::SimulationControl_Impl(const SimulationControl_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
  }

  SimulationControl_Impl::~SimulationControl_Impl()
  {}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> SimulationControl_Impl::parent() const
  {
    // Simulation is highest level, there is no parent
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> SimulationControl_Impl::children() const
  {
    std::vector<ModelObject> result;

    OptionalConvergenceLimits ocl = convergenceLimits();
    if (ocl) { result.push_back(*ocl); }

    //OptionalDaylightSavingsTime odst = daylightSavingsTime();
    //if (odst) { result.push_back(*odst); }

    OptionalHeatBalanceAlgorithm ohba = heatBalanceAlgorithm();
    if (ohba) { result.push_back(*ohba); }

    OptionalInsideSurfaceConvectionAlgorithm oisca = insideSurfaceConvectionAlgorithm();
    if (oisca) { result.push_back(*oisca); }

    OptionalOutsideSurfaceConvectionAlgorithm oosca = outsideSurfaceConvectionAlgorithm();
    if (oosca) { result.push_back(*oosca); }

    RunPeriodVector rps = runPeriods();
    result.insert(result.end(),rps.begin(),rps.end());

    OptionalShadowCalculation osc = shadowCalculation();
    if (osc) { result.push_back(*osc); }

    //SpecialDaysVector sds = specialDays();
    //result.insert(result.end(),sds.begin(),sds.end());

    OptionalSizingParameters osp = sizingParameters();
    if (osp) { result.push_back(*osp); }

    OptionalTimestep ot = timestep();
    if (ot) { result.push_back(*ot); }

    OptionalZoneAirContaminantBalance ozacb = zoneAirContaminantBalance();
    if (ozacb) { result.push_back(*ozacb); }

    OptionalZoneAirHeatBalanceAlgorithm ozahba = zoneAirHeatBalanceAlgorithm();
    if (ozahba) { result.push_back(*ozahba); }

    OptionalZoneAirMassFlowConservation ozamfc = zoneAirMassFlowConservation();
    if (ozamfc) { result.push_back(*ozamfc); }

    OptionalZoneCapacitanceMultiplierResearchSpecial ozcmrs;
    ozcmrs = zoneCapacitanceMultiplierResearchSpecial();
    if (ozcmrs) { result.push_back(*ozcmrs); }

    return result;
  }

  std::vector<IddObjectType> SimulationControl_Impl::allowableChildTypes() const {
    IddObjectTypeVector result;
    result.push_back(ConvergenceLimits::iddObjectType());
    result.push_back(HeatBalanceAlgorithm::iddObjectType());
    result.push_back(InsideSurfaceConvectionAlgorithm::iddObjectType());
    result.push_back(OutsideSurfaceConvectionAlgorithm::iddObjectType());
    result.push_back(RunPeriod::iddObjectType());
    result.push_back(ShadowCalculation::iddObjectType());
    result.push_back(SizingParameters::iddObjectType());
    result.push_back(Timestep::iddObjectType());
    result.push_back(ZoneAirContaminantBalance::iddObjectType());
    result.push_back(ZoneAirHeatBalanceAlgorithm::iddObjectType());
    result.push_back(ZoneAirMassFlowConservation::iddObjectType());
    result.push_back(ZoneCapacitanceMultiplierResearchSpecial::iddObjectType());
    return result;
  }

  const std::vector<std::string>& SimulationControl_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SimulationControl_Impl::iddObjectType() const {
    return SimulationControl::iddObjectType();
  }

  bool SimulationControl_Impl::doZoneSizingCalculation() const {
    boost::optional<std::string> value = getString(OS_SimulationControlFields::DoZoneSizingCalculation,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool SimulationControl_Impl::isDoZoneSizingCalculationDefaulted() const {
    return isEmpty(OS_SimulationControlFields::DoZoneSizingCalculation);
  }

  bool SimulationControl_Impl::doSystemSizingCalculation() const {
    boost::optional<std::string> value = getString(OS_SimulationControlFields::DoSystemSizingCalculation,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool SimulationControl_Impl::isDoSystemSizingCalculationDefaulted() const {
    return isEmpty(OS_SimulationControlFields::DoSystemSizingCalculation);
  }

  bool SimulationControl_Impl::doPlantSizingCalculation() const {
    boost::optional<std::string> value = getString(OS_SimulationControlFields::DoPlantSizingCalculation,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool SimulationControl_Impl::isDoPlantSizingCalculationDefaulted() const {
    return isEmpty(OS_SimulationControlFields::DoPlantSizingCalculation);
  }

  bool SimulationControl_Impl::runSimulationforSizingPeriods() const {
    boost::optional<std::string> value = getString(OS_SimulationControlFields::RunSimulationforSizingPeriods,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool SimulationControl_Impl::isRunSimulationforSizingPeriodsDefaulted() const {
    return isEmpty(OS_SimulationControlFields::RunSimulationforSizingPeriods);
  }

  bool SimulationControl_Impl::runSimulationforWeatherFileRunPeriods() const {
    boost::optional<std::string> value = getString(OS_SimulationControlFields::RunSimulationforWeatherFileRunPeriods,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool SimulationControl_Impl::isRunSimulationforWeatherFileRunPeriodsDefaulted() const {
    return isEmpty(OS_SimulationControlFields::RunSimulationforWeatherFileRunPeriods);
  }

  double SimulationControl_Impl::loadsConvergenceToleranceValue() const {
    boost::optional<double> value = getDouble(OS_SimulationControlFields::LoadsConvergenceToleranceValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SimulationControl_Impl::getLoadsConvergenceToleranceValue(bool returnIP) const {
    OptionalDouble value = loadsConvergenceToleranceValue();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SimulationControlFields::LoadsConvergenceToleranceValue, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SimulationControl_Impl::isLoadsConvergenceToleranceValueDefaulted() const {
    return isEmpty(OS_SimulationControlFields::LoadsConvergenceToleranceValue);
  }

  double SimulationControl_Impl::temperatureConvergenceToleranceValue() const {
    boost::optional<double> value = getDouble(OS_SimulationControlFields::TemperatureConvergenceToleranceValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SimulationControl_Impl::getTemperatureConvergenceToleranceValue(bool returnIP) const {
    OptionalDouble value = temperatureConvergenceToleranceValue();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SimulationControlFields::TemperatureConvergenceToleranceValue, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool SimulationControl_Impl::isTemperatureConvergenceToleranceValueDefaulted() const {
    return isEmpty(OS_SimulationControlFields::TemperatureConvergenceToleranceValue);
  }

  std::string SimulationControl_Impl::solarDistribution() const {
    boost::optional<std::string> value = getString(OS_SimulationControlFields::SolarDistribution,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SimulationControl_Impl::isSolarDistributionDefaulted() const {
    return isEmpty(OS_SimulationControlFields::SolarDistribution);
  }

  int SimulationControl_Impl::maximumNumberofWarmupDays() const {
    boost::optional<int> value = getInt(OS_SimulationControlFields::MaximumNumberofWarmupDays,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SimulationControl_Impl::isMaximumNumberofWarmupDaysDefaulted() const {
    return isEmpty(OS_SimulationControlFields::MaximumNumberofWarmupDays);
  }

  int SimulationControl_Impl::minimumNumberofWarmupDays() const {
    boost::optional<int> value = getInt(OS_SimulationControlFields::MinimumNumberofWarmupDays,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SimulationControl_Impl::isMinimumNumberofWarmupDaysDefaulted() const {
    return isEmpty(OS_SimulationControlFields::MinimumNumberofWarmupDays);
  }

  void SimulationControl_Impl::setDoZoneSizingCalculation(bool doZoneSizingCalculation) {
    bool result = false;
    if (doZoneSizingCalculation) {
      result = setString(OS_SimulationControlFields::DoZoneSizingCalculation, "Yes");
    } else {
      result = setString(OS_SimulationControlFields::DoZoneSizingCalculation, "No");
    }
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::resetDoZoneSizingCalculation() {
    bool result = setString(OS_SimulationControlFields::DoZoneSizingCalculation, "");
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::setDoSystemSizingCalculation(bool doSystemSizingCalculation) {
    bool result = false;
    if (doSystemSizingCalculation) {
      result = setString(OS_SimulationControlFields::DoSystemSizingCalculation, "Yes");
    } else {
      result = setString(OS_SimulationControlFields::DoSystemSizingCalculation, "No");
    }
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::resetDoSystemSizingCalculation() {
    bool result = setString(OS_SimulationControlFields::DoSystemSizingCalculation, "");
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::setDoPlantSizingCalculation(bool doPlantSizingCalculation) {
    bool result = false;
    if (doPlantSizingCalculation) {
      result = setString(OS_SimulationControlFields::DoPlantSizingCalculation, "Yes");
    } else {
      result = setString(OS_SimulationControlFields::DoPlantSizingCalculation, "No");
    }
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::resetDoPlantSizingCalculation() {
    bool result = setString(OS_SimulationControlFields::DoPlantSizingCalculation, "");
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::setRunSimulationforSizingPeriods(bool runSimulationforSizingPeriods) {
    bool result = false;
    if (runSimulationforSizingPeriods) {
      result = setString(OS_SimulationControlFields::RunSimulationforSizingPeriods, "Yes");
    } else {
      result = setString(OS_SimulationControlFields::RunSimulationforSizingPeriods, "No");
    }
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::resetRunSimulationforSizingPeriods() {
    bool result = setString(OS_SimulationControlFields::RunSimulationforSizingPeriods, "");
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods) {
    bool result = false;
    if (runSimulationforWeatherFileRunPeriods) {
      result = setString(OS_SimulationControlFields::RunSimulationforWeatherFileRunPeriods, "Yes");
    } else {
      result = setString(OS_SimulationControlFields::RunSimulationforWeatherFileRunPeriods, "No");
    }
    OS_ASSERT(result);
  }

  void SimulationControl_Impl::resetRunSimulationforWeatherFileRunPeriods() {
    bool result = setString(OS_SimulationControlFields::RunSimulationforWeatherFileRunPeriods, "");
    OS_ASSERT(result);
  }

  bool SimulationControl_Impl::setLoadsConvergenceToleranceValue(double loadsConvergenceToleranceValue) {
    bool result = setDouble(OS_SimulationControlFields::LoadsConvergenceToleranceValue, loadsConvergenceToleranceValue);
    return result;
  }

  bool SimulationControl_Impl::setLoadsConvergenceToleranceValue(const Quantity& loadsConvergenceToleranceValue) {
    OptionalDouble value = getDoubleFromQuantity(OS_SimulationControlFields::LoadsConvergenceToleranceValue,loadsConvergenceToleranceValue);
    if (!value) {
      return false;
    }
    return setLoadsConvergenceToleranceValue(value.get());
  }

  void SimulationControl_Impl::resetLoadsConvergenceToleranceValue() {
    bool result = setString(OS_SimulationControlFields::LoadsConvergenceToleranceValue, "");
    OS_ASSERT(result);
  }

  bool SimulationControl_Impl::setTemperatureConvergenceToleranceValue(double temperatureConvergenceToleranceValue) {
    bool result = setDouble(OS_SimulationControlFields::TemperatureConvergenceToleranceValue, temperatureConvergenceToleranceValue);
    return result;
  }

  bool SimulationControl_Impl::setTemperatureConvergenceToleranceValue(const Quantity& temperatureConvergenceToleranceValue) {
    OptionalDouble value = getDoubleFromQuantity(OS_SimulationControlFields::TemperatureConvergenceToleranceValue,temperatureConvergenceToleranceValue);
    if (!value) {
      return false;
    }
    return setTemperatureConvergenceToleranceValue(value.get());
  }

  void SimulationControl_Impl::resetTemperatureConvergenceToleranceValue() {
    bool result = setString(OS_SimulationControlFields::TemperatureConvergenceToleranceValue, "");
    OS_ASSERT(result);
  }

  bool SimulationControl_Impl::setSolarDistribution(std::string solarDistribution) {
    bool result = setString(OS_SimulationControlFields::SolarDistribution, solarDistribution);
    return result;
  }

  void SimulationControl_Impl::resetSolarDistribution() {
    bool result = setString(OS_SimulationControlFields::SolarDistribution, "");
    OS_ASSERT(result);
  }

  bool SimulationControl_Impl::setMaximumNumberofWarmupDays(int maximumNumberofWarmupDays) {
    bool result = setInt(OS_SimulationControlFields::MaximumNumberofWarmupDays, maximumNumberofWarmupDays);
    return result;
  }

  void SimulationControl_Impl::resetMaximumNumberofWarmupDays() {
    bool result = setString(OS_SimulationControlFields::MaximumNumberofWarmupDays, "");
    OS_ASSERT(result);
  }

  bool SimulationControl_Impl::setMinimumNumberofWarmupDays(int minimumNumberofWarmupDays) {
    bool result = setInt(OS_SimulationControlFields::MinimumNumberofWarmupDays, minimumNumberofWarmupDays);
    return result;
  }

  void SimulationControl_Impl::resetMinimumNumberofWarmupDays() {
    bool result = setString(OS_SimulationControlFields::MinimumNumberofWarmupDays, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity SimulationControl_Impl::loadsConvergenceToleranceValue_SI() const {
    return getLoadsConvergenceToleranceValue(false);
  }

  openstudio::Quantity SimulationControl_Impl::loadsConvergenceToleranceValue_IP() const {
    return getLoadsConvergenceToleranceValue(true);
  }

  openstudio::Quantity SimulationControl_Impl::temperatureConvergenceToleranceValue_SI() const {
    return getTemperatureConvergenceToleranceValue(false);
  }

  openstudio::Quantity SimulationControl_Impl::temperatureConvergenceToleranceValue_IP() const {
    return getTemperatureConvergenceToleranceValue(true);
  }

  std::vector<std::string> SimulationControl_Impl::solarDistributionValues() const {
    return SimulationControl::validSolarDistributionValues();
  }

  boost::optional<ConvergenceLimits> SimulationControl_Impl::convergenceLimits() const {
    return model().getOptionalUniqueModelObject<ConvergenceLimits>();
  }

  boost::optional<HeatBalanceAlgorithm> SimulationControl_Impl::heatBalanceAlgorithm() const {
    return model().getOptionalUniqueModelObject<HeatBalanceAlgorithm>();
  }

  boost::optional<InsideSurfaceConvectionAlgorithm>
  SimulationControl_Impl::insideSurfaceConvectionAlgorithm() const
  {
    return model().getOptionalUniqueModelObject<InsideSurfaceConvectionAlgorithm>();
  }

  boost::optional<OutsideSurfaceConvectionAlgorithm>
  SimulationControl_Impl::outsideSurfaceConvectionAlgorithm() const
  {
    return model().getOptionalUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();
  }

  std::vector<RunPeriod> SimulationControl_Impl::runPeriods() const {
    return model().getConcreteModelObjects<RunPeriod>();
  }

  boost::optional<ShadowCalculation> SimulationControl_Impl::shadowCalculation() const {
    return model().getOptionalUniqueModelObject<ShadowCalculation>();
  }

  boost::optional<SizingParameters> SimulationControl_Impl::sizingParameters() const {
    return model().getOptionalUniqueModelObject<SizingParameters>();
  }

  boost::optional<Timestep> SimulationControl_Impl::timestep() const {
    return model().getOptionalUniqueModelObject<Timestep>();
  }

  boost::optional<ZoneAirContaminantBalance>
  SimulationControl_Impl::zoneAirContaminantBalance() const
  {
    return model().getOptionalUniqueModelObject<ZoneAirContaminantBalance>();
  }

  boost::optional<ZoneAirHeatBalanceAlgorithm>
  SimulationControl_Impl::zoneAirHeatBalanceAlgorithm() const
  {
    return model().getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();
  }

  boost::optional<ZoneAirMassFlowConservation>
    SimulationControl_Impl::zoneAirMassFlowConservation() const
  {
    return model().getOptionalUniqueModelObject<ZoneAirMassFlowConservation>();
  }

  boost::optional<ZoneCapacitanceMultiplierResearchSpecial>
  SimulationControl_Impl::zoneCapacitanceMultiplierResearchSpecial() const
  {
    return model().getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();
  }

  std::vector<std::string> SimulationControl_Impl::sizingEnvironmentPeriods() const {
    StringVector result;
    OptionalSqlFile oSqlFile = model().sqlFile();
    if (oSqlFile && runSimulationforSizingPeriods()) {
      SizingPeriodVector sizingPeriods = model().getModelObjects<SizingPeriod>();
      StringVector environmentPeriods = oSqlFile->availableEnvPeriods();
      for (const SizingPeriod& sp : sizingPeriods) {
        std::string spName = sp.name().get();
        if (std::find(environmentPeriods.begin(),environmentPeriods.end(),spName) !=
            environmentPeriods.end())
        {
          result.push_back(spName);
        }
        else {
           LOG(Warn,"Expected to find environment period '" << spName << "' in SqlFile, but did not.");
        }
      }
    }
    return result;
  }

  std::vector<std::string> SimulationControl_Impl::annualSimulationEnvironmentPeriods() const {
    StringVector result;
    OptionalSqlFile oSqlFile = model().sqlFile();
    if (oSqlFile && runSimulationforWeatherFileRunPeriods()) {
      RunPeriodVector runPeriods = this->runPeriods();
      StringVector environmentPeriods = oSqlFile->availableEnvPeriods();
      for (const RunPeriod& runPeriod : runPeriods) {
        if (runPeriod.isAnnual() && !runPeriod.isRepeated()) {
          std::string rpName = runPeriod.name().get();
          StringVector::const_iterator it = std::find_if(environmentPeriods.begin(),environmentPeriods.end(),std::bind(istringEqual,rpName,std::placeholders::_1));
          if (it != environmentPeriods.end()) {
            result.push_back(*it);
          }
        }
      }
      if (result.empty()) {
        // try environment name from WeatherFile
        OptionalWeatherFile oWeatherFile = model().getOptionalUniqueModelObject<WeatherFile>();
        if (oWeatherFile) {
          OptionalString os = oWeatherFile->environmentName();
          if (os) {
            std::string candidate = *os;
            StringVector::const_iterator it = std::find_if(environmentPeriods.begin(),environmentPeriods.end(),std::bind(istringEqual,candidate,std::placeholders::_1));
            if (it != environmentPeriods.end()) {
              result.push_back(*it);
            }
          }
        }
      }
    }
    return result;
  }

  std::vector<std::string> SimulationControl_Impl::partialYearEnvironmentPeriods() const {
    StringVector result;
    OptionalSqlFile oSqlFile = model().sqlFile();
    if (oSqlFile && runSimulationforWeatherFileRunPeriods()) {
      RunPeriodVector runPeriods = this->runPeriods();
      StringVector environmentPeriods = oSqlFile->availableEnvPeriods();
      for (const RunPeriod& runPeriod : runPeriods) {
        if (runPeriod.isPartialYear()) {
          std::string rpName = runPeriod.name().get();
          StringVector::const_iterator it = std::find(environmentPeriods.begin(),environmentPeriods.end(),rpName);
          if (it != environmentPeriods.end()) {
            result.push_back(*it);
          }
        }
      }
    }
    return result;
  }

  std::vector<std::string> SimulationControl_Impl::repeatedIntervalEnvironmentPeriods() const {
    StringVector result;
    OptionalSqlFile oSqlFile = model().sqlFile();
    if (oSqlFile && runSimulationforWeatherFileRunPeriods()) {
      RunPeriodVector runPeriods = this->runPeriods();
      StringVector environmentPeriods = oSqlFile->availableEnvPeriods();
      for (const RunPeriod& runPeriod : runPeriods) {
        if (runPeriod.isRepeated()) {
          std::string rpName = runPeriod.name().get();
          StringVector::const_iterator it = std::find(environmentPeriods.begin(),environmentPeriods.end(),rpName);
          if (it != environmentPeriods.end()) {
            result.push_back(*it);
          }
        }
      }
    }
    return result;
  }

} // detail

SimulationControl::SimulationControl( const Model& model ):
  ParentObject(iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SimulationControl_Impl>());
}

SimulationControl::SimulationControl(std::shared_ptr<detail::SimulationControl_Impl> impl)
  : ParentObject(std::move(impl))
{}

IddObjectType SimulationControl::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SimulationControl);
}

std::vector<std::string> SimulationControl::validSolarDistributionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SimulationControlFields::SolarDistribution);
}

bool SimulationControl::doZoneSizingCalculation() const {
  return getImpl<detail::SimulationControl_Impl>()->doZoneSizingCalculation();
}

bool SimulationControl::isDoZoneSizingCalculationDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isDoZoneSizingCalculationDefaulted();
}

bool SimulationControl::doSystemSizingCalculation() const {
  return getImpl<detail::SimulationControl_Impl>()->doSystemSizingCalculation();
}

bool SimulationControl::isDoSystemSizingCalculationDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isDoSystemSizingCalculationDefaulted();
}

bool SimulationControl::doPlantSizingCalculation() const {
  return getImpl<detail::SimulationControl_Impl>()->doPlantSizingCalculation();
}

bool SimulationControl::isDoPlantSizingCalculationDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isDoPlantSizingCalculationDefaulted();
}

bool SimulationControl::runSimulationforSizingPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->runSimulationforSizingPeriods();
}

bool SimulationControl::isRunSimulationforSizingPeriodsDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isRunSimulationforSizingPeriodsDefaulted();
}

bool SimulationControl::runSimulationforWeatherFileRunPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->runSimulationforWeatherFileRunPeriods();
}

bool SimulationControl::isRunSimulationforWeatherFileRunPeriodsDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isRunSimulationforWeatherFileRunPeriodsDefaulted();
}

double SimulationControl::loadsConvergenceToleranceValue() const {
  return getImpl<detail::SimulationControl_Impl>()->loadsConvergenceToleranceValue();
}

Quantity SimulationControl::getLoadsConvergenceToleranceValue(bool returnIP) const {
  return getImpl<detail::SimulationControl_Impl>()->getLoadsConvergenceToleranceValue(returnIP);
}

bool SimulationControl::isLoadsConvergenceToleranceValueDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isLoadsConvergenceToleranceValueDefaulted();
}

double SimulationControl::temperatureConvergenceToleranceValue() const {
  return getImpl<detail::SimulationControl_Impl>()->temperatureConvergenceToleranceValue();
}

Quantity SimulationControl::getTemperatureConvergenceToleranceValue(bool returnIP) const {
  return getImpl<detail::SimulationControl_Impl>()->getTemperatureConvergenceToleranceValue(returnIP);
}

bool SimulationControl::isTemperatureConvergenceToleranceValueDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isTemperatureConvergenceToleranceValueDefaulted();
}

std::string SimulationControl::solarDistribution() const {
  return getImpl<detail::SimulationControl_Impl>()->solarDistribution();
}

bool SimulationControl::isSolarDistributionDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isSolarDistributionDefaulted();
}

int SimulationControl::maximumNumberofWarmupDays() const {
  return getImpl<detail::SimulationControl_Impl>()->maximumNumberofWarmupDays();
}

bool SimulationControl::isMaximumNumberofWarmupDaysDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isMaximumNumberofWarmupDaysDefaulted();
}

int SimulationControl::minimumNumberofWarmupDays() const {
  return getImpl<detail::SimulationControl_Impl>()->minimumNumberofWarmupDays();
}

bool SimulationControl::isMinimumNumberofWarmupDaysDefaulted() const {
  return getImpl<detail::SimulationControl_Impl>()->isMinimumNumberofWarmupDaysDefaulted();
}

void SimulationControl::setDoZoneSizingCalculation(bool doZoneSizingCalculation) {
  getImpl<detail::SimulationControl_Impl>()->setDoZoneSizingCalculation(doZoneSizingCalculation);
}

void SimulationControl::resetDoZoneSizingCalculation() {
  getImpl<detail::SimulationControl_Impl>()->resetDoZoneSizingCalculation();
}

void SimulationControl::setDoSystemSizingCalculation(bool doSystemSizingCalculation) {
  getImpl<detail::SimulationControl_Impl>()->setDoSystemSizingCalculation(doSystemSizingCalculation);
}

void SimulationControl::resetDoSystemSizingCalculation() {
  getImpl<detail::SimulationControl_Impl>()->resetDoSystemSizingCalculation();
}

void SimulationControl::setDoPlantSizingCalculation(bool doPlantSizingCalculation) {
  getImpl<detail::SimulationControl_Impl>()->setDoPlantSizingCalculation(doPlantSizingCalculation);
}

void SimulationControl::resetDoPlantSizingCalculation() {
  getImpl<detail::SimulationControl_Impl>()->resetDoPlantSizingCalculation();
}

void SimulationControl::setRunSimulationforSizingPeriods(bool runSimulationforSizingPeriods) {
  getImpl<detail::SimulationControl_Impl>()->setRunSimulationforSizingPeriods(runSimulationforSizingPeriods);
}

void SimulationControl::resetRunSimulationforSizingPeriods() {
  getImpl<detail::SimulationControl_Impl>()->resetRunSimulationforSizingPeriods();
}

void SimulationControl::setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods) {
  getImpl<detail::SimulationControl_Impl>()->setRunSimulationforWeatherFileRunPeriods(runSimulationforWeatherFileRunPeriods);
}

void SimulationControl::resetRunSimulationforWeatherFileRunPeriods() {
  getImpl<detail::SimulationControl_Impl>()->resetRunSimulationforWeatherFileRunPeriods();
}

bool SimulationControl::setLoadsConvergenceToleranceValue(double loadsConvergenceToleranceValue) {
  return getImpl<detail::SimulationControl_Impl>()->setLoadsConvergenceToleranceValue(loadsConvergenceToleranceValue);
}

bool SimulationControl::setLoadsConvergenceToleranceValue(const Quantity& loadsConvergenceToleranceValue) {
  return getImpl<detail::SimulationControl_Impl>()->setLoadsConvergenceToleranceValue(loadsConvergenceToleranceValue);
}

void SimulationControl::resetLoadsConvergenceToleranceValue() {
  getImpl<detail::SimulationControl_Impl>()->resetLoadsConvergenceToleranceValue();
}

bool SimulationControl::setTemperatureConvergenceToleranceValue(double temperatureConvergenceToleranceValue) {
  return getImpl<detail::SimulationControl_Impl>()->setTemperatureConvergenceToleranceValue(temperatureConvergenceToleranceValue);
}

bool SimulationControl::setTemperatureConvergenceToleranceValue(const Quantity& temperatureConvergenceToleranceValue) {
  return getImpl<detail::SimulationControl_Impl>()->setTemperatureConvergenceToleranceValue(temperatureConvergenceToleranceValue);
}

void SimulationControl::resetTemperatureConvergenceToleranceValue() {
  getImpl<detail::SimulationControl_Impl>()->resetTemperatureConvergenceToleranceValue();
}

bool SimulationControl::setSolarDistribution(std::string solarDistribution) {
  return getImpl<detail::SimulationControl_Impl>()->setSolarDistribution(solarDistribution);
}

void SimulationControl::resetSolarDistribution() {
  getImpl<detail::SimulationControl_Impl>()->resetSolarDistribution();
}

bool SimulationControl::setMaximumNumberofWarmupDays(int maximumNumberofWarmupDays) {
  return getImpl<detail::SimulationControl_Impl>()->setMaximumNumberofWarmupDays(maximumNumberofWarmupDays);
}

void SimulationControl::resetMaximumNumberofWarmupDays() {
  getImpl<detail::SimulationControl_Impl>()->resetMaximumNumberofWarmupDays();
}

bool SimulationControl::setMinimumNumberofWarmupDays(int minimumNumberofWarmupDays) {
  return getImpl<detail::SimulationControl_Impl>()->setMinimumNumberofWarmupDays(minimumNumberofWarmupDays);
}

void SimulationControl::resetMinimumNumberofWarmupDays() {
  getImpl<detail::SimulationControl_Impl>()->resetMinimumNumberofWarmupDays();
}

boost::optional<ConvergenceLimits> SimulationControl::convergenceLimits() const {
  return getImpl<detail::SimulationControl_Impl>()->convergenceLimits();
}

boost::optional<HeatBalanceAlgorithm> SimulationControl::heatBalanceAlgorithm() const {
  return getImpl<detail::SimulationControl_Impl>()->heatBalanceAlgorithm();
}

boost::optional<InsideSurfaceConvectionAlgorithm> SimulationControl::insideSurfaceConvectionAlgorithm() const {
  return getImpl<detail::SimulationControl_Impl>()->insideSurfaceConvectionAlgorithm();
}

boost::optional<OutsideSurfaceConvectionAlgorithm> SimulationControl::outsideSurfaceConvectionAlgorithm() const {
  return getImpl<detail::SimulationControl_Impl>()->outsideSurfaceConvectionAlgorithm();
}

std::vector<RunPeriod> SimulationControl::runPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->runPeriods();
}

boost::optional<ShadowCalculation> SimulationControl::shadowCalculation() const {
  return getImpl<detail::SimulationControl_Impl>()->shadowCalculation();
}

boost::optional<SizingParameters> SimulationControl::sizingParameters() const {
  return getImpl<detail::SimulationControl_Impl>()->sizingParameters();
}

boost::optional<Timestep> SimulationControl::timestep() const {
  return getImpl<detail::SimulationControl_Impl>()->timestep();
}

boost::optional<ZoneAirContaminantBalance> SimulationControl::zoneAirContaminantBalance() const {
  return getImpl<detail::SimulationControl_Impl>()->zoneAirContaminantBalance();
}

boost::optional<ZoneAirHeatBalanceAlgorithm> SimulationControl::zoneAirHeatBalanceAlgorithm() const {
  return getImpl<detail::SimulationControl_Impl>()->zoneAirHeatBalanceAlgorithm();
}

boost::optional<ZoneAirMassFlowConservation> SimulationControl::zoneAirMassFlowConservation() const {
  return getImpl<detail::SimulationControl_Impl>()->zoneAirMassFlowConservation();
}

boost::optional<ZoneCapacitanceMultiplierResearchSpecial> SimulationControl::zoneCapacitanceMultiplierResearchSpecial() const {
  return getImpl<detail::SimulationControl_Impl>()->zoneCapacitanceMultiplierResearchSpecial();
}

std::vector<std::string> SimulationControl::sizingEnvironmentPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->sizingEnvironmentPeriods();
}

std::vector<std::string> SimulationControl::annualSimulationEnvironmentPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->annualSimulationEnvironmentPeriods();
}

std::vector<std::string> SimulationControl::partialYearEnvironmentPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->partialYearEnvironmentPeriods();
}

std::vector<std::string> SimulationControl::repeatedIntervalEnvironmentPeriods() const {
  return getImpl<detail::SimulationControl_Impl>()->repeatedIntervalEnvironmentPeriods();
}

}
}
