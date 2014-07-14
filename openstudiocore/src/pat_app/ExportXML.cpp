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


#include "ExportXML.hpp"
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include "ExportXMLDialog.hpp"
#include "PatApp.hpp"
#include "PatMainWindow.hpp"

#include "../analysisdriver/SimpleProject.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/Problem.hpp"
#include "../analysis/InputVariable.hpp"
#include "../analysis/InputVariable_Impl.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../analysis/RubyMeasure_Impl.hpp"
#include "../analysis/MeasureGroup.hpp"
#include "../analysis/MeasureGroup_Impl.hpp"
#include "../analysis/WorkflowStep.hpp"

#include "../runmanager/lib/Job.hpp"
#include "../runmanager/lib/Job_Impl.hpp"

#include "../model/Model.hpp"
#include "../model/Building.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/UnitFactory.hpp"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <QMessageBox>

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
  //get the analysis from the project
  Analysis analysis = project.analysis();

  //get all the alternatives
  DataPointVector dataPoints = analysis.dataPoints();
  std::vector<std::string> alternativeNames;
  for (const DataPoint& datapoint : dataPoints) {
    alternativeNames.push_back(datapoint.name());
  } 

  // open modal dialog
  QSharedPointer<ExportXMLDialog> dialog(new ExportXMLDialog(alternativeNames));
  //ExportXMLDialog dialog = new ExportXMLDialog(alternativeNames);

  std::string edaBaselineName = "No EDA Baseline";
  std::string proposedBaselineName = "No Proposed Baseline";
  std::string certificationBaselineName = "No Certification Baseline";

  if (dialog->exec()){
    // get the user selections
    edaBaselineName = dialog->getEDABaseline();
    proposedBaselineName = dialog->getProposedBaseline();
    certificationBaselineName = dialog->getCertificationBaseline();
  }else{
    // user canceled, stop the export process
    return false;
  }

  //start the xml file
  QDomDocument doc;
  doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    
  //analysis (first element)
  QDomElement analysisElem = doc.createElement("analysis");
  doc.appendChild(analysisElem);
  analysisElem.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");    

  //simulation_software
  QDomElement softwareElem = doc.createElement("simulation_software");
  analysisElem.appendChild(softwareElem);
  QString softwareName = QString("OpenStudio");
  softwareElem.appendChild(doc.createTextNode(softwareName));

  //get the problem from the project
  Problem problem = analysis.problem();

  //get the input variables from the problem
  //std::vector<InputVariable> variables = problem.variables();

  //alternatives
  int numErrors = 0;
  QString errors;
  QDomElement alternativesElem = doc.createElement("alternatives");
  analysisElem.appendChild(alternativesElem);
  for (const DataPoint& datapoint : dataPoints) {
    //export the alternative if it has the necessary results
    if ( boost::optional<Attribute> reportAttr = datapoint.getOutputAttribute("report")) {
      std::vector<WorkflowStepJob> jobs = problem.getJobsByWorkflowStep(datapoint, true);
      if ( boost::optional<QDomElement> alternativeElem = exportAlternative(doc, *reportAttr, datapoint, jobs, edaBaselineName, proposedBaselineName, certificationBaselineName) ) {
        alternativesElem.appendChild(*alternativeElem);
      }
    }else{
      numErrors += 1;
      errors += toQString(datapoint.name());
    }
    // drop this DataPoint's Model and SqlFile from memory
    datapoint.clearFileDataFromCache();
  }    

  //warn the user if summary results were missing
  if ( numErrors > 0 ) {
    QMessageBox::warning(pat::PatApp::instance()->mainWindow, "Export XML Report - Warnings", errors + QString(" will be skipped; summary results not available.  Make sure you've run the reporting measure 'Xcel EDA Reporting and QAQC'"));
  }

  QFile file(xmlFilePath);
  if (file.open(QFile::WriteOnly)){
    QTextStream textStream(&file);
    textStream << doc.toString(2);
    file.close();
    return true;
  }

  return false;

}
  
