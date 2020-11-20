/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_FACILITY_IMPL_HPP
#define MODEL_FACILITY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

#include "../utilities/data/Attribute.hpp"
#include "../utilities/data/CalibrationResult.hpp"
#include "../utilities/data/EndUses.hpp"
#include "../utilities/data/DataEnums.hpp"

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

      virtual ~Facility_Impl() {}

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
