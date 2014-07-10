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

#ifndef UTILITIES_ECONOMICS_COSTOBJECT_HPP
#define UTILITIES_ECONOMICS_COSTOBJECT_HPP

#include <memory>
#include <string>
#include <vector>
#include "../UtilitiesAPI.hpp"

namespace openstudio{

class CostLineItem;

class UTILITIES_API CostObject {

  friend class Economics;

public:
  CostObject();
  virtual ~CostObject();

protected:
  void addCostInstance( std::string name, double matCost, double installCost, double fixedOM, double varOM, int expectedLife, double salvageCost );
  void calculateCashFlows( int analysisPeriod );
  double firstCosts();

  std::vector< double > cashFlows();
  void setCashFlows(const std::vector< double >& cashFlows);
  std::vector< double > cashFlows_Capital_Inflation();
  void setCashFlows_Capital_Inflation(const std::vector< double >& cashFlows);
  std::vector< double > cashFlows_DistrictCool_Inflation();
  void setCashFlows_DistrictCool_Inflation(const std::vector< double >& cashFlows);
  std::vector< double > cashFlows_DistrictHeat_Inflation();
  void setCashFlows_DistrictHeat_Inflation(const std::vector< double >& cashFlows);
  std::vector< double > cashFlows_Elec_Inflation();
  void setCashFlows_Elec_Inflation(const std::vector< double >& cashFlows);
  std::vector< double > cashFlows_Gas_Inflation();
  void setCashFlows_Gas_Inflation(const std::vector< double >& cashFlows);
  std::vector< double > cashFlows_Inflation();
  void setCashFlows_Inflation(const std::vector< double >& cashFlows);

  double discountRate();
  void setDiscountRate(double discountRate);

  double districtCoolCost();
  void setDistrictCoolCost(double districtCoolCost);
  double districtCoolInflation();
  void setDistrictCoolInflation(double districtCoolInflation);
  double districtCoolUse();
  void setDistrictCoolUse(double districtCoolUse);

  double districtHeatCost();
  void setDistrictHeatCost(double districtHeatCost);
  double districtHeatInflation();
  void setDistrictHeatInflation(double districtHeatInflation);
  double districtHeatUse();
  void setDistrictHeatUse(double districtHeatUse);

  double elecCost();
  void setElecCost(double elecCost);
  double elecInflation();
  void setElecInflation(double elecInflation);
  double elecUse();
  void setElecUse(double elecUse);

  double gasCost();
  void setGasCost(double gasCost);
  double gasInflation();
  void setGasInflation(double gasInflation);
  double gasUse();
  void setGasUse(double gasUse);

  double inflation();
  void setInflation(double inflation);
  double OMInflation();
  void setOMInflation(double OMInflation);

private:
  double m_discountRate;
  double m_districtHeatInflation;
  double m_districtHeatUse;
  double m_districtHeatCost;
  double m_districtCoolInflation;
  double m_districtCoolUse;
  double m_districtCoolCost;
  double m_elecCost;
  double m_elecUse;
  double m_elecInflation;
  double m_gasCost;
  double m_gasUse;
  double m_gasInflation;
  double m_OMInflation;
  double m_inflation;

  std::vector < std::shared_ptr<CostLineItem> > m_RefCostObjsData;
  std::vector< double > m_cashFlows;
  std::vector< double > m_cashFlows_Inflation;
  std::vector< double > m_cashFlows_Gas_Inflation;
  std::vector< double > m_cashFlows_Elec_Inflation;
  std::vector< double > m_cashFlows_DistrictHeat_Inflation;
  std::vector< double > m_cashFlows_DistrictCool_Inflation;
  std::vector< double > m_cashFlows_Capital_Inflation;

};

}

#endif // UTILITIES_ECONOMICS_COSTOBJECT_HPP
