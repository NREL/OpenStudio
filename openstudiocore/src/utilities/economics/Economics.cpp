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
#include <cstdlib>
#include <cmath>

#include "CostObject.hpp"
#include "Economics.hpp"

#include "../core/Assert.hpp"

#define MAX_ERR 0.0001
#define MAX_LIMIT 100000
#define MAX_ITERATIONS 10000

namespace openstudio{

Economics::Economics():
  m_analysisPeriod(0),
  m_capitalCostSavings(0),
  m_energyCostSavings(0),
  m_discountRate(0),
  m_DPB(0),
  m_DPB_TLCC(0),
  m_gasCostSavings(0),
  m_FV(0),
  m_elecCostSavings(0),
  m_gasUseSavings(0),
  m_elecUseSavings(0),
  m_districtHeatUseSavings(0),
  m_capitalAnalysisEnergyCostSavings(0),
  m_districtCoolCostSavings(0),
  m_districtCoolUseSavings(0),
  m_districtHeatCostSavings(0),
  m_virtualRate_Combined(0),
  m_IRR(0),
  m_LCOE_Cost(0),
  m_LCOE_Energy(0),
  m_margIncomeTaxRate(0),
  m_NPV(0),
  m_SPB(0),
  m_TLCC_Savings(0),
  m_totalEnergyCostSavings(0),
  m_totalEnergyUseSavings(0),

  m_stdRetrofit(true),
  m_realDiscountFactor(0),
  m_realElectricityPriceEscalationRate(0),
  m_realGasPriceEscalationRate(0),
  m_federalCorporateTaxRate(0),
  m_usefulLife(0),
  m_purchaseCost(0),
  m_installationCost(0),
  m_totalCapitalCost(0),
  m_salvageValueExisting(0),
  m_undepreciatedValueExisting(0),
  m_taxDeduction(0),
  m_foregoneTaxDeduction(0),
  m_disposalCost(0),
  m_fixedOM(0),
  m_fixedMV(0),
  m_replacementCost(0),
  m_remainingUsefulLifeOfMeasure(0),
  m_salvageValueMeasure(0),
  m_electricityCostSavings(0),
  m_electricityCostSavingsCodeMinimum(0), 
  m_gasCostSavingsCodeMinimum(0),
  m_specialTaxDeductions(0),
  m_specialTaxCredits(0),
  m_rebates(0),
  m_depreciationTaxDeduction(0),
  m_otherFinancialIncentives(0),
  m_wholeBldFinancialIncentives(0),
  m_projectPlanningAndDesign(0),
  m_packageMV(0),
  m_usefulLifeExisting(0),
  m_remainingUsefulLifeExisting(0),
  m_replacementCostCodeMinimum(0),
  m_salvageValue(0),
  m_remainingUsefulLifeProjectEnd(0),
  m_variablesSet(false),

