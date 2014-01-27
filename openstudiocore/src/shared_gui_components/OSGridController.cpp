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

#include "OSGridController.hpp"

#include <model/Model_impl.hpp>
#include <model/ModelObject_impl.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCase_Impl.hpp>
#include <model/RefrigerationWalkIn.hpp>
#include <model/RefrigerationWalkIn_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QWidget>

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
#define DEFROSTCONTROLTYPEVALUES "Defrost Control Type"
#define DEFROSTDRIPDOWNSCHEDULE "Defrost Drip Down Schedule"
#define DEFROSTPOWER "Defrost Power"
#define DEFROSTSCHEDULE "Defrost Schedule"
#define DEFROSTTYPEVALUES "Defrost Type"
#define HEATINGPOWERSCHEDULE "Heating Power Schedule"
#define INSULATEDFLOORSURFACEAREA "Insulated Floor Surface Area"
#define INSULATEDFLOORUVALUE "Insulated Floor U Value"
#define LIGHTINGSCHEDULE "Lighting Schedule"
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

OSGridController::OSGridController()
  : QObject()
{
}

OSGridController::OSGridController(IddObjectType iddObjectType, model::Model model)
  : QObject(),
  m_baseConcepts(std::vector<QSharedPointer<BaseConcept> >()),
  m_model(model),
  m_modelObjects(std::vector<model::ModelObject>()),
  m_iddObjectType(iddObjectType),
  m_categoriesAndFields(std::vector<std::pair<QString,std::vector<QString>>>())
{
  model::Schedule schedule = m_model.alwaysOnDiscreteSchedule();
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));

  addComboBoxColumn(QString("Defrost Type"),
                    &model::RefrigerationCase::caseDefrostTypeValues,
                    &model::RefrigerationCase::caseDefrostType,
                    &model::RefrigerationCase::setCaseDefrostType);
             
  addDoubleEditColumn(QString("Operating Temperature"),
                    &model::RefrigerationCase::caseOperatingTemperature,
                    &model::RefrigerationCase::setCaseOperatingTemperature);

  addLineEditColumn(QString("Latent Case Credit Curve Type"),
                    &model::RefrigerationCase::latentCaseCreditCurveType,
                    &model::RefrigerationCase::setLatentCaseCreditCurveType);

}

OSGridController::~OSGridController()
{
}

void OSGridController::setCaseCategoriesAndFields()
{
  // TODO strings below should be replaced with the tokens defined above

  {
    std::vector<QString> fields;
    fields.push_back("Rack Name");
    fields.push_back("Rack Saturated Suction Temperature (F)");
    fields.push_back("Fixture Name");
    fields.push_back("Manufacturer & Model No.");
    fields.push_back("Zone Location");
    fields.push_back("Fixture Type");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Case Length (ft)");
    fields.push_back("# of Doors");
    fields.push_back("Door Width (ft)");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Dimensions"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Under Case Return Air Fraction");
    fields.push_back("Case Operating Temperature (F)");
    fields.push_back("Design Evaporator Temperature (F)");
    fields.push_back("Rated Runtime Fraction");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Operation"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Rated Cooling Capacity (Btu/hr/ft or Btu/hr/dr)");
    fields.push_back("Actual Cooling Capacity (Btu/hr/ft or Btu/hr/dr)");
    fields.push_back("Case Credit Fraction Schedule Name");
    fields.push_back("Rated Latent Heat Ratio");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Cooling Capacity"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Case Fan Rated (W/ft or W/dr))");
    fields.push_back("Case Fan Operating (W/ft or W/dr)");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Fan"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Case Lighting Rated (W/ft or W/dr)");
    fields.push_back("Case Lighting Installed (W/ft or W/dr)");
    fields.push_back("Case Lighting Fraction to Case");
    fields.push_back("Case Lighting Schedule");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Lighting"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Anti-Sweat Heaters Rated (W/ft or W/dr)");
    fields.push_back("Anti-Sweat Heaters Actual (W/ft or W/dr)");
    fields.push_back("Anti-Sweat Heaters Control Type");
    fields.push_back("Anti-Sweat Heaters Minimum (W/ft or W/dr)");
    fields.push_back("Humidity At Zero Anti-Sweat Heater Energy (%)");
    fields.push_back("Anti-Sweat Heaters Fraction to Case");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Case Anti-Sweat Heaters"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Electric Defrost (W/ft or W/dr)");
    fields.push_back("Duration of Cycle (mins)");
    fields.push_back("Drip Time (mins)");
    fields.push_back("Defrost 1 Start Time");
    fields.push_back("Defrost 2 Start Time");
    fields.push_back("Defrost 3 Start Time");
    fields.push_back("Defrost 4 Start Time");
    fields.push_back("Defrost 5 Start Time");
    fields.push_back("Defrost 6 Start Time");
    fields.push_back("Defrost Correction Curve Type");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Defrost"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Restocking Schedule");
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

