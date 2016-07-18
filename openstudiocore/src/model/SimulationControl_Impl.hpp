/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
class ZoneAirMassFlowConservation;
class ZoneCapacitanceMultiplierResearchSpecial;

namespace detail {

  class MODEL_API SimulationControl_Impl : public ParentObject_Impl {
    

    
    

    
    

    
    

    
    

    
    

    
    
    
    

    
    
    
    

    
    
    

    
    

    
    

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

