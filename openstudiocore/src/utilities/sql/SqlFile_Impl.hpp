/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef UTILITIES_SQL_SQLFILE_IMPL_HPP
#define UTILITIES_SQL_SQLFILE_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include <sqlite/sqlite3.h>
#include "SummaryData.hpp"
#include "SqlFileEnums.hpp"
#include "SqlFileDataDictionary.hpp"
#include "../data/DataEnums.hpp"
#include "../data/EndUses.hpp"
#include "../core/Optional.hpp"
#include "../data/Matrix.hpp"

#include <boost/optional.hpp>

#include <string>
#include <vector>

// forward declaration
namespace resultsviewer{
  class TableView;
}

namespace openstudio{

  // forward declarations
  class SqlFileTimeSeriesQuery;
  class EpwFile;
  class DateTime;
  class Calendar;

  // private namespace
  namespace detail{

    class UTILITIES_API SqlFile_Impl {
    public:

      /// constructor from filesystem path, will throw ConstructorException if file does not exist
      /// or if file is not valid
      SqlFile_Impl(const openstudio::path& path);


      SqlFile_Impl(const openstudio::path &t_path, const openstudio::EpwFile &t_epwFile, const openstudio::DateTime &t_simulationTime,
          const openstudio::Calendar &t_calendar);

      // virtual destructor
      virtual ~SqlFile_Impl();

      /// remove useful indexes, good for inserting lots of data
      void removeIndexes();

      /// Create useful indexes
      void createIndexes();

      /// returns whether or not connection is open
      bool connectionOpen() const;

      /// get the path
      openstudio::path path() const;

      /// close the file
      bool close();

      /// re-open the file
      bool reopen();

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

      // return a vector of all the available time series
      // do not return variables only reported at run period intervals
      std::vector<std::string> availableTimeSeries();

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
      /// Attribute name: annualTotalUtilityCost
      boost::optional<double> annualTotalUtilityCost() const;

      /// Returns an EndUses object containing all end uses for the simulation.
      boost::optional<EndUses> endUses() const;

      OptionalDouble getElecOrGasUse(bool getGas = true) const;
      OptionalDouble getElecOrGasCost(bool bGetGas = true) const;

      /// Returns the energy consumption for the given fuel type, category and month
      /// Requires BUILDING ENERGY PERFORMANCE tabular report
      boost::optional<double> energyConsumptionByMonth(
          const openstudio::EndUseFuelType &t_fuelType,
          const openstudio::EndUseCategoryType &t_categoryType,
          const openstudio::MonthOfYear &t_monthOfYear) const;

      /// Returns the energy demand for the given fuel type, category and month.
      /// Requires BUILDING ENERGY PERFORMANCE tabular report. Value is energy use in W.
      boost::optional<double> peakEnergyDemandByMonth(
          const openstudio::EndUseFuelType &t_fuelType,
          const openstudio::EndUseCategoryType &t_categoryType,
          const openstudio::MonthOfYear &t_monthOfYear) const;

      /// Returns the total energy cost over the analysis period in dollars according to the discounting convention.
      /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
      /// Attribute name: economicsEnergyCost
      boost::optional<double> economicsEnergyCost() const;


      /// Returns the electric energy used for heating in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityHeating
      boost::optional<double> electricityHeating() const;

      /// Returns the electric energy used for cooling in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityCooling
      boost::optional<double> electricityCooling() const;

      /// Returns the electric energy used for interior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityInteriorLighting
      boost::optional<double> electricityInteriorLighting() const;

      /// Returns the electric energy used for exterior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityExteriorLighting
      boost::optional<double> electricityExteriorLighting() const;

      /// Returns the electric energy used for interior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityInteriorEquipment
      boost::optional<double> electricityInteriorEquipment() const;

      /// Returns the electric energy used for exterior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityExteriorEquipment
      boost::optional<double> electricityExteriorEquipment() const;

      /// Returns the electric energy used for fans in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityFans
      boost::optional<double> electricityFans() const;

      /// Returns the electric energy used for pumps in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityPumps
      boost::optional<double> electricityPumps() const;

      /// Returns the electric energy used for heat rejection in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityHeatRejection
      boost::optional<double> electricityHeatRejection() const;