boost::optional<QDomElement> ExportXML::exportAlternative(QDomDocument& doc, 
                                                          Attribute& alternativeAttr,
                                                          analysis::DataPoint dataPt,
                                                          std::vector<WorkflowStepJob>& jobs,
                                                          std::string edaBaselineName,
                                                          std::string proposedBaselineName,
                                                          std::string certificationBaselineName)
{
  //start the alternative
  QDomElement alternative = doc.createElement("alternative");

  //name
  QDomElement nameElem = doc.createElement("name");
  alternative.appendChild(nameElem);
  QString name = toQString(dataPt.name());
  nameElem.appendChild(doc.createTextNode(name));
  
  //baseline type
  if ( dataPt.name() == edaBaselineName ) {
    QDomElement nameElem = doc.createElement("baseline_type");
    alternative.appendChild(nameElem);
    nameElem.appendChild(doc.createTextNode("EDA Baseline"));
  }
  
  if ( dataPt.name() == proposedBaselineName ) {
    QDomElement nameElem = doc.createElement("baseline_type");
    alternative.appendChild(nameElem);
    nameElem.appendChild(doc.createTextNode("Proposed Baseline"));
  }
  
  if ( dataPt.name() == certificationBaselineName ) {
    QDomElement nameElem = doc.createElement("baseline_type");
    alternative.appendChild(nameElem);
    nameElem.appendChild(doc.createTextNode("Certification Baseline"));
  }

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
  
  //information stored under "results"
  if ( boost::optional<Attribute> oresultsAttr = alternativeAttr.findChildByName("results") ) {
    Attribute resultsAttr = *oresultsAttr;

    //floor_area
    QDomElement floorAreaElem = doc.createElement("floor_area");
    buildingElem.appendChild(floorAreaElem);
    if ( boost::optional<Attribute> condArea = resultsAttr.findChildByName("floor_area") ) {
      QString floorArea = QString::number(condArea->valueAsDouble());
      floorAreaElem.appendChild(doc.createTextNode(floorArea));
    }
    else {
      QString floorArea = "Building has no conditioned floor area";
      floorAreaElem.appendChild(doc.createTextNode(floorArea));
    }  

    //number of measures in a design alt. differentiates whether it is called a measure
    //or a design alternative in Xcel EDA
    double numMeasures = 0.0;

    //measures
    QDomElement measuresElem = doc.createElement("measures");
    alternative.appendChild(measuresElem);
    for (const WorkflowStepJob& job : jobs) {
      //record the details of the measure in the xml.
      if (boost::optional<QDomElement> measureElem = exportMeasure(doc, job)){
        measuresElem.appendChild(*measureElem);
        numMeasures += 1;
      }
    }
    
    //alternative_type
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

    //results
    QDomElement resultsElem = doc.createElement("results");
    alternative.appendChild(resultsElem);

    //cash flows
    QDomElement cashFlowsElem = doc.createElement("cash_flows");
    resultsElem.appendChild(cashFlowsElem);
    if ( boost::optional<Attribute> cashFlowsAttr = resultsAttr.findChildByName("cash_flows") ) {
      for ( const Attribute & cashFlowAttr : cashFlowsAttr->valueAsAttributeVector()) {
        if ( boost::optional<QDomElement> cashFlowElem = exportCashFlow(doc, cashFlowAttr) ){
          cashFlowsElem.appendChild(*cashFlowElem);
        }       
      }
    }

    //annual
    if ( boost::optional<Attribute> annualAttr = resultsAttr.findChildByName("annual") ) {
      if ( boost::optional<QDomElement> annualElem = exportAnnual(doc, *annualAttr) ){
        resultsElem.appendChild(*annualElem);
      }       
    }
      
    //monthly
    if ( boost::optional<Attribute> monthlyAttr = resultsAttr.findChildByName("monthly") ) {
      if ( boost::optional<QDomElement> monthlyElem = exportMonthly(doc, *monthlyAttr) ){
        resultsElem.appendChild(*monthlyElem);
      }       
    }

    //checks
    if ( boost::optional<Attribute> checksAttr = alternativeAttr.findChildByName("checks") ) {
      if ( boost::optional<QDomElement> checksElem = exportChecks(doc, *checksAttr) ){
        alternative.appendChild(*checksElem);
      }       
    }

  }

  return alternative;
}

