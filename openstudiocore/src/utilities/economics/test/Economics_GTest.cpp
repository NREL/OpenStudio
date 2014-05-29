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

#include <vector>
#include <gtest/gtest.h>
#include "../Economics.hpp"

using namespace openstudio;

TEST(Economics, EconomicsTest) {

  openstudio::Economics economics;
  double returnValue;

  // TODO calls to eventually implement
  //returnValue = economics.FirstCosts( BuildingType buildingType );
  //returnValue = economics.GetCapitalCost( BuildingType buildingType );
  //returnValue = economics.GetEnergyCost( BuildingType buildingType );
  //returnValue = economics.GetCapitalAnalysisEnergyCost( BuildingType buildingType );
  //returnValue = economics.GetVirtualRate( BuildingType buildingType, VirtualRateType virtualRateType );
  //returnValue = economics.PayBack( const std::vector< double >& cashFlowSavings );
  //returnValue = economics.LevelizedCostOfEnergy( const double TLCC, LevelizationType levelOn );

  double rate = 0.005; // 6%/12 = 0.06/12
  double periods = 360;
  double presentValue = 100000;
  returnValue = economics.getFV( rate, periods, presentValue );
  EXPECT_EQ(static_cast<int>(returnValue),602257);

  /***********************************************************************************
  *                                                                                  *
  *  Values obtained from Table A-1                                                  *
  *  NISTIR 85-3273-24                                                               *
  *  (Rev. 5/09)                                                                     *
  *  Energy Price Indices and Discount Factors for Life-Cycle Cost Analysis – 2009   *
  *                                                                                  *
  ***********************************************************************************/
  rate = 0.03; // 3% = 0.03
  periods = 7;
  returnValue = economics.getSPVFactor(rate,periods);
  returnValue *= 1000000;
  EXPECT_EQ(static_cast<int>(returnValue),813091);

  /***********************************************************************************
  *                                                                                  *
  *  Values obtained from Table A-1                                                  *
  *  NISTIR 85-3273-24                                                               *
  *  (Rev. 5/09)                                                                     *
  *  Energy Price Indices and Discount Factors for Life-Cycle Cost Analysis – 2009   *
  *                                                                                  *
  ***********************************************************************************/
  returnValue = economics.getUPVFactor(rate,periods);
  returnValue *= 100000;
  EXPECT_EQ(static_cast<int>(returnValue),623028);

  std::vector <double> cashFlows;
  cashFlows.push_back(-70000);
  cashFlows.push_back(12000);
  cashFlows.push_back(15000);

  returnValue = economics.getSum(cashFlows);
  EXPECT_EQ(returnValue,-43000);

  /***********************************************************************************
  *                                                                                  *
  *  Values obtained from Table 5-2                                                  *
  *  NIST Handbook 135                                                               *
  *  1995 Edition                                                                    *
  *  Life-Cycle Costing Manual for the Federal Energy Management Program             *
  *                                                                                  *
  ***********************************************************************************/
  openstudio::Economics::BuildingType buildingType = openstudio::Economics::btReference;
  std::vector< double > cashFlowsInflation;
  cashFlowsInflation.push_back(103000);
  cashFlowsInflation.push_back(8412);
  cashFlowsInflation.push_back(-1939);
  cashFlowsInflation.push_back(302600);
  cashFlowsInflation.push_back(104160);
  economics.setCashFlowsInflation(buildingType,cashFlowsInflation);
  returnValue = economics.getTLCC(buildingType);
  EXPECT_EQ(returnValue,-516233);

  /***********************************************************************************
  *                                                                                  *
  *  All IRR values obtained Microsoft Excel 2008                                    *
  *  NIST Handbook 135                                                               *
  *  1995 Edition                                                                    *
  *  Life-Cycle Costing Manual for the Federal Energy Management Program             *
  *                                                                                  *
  ***********************************************************************************/
  double irr;
  EXPECT_EQ(economics.getIRR(cashFlows,irr),true);
  irr *= 10000;
  EXPECT_EQ(static_cast<int>(irr),-4435);

  cashFlows.push_back(18000);
  EXPECT_EQ(economics.getIRR(cashFlows,irr),true);
  irr *= 10000;
  EXPECT_EQ(static_cast<int>(irr),-1821);

  cashFlows.push_back(21000);
  EXPECT_EQ(economics.getIRR(cashFlows,irr),true);
  irr *= 10000;
  EXPECT_EQ(static_cast<int>(irr),-212);
  
  cashFlows.push_back(26000);
  EXPECT_EQ(economics.getIRR(cashFlows,irr),true);
  irr *= 10000;
  EXPECT_EQ(static_cast<int>(irr),866);

  /***********************************************************************************
  *                                                                                  *
  *  TODO Add NIST citation, and use their example data                              *
  *  NIST Handbook 135                                                               *
  *  1995 Edition                                                                    *
  *  Life-Cycle Costing Manual for the Federal Energy Management Program             *
  *                                                                                  *
  ***********************************************************************************/
  Economics::BuildingType referenceBldg = Economics::btReference;
  economics.addCostInstance(referenceBldg,"Obj1",100000,10000,1000,100,30,0);
  economics.addDistrictCool(referenceBldg,1000,100,0.1);
  economics.addDistrictHeat(referenceBldg,1000,100,0.1);
  economics.addElectricity(referenceBldg,1000,100,0.1);
  economics.addGas(referenceBldg,1000,100,0.1);
  economics.setInflation(referenceBldg, 0.06);
  economics.setOMInflation(referenceBldg,0.06);
  economics.setDiscountRate(0.03);
  economics.setAnalysisPeriod(30);

  economics.calculateSingleEconomics(referenceBldg);

  std::vector<double> returnVectorRef;
  ///! cash flows
  economics.cashFlows(referenceBldg,returnVectorRef);
  economics.cashFlowsInflation(referenceBldg,returnVectorRef);
  economics.cashFlowsGasInflation(referenceBldg,returnVectorRef);
  economics.cashFlowsElecInflation(referenceBldg,returnVectorRef);
  economics.cashFlowsDistrictHeatInflation(referenceBldg,returnVectorRef);
  economics.cashFlowsDistrictCoolInflation(referenceBldg,returnVectorRef);
  economics.cashFlowsCapitalInflation(referenceBldg,returnVectorRef);

  ///! TLCC
  returnValue = economics.getTLCC(referenceBldg);
  // TODO EXPECT_EQ(returnValue,???);

  /***********************************************************************************
  *                                                                                  *
  *  TODO Add NIST citation, and use their example data                              *
  *  NIST Handbook 135                                                               *
  *  1995 Edition                                                                    *
  *  Life-Cycle Costing Manual for the Federal Energy Management Program             *
  *                                                                                  *
  ***********************************************************************************/
  Economics::BuildingType selectedBldg = Economics::btSelected;

  economics.addCostInstance(selectedBldg,"Obj2",100000,10000,1000,100,30,0);
  economics.addDistrictCool(selectedBldg,10000,100,0.1);
  economics.addDistrictHeat(selectedBldg,10000,100,0.1);
  economics.addElectricity(selectedBldg,10000,100,0.1);
  economics.addGas(selectedBldg,10000,100,0.1);
  economics.setInflation(selectedBldg, 0.06);
  economics.setOMInflation(selectedBldg,0.06);
  economics.setDiscountRate(0.03);

  economics.calculateSingleEconomics(selectedBldg);

  economics.calculateEconomics();

  returnValue = economics.simplePayback();
  // TODO EXPECT_EQ(returnValue,???);
  returnValue = economics.discountedPayback();
  // TODO EXPECT_EQ(returnValue,???);

}

