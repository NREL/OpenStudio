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

#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/LifeCycleCost.hpp>
#include <model/LifeCycleCost_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/ExteriorLights.hpp>
#include <model/ExteriorLights_Impl.hpp>
#include <model/LifeCycleCostParameters.hpp>
#include <model/LifeCycleCostParameters_Impl.hpp>
#include <model/Meter.hpp>
#include <model/Meter_Impl.hpp>
#include <model/PlanarSurface.hpp>
#include <model/PlanarSurface_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/TimeDependentValuation.hpp>
#include <model/TimeDependentValuation_Impl.hpp>
#include <model/UtilityBill.hpp>
#include <model/UtilityBill_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/economics/Economics.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <boost/foreach.hpp>

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

  Facility_Impl::Facility_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Facility::iddObjectType());
  }

  Facility_Impl::Facility_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Facility::iddObjectType());
  }

  Facility_Impl::Facility_Impl(const Facility_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> Facility_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  std::vector<ModelObject> Facility_Impl::children() const
  {
    std::vector<ModelObject> result;

    MeterVector meters = this->meters();
    result.insert(result.end(),meters.begin(),meters.end());

    // building
    OptionalBuilding building = this->building();
    if (building){
      result.push_back(*building);
    }

    ExteriorLightsVector exteriorLights = this->exteriorLights();
    result.insert(result.end(),exteriorLights.begin(),exteriorLights.end());

    return result;
  }

  std::vector<IddObjectType> Facility_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Building);
    return result;
  }

  const std::vector<std::string>& Facility_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Facility_Impl::iddObjectType() const {
    return Facility::iddObjectType();
  }

  boost::optional<Building> Facility_Impl::building() const
  {
    return this->model().building();
  }

  /// get meter requests for the facility
  std::vector<Meter> Facility_Impl::meters() const
  {
    MeterVector result;
    MeterVector meters = this->model().getModelObjects<Meter>();
    BOOST_FOREACH(const Meter& meter, meters){
      if (meter.installLocationType() && (InstallLocationType::Facility == meter.installLocationType().get().value())){
        result.push_back(meter);
      }
    }
    return result;
  }
  
  boost::optional<Meter> Facility_Impl::getMeterByFuelType(
    const FuelType& fuelType,
    const std::string& reportingFrequency,
    const boost::optional<EndUseType>& endUseType,
    const boost::optional<std::string>& specificEndUse) const
  {
    OptionalMeter result;
    BOOST_FOREACH(const Meter& meter,this->meters()) {
      if (meter.fuelType() && (meter.fuelType() == fuelType)) {
        if (istringEqual(meter.reportingFrequency(),reportingFrequency)) {
          OptionalEndUseType meterEndUseType = meter.endUseType();
          if ((!endUseType && !meterEndUseType) || 
              (endUseType && meterEndUseType && (*meterEndUseType == *endUseType))) 
          {
            OptionalString meterSpecificEndUse = meter.specificEndUse();
            if ((!specificEndUse && !meterSpecificEndUse) ||
                (specificEndUse && meterSpecificEndUse && istringEqual(*meterSpecificEndUse,*specificEndUse)))
            {
              if (result) {
                LOG(Warn,"Found duplicate meters: " << result->name() << " and " << meter.name() << ".");
              }
              else {
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
    return model().getModelObjects<ExteriorLights>();
  }

  OptionalDouble Facility_Impl::totalSiteEnergy() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->totalSiteEnergy();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::netSiteEnergy() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->netSiteEnergy();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::totalSourceEnergy() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->totalSourceEnergy();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::netSourceEnergy() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->netSourceEnergy();
    }
    return OptionalDouble();
  }

   // pass in "Electric", "Gas", or "Other"
  OptionalDouble Facility_Impl::annualTotalCost(const FuelType& fuel) const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->annualTotalCost(fuel);
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::annualTotalCostPerBldgArea(const FuelType& fuel) const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->annualTotalCostPerBldgArea(fuel);
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->annualTotalCostPerNetConditionedBldgArea(fuel);
    }

    return OptionalDouble();
  }

  boost::optional<double> Facility_Impl::annualTotalUtilityCost() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->annualTotalUtilityCost();
    }
    return OptionalDouble();
  }

  boost::optional<double> Facility_Impl::annualElectricTotalCost() const
  {
    return annualTotalCost(FuelType::Electricity);
  }

  boost::optional<double> Facility_Impl::annualGasTotalCost() const
  {
    return annualTotalCost(FuelType::Gas);
  }

  boost::optional<double> Facility_Impl::annualDistrictCoolingTotalCost() const
  {
    return annualTotalCost(FuelType::DistrictCooling);
  }

  boost::optional<double> Facility_Impl::annualDistrictHeatingTotalCost() const
  {
    return annualTotalCost(FuelType::DistrictHeating);
  }

  boost::optional<double> Facility_Impl::annualWaterTotalCost() const
  {
    return annualTotalCost(FuelType::Water);
  }

  boost::optional<double> Facility_Impl::totalEnergyTimeDependentValuation() const {
    OptionalDouble result;
    OptionalTimeDependentValuation oTDV = model().getOptionalUniqueModelObject<TimeDependentValuation>();
    if (oTDV) {
      TimeDependentValuation tdv = *oTDV;
      result = tdv.energyTimeDependentValuation();
    }
    else {
      LOG(Debug,"Cannot return totalEnergyTimeDependentValuation because there is no TimeDependentValuation object in model.");
    }
    return result;
  }

  boost::optional<double> Facility_Impl::totalCostTimeDependentValuation() const {
    OptionalDouble result;
    OptionalTimeDependentValuation oTDV = model().getOptionalUniqueModelObject<TimeDependentValuation>();
    if (oTDV) {
      TimeDependentValuation tdv = *oTDV;
      result = tdv.costTimeDependentValuation();
    }
    return result;
  }

  boost::optional<double> Facility_Impl::electricityEnergyTimeDependentValuation() const {
    OptionalDouble result;
    OptionalTimeDependentValuation oTDV = model().getOptionalUniqueModelObject<TimeDependentValuation>();
    if (oTDV) {
      TimeDependentValuation tdv = *oTDV;
      result = tdv.getEnergyTimeDependentValuation(FuelType(FuelType::Electricity));
    }
    return result;
  }

  boost::optional<double> Facility_Impl::electricityCostTimeDependentValuation() const {
    OptionalDouble result;
    OptionalTimeDependentValuation oTDV = model().getOptionalUniqueModelObject<TimeDependentValuation>();
    if (oTDV) {
      TimeDependentValuation tdv = *oTDV;
      result = tdv.getCostTimeDependentValuation(FuelType(FuelType::Electricity));
    }
    return result;
  }

  boost::optional<double> Facility_Impl::fossilFuelEnergyTimeDependentValuation() const {
    OptionalDouble result;
    OptionalTimeDependentValuation oTDV = model().getOptionalUniqueModelObject<TimeDependentValuation>();
    if (oTDV) {
      TimeDependentValuation tdv = *oTDV;
      FuelTypeVector fts = Facility::fossilFuels();
      BOOST_FOREACH(const FuelType& ft,fts) {
        OptionalDouble candidate = tdv.getEnergyTimeDependentValuation(ft);
        if (candidate) {
          if (result) { result = (*result + *candidate); }
          else { result = candidate; }
        }
      }
    }
    return result;
  }

  boost::optional<double> Facility_Impl::fossilFuelCostTimeDependentValuation() const {
    OptionalDouble result;
    OptionalTimeDependentValuation oTDV = model().getOptionalUniqueModelObject<TimeDependentValuation>();
    if (oTDV) {
      TimeDependentValuation tdv = *oTDV;
      FuelTypeVector fts = Facility::fossilFuels();
      BOOST_FOREACH(const FuelType& ft,fts) {
        OptionalDouble candidate = tdv.getCostTimeDependentValuation(ft);
        if (candidate) {
          if (result) { result = (*result + *candidate); }
          else { result = candidate; }
        }
      }
    }
    return result;
  }

  OptionalDouble Facility_Impl::economicsCapitalCost() const
  {
    OptionalDouble result;
    Economics::BuildingType referenceBldg = Economics::btReference;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.firstCosts(referenceBldg);
    return result;
  }

  OptionalDouble Facility_Impl::economicsEnergyCost() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->economicsEnergyCost();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::economicsVirtualRateGas() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.getVirtualRate(Economics::btReference,Economics::vrGas);
    return result;
  }

  OptionalDouble Facility_Impl::economicsVirtualRateElec() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.getVirtualRate(Economics::btReference,Economics::vrElec);
    return result;
  }

  OptionalDouble Facility_Impl::economicsVirtualRateCombined() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.getVirtualRate(Economics::btReference,Economics::vrCombined);
    return result;
  }

  OptionalDouble Facility_Impl::getElecOrGasUse(bool bGetGas) const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->getElecOrGasUse(bGetGas);
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::getElecOrGasCost(bool bGetGas) const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->getElecOrGasCost(bGetGas);
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::gasUse() const
  {
    return getElecOrGasUse();
  }

  OptionalDouble Facility_Impl::gasCost() const
  {
    return getElecOrGasCost();
  }

  OptionalDouble Facility_Impl::elecUse() const
  {
    return getElecOrGasUse(false);
  }

  OptionalDouble Facility_Impl::elecCost() const
  {
    return getElecOrGasCost(false);
  }

  bool Facility_Impl::initEconomics(Economics& economics) const
  {
    bool success = false;

/*
    // DLM: we are redoing this function

    Economics::BuildingType referenceBldg = Economics::btReference;
    OptionalDouble matCost;
    OptionalDouble installCost;
    OptionalDouble fixedOM;
    OptionalDouble variableOM;
    OptionalInt expectedLife;
    OptionalDouble salvageCost;
    double netArea;
    std::string constructionName;
    ComponentCostLineItemVector componentCostLineItems = this->model().getModelObjects<ComponentCostLineItem>();
    BOOST_FOREACH(ComponentCostLineItem componentCostLineItem, componentCostLineItems){

      ModelObject item = componentCostLineItem.item();

      if(item.optionalCast<ConstructionBase>()){

        ConstructionBase construction = item.cast<ConstructionBase>();

        // query area of construction from model
        std::vector<PlanarSurface> surfaces = construction.getModelObjectSources<PlanarSurface>();

        netArea = 0;
        BOOST_FOREACH(PlanarSurface surface, surfaces){
          netArea += surface.netArea();
        }

        matCost = componentCostLineItem.materialCost();
        installCost = componentCostLineItem.installationCost();
        //fixedOM = componentCostLineItem.fixedOM();
        //variableOM = componentCostLineItem.variableOM();
        //expectedLife = componentCostLineItem.expectedLife();
        //salvageCost = componentCostLineItem.salvageValue();

        economics.addCostInstance(referenceBldg,"Obj",matCost,installCost,fixedOM,variableOM,expectedLife,salvageCost,netArea);

        if (!matCost) matCost = 0;
        if (!installCost) installCost = 0;
        if (!fixedOM) fixedOM = 0;
        if (!variableOM) variableOM = 0;
        if (!expectedLife) expectedLife = 0;
        if (!salvageCost) salvageCost = 0;

        LOG(Debug, "Added cost object " << componentCostLineItem << " for construction of name '" << constructionName << "'." << std::endl
          << "matCost = " << *matCost << ", installCost = " << *installCost << ", fixedOM = " << *fixedOM << ", variableOM = " << *variableOM
          << ", expectedLife = " << *expectedLife << ", salvageCost = " << *salvageCost << ", netArea = " << netArea );


        // set at least 1 cost instance
        success = true;
      }else{
        LOG(Warn, "Cost object " << componentCostLineItem << " could not be added to the total cost because its type is not currently handled");
      }
    }

    if(!success){
      LOG(Error, "No cost objects processed in initEconomics.");
      return success;
    }
    else{
      // reset to false and test more conditions
      success = false;
    }

    OptionalLifeCycleCost_Parameters lifeCycleCostParameters = this->model().getUniqueModelObject<LifeCycleCost_Parameters>();
    if(!lifeCycleCostParameters) {
      LOG(Error, "LifeCycleCost_Parameters object not found.");
      return success;
    }

    OptionalDouble inflation = lifeCycleCostParameters->inflation();
    OptionalDouble omInflation = lifeCycleCostParameters->omInflation();
    OptionalDouble discountRate = lifeCycleCostParameters->nominalDiscountRate();
    OptionalInt analysisPeriod = lifeCycleCostParameters->lengthOfStudyPeriodInYears();

    // economics.addDistrictCool(referenceBldg,use,cost,inflation);  TODO Nick: district cooling not available in tariff, see Dan
    // economics.addDistrictHeat(referenceBldg,use,cost,inflation);  TODO Nick: distruct heating not available in tariff, see Dan

    OptionalDouble elecUse = this->elecUse();
    OptionalDouble elecCost = this->elecCost();
    OptionalDouble gasUse = this->gasUse();
    OptionalDouble gasCost = this->gasCost();

    economics.addElectricity(referenceBldg,elecUse,elecCost,inflation);
    economics.addGas(referenceBldg,gasUse,gasCost,inflation);
    economics.setInflation(referenceBldg,inflation);
    economics.setOMInflation(referenceBldg,omInflation);
    economics.setDiscountRate(discountRate);
    economics.setAnalysisPeriod(analysisPeriod);

    economics.calculateSingleEconomics(referenceBldg);

    success = true;

    LOG(Debug, "initEconomics completed successfully");
*/
    return success;
  }

  OptionalDouble Facility_Impl::economicsTLCC() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.getTLCC(Economics::btReference);
    return result;
  }

  OptionalDouble Facility_Impl::economicsSPB() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.simplePayback();
    return result;
  }

  OptionalDouble Facility_Impl::economicsDPB() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.discountedPayback();
    return result;
  }

  OptionalDouble Facility_Impl::economicsNPV() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.netPresentValue();
    return result;
  }

  OptionalDouble Facility_Impl::economicsIRR() const
  {
    OptionalDouble result;
    Economics economics;
    if(!initEconomics(economics)) return result;
    result = economics.internalRateOfReturn();
    return result;
  }

  OptionalDouble Facility_Impl::electricityHeating() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityHeating();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityCooling() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityCooling();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityInteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityInteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityExteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityExteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityInteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityInteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityExteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityExteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityFans() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityFans();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityPumps() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityPumps();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityHeatRejection() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityHeatRejection();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityHumidification() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityHumidification();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityHeatRecovery() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityHeatRecovery();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityWaterSystems() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityWaterSystems();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityRefrigeration() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityRefrigeration();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityGenerators() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityGenerators();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::electricityTotalEndUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->electricityTotalEndUses();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasHeating() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasHeating();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasCooling() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasCooling();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasInteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasInteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasExteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasExteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasInteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasInteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasExteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasExteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasFans() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasFans();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasPumps() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasPumps();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasHeatRejection() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasHeatRejection();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasHumidification() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasHumidification();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasHeatRecovery() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasHeatRecovery();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasWaterSystems() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasWaterSystems();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasRefrigeration() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasRefrigeration();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasGenerators() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasGenerators();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::naturalGasTotalEndUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->naturalGasTotalEndUses();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelHeating() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelHeating();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelCooling() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelCooling();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelInteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelInteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelExteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelExteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelInteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelInteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelExteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelExteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelFans() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelFans();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelPumps() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelPumps();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelHeatRejection() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelHeatRejection();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelHumidification() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelHumidification();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelHeatRecovery() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelHeatRecovery();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelWaterSystems() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelWaterSystems();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelRefrigeration() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelRefrigeration();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelGenerators() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelGenerators();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::otherFuelTotalEndUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->otherFuelTotalEndUses();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingHeating() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingHeating();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingCooling() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingCooling();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingInteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingInteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingExteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingExteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingInteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingInteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingExteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingExteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingFans() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingFans();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingPumps() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingPumps();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingHeatRejection() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingHeatRejection();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingHumidification() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingHumidification();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingHeatRecovery() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingHeatRecovery();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingWaterSystems() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingWaterSystems();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingRefrigeration() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingRefrigeration();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingGenerators() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingGenerators();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtCoolingTotalEndUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtCoolingTotalEndUses();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingHeating() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingHeating();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingCooling() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingCooling();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingInteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingInteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingExteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingExteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingInteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingInteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingExteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingExteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingFans() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingFans();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingPumps() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingPumps();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingHeatRejection() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingHeatRejection();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingHumidification() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingHumidification();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingHeatRecovery() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingHeatRecovery();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingWaterSystems() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingWaterSystems();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingRefrigeration() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingRefrigeration();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingGenerators() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingGenerators();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::districtHeatingTotalEndUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->districtHeatingTotalEndUses();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterHeating() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterHeating();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterCooling() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterCooling();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterInteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterInteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterExteriorLighting() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterExteriorLighting();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterInteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterInteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterExteriorEquipment() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterExteriorEquipment();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterFans() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterFans();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterPumps() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterPumps();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterHeatRejection() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterHeatRejection();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterHumidification() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterHumidification();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterHeatRecovery() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterHeatRecovery();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterWaterSystems() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterWaterSystems();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterRefrigeration() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterRefrigeration();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterGenerators() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterGenerators();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::waterTotalEndUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->waterTotalEndUses();
    }
    return OptionalDouble();
  }

  OptionalDouble Facility_Impl::hoursHeatingSetpointNotMet() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->hoursHeatingSetpointNotMet();
    }
    return OptionalDouble();
  }
  
  OptionalDouble Facility_Impl::hoursCoolingSetpointNotMet() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->hoursCoolingSetpointNotMet();
    }
    return OptionalDouble();
  }

  boost::optional<EndUses> Facility_Impl::endUses() const
  {
    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      return mySqlFile->endUses();
    }
    return boost::optional<EndUses>();
  }

  boost::optional<Attribute> Facility_Impl::endUsesAttribute() const
  {
    boost::optional<Attribute> result;
    boost::optional<EndUses> endUses = this->endUses();
    if (endUses){
      result = endUses->attribute();
    }
    return result;
  }

  boost::optional<CalibrationResult> Facility_Impl::calibrationResult() const
  {
    boost::optional<CalibrationResult> result; 

    OptionalSqlFile mySqlFile = model().sqlFile();
    if (mySqlFile && mySqlFile->connectionOpen())
    {
      result = CalibrationResult();
      BOOST_FOREACH(const model::UtilityBill& utilityBill, this->model().getModelObjects<model::UtilityBill>()){
        CalibrationUtilityBill calibrationUtilityBill(utilityBill.name().get(), utilityBill.fuelType(),
          utilityBill.meterInstallLocation(), utilityBill.meterSpecificInstallLocation(), 
          utilityBill.meterEndUseCategory(), utilityBill.meterSpecificEndUse(), utilityBill.consumptionUnit(),
          utilityBill.consumptionUnitConversionFactor(), utilityBill.peakDemandUnit(), utilityBill.peakDemandUnitConversionFactor(),
          utilityBill.timestepsInPeakDemandWindow(), utilityBill.minutesInPeakDemandWindow(), utilityBill.numberBillingPeriodsInCalculations(),
          utilityBill.CVRMSE(), utilityBill.NMBE());

        BOOST_FOREACH(const model::BillingPeriod& billingPeriod, utilityBill.billingPeriods()){
          CalibrationBillingPeriod calibrationBillingPeriod(billingPeriod.startDate(), billingPeriod.numberOfDays(),
             utilityBill.consumptionUnit(), utilityBill.peakDemandUnit(),
             billingPeriod.consumption(), billingPeriod.peakDemand(), billingPeriod.totalCost(),
             billingPeriod.modelConsumption(), billingPeriod.modelPeakDemand(), billingPeriod.modelTotalCost());

          calibrationUtilityBill.addBillingPeriod(calibrationBillingPeriod);
        }
        result->addUtilityBill(calibrationUtilityBill);
      }
    }
    return result;
  }

  boost::optional<Attribute> Facility_Impl::calibrationResultAttribute() const
  {
    boost::optional<Attribute> result;
    boost::optional<CalibrationResult> calibrationResult = this->calibrationResult();
    if (calibrationResult){
      result = calibrationResult->attribute();
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

}// detail

Facility::Facility(boost::shared_ptr<detail::Facility_Impl> impl)
  : ParentObject(impl)
{}

Facility::Facility(Model& model)
  : ParentObject(Facility::iddObjectType(),model)
{}

OptionalDouble Facility::totalSiteEnergy() const
{
  return getImpl<detail::Facility_Impl>()->totalSiteEnergy();
}
OptionalDouble Facility::netSiteEnergy() const
{
  return getImpl<detail::Facility_Impl>()->netSiteEnergy();
}
OptionalDouble Facility::totalSourceEnergy() const
{
  return getImpl<detail::Facility_Impl>()->totalSourceEnergy();
}
OptionalDouble Facility::netSourceEnergy() const
{
  return getImpl<detail::Facility_Impl>()->netSourceEnergy();
}
OptionalDouble Facility::annualTotalCost(const FuelType& fuel) const
{
  return getImpl<detail::Facility_Impl>()->annualTotalCost(fuel);
}
OptionalDouble Facility::annualTotalCostPerBldgArea(const FuelType& fuel) const
{
  return getImpl<detail::Facility_Impl>()->annualTotalCostPerBldgArea(fuel);
}
OptionalDouble Facility::annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const
{
  return getImpl<detail::Facility_Impl>()->annualTotalCostPerNetConditionedBldgArea(fuel);
}
OptionalDouble Facility::annualTotalUtilityCost() const
{
  return getImpl<detail::Facility_Impl>()->annualTotalUtilityCost();
}
OptionalDouble Facility::annualElectricTotalCost() const
{
  return getImpl<detail::Facility_Impl>()->annualElectricTotalCost();
}
OptionalDouble Facility::annualGasTotalCost() const
{
  return getImpl<detail::Facility_Impl>()->annualGasTotalCost();
}
OptionalDouble Facility::annualDistrictCoolingTotalCost() const
{
  return getImpl<detail::Facility_Impl>()->annualDistrictCoolingTotalCost();
}
OptionalDouble Facility::annualDistrictHeatingTotalCost() const
{
  return getImpl<detail::Facility_Impl>()->annualDistrictHeatingTotalCost();
}
OptionalDouble Facility::annualWaterTotalCost() const
{
  return getImpl<detail::Facility_Impl>()->annualWaterTotalCost();
}

boost::optional<double> Facility::totalEnergyTimeDependentValuation() const {
  return getImpl<detail::Facility_Impl>()->totalEnergyTimeDependentValuation();
}
boost::optional<double> Facility::totalCostTimeDependentValuation() const {
  return getImpl<detail::Facility_Impl>()->totalCostTimeDependentValuation();
}
boost::optional<double> Facility::electricityEnergyTimeDependentValuation() const {
  return getImpl<detail::Facility_Impl>()->electricityEnergyTimeDependentValuation();
}
boost::optional<double> Facility::electricityCostTimeDependentValuation() const {
  return getImpl<detail::Facility_Impl>()->electricityCostTimeDependentValuation();
}
boost::optional<double> Facility::fossilFuelEnergyTimeDependentValuation() const {
  return getImpl<detail::Facility_Impl>()->fossilFuelEnergyTimeDependentValuation();
}
boost::optional<double> Facility::fossilFuelCostTimeDependentValuation() const {
  return getImpl<detail::Facility_Impl>()->fossilFuelCostTimeDependentValuation();
}

OptionalDouble Facility::economicsCapitalCost() const
{
  return getImpl<detail::Facility_Impl>()->economicsCapitalCost();
}

OptionalDouble Facility::economicsEnergyCost() const
{
  return getImpl<detail::Facility_Impl>()->economicsEnergyCost();
}

OptionalDouble Facility::economicsTLCC() const
{
  return getImpl<detail::Facility_Impl>()->economicsTLCC();
}

OptionalDouble Facility::economicsVirtualRateGas() const
{
  return getImpl<detail::Facility_Impl>()->economicsVirtualRateGas();
}

OptionalDouble Facility::economicsVirtualRateElec() const
{
  return getImpl<detail::Facility_Impl>()->economicsVirtualRateElec();
}

OptionalDouble Facility::economicsVirtualRateCombined() const
{
  return getImpl<detail::Facility_Impl>()->economicsVirtualRateCombined();
}

OptionalDouble Facility::economicsSPB() const
{
  return getImpl<detail::Facility_Impl>()->economicsSPB();
}

OptionalDouble Facility::economicsDPB() const
{
  return getImpl<detail::Facility_Impl>()->economicsDPB();
}

OptionalDouble Facility::economicsNPV() const
{
  return getImpl<detail::Facility_Impl>()->economicsNPV();
}

OptionalDouble Facility::economicsIRR() const
{
  return getImpl<detail::Facility_Impl>()->economicsIRR();
}

OptionalDouble Facility::electricityHeating() const
{
  return getImpl<detail::Facility_Impl>()->electricityHeating();
}

OptionalDouble Facility::electricityCooling() const
{
  return getImpl<detail::Facility_Impl>()->electricityCooling();
}

OptionalDouble Facility::electricityInteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->electricityInteriorLighting();
}

