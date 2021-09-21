/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_SQL_SQLFILE_HPP
#define UTILITIES_SQL_SQLFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "SummaryData.hpp"
#include "SqlFileDataDictionary.hpp"
#include "SqlFileEnums.hpp"
#include "SqlFile_Impl.hpp"

#include "../data/Vector.hpp"
#include "../data/Matrix.hpp"
#include "../data/DataEnums.hpp"
#include "../data/EndUses.hpp"

#include "../units/SIUnit.hpp"

#include "../core/Path.hpp"
#include "../core/Logger.hpp"
#include "../core/Optional.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio {

// forward declarations
class DateTime;
class EpwFile;
class Calendar;
class SqlFileTimeSeriesQuery;

//namespace detail {
//class SqlFile_Impl;
//}

/** SqlFile class is a transaction script around the sql output of EnergyPlus. */
class UTILITIES_API SqlFile
{
 public:
  /** @name Constructors */
  //@{

  /// default constructor
  SqlFile();

  /// constructor from path
  /// Creates indexes by default, pass in false for no new indexes and quicker opening
  explicit SqlFile(const openstudio::path& path, const bool createIndexes = true);

  /// initializes a new sql file for output
  /// Creates indexes by default, pass in false for no indexes and quicker creation
  SqlFile(const openstudio::path& t_path, const openstudio::EpwFile& t_epwFile, const openstudio::DateTime& t_simulationTime,
          const openstudio::Calendar& t_calendar, const bool createIndexes = true);

  // virtual destructor
  virtual ~SqlFile();

  // Remove indexes that exist, useful for performance reasons.
  void removeIndexes();

  // create indexes on the sql file if they do not exist
  void createIndexes();

  //@}
  /** @name File Queries and Operations */
  //@{

  /// returns whether or not connection is open
  bool connectionOpen() const;

  /// get the path
  openstudio::path path() const;

  /// \returns true if the sqlfile is of a version that's in our supported range
  bool supportedVersion() const;

  // Check if the SQLFile contains the 'Year' field (Added in E+ 8.9.0, but perhaps not working for 8.9.0 itself)
  bool hasYear() const;

  // Check if the SqlFile contains 'Year' field for DaylightMapHourlyReports (added Year in 9.2.0)
  bool hasIlluminanceMapYear() const;

  /// close the file
  bool close();

  /// re-open the file
  bool reopen();

  /// Energy Plus eplusout.sql file name
  std::string energyPlusSqliteFile() const;

  //@}
  /** @name Standard Queries */
  //@{

  /// hours simulated
  boost::optional<double> hoursSimulated() const;

  /// net site energy in GJ
  boost::optional<double> netSiteEnergy() const;

  /// net source energy in GJ
  boost::optional<double> netSourceEnergy() const;

  /// total site energy in GJ
  boost::optional<double> totalSiteEnergy() const;

  /// total source energy in GJ
  boost::optional<double> totalSourceEnergy() const;

  /// Returns the annual total cost associated with the given fuel type in dollars.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> annualTotalCost(const FuelType& fuel) const;

  /// Returns the annual total cost per total building area associated with the given fuel type in dollars per square meter.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> annualTotalCostPerBldgArea(const FuelType& fuel) const;

  /// Returns the annual total cost per net conditioned building area associated with the given fuel type in dollars per square meter.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const;

  /// Returns the annual total cost for all fuel types in dollars. Requires EnergyPlus simulation output to calculate.
  boost::optional<double> annualTotalUtilityCost() const;

  /// Returns the total energy cost over the analysis period in dollars according to the discounting convention.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  boost::optional<double> economicsEnergyCost() const;

  OptionalDouble getElecOrGasUse(bool t_getGas = true) const;
  OptionalDouble getElecOrGasCost(bool t_getGas = true) const;

  /// Returns an EndUses object containing all end uses for the simulation.
  boost::optional<EndUses> endUses() const;

