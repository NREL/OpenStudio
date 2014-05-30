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

#ifndef UTILITIES_ECONOMICS_ECONOMICS_HPP
#define UTILITIES_ECONOMICS_ECONOMICS_HPP

#include <memory>
#include <string>
#include <vector>
#include "../UtilitiesAPI.hpp"
#include "../core/Optional.hpp"

namespace openstudio{

class CostObject;

class UTILITIES_API Economics {

public:
  Economics ( );
  virtual ~Economics ( );

  enum BuildingType { btReference,
                    btSelected };
  enum LevelizationType { ltCost,
                          ltEnergy };
  enum VirtualRateType { vrCombined,
                         vrElec,
                         vrGas };

  void addCostInstance( BuildingType buildingType, std::string name, OptionalDouble matCost, OptionalDouble installCost, OptionalDouble fixedOM, OptionalDouble varOM, OptionalInt expectedLife, OptionalDouble salvageCost, OptionalDouble netArea );
  void addCostInstance( BuildingType buildingType, std::string name, double matCost, double installCost, double fixedOM, double varOM, int expectedLife, double salvageCost, double netArea );
  void addCostInstance( BuildingType buildingType, std::string name, double matCost, double installCost, double fixedOM, double varOM, int expectedLife, double salvageCost ) /* # overload */;
  void addDistrictCool( BuildingType buildingType, double use, double cost, double inflation );
  void addDistrictHeat( BuildingType buildingType, double use, double cost, double inflation );
  void addElectricity( BuildingType buildingType, OptionalDouble use, OptionalDouble cost, OptionalDouble inflation );
  void addElectricity( BuildingType buildingType, double use, double cost, double inflation );
  void addGas( BuildingType buildingType, OptionalDouble use, OptionalDouble cost, OptionalDouble inflation );
  void addGas( BuildingType buildingType, double use, double cost, double inflation );

  void calculateEconomics( );
  void calculateSingleEconomics( BuildingType buildingType );

  void cashFlows( BuildingType buildingType, std::vector< double >& resArray );
  void cashFlowsInflation( BuildingType buildingType, std::vector< double >& resArray );
  void cashFlowsGasInflation( BuildingType buildingType, std::vector< double >& resArray );
  void cashFlowsElecInflation( BuildingType buildingType, std::vector< double >& resArray );
  void cashFlowsDistrictHeatInflation( BuildingType buildingType, std::vector< double >& resArray );
  void cashFlowsDistrictCoolInflation( BuildingType buildingType, std::vector< double >& resArray );
  void cashFlowsCapitalInflation( BuildingType buildingType, std::vector< double >& resArray );

  double firstCosts( BuildingType buildingType );
  std::shared_ptr<openstudio::CostObject> getBuildingObject( BuildingType buildingType );
  double getCapitalAnalysisEnergyCost( BuildingType buildingType );
  double getCapitalCost( BuildingType buildingType );
  void getDistrictCool( BuildingType buildingType, double& use, double& cost );
  void getDistrictHeat( BuildingType buildingType, double& use, double& cost );
  void getElec( BuildingType buildingType, double& use, double& cost );
  double getEnergyCost( BuildingType buildingType );
  void getGas( BuildingType buildingType, double& use, double& cost );
  double getVirtualRate( BuildingType buildingType, VirtualRateType virtualRateType );

  double getFV( double rate, double periods, double presentValue );
  bool getIRR( const std::vector<double>& cashFlows, double& guess );
  double getSum( const std::vector<double>& doubles );
  double getSPVFactor( double rate, double periods ); // Single Present Value Factor
  double getUPVFactor( double rate, double periods ); // Uniform Present Value Factor
  double getTLCC( BuildingType buildingType ); // Total Life-Cycle Cost

  double simplePayback() {return m_SPB;} 
  double discountedPayback() {return m_DPB;} 
  double discountedPaybackTotalLifeCycleCost() {return m_DPB_TLCC;}
  double futureValue() {return m_FV;} 
  double internalRateOfReturn() {return m_IRR;} 
  double netPresentValue() {return m_NPV;}
  double totalLifeCycleCost() {return m_TLCC_Savings;} 

  void setCashFlowsInflation( BuildingType buildingType, const std::vector< double >& cashFlows );
  void setInflation( BuildingType buildingType, OptionalDouble inflation );
  void setInflation( BuildingType buildingType, double inflation );
  ///! OM = Operation, Maintenance
  void setOMInflation( BuildingType buildingType, OptionalDouble OMInflation );
  void setOMInflation( BuildingType buildingType, double OMInflation );
  void setDiscountRate( OptionalDouble discountRate );
  void setDiscountRate( const double discountRate );

  int analysisPeriod() {return m_analysisPeriod;}
  void setAnalysisPeriod( OptionalInt analysisPeriod );
  void setAnalysisPeriod( const int analysisPeriod = 30 );