OptionalDouble Facility::electricityExteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->electricityExteriorLighting();
}

OptionalDouble Facility::electricityInteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->electricityInteriorEquipment();
}

OptionalDouble Facility::electricityExteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->electricityExteriorEquipment();
}

OptionalDouble Facility::electricityFans() const
{
  return getImpl<detail::Facility_Impl>()->electricityFans();
}

OptionalDouble Facility::electricityPumps() const
{
  return getImpl<detail::Facility_Impl>()->electricityPumps();
}

OptionalDouble Facility::electricityHeatRejection() const
{
  return getImpl<detail::Facility_Impl>()->electricityHeatRejection();
}

OptionalDouble Facility::electricityHumidification() const
{
  return getImpl<detail::Facility_Impl>()->electricityHumidification();
}

OptionalDouble Facility::electricityHeatRecovery() const
{
  return getImpl<detail::Facility_Impl>()->electricityHeatRecovery();
}

OptionalDouble Facility::electricityWaterSystems() const
{
  return getImpl<detail::Facility_Impl>()->electricityWaterSystems();
}

OptionalDouble Facility::electricityRefrigeration() const
{
  return getImpl<detail::Facility_Impl>()->electricityRefrigeration();
}

OptionalDouble Facility::electricityGenerators() const
{
  return getImpl<detail::Facility_Impl>()->electricityGenerators();
}

