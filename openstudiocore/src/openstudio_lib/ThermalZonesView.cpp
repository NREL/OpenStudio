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

#include "ThermalZonesView.hpp"

#include "IconLibrary.hpp"
#include "MainTabView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"
#include "ThermalZonesGridView.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSSwitch.hpp"

#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/PortList.hpp"
#include "../model/Schedule.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/SizingZone.hpp"
#include "../model/SizingZone_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/ZoneControlHumidistat.hpp"
#include "../model/ZoneControlHumidistat_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"
#include "../model/ZoneHVACEquipmentList.hpp"
#include "../model/ZoneHVACEquipmentList_Impl.hpp"

#include "../utilities/idd/OS_Space_FieldEnums.hxx"
#include "../utilities/idd/OS_ThermostatSetpoint_DualSetpoint_FieldEnums.hxx"
#include "../utilities/idd/OS_ZoneControl_Humidistat_FieldEnums.hxx"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"

#include <QCheckBox>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

namespace openstudio {

//BuildingStoriesView::BuildingStoriesView(const openstudio::model::Model& model, 
//                                         QWidget * parent)
//  : SubTabView(new ModelObjectListView(IddObjectType::OS_BuildingStory, model, true, parent),
//               new BuildingStoryInspectorView(model, parent),
//               parent)

ThermalZonesView::ThermalZonesView(bool isIP, const model::Model & model,
                                   QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_ThermalZone,model,true,parent),
                    new ThermalZoneView(isIP,model,parent),
                    parent)
{
}

