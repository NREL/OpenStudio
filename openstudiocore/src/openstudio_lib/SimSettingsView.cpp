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

#include "SimSettingsView.hpp"

#include "CollapsibleInspector.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSAppBase.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "OSDocument.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSSwitch.hpp"
#include "SchedulesView.hpp" // ScheduleCalendarWidget, MonthView

#include "../model/ConvergenceLimits.hpp"
#include "../model/ConvergenceLimits_Impl.hpp"
#include "../model/HeatBalanceAlgorithm.hpp"
#include "../model/HeatBalanceAlgorithm_Impl.hpp"
#include "../model/InsideSurfaceConvectionAlgorithm.hpp"
#include "../model/InsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/OutputControlReportingTolerances.hpp"
#include "../model/OutputControlReportingTolerances_Impl.hpp"
#include "../model/OutsideSurfaceConvectionAlgorithm.hpp"
#include "../model/OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "../model/ProgramControl.hpp"
#include "../model/ProgramControl_Impl.hpp"
#include "../model/RadianceParameters.hpp"
#include "../model/RadianceParameters_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/RunPeriodControlDaylightSavingTime.hpp"
#include "../model/RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "../model/ShadowCalculation.hpp"
#include "../model/ShadowCalculation_Impl.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/SimulationControl_Impl.hpp"
#include "../model/SizingParameters.hpp"
#include "../model/SizingParameters_Impl.hpp"
#include "../model/Timestep.hpp"
#include "../model/Timestep_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"
#include "../model/ZoneAirContaminantBalance.hpp"
#include "../model/ZoneAirContaminantBalance_Impl.hpp"
#include "../model/ZoneAirHeatBalanceAlgorithm.hpp"
#include "../model/ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "../model/ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../model/ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

#include "../utilities/time/Date.hpp"
#include "../utilities/time/Time.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QDate>
#include <QDateEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>
#include <QTimer>

#define TEXT_FIELD_WIDTH 400
#define OSCOMBOBOX_FIELD_WIDTH 200
#define OSINTEGEREDIT_FIELD_WIDTH 90
#define SPACERITEM_HEIGHT 15
#define SPACERITEM_WIDTH 70
#define GRID_LAYOUT_SPACING 5

