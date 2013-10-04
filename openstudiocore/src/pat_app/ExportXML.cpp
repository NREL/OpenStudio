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


#include <pat_app/ExportXML.hpp>
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>

#include <analysisdriver/SimpleProject.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/Problem.hpp>
#include <analysis/InputVariable.hpp>
#include <analysis/InputVariable_Impl.hpp>
#include <analysis/Measure.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/RubyMeasure.hpp>
#include <analysis/RubyMeasure_Impl.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/MeasureGroup_Impl.hpp>
#include <analysis/WorkflowStep.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/Job_Impl.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>

#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/Unit.hpp>
#include <utilities/units/UnitFactory.hpp>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace openstudio {
namespace analysis {
namespace exportxml {

ExportXML::ExportXML()
{
}

ExportXML::~ExportXML()
{
}

bool ExportXML::exportXML(const analysisdriver::SimpleProject project, QString xmlFilePath)
{
   
  //start the xml file
  QDomDocument doc;
  doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    
  //project models (first element)
  QDomElement projectModelsElem = doc.createElement("project_models");
  doc.appendChild(projectModelsElem);
  projectModelsElem.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");    
    
  //measures
  QDomElement measuresElem = doc.createElement("measures");
  projectModelsElem.appendChild(measuresElem);
  
  //get the analysis from the project
  Analysis analysis = project.analysis();
    
  //get the problem from the project
  Problem problem = analysis.problem();

  //get the input variables from the problem
  std::vector<InputVariable> variables = problem.variables();

  //make a vector to hold all the uids of the measure definitions in the project
  std::vector<std::string> measDefUids;

  //loop through all measures in the project
  Q_FOREACH( const InputVariable & variable, variables) {
    if ( boost::optional<MeasureGroup> discVar = variable.optionalCast<MeasureGroup>() ) {
      //get the measure from the variable
      std::vector<Measure> measures = discVar->measures(false);
      BOOST_FOREACH( Measure & measure, measures) {
        if ( boost::optional<RubyMeasure> rubyMeasure = measure.optionalCast<RubyMeasure>() ) {
          if ( boost::optional<BCLMeasure> bclMeasure = rubyMeasure->measure() ) {
            //skip if measure def has already been added to file
            if (std::find(measDefUids.begin(), measDefUids.end(), bclMeasure->uid()) != measDefUids.end()) {
              continue;
            }
            //skip is it is a fixed measure (present in all design alternatives)
            if (discVar->measures(false).size() == 1) {
              continue;
            }
            //now, add the uuid to the vector so it won't get recorded again
            measDefUids.push_back(bclMeasure->uid());
            //add measure def
            if ( boost::optional<QDomElement> measureElem = exportMeasure(doc, *bclMeasure) ) {
              measuresElem.appendChild(*measureElem);
            }
          }
        }
      }
    }
  }
  
  //analysis groups
  QDomElement analysisGroupsElem = doc.createElement("analysis_groups");
  projectModelsElem.appendChild(analysisGroupsElem);
  
  //analysis (for this use case there will only be one analysis)
  QDomElement analysisElem = doc.createElement("analysis");
  analysisGroupsElem.appendChild(analysisElem);
    
  //baseline
  analysis::DataPoint baseline = project.baselineDataPoint();
  boost::optional<model::Model> optionalModel = baseline.model();
  boost::optional<SqlFile> optionalSql = baseline.sqlFile();
  std::vector<WorkflowStepJob> jobs = problem.getJobsByWorkflowStep(baseline, true);
  if ( optionalSql ) {
    if ( boost::optional<QDomElement> measureElem = exportDesignAlternative(doc, *optionalSql, baseline, jobs, true ) ){
      analysisElem.appendChild(*measureElem);
    }
  }

  //alternatives
  QDomElement alternativesElem = doc.createElement("alternatives");
  analysisElem.appendChild(alternativesElem);
  DataPointVector dataPoints = analysis.dataPoints();
  Q_FOREACH(const DataPoint& datapoint, dataPoints) {
    //don't re-log the baseline
    if (datapoint.uuid() == baseline.uuid() ){
      continue;
    }
    //will not have model or sqlfile if not complete or if failed
    //only continue if the model and sql file and runmanager results are available
    optionalModel = datapoint.model();
    optionalSql = datapoint.sqlFile();
    std::vector<WorkflowStepJob> jobs = problem.getJobsByWorkflowStep(datapoint, true);
    if ( optionalSql ) {
      if ( boost::optional<QDomElement> measureElem = exportDesignAlternative(doc, *optionalSql, datapoint, jobs, false ) ){
        alternativesElem.appendChild(*measureElem);
      }
    }
  }    
    
  //write out the xml file
  //boost::optional<std::string> xmlFilePath = toString(project.projectDir());
  //if (xmlFilePath){
  //  QFile file(toQString(*xmlFilePath+ "/testXMLexport.xml"));
  //  if (file.open(QFile::WriteOnly)){
  //    QTextStream textStream(&file);
  //    textStream << doc.toString(2);
  //    file.close();
  //    return true;
  //  }
  //}
  QFile file(xmlFilePath);
  if (file.open(QFile::WriteOnly)){
    QTextStream textStream(&file);
    textStream << doc.toString(2);
    file.close();
    return true;
  }

  return false;

}

QString ExportXML::escapeName(const std::string& name)
{
  QString result = toQString(name);
  result.replace(" ", "_");
  result.replace("(", "_");
  result.replace(")", "_");
  result.replace("{", "");
  result.replace("}", "");
  return result;
}

boost::optional<QDomElement> ExportXML::exportMeasure(QDomDocument& doc, BCLMeasure& measure)
{
  //start the measure
  QDomElement measureElem = doc.createElement("measure");

  //id
  QDomElement idElem = doc.createElement("id");
  measureElem.appendChild(idElem);
  QString id = toQString(measure.uid());
  idElem.appendChild(doc.createTextNode(id));

  //uri
  QDomElement uriElem = doc.createElement("uri");
  measureElem.appendChild(uriElem);
  QString uri = "TODO uri goes here";
  uriElem.appendChild(doc.createTextNode(uri));

  //category
  QDomElement categoryElem = doc.createElement("category");
  measureElem.appendChild(categoryElem);
  QString category = toQString(measure.taxonomyTag());
  categoryElem.appendChild(doc.createTextNode(category));

  //name
  QDomElement nameElem = doc.createElement("name");
  measureElem.appendChild(nameElem);
  QString name = toQString(measure.name());
  nameElem.appendChild(doc.createTextNode(name));
   
  //description
  QDomElement descElem = doc.createElement("description");
  measureElem.appendChild(descElem);
  QString desc = toQString(measure.description());
  descElem.appendChild(doc.createTextNode(desc));

  return measureElem;
}
  
boost::optional<QDomElement> ExportXML::exportDesignAlternative(QDomDocument& doc, 
                                                                //model::Model& model,
                                                                SqlFile& sql,
                                                                analysis::DataPoint dataPt,
                                                                std::vector<WorkflowStepJob>& jobs,
                                                                bool isBaseline)
{
  //start the alternative
  QDomElement alternative;
  if (isBaseline) {
    alternative = doc.createElement("baseline");
  }
  else {
    alternative = doc.createElement("alternative");
  }
  
  //name
  QDomElement nameElem = doc.createElement("name");
  alternative.appendChild(nameElem);
  QString name = toQString(dataPt.name());
  nameElem.appendChild(doc.createTextNode(name));
  
  //description
  QDomElement descriptionElem = doc.createElement("description");
  alternative.appendChild(descriptionElem);
  QString description = toQString(dataPt.description());
  descriptionElem.appendChild(doc.createTextNode(description));

  //characteristics
  QDomElement characteristicsElem = doc.createElement("characteristics");
  alternative.appendChild(characteristicsElem);
    
  //building
  QDomElement buildingElem = doc.createElement("building");
  characteristicsElem.appendChild(buildingElem);

  //TODO building_type
  QDomElement buildingTypeElem = doc.createElement("building_type");
  buildingElem.appendChild(buildingTypeElem);
  QString buildingType = "TODO The building type goes here";
  buildingTypeElem.appendChild(doc.createTextNode(buildingType));
  
  //floor_area
  QDomElement floorAreaElem = doc.createElement("floor_area");
  buildingElem.appendChild(floorAreaElem);
  std::string s = "SELECT Value FROM tabulardatawithstrings WHERE \
                  ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                  ReportForString='Entire Facility' AND \
                  TableName='Building Area' AND \
                  RowName='Net Conditioned Building Area' AND \
                  ColumnName='Area' AND \
                  Units='m2'";   
  if ( boost::optional<double> condArea = sql.execAndReturnFirstDouble(s) ) {
    QString floorArea = QString::number(*condArea);
    floorAreaElem.appendChild(doc.createTextNode(floorArea));
  }
  else {
    QString floorArea = "Building has no conditioned floor area";
    floorAreaElem.appendChild(doc.createTextNode(floorArea));
  }  

  //results
  QDomElement resultsElem = doc.createElement("results");
  alternative.appendChild(resultsElem);
    
  //cash flows
  std::string infApprQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                  ReportName='Life-Cycle Cost Report' AND \
                  ReportForString='Entire Facility' AND \
                  TableName='Life-Cycle Cost Parameters' AND \
                  RowName='Inflation Approach' AND \
                  ColumnName='Value'";    
  if ( boost::optional<std::string> infApprStr = sql.execAndReturnFirstString(infApprQuery) ) {
    std::string infAppr = "ConstantDollar";
    if (*infApprStr == "ConstantDollar") {
      infAppr = "Constant Dollar";
    }
    if (*infApprStr == "CurrentDollar") {
      infAppr = "Current Dollar";
    }
    std::string baseYrQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                ReportName='Life-Cycle Cost Report' AND \
                ReportForString='Entire Facility' AND \
                TableName='Life-Cycle Cost Parameters' AND \
                RowName='Base Date' AND \
                ColumnName='Value'"; 
    if ( boost::optional<std::string> baseYrString = sql.execAndReturnFirstString(baseYrQuery) ) {
      std::string lengthYrsQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                      ReportName='Life-Cycle Cost Report' AND \
                      ReportForString='Entire Facility' AND \
                      TableName='Life-Cycle Cost Parameters' AND \
                      RowName='Length of Study Period in Years' AND \
                      ColumnName='Value'";    
      //parse the year out of the base year string which looks like: "January    2012"
      boost::regex re ("\\d\\d\\d\\d");
      boost::smatch matches;
      if (boost::regex_search(*baseYrString,matches,re)) {
        std::string baseYrStr = std::string(matches[0]);
        int baseYr = atoi(baseYrStr.c_str());
        if ( boost::optional<double> lengthYrs = sql.execAndReturnFirstDouble(lengthYrsQuery) ) { 
          //loop through all years in the analysis and record the cash flow in these maps
          std::map<std::string,double> capCashFlow;
          std::map<std::string,double> omCashFlow;
          std::map<std::string,double> energyCashFlow;
          std::map<std::string,double> waterCashFlow;
          std::map<std::string,double> totCashFlow;

          //loop through each year in the analysis and log cash flow
          for ( int i = 0; i < *lengthYrs; i++ ) {
            int newYr = baseYr + i;
            std::string yr = "'January           " + boost::lexical_cast<std::string>(newYr) + "'";
            double annCapCash = 0.0;
            double annOmCash = 0.0;
            double annEnergyCash = 0.0;
            double annWaterCash = 0.0;
            double annTotCash = 0.0;

            //capital cash flow for this year
            std::string capCashQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='Life-Cycle Cost Report' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='Capital Cash Flow by Category (Without Escalation)' AND \
                            RowName=" + yr + "AND \
                            ColumnName='Total'";    
            if ( boost::optional<double> capCash = sql.execAndReturnFirstDouble(capCashQuery) ) { 
              annCapCash += *capCash;
              annTotCash += *capCash;
            }
          
            //operations and maintenance cash flow (excluding utility costs)
            std::vector<std::string> omTypes;
            omTypes.push_back("Maintenance");
            omTypes.push_back("Repair");
            omTypes.push_back("Operation");
            omTypes.push_back("Replacement");
            omTypes.push_back("MinorOverhaul");
            omTypes.push_back("MajorOverhaul");
            omTypes.push_back("OtherOperational");    
            Q_FOREACH( const std::string& omType, omTypes) {
              std::string omCashQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='Life-Cycle Cost Report' AND \
                              ReportForString='Entire Facility' AND \
                              TableName='Operating Cash Flow by Category (Without Escalation)' AND \
                              RowName=" + yr + " AND \
                              ColumnName='" + omType + "'";             
              if ( boost::optional<double> omCash = sql.execAndReturnFirstDouble(omCashQuery) ) { 
                annOmCash += *omCash;
                annTotCash += *omCash;
              }
            }

            //energy cash flow for this year
            std::string energyCashQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='Life-Cycle Cost Report' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='Operating Cash Flow by Category (Without Escalation)' AND \
                            RowName=" + yr + "AND \
                            ColumnName='Energy'";    
            if ( boost::optional<double> energyCash = sql.execAndReturnFirstDouble(energyCashQuery) ) { 
              annEnergyCash += *energyCash;
              annTotCash += *energyCash;
            }

            //water cash flow for this year
            std::string waterCashQuery = "SELECT Value FROM tabulardatawithstrings WHERE \
                            ReportName='Life-Cycle Cost Report' AND \
                            ReportForString='Entire Facility' AND \
                            TableName='Operating Cash Flow by Category (Without Escalation)' AND \
                            RowName=" + yr + "AND \
                            ColumnName='Water'";    
            if ( boost::optional<double> waterCash = sql.execAndReturnFirstDouble(waterCashQuery) ) { 
              annWaterCash += *waterCash;
              annTotCash += *waterCash;
            }

            //log for this year
            capCashFlow.insert(std::make_pair(yr,annCapCash));
            omCashFlow.insert(std::make_pair(yr,annOmCash));
            energyCashFlow.insert(std::make_pair(yr,annEnergyCash));
            waterCashFlow.insert(std::make_pair(yr,annWaterCash));
            totCashFlow.insert(std::make_pair(yr,annTotCash));

          }
        
          //cash flows
          QDomElement cashFlowsElem = doc.createElement("cash_flows");
          resultsElem.appendChild(cashFlowsElem);
        
          //capital
          if ( boost::optional<QDomElement> cashFlowElem = exportCashFlow(doc, capCashFlow, infAppr + " Capital Costs") ){
            cashFlowsElem.appendChild(*cashFlowElem);
          }

          //o&m
          if ( boost::optional<QDomElement> cashFlowElem = exportCashFlow(doc, omCashFlow, infAppr + " Operating Costs") ){
            cashFlowsElem.appendChild(*cashFlowElem);
          } 
        
          //energy
          if ( boost::optional<QDomElement> cashFlowElem = exportCashFlow(doc, energyCashFlow, infAppr + " Energy Costs") ){
            cashFlowsElem.appendChild(*cashFlowElem);
          } 
        
          //water
          if ( boost::optional<QDomElement> cashFlowElem = exportCashFlow(doc, waterCashFlow, infAppr + " Water Costs") ){
            cashFlowsElem.appendChild(*cashFlowElem);
          } 
        
          //total
          if ( boost::optional<QDomElement> cashFlowElem = exportCashFlow(doc, totCashFlow, infAppr + " Total Costs") ){
            cashFlowsElem.appendChild(*cashFlowElem);
          } 
        }
      }
    }
  }

  //annual
  //if ( boost::optional<QDomElement> annualElem = exportAnnual(doc, model, sql) ){
  if ( boost::optional<QDomElement> annualElem = exportAnnual(doc, sql) ){
    resultsElem.appendChild(*annualElem);
  }         
  
  //monthly
  //if ( boost::optional<QDomElement> monthlyElem = exportMonthly(doc, model, sql) ){
  if ( boost::optional<QDomElement> monthlyElem = exportMonthly(doc, sql) ){
    resultsElem.appendChild(*monthlyElem);
  }   
    
  //number of measures in a design alt. differentiates whether it is called a measure
  //or a design alternative in Xcel EDA
  double numMeasures = 0.0;

  //alternative measures
  if (!isBaseline){
    QDomElement alternativeMeasuresElem = doc.createElement("measures");
    alternative.appendChild(alternativeMeasuresElem);
    BOOST_FOREACH(WorkflowStepJob& job, jobs) {
      WorkflowStep step = job.step;
      if ( step.isInputVariable() ){
        if (boost::optional<analysis::Measure> measure = job.measure) {
          if (boost::optional<analysis::AnalysisObject> parObj = measure->parent()) {
            if (boost::optional<MeasureGroup> discVar = parObj->optionalCast<MeasureGroup>()) {
              //skip is it is a fixed measure (present in all design alternatives)
              if (discVar->measures(false).size() != 1) {
                //log how many measures the alternative has.
                numMeasures += 1;
                //record the details of the measure in the xml.
                if (boost::optional<QDomElement> alternativeMeasureElem = exportAlternativeMeasure(doc, job)){
                  alternativeMeasuresElem.appendChild(*alternativeMeasureElem);
                }
              }
            }
          }
        }
      }
    }
  }

  //alternative_type
  if (!isBaseline) {
    QDomElement altTypeElem = doc.createElement("alternative_type");
    alternative.appendChild(altTypeElem);
    if ( numMeasures > 1 ) {
      QString altType = "design_alternative";
      altTypeElem.appendChild(doc.createTextNode(altType));
    }
    else {
      QString altType = "single_measure";
      altTypeElem.appendChild(doc.createTextNode(altType));
    }
  }

  return alternative;
}
  
boost::optional<QDomElement> ExportXML::exportAnnual(QDomDocument& doc,
                                                      //model::Model& model,
                                                      SqlFile& sql)
{
  //setup a map between end use names in C++ and for the xml file
  std::map<EndUseCategoryType,QString> xmlEndUses;
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Heating,"heating"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Cooling,"cooling"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::InteriorLights,"lighting_interior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::ExteriorLights,"lighting_exterior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::InteriorEquipment,"equipment_interior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::ExteriorEquipment,"equipment_exterior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Fans,"fans"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Pumps,"pumps"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::HeatRejection,"heat_rejection"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Humidifier,"humidification"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::HeatRecovery,"heat_recovery"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::WaterSystems,"water_systems"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Refrigeration,"refrigeration"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Generators,"generators"));

