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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ComponentCostAdjustments.hpp"
#include "../ComponentCostAdjustments_Impl.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"
#include "../Meter.hpp"
#include "../Meter_Impl.hpp"
#include "../RunPeriod.hpp"
#include "../RunPeriod_Impl.hpp"
#include "../LifeCycleCostParameters.hpp"
#include "../LifeCycleCostParameters_Impl.hpp"
#include "../LifeCycleCostUsePriceEscalation.hpp"
#include "../LifeCycleCostUsePriceEscalation_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/data/EndUses.hpp"

using namespace openstudio::model;
using namespace openstudio;

/*
TEST_F(ModelFixture, FacilityTest) {
  Workspace workspace(idfFile);

  // translate energyplus simulation to Facility model
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel)<< "OptModel not reverse translated - Facility";
  Model model(*optionalModel);

  Facility facility = model.getUniqueModelObject<Facility>();

  EXPECT_FALSE(facility.getAttribute("notAnAttribute"));
  EXPECT_FALSE(facility.isSettableAttribute("notAnAttribute"));
  EXPECT_FALSE(facility.isOptionalAttribute("notAnAttribute"));

  // cannot get results before sql file is loaded
  EXPECT_FALSE(facility.totalSiteEnergy());
  EXPECT_FALSE(facility.isSettableAttribute("totalSiteEnergy"));
  EXPECT_TRUE(facility.isOptionalAttribute("totalSiteEnergy"));
  EXPECT_FALSE(facility.getAttribute("totalSiteEnergy"));
  EXPECT_FALSE(facility.isSettableAttribute("endUsesAttribute"));
  EXPECT_TRUE(facility.isOptionalAttribute("endUsesAttribute"));
  EXPECT_FALSE(facility.getAttribute("endUsesAttribute"));

  // check getters
  openstudio::path sqlPath = resourcesPath() / toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  SqlFile sqlFile(sqlPath);
  ASSERT_TRUE(sqlFile.connectionOpen()) << "sqlFile connection not opened";
  model.setSqlFile(sqlFile);

  // get results
  OptionalDouble totalSiteEnergy = facility.totalSiteEnergy();
  ASSERT_TRUE(totalSiteEnergy);
  EXPECT_TRUE(*totalSiteEnergy > 0);

  OptionalDouble netSiteEnergy = facility.netSiteEnergy();
  ASSERT_TRUE(netSiteEnergy);
  EXPECT_TRUE(*netSiteEnergy > 0);

  OptionalDouble totalSourceEnergy = facility.totalSourceEnergy();
  ASSERT_TRUE(totalSourceEnergy);
  EXPECT_TRUE(*totalSourceEnergy > 0);

  OptionalDouble netSourceEnergy = facility.netSourceEnergy();
  ASSERT_TRUE(netSourceEnergy);
  EXPECT_TRUE(*netSourceEnergy > 0);

  boost::optional<EndUses> endUses = facility.endUses();
  ASSERT_TRUE(endUses);
  EXPECT_NEAR(*totalSiteEnergy, endUses->getEndUseByFuelType(EndUseFuelType::Electricity) +
                                endUses->getEndUseByFuelType(EndUseFuelType::Gas) +
                                endUses->getEndUseByFuelType(EndUseFuelType::OtherFuel) +
                                endUses->getEndUseByFuelType(EndUseFuelType::DistrictCooling) +
                                endUses->getEndUseByFuelType(EndUseFuelType::DistrictHeating), 0.01*(*totalSiteEnergy));

  EXPECT_TRUE(*totalSiteEnergy >= *netSiteEnergy);
  EXPECT_TRUE(*totalSourceEnergy >= *netSourceEnergy);
  EXPECT_TRUE(*totalSourceEnergy >= *totalSiteEnergy);
  EXPECT_TRUE(*netSourceEnergy >= *netSiteEnergy);

  // no utility data in model
  OptionalDouble annualElectricTotalCost = facility.annualElectricTotalCost();
  EXPECT_FALSE(annualElectricTotalCost);

  // get results using attributes
  ASSERT_TRUE(facility.getAttribute("totalSiteEnergy"));
  Attribute attribute = facility.getAttribute("totalSiteEnergy").get();
  EXPECT_EQ(AttributeValueType::Double, attribute.valueType().value());
  EXPECT_EQ(*totalSiteEnergy, attribute.valueAsDouble());

  ASSERT_TRUE(facility.getAttribute("netSiteEnergy"));
  attribute = facility.getAttribute("netSiteEnergy").get();
  EXPECT_EQ(AttributeValueType::Double, attribute.valueType().value());
  EXPECT_EQ(*netSiteEnergy, attribute.valueAsDouble());

  ASSERT_TRUE(facility.getAttribute("endUsesAttribute"));
  attribute = facility.getAttribute("endUsesAttribute").get();
  ASSERT_TRUE(EndUses::fromAttribute(attribute));

  // no utility data in model
  EXPECT_FALSE(facility.getAttribute("annualElectricTotalCost"));
}
*/