namespace openstudio {

SimSettingsView::SimSettingsView(bool isIP,
  const model::Model & model,
  QWidget * parent)
  : QWidget(parent),
  m_model(model),
  m_runSimWeatherFiles(NULL),
  m_runSimDesignDays(NULL),
  m_performZoneSizing(NULL),
  m_performSystemSizing(NULL),
  m_performPlantSizing(NULL),
  m_dateRangelabel(NULL),
  m_startDateEdit(NULL),
  m_endDateEdit(NULL),
  m_runPeriodGroup(NULL),
  m_radianceGroup(NULL),
  m_runPeriodName(NULL),
  m_isIP(false),
  // SimulationControl
  m_doZoneSizingCalculation(NULL),
  m_doSystemSizingCalculation(NULL),
  m_doPlantSizingCalculation(NULL),
  m_runSimulationforSizingPeriods(NULL),
  m_runSimulationforWeatherFileRunPeriods(NULL),
  m_maximumNumberofWarmupDays(NULL),
  m_minimumNumberofWarmupDays(NULL),
  m_loadsConvergenceToleranceValue(NULL),
  m_temperatureConvergenceToleranceValue(NULL),
  m_solarDistribution(NULL),
  // Radiance
  m_accumulatedRaysperRecord(NULL),
  m_directThreshold(NULL),
  m_directCertainty(NULL),
  m_directJitter(NULL),
  m_directPretest(NULL),
  m_ambientBouncesVMX(NULL),
  m_ambientBouncesDMX(NULL),
  m_ambientDivisionsVMX(NULL),
  m_ambientDivisionsDMX(NULL),
  m_ambientSupersamples(NULL),
  m_limitWeightVMX(NULL),
  m_limitWeightDMX(NULL),
  m_klemsSamplingDensity(NULL),
  m_skyDiscretizationResolution(NULL),
  m_accumulatedRaysperRecordLbl(NULL),
  m_directThresholdLbl(NULL),
  m_directCertaintyLbl(NULL),
  m_directJitterLbl(NULL),
  m_directPretestLbl(NULL),
  m_ambientBouncesVMXLbl(NULL),
  m_ambientBouncesDMXLbl(NULL),
  m_ambientDivisionsVMXLbl(NULL),
  m_ambientDivisionsDMXLbl(NULL),
  m_ambientSupersamplesLbl(NULL),
  m_limitWeightVMXLbl(NULL),
  m_limitWeightDMXLbl(NULL),
  m_klemsSamplingDensityLbl(NULL),
  m_skyDiscretizationResolutionLbl(NULL),
  // SizingParameters
  m_heatingSizingFactor(NULL),
  m_coolingSizingFactor(NULL),
  m_timestepsinAveragingWindow(NULL),
  // ProgramControl
  m_numberOfThreadsAllowed(NULL),
  // Timestep
  m_numberOfTimestepsPerHour(NULL),
  // OutputControlReportingTolerances
  m_toleranceForTimeHeatingSetpointNotMet(NULL),
  m_toleranceForTimeCoolingSetpointNotMet(NULL),
  // ConvergenceLimits
  m_maximumHVACIterations(NULL),
  m_minimumPlantIterations(NULL),
  m_maximumPlantIterations(NULL),
  m_minimumSystemTimestep(NULL),
  // ShadowCalculation
  m_calculationFrequency(NULL),
  m_maximumFiguresInShadowOverlapCalculations(NULL),
  m_polygonClippingAlgorithm(NULL),
  m_skyDiffuseModelingAlgorithm(NULL),
  // SurfaceConvectionAlgorithmInside
  m_algorithmSurfaceConvectionInside(NULL),
  // SurfaceConvectionAlgorithmOutside
  m_algorithmSurfaceConvectionOutside(NULL),
  // HeatBalance
  m_algorithmHeatBalance(NULL),
  m_surfaceTemperatureUpperLimit(NULL),
  m_minimumSurfaceConvectionHeatTransferCoefficientValue(NULL),
  m_maximumSurfaceConvectionHeatTransferCoefficientValue(NULL),
  // ZoneAirHeatBalanceAlgorithm
  m_algorithmZoneAirHeatBalance(NULL),
  // ZoneAirContaminantBalance
  m_carbonDioxideConcentration(NULL),
  m_outdoorCarbonDioxideScheduleName(NULL),
  // ZoneCapacitanceMultiplierResearchSpecial
  m_temperatureCapacityMultiplier(NULL),
  m_humidityCapacityMultiplier(NULL),
  m_carbonDioxideCapacityMultiplier(NULL)
{
  bool isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                             this,SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  // when the year settings object changes need to update the year in all child widgets
  model::YearDescription yearDescription = m_model.getUniqueModelObject<model::YearDescription>();
  isConnected = connect(yearDescription.getImpl<model::detail::YearDescription_Impl>().get(), SIGNAL(onChange()),
                        this, SLOT(updateYearDescription()));
  OS_ASSERT(isConnected);

  createWidgets();
  attachAll();
  updateYearDescription();
}

void SimSettingsView::createWidgets()
{
  QVBoxLayout * mainLayout = new QVBoxLayout() ;
  mainLayout->setContentsMargins(0,0,0,0);
  mainLayout->setSpacing(0);

  setLayout(mainLayout);
  setContentsMargins(0,0,0,0);

  CollapsibleInspector * collapsibleInspector = NULL;
  
  //******************* OS:RunPeriod *******************
  mainLayout->addWidget(createRunPeriodWidget());

  //******************* OS:RunControl *******************
  mainLayout->addWidget(createRunControlWidget());

  //******************* OS:Sizing:Parameters *******************
  mainLayout->addWidget(createSizingParametersWidget());

  //******************* OS:Timestep *******************
  mainLayout->addWidget(createTimestepWidget());

  //******************* OS:RadianceParameters *******************
  collapsibleInspector = new CollapsibleInspector("Radiance Parameters",createRadianceParametersWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:SimulationControl *******************
  collapsibleInspector = new CollapsibleInspector("Simulation Control",createSimulationControlWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:ProgramControl *******************
  collapsibleInspector = new CollapsibleInspector("Program Control",createProgramControlWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:OutputControl:ReportingTolerances *******************
  collapsibleInspector = new CollapsibleInspector("Output Control Reporting Tolerances",createOutputControlReportingTolerancesWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:ConvergenceLimits *******************
  collapsibleInspector = new CollapsibleInspector("Convergence Limits",createConvergenceLimitsWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:ShadowCalculation *******************
  collapsibleInspector = new CollapsibleInspector("Shadow Calculation",createShadowCalculationWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:SurfaceConvectionAlgorithm:Inside *******************
  collapsibleInspector = new CollapsibleInspector("Inside Surface Convection Algorithm",createSurfaceConvectionAlgorithmInsideWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:SurfaceConvectionAlgorithm:Outside *******************
  collapsibleInspector = new CollapsibleInspector("Outside Surface Convection Algorithm",createSurfaceConvectionAlgorithmOutsideWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:HeatBalanceAlgorithm *******************
  collapsibleInspector = new CollapsibleInspector("Heat Balance Algorithm",createHeatBalanceAlgorithmWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:ZoneAirHeatBalanceAlgorithm *******************
  collapsibleInspector = new CollapsibleInspector("Zone Air Heat Balance Algorithm",createZoneAirHeatBalanceAlgorithmWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:ZoneAirContaminantBalance *******************
  collapsibleInspector = new CollapsibleInspector("Zone Air Contaminant Balance",createZoneAirContaminantBalanceWidget());
  mainLayout->addWidget(collapsibleInspector);

  //******************* OS:ZoneCapacitanceMultiplier:ResearchSpecial *******************
  collapsibleInspector = new CollapsibleInspector("Zone Capacitance Multiple Research Special",createZoneCapacitanceMultipleResearchSpecialWidget());
  mainLayout->addWidget(collapsibleInspector); 
  
  mainLayout->addStretch();
}

QWidget * SimSettingsView::createRunPeriodWidget()
{
  QVBoxLayout * mainVLayout = new QVBoxLayout() ;
  mainVLayout->setContentsMargins(7,0,0,0);
  mainVLayout->setSpacing(0);

  QHBoxLayout * mainHLayout = new QHBoxLayout() ;
  mainHLayout->setContentsMargins(7,7,7,7);
  mainHLayout->setSpacing(0);

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

  QHBoxLayout * hLayout = NULL;

  label = new QLabel("Run Period");
  label->setObjectName("H1");
  mainVLayout->addWidget(label);

  mainVLayout->addLayout(mainHLayout);

  // LEFT
  // DLM: removing name from the Run Period inspector because this is a unique object
  //vLayout = new QVBoxLayout() ;
  //vLayout->setContentsMargins(0,0,0,0);
  //vLayout->setSpacing(10);
  //mainHLayout->addLayout(vLayout);
  //
  //label = new QLabel("Name");
  //label->setObjectName("H2");
  //vLayout->addWidget(label);
  //
  //hLayout = new QHBoxLayout() ;
  //hLayout->setContentsMargins(10,0,0,0);
  //hLayout->setSpacing(10);
  //vLayout->addLayout(hLayout);
  //
  //m_runPeriodName = new QLineEdit();
  //m_runPeriodName->setReadOnly(true);
  //m_runPeriodName->setFixedWidth(TEXT_FIELD_WIDTH);
  //hLayout->addWidget(m_runPeriodName);
  //
  //mainHLayout->addStretch();

  // CENTER
  // DLM: removing this button group because this is not an exclusive choice
  //vLayout = new QVBoxLayout() ;
  //vLayout->setContentsMargins(0,0,0,0);
  //vLayout->setSpacing(10);
  //mainHLayout->addLayout(vLayout);
  //
  //bool isConnected = false;
  //
  //m_runPeriodGroup = new QButtonGroup(this);
  //
  //isConnected = connect(m_runPeriodGroup, SIGNAL(buttonClicked(int)),
  //  this, SLOT(on_runPeriodGroupClicked(int)));
  //OS_ASSERT(isConnected);
  //
  //QRadioButton * radioButton = NULL;
  //
  //int buttonCount = 0;
  //
  //radioButton = new QRadioButton("Annual Simulation");
  //m_runPeriodGroup->addButton(radioButton,buttonCount++);
  //vLayout->addWidget(radioButton);
  //
  //radioButton = new QRadioButton("Custom Period");
  //m_runPeriodGroup->addButton(radioButton,buttonCount);
  //vLayout->addWidget(radioButton);
  //
  //m_runPeriodGroup->button(0)->setChecked(true);
  //
  //mainHLayout->addStretch();

  // RIGHT
  vLayout = new QVBoxLayout() ;
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->setSpacing(10);
  mainHLayout->addLayout(vLayout);

  mainHLayout->addStretch();

  m_dateRangelabel = new QLabel("Date Range");
  m_dateRangelabel->setObjectName("H2");
  m_dateRangelabel->setEnabled(true);
  vLayout->addWidget(m_dateRangelabel);

  hLayout = new QHBoxLayout() ;
  hLayout->setContentsMargins(0,0,0,0);
  hLayout->setSpacing(10);
  vLayout->addLayout(hLayout);

  m_startDateEdit = new QDateEdit();
  m_startDateEdit->setDisplayFormat("MMMM d");
  m_startDateEdit->setCalendarPopup(true);
  //m_startDateEdit->setEnabled(false);
  hLayout->addWidget(m_startDateEdit);

  bool isConnected = connect(m_startDateEdit,SIGNAL(dateChanged(const QDate &)),
    this,SLOT(on_startDateChanged(const QDate &)));
  OS_ASSERT(isConnected);

  m_endDateEdit = new QDateEdit();
  m_endDateEdit->setDisplayFormat("MMMM d");
  m_endDateEdit->setCalendarPopup(true);
  //m_endDateEdit->setEnabled(false);
  hLayout->addWidget(m_endDateEdit);

  isConnected = connect(m_endDateEdit,SIGNAL(dateChanged(const QDate &)),
    this,SLOT(on_endDateChanged(const QDate &)));
  OS_ASSERT(isConnected);

  QWidget * widget = new QWidget();
  widget->setLayout(mainVLayout);

  return widget;
}

QWidget * SimSettingsView::createRunControlWidget()
{
  QVBoxLayout * mainLayout = new QVBoxLayout() ;
  mainLayout->setContentsMargins(0,0,0,0);
  mainLayout->setSpacing(0);

  QLabel * label = NULL;

  label = new QLabel("Run Control");
  label->setObjectName("H1");
  mainLayout->addWidget(label);

  QVBoxLayout * layout = new QVBoxLayout();
  layout->setContentsMargins(10,0,0,0);
  layout->setSpacing(0);
  mainLayout->addLayout(layout);

  bool isConnected = false;

  m_runSimWeatherFiles = new QCheckBox("Run Simulation for Weather File");
  layout->addWidget(m_runSimWeatherFiles);

  isConnected = connect(m_runSimWeatherFiles, SIGNAL(stateChanged(int)),
    this, SLOT(on_runSimWeatherFiles(int)));
  OS_ASSERT(isConnected);

  m_runSimDesignDays = new QCheckBox("Run Simulation for Design Days");
  layout->addWidget(m_runSimDesignDays);

  isConnected = connect(m_runSimDesignDays, SIGNAL(stateChanged(int)),
    this, SLOT(on_runSimDesignDays(int)));
  OS_ASSERT(isConnected);

  m_performZoneSizing = new QCheckBox("Perform Zone Sizing");
  layout->addWidget(m_performZoneSizing);

  isConnected = connect(m_performZoneSizing, SIGNAL(stateChanged(int)),
    this, SLOT(on_performZoneSizing(int)));
  OS_ASSERT(isConnected);

  m_performSystemSizing = new QCheckBox("Perform System Sizing");
  layout->addWidget(m_performSystemSizing);

  isConnected = connect(m_performSystemSizing, SIGNAL(stateChanged(int)),
    this, SLOT(on_performSystemSizing(int)));
  OS_ASSERT(isConnected);

  m_performPlantSizing = new QCheckBox("Perform Plant Sizing");
  layout->addWidget(m_performPlantSizing);

  isConnected = connect(m_performPlantSizing, SIGNAL(stateChanged(int)),
    this, SLOT(on_performPlantSizing(int)));
  OS_ASSERT(isConnected);

  QWidget * widget = new QWidget();
  widget->setLayout(mainLayout);
  widget->hide(); // TODO delete when ready to use

  return widget;
}

QWidget * SimSettingsView::createSimulationControlWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Do Zone Sizing Calculation",m_doZoneSizingCalculation);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Do System Sizing Calculation",m_doSystemSizingCalculation);

  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,"Do Plant Sizing Calculation",m_doPlantSizingCalculation);
  col = col + 2;
  addField(gridLayout,row,col,"Run Simulation For Sizing Periods",m_runSimulationforSizingPeriods);

  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,"Run Simulation For Weather File Run Periods",m_runSimulationforWeatherFileRunPeriods);
  col = col + 2;
  addField(gridLayout,row,col,"Maximum Number Of Warmup Days",m_maximumNumberofWarmupDays);

  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,"Minimum Number Of Warmup Days",m_minimumNumberofWarmupDays);
  col = col + 2;
  addField(gridLayout,row,col,"Loads Convergence Tolerance Value",m_loadsConvergenceToleranceValue);

  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,"Temperature Convergence Tolerance Value",m_temperatureConvergenceToleranceValue);
  col = col + 2;
  addField(gridLayout,row,col,"Solar Distribution",m_solarDistribution);
  
  std::vector<std::string>  validSolarDistributionValues = model::SimulationControl::validSolarDistributionValues();
  for (std::string validSolarDistributionValue : validSolarDistributionValues){
    m_solarDistribution->addItem(validSolarDistributionValue.c_str());
  }

  gridLayout->setRowStretch(100,100);
  gridLayout->setColumnStretch(100,100);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createSizingParametersWidget()
{
  QVBoxLayout * mainLayout = new QVBoxLayout() ;
  mainLayout->setContentsMargins(7,7,7,7);
  mainLayout->setSpacing(0);

  QLabel * label = NULL;

  label = new QLabel("Sizing Parameters");
  label->setObjectName("H1");
  mainLayout->addWidget(label);

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(gridLayout);

  int row = 0;
  int col = 0;

  addField(gridLayout,row,col,"Heating Sizing Factor",m_heatingSizingFactor);
  row = row + 2;
  addField(gridLayout,row,col,"Cooling Sizing Factor",m_coolingSizingFactor);
  row = row + 2;
  addField(gridLayout,row,col,"Timesteps In Averaging Window",m_timestepsinAveragingWindow);

  QWidget * widget = new QWidget();
  widget->setLayout(mainLayout);

  return widget;
}

QWidget * SimSettingsView::createProgramControlWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;

  addField(gridLayout,row,col,"Number Of Threads Allowed",m_numberOfThreadsAllowed);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createTimestepWidget()
{
  QVBoxLayout * mainLayout = new QVBoxLayout() ;
  mainLayout->setContentsMargins(7,7,7,7);
  mainLayout->setSpacing(0);

  QLabel * label = NULL;

  label = new QLabel("Timestep");
  label->setObjectName("H1");
  mainLayout->addWidget(label);

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(gridLayout);

  int row = 0;
  int col = 0;

  addField(gridLayout,row,col,"Number Of Timesteps Per Hour",m_numberOfTimestepsPerHour);

  QWidget * widget = new QWidget();
  widget->setLayout(mainLayout);

  return widget;
}

QWidget * SimSettingsView::createOutputControlReportingTolerancesWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Tolerance For Time Heating Setpoint Not Met",m_toleranceForTimeHeatingSetpointNotMet);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Tolerance For Time Cooling Setpoint Not Met",m_toleranceForTimeCoolingSetpointNotMet);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createConvergenceLimitsWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Maximum HVAC Iterations",m_maximumHVACIterations);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Minimum Plant Iterations",m_minimumPlantIterations);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;
  
  addField(gridLayout,row,col,"Maximum Plant Iterations",m_maximumPlantIterations);
  col = col + 2;
  addField(gridLayout,row,col,"Minimum System Timestep",m_minimumSystemTimestep);

  gridLayout->setRowStretch(100,100);
  gridLayout->setColumnStretch(100,100);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createShadowCalculationWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Calculation Frequency",m_calculationFrequency);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Maximum Figures In Shadow Overlap Calculations",m_maximumFiguresInShadowOverlapCalculations);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;
  
  addField(gridLayout,row,col,"Polygon Clipping Algorithm",m_polygonClippingAlgorithm);
  col = col + 2;
  addField(gridLayout,row,col,"Sky Diffuse Modeling Algorithm",m_skyDiffuseModelingAlgorithm);

  gridLayout->setRowStretch(100,100);
  gridLayout->setColumnStretch(100,100);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createSurfaceConvectionAlgorithmInsideWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;

  addField(gridLayout,row,col,"Algorithm",m_algorithmSurfaceConvectionInside);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createSurfaceConvectionAlgorithmOutsideWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;

  addField(gridLayout,row,col,"Algorithm",m_algorithmSurfaceConvectionOutside);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createHeatBalanceAlgorithmWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Surface Temperature Upper Limit",m_surfaceTemperatureUpperLimit);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Minimum Surface Convection Heat Transfer Coefficient Value",m_minimumSurfaceConvectionHeatTransferCoefficientValue);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,"Maximum Surface Convection Heat Transfer Coefficient Value",m_maximumSurfaceConvectionHeatTransferCoefficientValue);
  col = col + 2;
  addField(gridLayout,row,col,"Algorithm",m_algorithmHeatBalance);

