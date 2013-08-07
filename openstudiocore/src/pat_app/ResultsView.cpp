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

#include <pat_app/ResultsView.hpp>

#include <pat_app/PatApp.hpp>

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include <analysis/Analysis.hpp>
#include <analysis/Analysis_Impl.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscretePerturbation_Impl.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/NullPerturbation_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Problem_Impl.hpp>
#include <analysis/Variable.hpp>
#include <analysis/Variable_Impl.hpp>

#include <analysisdriver/AnalysisDriver.hpp>
#include <analysisdriver/AnalysisDriver_Impl.hpp>
#include <analysisdriver/SimpleProject.hpp>

#include <project/AnalysisRecord.hpp>
#include <project/AnalysisRecord_Impl.hpp>

#include <utilities/data/Attribute.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/Unit.hpp>
#include <utilities/units/UnitFactory.hpp>

#include <QBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QStringList>
#include <QStyleOption>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

#define NAME_LABEL_WIDTH 90
#define SPACER_WIDTH 3
#define RESULT_LABEL_WIDTH 90
#define RESULT_DIV_LABEL_WIDTH 5
#define RESULT_PCT_LABEL_WIDTH 40
#define SCROLL_SPACER_WIDTH 18

namespace openstudio {
  
namespace pat {

ResultsView::ResultsView()
  : PatMainTabView()
{
  setTitle("Create and View Reports");

  // Main Content

  QWidget * mainContent = new QWidget();

  QVBoxLayout * mainContentVLayout = new QVBoxLayout();
  mainContentVLayout->setContentsMargins(0,0,0,0);
  mainContentVLayout->setSpacing(0);
  mainContentVLayout->setAlignment(Qt::AlignTop);
  mainContent->setLayout(mainContentVLayout);

  QVBoxLayout * innerContentVLayout = new QVBoxLayout();
  innerContentVLayout->setContentsMargins(5,5,5,5);
  innerContentVLayout->setSpacing(5);
  innerContentVLayout->setAlignment(Qt::AlignTop);
  mainContentVLayout->addLayout(innerContentVLayout);

  viewSwitcher->setView(mainContent);

  // Baseline header
  QWidget * baselineHeader = new ResultsHeader(true);
  innerContentVLayout->addWidget(baselineHeader);

  // Baseline list
  baselineDataPointResultListView = new OSListView();
  baselineDataPointResultListView->setSpacing(0);
  baselineDataPointResultListView->setContentsMargins(1,1,1,1);
  baselineDataPointResultListView->setStyleSheet("openstudio--OSListView { border: 1px solid black; }");

  QHBoxLayout * baselineLayout = new QHBoxLayout();
  baselineLayout->setContentsMargins(0,0,0,0);
  baselineLayout->setSpacing(0);
  baselineLayout->addWidget(baselineDataPointResultListView);

  innerContentVLayout->addLayout(baselineLayout);

  // Results header
  QWidget * resultsHeader = new ResultsHeader(false);
  innerContentVLayout->addWidget(resultsHeader);

  // Results list
  dataPointResultsListView = new OSListView(true);
  dataPointResultsListView->setVerticalScrollBarAlwaysOn(true);
  dataPointResultsListView->setSpacing(0);
  dataPointResultsListView->setContentsMargins(1,1,1,1);
  dataPointResultsListView->setStyleSheet("openstudio--OSListView { border: 1px solid black; }");
  innerContentVLayout->addWidget(dataPointResultsListView);

  bool isConnected = false;

  QWidget * footer = new QWidget();
  footer->setObjectName("Footer");
  QString style;
  style.append("QWidget#Footer {");
  style.append("border-top: 1px solid black; ");
  style.append("background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop: 0 #B6B5B6, stop: 1 #737172); ");
  style.append("}");
  footer->setStyleSheet(style);

  // Open File Button
  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(5,5,5,5);
  hLayout->setSpacing(10);
  footer->setLayout(hLayout);

  m_viewFileButton = new GrayButton();
  m_viewFileButton->setFixedHeight(42);
  m_viewFileButton->setEnabled(false);
  m_viewFileButton->setText("Open a COPY of the Selected File\nin the OpenStudio Application");
  hLayout->addWidget(m_viewFileButton);
  
  isConnected = connect(m_viewFileButton, SIGNAL(clicked(bool)), this, SIGNAL(openButtonClicked(bool)));
  OS_ASSERT(isConnected);

  m_openDirButton = new OpenDirectoryButton(this);
  m_openDirButton->setToolTip("Open the directory for the selected file.");
  m_openDirButton->setEnabled(false);
  hLayout->addWidget(m_openDirButton);

  isConnected = connect(m_openDirButton, SIGNAL(clicked(bool)), this, SIGNAL(openDirButtonClicked(bool)));
  OS_ASSERT(isConnected);
  
  hLayout->addStretch();
  mainContentVLayout->addWidget(footer);
}

void ResultsView::enableViewFileButton(bool enable)
{
  m_viewFileButton->setEnabled(enable);
}

void ResultsView::enableOpenDirectoryButton(bool enable)
{
  m_openDirButton->setEnabled(enable);
}

ResultsHeader::ResultsHeader(bool isBaseline)
  : QWidget()
{
  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0,0,0,0);
  hLayout->setSpacing(0);
  this->setLayout(hLayout);

