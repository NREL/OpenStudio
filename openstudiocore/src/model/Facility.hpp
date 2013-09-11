/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_FACILITY_HPP
#define MODEL_FACILITY_HPP

#include <model/ModelAPI.hpp>
#include <model/ParentObject.hpp>

#include <utilities/data/DataEnums.hpp>

namespace openstudio {

class CalibrationResult;
class EndUses;

namespace model {

class Building;
class Meter;
class ExteriorLights;

namespace detail {

  class Facility_Impl;

} // detail

/** Facility derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:Facility".
 *
 *  Facility is a unique object which parents the Building in the model.  Conceptually,
 *  the Facility object includes the Building as well as exterior equipment,
 *  parking lot lighting, water systems for grounds, etc.  The Facility object currently does not
 *  have any fields, it is simply used to access \link Meter Meters \endlink and high level results 
 *  which include exterior end uses. Facility does not have a public constructor because it is a unique ModelObject.  
 *  To get the Facility object for a Model or create one if it does not yet exist use model.getUniqueObject<Facility>().  
 *  To get the Facility object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<Facility>().
 */
class MODEL_API Facility : public ParentObject {
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

  /// Returns all \link Meter Meters \endlink at the Facility level.
  std::vector<Meter> meters() const;
  
  boost::optional<Meter> getMeterByFuelType(
      const FuelType& fuelType,
      const std::string& reportingFrequency="Hourly",
      const boost::optional<EndUseType>& endUseType=boost::none,
      const boost::optional<std::string>& specificEndUse=boost::none) const;

  /// Returns all ExteriorLights.
  std::vector<ExteriorLights> exteriorLights() const;

  //@}
  /** @name Other */
  //@{

  /// Returns the total site energy in gigajoules. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: totalSiteEnergy
  boost::optional<double> totalSiteEnergy() const;

  /// Returns the net site energy in gigajoules. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: netSiteEnergy
  boost::optional<double> netSiteEnergy() const;

  /// Returns the total source energy in gigajoules. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: totalSourceEnergy
  boost::optional<double> totalSourceEnergy() const;

  /// Returns the net source energy in gigajoules. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: netSourceEnergy
  boost::optional<double> netSourceEnergy() const;

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

  /// Returns the annual electric total cost in dollars. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: annualElectricTotalCost
  boost::optional<double> annualElectricTotalCost() const;

  /// Returns the annual gas total cost in dollars. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: annualGasTotalCost
  boost::optional<double> annualGasTotalCost() const;

  /// Returns the annual district cooling total cost in dollars. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: annualDistrictCoolingTotalCost
  boost::optional<double> annualDistrictCoolingTotalCost() const;

  /// Returns the annual district heating total cost in dollars. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: annualDistrictHeatingTotalCost
  boost::optional<double> annualDistrictHeatingTotalCost() const;

  /// Returns the annual water total cost in dollars. Requires EnergyPlus simulation output to calculate.
  /// Attribute name: annualWaterTotalCost
  boost::optional<double> annualWaterTotalCost() const;

  /// Returns the time dependent valuation for all energy use in joules. 
  /// Requires EnergyPlus simulation output and TimeDependentValuation input object to calculate.
  /// Attribute name: totalEnergyTimeDependentValuation
  boost::optional<double> totalEnergyTimeDependentValuation() const;

  /// Returns the time dependent valuation for all energy use in dollars. 
  /// Requires EnergyPlus simulation output and TimeDependentValuation input object to calculate.
  /// Attribute name: totalCostTimeDependentValuation
  boost::optional<double> totalCostTimeDependentValuation() const;

  /// Returns the time dependent valuation for electricity use in joules. 
  /// Requires EnergyPlus simulation output and TimeDependentValuation input object to calculate.
  /// Attribute name: electricityEnergyTimeDependentValuation
  boost::optional<double> electricityEnergyTimeDependentValuation() const;