  //setup a map between fuel types in EndUseFuelTypes and FuelTypes
  std::map<EndUseFuelType,FuelType> fuelMap;  
  fuelMap.insert(std::make_pair(EndUseFuelType::Electricity,FuelType::Electricity));
  fuelMap.insert(std::make_pair(EndUseFuelType::Gas,FuelType::Gas));
  fuelMap.insert(std::make_pair(EndUseFuelType::AdditionalFuel,FuelType::Diesel)); //TODO add other fuel types
  fuelMap.insert(std::make_pair(EndUseFuelType::DistrictCooling,FuelType::DistrictCooling));
  fuelMap.insert(std::make_pair(EndUseFuelType::DistrictHeating,FuelType::DistrictHeating));
  fuelMap.insert(std::make_pair(EndUseFuelType::Water,FuelType::Water));

  //start the annual results
  QDomElement annual = doc.createElement("annual");
  
  //consumption
  QDomElement consumptionElem = doc.createElement("consumption");
  annual.appendChild(consumptionElem);

    //gas
    QDomElement gasElem = doc.createElement("gas");
    consumptionElem.appendChild(gasElem);
    if ( boost::optional<double> val = sql.naturalGasTotalEndUses() ) {
      QString gas = QString::number(*val);
      gasElem.appendChild(doc.createTextNode(gas));
    }