  QString style("QLabel {font-size: 12px; font: bold; color: black;}");

  QLabel* nameLabel = new QLabel();
  nameLabel->setStyleSheet(style);
  nameLabel->setMinimumWidth(NAME_LABEL_WIDTH);
  //nameLabel->setFixedWidth(NAME_LABEL_WIDTH);
  nameLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(nameLabel);

  QLabel* space = new QLabel();
  space->setFixedWidth(SPACER_WIDTH);
  space->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(space);

  QLabel* netSiteEnergyIntensityLabel = new QLabel();
  netSiteEnergyIntensityLabel->setStyleSheet(style);
  netSiteEnergyIntensityLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  netSiteEnergyIntensityLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(netSiteEnergyIntensityLabel);

  space = new QLabel();
  space->setFixedWidth(SPACER_WIDTH);
  space->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(space);

  QLabel* peakElectricDemandLabel = new QLabel();
  peakElectricDemandLabel->setStyleSheet(style);
  peakElectricDemandLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  peakElectricDemandLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(peakElectricDemandLabel);

  space = new QLabel();
  space->setFixedWidth(SPACER_WIDTH);
  space->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(space);

  QLabel* electricityLabel = new QLabel();
  electricityLabel->setStyleSheet(style);
  electricityLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  electricityLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(electricityLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  space->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(space);

  QLabel* naturalGasLabel = new QLabel();
  naturalGasLabel->setStyleSheet(style);
  naturalGasLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  naturalGasLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(naturalGasLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  space->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(space);

  QLabel* districtCoolingLabel = new QLabel();
  districtCoolingLabel->setStyleSheet(style);
  districtCoolingLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  districtCoolingLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(districtCoolingLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  space->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(space);

  QLabel* districtHeatingLabel = new QLabel();
  districtHeatingLabel->setStyleSheet(style);
  districtHeatingLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  districtHeatingLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(districtHeatingLabel);

  QLabel* capitalCostLabel = new QLabel();
  capitalCostLabel->setStyleSheet(style);
  capitalCostLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  capitalCostLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(capitalCostLabel);

  QLabel* energyCostLabel = new QLabel();
  energyCostLabel->setStyleSheet(style);
  energyCostLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  energyCostLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(energyCostLabel);

  QLabel* m_simplePayBackLabel = new QLabel();
  m_simplePayBackLabel->setStyleSheet(style);
  m_simplePayBackLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  m_simplePayBackLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(m_simplePayBackLabel);

  QLabel* m_lifeCycleCostLabel = new QLabel();
  m_lifeCycleCostLabel->setStyleSheet(style);
  m_lifeCycleCostLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  m_lifeCycleCostLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  hLayout->addWidget(m_lifeCycleCostLabel);

  QLabel* scrollSpacer = new QLabel();
  hLayout->addWidget(scrollSpacer);

  if (isBaseline){
    nameLabel->setText("Design\nAlternative\nName");
    netSiteEnergyIntensityLabel->setText("Energy Use\nIntensity\n(kBtu/ft2-yr)");
    peakElectricDemandLabel->setText("Peak Electric\nDemand\n(kW)");
    electricityLabel->setText("Electricity\nConsumption\n(kWh)");
    naturalGasLabel->setText("Natural Gas\nConsumption\n(Million Btu)");
    districtCoolingLabel->setText("District\nCooling\nConsumption\n(Million Btu)");
    districtHeatingLabel->setText("District\nHeating\nConsumption\n(Million Btu)");
    capitalCostLabel->setText("First Year\nCapital Cost\n($)");
    energyCostLabel->setText("Annual Utility\nCost\n($)");
    m_simplePayBackLabel->setText("");
    m_lifeCycleCostLabel->setText("Total LCC\n($)");

    scrollSpacer->setFixedWidth(SCROLL_SPACER_WIDTH);
  }else{
    nameLabel->setText("Design\nAlternative\nName");
    netSiteEnergyIntensityLabel->setText("Energy Use\nIntensity\nReduction\n(kBtu/ft2-yr)");
    peakElectricDemandLabel->setText("Peak Electric\nDemand\nReduction\n(kW)");
    electricityLabel->setText("Electricity\nSavings\n(kWh)");
    naturalGasLabel->setText("Natural Gas\nSavings\n(Million Btu)");
    districtCoolingLabel->setText("District\nCooling\nSavings\n(Million Btu)");
    districtHeatingLabel->setText("District\nHeating\nSavings\n(Million Btu)");
    capitalCostLabel->setText("First Year\nCapital Cost\nIncrease\n($)");
    energyCostLabel->setText("Annual Utility\nCost\nSavings\n($)");
    m_simplePayBackLabel->setText("Simple\nPayback\n(years)");
    m_lifeCycleCostLabel->setText("Total LCC\nSavings\n($)");

    scrollSpacer->setFixedWidth(SCROLL_SPACER_WIDTH);
  }

}

DataPointResultsView::DataPointResultsView(const openstudio::analysis::DataPoint& dataPoint,
                                           const openstudio::analysis::DataPoint& baselineDataPoint, 
                                           bool alternateRow)
  : QAbstractButton(), m_dataPoint(dataPoint), m_baselineDataPoint(baselineDataPoint), m_alternateRow(alternateRow)
{
  this->setMinimumHeight(75);

  QHBoxLayout* hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(0,0,0,0);
  hLayout->setSpacing(0);
  this->setLayout(hLayout);

  QString style("QLabel { border-right: 1px solid black; color: black; }");

  m_nameLabel = new QLabel();
  m_nameLabel->setWordWrap(true);
  m_nameLabel->setStyleSheet(style);
  m_nameLabel->setMinimumWidth(NAME_LABEL_WIDTH);
  //m_nameLabel->setFixedWidth(NAME_LABEL_WIDTH);
  hLayout->addWidget(m_nameLabel);

  QLabel* space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_netSiteEnergyIntensityLabel = new QLabel();
  m_netSiteEnergyIntensityLabel->setStyleSheet(style);
  hLayout->addWidget(m_netSiteEnergyIntensityLabel);

  m_netSiteEnergyIntensityDivLabel = new QLabel();
  m_netSiteEnergyIntensityDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_netSiteEnergyIntensityDivLabel);

  m_netSiteEnergyIntensityPctLabel = new QLabel();
  m_netSiteEnergyIntensityPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_netSiteEnergyIntensityPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_peakElectricDemandLabel = new QLabel();
  m_peakElectricDemandLabel->setStyleSheet(style);
  hLayout->addWidget(m_peakElectricDemandLabel);

  m_peakElectricDemandDivLabel = new QLabel();
  m_peakElectricDemandDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_peakElectricDemandDivLabel);

  m_peakElectricDemandPctLabel = new QLabel();
  m_peakElectricDemandPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_peakElectricDemandPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_electricityLabel = new QLabel();
  m_electricityLabel->setStyleSheet(style);
  hLayout->addWidget(m_electricityLabel);

  m_electricityDivLabel = new QLabel();
  m_electricityDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_electricityDivLabel);