  /// Returns the time dependent valuation for electricity use in dollars. 
  /// Requires EnergyPlus simulation output and TimeDependentValuation input object to calculate.
  /// Attribute name: electricityCostTimeDependentValuation
  boost::optional<double> electricityCostTimeDependentValuation() const;

  /// Returns the time dependent valuation for fossil fuel use in joules. 
  /// Requires EnergyPlus simulation output and TimeDependentValuation input object to calculate.
  /// Attribute name: fossilFuelEnergyTimeDependentValuation
  boost::optional<double> fossilFuelEnergyTimeDependentValuation() const;

  /// Returns the time dependent valuation for fossil fuel use in dollars. 
  /// Requires EnergyPlus simulation output and TimeDependentValuation input object to calculate.
  /// Attribute name: fossilFuelCostTimeDependentValuation
  boost::optional<double> fossilFuelCostTimeDependentValuation() const;

  /// Returns the total capital cost associated with the building in dollars by summing all costs from all 
  /// ComponentCost_LineItem objects.  Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// \todo: only ComponentCost_LineItem of line item type "Construction" are currently handled
  /// \todo: this should not require life cycle parameters to calculate
  /// Attribute name: economicsCapitalCost
  boost::optional<double> economicsCapitalCost() const;

  /// Returns the total energy cost over the analysis period in dollars according to the discounting convention.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsEnergyCost
  boost::optional<double> economicsEnergyCost() const;

  /// Returns the total life cycle cost over the analysis period in dollars according to the discounting convention.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsTLCC
  boost::optional<double> economicsTLCC() const;

  /// Returns the virtual rate of gas energy in dollars per gigajoule.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsVirtualRateGas
  boost::optional<double> economicsVirtualRateGas() const;

  /// Returns the virtual rate of electric energy in dollars per gigajoule.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsVirtualRateElec
  boost::optional<double> economicsVirtualRateElec() const;

  /// Returns the virtual rate of all energy in dollars per gigajoule.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsVirtualRateCombined
  boost::optional<double> economicsVirtualRateCombined() const;

  /// Returns the simple payback period in years.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsSPB
  boost::optional<double> economicsSPB() const;

  /// Returns the discounted payback period in years.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsSPB
  boost::optional<double> economicsDPB() const;

  /// Returns the net present value in dollars.
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsSPB
  boost::optional<double> economicsNPV() const;

  /// Returns the internal rate of return as a fractional rate (e.g. 5% = 0.05).
  /// Requires EnergyPlus simulation output and LifeCycleCost_Parameters input object to calculate.
  /// Attribute name: economicsSPB
  boost::optional<double> economicsIRR() const;

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
  /// Requires EnergyPlus simulation output to calculate.class EndUseType;
  /// Attribute name: waterGenerators
  boost::optional<double> waterGenerators() const;

  /// Returns the water used for all uses in cubic meters.
  /// Requires EnergyPlus simulation output to calculate.
  /// Attribute name: waterTotalEndUses
  boost::optional<double> waterTotalEndUses() const;

  /// Returns the total number of hours the heating setpoint is not met.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> hoursHeatingSetpointNotMet() const;

  /// Returns the total number of hours the cooling setpoint is not met.
  /// Requires EnergyPlus simulation output to calculate.
  boost::optional<double> hoursCoolingSetpointNotMet() const;

  /// Returns an EndUse object containing all end uses for the simulation.
  boost::optional<EndUses> endUses() const;

  /// Returns the underlying Attribute of the EndUse object.
  boost::optional<Attribute> endUsesAttribute() const;

  /// Returns a CalibrationResult object containing data related to model calibration.
  boost::optional<CalibrationResult> calibrationResult() const;

  /// Returns the underlying Attribute of the CalibrationResult object.
  boost::optional<Attribute> calibrationResultAttribute() const;

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

  explicit Facility(boost::shared_ptr<detail::Facility_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Facility");
};

/** \relates Facility*/
typedef boost::optional<Facility> OptionalFacility;

} // model
} // openstudio

#endif // MODEL_FACILITY_HPP