    //electricity
    QDomElement electricityElem = doc.createElement("electricity");
    consumptionElem.appendChild(electricityElem);
    if ( boost::optional<double> val = sql.electricityTotalEndUses() ) {
      QString electricity = QString::number(*val);
      electricityElem.appendChild(doc.createTextNode(electricity));
    }

    //other_energy
    QDomElement otherEnergyElem = doc.createElement("other_energy");
    consumptionElem.appendChild(otherEnergyElem);
    if ( boost::optional<double> val = sql.otherFuelTotalEndUses() ) {
      QString otherEnergy = QString::number(*val);
      otherEnergyElem.appendChild(doc.createTextNode(otherEnergy));
    }

    //district_heating
    QDomElement districtHeatingElem = doc.createElement("district_heating");
    consumptionElem.appendChild(districtHeatingElem);
    if ( boost::optional<double> val = sql.districtHeatingTotalEndUses() ) {
      QString districtHeating = QString::number(*val);
      districtHeatingElem.appendChild(doc.createTextNode(districtHeating));
    }

     //district_cooling
    QDomElement districtCoolingElem = doc.createElement("district_cooling");
    consumptionElem.appendChild(districtCoolingElem);
    if ( boost::optional<double> val = sql.districtCoolingTotalEndUses() ) {
      QString districtCooling = QString::number(*val);
      districtCoolingElem.appendChild(doc.createTextNode(districtCooling));
    }