  m_electricityPctLabel = new QLabel();
  m_electricityPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_electricityPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_naturalGasLabel = new QLabel();
  m_naturalGasLabel->setStyleSheet(style);
  hLayout->addWidget(m_naturalGasLabel);

  m_naturalGasDivLabel = new QLabel();
  m_naturalGasDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_naturalGasDivLabel);

  m_naturalGasPctLabel = new QLabel();
  m_naturalGasPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_naturalGasPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_districtCoolingLabel = new QLabel();
  m_districtCoolingLabel->setStyleSheet(style);
  hLayout->addWidget(m_districtCoolingLabel);

  m_districtCoolingDivLabel = new QLabel();
  m_districtCoolingDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_districtCoolingDivLabel);

  m_districtCoolingPctLabel = new QLabel();
  m_districtCoolingPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_districtCoolingPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_districtHeatingLabel = new QLabel();
  m_districtHeatingLabel->setStyleSheet(style);
  hLayout->addWidget(m_districtHeatingLabel);

  m_districtHeatingDivLabel = new QLabel();
  m_districtHeatingDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_districtHeatingDivLabel);

  m_districtHeatingPctLabel = new QLabel();
  m_districtHeatingPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_districtHeatingPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_capitalCostLabel = new QLabel();
  m_capitalCostLabel->setStyleSheet(style);
  hLayout->addWidget(m_capitalCostLabel);

  m_capitalCostDivLabel = new QLabel();
  m_capitalCostDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_capitalCostDivLabel);

  m_capitalCostPctLabel = new QLabel();
  m_capitalCostPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_capitalCostPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_energyCostLabel = new QLabel();
  m_energyCostLabel->setStyleSheet(style);
  hLayout->addWidget(m_energyCostLabel);

  m_energyCostDivLabel = new QLabel();
  m_energyCostDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_energyCostDivLabel);

  m_energyCostPctLabel = new QLabel();
  m_energyCostPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_energyCostPctLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_simplePayBackLabel = new QLabel();
  m_simplePayBackLabel->setStyleSheet(style);
  hLayout->addWidget(m_simplePayBackLabel);

  space = new QLabel();
  space->setStyleSheet(style);
  space->setFixedWidth(SPACER_WIDTH);
  hLayout->addWidget(space);

  m_lifeCycleCostLabel = new QLabel();
  m_lifeCycleCostLabel->setStyleSheet(style);
  hLayout->addWidget(m_lifeCycleCostLabel);

  m_lifeCycleCostDivLabel = new QLabel();
  m_lifeCycleCostDivLabel->setStyleSheet(style);
  hLayout->addWidget(m_lifeCycleCostDivLabel);

  m_lifeCycleCostPctLabel = new QLabel();
  m_lifeCycleCostPctLabel->setStyleSheet(style);
  hLayout->addWidget(m_lifeCycleCostPctLabel);

  m_scrollSpacer = new QLabel();
  m_scrollSpacer->setStyleSheet(style);
  hLayout->addWidget(m_scrollSpacer);

  bool test = dataPoint.connect(SIGNAL(changed(ChangeType)), this, SLOT(update()));
  OS_ASSERT(test);

  test = baselineDataPoint.connect(SIGNAL(changed(ChangeType)), this, SLOT(update()));
  OS_ASSERT(test);

  update();
}

