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

#include "RefrigerationGridView.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/RefrigerationCase.hpp"
#include "../model/RefrigerationCase_Impl.hpp"
#include "../model/RefrigerationSystem.hpp"
#include "../model/RefrigerationSystem_Impl.hpp"
#include "../model/RefrigerationWalkIn.hpp"
#include "../model/RefrigerationWalkIn_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/ScheduleTypeRegistry.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include <utilities/idd/Refrigeration_Case_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_WalkIn_FieldEnums.hxx>

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QTimer>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

// Display Case Fields
#define ANTISWEATHEATERCONTROLTYPE "Anti Sweat Heater Control Type"
#define AVAILABILITYSCHEDULE "Availability Schedule"
#define AVERAGEREFRIGERANTCHARGEINVENTORY "Average Refrigerant Charge Inventory"
#define CASEANTISWEATHEATERPOWERPERUNITLENGTH "Case Anti Sweat Heater Power\nper Unit Length"
#define CASECREDITFRACTIONSCHEDULE "Case Credit Fraction Schedule"
#define CASEDEFROSTDRIPDOWNSCHEDULE "Case Defrost Drip Down Schedule"
#define CASEDEFROSTPOWERPERUNITLENGTH "Case Defrost Power\nper Unit Length"
#define CASEDEFROSTSCHEDULE "Case Defrost Schedule"
#define CASEDEFROSTTYPE "Case Defrost Type"
#define CASEHEIGHT "Case Height"
#define CASELENGTH "Case Length"
#define CASELIGHTINGSCHEDULE "Case Lighting Schedule"
#define CASEOPERATINGTEMPERATURE "Case Operating Temperature"
#define DEFROSTENERGYCORRECTIONCURVE "Defrost Energy Correction Curve"
#define DEFROSTENERGYCORRECTIONCURVETYPE "Defrost Energy Correction Curve Type"
#define DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE "Design Evaporator Temperature\nor Brine Inlet Temperature"
#define FRACTIONOFANTISWEATHEATERENERGYTOCASE "Fraction of Anti Sweat\nHeater Energy to Case"
#define FRACTIONOFLIGHTINGENERGYTOCASE "Fraction of Lighting Energy to Case"
#define HUMIDITYATZEROANTISWEATHEATERENERGY "Humidity at Zero Anti Sweat\nHeater Energy"
#define INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH "Installed Case Lighting Power\nper Unit Length"
#define LATENTCASECREDITCURVETYPE "Latent Case Credit Curve Type"
#define MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH "Minimum Anti Sweat Heater Power\nper Unit Length"
#define NAME "Name"
#define OPERATINGCASEFANPOWERPERUNITLENGTH "Operating Case Fan Power\nper Unit Length"
#define RATEDAMBIENTRELATIVEHUMIDITY "Rated Ambient Relative Humidity"
#define RATEDAMBIENTTEMPERATURE "Rated Ambient Temperature"
#define RATEDLATENTHEATRATIO "Rated Latent Heat Ratio"
#define RATEDRUNTIMEFRACTION "Rated Runtime Fraction"
#define RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH "Rated Total Cooling Capacity\nper Unit Length"
#define REFRIGERATEDCASERESTOCKINGSCHEDULE "Refrigerated Case Restocking Schedule"
#define STANDARDCASEFANPOWERPERUNITLENGTH "Standard Case Fan Power\nper Unit Length"
#define STANDARDCASELIGHTINGPOWERPERUNITLENGTH "Standard Case Lighting Power\nper Unit Length"
#define THERMALZONE "Thermal Zone"
#define RACK "Rack"
#define UNDERCASEHVACRETURNAIRFRACTION "Under Case HVAC Return Air Fraction"

// Walk In Fields
#define AVAILABILITYSCHEDULE "Availability Schedule"
#define AVERAGEREFRIGERANTCHARGEINVENTORY "Average Refrigerant Charge Inventory"
#define DEFROSTCONTROLTYPE "Defrost Control Type"
#define DEFROSTDRIPDOWNSCHEDULE "Defrost Drip Down Schedule"
#define DEFROSTPOWER "Defrost Power"
#define DEFROSTSCHEDULE "Defrost Schedule"
#define DEFROSTTYPE "Defrost Type"
#define HEATINGPOWERSCHEDULE "Heating Power Schedule"
#define INSULATEDFLOORSURFACEAREA "Insulated Floor Surface Area"
#define INSULATEDFLOORUVALUE "Insulated Floor U Value"
#define LIGHTINGSCHEDULE "Lighting Schedule"
#define NAME "Name"
#define OPERATINGTEMPERATURE "Operating Temperature"
#define RATEDCIRCULATIONFANPOWER "Rated Circulation Fan Power"
#define RATEDCOILCOOLINGCAPACITY "Rated Coil Cooling Capacity"
#define RATEDCOOLINGCOILFANPOWER "Rated Cooling Coil Fan Power"
#define RATEDCOOLINGSOURCETEMPERATURE "Rated Cooling Source Temperature"
#define RATEDTOTALHEATINGPOWER "Rated Total Heating Power"
#define RATEDTOTALLIGHTINGPOWER "Rated Total Lighting Power"
#define RESTOCKINGSCHEDULE "Restocking Schedule"
#define TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE "Temperature Termination\nDefrost Fraction to Ice"
#define ZONEBOUNDARIES "Zone Boundaries"
#define ZONEBOUNDARYTHERMALZONE "Thermal Zone"
#define ZONEBOUNDARYTOTALINSULATEDSURFACEAREAFACINGZONE "Total Insulated Surface\nArea Facing Zone"
#define ZONEBOUNDARYAREAOFGLASSREACHINDOORSFACINGZONE "Area of Glass Reach In\nDoors Facing Zone"
#define ZONEBOUNDARYHEIGHTOFGLASSREACHINDOORSFACINGZONE "Height of Glass Reach In\nDoors Facing Zone"
#define ZONEBOUNDARYAREAOFSTOCKINGDOORSFACINGZONE "Area of Stocking Doors Facing Zone"
#define ZONEBOUNDARYHEIGHTOFSTOCKINGDOORSFACINGZONE "Height of Stocking Doors Facing Zone"
#define ZONEBOUNDARYINSULATEDSURFACEUVALUEFACINGZONE "Insulated Surface U\nValue Facing Zone"
#define ZONEBOUNDARYGLASSREACHINDOORUVALUEFACINGZONE "Glass Reach In Door U\nValue Facing Zone"
#define ZONEBOUNDARYSTOCKINGDOORUVALUEFACINGZONE "Stocking Door U\nValue Facing Zone"
#define ZONEBOUNDARYSTOCKINGDOOROPENINGSCHEDULEFACINGZONE "Stocking Door Opening\nSchedule Facing Zone"