    //water
    QDomElement waterElem = doc.createElement("water");
    consumptionElem.appendChild(waterElem);
    if ( boost::optional<double> val = sql.waterTotalEndUses() ) {
      QString water = QString::number(*val);
      waterElem.appendChild(doc.createTextNode(water));
    } 
    
  //demand
  QDomElement demandElem = doc.createElement("demand");
  annual.appendChild(demandElem);

    //get the weather file (as opposed to design day) run period
    boost::optional<std::string> annEnvPd = boost::none;
    std::vector<std::string> envPds = sql.availableEnvPeriods();
    Q_FOREACH( const std::string& envPd, envPds) {
      if ( boost::optional<openstudio::EnvironmentType> envType = sql.environmentType(envPd) ) {
        if ( (*envType) == openstudio::EnvironmentType::WeatherRunPeriod ) {
          annEnvPd = envPd;
        }
      }
    }

    //assuming there is an annual run, get the peak data
    if ( annEnvPd ) {
      
      //create some units to use
      Unit jouleUnit = UnitFactory::instance().createUnit("J").get();
      Unit gigaJouleUnit = UnitFactory::instance().createUnit("GJ").get();
      Unit hrsUnit = UnitFactory::instance().createUnit("h").get();
      Unit wUnit = UnitFactory::instance().createUnit("W").get();

      //electricity_peak_demand      
      QString electricityPeakDemand = QString::number(0.0);
      if ( boost::optional<TimeSeries> elec = sql.timeSeries(*annEnvPd, "Zone Timestep", "Electricity:Facility", "") ) {
        Quantity electricityPeakDemandTimestepJ = Quantity(maximum(elec->values()), jouleUnit);
        //deduce the timestep based on the hours simulated and the size of the interval data vector
        if ( boost::optional<double> hrsSimulated = sql.hoursSimulated() ) {
          double numIntervals = elec->values().size();
          Quantity intLengthHours = Quantity((*hrsSimulated / numIntervals), hrsUnit);
          Quantity electricityPeakDemandHourlyJPerHr = electricityPeakDemandTimestepJ / intLengthHours;
          if ( boost::optional<Quantity> electricityPeakDemandHourlyW = openstudio::convert(electricityPeakDemandHourlyJPerHr, wUnit) ) {
            electricityPeakDemand = QString::number(electricityPeakDemandHourlyW->value());
          }
        }
      }
      QDomElement electricityPeakDemandElem = doc.createElement("electricity_peak_demand");
      demandElem.appendChild(electricityPeakDemandElem);
      electricityPeakDemandElem.appendChild(doc.createTextNode(electricityPeakDemand));

      //electricity_annual_avg_peak_demand
      QString electricityAnnualAvgPeakDemand = QString::number(0.0);
      if ( boost::optional<double> val = sql.electricityTotalEndUses() ) {
        if ( boost::optional<double> hrs = sql.hoursSimulated() ) {    
          Quantity annElecGJ = Quantity(*val, gigaJouleUnit);
          Quantity annHrs = Quantity(*hrs, hrsUnit);
          Quantity electricityAnnualAvgPeakDemandHourlyGJPerHr = annElecGJ / annHrs;
          if ( boost::optional<Quantity> electricityAnnualAvgPeakDemandHourlyW = openstudio::convert(electricityAnnualAvgPeakDemandHourlyGJPerHr, wUnit) ) {
            electricityAnnualAvgPeakDemand = QString::number(electricityAnnualAvgPeakDemandHourlyW->value());
          }
        }
      }
      QDomElement electricityAnnualAvgPeakDemandElem = doc.createElement("electricity_annual_avg_peak_demand");
      demandElem.appendChild(electricityAnnualAvgPeakDemandElem);
      electricityAnnualAvgPeakDemandElem.appendChild(doc.createTextNode(electricityAnnualAvgPeakDemand));

      //district_cooling_peak_demand      
      QString districtCoolingPeakDemand = QString::number(0.0);
      if ( boost::optional<TimeSeries> elec = sql.timeSeries(*annEnvPd, "Zone Timestep", "DistrictCooling:Facility", "") ) {
        Quantity districtCoolingPeakDemandTimestepJ = Quantity(maximum(elec->values()), jouleUnit);
        //deduce the timestep based on the hours simulated and the size of the interval data vector
        if ( boost::optional<double> hrsSimulated = sql.hoursSimulated() ) {
          double numIntervals = elec->values().size();
          Quantity intLengthHours = Quantity((*hrsSimulated / numIntervals), hrsUnit);
          Quantity districtCoolingPeakDemandHourlyJPerHr = districtCoolingPeakDemandTimestepJ / intLengthHours;
          if ( boost::optional<Quantity> districtCoolingPeakDemandHourlyW = openstudio::convert(districtCoolingPeakDemandHourlyJPerHr, wUnit) ) {
            districtCoolingPeakDemand = QString::number(districtCoolingPeakDemandHourlyW->value());
          }
        }
      }
      QDomElement districtCoolingPeakDemandElem = doc.createElement("district_cooling_peak_demand");
      demandElem.appendChild(districtCoolingPeakDemandElem);
      districtCoolingPeakDemandElem.appendChild(doc.createTextNode(districtCoolingPeakDemand));

    }

