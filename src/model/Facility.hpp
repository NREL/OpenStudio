/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FACILITY_HPP
#define MODEL_FACILITY_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

#include "../utilities/data/DataEnums.hpp"
#include "../utilities/core/Deprecated.hpp"

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
    virtual ~Facility() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Facility(const Facility& other) = default;
    Facility(Facility&& other) = default;
    Facility& operator=(const Facility&) = default;
    Facility& operator=(Facility&&) = default;

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

    boost::optional<OutputMeter> getMeterByFuelType(const FuelType& fuelType, const std::string& reportingFrequency = "Hourly",
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
    boost::optional<double> annualTotalCost(const FuelType& fuel) const;

    /// Returns the annual total cost per total building area associated with the given fuel type in dollars per square meter.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCostPerBldgArea(const FuelType& fuel) const;

    /// Returns the annual total cost per net conditioned building area associated with the given fuel type in dollars per square meter.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const;

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

    /// Gasoline
    /// Returns the energy used for heating from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineHeating() const;

    /// Returns the energy used for cooling from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineCooling() const;

    /// Returns the energy used for interior lighting from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineInteriorLighting() const;

    /// Returns the energy used for exterior lighting from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineExteriorLighting() const;

    /// Returns the energy used for interior equipment from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineInteriorEquipment() const;

    /// Returns the energy used for exterior equipment from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineExteriorEquipment() const;

    /// Returns the energy used for fans from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineFans() const;

    /// Returns the energy used for pumps from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolinePumps() const;

    /// Returns the energy used for heat rejection from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineHeatRejection() const;

    /// Returns the energy used for humidification from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineHumidification() const;

    /// Returns the energy used for heat recovery from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineHeatRecovery() const;

    /// Returns the energy used for water systems from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineWaterSystems() const;

    /// Returns the energy used for refrigeration from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineRefrigeration() const;

    /// Returns the energy used for generators from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineGenerators() const;

    /// Returns the energy used for all end uses from gasoline in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> gasolineTotalEndUses() const;

    /// Diesel
    /// Returns the energy used for heating from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselHeating() const;

    /// Returns the energy used for cooling from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselCooling() const;

    /// Returns the energy used for interior lighting from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselInteriorLighting() const;

    /// Returns the energy used for exterior lighting from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselExteriorLighting() const;

    /// Returns the energy used for interior equipment from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselInteriorEquipment() const;

    /// Returns the energy used for exterior equipment from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselExteriorEquipment() const;

    /// Returns the energy used for fans from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselFans() const;

    /// Returns the energy used for pumps from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselPumps() const;

    /// Returns the energy used for heat rejection from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselHeatRejection() const;

    /// Returns the energy used for humidification from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselHumidification() const;

    /// Returns the energy used for heat recovery from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselHeatRecovery() const;

    /// Returns the energy used for water systems from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselWaterSystems() const;

    /// Returns the energy used for refrigeration from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselRefrigeration() const;

    /// Returns the energy used for generators from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselGenerators() const;

    /// Returns the energy used for all end uses from diesel in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> dieselTotalEndUses() const;

    /// Coal
    /// Returns the energy used for heating from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalHeating() const;

    /// Returns the energy used for cooling from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalCooling() const;

    /// Returns the energy used for interior lighting from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalInteriorLighting() const;

    /// Returns the energy used for exterior lighting from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalExteriorLighting() const;

    /// Returns the energy used for interior equipment from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalInteriorEquipment() const;

    /// Returns the energy used for exterior equipment from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalExteriorEquipment() const;

    /// Returns the energy used for fans from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalFans() const;

    /// Returns the energy used for pumps from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalPumps() const;

    /// Returns the energy used for heat rejection from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalHeatRejection() const;

    /// Returns the energy used for humidification from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalHumidification() const;

    /// Returns the energy used for heat recovery from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalHeatRecovery() const;

    /// Returns the energy used for water systems from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalWaterSystems() const;

    /// Returns the energy used for refrigeration from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalRefrigeration() const;

    /// Returns the energy used for generators from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalGenerators() const;

    /// Returns the energy used for all end uses from coal in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> coalTotalEndUses() const;

    /// Fuel Oil No 1
    /// Returns the energy used for heating from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Heating() const;

    /// Returns the energy used for cooling from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Cooling() const;

    /// Returns the energy used for interior lighting from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1InteriorLighting() const;

    /// Returns the energy used for exterior lighting from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1ExteriorLighting() const;

    /// Returns the energy used for interior equipment from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1InteriorEquipment() const;

    /// Returns the energy used for exterior equipment from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1ExteriorEquipment() const;

    /// Returns the energy used for fans from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Fans() const;

    /// Returns the energy used for pumps from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Pumps() const;

    /// Returns the energy used for heat rejection from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1HeatRejection() const;

    /// Returns the energy used for humidification from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Humidification() const;

    /// Returns the energy used for heat recovery from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1HeatRecovery() const;

    /// Returns the energy used for water systems from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1WaterSystems() const;

    /// Returns the energy used for refrigeration from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Refrigeration() const;

    /// Returns the energy used for generators from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1Generators() const;

    /// Returns the energy used for all end uses from fuelOilNo1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo1TotalEndUses() const;

    /// Fuel Oil No 2
    /// Returns the energy used for heating from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Heating() const;

    /// Returns the energy used for cooling from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Cooling() const;

    /// Returns the energy used for interior lighting from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2InteriorLighting() const;

    /// Returns the energy used for exterior lighting from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2ExteriorLighting() const;

    /// Returns the energy used for interior equipment from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2InteriorEquipment() const;

    /// Returns the energy used for exterior equipment from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2ExteriorEquipment() const;

    /// Returns the energy used for fans from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Fans() const;

    /// Returns the energy used for pumps from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Pumps() const;

    /// Returns the energy used for heat rejection from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2HeatRejection() const;

    /// Returns the energy used for humidification from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Humidification() const;

    /// Returns the energy used for heat recovery from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2HeatRecovery() const;

    /// Returns the energy used for water systems from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2WaterSystems() const;

    /// Returns the energy used for refrigeration from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Refrigeration() const;

    /// Returns the energy used for generators from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2Generators() const;

    /// Returns the energy used for all end uses from fuelOilNo2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> fuelOilNo2TotalEndUses() const;

    /// Propane
    /// Returns the energy used for heating from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneHeating() const;

    /// Returns the energy used for cooling from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneCooling() const;

    /// Returns the energy used for interior lighting from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneInteriorLighting() const;

    /// Returns the energy used for exterior lighting from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneExteriorLighting() const;

    /// Returns the energy used for interior equipment from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneInteriorEquipment() const;

    /// Returns the energy used for exterior equipment from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneExteriorEquipment() const;

    /// Returns the energy used for fans from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneFans() const;

    /// Returns the energy used for pumps from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propanePumps() const;

    /// Returns the energy used for heat rejection from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneHeatRejection() const;

    /// Returns the energy used for humidification from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneHumidification() const;

    /// Returns the energy used for heat recovery from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneHeatRecovery() const;

    /// Returns the energy used for water systems from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneWaterSystems() const;

    /// Returns the energy used for refrigeration from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneRefrigeration() const;

    /// Returns the energy used for generators from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneGenerators() const;

    /// Returns the energy used for all end uses from propane in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> propaneTotalEndUses() const;

    /// Other Fuel 1
    /// Returns the energy used for heating from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Heating() const;

    /// Returns the energy used for cooling from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Cooling() const;

    /// Returns the energy used for interior lighting from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1InteriorLighting() const;

    /// Returns the energy used for exterior lighting from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1ExteriorLighting() const;

    /// Returns the energy used for interior equipment from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1InteriorEquipment() const;

    /// Returns the energy used for exterior equipment from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1ExteriorEquipment() const;

    /// Returns the energy used for fans from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Fans() const;

    /// Returns the energy used for pumps from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Pumps() const;

    /// Returns the energy used for heat rejection from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1HeatRejection() const;

    /// Returns the energy used for humidification from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Humidification() const;

    /// Returns the energy used for heat recovery from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1HeatRecovery() const;

    /// Returns the energy used for water systems from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1WaterSystems() const;

    /// Returns the energy used for refrigeration from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Refrigeration() const;

    /// Returns the energy used for generators from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1Generators() const;

    /// Returns the energy used for all end uses from otherFuel1 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel1TotalEndUses() const;

    /// Other Fuel 2
    /// Returns the energy used for heating from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Heating() const;

    /// Returns the energy used for cooling from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Cooling() const;

    /// Returns the energy used for interior lighting from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2InteriorLighting() const;

    /// Returns the energy used for exterior lighting from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2ExteriorLighting() const;

    /// Returns the energy used for interior equipment from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2InteriorEquipment() const;

    /// Returns the energy used for exterior equipment from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2ExteriorEquipment() const;

    /// Returns the energy used for fans from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Fans() const;

    /// Returns the energy used for pumps from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Pumps() const;

    /// Returns the energy used for heat rejection from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2HeatRejection() const;

    /// Returns the energy used for humidification from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Humidification() const;

    /// Returns the energy used for heat recovery from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2HeatRecovery() const;

    /// Returns the energy used for water systems from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2WaterSystems() const;

    /// Returns the energy used for refrigeration from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Refrigeration() const;

    /// Returns the energy used for generators from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2Generators() const;

    /// Returns the energy used for all end uses from otherFuel2 in gigajoules.
    /// Requires EnergyPlus simulation output to calculate.
    boost::optional<double> otherFuel2TotalEndUses() const;

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
    using ImplType = detail::Facility_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit Facility(std::shared_ptr<detail::Facility_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Facility");
  };

  /** \relates Facility*/
  using OptionalFacility = boost::optional<Facility>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FACILITY_HPP
