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

#include <vector>
#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include "PostProcessReporting.hpp"

namespace openstudio {
namespace runmanager {
namespace detail {
 
  std::vector<Attribute> PostProcessReporting::go(const SqlFile &t_sqlFile, 
                                                  const std::string& jobType)
  {
    std::vector<Attribute> attributes;
    boost::optional<double> val;
    std::string query;

    //Total Site Energy (GJ)
    val = t_sqlFile.totalSiteEnergy();
    if (val){
      attributes.push_back(Attribute("Total Site Energy", *val, "GJ"));
    }

    //Net Site Energy (GJ)
    val = t_sqlFile.netSiteEnergy();
    if (val){
      attributes.push_back(Attribute("Net Site Energy", *val, "GJ"));
    }

    //Total Source Energy (GJ)
    val = t_sqlFile.totalSourceEnergy();
    if (val){
      attributes.push_back(Attribute("Total Source Energy", *val, "GJ"));
    }

    //Net Source Energy (GJ)
    val = t_sqlFile.totalSourceEnergy();
    if (val){
      attributes.push_back(Attribute("Net Source Energy", *val, "GJ"));
    }

    //Annual Total Utility Cost ($)
    val = t_sqlFile.annualTotalUtilityCost();
    if (val){
      attributes.push_back(Attribute("Annual Total Utility Cost", *val, "$"));
    }

    //Annual Electrict Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::Electricity);
    if (val){
      attributes.push_back(Attribute("Annual Electric Total Cost", *val, "$"));
    }

    //Annual Gas Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::Gas);
    if (val){
      attributes.push_back(Attribute("Annual Gas Total Cost", *val, "$"));
    }

    //Annual District Cooling Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::DistrictCooling);
    if (val){
      attributes.push_back(Attribute("Annual District Cooling Total Cost", *val, "$"));
    }

    //Annual District Heating Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::DistrictHeating);
    if (val){
      attributes.push_back(Attribute("Annual District Heating Total Cost", *val, "$"));
    }

    //Annual Water Total Cost ($)
    val = t_sqlFile.annualTotalCost(FuelType::Water);
    if (val){
      attributes.push_back(Attribute("Annual Water Total Cost", *val, "$"));
    }

    //Net Site Energy Use Intensity (MJ/m^2)
    boost::optional<double> hours = t_sqlFile.hoursSimulated();
    if (!hours){
      LOG(Warn, "Reporting Net Site Energy Use Intensity \
                with unknown number of simulation hours");
    }else if(*hours != 8760){
      LOG(Warn, "Reporting Net Site Energy Use Intensity with " << *hours << " hrs");
    }
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='Site and Source Energy' AND \
                            RowName='Net Site Energy' AND \
                            ColumnName='Energy Per Total Building Area' AND \
                            Units='MJ/m2'";
    val = t_sqlFile.execAndReturnFirstDouble(query);    
    if (val){
      attributes.push_back(Attribute("Net Site Energy Use Intensity", *val, "MJ/m^2"));
    }    

    //Instantaneous Peak Electricity Demand (W)    
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='DemandEndUseComponentsSummary' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='End Uses' AND \
                            RowName='Total End Uses' AND \
                            ColumnName='Electricity' AND \
                            Units='W'";
    val = t_sqlFile.execAndReturnFirstDouble(query);
    if (val){
      attributes.push_back(Attribute("Instantaneous Peak Electricity Demand", *val, "kW"));
    }         

    //Electricity Total End Uses (GJ)
    val = t_sqlFile.electricityTotalEndUses();
    if (val){
      attributes.push_back(Attribute("Electricity Total End Uses", *val, "GJ"));
    }      
    
    //Natural Gas Total End Uses (GJ)
    val = t_sqlFile.naturalGasTotalEndUses();
    if (val){
      attributes.push_back(Attribute("Natural Gas Total End Uses", *val, "GJ"));
    }      
          
    //District Cooling Total End Uses (GJ)
    val = t_sqlFile.districtCoolingTotalEndUses();
    if (val){
      attributes.push_back(Attribute("District Cooling Total End Uses", *val, "GJ"));
    }      
        
    //District Heating Total End Uses (GJ)
    val = t_sqlFile.districtHeatingTotalEndUses();
    if (val){
      attributes.push_back(Attribute("District Heating Total End Uses", *val, "GJ"));
    }      
        
    //Other Fuel Total End Uses (GJ)
    val = t_sqlFile.otherFuelTotalEndUses();
    if (val){
      attributes.push_back(Attribute("Other Fuel Total End Uses", *val, "GJ"));
    }      
       
    //End Uses   
    boost::optional<EndUses> enduses = t_sqlFile.endUses();
    if (enduses){
      std::vector<Attribute> v = enduses->attribute().valueAsAttributeVector();

      for (const auto & elem1 : v)
      {
        std::vector<Attribute> v2 = elem1.valueAsAttributeVector();
        for (const auto & elem2 : v2)
        {
          std::vector<Attribute> v3 = elem2.valueAsAttributeVector();
          for (const auto & elem3 : v3)
          {
            attributes.push_back(Attribute(enduses->attribute().name() + "." + elem1.name() + "." + elem2.name() + "." + elem3.name(), 
                  elem3.valueAsDouble(), 
                  elem3.units()));
          }
        }
      }

    }

    //First Year Capital Costs
    //find the inflation approach (either constant dollar or current dollar)
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
             ReportName='Life-Cycle Cost Report' AND \
             ReportForString='Entire Facility' AND \
             TableName='Life-Cycle Cost Parameters' AND \
             RowName='Inflation Approach' AND \
             ColumnName='Value'";
    if ( boost::optional<std::string> infApprStr = t_sqlFile.execAndReturnFirstString(query) ) {
      //find the base year (first year)
      query = "SELECT Value FROM tabulardatawithstrings WHERE \
               ReportName='Life-Cycle Cost Report' AND \
               ReportForString='Entire Facility' AND \
               TableName='Life-Cycle Cost Parameters' AND \
               RowName='Base Date' AND \
               ColumnName='Value'"; 
      if ( boost::optional<std::string> baseYrString = t_sqlFile.execAndReturnFirstString(query) ) {
        //capital cash flow for this year
        query = "SELECT Value FROM tabulardatawithstrings WHERE \
                 ReportName='Life-Cycle Cost Report' AND \
                 ReportForString='Entire Facility' AND \
                 TableName='Capital Cash Flow by Category (Without Escalation)' AND \
                 RowName='" + *baseYrString + "' AND \
                 ColumnName='Total'";
        val = t_sqlFile.execAndReturnFirstDouble(query);
      }
      if (val){
        attributes.push_back(Attribute("First Year Capital Costs", *val, "$"));
      }
    }

    //Total LifeCycle Cost (present value)
    query = "SELECT Value FROM tabulardatawithstrings WHERE \
             ReportName='Life-Cycle Cost Report' AND \
             ReportForString='Entire Facility' AND \
             TableName='Present Value by Year' AND \
             RowName='TOTAL' AND \
             ColumnName='Present Value of Costs'"; 
    val = t_sqlFile.execAndReturnFirstDouble(query);
    if (val){
      attributes.push_back(Attribute("Total Life Cycle Cost", *val, "$"));
    }

    if (attributes.empty())
    {
      LOG(Warn, "No attributes loaded for report");
    }

    for (Attribute& attribute : attributes) {
      attribute.setSource(jobType);
    }

    return attributes;
  }



} // detail
} // runmanager
} // openstudio