  //utility_cost
  QDomElement utilityCostElem = doc.createElement("utility_cost");
  annual.appendChild(utilityCostElem); 

    //electricity
    QDomElement electricityCostElem = doc.createElement("electricity");
    utilityCostElem.appendChild(electricityCostElem);
     if ( boost::optional<double> val = sql.annualTotalCost(FuelType::Electricity) ) {
      QString valStr = QString::number(*val);
      electricityCostElem.appendChild(doc.createTextNode(valStr));
    }    
    
    //gas
    QDomElement gasCostElem = doc.createElement("gas");
    utilityCostElem.appendChild(gasCostElem);
     if ( boost::optional<double> val = sql.annualTotalCost(FuelType::Gas) ) {
      QString valStr = QString::number(*val);
      gasCostElem.appendChild(doc.createTextNode(valStr));
    }  

    //other_energy
    QDomElement otherEnergyCostElem = doc.createElement("other_energy");
    utilityCostElem.appendChild(otherEnergyCostElem);
     if ( boost::optional<double> val = sql.annualTotalCost(FuelType::Diesel) ) {
        //TODO all other fuel types that need to go in here
       QString valStr = QString::number(*val);
      otherEnergyCostElem.appendChild(doc.createTextNode(valStr));
    }  

    //district_heating
    QDomElement distHeatingCostElem = doc.createElement("district_heating");
    utilityCostElem.appendChild(distHeatingCostElem);
     if ( boost::optional<double> val = sql.annualTotalCost(FuelType::DistrictHeating) ) {
      QString valStr = QString::number(*val);
      distHeatingCostElem.appendChild(doc.createTextNode(valStr));
    }  