      /// Returns the electric energy used for humidification in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityHumidification
      boost::optional<double> electricityHumidification() const;

      /// Returns the electric energy used for heat recovery in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityHeatRecovery
      boost::optional<double> electricityHeatRecovery() const;

      /// Returns the electric energy used for water systems in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityWaterSystems
      boost::optional<double> electricityWaterSystems() const;

      /// Returns the electric energy used for refrigeration in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityRefrigeration
      boost::optional<double> electricityRefrigeration() const;

      /// Returns the electric energy used for generators in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityGenerators
      boost::optional<double> electricityGenerators() const;

      /// Returns the electric energy used for all uses in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: electricityTotalEndUses
      boost::optional<double> electricityTotalEndUses() const;

      /// Returns the natural gas energy used for heating in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasHeating
      boost::optional<double> naturalGasHeating() const;

      /// Returns the natural gas energy used for cooling in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasCooling
      boost::optional<double> naturalGasCooling() const;

      /// Returns the natural gas energy used for interior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasInteriorLighting
      boost::optional<double> naturalGasInteriorLighting() const;

      /// Returns the natural gas energy used for exterior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasExteriorLighting
      boost::optional<double> naturalGasExteriorLighting() const;

      /// Returns the natural gas energy used for interior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasInteriorEquipment
      boost::optional<double> naturalGasInteriorEquipment() const;

      /// Returns the natural gas energy used for exterior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasInteriorEquipment
      boost::optional<double> naturalGasExteriorEquipment() const;

      /// Returns the natural gas energy used for fans in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasFans
      boost::optional<double> naturalGasFans() const;

      /// Returns the natural gas energy used for pumps in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasPumps
      boost::optional<double> naturalGasPumps() const;

      /// Returns the natural gas energy used for heat rejection in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasHeatRejection
      boost::optional<double> naturalGasHeatRejection() const;

      /// Returns the natural gas energy used for humidification in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasHumidification
      boost::optional<double> naturalGasHumidification() const;

      /// Returns the natural gas energy used for heat recovery in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasHeatRecovery
      boost::optional<double> naturalGasHeatRecovery() const;

      /// Returns the natural gas energy used for water systems in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasWaterSystems
      boost::optional<double> naturalGasWaterSystems() const;

      /// Returns the natural gas energy used for refrigeration in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasRefrigeration
      boost::optional<double> naturalGasRefrigeration() const;

      /// Returns the natural gas energy used for generators in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasGenerators
      boost::optional<double> naturalGasGenerators() const;

      /// Returns the natural gas energy used for all end uses in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: naturalGasGenerators
      boost::optional<double> naturalGasTotalEndUses() const;

      /// Returns the energy used for heating from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelHeating
      boost::optional<double> otherFuelHeating() const;

      /// Returns the energy used for cooling from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelCooling
      boost::optional<double> otherFuelCooling() const;

      /// Returns the energy used for interior lighting from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelInteriorLighting
      boost::optional<double> otherFuelInteriorLighting() const;

      /// Returns the energy used for exterior lighting from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelExteriorLighting
      boost::optional<double> otherFuelExteriorLighting() const;

      /// Returns the energy used for interior equipment from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelInteriorEquipment
      boost::optional<double> otherFuelInteriorEquipment() const;

      /// Returns the energy used for exterior equipment from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelExteriorEquipment
      boost::optional<double> otherFuelExteriorEquipment() const;

      /// Returns the energy used for fans from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelFans
      boost::optional<double> otherFuelFans() const;

      /// Returns the energy used for pumps from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelPumps
      boost::optional<double> otherFuelPumps() const;

      /// Returns the energy used for heat rejection from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelHeatRejection
      boost::optional<double> otherFuelHeatRejection() const;

      /// Returns the energy used for humidification from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelHumidification
      boost::optional<double> otherFuelHumidification() const;

      /// Returns the energy used for heat recovery from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelHeatRecovery
      boost::optional<double> otherFuelHeatRecovery() const;

      /// Returns the energy used for water systems from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelWaterSystems
      boost::optional<double> otherFuelWaterSystems() const;

      /// Returns the energy used for refrigeration from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelRefrigeration
      boost::optional<double> otherFuelRefrigeration() const;

      /// Returns the energy used for generators from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelGenerators
      boost::optional<double> otherFuelGenerators() const;

