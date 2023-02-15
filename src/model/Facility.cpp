/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Facility.hpp"
#include "Facility_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "LifeCycleCost_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ExteriorLights.hpp"
#include "ExteriorLights_Impl.hpp"
#include "ExteriorFuelEquipment.hpp"
#include "ExteriorFuelEquipment_Impl.hpp"
#include "ExteriorWaterEquipment.hpp"
#include "ExteriorWaterEquipment_Impl.hpp"
#include "LifeCycleCostParameters.hpp"
#include "LifeCycleCostParameters_Impl.hpp"
#include "OutputMeter.hpp"
#include "OutputMeter_Impl.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "UtilityBill.hpp"
#include "UtilityBill_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/sql/SqlFile.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;
using openstudio::EndUses;
using openstudio::CalibrationResult;

namespace openstudio {
namespace model {

  namespace detail {

    Facility_Impl::Facility_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Facility::iddObjectType());
    }

    Facility_Impl::Facility_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Facility::iddObjectType());
    }

    Facility_Impl::Facility_Impl(const Facility_Impl& other, Model_Impl* model, bool keepHandle) : ParentObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> Facility_Impl::parent() const {
      return boost::optional<ParentObject>();
    }

    std::vector<ModelObject> Facility_Impl::children() const {
      std::vector<ModelObject> result;

      OutputMeterVector meters = this->meters();
      result.insert(result.end(), meters.begin(), meters.end());

      // building
      OptionalBuilding building = this->building();
      if (building) {
        result.push_back(*building);
      }

      ExteriorLightsVector exteriorLights = this->exteriorLights();
      result.insert(result.end(), exteriorLights.begin(), exteriorLights.end());

      ExteriorFuelEquipmentVector exteriorFuelEquipments = this->exteriorFuelEquipments();
      result.insert(result.end(), exteriorFuelEquipments.begin(), exteriorFuelEquipments.end());

      ExteriorWaterEquipmentVector exteriorWaterEquipments = this->exteriorWaterEquipments();
      result.insert(result.end(), exteriorWaterEquipments.begin(), exteriorWaterEquipments.end());

      return result;
    }

    std::vector<IddObjectType> Facility_Impl::allowableChildTypes() const {
      std::vector<IddObjectType> result;
      result.push_back(IddObjectType::OS_Building);
      return result;
    }

    const std::vector<std::string>& Facility_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Facility_Impl::iddObjectType() const {
      return Facility::iddObjectType();
    }

    boost::optional<Building> Facility_Impl::building() const {
      return this->model().building();
    }

    /// get meter requests for the facility
    std::vector<OutputMeter> Facility_Impl::meters() const {
      OutputMeterVector result;
      OutputMeterVector meters = this->model().getConcreteModelObjects<OutputMeter>();
      for (const OutputMeter& meter : meters) {
        if (meter.installLocationType() && (InstallLocationType::Facility == meter.installLocationType().get().value())) {
          result.push_back(meter);
        }
      }
      return result;
    }

    boost::optional<OutputMeter> Facility_Impl::getMeterByFuelType(const FuelType& fuelType, const std::string& reportingFrequency,
                                                                   const boost::optional<EndUseType>& endUseType,
                                                                   const boost::optional<std::string>& specificEndUse) const {
      OptionalOutputMeter result;
      for (const OutputMeter& meter : this->meters()) {
        if (meter.fuelType() && (meter.fuelType() == fuelType)) {
          if (istringEqual(meter.reportingFrequency(), reportingFrequency)) {
            OptionalEndUseType meterEndUseType = meter.endUseType();
            if ((!endUseType && !meterEndUseType) || (endUseType && meterEndUseType && (*meterEndUseType == *endUseType))) {
              OptionalString meterSpecificEndUse = meter.specificEndUse();
              if ((!specificEndUse && !meterSpecificEndUse)
                  || (specificEndUse && meterSpecificEndUse && istringEqual(*meterSpecificEndUse, *specificEndUse))) {
                if (result) {
                  LOG(Warn, "Found duplicate meters: " << result->name() << " and " << meter.name() << ".");
                } else {
                  result = meter;
                }
              }
            }
          }
        }
      }
      return result;
    }

    std::vector<ExteriorLights> Facility_Impl::exteriorLights() const {
      return model().getConcreteModelObjects<ExteriorLights>();
    }

    std::vector<ExteriorFuelEquipment> Facility_Impl::exteriorFuelEquipments() const {
      return model().getConcreteModelObjects<ExteriorFuelEquipment>();
    }

    std::vector<ExteriorWaterEquipment> Facility_Impl::exteriorWaterEquipments() const {
      return model().getConcreteModelObjects<ExteriorWaterEquipment>();
    }

    OptionalDouble Facility_Impl::totalSiteEnergy() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->totalSiteEnergy();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::netSiteEnergy() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->netSiteEnergy();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::totalSourceEnergy() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->totalSourceEnergy();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::netSourceEnergy() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->netSourceEnergy();
      }
      return OptionalDouble();
    }

    // pass in "Electric", "NaturalGas", or "Other"
    OptionalDouble Facility_Impl::annualTotalCost(const FuelType& fuel) const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->annualTotalCost(fuel);
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::annualTotalCostPerBldgArea(const FuelType& fuel) const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->annualTotalCostPerBldgArea(fuel);
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->annualTotalCostPerNetConditionedBldgArea(fuel);
      }

      return OptionalDouble();
    }

    boost::optional<double> Facility_Impl::annualTotalUtilityCost() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->annualTotalUtilityCost();
      }
      return OptionalDouble();
    }

    boost::optional<double> Facility_Impl::annualElectricTotalCost() const {
      return annualTotalCost(FuelType::Electricity);
    }

    boost::optional<double> Facility_Impl::annualGasTotalCost() const {
      return annualTotalCost(FuelType::Gas);
    }

    boost::optional<double> Facility_Impl::annualDistrictCoolingTotalCost() const {
      return annualTotalCost(FuelType::DistrictCooling);
    }

    boost::optional<double> Facility_Impl::annualDistrictHeatingTotalCost() const {
      return annualTotalCost(FuelType::DistrictHeating);
    }

    boost::optional<double> Facility_Impl::annualWaterTotalCost() const {
      return annualTotalCost(FuelType::Water);
    }

    OptionalDouble Facility_Impl::economicsCapitalCost() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsEnergyCost() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->economicsEnergyCost();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::economicsVirtualRateGas() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsVirtualRateElec() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsVirtualRateCombined() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::getElecOrGasUse(bool bGetGas) const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->getElecOrGasUse(bGetGas);
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::getElecOrGasCost(bool bGetGas) const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->getElecOrGasCost(bGetGas);
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasUse() const {
      return getElecOrGasUse();
    }

    OptionalDouble Facility_Impl::gasCost() const {
      return getElecOrGasCost();
    }

    OptionalDouble Facility_Impl::elecUse() const {
      return getElecOrGasUse(false);
    }

    OptionalDouble Facility_Impl::elecCost() const {
      return getElecOrGasCost(false);
    }

    OptionalDouble Facility_Impl::economicsTLCC() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsSPB() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsDPB() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsNPV() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::economicsIRR() const {
      OptionalDouble result;
      return result;
    }

    OptionalDouble Facility_Impl::electricityHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::electricityTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->electricityTotalEndUses();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::naturalGasTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->naturalGasTotalEndUses();
      }
      return OptionalDouble();
    }

    // Gasoline
    OptionalDouble Facility_Impl::gasolineHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolinePumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolinePumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::gasolineTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->gasolineTotalEndUses();
      }
      return OptionalDouble();
    }

    // Diesel
    OptionalDouble Facility_Impl::dieselHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::dieselTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->dieselTotalEndUses();
      }
      return OptionalDouble();
    }

    // Coal
    OptionalDouble Facility_Impl::coalHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::coalTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->coalTotalEndUses();
      }
      return OptionalDouble();
    }

    // Fuel Oil No 1
    OptionalDouble Facility_Impl::fuelOilNo1Heating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Heating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1Cooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Cooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1InteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1InteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1ExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1ExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1InteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1InteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1ExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1ExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1Fans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Fans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1Pumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Pumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1HeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1HeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1Humidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Humidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1HeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1HeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1WaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1WaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1Refrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Refrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1Generators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1Generators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo1TotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo1TotalEndUses();
      }
      return OptionalDouble();
    }

    // Fuel Oil No 2
    OptionalDouble Facility_Impl::fuelOilNo2Heating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Heating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2Cooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Cooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2InteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2InteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2ExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2ExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2InteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2InteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2ExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2ExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2Fans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Fans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2Pumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Pumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2HeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2HeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2Humidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Humidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2HeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2HeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2WaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2WaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2Refrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Refrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2Generators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2Generators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::fuelOilNo2TotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->fuelOilNo2TotalEndUses();
      }
      return OptionalDouble();
    }

    // Propane
    OptionalDouble Facility_Impl::propaneHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propanePumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propanePumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::propaneTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->propaneTotalEndUses();
      }
      return OptionalDouble();
    }

    // Other Fuel 1
    OptionalDouble Facility_Impl::otherFuel1Heating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Heating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1Cooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Cooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1InteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1InteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1ExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1ExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1InteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1InteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1ExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1ExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1Fans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Fans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1Pumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Pumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1HeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1HeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1Humidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Humidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1HeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1HeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1WaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1WaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1Refrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Refrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1Generators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1Generators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel1TotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel1TotalEndUses();
      }
      return OptionalDouble();
    }

    // Other Fuel 2
    OptionalDouble Facility_Impl::otherFuel2Heating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Heating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2Cooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Cooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2InteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2InteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2ExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2ExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2InteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2InteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2ExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2ExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2Fans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Fans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2Pumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Pumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2HeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2HeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2Humidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Humidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2HeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2HeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2WaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2WaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2Refrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Refrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2Generators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2Generators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::otherFuel2TotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->otherFuel2TotalEndUses();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtCoolingTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtCoolingTotalEndUses();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::districtHeatingTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->districtHeatingTotalEndUses();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterHeating() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterHeating();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterCooling() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterCooling();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterInteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterInteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterExteriorLighting() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterExteriorLighting();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterInteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterInteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterExteriorEquipment() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterExteriorEquipment();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterFans() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterFans();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterPumps() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterPumps();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterHeatRejection() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterHeatRejection();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterHumidification() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterHumidification();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterHeatRecovery() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterHeatRecovery();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterWaterSystems() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterWaterSystems();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterRefrigeration() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterRefrigeration();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterGenerators() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterGenerators();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::waterTotalEndUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->waterTotalEndUses();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::hoursHeatingSetpointNotMet() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->hoursHeatingSetpointNotMet();
      }
      return OptionalDouble();
    }

    OptionalDouble Facility_Impl::hoursCoolingSetpointNotMet() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->hoursCoolingSetpointNotMet();
      }
      return OptionalDouble();
    }

    boost::optional<EndUses> Facility_Impl::endUses() const {
      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        return mySqlFile->endUses();
      }
      return boost::optional<EndUses>();
    }

    boost::optional<CalibrationResult> Facility_Impl::calibrationResult() const {
      boost::optional<CalibrationResult> result;

      OptionalSqlFile mySqlFile = model().sqlFile();
      if (mySqlFile && mySqlFile->connectionOpen()) {
        result = CalibrationResult();
        for (const model::UtilityBill& utilityBill : this->model().getConcreteModelObjects<model::UtilityBill>()) {
          CalibrationUtilityBill calibrationUtilityBill(
            utilityBill.name().get(), utilityBill.fuelType(), utilityBill.meterInstallLocation(), utilityBill.meterSpecificInstallLocation(),
            utilityBill.meterEndUseCategory(), utilityBill.meterSpecificEndUse(), utilityBill.consumptionUnit(),
            utilityBill.consumptionUnitConversionFactor(), utilityBill.peakDemandUnit(), utilityBill.peakDemandUnitConversionFactor(),
            utilityBill.timestepsInPeakDemandWindow(), utilityBill.minutesInPeakDemandWindow(), utilityBill.numberBillingPeriodsInCalculations(),
            utilityBill.CVRMSE(), utilityBill.NMBE());

          for (const model::BillingPeriod& billingPeriod : utilityBill.billingPeriods()) {
            CalibrationBillingPeriod calibrationBillingPeriod(billingPeriod.startDate(), billingPeriod.numberOfDays(), utilityBill.consumptionUnit(),
                                                              utilityBill.peakDemandUnit(), billingPeriod.consumption(), billingPeriod.peakDemand(),
                                                              billingPeriod.totalCost(), billingPeriod.modelConsumption(),
                                                              billingPeriod.modelPeakDemand(), billingPeriod.modelTotalCost());

            calibrationUtilityBill.addBillingPeriod(calibrationBillingPeriod);
          }
          result->addUtilityBill(calibrationUtilityBill);
        }
      }
      return result;
    }

    boost::optional<ModelObject> Facility_Impl::buildingAsModelObject() const {
      OptionalModelObject result;
      OptionalBuilding intermediate = building();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    std::vector<ModelObject> Facility_Impl::metersAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(meters());
      return result;
    }

    std::vector<ModelObject> Facility_Impl::exteriorLightsAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(exteriorLights());
      return result;
    }

    std::vector<ModelObject> Facility_Impl::exteriorFuelEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(exteriorFuelEquipments());
      return result;
    }

    std::vector<ModelObject> Facility_Impl::exteriorWaterEquipmentAsModelObjects() const {
      ModelObjectVector result = castVector<ModelObject>(exteriorWaterEquipments());
      return result;
    }

  }  // namespace detail

  Facility::Facility(std::shared_ptr<detail::Facility_Impl> impl) : ParentObject(std::move(impl)) {}

  Facility::Facility(Model& model) : ParentObject(Facility::iddObjectType(), model) {}

  OptionalDouble Facility::totalSiteEnergy() const {
    return getImpl<detail::Facility_Impl>()->totalSiteEnergy();
  }
  OptionalDouble Facility::netSiteEnergy() const {
    return getImpl<detail::Facility_Impl>()->netSiteEnergy();
  }
  OptionalDouble Facility::totalSourceEnergy() const {
    return getImpl<detail::Facility_Impl>()->totalSourceEnergy();
  }
  OptionalDouble Facility::netSourceEnergy() const {
    return getImpl<detail::Facility_Impl>()->netSourceEnergy();
  }
  OptionalDouble Facility::annualTotalCost(const FuelType& fuel) const {
    return getImpl<detail::Facility_Impl>()->annualTotalCost(fuel);
  }
  OptionalDouble Facility::annualTotalCostPerBldgArea(const FuelType& fuel) const {
    return getImpl<detail::Facility_Impl>()->annualTotalCostPerBldgArea(fuel);
  }
  OptionalDouble Facility::annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const {
    return getImpl<detail::Facility_Impl>()->annualTotalCostPerNetConditionedBldgArea(fuel);
  }
  OptionalDouble Facility::annualTotalUtilityCost() const {
    return getImpl<detail::Facility_Impl>()->annualTotalUtilityCost();
  }
  OptionalDouble Facility::annualElectricTotalCost() const {
    return getImpl<detail::Facility_Impl>()->annualElectricTotalCost();
  }
  OptionalDouble Facility::annualGasTotalCost() const {
    return getImpl<detail::Facility_Impl>()->annualGasTotalCost();
  }
  OptionalDouble Facility::annualDistrictCoolingTotalCost() const {
    return getImpl<detail::Facility_Impl>()->annualDistrictCoolingTotalCost();
  }
  OptionalDouble Facility::annualDistrictHeatingTotalCost() const {
    return getImpl<detail::Facility_Impl>()->annualDistrictHeatingTotalCost();
  }
  OptionalDouble Facility::annualWaterTotalCost() const {
    return getImpl<detail::Facility_Impl>()->annualWaterTotalCost();
  }

  OptionalDouble Facility::economicsCapitalCost() const {
    return getImpl<detail::Facility_Impl>()->economicsCapitalCost();
  }

  OptionalDouble Facility::economicsEnergyCost() const {
    return getImpl<detail::Facility_Impl>()->economicsEnergyCost();
  }

  OptionalDouble Facility::economicsTLCC() const {
    return getImpl<detail::Facility_Impl>()->economicsTLCC();
  }

  OptionalDouble Facility::economicsVirtualRateGas() const {
    return getImpl<detail::Facility_Impl>()->economicsVirtualRateGas();
  }

  OptionalDouble Facility::economicsVirtualRateElec() const {
    return getImpl<detail::Facility_Impl>()->economicsVirtualRateElec();
  }

  OptionalDouble Facility::economicsVirtualRateCombined() const {
    return getImpl<detail::Facility_Impl>()->economicsVirtualRateCombined();
  }

  OptionalDouble Facility::economicsSPB() const {
    return getImpl<detail::Facility_Impl>()->economicsSPB();
  }

  OptionalDouble Facility::economicsDPB() const {
    return getImpl<detail::Facility_Impl>()->economicsDPB();
  }

  OptionalDouble Facility::economicsNPV() const {
    return getImpl<detail::Facility_Impl>()->economicsNPV();
  }

  OptionalDouble Facility::economicsIRR() const {
    return getImpl<detail::Facility_Impl>()->economicsIRR();
  }

  OptionalDouble Facility::electricityHeating() const {
    return getImpl<detail::Facility_Impl>()->electricityHeating();
  }

  OptionalDouble Facility::electricityCooling() const {
    return getImpl<detail::Facility_Impl>()->electricityCooling();
  }

  OptionalDouble Facility::electricityInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->electricityInteriorLighting();
  }

  OptionalDouble Facility::electricityExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->electricityExteriorLighting();
  }

  OptionalDouble Facility::electricityInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->electricityInteriorEquipment();
  }

  OptionalDouble Facility::electricityExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->electricityExteriorEquipment();
  }

  OptionalDouble Facility::electricityFans() const {
    return getImpl<detail::Facility_Impl>()->electricityFans();
  }

  OptionalDouble Facility::electricityPumps() const {
    return getImpl<detail::Facility_Impl>()->electricityPumps();
  }

  OptionalDouble Facility::electricityHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->electricityHeatRejection();
  }

  OptionalDouble Facility::electricityHumidification() const {
    return getImpl<detail::Facility_Impl>()->electricityHumidification();
  }

  OptionalDouble Facility::electricityHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->electricityHeatRecovery();
  }

  OptionalDouble Facility::electricityWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->electricityWaterSystems();
  }

  OptionalDouble Facility::electricityRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->electricityRefrigeration();
  }

  OptionalDouble Facility::electricityGenerators() const {
    return getImpl<detail::Facility_Impl>()->electricityGenerators();
  }

  OptionalDouble Facility::electricityTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->electricityTotalEndUses();
  }

  OptionalDouble Facility::naturalGasHeating() const {
    return getImpl<detail::Facility_Impl>()->naturalGasHeating();
  }

  OptionalDouble Facility::naturalGasCooling() const {
    return getImpl<detail::Facility_Impl>()->naturalGasCooling();
  }

  OptionalDouble Facility::naturalGasInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->naturalGasInteriorLighting();
  }

  OptionalDouble Facility::naturalGasExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->naturalGasExteriorLighting();
  }

  OptionalDouble Facility::naturalGasInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->naturalGasInteriorEquipment();
  }

  OptionalDouble Facility::naturalGasExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->naturalGasExteriorEquipment();
  }

  OptionalDouble Facility::naturalGasFans() const {
    return getImpl<detail::Facility_Impl>()->naturalGasFans();
  }

  OptionalDouble Facility::naturalGasPumps() const {
    return getImpl<detail::Facility_Impl>()->naturalGasPumps();
  }

  OptionalDouble Facility::naturalGasHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->naturalGasHeatRejection();
  }

  OptionalDouble Facility::naturalGasHumidification() const {
    return getImpl<detail::Facility_Impl>()->naturalGasHumidification();
  }

  OptionalDouble Facility::naturalGasHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->naturalGasHeatRecovery();
  }

  OptionalDouble Facility::naturalGasWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->naturalGasWaterSystems();
  }

  OptionalDouble Facility::naturalGasRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->naturalGasRefrigeration();
  }

  OptionalDouble Facility::naturalGasGenerators() const {
    return getImpl<detail::Facility_Impl>()->naturalGasGenerators();
  }

  OptionalDouble Facility::naturalGasTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->naturalGasTotalEndUses();
  }

  // Gasoline
  OptionalDouble Facility::gasolineHeating() const {
    return getImpl<detail::Facility_Impl>()->gasolineHeating();
  }

  OptionalDouble Facility::gasolineCooling() const {
    return getImpl<detail::Facility_Impl>()->gasolineCooling();
  }

  OptionalDouble Facility::gasolineInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->gasolineInteriorLighting();
  }

  OptionalDouble Facility::gasolineExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->gasolineExteriorLighting();
  }

  OptionalDouble Facility::gasolineInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->gasolineInteriorEquipment();
  }

  OptionalDouble Facility::gasolineExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->gasolineExteriorEquipment();
  }

  OptionalDouble Facility::gasolineFans() const {
    return getImpl<detail::Facility_Impl>()->gasolineFans();
  }

  OptionalDouble Facility::gasolinePumps() const {
    return getImpl<detail::Facility_Impl>()->gasolinePumps();
  }

  OptionalDouble Facility::gasolineHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->gasolineHeatRejection();
  }

  OptionalDouble Facility::gasolineHumidification() const {
    return getImpl<detail::Facility_Impl>()->gasolineHumidification();
  }

  OptionalDouble Facility::gasolineHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->gasolineHeatRecovery();
  }

  OptionalDouble Facility::gasolineWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->gasolineWaterSystems();
  }

  OptionalDouble Facility::gasolineRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->gasolineRefrigeration();
  }

  OptionalDouble Facility::gasolineGenerators() const {
    return getImpl<detail::Facility_Impl>()->gasolineGenerators();
  }

  OptionalDouble Facility::gasolineTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->gasolineTotalEndUses();
  }

  // Diesel
  OptionalDouble Facility::dieselHeating() const {
    return getImpl<detail::Facility_Impl>()->dieselHeating();
  }

  OptionalDouble Facility::dieselCooling() const {
    return getImpl<detail::Facility_Impl>()->dieselCooling();
  }

  OptionalDouble Facility::dieselInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->dieselInteriorLighting();
  }

  OptionalDouble Facility::dieselExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->dieselExteriorLighting();
  }

  OptionalDouble Facility::dieselInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->dieselInteriorEquipment();
  }

  OptionalDouble Facility::dieselExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->dieselExteriorEquipment();
  }

  OptionalDouble Facility::dieselFans() const {
    return getImpl<detail::Facility_Impl>()->dieselFans();
  }

  OptionalDouble Facility::dieselPumps() const {
    return getImpl<detail::Facility_Impl>()->dieselPumps();
  }

  OptionalDouble Facility::dieselHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->dieselHeatRejection();
  }

  OptionalDouble Facility::dieselHumidification() const {
    return getImpl<detail::Facility_Impl>()->dieselHumidification();
  }

  OptionalDouble Facility::dieselHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->dieselHeatRecovery();
  }

  OptionalDouble Facility::dieselWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->dieselWaterSystems();
  }

  OptionalDouble Facility::dieselRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->dieselRefrigeration();
  }

  OptionalDouble Facility::dieselGenerators() const {
    return getImpl<detail::Facility_Impl>()->dieselGenerators();
  }

  OptionalDouble Facility::dieselTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->dieselTotalEndUses();
  }

  // Coal
  OptionalDouble Facility::coalHeating() const {
    return getImpl<detail::Facility_Impl>()->coalHeating();
  }

  OptionalDouble Facility::coalCooling() const {
    return getImpl<detail::Facility_Impl>()->coalCooling();
  }

  OptionalDouble Facility::coalInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->coalInteriorLighting();
  }

  OptionalDouble Facility::coalExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->coalExteriorLighting();
  }

  OptionalDouble Facility::coalInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->coalInteriorEquipment();
  }

  OptionalDouble Facility::coalExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->coalExteriorEquipment();
  }

  OptionalDouble Facility::coalFans() const {
    return getImpl<detail::Facility_Impl>()->coalFans();
  }

  OptionalDouble Facility::coalPumps() const {
    return getImpl<detail::Facility_Impl>()->coalPumps();
  }

  OptionalDouble Facility::coalHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->coalHeatRejection();
  }

  OptionalDouble Facility::coalHumidification() const {
    return getImpl<detail::Facility_Impl>()->coalHumidification();
  }

  OptionalDouble Facility::coalHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->coalHeatRecovery();
  }

  OptionalDouble Facility::coalWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->coalWaterSystems();
  }

  OptionalDouble Facility::coalRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->coalRefrigeration();
  }

  OptionalDouble Facility::coalGenerators() const {
    return getImpl<detail::Facility_Impl>()->coalGenerators();
  }

  OptionalDouble Facility::coalTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->coalTotalEndUses();
  }

  // Fuel Oil No 1
  OptionalDouble Facility::fuelOilNo1Heating() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Heating();
  }

  OptionalDouble Facility::fuelOilNo1Cooling() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Cooling();
  }

  OptionalDouble Facility::fuelOilNo1InteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1InteriorLighting();
  }

  OptionalDouble Facility::fuelOilNo1ExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1ExteriorLighting();
  }

  OptionalDouble Facility::fuelOilNo1InteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1InteriorEquipment();
  }

  OptionalDouble Facility::fuelOilNo1ExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1ExteriorEquipment();
  }

  OptionalDouble Facility::fuelOilNo1Fans() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Fans();
  }

  OptionalDouble Facility::fuelOilNo1Pumps() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Pumps();
  }

  OptionalDouble Facility::fuelOilNo1HeatRejection() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1HeatRejection();
  }

  OptionalDouble Facility::fuelOilNo1Humidification() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Humidification();
  }

  OptionalDouble Facility::fuelOilNo1HeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1HeatRecovery();
  }

  OptionalDouble Facility::fuelOilNo1WaterSystems() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1WaterSystems();
  }

  OptionalDouble Facility::fuelOilNo1Refrigeration() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Refrigeration();
  }

  OptionalDouble Facility::fuelOilNo1Generators() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1Generators();
  }

  OptionalDouble Facility::fuelOilNo1TotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo1TotalEndUses();
  }

  // Fuel Oil No 2
  OptionalDouble Facility::fuelOilNo2Heating() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Heating();
  }

  OptionalDouble Facility::fuelOilNo2Cooling() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Cooling();
  }

  OptionalDouble Facility::fuelOilNo2InteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2InteriorLighting();
  }

  OptionalDouble Facility::fuelOilNo2ExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2ExteriorLighting();
  }

  OptionalDouble Facility::fuelOilNo2InteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2InteriorEquipment();
  }

  OptionalDouble Facility::fuelOilNo2ExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2ExteriorEquipment();
  }

  OptionalDouble Facility::fuelOilNo2Fans() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Fans();
  }

  OptionalDouble Facility::fuelOilNo2Pumps() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Pumps();
  }

  OptionalDouble Facility::fuelOilNo2HeatRejection() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2HeatRejection();
  }

  OptionalDouble Facility::fuelOilNo2Humidification() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Humidification();
  }

  OptionalDouble Facility::fuelOilNo2HeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2HeatRecovery();
  }

  OptionalDouble Facility::fuelOilNo2WaterSystems() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2WaterSystems();
  }

  OptionalDouble Facility::fuelOilNo2Refrigeration() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Refrigeration();
  }

  OptionalDouble Facility::fuelOilNo2Generators() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2Generators();
  }

  OptionalDouble Facility::fuelOilNo2TotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->fuelOilNo2TotalEndUses();
  }

  // Propane
  OptionalDouble Facility::propaneHeating() const {
    return getImpl<detail::Facility_Impl>()->propaneHeating();
  }

  OptionalDouble Facility::propaneCooling() const {
    return getImpl<detail::Facility_Impl>()->propaneCooling();
  }

  OptionalDouble Facility::propaneInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->propaneInteriorLighting();
  }

  OptionalDouble Facility::propaneExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->propaneExteriorLighting();
  }

  OptionalDouble Facility::propaneInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->propaneInteriorEquipment();
  }

  OptionalDouble Facility::propaneExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->propaneExteriorEquipment();
  }

  OptionalDouble Facility::propaneFans() const {
    return getImpl<detail::Facility_Impl>()->propaneFans();
  }

  OptionalDouble Facility::propanePumps() const {
    return getImpl<detail::Facility_Impl>()->propanePumps();
  }

  OptionalDouble Facility::propaneHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->propaneHeatRejection();
  }

  OptionalDouble Facility::propaneHumidification() const {
    return getImpl<detail::Facility_Impl>()->propaneHumidification();
  }

  OptionalDouble Facility::propaneHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->propaneHeatRecovery();
  }

  OptionalDouble Facility::propaneWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->propaneWaterSystems();
  }

  OptionalDouble Facility::propaneRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->propaneRefrigeration();
  }

  OptionalDouble Facility::propaneGenerators() const {
    return getImpl<detail::Facility_Impl>()->propaneGenerators();
  }

  OptionalDouble Facility::propaneTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->propaneTotalEndUses();
  }

  // Other Fuel 1
  OptionalDouble Facility::otherFuel1Heating() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Heating();
  }

  OptionalDouble Facility::otherFuel1Cooling() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Cooling();
  }

  OptionalDouble Facility::otherFuel1InteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1InteriorLighting();
  }

  OptionalDouble Facility::otherFuel1ExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1ExteriorLighting();
  }

  OptionalDouble Facility::otherFuel1InteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1InteriorEquipment();
  }

  OptionalDouble Facility::otherFuel1ExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1ExteriorEquipment();
  }

  OptionalDouble Facility::otherFuel1Fans() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Fans();
  }

  OptionalDouble Facility::otherFuel1Pumps() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Pumps();
  }

  OptionalDouble Facility::otherFuel1HeatRejection() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1HeatRejection();
  }

  OptionalDouble Facility::otherFuel1Humidification() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Humidification();
  }

  OptionalDouble Facility::otherFuel1HeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1HeatRecovery();
  }

  OptionalDouble Facility::otherFuel1WaterSystems() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1WaterSystems();
  }

  OptionalDouble Facility::otherFuel1Refrigeration() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Refrigeration();
  }

  OptionalDouble Facility::otherFuel1Generators() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1Generators();
  }

  OptionalDouble Facility::otherFuel1TotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->otherFuel1TotalEndUses();
  }

  // Other Fuel 2
  OptionalDouble Facility::otherFuel2Heating() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Heating();
  }

  OptionalDouble Facility::otherFuel2Cooling() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Cooling();
  }

  OptionalDouble Facility::otherFuel2InteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2InteriorLighting();
  }

  OptionalDouble Facility::otherFuel2ExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2ExteriorLighting();
  }

  OptionalDouble Facility::otherFuel2InteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2InteriorEquipment();
  }

  OptionalDouble Facility::otherFuel2ExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2ExteriorEquipment();
  }

  OptionalDouble Facility::otherFuel2Fans() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Fans();
  }

  OptionalDouble Facility::otherFuel2Pumps() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Pumps();
  }

  OptionalDouble Facility::otherFuel2HeatRejection() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2HeatRejection();
  }

  OptionalDouble Facility::otherFuel2Humidification() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Humidification();
  }

  OptionalDouble Facility::otherFuel2HeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2HeatRecovery();
  }

  OptionalDouble Facility::otherFuel2WaterSystems() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2WaterSystems();
  }

  OptionalDouble Facility::otherFuel2Refrigeration() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Refrigeration();
  }

  OptionalDouble Facility::otherFuel2Generators() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2Generators();
  }

  OptionalDouble Facility::otherFuel2TotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->otherFuel2TotalEndUses();
  }

  OptionalDouble Facility::districtCoolingHeating() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingHeating();
  }

  OptionalDouble Facility::districtCoolingCooling() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingCooling();
  }

  OptionalDouble Facility::districtCoolingInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingInteriorLighting();
  }

  OptionalDouble Facility::districtCoolingExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingExteriorLighting();
  }

  OptionalDouble Facility::districtCoolingInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingInteriorEquipment();
  }

  OptionalDouble Facility::districtCoolingExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingExteriorEquipment();
  }

  OptionalDouble Facility::districtCoolingFans() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingFans();
  }

  OptionalDouble Facility::districtCoolingPumps() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingPumps();
  }

  OptionalDouble Facility::districtCoolingHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingHeatRejection();
  }

  OptionalDouble Facility::districtCoolingHumidification() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingHumidification();
  }

  OptionalDouble Facility::districtCoolingHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingHeatRecovery();
  }

  OptionalDouble Facility::districtCoolingWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingWaterSystems();
  }

  OptionalDouble Facility::districtCoolingRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingRefrigeration();
  }

  OptionalDouble Facility::districtCoolingGenerators() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingGenerators();
  }

  OptionalDouble Facility::districtCoolingTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->districtCoolingTotalEndUses();
  }

  OptionalDouble Facility::districtHeatingHeating() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingHeating();
  }

  OptionalDouble Facility::districtHeatingCooling() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingCooling();
  }

  OptionalDouble Facility::districtHeatingInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingInteriorLighting();
  }

  OptionalDouble Facility::districtHeatingExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingExteriorLighting();
  }

  OptionalDouble Facility::districtHeatingInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingInteriorEquipment();
  }

  OptionalDouble Facility::districtHeatingExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingExteriorEquipment();
  }

  OptionalDouble Facility::districtHeatingFans() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingFans();
  }

  OptionalDouble Facility::districtHeatingPumps() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingPumps();
  }

  OptionalDouble Facility::districtHeatingHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingHeatRejection();
  }

  OptionalDouble Facility::districtHeatingHumidification() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingHumidification();
  }

  OptionalDouble Facility::districtHeatingHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingHeatRecovery();
  }

  OptionalDouble Facility::districtHeatingWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingWaterSystems();
  }

  OptionalDouble Facility::districtHeatingRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingRefrigeration();
  }

  OptionalDouble Facility::districtHeatingGenerators() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingGenerators();
  }

  OptionalDouble Facility::districtHeatingTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->districtHeatingTotalEndUses();
  }

  OptionalDouble Facility::waterHeating() const {
    return getImpl<detail::Facility_Impl>()->waterHeating();
  }

  OptionalDouble Facility::waterCooling() const {
    return getImpl<detail::Facility_Impl>()->waterCooling();
  }

  OptionalDouble Facility::waterInteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->waterInteriorLighting();
  }

  OptionalDouble Facility::waterExteriorLighting() const {
    return getImpl<detail::Facility_Impl>()->waterExteriorLighting();
  }

  OptionalDouble Facility::waterInteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->waterInteriorEquipment();
  }

  OptionalDouble Facility::waterExteriorEquipment() const {
    return getImpl<detail::Facility_Impl>()->waterExteriorEquipment();
  }

  OptionalDouble Facility::waterFans() const {
    return getImpl<detail::Facility_Impl>()->waterFans();
  }

  OptionalDouble Facility::waterPumps() const {
    return getImpl<detail::Facility_Impl>()->waterPumps();
  }

  OptionalDouble Facility::waterHeatRejection() const {
    return getImpl<detail::Facility_Impl>()->waterHeatRejection();
  }

  OptionalDouble Facility::waterHumidification() const {
    return getImpl<detail::Facility_Impl>()->waterHumidification();
  }

  OptionalDouble Facility::waterHeatRecovery() const {
    return getImpl<detail::Facility_Impl>()->waterHeatRecovery();
  }

  OptionalDouble Facility::waterWaterSystems() const {
    return getImpl<detail::Facility_Impl>()->waterWaterSystems();
  }

  OptionalDouble Facility::waterRefrigeration() const {
    return getImpl<detail::Facility_Impl>()->waterRefrigeration();
  }

  OptionalDouble Facility::waterGenerators() const {
    return getImpl<detail::Facility_Impl>()->waterGenerators();
  }

  OptionalDouble Facility::waterTotalEndUses() const {
    return getImpl<detail::Facility_Impl>()->waterTotalEndUses();
  }

  OptionalDouble Facility::hoursHeatingSetpointNotMet() const {
    return getImpl<detail::Facility_Impl>()->hoursHeatingSetpointNotMet();
  }

  OptionalDouble Facility::hoursCoolingSetpointNotMet() const {
    return getImpl<detail::Facility_Impl>()->hoursCoolingSetpointNotMet();
  }

  boost::optional<EndUses> Facility::endUses() const {
    return getImpl<detail::Facility_Impl>()->endUses();
  }

  boost::optional<CalibrationResult> Facility::calibrationResult() const {
    return getImpl<detail::Facility_Impl>()->calibrationResult();
  }

  // get the building
  boost::optional<Building> Facility::building() const {
    return getImpl<detail::Facility_Impl>()->building();
  }

  /// get meter requests for the facility
  std::vector<OutputMeter> Facility::meters() const {
    return getImpl<detail::Facility_Impl>()->meters();
  }

  boost::optional<OutputMeter> Facility::getMeterByFuelType(const FuelType& fuelType, const std::string& reportingFrequency,
                                                            const boost::optional<EndUseType>& endUseType,
                                                            const boost::optional<std::string>& specificEndUse) const {
    return getImpl<detail::Facility_Impl>()->getMeterByFuelType(fuelType, reportingFrequency, endUseType, specificEndUse);
  }

  std::vector<ExteriorLights> Facility::exteriorLights() const {
    return getImpl<detail::Facility_Impl>()->exteriorLights();
  }

  std::vector<ExteriorFuelEquipment> Facility::exteriorFuelEquipments() const {
    return getImpl<detail::Facility_Impl>()->exteriorFuelEquipments();
  }

  std::vector<ExteriorWaterEquipment> Facility::exteriorWaterEquipments() const {
    return getImpl<detail::Facility_Impl>()->exteriorWaterEquipments();
  }

  std::vector<FuelType> Facility::fossilFuels() {
    FuelTypeVector result;
    result.push_back(FuelType(FuelType::Gas));
    result.push_back(FuelType(FuelType::Gasoline));
    result.push_back(FuelType(FuelType::Diesel));
    result.push_back(FuelType(FuelType::FuelOil_1));
    result.push_back(FuelType(FuelType::FuelOil_2));
    result.push_back(FuelType(FuelType::Propane));
    return result;
  }

  IddObjectType Facility::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Facility);
    return result;
  }

}  // namespace model
}  // namespace openstudio
