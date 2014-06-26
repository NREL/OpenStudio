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
#define COOLINGTHERMOSTATSCHEDULE "Cooling Thermostat Schedule"
#define HEATINGTHERMOSTATSCHEDULE "Heating Thermostat Schedule"

//COOLING SIZING PARAMETERS
#define ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE "Zone Cooling Design Supply Air Temperature"
#define ZONECOOLINGDESIGNSUPPLYAIRHUMIDITYRATIO "Zone Cooling Design Supply Air Humidity Ratio"
#define ZONECOOLINGSIZINGFACTOR "Zone Cooling Sizing Factor"
#define COOLINGDESIGNAIRFLOWMETHOD "Cooling Design Air Flow Method"
#define COOLINGDESIGNAIRFLOWRATE "Cooling Design Air Flow Rate"
#define DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINCOOLINGMODE "Design Zone Air Distribution Effectiveness in Cooling Mode"

//HEATING SIZING PARAMETERS
#define ZONEHEATINGDESIGNSUPPLYAIRTEMPERATURE "Zone Heating Design Supply Air Temperature"
#define ZONEHEATINGDESIGNSUPPLYAIRHUMIDITYRATIO "Zone Heating Design Supply Air Humidity Ratio"
#define ZONEHEATINGSIZINGFACTOR "Zone Heating Sizing Factor"
#define HEATINGMAXIMUMAIRFLOWPERZONEFLOORAREA "Heating Maximum Air Flow per Zone Floor Area"
#define HEATINGMAXIMUMAIRFLOWFRACTION "Heating Maximum Air Flow Fraction"
#define DESIGNZONEAIRDISTRIBUTIONEFFECTIVENESSINHEATINGMODE "Design Zone Air Distribution Effectiveness in Heating Mode"