void OSGridController::setWalkInCategoriesAndFields()
{
  // TODO strings below should be replaced with the tokens defined above

  {
    std::vector<QString> fields;
    fields.push_back("Rack Name");
    fields.push_back("Rack Saturated Suction Temperature (F)");
    fields.push_back("Walk-in Name");
    fields.push_back("Walk-in Type");
    fields.push_back("Manufacturer & Model No.");
    fields.push_back("Zone Adjacent");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Length (ft)");
    fields.push_back("Width (ft)");
    fields.push_back("Height (ft)");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Dimensions"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }
  
  {
    std::vector<QString> fields;
    fields.push_back("Floor R-Value");
    fields.push_back("Wall/Roof R-Value");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Construction"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }
  
  {
    std::vector<QString> fields;
    fields.push_back("Stocking Door Area (ft2)");
    fields.push_back("Stocking Door Height (ft)");
    fields.push_back("Stocking Door R-Value (hr-ft2-F/Btu)");
    fields.push_back("Stocking Door Opening Schedule");
    fields.push_back("Stocking Door Opening Protection");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Stocking Doors"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Availability Schedule");
    fields.push_back("Operating Temperature (F)");
    fields.push_back("Walk-In Rated Cooling Source Temperature (F)");
    fields.push_back("Walk-In Rated Cooling Capacity (Btu/hr)");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Operation"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Walk-In Rated Cooling Coil Fan Power (W)");
    fields.push_back("Circulation Fan Power (W)");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Fans"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Walk-In Rated Total Lighting Power (W)");
    fields.push_back("Lighting Schedule");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Lighting"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("WalkIn Rated Total Heating Power (W)");
    fields.push_back("WalkIn Heating Power Schedule");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Heating"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back("Walk-In Defrost Type");
    fields.push_back("Walk-In Defrost Control Type");
    fields.push_back("Drain/Supplemental (Drain and Drip-Pan only) Heaters (W)");
    fields.push_back("Electric Defrost (W)");
    fields.push_back("Duraction of Cycle (mins)");
    fields.push_back("Drip Time (mins)");
    fields.push_back("Defrost 1 Start Time");
    fields.push_back("Defrost 2 Start Time");
    fields.push_back("Defrost 3 Start Time");
    fields.push_back("Defrost 4 Start Time");
    fields.push_back("Defrost 5 Start Time");
    fields.push_back("Defrost 6 Start Time");
    fields.push_back("Temperature Termination Defrost Fraction to Ice");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Defrost"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }
  
  {
    std::vector<QString> fields;
    fields.push_back("Restocking Schedule");
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Restocking"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(DEFROSTCONTROLTYPEVALUES);
    fields.push_back(DEFROSTPOWER);
    std::pair<QString,std::vector<QString>> categoryAndFields = std::make_pair(QString("Custom"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

}

std::vector<QString> OSGridController::categories()
{
  std::vector<QString> categories;

  for(unsigned i = 0; i < m_categoriesAndFields.size(); i++){
    categories.push_back(m_categoriesAndFields.at(i).first);
  }

  return categories;
}

std::vector<std::pair<QString,std::vector<QString>>> OSGridController::categoriesAndFields()
{
  return m_categoriesAndFields;
}

void OSGridController::categorySelected(int index)
{
  std::vector<QString> fields = m_categoriesAndFields.at(index).second;
 
  //addColumns(fields); // TODO use this after derived classes exist
  addDisplayCaseColumns(fields);
}

void OSGridController::addDisplayCaseColumns(const std::vector<QString> & fields)
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
      //boost::optional<double> designEvaporatorTemperatureorBrineInletTemperature() const;
    }else if(field == CASELIGHTINGSCHEDULE){
      //boost::optional<Schedule> caseLightingSchedule() const;
    }else if(field == CASEDEFROSTSCHEDULE){
      //boost::optional<Schedule> caseDefrostSchedule() const;
    }else if(field == CASEDEFROSTDRIPDOWNSCHEDULE){
      //boost::optional<Schedule> caseDefrostDripDownSchedule() const;
    }else if(field == REFRIGERATEDCASERESTOCKINGSCHEDULE){
      //boost::optional<Schedule> refrigeratedCaseRestockingSchedule() const;
    }else if(field == CASECREDITFRACTIONSCHEDULE){
      //boost::optional<Schedule> caseCreditFractionSchedule() const;
    }else if(field == AVAILABILITYSCHEDULE){
      //boost::optional<Schedule> availabilitySchedule() const;
    }else if(field == THERMALZONE){
      //boost::optional<ThermalZone> thermalZone() const;
    }else if(field == DEFROSTENERGYCORRECTIONCURVE){
      //boost::optional<CurveCubic> defrostEnergyCorrectionCurve() const;
    }else{
      // unhandled
//      OS_ASSERT(false); TODO add this back at a later time
    }
  }
}