  gridLayout->setRowStretch(100,100);
  gridLayout->setColumnStretch(100,100);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createZoneAirHeatBalanceAlgorithmWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;

  addField(gridLayout,row,col,"Algorithm",m_algorithmZoneAirHeatBalance);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createZoneAirContaminantBalanceWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Carbon Dioxide Concentration",m_carbonDioxideConcentration);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Outdoor Carbon Dioxide Schedule Name",m_outdoorCarbonDioxideScheduleName);

  gridLayout->setRowStretch(100,100);
  gridLayout->setColumnStretch(100,100);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createZoneCapacitanceMultipleResearchSpecialWidget()
{
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);
  gridLayout->setAlignment(Qt::AlignLeft);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,"Temperature Capacity Multiplier",m_temperatureCapacityMultiplier);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,"Humidity Capacity Multiplier",m_humidityCapacityMultiplier);

  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,"Carbon Dioxide Capacity Multiplier",m_carbonDioxideCapacityMultiplier);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);
  widget->hide();

  return widget;
}

QWidget * SimSettingsView::createRadianceParametersWidget()
{
  QVBoxLayout * vLayout = new QVBoxLayout();

  m_radianceGroup = new QButtonGroup(this);

  bool isConnected = connect(m_radianceGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(on_radianceGroupClicked(int)));
  OS_ASSERT(isConnected);

  int buttonCount = 0;

  QRadioButton * radioButton = NULL ;

  radioButton = new QRadioButton("Coarse (Fast, less accurate)");
  m_radianceGroup->addButton(radioButton,buttonCount++);
  vLayout->addWidget(radioButton);

  radioButton = new QRadioButton("Fine (Slow, more accurate)");
  m_radianceGroup->addButton(radioButton,buttonCount++);
  vLayout->addWidget(radioButton);

  radioButton = new QRadioButton("Custom");
  m_radianceGroup->addButton(radioButton,buttonCount);
  vLayout->addWidget(radioButton);

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(7,7,7,7);
  gridLayout->setSpacing(GRID_LAYOUT_SPACING);

  int row = 0;
  int col = 0;
  QSpacerItem * spacerItem = NULL;

  addField(gridLayout,row,col,m_accumulatedRaysperRecordLbl,"Accumulated Rays per Record: ",m_accumulatedRaysperRecord);
  col++;
  spacerItem = new QSpacerItem(SPACERITEM_WIDTH,1,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row,col++);
  addField(gridLayout,row,col,m_directThresholdLbl,"Direct Threshold: ",m_directThreshold);

  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,m_directCertaintyLbl,"Direct Certainty: ",m_directCertainty);
  col = col + 2;
  addField(gridLayout,row,col,m_directJitterLbl,"Direct Jitter: ",m_directJitter);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,m_directPretestLbl,"Direct Pretest: ",m_directPretest);
  col = col + 2;
  addField(gridLayout,row,col,m_ambientBouncesVMXLbl,"Ambient Bounces VMX: ",m_ambientBouncesVMX);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,m_ambientBouncesDMXLbl,"Ambient Bounces DMX: ",m_ambientBouncesDMX);
  col = col + 2;
  addField(gridLayout,row,col,m_ambientDivisionsVMXLbl,"Ambient Divisions VMX: ",m_ambientDivisionsVMX);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;

  addField(gridLayout,row,col,m_ambientDivisionsDMXLbl,"Ambient Divisions DMX: ",m_ambientDivisionsDMX);
  col = col + 2;
  addField(gridLayout,row,col,m_ambientSupersamplesLbl,"Ambient Supersamples: ",m_ambientSupersamples);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;
  
  addField(gridLayout,row,col,m_limitWeightVMXLbl,"Limit Weight VMX: ",m_limitWeightVMX);
  col = col + 2;
  addField(gridLayout,row,col,m_limitWeightDMXLbl,"Limit Weight DMX: ",m_limitWeightDMX);
  
  row = row + 2;
  spacerItem = new QSpacerItem(1,SPACERITEM_HEIGHT,QSizePolicy::Fixed,QSizePolicy::Fixed);
  gridLayout->addItem(spacerItem,row++,0);
  col = 0;
  
  addField(gridLayout,row,col,m_klemsSamplingDensityLbl,"Klems Sampling Density: ",m_klemsSamplingDensity);
  col = col + 2;
  addField(gridLayout,row,col,m_skyDiscretizationResolutionLbl,"Sky Discretization Resolution: ",m_skyDiscretizationResolution);

  std::vector<std::string>  skyDiscretizationResolutionValues = model::RadianceParameters::skyDiscretizationResolutionValues();
  for (std::string skyDiscretizationResolutionValue : skyDiscretizationResolutionValues){
    m_skyDiscretizationResolution->addItem(skyDiscretizationResolutionValue.c_str());
  }

  gridLayout->setRowStretch(100,100);
  gridLayout->setColumnStretch(100,100);

  vLayout->addLayout(gridLayout);

  QWidget * widget = new QWidget();
  widget->setLayout(vLayout);
  widget->hide();

  m_radianceGroup->button(0)->setChecked(true);
  attachRadianceParameters();
  enableRadianceParametersWidget(false);
  setCoarseRadianceSettings();

  return widget;
}