  m_refBuilding(std::shared_ptr<CostObject>(new CostObject)),
  m_selBuilding(std::shared_ptr<CostObject>(new CostObject))
{
}

Economics::~Economics()
{
}

void Economics::addCostInstance( BuildingType buildingType, std::string name, OptionalDouble matCost, OptionalDouble installCost, OptionalDouble fixedOM, OptionalDouble varOM, OptionalInt expectedLife, OptionalDouble salvageCost, OptionalDouble netArea )
{
  double _matCost = 0, _installCost = 0, _fixedOM = 0, _varOM = 0, _salvageCost = 0, _netArea = 0;
  int _expectedLife = 0;

  if(matCost) _matCost = *matCost;
  if(installCost) _installCost = *installCost;
  if(fixedOM) _fixedOM = *fixedOM;
  if(varOM) _varOM =  *varOM;
  if(salvageCost) _salvageCost = *salvageCost;
  if(netArea) _netArea = *netArea;
  if(expectedLife) _expectedLife = *expectedLife;

  addCostInstance(buildingType, name, _matCost, _installCost, _fixedOM, _varOM, _expectedLife, _salvageCost, _netArea);
}

void Economics::addCostInstance( BuildingType buildingType, std::string name, double matCost, double installCost, double fixedOM, double varOM, int expectedLife, double salvageCost, double netArea )
{
  addCostInstance(buildingType, name, matCost*netArea, installCost*netArea, fixedOM*netArea, varOM*netArea, expectedLife, salvageCost*netArea);
}

void Economics::addCostInstance( BuildingType buildingType, std::string name, double matCost, double installCost, double fixedOM, double varOM, int expectedLife, double salvageCost )
{
  switch ( buildingType )
  {
    case btReference:
      m_refBuilding->addCostInstance( name, matCost, installCost, fixedOM, varOM, expectedLife, salvageCost );
    break;
    case btSelected:
      m_selBuilding->addCostInstance( name, matCost, installCost, fixedOM, varOM, expectedLife, salvageCost );
    break;
  } //case
}

void Economics::addElectricity( BuildingType buildingType, OptionalDouble use, OptionalDouble cost, OptionalDouble inflation )
{
  double _use = 0, _cost = 0, _inflation = 0;

  if(use) _use = *use;
  if(cost) _cost = *cost;
  if(inflation) _inflation = *inflation;

  addElectricity( buildingType, _use, _cost, _inflation );
}

void Economics::addElectricity( BuildingType buildingType, double use, double cost, double inflation )
{
  switch ( buildingType )
  {
    case btReference:
    {
      m_refBuilding->setElecUse(use);
      m_refBuilding->setElecCost(cost);
      m_refBuilding->setElecInflation(inflation);
    }
    break;
    case btSelected:
    {
      m_selBuilding->setElecUse(use);
      m_selBuilding->setElecCost(cost);
      m_selBuilding->setElecInflation(inflation);
    }
    break;
  } //case
}

void Economics::addGas( BuildingType buildingType, OptionalDouble use, OptionalDouble cost, OptionalDouble inflation )
{
  double _use = 0, _cost = 0, _inflation = 0;

  if(use) _use = *use;
  if(cost) _cost = *cost;
  if(inflation) _inflation = *inflation;

  addGas( buildingType, _use, _cost, _inflation );
}

void Economics::addGas( BuildingType buildingType, double use, double cost, double inflation )
{
  switch ( buildingType )
  {
    case btReference:
    {
      m_refBuilding->setGasUse(use);
      m_refBuilding->setGasCost(cost);
      m_refBuilding->setGasInflation(inflation);
    }
    break;
    case btSelected:
    {
      m_selBuilding->setGasUse(use);
      m_selBuilding->setGasCost(cost);
      m_selBuilding->setGasInflation(inflation);
    }
    break;
  } //case
}

void Economics::addDistrictHeat( BuildingType buildingType, double use, double cost, double inflation )
{
  switch ( buildingType )
  {
    case btReference:
    {
      m_refBuilding->setDistrictHeatUse(use);
      m_refBuilding->setDistrictHeatCost(cost);
      m_refBuilding->setDistrictHeatInflation(inflation);
    }
    break;
    case btSelected:
    {
      m_selBuilding->setDistrictHeatUse(use);
      m_selBuilding->setDistrictHeatCost(cost);
      m_selBuilding->setDistrictHeatInflation(inflation);
    }
    break;
  } //case
}

void Economics::addDistrictCool( BuildingType buildingType, double use, double cost, double inflation )
{
  switch ( buildingType )
  {
    case btReference:
    {
      m_refBuilding->setDistrictCoolUse(use);
      m_refBuilding->setDistrictCoolCost(cost);
      m_refBuilding->setDistrictCoolInflation(inflation);
    }
    break;
    case btSelected:
    {
      m_selBuilding->setDistrictCoolUse(use);
      m_selBuilding->setDistrictCoolCost(cost);
      m_selBuilding->setDistrictCoolInflation(inflation);
    }
    break;
  } //case
}

void Economics::getGas( BuildingType buildingType, double& use, double& cost )
{
  switch ( buildingType )
  {
    case btReference:
    {
      use = m_refBuilding->gasUse();
      cost = m_refBuilding->gasCost();
    }
    break;
    case btSelected:
    {
      use = m_selBuilding->gasUse();
      cost = m_selBuilding->gasCost();
    }
    break;
  } //case
}

void Economics::calculateCashFlowSavings( const std::vector< double >& refCashFlows, const std::vector< double >& selCashFlows )
{
  if(refCashFlows.size() != selCashFlows.size()) return;

  m_cashFlowSavings.clear();
  m_discountedCashFlowSavings.clear();
  // TODO replace resize in project with push_back
  m_cashFlowSavings.resize(m_analysisPeriod + 1);
  for (unsigned i = 0; i < m_cashFlowSavings.size(); i++)
  {
    m_cashFlowSavings.at(i) = selCashFlows.at(i) - refCashFlows.at(i);
  }

  //Discounted CashFlow Savings
  m_discountedCashFlowSavings.resize(m_analysisPeriod + 1);
  for (unsigned i = 0; i < m_discountedCashFlowSavings.size(); i++)
  {
    double PVIF = double( 1 ) / ( pow( 1 + m_discountRate, static_cast<int>(i) ) );
    m_discountedCashFlowSavings.at(i) = PVIF * ( selCashFlows.at(i) - refCashFlows.at(i) );
  }
}

void Economics::calculateEconomics()
{
  //year 0:  First cost
  //year 1:  payment
  //year 15: equipment/maintenance --
  m_gasCostSavings = m_refBuilding->gasCost() - m_selBuilding->gasCost();
  m_gasUseSavings = m_refBuilding->gasUse() - m_selBuilding->gasUse();
  m_elecCostSavings = m_refBuilding->elecCost() - m_selBuilding->elecCost();
  m_elecUseSavings = m_refBuilding->elecUse() - m_selBuilding->elecUse();
  m_districtHeatCostSavings = m_refBuilding->districtHeatCost() - m_selBuilding->districtHeatCost();
  m_districtHeatUseSavings = m_refBuilding->districtHeatUse() - m_selBuilding->districtHeatUse();
  m_districtCoolCostSavings = m_refBuilding->districtCoolCost() - m_selBuilding->districtCoolCost();
  m_districtCoolUseSavings = m_refBuilding->districtCoolUse() - m_selBuilding->districtCoolUse();
  m_totalEnergyCostSavings = m_gasCostSavings + m_elecCostSavings + m_districtHeatCostSavings + m_districtCoolCostSavings;
  m_totalEnergyUseSavings = m_gasUseSavings + m_elecUseSavings + m_districtHeatUseSavings + m_districtCoolUseSavings;
  calculateCashFlowSavings( m_refBuilding->cashFlows(), m_selBuilding->cashFlows() );
  m_capitalCostSavings = - m_cashFlowSavings.at(0);  //negate for cash flow into building
  m_energyCostSavings = - m_totalEnergyCostSavings; //negate for cash flow into of building
  m_capitalAnalysisEnergyCostSavings = m_capitalCostSavings + ( m_analysisPeriod * m_energyCostSavings );
  m_NPV = getSum( m_discountedCashFlowSavings );
  if(!getIRR( m_cashFlowSavings, m_IRR )){
    m_IRR = 0;
  }
  m_SPB = payBack( m_cashFlowSavings );
  m_DPB = payBack( m_discountedCashFlowSavings );

  m_FV = getFV( m_discountRate, m_analysisPeriod, m_cashFlowSavings.at(0) );

  //add inflations to cash flows
  calculateCashFlowSavings( m_refBuilding->cashFlows_Inflation(), m_selBuilding->cashFlows_Inflation() );
  m_TLCC_Savings = getSum( m_discountedCashFlowSavings );
  m_LCOE_Cost = levelizedCostOfEnergy( m_TLCC_Savings, ltCost );
  m_LCOE_Energy = levelizedCostOfEnergy( m_TLCC_Savings, ltEnergy );
  m_DPB_TLCC = payBack( m_discountedCashFlowSavings );
}

double Economics::getNPV(const double rate, const std::vector<double>& cashFlows)
{
  double sum = 0;
  for(unsigned i=0; i<cashFlows.size(); i++){
    if(pow(1+rate,static_cast<int>(i)) != 0){
      sum += cashFlows.at(i) / pow(1+rate,static_cast<int>(i));
    }
  }
  return sum;
}

void Economics::calculateSingleEconomics( BuildingType buildingType )
{
  //this procedure calculates all of the cash flows for the building type passed.
  switch ( buildingType )
  {
    case btReference:
    {
      //output the cost objects for each
      m_refBuilding->calculateCashFlows( m_analysisPeriod );  //populate the cashflow array from TCostObjs to do calculations
    }
    break;
    case btSelected:
    {
      //output the cost objects for each
      m_selBuilding->calculateCashFlows( m_analysisPeriod );  //populate the cashflow array from TCostObjs to do calculations
    }
    break;  //btSelected
  }  //case
}

double Economics::firstCosts( BuildingType buildingType )
{
  double result = 0.0;
  switch ( buildingType )
  {
    case btReference:
      result = m_refBuilding->firstCosts();
    break;
    case btSelected:
      result = m_selBuilding->firstCosts();
    break;
  } //case
  return result;
}

void Economics::cashFlows( BuildingType buildingType, std::vector< double >& resArray )
{
  if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows();
  }
}