      /// Returns the energy used for all end uses from other fuel in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: otherFuelTotalEndUses
      boost::optional<double> otherFuelTotalEndUses() const;

      /// Returns the district cooling energy used for heating in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingHeating
      boost::optional<double> districtCoolingHeating() const;

      /// Returns the district cooling energy used for cooling in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingCooling
      boost::optional<double> districtCoolingCooling() const;

      /// Returns the district cooling energy used for interior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingInteriorLighting
      boost::optional<double> districtCoolingInteriorLighting() const;

      /// Returns the district cooling energy used for exterior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingExteriorLighting
      boost::optional<double> districtCoolingExteriorLighting() const;

      /// Returns the district cooling energy used for interior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingInteriorEquipment
      boost::optional<double> districtCoolingInteriorEquipment() const;

      /// Returns the district cooling energy used for exterior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingExteriorEquipment
      boost::optional<double> districtCoolingExteriorEquipment() const;

      /// Returns the district cooling energy used for fans in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingFans
      boost::optional<double> districtCoolingFans() const;

      /// Returns the district cooling energy used for pumps in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingPumps
      boost::optional<double> districtCoolingPumps() const;

      /// Returns the district cooling energy used for heat rejection in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingHeatRejection
      boost::optional<double> districtCoolingHeatRejection() const;

      /// Returns the district cooling energy used for humidification in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingHumidification
      boost::optional<double> districtCoolingHumidification() const;

      /// Returns the district cooling energy used for heat recovery in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingHeatRecovery
      boost::optional<double> districtCoolingHeatRecovery() const;

      /// Returns the district cooling energy used for water systems in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingWaterSystems
      boost::optional<double> districtCoolingWaterSystems() const;

      /// Returns the district cooling energy used for refrigeration in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingRefrigeration
      boost::optional<double> districtCoolingRefrigeration() const;

      /// Returns the district cooling energy used for generators in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingRefrigeration
      boost::optional<double> districtCoolingGenerators() const;

      /// Returns the district cooling energy used for all end uses in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtCoolingRefrigeration
      boost::optional<double> districtCoolingTotalEndUses() const;

      /// Returns the district heating energy used for heating in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingHeating
      boost::optional<double> districtHeatingHeating() const;

      /// Returns the district heating energy used for cooling in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingCooling
      boost::optional<double> districtHeatingCooling() const;

      /// Returns the district heating energy used for interior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingInteriorLighting
      boost::optional<double> districtHeatingInteriorLighting() const;

      /// Returns the district heating energy used for exterior lighting in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingExteriorLighting
      boost::optional<double> districtHeatingExteriorLighting() const;

      /// Returns the district heating energy used for interior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingInteriorEquipment
      boost::optional<double> districtHeatingInteriorEquipment() const;

      /// Returns the district heating energy used for exterior equipment in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingExteriorEquipment
      boost::optional<double> districtHeatingExteriorEquipment() const;

      /// Returns the district heating energy used for fans in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingFans
      boost::optional<double> districtHeatingFans() const;

      /// Returns the district heating energy used for pumps in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingPumps
      boost::optional<double> districtHeatingPumps() const;

      /// Returns the district heating energy used for heat rejection in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingHeatRejection
      boost::optional<double> districtHeatingHeatRejection() const;

      /// Returns the district heating energy used for humidification in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingHumidification
      boost::optional<double> districtHeatingHumidification() const;

      /// Returns the district heating energy used for heat recovery in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingHeatRecovery
      boost::optional<double> districtHeatingHeatRecovery() const;

      /// Returns the district heating energy used for water systems in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingWaterSystems
      boost::optional<double> districtHeatingWaterSystems() const;

      /// Returns the district heating energy used for refrigeration in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingRefrigeration
      boost::optional<double> districtHeatingRefrigeration() const;

      /// Returns the district heating energy used for generators in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingGenerators
      boost::optional<double> districtHeatingGenerators() const;

      /// Returns the district heating energy used for all end uses in gigajoules.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: districtHeatingTotalEndUses
      boost::optional<double> districtHeatingTotalEndUses() const;

      /// Returns the water used for heating in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterHeating
      boost::optional<double> waterHeating() const;