  // This function adopts Excel spreadsheet methodology
  double getNPV(const double rate, const std::vector<double>& cashFlows);
  // This function adopts Excel spreadsheet methodology
  double getPayBack(const std::vector< double >& cashFlowSavings);
  std::vector<double> getCashFlows(const bool includeEnergySavings);
  void setVariables(const bool stdRetrofit,
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
                    const double replacementCostCodeMinimum);
  double realDiscountFactor() {return m_realDiscountFactor;}

private:
  int m_analysisPeriod;
  double m_capitalCostSavings;
  double m_energyCostSavings;
  double m_discountRate;
  double m_DPB; // Discounted Payback
  double m_DPB_TLCC; // Discounted Payback Total Life-Cycle Cost
  double m_gasCostSavings;
  double m_FV; // Future Value
  double m_elecCostSavings;
  double m_gasUseSavings;
  double m_elecUseSavings;
  double m_districtHeatUseSavings;
  double m_capitalAnalysisEnergyCostSavings;
  double m_districtCoolCostSavings;
  double m_districtCoolUseSavings;
  double m_districtHeatCostSavings;
  double m_virtualRate_Combined;
  ///! IRR assumes cash flows are reinvested at the IRR
  double m_IRR; // Internal Rate of Return
  double m_LCOE_Cost; // Levelized Cost of Electricity Cost
  double m_LCOE_Energy; // Levelized Cost of Electricity Energy
  double m_margIncomeTaxRate;
  ///!  Modified IRR assumes positive cash flows are reinvested at the cost of capital,
  ///! and that initial outlays are financed at the cost of financing 
  double m_NPV; // Net Present Value
  double m_SPB; // Simple Payback
  double m_TLCC_Savings; // Total Life-Cycle Cost
  double m_totalEnergyCostSavings;
  double m_totalEnergyUseSavings;

  // Standard or deep retrofit
  bool m_stdRetrofit;
  // Real discount factor
  double m_realDiscountFactor;
  // Real Electricity Price Escalation Rate
  double m_realElectricityPriceEscalationRate;
  // Real Gas Price Escalation Rate
  double m_realGasPriceEscalationRate;
  // Federal CorporateTaxRate
  double m_federalCorporateTaxRate;
  // Useful Life (Yrs) (LITERATURE)
  int m_usefulLife;
  // Purchase Cost (COST ESTIMATOR)
  double m_purchaseCost;
  // Installation Cost (COST ESTIMATOR)
  double m_installationCost;
  // Total Capital Cost (CALCULATED)
  double m_totalCapitalCost;
  // Salvage Value of Existing Equipment if Measure is Implemented (COST ESTIMATOR)
  double m_salvageValueExisting;
  // Undepreciated Value of Existing Equipment (CALCULATED)
  double m_undepreciatedValueExisting;
  // Tax Deduction due to Business Loss for Disposing of Existing Equipment (CALCULATED)
  double m_taxDeduction;
  // Foregone Tax Deduction for Not Depreciating Existing Equipment (NPV) if Measure is Implemented (CALCULATED)
  double m_foregoneTaxDeduction;
  // Disposal Cost of Existing Equipment if Measure is Implemented (COST ESTIMATOR)
  double m_disposalCost;
  // Additional Annual O&M for Measure Compared to Reference Case (COST ESTIMATOR)
  double m_fixedOM;
  // Additional Annual M&V for Measure Compared to Reference Case (LITERATURE)
  double m_fixedMV;
  // Replacement Cost for Measure at the End of Its Useful Life (Replace with Same Equipment) (COST ESTIMATOR)
  double m_replacementCost;
  // Remaining Useful Life of Measure at Project End (CALCULATED)
  int m_remainingUsefulLifeOfMeasure;
  // Salvage Value of Measure (Assumes Straight-Line Loss of Value) (CALCULATED)
  double m_salvageValueMeasure;
  // Annual Electricity Cost Savings vs Existing Equipment (SIMULATION)
  double m_electricityCostSavings;
  // Annual Electricity Cost Savings vs Code Minimum Replacement Equipment in Reference Building (SIMULATION)
  double m_electricityCostSavingsCodeMinimum; 
  // Annual Gas Cost Savings vs Code Minimum Replacement Equipment in Reference Building (SIMULATION)
  double m_gasCostSavingsCodeMinimum;
  // Special Tax Deductions (NPV) (LITERATURE)
  double m_specialTaxDeductions;
  // Special Tax Credits (NPV) (LITERATURE)
  double m_specialTaxCredits;
  // Rebates (NPV) (LITERATURE)
  double m_rebates;
  // Depreciation Tax Deduction (NPV) (LITERATURE)
  double m_depreciationTaxDeduction;
  // Other Financial Incentives (NPV) (LITERATURE)
  double m_otherFinancialIncentives;
  // Whole Bld Financial Incentives (NPV) (LITERATURE)
  double m_wholeBldFinancialIncentives;
  // Project Planning and Design (LITERATURE)
  double m_projectPlanningAndDesign;
  // Annual M&V for Package (LITERATURE)
  double m_packageMV;
  // Original Useful Life of Existing Equipment (Yrs) (LITERATURE)
  int m_usefulLifeExisting;
  // Remaining Useful Life of Existing Equipment at Year 0 (Yrs) (CALCULATED)
  int m_remainingUsefulLifeExisting;
  // Cost to Replace Existing Equipment with Code Minimum Equipment at End of Life (COST ESTIMATOR)
  double m_replacementCostCodeMinimum;
  // Salvage Value (CALCULATED)
  double m_salvageValue;
  // Remaining Useful Life at Project End (Year 20)
  int m_remainingUsefulLifeProjectEnd;
  // Have the above variables been set by the user?
  bool m_variablesSet;

