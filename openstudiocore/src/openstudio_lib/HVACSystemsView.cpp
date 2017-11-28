/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "HVACSystemsView.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "OSItem.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSSwitch.hpp"
#include "ModelObjectItem.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/Buttons.hpp"
#include <QStackedWidget>
#include <QObjectList>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QComboBox>
#include <QAbstractItemView>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QButtonGroup>

namespace openstudio {

HVACToolbarView::HVACToolbarView()
  : QWidget()
{
  // There are two modes.  One that shows just the label and the systemComboBox,
  // the other shows all of the controls but no label

  QString style;

  style.append("QWidget#NavigatorWidget { ");
  style.append("background: #808080; ");
  style.append("border-bottom: 1px solid black; ");
  style.append("}");

  setObjectName("NavigatorWidget");
  setStyleSheet(style);

  auto mainHlayout = new QHBoxLayout();
  mainHlayout->setContentsMargins(5,5,5,5);
  mainHlayout->setSpacing(5);
  setLayout(mainHlayout);

  m_viewStack = new QStackedWidget();
  m_viewStack->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  mainHlayout->addWidget(m_viewStack);

  // Label Widget

  auto labelWidget = new QWidget();
  auto labelLayout = new QHBoxLayout();
  labelLayout->setContentsMargins(0,0,0,0); 
  labelLayout->setSpacing(5);
  labelLayout->setAlignment(Qt::AlignLeft);
  labelWidget->setLayout(labelLayout);
  m_viewStack->addWidget(labelWidget);

  label = new QLabel();
  label->setObjectName("H2");
  labelLayout->addWidget(label);

  // Controls Widget

  auto controlWidget = new QWidget();
  auto controlLayout = new QHBoxLayout();
  controlLayout->setContentsMargins(0,0,0,0);
  controlLayout->setSpacing(5);
  controlWidget->setLayout(controlLayout);
  m_viewStack->addWidget(controlWidget);

  addButton = new QPushButton();
  addButton->setObjectName("AddButton");
  addButton->setFixedSize(24,24);
  addButton->setFlat(true);
  controlLayout->addWidget(addButton);

  deleteButton = new QPushButton(this);
  deleteButton->setObjectName("DeleteButton");
  deleteButton->setFixedSize(24,24);
  deleteButton->setFlat(true);
  controlLayout->addWidget(deleteButton);

  controlLayout->addStretch();

  auto zoomButtonGroup = new QButtonGroup(this);
  zoomButtonGroup->setExclusive(true);

  topologyViewButton = new GrayButton();
  topologyViewButton->setCheckable(true);
  topologyViewButton->setText("Layout");
  zoomButtonGroup->addButton(topologyViewButton);
  controlLayout->addWidget(topologyViewButton);

  controlsViewButton = new GrayButton();
  controlsViewButton->setCheckable(true);
  controlsViewButton->setText("Control");
  zoomButtonGroup->addButton(controlsViewButton);
  controlLayout->addWidget(controlsViewButton);

  gridViewButton = new GrayButton();
  gridViewButton->setCheckable(true);
  gridViewButton->setText("Grid");
  zoomButtonGroup->addButton(gridViewButton);
  controlLayout->addWidget(gridViewButton);

  topologyViewButton->setChecked(true);

  controlLayout->addStretch();

  style.clear();
  zoomInButton = new QPushButton();
  zoomInButton->setFlat(true);
  style.append("QPushButton {");
  style.append("              border: none;");
  style.append("              height: 20px;");
  style.append("              width: 20px;");
  style.append("              background-image: url(\":images/zoom-in.png\");");
  style.append("} ");
  style.append("QPushButton:hover { background-image: url(\":images/zoom-in-over.png\"); } ");
  style.append("QPushButton:pressed { background-image: url(\":images/zoom-in-press.png\"); } ");
  zoomInButton->setStyleSheet(style);
  controlLayout->addWidget(zoomInButton);

  style.clear();

  zoomOutButton = new QPushButton();
  zoomOutButton->setFlat(true);
  style.append("QPushButton {");
  style.append("              border: none;");
  style.append("              height: 20px;");
  style.append("              width: 20px;");
  style.append("              background-image: url(\":images/zoom-out.png\");");
  style.append("} ");
  style.append("QPushButton:hover { background-image: url(\":images/zoom-out-over.png\"); } ");
  style.append("QPushButton:pressed { background-image: url(\":images/zoom-out-press.png\"); } ");
  zoomOutButton->setStyleSheet(style);
  controlLayout->addWidget(zoomOutButton);

  controlLayout->addStretch();

  systemComboBox = new OSComboBox2();
  systemComboBox->setMinimumContentsLength(40);
  mainHlayout->addWidget(systemComboBox);
  systemComboBox->setEnabled(true);
}

void HVACToolbarView::showControls(bool show)
{
  if( show )
  {
    m_viewStack->setCurrentIndex(1);
  }
  else
  {
    m_viewStack->setCurrentIndex(0);
  }
}

void HVACToolbarView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HVACToolbarView::~HVACToolbarView()
{
}

HVACSystemsView::HVACSystemsView()
  : QWidget()
{
  setObjectName("GrayWidgetWithLeftTopBorders");

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(1,1,0,0);
  mainVLayout->setSpacing(0);
  mainVLayout->setAlignment(Qt::AlignTop);
  setLayout(mainVLayout);

  hvacToolbarView = new HVACToolbarView();
  mainVLayout->addWidget(hvacToolbarView);

  mainViewSwitcher = new OSViewSwitcher();  
  mainVLayout->addWidget(mainViewSwitcher);
}

//void HVACSystemsView::enableDeleteButton(bool enable)
//{
//  m_deleteButton->setEnabled(enable);
//}

//void HVACSystemsView::configureHeaderForNavigating()
//{
//  m_navigatorStack->setCurrentIndex(0);
//}

//void HVACSystemsView::configureHeaderWithText(const QString & text)
//{
//  m_text->setText(text);
//
//  m_navigatorStack->setCurrentIndex(1);
//}

QSize HVACSystemsView::sizeHint () const
{
  return QSize(800,550);
}

//model::OptionalModel HVACSystemsView::model() const
//{
//  return m_model;
//}

void HVACSystemsView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//OSComboBox2 * HVACSystemsView::chooser() const
//{
//  return m_chooser;
//}

HVACGraphicsView::HVACGraphicsView( QWidget * parent )
  : QGraphicsView(parent)
{
  setAcceptDrops(true);

  resetZoom();

  setObjectName("GrayWidget");
}

void HVACGraphicsView::zoomIn()
{
  this->scale(1.25,1.25);
}

void HVACGraphicsView::zoomOut()
{
  this->scale(0.8,0.8);
}

void HVACGraphicsView::resetZoom()
{
  this->resetTransform();

  this->scale(0.65,0.65);
}

HVACControlsView::HVACControlsView()
  : QScrollArea()
{
  auto widget = new QWidget();

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(5,5,5,5);
  mainVLayout->setSpacing(10);
  mainVLayout->setAlignment(Qt::AlignTop);
  widget->setLayout(mainVLayout);

  setWidget(widget);
  setWidgetResizable(true);
  setFrameShape(QFrame::NoFrame);

  systemNameLabel = new QLabel("HVAC System");
  systemNameLabel->setObjectName("H1");
  mainVLayout->addWidget(systemNameLabel);

  auto hClassificationLayout = new QHBoxLayout();
  hClassificationLayout->setContentsMargins(0,0,0,0);
  hClassificationLayout->setSpacing(5);
  mainVLayout->addLayout(hClassificationLayout);

  QLabel * coolingTypeTitle = new QLabel("Cooling Type: ");
  coolingTypeTitle->setObjectName("H2");
  hClassificationLayout->addWidget(coolingTypeTitle);

  coolingTypeLabel = new QLabel();
  hClassificationLayout->addWidget(coolingTypeLabel);
  hClassificationLayout->addStretch();

  QLabel * heatingTypeTitle = new QLabel("Heating Type: ");
  heatingTypeTitle->setObjectName("H2");
  hClassificationLayout->addWidget(heatingTypeTitle);

  heatingTypeLabel = new QLabel();
  hClassificationLayout->addWidget(heatingTypeLabel);
  hClassificationLayout->addStretch();

  auto line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line);