      /// Returns the water used for cooling in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterCooling
      boost::optional<double> waterCooling() const;

      /// Returns the water used for interior lighting in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterInteriorLighting
      boost::optional<double> waterInteriorLighting() const;

      /// Returns the water used for exterior lighting in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterExteriorLighting
      boost::optional<double> waterExteriorLighting() const;

      /// Returns the water used for interior equipment in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterInteriorEquipment
      boost::optional<double> waterInteriorEquipment() const;

      /// Returns the water used for exterior equipment in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterExteriorEquipment
      boost::optional<double> waterExteriorEquipment() const;

      /// Returns the water used for fans in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterFans
      boost::optional<double> waterFans() const;

      /// Returns the water used for pumps in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterPumps
      boost::optional<double> waterPumps() const;

      /// Returns the water used for heat rejection in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterHeatRejection
      boost::optional<double> waterHeatRejection() const;

      /// Returns the water used for humidification in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterHumidification
      boost::optional<double> waterHumidification() const;

      /// Returns the water used for heat recovery in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterHeatRecovery
      boost::optional<double> waterHeatRecovery() const;

      /// Returns the water used for water systems in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterWaterSystems
      boost::optional<double> waterWaterSystems() const;

      /// Returns the water used for refrigeration in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterRefrigeration
      boost::optional<double> waterRefrigeration() const;

      /// Returns the water used for generators in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterGenerators
      boost::optional<double> waterGenerators() const;

      /// Returns the water used for all uses in cubic meters.
      /// Requires EnergyPlus simulation output to calculate.
      /// Attribute name: waterTotalEndUses
      boost::optional<double> waterTotalEndUses() const;

      boost::optional<double> hoursHeatingSetpointNotMet() const;

      boost::optional<double> hoursCoolingSetpointNotMet() const;

      // return a vector of all the available reporting frequencies for a given timeseries and environment period
      // std::vector<ReportingFrequency> availableReportingFrequencies(const std::string& envPeriod, const std::string& timeSeriesName) ;

      // get the type of environment period
      boost::optional<EnvironmentType> environmentType(const std::string& envPeriod) const;

      // return a vector of all the available environment periods
      std::vector<std::string> availableEnvPeriods() const;

      // return a vector of all the available reporting frequencies for a given environment period
      std::vector<std::string> availableReportingFrequencies(const std::string& envPeriod);

      // version specific translation for reporting frequencies used in database to reporting frequency enum values
      openstudio::ReportingFrequency reportingFrequencyFromDB(const std::string& dbReportingFrequency);

      // return a vector of all the available variableName for environment period and reporting frequency
      std::vector<std::string> availableVariableNames(const std::string& envPeriod, const std::string&  reportingFrequency) const;

      // return a vector of all timeseries matching name, envPeriod, and reportingFrequency
      // this could be used to get "Mean Air Temperature" for all zones for instance
      std::vector<TimeSeries> timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName);

      // return a vector of all keyValues matching name, envPeriod, and reportingFrequency
      std::vector<std::string> availableKeyValues(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName);

      // return a runPeriod value e.g. Electricity::Facility
      boost::optional<double> runPeriodValue(const std::string& envPeriod, const std::string& timeSeriesName, const std::string& keyValue);

      // return a single timeseries matching name, keyValue, envPeriod, and reportingFrequency
      // this could be used to get "Mean Air Temperature" for a particular zone
      boost::optional<TimeSeries> timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName, const std::string& keyValue);

      /** Expands query to create a vector of all matching queries. The returned queries will have
       *  one environment period, one reporting frequency, and one time series name specified. The
       *  returned queries will also be "vetted". */
      std::vector<SqlFileTimeSeriesQuery> expandQuery(const SqlFileTimeSeriesQuery& query);

      std::vector<SqlFileTimeSeriesQuery> expandEnvironment(const SqlFileTimeSeriesQuery& query);

      std::vector<SqlFileTimeSeriesQuery> expandReportingFrequency(const SqlFileTimeSeriesQuery& query);

      std::vector<SqlFileTimeSeriesQuery> expandTimeSeries(const SqlFileTimeSeriesQuery& query);

      std::vector<SqlFileTimeSeriesQuery> expandKeyValues(const SqlFileTimeSeriesQuery& query);