void DataPointResultsView::update()
{
  QString name;
  QString measures;

  boost::optional<double> netSiteEnergyIntensity;
  boost::optional<double> peakElectricDemand;
  boost::optional<double> peakElectricDemand_kW;
  boost::optional<double> electricity;
  boost::optional<double> naturalGas;
  boost::optional<double> districtCooling;
  boost::optional<double> districtHeating;
  boost::optional<double> capitalCost;
  boost::optional<double> capitalCostIncrease;
  boost::optional<double> energyCost;
  boost::optional<double> simplePayBack;
  boost::optional<double> lifeCycleCost;

  boost::optional<double> netSiteEnergyIntensityPct;
  boost::optional<double> peakElectricDemandPct;
  boost::optional<double> electricityPct;
  boost::optional<double> naturalGasPct;
  boost::optional<double> districtCoolingPct;
  boost::optional<double> districtHeatingPct;
  boost::optional<double> capitalCostPct;
  boost::optional<double> capitalCostPctIncrease;
  boost::optional<double> energyCostPct;
  boost::optional<double> lifeCycleCostPct;

  if (m_dataPoint.uuid() == m_baselineDataPoint.uuid()){

    name = "Baseline";
    measures = "";
    
    netSiteEnergyIntensity = getBaselineValue("Net Site Energy Use Intentsity");
    peakElectricDemand = getBaselineValue("Instantaneous Peak Electricity Demand");
    // NOTE: convert E+'s "W" value to a "kW" value
    if(peakElectricDemand){
      peakElectricDemand_kW = peakElectricDemand.get()/1000;
    }
    electricity = getBaselineValue("Electricity Total End Uses");
    naturalGas = getBaselineValue("Natural Gas Total End Uses");
    districtCooling = getBaselineValue("District Cooling Total End Uses");
    districtHeating = getBaselineValue("District Heating Total End Uses");
    capitalCost = getBaselineValue("First Year Capital Costs");
    energyCost = getBaselineValue("Annual Total Utility Cost");
    lifeCycleCost = getBaselineValue("Total Life Cycle Cost");

  }
  else{

    name = m_dataPoint.name().c_str();

    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    if(project){
      openstudio::analysis::Problem problem = project->analysis().problem();
      std::vector<QVariant> variableValues = m_dataPoint.variableValues();
      std::vector<boost::optional<analysis::DiscretePerturbation> > discretePerturbations = problem.getDiscretePerturbations(variableValues);
      Q_FOREACH(boost::optional<analysis::DiscretePerturbation> discretePerturbation, discretePerturbations){
        if (discretePerturbation){
          if (!discretePerturbation->optionalCast<analysis::NullPerturbation>()){
            measures += discretePerturbation->name().c_str();
            measures += '\n';
          }
        }
      }
    }

    measures = measures.trimmed();

    netSiteEnergyIntensity = getDifference("Net Site Energy Use Intentsity");
    peakElectricDemand = getDifference("Instantaneous Peak Electricity Demand");
    // NOTE: convert E+'s "W" value to a "kW" value
    if(peakElectricDemand){
      peakElectricDemand_kW = peakElectricDemand.get()/1000;
    }
    electricity = getDifference("Electricity Total End Uses");
    naturalGas = getDifference("Natural Gas Total End Uses");
    districtCooling = getDifference("District Cooling Total End Uses");
    districtHeating = getDifference("District Heating Total End Uses");
    capitalCost = getDifference("First Year Capital Costs");
    if(capitalCost){
      capitalCostIncrease = -1 * capitalCost.get();
    }
    energyCost = getDifference("Annual Total Utility Cost");
    lifeCycleCost = getDifference("Total Life Cycle Cost");

    netSiteEnergyIntensityPct = getPctDifference("Net Site Energy Use Intentsity");
    peakElectricDemandPct = getPctDifference("Instantaneous Peak Electricity Demand");
    electricityPct = getPctDifference("Electricity Total End Uses");
    naturalGasPct = getPctDifference("Natural Gas Total End Uses");
    districtCoolingPct = getPctDifference("District Cooling Total End Uses");
    districtHeatingPct = getPctDifference("District Heating Total End Uses");
    capitalCostPct = getPctDifference("First Year Capital Costs");
    if(capitalCostPct){
      capitalCostPctIncrease = -1 * capitalCostPct.get();
    }
    energyCostPct = getPctDifference("Annual Total Utility Cost");
    lifeCycleCostPct = getPctDifference("Total Life Cycle Cost");
  }

  if (m_dataPoint.uuid() == m_baselineDataPoint.uuid()){

    m_nameLabel->setText(name);
    m_nameLabel->setToolTip(name);
    m_nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    m_nameLabel->setMinimumWidth(NAME_LABEL_WIDTH);
    //m_nameLabel->setFixedWidth(NAME_LABEL_WIDTH);

    setBaselineLabels(m_netSiteEnergyIntensityLabel, m_netSiteEnergyIntensityDivLabel, m_netSiteEnergyIntensityPctLabel, netSiteEnergyIntensity);
    setBaselineLabels(m_peakElectricDemandLabel, m_peakElectricDemandDivLabel, m_peakElectricDemandPctLabel, peakElectricDemand_kW);
    setBaselineLabels(m_electricityLabel, m_electricityDivLabel, m_electricityPctLabel, electricity);
    setBaselineLabels(m_naturalGasLabel, m_naturalGasDivLabel, m_naturalGasPctLabel, naturalGas);
    setBaselineLabels(m_districtCoolingLabel, m_districtCoolingDivLabel, m_districtCoolingPctLabel, districtCooling);
    setBaselineLabels(m_districtHeatingLabel, m_districtHeatingDivLabel, m_districtHeatingPctLabel, districtHeating);
    setBaselineLabels(m_capitalCostLabel, m_capitalCostDivLabel, m_capitalCostPctLabel, capitalCost);
    setBaselineLabels(m_energyCostLabel, m_energyCostDivLabel, m_energyCostPctLabel, energyCost);
    setBaselineLabels(m_simplePayBackLabel, simplePayback());
    setBaselineLabels(m_lifeCycleCostLabel, m_lifeCycleCostDivLabel, m_lifeCycleCostPctLabel, lifeCycleCost);

    m_scrollSpacer->setFixedWidth(SCROLL_SPACER_WIDTH);
  }else{

    m_nameLabel->setText(name);
    m_nameLabel->setToolTip(measures);
    m_nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    m_nameLabel->setMinimumWidth(NAME_LABEL_WIDTH);
    //m_nameLabel->setFixedWidth(NAME_LABEL_WIDTH);

    setResultLabels(m_netSiteEnergyIntensityLabel, m_netSiteEnergyIntensityDivLabel, m_netSiteEnergyIntensityPctLabel, netSiteEnergyIntensity, netSiteEnergyIntensityPct);
    setResultLabels(m_peakElectricDemandLabel, m_peakElectricDemandDivLabel, m_peakElectricDemandPctLabel, peakElectricDemand_kW, peakElectricDemandPct);
    setResultLabels(m_electricityLabel, m_electricityDivLabel, m_electricityPctLabel, electricity, electricityPct);
    setResultLabels(m_naturalGasLabel, m_naturalGasDivLabel, m_naturalGasPctLabel, naturalGas, naturalGasPct);
    setResultLabels(m_districtCoolingLabel, m_districtCoolingDivLabel, m_districtCoolingPctLabel, districtCooling, districtCoolingPct);
    setResultLabels(m_districtHeatingLabel, m_districtHeatingDivLabel, m_districtHeatingPctLabel, districtHeating, districtHeatingPct);
    setResultLabels(m_capitalCostLabel, m_capitalCostDivLabel, m_capitalCostPctLabel, capitalCostIncrease, capitalCostPctIncrease);
    setResultLabels(m_energyCostLabel, m_energyCostDivLabel, m_energyCostPctLabel, energyCost, energyCostPct);
    setResultLabels(m_simplePayBackLabel, simplePayback());
    setResultLabels(m_lifeCycleCostLabel, m_lifeCycleCostDivLabel, m_lifeCycleCostPctLabel, lifeCycleCost, lifeCycleCostPct);

    m_scrollSpacer->setFixedWidth(0);
  }
}

