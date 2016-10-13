/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