void SimSettingsView::enableRadianceParametersWidget(bool isEnabled)
{
  m_accumulatedRaysperRecord->setEnabled(isEnabled);
  m_directThreshold->setEnabled(isEnabled);
  m_directCertainty->setEnabled(isEnabled);
  m_directJitter->setEnabled(isEnabled);
  m_directPretest->setEnabled(isEnabled);
  m_ambientBouncesVMX->setEnabled(isEnabled);
  m_ambientBouncesDMX->setEnabled(isEnabled);
  m_ambientDivisionsVMX->setEnabled(isEnabled);
  m_ambientDivisionsDMX->setEnabled(isEnabled);
  m_ambientSupersamples->setEnabled(isEnabled);
  m_limitWeightVMX->setEnabled(isEnabled);
  m_limitWeightDMX->setEnabled(isEnabled);
  m_klemsSamplingDensity->setEnabled(isEnabled);
  m_skyDiscretizationResolution->setEnabled(isEnabled);
  m_accumulatedRaysperRecordLbl->setEnabled(isEnabled);
  m_directThresholdLbl->setEnabled(isEnabled);
  m_directCertaintyLbl->setEnabled(isEnabled);
  m_directJitterLbl->setEnabled(isEnabled);
  m_directPretestLbl->setEnabled(isEnabled);
  m_ambientBouncesVMXLbl->setEnabled(isEnabled);
  m_ambientBouncesDMXLbl->setEnabled(isEnabled);
  m_ambientDivisionsVMXLbl->setEnabled(isEnabled);
  m_ambientDivisionsDMXLbl->setEnabled(isEnabled);
  m_ambientSupersamplesLbl->setEnabled(isEnabled);
  m_limitWeightVMXLbl->setEnabled(isEnabled);
  m_limitWeightDMXLbl->setEnabled(isEnabled);
  m_klemsSamplingDensityLbl->setEnabled(isEnabled);
  m_skyDiscretizationResolutionLbl->setEnabled(isEnabled);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSComboBox * & comboBox)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  comboBox = new OSComboBox(this);
  comboBox->setFixedWidth(OSCOMBOBOX_FIELD_WIDTH);
  gridLayout->addWidget(comboBox,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSComboBox2 * & comboBox)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  comboBox = new OSComboBox2(this);
  comboBox->setFixedWidth(OSCOMBOBOX_FIELD_WIDTH);
  gridLayout->addWidget(comboBox,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QLabel * & label,
                               QString text,
                               OSComboBox * & comboBox)
{
  label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  comboBox = new OSComboBox(this);
  comboBox->setFixedWidth(OSCOMBOBOX_FIELD_WIDTH);
  gridLayout->addWidget(comboBox,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QLabel * & label,
                               QString text,
                               OSComboBox2 * & comboBox)
{
  label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  comboBox = new OSComboBox2(this);
  comboBox->setFixedWidth(OSCOMBOBOX_FIELD_WIDTH);
  gridLayout->addWidget(comboBox,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSIntegerEdit * & integerEdit)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  integerEdit = new OSIntegerEdit(this);
  integerEdit->setFixedWidth(OSINTEGEREDIT_FIELD_WIDTH);
  gridLayout->addWidget(integerEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSIntegerEdit2 * & integerEdit)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  integerEdit = new OSIntegerEdit2(this);
  integerEdit->setFixedWidth(OSINTEGEREDIT_FIELD_WIDTH);
  gridLayout->addWidget(integerEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QLabel * & label,
                               QString text,
                               OSIntegerEdit * & integerEdit)
{
  label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  integerEdit = new OSIntegerEdit(this);
  integerEdit->setFixedWidth(OSINTEGEREDIT_FIELD_WIDTH);
  gridLayout->addWidget(integerEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QLabel * & label,
                               QString text,
                               OSIntegerEdit2 * & integerEdit)
{
  label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  integerEdit = new OSIntegerEdit2(this);
  integerEdit->setFixedWidth(OSINTEGEREDIT_FIELD_WIDTH);
  gridLayout->addWidget(integerEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSLineEdit * & lineEdit)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  lineEdit = new OSLineEdit(this);
  lineEdit->setFixedWidth(TEXT_FIELD_WIDTH);
  gridLayout->addWidget(lineEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSQuantityEdit * & quantityEdit)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  quantityEdit = new OSQuantityEdit(m_isIP,this);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             quantityEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);

  gridLayout->addWidget(quantityEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QLabel * & label,
                               QString text,
                               OSQuantityEdit * & quantityEdit)
{
  label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  quantityEdit = new OSQuantityEdit(m_isIP,this);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             quantityEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);

  gridLayout->addWidget(quantityEdit,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSSwitch * & osSwitch)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  osSwitch = new OSSwitch(this);
  gridLayout->addWidget(osSwitch,row,column);
}

void SimSettingsView::addField(QGridLayout * gridLayout,
                               int row,
                               int column,
                               QString text,
                               OSSwitch2 * & osSwitch)
{
  QLabel * label = new QLabel(text,this);
  label->setFixedWidth(TEXT_FIELD_WIDTH);
  label->setObjectName("H2");
  gridLayout->addWidget(label,row++,column);

  osSwitch = new OSSwitch2(this);
  gridLayout->addWidget(osSwitch,row,column);
}

void SimSettingsView::attachAll()
{
  attachRunPeriod();
  attachRunPeriodControlDaylightSavingTime();
  attachSimulationControl();
  attachSizingParameters();
  attachProgramControl();
  attachTimestep();
  attachOutputControlReportingTolerances();
  attachConvergenceLimits();
  attachShadowCalculation();
  attachSurfaceConvectionAlgorithmInside();
  attachSurfaceConvectionAlgorithmOutside();
  attachHeatBalanceAlgorithm();
  attachZoneAirHeatBalanceAlgorithm();
  attachZoneAirContaminantBalance();
  attachZoneCapacitanceMultipleResearchSpecial();
}

void SimSettingsView::detachAll()
{
  detachRunPeriod();
  detachRunPeriodControlDaylightSavingTime();
  detachSimulationControl();
  detachSizingParameters();
  detachProgramControl();
  detachTimestep();
  detachOutputControlReportingTolerances();
  detachConvergenceLimits();
  detachShadowCalculation();
  detachSurfaceConvectionAlgorithmInside();
  detachSurfaceConvectionAlgorithmOutside();
  detachHeatBalanceAlgorithm();
  detachZoneAirHeatBalanceAlgorithm();
  detachZoneAirContaminantBalance();
  detachZoneCapacitanceMultipleResearchSpecial();
  detachRadianceParameters();
}

void SimSettingsView::attachRunPeriod()
{
}

void SimSettingsView::attachRunPeriodControlDaylightSavingTime()
{
}

void SimSettingsView::attachSimulationControl()
{
  m_simulationControl = m_model.getUniqueModelObject<model::SimulationControl>();

  m_doZoneSizingCalculation->bind(
      *m_simulationControl,
      std::bind(&model::SimulationControl::doZoneSizingCalculation,m_simulationControl.get_ptr()),
      boost::optional<BoolSetter>(std::bind(&model::SimulationControl::setDoZoneSizingCalculation,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetDoZoneSizingCalculation,m_simulationControl.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isDoZoneSizingCalculationDefaulted,m_simulationControl.get_ptr())));
  m_doSystemSizingCalculation->bind(
      *m_simulationControl,
      std::bind(&model::SimulationControl::doSystemSizingCalculation,m_simulationControl.get_ptr()),
      boost::optional<BoolSetter>(std::bind(&model::SimulationControl::setDoSystemSizingCalculation,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetDoSystemSizingCalculation,m_simulationControl.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isDoSystemSizingCalculationDefaulted,m_simulationControl.get_ptr())));
  m_doPlantSizingCalculation->bind(
      *m_simulationControl,
      std::bind(&model::SimulationControl::doPlantSizingCalculation,m_simulationControl.get_ptr()),
      boost::optional<BoolSetter>(std::bind(&model::SimulationControl::setDoPlantSizingCalculation,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetDoPlantSizingCalculation,m_simulationControl.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isDoPlantSizingCalculationDefaulted,m_simulationControl.get_ptr())));
  m_runSimulationforSizingPeriods->bind(
      *m_simulationControl,
      std::bind(&model::SimulationControl::runSimulationforSizingPeriods,m_simulationControl.get_ptr()),
      boost::optional<BoolSetter>(std::bind(&model::SimulationControl::setRunSimulationforSizingPeriods,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetRunSimulationforSizingPeriods,m_simulationControl.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isRunSimulationforSizingPeriodsDefaulted,m_simulationControl.get_ptr())));
  m_runSimulationforWeatherFileRunPeriods->bind(
      *m_simulationControl,
      std::bind(&model::SimulationControl::runSimulationforWeatherFileRunPeriods,m_simulationControl.get_ptr()),
      boost::optional<BoolSetter>(std::bind(&model::SimulationControl::setRunSimulationforWeatherFileRunPeriods,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetRunSimulationforWeatherFileRunPeriods,m_simulationControl.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isRunSimulationforWeatherFileRunPeriodsDefaulted,m_simulationControl.get_ptr())));
  m_maximumNumberofWarmupDays->bind(
      *m_simulationControl,
      IntGetter(std::bind(&model::SimulationControl::maximumNumberofWarmupDays,m_simulationControl.get_ptr())),
      boost::optional<IntSetter>(std::bind(&model::SimulationControl::setMaximumNumberofWarmupDays,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetMaximumNumberofWarmupDays,m_simulationControl.get_ptr())),
      boost::none,
      boost::none,
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isMaximumNumberofWarmupDaysDefaulted,m_simulationControl.get_ptr())));
  m_minimumNumberofWarmupDays->bind(
      *m_simulationControl,
      IntGetter(std::bind(&model::SimulationControl::minimumNumberofWarmupDays,m_simulationControl.get_ptr())),
      boost::optional<IntSetter>(std::bind(&model::SimulationControl::setMinimumNumberofWarmupDays,m_simulationControl.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::SimulationControl::resetMinimumNumberofWarmupDays,m_simulationControl.get_ptr())),
      boost::none,
      boost::none,
      boost::optional<BasicQuery>(std::bind(&model::SimulationControl::isMinimumNumberofWarmupDaysDefaulted,m_simulationControl.get_ptr())));
  m_loadsConvergenceToleranceValue->bind(*m_simulationControl,"loadsConvergenceToleranceValue",m_isIP);
  m_temperatureConvergenceToleranceValue->bind(*m_simulationControl,"temperatureConvergenceToleranceValue",m_isIP);
  m_solarDistribution->bind(*m_simulationControl,"solarDistribution");
}