OptionalDouble Facility::electricityTotalEndUses() const
{
  return getImpl<detail::Facility_Impl>()->electricityTotalEndUses();
}

OptionalDouble Facility::naturalGasHeating() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasHeating();
}

OptionalDouble Facility::naturalGasCooling() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasCooling();
}

OptionalDouble Facility::naturalGasInteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasInteriorLighting();
}

OptionalDouble Facility::naturalGasExteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasExteriorLighting();
}

OptionalDouble Facility::naturalGasInteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasInteriorEquipment();
}

OptionalDouble Facility::naturalGasExteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasExteriorEquipment();
}

OptionalDouble Facility::naturalGasFans() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasFans();
}

OptionalDouble Facility::naturalGasPumps() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasPumps();
}

OptionalDouble Facility::naturalGasHeatRejection() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasHeatRejection();
}

OptionalDouble Facility::naturalGasHumidification() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasHumidification();
}

OptionalDouble Facility::naturalGasHeatRecovery() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasHeatRecovery();
}

OptionalDouble Facility::naturalGasWaterSystems() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasWaterSystems();
}

OptionalDouble Facility::naturalGasRefrigeration() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasRefrigeration();
}

OptionalDouble Facility::naturalGasGenerators() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasGenerators();
}

OptionalDouble Facility::naturalGasTotalEndUses() const
{
  return getImpl<detail::Facility_Impl>()->naturalGasTotalEndUses();
}

