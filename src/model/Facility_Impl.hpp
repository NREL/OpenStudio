/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FACILITY_IMPL_HPP
#define MODEL_FACILITY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

#include "../utilities/data/CalibrationResult.hpp"
#include "../utilities/data/EndUses.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Building;
  class OutputMeter;
  class ExteriorLights;
  class ExteriorFuelEquipment;
  class ExteriorWaterEquipment;

  namespace detail {

    /** Facility_Impl is a ParentObject_Impl that is the implementation class for Facility.*/
    class MODEL_API Facility_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      Facility_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Facility_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Facility_Impl(const Facility_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Facility_Impl() override = default;

      //@}

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

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Children */
      //@{

      /// get the building
      boost::optional<Building> building() const;

      // get meter requests for the facility
      std::vector<OutputMeter> meters() const;

      boost::optional<OutputMeter> getMeterByFuelType(const FuelType& fuelType, const std::string& reportingFrequency,
                                                      const boost::optional<EndUseType>& endUseType,
                                                      const boost::optional<std::string>& specificEndUse) const;

      /// Returns all ExteriorLights.
      std::vector<ExteriorLights> exteriorLights() const;

      /// Returns all ExteriorFuelEquipment.
      std::vector<ExteriorFuelEquipment> exteriorFuelEquipments() const;

      /// Returns all ExteriorWaterEquipment.
      std::vector<ExteriorWaterEquipment> exteriorWaterEquipments() const;

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

      // gasoline
      boost::optional<double> gasolineHeating() const;
      boost::optional<double> gasolineCooling() const;
      boost::optional<double> gasolineInteriorLighting() const;
      boost::optional<double> gasolineExteriorLighting() const;
      boost::optional<double> gasolineInteriorEquipment() const;
      boost::optional<double> gasolineExteriorEquipment() const;
      boost::optional<double> gasolineFans() const;
      boost::optional<double> gasolinePumps() const;
      boost::optional<double> gasolineHeatRejection() const;
      boost::optional<double> gasolineHumidification() const;
      boost::optional<double> gasolineHeatRecovery() const;
      boost::optional<double> gasolineWaterSystems() const;
      boost::optional<double> gasolineRefrigeration() const;
      boost::optional<double> gasolineGenerators() const;
      boost::optional<double> gasolineTotalEndUses() const;

      // diesel
      boost::optional<double> dieselHeating() const;
      boost::optional<double> dieselCooling() const;
      boost::optional<double> dieselInteriorLighting() const;
      boost::optional<double> dieselExteriorLighting() const;
      boost::optional<double> dieselInteriorEquipment() const;
      boost::optional<double> dieselExteriorEquipment() const;
      boost::optional<double> dieselFans() const;
      boost::optional<double> dieselPumps() const;
      boost::optional<double> dieselHeatRejection() const;
      boost::optional<double> dieselHumidification() const;
      boost::optional<double> dieselHeatRecovery() const;
      boost::optional<double> dieselWaterSystems() const;
      boost::optional<double> dieselRefrigeration() const;
      boost::optional<double> dieselGenerators() const;
      boost::optional<double> dieselTotalEndUses() const;

      // coal
      boost::optional<double> coalHeating() const;
      boost::optional<double> coalCooling() const;
      boost::optional<double> coalInteriorLighting() const;
      boost::optional<double> coalExteriorLighting() const;
      boost::optional<double> coalInteriorEquipment() const;
      boost::optional<double> coalExteriorEquipment() const;
      boost::optional<double> coalFans() const;
      boost::optional<double> coalPumps() const;
      boost::optional<double> coalHeatRejection() const;
      boost::optional<double> coalHumidification() const;
      boost::optional<double> coalHeatRecovery() const;
      boost::optional<double> coalWaterSystems() const;
      boost::optional<double> coalRefrigeration() const;
      boost::optional<double> coalGenerators() const;
      boost::optional<double> coalTotalEndUses() const;

      // Fuel oil No 1
      boost::optional<double> fuelOilNo1Heating() const;
      boost::optional<double> fuelOilNo1Cooling() const;
      boost::optional<double> fuelOilNo1InteriorLighting() const;
      boost::optional<double> fuelOilNo1ExteriorLighting() const;
      boost::optional<double> fuelOilNo1InteriorEquipment() const;
      boost::optional<double> fuelOilNo1ExteriorEquipment() const;
      boost::optional<double> fuelOilNo1Fans() const;
      boost::optional<double> fuelOilNo1Pumps() const;
      boost::optional<double> fuelOilNo1HeatRejection() const;
      boost::optional<double> fuelOilNo1Humidification() const;
      boost::optional<double> fuelOilNo1HeatRecovery() const;
      boost::optional<double> fuelOilNo1WaterSystems() const;
      boost::optional<double> fuelOilNo1Refrigeration() const;
      boost::optional<double> fuelOilNo1Generators() const;
      boost::optional<double> fuelOilNo1TotalEndUses() const;

      // Fuel oil No 2
      boost::optional<double> fuelOilNo2Heating() const;
      boost::optional<double> fuelOilNo2Cooling() const;
      boost::optional<double> fuelOilNo2InteriorLighting() const;
      boost::optional<double> fuelOilNo2ExteriorLighting() const;
      boost::optional<double> fuelOilNo2InteriorEquipment() const;
      boost::optional<double> fuelOilNo2ExteriorEquipment() const;
      boost::optional<double> fuelOilNo2Fans() const;
      boost::optional<double> fuelOilNo2Pumps() const;
      boost::optional<double> fuelOilNo2HeatRejection() const;
      boost::optional<double> fuelOilNo2Humidification() const;
      boost::optional<double> fuelOilNo2HeatRecovery() const;
      boost::optional<double> fuelOilNo2WaterSystems() const;
      boost::optional<double> fuelOilNo2Refrigeration() const;
      boost::optional<double> fuelOilNo2Generators() const;
      boost::optional<double> fuelOilNo2TotalEndUses() const;

      // Propane
      boost::optional<double> propaneHeating() const;
      boost::optional<double> propaneCooling() const;
      boost::optional<double> propaneInteriorLighting() const;
      boost::optional<double> propaneExteriorLighting() const;
      boost::optional<double> propaneInteriorEquipment() const;
      boost::optional<double> propaneExteriorEquipment() const;
      boost::optional<double> propaneFans() const;
      boost::optional<double> propanePumps() const;
      boost::optional<double> propaneHeatRejection() const;
      boost::optional<double> propaneHumidification() const;
      boost::optional<double> propaneHeatRecovery() const;
      boost::optional<double> propaneWaterSystems() const;
      boost::optional<double> propaneRefrigeration() const;
      boost::optional<double> propaneGenerators() const;
      boost::optional<double> propaneTotalEndUses() const;

      // Other Fuel 1
      boost::optional<double> otherFuel1Heating() const;
      boost::optional<double> otherFuel1Cooling() const;
      boost::optional<double> otherFuel1InteriorLighting() const;
      boost::optional<double> otherFuel1ExteriorLighting() const;
      boost::optional<double> otherFuel1InteriorEquipment() const;
      boost::optional<double> otherFuel1ExteriorEquipment() const;
      boost::optional<double> otherFuel1Fans() const;
      boost::optional<double> otherFuel1Pumps() const;
      boost::optional<double> otherFuel1HeatRejection() const;
      boost::optional<double> otherFuel1Humidification() const;
      boost::optional<double> otherFuel1HeatRecovery() const;
      boost::optional<double> otherFuel1WaterSystems() const;
      boost::optional<double> otherFuel1Refrigeration() const;
      boost::optional<double> otherFuel1Generators() const;
      boost::optional<double> otherFuel1TotalEndUses() const;

      // Other Fuel2
      boost::optional<double> otherFuel2Heating() const;
      boost::optional<double> otherFuel2Cooling() const;
      boost::optional<double> otherFuel2InteriorLighting() const;
      boost::optional<double> otherFuel2ExteriorLighting() const;
      boost::optional<double> otherFuel2InteriorEquipment() const;
      boost::optional<double> otherFuel2ExteriorEquipment() const;
      boost::optional<double> otherFuel2Fans() const;
      boost::optional<double> otherFuel2Pumps() const;
      boost::optional<double> otherFuel2HeatRejection() const;
      boost::optional<double> otherFuel2Humidification() const;
      boost::optional<double> otherFuel2HeatRecovery() const;
      boost::optional<double> otherFuel2WaterSystems() const;
      boost::optional<double> otherFuel2Refrigeration() const;
      boost::optional<double> otherFuel2Generators() const;
      boost::optional<double> otherFuel2TotalEndUses() const;

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

      boost::optional<CalibrationResult> calibrationResult() const;

      //@}
     protected:
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
      std::vector<ModelObject> exteriorFuelEquipmentAsModelObjects() const;
      std::vector<ModelObject> exteriorWaterEquipmentAsModelObjects() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FACILITY_IMPL_HPP