boost::optional<QDomElement> ExportXML::exportMeasure(QDomDocument& doc,
                                                      const WorkflowStepJob& wfJob)
{
      
    WorkflowStep step = wfJob.step;

    //skip 
    if ( step.isInputVariable() == false ){
      return boost::none;
    }

    //skip fixed measures (present in all design alternatives)
    if (boost::optional<analysis::Measure> measure = wfJob.measure) {
      if (boost::optional<analysis::AnalysisObject> parObj = measure->parent()) {
        if (boost::optional<MeasureGroup> discVar = parObj->optionalCast<MeasureGroup>()) {
          if (discVar->measures(false).size() == 1) {
            return boost::none;
          }
        }
      }
    }

    //skip reporting measures
    if (boost::optional<analysis::Measure> measure = wfJob.measure) {
      if ( boost::optional<RubyMeasure> rubyMeasure = measure->optionalCast<RubyMeasure>() ) {
        if ( boost::optional<BCLMeasure> bclMeasure = rubyMeasure->measure() ) {
          if (bclMeasure->measureType() == MeasureType::ReportingMeasure ) {
            return boost::none;
          }
        }
      }
    }

    //skip post-process, pre-process, and energyplus jobs
    if ( step.isInputVariable()){
      boost::optional<runmanager::Job> job = wfJob.job;
      if (job == boost::none) {
        return boost::none;
      }
    }

    //this is a measure we want to record
    if (wfJob.job) {
    
      //start the measure
      QDomElement measureElem = doc.createElement("measure");   
           
      if ( boost::optional<Measure> measure = wfJob.measure ) {
          
        //name
        QString measureName = "unknown";
        if ( boost::optional<RubyMeasure> rubyMeasure = measure->optionalCast<RubyMeasure>() ) {
          measureName = toQString(rubyMeasure->name());
        }
        QDomElement instanceNameElem = doc.createElement("name");
        measureElem.appendChild(instanceNameElem);
        instanceNameElem.appendChild(doc.createTextNode(measureName));        
          
        //get information out of the measure
        QString measureId = "uuid unknown";
        QString category = "category unknown";
        if ( boost::optional<RubyMeasure> rubyMeasure = measure->optionalCast<RubyMeasure>() ) {
          if ( boost::optional<BCLMeasure> bclMeasure = rubyMeasure->measure() ) {
            measureId = toQString(bclMeasure->uid());
            category = toQString(bclMeasure->taxonomyTag());
          }
        }
        //id
        QDomElement measureIdElem = doc.createElement("id");
        measureElem.appendChild(measureIdElem);
        measureIdElem.appendChild(doc.createTextNode(measureId));

        //category
        QDomElement categoryElem = doc.createElement("category");
        measureElem.appendChild(categoryElem);
        categoryElem.appendChild(doc.createTextNode(category));
          
        //description
        QDomElement descElem = doc.createElement("description");
        measureElem.appendChild(descElem);
        QString desc = toQString(measure->description());
        descElem.appendChild(doc.createTextNode(desc));          
      }

      //get the job errors
      runmanager::JobErrors jobErrs = wfJob.errors().get();
      
      //initial_condition
      std::string initCondMsg = "";
      std::vector<std::string> initConds =  jobErrs.initialConditions();
      for (std::string initCond : initConds) {
        boost::regex re("^\\[.*\\]");
        initCond = boost::regex_replace(initCond,re,"");
        initCondMsg += initCond;
      }
      QDomElement initCondElem = doc.createElement("initial_condition");
      measureElem.appendChild(initCondElem);
      initCondElem.appendChild(doc.createTextNode(toQString(initCondMsg)));        
  
      //final_condition
      std::string finCondMsg = "";
      std::vector<std::string> finConds =  jobErrs.finalConditions();
      for (std::string finCond : finConds) {
        boost::regex re("^\\[.*\\]");
        finCond = boost::regex_replace(finCond,re,"");
        finCondMsg += finCond;
      }
      QDomElement finCondElem = doc.createElement("final_condition");
      measureElem.appendChild(finCondElem);
      finCondElem.appendChild(doc.createTextNode(toQString(finCondMsg)));        

      return measureElem;
  }

    return boost::none;

}