void Economics::cashFlowsInflation( BuildingType buildingType, std::vector< double >& resArray )
{
  if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows_Inflation();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows_Inflation();
  }
}

void Economics::cashFlowsElecInflation( BuildingType buildingType, std::vector< double >& resArray )
{
  if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows_Elec_Inflation();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows_Elec_Inflation();
  }
}

void Economics::cashFlowsDistrictHeatInflation( BuildingType buildingType, std::vector< double >& resArray )
{
  if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows_DistrictHeat_Inflation();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows_DistrictHeat_Inflation();
  }
}

void Economics::cashFlowsDistrictCoolInflation( BuildingType buildingType, std::vector< double >& resArray )
{
  if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows_DistrictCool_Inflation();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows_DistrictCool_Inflation();
  }
}

void Economics::cashFlowsCapitalInflation( BuildingType buildingType, std::vector< double >& resArray )
{
  if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows_Capital_Inflation();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows_Capital_Inflation();
  }
}

void Economics::cashFlowsGasInflation( BuildingType buildingType, std::vector< double >& resArray )
{
 if(buildingType == btReference){
    resArray = m_refBuilding->cashFlows_Gas_Inflation();
  }
  else if(buildingType == btSelected){
    resArray = m_selBuilding->cashFlows_Gas_Inflation();
  }
}

void Economics::getElec( BuildingType buildingType, double& use, double& cost )
{
  switch ( buildingType )
  {
    case btReference:
    {
      use = m_refBuilding->elecUse();
      cost = m_refBuilding->elecCost();
    }
    break;
    case btSelected:
    {
      use = m_selBuilding->elecUse();
      cost = m_selBuilding->elecCost();
    }
    break;
  } //case
}