void SimSettingsView::attachSizingParameters()
{
  model::SizingParameters mo = m_model.getUniqueModelObject<model::SizingParameters>();

  m_heatingSizingFactor->bind(mo,"heatingSizingFactor",m_isIP);
  m_coolingSizingFactor->bind(mo,"coolingSizingFactor",m_isIP);
  m_timestepsinAveragingWindow->bind(mo,"timestepsinAveragingWindow");
}

void SimSettingsView::attachProgramControl()
{
  model::ProgramControl mo = m_model.getUniqueModelObject<model::ProgramControl>();

  m_numberOfThreadsAllowed->bind(mo,"numberofThreadsAllowed");
}

void SimSettingsView::attachTimestep()
{
  model::Timestep mo = m_model.getUniqueModelObject<model::Timestep>();

  m_numberOfTimestepsPerHour->bind(mo,"numberOfTimestepsPerHour");
}

void SimSettingsView::attachOutputControlReportingTolerances()
{
  model::OutputControlReportingTolerances mo = m_model.getUniqueModelObject<model::OutputControlReportingTolerances>();

  m_toleranceForTimeHeatingSetpointNotMet->bind(mo,"toleranceforTimeHeatingSetpointNotMet",m_isIP);
  m_toleranceForTimeCoolingSetpointNotMet->bind(mo,"toleranceforTimeCoolingSetpointNotMet",m_isIP);
}

