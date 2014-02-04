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

#include "RefrigerationGridView.hpp"

#include <shared_gui_components/OSGridView.hpp>

#include <openstudio_lib/ModelObjectItem.hpp>

#include <model/ModelObject.hpp>
#include <model/ModelObject_impl.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCase_impl.hpp>
#include <model/RefrigerationWalkIn.hpp>
#include <model/RefrigerationWalkIn_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/idd/Refrigeration_Case_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_WalkIn_FieldEnums.hxx>

#include <QBoxLayout>
#include <QLabel>
#include <QSettings>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

// Display Case Fields
#define ANTISWEATHEATERCONTROLTYPE "Anti Sweat Heater Control Type"
#define AVAILABILITYSCHEDULE "Availability Schedule"
#define AVERAGEREFRIGERANTCHARGEINVENTORY "Average Refrigerant Charge Inventory"
#define CASEANTISWEATHEATERPOWERPERUNITLENGTH "Case Anti Sweat Heater Power per Unit Length"
#define CASECREDITFRACTIONSCHEDULE "Case Credit Fraction Schedule"
#define CASEDEFROSTDRIPDOWNSCHEDULE "Case Defrost Drip Down Schedule"
#define CASEDEFROSTPOWERPERUNITLENGTH "Case Defrost Power per Unit Length"
#define CASEDEFROSTSCHEDULE "Case Defrost Schedule"
#define CASEDEFROSTTYPE "Case Defrost Type"
#define CASEHEIGHT "Case Height"
#define CASELENGTH "Case Length"
#define CASELIGHTINGSCHEDULE "Case Lighting Schedule"
#define CASEOPERATINGTEMPERATURE "Case Operating Temperature"
#define DEFROSTENERGYCORRECTIONCURVE "Defrost Energy Correction Curve"
#define DEFROSTENERGYCORRECTIONCURVETYPE "Defrost Energy Correction Curve Type"
#define DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE "Design Evaporator Temperatureor Brine Inlet Temperature"
#define FRACTIONOFANTISWEATHEATERENERGYTOCASE "Fraction of Anti Sweat Heater Energy to Case"
#define FRACTIONOFLIGHTINGENERGYTOCASE "Fraction of Lighting Energy to Case"
#define HUMIDITYATZEROANTISWEATHEATERENERGY "Humidity at Zero Anti Sweat Heater Energy"
#define INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH "Installed Case Lighting Power per Unit Length"
#define LATENTCASECREDITCURVETYPE "Latent Case Credit Curve Type"
#define MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH "Minimum Anti Sweat Heater Power per Unit Length"
#define NAME "Name"
#define OPERATINGCASEFANPOWERPERUNITLENGTH "Operating Case Fan Power per Unit Length"
#define RATEDAMBIENTRELATIVEHUMIDITY "Rated Ambient Relative Humidity"
#define RATEDAMBIENTTEMPERATURE "Rated Ambient Temperature"
#define RATEDLATENTHEATRATIO "Rated Latent Heat Ratio"
#define RATEDRUNTIMEFRACTION "Rated Runtime Fraction"
#define RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH "Rated Total Cooling Capacity per Unit Length"
#define REFRIGERATEDCASERESTOCKINGSCHEDULE "Refrigerated Case Restocking Schedule"
#define STANDARDCASEFANPOWERPERUNITLENGTH "Standard Case Fan Power per Unit Length"
#define STANDARDCASELIGHTINGPOWERPERUNITLENGTH "Standard Case Lighting Power per Unit Length"
#define THERMALZONE "Thermal Zone"
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
#define TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE "Temperature Termination Defrost Fraction to Ice"
#define ZONEBOUNDARIES "Zone Boundaries"

namespace openstudio {

RefrigerationGridView::RefrigerationGridView(const model::Model & model, QWidget * parent)
  : QWidget(parent)
{
  QVBoxLayout * layout = 0;
  
  layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);