void Economics::getDistrictHeat( BuildingType buildingType, double& use, double& cost )
{
  switch ( buildingType )
  {
    case btReference:
    {
      use = m_refBuilding->districtHeatUse();
      cost = m_refBuilding->districtHeatCost();
    }
    break;
    case btSelected:
    {
      use = m_selBuilding->districtHeatUse();
      cost = m_selBuilding->districtHeatCost();
    }
    break;
  } //case
}

void Economics::getDistrictCool( BuildingType buildingType, double& use, double& cost )
{
  switch ( buildingType )
  {
    case btReference:
    {
      use = m_refBuilding->districtCoolUse();
      cost = m_refBuilding->districtCoolCost();
    }
    break;
    case btSelected:
    {
      use = m_selBuilding->districtCoolUse();
      cost = m_selBuilding->districtCoolCost();
    }
    break;
  } //case
}

std::shared_ptr<CostObject> Economics::getBuildingObject( BuildingType buildingType )
{
  std::shared_ptr<CostObject> result;
  switch ( buildingType )
  {
    case btReference:
      result = m_refBuilding;
    break;
    case btSelected:
      result = m_selBuilding;
    break;
  } //case
  return result;
}

double Economics::getCapitalCost( BuildingType buildingType )
{
  double result = 0.0;
  switch ( buildingType )
  {
    case btReference:
    {
      if ( m_refBuilding->cashFlows().size() > 0 ) 
        result = - m_refBuilding->cashFlows().at(0);
      else
        result = 0;
    }
    break; //begin

    case btSelected:
    {
      if ( m_selBuilding->cashFlows().size() > 0 ) 
        result = - m_selBuilding->cashFlows().at(0);
      else
        result = 0;
    }
    break; //begin
  } //case
  return result;
}

double Economics::getEnergyCost( BuildingType buildingType )
{
  double result = 0.0;
  switch ( buildingType )
  {
    case btReference:
      result = m_refBuilding->elecCost() + m_refBuilding->gasCost();
    break;
    case btSelected:
      result = m_selBuilding->elecCost() + m_selBuilding->gasCost();
    break;
  } //case
  return result;
}

double Economics::getCapitalAnalysisEnergyCost( BuildingType buildingType )
{
  double result = 0.0;
  switch ( buildingType )
  {
    case btReference:
    {
      if ( m_refBuilding->cashFlows().size() > 0 ) 
        result = - m_refBuilding->cashFlows().at(0) + m_analysisPeriod * ( m_refBuilding->elecCost() + m_refBuilding->gasCost() );
      else
        result = 0;
    }
    break; //begin

    case btSelected:
    {
      if ( m_selBuilding->cashFlows().size() > 0 ) 
        result = - m_selBuilding->cashFlows().at(0) + m_analysisPeriod * ( m_selBuilding->elecCost() + m_selBuilding->gasCost() );
      else
        result = 0;
    }
    break; //begin
  } //case
  return result;
}

double Economics::getSum( const std::vector<double>& doubles )
{
  double sum = 0;
  for(unsigned i=0; i<doubles.size(); i++){
    sum += doubles.at(i);
  }
  return sum;
}

double Economics::getSPVFactor( double rate, double periods )
{
  return 1/pow(1+rate,periods);
}

double Economics::getUPVFactor( double rate, double periods )
{
  return (pow(1+rate,periods) - 1)/(rate * pow(1+rate,periods));
}

double Economics::getFV( double rate, double periods, double presentValue )
{
  return presentValue * pow(1+rate,periods);
}

double Economics::getTLCC( BuildingType buildingType )
{
  double result = 0.0;
  switch ( buildingType )
  {
    case btReference:
    {
      if ( m_refBuilding->cashFlows_Inflation().size() > 0 ) 
        result = - getSum( m_refBuilding->cashFlows_Inflation() );
      else
        result = 0;
    }
    break; //begin

    case btSelected:
    {
      if ( m_selBuilding->cashFlows_Inflation().size() > 0 ) 
        result = - getSum( m_selBuilding->cashFlows_Inflation() );
      else
        result = 0;
    }
    break; //begin
  } //case
  return result;
}

