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

#include <math.h>
#include "CostLineItem.hpp"

#include "CostObject.hpp"

namespace openstudio{

CostObject::CostObject():
  m_discountRate(0),
  m_districtHeatInflation(0),
  m_districtHeatUse(0),
  m_districtHeatCost(0),
  m_districtCoolInflation(0),
  m_districtCoolUse(0),
  m_districtCoolCost(0),
  m_elecCost(0),
  m_elecUse(0),
  m_elecInflation(0),
  m_gasCost(0),
  m_gasUse(0),
  m_gasInflation(0),
  m_OMInflation(0),
  m_inflation(0)
{
}

CostObject::~CostObject()
{
}

void CostObject::addCostInstance( std::string name, double matCost, double installCost, double fixedOM, double varOM, int expectedLife, double salvageCost )
{
  int iIndex = -1;
  for(unsigned i=0; i<m_RefCostObjsData.size(); i++){
    if(m_RefCostObjsData.at(i)->name() == name){
      iIndex = i;
      break;
    }
  }

  std::shared_ptr<CostLineItem> costLineItem = std::shared_ptr<CostLineItem>(new CostLineItem);
  if ( iIndex < 0 ) 
  {
    costLineItem->setName(name);
    costLineItem->setMatCost(matCost);
    costLineItem->setInstallCost(installCost);
    costLineItem->setFixedOM(fixedOM);
    costLineItem->setVariableOM(varOM);
    costLineItem->setExpectedLife(expectedLife);
    costLineItem->setSalvageCost(salvageCost);
    m_RefCostObjsData.push_back(costLineItem);
  }
  else
  {
    costLineItem = m_RefCostObjsData.at(iIndex);
    costLineItem->setMatCost(costLineItem->matCost()+ matCost);
    costLineItem->setInstallCost(costLineItem->installCost() + installCost);
    costLineItem->setFixedOM(costLineItem->fixedOM() + fixedOM);
    costLineItem->setVariableOM(costLineItem->variableOM() + varOM);
    costLineItem->setSalvageCost(costLineItem->salvageCost() + salvageCost);
  }
}

void CostObject::calculateCashFlows( int analysisPeriod )
{
  m_cashFlows.clear(); //clear out result
  m_cashFlows_Inflation.clear();
  m_cashFlows_Gas_Inflation.clear();
  m_cashFlows_Elec_Inflation.clear();
  m_cashFlows_DistrictHeat_Inflation.clear();
  m_cashFlows_DistrictCool_Inflation.clear();
  m_cashFlows_Capital_Inflation.clear();

  m_cashFlows.resize(analysisPeriod + 1);
  m_cashFlows_Inflation.resize(analysisPeriod + 1);
  m_cashFlows_Gas_Inflation.resize(analysisPeriod + 1);
  m_cashFlows_Elec_Inflation.resize(analysisPeriod + 1);
  m_cashFlows_DistrictHeat_Inflation.resize(analysisPeriod + 1);
  m_cashFlows_DistrictCool_Inflation.resize(analysisPeriod + 1);
  m_cashFlows_Capital_Inflation.resize(analysisPeriod + 1);

  for (int iYear = 0; iYear <= analysisPeriod; iYear++) //year 0 is initial capital
  {
    double PVIF = double( 1 ) / ( pow( 1 + m_discountRate, iYear ) );
    for (unsigned j = 0; j < m_RefCostObjsData.size(); j++)
    {
      if ( iYear == 0 ) 
      {
        m_cashFlows.at(iYear) = m_cashFlows.at(iYear) - m_RefCostObjsData.at(j)->matCost() - m_RefCostObjsData.at(j)->installCost();
        m_cashFlows_Inflation.at(iYear) = m_cashFlows_Inflation.at(iYear) - PVIF * ( ( pow( 1 + m_inflation, iYear ) * ( m_RefCostObjsData.at(j)->matCost() + m_RefCostObjsData.at(j)->installCost() ) ) );
        m_cashFlows_Capital_Inflation.at(iYear) = m_cashFlows_Inflation.at(iYear) - PVIF * ( 0 );
      }
      else
      {
        // TODO / NOTE : need to determine why there is a J==0 case.  Was this specific to the original OptEPlus code for a dummy placeholder for metadata?
        // In this application, Matt L. had only 1 costinstance and need O & M data.  He had to gen a dummy first costinstance to get valid results specific
        // to fixed O & M.
        if ( j == 0 ) 
        {
          //add energy cost
          m_cashFlows.at(iYear) = m_cashFlows.at(iYear) - m_elecCost - m_gasCost - m_districtHeatCost - m_districtCoolCost;
          m_cashFlows_Inflation.at(iYear) = m_cashFlows_Inflation.at(iYear) - PVIF * ( ( pow( 1 + m_elecInflation, iYear ) * ( m_elecCost ) ) + ( pow( 1 + m_gasInflation, iYear ) * ( m_gasCost ) ) + ( pow( 1 + m_districtHeatInflation, iYear ) * ( m_districtHeatCost ) ) + ( pow( 1 + m_districtCoolInflation, iYear ) * ( m_districtCoolCost ) ) );

          //these are used for graphing the results in the detailed results browser
          m_cashFlows_Gas_Inflation.at(iYear) = - PVIF * ( pow( 1 + m_gasInflation, iYear ) * ( m_gasCost ) );
          m_cashFlows_Elec_Inflation.at(iYear) = - PVIF * ( pow( 1 + m_elecInflation, iYear ) * ( m_elecCost ) );
          m_cashFlows_DistrictHeat_Inflation.at(iYear) = - PVIF * ( pow( 1 + m_districtHeatInflation, iYear ) * ( m_districtHeatCost ) );
          m_cashFlows_DistrictCool_Inflation.at(iYear) = - PVIF * ( pow( 1 + m_districtCoolInflation, iYear ) * ( m_districtCoolCost ) );
        }
        else
        {
          m_cashFlows.at(iYear) = m_cashFlows.at(iYear) - m_RefCostObjsData.at(j)->fixedOM() - m_RefCostObjsData.at(j)->variableOM();
          m_cashFlows_Inflation.at(iYear) = m_cashFlows_Inflation.at(iYear) - PVIF * ( ( pow( 1 + m_OMInflation, iYear ) * ( m_RefCostObjsData.at(j)->fixedOM() + m_RefCostObjsData.at(j)->variableOM() ) ) );
          m_cashFlows_Capital_Inflation.at(iYear) = m_cashFlows_Capital_Inflation.at(iYear) - PVIF * ( ( pow( 1 + m_OMInflation, iYear ) * ( m_RefCostObjsData.at(j)->fixedOM() + m_RefCostObjsData.at(j)->variableOM() ) ) );
        }
        if ( m_RefCostObjsData.at(j)->expectedLife() != 0 ) 
        {
          if ( ( iYear % m_RefCostObjsData.at(j)->expectedLife() == 0 ) && ( analysisPeriod != iYear ) ) 
          {
            m_cashFlows.at(iYear) = m_cashFlows.at(iYear) + m_RefCostObjsData.at(j)->salvageCost() - m_RefCostObjsData.at(j)->matCost() - m_RefCostObjsData.at(j)->installCost();
            m_cashFlows_Inflation.at(iYear) = m_cashFlows_Inflation.at(iYear) + PVIF * ( ( pow( 1 + m_inflation, iYear ) * ( m_RefCostObjsData.at(j)->salvageCost() ) ) - ( pow( 1 + m_inflation, iYear ) * ( m_RefCostObjsData.at(j)->matCost() + m_RefCostObjsData.at(j)->installCost() ) ) );
            m_cashFlows_Capital_Inflation.at(iYear) = m_cashFlows_Capital_Inflation.at(iYear) + PVIF * ( ( pow( 1 + m_inflation, iYear ) * ( m_RefCostObjsData.at(j)->salvageCost() ) ) - ( pow( 1 + m_inflation, iYear ) * ( m_RefCostObjsData.at(j)->matCost() + m_RefCostObjsData.at(j)->installCost() ) ) );
          }
        } //if expectedlife != 0
      }
    } //for j
  } //for iYear
}

double CostObject::firstCosts()
{
  double dFirstCosts = 0.0;
  for (unsigned i=0; i<m_RefCostObjsData.size(); i++){
    dFirstCosts = dFirstCosts + m_RefCostObjsData.at(i)->matCost() + m_RefCostObjsData.at(i)->installCost();
  }
  return dFirstCosts;
}

std::vector< double > CostObject::cashFlows()
{
  return m_cashFlows;
}

std::vector< double > CostObject::cashFlows_Inflation()
{
  return m_cashFlows_Inflation;
}

std::vector< double > CostObject::cashFlows_Gas_Inflation()
{
  return m_cashFlows_Gas_Inflation;
}

std::vector< double > CostObject::cashFlows_Elec_Inflation()
{
  return m_cashFlows_Elec_Inflation;
}

std::vector< double > CostObject::cashFlows_DistrictHeat_Inflation()
{
  return m_cashFlows_DistrictHeat_Inflation;
}

std::vector< double > CostObject::cashFlows_DistrictCool_Inflation()
{
  return m_cashFlows_DistrictCool_Inflation;
}

std::vector< double > CostObject::cashFlows_Capital_Inflation()
{
  return m_cashFlows_Capital_Inflation;
}

void CostObject::setCashFlows(const std::vector< double >& cashFlows)
{
  m_cashFlows = cashFlows;
}

void CostObject::setCashFlows_Inflation(const std::vector< double >& cashFlows)
{
  m_cashFlows_Inflation = cashFlows;
}

void CostObject::setCashFlows_Gas_Inflation(const std::vector< double >& cashFlows){
  m_cashFlows_Gas_Inflation = cashFlows;
}

void CostObject::setCashFlows_Elec_Inflation(const std::vector< double >& cashFlows){
  m_cashFlows_Elec_Inflation = cashFlows;
}

void CostObject::setCashFlows_DistrictHeat_Inflation(const std::vector< double >& cashFlows){
  m_cashFlows_DistrictHeat_Inflation = cashFlows;
}

void CostObject::setCashFlows_DistrictCool_Inflation(const std::vector< double >& cashFlows){
  m_cashFlows_DistrictCool_Inflation = cashFlows;
}

void CostObject::setCashFlows_Capital_Inflation(const std::vector< double >& cashFlows){
  m_cashFlows_Capital_Inflation = cashFlows;
}

double CostObject::discountRate()
{
  return m_discountRate;
}

double CostObject::districtHeatInflation()
{
  return m_districtHeatInflation;
}

double CostObject::districtHeatUse()
{
  return m_districtHeatUse;
}

double CostObject::districtHeatCost()
{
  return m_districtHeatCost;
}

double CostObject::districtCoolInflation()
{
  return m_districtCoolInflation;
}

double CostObject::districtCoolUse()
{
  return m_districtCoolUse;
}

double CostObject::districtCoolCost()
{
  return m_districtCoolCost;
}

double CostObject::elecCost()
{
  return m_elecCost;
}

double CostObject::elecUse()
{
  return m_elecUse;
}

double CostObject::elecInflation()
{
  return m_elecInflation;
}

double CostObject::gasCost()
{
  return m_gasCost;
}

double CostObject::gasUse()
{
  return m_gasUse;
}

double CostObject::gasInflation()
{
  return m_gasInflation;
}

double CostObject::OMInflation()
{
  return m_OMInflation;
}

double CostObject::inflation()
{
  return m_inflation;
}

void CostObject::setDiscountRate(double discountRate)
{
  m_discountRate = discountRate;
}

void CostObject::setDistrictHeatInflation(double districtHeatInflation)
{
  m_districtHeatInflation = districtHeatInflation;
}

void CostObject::setDistrictHeatUse(double districtHeatUse)
{
  m_districtHeatUse = districtHeatUse;
}

void CostObject::setDistrictHeatCost(double districtHeatCost)
{
  m_districtHeatCost = districtHeatCost;
}

void CostObject::setDistrictCoolInflation(double districtCoolInflation)
{
  m_districtCoolInflation = districtCoolInflation;
}

void CostObject::setDistrictCoolUse(double districtCoolUse)
{
  m_districtCoolUse = districtCoolUse;
}

void CostObject::setDistrictCoolCost(double districtCoolCost)
{
  m_districtCoolCost = districtCoolCost;
}

void CostObject::setElecCost(double elecCost)
{
  m_elecCost = elecCost;
}

void CostObject::setElecUse(double elecUse)
{
  m_elecUse = elecUse;
}

void CostObject::setElecInflation(double elecInflation)
{
  m_elecInflation = elecInflation;
}

void CostObject::setGasCost(double gasCost)
{
  m_gasCost = gasCost;
}

void CostObject::setGasUse(double gasUse)
{
  m_gasUse = gasUse;
}

void CostObject::setGasInflation(double gasInflation)
{
  m_gasInflation = gasInflation;
}

void CostObject::setOMInflation(double OMInflation)
{
  m_OMInflation = OMInflation;
}

void CostObject::setInflation(double inflation)
{
  m_inflation = inflation;
}

}