OptionalDouble Facility::otherFuelHeating() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelHeating();
}

OptionalDouble Facility::otherFuelCooling() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelCooling();
}

OptionalDouble Facility::otherFuelInteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelInteriorLighting();
}

OptionalDouble Facility::otherFuelExteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelExteriorLighting();
}

OptionalDouble Facility::otherFuelInteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelInteriorEquipment();
}

OptionalDouble Facility::otherFuelExteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelExteriorEquipment();
}

OptionalDouble Facility::otherFuelFans() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelFans();
}

OptionalDouble Facility::otherFuelPumps() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelPumps();
}

OptionalDouble Facility::otherFuelHeatRejection() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelHeatRejection();
}

OptionalDouble Facility::otherFuelHumidification() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelHumidification();
}

OptionalDouble Facility::otherFuelHeatRecovery() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelHeatRecovery();
}

OptionalDouble Facility::otherFuelWaterSystems() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelWaterSystems();
}

OptionalDouble Facility::otherFuelRefrigeration() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelRefrigeration();
}

OptionalDouble Facility::otherFuelGenerators() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelGenerators();
}

OptionalDouble Facility::otherFuelTotalEndUses() const
{
  return getImpl<detail::Facility_Impl>()->otherFuelTotalEndUses();
}

OptionalDouble Facility::districtCoolingHeating() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingHeating();
}