double Economics::getVirtualRate( BuildingType buildingType, VirtualRateType virtualRateType )
{
  double result = - 9999;
  if ( buildingType == btReference ) 
    /* # with m_refBuilding do */
    {
      switch ( virtualRateType )
      {
        case vrElec:
          if ( m_refBuilding->elecUse() != 0 ) 
            result = m_refBuilding->elecCost() / m_refBuilding->elecUse();
        break;
        case vrGas:
          if ( m_refBuilding->gasUse() != 0 ) 
            result = m_refBuilding->gasCost() / m_refBuilding->gasUse();
        break;
        case vrCombined:
          if ( ( m_refBuilding->elecUse() + m_refBuilding->gasUse() ) != 0 ) 
            result = ( ( m_refBuilding->gasCost() + m_refBuilding->elecCost() ) / ( m_refBuilding->gasUse() + m_refBuilding->elecUse() ) );
        break;
      } //case
    }
  else
    /* # with m_selBuilding do */
    {
      switch ( virtualRateType )
      {
        case vrElec:
          if ( m_selBuilding->elecUse() != 0 ) 
            result = m_selBuilding->elecCost() / m_selBuilding->elecUse();
        break;
        case vrGas:
          if ( m_selBuilding->gasUse() != 0 ) 
            result = m_selBuilding->gasCost() / m_selBuilding->gasUse();
        break;
        case vrCombined:
          if ( ( m_selBuilding->elecUse() + m_selBuilding->gasUse() ) != 0 ) 
            result = ( ( m_selBuilding->gasCost() + m_selBuilding->elecCost() ) / ( m_selBuilding->gasUse() + m_selBuilding->elecUse() ) );
        break;
      } //case
    } //begin
  return result;
}

double Economics::levelizedCostOfEnergy( const double TLCC, LevelizationType levelOn )
{
  double result = 0.0;
  double dResult = 0.0;
  //LCOE pg48 equation 4-7 Walter Short   -- todo: cost savings -
  //Uniform Capital Recovery Factor
  double UCRF = ( m_discountRate * ( pow( 1 + m_discountRate, m_analysisPeriod ) ) ) / ( pow( 1 + m_discountRate, m_analysisPeriod ) - 1 );
  if ( levelOn == ltCost ) 
  {
    if ( m_totalEnergyCostSavings != 0 ) 
      dResult = ( TLCC / m_totalEnergyCostSavings ) * UCRF;
    else
      dResult = 0;
  }
  else
    if ( levelOn == ltEnergy ) 
    {
      if ( m_totalEnergyUseSavings != 0 ) 
        dResult = ( TLCC / m_totalEnergyUseSavings ) * UCRF;
      else
        dResult = 0;
    }//not envery
  result = dResult;
  return result;
}

void Economics::setOMInflation( BuildingType buildingType, OptionalDouble inflation )
{
  double _inflation = 0;

  if(inflation) _inflation = *inflation;

  setOMInflation( buildingType, _inflation );
}

void Economics::setOMInflation( BuildingType buildingType, double OMInflation )
{
  switch ( buildingType )
  {
    case btReference:
      m_refBuilding->setOMInflation(OMInflation);
    break;
    case btSelected:
      m_selBuilding->setOMInflation(OMInflation);
    break;
  } //case
}

void Economics::setInflation( BuildingType buildingType, OptionalDouble inflation )
{
  double _inflation = 0;

  if(inflation) _inflation = *inflation;

  setInflation( buildingType, _inflation );
}

void Economics::setInflation( BuildingType buildingType, double inflation )
{
  switch ( buildingType )
  {
    case btReference:
      m_refBuilding->setInflation(inflation);
    break;
    case btSelected:
      m_selBuilding->setInflation(inflation);
    break;
  } //case
}

double Economics::getPayBack( const std::vector< double >& cashFlowSavings )
{
  double result = 0;
  if(cashFlowSavings.size() >= 2){
    if(cashFlowSavings[1] != 0){
      result = cashFlowSavings[0] / cashFlowSavings[1];
    }
  }
  return fabs(result);
}

double Economics::payBack( const std::vector< double >& cashFlowSavings )
{
  //this calculates payback.  warning... if the expected life or
  //the equipment isn't symmetrical, then the SPB may be longer than required.
  double result = 0.0;
  double diff = 0.0;
  double iResult = 0.0;
  bool bFound = false;
  double fracYr = 0.0;
  int iIter = 0;
  int PAYBACK_ITER_MAX = 20;
  iIter = 1;
  while ( ( ! bFound ) && ( iIter < PAYBACK_ITER_MAX ) ) 
  {
    for (unsigned iYear = 0; iYear < cashFlowSavings.size(); iYear++)
    {
      if ( iIter == 1 ) 
      {
        diff = diff + cashFlowSavings.at(iYear);
      }
      else
      {
        //skip the initial capital for the iterated years
        if ( iYear != 0 ) 
          diff = diff + cashFlowSavings.at(iYear);
      }
      if ( diff > 0 ) 
      {
        if ( cashFlowSavings.at(iYear) != 0 ) 
          fracYr = diff / cashFlowSavings.at(iYear);
        else
          fracYr = 0;
        if ( iYear == 0 ) 
        {
          iResult = ( 1 - fracYr );  //this is ignored if iIter > 1
        }
        else
        {
          iResult = ( ( iIter - 1 ) * ( cashFlowSavings.size() - 1 ) ) + ( iYear ) - fracYr;
        }
        bFound = true;
        break;
      } //if
    } //for
    iIter++;
  } //while not
  if ( bFound ) 
    result = iResult;
  else
    result = 9999;
  return result;
}