void SimSettingsView::attachConvergenceLimits()
{
  model::ConvergenceLimits mo = m_model.getUniqueModelObject<model::ConvergenceLimits>();

  m_maximumHVACIterations->bind(mo,"maximumHVACIterations");
  m_minimumPlantIterations->bind(mo,"minimumPlantIterations");
  m_maximumPlantIterations->bind(mo,"maximumPlantIterations");
  m_minimumSystemTimestep->bind(mo,"minimumSystemTimestep");
}

void SimSettingsView::attachShadowCalculation()
{
  m_shadowCalculation = m_model.getUniqueModelObject<model::ShadowCalculation>();

  m_calculationFrequency->bind(*m_shadowCalculation,"calculationFrequency");
  m_maximumFiguresInShadowOverlapCalculations->bind(*m_shadowCalculation,
                                                    "maximumFiguresInShadowOverlapCalculations");
  m_polygonClippingAlgorithm->bind<std::string>(
      *m_shadowCalculation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(model::ShadowCalculation::validPolygonClippingAlgorithmValues),
      std::function<boost::optional<std::string> ()>(std::bind(&model::ShadowCalculation::polygonClippingAlgorithm,m_shadowCalculation.get_ptr())),
      std::bind(&model::ShadowCalculation::setPolygonClippingAlgorithm,m_shadowCalculation.get_ptr(),std::placeholders::_1),
      NoFailAction(std::bind(&model::ShadowCalculation::resetPolygonClippingAlgorithm,m_shadowCalculation.get_ptr())));
  m_skyDiffuseModelingAlgorithm->bind<std::string>(
      *m_shadowCalculation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(model::ShadowCalculation::validSkyDiffuseModelingAlgorithmValues),
      std::function<boost::optional<std::string> ()>(std::bind(&model::ShadowCalculation::skyDiffuseModelingAlgorithm,m_shadowCalculation.get_ptr())),
      std::bind(&model::ShadowCalculation::setSkyDiffuseModelingAlgorithm,m_shadowCalculation.get_ptr(),std::placeholders::_1),
      NoFailAction(std::bind(&model::ShadowCalculation::resetSkyDiffuseModelingAlgorithm,m_shadowCalculation.get_ptr())));
}

void SimSettingsView::attachSurfaceConvectionAlgorithmInside()
{
  model::InsideSurfaceConvectionAlgorithm mo = m_model.getUniqueModelObject<model::InsideSurfaceConvectionAlgorithm>();

  m_algorithmSurfaceConvectionInside->bind(mo,"algorithmSurfaceConvectionInside");
}