    //district_cooling
    QDomElement distCoolingCostElem = doc.createElement("district_cooling");
    utilityCostElem.appendChild(distCoolingCostElem);
     if ( boost::optional<double> val = sql.annualTotalCost(FuelType::DistrictCooling) ) {
      QString valStr = QString::number(*val);
      distCoolingCostElem.appendChild(doc.createTextNode(valStr));
    }  

    //water
    QDomElement waterCostElem = doc.createElement("water");
    utilityCostElem.appendChild(waterCostElem);
     if ( boost::optional<double> val = sql.annualTotalCost(FuelType::Water) ) {
      QString valStr = QString::number(*val);
      waterCostElem.appendChild(doc.createTextNode(valStr));
    }  
              
    //total
    QDomElement totalCostElem = doc.createElement("total_cost");
    utilityCostElem.appendChild(totalCostElem);
     if ( boost::optional<double> val = sql.annualTotalUtilityCost() ) {
      QString valStr = QString::number(*val);
      totalCostElem.appendChild(doc.createTextNode(valStr));
    }  

    //utility costs by end use using average blended cost
    QDomElement endUsesElem = doc.createElement("end_uses");
    utilityCostElem.appendChild(endUsesElem);

      //make a map to store the aggregate costs of each fuel type by end use
      std::map<EndUseCategoryType,double> costsByEndUse;

      //fill the map with 0.0 for each fuel type to start
      std::set<int> endUseEnums = EndUseCategoryType::getValues();
      double annUsg = 0.0;
      Q_FOREACH(int endUseEnum, endUseEnums) {
        EndUseCategoryType endUse(endUseEnum); 
        costsByEndUse.insert(std::make_pair(endUse,0.0));    
      }

      //get the fuel type 
      if (boost::optional<EndUses> endUses = sql.endUses() ) {

        //loop through all the fuel types
        std::set<int> fuelTypeEnums = EndUseFuelType::getValues();
        Q_FOREACH(int fuelTypeEnum,fuelTypeEnums) { 
          EndUseFuelType fuelType(fuelTypeEnum);
    
          //get the annual total cost for this fuel type
          //need the fuel 
          FuelType regularFuelType= fuelMap.find(fuelType)->second;
          boost::optional<double> annCost =  sql.annualTotalCost(regularFuelType);    

          //get the total annual usage for this fuel type in all categories
          //loop through all end uses, adding the usage value to the aggregator
          std::set<int> endUseVals = EndUseCategoryType::getValues();
          annUsg = 0.0;
          Q_FOREACH(int endUseEnum, endUseEnums) {
            EndUseCategoryType endUse(endUseEnum);     
            annUsg +=  endUses->getEndUse(fuelType, endUse);
          }
    
          //figure out the annual blended rate for this fuel type
          double avgAnnRate = 0.0;
          if ( annCost && (annUsg > 0) ) {
            avgAnnRate = *annCost/annUsg;
          }

          //for each end use category, figure out the cost if using the avg rate
          //add this cost to the map
          std::set<int> endUseEnums = EndUseCategoryType::getValues();
          Q_FOREACH(int endUseEnum, endUseEnums) {
            EndUseCategoryType endUse(endUseEnum); 
            //new value = value in map + additional cost for this fuel type
            double oldVal = costsByEndUse.find(endUse)->second;
            double newVal = endUses->getEndUse(fuelType, endUse) * avgAnnRate;
            costsByEndUse.find(endUse)->second = oldVal + newVal;
          }
        }
      }