void DataPointResultsView::setBaselineLabels(QLabel* resultLabel, const boost::optional<double> & value)
{
  resultLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  resultLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  resultLabel->setText("");
}

void DataPointResultsView::setBaselineLabels(QLabel* resultLabel, QLabel* resultDivLabel, QLabel* resultPctLabel, const boost::optional<double> & value)
{
  // result on top of value
  resultLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  resultDivLabel->setFixedWidth(0);
  resultPctLabel->setFixedWidth(0);

  resultLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  resultDivLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  resultPctLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

  setResultLabelText(resultLabel,value);
  resultDivLabel->setText("");
  resultPctLabel->setText("");
}
  
void DataPointResultsView::setResultLabels(QLabel* resultLabel, const boost::optional<double> & value)
{
  resultLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  resultLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  setResultLabelText(resultLabel,value);
}

void DataPointResultsView::setResultLabels(QLabel* resultLabel, QLabel* resultDivLabel, QLabel* resultPctLabel, const boost::optional<double> & value, const boost::optional<double> & pct)
{
  // result on top of value
  resultLabel->setFixedWidth(RESULT_LABEL_WIDTH);
  resultDivLabel->setFixedWidth(0);
  resultPctLabel->setFixedWidth(0);

  resultLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  resultDivLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
  resultPctLabel->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

  setResultLabelText(resultLabel,value);
  resultDivLabel->setTextFormat(Qt::RichText);
  setResultPctLabelText(resultLabel,pct); // on top
}

