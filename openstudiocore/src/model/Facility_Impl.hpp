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

#ifndef MODEL_FACILITY_IMPL_HPP
#define MODEL_FACILITY_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ParentObject_Impl.hpp>

#include <utilities/data/Attribute.hpp>
#include <utilities/data/CalibrationResult.hpp>
#include <utilities/data/EndUses.hpp>
#include <utilities/data/DataEnums.hpp>

namespace openstudio {  

class Economics;

namespace model {

class Building;
class Meter;
class ExteriorLights;

namespace detail {

  /** Facility_Impl is a ParentObject_Impl that is the implementation class for Facility.*/
  class MODEL_API Facility_Impl : public ParentObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> totalSiteEnergy READ totalSiteEnergy STORED false);
    Q_PROPERTY(boost::optional<double> netSiteEnergy READ netSiteEnergy STORED false);
    Q_PROPERTY(boost::optional<double> totalSourceEnergy READ totalSourceEnergy STORED false);
    Q_PROPERTY(boost::optional<double> netSourceEnergy READ netSourceEnergy STORED false);
    Q_PROPERTY(boost::optional<double> annualTotalUtilityCost READ annualTotalUtilityCost STORED false);
    Q_PROPERTY(boost::optional<double> annualElectricTotalCost READ annualElectricTotalCost STORED false);
    Q_PROPERTY(boost::optional<double> annualGasTotalCost READ annualGasTotalCost STORED false);
    Q_PROPERTY(boost::optional<double> annualDistrictCoolingTotalCost READ annualDistrictCoolingTotalCost STORED false);
    Q_PROPERTY(boost::optional<double> annualDistrictHeatingTotalCost READ annualDistrictHeatingTotalCost STORED false);
    Q_PROPERTY(boost::optional<double> annualWaterTotalCost READ annualWaterTotalCost STORED false);

    Q_PROPERTY(boost::optional<double> totalEnergyTimeDependentValuation READ totalEnergyTimeDependentValuation STORED false);
    Q_PROPERTY(boost::optional<double> totalCostTimeDependentValuation READ totalCostTimeDependentValuation STORED false);
    Q_PROPERTY(boost::optional<double> electricityEnergyTimeDependentValuation READ electricityEnergyTimeDependentValuation STORED false);
    Q_PROPERTY(boost::optional<double> electricityCostTimeDependentValuation READ electricityCostTimeDependentValuation STORED false);
    Q_PROPERTY(boost::optional<double> fossilFuelEnergyTimeDependentValuation READ fossilFuelEnergyTimeDependentValuation STORED false);
    Q_PROPERTY(boost::optional<double> fossilFuelCostTimeDependentValuation READ fossilFuelCostTimeDependentValuation STORED false);

    Q_PROPERTY(boost::optional<double> economicsCapitalCost READ economicsCapitalCost STORED false);
    Q_PROPERTY(boost::optional<double> economicsEnergyCost READ economicsEnergyCost STORED false);
    Q_PROPERTY(boost::optional<double> economicsTLCC READ economicsTLCC STORED false);
    Q_PROPERTY(boost::optional<double> economicsVirtualRateGas READ economicsVirtualRateGas STORED false);
    Q_PROPERTY(boost::optional<double> economicsVirtualRateElec READ economicsVirtualRateElec STORED false);
    Q_PROPERTY(boost::optional<double> economicsVirtualRateCombined READ economicsVirtualRateCombined STORED false);
    Q_PROPERTY(boost::optional<double> economicsSPB READ economicsSPB STORED false);
    Q_PROPERTY(boost::optional<double> economicsDPB READ economicsDPB STORED false);
    Q_PROPERTY(boost::optional<double> economicsNPV READ economicsNPV STORED false);
    Q_PROPERTY(boost::optional<double> economicsIRR READ economicsIRR STORED false);