void Economics::setDiscountRate( OptionalDouble discountRate )
{
  double _discountRate = 0;

  if(discountRate) _discountRate = *discountRate;

  setDiscountRate( _discountRate );
}

void Economics::setDiscountRate( const double discountRate )
{
  if( m_discountRate != discountRate ) 
  {
    m_discountRate = discountRate;
  }
  //also set the cost objects discount rates
  m_refBuilding->setDiscountRate(discountRate);
  m_selBuilding->setDiscountRate(discountRate);
}

bool Economics::getIRR( const std::vector<double>& cashFlows, double& guess  )
{
  bool success = false;
  int count = 0;

  //cashFlows first element must be positive, and cashFlows must have at least 2 elements
  if(cashFlows.size() < 2 || (cashFlows.at(0) > 0)) return success;

  // make init guess
  guess = -1*(1 + (cashFlows.at(1)/cashFlows.at(0)));

  success = recursiveNewtonCalc(cashFlows,guess,count);

  if(guess > 1) return success = false;

  return success;
}

bool Economics::recursiveNewtonCalc( const std::vector<double>& cashFlows, double& guess, int& count )
{
  bool success = false;

  if(count >= MAX_ITERATIONS) return success;

  if( (guess == -1) || (guess >= MAX_LIMIT) || (guess <= -MAX_LIMIT) ) return success;

  count++;

  double polynomialSum = 0;
  for (unsigned i = 0; i < cashFlows.size(); i++){
    polynomialSum += cashFlows.at(i)/(pow((1 + guess), static_cast<int>(i)));
  }

  if(fabs(polynomialSum) <= MAX_ERR){
    return success = true;
  };

  double derivativeSum = 0;
  for (unsigned i=0; i<cashFlows.size(); i++){
    derivativeSum += cashFlows.at(i)*i/pow(1 + guess, static_cast<int>(i));
  }
  derivativeSum *= -1;

  if(derivativeSum == 0) return success;

  guess -= polynomialSum/derivativeSum;

  if(recursiveNewtonCalc(cashFlows,guess,count)) success = true;

  return success;
}

void Economics::setCashFlowsInflation( BuildingType buildingType, const std::vector< double >& cashFlows )
{
  switch ( buildingType )
  {
    case btReference:
      m_refBuilding->setCashFlows_Inflation(cashFlows);
    break;
    case btSelected:
      m_selBuilding->setCashFlows_Inflation(cashFlows);
    break;
  } //case
}

void Economics::setAnalysisPeriod( OptionalInt analysisPeriod)
{
  if(analysisPeriod){
    setAnalysisPeriod(*analysisPeriod);
  }
  else{
    setAnalysisPeriod();
  }
}
  
void Economics::setAnalysisPeriod( const int analysisPeriod )
{
  m_analysisPeriod = analysisPeriod;
}


void Economics::setStdRetrofit( const bool stdRetrofit)
{
  m_stdRetrofit = stdRetrofit;
}

void Economics::setUsefulLife(const int usefulLife)
{
  m_usefulLife = usefulLife;
}

void Economics::setUsefulLifeExisting(const int usefulLifeExisting)
{
  m_usefulLifeExisting = usefulLifeExisting;
}

void Economics::setReplacementCostCodeMinimum(const double replacementCostCodeMinimum)
{
  m_replacementCostCodeMinimum = replacementCostCodeMinimum;
}

void Economics::setRealDiscountFactor( const double realDiscountFactor)
{
  m_realDiscountFactor = realDiscountFactor;
}

void Economics::setRealElectricityPriceEscalationRate( const double realElectricityPriceEscalationRate)
{
  m_realElectricityPriceEscalationRate = realElectricityPriceEscalationRate;
}

void Economics::setRealGasPriceEscalationRate( const double realGasPriceEscalationRate)
{
  m_realGasPriceEscalationRate = realGasPriceEscalationRate;
}

void Economics::setFederalCorporateTaxRate( const double federalCorporateTaxRate)
{
  m_federalCorporateTaxRate = federalCorporateTaxRate;
}

void Economics::setPurchaseCost(const double purchaseCost)
{
  m_purchaseCost = purchaseCost;
}

void Economics::setInstallationCost(const double installationCost)
{
  m_installationCost = installationCost;
}

//void setTotalCapitalCost(const double totalCapitalCost){}

void Economics::setSalvageValueExisting( const double salvageValueExisting)
{
  m_salvageValueExisting = salvageValueExisting;
}

void Economics::setDisposalCost( const double disposalCost)
{
  m_disposalCost = disposalCost;
}

void Economics::setFixedOM( const double fixedOM)
{
  m_fixedOM = fixedOM;
}

void Economics::setFixedMV( const double fixedMV)
{
  m_fixedMV = fixedMV;
}

void Economics::setReplacementCost( const double replacementCost)
{
  m_replacementCost = replacementCost;
}

void Economics::setElectricityCostSavings( const double electricityCostSavings)
{
  m_electricityCostSavings = electricityCostSavings;
}