ThermalZoneView::ThermalZoneView(bool isIP, const model::Model & model,
                                 QWidget * parent )
  : ModelObjectInspectorView(model,true,parent),
    m_isIP(isIP),
    m_dirty(false)
{
  connect(this,SIGNAL(toggleUnitsClicked(bool)),this,SLOT(onToggleUnitsClicked(bool)));

  QWidget* emptyWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, emptyWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,1);
  mainVLayout->setSpacing(0);
  visibleWidget->setLayout(mainVLayout);

  QVBoxLayout * vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(7,7,7,7);
  vLayout->setSpacing(7);
  mainVLayout->addLayout(vLayout);

  ThermalZonesGridView * thermalZonesGridView = new ThermalZonesGridView(this->m_isIP,this->m_model,this);
  vLayout->addWidget(thermalZonesGridView);

  // Name

  QLabel * nameLabel = new QLabel("Name:");
  nameLabel->setObjectName("H2");
  vLayout->addWidget(nameLabel);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  // HVAC

  QFrame * equipmentLine0 = new QFrame();
  equipmentLine0->setFrameShape(QFrame::HLine);
  equipmentLine0->setFrameShadow(QFrame::Sunken);
  vLayout->addWidget(equipmentLine0);

  QWidget * hvacHeader = new QWidget();
  vLayout->addWidget(hvacHeader);
  QHBoxLayout * hvacHeaderLayout = new QHBoxLayout();
  hvacHeaderLayout->setContentsMargins(0,0,0,0);
  hvacHeader->setLayout(hvacHeaderLayout);
  QLabel * hvacImage = new QLabel();
  hvacImage->setPixmap(QPixmap(":/images/hvac-icon.png"));
  hvacHeaderLayout->addWidget(hvacImage);
  QLabel * hvacLabel = new QLabel();
  hvacLabel->setText("HVAC Systems");
  hvacLabel->setObjectName("H1");
  hvacHeaderLayout->addWidget(hvacLabel);
  hvacHeaderLayout->addStretch();

  QLabel * idealAirLoadsLabel = new QLabel();
  idealAirLoadsLabel->setText("Ideal Air Loads");
  idealAirLoadsLabel->setObjectName("H2");
  hvacHeaderLayout->addWidget(idealAirLoadsLabel);

  m_idealLoadsBox = new OSSwitch();
  hvacHeaderLayout->addWidget(m_idealLoadsBox,0,Qt::AlignVCenter);

  QHBoxLayout * airLoopLayout = new QHBoxLayout();
  airLoopLayout->setContentsMargins(0,0,0,0);
  airLoopLayout->setSpacing(10);
  QLabel * airLoopImage = new QLabel();
  airLoopImage->setPixmap(QPixmap(":/images/loop_name_icon.png"));
  airLoopLayout->addWidget(airLoopImage);
  QLabel * airLoopLabel = new QLabel();
  airLoopLabel->setText("Air Loop Name: ");
  airLoopLabel->setObjectName("H2");
  airLoopLayout->addWidget(airLoopLabel,0,Qt::AlignVCenter);
  vLayout->addLayout(airLoopLayout);

  m_airLoopNameLabel = new QLabel();
  m_airLoopNameLabel->setText("None");

  airLoopLayout->addWidget(m_airLoopNameLabel);

  airLoopLayout->addStretch();

  // Equipment

  QLabel * zoneEquipmentLabel = new QLabel();
  zoneEquipmentLabel->setText("Zone Equipment: ");
  zoneEquipmentLabel->setObjectName("H2");

  QHBoxLayout * zoneEquipmentLabelHLayout = new QHBoxLayout();
  zoneEquipmentLabelHLayout->setContentsMargins(0,0,0,0);
  zoneEquipmentLabelHLayout->setSpacing(10);
  zoneEquipmentLabelHLayout->addSpacing(30);
  zoneEquipmentLabelHLayout->addWidget(zoneEquipmentLabel);
  zoneEquipmentLabelHLayout->addStretch();
  vLayout->addLayout(zoneEquipmentLabelHLayout);

  m_equipmentVectorController = std::shared_ptr<ZoneEquipmentVectorController>(new ZoneEquipmentVectorController());

  OSDropZone * equipmentDropZone = new OSDropZone(m_equipmentVectorController.get());
  equipmentDropZone->setUseLargeIcon(true);
  equipmentDropZone->setMinItems(0);
  equipmentDropZone->setItemsAcceptDrops(true);
  connect(equipmentDropZone,SIGNAL(itemClicked(OSItem*)),this,SLOT(onZoneHVACEquipmentItemClicked(OSItem*)));

  QHBoxLayout * equipmentHLayout = new QHBoxLayout();
  equipmentHLayout->setContentsMargins(0,0,0,0);
  equipmentHLayout->setSpacing(10);
  equipmentHLayout->addSpacing(30);
  equipmentHLayout->addWidget(equipmentDropZone);
  vLayout->addLayout(equipmentHLayout);

  QFrame * equipmentLine1 = new QFrame();
  equipmentLine1->setFrameShape(QFrame::HLine);
  equipmentLine1->setFrameShadow(QFrame::Sunken);
  vLayout->addWidget(equipmentLine1);

  // Thermostat

  QWidget * thermostatHeader = new QWidget();
  QHBoxLayout * thermostatHeaderLayout = new QHBoxLayout();
  thermostatHeaderLayout->setContentsMargins(0,0,0,0);
  thermostatHeader->setLayout(thermostatHeaderLayout);
  QLabel * thermostatImage = new QLabel();
  thermostatImage->setPixmap(QPixmap(":/images/thermostat.png"));
  thermostatHeaderLayout->addWidget(thermostatImage);
  QLabel * thermostatLabel = new QLabel();
  thermostatLabel->setText("Thermostat");
  thermostatLabel->setObjectName("H1");
  thermostatHeaderLayout->addWidget(thermostatLabel);
  thermostatHeaderLayout->addStretch();
  vLayout->addWidget(thermostatHeader);
  QWidget * zoneThermostatWidget = new QWidget();
  vLayout->addWidget(zoneThermostatWidget);

  QGridLayout * thermostatLayout = new QGridLayout();
  thermostatLayout->setContentsMargins(0,0,0,0);
  thermostatLayout->setSpacing(5);
  thermostatLayout->setColumnStretch(0,0);
  thermostatLayout->setColumnStretch(1,100);
  thermostatLayout->setColumnStretch(2,0);
  thermostatLayout->setColumnStretch(3,900);

  zoneThermostatWidget->setLayout(thermostatLayout);

  QLabel * coolingLabel = new QLabel();
  coolingLabel->setFixedSize(24,24);
  coolingLabel->setPixmap(QPixmap(":images/snowflake.png"));
  thermostatLayout->addWidget(coolingLabel,0,0,Qt::AlignLeft);

  QLabel * coolingTextLabel = new QLabel("Cooling Thermostat Schedule");
  coolingTextLabel->setObjectName("H2");
  thermostatLayout->addWidget(coolingTextLabel,0,1);
  
  m_coolingThermostatVectorController = new CoolingScheduleVectorController();
  m_coolingThermostatDropZone = new OSDropZone(m_coolingThermostatVectorController);
  m_coolingThermostatDropZone->setMinItems(0);
  m_coolingThermostatDropZone->setMaxItems(1);
  m_coolingThermostatDropZone->setItemsAcceptDrops(true);
  thermostatLayout->addWidget(m_coolingThermostatDropZone,1,1,Qt::AlignLeft);

  QLabel * heatingLabel = new QLabel();
  heatingLabel->setPixmap(QPixmap(":images/fire.png"));
  heatingLabel->setFixedSize(24,24);
  thermostatLayout->addWidget(heatingLabel,0,2,Qt::AlignLeft);

  QLabel * heatingTextLabel = new QLabel("Heating Thermostat Schedule");
  heatingTextLabel->setObjectName("H2");
  thermostatLayout->addWidget(heatingTextLabel,0,3);
  
  m_heatingThermostatVectorController = new HeatingScheduleVectorController();
  m_heatingThermostatDropZone = new OSDropZone(m_heatingThermostatVectorController);
  m_heatingThermostatDropZone->setMinItems(0);
  m_heatingThermostatDropZone->setMaxItems(1);
  m_heatingThermostatDropZone->setItemsAcceptDrops(true);
  thermostatLayout->addWidget(m_heatingThermostatDropZone,1,3,Qt::AlignLeft);

  m_thermostatButton = new OSSwitch();
  thermostatHeaderLayout->addWidget(m_thermostatButton);
  connect( m_thermostatButton, SIGNAL(clicked(bool)), this, SLOT(onThermostatButtonClicked(bool)));

  QFrame * hvacLine2 = new QFrame();
  hvacLine2->setFrameShape(QFrame::HLine);
  hvacLine2->setFrameShadow(QFrame::Sunken);
  vLayout->addWidget(hvacLine2);

  // Humidistat

  QWidget * humidistatHeader = new QWidget();
  QHBoxLayout * humidistatHeaderLayout = new QHBoxLayout();
  humidistatHeaderLayout->setContentsMargins(0,0,0,0);
  humidistatHeader->setLayout(humidistatHeaderLayout);
  QLabel * humidistatImage = new QLabel();
  humidistatImage->setPixmap(QPixmap(":/images/humidistat.png"));
  humidistatHeaderLayout->addWidget(humidistatImage);
  QLabel * humidistatLabel = new QLabel();
  humidistatLabel->setText("Humidistat");
  humidistatLabel->setObjectName("H1");
  humidistatHeaderLayout->addWidget(humidistatLabel);
  humidistatHeaderLayout->addStretch();
  vLayout->addWidget(humidistatHeader);
  QWidget * zoneHumidistatWidget = new QWidget();
  vLayout->addWidget(zoneHumidistatWidget);

  QGridLayout * humidistatLayout = new QGridLayout();
  humidistatLayout->setContentsMargins(0,0,0,0);
  humidistatLayout->setSpacing(5);
  humidistatLayout->setColumnStretch(0,0);
  humidistatLayout->setColumnStretch(1,100);
  humidistatLayout->setColumnStretch(2,0);
  humidistatLayout->setColumnStretch(3,900);

  zoneHumidistatWidget->setLayout(humidistatLayout);

  QLabel * humidifyingLabel = new QLabel();
  humidifyingLabel->setFixedSize(24,24);
  humidifyingLabel->setPixmap(QPixmap(":images/humidify_schedule_icon.png"));
  humidistatLayout->addWidget(humidifyingLabel,0,0,Qt::AlignLeft);

  QLabel * humidifyingTextLabel = new QLabel("Humidifying Setpoint Schedule");
  humidifyingTextLabel->setObjectName("H2");
  humidistatLayout->addWidget(humidifyingTextLabel,0,1);
  
  m_humidifyingHumidistatVectorController = new HumidifyingScheduleVectorController();
  m_humidifyingHumidistatDropZone = new OSDropZone(m_humidifyingHumidistatVectorController);
  m_humidifyingHumidistatDropZone->setMinItems(0);
  m_humidifyingHumidistatDropZone->setMaxItems(1);
  m_humidifyingHumidistatDropZone->setItemsAcceptDrops(true);
  humidistatLayout->addWidget(m_humidifyingHumidistatDropZone,1,1,Qt::AlignLeft);

  QLabel * dehumidifyingLabel = new QLabel();
  dehumidifyingLabel->setPixmap(QPixmap(":images/dehumidify_schedule_icon.png"));
  dehumidifyingLabel->setFixedSize(24,24);
  humidistatLayout->addWidget(dehumidifyingLabel,0,2,Qt::AlignLeft);

  QLabel * dehumidifyingTextLabel = new QLabel("Dehumidifying Setpoint Schedule");
  dehumidifyingTextLabel->setObjectName("H2");
  humidistatLayout->addWidget(dehumidifyingTextLabel,0,3);
  
  m_dehumidifyingHumidistatVectorController = new DehumidifyingScheduleVectorController();
  m_dehumidifyingHumidistatDropZone = new OSDropZone(m_dehumidifyingHumidistatVectorController);
  m_dehumidifyingHumidistatDropZone->setMinItems(0);
  m_dehumidifyingHumidistatDropZone->setMaxItems(1);
  m_dehumidifyingHumidistatDropZone->setItemsAcceptDrops(true);
  humidistatLayout->addWidget(m_dehumidifyingHumidistatDropZone,1,3,Qt::AlignLeft);

  m_humidistatButton = new OSSwitch();
  humidistatHeaderLayout->addWidget(m_humidistatButton);
  connect( m_humidistatButton, SIGNAL(clicked(bool)), this, SLOT(onHumidistatButtonClicked(bool)));

  QFrame * hvacLine3 = new QFrame();
  hvacLine3->setFrameShape(QFrame::HLine);
  hvacLine3->setFrameShadow(QFrame::Sunken);
  vLayout->addWidget(hvacLine3);

  // Sizing Parameters

  QWidget * sizingHeader = new QWidget();
  QHBoxLayout * sizingHeaderLayout = new QHBoxLayout();
  sizingHeaderLayout->setContentsMargins(0,0,0,0);
  sizingHeader->setLayout(sizingHeaderLayout);
  QLabel * sizingImage = new QLabel();
  sizingImage->setPixmap(QPixmap(":/images/sizing.png"));
  sizingHeaderLayout->addWidget(sizingImage);
  QLabel * sizingLabel = new QLabel();
  sizingLabel->setText("Sizing Parameters");
  sizingLabel->setObjectName("H1");
  sizingHeaderLayout->addWidget(sizingLabel);
  sizingHeaderLayout->addStretch();
  vLayout->addWidget(sizingHeader);
  QWidget * zonesizingWidget = new QWidget();
  vLayout->addWidget(zonesizingWidget);

  QGridLayout * sizingLayout = new QGridLayout();
  sizingLayout->setContentsMargins(0,0,0,0);
  sizingLayout->setSpacing(5);
  sizingLayout->setColumnStretch(1,1000);
  sizingLayout->setColumnStretch(0,0);

  zonesizingWidget->setLayout(sizingLayout);

  sizingLayout->setColumnMinimumWidth(0,30);

  int row = 0;

  m_sizingParametersView = new ModelObjectPropertyView(m_isIP);
  sizingLayout->addWidget(m_sizingParametersView,row,1);

  connect(this,SIGNAL(toggleUnitsClicked(bool)),m_sizingParametersView,SIGNAL(toggleUnitsClicked(bool)));

  m_sizingParametersView->addQuantityEdit("Zone Cooling Design Supply Air Temperature",
                                          "zoneCoolingDesignSupplyAirTemperature");

  m_sizingParametersView->addQuantityEdit("Zone Heating Design Supply Air Temperature",
                                          "zoneHeatingDesignSupplyAirTemperature");

  m_sizingParametersView->addQuantityEdit("Zone Cooling Design Supply Air Humidity Ratio",
                                          "zoneCoolingDesignSupplyAirHumidityRatio");

  m_sizingParametersView->addQuantityEdit("Zone Heating Design Supply Air Humidity Ratio",
                                          "zoneHeatingDesignSupplyAirHumidityRatio");

  m_sizingParametersView->addQuantityEdit("Zone Heating Sizing Factor",
                                          "zoneHeatingSizingFactor");
  
  m_sizingParametersView->addQuantityEdit("Zone Cooling Sizing Factor",
                                          "zoneCoolingSizingFactor");

  m_sizingParametersView->addComboBox("Cooling Design Air Flow Method",
                                       "coolingDesignAirFlowMethod");

  m_sizingParametersView->addQuantityEdit("Cooling Design Air Flow Rate",
                                          "coolingDesignAirFlowRate");
  
  m_sizingParametersView->addQuantityEdit("Heating Maximum Air Flow per Zone Floor Area",
                                          "heatingMaximumAirFlowperZoneFloorArea");
  
  m_sizingParametersView->addQuantityEdit("Heating Maximum Air Flow Fraction",
                                          "heatingMaximumAirFlowFraction");
  
  m_sizingParametersView->addQuantityEdit("Design Zone Air Distribution Effectiveness in Cooling Mode",
                                          "designZoneAirDistributionEffectivenessinCoolingMode");
  
  m_sizingParametersView->addQuantityEdit("Design Zone Air Distribution Effectiveness in Heating Mode",
                                          "designZoneAirDistributionEffectivenessinHeatingMode");

  sizingLayout->setColumnStretch(2,100);

  vLayout->addStretch();

  refresh();
}