void OSGridController::addWalkInColumns(const std::vector<QString> & fields)
{
  Q_FOREACH(QString field, fields){
    if(field == DEFROSTTYPEVALUES){
      addComboBoxColumn(QString(DEFROSTTYPEVALUES),
                      &model::RefrigerationWalkIn::defrostTypeValues,
                      &model::RefrigerationWalkIn::defrostType,
                      &model::RefrigerationWalkIn::setDefrostType);
    }else if(field == DEFROSTCONTROLTYPEVALUES){
      addComboBoxColumn(QString(DEFROSTCONTROLTYPEVALUES),
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
      //boost::optional<double> defrostPower() const;
    }else if(field == TEMPERATURETERMINATIONDEFROSTFRACTIONTOICE){
      //boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;
    }else if(field == AVAILABILITYSCHEDULE){
      //boost::optional<Schedule> availabilitySchedule() const;
    }else if(field == HEATINGPOWERSCHEDULE){
      //boost::optional<Schedule> heatingPowerSchedule() const;
    }else if(field == LIGHTINGSCHEDULE){
      //boost::optional<Schedule> lightingSchedule() const;
    }else if(field == DEFROSTDRIPDOWNSCHEDULE){
      //boost::optional<Schedule> defrostDripDownSchedule() const;
    }else if(field == RESTOCKINGSCHEDULE){
      //boost::optional<Schedule> restockingSchedule() const;
    }else if(field == ZONEBOUNDARIES){
      //std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries() const;
    }else{
      // unhandled
      OS_ASSERT(false);
    }
  }
}

QWidget * OSGridController::widgetAt(int i, int j)
{
  OS_ASSERT(i >= 0);
  OS_ASSERT(j >= 0);
  OS_ASSERT(m_modelObjects.size() > static_cast<unsigned>(i));
  OS_ASSERT(m_baseConcepts.size() > static_cast<unsigned>(j));

  model::ModelObject mo = m_modelObjects[i];

  QSharedPointer<BaseConcept> baseConcept = m_baseConcepts[j];

  if(QSharedPointer<CheckBoxConcept> checkBoxConcept = baseConcept.dynamicCast<CheckBoxConcept>()){

    OSCheckBox2 * checkBox = new OSCheckBox2();

    checkBox->bind(mo,
              boost::bind(&CheckBoxConcept::get,checkBoxConcept.data(),mo),
              boost::optional<BoolSetter>(boost::bind(&CheckBoxConcept::set,checkBoxConcept.data(),mo,_1)),
              boost::none,
              boost::none);

    return checkBox;

  } else if(QSharedPointer<ComboBoxConcept> comboBoxConcept = baseConcept.dynamicCast<ComboBoxConcept>()) {

      OSComboBox2 * comboBox = new OSComboBox2();

      comboBox->bindRequired(mo,
                boost::bind(&ComboBoxConcept::choices,comboBoxConcept.data()),
                boost::bind(&ComboBoxConcept::get,comboBoxConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&ComboBoxConcept::set,comboBoxConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      return comboBox;

  } else if(QSharedPointer<DoubleEditConcept> doubleEditConcept = baseConcept.dynamicCast<DoubleEditConcept>()) {

      OSDoubleEdit2 * doubleEdit = new OSDoubleEdit2();

      doubleEdit->bindRequired(mo,
                boost::bind(&DoubleEditConcept::get,doubleEditConcept.data(),mo),
                boost::optional<DoubleSetter>(boost::bind(&DoubleEditConcept::set,doubleEditConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      return doubleEdit;

  } else if(QSharedPointer<IntegerEditConcept> integerEditConcept = baseConcept.dynamicCast<IntegerEditConcept>()) {

      OSIntegerEdit2 * integerEdit = new OSIntegerEdit2();

      integerEdit->bindRequired(mo,
                boost::bind(&IntegerEditConcept::get,integerEditConcept.data(),mo),
                boost::optional<IntSetter>(boost::bind(&IntegerEditConcept::set,integerEditConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      return integerEdit;

  } else if(QSharedPointer<LineEditConcept> lineEditConcept = baseConcept.dynamicCast<LineEditConcept>()) {

      OSLineEdit2 * lineEdit = new OSLineEdit2();

      lineEdit->bindRequired(mo,
                boost::bind(&LineEditConcept::get,lineEditConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&LineEditConcept::set,lineEditConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      return lineEdit;

  } else if(QSharedPointer<QuantityEditConcept> quantityEditConcept = baseConcept.dynamicCast<QuantityEditConcept>()) {

      OSQuantityEdit2 * quantityEdit = new OSQuantityEdit2("people/m^2", "people/m^2", "people/ft^2", true);

      quantityEdit->bindRequired(true,
                mo,
                boost::bind(&QuantityEditConcept::get,quantityEditConcept.data(),mo),
                boost::optional<DoubleSetter>(boost::bind(&QuantityEditConcept::set,quantityEditConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      return quantityEdit;

  } else if(QSharedPointer<UnsignedEditConcept> unsignedEdiConcept = baseConcept.dynamicCast<UnsignedEditConcept>()) {

      OSUnsignedEdit2 * unsignedEdit = new OSUnsignedEdit2();

      unsignedEdit->bind(mo,
                boost::bind(&UnsignedEditConcept::get,unsignedEdiConcept.data(),mo),
                boost::optional<UnsignedSetter>(boost::bind(&UnsignedEditConcept::set,unsignedEdiConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      return unsignedEdit;

  } else {
    // Unknown type
    OS_ASSERT(false);
    return new QWidget();
  }
}

int OSGridController::rowCount() const
{
  return m_modelObjects.size();
}
   
int OSGridController::columnCount() const
{
  return m_baseConcepts.size();
}

std::vector<QWidget *> OSGridController::row(int i)
{
  return std::vector<QWidget *>();
}

} // openstudio