      /** Executes query. ReportingFrequency must be specified. Use expandQuery first to break query
       *  down by ReportingFrequency and determine how many TimeSeries will be returned. */
      std::vector<TimeSeries> timeSeries(const SqlFileTimeSeriesQuery& query);

      // returns an optional pair of date times for begin and end of daylight savings time
      boost::optional<std::pair<openstudio::DateTime, openstudio::DateTime> > daylightSavingsPeriod() const;

      /// returns datadictionary of available timeseries
      DataDictionaryTable dataDictionary() const;

      /// Energy plus version number
      // DLM@20100511: can we query this?
      std::string energyPlusVersion() const;

      /// Energy Plus eplusout.sql file name
      std::string energyPlusSqliteFile() const;

      /// get names of all available illuminance maps
      std::vector<std::string> illuminanceMapNames() const;
      std::vector<std::string> illuminanceMapNames(const std::string& envPeriod) const;

      /// get zone names of for specified illuminance map
      std::vector<std::string> illuminanceMapZoneNames(const std::string& name) const;
      std::vector<std::string> illuminanceMapZoneNames(const int& mapIndex) const;

      /// reference point for map - form RefPtn=(x:y:illuminance)
      boost::optional<std::string> illuminanceMapRefPt(const std::string& name, const int& ptNum) const;
      boost::optional<std::string> illuminanceMapRefPt(const int& mapIndex, const int& ptNum) const;

      /// minimum value of map
      boost::optional<double> illuminanceMapMinValue(const std::string& name) const;
      boost::optional<double> illuminanceMapMinValue(const int& mapIndex) const;

      /// maximum value of map
      boost::optional<double> illuminanceMapMaxValue(const std::string& name) const;
      boost::optional<double> illuminanceMapMaxValue(const int& mapIndex) const;

      /// minimum and maximum of map
      void illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const;
      void illuminanceMapMaxValue(const int& mapIndex, double& minValue, double& maxValue) const;

      /// x position (m) of the illuminance map
      Vector illuminanceMapX(const std::string& name, const DateTime& dateTime) const;

      /// y position (m) of the illuminance map
      Vector illuminanceMapY(const std::string& name, const DateTime& dateTime) const;

      /// x position (m) of the illuminance map
      Vector illuminanceMapX(const int& hourlyReportIndex) const;

      /// y position (m) of the illuminance map
      Vector illuminanceMapY(const int& hourlyReportIndex) const;

      /// hourly report index for mapIndex at date and time
      boost::optional<int> illuminanceMapHourlyReportIndex(const int& mapIndex, const DateTime& dateTime) const;

      // find the illuminance map index by name
      boost::optional<int> illuminanceMapIndex(const std::string& name) const;

      /// date time at hourly report index
      boost::optional<DateTime> illuminanceMapDate(const int& hourlyReportIndex) const;

      /// hourly report indices of the illuminance map
      std::vector<int> illuminanceMapHourlyReportIndices(const std::string& name) const;

      /// hourly report indices and dates of the illuminance map
      std::vector< std::pair<int, DateTime> > illuminanceMapHourlyReportIndicesDates(const int& mapIndex) const;

      /// hourly report indices and dates of the illuminance map
      std::vector< std::pair<int, DateTime> > illuminanceMapHourlyReportIndicesDates(const std::string& name) const;

      /// hourly report indices of the illuminance map
      std::vector<int> illuminanceMapHourlyReportIndices(const int& mapIndex) const;

      /// value (lux) of the illuminance map at date and time
      /// value(i,j) is the illuminance at x(i), y(j)
      Matrix illuminanceMap(const std::string& name, const DateTime& dateTime) const;

      /// value (lux) of the illuminance map at hourlyReportIndex
      /// value(i,j) is the illuminance at x(i), y(j)
      Matrix illuminanceMap(const int& hourlyReportIndex) const  ;

      /// value (lux) of the illuminance map at hourlyReportIndex
      /// value(i,j) is the illuminance at x(i), y(j) - returns x, y and illuminance
      void illuminanceMap(const int& hourlyReportIndex, std::vector<double>& x, std::vector<double>& y, std::vector<double>& illuminance) const  ;

      // execute a statement and return the first (if any) value as a double
      boost::optional<double> execAndReturnFirstDouble(const std::string& statement) const;