    Q_PROPERTY(boost::optional<double> electricityHeating READ electricityHeating STORED false);
    Q_PROPERTY(boost::optional<double> electricityCooling READ electricityCooling STORED false);
    Q_PROPERTY(boost::optional<double> electricityInteriorLighting READ electricityInteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> electricityExteriorLighting READ electricityExteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> electricityInteriorEquipment READ electricityInteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> electricityExteriorEquipment READ electricityExteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> electricityFans READ electricityFans STORED false);
    Q_PROPERTY(boost::optional<double> electricityPumps READ electricityPumps STORED false);
    Q_PROPERTY(boost::optional<double> electricityHeatRejection READ electricityHeatRejection STORED false);
    Q_PROPERTY(boost::optional<double> electricityHumidification READ electricityHumidification STORED false);
    Q_PROPERTY(boost::optional<double> electricityHeatRecovery READ electricityHeatRecovery STORED false);
    Q_PROPERTY(boost::optional<double> electricityWaterSystems READ electricityWaterSystems STORED false);
    Q_PROPERTY(boost::optional<double> electricityRefrigeration READ electricityRefrigeration STORED false);
    Q_PROPERTY(boost::optional<double> electricityGenerators READ electricityGenerators STORED false);
    Q_PROPERTY(boost::optional<double> electricityTotalEndUses READ electricityTotalEndUses STORED false);

    Q_PROPERTY(boost::optional<double> naturalGasHeating READ naturalGasHeating STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasCooling READ naturalGasCooling STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasInteriorLighting READ naturalGasInteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasExteriorLighting READ naturalGasExteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasInteriorEquipment READ naturalGasInteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasExteriorEquipment READ naturalGasExteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasFans READ naturalGasFans STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasPumps READ naturalGasPumps STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasHeatRejection READ naturalGasHeatRejection STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasHumidification READ naturalGasHumidification STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasHeatRecovery READ naturalGasHeatRecovery STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasWaterSystems READ naturalGasWaterSystems STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasRefrigeration READ naturalGasRefrigeration STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasGenerators READ naturalGasGenerators STORED false);
    Q_PROPERTY(boost::optional<double> naturalGasTotalEndUses READ naturalGasTotalEndUses STORED false);

    Q_PROPERTY(boost::optional<double> otherFuelHeating READ otherFuelHeating STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelCooling READ otherFuelCooling STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelInteriorLighting READ otherFuelInteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelExteriorLighting READ otherFuelExteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelInteriorEquipment READ otherFuelInteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelExteriorEquipment READ otherFuelExteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelFans READ otherFuelFans STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelPumps READ otherFuelPumps STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelHeatRejection READ otherFuelHeatRejection STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelHumidification READ otherFuelHumidification STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelHeatRecovery READ otherFuelHeatRecovery STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelWaterSystems READ otherFuelWaterSystems STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelRefrigeration READ otherFuelRefrigeration STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelGenerators READ otherFuelGenerators STORED false);
    Q_PROPERTY(boost::optional<double> otherFuelTotalEndUses READ otherFuelTotalEndUses STORED false);

    Q_PROPERTY(boost::optional<double> districtCoolingHeating READ districtCoolingHeating STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingCooling READ districtCoolingCooling STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingInteriorLighting READ districtCoolingInteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingExteriorLighting READ districtCoolingExteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingInteriorEquipment READ districtCoolingInteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingExteriorEquipment READ districtCoolingExteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingFans READ districtCoolingFans STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingPumps READ districtCoolingPumps STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingHeatRejection READ districtCoolingHeatRejection STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingHumidification READ districtCoolingHumidification STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingHeatRecovery READ districtCoolingHeatRecovery STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingWaterSystems READ districtCoolingWaterSystems STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingRefrigeration READ districtCoolingRefrigeration STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingGenerators READ districtCoolingGenerators STORED false);
    Q_PROPERTY(boost::optional<double> districtCoolingTotalEndUses READ districtCoolingTotalEndUses STORED false);