  model::Schedule schedule = model.alwaysOnDiscreteSchedule();

  std::vector<model::ModelObject> caseModelObjects;
  caseModelObjects.push_back(model::RefrigerationCase(model,schedule));
  caseModelObjects.push_back(model::RefrigerationCase(model,schedule));
  caseModelObjects.push_back(model::RefrigerationCase(model,schedule));
  caseModelObjects.push_back(model::RefrigerationCase(model,schedule));
  caseModelObjects.push_back(model::RefrigerationCase(model,schedule));

  RefrigerationCaseGridController * refrigerationCaseGridController  = new RefrigerationCaseGridController("Display Cases", model::RefrigerationCase::iddObjectType(), model, caseModelObjects);
  OSGridView * caseGridView = new OSGridView(refrigerationCaseGridController, "Display Cases", parent);
  layout->addWidget(caseGridView);

  std::vector<model::ModelObject> walkInModelObjects;
  walkInModelObjects.push_back(model::RefrigerationWalkIn(model,schedule));
  walkInModelObjects.push_back(model::RefrigerationWalkIn(model,schedule));
  walkInModelObjects.push_back(model::RefrigerationWalkIn(model,schedule));

  RefrigerationWalkInGridController * refrigerationWalkInGridController  = new RefrigerationWalkInGridController("Walk Ins", model::RefrigerationWalkIn::iddObjectType(), model, walkInModelObjects);
  OSGridView * walkInView = new OSGridView(refrigerationWalkInGridController, "Walk Ins", parent);
  layout->addWidget(walkInView);

}

// CaseThermalZoneVectorController

void CaseThermalZoneVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == Refrigeration_CaseFields::ZoneName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> CaseThermalZoneVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::RefrigerationCase refrigerationCase = m_modelObject->cast<model::RefrigerationCase>();
    boost::optional<model::ThermalZone> thermalZone = refrigerationCase.thermalZone();
    if (thermalZone){
      result.push_back(modelObjectToItemId(*thermalZone, false));
    }
  }
  return result;
}

void CaseThermalZoneVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::RefrigerationCase refrigerationCase = m_modelObject->cast<model::RefrigerationCase>();
    refrigerationCase.resetThermalZone();
  }
}

void CaseThermalZoneVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void CaseThermalZoneVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::RefrigerationCase refrigerationCase = m_modelObject->cast<model::RefrigerationCase>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::ThermalZone>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::ThermalZone thermalZone = modelObject->cast<model::ThermalZone>();
        refrigerationCase.setThermalZone(thermalZone);
      }
    }
  }
}