boost::optional<QDomElement> ExportXML::exportCashFlow(QDomDocument& doc, 
                                                       const Attribute& cashFlowAttr)
  {
    //start the cash flow
    QDomElement cashFlowElem = doc.createElement("cash_flow");
    
    //values
    QDomElement valuesElem = doc.createElement("values");
    for ( const Attribute & cashFlowValAttr : cashFlowAttr.valueAsAttributeVector()) {
      if ( cashFlowValAttr.name() == "type" ) {
        //cash flow type
        QDomElement cashFlowTypeElem = doc.createElement("type");
        cashFlowElem.appendChild(cashFlowTypeElem);
        cashFlowTypeElem.appendChild(doc.createTextNode(toQString(cashFlowValAttr.valueAsString())));
      }
      if ( cashFlowValAttr.name() == "year" ) {
        QDomElement yearValueElem = doc.createElement("year");
        valuesElem.appendChild(yearValueElem); 
        QString annCashVal = QString::number(cashFlowValAttr.valueAsDouble() );
        yearValueElem.appendChild(doc.createTextNode(annCashVal)); 
      }
    }
    cashFlowElem.appendChild(valuesElem);  

    return cashFlowElem;

  }

boost::optional<QDomElement> ExportXML::exportAnnual(QDomDocument& doc,
                                                      const Attribute& annualAttr)
{

  //start the annual results
  QDomElement annualElem = doc.createElement("annual");
    
  //consumption
  if ( boost::optional<Attribute> consumptionAttr = annualAttr.findChildByName("consumption") ) {

    QDomElement consumptionElem = doc.createElement("consumption");
    annualElem.appendChild(consumptionElem);

    for ( const Attribute & consumptionAttrVal : consumptionAttr->valueAsAttributeVector()) {

      //gas
      if ( consumptionAttrVal.name() == "gas" ) {
        QDomElement fuelTypeElem = doc.createElement("gas");
        consumptionElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(consumptionAttrVal.valueAsDouble())));
      }

      //electricity
      if ( consumptionAttrVal.name() == "electricity" ) {
        QDomElement fuelTypeElem = doc.createElement("electricity");
        consumptionElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(consumptionAttrVal.valueAsDouble())));
      }

      //other_energy
      if ( consumptionAttrVal.name() == "other_energy" ) {
        QDomElement fuelTypeElem = doc.createElement("other_energy");
        consumptionElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(consumptionAttrVal.valueAsDouble())));
      }

      //district_heating
      if ( consumptionAttrVal.name() == "district_heating" ) {
        QDomElement fuelTypeElem = doc.createElement("district_heating");
        consumptionElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(consumptionAttrVal.valueAsDouble())));
      }

      //district_cooling
      if ( consumptionAttrVal.name() == "district_cooling" ) {
        QDomElement fuelTypeElem = doc.createElement("district_cooling");
        consumptionElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(consumptionAttrVal.valueAsDouble())));
      }

      //water
      if ( consumptionAttrVal.name() == "water" ) {
        QDomElement fuelTypeElem = doc.createElement("water");
        consumptionElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(consumptionAttrVal.valueAsDouble())));
      }

    }
  } //end consumption

  //demand
  if ( boost::optional<Attribute> demandAttr = annualAttr.findChildByName("demand") ) {

    QDomElement demandElem = doc.createElement("demand");
    annualElem.appendChild(demandElem);

    for ( const Attribute & demandAttrVal : demandAttr->valueAsAttributeVector()) {

      //electricity_peak_demand
      if ( demandAttrVal.name() == "electricity_peak_demand" ) {
        QDomElement demandTypeElem = doc.createElement("electricity_peak_demand");
        demandElem.appendChild(demandTypeElem);
        demandTypeElem.appendChild(doc.createTextNode(QString::number(demandAttrVal.valueAsDouble())));
      }

      //electricity_annual_avg_peak_demand
      if ( demandAttrVal.name() == "electricity_annual_avg_peak_demand" ) {
        QDomElement demandTypeElem = doc.createElement("electricity_annual_avg_peak_demand");
        demandElem.appendChild(demandTypeElem);
        demandTypeElem.appendChild(doc.createTextNode(QString::number(demandAttrVal.valueAsDouble())));
      }

      //district_cooling_peak_demand
      if ( demandAttrVal.name() == "district_cooling_peak_demand" ) {
        QDomElement demandTypeElem = doc.createElement("district_cooling_peak_demand");
        demandElem.appendChild(demandTypeElem);
        demandTypeElem.appendChild(doc.createTextNode(QString::number(demandAttrVal.valueAsDouble())));
      }

    }
  } //end demand

  //utility_cost
  if ( boost::optional<Attribute> utilityCostAttr = annualAttr.findChildByName("utility_cost") ) {

    QDomElement utilityCostElem = doc.createElement("utility_cost");
    annualElem.appendChild(utilityCostElem);

    for ( const Attribute & utilityCostAttrVal : utilityCostAttr->valueAsAttributeVector()) {

      //electricity
      if ( utilityCostAttrVal.name() == "electricity" ) {
        QDomElement fuelTypeElem = doc.createElement("electricity");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //electricity_consumption_charge
      if ( utilityCostAttrVal.name() == "electricity_consumption_charge" ) {
        QDomElement fuelTypeElem = doc.createElement("electricity_consumption_charge");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //electricity_demand_charge
      if ( utilityCostAttrVal.name() == "electricity_demand_charge" ) {
        QDomElement fuelTypeElem = doc.createElement("electricity_demand_charge");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //gas
      if ( utilityCostAttrVal.name() == "gas" ) {
        QDomElement fuelTypeElem = doc.createElement("gas");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //other_energy
      if ( utilityCostAttrVal.name() == "other_energy" ) {
        QDomElement fuelTypeElem = doc.createElement("other_energy");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //district_heating
      if ( utilityCostAttrVal.name() == "district_heating" ) {
        QDomElement fuelTypeElem = doc.createElement("district_heating");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //district_cooling
      if ( utilityCostAttrVal.name() == "district_cooling" ) {
        QDomElement fuelTypeElem = doc.createElement("district_cooling");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //water
      if ( utilityCostAttrVal.name() == "water" ) {
        QDomElement fuelTypeElem = doc.createElement("water");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //total
      if ( utilityCostAttrVal.name() == "total" ) {
        QDomElement fuelTypeElem = doc.createElement("total");
        utilityCostElem.appendChild(fuelTypeElem);
        fuelTypeElem.appendChild(doc.createTextNode(QString::number(utilityCostAttrVal.valueAsDouble())));
      }

      //end_uses
      if ( utilityCostAttrVal.name() == "end_uses" ) {
        QDomElement endUsesElem = doc.createElement("end_uses");
        utilityCostElem.appendChild(endUsesElem);

        for ( const Attribute & endUsesAttrVal : utilityCostAttrVal.valueAsAttributeVector()) {
          QDomElement endUseElem = doc.createElement(toQString(endUsesAttrVal.name()));
          endUsesElem.appendChild(endUseElem);
          endUseElem.appendChild(doc.createTextNode(QString::number(endUsesAttrVal.valueAsDouble())));
        }  
        
      } //end end_uses

    }
  } //end utility_cost

  return annualElem;
}
  
boost::optional<QDomElement> ExportXML::exportMonthly(QDomDocument& doc,
                                                      const Attribute& monthlyAttr)
  {
      
  //start the monthly results
  QDomElement monthlyElem = doc.createElement("monthly");
  
  //consumption
  if ( boost::optional<Attribute> consumptionAttr = monthlyAttr.findChildByName("consumption") ) {
    QDomElement consumptionElem = doc.createElement("consumption");
    monthlyElem.appendChild(consumptionElem);
    //loop through all end uses
    for ( const Attribute & endUseAttr : consumptionAttr->valueAsAttributeVector()) {
      QDomElement endUseElem = doc.createElement(toQString(endUseAttr.name()));
      consumptionElem.appendChild(endUseElem);
      //loop through all fuel types in this end use
      for ( const Attribute & fuelTypeAttr : endUseAttr.valueAsAttributeVector()) {
        QDomElement fuelTypeElem = doc.createElement(toQString(fuelTypeAttr.name()));          
        endUseElem.appendChild(fuelTypeElem);
        //loop through all months in this fuel type in this end use
        for ( const Attribute & monthAttr : fuelTypeAttr.valueAsAttributeVector()) {
          QDomElement monthElem = doc.createElement(toQString(monthAttr.name()));          
          fuelTypeElem.appendChild(monthElem);
          monthElem.appendChild(doc.createTextNode(QString::number(monthAttr.valueAsDouble())));
        }  
      }
    }
  }

  //demand
  if ( boost::optional<Attribute> demandAttr = monthlyAttr.findChildByName("demand") ) {
    QDomElement demandElem = doc.createElement("demand");
    monthlyElem.appendChild(demandElem);
    //loop through all end uses
    for ( const Attribute & endUseAttr : demandAttr->valueAsAttributeVector()) {
      QDomElement endUseElem = doc.createElement(toQString(endUseAttr.name()));
      demandElem.appendChild(endUseElem);
      //loop through all fuel types in this end use
      for ( const Attribute & fuelTypeAttr : endUseAttr.valueAsAttributeVector()) {
        QDomElement fuelTypeElem = doc.createElement(toQString(fuelTypeAttr.name()));          
        endUseElem.appendChild(fuelTypeElem);
        //loop through all months in this fuel type in this end use
        for ( const Attribute & monthAttr : fuelTypeAttr.valueAsAttributeVector()) {
          QDomElement monthElem = doc.createElement(toQString(monthAttr.name()));          
          fuelTypeElem.appendChild(monthElem);
          monthElem.appendChild(doc.createTextNode(QString::number(monthAttr.valueAsDouble())));
        }  
      }
    }
  }

  return monthlyElem;
} 

boost::optional<QDomElement> ExportXML::exportChecks(QDomDocument& doc,
                                                      const Attribute& checksAttr)
  {
      
  //checks
  QDomElement checksElem = doc.createElement("checks");
  for ( const Attribute & checkAttr : checksAttr.valueAsAttributeVector()) {
    if ( checkAttr.name() == "check" ) {
      QDomElement checkElem = doc.createElement("check");
      checksElem.appendChild(checkElem);
      for ( const Attribute & checkValAttr : checkAttr.valueAsAttributeVector()) {
        if ( checkValAttr.name() == "name" ) {
          QDomElement elem = doc.createElement("name");
          checkElem.appendChild(elem);
          elem.appendChild(doc.createTextNode(toQString(checkValAttr.valueAsString())));
        }
        if ( checkValAttr.name() == "description" ) {
          QDomElement elem = doc.createElement("description");
          checkElem.appendChild(elem); 
          elem.appendChild(doc.createTextNode(toQString(checkValAttr.valueAsString()))); 
        }
        if ( checkValAttr.name() == "category" ) {
          QDomElement elem = doc.createElement("category");
          checkElem.appendChild(elem); 
          elem.appendChild(doc.createTextNode(toQString(checkValAttr.valueAsString()))); 
        }
        if ( checkValAttr.name() == "flag" ) {
          QDomElement elem = doc.createElement("flag");
          checkElem.appendChild(elem); 
          elem.appendChild(doc.createTextNode(toQString(checkValAttr.valueAsString()))); 
        }
      }
    }
  }

  return checksElem;
} 


} // exportxml
} // analysis
} // openstudio