void ThermalZoneView::onZoneHVACEquipmentItemClicked(OSItem* item)
{
  boost::optional<model::ModelObject> mo = OSAppBase::instance()->currentDocument()->getModelObject(item->itemId());

  emit modelObjectSelected(mo, item->itemId().isDefaulted());
}

boost::optional<model::ThermalZone> ThermalZoneView::thermalZone()
{
  if( boost::optional<model::ModelObject> mo = modelObject() )
  {
    return mo->optionalCast<model::ThermalZone>();
  }
  
  return boost::none;
}

void ThermalZoneView::onClearSelection()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_idealLoadsBox->unbind();

  m_equipmentVectorController->detach();
  m_heatingThermostatVectorController->detach();
  m_coolingThermostatVectorController->detach();
  m_humidifyingHumidistatVectorController->detach();
  m_dehumidifyingHumidistatVectorController->detach();

  m_sizingParametersView->unbind();
}

void ThermalZoneView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  this->stackedWidget()->setCurrentIndex(1);

  model::ThermalZone thermalZone = modelObject.cast<model::ThermalZone>();

  model::SizingZone sizingZone = thermalZone.sizingZone();

  m_nameEdit->bind(thermalZone,"name");
  m_idealLoadsBox->bind(thermalZone,"useIdealAirLoads");

  m_equipmentVectorController->attach(thermalZone);

  m_sizingParametersView->bind(sizingZone);

  refresh();
}