void DataPointResultsView::setResultLabelText(QLabel * label, const boost::optional<double> & value)
{
  QString text("<font color='black'>--</font>");

  if(value.is_initialized()){

    if(*value < 0){
      double tempValue = *value * -1;
      QString number = QLocale(QLocale::English).toString(tempValue, 'f', 0);
      text = "<font color='red'>(";
      text += number;
      text += ")</font>";
    }
    else{
      QString number = QLocale(QLocale::English).toString(*value, 'f', 0);
      text = "<font color='black'>";
      text += number;
      text += "</font>";
    }
  }
  
  label->setTextFormat(Qt::RichText);
  label->setText(text);
  label->setToolTip(text);
}

void DataPointResultsView::setResultPctLabelText(QLabel * label, const boost::optional<double> & value)
{
  QString text("<font color='black'>--</font>");

  if(value.is_initialized()){

    if(*value < 0){
      double tempValue = *value * -1;
      QString number = QLocale(QLocale::English).toString(tempValue, 'f', 0);
      text = "<font color='red'> (";
      text += number;
      text += "%";
      text += ") </font>";
    }
    else{
      QString number = QLocale(QLocale::English).toString(*value, 'f', 0);
      text = "<font color='black'>";
      text += number;
      text += "%";
      text += "</font>";
    }
  }

  label->setTextFormat(Qt::RichText);
  label->setText(label->text() + QString("<br>") + text); // on top
  label->setToolTip(label->text());
}