OptionalDouble Facility::districtCoolingCooling() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingCooling();
}

OptionalDouble Facility::districtCoolingInteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingInteriorLighting();
}

OptionalDouble Facility::districtCoolingExteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingExteriorLighting();
}

OptionalDouble Facility::districtCoolingInteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingInteriorEquipment();
}

OptionalDouble Facility::districtCoolingExteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingExteriorEquipment();
}

OptionalDouble Facility::districtCoolingFans() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingFans();
}

OptionalDouble Facility::districtCoolingPumps() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingPumps();
}

OptionalDouble Facility::districtCoolingHeatRejection() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingHeatRejection();
}

OptionalDouble Facility::districtCoolingHumidification() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingHumidification();
}

OptionalDouble Facility::districtCoolingHeatRecovery() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingHeatRecovery();
}

OptionalDouble Facility::districtCoolingWaterSystems() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingWaterSystems();
}

OptionalDouble Facility::districtCoolingRefrigeration() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingRefrigeration();
}

OptionalDouble Facility::districtCoolingGenerators() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingGenerators();
}

OptionalDouble Facility::districtCoolingTotalEndUses() const
{
  return getImpl<detail::Facility_Impl>()->districtCoolingTotalEndUses();
}

OptionalDouble Facility::districtHeatingHeating() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingHeating();
}