      // execute a statement and return the first (if any) value as an int
      boost::optional<int> execAndReturnFirstInt(const std::string& statement) const;

      // execute a statement and return the first (if any) value as a string
      boost::optional<std::string> execAndReturnFirstString(const std::string& statement) const;

      /// execute a statement and return the results (if any) in a vector of double
      boost::optional<std::vector<double> > execAndReturnVectorOfDouble(const std::string& statement) const;

      /// execute a statement and return the results (if any) in a vector of int
      boost::optional<std::vector<int> > execAndReturnVectorOfInt(const std::string& statement) const;

      /// execute a statement and return the results (if any) in a vector of string
      boost::optional<std::vector<std::string> > execAndReturnVectorOfString(const std::string& statement) const;

      // execute a statement and return the error code, used for create/drop tables
      int execute(const std::string& statement);

      /// Returns the summary data for each install location and fuel type found in report variables
      std::vector<openstudio::SummaryData> getSummaryData() const;

      void insertTimeSeriesData(const std::string &t_variableType, const std::string &t_indexGroup,
          const std::string &t_timestepType, const std::string &t_keyValue, const std::string &t_variableName,
          const openstudio::ReportingFrequency &t_reportingFrequency, const boost::optional<std::string> &t_scheduleName,
          const std::string &t_variableUnits, const openstudio::TimeSeries &t_timeSeries);

      int insertZone(const std::string &t_name,
          double t_relNorth,
          double t_originX, double t_originY, double t_originZ,
          double t_centroidX, double t_centroidY, double t_centroidZ,
          int t_ofType,
          double t_multiplier,
          double t_listMultiplier,
          double t_minimumX, double t_maximumX,
          double t_minimumY, double t_maximumY,
          double t_minimumZ, double t_maximumZ,
          double t_ceilingHeight,
          double t_volume,
          int t_insideConvectionAlgo,
          int t_outsideConvectionAlgo,
          double t_floorArea,
          double t_extGrossWallArea,
          double t_extNetWallArea,
          double t_extWindowArea,
          bool t_isPartOfTotalArea);

      void insertIlluminanceMap(const std::string &t_zoneName, const std::string &name, const std::string &t_environmentName,
          const std::vector<DateTime> &t_times,
          const std::vector<double> &t_xs, const std::vector<double> &t_ys, double t_z, const std::vector<Matrix> &t_maps);

      bool isSupportedVersion() const;

    private:

      void init(const openstudio::path& path);

      void retrieveDataDictionary();

      void execAndThrowOnError(const std::string &t_stmt);
      void addSimulation(const openstudio::EpwFile &t_epwFile, const openstudio::DateTime &t_simulationTime,
        const openstudio::Calendar &t_calendar);
      int getNextIndex(const std::string &t_tableName, const std::string &t_columnName);

      // return a single timeseries matching recordIndex - internally used to retrieve timeseries
      boost::optional<TimeSeries> timeSeries(const DataDictionaryItem& dataDictionary);
      std::vector<double> timeSeriesValues(const DataDictionaryItem& dataDictionary);
      boost::optional<Date> timeSeriesStartDate(const DataDictionaryItem& dataDictionary);

      // return first date in time table used for start date of run period variables
      openstudio::DateTime firstDateTime(bool includeHourAndMinute);

      boost::optional<Time> timeSeriesInterval(const DataDictionaryItem& dataDictionary);
      std::vector<DateTime> dateTimeVec(const DataDictionaryItem& dataDictionary);

      bool isValidConnection();

      void mf_makeConsistent(std::vector<SqlFileTimeSeriesQuery>& queries);

      openstudio::path m_path;
      bool m_connectionOpen;
      DataDictionaryTable m_dataDictionary;
      sqlite3* m_db;
      std::string m_sqliteFilename;

      bool m_supportedVersion;

      REGISTER_LOGGER("openstudio.energyplus.SqlFile");
    };

    // ETH@20100920 SqlFile_Impl& cannot be const because function calls non-const getter.
    UTILITIES_API std::set<ReportingFrequency> availableReportingFrequencySet(SqlFile_Impl& sqlFileImpl,
                                                                              const std::string& envPeriod);
  } // detail

} // openstudio

#endif // UTILITIES_SQL_SQLFILE_IMPL_HPP