void SimSettingsView::attachSurfaceConvectionAlgorithmOutside()
{
  model::OutsideSurfaceConvectionAlgorithm mo = m_model.getUniqueModelObject<model::OutsideSurfaceConvectionAlgorithm>();

  m_algorithmSurfaceConvectionOutside->bind(mo,"algorithmSurfaceConvectionOutside");
}

void SimSettingsView::attachHeatBalanceAlgorithm()
{
  model::HeatBalanceAlgorithm mo = m_model.getUniqueModelObject<model::HeatBalanceAlgorithm>();

  m_algorithmHeatBalance->bind(mo,"algorithmHeatBalance");
  m_surfaceTemperatureUpperLimit->bind(mo,"surfaceTemperatureUpperLimit",m_isIP);
  m_minimumSurfaceConvectionHeatTransferCoefficientValue->bind(mo,"minimumSurfaceConvectionHeatTransferCoefficientValue",m_isIP);
  m_maximumSurfaceConvectionHeatTransferCoefficientValue->bind(mo,"maximumSurfaceConvectionHeatTransferCoefficientValue",m_isIP);
}

void SimSettingsView::attachZoneAirHeatBalanceAlgorithm()
{
  model::ZoneAirHeatBalanceAlgorithm mo = m_model.getUniqueModelObject<model::ZoneAirHeatBalanceAlgorithm>();

  m_algorithmZoneAirHeatBalance->bind(mo,"algorithmZoneAirHeatBalance");
}

void SimSettingsView::attachZoneAirContaminantBalance()
{
  model::ZoneAirContaminantBalance mo = m_model.getUniqueModelObject<model::ZoneAirContaminantBalance>();

  m_carbonDioxideConcentration->bind(mo,"carbonDioxideConcentration");
  m_outdoorCarbonDioxideScheduleName->bind(mo,"outdoorCarbonDioxideScheduleName");
}

void SimSettingsView::attachZoneCapacitanceMultipleResearchSpecial()
{
  model::ZoneCapacitanceMultiplierResearchSpecial mo = m_model.getUniqueModelObject<model::ZoneCapacitanceMultiplierResearchSpecial>();

  m_temperatureCapacityMultiplier->bind(mo,"temperatureCapacityMultiplier",m_isIP);
  m_humidityCapacityMultiplier->bind(mo,"humidityCapacityMultiplier",m_isIP);
  m_carbonDioxideCapacityMultiplier->bind(mo,"carbonDioxideCapacityMultiplier",m_isIP);
}

void SimSettingsView::attachRadianceParameters()
{
  model::RadianceParameters mo = m_model.getUniqueModelObject<model::RadianceParameters>();

  m_accumulatedRaysperRecord->bind(mo,"accumulatedRaysperRecord");
  m_directThreshold->bind(mo,"directThreshold",m_isIP);
  m_directCertainty->bind(mo,"directCertainty",m_isIP);
  m_directJitter->bind(mo,"directJitter",m_isIP);
  m_directPretest->bind(mo,"directPretest",m_isIP);
  m_ambientBouncesVMX->bind(mo,"ambientBouncesVMX");
  m_ambientBouncesDMX->bind(mo,"ambientBouncesDMX");
  m_ambientDivisionsVMX->bind(mo,"ambientDivisionsVMX");
  m_ambientDivisionsDMX->bind(mo,"ambientDivisionsDMX");
  m_ambientSupersamples->bind(mo,"ambientSupersamples");
  m_limitWeightVMX->bind(mo,"limitWeightVMX",m_isIP);
  m_limitWeightDMX->bind(mo,"limitWeightDMX",m_isIP);
  m_klemsSamplingDensity->bind(mo,"klemsSamplingDensity");
  m_skyDiscretizationResolution->bind(mo,"skyDiscretizationResolution");
}

void SimSettingsView::detachRunPeriod()
{
}

void SimSettingsView::detachRunPeriodControlDaylightSavingTime()
{
}

void SimSettingsView::detachSimulationControl()
{
  m_doZoneSizingCalculation->unbind();
  m_doSystemSizingCalculation->unbind();
  m_doPlantSizingCalculation->unbind();
  m_runSimulationforSizingPeriods->unbind();
  m_runSimulationforWeatherFileRunPeriods->unbind();
  m_maximumNumberofWarmupDays->unbind();
  m_minimumNumberofWarmupDays->unbind();
  m_loadsConvergenceToleranceValue->unbind();
  m_temperatureConvergenceToleranceValue->unbind();
  m_solarDistribution->unbind();
}

void SimSettingsView::detachSizingParameters()
{
  m_heatingSizingFactor->unbind();
  m_coolingSizingFactor->unbind();
  m_timestepsinAveragingWindow->unbind();
}

void SimSettingsView::detachProgramControl()
{
  m_numberOfThreadsAllowed->unbind();
}

void SimSettingsView::detachTimestep()
{
  m_numberOfTimestepsPerHour->unbind();
}
void SimSettingsView::detachOutputControlReportingTolerances()
{
  m_toleranceForTimeHeatingSetpointNotMet->unbind();
  m_toleranceForTimeCoolingSetpointNotMet->unbind();
}

void SimSettingsView::detachConvergenceLimits()
{
  m_maximumHVACIterations->unbind();
  m_minimumPlantIterations->unbind();
  m_maximumPlantIterations->unbind();
  m_minimumSystemTimestep->unbind();
}

void SimSettingsView::detachShadowCalculation()
{
  m_calculationFrequency->unbind();
  m_maximumFiguresInShadowOverlapCalculations->unbind();
  m_polygonClippingAlgorithm->unbind();
  m_skyDiffuseModelingAlgorithm->unbind();
}

void SimSettingsView::detachSurfaceConvectionAlgorithmInside()
{
  m_algorithmSurfaceConvectionInside->unbind();
}

void SimSettingsView::detachSurfaceConvectionAlgorithmOutside()
{
  m_algorithmSurfaceConvectionOutside->unbind();
}

void SimSettingsView::detachHeatBalanceAlgorithm()
{
  m_algorithmHeatBalance->unbind();
  m_surfaceTemperatureUpperLimit->unbind();
  m_minimumSurfaceConvectionHeatTransferCoefficientValue->unbind();
  m_maximumSurfaceConvectionHeatTransferCoefficientValue->unbind();
}

void SimSettingsView::detachZoneAirHeatBalanceAlgorithm()
{
  m_algorithmZoneAirHeatBalance->unbind();
}

void SimSettingsView::detachZoneAirContaminantBalance()
{
  m_carbonDioxideConcentration->unbind();
  m_outdoorCarbonDioxideScheduleName->unbind();
}

void SimSettingsView::detachZoneCapacitanceMultipleResearchSpecial()
{
  m_temperatureCapacityMultiplier->unbind();
  m_humidityCapacityMultiplier->unbind();
  m_carbonDioxideCapacityMultiplier->unbind();
}