  QLabel * timeTitle = new QLabel("Time of Operation");
  timeTitle->setObjectName("H1");
  mainVLayout->addWidget(timeTitle);

  QLabel * operationScheduleTitle = new QLabel("HVAC Operation Schedule");
  operationScheduleTitle->setObjectName("H2");
  mainVLayout->addWidget(operationScheduleTitle);

  hvacOperationViewSwitcher = new OSViewSwitcher();
  hvacOperationViewSwitcher->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  mainVLayout->addWidget(hvacOperationViewSwitcher);

  QLabel * nightCycleTitle = new QLabel("Use Night Cycle");
  nightCycleTitle->setObjectName("H2");
  mainVLayout->addWidget(nightCycleTitle);

  auto nightCycleHLayout = new QHBoxLayout();
  nightCycleHLayout->setContentsMargins(0,0,0,0);
  nightCycleHLayout->setSpacing(5);
  nightCycleComboBox = new OSComboBox2();
  nightCycleComboBox->addItem("Follow the HVAC Operation Schedule","StayOff");
  nightCycleComboBox->addItem("Cycle on Full System if Heating or Cooling Required","CycleOnAny");
  nightCycleComboBox->addItem("Cycle on Zone Terminal Units if Heating or Cooling Required","CycleOnAnyZoneFansOnly");
  nightCycleHLayout->addWidget(nightCycleComboBox);