  /// Returns the energy consumption for the given fuel type, category and month.
  /// Requires BUILDING ENERGY PERFORMANCE tabular report. Value is energy use in J.
  boost::optional<double> energyConsumptionByMonth(const openstudio::EndUseFuelType& t_fuelType, const openstudio::EndUseCategoryType& t_categoryType,
                                                   const openstudio::MonthOfYear& t_monthOfYear) const;

  /// Returns the energy demand for the given fuel type, category and month.
  /// Requires BUILDING ENERGY PERFORMANCE tabular report. Value is energy use in W.
  boost::optional<double> peakEnergyDemandByMonth(const openstudio::EndUseFuelType& t_fuelType, const openstudio::EndUseCategoryType& t_categoryType,
                                                  const openstudio::MonthOfYear& t_monthOfYear) const;

  /// Returns the electric energy used for heating in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityHeating() const;

  /// Returns the electric energy used for cooling in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityCooling() const;

  /// Returns the electric energy used for interior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityInteriorLighting() const;

  /// Returns the electric energy used for exterior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityExteriorLighting() const;

  /// Returns the electric energy used for interior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityInteriorEquipment() const;

  /// Returns the electric energy used for exterior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityExteriorEquipment() const;

  /// Returns the electric energy used for fans in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityFans() const;

  /// Returns the electric energy used for pumps in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityPumps() const;

  /// Returns the electric energy used for heat rejection in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityHeatRejection() const;

  /// Returns the electric energy used for humidification in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityHumidification() const;

  /// Returns the electric energy used for heat recovery in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityHeatRecovery() const;

  /// Returns the electric energy used for water systems in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityWaterSystems() const;

  /// Returns the electric energy used for refrigeration in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityRefrigeration() const;

  /// Returns the electric energy used for generators in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityGenerators() const;

  /// Returns the electric energy used for all uses in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> electricityTotalEndUses() const;

  /// Returns the natural gas energy used for heating in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasHeating() const;

  /// Returns the natural gas energy used for cooling in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasCooling() const;

  /// Returns the natural gas energy used for interior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasInteriorLighting() const;

  /// Returns the natural gas energy used for exterior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasExteriorLighting() const;

  /// Returns the natural gas energy used for interior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasInteriorEquipment() const;

  /// Returns the natural gas energy used for exterior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasExteriorEquipment() const;

  /// Returns the natural gas energy used for fans in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasFans() const;

  /// Returns the natural gas energy used for pumps in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasPumps() const;

  /// Returns the natural gas energy used for heat rejection in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasHeatRejection() const;

  /// Returns the natural gas energy used for humidification in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasHumidification() const;

  /// Returns the natural gas energy used for heat recovery in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasHeatRecovery() const;

  /// Returns the natural gas energy used for water systems in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasWaterSystems() const;

  /// Returns the natural gas energy used for refrigeration in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasRefrigeration() const;

  /// Returns the natural gas energy used for generators in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasGenerators() const;

  /// Returns the natural gas energy used for all end uses in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> naturalGasTotalEndUses() const;

  /// Returns the energy used for heating from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelHeating() const;

  /// Returns the energy used for cooling from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelCooling() const;

  /// Returns the energy used for interior lighting from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelInteriorLighting() const;

  /// Returns the energy used for exterior lighting from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelExteriorLighting() const;

  /// Returns the energy used for interior equipment from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelInteriorEquipment() const;

  /// Returns the energy used for exterior equipment from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelExteriorEquipment() const;

  /// Returns the energy used for fans from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelFans() const;

  /// Returns the energy used for pumps from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelPumps() const;

  /// Returns the energy used for heat rejection from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelHeatRejection() const;

  /// Returns the energy used for humidification from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelHumidification() const;

  /// Returns the energy used for heat recovery from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelHeatRecovery() const;

  /// Returns the energy used for water systems from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelWaterSystems() const;