void ThermalZoneView::onUpdate()
{
  refresh();
}

void ThermalZoneView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ThermalZoneView::refresh()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(refreshNow()));
}

void ThermalZoneView::refreshNow()
{
  if( m_dirty )
  {
    if( boost::optional<model::ThermalZone> zone = this->thermalZone() )
    {
      boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = zone->thermostatSetpointDualSetpoint();
      boost::optional<model::ZoneControlHumidistat> humidistat = zone->zoneControlHumidistat();
    
      if(thermostat)
      {
        m_thermostatButton->setChecked(true);
      
        m_heatingThermostatDropZone->setEnabled(true);
        m_coolingThermostatDropZone->setEnabled(true);

        m_heatingThermostatVectorController->attach(thermostat.get());
        m_coolingThermostatVectorController->attach(thermostat.get());

        m_heatingThermostatVectorController->reportItems(); 
        m_coolingThermostatVectorController->reportItems(); 
      }
      else
      {
        m_thermostatButton->setChecked(false);

        m_heatingThermostatVectorController->detach();
        m_coolingThermostatVectorController->detach();

        m_heatingThermostatVectorController->reportItems(); 
        m_coolingThermostatVectorController->reportItems(); 

        m_heatingThermostatDropZone->setEnabled(false);
        m_coolingThermostatDropZone->setEnabled(false);
      }

      if(humidistat)
      {
        m_humidistatButton->setChecked(true);
      
        m_humidifyingHumidistatDropZone->setEnabled(true);
        m_dehumidifyingHumidistatDropZone->setEnabled(true);

        m_humidifyingHumidistatVectorController->attach(humidistat.get());
        m_dehumidifyingHumidistatVectorController->attach(humidistat.get());

        m_humidifyingHumidistatVectorController->reportItems(); 
        m_dehumidifyingHumidistatVectorController->reportItems(); 
      }
      else
      {
        m_humidistatButton->setChecked(false);

        m_humidifyingHumidistatVectorController->detach();
        m_dehumidifyingHumidistatVectorController->detach();

        m_humidifyingHumidistatVectorController->reportItems(); 
        m_dehumidifyingHumidistatVectorController->reportItems(); 

        m_humidifyingHumidistatDropZone->setEnabled(false);
        m_dehumidifyingHumidistatDropZone->setEnabled(false);
      }

      if( boost::optional<model::AirLoopHVAC> airLoop = zone->airLoopHVAC() )
      {
        m_airLoopNameLabel->setText(QString::fromStdString(airLoop->name().get()));
      }
      else
      {
        m_airLoopNameLabel->setText("None");
      }

      m_dirty = false;
    }
  }
}