  nightCycleHLayout->addStretch();

  mainVLayout->addLayout(nightCycleHLayout);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line);

  QLabel * supplyAirTemperatureTitle = new QLabel("Supply Air Temperature");
  supplyAirTemperatureTitle->setObjectName("H1");
  mainVLayout->addWidget(supplyAirTemperatureTitle);

  supplyAirTemperatureViewSwitcher = new OSViewSwitcher();
  supplyAirTemperatureViewSwitcher->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  mainVLayout->addWidget(supplyAirTemperatureViewSwitcher);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line);

  QLabel * outdoorAirTitle = new QLabel("Mechanical Ventilation");
  outdoorAirTitle->setObjectName("H1");
  mainVLayout->addWidget(outdoorAirTitle);

  ventilationViewSwitcher = new OSViewSwitcher();
  ventilationViewSwitcher->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  mainVLayout->addWidget(ventilationViewSwitcher);
}

MechanicalVentilationView::MechanicalVentilationView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  auto economizerHBoxLayout = new QHBoxLayout();
  economizerHBoxLayout->setSpacing(10);

  QLabel * economizerTitle = new QLabel("Economizer");
  economizerTitle->setObjectName("H2");
  economizerHBoxLayout->addWidget(economizerTitle);

  economizerComboBox = new OSComboBox2();
  economizerComboBox->setEnabled(true);
  economizerComboBox->addItem("Fixed Dry Bulb","FixedDryBulb");
  economizerComboBox->addItem("Fixed Enthalpy","FixedEnthalpy");
  economizerComboBox->addItem("Differential Dry Bulb","DifferentialDryBulb");
  economizerComboBox->addItem("Differential Enthalpy","DifferentialEnthalpy");
  economizerComboBox->addItem("Fixed Dewpoint and Dry Bulb","FixedDewPointAndDryBulb");
  economizerComboBox->addItem("No Economizer","NoEconomizer");
  economizerHBoxLayout->addWidget(economizerComboBox);

  economizerHBoxLayout->addStretch();
  mainVLayout->addLayout(economizerHBoxLayout);

  auto methodHBoxLayout = new QHBoxLayout();
  methodHBoxLayout->setSpacing(10);

  QLabel * methodTitle = new QLabel("Ventilation Calculation Method");
  methodTitle->setObjectName("H2");
  methodHBoxLayout->addWidget(methodTitle);
  methodTitle->setVisible(false);

  ventilationCalcMethodComboBox = new OSComboBox2();
  ventilationCalcMethodComboBox->setEnabled(false);
  ventilationCalcMethodComboBox->addItem("Zone Sum","ZoneSum");
  ventilationCalcMethodComboBox->addItem("Ventilation Rate Procedure","VentilationRateProcedure");
  ventilationCalcMethodComboBox->addItem("Indoor Air Quality Procedure","IndoorAirQualityProcedure");
  ventilationCalcMethodComboBox->addItem("Proportional Control","ProportionalControl");
  ventilationCalcMethodComboBox->setVisible(false);

  methodHBoxLayout->addWidget(ventilationCalcMethodComboBox);
  methodHBoxLayout->addStretch();
  mainVLayout->addLayout(methodHBoxLayout);

  auto dcvHBoxLayout = new QHBoxLayout();
  dcvHBoxLayout->setSpacing(10);
  mainVLayout->addLayout(dcvHBoxLayout);

  QLabel * dcvTitle = new QLabel("Demand Controlled Ventilation");
  dcvTitle->setObjectName("H2");
  dcvHBoxLayout->addWidget(dcvTitle);

  dcvButton = new OSSwitch2();
  dcvHBoxLayout->addWidget(dcvButton);

  dcvHBoxLayout->addStretch();
}

MechanicalVentilationView::~MechanicalVentilationView()
{
}

NoMechanicalVentilationView::NoMechanicalVentilationView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);

  QLabel * noVentilationTitle = new QLabel("This system configuration does not provide mechanical ventilation");
  mainVLayout->addWidget(noVentilationTitle);
}

NoMechanicalVentilationView::~NoMechanicalVentilationView()
{
}