OptionalDouble Facility::districtHeatingCooling() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingCooling();
}

OptionalDouble Facility::districtHeatingInteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingInteriorLighting();
}

OptionalDouble Facility::districtHeatingExteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingExteriorLighting();
}

OptionalDouble Facility::districtHeatingInteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingInteriorEquipment();
}

OptionalDouble Facility::districtHeatingExteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingExteriorEquipment();
}

OptionalDouble Facility::districtHeatingFans() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingFans();
}

OptionalDouble Facility::districtHeatingPumps() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingPumps();
}

OptionalDouble Facility::districtHeatingHeatRejection() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingHeatRejection();
}

OptionalDouble Facility::districtHeatingHumidification() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingHumidification();
}

OptionalDouble Facility::districtHeatingHeatRecovery() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingHeatRecovery();
}

OptionalDouble Facility::districtHeatingWaterSystems() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingWaterSystems();
}

OptionalDouble Facility::districtHeatingRefrigeration() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingRefrigeration();
}

OptionalDouble Facility::districtHeatingGenerators() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingGenerators();
}

OptionalDouble Facility::districtHeatingTotalEndUses() const
{
  return getImpl<detail::Facility_Impl>()->districtHeatingTotalEndUses();
}

OptionalDouble Facility::waterHeating() const
{
  return getImpl<detail::Facility_Impl>()->waterHeating();
}