void ThermalZoneView::onToggleUnitsClicked(bool isIP)
{
  m_isIP = isIP;
}

void ThermalZoneView::onThermostatButtonClicked(bool state)
{
  if( boost::optional<model::ThermalZone> zone = this->thermalZone() )
  {
    emit enableThermostatClicked(zone.get(),state);
  }
}

void ThermalZoneView::onHumidistatButtonClicked(bool state)
{
  if( boost::optional<model::ThermalZone> zone = this->thermalZone() )
  {
    emit enableHumidistatClicked(zone.get(),state);
  }
}

void ZoneEquipmentVectorController::reportItemsLater()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(reportItemsOnce()));
}

void ZoneEquipmentVectorController::reportItemsOnce()
{
  if( m_dirty )
  {
    reportItems();

    m_dirty = false;
  }
}

std::vector<OSItemId> ZoneEquipmentVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( m_thermalZone && (! m_thermalZone->handle().isNull()) )
  {
    std::vector<model::ModelObject> zoneEquipment = m_thermalZone->equipmentInHeatingOrder();

    for( std::vector<model::ModelObject>::iterator it = zoneEquipment.begin();
         it != zoneEquipment.end();
         ++it )
    {
      result.push_back(modelObjectToItemId(*it, false));
    }
  }
  
  return result;
}

void ZoneEquipmentVectorController::onRemoveItem(OSItem* item)
{
  if( m_thermalZone )
  {

    boost::optional<model::ModelObject> mo = OSAppBase::instance()->currentDocument()->getModelObject(item->itemId());

    if( mo )
    {
      mo->remove();

      reportItemsLater();
    }
  }
}

void ZoneEquipmentVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  if( m_thermalZone )
  {
    boost::optional<model::ModelObject> currentMo = OSAppBase::instance()->currentDocument()->getModelObject(currentItem->itemId());
    boost::optional<model::ModelObject> replacementMo = OSAppBase::instance()->currentDocument()->getModelObject(replacementItemId);

    if( currentMo && replacementMo )
    {
      if( boost::optional<model::ZoneHVACComponent> zoneHVACComp = replacementMo->optionalCast<model::ZoneHVACComponent>() )
      {
        currentMo->remove();

        model::ZoneHVACComponent compClone = zoneHVACComp->clone(m_thermalZone->model()).cast<model::ZoneHVACComponent>();

        bool added = compClone.addToThermalZone(m_thermalZone.get());

        if( ! added )
        {
          compClone.remove();
        }

        reportItemsLater();
      }
    }
  }
}