namespace openstudio {

struct ModelObjectNameSorter{
  // sort by name
  bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
    return (lhs.name() < rhs.name());
  }
};

RefrigerationGridView::RefrigerationGridView(bool isIP, const model::Model & model, QWidget * parent)
  : QWidget(parent),
  m_isIP(isIP)
{
  QVBoxLayout * layout = 0;

  layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);

  QVBoxLayout * scrollLayout = new QVBoxLayout();
  scrollLayout->setSpacing(0);
  scrollLayout->setContentsMargins(0,0,0,0);

  QWidget * scrollWidget = new QWidget();
  scrollWidget->setObjectName("ScrollWidget");
  scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
  scrollWidget->setLayout(scrollLayout);

  QScrollArea * scrollArea = new QScrollArea();
  scrollArea->setContentsMargins(0,0,0,0);
  scrollArea->setFrameStyle(QFrame::NoFrame);
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);
  scrollArea->setBackgroundRole(QPalette::NoRole);
  layout->addWidget(scrollArea);

  model::Schedule schedule = model.alwaysOnDiscreteSchedule();

  std::vector<model::RefrigerationCase> refrigerationCases = model.getModelObjects<model::RefrigerationCase>();
  std::vector<model::ModelObject> caseModelObjects = subsetCastVector<model::ModelObject>(refrigerationCases);

  RefrigerationCaseGridController * refrigerationCaseGridController  = new RefrigerationCaseGridController(m_isIP, "Display Cases", model, caseModelObjects);
  OSGridView * caseGridView = new OSGridView(refrigerationCaseGridController, "Display Cases", "Drop\nCase", parent);
  scrollLayout->addWidget(caseGridView,0,Qt::AlignTop);

  std::vector<model::RefrigerationWalkIn> refrigerationWalkIns = model.getModelObjects<model::RefrigerationWalkIn>();
  std::vector<model::ModelObject> walkInModelObjects = subsetCastVector<model::ModelObject>(refrigerationWalkIns);

  RefrigerationWalkInGridController * refrigerationWalkInGridController  = new RefrigerationWalkInGridController(m_isIP, "Walk Ins", model, walkInModelObjects);
  OSGridView * walkInView = new OSGridView(refrigerationWalkInGridController, "Walk Ins", "Drop\nWalk In", parent);
  scrollLayout->addWidget(walkInView,0,Qt::AlignTop);

  scrollLayout->addStretch(1);

  bool isConnected = false;

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        refrigerationCaseGridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        refrigerationWalkInGridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);
  
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        refrigerationCaseGridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        refrigerationWalkInGridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  std::vector<model::RefrigerationSystem> refrigerationSystems = model.getModelObjects<model::RefrigerationSystem>(); // NOTE for horizontal system list

}

RefrigerationCaseGridController::RefrigerationCaseGridController(bool isIP,
  const QString & headerText,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, model, modelObjects)
{
  setCategoriesAndFields();
}

