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

#ifndef MODEL_FACILITY_HPP
#define MODEL_FACILITY_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

#include "../utilities/data/DataEnums.hpp"

namespace openstudio {

class CalibrationResult;
class EndUses;

namespace model {

  class Building;
  class OutputMeter;
  class ExteriorLights;
  class ExteriorFuelEquipment;
  class ExteriorWaterEquipment;

  namespace detail {

    class Facility_Impl;

  }  // namespace detail

  /** Facility derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:Facility".
 *
 *  Facility is a unique object which parents the Building in the model.  Conceptually,
 *  the Facility object includes the Building as well as exterior equipment,
 *  parking lot lighting, water systems for grounds, etc.  The Facility object currently does not
 *  have any fields, it is simply used to access \link OutputMeter OutputMeters \endlink and high level results
 *  which include exterior end uses. Facility does not have a public constructor because it is a unique ModelObject.
 *  To get the Facility object for a Model or create one if it does not yet exist use model.getUniqueObject<Facility>().
 *  To get the Facility object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<Facility>().
 */
  class MODEL_API Facility : public ParentObject
  {

   public:
    virtual ~Facility() {}

    static IddObjectType iddObjectType();

    /// Returns all FuelTypes which are fossil fuels.
    static std::vector<FuelType> fossilFuels();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Children */
    //@{

    /// Returns the child Building.
    boost::optional<Building> building() const;

    /// Returns all \link OutputMeter OutputMeters \endlink at the Facility level.
    std::vector<OutputMeter> meters() const;

    boost::optional<OutputMeter> getMeterByFuelType(openstudio::FuelType fuelType, const std::string& reportingFrequency = "Hourly",
                                                    const boost::optional<EndUseType>& endUseType = boost::none,
                                                    const boost::optional<std::string>& specificEndUse = boost::none) const;

    /// Returns all ExteriorLights.
    std::vector<ExteriorLights> exteriorLights() const;

    /// Returns all ExteriorFuelEquipment.
    std::vector<ExteriorFuelEquipment> exteriorFuelEquipments() const;

    /// Returns all ExteriorWaterEquipment.
    std::vector<ExteriorWaterEquipment> exteriorWaterEquipments() const;

    //@}
    /** @name Other */
    //@{

    /// Returns the total site energy in gigajoules. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> totalSiteEnergy() const;

    /// Returns the net site energy in gigajoules. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> netSiteEnergy() const;

    /// Returns the total source energy in gigajoules. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> totalSourceEnergy() const;

    /// Returns the net source energy in gigajoules. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> netSourceEnergy() const;

    /// Returns the annual total cost associated with the given fuel type in dollars.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCost(openstudio::FuelType fuel) const;

    /// Returns the annual total cost per total building area associated with the given fuel type in dollars per square meter.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCostPerBldgArea(openstudio::FuelType fuel) const;

    /// Returns the annual total cost per net conditioned building area associated with the given fuel type in dollars per square meter.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCostPerNetConditionedBldgArea(openstudio::FuelType fuel) const;

    /// Returns the annual total cost for all fuel types in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalUtilityCost() const;

    /// Returns the annual electric total cost in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualElectricTotalCost() const;

    /// Returns the annual gas total cost in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualGasTotalCost() const;

    /// Returns the annual district cooling total cost in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualDistrictCoolingTotalCost() const;

    /// Returns the annual district heating total cost in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualDistrictHeatingTotalCost() const;

    /// Returns the annual water total cost in dollars. Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualWaterTotalCost() const;

    /// Returns the total capital cost associated with the building in dollars by summing all costs from all
    /// ComponentCost_LineItem objects.  Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    /// \todo: only ComponentCost_LineItem of line item type "Construction" are currently handled
    /// \todo: this should not require life cycle parameters to calculate
    boost::optional<double> economicsCapitalCost() const;

    /// Returns the total energy cost over the analysis period in dollars according to the discounting convention.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsEnergyCost() const;

    /// Returns the total life cycle cost over the analysis period in dollars according to the discounting convention.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsTLCC() const;

    /// Returns the virtual rate of gas energy in dollars per gigajoule.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsVirtualRateGas() const;

    /// Returns the virtual rate of electric energy in dollars per gigajoule.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsVirtualRateElec() const;

    /// Returns the virtual rate of all energy in dollars per gigajoule.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsVirtualRateCombined() const;

    /// Returns the simple payback period in years.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsSPB() const;

    /// Returns the discounted payback period in years.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsDPB() const;

    /// Returns the net present value in dollars.
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsNPV() const;

    /// Returns the internal rate of return as a fractional rate (e.g. 5% = 0.05).
    /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
    boost::optional<double> economicsIRR() const;

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
    /// Requires EnergyPlus simulation output to calculate.class EndUseType;
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

    /// Returns an EndUse object containing all end uses for the simulation.
    boost::optional<EndUses> endUses() const;

    /// Returns a CalibrationResult object containing data related to model calibration.
    boost::optional<CalibrationResult> calibrationResult() const;

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    explicit Facility(Model& model);

    //@}

    /// @cond
    typedef detail::Facility_Impl ImplType;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit Facility(std::shared_ptr<detail::Facility_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Facility");
  };

  /** \relates Facility*/
  typedef boost::optional<Facility> OptionalFacility;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FACILITY_HPP