void ZoneEquipmentVectorController::onDrop(const OSItemId& itemId)
{
  if( m_thermalZone )
  {
    boost::optional<model::ModelObject> mo = OSAppBase::instance()->currentDocument()->getModelObject(itemId);

    if( mo )
    {
      if( boost::optional<model::ZoneHVACComponent> zoneHVACComp = mo->optionalCast<model::ZoneHVACComponent>() )
      {
        model::ZoneHVACComponent compClone = zoneHVACComp->clone(m_thermalZone->model()).cast<model::ZoneHVACComponent>();

        bool added = compClone.addToThermalZone(m_thermalZone.get());

        if( ! added )
        {
          compClone.remove();
        }

        reportItemsLater();
      }
    }
  }
}

void ZoneEquipmentVectorController::attach(model::ThermalZone & thermalZone)
{
  detach();

  m_thermalZone = thermalZone;

  ModelObjectVectorController::attach(thermalZone.getImpl<model::detail::ThermalZone_Impl>()->zoneHVACEquipmentList());

  reportItemsLater();
}

void ZoneEquipmentVectorController::detach()
{
  m_thermalZone = boost::none;

  ModelObjectVectorController::detach();

  reportItemsLater();
}

void ZoneEquipmentVectorController::onChange(const openstudio::model::ModelObject& modelObject)
{
  reportItemsLater();
}

void ThermalZoneView::HeatingScheduleVectorController::onChangeRelationship(
       const openstudio::model::ModelObject& modelObject, 
       int index, 
       Handle newHandle, 
       Handle oldHandle)
{
  if( index == OS_ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName )
  {
    reportItems();
  }
}

void ThermalZoneView::HeatingScheduleVectorController::onObjectRemoved( const openstudio::model::ModelObject& modelObject, 
                                                                        const openstudio::IddObjectType& iddObjectType, 
                                                                        const openstudio::UUID& handle )
{
  if( boost::optional<model::ThermostatSetpointDualSetpoint> t = thermostatSetpointDualSetpoint() )
  {
    if( t->handle() == handle )
    {
      reportItems();
    }
    else if( ! t->getHeatingSchedule() )
    {
      reportItems();
    }
  }
}

boost::optional<model::ThermostatSetpointDualSetpoint> 
  ThermalZoneView::HeatingScheduleVectorController::thermostatSetpointDualSetpoint()
{
  if( m_modelObject )
  {
    return m_modelObject->optionalCast<model::ThermostatSetpointDualSetpoint>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> ThermalZoneView::HeatingScheduleVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::ThermostatSetpointDualSetpoint> mo = thermostatSetpointDualSetpoint() )
  {
    if( boost::optional<model::Schedule> s = mo->getHeatingSchedule() )
    {
      result.push_back(modelObjectToItemId(s.get(), false)); 
    }
  }

  return result;
}

void ThermalZoneView::HeatingScheduleVectorController::onRemoveItem(OSItem* item)
{
  if( boost::optional<model::ThermostatSetpointDualSetpoint> mo = thermostatSetpointDualSetpoint() )
  {
    mo->resetHeatingSchedule();
  }
}

void ThermalZoneView::HeatingScheduleVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void ThermalZoneView::HeatingScheduleVectorController::onDrop(const OSItemId& itemId)
{
  if( boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = thermostatSetpointDualSetpoint() )
  {
    boost::optional<model::ModelObject> mo = this->getModelObject(itemId);

    if (mo)
    {
      if (mo->optionalCast<model::Schedule>())
      {
        if (this->fromComponentLibrary(itemId))
        {
          mo = mo->clone(thermostat->model());
        }
        model::Schedule schedule = mo->cast<model::Schedule>();
        thermostat->setHeatingSetpointTemperatureSchedule(schedule);
      }
    }
  }
}

void ThermalZoneView::CoolingScheduleVectorController::onChangeRelationship(
       const openstudio::model::ModelObject& modelObject, 
       int index, 
       Handle newHandle, 
       Handle oldHandle)
{
  if( index == OS_ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName )
  {
    reportItems();
  }
}

void ThermalZoneView::CoolingScheduleVectorController::onObjectRemoved( const openstudio::model::ModelObject& modelObject, 
                                                                        const openstudio::IddObjectType& iddObjectType, 
                                                                        const openstudio::UUID& handle )
{
  if( boost::optional<model::ThermostatSetpointDualSetpoint> t = thermostatSetpointDualSetpoint() )
  {
    if( t->handle() == handle )
    {
      reportItems();
    }
    else if( ! t->getCoolingSchedule() )
    {
      reportItems();
    }
  }
}