  /// Returns the energy used for refrigeration from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelRefrigeration() const;

  /// Returns the energy used for generators from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelGenerators() const;

  /// Returns the energy used for all end uses from other fuel in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> otherFuelTotalEndUses() const;

  /// Returns the district cooling energy used for heating in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingHeating() const;

  /// Returns the district cooling energy used for cooling in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingCooling() const;

  /// Returns the district cooling energy used for interior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingInteriorLighting() const;

  /// Returns the district cooling energy used for exterior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingExteriorLighting() const;

  /// Returns the district cooling energy used for interior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingInteriorEquipment() const;

  /// Returns the district cooling energy used for exterior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingExteriorEquipment() const;

  /// Returns the district cooling energy used for fans in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingFans() const;

  /// Returns the district cooling energy used for pumps in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingPumps() const;

  /// Returns the district cooling energy used for heat rejection in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingHeatRejection() const;

  /// Returns the district cooling energy used for humidification in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingHumidification() const;

  /// Returns the district cooling energy used for heat recovery in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingHeatRecovery() const;

  /// Returns the district cooling energy used for water systems in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingWaterSystems() const;

  /// Returns the district cooling energy used for refrigeration in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingRefrigeration() const;

  /// Returns the district cooling energy used for generators in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingGenerators() const;

  /// Returns the district cooling energy used for all end uses in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtCoolingTotalEndUses() const;

  /// Returns the district heating energy used for heating in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingHeating() const;

  /// Returns the district heating energy used for cooling in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingCooling() const;

  /// Returns the district heating energy used for interior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingInteriorLighting() const;

  /// Returns the district heating energy used for exterior lighting in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingExteriorLighting() const;

  /// Returns the district heating energy used for interior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingInteriorEquipment() const;

  /// Returns the district heating energy used for exterior equipment in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingExteriorEquipment() const;

  /// Returns the district heating energy used for fans in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingFans() const;

  /// Returns the district heating energy used for pumps in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingPumps() const;

  /// Returns the district heating energy used for heat rejection in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingHeatRejection() const;

  /// Returns the district heating energy used for humidification in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingHumidification() const;

  /// Returns the district heating energy used for heat recovery in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingHeatRecovery() const;

  /// Returns the district heating energy used for water systems in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingWaterSystems() const;

  /// Returns the district heating energy used for refrigeration in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingRefrigeration() const;

  /// Returns the district heating energy used for generators in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingGenerators() const;

  /// Returns the district heating energy used for all end uses in gigajoules.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> districtHeatingTotalEndUses() const;

  /// Returns the water used for heating in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterHeating() const;

  /// Returns the water used for cooling in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterCooling() const;

  /// Returns the water used for interior lighting in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterInteriorLighting() const;

  /// Returns the water used for exterior lighting in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterExteriorLighting() const;

  /// Returns the water used for interior equipment in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterInteriorEquipment() const;

  /// Returns the water used for exterior equipment in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterExteriorEquipment() const;

  /// Returns the water used for fans in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterFans() const;

  /// Returns the water used for pumps in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterPumps() const;

  /// Returns the water used for heat rejection in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterHeatRejection() const;

  /// Returns the water used for humidification in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterHumidification() const;

  /// Returns the water used for heat recovery in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterHeatRecovery() const;

  /// Returns the water used for water systems in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterWaterSystems() const;

  /// Returns the water used for refrigeration in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterRefrigeration() const;

  /// Returns the water used for generators in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterGenerators() const;

  /// Returns the water used for all uses in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> waterTotalEndUses() const;

  /// Returns the total number of hours the heating setpoint is not met.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> hoursHeatingSetpointNotMet() const;

  /// Returns the total number of hours the cooling setpoint is not met.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> hoursCoolingSetpointNotMet() const;

  // returns an optional pair of date times for begin and end of daylight savings time
  boost::optional<std::pair<DateTime, DateTime>> daylightSavingsPeriod() const;

