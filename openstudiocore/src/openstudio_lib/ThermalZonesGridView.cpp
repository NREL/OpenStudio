/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelSubTabView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/SizingZone.hpp"
#include "../model/SizingZone_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/ZoneControlHumidistat.hpp"
#include "../model/ZoneControlHumidistat_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"

#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_ThermalZone_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QStackedWidget>
#include <QSettings>
#include <QTimer>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Name"
#define SELECTED "All"

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
#define COOLINGMINIMUMAIRFLOWPERZONEFLOORAREA "Cooling Minimum Air\nFlow per Zone\nFloor Area"
#define DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE "Design Zone Air\nDistribution Effectiveness\nin Cooling Mode"
#define COOLINGMINIMUMAIRFLOWFRACTION "Cooling Minimum\nAir Flow Fraction"
#define COOLINGDESIGNAIRFLOWMETHOD "Cooling Design\nAir Flow Method"
#define COOLINGDESIGNAIRFLOWRATE "Cooling Design\nAir Flow Rate"
#define COOLINGMINIMUMAIRFLOW "Cooling Minimum\nAir Flow"

//HEATING SIZING PARAMETERS
#define ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE "Zone Heating\nDesign Supply\nAir Temperature"
#define ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO "Zone Heating\nDesign Supply\nAir Humidity Ratio"
#define ZONEHEATINGSIZINGFACTOR "Zone Heating\nSizing Factor"
#define HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA "Heating Maximum Air\nFlow per Zone\nFloor Area"
#define DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE "Design Zone Air\nDistribution Effectiveness\nin Heating Mode"
#define HEATINGMAXIMUMAIRFLOWFRACTION "Heating Maximum\nAir Flow Fraction"
#define HEATINGDESIGNAIRFLOWMETHOD "Heating Design\nAir Flow Method"
#define HEATINGDESIGNAIRFLOWRATE "Heating Design\nAir Flow Rate"
#define HEATINGMAXIMUMAIRFLOW "Heating Maximum\nAir Flow"

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

  m_thermalZonesGridController = new ThermalZonesGridController(m_isIP, "Thermal Zones", IddObjectType::OS_ThermalZone, model, thermalZoneModelObjects);
  OSGridView * gridView = new OSGridView(m_thermalZonesGridController, "Thermal Zones", "Drop\nZone", false, parent);

  bool isConnected = false;

  isConnected = connect(gridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemSelected(OSItem *)), gridView, SIGNAL(itemSelected(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(selectionCleared()), gridView, SLOT(onSelectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(gridView, SIGNAL(gridRowSelected(OSItem*)), this, SIGNAL(gridRowSelected(OSItem*)));
  OS_ASSERT(isConnected);

  gridView->m_dropZone->hide();

  layout->addWidget(gridView,0,Qt::AlignTop);

  layout->addStretch(1);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalZonesGridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalZonesGridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  std::vector<model::ThermalZone> thermalZone = model.getModelObjects<model::ThermalZone>(); // NOTE for horizontal system lists

}

// THIS shoulf be in the base class
std::vector<model::ModelObject> ThermalZonesGridView::selectedObjects() const
{
  const auto os = m_thermalZonesGridController->getObjectSelector()->getSelectedObjects();

  return std::vector<model::ModelObject>(os.cbegin(), os.cend());
}

ThermalZonesGridController::ThermalZonesGridController(bool isIP,
  const QString & headerText,
  IddObjectType iddObjectType,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
{
  setCategoriesAndFields();
}

void ThermalZonesGridController::setCategoriesAndFields()
{

  {
    std::vector<QString> fields;
    fields.push_back(IDEALAIRLOADS);
    fields.push_back(AIRLOOPNAME);
    fields.push_back(ZONEEQUIPMENT);
    fields.push_back(COOLINGTHERMOSTATSCHEDULE);
    fields.push_back(HEATINGTHERMOSTATSCHEDULE);
    fields.push_back(HUMIDIFYINGSETPOINTSCHEDULE);
    fields.push_back(DEHUMIDIFYINGSETPOINTSCHEDULE);
    fields.push_back(MULTIPLIER);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("HVAC\nSystems"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE);
    fields.push_back(ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO);
    fields.push_back(ZONECOOLINGSIZINGFACTOR);
    fields.push_back(COOLINGMINIMUMAIRFLOWPERZONEFLOORAREA);
    fields.push_back(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE);
    fields.push_back(COOLINGMINIMUMAIRFLOWFRACTION);
    fields.push_back(COOLINGDESIGNAIRFLOWMETHOD);
    fields.push_back(COOLINGDESIGNAIRFLOWRATE);
    fields.push_back(COOLINGMINIMUMAIRFLOW);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Cooling\nSizing\nParameters"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE);
    fields.push_back(ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO);
    fields.push_back(ZONEHEATINGSIZINGFACTOR);
    fields.push_back(HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA);
    fields.push_back(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE);
    fields.push_back(HEATINGMAXIMUMAIRFLOWFRACTION);
    fields.push_back(HEATINGDESIGNAIRFLOWMETHOD);
    fields.push_back(HEATINGDESIGNAIRFLOWRATE);
    fields.push_back(HEATINGMAXIMUMAIRFLOW);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Heating\nSizing\nParameters"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}

void ThermalZonesGridController::addColumns(const QString &/*category*/, std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), {NAME, SELECTED});

  m_baseConcepts.clear();

  Q_FOREACH(QString field, fields){
    if(field == IDEALAIRLOADS){
      addCheckBoxColumn(Heading(QString(IDEALAIRLOADS),true,false),
                        std::string("Check to enable ideal air loads."),
                        NullAdapter(&model::ThermalZone::useIdealAirLoads),
                        NullAdapter(&model::ThermalZone::setUseIdealAirLoads));
    }else if(field == SELECTED){
      auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
      checkbox->setToolTip("Check to select all rows");
      connect(checkbox.data(), &QCheckBox::stateChanged, this, &ThermalZonesGridController::selectAllStateChanged);

      addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
    }else if(field == ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE){
      addQuantityEditColumn(Heading(QString(ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE)),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneCoolingDesignSupplyAirTemperature, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<void (model::SizingZone::*)(double)>(&model::SizingZone::setZoneCoolingDesignSupplyAirTemperature), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE){
      addQuantityEditColumn(Heading(QString(ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE)),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneHeatingDesignSupplyAirTemperature, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<void (model::SizingZone::*)(double)>(&model::SizingZone::setZoneHeatingDesignSupplyAirTemperature), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO){
      addQuantityEditColumn(Heading(QString(ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO)),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneCoolingDesignSupplyAirHumidityRatio, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneCoolingDesignSupplyAirHumidityRatio), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO){
      addQuantityEditColumn(Heading(QString(ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO)),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneHeatingDesignSupplyAirHumidityRatio, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneHeatingDesignSupplyAirHumidityRatio), 
                            &model::ThermalZone::sizingZone));
    }else if(field == ZONEHEATINGSIZINGFACTOR){
      addQuantityEditColumn(Heading(QString(ZONEHEATINGSIZINGFACTOR)),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneHeatingSizingFactor, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneHeatingSizingFactor), 
                              &model::ThermalZone::sizingZone));
    }else if(field == ZONECOOLINGSIZINGFACTOR){
      addQuantityEditColumn(Heading(QString()),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::zoneCoolingSizingFactor, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setZoneCoolingSizingFactor), 
                              &model::ThermalZone::sizingZone));

    }else if (field == HEATINGDESIGNAIRFLOWRATE){
      addQuantityEditColumn(Heading(QString(HEATINGDESIGNAIRFLOWRATE)),
        QString("m^3/s"),
        QString("m^3/s"),
        QString("ft^3/min"),
        m_isIP,
        ProxyAdapter(&model::SizingZone::heatingDesignAirFlowRate, &model::ThermalZone::sizingZone),
        ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setHeatingDesignAirFlowRate),
        &model::ThermalZone::sizingZone));
    }else if (field == HEATINGMAXIMUMAIRFLOW){
        addQuantityEditColumn(Heading(QString(HEATINGMAXIMUMAIRFLOW)),
        QString("m^3/s"),
        QString("m^3/s"),
        QString("ft^3/min"),
        m_isIP,
        ProxyAdapter(&model::SizingZone::heatingMaximumAirFlow, &model::ThermalZone::sizingZone),
        ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setHeatingMaximumAirFlow),
        &model::ThermalZone::sizingZone));
    }else if (field == HEATINGDESIGNAIRFLOWMETHOD){
      addComboBoxColumn<std::string, model::ThermalZone>(Heading(QString(HEATINGDESIGNAIRFLOWMETHOD)),
        std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
        std::function<std::vector<std::string>()>(&model::SizingZone::heatingDesignAirFlowMethodValues),
        std::function<std::string(model::ThermalZone *)>([](model::ThermalZone *t_z) {
          try {
            return t_z->sizingZone().heatingDesignAirFlowMethod();
          }
          catch (const std::exception &e) {
            // If this code is called there is no sizingZone currently set. This means
            // that the ThermalZone is probably in the process of being destructed. So,
            // we don't want to create a new sizingZone, we really just want to try to 
            // continue gracefully, so let's pretend that the current heatingDesignAirFlowMethod
            // is the 0th one in the list of possible options.
            //
            // This might not be the best solution to this problem
            LOG(Debug, "Exception while obtaining heatingDesignAirflowMethod " << e.what());
            return model::SizingZone::heatingDesignAirFlowMethodValues().at(0);
          }
        }
        ),
        std::function<bool(model::ThermalZone *, std::string)>([](model::ThermalZone *t_z, std::string t_val){ auto sz = t_z->sizingZone(); return sz.setHeatingDesignAirFlowMethod(t_val); }),
        boost::optional<std::function<void(openstudio::model::ThermalZone *)>>(),
        boost::optional<std::function<bool(openstudio::model::ThermalZone *)>>(),
        boost::optional<openstudio::DataSource>()
        );

    }else if(field == COOLINGDESIGNAIRFLOWRATE){
      addQuantityEditColumn(Heading(QString(COOLINGDESIGNAIRFLOWRATE)),
                            QString("m^3/s"),
                            QString("m^3/s"),
                            QString("ft^3/min"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::coolingDesignAirFlowRate, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setCoolingDesignAirFlowRate), 
                              &model::ThermalZone::sizingZone));
    }else if (field == COOLINGMINIMUMAIRFLOWPERZONEFLOORAREA){
      addQuantityEditColumn(Heading(QString(COOLINGMINIMUMAIRFLOWPERZONEFLOORAREA)),
        QString("m^3/min*m^2"),
        QString("m^3/min*m^2"),
        QString("ft^3/min*ft^2"),
        m_isIP,
        ProxyAdapter(&model::SizingZone::coolingMinimumAirFlowperZoneFloorArea, &model::ThermalZone::sizingZone),
        ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setCoolingMinimumAirFlowperZoneFloorArea),
        &model::ThermalZone::sizingZone));
    }
    else if (field == COOLINGMINIMUMAIRFLOW){
      addQuantityEditColumn(Heading(QString(COOLINGMINIMUMAIRFLOW)),
        QString("m^3/s"),
        QString("m^3/s"),
        QString("ft^3/min"),
        m_isIP,
        ProxyAdapter(&model::SizingZone::coolingMinimumAirFlow, &model::ThermalZone::sizingZone),
        ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setCoolingMinimumAirFlow),
        &model::ThermalZone::sizingZone));
    }else if (field == COOLINGMINIMUMAIRFLOWFRACTION){
      addQuantityEditColumn(Heading(QString(COOLINGMINIMUMAIRFLOWFRACTION)),
        QString(""),
        QString(""),
        QString(""),
        m_isIP,
        ProxyAdapter(&model::SizingZone::coolingMinimumAirFlowFraction, &model::ThermalZone::sizingZone),
        ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setCoolingMinimumAirFlowFraction),
        &model::ThermalZone::sizingZone));
    }else if(field == HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA){
      addQuantityEditColumn(Heading(QString(HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA)),
                            QString("m^3/min*m^2"),
                            QString("m^3/min*m^2"),
                            QString("ft^3/min*ft^2"),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::heatingMaximumAirFlowperZoneFloorArea, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setHeatingMaximumAirFlowperZoneFloorArea), 
                              &model::ThermalZone::sizingZone));
    }else if(field == HEATINGMAXIMUMAIRFLOWFRACTION){
      addQuantityEditColumn(Heading(QString(HEATINGMAXIMUMAIRFLOWFRACTION)),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::heatingMaximumAirFlowFraction, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setHeatingMaximumAirFlowFraction), 
                              &model::ThermalZone::sizingZone));
    }else if(field == DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE){
      addQuantityEditColumn(Heading(QString(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE)),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::designZoneAirDistributionEffectivenessinCoolingMode, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setDesignZoneAirDistributionEffectivenessinCoolingMode), 
                              &model::ThermalZone::sizingZone));
    }else if(field == DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE){
      addQuantityEditColumn(Heading(QString(DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE)),
                            QString(""),
                            QString(""),
                            QString(""),
                            m_isIP,
                            ProxyAdapter(&model::SizingZone::designZoneAirDistributionEffectivenessinHeatingMode, &model::ThermalZone::sizingZone),
                            ProxyAdapter(static_cast<bool (model::SizingZone::*)(double)>(&model::SizingZone::setDesignZoneAirDistributionEffectivenessinHeatingMode), 
                              &model::ThermalZone::sizingZone));

    }else if(field == COOLINGDESIGNAIRFLOWMETHOD){
      addComboBoxColumn<std::string, model::ThermalZone>(Heading(QString(COOLINGDESIGNAIRFLOWMETHOD)),
                        std::function<std::string (const std::string &)>(static_cast<std::string (*)(const std::string&)>(&openstudio::toString)),
                        std::function<std::vector<std::string> ()>(&model::SizingZone::coolingDesignAirFlowMethodValues),
                        std::function<std::string (model::ThermalZone *)>([](model::ThermalZone *t_z) {
                            try {
                              return t_z->sizingZone().coolingDesignAirFlowMethod();
                            } catch (const std::exception &e) {
                              // If this code is called there is no sizingZone currently set. This means
                              // that the ThermalZone is probably in the process of being destructed. So,
                              // we don't want to create a new sizingZone, we really just want to try to 
                              // continue gracefully, so let's pretend that the current coolingDesignAirFlowMethod
                              // is the 0th one in the list of possible options.
                              //
                              // This might not be the best solution to this problem
                              LOG(Debug, "Exception while obtaining coolingDesignAirflowMethod " << e.what());
                              return model::SizingZone::coolingDesignAirFlowMethodValues().at(0);
                            }
                          }
                          ),
                          std::function<bool(model::ThermalZone *, std::string)>([](model::ThermalZone *t_z, std::string t_val){ auto sz = t_z->sizingZone(); return sz.setCoolingDesignAirFlowMethod(t_val); }),
                          boost::optional<std::function<void(openstudio::model::ThermalZone *)>>(), 
                          boost::optional<std::function<bool(openstudio::model::ThermalZone *)>>(),
                          boost::optional<openstudio::DataSource>()
                          );

    }else if(field == COOLINGTHERMOSTATSCHEDULE){

      std::function<boost::optional<model::Schedule>(model::ThermalZone *)> coolingSchedule(
        [](model::ThermalZone * z) {
          boost::optional<model::Schedule> schedule;
          if (boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = z->thermostatSetpointDualSetpoint()) {
            schedule = thermostat->coolingSetpointTemperatureSchedule();
          }
          return schedule;
        }
      );

      std::function<bool(model::ThermalZone *, const model::Schedule &)> setCoolingSchedule(
        [](model::ThermalZone * z, model::Schedule t_s) {
          if (boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = z->thermostatSetpointDualSetpoint()) {
            return thermostat->setCoolingSetpointTemperatureSchedule(t_s);
          }
          else {
            model::ThermostatSetpointDualSetpoint t(z->model());
            z->setThermostatSetpointDualSetpoint(t);
            return t.setCoolingSetpointTemperatureSchedule(t_s);
          }
        }
      );

      boost::optional<std::function<void(model::ThermalZone *)> > resetCoolingSchedule(
        [](model::ThermalZone * z) {
          if (boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = z->thermostatSetpointDualSetpoint()) {
            thermostat->resetCoolingSetpointTemperatureSchedule();
          }
        }
      );

      addDropZoneColumn(Heading(QString(COOLINGTHERMOSTATSCHEDULE)),
        coolingSchedule,
        setCoolingSchedule,
        resetCoolingSchedule);

    }else if (field == HEATINGTHERMOSTATSCHEDULE){

      std::function<boost::optional<model::Schedule>(model::ThermalZone *)> heatingSchedule(
        [](model::ThermalZone * z) {
          boost::optional<model::Schedule> schedule;
          if (boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = z->thermostatSetpointDualSetpoint()) {
            schedule = thermostat->heatingSetpointTemperatureSchedule();
          }
          return schedule;
        }
      );

      std::function<bool(model::ThermalZone *, const model::Schedule &)> setHeatingSchedule(
        [](model::ThermalZone * z, model::Schedule t_s) {
          if (boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = z->thermostatSetpointDualSetpoint()) {
            return thermostat->setHeatingSetpointTemperatureSchedule(t_s);
          }
          else {
            model::ThermostatSetpointDualSetpoint t(z->model());
            z->setThermostatSetpointDualSetpoint(t);
            return t.setHeatingSetpointTemperatureSchedule(t_s);
          }
        }
      );

      boost::optional<std::function<void(model::ThermalZone *)> > resetHeatingSchedule(
        [](model::ThermalZone * z) {
          if (boost::optional<model::ThermostatSetpointDualSetpoint> thermostat = z->thermostatSetpointDualSetpoint()) {
            thermostat->resetHeatingSetpointTemperatureSchedule();
          }
        }
      );

      addDropZoneColumn(Heading(QString(HEATINGTHERMOSTATSCHEDULE)),
        heatingSchedule,
        setHeatingSchedule,
        resetHeatingSchedule);

    }else if (field == HUMIDIFYINGSETPOINTSCHEDULE){

      std::function<boost::optional<model::Schedule>(model::ThermalZone *)> humidifyingSchedule(
        [](model::ThermalZone * z) {
          boost::optional<model::Schedule> schedule;
          if (boost::optional<model::ZoneControlHumidistat> thermostat = z->zoneControlHumidistat()) {
            schedule = thermostat->humidifyingRelativeHumiditySetpointSchedule();
          }
          return schedule;
        }
      );

      std::function<bool(model::ThermalZone *, const model::Schedule &)> setHumidifyingSchedule(
        [](model::ThermalZone * z, model::Schedule t_s) {
          if (boost::optional<model::ZoneControlHumidistat> thermostat = z->zoneControlHumidistat()) {
            return thermostat->setHumidifyingRelativeHumiditySetpointSchedule(t_s);
          }
          else {
            model::ZoneControlHumidistat t(z->model());
            z->setZoneControlHumidistat(t);
            return t.setHumidifyingRelativeHumiditySetpointSchedule(t_s);
          }
        }
      );

      boost::optional<std::function<void(model::ThermalZone *)> > resetHumidifyingSchedule(
        [](model::ThermalZone * z) {
          if (boost::optional<model::ZoneControlHumidistat> thermostat = z->zoneControlHumidistat()) {
            thermostat->resetHumidifyingRelativeHumiditySetpointSchedule();
            if( ! thermostat->dehumidifyingRelativeHumiditySetpointSchedule() ) {
              thermostat->remove();
            }
          }
        }
      );

      addDropZoneColumn(Heading(QString(HUMIDIFYINGSETPOINTSCHEDULE)),
        humidifyingSchedule,
        setHumidifyingSchedule,
        resetHumidifyingSchedule);

    }else if (field == DEHUMIDIFYINGSETPOINTSCHEDULE){

      std::function<boost::optional<model::Schedule>(model::ThermalZone *)> dehumidifyingSchedule(
        [](model::ThermalZone * z) {
          boost::optional<model::Schedule> schedule;
          if (boost::optional<model::ZoneControlHumidistat> thermostat = z->zoneControlHumidistat()) {
            schedule = thermostat->dehumidifyingRelativeHumiditySetpointSchedule();
          }
          return schedule;
        }
      );

      std::function<bool(model::ThermalZone *, const model::Schedule &)> setDehumidifyingSchedule(
        [](model::ThermalZone * z, model::Schedule t_s) {
          if (boost::optional<model::ZoneControlHumidistat> thermostat = z->zoneControlHumidistat()) {
            return thermostat->setDehumidifyingRelativeHumiditySetpointSchedule(t_s);
          }
          else {
            model::ZoneControlHumidistat t(z->model());
            z->setZoneControlHumidistat(t);
            return t.setDehumidifyingRelativeHumiditySetpointSchedule(t_s);
          }
        }
      );

      boost::optional<std::function<void(model::ThermalZone *)> > resetDehumidifyingSchedule(
        [](model::ThermalZone * z) {
          if (boost::optional<model::ZoneControlHumidistat> thermostat = z->zoneControlHumidistat()) {
            thermostat->resetDehumidifyingRelativeHumiditySetpointSchedule();
            if( ! thermostat->humidifyingRelativeHumiditySetpointSchedule() ) {
              thermostat->remove();
            }
          }
        }
      );

      addDropZoneColumn(Heading(QString(DEHUMIDIFYINGSETPOINTSCHEDULE)),
        dehumidifyingSchedule,
        setDehumidifyingSchedule,
        resetDehumidifyingSchedule);

    } else if (field == ZONEEQUIPMENT) {
      std::function<boost::optional<model::ModelObject>(model::ThermalZone *)>  getter;
      std::function<bool(model::ThermalZone *, const model::ModelObject &)> setter(
      [](model::ThermalZone *t_z, const model::ModelObject &t_mo) {
        try {
          if (t_mo.cast<model::ZoneHVACComponent>().thermalZone()) {
            boost::optional<model::ModelObject> clone_mo = t_mo.cast<model::ZoneHVACComponent>().clone(t_mo.model());
            OS_ASSERT(clone_mo);
            auto success = clone_mo->cast<model::ZoneHVACComponent>().addToThermalZone(*t_z);
            OS_ASSERT(success);
            OS_ASSERT(t_mo.cast<model::ZoneHVACComponent>().thermalZone());
            OS_ASSERT(clone_mo->cast<model::ZoneHVACComponent>().thermalZone());
            return true;
          }
          return t_mo.cast<model::ZoneHVACComponent>().addToThermalZone(*t_z);
        }
        catch (const std::exception &) {
          return false;
        }
      }
      );

      std::function<void(model::ThermalZone *)> reset;
      std::function<std::vector<model::ModelObject>(const model::ThermalZone &)> equipment(
        []( model::ThermalZone t ) {
        // we need to pass in a const &, but the function expects non-const, so let's copy the wrapper
        // object in the param list
        return t.equipmentInHeatingOrder();
      }
      );

      addNameLineEditColumn(Heading(QString(ZONEEQUIPMENT)),
        true,
        false,
        CastNullAdapter<model::ModelObject>(&model::ModelObject::name),
        CastNullAdapter<model::ModelObject>(&model::ModelObject::setName),
        boost::optional<std::function<void(model::ModelObject *)>>(
        std::function<void(model::ModelObject *)>(
        [](model::ModelObject *t_mo)
          {
            t_mo->remove();
          }
        )
        ),
        DataSource(
        equipment,
        false,
        QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::ModelObject, model::ThermalZone>(Heading(ZONEEQUIPMENT),
        getter, setter, reset))
        )
        );

    }else if(field == NAME){
      addNameLineEditColumn(Heading(QString(NAME), false, false),
                            false,
                            false,
                            CastNullAdapter<model::ThermalZone>(&model::ThermalZone::name),
                            CastNullAdapter<model::ThermalZone>(&model::ThermalZone::setName),
                            boost::optional<std::function<void (model::ThermalZone *)>>());

    }else if(field == AIRLOOPNAME){
      // Notes: this only requires a static_cast because `name` comes from IdfObject
      // we are passing in an empty std::function for the separate parameter because there's no way to set it
      addNameLineEditColumn(Heading(QString(AIRLOOPNAME),true,false),
                            false,
                            false,
                            ProxyAdapter(static_cast<boost::optional<std::string> (model::AirLoopHVAC::*)(bool) const>(&model::AirLoopHVAC::name), 
                              &model::ThermalZone::airLoopHVAC, boost::optional<std::string>("None")),
                            std::function<boost::optional<std::string>(model::HVACComponent*, const std::string &)>(),
                            boost::optional<std::function<void (model::HVACComponent *)>>());

    }else if(field == MULTIPLIER){
      addValueEditColumn(Heading(QString(MULTIPLIER)),
                         NullAdapter(&model::ThermalZone::multiplier),
                         NullAdapter(&model::ThermalZone::setMultiplier));

    }else{
      // unhandled
      OS_ASSERT(false);
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
  std::vector<model::ThermalZone> thermalZones = m_model.getModelObjects<model::ThermalZone>();
  m_modelObjects = subsetCastVector<model::ModelObject>(thermalZones);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void ThermalZonesGridController::onComboBoxIndexChanged(int index)
{
}

} // openstudio