TEST(Economics, EconomicsTest2)
{
  openstudio::Economics economics;
  
    // Test divide by zero
  economics.setVariables(true,
                         0,
                         0.0,
                         0.0,
                         0.0,
                         0,
                         0,
                         0.0,
                         0.0,
                         0,
                         0.0,
                         0.0,
                         0,
                         0.0,
                         0.0,
                         0.0,
                         0.0,
                         0.0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0.0);

  std::vector<double> cashFlows = economics.getCashFlows(true);

  {
  double realDiscountFactor = economics.realDiscountFactor();
  /* double npv = */ economics.getNPV(realDiscountFactor,cashFlows);
  /* double payback = */ economics.getPayBack(cashFlows);
  }

  // Test Miami SC_ED_01, includeEnergySavings = true
  economics.setVariables(true,
                         20,
                         0.03, // 3%
                         0.005, // 0.5%
                         0.02, // 2.0%
                         0,
                         10,
                         17580.00,
                         13273.00,
                         0,
                         589.00,
                         -8841.20,
                         0,
                         25153.60,
                         3202.25,
                         -43.45,
                         3202.25,
                         -43.45,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         20,
                         18865.20);

  cashFlows = economics.getCashFlows(true);
  double desiredCashFlow = 0;
  for(unsigned year=0; year<cashFlows.size(); year++){
    double cashFlow = cashFlows.at(year);
    switch(year)
    {
    case 0:
      desiredCashFlow = -31442;
      break;
    case 1:
      desiredCashFlow = 12015;
      break;
    case 5:
      desiredCashFlow = 12076;
      break;
    case 10:
      desiredCashFlow = 5866;
      break;
    case 15:
      desiredCashFlow = 12234;
      break;
    case 20:
      desiredCashFlow = 2882;
      break;
    default:
      break;
    }

    if(year==0 || year==1 || year==5 || year==10 || year==15 || year==20){
      EXPECT_NEAR(desiredCashFlow,cashFlow,1.0);
    }

    if(year==20){
      double desiredNPV = 139384.46;
      double desiredPayback = 2.6;
      double npv = economics.getNPV(economics.realDiscountFactor(),cashFlows);
      double payback = economics.getPayBack(cashFlows);
      EXPECT_NEAR(desiredNPV,npv,1.0);
      EXPECT_NEAR(desiredPayback,payback,1.0);
    }
  }

  // Test Miami SC_ED_01, includeEnergySavings = false
  economics.setVariables(true,
                         20,
                         0.03, // 3%
                         0.005, // 0.5%
                         0.02, // 2.0%
                         0,
                         10,
                         17580.00,
                         13273.00,
                         0,
                         589.00,
                         -8841.20,
                         0,
                         25153.60,
                         3202.25,
                         -43.45,
                         3202.25,
                         -43.45,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         20,
                         18865.20);

  cashFlows = economics.getCashFlows(false);
  desiredCashFlow = 0;
  for(unsigned year=0; year<cashFlows.size(); year++){
    double cashFlow = cashFlows.at(year);
    switch(year)
    {
    case 0:
      desiredCashFlow = -31442;
      break;
    case 1:
      desiredCashFlow = 8841;
      break;
    case 5:
      desiredCashFlow = 8841;
      break;
    case 10:
      desiredCashFlow = 2553;
      break;
    case 15:
      desiredCashFlow = 8841;
      break;
    case 20:
      desiredCashFlow = -591;
      break;
    default:
      break;
    }

    if(year==0 || year==1 || year==5 || year==10 || year==15 || year==20){
      EXPECT_NEAR(desiredCashFlow,cashFlow,1.0);
    }
  }

  // Test Miami SC_ED_02, includeEnergySavings = true
  economics.setVariables(true,
                         20,
                         0.03, // 3%
                         0.005, // 0.5%
                         0.02, // 2.0%
                         0,
                         12,
                         251897.00,
                         237433.00,
                         0,
                         60859.00,
                         1685.43,
                         0,
                         391464.00,
                         32640.56,
                         783.62,
                         32640.56,
                         783.62,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         12,
                         354457.00);

  cashFlows = economics.getCashFlows(true);
  desiredCashFlow = 0;
  for(unsigned year=0; year<cashFlows.size(); year++){
    double cashFlow = cashFlows.at(year);
    switch(year)
    {
    case 0:
      desiredCashFlow = -550189;
      break;
    case 1:
      desiredCashFlow = 31918;
      break;
    case 6:
      desiredCashFlow = 387286;
      break;
    case 12:
      desiredCashFlow = -357502;
      break;
    case 18:
      desiredCashFlow = 389597;
      break;
    case 20:
      desiredCashFlow = -129349;
      break;
    }

    if(year==0 || year==1 || year==6 || year==12 || year==18 || year==20){
      EXPECT_NEAR(desiredCashFlow,cashFlow,1.0);
    }

    if(year==20){
      double desiredNPV = 87538.42;
      double desiredPayback = 17.2;
      double npv = economics.getNPV(economics.realDiscountFactor(),cashFlows);
      double payback = economics.getPayBack(cashFlows);
      EXPECT_NEAR(desiredNPV,npv,1.0);
      EXPECT_NEAR(desiredPayback,payback,1.0);
    }
  }

  // Test Miami SC_ED_02, includeEnergySavings = false
  economics.setVariables(true,
                         20,
                         0.03, // 3%
                         0.005, // 0.5%
                         0.02, // 2.0%
                         0,
                         12,
                         251897.00,
                         237433.00,
                         0,
                         60859.00,
                         1685.43,
                         0,
                         391464.00,
                         32640.56,
                         783.62,
                         32640.56,
                         783.62,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         12,
                         354457.00);

  cashFlows = economics.getCashFlows(false);
  desiredCashFlow = 0;
  for(unsigned year=0; year<cashFlows.size(); year++){
    double cashFlow = cashFlows.at(year);
    switch(year)
    {
    case 0:
      desiredCashFlow = -550189;
      break;
    case 1:
      desiredCashFlow = -1685;
      break;
    case 6:
      desiredCashFlow = 352772;
      break;
    case 12:
      desiredCashFlow = -393149;
      break;
    case 18:
      desiredCashFlow = 352772;
      break;
    case 20:
      desiredCashFlow = -166578;
      break;
    default:
      break;
    }

    if(year==0 || year==1 || year==6 || year==12 || year==18 || year==20){
      EXPECT_NEAR(desiredCashFlow,cashFlow,1.0);
    }
  }

  // Test Miami SC_ED_01 MODIFIED TO FULLY EXERCISE SPREADSHEET, includeEnergySavings = true
  economics.setVariables(true,
                         20,
                         0.03, // 3%
                         0.005, // 0.5%
                         0.02, // 2.0%
                         0.0125, // 1.25%
                         10,
                         17580.00,
                         13273.00,
                         5000,
                         589.00,
                         -8841.20,
                         0,
                         25153.60,
                         3202.25,
                         -43.45,
                         3202.25,
                         -43.45,
                         10,
                         20,
                         30,
                         40,
                         50,
                         60,
                         70,
                         80,
                         20,
                         18865.20);

  cashFlows = economics.getCashFlows(true);
  desiredCashFlow = 0;
  for(unsigned year=0; year<cashFlows.size(); year++){
    double cashFlow = cashFlows.at(year);
    switch(year)
    {
    case 0:
      desiredCashFlow = -26458.93;
      break;
    case 1:
      desiredCashFlow = 11785.95;
      break;
    case 5:
      desiredCashFlow = 11846.39;
      break;
    case 10:
      desiredCashFlow = 5634.92;
      break;
    case 15:
      desiredCashFlow = 12001.81;
      break;
    case 20:
      desiredCashFlow = 2649.25;
      break;
    default:
      break;
    }

    if(year==0 || year==1 || year==5 || year==10 || year==15 || year==20){
      EXPECT_NEAR(desiredCashFlow,cashFlow,1.0);
    }

    if(year==20){
      double desiredNPV = 140933.11;
      double desiredPayback = 2.24;
      double npv = economics.getNPV(economics.realDiscountFactor(),cashFlows);
      double payback = economics.getPayBack(cashFlows);
      EXPECT_NEAR(desiredNPV,npv,1.0);
      EXPECT_NEAR(desiredPayback,payback,1.0);
    }
  }

  // Test Miami SC_ED_01 MODIFIED TO FULLY EXERCISE SPREADSHEET, includeEnergySavings = false
  economics.setVariables(true,
                         20,
                         0.03, // 3%
                         0.005, // 0.5%
                         0.02, // 2.0%
                         0.0125, // 1.25%
                         10,
                         17580.00,
                         13273.00,
                         5000,
                         589.00,
                         -8841.20,
                         0,
                         25153.60,
                         3202.25,
                         -43.45,
                         3202.25,
                         -43.45,
                         10,
                         20,
                         30,
                         40,
                         50,
                         60,
                         70,
                         80,
                         20,
                         18865.20);

  cashFlows = economics.getCashFlows(false);
  desiredCashFlow = 0;
  for(unsigned year=0; year<cashFlows.size(); year++){
    double cashFlow = cashFlows.at(year);
    switch(year)
    {
    case 0:
      desiredCashFlow = -26458.93;
      break;
    case 1:
      desiredCashFlow = 8651.69;
      break;
    case 5:
      desiredCashFlow = 8651.69;
      break;
    case 10:
      desiredCashFlow = 2363.29;
      break;
    case 15:
      desiredCashFlow = 8651.69;
      break;
    case 20:
      desiredCashFlow = -780.91;
      break;
    default:
      break;
    }

    if(year==0 || year==1 || year==5 || year==10 || year==15 || year==20){
      EXPECT_NEAR(desiredCashFlow,cashFlow,1.0);
    }
  }

}