      //now, go through map and put each value into an xml element
      std::map<EndUseCategoryType,double>::iterator iter;
      for (iter = costsByEndUse.begin(); iter != costsByEndUse.end(); ++iter) {
        EndUseCategoryType endUse = iter->first;
        double annCostByEndUse = iter->second;
        QString xmlEndUse = xmlEndUses.find(endUse)->second;
        QDomElement annCostElem = doc.createElement(xmlEndUse);
        endUsesElem.appendChild(annCostElem);
        annCostElem.appendChild(doc.createTextNode(QString::number(annCostByEndUse)));
      }

  return annual;
}
  
boost::optional<QDomElement> ExportXML::exportMonthly(QDomDocument& doc,
                                                      //model::Model& model,
                                                      SqlFile& sql)
  {
  //setup a map between end use names in C++ and for the xml file
  std::map<EndUseCategoryType,QString> xmlEndUses;
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Heating,"heating"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Cooling,"cooling"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::InteriorLights,"lighting_interior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::ExteriorLights,"lighting_exterior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::InteriorEquipment,"equipment_interior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::ExteriorEquipment,"equipment_exterior"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Fans,"fans"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Pumps,"pumps"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::HeatRejection,"heat_rejection"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Humidifier,"humidification"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::HeatRecovery,"heat_recovery"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::WaterSystems,"water_systems"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Refrigeration,"refrigeration"));
  xmlEndUses.insert(std::make_pair(EndUseCategoryType::Generators,"generators"));
    
  //setup a map between fuel type names in C++ and for the xml file
  std::map<EndUseFuelType,QString> xmlFuelTypes;
  xmlFuelTypes.insert(std::make_pair(EndUseFuelType::Electricity,"electricity"));
  xmlFuelTypes.insert(std::make_pair(EndUseFuelType::Gas,"gas"));
  xmlFuelTypes.insert(std::make_pair(EndUseFuelType::AdditionalFuel,"other_fuel"));
  xmlFuelTypes.insert(std::make_pair(EndUseFuelType::DistrictCooling,"district_cooling"));
  xmlFuelTypes.insert(std::make_pair(EndUseFuelType::DistrictHeating,"district_heating"));
  xmlFuelTypes.insert(std::make_pair(EndUseFuelType::Water,"water"));
    
  //start the monthly results
  QDomElement monthly = doc.createElement("monthly");
    
  //get a list of the enduses and fuel types
  std::set<int> endUseVals = EndUseCategoryType::getValues();
  std::set<int> fuelTypeVals = EndUseFuelType::getValues();

  //consumption
  QDomElement consumptionElem = doc.createElement("consumption");
  monthly.appendChild(consumptionElem);

    //loop through all end uses
    Q_FOREACH(int endUseVal,endUseVals) {
      EndUseCategoryType endUse(endUseVal);     
      QDomElement endUseElem = doc.createElement(xmlEndUses.find(endUse)->second);
      consumptionElem.appendChild(endUseElem);
      //in each end use, loop through all fuel types
      Q_FOREACH(int fuelTypeVal,fuelTypeVals) { 
        EndUseFuelType fuelType(fuelTypeVal);     
        QDomElement fuelTypeElem = doc.createElement(xmlFuelTypes.find(fuelType)->second);
        endUseElem.appendChild(fuelTypeElem);          
        //in each fuel type, loop through and get months 
        double annualTotalUse = 0.0;
        std::set<int> monthVals = MonthOfYear::getValues();
        Q_FOREACH(int monthVal,monthVals) {
          //skip enums NotAMonth and NumMonths
          MonthOfYear month(monthVal);
          if (month >= 1 && month <= 12) {
            double value = 0.0;
            if ( boost::optional<double> val = sql.energyConsumptionByMonth(fuelType, endUse, month) ) {
              value = *val;
            } 
            annualTotalUse += value;
            QDomElement monthElem = doc.createElement("month");
            fuelTypeElem.appendChild(monthElem);
            QString stringValue = QString::number(value);
            monthElem.appendChild(doc.createTextNode(stringValue));

          }
        }
        QDomElement totalElem = doc.createElement("total");
        fuelTypeElem.appendChild(totalElem);
        QString stringValue = QString::number(annualTotalUse);
        totalElem.appendChild(doc.createTextNode(stringValue));          
      }
    } 
                
  //demand
  QDomElement demandElem = doc.createElement("demand");
  monthly.appendChild(demandElem);

    //loop through all end uses
    Q_FOREACH(int endUseVal,endUseVals) {
      EndUseCategoryType endUse(endUseVal);     
      QDomElement endUseElem = doc.createElement(xmlEndUses.find(endUse)->second);
      demandElem.appendChild(endUseElem);
      //in each end use, loop through all fuel types
      Q_FOREACH(int fuelTypeVal,fuelTypeVals) { 
        EndUseFuelType fuelType(fuelTypeVal);     
        QDomElement fuelTypeElem = doc.createElement(xmlFuelTypes.find(fuelType)->second);
        endUseElem.appendChild(fuelTypeElem);          
        //in each fuel type, loop through and get months 
        double peakAnnualDemand = 0.0;
        std::set<int> monthVals = MonthOfYear::getValues();
        Q_FOREACH(int monthVal,monthVals) {
          //skip enums NotAMonth and NumMonths
          MonthOfYear month(monthVal);
          if (month >= 1 && month <= 12) {
            double value = 0.0;
            if ( boost::optional<double> val = sql.peakEnergyDemandByMonth(fuelType, endUse, month) ) {
              value = *val;
            } 
            if ( value > peakAnnualDemand ) {
              peakAnnualDemand = value;
            }
            QDomElement monthElem = doc.createElement("month");
            fuelTypeElem.appendChild(monthElem);
            QString stringValue = QString::number(value);
            monthElem.appendChild(doc.createTextNode(stringValue));
          }
        }
        QDomElement totalElem = doc.createElement("total");
        fuelTypeElem.appendChild(totalElem);
        QString stringValue = QString::number(peakAnnualDemand);
        totalElem.appendChild(doc.createTextNode(stringValue));          
      }
    } 

    return monthly;
  }
  