void RefrigerationCaseGridController::setCategoriesAndFields()
{

  {
    std::vector<QString> fields;
    fields.push_back(RACK);
    fields.push_back(THERMALZONE);
    fields.push_back(CASELENGTH);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(UNDERCASEHVACRETURNAIRFRACTION);
    fields.push_back(CASEOPERATINGTEMPERATURE);
    fields.push_back(DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE);
    fields.push_back(RATEDRUNTIMEFRACTION);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Operation"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH);
    fields.push_back(CASECREDITFRACTIONSCHEDULE);
    fields.push_back(RATEDLATENTHEATRATIO);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Cooling\nCapacity"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(STANDARDCASEFANPOWERPERUNITLENGTH);
    fields.push_back(OPERATINGCASEFANPOWERPERUNITLENGTH);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Fan"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(STANDARDCASELIGHTINGPOWERPERUNITLENGTH);
    fields.push_back(INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH);
    fields.push_back(FRACTIONOFLIGHTINGENERGYTOCASE);
    fields.push_back(CASELIGHTINGSCHEDULE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Lighting"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(CASEANTISWEATHEATERPOWERPERUNITLENGTH);
    fields.push_back(ANTISWEATHEATERCONTROLTYPE);
    fields.push_back(MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH);
    fields.push_back(HUMIDITYATZEROANTISWEATHEATERENERGY);
    fields.push_back(FRACTIONOFANTISWEATHEATERENERGYTOCASE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Case\nAnti-Sweat\nHeaters"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(CASEDEFROSTPOWERPERUNITLENGTH);
    fields.push_back(REFRIGERATEDCASERESTOCKINGSCHEDULE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Defrost\nAnd\nRestocking"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}
void RefrigerationCaseGridController::addColumns(std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), NAME);

  m_baseConcepts.clear();

  for (const QString& field : fields) {
    if(field == RATEDAMBIENTTEMPERATURE){
      addQuantityEditColumn(QString(RATEDAMBIENTTEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            &model::RefrigerationCase::ratedAmbientTemperature,
                            &model::RefrigerationCase::setRatedAmbientTemperature);
    }else if(field == RATEDAMBIENTRELATIVEHUMIDITY){
      addValueEditColumn(QString(RATEDAMBIENTRELATIVEHUMIDITY),
                         &model::RefrigerationCase::ratedAmbientRelativeHumidity,
                         &model::RefrigerationCase::setRatedAmbientRelativeHumidity);
    }else if(field == RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH){
      addQuantityEditColumn(QString(RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("Btu/hr*ft"),
                            m_isIP,
                            &model::RefrigerationCase::ratedTotalCoolingCapacityperUnitLength,
                            &model::RefrigerationCase::setRatedTotalCoolingCapacityperUnitLength);
    }else if(field == RATEDLATENTHEATRATIO){
      addValueEditColumn(QString(RATEDLATENTHEATRATIO),
                         &model::RefrigerationCase::ratedLatentHeatRatio,
                         &model::RefrigerationCase::setRatedLatentHeatRatio);
    }else if(field == RATEDRUNTIMEFRACTION){
      addValueEditColumn(QString(RATEDRUNTIMEFRACTION),
                         &model::RefrigerationCase::ratedRuntimeFraction,
                         &model::RefrigerationCase::setRatedRuntimeFraction);
    }else if(field == CASELENGTH){
      addQuantityEditColumn(QString(CASELENGTH),
                            QString("m"),
                            QString("m"),
                            QString("ft"),
                            m_isIP,
                            &model::RefrigerationCase::caseLength,
                            &model::RefrigerationCase::setCaseLength);
    }else if(field == CASEOPERATINGTEMPERATURE){
      addQuantityEditColumn(QString(CASEOPERATINGTEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            &model::RefrigerationCase::caseOperatingTemperature,
                            &model::RefrigerationCase::setCaseOperatingTemperature);
    }else if(field == STANDARDCASEFANPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(STANDARDCASEFANPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::standardCaseFanPowerperUnitLength,
                            &model::RefrigerationCase::setStandardCaseFanPowerperUnitLength);
    }else if(field == OPERATINGCASEFANPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(OPERATINGCASEFANPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::operatingCaseFanPowerperUnitLength,
                            &model::RefrigerationCase::setOperatingCaseFanPowerperUnitLength);
    }else if(field == FRACTIONOFLIGHTINGENERGYTOCASE){
      addValueEditColumn(QString(FRACTIONOFLIGHTINGENERGYTOCASE),
                         &model::RefrigerationCase::fractionofLightingEnergytoCase,
                         &model::RefrigerationCase::setFractionofLightingEnergytoCase);
    }else if(field == CASEANTISWEATHEATERPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(CASEANTISWEATHEATERPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::caseAntiSweatHeaterPowerperUnitLength,
                            &model::RefrigerationCase::setCaseAntiSweatHeaterPowerperUnitLength);
    }else if(field == MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::minimumAntiSweatHeaterPowerperUnitLength,
                            &model::RefrigerationCase::setMinimumAntiSweatHeaterPowerperUnitLength);
    }else if(field == CASEHEIGHT){
      addQuantityEditColumn(QString(CASEHEIGHT),
                            QString("m"),
                            QString("m"),
                            QString("ft"),
                            m_isIP,
                            &model::RefrigerationCase::caseHeight,
                            &model::RefrigerationCase::setCaseHeight);
    }else if(field == FRACTIONOFANTISWEATHEATERENERGYTOCASE){
      addValueEditColumn(QString(FRACTIONOFANTISWEATHEATERENERGYTOCASE),
                         &model::RefrigerationCase::fractionofAntiSweatHeaterEnergytoCase,
                         &model::RefrigerationCase::setFractionofAntiSweatHeaterEnergytoCase);
    }else if(field == CASEDEFROSTPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(CASEDEFROSTPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::caseDefrostPowerperUnitLength,
                            &model::RefrigerationCase::setCaseDefrostPowerperUnitLength);
    }else if(field == UNDERCASEHVACRETURNAIRFRACTION){
      addValueEditColumn(QString(UNDERCASEHVACRETURNAIRFRACTION),
                         &model::RefrigerationCase::underCaseHVACReturnAirFraction,
                         &model::RefrigerationCase::setUnderCaseHVACReturnAirFraction);
    }else if(field == STANDARDCASELIGHTINGPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(STANDARDCASELIGHTINGPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::standardCaseLightingPowerperUnitLength,
                            &model::RefrigerationCase::setStandardCaseLightingPowerperUnitLength);
    }else if(field == HUMIDITYATZEROANTISWEATHEATERENERGY){
      addValueEditColumn(QString(HUMIDITYATZEROANTISWEATHEATERENERGY),
                         &model::RefrigerationCase::humidityatZeroAntiSweatHeaterEnergy,
                         &model::RefrigerationCase::setHumidityatZeroAntiSweatHeaterEnergy);
    }else if(field == AVERAGEREFRIGERANTCHARGEINVENTORY){
      addQuantityEditColumn(QString(AVERAGEREFRIGERANTCHARGEINVENTORY),
                            QString("kg/m"),
                            QString("kg/m"),
                            QString("lb/ft"),
                            m_isIP,
                            &model::RefrigerationCase::averageRefrigerantChargeInventory,
                            &model::RefrigerationCase::setAverageRefrigerantChargeInventory);
    }else if(field == LATENTCASECREDITCURVETYPE){
      addComboBoxColumn<std::string,model::RefrigerationCase>(
            QString(LATENTCASECREDITCURVETYPE),
            static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
            &model::RefrigerationCase::latentCaseCreditCurveTypeValues,
            boost::function<std::string (model::RefrigerationCase*)>(&model::RefrigerationCase::latentCaseCreditCurveType),
            &model::RefrigerationCase::setLatentCaseCreditCurveType);
    }else if(field == ANTISWEATHEATERCONTROLTYPE){
      addComboBoxColumn<std::string,model::RefrigerationCase>(
            QString(ANTISWEATHEATERCONTROLTYPE),
            static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
            &model::RefrigerationCase::antiSweatHeaterControlTypeValues,
            boost::function<std::string (model::RefrigerationCase*)>(&model::RefrigerationCase::antiSweatHeaterControlType),
            &model::RefrigerationCase::setAntiSweatHeaterControlType);
    }else if(field == CASEDEFROSTTYPE){
      addComboBoxColumn<std::string,model::RefrigerationCase>(
            QString(CASEDEFROSTTYPE),
            static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
            &model::RefrigerationCase::caseDefrostTypeValues,
            boost::function<std::string (model::RefrigerationCase*)>(&model::RefrigerationCase::caseDefrostType),
            &model::RefrigerationCase::setCaseDefrostType);
    }else if(field == DEFROSTENERGYCORRECTIONCURVETYPE){
      addComboBoxColumn<std::string,model::RefrigerationCase>(
            QString(DEFROSTENERGYCORRECTIONCURVETYPE),
            static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
            &model::RefrigerationCase::defrostEnergyCorrectionCurveTypeValues,
            boost::function<std::string (model::RefrigerationCase*)>(&model::RefrigerationCase::defrostEnergyCorrectionCurveType),
            &model::RefrigerationCase::setDefrostEnergyCorrectionCurveType);
    }else if(field == INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH){
      addQuantityEditColumn(QString(INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH),
                            QString("W/m"),
                            QString("W/m"),
                            QString("W/ft"),
                            m_isIP,
                            &model::RefrigerationCase::installedCaseLightingPowerperUnitLength,
                            &model::RefrigerationCase::setInstalledCaseLightingPowerperUnitLength);
    }else if(field == DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE){
      addQuantityEditColumn(QString(DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            &model::RefrigerationCase::designEvaporatorTemperatureorBrineInletTemperature,
                            &model::RefrigerationCase::setDesignEvaporatorTemperatureorBrineInletTemperature);
    }else if(field == CASELIGHTINGSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationCase>(
          QString(CASELIGHTINGSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationCase",
                                  "Case Lighting")), // NOTE these strings found in ScheduleTypeRegistry.cpp
          &model::RefrigerationCase::caseLightingSchedule,
          &model::RefrigerationCase::setCaseLightingSchedule,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetCaseLightingSchedule));
    }else if(field == CASEDEFROSTSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationCase>(
          QString(CASEDEFROSTSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationCase",
                                  "Case Defrost")),
          &model::RefrigerationCase::caseDefrostSchedule,
          &model::RefrigerationCase::setCaseDefrostSchedule,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetCaseDefrostSchedule));
    }else if(field == CASEDEFROSTDRIPDOWNSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationCase>(
          QString(CASEDEFROSTDRIPDOWNSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationCase",
                                  "Case Defrost Drip-Down")),
          &model::RefrigerationCase::caseDefrostDripDownSchedule,
          &model::RefrigerationCase::setCaseDefrostDripDownSchedule,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetCaseDefrostDripDownSchedule));
    }else if(field == REFRIGERATEDCASERESTOCKINGSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationCase>(
          QString(REFRIGERATEDCASERESTOCKINGSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationCase",
                                  "Refrigerated Case Restocking")),
          &model::RefrigerationCase::refrigeratedCaseRestockingSchedule,
          &model::RefrigerationCase::setRefrigeratedCaseRestockingSchedule,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetRefrigeratedCaseRestockingSchedule));
    }else if(field == CASECREDITFRACTIONSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationCase>(
          QString(CASECREDITFRACTIONSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationCase",
                                  "Case Credit Fraction")),
          &model::RefrigerationCase::caseCreditFractionSchedule,
          &model::RefrigerationCase::setCaseCreditFractionSchedule,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetCaseCreditFractionSchedule));
    }else if(field == AVAILABILITYSCHEDULE){
        addComboBoxColumn<model::Schedule,model::RefrigerationCase>(
          QString(AVAILABILITYSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationCase",
                                  "Availability")),
          &model::RefrigerationCase::availabilitySchedule,
          &model::RefrigerationCase::setAvailabilitySchedule,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetAvailabilitySchedule));
    }else if(field == THERMALZONE){
      addComboBoxColumn<model::ThermalZone,model::RefrigerationCase>(
          QString(THERMALZONE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::ThermalZone>,
                      std::bind(&model::Model::getConcreteModelObjects<model::ThermalZone>,m_model)),
          &model::RefrigerationCase::thermalZone,
          &model::RefrigerationCase::setThermalZone,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::resetThermalZone));
    }else if(field == RACK){
      addComboBoxColumn<model::RefrigerationSystem,model::RefrigerationCase>(
          QString(RACK),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::RefrigerationSystem>,
                      std::bind(&model::Model::getConcreteModelObjects<model::RefrigerationSystem>,m_model)),
          &model::RefrigerationCase::system,
          &model::RefrigerationCase::addToSystem,
          boost::optional<boost::function<void (model::RefrigerationCase*)> >(&model::RefrigerationCase::removeFromSystem));
    }else if(field == DEFROSTENERGYCORRECTIONCURVE){
      //boost::optional<CurveCubic> defrostEnergyCorrectionCurve() const; TODO
    }else if(field == NAME){
      addNameLineEditColumn(QString(NAME),
                            &model::RefrigerationCase::name,
                            &model::RefrigerationCase::setName);
    }else{
      // unhandled
      OS_ASSERT(false);
    }
  }
}