  std::shared_ptr<CostObject> m_refBuilding;
  std::shared_ptr<CostObject> m_selBuilding;
  std::vector< double > m_cashFlowSavings;
  std::vector< double > m_discountedCashFlowSavings;
  
  void calculateCashFlowSavings( const std::vector< double >& refCashFlows, const std::vector< double >& selCashFlows );
  double levelizedCostOfEnergy( const double TLCC, LevelizationType levelOn );
  double payBack( const std::vector< double >& cashFlowSavings );
  bool recursiveNewtonCalc( const std::vector<double>& cashFlows, double& guess, int& count );

  bool stdRetrofit() {return m_stdRetrofit;}
  void setStdRetrofit( const bool stdRetrofit);
  int usefulLife() {return m_usefulLife;}
  void setUsefulLife(const int usefulLife);
  int usefulLifeExisting() {return m_usefulLifeExisting;}
  void setUsefulLifeExisting(const int usefulLifeExisting);
  void setRealDiscountFactor( const double realDiscountFactor);
  double realElectricityPriceEscalationRate() {return m_realElectricityPriceEscalationRate;}
  void setRealElectricityPriceEscalationRate( const double realElectricityPriceEscalationRate);
  double realGasPriceEscalationRate() {return m_realGasPriceEscalationRate;}
  void setRealGasPriceEscalationRate( const double realGasPriceEscalationRate);
  double federalCorporateTaxRate() {return m_federalCorporateTaxRate;}
  void setFederalCorporateTaxRate( const double federalCorporateTaxRate);
  double purchaseCost() {return m_purchaseCost;}
  void setPurchaseCost(const double purchaseCost);
  double installationCost() {return m_installationCost;}
  void setInstallationCost(const double installationCost);
  double totalCapitalCost() {return m_totalCapitalCost;}
  void setTotalCapitalCost(const double totalCapitalCost);
  double salvageValueExisting() {return m_salvageValueExisting;}
  void setSalvageValueExisting( const double salvageValueExisting);
  double disposalCost() {return m_disposalCost;}
  void setDisposalCost( const double disposalCost);
  double fixedOM() {return m_fixedOM;}
  void setFixedOM( const double fixedOM);
  double fixedMV() {return m_fixedMV;}
  void setFixedMV( const double fixedMV);
  double replacementCost() {return m_replacementCost;}
  void setReplacementCost( const double replacementCost);
  double electricityCostSavings() {return m_electricityCostSavings;}
  void setElectricityCostSavings( const double electricityCostSavings);
  double gasCostSavings() {return m_gasCostSavings;}
  void setGasCostSavings( const double gasCostSavings);
  double electricityCostSavingsCodeMinimum() {return m_electricityCostSavingsCodeMinimum;}
  void setElectricityCostSavingsCodeMinimum( const double electricityCostSavingsCodeMinimum);
  double gasCostSavingsCodeMinimum() {return m_gasCostSavingsCodeMinimum;}
  void setGasCostSavingsCodeMinimum( const double gasCostSavingsCodeMinimum);
  double specialTaxDeductions() {return m_specialTaxDeductions;}
  void setSpecialTaxDeductions( const double specialTaxDeductions);
  double specialTaxCredits() {return m_specialTaxCredits;}
  void setSpecialTaxCredits( const double specialTaxCredits);
  double rebates() {return m_rebates;}
  void setRebates( const double rebates);
  double depreciationTaxDeduction() {return m_depreciationTaxDeduction;}
  void setDepreciationTaxDeduction( const double depreciationTaxDeduction);
  double otherFinancialIncentives() {return m_otherFinancialIncentives;}
  void setOtherFinancialIncentives( const double otherFinancialIncentives);
  double wholeBldFinancialIncentives() {return m_wholeBldFinancialIncentives;}
  void setWholeBldFinancialIncentives( const double wholeBldFinancialIncentives);
  double projectPlanningAndDesign() {return m_projectPlanningAndDesign;}
  void setProjectPlanningAndDesign( const double projectPlanningAndDesign);
  double packageMV() {return m_packageMV;}
  void setPackageMV( const double packageMV);
  double replacementCostCodeMinimum() {return m_replacementCostCodeMinimum;}
  void setReplacementCostCodeMinimum( const double replacementCostCodeMinimum);
  void calcVariables();
  void calcCashFlows( std::vector<double>& cashFlows, const bool includeEnergySavings);
};

}

#endif // UTILITIES_ECONOMICS_ECONOMICS_HPP