boost::optional<model::ThermostatSetpointDualSetpoint> 
  ThermalZoneView::CoolingScheduleVectorController::thermostatSetpointDualSetpoint()
{
  if( m_modelObject )
  {
    return m_modelObject->optionalCast<model::ThermostatSetpointDualSetpoint>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> ThermalZoneView::CoolingScheduleVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::ThermostatSetpointDualSetpoint> mo = thermostatSetpointDualSetpoint() )
  {
    if( boost::optional<model::Schedule> s = mo->getCoolingSchedule() )
    {
      result.push_back(modelObjectToItemId(s.get(), false)); 
    }
  }

  return result;
}

void ThermalZoneView::CoolingScheduleVectorController::onRemoveItem(OSItem* item)
{
  if( boost::optional<model::ThermostatSetpointDualSetpoint> mo = thermostatSetpointDualSetpoint() )
  {
    mo->resetCoolingSchedule();
  }
}

void ThermalZoneView::CoolingScheduleVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void ThermalZoneView::CoolingScheduleVectorController::onDrop(const OSItemId& itemId)
{
  if( boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = thermostatSetpointDualSetpoint() )
  {
    boost::optional<model::ModelObject> mo = this->getModelObject(itemId);

    if (mo)
    {
      if (mo->optionalCast<model::Schedule>())
      {
        if (this->fromComponentLibrary(itemId))
        {
          mo = mo->clone(thermostat->model());
        }

        model::Schedule schedule = mo->cast<model::Schedule>();
        thermostat->setCoolingSetpointTemperatureSchedule(schedule);
      }
    }
  }
}

void ThermalZoneView::HumidifyingScheduleVectorController::onChangeRelationship(
       const openstudio::model::ModelObject& modelObject, 
       int index, 
       Handle newHandle, 
       Handle oldHandle)
{
  if( index == OS_ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName )
  {
    reportItems();
  }
}

void ThermalZoneView::HumidifyingScheduleVectorController::onObjectRemoved( const openstudio::model::ModelObject& modelObject, 
                                                                        const openstudio::IddObjectType& iddObjectType, 
                                                                        const openstudio::UUID& handle )
{
  if( boost::optional<model::ZoneControlHumidistat> h = zoneControlHumidistat() )
  {
    if( h->handle() == handle )
    {
      reportItems();
    }
    else if( ! h->humidifyingRelativeHumiditySetpointSchedule() )
    {
      reportItems();
    }
  }
}

boost::optional<model::ZoneControlHumidistat> 
  ThermalZoneView::HumidifyingScheduleVectorController::zoneControlHumidistat()
{
  if( m_modelObject )
  {
    return m_modelObject->optionalCast<model::ZoneControlHumidistat>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> ThermalZoneView::HumidifyingScheduleVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::ZoneControlHumidistat> mo = zoneControlHumidistat() )
  {
    if( boost::optional<model::Schedule> s = mo->humidifyingRelativeHumiditySetpointSchedule() )
    {
      result.push_back(modelObjectToItemId(s.get(), false)); 
    }
  }

  return result;
}

void ThermalZoneView::HumidifyingScheduleVectorController::onRemoveItem(OSItem* item)
{
  if( boost::optional<model::ZoneControlHumidistat> mo = zoneControlHumidistat() )
  {
    mo->resetHumidifyingRelativeHumiditySetpointSchedule();
  }
}

void ThermalZoneView::HumidifyingScheduleVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void ThermalZoneView::HumidifyingScheduleVectorController::onDrop(const OSItemId& itemId)
{
  if( boost::optional<model::ZoneControlHumidistat> humidistat = zoneControlHumidistat() )
  {
    boost::optional<model::ModelObject> mo = this->getModelObject(itemId);

    if (mo)
    {
      if (mo->optionalCast<model::Schedule>())
      {
        if (this->fromComponentLibrary(itemId))
        {
          mo = mo->clone(humidistat->model());
        }
        model::Schedule schedule = mo->cast<model::Schedule>();
        humidistat->setHumidifyingRelativeHumiditySetpointSchedule(schedule);
      }
    }
  }
}

void ThermalZoneView::DehumidifyingScheduleVectorController::onChangeRelationship(
       const openstudio::model::ModelObject& modelObject, 
       int index, 
       Handle newHandle, 
       Handle oldHandle)
{
  if( index == OS_ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName )
  {
    reportItems();
  }
}

void ThermalZoneView::DehumidifyingScheduleVectorController::onObjectRemoved( const openstudio::model::ModelObject& modelObject, 
                                                                        const openstudio::IddObjectType& iddObjectType, 
                                                                        const openstudio::UUID& handle )
{
  if( boost::optional<model::ZoneControlHumidistat> t = zoneControlHumidistat() )
  {
    if( t->handle() == handle )
    {
      reportItems();
    }
    else if( ! t->dehumidifyingRelativeHumiditySetpointSchedule() )
    {
      reportItems();
    }
  }
}