    Q_PROPERTY(boost::optional<double> districtHeatingHeating READ districtHeatingHeating STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingCooling READ districtHeatingCooling STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingInteriorLighting READ districtHeatingInteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingExteriorLighting READ districtHeatingExteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingInteriorEquipment READ districtHeatingInteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingExteriorEquipment READ districtHeatingExteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingFans READ districtHeatingFans STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingPumps READ districtHeatingPumps STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingHeatRejection READ districtHeatingHeatRejection STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingHumidification READ districtHeatingHumidification STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingHeatRecovery READ districtHeatingHeatRecovery STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingWaterSystems READ districtHeatingWaterSystems STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingRefrigeration READ districtHeatingRefrigeration STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingGenerators READ districtHeatingGenerators STORED false);
    Q_PROPERTY(boost::optional<double> districtHeatingTotalEndUses READ districtHeatingTotalEndUses STORED false);

    Q_PROPERTY(boost::optional<double> waterHeating READ waterHeating STORED false);
    Q_PROPERTY(boost::optional<double> waterCooling READ waterCooling STORED false);
    Q_PROPERTY(boost::optional<double> waterInteriorLighting READ waterInteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> waterExteriorLighting READ waterExteriorLighting STORED false);
    Q_PROPERTY(boost::optional<double> waterInteriorEquipment READ waterInteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> waterExteriorEquipment READ waterExteriorEquipment STORED false);
    Q_PROPERTY(boost::optional<double> waterFans READ waterFans STORED false);
    Q_PROPERTY(boost::optional<double> waterPumps READ waterPumps STORED false);
    Q_PROPERTY(boost::optional<double> waterHeatRejection READ waterHeatRejection STORED false);
    Q_PROPERTY(boost::optional<double> waterHumidification READ waterHumidification STORED false);
    Q_PROPERTY(boost::optional<double> waterHeatRecovery READ waterHeatRecovery STORED false);
    Q_PROPERTY(boost::optional<double> waterWaterSystems READ waterWaterSystems STORED false);
    Q_PROPERTY(boost::optional<double> waterRefrigeration READ waterRefrigeration STORED false);
    Q_PROPERTY(boost::optional<double> waterGenerators READ waterGenerators STORED false);
    Q_PROPERTY(boost::optional<double> waterTotalEndUses READ waterTotalEndUses STORED false);

    Q_PROPERTY(boost::optional<double> hoursHeatingSetpointNotMet READ hoursHeatingSetpointNotMet STORED false);
    Q_PROPERTY(boost::optional<double> hoursCoolingSetpointNotMet READ hoursCoolingSetpointNotMet STORED false);

    Q_PROPERTY(boost::optional<openstudio::Attribute> endUsesAttribute READ endUsesAttribute STORED false);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> building READ buildingAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> meters READ metersAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> exteriorLights READ exteriorLightsAsModelObjects);
   public:

    /** @name Constructors and Destructors */
    //@{