OptionalDouble Facility::waterCooling() const
{
  return getImpl<detail::Facility_Impl>()->waterCooling();
}

OptionalDouble Facility::waterInteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->waterInteriorLighting();
}

OptionalDouble Facility::waterExteriorLighting() const
{
  return getImpl<detail::Facility_Impl>()->waterExteriorLighting();
}

OptionalDouble Facility::waterInteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->waterInteriorEquipment();
}

OptionalDouble Facility::waterExteriorEquipment() const
{
  return getImpl<detail::Facility_Impl>()->waterExteriorEquipment();
}

OptionalDouble Facility::waterFans() const
{
  return getImpl<detail::Facility_Impl>()->waterFans();
}

OptionalDouble Facility::waterPumps() const
{
  return getImpl<detail::Facility_Impl>()->waterPumps();
}

OptionalDouble Facility::waterHeatRejection() const
{
  return getImpl<detail::Facility_Impl>()->waterHeatRejection();
}

OptionalDouble Facility::waterHumidification() const
{
  return getImpl<detail::Facility_Impl>()->waterHumidification();
}

OptionalDouble Facility::waterHeatRecovery() const
{
  return getImpl<detail::Facility_Impl>()->waterHeatRecovery();
}

OptionalDouble Facility::waterWaterSystems() const
{
  return getImpl<detail::Facility_Impl>()->waterWaterSystems();
}