void Economics::setGasCostSavings( const double gasCostSavings)
{
  m_gasCostSavings = gasCostSavings;
}

void Economics::setElectricityCostSavingsCodeMinimum( const double electricityCostSavingsCodeMinimum)
{
  m_electricityCostSavingsCodeMinimum = electricityCostSavingsCodeMinimum;
}

void Economics::setGasCostSavingsCodeMinimum( const double gasCostSavingsCodeMinimum)
{
  m_gasCostSavingsCodeMinimum = gasCostSavingsCodeMinimum;
}

void Economics::setSpecialTaxDeductions( const double specialTaxDeductions)
{
  m_specialTaxDeductions = specialTaxDeductions;
}

void Economics::setSpecialTaxCredits( const double specialTaxCredits)
{
  m_specialTaxCredits = specialTaxCredits;
}

void Economics::setRebates( const double rebates)
{
  m_rebates = rebates;
}

void Economics::setDepreciationTaxDeduction( const double depreciationTaxDeduction)
{
  m_depreciationTaxDeduction = depreciationTaxDeduction;
}

void Economics::setOtherFinancialIncentives( const double otherFinancialIncentives)
{
  m_otherFinancialIncentives = otherFinancialIncentives;
}

void Economics::setWholeBldFinancialIncentives( const double wholeBldFinancialIncentives)
{
  m_wholeBldFinancialIncentives = wholeBldFinancialIncentives;
}

void Economics::setProjectPlanningAndDesign( const double projectPlanningAndDesign)
{
  m_projectPlanningAndDesign = projectPlanningAndDesign;
}

void Economics::setPackageMV( const double packageMV)
{
  m_packageMV = packageMV;
}

void Economics::setVariables(const bool stdRetrofit,
                             const int analysisPeriod,
                             const double realDiscountFactor,
                             const double realElectricityPriceEscalationRate,
                             const double realGasPriceEscalationRate,
                             const double federalCorporateTaxRate,
                             const int usefulLife,
                             const double purchaseCost,
                             const double installationCost,
                             const double salvageValueExisting,
                             const double disposalCost,
                             const double fixedOM,
                             const double fixedMV,
                             const double replacementCost,
                             const double electricityCostSavings,
                             const double gasCostSavings,
                             const double electricityCostSavingsCodeMinimum,
                             const double gasCostSavingsCodeMinimum,
                             const double specialTaxDeductions,
                             const double specialTaxCredits,
                             const double rebates,
                             const double depreciationTaxDeduction,
                             const double otherFinancialIncentives,
                             const double wholeBldFinancialIncentives,
                             const double projectPlanningAndDesign,
                             const double packageMV,
                             const int usefulLifeExisting,
                             const double replacementCostCodeMinimum)
{
  setStdRetrofit(stdRetrofit);
  setAnalysisPeriod(analysisPeriod);
  setRealDiscountFactor(realDiscountFactor);
  setRealElectricityPriceEscalationRate(realElectricityPriceEscalationRate);
  setRealGasPriceEscalationRate(realGasPriceEscalationRate);
  setFederalCorporateTaxRate(federalCorporateTaxRate);
  setUsefulLife(usefulLife);
  setPurchaseCost(purchaseCost);
  setInstallationCost(installationCost);
  setSalvageValueExisting(salvageValueExisting);
  setDisposalCost(disposalCost);
  setFixedOM(fixedOM);
  setFixedMV(fixedMV);
  setReplacementCost(replacementCost);
  setElectricityCostSavings(electricityCostSavings);
  setGasCostSavings(gasCostSavings);
  setElectricityCostSavingsCodeMinimum(electricityCostSavingsCodeMinimum);
  setGasCostSavingsCodeMinimum(gasCostSavingsCodeMinimum);
  setSpecialTaxDeductions(specialTaxDeductions);
  setSpecialTaxCredits(specialTaxCredits);
  setRebates(rebates);
  setDepreciationTaxDeduction(depreciationTaxDeduction);
  setOtherFinancialIncentives(otherFinancialIncentives);
  setWholeBldFinancialIncentives(wholeBldFinancialIncentives);
  setProjectPlanningAndDesign(projectPlanningAndDesign);
  setPackageMV(packageMV);
  setUsefulLifeExisting(usefulLifeExisting);
  setReplacementCostCodeMinimum(replacementCostCodeMinimum);

  m_variablesSet = true;
}

