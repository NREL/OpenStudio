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

#include "ThermalZonesGridView.hpp"

#include <shared_gui_components/OSGridView.hpp>

#include <openstudio_lib/ModelObjectItem.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>

#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include <model/SizingZone.hpp>
#include <model/SizingZone_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QTimer>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Name"

//HVAC SYSTEMS
#define IDEALAIRLOADS "Turn On\nIdeal\nAir Loads"
#define AIRLOOPNAME "Air Loop Name"
#define ZONEEQUIPMENT "Zone Equipment"
#define COOLINGTHERMOSTATSCHEDULE "Cooling Thermostat\nSchedule"
#define HEATINGTHERMOSTATSCHEDULE "Heating Thermostat\nSchedule"
#define HUMIDIFYINGSETPOINTSCHEDULE "Humidifying Setpoint\nSchedule"
#define DEHUMIDIFYINGSETPOINTSCHEDULE "Dehumidifying Setpoint\nSchedule"
#define MULTIPLIER "Multiplier"

//COOLING SIZING PARAMETERS
#define ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE "Zone Cooling\nDesign Supply\nAir Temperature"
#define ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO "Zone Cooling\nDesign Supply\nAir Humidity Ratio"
#define ZONECOOLINGSIZINGFACTOR "Zone Cooling\nSizing Factor"
#define COOLINGDESIGNAIRFLOWMETHOD "Cooling Design\nAir Flow Method"
#define COOLINGDESIGNAIRFLOWRATE "Cooling Design\nAir Flow Rate"
#define DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE "Design Zone Air\nDistribution Effectiveness\nin Cooling Mode"

//HEATING SIZING PARAMETERS
#define ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE "Zone Heating\nDesign Supply\nAir Temperature"
#define ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO "Zone Heating\nDesign Supply\nAir Humidity Ratio"
#define ZONEHEATINGSIZINGFACTOR "Zone Heating\nSizing Factor"
#define HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA "Heating Maximum Air\nFlow per Zone\nFloor Area"
#define HEATINGMAXIMUMAIRFLOWFRACTION "Heating Maximum\nAir Flow Fraction"
#define DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE "Design Zone Air\nDistribution Effectiveness\nin Heating Mode"

namespace openstudio {

struct ModelObjectNameSorter{
  // sort by name
  bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
    return (lhs.name() < rhs.name());
  }
};

ThermalZonesGridView::ThermalZonesGridView(bool isIP, const model::Model & model, QWidget * parent)
  : QWidget(parent),
  m_isIP(isIP)
{
  QVBoxLayout * layout = 0;

  layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);
  
  std::vector<model::ThermalZone> thermalZones = model.getModelObjects<model::ThermalZone>();
  std::vector<model::ModelObject> thermalZoneModelObjects = subsetCastVector<model::ModelObject>(thermalZones);

  ThermalZonesGridController * thermalZonesGridController  = new ThermalZonesGridController(m_isIP, "Thermal Zones", model, thermalZoneModelObjects);
  OSGridView * gridView = new OSGridView(thermalZonesGridController, "Thermal Zones", "Drop\nZone", parent);
  layout->addWidget(gridView,0,Qt::AlignTop);

  layout->addStretch(1);

  bool isConnected = false;

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        thermalZonesGridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        thermalZonesGridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  std::vector<model::ThermalZone> thermalZone = model.getModelObjects<model::ThermalZone>(); // NOTE for horizontal system lists

}

ThermalZonesGridController::ThermalZonesGridController(bool isIP,
  const QString & headerText,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, model, modelObjects)
{
  setCategoriesAndFields();
}