  /// Energy plus version number
  std::string energyPlusVersion() const;

  //@}
  /** @name Generic TimeSeries Interface */
  //@{

  // return a vector of all the available time series
  // do not return variables only reported at run period intervals
  std::vector<std::string> availableTimeSeries();

  // return a vector of all the available reporting frequencies for a given timeseries and environment period
  //  std::vector<ReportingFrequency> availableReportingFrequencies(const std::string& envPeriod, const std::string& timeSeriesName) ;

  // get the type of environment period
  boost::optional<openstudio::EnvironmentType> environmentType(const std::string& envPeriod) const;

  // return a vector of all the available environment periods
  std::vector<std::string> availableEnvPeriods() const;

  // return a vector of all the available reporting frequencies for a given environment period
  std::vector<std::string> availableReportingFrequencies(const std::string& envPeriod);

  // version specific translation for reporting frequencies used in database to reporting frequency enum values
  boost::optional<openstudio::ReportingFrequency> reportingFrequencyFromDB(const std::string& dbReportingFrequency);

  // return a vector of all the available variableName for environment period and reporting frequency
  std::vector<std::string> availableVariableNames(const std::string& envPeriod, const std::string& reportingFrequency) const;

  // return a vector of all timeseries matching name, envPeriod, and reportingFrequency
  // this could be used to get "Mean Air Temperature" for all zones for instance
  std::vector<TimeSeries> timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName);

  // return a vector of all keyValues matching name, envPeriod, and reportingFrequency
  std::vector<std::string> availableKeyValues(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName);

  // return a runPeriod value e.g. Electricity::Facility
  boost::optional<double> runPeriodValue(const std::string& envPeriod, const std::string& timeSeriesName, const std::string& keyValue);

  // return a single timeseries matching name, keyValue, envPeriod, and reportingFrequency
  // this could be used to get "Mean Air Temperature" for a particular zone
  boost::optional<TimeSeries> timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName,
                                         const std::string& keyValue);

  /** Expands query to create a vector of all matching queries. The returned queries will have
   *  one environment period, one reporting frequency, and one time series name specified. The
   *  returned queries will also be "vetted". */
  std::vector<SqlFileTimeSeriesQuery> expandQuery(const SqlFileTimeSeriesQuery& query);

  /** Executes query. ReportingFrequency must be specified. Use expandQuery first to break query
   *  down by ReportingFrequency and determine how many TimeSeries will be returned. */
  std::vector<TimeSeries> timeSeries(const SqlFileTimeSeriesQuery& query);

  //@}
  /** @name Illuminance Map Interface */
  //@{

  /// get names of all available illuminance maps
  std::vector<std::string> illuminanceMapNames() const;
  std::vector<std::string> illuminanceMapNames(const std::string& envPeriod) const;

  /// get zone names of for specified illuminance map
  std::vector<std::string> illuminanceMapZoneNames(const std::string& name) const;
  std::vector<std::string> illuminanceMapZoneNames(int mapIndex) const;

  /// reference point for map - form RefPtn=(x:y:illuminance)
  boost::optional<std::string> illuminanceMapRefPt(const std::string& name, int ptNum) const;
  boost::optional<std::string> illuminanceMapRefPt(int mapIndex, int ptNum) const;

  /// minimum value of map
  boost::optional<double> illuminanceMapMinValue(const std::string& name) const;
  boost::optional<double> illuminanceMapMinValue(int mapIndex) const;

  /// maximum value of map
  boost::optional<double> illuminanceMapMaxValue(const std::string& name) const;
  boost::optional<double> illuminanceMapMaxValue(int mapIndex) const;

  /// minimum and maximum of map
  void illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const;
  void illuminanceMapMaxValue(int mapIndex, double& minValue, double& maxValue) const;

  // find the illuminance map index by name
  boost::optional<int> illuminanceMapIndex(const std::string& name) const;

  /// x position (m) of the illuminance map
  Vector illuminanceMapX(const std::string& name, const DateTime& dateTime) const;

  /// y position (m) of the illuminance map
  Vector illuminanceMapY(const std::string& name, const DateTime& dateTime) const;

  /// x position (m) of the illuminance map
  Vector illuminanceMapX(int hourlyReportIndex) const;

  /// y position (m) of the illuminance map
  Vector illuminanceMapY(int hourlyReportIndex) const;

  /// hourly report index for mapIndex at date and time
  boost::optional<int> illuminanceMapHourlyReportIndex(int mapIndex, const DateTime& dateTime) const;

  /// date time at hourly report index
  boost::optional<DateTime> illuminanceMapDate(int hourlyReportIndex) const;

  /// hourly report indices of the illuminance map
  std::vector<int> illuminanceMapHourlyReportIndices(const std::string& name) const;

  /// hourly report indices of the illuminance map
  std::vector<std::pair<int, DateTime>> illuminanceMapHourlyReportIndicesDates(int mapIndex) const;

  /// hourly report indices of the illuminance map
  std::vector<std::pair<int, DateTime>> illuminanceMapHourlyReportIndicesDates(const std::string& name) const;

  /// hourly report indices of the illuminance map
  std::vector<int> illuminanceMapHourlyReportIndices(int mapIndex) const;

  /** value (lux) of the illuminance map at date and time
   *  value(i,j) is the illuminance at x(i), y(j) */
  Matrix illuminanceMap(const std::string& name, const DateTime& dateTime) const;

  /** value (lux) of the illuminance map at hourlyReportIndex
   *  value(i,j) is the illuminance at x(i), y(j) */
  Matrix illuminanceMap(int hourlyReportIndex) const;

  /** value (lux) of the illuminance map at hourlyReportIndex
   *  value(i,j) is the illuminance at x(i), y(j) fills in x,y, illuminance*/
  void illuminanceMap(int hourlyReportIndex, std::vector<double>& x, std::vector<double>& y, std::vector<double>& illuminance) const;

  /// Returns the summary data for each installlocation and fuel type found in report variables
  std::vector<SummaryData> getSummaryData() const;

  int insertZone(const std::string& t_name, double t_relNorth, double t_originX, double t_originY, double t_originZ, double t_centroidX,
                 double t_centroidY, double t_centroidZ, int t_ofType, double t_multiplier, double t_listMultiplier, double t_minimumX,
                 double t_maximumX, double t_minimumY, double t_maximumY, double t_minimumZ, double t_maximumZ, double t_ceilingHeight,
                 double t_volume, int t_insideConvectionAlgo, int t_outsideConvectionAlgo, double t_floorArea, double t_extGrossWallArea,
                 double t_extNetWallArea, double t_extWindowArea, bool t_isPartOfTotalArea);

  void insertIlluminanceMap(const std::string& t_zoneName, const std::string& t_name, const std::string& t_environmentName,
                            const std::vector<DateTime>& t_times, const std::vector<double>& t_xs, const std::vector<double>& t_ys, double t_z,
                            const std::vector<Matrix>& t_maps);

  //@}
  /** @name Generic Query Interface */
  //@{

  /// execute a statement and return the first (if any) value as a double
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  boost::optional<double> execAndReturnFirstDouble(const std::string& statement, Args&&... args) const {
    boost::optional<double> result;
    if (m_impl) {
      result = m_impl->execAndReturnFirstDouble(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  boost::optional<double> execAndReturnFirstDouble(const std::string& statement) const {
    // Forward to the variadic one. This one is for the ruby bindings
    return execAndReturnFirstDouble<>(statement);
  }

  /// execute a statement and return the first (if any) value as a int
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  boost::optional<int> execAndReturnFirstInt(const std::string& statement, Args&&... args) const {
    boost::optional<int> result;
    if (m_impl) {
      result = m_impl->execAndReturnFirstInt(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  boost::optional<int> execAndReturnFirstInt(const std::string& statement) const {
    // Forward to the variadic one. This one is for the ruby bindings
    return execAndReturnFirstInt<>(statement);
  }

  /// execute a statement and return the first (if any) value as a string
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  boost::optional<std::string> execAndReturnFirstString(const std::string& statement, Args&&... args) const {
    boost::optional<std::string> result;
    if (m_impl) {
      result = m_impl->execAndReturnFirstString(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  boost::optional<std::string> execAndReturnFirstString(const std::string& statement) const {
    // Forward to the variadic one. This one is for the ruby bindings
    return execAndReturnFirstString<>(statement);
  }

  /// execute a statement and return the results (if any) in a vector of double
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  boost::optional<std::vector<double>> execAndReturnVectorOfDouble(const std::string& statement, Args&&... args) const {
    boost::optional<std::vector<double>> result;
    if (m_impl) {
      result = m_impl->execAndReturnVectorOfDouble(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  boost::optional<std::vector<double>> execAndReturnVectorOfDouble(const std::string& statement) const {
    // Forward to the variadic one. This one is for the ruby bindings
    return execAndReturnVectorOfDouble<>(statement);
  }

  /// execute a statement and return the results (if any) in a vector of int
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  boost::optional<std::vector<int>> execAndReturnVectorOfInt(const std::string& statement, Args&&... args) const {
    boost::optional<std::vector<int>> result;
    if (m_impl) {
      result = m_impl->execAndReturnVectorOfInt(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  boost::optional<std::vector<int>> execAndReturnVectorOfInt(const std::string& statement) const {
    // Forward to the variadic one. This one is for the ruby bindings
    return execAndReturnVectorOfInt<>(statement);
  }

  /// execute a statement and return the results (if any) in a vector of string
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  boost::optional<std::vector<std::string>> execAndReturnVectorOfString(const std::string& statement, Args&&... args) const {
    boost::optional<std::vector<std::string>> result;
    if (m_impl) {
      result = m_impl->execAndReturnVectorOfString(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  boost::optional<std::vector<std::string>> execAndReturnVectorOfString(const std::string& statement) const {
    // Forward to the variadic one. This one is for the ruby bindings
    return execAndReturnVectorOfString<>(statement);
  }

  /// execute a statement and return the error code, used for create/drop tables
  // Variadic arguments are the bind arguments if any, to replace '?' placeholders in the statement string
  template <typename... Args>
  int execute(const std::string& statement, Args&&... args) {
    int result = 1;  // SQLITE_ERROR
    if (m_impl) {
      result = m_impl->execute(statement, std::forward<Args>(args)...);
    }
    return result;
  }

  int execute(const std::string& statement) {
    // Forward to the variadic one. This one is for the ruby bindings
    return execute<>(statement);
  }

  void insertTimeSeriesData(const std::string& t_variableType, const std::string& t_indexGroup, const std::string& t_timestepType,
                            const std::string& t_keyValue, const std::string& t_variableName,
                            const openstudio::ReportingFrequency& t_reportingFrequency, const boost::optional<std::string>& t_scheduleName,
                            const std::string& t_variableUnits, const openstudio::TimeSeries& t_timeSeries);

  //@}
  /** @name Operators */
  //@{

  // equality test
  bool operator==(const SqlFile& other) const;

  // inequality test
  bool operator<(const SqlFile& other) const;

  //@}

 private:
  REGISTER_LOGGER("openstudio.sql.SqlFile");

  std::shared_ptr<detail::SqlFile_Impl> m_impl;

  /// returns datadictionary of available timeseries
  detail::DataDictionaryTable dataDictionary() const;
};

/// optional SqlFile
typedef boost::optional<SqlFile> OptionalSqlFile;

}  // namespace openstudio

#endif  // UTILITIES_SQL_SQLFILE_HPP