void DataPointResultsView::setHasEmphasis(bool hasEmphasis)
{
  if( hasEmphasis ){
    setStyleSheet("openstudio--pat--DataPointResultsView { background: #FECD60; border: 2px solid #EE641A; }");
  }else{
    if (m_dataPoint.uuid() == m_baselineDataPoint.uuid()){
      setStyleSheet("openstudio--pat--DataPointResultsView {background: #B6B5B6;}");
    }else if (m_alternateRow){
      setStyleSheet("openstudio--pat--DataPointResultsView {background: #E0E0E0;}");
    }else{
      setStyleSheet("openstudio--pat--DataPointResultsView {background: #D0D0D0;}");
    }
  }
}

void DataPointResultsView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

boost::optional<double> DataPointResultsView::getValue(const std::string& attribute, const Attribute& attr)
{
  Unit currentUnit;
  Unit desiredUnit;
  
  boost::optional<std::string> optString;
  boost::optional<double> currentResult;
  boost::optional<double> desiredResult;
  
  currentResult = attr.valueAsDouble();
  optString = attr.units();
  if(currentResult && optString){
    if(attribute == "First Year Capital Costs"){
      return currentResult;
    }
    else if(attribute == "Annual Total Utility Cost"){
      return currentResult;
    }
    else if(attribute == "Simple Payback"){
      return simplePayback();
    }
    else if(attribute == "Total Life Cycle Cost"){
      return currentResult;
    }
    boost::optional<Unit> optionalUnit = createUnit(*optString);
    if(optionalUnit){
      currentUnit = optionalUnit.get();
    
      if(attribute == "Net Site Energy Use Intentsity"){
        desiredUnit = createUnit("kBtu/ft^2").get();
      }
      else if(attribute == "Instantaneous Peak Electricity Demand"){
        desiredUnit = createUnit("kW").get();
      }
      else if(attribute == "Electricity Total End Uses"){
        desiredUnit = createUnit("kWh").get();
      }
      else if(attribute == "Natural Gas Total End Uses"){
        desiredUnit = createUnit("MBtu").get();
      }
      else if(attribute == "District Cooling Total End Uses"){
        desiredUnit = createUnit("MBtu").get();
      }
      else if(attribute == "District Heating Total End Uses"){
        desiredUnit = createUnit("MBtu").get();
      }
      else{
        // should never get here
        OS_ASSERT(false);
      }

      Quantity currentQuantity(*currentResult,currentUnit);
      OptionalQuantity desiredQuantity = QuantityConverter::instance().convert(currentQuantity, desiredUnit);
      if(desiredQuantity){
        desiredResult = desiredQuantity->value();
        Unit unit = desiredQuantity->units();
        std::string standardString = unit.standardString();
      }
    }
  }
 
  return desiredResult;
}

