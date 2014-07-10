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

#ifndef MODEL_SIMULATIONCONTROL_IMPL_HPP
#define MODEL_SIMULATIONCONTROL_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

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
class ZoneCapacitanceMultiplierResearchSpecial;

namespace detail {

  class MODEL_API SimulationControl_Impl : public ParentObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(bool doZoneSizingCalculation READ doZoneSizingCalculation WRITE setDoZoneSizingCalculation RESET resetDoZoneSizingCalculation);
    Q_PROPERTY(bool isDoZoneSizingCalculationDefaulted READ isDoZoneSizingCalculationDefaulted);

    Q_PROPERTY(bool doSystemSizingCalculation READ doSystemSizingCalculation WRITE setDoSystemSizingCalculation RESET resetDoSystemSizingCalculation);
    Q_PROPERTY(bool isDoSystemSizingCalculationDefaulted READ isDoSystemSizingCalculationDefaulted);

    Q_PROPERTY(bool doPlantSizingCalculation READ doPlantSizingCalculation WRITE setDoPlantSizingCalculation RESET resetDoPlantSizingCalculation);
    Q_PROPERTY(bool isDoPlantSizingCalculationDefaulted READ isDoPlantSizingCalculationDefaulted);

    Q_PROPERTY(bool runSimulationforSizingPeriods READ runSimulationforSizingPeriods WRITE setRunSimulationforSizingPeriods RESET resetRunSimulationforSizingPeriods);
    Q_PROPERTY(bool isRunSimulationforSizingPeriodsDefaulted READ isRunSimulationforSizingPeriodsDefaulted);

    Q_PROPERTY(bool runSimulationforWeatherFileRunPeriods READ runSimulationforWeatherFileRunPeriods WRITE setRunSimulationforWeatherFileRunPeriods RESET resetRunSimulationforWeatherFileRunPeriods);
    Q_PROPERTY(bool isRunSimulationforWeatherFileRunPeriodsDefaulted READ isRunSimulationforWeatherFileRunPeriodsDefaulted);

    Q_PROPERTY(double loadsConvergenceToleranceValue READ loadsConvergenceToleranceValue WRITE setLoadsConvergenceToleranceValue RESET resetLoadsConvergenceToleranceValue);
    Q_PROPERTY(openstudio::Quantity loadsConvergenceToleranceValue_SI READ loadsConvergenceToleranceValue_SI WRITE setLoadsConvergenceToleranceValue RESET resetLoadsConvergenceToleranceValue);
    Q_PROPERTY(openstudio::Quantity loadsConvergenceToleranceValue_IP READ loadsConvergenceToleranceValue_IP WRITE setLoadsConvergenceToleranceValue RESET resetLoadsConvergenceToleranceValue);
    Q_PROPERTY(bool isLoadsConvergenceToleranceValueDefaulted READ isLoadsConvergenceToleranceValueDefaulted);

    Q_PROPERTY(double temperatureConvergenceToleranceValue READ temperatureConvergenceToleranceValue WRITE setTemperatureConvergenceToleranceValue RESET resetTemperatureConvergenceToleranceValue);
    Q_PROPERTY(openstudio::Quantity temperatureConvergenceToleranceValue_SI READ temperatureConvergenceToleranceValue_SI WRITE setTemperatureConvergenceToleranceValue RESET resetTemperatureConvergenceToleranceValue);
    Q_PROPERTY(openstudio::Quantity temperatureConvergenceToleranceValue_IP READ temperatureConvergenceToleranceValue_IP WRITE setTemperatureConvergenceToleranceValue RESET resetTemperatureConvergenceToleranceValue);
    Q_PROPERTY(bool isTemperatureConvergenceToleranceValueDefaulted READ isTemperatureConvergenceToleranceValueDefaulted);

    Q_PROPERTY(std::string solarDistribution READ solarDistribution WRITE setSolarDistribution RESET resetSolarDistribution);
    Q_PROPERTY(bool isSolarDistributionDefaulted READ isSolarDistributionDefaulted);
    Q_PROPERTY(std::vector<std::string> solarDistributionValues READ solarDistributionValues);

    Q_PROPERTY(int maximumNumberofWarmupDays READ maximumNumberofWarmupDays WRITE setMaximumNumberofWarmupDays RESET resetMaximumNumberofWarmupDays);
    Q_PROPERTY(bool isMaximumNumberofWarmupDaysDefaulted READ isMaximumNumberofWarmupDaysDefaulted);

    Q_PROPERTY(int minimumNumberofWarmupDays READ minimumNumberofWarmupDays WRITE setMinimumNumberofWarmupDays RESET resetMinimumNumberofWarmupDays);
    Q_PROPERTY(bool isMinimumNumberofWarmupDaysDefaulted READ isMinimumNumberofWarmupDaysDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:

    /** @name Constructors and Destructors */
    //@{

    // constructor
    SimulationControl_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

    // construct from workspace
    SimulationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    // copy constructor
    SimulationControl_Impl(const SimulationControl_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    // virtual destructor
    virtual ~SimulationControl_Impl();

    //@}
    /** @name Getters */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

    void setDoZoneSizingCalculation(bool doZoneSizingCalculation);

    void resetDoZoneSizingCalculation();

    void setDoSystemSizingCalculation(bool doSystemSizingCalculation);

    void resetDoSystemSizingCalculation();

    void setDoPlantSizingCalculation(bool doPlantSizingCalculation);

    void resetDoPlantSizingCalculation();

    void setRunSimulationforSizingPeriods(bool runSimulationforSizingPeriods);

    void resetRunSimulationforSizingPeriods();

    void setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods);

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

    openstudio::Quantity loadsConvergenceToleranceValue_SI() const;
    openstudio::Quantity loadsConvergenceToleranceValue_IP() const;
    openstudio::Quantity temperatureConvergenceToleranceValue_SI() const;
    openstudio::Quantity temperatureConvergenceToleranceValue_IP() const;
    std::vector<std::string> solarDistributionValues() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SIMULATIONCONTROL_IMPL_HPP