boost::optional<model::ZoneControlHumidistat> 
  ThermalZoneView::DehumidifyingScheduleVectorController::zoneControlHumidistat()
{
  if( m_modelObject )
  {
    return m_modelObject->optionalCast<model::ZoneControlHumidistat>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> ThermalZoneView::DehumidifyingScheduleVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::ZoneControlHumidistat> mo = zoneControlHumidistat() )
  {
    if( boost::optional<model::Schedule> s = mo->dehumidifyingRelativeHumiditySetpointSchedule() )
    {
      result.push_back(modelObjectToItemId(s.get(), false)); 
    }
  }

  return result;
}

void ThermalZoneView::DehumidifyingScheduleVectorController::onRemoveItem(OSItem* item)
{
  if( boost::optional<model::ZoneControlHumidistat> mo = zoneControlHumidistat() )
  {
    mo->resetDehumidifyingRelativeHumiditySetpointSchedule();
  }
}

void ThermalZoneView::DehumidifyingScheduleVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void ThermalZoneView::DehumidifyingScheduleVectorController::onDrop(const OSItemId& itemId)
{
  if( boost::optional<model::ZoneControlHumidistat> humidistat = zoneControlHumidistat() )
  {
    boost::optional<model::ModelObject> mo = this->getModelObject(itemId);

    if (mo)
    {
      if (mo->optionalCast<model::Schedule>())
      {
        if (this->fromComponentLibrary(itemId))
        {
          mo = mo->clone(humidistat->model());
        }

        model::Schedule schedule = mo->cast<model::Schedule>();
        humidistat->setDehumidifyingRelativeHumiditySetpointSchedule(schedule);
      }
    }
  }
}

ModelObjectPropertyView::ModelObjectPropertyView(bool isIP)
  : QWidget(),
    m_isIP(isIP)
{
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->setContentsMargins(0,0,0,0);
  m_mainLayout->setSpacing(10);

  setLayout(m_mainLayout);
}

void ModelObjectPropertyView::addQuantityEdit(const QString & label, const char* propertyName)
{
  QWidget * widget = new QWidget();

  QVBoxLayout * mainVBoxLayout = new QVBoxLayout();
  mainVBoxLayout->setContentsMargins(0,0,0,0);
  mainVBoxLayout->setSpacing(5);

  widget->setLayout(mainVBoxLayout);

  QLabel * labelWidget = new QLabel(label);
  labelWidget->setObjectName("H2");

  mainVBoxLayout->addWidget(labelWidget);

  OSQuantityEdit * quantityEdit = new OSQuantityEdit(m_isIP);
  connect(this, SIGNAL(toggleUnitsClicked(bool)), 
          quantityEdit, SLOT(onUnitSystemChange(bool)));
  mainVBoxLayout->addWidget(quantityEdit);

  m_labels.push_back(label);
  m_propertyNames.push_back(propertyName);
  m_views.push_back(quantityEdit);

  m_mainLayout->addWidget(widget);
}

void ModelObjectPropertyView::addComboBox(const QString & label, const char* propertyName)
{
  QWidget * widget = new QWidget();

  QVBoxLayout * mainVBoxLayout = new QVBoxLayout();
  mainVBoxLayout->setContentsMargins(0,0,0,0);
  mainVBoxLayout->setSpacing(5);

  widget->setLayout(mainVBoxLayout);

  QLabel * labelWidget = new QLabel(label);
  labelWidget->setObjectName("H2");

  mainVBoxLayout->addWidget(labelWidget);

  QHBoxLayout * hBoxLayout = new QHBoxLayout();
  OSComboBox * comboBox = new OSComboBox();
  hBoxLayout->addWidget(comboBox);
  hBoxLayout->addStretch();

  mainVBoxLayout->addLayout(hBoxLayout);

  m_labels.push_back(label);
  m_propertyNames.push_back(propertyName);
  m_views.push_back(comboBox);

  m_mainLayout->addWidget(widget);
}

void ModelObjectPropertyView::bind(model::ModelObject& modelObject)
{
  int i = 0;

  for( std::vector<QWidget *>::iterator it = m_views.begin();
       it != m_views.end();
       ++it )
  {
    const char* propertyName = m_propertyNames[i];

    if( OSQuantityEdit * qe = qobject_cast<OSQuantityEdit *>(*it) )
    {
      qe->bind(modelObject,propertyName,m_isIP);
    }
    else if( OSComboBox * cb = qobject_cast<OSComboBox *>(*it) )
    {
      cb->bind(modelObject,propertyName);
    }

    i++;
  }
}

void ModelObjectPropertyView::unbind()
{
  for( std::vector<QWidget *>::iterator it = m_views.begin();
       it != m_views.end();
       ++it )
  {
    if( OSQuantityEdit * qe = qobject_cast<OSQuantityEdit *>(*it) )
    {
      qe->unbind();
    }
    else if( OSComboBox * cb = qobject_cast<OSComboBox *>(*it) )
    {
      cb->unbind();
    }
  }
}

} // openstudio