boost::optional<double> DataPointResultsView::getValue(const std::string& attribute)
{
  boost::optional<double> result;
  boost::optional<Attribute> attr = m_dataPoint.getOutputAttribute(attribute);
  if(attr){
    result = getValue(attribute, *attr);
  }
  return result;
}

boost::optional<double> DataPointResultsView::getBaselineValue(const std::string& attribute)
{
  boost::optional<double> result;
  boost::optional<Attribute> attr = m_baselineDataPoint.getOutputAttribute(attribute);
  if(attr){
    result = getValue(attribute, *attr);
  }
  return result;
}

boost::optional<double> DataPointResultsView::getDifference(const std::string& attribute)
{
  boost::optional<double> result;
  boost::optional<double> value = getValue(attribute);
  boost::optional<double> baselineValue = getBaselineValue(attribute);
  if (value && baselineValue){
    result = *baselineValue - *value;
  }
  return result;
}

boost::optional<double> DataPointResultsView::getPctDifference(const std::string& attribute)
{
  boost::optional<double> result;
  boost::optional<double> difference = getDifference(attribute);
  boost::optional<double> baselineValue = getBaselineValue(attribute);
  if (difference && baselineValue && (*baselineValue != 0)){
    result = (*difference / *baselineValue) * 100;
  }
  return result;
}

boost::optional<double> DataPointResultsView::simplePayback()
{
  boost::optional<double> result;
  boost::optional<double> increasedCapitalCosts;
  boost::optional<double> differenceCapitalCosts = getDifference("First Year Capital Costs");
  if (differenceCapitalCosts){
    increasedCapitalCosts = -(*differenceCapitalCosts);
  }
  boost::optional<double> utilityCostsSavings = getDifference("Annual Total Utility Cost");
  if (increasedCapitalCosts && utilityCostsSavings){
    if(*utilityCostsSavings == 0){
      // NA
      return result;
    }
    else if(*utilityCostsSavings < 0){
      // NA
      return result;
    }
    else if(*increasedCapitalCosts == 0){
      result = 0;
    }
    else if(*increasedCapitalCosts < 0){
      result = 0;
    }
    else {
      result = (*increasedCapitalCosts / *utilityCostsSavings);
    }
  }
  return result;
}

} // pat

} // openstudio