    Facility_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Facility_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    Facility_Impl(const Facility_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    virtual ~Facility_Impl() {}

    //@}
    
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

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Children */
    //@{
   
    /// get the building
    boost::optional<Building> building() const;

    // get meter requests for the facility
    std::vector<Meter> meters() const;
    
    boost::optional<Meter> getMeterByFuelType(
      const FuelType& fuelType,
      const std::string& reportingFrequency,
      const boost::optional<EndUseType>& endUseType,
      const boost::optional<std::string>& specificEndUse) const;

    /// Returns all ExteriorLights.
    std::vector<ExteriorLights> exteriorLights() const;

    //@}
    /** @name Other */
    //@{

    boost::optional<double> totalSiteEnergy() const;
    boost::optional<double> netSiteEnergy() const;
    boost::optional<double> totalSourceEnergy() const;
    boost::optional<double> netSourceEnergy() const;

    boost::optional<double> annualTotalCost(const FuelType& fuel) const;
    boost::optional<double> annualTotalCostPerBldgArea(const FuelType& fuel) const;
    boost::optional<double> annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const;

    boost::optional<double> annualTotalUtilityCost() const;
    boost::optional<double> annualElectricTotalCost() const;
    boost::optional<double> annualGasTotalCost() const;
    boost::optional<double> annualDistrictCoolingTotalCost() const;
    boost::optional<double> annualDistrictHeatingTotalCost() const;
    boost::optional<double> annualWaterTotalCost() const;

    boost::optional<double> totalEnergyTimeDependentValuation() const;
    boost::optional<double> totalCostTimeDependentValuation() const;
    boost::optional<double> electricityEnergyTimeDependentValuation() const;
    boost::optional<double> electricityCostTimeDependentValuation() const;
    boost::optional<double> fossilFuelEnergyTimeDependentValuation() const;
    boost::optional<double> fossilFuelCostTimeDependentValuation() const;

    boost::optional<double> economicsCapitalCost() const;
    boost::optional<double> economicsEnergyCost() const;
    boost::optional<double> economicsTLCC() const;
    boost::optional<double> economicsVirtualRateGas() const;
    boost::optional<double> economicsVirtualRateElec() const;
    boost::optional<double> economicsVirtualRateCombined() const;
    boost::optional<double> economicsSPB() const;
    boost::optional<double> economicsDPB() const;
    boost::optional<double> economicsNPV() const;
    boost::optional<double> economicsIRR() const;

    boost::optional<double> electricityHeating() const;
    boost::optional<double> electricityCooling() const;
    boost::optional<double> electricityInteriorLighting() const;
    boost::optional<double> electricityExteriorLighting() const;
    boost::optional<double> electricityInteriorEquipment() const;
    boost::optional<double> electricityExteriorEquipment() const;
    boost::optional<double> electricityFans() const;
    boost::optional<double> electricityPumps() const;
    boost::optional<double> electricityHeatRejection() const;
    boost::optional<double> electricityHumidification() const;
    boost::optional<double> electricityHeatRecovery() const;
    boost::optional<double> electricityWaterSystems() const;
    boost::optional<double> electricityRefrigeration() const;
    boost::optional<double> electricityGenerators() const;
    boost::optional<double> electricityTotalEndUses() const;

    boost::optional<double> naturalGasHeating() const;
    boost::optional<double> naturalGasCooling() const;
    boost::optional<double> naturalGasInteriorLighting() const;
    boost::optional<double> naturalGasExteriorLighting() const;
    boost::optional<double> naturalGasInteriorEquipment() const;
    boost::optional<double> naturalGasExteriorEquipment() const;
    boost::optional<double> naturalGasFans() const;
    boost::optional<double> naturalGasPumps() const;
    boost::optional<double> naturalGasHeatRejection() const;
    boost::optional<double> naturalGasHumidification() const;
    boost::optional<double> naturalGasHeatRecovery() const;
    boost::optional<double> naturalGasWaterSystems() const;
    boost::optional<double> naturalGasRefrigeration() const;
    boost::optional<double> naturalGasGenerators() const;
    boost::optional<double> naturalGasTotalEndUses() const;

    boost::optional<double> otherFuelHeating() const;
    boost::optional<double> otherFuelCooling() const;
    boost::optional<double> otherFuelInteriorLighting() const;
    boost::optional<double> otherFuelExteriorLighting() const;
    boost::optional<double> otherFuelInteriorEquipment() const;
    boost::optional<double> otherFuelExteriorEquipment() const;
    boost::optional<double> otherFuelFans() const;
    boost::optional<double> otherFuelPumps() const;
    boost::optional<double> otherFuelHeatRejection() const;
    boost::optional<double> otherFuelHumidification() const;
    boost::optional<double> otherFuelHeatRecovery() const;
    boost::optional<double> otherFuelWaterSystems() const;
    boost::optional<double> otherFuelRefrigeration() const;
    boost::optional<double> otherFuelGenerators() const;
    boost::optional<double> otherFuelTotalEndUses() const;

    boost::optional<double> districtCoolingHeating() const;
    boost::optional<double> districtCoolingCooling() const;
    boost::optional<double> districtCoolingInteriorLighting() const;
    boost::optional<double> districtCoolingExteriorLighting() const;
    boost::optional<double> districtCoolingInteriorEquipment() const;
    boost::optional<double> districtCoolingExteriorEquipment() const;
    boost::optional<double> districtCoolingFans() const;
    boost::optional<double> districtCoolingPumps() const;
    boost::optional<double> districtCoolingHeatRejection() const;
    boost::optional<double> districtCoolingHumidification() const;
    boost::optional<double> districtCoolingHeatRecovery() const;
    boost::optional<double> districtCoolingWaterSystems() const;
    boost::optional<double> districtCoolingRefrigeration() const;
    boost::optional<double> districtCoolingGenerators() const;
    boost::optional<double> districtCoolingTotalEndUses() const;

    boost::optional<double> districtHeatingHeating() const;
    boost::optional<double> districtHeatingCooling() const;
    boost::optional<double> districtHeatingInteriorLighting() const;
    boost::optional<double> districtHeatingExteriorLighting() const;
    boost::optional<double> districtHeatingInteriorEquipment() const;
    boost::optional<double> districtHeatingExteriorEquipment() const;
    boost::optional<double> districtHeatingFans() const;
    boost::optional<double> districtHeatingPumps() const;
    boost::optional<double> districtHeatingHeatRejection() const;
    boost::optional<double> districtHeatingHumidification() const;
    boost::optional<double> districtHeatingHeatRecovery() const;
    boost::optional<double> districtHeatingWaterSystems() const;
    boost::optional<double> districtHeatingRefrigeration() const;
    boost::optional<double> districtHeatingGenerators() const;
    boost::optional<double> districtHeatingTotalEndUses() const;

    boost::optional<double> waterHeating() const;
    boost::optional<double> waterCooling() const;
    boost::optional<double> waterInteriorLighting() const;
    boost::optional<double> waterExteriorLighting() const;
    boost::optional<double> waterInteriorEquipment() const;
    boost::optional<double> waterExteriorEquipment() const;
    boost::optional<double> waterFans() const;
    boost::optional<double> waterPumps() const;
    boost::optional<double> waterHeatRejection() const;
    boost::optional<double> waterHumidification() const;
    boost::optional<double> waterHeatRecovery() const;
    boost::optional<double> waterWaterSystems() const;
    boost::optional<double> waterRefrigeration() const;
    boost::optional<double> waterGenerators() const;
    boost::optional<double> waterTotalEndUses() const;

    boost::optional<double> hoursHeatingSetpointNotMet() const;
    boost::optional<double> hoursCoolingSetpointNotMet() const;

    boost::optional<EndUses> endUses() const;
    boost::optional<Attribute> endUsesAttribute() const;

    boost::optional<CalibrationResult> calibrationResult() const;
    boost::optional<Attribute> calibrationResultAttribute() const;

    //@}
   protected:
   
    bool initEconomics(Economics& economics) const;
    boost::optional<double> elecUse() const;
    boost::optional<double> elecCost() const;
    boost::optional<double> gasUse() const;
    boost::optional<double> gasCost() const;
    boost::optional<double> getElecOrGasUse(bool getGas = true) const;
    boost::optional<double> getElecOrGasCost(bool bGetGas = true) const;

   private:
    REGISTER_LOGGER("openstudio.model.Facility");

    boost::optional<ModelObject> buildingAsModelObject() const;
    std::vector<ModelObject> metersAsModelObjects() const;
    std::vector<ModelObject> exteriorLightsAsModelObjects() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_FACILITY_IMPL_HPP