SingleZoneReheatSPMView::SingleZoneReheatSPMView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QString singleZoneResetSPText;
  singleZoneResetSPText.append("Supply temperature is controlled by a \"SingleZoneReheat\" setpoint manager.");
  auto singleZoneResetSPTitle = new QLabel(singleZoneResetSPText);
  mainVLayout->addWidget(singleZoneResetSPTitle);

  auto zoneSelectorHBoxLayout = new QHBoxLayout();
  zoneSelectorHBoxLayout->setSpacing(10);
  zoneSelectorHBoxLayout->setContentsMargins(0,0,0,0);

  QLabel * controlZoneTitle = new QLabel("Control Zone");
  controlZoneTitle->setObjectName("H2");
  zoneSelectorHBoxLayout->addWidget(controlZoneTitle);

  controlZoneComboBox = new OSComboBox2();
  controlZoneComboBox->setEnabled(true);
  zoneSelectorHBoxLayout->addWidget(controlZoneComboBox);
  zoneSelectorHBoxLayout->addStretch();

  mainVLayout->addLayout(zoneSelectorHBoxLayout);
}

SingleZoneReheatSPMView::~SingleZoneReheatSPMView()
{
}

OAResetSPMView::OAResetSPMView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QString text;
  text.append("Supply temperature is controlled by an outdoor air reset setpoint manager.");
  auto title = new QLabel(text);
  mainVLayout->addWidget(title);
}

OAResetSPMView::~OAResetSPMView()
{
}

ScheduledSPMView::ScheduledSPMView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QLabel * scheduledSPMlabel = new QLabel("Supply air temperature is controlled by a scheduled setpoint manager.");
  mainVLayout->addWidget(scheduledSPMlabel);

  QLabel * supplyAirTempScheduleTitle = new QLabel("Supply Air Temperature Schedule");
  supplyAirTempScheduleTitle->setObjectName("H2");
  mainVLayout->addWidget(supplyAirTempScheduleTitle);

  supplyAirTemperatureViewSwitcher = new OSViewSwitcher();
  supplyAirTemperatureViewSwitcher->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  mainVLayout->addWidget(supplyAirTemperatureViewSwitcher);
}

ScheduledSPMView::~ScheduledSPMView()
{
}

FollowOATempSPMView::FollowOATempSPMView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QLabel * followOATempSPMlabel = new QLabel("Supply air temperature follows the outdoor air temperature.");
  mainVLayout->addWidget(followOATempSPMlabel);
}

FollowOATempSPMView::~FollowOATempSPMView()
{
}

AirLoopHVACUnitaryHeatPumpAirToAirControlView::AirLoopHVACUnitaryHeatPumpAirToAirControlView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QLabel * heatPumplabel = new QLabel("Supply air temperature is managed by the \"AirLoopHVACUnitaryHeatPumpAirToAir\" component.");
  mainVLayout->addWidget(heatPumplabel);

  auto zoneSelectorHBoxLayout = new QHBoxLayout();
  zoneSelectorHBoxLayout->setSpacing(10);
  zoneSelectorHBoxLayout->setContentsMargins(0,0,0,0);

  QLabel * controlZoneTitle = new QLabel("Control Zone");
  controlZoneTitle->setObjectName("H2");
  zoneSelectorHBoxLayout->addWidget(controlZoneTitle);

  controlZoneComboBox = new OSComboBox2();
  controlZoneComboBox->setEnabled(true);
  zoneSelectorHBoxLayout->addWidget(controlZoneComboBox);
  zoneSelectorHBoxLayout->addStretch();

  mainVLayout->addLayout(zoneSelectorHBoxLayout);
}

AirLoopHVACUnitaryHeatPumpAirToAirControlView::~AirLoopHVACUnitaryHeatPumpAirToAirControlView()
{
}

NoSupplyAirTempControlView::NoSupplyAirTempControlView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QLabel * label = new QLabel("Unknown or missing supply air temperature control.  Try adding a setpoint manager to your air system.");
  label->setWordWrap(true);
  mainVLayout->addWidget(label);
}

NoSupplyAirTempControlView::~NoSupplyAirTempControlView()
{
}

NoControlsView::NoControlsView()
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(5,5,5,5);
  mainVLayout->setSpacing(10);
  mainVLayout->setAlignment(Qt::AlignTop);
  setLayout(mainVLayout);

  QLabel * label = new QLabel("Control options are currently only available for air systems.");
  label->setWordWrap(true);
  label->setObjectName("H1");
  mainVLayout->addWidget(label);
}

NoControlsView::~NoControlsView()
{
}

} // openstudio