/*
TEST_F(ModelFixture, Facility_EconomicsTest) {

  openstudio::path osmPath = resourcesPath() / toPath("model/Daylighting_Office/in.osm");
  OptionalModel optionalModel = Model::load(osmPath);
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  openstudio::path sqlPath = resourcesPath() / toPath("model/Daylighting_Office/eplusout.sql");
  SqlFile sqlFile(sqlPath);
  ASSERT_TRUE(sqlFile.connectionOpen()) << "sqlFile connection not opened";
  model.setSqlFile(sqlFile);

  Facility facility = model.getUniqueModelObject<Facility>();

  boost::optional<double> optionalUtilityCost = facility.annualTotalUtilityCost();
  ASSERT_TRUE(optionalUtilityCost);
  double cost = optionalUtilityCost.get();
  EXPECT_NEAR(cost, 4215.91, 0.1);

  //-----------------------------------------------------------------------------

  ///! Remove unknown costs
  ComponentCost_AdjustmentsVector componentCostAdjustmentss = model.getModelObjects<ComponentCost_Adjustments>();
  for (ComponentCost_Adjustments componentCostAdjustments : componentCostAdjustmentss){
    componentCostAdjustments.remove();
  }

  ComponentCost_LineItemVector componentCostLineItems = model.getModelObjects<ComponentCost_LineItem>();
  for (ComponentCost_LineItem componentCostLineItem : componentCostLineItems){
    componentCostLineItem.remove();
  }

  ComponentCost_ReferenceVector componentCostReferences = model.getModelObjects<ComponentCost_Reference>();
  for (ComponentCost_Reference componentCostReference : componentCostReferences){
    componentCostReference.remove();
  }

  LifeCycleCost_NonrecurringCostVector lifeCycleCostNonrecurringCosts = model.getModelObjects<LifeCycleCost_NonrecurringCost>();
  for (LifeCycleCost_NonrecurringCost lifeCycleCostNonrecurringCost : lifeCycleCostNonrecurringCosts){
    lifeCycleCostNonrecurringCost.remove();
  }

  LifeCycleCost_ParametersVector lifeCycleCostParameterss = model.getModelObjects<LifeCycleCost_Parameters>();
  for (LifeCycleCost_Parameters lifeCycleCostParameters : lifeCycleCostParameterss){
    lifeCycleCostParameters.remove();
  }

  LifeCycleCost_RecurringCostsVector lifeCycleCostRecurringCostss = model.getModelObjects<LifeCycleCost_RecurringCosts>();
  for (LifeCycleCost_RecurringCosts lifeCycleCostRecurringCosts : lifeCycleCostRecurringCostss){
    lifeCycleCostRecurringCosts.remove();
  }

  LifeCycleCost_UseAdjustmentVector lifeCycleCostUseAdjustments = model.getModelObjects<LifeCycleCost_UseAdjustment>();
  for (LifeCycleCost_UseAdjustment lifeCycleCostUseAdjustment : lifeCycleCostUseAdjustments){
    lifeCycleCostUseAdjustment.remove();
  }

  LifeCycleCost_UsePriceEscalationVector lifeCycleCostUsePriceEscalations = model.getModelObjects<LifeCycleCost_UsePriceEscalation>();
  for (LifeCycleCost_UsePriceEscalation lifeCycleCostUsePriceEscalation : lifeCycleCostUsePriceEscalations){
    lifeCycleCostUsePriceEscalation.remove();
  }

  //-----------------------------------------------------------------------------

  ///! Add known ComponentCost_LineItems
  OptionalWorkspaceObject optionalWorkspaceObject;
  OptionalComponentCost_LineItem optionalComponentCost_LineItem;

  optionalWorkspaceObject = model.addObject(openstudio::IdfObject(openstudio::IddObjectType::OS_ComponentCost_LineItem));
  ASSERT_TRUE(optionalWorkspaceObject);
  optionalComponentCost_LineItem = model.getModelObject<ComponentCost_LineItem>(optionalWorkspaceObject->handle());
  ASSERT_TRUE(optionalComponentCost_LineItem);
  optionalComponentCost_LineItem->setName("EXT-SLAB COST");
  optionalComponentCost_LineItem->setItemName("EXT-SLAB");
  optionalComponentCost_LineItem->setLineItemType("CONSTRUCTION");
  optionalComponentCost_LineItem->setMaterialCost(1);

  optionalWorkspaceObject = model.addObject(openstudio::IdfObject(openstudio::IddObjectType::OS_ComponentCost_LineItem));
  ASSERT_TRUE(optionalWorkspaceObject);
  optionalComponentCost_LineItem = model.getModelObject<ComponentCost_LineItem>(optionalWorkspaceObject->handle());
  ASSERT_TRUE(optionalComponentCost_LineItem);
  optionalComponentCost_LineItem->setName("EXT-WALLS-MASS-NONRES COST");
  optionalComponentCost_LineItem->setItemName("EXT-WALLS-MASS-NONRES");
  optionalComponentCost_LineItem->setLineItemType("CONSTRUCTION");
  optionalComponentCost_LineItem->setMaterialCost(1);

  optionalWorkspaceObject = model.addObject(openstudio::IdfObject(openstudio::IddObjectType::OS_ComponentCost_LineItem));
  ASSERT_TRUE(optionalWorkspaceObject);
  optionalComponentCost_LineItem = model.getModelObject<ComponentCost_LineItem>(optionalWorkspaceObject->handle());
  ASSERT_TRUE(optionalComponentCost_LineItem);
  optionalComponentCost_LineItem->setName("WINDOW-90.1-2004-NONRES-FIXED COST");
  optionalComponentCost_LineItem->setItemName("WINDOW-90.1-2004-NONRES-FIXED");
  optionalComponentCost_LineItem->setLineItemType("CONSTRUCTION");
  optionalComponentCost_LineItem->setMaterialCost(1);

  //-----------------------------------------------------------------------------

  ///! Add known LifeCycleCost_Parameters
  optionalWorkspaceObject = model.addObject(openstudio::IdfObject(openstudio::IddObjectType::OS_LifeCycleCost_Parameters));
  ASSERT_TRUE(optionalWorkspaceObject);
  OptionalLifeCycleCost_Parameters optionalLifeCycleCost_Parameters = model.getModelObject<LifeCycleCost_Parameters>(optionalWorkspaceObject->handle());
  ASSERT_TRUE(optionalLifeCycleCost_Parameters);

  ///! Rates are annual, not monthly
  std::vector<double> realDiscountRate;
  std::vector<double> nominalDiscountRate;
  std::vector<double> inflation;
  std::vector<double> expectedCapitalCost;
  std::vector<double> expectedTLCC;

  // -- ConstantDollar - 30 year ------------------------------------------

  optionalLifeCycleCost_Parameters->setInflationApproach(std::string("ConstantDollar"));
  optionalLifeCycleCost_Parameters->setLengthOfStudyPeriodInYears(30);

  ///! EXT-SLAB:                      511.16
  ///! EXT-WALLS-MASS-NONRES:          59.68
  ///! WINDOW-90.1-2004-NONRES-FIXED: 221.83
  ///!                                ------
  ///! TOTAL:                         792.67
  ///! each at $1/sqft
  realDiscountRate.clear();
  realDiscountRate.push_back(0.00); // 0%
  realDiscountRate.push_back(0.03); // 3%
  realDiscountRate.push_back(0.06); // 6%
  realDiscountRate.push_back(0.12); //12%
  nominalDiscountRate.clear(); // nominal discout rate ignored for constant dollar
  inflation.clear();
  expectedCapitalCost.clear();
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedTLCC.clear();
  expectedTLCC.push_back(127269.97);
  expectedTLCC.push_back(83426.37);
  expectedTLCC.push_back(58823.96);
  expectedTLCC.push_back(34752.60);

  for (unsigned i = 0; i < inflation.size(); ++i){
    optionalLifeCycleCost_Parameters->setRealDiscountRate(realDiscountRate[i]);
    optionalLifeCycleCost_Parameters->setNominalDiscountRate(boost::none);
    optionalLifeCycleCost_Parameters->setInflation(boost::none);

    OptionalDouble result = facility.economicsCapitalCost();
    ASSERT_TRUE(result);
    EXPECT_NEAR(expectedCapitalCost[i], *result, 1.0);

    result = facility.economicsTLCC();
    ASSERT_TRUE(result);
    EXPECT_NEAR(expectedTLCC[i], *result, 1.0);
  }

  // -- CurrentDollar - 30 year ------------------------------------------

  optionalLifeCycleCost_Parameters->setInflationApproach(std::string("CurrentDollar"));
  optionalLifeCycleCost_Parameters->setLengthOfStudyPeriodInYears(30);

  ///! EXT-SLAB:                      511.16
  ///! EXT-WALLS-MASS-NONRES:          59.68
  ///! WINDOW-90.1-2004-NONRES-FIXED: 221.83
  ///!                                ------
  ///! TOTAL:                         792.67
  ///! each at $1/sqft
  realDiscountRate.clear(); // real discout rate ignored for current dollar
  nominalDiscountRate.clear();
  nominalDiscountRate.push_back(0.00); // 0%
  nominalDiscountRate.push_back(0.03); // 3%
  nominalDiscountRate.push_back(0.06); // 6%
  nominalDiscountRate.push_back(0.09); // 9%
  nominalDiscountRate.push_back(0.15); //15%
  inflation.clear();
  inflation.push_back(0.03);
  inflation.push_back(0.03);
  inflation.push_back(0.03);
  inflation.push_back(0.03);
  inflation.push_back(0.03);
  expectedCapitalCost.clear();
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedCapitalCost.push_back(792.67);
  expectedTLCC.clear();
  expectedTLCC.push_back(207383.55);
  expectedTLCC.push_back(127269.97);
  expectedTLCC.push_back(84367.56);
  expectedTLCC.push_back(59925.45);
  expectedTLCC.push_back(35652.67);

  for (unsigned i = 0; i < inflation.size(); ++i){
    optionalLifeCycleCost_Parameters->setRealDiscountRate(boost::none);
    optionalLifeCycleCost_Parameters->setNominalDiscountRate(nominalDiscountRate[i]);
    optionalLifeCycleCost_Parameters->setInflation(inflation[i]);

    OptionalDouble result = facility.economicsCapitalCost();
    ASSERT_TRUE(result);
    EXPECT_NEAR(expectedCapitalCost[i], *result, 1.0);

    result = facility.economicsTLCC();
    ASSERT_TRUE(result);
    EXPECT_NEAR(expectedTLCC[i], *result, 1.0);
  }

  / * Excel Formula for test
  $83,426.37  Constant Dollar TLCC                      $83,426.37  Current Dollar TLCC
  3.00% Real Discount Rate           =(1+B2)*(1+E3)-1   6.09% Nominal Discount Rate
                                                        3.00% Inflation Rate
  $792.67 Capital cost                                  $792.67 Capital cost
1 $4,215.91 EnergyCost               =(1+$E$3)*B5       $4,342.39 EnergyCost
2 $4,215.91 EnergyCost               =(1+$E$3)*E5       $4,472.66 EnergyCost
3 $4,215.91 EnergyCost               =(1+$E$3)*E6       $4,606.84 EnergyCost
4 $4,215.91 EnergyCost               =(1+$E$3)*E7       $4,745.04 EnergyCost
5 $4,215.91 EnergyCost               …                  $4,887.40 EnergyCost
6 $4,215.91 EnergyCost                                  $5,034.02 EnergyCost
7 $4,215.91 EnergyCost                                  $5,185.04 EnergyCost
8 $4,215.91 EnergyCost                                  $5,340.59 EnergyCost
9 $4,215.91 EnergyCost                                  $5,500.81 EnergyCost
10  $4,215.91 EnergyCost                                $5,665.83 EnergyCost
11  $4,215.91 EnergyCost                                $5,835.81 EnergyCost
12  $4,215.91 EnergyCost                                $6,010.88 EnergyCost
13  $4,215.91 EnergyCost                                $6,191.21 EnergyCost
14  $4,215.91 EnergyCost                                $6,376.94 EnergyCost
15  $4,215.91 EnergyCost                                $6,568.25 EnergyCost
16  $4,215.91 EnergyCost                                $6,765.30 EnergyCost
17  $4,215.91 EnergyCost                                $6,968.26 EnergyCost
18  $4,215.91 EnergyCost                                $7,177.30 EnergyCost
19  $4,215.91 EnergyCost                                $7,392.62 EnergyCost
20  $4,215.91 EnergyCost                                $7,614.40 EnergyCost
21  $4,215.91 EnergyCost                                $7,842.83 EnergyCost
22  $4,215.91 EnergyCost                                $8,078.12 EnergyCost
23  $4,215.91 EnergyCost                                $8,320.46 EnergyCost
24  $4,215.91 EnergyCost                                $8,570.08 EnergyCost
25  $4,215.91 EnergyCost                                $8,827.18 EnergyCost
26  $4,215.91 EnergyCost                                $9,091.99 EnergyCost
27  $4,215.91 EnergyCost                                $9,364.75 EnergyCost
28  $4,215.91 EnergyCost                                $9,645.70 EnergyCost
29  $4,215.91 EnergyCost                                $9,935.07 EnergyCost
30  $4,215.91 EnergyCost                                $10,233.12  EnergyCost
  * /
}

TEST_F(ModelFixture, Facility_EconomicsTest_NoLifeCycleParameters) {

  openstudio::path osmPath = resourcesPath() / toPath("model/Daylighting_Office/in.osm");
  OptionalModel optionalModel = Model::load(osmPath);
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  openstudio::path sqlPath = resourcesPath() / toPath("model/Daylighting_Office/eplusout.sql");
  SqlFile sqlFile(sqlPath);
  ASSERT_TRUE(sqlFile.connectionOpen()) << "sqlFile connection not opened";
  model.setSqlFile(sqlFile);

  Facility facility = model.getUniqueModelObject<Facility>();

  boost::optional<double> optionalUtilityCost = facility.annualTotalUtilityCost();
  ASSERT_TRUE(optionalUtilityCost);
  double cost = optionalUtilityCost.get();
  EXPECT_NEAR(cost, 4215.91, 0.1);

  //-----------------------------------------------------------------------------

  ///! Remove unknown costs
  ComponentCost_AdjustmentsVector componentCostAdjustmentss = model.getModelObjects<ComponentCost_Adjustments>();
  for (ComponentCost_Adjustments componentCostAdjustments : componentCostAdjustmentss){
    componentCostAdjustments.remove();
  }

  ComponentCost_LineItemVector componentCostLineItems = model.getModelObjects<ComponentCost_LineItem>();
  for (ComponentCost_LineItem componentCostLineItem : componentCostLineItems){
    componentCostLineItem.remove();
  }

  ComponentCost_ReferenceVector componentCostReferences = model.getModelObjects<ComponentCost_Reference>();
  for (ComponentCost_Reference componentCostReference : componentCostReferences){
    componentCostReference.remove();
  }

  LifeCycleCost_NonrecurringCostVector lifeCycleCostNonrecurringCosts = model.getModelObjects<LifeCycleCost_NonrecurringCost>();
  for (LifeCycleCost_NonrecurringCost lifeCycleCostNonrecurringCost : lifeCycleCostNonrecurringCosts){
    lifeCycleCostNonrecurringCost.remove();
  }

  LifeCycleCost_ParametersVector lifeCycleCostParameterss = model.getModelObjects<LifeCycleCost_Parameters>();
  for (LifeCycleCost_Parameters lifeCycleCostParameters : lifeCycleCostParameterss){
    lifeCycleCostParameters.remove();
  }

  LifeCycleCost_RecurringCostsVector lifeCycleCostRecurringCostss = model.getModelObjects<LifeCycleCost_RecurringCosts>();
  for (LifeCycleCost_RecurringCosts lifeCycleCostRecurringCosts : lifeCycleCostRecurringCostss){
    lifeCycleCostRecurringCosts.remove();
  }

  LifeCycleCost_UseAdjustmentVector lifeCycleCostUseAdjustments = model.getModelObjects<LifeCycleCost_UseAdjustment>();
  for (LifeCycleCost_UseAdjustment lifeCycleCostUseAdjustment : lifeCycleCostUseAdjustments){
    lifeCycleCostUseAdjustment.remove();
  }

  LifeCycleCost_UsePriceEscalationVector lifeCycleCostUsePriceEscalations = model.getModelObjects<LifeCycleCost_UsePriceEscalation>();
  for (LifeCycleCost_UsePriceEscalation lifeCycleCostUsePriceEscalation : lifeCycleCostUsePriceEscalations){
    lifeCycleCostUsePriceEscalation.remove();
  }

  OptionalDouble result = facility.economicsCapitalCost();
  EXPECT_FALSE(result);

  result = facility.economicsTLCC();
  EXPECT_FALSE(result);
}

TEST_F(ModelFixture, FacilityEconomicsTest_NoSqlFile) {

  openstudio::path osmPath = resourcesPath() / toPath("model/Daylighting_Office/in.osm");
  OptionalModel optionalModel = Model::load(osmPath);
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  Facility facility = model.getUniqueModelObject<Facility>();

  boost::optional<double> optionalUtilityCost = facility.annualTotalUtilityCost();
  EXPECT_FALSE(optionalUtilityCost);

  //-----------------------------------------------------------------------------

  ///! Remove unknown costs
  ComponentCost_AdjustmentsVector componentCostAdjustmentss = model.getModelObjects<ComponentCost_Adjustments>();
  for (ComponentCost_Adjustments componentCostAdjustments : componentCostAdjustmentss){
    componentCostAdjustments.remove();
  }

  ComponentCost_LineItemVector componentCostLineItems = model.getModelObjects<ComponentCost_LineItem>();
  for (ComponentCost_LineItem componentCostLineItem : componentCostLineItems){
    componentCostLineItem.remove();
  }

  ComponentCost_ReferenceVector componentCostReferences = model.getModelObjects<ComponentCost_Reference>();
  for (ComponentCost_Reference componentCostReference : componentCostReferences){
    componentCostReference.remove();
  }

  LifeCycleCost_NonrecurringCostVector lifeCycleCostNonrecurringCosts = model.getModelObjects<LifeCycleCost_NonrecurringCost>();
  for (LifeCycleCost_NonrecurringCost lifeCycleCostNonrecurringCost : lifeCycleCostNonrecurringCosts){
    lifeCycleCostNonrecurringCost.remove();
  }

  LifeCycleCost_ParametersVector lifeCycleCostParameterss = model.getModelObjects<LifeCycleCost_Parameters>();
  for (LifeCycleCost_Parameters lifeCycleCostParameters : lifeCycleCostParameterss){
    lifeCycleCostParameters.remove();
  }

  LifeCycleCost_RecurringCostsVector lifeCycleCostRecurringCostss = model.getModelObjects<LifeCycleCost_RecurringCosts>();
  for (LifeCycleCost_RecurringCosts lifeCycleCostRecurringCosts : lifeCycleCostRecurringCostss){
    lifeCycleCostRecurringCosts.remove();
  }

  LifeCycleCost_UseAdjustmentVector lifeCycleCostUseAdjustments = model.getModelObjects<LifeCycleCost_UseAdjustment>();
  for (LifeCycleCost_UseAdjustment lifeCycleCostUseAdjustment : lifeCycleCostUseAdjustments){
    lifeCycleCostUseAdjustment.remove();
  }

  LifeCycleCost_UsePriceEscalationVector lifeCycleCostUsePriceEscalations = model.getModelObjects<LifeCycleCost_UsePriceEscalation>();
  for (LifeCycleCost_UsePriceEscalation lifeCycleCostUsePriceEscalation : lifeCycleCostUsePriceEscalations){
    lifeCycleCostUsePriceEscalation.remove();
  }

  OptionalDouble result = facility.economicsCapitalCost();
  EXPECT_FALSE(result);

  result = facility.economicsTLCC();
  EXPECT_FALSE(result);
}
*/

TEST_F(ModelFixture, Facility_EconomicsTest_EmptyModel) {

  Model model;

  Facility facility = model.getUniqueModelObject<Facility>();

  OptionalDouble result = facility.economicsCapitalCost();
  EXPECT_FALSE(result);

  result = facility.economicsTLCC();
  EXPECT_FALSE(result);

  EXPECT_EQ(0u, facility.meters().size());

  boost::optional<Meter> electricMeter = facility.getMeterByFuelType(FuelType::Electricity, "Hourly");
  EXPECT_FALSE(electricMeter);

  Meter meter(model);
  meter.setFuelType(FuelType::Electricity);
  meter.setInstallLocationType(InstallLocationType::Facility);
  meter.setReportingFrequency("Hourly");
  
  electricMeter = facility.getMeterByFuelType(FuelType::Electricity, "Hourly");
  ASSERT_TRUE(electricMeter);

  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();

  boost::optional<openstudio::TimeSeries> timeSeries = electricMeter->getData(runPeriod.name().get()); 
  if (timeSeries){
    openstudio::Vector values = timeSeries->values();
    unsigned numValues = values.size();
    for (unsigned i = 0; i < numValues; ++i){
      EXPECT_NO_THROW(values(i));
    }
  }

}