void Economics::calcVariables()
{
  m_totalCapitalCost = m_purchaseCost + m_installationCost;

  if(m_stdRetrofit){
    m_remainingUsefulLifeExisting = static_cast<int>(m_usefulLifeExisting/2+0.5);
  }
  else{
    m_remainingUsefulLifeExisting = 0;
  }

  if(m_usefulLifeExisting == 0){
    m_undepreciatedValueExisting = 0;
  }
  else{
    m_undepreciatedValueExisting = m_replacementCostCodeMinimum*m_remainingUsefulLifeExisting/m_usefulLifeExisting;
  }

  m_taxDeduction = (m_undepreciatedValueExisting-m_salvageValueExisting)*m_federalCorporateTaxRate;

  if(m_remainingUsefulLifeExisting == 0 || m_realDiscountFactor ==0 || pow(1+m_realDiscountFactor,m_remainingUsefulLifeExisting) == 0){
    m_foregoneTaxDeduction = 0;
  }
  else{
    m_foregoneTaxDeduction = m_undepreciatedValueExisting/static_cast<double>(m_remainingUsefulLifeExisting)/m_realDiscountFactor*(1-1/pow(1+m_realDiscountFactor,m_remainingUsefulLifeExisting))*m_federalCorporateTaxRate;
  }

  if(m_usefulLife) m_remainingUsefulLifeOfMeasure = m_usefulLife - m_analysisPeriod % m_usefulLife;

  if(m_usefulLifeExisting) m_remainingUsefulLifeProjectEnd = m_usefulLifeExisting - (m_analysisPeriod - m_remainingUsefulLifeExisting) % m_usefulLifeExisting;

  if(m_usefulLifeExisting ==0){
    m_salvageValue = 0;
  }
  else{
    if(m_remainingUsefulLifeExisting <= m_analysisPeriod){
      m_salvageValue = static_cast<double>(m_remainingUsefulLifeProjectEnd)/m_usefulLifeExisting*m_replacementCostCodeMinimum;
    }
    else{
      m_salvageValue = 0;
    }
  }

  if(m_usefulLife ==0){
    m_salvageValueMeasure = 0;
  }
  else{
    if(m_usefulLife <= m_analysisPeriod){
      m_salvageValueMeasure = static_cast<double>(m_remainingUsefulLifeOfMeasure)/m_usefulLife*m_replacementCost;
    }
    else{
      m_salvageValueMeasure = static_cast<double>(m_remainingUsefulLifeOfMeasure)/m_usefulLife*m_totalCapitalCost;
    }
  }
}

void Economics::calcCashFlows( std::vector<double>& cashFlows, const bool includeEnergySavings)
{
  cashFlows.clear();

  for(int year=0; year<=m_analysisPeriod; year++){
    double cashFlow = 0;
    if(year == 0){
      cashFlow += -m_totalCapitalCost;
      if(m_remainingUsefulLifeExisting == 0){
        cashFlow += m_replacementCostCodeMinimum;
      }
      cashFlow += m_salvageValueExisting;
      cashFlow += m_taxDeduction;
      cashFlow -= m_foregoneTaxDeduction;
      cashFlow -= m_disposalCost*(1-m_federalCorporateTaxRate);
      cashFlow -= m_wholeBldFinancialIncentives;
      cashFlow -= m_projectPlanningAndDesign*(1-m_federalCorporateTaxRate);
      cashFlow += m_specialTaxDeductions;
      cashFlow += m_specialTaxCredits;
      cashFlow += m_rebates;
      cashFlow += m_depreciationTaxDeduction;
      cashFlow += m_otherFinancialIncentives;
    }
    else{
      if(m_usefulLife && (year % m_usefulLife == 0)){
        cashFlow -= m_replacementCost;
      }
      if(year == m_remainingUsefulLifeExisting){
        cashFlow += m_replacementCostCodeMinimum;
      }
      else if(m_usefulLifeExisting && ((year - m_remainingUsefulLifeExisting) % m_usefulLifeExisting == 0)){
        cashFlow += m_replacementCostCodeMinimum;
      }
      if(includeEnergySavings){
        if(year<=m_remainingUsefulLifeExisting){
          cashFlow += m_electricityCostSavings*(1-m_federalCorporateTaxRate)*pow(1+m_realElectricityPriceEscalationRate,year)+m_gasCostSavings*(1-m_federalCorporateTaxRate)*pow(1+m_realGasPriceEscalationRate,year);
        }
        else{
          cashFlow += m_electricityCostSavingsCodeMinimum*(1-m_federalCorporateTaxRate)*pow(1+m_realElectricityPriceEscalationRate,year)+m_gasCostSavingsCodeMinimum*(1-m_federalCorporateTaxRate)*pow(1+m_realGasPriceEscalationRate,year);
        }
      }
      cashFlow -= m_fixedOM*(1-m_federalCorporateTaxRate);
      cashFlow += m_fixedMV*(1-m_federalCorporateTaxRate)-m_packageMV*(1-m_federalCorporateTaxRate);
      if(year == m_analysisPeriod){
        cashFlow += m_salvageValueMeasure-m_salvageValue;
      }
    }
    cashFlows.push_back(cashFlow);
  }
  OS_ASSERT(cashFlows.size() == static_cast<size_t>(m_analysisPeriod + 1));
}

std::vector<double> Economics::getCashFlows(const bool includeEnergySavings)
{
  std::vector<double> cashFlows;

  if(!m_variablesSet) return cashFlows;
  m_variablesSet = false;

  calcVariables();
  calcCashFlows(cashFlows,includeEnergySavings);

  return cashFlows;
}

}