//MEASURE TAGS
#define STANDARDSBUILDINGTYPE "Standards Building Type"
#define STANDARDSSPACETYPE "Standards Space Type"

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

  std::vector<model::ThermalZone> refrigerationSystems = model.getModelObjects<model::ThermalZone>(); // NOTE for horizontal system list

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
    fields.push_back(AIRLOOPNAME);
    fields.push_back(ZONEEQUIPMENT);
    fields.push_back(COOLINGTHERMOSTATSCHEDULE);
    fields.push_back(HEATINGTHERMOSTATSCHEDULE);
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

  {
    std::vector<QString> fields;
    fields.push_back(STANDARDSBUILDINGTYPE);
    fields.push_back(STANDARDSSPACETYPE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Measure Tags"),fields);
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
      //addCheckBoxColumn(QString(IDEALAIRLOADS),
      //                  &model::ThermalZone::useIdealAirLoads,
      //                  &model::ThermalZone::setUseIdealAirLoads);
      //addQuantityEditColumn(QString(ZONECOOLINGDESIGNSUPPLYAIRTEMPERATURE),
      //                      QString("C"),
      //                      QString("C"),
      //                      QString("F"),
      //                      m_isIP,
      //                      &model::ThermalZone::ratedAmbientTemperature,
      //                      &model::ThermalZone::setRatedAmbientTemperature);
    //}else if(field == RATEDAMBIENTRELATIVEHUMIDITY){
    //  addValueEditColumn(QString(RATEDAMBIENTRELATIVEHUMIDITY),
    //                     &model::ThermalZone::ratedAmbientRelativeHumidity,
    //                     &model::ThermalZone::setRatedAmbientRelativeHumidity);
    //}else if(field == RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH){
    //  addQuantityEditColumn(QString(RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("Btu/hr*ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::ratedTotalCoolingCapacityperUnitLength,
    //                        &model::ThermalZone::setRatedTotalCoolingCapacityperUnitLength);
    //}else if(field == RATEDLATENTHEATRATIO){
    //  addValueEditColumn(QString(RATEDLATENTHEATRATIO),
    //                     &model::ThermalZone::ratedLatentHeatRatio,
    //                     &model::ThermalZone::setRatedLatentHeatRatio);
    //}else if(field == RATEDRUNTIMEFRACTION){
    //  addValueEditColumn(QString(RATEDRUNTIMEFRACTION),
    //                     &model::ThermalZone::ratedRuntimeFraction,
    //                     &model::ThermalZone::setRatedRuntimeFraction);
    //}else if(field == CASELENGTH){
    //  addQuantityEditColumn(QString(CASELENGTH),
    //                        QString("m"),
    //                        QString("m"),
    //                        QString("ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::caseLength,
    //                        &model::ThermalZone::setCaseLength);
    //}else if(field == CASEOPERATINGTEMPERATURE){
    //  addQuantityEditColumn(QString(CASEOPERATINGTEMPERATURE),
    //                        QString("C"),
    //                        QString("C"),
    //                        QString("F"),
    //                        m_isIP,
    //                        &model::ThermalZone::caseOperatingTemperature,
    //                        &model::ThermalZone::setCaseOperatingTemperature);
    //}else if(field == STANDARDCASEFANPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(STANDARDCASEFANPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::standardCaseFanPowerperUnitLength,
    //                        &model::ThermalZone::setStandardCaseFanPowerperUnitLength);
    //}else if(field == OPERATINGCASEFANPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(OPERATINGCASEFANPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::operatingCaseFanPowerperUnitLength,
    //                        &model::ThermalZone::setOperatingCaseFanPowerperUnitLength);
    //}else if(field == FRACTIONOFLIGHTINGENERGYTOCASE){
    //  addValueEditColumn(QString(FRACTIONOFLIGHTINGENERGYTOCASE),
    //                     &model::ThermalZone::fractionofLightingEnergytoCase,
    //                     &model::ThermalZone::setFractionofLightingEnergytoCase);
    //}else if(field == CASEANTISWEATHEATERPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(CASEANTISWEATHEATERPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::caseAntiSweatHeaterPowerperUnitLength,
    //                        &model::ThermalZone::setCaseAntiSweatHeaterPowerperUnitLength);
    //}else if(field == MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::minimumAntiSweatHeaterPowerperUnitLength,
    //                        &model::ThermalZone::setMinimumAntiSweatHeaterPowerperUnitLength);
    //}else if(field == CASEHEIGHT){
    //  addQuantityEditColumn(QString(CASEHEIGHT),
    //                        QString("m"),
    //                        QString("m"),
    //                        QString("ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::caseHeight,
    //                        &model::ThermalZone::setCaseHeight);
    //}else if(field == FRACTIONOFANTISWEATHEATERENERGYTOCASE){
    //  addValueEditColumn(QString(FRACTIONOFANTISWEATHEATERENERGYTOCASE),
    //                     &model::ThermalZone::fractionofAntiSweatHeaterEnergytoCase,
    //                     &model::ThermalZone::setFractionofAntiSweatHeaterEnergytoCase);
    //}else if(field == CASEDEFROSTPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(CASEDEFROSTPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::caseDefrostPowerperUnitLength,
    //                        &model::ThermalZone::setCaseDefrostPowerperUnitLength);
    //}else if(field == UNDERCASEHVACRETURNAIRFRACTION){
    //  addValueEditColumn(QString(UNDERCASEHVACRETURNAIRFRACTION),
    //                     &model::ThermalZone::underCaseHVACReturnAirFraction,
    //                     &model::ThermalZone::setUnderCaseHVACReturnAirFraction);
    //}else if(field == STANDARDCASELIGHTINGPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(STANDARDCASELIGHTINGPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::standardCaseLightingPowerperUnitLength,
    //                        &model::ThermalZone::setStandardCaseLightingPowerperUnitLength);
    //}else if(field == HUMIDITYATZEROANTISWEATHEATERENERGY){
    //  addValueEditColumn(QString(HUMIDITYATZEROANTISWEATHEATERENERGY),
    //                     &model::ThermalZone::humidityatZeroAntiSweatHeaterEnergy,
    //                     &model::ThermalZone::setHumidityatZeroAntiSweatHeaterEnergy);
    //}else if(field == AVERAGEREFRIGERANTCHARGEINVENTORY){
    //  addQuantityEditColumn(QString(AVERAGEREFRIGERANTCHARGEINVENTORY),
    //                        QString("kg/m"),
    //                        QString("kg/m"),
    //                        QString("lb/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::averageRefrigerantChargeInventory,
    //                        &model::ThermalZone::setAverageRefrigerantChargeInventory);
    //}else if(field == LATENTCASECREDITCURVETYPE){
    //  addComboBoxColumn<std::string,model::ThermalZone>(
    //        QString(LATENTCASECREDITCURVETYPE),
    //        static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    //        &model::ThermalZone::latentCaseCreditCurveTypeValues,
    //        boost::function<std::string (model::ThermalZone*)>(&model::ThermalZone::latentCaseCreditCurveType),
    //        &model::ThermalZone::setLatentCaseCreditCurveType);
    //}else if(field == ANTISWEATHEATERCONTROLTYPE){
    //  addComboBoxColumn<std::string,model::ThermalZone>(
    //        QString(ANTISWEATHEATERCONTROLTYPE),
    //        static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    //        &model::ThermalZone::antiSweatHeaterControlTypeValues,
    //        boost::function<std::string (model::ThermalZone*)>(&model::ThermalZone::antiSweatHeaterControlType),
    //        &model::ThermalZone::setAntiSweatHeaterControlType);
    //}else if(field == CASEDEFROSTTYPE){
    //  addComboBoxColumn<std::string,model::ThermalZone>(
    //        QString(CASEDEFROSTTYPE),
    //        static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    //        &model::ThermalZone::caseDefrostTypeValues,
    //        boost::function<std::string (model::ThermalZone*)>(&model::ThermalZone::caseDefrostType),
    //        &model::ThermalZone::setCaseDefrostType);
    //}else if(field == DEFROSTENERGYCORRECTIONCURVETYPE){
    //  addComboBoxColumn<std::string,model::ThermalZone>(
    //        QString(DEFROSTENERGYCORRECTIONCURVETYPE),
    //        static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    //        &model::ThermalZone::defrostEnergyCorrectionCurveTypeValues,
    //        boost::function<std::string (model::ThermalZone*)>(&model::ThermalZone::defrostEnergyCorrectionCurveType),
    //        &model::ThermalZone::setDefrostEnergyCorrectionCurveType);
    //}else if(field == INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH){
    //  addQuantityEditColumn(QString(INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH),
    //                        QString("W/m"),
    //                        QString("W/m"),
    //                        QString("W/ft"),
    //                        m_isIP,
    //                        &model::ThermalZone::installedCaseLightingPowerperUnitLength,
    //                        &model::ThermalZone::setInstalledCaseLightingPowerperUnitLength);
    //}else if(field == DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE){
    //  addQuantityEditColumn(QString(DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE),
    //                        QString("C"),
    //                        QString("C"),
    //                        QString("F"),
    //                        m_isIP,
    //                        &model::ThermalZone::designEvaporatorTemperatureorBrineInletTemperature,
    //                        &model::ThermalZone::setDesignEvaporatorTemperatureorBrineInletTemperature);
    //}else if(field == CASELIGHTINGSCHEDULE){
    //  addComboBoxColumn<model::Schedule,model::ThermalZone>(
    //      QString(CASELIGHTINGSCHEDULE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::Schedule>,
    //                  boost::bind(&openstudio::model::getCompatibleSchedules,
    //                              m_model,
    //                              "RefrigerationCase",
    //                              "Case Lighting")), // NOTE these strings found in ScheduleTypeRegistry.cpp
    //      &model::ThermalZone::caseLightingSchedule,
    //      &model::ThermalZone::setCaseLightingSchedule,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetCaseLightingSchedule));
    //}else if(field == CASEDEFROSTSCHEDULE){
    //  addComboBoxColumn<model::Schedule,model::ThermalZone>(
    //      QString(CASEDEFROSTSCHEDULE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::Schedule>,
    //                  boost::bind(&openstudio::model::getCompatibleSchedules,
    //                              m_model,
    //                              "RefrigerationCase",
    //                              "Case Defrost")),
    //      &model::ThermalZone::caseDefrostSchedule,
    //      &model::ThermalZone::setCaseDefrostSchedule,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetCaseDefrostSchedule));
    //}else if(field == CASEDEFROSTDRIPDOWNSCHEDULE){
    //  addComboBoxColumn<model::Schedule,model::ThermalZone>(
    //      QString(CASEDEFROSTDRIPDOWNSCHEDULE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::Schedule>,
    //                  boost::bind(&openstudio::model::getCompatibleSchedules,
    //                              m_model,
    //                              "RefrigerationCase",
    //                              "Case Defrost Drip-Down")),
    //      &model::ThermalZone::caseDefrostDripDownSchedule,
    //      &model::ThermalZone::setCaseDefrostDripDownSchedule,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetCaseDefrostDripDownSchedule));
    //}else if(field == REFRIGERATEDCASERESTOCKINGSCHEDULE){
    //  addComboBoxColumn<model::Schedule,model::ThermalZone>(
    //      QString(REFRIGERATEDCASERESTOCKINGSCHEDULE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::Schedule>,
    //                  boost::bind(&openstudio::model::getCompatibleSchedules,
    //                              m_model,
    //                              "RefrigerationCase",
    //                              "Refrigerated Case Restocking")),
    //      &model::ThermalZone::refrigeratedCaseRestockingSchedule,
    //      &model::ThermalZone::setRefrigeratedCaseRestockingSchedule,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetRefrigeratedCaseRestockingSchedule));
    //}else if(field == CASECREDITFRACTIONSCHEDULE){
    //  addComboBoxColumn<model::Schedule,model::ThermalZone>(
    //      QString(CASECREDITFRACTIONSCHEDULE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::Schedule>,
    //                  boost::bind(&openstudio::model::getCompatibleSchedules,
    //                              m_model,
    //                              "RefrigerationCase",
    //                              "Case Credit Fraction")),
    //      &model::ThermalZone::caseCreditFractionSchedule,
    //      &model::ThermalZone::setCaseCreditFractionSchedule,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetCaseCreditFractionSchedule));
    //}else if(field == AVAILABILITYSCHEDULE){
    //    addComboBoxColumn<model::Schedule,model::ThermalZone>(
    //      QString(AVAILABILITYSCHEDULE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::Schedule>,
    //                  boost::bind(&openstudio::model::getCompatibleSchedules,
    //                              m_model,
    //                              "RefrigerationCase",
    //                              "Availability")),
    //      &model::ThermalZone::availabilitySchedule,
    //      &model::ThermalZone::setAvailabilitySchedule,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetAvailabilitySchedule));
    //}else if(field == THERMALZONE){
    //  addComboBoxColumn<model::ThermalZone,model::ThermalZone>(
    //      QString(THERMALZONE),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::ThermalZone>,
    //                  boost::bind(&model::Model::getConcreteModelObjects<model::ThermalZone>,m_model)),
    //      &model::ThermalZone::thermalZone,
    //      &model::ThermalZone::setThermalZone,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::resetThermalZone));
    //}else if(field == RACK){
    //  addComboBoxColumn<model::RefrigerationSystem,model::ThermalZone>(
    //      QString(RACK),
    //      &openstudio::objectName,
    //      boost::bind(&openstudio::sortByObjectName<model::RefrigerationSystem>,
    //                  boost::bind(&model::Model::getConcreteModelObjects<model::RefrigerationSystem>,m_model)),
    //      &model::ThermalZone::system,
    //      &model::ThermalZone::addToSystem,
    //      boost::optional<boost::function<void (model::ThermalZone*)> >(&model::ThermalZone::removeFromSystem));
    //}else if(field == DEFROSTENERGYCORRECTIONCURVE){
    //  //boost::optional<CurveCubic> defrostEnergyCorrectionCurve() const; TODO
    //}else if(field == NAME){
    //  addNameLineEditColumn(QString(NAME),
    //                        &model::ThermalZone::name,
    //                        &model::ThermalZone::setName);


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