void SimSettingsView::detachRadianceParameters()
{
  m_accumulatedRaysperRecord->unbind();
  m_directThreshold->unbind();
  m_directCertainty->unbind();
  m_directJitter->unbind();
  m_directPretest->unbind();
  m_ambientBouncesVMX->unbind();
  m_ambientBouncesDMX->unbind();
  m_ambientDivisionsVMX->unbind();
  m_ambientDivisionsDMX->unbind();
  m_ambientSupersamples->unbind();
  m_limitWeightVMX->unbind();
  m_limitWeightDMX->unbind();
  m_klemsSamplingDensity->unbind();
  m_skyDiscretizationResolution->unbind();
}

void SimSettingsView::setCoarseRadianceSettings()
{
  model::RadianceParameters radianceParameters = m_model.getUniqueModelObject<model::RadianceParameters>();

  radianceParameters.setAccumulatedRaysperRecord(1);
  radianceParameters.setDirectThreshold(0.0);
  radianceParameters.setDirectCertainty(1.0);
  radianceParameters.setDirectJitter(1.0);
  radianceParameters.setDirectPretest(1.0);
  radianceParameters.setAmbientBouncesVMX(6);
  radianceParameters.setAmbientBouncesDMX(2);
  radianceParameters.setAmbientDivisionsVMX(4050);
  radianceParameters.setAmbientDivisionsDMX(512);
  radianceParameters.setAmbientSupersamples(256);
  radianceParameters.setLimitWeightVMX(0.001);
  radianceParameters.setLimitWeightDMX(0.001);
  radianceParameters.setKlemsSamplingDensity(500);
  radianceParameters.setSkyDiscretizationResolution("146");
}

void SimSettingsView::setFineRadianceSettings()
{
  model::RadianceParameters radianceParameters = m_model.getUniqueModelObject<model::RadianceParameters>();

  radianceParameters.setAccumulatedRaysperRecord(1);
  radianceParameters.setDirectThreshold(0.0);
  radianceParameters.setDirectCertainty(1.0);
  radianceParameters.setDirectJitter(1.0);
  radianceParameters.setDirectPretest(1.0);
  radianceParameters.setAmbientBouncesVMX(10);
  radianceParameters.setAmbientBouncesDMX(3);
  radianceParameters.setAmbientDivisionsVMX(65536);
  radianceParameters.setAmbientDivisionsDMX(1024);
  radianceParameters.setAmbientSupersamples(512);
  radianceParameters.setLimitWeightVMX(0.0000152);
  radianceParameters.setLimitWeightDMX(0.0001);
  radianceParameters.setKlemsSamplingDensity(1000);
  radianceParameters.setSkyDiscretizationResolution("581");
}

//***** SLOTS *****

void SimSettingsView::on_runPeriodGroupClicked(int idx)
{
  if(idx == 0){
    m_dateRangelabel->setEnabled(false);
    m_startDateEdit->setEnabled(false);
    m_endDateEdit->setEnabled(false);
  }
  else if(idx == 1){
    m_dateRangelabel->setEnabled(true);
    m_startDateEdit->setEnabled(true);
    m_endDateEdit->setEnabled(true);
  }
  else{
    // Should never get here
    OS_ASSERT(false);
  }
}

void SimSettingsView::on_radianceGroupClicked(int idx)
{
  if(idx == 0){
    enableRadianceParametersWidget(false);
    setCoarseRadianceSettings();
  }
  else if(idx == 1){
    enableRadianceParametersWidget(false);
    setFineRadianceSettings();
  }
  else if(idx == 2){
    enableRadianceParametersWidget(true);
  }
  else{
    // Should never get here
    OS_ASSERT(false);
  }
}

void SimSettingsView::on_startDateChanged(const QDate & date)
{
  static bool firstTimeInFunction = true;
  if(firstTimeInFunction && m_startDateEdit->date().month() == 12 && m_startDateEdit->date().day() == 31){
    firstTimeInFunction = false;
    QTimer::singleShot(0, this, SLOT(initStartDateEdit()));
    return;
  }

  if(date > m_endDateEdit->date()){
    m_endDateEdit->blockSignals(true);
    m_endDateEdit->setDate(date);
    m_endDateEdit->blockSignals(false);
  }

  model::RunPeriod mo = m_model.getUniqueModelObject<model::RunPeriod>();

  mo.setBeginMonth(m_startDateEdit->date().month());
  mo.setBeginDayOfMonth(m_startDateEdit->date().day());

  mo.setEndMonth(m_endDateEdit->date().month());
  mo.setEndDayOfMonth(m_endDateEdit->date().day());
}

void SimSettingsView::initStartDateEdit()
{
  // Note: QDateEdit bug workaround
  int year = m_startDateEdit->date().year();
  model::RunPeriod runPeriod = m_model.getUniqueModelObject<model::RunPeriod>();
  m_startDateEdit->blockSignals(true);
  m_startDateEdit->setDate(QDate(year, runPeriod.getBeginMonth(), runPeriod.getBeginDayOfMonth()));
  m_startDateEdit->blockSignals(false);
}

void SimSettingsView::on_endDateChanged(const QDate & date)
{
  if(m_startDateEdit->date() > date){
    m_startDateEdit->blockSignals(true);
    m_startDateEdit->setDate(date);
    m_startDateEdit->blockSignals(false);
  }

  model::RunPeriod mo = m_model.getUniqueModelObject<model::RunPeriod>();

  mo.setBeginMonth(m_startDateEdit->date().month());
  mo.setBeginDayOfMonth(m_startDateEdit->date().day());

  mo.setEndMonth(m_endDateEdit->date().month());
  mo.setEndDayOfMonth(m_endDateEdit->date().day());
}

void SimSettingsView::on_runSimWeatherFiles(int state)
{
  // TODO
}
void SimSettingsView::on_runSimDesignDays(int state)
{
  // TODO
}

void SimSettingsView::on_performZoneSizing(int state)
{
  // TODO
}

void SimSettingsView::on_performSystemSizing(int state)
{
  // TODO
}

void SimSettingsView::on_performPlantSizing(int state)
{
  // TODO
}


void SimSettingsView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

void SimSettingsView::updateYearDescription()
{
  model::YearDescription yearDescription = m_model.getUniqueModelObject<model::YearDescription>();
  model::RunPeriod runPeriod = m_model.getUniqueModelObject<model::RunPeriod>();

  m_startDateEdit->blockSignals(true);
  m_startDateEdit->setMinimumDate(QDate(yearDescription.assumedYear(), 1, 1));
  m_startDateEdit->setMaximumDate(QDate(yearDescription.assumedYear(), 12, 31));
  m_startDateEdit->setDate(QDate(yearDescription.assumedYear(), runPeriod.getBeginMonth(), runPeriod.getBeginDayOfMonth()));
  m_startDateEdit->blockSignals(false);

  m_endDateEdit->blockSignals(true);
  m_endDateEdit->setMinimumDate(QDate(yearDescription.assumedYear(), 1, 1));
  m_endDateEdit->setMaximumDate(QDate(yearDescription.assumedYear(), 12, 31));
  m_endDateEdit->setDate(QDate(yearDescription.assumedYear(), runPeriod.getEndMonth(), runPeriod.getEndDayOfMonth()));
  m_endDateEdit->blockSignals(false);
}

} // openstudio