QString RefrigerationCaseGridController::getColor(const model:: ModelObject & modelObject)
{
  QColor defaultColor(Qt::lightGray);
  QString color(defaultColor.name());

  // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
  // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

  //std::vector<model::RefrigerationSystem> refrigerationSystems = m_model.getModelObjects<model::RefrigerationSystem>();

  //boost::optional<model::RefrigerationCase> refrigerationCase = modelObject.optionalCast<model::RefrigerationCase>();
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

void RefrigerationCaseGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  // Don't show the name column check box
  // From above in addColumns, we know that NAME is the first entry
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(0));
  OS_ASSERT(horizontalHeaderWidget);
  horizontalHeaderWidget->m_checkBox->hide();

  OSGridController::checkSelectedFields();
}

void RefrigerationCaseGridController::onItemDropped(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::RefrigerationCase>()){
      modelObject->clone(m_model);
      emit modelReset();
    }
  }
}

void RefrigerationCaseGridController::refreshModelObjects()
{
  std::vector<model::RefrigerationCase> refrigerationCases = m_model.getModelObjects<model::RefrigerationCase>();
  m_modelObjects = subsetCastVector<model::ModelObject>(refrigerationCases);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void RefrigerationCaseGridController::onComboBoxIndexChanged(int index)
{
  // Note: find the correct system color on RACK change,
  // but currently unable to know which row changed.
  for(unsigned index = 0; index < m_horizontalHeader.size(); index++){
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(index));
    if(horizontalHeaderWidget->m_label->text() == RACK){
      // NOTE required due to a race condition
      // Code below commented out due to a very infrequent crash in the bowels of Qt appears to be exasperated by this refresh.
      // A new refresh scheme with finer granularity may eliminate the problem, and restore rack colors.
      //QTimer::singleShot(0, this, SLOT(reset())); TODO
      break;
    }
  }
}