RefrigerationCaseGridController::RefrigerationCaseGridController(const QString & headerText,
  IddObjectType iddObjectType,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(headerText, iddObjectType, model, modelObjects)
{
  m_vectorController = new CaseThermalZoneVectorController();
  setCategoriesAndFields();
}

void RefrigerationCaseGridController::setCategoriesAndFields()
{
  // TODO strings below should be replaced with the tokens defined above

  {
    std::vector<QString> fields;
    //fields.push_back("Rack Name");
    //fields.push_back("Rack Saturated Suction Temperature (F)");
    //fields.push_back("Fixture Name");
    fields.push_back(THERMALZONE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(CASELENGTH);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Dimensions"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(UNDERCASEHVACRETURNAIRFRACTION);
    fields.push_back(CASEOPERATINGTEMPERATURE);
    fields.push_back(DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE);
    fields.push_back(RATEDRUNTIMEFRACTION);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Operation"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH);
    fields.push_back(CASECREDITFRACTIONSCHEDULE);
    fields.push_back(RATEDLATENTHEATRATIO);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Cooling Capacity"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(STANDARDCASEFANPOWERPERUNITLENGTH);
    fields.push_back(OPERATINGCASEFANPOWERPERUNITLENGTH);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Fan"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(STANDARDCASELIGHTINGPOWERPERUNITLENGTH);
    fields.push_back(INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH);
    fields.push_back(FRACTIONOFLIGHTINGENERGYTOCASE);
    fields.push_back(CASELIGHTINGSCHEDULE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Lighting"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(CASEANTISWEATHEATERPOWERPERUNITLENGTH);
    fields.push_back(ANTISWEATHEATERCONTROLTYPE);
    fields.push_back(MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH);
    fields.push_back(HUMIDITYATZEROANTISWEATHEATERENERGY);
    fields.push_back(FRACTIONOFANTISWEATHEATERENERGYTOCASE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Case Anti-Sweat Heaters"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(CASEDEFROSTPOWERPERUNITLENGTH);
    //fields.push_back("Duration of Cycle (mins)");
    //fields.push_back("Drip Time (mins)");
    //fields.push_back("Defrost 1 Start Time");
    //fields.push_back("Defrost 2 Start Time");
    //fields.push_back("Defrost 3 Start Time");
    //fields.push_back("Defrost 4 Start Time");
    //fields.push_back("Defrost 5 Start Time");
    //fields.push_back("Defrost 6 Start Time");
    fields.push_back(DEFROSTENERGYCORRECTIONCURVE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Defrost"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(REFRIGERATEDCASERESTOCKINGSCHEDULE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Restocking"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(CASEDEFROSTTYPE);
    fields.push_back(CASEHEIGHT);
    fields.push_back(CASELENGTH);
    fields.push_back(LATENTCASECREDITCURVETYPE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Custom"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

}
void RefrigerationCaseGridController::addColumns(const std::vector<QString> & fields)
{
  m_baseConcepts.clear();

  Q_FOREACH(QString field, fields){
    if(field == RATEDAMBIENTTEMPERATURE){
      addDoubleEditColumn(QString(RATEDAMBIENTTEMPERATURE),
                          &model::RefrigerationCase::ratedAmbientTemperature,
                          &model::RefrigerationCase::setRatedAmbientTemperature);
    }else if(field == RATEDAMBIENTRELATIVEHUMIDITY){
      addDoubleEditColumn(QString(RATEDAMBIENTRELATIVEHUMIDITY),
                          &model::RefrigerationCase::ratedAmbientRelativeHumidity,
                          &model::RefrigerationCase::setRatedAmbientRelativeHumidity);
    }else if(field == RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH){
      addDoubleEditColumn(QString(RATEDTOTALCOOLINGCAPACITYPERUNITLENGTH),
                          &model::RefrigerationCase::ratedTotalCoolingCapacityperUnitLength,
                          &model::RefrigerationCase::setRatedTotalCoolingCapacityperUnitLength);
    }else if(field == RATEDLATENTHEATRATIO){
      addDoubleEditColumn(QString(RATEDLATENTHEATRATIO),
                          &model::RefrigerationCase::ratedLatentHeatRatio,
                          &model::RefrigerationCase::setRatedLatentHeatRatio);
    }else if(field == RATEDRUNTIMEFRACTION){
      addDoubleEditColumn(QString(RATEDRUNTIMEFRACTION),
                          &model::RefrigerationCase::ratedRuntimeFraction,
                          &model::RefrigerationCase::setRatedRuntimeFraction);
    }else if(field == CASELENGTH){
      addDoubleEditColumn(QString(CASELENGTH),
                          &model::RefrigerationCase::caseLength,
                          &model::RefrigerationCase::setCaseLength);
    }else if(field == CASEOPERATINGTEMPERATURE){
      addDoubleEditColumn(QString(CASEOPERATINGTEMPERATURE),
                          &model::RefrigerationCase::caseOperatingTemperature,
                          &model::RefrigerationCase::setCaseOperatingTemperature);
    }else if(field == STANDARDCASEFANPOWERPERUNITLENGTH){
      addDoubleEditColumn(QString(STANDARDCASEFANPOWERPERUNITLENGTH),
                          &model::RefrigerationCase::standardCaseFanPowerperUnitLength,
                          &model::RefrigerationCase::setStandardCaseFanPowerperUnitLength);
    }else if(field == OPERATINGCASEFANPOWERPERUNITLENGTH){
      addDoubleEditColumn(QString(OPERATINGCASEFANPOWERPERUNITLENGTH),
                          &model::RefrigerationCase::operatingCaseFanPowerperUnitLength,
                          &model::RefrigerationCase::setOperatingCaseFanPowerperUnitLength);
    }else if(field == FRACTIONOFLIGHTINGENERGYTOCASE){
      addDoubleEditColumn(QString(FRACTIONOFLIGHTINGENERGYTOCASE),
                          &model::RefrigerationCase::fractionofLightingEnergytoCase,
                          &model::RefrigerationCase::setFractionofLightingEnergytoCase);
    }else if(field == CASEANTISWEATHEATERPOWERPERUNITLENGTH){
      addDoubleEditColumn(QString(CASEANTISWEATHEATERPOWERPERUNITLENGTH),
                          &model::RefrigerationCase::caseAntiSweatHeaterPowerperUnitLength,
                          &model::RefrigerationCase::setCaseAntiSweatHeaterPowerperUnitLength);
    }else if(field == MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH){
      addDoubleEditColumn(QString(MINIMUMANTISWEATHEATERPOWERPERUNITLENGTH),
                          &model::RefrigerationCase::minimumAntiSweatHeaterPowerperUnitLength,
                          &model::RefrigerationCase::setMinimumAntiSweatHeaterPowerperUnitLength);
    }else if(field == CASEHEIGHT){
      addDoubleEditColumn(QString(CASEHEIGHT),
                          &model::RefrigerationCase::caseHeight,
                          &model::RefrigerationCase::setCaseHeight);
    }else if(field == FRACTIONOFANTISWEATHEATERENERGYTOCASE){
      addDoubleEditColumn(QString(FRACTIONOFANTISWEATHEATERENERGYTOCASE),
                          &model::RefrigerationCase::fractionofAntiSweatHeaterEnergytoCase,
                          &model::RefrigerationCase::setFractionofAntiSweatHeaterEnergytoCase);
    }else if(field == CASEDEFROSTPOWERPERUNITLENGTH){
      addDoubleEditColumn(QString(CASEDEFROSTPOWERPERUNITLENGTH),
                          &model::RefrigerationCase::caseDefrostPowerperUnitLength,
                          &model::RefrigerationCase::setCaseDefrostPowerperUnitLength);
    }else if(field == UNDERCASEHVACRETURNAIRFRACTION){
      addDoubleEditColumn(QString(UNDERCASEHVACRETURNAIRFRACTION),
                          &model::RefrigerationCase::underCaseHVACReturnAirFraction,
                          &model::RefrigerationCase::setUnderCaseHVACReturnAirFraction);
    }else if(field == STANDARDCASELIGHTINGPOWERPERUNITLENGTH){
      //addDoubleEditColumn(QString(STANDARDCASELIGHTINGPOWERPERUNITLENGTH),
      //                    &model::RefrigerationCase::standardCaseLightingPowerperUnitLength,
      //                    &model::RefrigerationCase::setStandardCaseLightingPowerperUnitLength); // TODO does not return bool
    }else if(field == HUMIDITYATZEROANTISWEATHEATERENERGY){
      //addDoubleEditColumn(QString(HUMIDITYATZEROANTISWEATHEATERENERGY),
      //                    &model::RefrigerationCase::humidityatZeroAntiSweatHeaterEnergy,
      //                    &model::RefrigerationCase::setHumidityatZeroAntiSweatHeaterEnergy); // TODO does not return bool
    }else if(field == AVERAGEREFRIGERANTCHARGEINVENTORY){
      //addDoubleEditColumn(QString(AVERAGEREFRIGERANTCHARGEINVENTORY),
      //                    &model::RefrigerationCase::averageRefrigerantChargeInventory,
      //                    &model::RefrigerationCase::setAverageRefrigerantChargeInventory); // TODO does not return bool
    }else if(field == LATENTCASECREDITCURVETYPE){
      addComboBoxColumn(QString(LATENTCASECREDITCURVETYPE),
                        &model::RefrigerationCase::latentCaseCreditCurveTypeValues,
                        &model::RefrigerationCase::latentCaseCreditCurveType,
                        &model::RefrigerationCase::setLatentCaseCreditCurveType);
    }else if(field == ANTISWEATHEATERCONTROLTYPE){
      addComboBoxColumn(QString(ANTISWEATHEATERCONTROLTYPE),
                        &model::RefrigerationCase::antiSweatHeaterControlTypeValues,
                        &model::RefrigerationCase::antiSweatHeaterControlType,
                        &model::RefrigerationCase::setAntiSweatHeaterControlType);
    }else if(field == CASEDEFROSTTYPE){
      addComboBoxColumn(QString(CASEDEFROSTTYPE),
                        &model::RefrigerationCase::caseDefrostTypeValues,
                        &model::RefrigerationCase::caseDefrostType,
                        &model::RefrigerationCase::setCaseDefrostType);
    }else if(field == DEFROSTENERGYCORRECTIONCURVETYPE){
      addComboBoxColumn(QString(DEFROSTENERGYCORRECTIONCURVETYPE),
                        &model::RefrigerationCase::defrostEnergyCorrectionCurveTypeValues,
                        &model::RefrigerationCase::defrostEnergyCorrectionCurveType,
                        &model::RefrigerationCase::setDefrostEnergyCorrectionCurveType);
    }else if(field == INSTALLEDCASELIGHTINGPOWERPERUNITLENGTH){
      //boost::optional<double> installedCaseLightingPowerperUnitLength() const;
    }else if(field == DESIGNEVAPORATORTEMPERATUREORBRINEINLETTEMPERATURE){
      // TODO required
      //boost::optional<double> designEvaporatorTemperatureorBrineInletTemperature() const;
    }else if(field == CASELIGHTINGSCHEDULE){
      // TODO required
      //boost::optional<Schedule> caseLightingSchedule() const;
    }else if(field == CASEDEFROSTSCHEDULE){
      //boost::optional<Schedule> caseDefrostSchedule() const;
    }else if(field == CASEDEFROSTDRIPDOWNSCHEDULE){
      //boost::optional<Schedule> caseDefrostDripDownSchedule() const;
    }else if(field == REFRIGERATEDCASERESTOCKINGSCHEDULE){
      // TODO required
      //boost::optional<Schedule> refrigeratedCaseRestockingSchedule() const;
    }else if(field == CASECREDITFRACTIONSCHEDULE){
      //boost::optional<Schedule> caseCreditFractionSchedule() const;
    }else if(field == AVAILABILITYSCHEDULE){
      // TODO required
      //boost::optional<Schedule> availabilitySchedule() const;
    }else if(field == THERMALZONE){
      //addDropZoneColumn(QString(THERMALZONE), // TODO Evan
      //                  &model::RefrigerationCase::thermalZone,
      //                  model::RefrigerationCase::setThermalZone);
    }else if(field == DEFROSTENERGYCORRECTIONCURVE){
      // TODO required
      //boost::optional<CurveCubic> defrostEnergyCorrectionCurve() const;
    }else{
      // unhandled
//      OS_ASSERT(false); TODO add this back at a later time
    }
  }
}

RefrigerationWalkInGridController::RefrigerationWalkInGridController(const QString & headerText,
  IddObjectType iddObjectType,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(headerText, iddObjectType, model, modelObjects)
{
  setCategoriesAndFields();
}

void RefrigerationWalkInGridController::setCategoriesAndFields()
{
  // TODO strings below should be replaced with the tokens defined above

  {
    std::vector<QString> fields;
    //fields.push_back("Rack Name");
    //fields.push_back("Rack Saturated Suction Temperature (F)");
    fields.push_back(NAME);
    //fields.push_back("Walk-in Type");
    //fields.push_back("Manufacturer & Model No.");
    //fields.push_back("Zone Adjacent");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    //fields.push_back("Length (ft)");
    //fields.push_back("Width (ft)");
    //fields.push_back("Height (ft)");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Dimensions"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }
  
  {
    std::vector<QString> fields;
    fields.push_back(INSULATEDFLOORUVALUE);
    //fields.push_back("Wall/Roof R-Value");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Construction"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }
  
  {
    std::vector<QString> fields;
    //fields.push_back("Stocking Door Area (ft2)");
    //fields.push_back("Stocking Door Height (ft)");
    //fields.push_back("Stocking Door R-Value (hr-ft2-F/Btu)");
    //fields.push_back("Stocking Door Opening Schedule");
    //fields.push_back("Stocking Door Opening Protection");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Stocking Doors"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(AVAILABILITYSCHEDULE);
    fields.push_back(OPERATINGTEMPERATURE);
    fields.push_back(RATEDCOOLINGSOURCETEMPERATURE);
    fields.push_back(RATEDCOILCOOLINGCAPACITY);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Operation"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDCOOLINGCOILFANPOWER);
    fields.push_back(RATEDCIRCULATIONFANPOWER);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Fans"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDTOTALLIGHTINGPOWER);
    fields.push_back(LIGHTINGSCHEDULE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Lighting"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(RATEDTOTALHEATINGPOWER);
    fields.push_back(HEATINGPOWERSCHEDULE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Heating"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(DEFROSTTYPE);
    fields.push_back(DEFROSTCONTROLTYPE);
    //fields.push_back("Drain/Supplemental (Drain and Drip-Pan only) Heaters (W)");
    fields.push_back(DEFROSTPOWER);
    //fields.push_back("Duraction of Cycle (mins)");
    //fields.push_back("Drip Time (mins)");
    //fields.push_back("Defrost 1 Start Time");
    //fields.push_back("Defrost 2 Start Time");
    //fields.push_back("Defrost 3 Start Time");
    //fields.push_back("Defrost 4 Start Time");
    //fields.push_back("Defrost 5 Start Time");
    //fields.push_back("Defrost 6 Start Time");
    fields.push_back(TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Defrost"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }
  
  {
    std::vector<QString> fields;
    fields.push_back(RESTOCKINGSCHEDULE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Restocking"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(DEFROSTCONTROLTYPE);
    fields.push_back(INSULATEDFLOORUVALUE);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Custom"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

}

void RefrigerationWalkInGridController::addColumns(const std::vector<QString> & fields)
{
  m_baseConcepts.clear();

  Q_FOREACH(QString field, fields){
    if(field == DEFROSTTYPE){
      addComboBoxColumn(QString(DEFROSTTYPE),
                      &model::RefrigerationWalkIn::defrostTypeValues,
                      &model::RefrigerationWalkIn::defrostType,
                      &model::RefrigerationWalkIn::setDefrostType);
    }else if(field == DEFROSTCONTROLTYPE){
      addComboBoxColumn(QString(DEFROSTCONTROLTYPE),
                      &model::RefrigerationWalkIn::defrostControlTypeValues,
                      &model::RefrigerationWalkIn::defrostControlType,
                      &model::RefrigerationWalkIn::setDefrostControlType);
    }else if(field == OPERATINGTEMPERATURE){
      addDoubleEditColumn(QString(OPERATINGTEMPERATURE),
                          &model::RefrigerationWalkIn::operatingTemperature,
                          &model::RefrigerationWalkIn::setOperatingTemperature);
    }else if(field == RATEDCOOLINGSOURCETEMPERATURE){
      addDoubleEditColumn(QString(RATEDCOOLINGSOURCETEMPERATURE),
                          &model::RefrigerationWalkIn::ratedCoolingSourceTemperature,
                          &model::RefrigerationWalkIn::setRatedCoolingSourceTemperature);
    }else if(field == RATEDCOOLINGCOILFANPOWER){
      addDoubleEditColumn(QString(RATEDCOOLINGCOILFANPOWER),
                          &model::RefrigerationWalkIn::ratedCoolingCoilFanPower,
                          &model::RefrigerationWalkIn::setRatedCoolingCoilFanPower);
    }else if(field == RATEDCIRCULATIONFANPOWER){
      addDoubleEditColumn(QString(RATEDCIRCULATIONFANPOWER),
                          &model::RefrigerationWalkIn::ratedCirculationFanPower,
                          &model::RefrigerationWalkIn::setRatedCirculationFanPower);
    }else if(field == INSULATEDFLOORSURFACEAREA){
      addDoubleEditColumn(QString(INSULATEDFLOORSURFACEAREA),
                          &model::RefrigerationWalkIn::insulatedFloorSurfaceArea,
                          &model::RefrigerationWalkIn::setInsulatedFloorSurfaceArea);
    }else if(field == INSULATEDFLOORUVALUE){
      addDoubleEditColumn(QString(INSULATEDFLOORUVALUE),
                          &model::RefrigerationWalkIn::insulatedFloorUValue,
                          &model::RefrigerationWalkIn::setInsulatedFloorUValue); 
    }else if(field == RATEDCOILCOOLINGCAPACITY){
      //addDoubleEditColumn(QString(RATEDCOILCOOLINGCAPACITY),
      //                    &model::RefrigerationWalkIn::ratedCoilCoolingCapacity,
      //                    &model::RefrigerationWalkIn::setRatedCoilCoolingCapacity); // TODO does not return bool
    }else if(field == RATEDTOTALHEATINGPOWER){
      //addDoubleEditColumn(QString(RATEDTOTALHEATINGPOWER),
      //                    &model::RefrigerationWalkIn::ratedTotalHeatingPower,
      //                    &model::RefrigerationWalkIn::setRatedTotalHeatingPower); // TODO does not return bool
    }else if(field == RATEDTOTALLIGHTINGPOWER){
      // TODO required
      //addDoubleEditColumn(QString(RATEDTOTALLIGHTINGPOWER),
      //                    &model::RefrigerationWalkIn::ratedTotalLightingPower,
      //                    &model::RefrigerationWalkIn::setRatedTotalLightingPower); // TODO does not return bool
    }else if(field == AVERAGEREFRIGERANTCHARGEINVENTORY){
      //addDoubleEditColumn(QString(AVERAGEREFRIGERANTCHARGEINVENTORY),
      //                    &model::RefrigerationWalkIn::averageRefrigerantChargeInventory,
      //                    &model::RefrigerationWalkIn::setAverageRefrigerantChargeInventory); // TODO does not return bool
    }else if(field == DEFROSTSCHEDULE){
      //Schedule defrostSchedule() const;
    }else if(field == DEFROSTPOWER){
      // TODO required
      //boost::optional<double> defrostPower() const;
    }else if(field == TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE){
      // TODO required
      //boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;
    }else if(field == AVAILABILITYSCHEDULE){
      // TODO required
      //boost::optional<Schedule> availabilitySchedule() const;
    }else if(field == HEATINGPOWERSCHEDULE){
      // TODO required
      //boost::optional<Schedule> heatingPowerSchedule() const;
    }else if(field == LIGHTINGSCHEDULE){
      // TODO required
      //boost::optional<Schedule> lightingSchedule() const;
    }else if(field == DEFROSTDRIPDOWNSCHEDULE){
      //boost::optional<Schedule> defrostDripDownSchedule() const;
    }else if(field == RESTOCKINGSCHEDULE){
      // TODO required
      //boost::optional<Schedule> restockingSchedule() const;
    }else if(field == ZONEBOUNDARIES){
      //std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries() const;
    }else{
      // unhandled
//      OS_ASSERT(false); TODO add this back at a later time
    }
  }
}

} // openstudio