void ThermalZonesGridController::setCategoriesAndFields()
{

  {
    std::vector<QString> fields;
    fields.push_back(IDEALAIRLOADS);
    //fields.push_back(AIRLOOPNAME);                   // TODO LineEdit, but needs an optional AirLoopHVAC 
    //fields.push_back(ZONEEQUIPMENT);                 // TODO Extensible DropZone
    //fields.push_back(COOLINGTHERMOSTATSCHEDULE);     // TODO DropZone
    //fields.push_back(HEATINGTHERMOSTATSCHEDULE);     // TODO DropZone
    //fields.push_back(HUMIDIFYINGSETPOINTSCHEDULE);   // TODO DropZone
    //fields.push_back(DEHUMIDIFYINGSETPOINTSCHEDULE); // TODO DropZone
    fields.push_back(MULTIPLIER);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("HVAC Systems"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE);
    fields.push_back(ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO);
    fields.push_back(ZONECOOLINGSIZINGFACTOR);
    fields.push_back(COOLINGDESIGNAIRFLOWMETHOD);
    fields.push_back(COOLINGDESIGNAIRFLOWRATE);
    fields.push_back(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Cooling Sizing Parameters"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE);
    fields.push_back(ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO);
    fields.push_back(ZONEHEATINGSIZINGFACTOR);
    fields.push_back(HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA);
    fields.push_back(HEATINGMAXIMUMAIRFLOWFRACTION);
    fields.push_back(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Heating  Sizing Parameters"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}

void ThermalZonesGridController::addColumns(std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), NAME);

  m_baseConcepts.clear();

  Q_FOREACH(QString field, fields){
    if(field == IDEALAIRLOADS){
      addCheckBoxColumn(QString(IDEALAIRLOADS),
                        NullAdapter(&model::ThermalZone::useIdealAirLoads),
                        NullAdapter(&model::ThermalZone::setUseIdealAirLoads));

    }else if(field == ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE){
      addQuantityEditColumn(QString(ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneCoolingDesignSupplyAirTemperature, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<void (model::SizingZone::*)(double)>(&model::SizingZone::setZoneCoolingDesignSupplyAirTemperature), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE){
      addQuantityEditColumn(QString(ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneHeatingDesignSupplyAirTemperature, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<void (model::SizingZone::*)(double)>(&model::SizingZone::setZoneHeatingDesignSupplyAirTemperature), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO){
      addQuantityEditColumn(QString(ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneCoolingDesignSupplyAirHumidityRatio, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneCoolingDesignSupplyAirHumidityRatio), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO){
      addQuantityEditColumn(QString(ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneHeatingDesignSupplyAirHumidityRatio, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneHeatingDesignSupplyAirHumidityRatio), 
                            &model::ThermalZone::sizingZone));
    }else if(field == ZONEHEATINGSIZINGFACTOR){
      addQuantityEditColumn(QString(ZONEHEATINGSIZINGFACTOR),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneHeatingSizingFactor, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneHeatingSizingFactor), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONECOOLINGSIZINGFACTOR){
      addQuantityEditColumn(QString(),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneCoolingSizingFactor, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneCoolingSizingFactor), 
                              &model::ThermalZone::sizingZone));
    }else if(field == COOLINGDESIGNAIRFLOWRATE){
      addQuantityEditColumn(QString(COOLINGDESIGNAIRFLOWRATE),
                            QString("m^3/s"),
                            QString("m^3/s"),
                            QString("ft^3/s"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::coolingDesignAirFlowRate, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setCoolingDesignAirFlowRate), 
                              &model::ThermalZone::sizingZone));
    }else if(field == HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA){
      addQuantityEditColumn(QString(HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA),
                            QString("m/s"),
                            QString("m/s"),
                            QString("ft/min"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::heatingMaximumAirFlowperZoneFloorArea, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setHeatingMaximumAirFlowperZoneFloorArea), 
                              &model::ThermalZone::sizingZone));
    }else if(field == HEATINGMAXIMUMAIRFLOWFRACTION){
      addQuantityEditColumn(QString(HEATINGMAXIMUMAIRFLOWFRACTION),
                            QString("W/m"),
                            QString("W/m"),
                            QString("Btu/hr*ft"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::heatingMaximumAirFlowFraction, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setHeatingMaximumAirFlowFraction), 
                              &model::ThermalZone::sizingZone));
    }else if(field == DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE){
      addQuantityEditColumn(QString(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE),
                            QString("W/m"),
                            QString("W/m"),
                            QString("Btu/hr*ft"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::designZoneAirDistributionEffectivenessinCoolingMode, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setDesignZoneAirDistributionEffectivenessinCoolingMode), 
                              &model::ThermalZone::sizingZone));
    }else if(field == DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE){
      addQuantityEditColumn(QString(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE),
                            QString("W/m"),
                            QString("W/m"),
                            QString("Btu/hr*ft"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::designZoneAirDistributionEffectivenessinHeatingMode, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setDesignZoneAirDistributionEffectivenessinHeatingMode), 
                              &model::ThermalZone::sizingZone));

    }else if(field == COOLINGDESIGNAIRFLOWMETHOD){
      addComboBoxColumn(QString(COOLINGDESIGNAIRFLOWMETHOD),
                        boost::function<std::string (const std::string &)>(static_cast<std::string (*)(const std::string&)>(&openstudio::toString)),
                        boost::function<std::vector<std::string> ()>(&model::SizingZone::coolingDesignAirFlowMethodValues),
                        ProxyAdapter(&model::SizingZone::coolingDesignAirFlowMethod, &model::ThermalZone::sizingZone),
                        ProxyAdapter(static_cast<bool (model::SizingZone::*)(const std::string &)>(&model::SizingZone::setCoolingDesignAirFlowMethod),
                          &model::ThermalZone::sizingZone));

    //}else if(field == COOLINGTHERMOSTATSCHEDULE){
      //addDropZoneColumn<std::string,SizingZoneProxy>(QString(COOLINGTHERMOSTATSCHEDULE),
      //                                               static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      //                                               &SizingZoneProxy::coolingDesignAirFlowMethodValues,
      //                                               boost::function<std::string (SizingZoneProxy*)>(&SizingZoneProxy::coolingThermostatSchedule),
      //                                               &SizingZoneProxy::setCoolingThermostatSchedule);
      //                                               ProxyAdapter(&model::SizingZone::??, &model::ThermalZone::sizingZone),
      //                                               ProxyAdapter(static_cast<void (model::SizingZone::*)(double)>(&model::SizingZone::??), 
      //                                                 &model::ThermalZone::sizingZone));

    //fields.push_back(HEATINGTHERMOSTATSCHEDULE);     // TODO DropZone
    //fields.push_back(HUMIDIFYINGSETPOINTSCHEDULE);   // TODO DropZone
    //fields.push_back(DEHUMIDIFYINGSETPOINTSCHEDULE); // TODO DropZone
    //fields.push_back(ZONEEQUIPMENT);                 // TODO Extensible DropZone

    }else if(field == NAME){
      addNameLineEditColumn(QString(NAME),
                            NullAdapter(&model::ThermalZone::name),
                            NullAdapter(&model::ThermalZone::setName));

    //}else if(field == AIRLOOPNAME){
    //  addNameLineEditColumn(QString(AIRLOOPNAME),
    //                        ProxyAdapter(&model::AirLoopHVAC::name, &model::ThermalZone::airLoopHVAC),
    //                        ProxyAdapter(static_cast<void (model::AirLoopHVAC::*)(std::string)>(&model::AirLoopHVAC::setName), 
    //                        &model::ThermalZone::airLoopHVAC));

    }else if(field == MULTIPLIER){
      addValueEditColumn(QString(MULTIPLIER),
                         NullAdapter(&model::ThermalZone::multiplier),
                         NullAdapter(&model::ThermalZone::setMultiplier));

    }else{
      // unhandled
      //OS_ASSERT(false);
    }
  }
}