RefrigerationWalkInGridController::RefrigerationWalkInGridController(bool isIP,
  const QString & headerText,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, model, modelObjects)
{
  setCategoriesAndFields();
}

void RefrigerationWalkInGridController::setCategoriesAndFields()
{

  {
    std::vector<QString> fields;
    fields.push_back(RACK);
    fields.push_back(ZONEBOUNDARYTHERMALZONE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(ZONEBOUNDARYTOTALINSULATEDSURFACEAREAFACINGZONE);
    fields.push_back(ZONEBOUNDARYAREAOFGLASSREACHINDOORSFACINGZONE);
    fields.push_back(ZONEBOUNDARYHEIGHTOFGLASSREACHINDOORSFACINGZONE);
    fields.push_back(ZONEBOUNDARYAREAOFSTOCKINGDOORSFACINGZONE);
    fields.push_back(ZONEBOUNDARYHEIGHTOFSTOCKINGDOORSFACINGZONE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Dimensions"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(INSULATEDFLOORUVALUE);
    fields.push_back(ZONEBOUNDARYINSULATEDSURFACEUVALUEFACINGZONE);
    fields.push_back(ZONEBOUNDARYGLASSREACHINDOORUVALUEFACINGZONE);
    fields.push_back(ZONEBOUNDARYSTOCKINGDOORUVALUEFACINGZONE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Construction"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(AVAILABILITYSCHEDULE);
    fields.push_back(OPERATINGTEMPERATURE);
    fields.push_back(RATEDCOOLINGSOURCETEMPERATURE);
    fields.push_back(RATEDCOILCOOLINGCAPACITY);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Operation"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDCOOLINGCOILFANPOWER);
    fields.push_back(RATEDCIRCULATIONFANPOWER);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Fans"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDTOTALLIGHTINGPOWER);
    fields.push_back(LIGHTINGSCHEDULE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Lighting"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDTOTALHEATINGPOWER);
    fields.push_back(HEATINGPOWERSCHEDULE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Heating"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(DEFROSTTYPE);
    fields.push_back(DEFROSTCONTROLTYPE);
    fields.push_back(DEFROSTPOWER);
    fields.push_back(TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Defrost"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RESTOCKINGSCHEDULE);
    fields.push_back(ZONEBOUNDARYSTOCKINGDOOROPENINGSCHEDULEFACINGZONE);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Restocking"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}

void RefrigerationWalkInGridController::addColumns(std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), NAME);

  m_baseConcepts.clear();

  for (const QString& field : fields) {
    if(field == DEFROSTTYPE){
      addComboBoxColumn<std::string,model::RefrigerationWalkIn>(
            QString(DEFROSTTYPE),
            static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
            &model::RefrigerationWalkIn::defrostTypeValues,
            boost::function<std::string (model::RefrigerationWalkIn*)>(&model::RefrigerationWalkIn::defrostType),
            &model::RefrigerationWalkIn::setDefrostType);
    }else if(field == DEFROSTCONTROLTYPE){
      addComboBoxColumn<std::string,model::RefrigerationWalkIn>(
            QString(DEFROSTCONTROLTYPE),
            static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
            &model::RefrigerationWalkIn::defrostControlTypeValues,
            boost::function<std::string (model::RefrigerationWalkIn*)>(&model::RefrigerationWalkIn::defrostControlType),
            &model::RefrigerationWalkIn::setDefrostControlType);
    }else if(field == OPERATINGTEMPERATURE){
      addQuantityEditColumn(QString(OPERATINGTEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            &model::RefrigerationWalkIn::operatingTemperature,
                            &model::RefrigerationWalkIn::setOperatingTemperature);
    }else if(field == RATEDCOOLINGSOURCETEMPERATURE){
      addQuantityEditColumn(QString(RATEDCOOLINGSOURCETEMPERATURE),
                            QString("C"),
                            QString("C"),
                            QString("F"),
                            m_isIP,
                            &model::RefrigerationWalkIn::ratedCoolingSourceTemperature,
                            &model::RefrigerationWalkIn::setRatedCoolingSourceTemperature);
    }else if(field == RATEDCOOLINGCOILFANPOWER){
      addQuantityEditColumn(QString(RATEDCOOLINGCOILFANPOWER),
                            QString("W"),
                            QString("W"),
                            QString("W"),
                            m_isIP,
                            &model::RefrigerationWalkIn::ratedCoolingCoilFanPower,
                            &model::RefrigerationWalkIn::setRatedCoolingCoilFanPower);
    }else if(field == RATEDCIRCULATIONFANPOWER){
      addQuantityEditColumn(QString(RATEDCIRCULATIONFANPOWER),
                            QString("W"),
                            QString("W"),
                            QString("W"),
                            m_isIP,
                            &model::RefrigerationWalkIn::ratedCirculationFanPower,
                            &model::RefrigerationWalkIn::setRatedCirculationFanPower);
    }else if(field == INSULATEDFLOORSURFACEAREA){
      addQuantityEditColumn(QString(INSULATEDFLOORSURFACEAREA),
                            QString("m2"),
                            QString("m2"),
                            QString("ft^2"),
                            m_isIP,
                            &model::RefrigerationWalkIn::insulatedFloorSurfaceArea,
                            &model::RefrigerationWalkIn::setInsulatedFloorSurfaceArea);
    }else if(field == INSULATEDFLOORUVALUE){
      addQuantityEditColumn(QString(INSULATEDFLOORUVALUE),
                            QString("W/m^2*K"),
                            QString("W/m^2*K"),
                            QString("Btu/hr*ft^2*F"),
                            m_isIP,
                            &model::RefrigerationWalkIn::insulatedFloorUValue,
                            &model::RefrigerationWalkIn::setInsulatedFloorUValue);
    }else if(field == RATEDCOILCOOLINGCAPACITY){
      addQuantityEditColumn(QString(RATEDCOILCOOLINGCAPACITY),
                            QString("W"),
                            QString("W"),
                            QString("Btu/hr"),
                            m_isIP,
                            &model::RefrigerationWalkIn::ratedCoilCoolingCapacity,
                            &model::RefrigerationWalkIn::setRatedCoilCoolingCapacity);
    }else if(field == RATEDTOTALHEATINGPOWER){
      addQuantityEditColumn(QString(RATEDTOTALHEATINGPOWER),
                            QString("W"),
                            QString("W"),
                            QString("W"),
                            m_isIP,
                            &model::RefrigerationWalkIn::ratedTotalHeatingPower,
                            &model::RefrigerationWalkIn::setRatedTotalHeatingPower);
    }else if(field == RATEDTOTALLIGHTINGPOWER){
      addQuantityEditColumn(QString(RATEDTOTALLIGHTINGPOWER),
                            QString("W"),
                            QString("W"),
                            QString("W"),
                            m_isIP,
                            &model::RefrigerationWalkIn::ratedTotalLightingPower,
                            &model::RefrigerationWalkIn::setRatedTotalLightingPower);
    }else if(field == AVERAGEREFRIGERANTCHARGEINVENTORY){
      addQuantityEditColumn(QString(AVERAGEREFRIGERANTCHARGEINVENTORY),
                            QString("kg"),
                            QString("kg"),
                            QString("lb"),
                            m_isIP,
                            &model::RefrigerationWalkIn::averageRefrigerantChargeInventory,
                            &model::RefrigerationWalkIn::setAverageRefrigerantChargeInventory);
    }else if(field == DEFROSTSCHEDULE){
      //addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
      //    QString(DEFROSTSCHEDULE),
      //    &openstudio::objectName,
      //    std::bind(&openstudio::sortByObjectName<model::Schedule>,
      //                std::bind(&openstudio::model::getCompatibleSchedules,
      //                            m_model,
      //                            "RefrigerationWalkIn",
      //                            "Defrost")),
      //    &model::RefrigerationWalkIn::defrostSchedule,
      //    &model::RefrigerationWalkIn::setDefrostSchedule,
      //    boost::none);
    }else if(field == DEFROSTPOWER){
      addQuantityEditColumn(QString(DEFROSTPOWER),
                            QString("W"),
                            QString("W"),
                            QString("W"),
                            m_isIP,
                            &model::RefrigerationWalkIn::defrostPower,
                            &model::RefrigerationWalkIn::setDefrostPower);
    }else if(field == TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE){
      addValueEditColumn(QString(TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE),
                         &model::RefrigerationWalkIn::temperatureTerminationDefrostFractiontoIce,
                         &model::RefrigerationWalkIn::setTemperatureTerminationDefrostFractiontoIce);
    }else if(field == AVAILABILITYSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
          QString(AVAILABILITYSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationWalkIn",
                                  "Availability")),
          &model::RefrigerationWalkIn::availabilitySchedule,
          &model::RefrigerationWalkIn::setAvailabilitySchedule,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetAvailabilitySchedule));
    }else if(field == HEATINGPOWERSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
          QString(HEATINGPOWERSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationWalkIn",
                                  "Heating Power")),
          &model::RefrigerationWalkIn::heatingPowerSchedule,
          &model::RefrigerationWalkIn::setHeatingPowerSchedule,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetHeatingPowerSchedule));
    }else if(field == LIGHTINGSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
          QString(LIGHTINGSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationWalkIn",
                                  "Lighting")),
          &model::RefrigerationWalkIn::lightingSchedule,
          &model::RefrigerationWalkIn::setLightingSchedule,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetLightingSchedule));
    }else if(field == DEFROSTDRIPDOWNSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
          QString(DEFROSTDRIPDOWNSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationWalkIn",
                                  "Defrost Drip-Down")),
          &model::RefrigerationWalkIn::defrostDripDownSchedule,
          &model::RefrigerationWalkIn::setDefrostDripDownSchedule,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetDefrostDripDownSchedule));
    }else if(field == RESTOCKINGSCHEDULE){
      addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
          QString(RESTOCKINGSCHEDULE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationWalkIn",
                                  "Restocking")),
          &model::RefrigerationWalkIn::restockingSchedule,
          &model::RefrigerationWalkIn::setRestockingSchedule,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetRestockingSchedule));
    }else if(field == ZONEBOUNDARIES){
      //std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries() const; TODO
    }else if(field == NAME){
      addNameLineEditColumn(QString(NAME),
                              &model::RefrigerationWalkIn::name,
                              &model::RefrigerationWalkIn::setName);
    }else if(field == RACK){
      addComboBoxColumn<model::RefrigerationSystem,model::RefrigerationWalkIn>(
          QString(RACK),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::RefrigerationSystem>,
                      std::bind(&model::Model::getConcreteModelObjects<model::RefrigerationSystem>,m_model)),
          &model::RefrigerationWalkIn::system,
          &model::RefrigerationWalkIn::addToSystem,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::removeFromSystem));
    }else if(field == ZONEBOUNDARYTHERMALZONE){
      addComboBoxColumn<model::ThermalZone,model::RefrigerationWalkIn>(
          QString(ZONEBOUNDARYTHERMALZONE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::ThermalZone>,
                      std::bind(&model::Model::getConcreteModelObjects<model::ThermalZone>,m_model)),
          &model::RefrigerationWalkIn::zoneBoundaryThermalZone,
          &model::RefrigerationWalkIn::setZoneBoundaryThermalZone,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetZoneBoundaryThermalZone));
    }else if(field == ZONEBOUNDARYTOTALINSULATEDSURFACEAREAFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYTOTALINSULATEDSURFACEAREAFACINGZONE),
                            QString("m^2"),
                            QString("m^2"),
                            QString("ft^2"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryTotalInsulatedSurfaceAreaFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryTotalInsulatedSurfaceAreaFacingZone);
    }else if(field == ZONEBOUNDARYAREAOFGLASSREACHINDOORSFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYAREAOFGLASSREACHINDOORSFACINGZONE),
                            QString("m^2"),
                            QString("m^2"),
                            QString("ft^2"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryAreaofGlassReachInDoorsFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryAreaofGlassReachInDoorsFacingZone);
    }else if(field == ZONEBOUNDARYHEIGHTOFGLASSREACHINDOORSFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYHEIGHTOFGLASSREACHINDOORSFACINGZONE),
                            QString("m"),
                            QString("m"),
                            QString("ft"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryHeightofGlassReachInDoorsFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryHeightofGlassReachInDoorsFacingZone);
    }else if(field == ZONEBOUNDARYAREAOFSTOCKINGDOORSFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYAREAOFSTOCKINGDOORSFACINGZONE),
                            QString("m^2"),
                            QString("m^2"),
                            QString("ft^2"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryAreaofStockingDoorsFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryAreaofStockingDoorsFacingZone);
    }else if(field == ZONEBOUNDARYHEIGHTOFSTOCKINGDOORSFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYHEIGHTOFSTOCKINGDOORSFACINGZONE),
                            QString("m"),
                            QString("m"),
                            QString("ft"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryHeightofStockingDoorsFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryHeightofStockingDoorsFacingZone);
    }else if(field == ZONEBOUNDARYINSULATEDSURFACEUVALUEFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYINSULATEDSURFACEUVALUEFACINGZONE),
                            QString("W/m^2*K"),
                            QString("W/m^2*K"),
                            QString("Btu/hr*ft^2*F"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryInsulatedSurfaceUValueFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryInsulatedSurfaceUValueFacingZone);
    }else if(field == ZONEBOUNDARYGLASSREACHINDOORUVALUEFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYGLASSREACHINDOORUVALUEFACINGZONE),
                            QString("W/m^2*K"),
                            QString("W/m^2*K"),
                            QString("Btu/hr*ft^2*F"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryGlassReachInDoorUValueFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryGlassReachInDoorUValueFacingZone);
    }else if(field == ZONEBOUNDARYSTOCKINGDOORUVALUEFACINGZONE){
      addQuantityEditColumn(QString(ZONEBOUNDARYSTOCKINGDOORUVALUEFACINGZONE),
                            QString("W/m^2*K"),
                            QString("W/m^2*K"),
                            QString("Btu/hr*ft^2*F"),
                            m_isIP,
                            &model::RefrigerationWalkIn::zoneBoundaryStockingDoorUValueFacingZone,
                            &model::RefrigerationWalkIn::setZoneBoundaryStockingDoorUValueFacingZone);
    }else if(field == ZONEBOUNDARYSTOCKINGDOOROPENINGSCHEDULEFACINGZONE){
      addComboBoxColumn<model::Schedule,model::RefrigerationWalkIn>(
          QString(ZONEBOUNDARYSTOCKINGDOOROPENINGSCHEDULEFACINGZONE),
          &openstudio::objectName,
          std::bind(&openstudio::sortByObjectName<model::Schedule>,
                      std::bind(&openstudio::model::getCompatibleSchedules,
                                  m_model,
                                  "RefrigerationWalkInZoneBoundary",
                                  "Stocking Door Opening  Facing Zone")),
          &model::RefrigerationWalkIn::zoneBoundaryStockingDoorOpeningScheduleFacingZone,
          &model::RefrigerationWalkIn::setZoneBoundaryStockingDoorOpeningScheduleFacingZone,
          boost::optional<boost::function<void (model::RefrigerationWalkIn*)> >(&model::RefrigerationWalkIn::resetZoneBoundaryStockingDoorOpeningScheduleFacingZone));
    }else{
      // unhandled
      OS_ASSERT(false);
    }
  }
}