boost::optional<QDomElement> ExportXML::exportAlternativeMeasure(QDomDocument& doc, WorkflowStepJob& wfJob)
  {
    
    WorkflowStep step = wfJob.step;
    
    //only log the measures, not the post-and pre-process and energyplus job info
    if ( step.isInputVariable()){
    
      if ( boost::optional<runmanager::Job> job = wfJob.job ) {
    
        runmanager::JobErrors jobErrs = job->errors();
    
        //start the alternative measure
        QDomElement alternativeMeasure = doc.createElement("measure");   

        //measure_id
        QString measureId = "uuid unknown";
        if ( boost::optional<Measure> measure = wfJob.measure ) {
          if ( boost::optional<RubyMeasure> rubyMeasure = measure->optionalCast<RubyMeasure>() ) {
            if ( boost::optional<BCLMeasure> bclMeasure = rubyMeasure->measure() ) {
              measureId = toQString(bclMeasure->uid());
            }
          }
        }
        QDomElement measureIdElem = doc.createElement("measure_id");
        alternativeMeasure.appendChild(measureIdElem);
        measureIdElem.appendChild(doc.createTextNode(measureId));

        //instance_name
        QString instanceName = "unknown";
        if ( boost::optional<Measure> measure = wfJob.measure ) {
          if ( boost::optional<RubyMeasure> rubyMeasure = measure->optionalCast<RubyMeasure>() ) {
            instanceName = toQString(rubyMeasure->name());
          }
        }
        QDomElement instanceNameElem = doc.createElement("instance_name");
        alternativeMeasure.appendChild(instanceNameElem);
        instanceNameElem.appendChild(doc.createTextNode(instanceName));

        //report
        QDomElement reportElem = doc.createElement("report");
        alternativeMeasure.appendChild(reportElem);
      
        //initial_condition
        std::string initCondMsg = "";
        std::vector<std::string> initConds =  jobErrs.initialConditions();
        Q_FOREACH(std::string initCond, initConds) {
          boost::regex re("^\\[.*\\]");
          initCond = boost::regex_replace(initCond,re,"");
          initCondMsg += initCond;
        }
        QDomElement initCondElem = doc.createElement("initial_condition");
        reportElem.appendChild(initCondElem);
        initCondElem.appendChild(doc.createTextNode(toQString(initCondMsg)));        
  
        //final_condition
        std::string finCondMsg = "";
        std::vector<std::string> finConds =  jobErrs.finalConditions();
        Q_FOREACH(std::string finCond, finConds) {
          boost::regex re("^\\[.*\\]");
          finCond = boost::regex_replace(finCond,re,"");
          finCondMsg += finCond;
        }
        QDomElement finCondElem = doc.createElement("final_condition");
        reportElem.appendChild(finCondElem);
        finCondElem.appendChild(doc.createTextNode(toQString(finCondMsg)));        

        return alternativeMeasure;
      }
    }

    return boost::none;
  }

boost::optional<QDomElement> ExportXML::exportCashFlow(QDomDocument& doc, 
                                                        std::map<std::string, double>& cashFlowMap, 
                                                        std::string cashFlowTypeStr)
  {
    //start the cash flow
    QDomElement cashFlow = doc.createElement("cash_flow");
    //cash flow type
    QDomElement cashFlowTypeElem = doc.createElement("type");
    cashFlow.appendChild(cashFlowTypeElem);
    cashFlowTypeElem.appendChild(doc.createTextNode(toQString(cashFlowTypeStr)));
    //values
    QDomElement valuesElem = doc.createElement("values");
    cashFlow.appendChild(valuesElem);
    //value for each year
    std::map<std::string,double>::iterator iter;
    for (iter = cashFlowMap.begin(); iter != cashFlowMap.end(); ++iter) {
      QDomElement yearValueElem = doc.createElement("year_value");
      valuesElem.appendChild(yearValueElem); 
      QString annCashVal = QString::number(iter->second);
      yearValueElem.appendChild(doc.createTextNode(annCashVal)); 
    }   

    return cashFlow;

  }

} // exportxml
} // analysis
} // openstudio