QString ThermalZonesGridController::getColor(const model:: ModelObject & modelObject)
{
  QColor defaultColor(Qt::lightGray);
  QString color(defaultColor.name());

  // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
  // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

  //std::vector<model::RefrigerationSystem> refrigerationSystems = m_model.getModelObjects<model::RefrigerationSystem>();

  //boost::optional<model::ThermalZone> refrigerationCase = modelObject.optionalCast<model::ThermalZone>();
  //OS_ASSERT(refrigerationCase);

  //boost::optional<model::RefrigerationSystem> refrigerationSystem = refrigerationCase->system();
  //if(!refrigerationSystem){
  //  return color;
  //}

  //std::vector<model::RefrigerationSystem>::iterator it;
  //it = std::find(refrigerationSystems.begin(), refrigerationSystems.end(), refrigerationSystem.get());
  //if(it != refrigerationSystems.end()){
  //  int index = std::distance(refrigerationSystems.begin(), it);
  //  if(index >= static_cast<int>(m_colors.size())){
  //    index = m_colors.size() - 1; // similar to scheduleView's approach
  //  }
  //  color = this->m_colors.at(index).name();
  //}

  return color;
}

void ThermalZonesGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  // Don't show the name column check box
  // From above in addColumns, we know that NAME is the first entry
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(0));
  OS_ASSERT(horizontalHeaderWidget);
  horizontalHeaderWidget->m_checkBox->hide();

  OSGridController::checkSelectedFields();
}

void ThermalZonesGridController::onItemDropped(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::ThermalZone>()){
      modelObject->clone(m_model);
      emit modelReset();
    }
  }
}

void ThermalZonesGridController::refreshModelObjects()
{
  std::vector<model::ThermalZone> refrigerationCases = m_model.getModelObjects<model::ThermalZone>();
  m_modelObjects = subsetCastVector<model::ModelObject>(refrigerationCases);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void ThermalZonesGridController::onComboBoxIndexChanged(int index)
{
}

} // openstudio