QString RefrigerationWalkInGridController::getColor(const model:: ModelObject & modelObject)
{
  QColor defaultColor(Qt::lightGray);
  QString color(defaultColor.name());

  // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
  // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

  //std::vector<model::RefrigerationSystem> refrigerationSystems = m_model.getModelObjects<model::RefrigerationSystem>();

  //boost::optional<model::RefrigerationWalkIn> refrigerationWalkIn = modelObject.optionalCast<model::RefrigerationWalkIn>();
  //OS_ASSERT(refrigerationWalkIn);

  //boost::optional<model::RefrigerationSystem> refrigerationSystem = refrigerationWalkIn->system();
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

void RefrigerationWalkInGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  // Don't show the name column check box
  // From above in addColumns, we know that NAME is the first entry
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(0));
  OS_ASSERT(horizontalHeaderWidget);
  horizontalHeaderWidget->m_checkBox->hide();

  OSGridController::checkSelectedFields();
}

void RefrigerationWalkInGridController::onItemDropped(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::RefrigerationWalkIn>()){
      modelObject->clone(m_model);
      emit modelReset();
    }
  }
}

void RefrigerationWalkInGridController::refreshModelObjects()
{
  std::vector<model::RefrigerationWalkIn> refrigerationWalkIns = m_model.getModelObjects<model::RefrigerationWalkIn>();
  m_modelObjects = subsetCastVector<model::ModelObject>(refrigerationWalkIns);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void RefrigerationWalkInGridController::onComboBoxIndexChanged(int index)
{
  // Note: find the correct system color on RACK change,
  // but currently unable to know which row changed.
  for(unsigned index = 0; index < m_horizontalHeader.size(); index++){
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(index));
    if(horizontalHeaderWidget->m_label->text() == RACK){
      // NOTE required due to a race condition
      // Code below commented out due to a very infrequent crash in the bowels of Qt appears to be exasperated by this refresh.
      // A new refresh scheme with finer granularity may eliminate the problem, and restore rack colors.
      //QTimer::singleShot(0, this, SLOT(reset())); TODO
      break;
    }
  }
}

} // openstudio