OptionalDouble Facility::waterRefrigeration() const
{
  return getImpl<detail::Facility_Impl>()->waterRefrigeration();
}

OptionalDouble Facility::waterGenerators() const
{
  return getImpl<detail::Facility_Impl>()->waterGenerators();
}

OptionalDouble Facility::waterTotalEndUses() const
{
  return getImpl<detail::Facility_Impl>()->waterTotalEndUses();
}

OptionalDouble Facility::hoursHeatingSetpointNotMet() const
{
  return getImpl<detail::Facility_Impl>()->hoursHeatingSetpointNotMet();
}

OptionalDouble Facility::hoursCoolingSetpointNotMet() const
{
  return getImpl<detail::Facility_Impl>()->hoursCoolingSetpointNotMet();
}

boost::optional<EndUses> Facility::endUses() const
{
  return getImpl<detail::Facility_Impl>()->endUses();
}

boost::optional<Attribute> Facility::endUsesAttribute() const
{
  return getImpl<detail::Facility_Impl>()->endUsesAttribute();
}

boost::optional<CalibrationResult> Facility::calibrationResult() const
{
  return getImpl<detail::Facility_Impl>()->calibrationResult();
}

boost::optional<Attribute> Facility::calibrationResultAttribute() const
{
  return getImpl<detail::Facility_Impl>()->calibrationResultAttribute();
}


// get the building
boost::optional<Building> Facility::building() const
{
  return getImpl<detail::Facility_Impl>()->building();
}

/// get meter requests for the facility
std::vector<Meter> Facility::meters() const
{
  return getImpl<detail::Facility_Impl>()->meters();
}

boost::optional<Meter> Facility::getMeterByFuelType(
    const FuelType& fuelType,
    const std::string& reportingFrequency,
    const boost::optional<EndUseType>& endUseType,
    const boost::optional<std::string>& specificEndUse) const
{
  return getImpl<detail::Facility_Impl>()->getMeterByFuelType(fuelType,
                                                              reportingFrequency,
                                                              endUseType,
                                                              specificEndUse);
}

std::vector<ExteriorLights> Facility::exteriorLights() const {
  return getImpl<detail::Facility_Impl>()->exteriorLights();
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

} // model
} // openstudio

