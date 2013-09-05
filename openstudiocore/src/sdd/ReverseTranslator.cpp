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

#include <sdd/ReverseTranslator.hpp>

#include <model/Model.hpp>
#include <model/Component.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/TimeDependentValuation.hpp>
#include <model/TimeDependentValuation_Impl.hpp>
#include <model/Construction.hpp>
#include <model/SimpleGlazing.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/Timestep.hpp>
#include <model/Timestep_Impl.hpp>
#include <model/Meter.hpp>
#include <model/SimulationControl.hpp>
#include <model/SimulationControl_Impl.hpp>
#include <model/RunPeriod.hpp>
#include <model/RunPeriod_Impl.hpp>
#include <model/YearDescription.hpp>
#include <model/YearDescription_Impl.hpp>
#include <model/OutputControlReportingTolerances.hpp>
#include <model/OutputControlReportingTolerances_Impl.hpp>

#include <energyplus/ReverseTranslator.hpp>

#include <osversion/VersionTranslator.hpp>

#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/plot/ProgressBar.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/IPUnit.hpp>
#include <utilities/units/SIUnit.hpp>
#include <utilities/units/BTUUnit.hpp>
#include <utilities/units/CFMUnit.hpp>
#include <utilities/units/FahrenheitUnit.hpp>
#include <utilities/units/MPHUnit.hpp>
#include <utilities/units/WhUnit.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/Unit.hpp>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QThread>

namespace openstudio {
namespace sdd {

  std::ostream& operator<<(std::ostream& os, const QDomElement& element)
  {
    QString str;
    QTextStream qts(&str);
    element.save(qts, 2);
    os << str.toStdString();
    return os;
  }

  ReverseTranslator::ReverseTranslator( bool masterAutosize )
    : m_autosize(true),
      m_masterAutosize(masterAutosize)
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.sdd\\.ReverseTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  ReverseTranslator::~ReverseTranslator()
  {
  }

  boost::optional<openstudio::model::Model> ReverseTranslator::loadModel(const openstudio::path& path, ProgressBar* progressBar){
    
    m_path = path;

    m_progressBar = progressBar;

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    boost::optional<openstudio::model::Model> result;

    if (boost::filesystem::exists(path)){

      QFile file(toQString(path));
      if (file.open(QFile::ReadOnly)){
        QDomDocument doc;
        doc.setContent(&file);
        file.close();

        result = this->convert(doc);
      }
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::errors() const
  {
    std::vector<LogMessage> result;

    BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::string ReverseTranslator::escapeName(QString name)
  {
    return name.replace(',', '-').replace(';', '-').toStdString();
  }

  boost::optional<model::Model> ReverseTranslator::convert(const QDomDocument& doc)
  {
    return translateSDD(doc.documentElement(), doc);
  }

  boost::optional<model::Model> ReverseTranslator::translateSDD(const QDomElement& element, const QDomDocument& doc)
  {
    boost::optional<model::Model> result;

    // get project, assume one project per file
    QDomElement projectElement = element.firstChildElement("Proj");
    if (!projectElement.isNull()){

      result = openstudio::model::Model();
      result->setFastNaming(true);

      // do runperiod
      boost::optional<model::ModelObject> runPeriod = translateRunPeriod(projectElement, doc, *result);
      //OS_ASSERT(!runPeriod.empty()); // what type of error handling do we want?

      // do design days
      std::vector<WorkspaceObject> designDays = translateDesignDays(projectElement, doc, *result);
      //OS_ASSERT(!designDays.empty()); // what type of error handling do we want?

      // do weather file
      boost::optional<model::ModelObject> weatherFile = translateWeatherFile(projectElement, doc, *result);
      //OS_ASSERT(weatherFile); // what type of error handling do we want?

      // do site after design days and weather file
      boost::optional<model::ModelObject> site = translateSite(projectElement, doc, *result);
      //OS_ASSERT(site); // what type of error handling do we want?
      if (!site){
        LOG(Error, "Could not find site information in SDD");
      }

      // HVACAutoSizing
      QDomElement hvacAutoSizingElement = projectElement.firstChildElement("HVACAutoSizing");
      if( hvacAutoSizingElement.text().toInt() == 0 )
      {
        m_autosize = false;
      }

      // do materials before constructions
      QDomNodeList materialElements = projectElement.elementsByTagName("Mat");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Materials"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(materialElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < materialElements.count(); i++){
        QDomElement materialElement = materialElements.at(i).toElement();
        boost::optional<model::ModelObject> material = translateMaterial(materialElement, doc, *result);
        OS_ASSERT(material); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // do constructions before geometry

      // layered constructions
      QDomNodeList constructionElements = projectElement.elementsByTagName("ConsAssm");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Constructions"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(constructionElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < constructionElements.count(); i++){
        QDomElement constructionElement = constructionElements.at(i).toElement();
        boost::optional<model::ModelObject> construction = translateConstructAssembly(constructionElement, doc, *result);
        OS_ASSERT(construction); // what type of error handling do we want?
                
        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // door constructions
      QDomNodeList doorConstructionElements = projectElement.elementsByTagName("DrCons");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Door Constructions"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(doorConstructionElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < doorConstructionElements.count(); i++){
        QDomElement doorConstructionElement = doorConstructionElements.at(i).toElement();
        boost::optional<model::ModelObject> doorConstruction = translateDoorConstruction(doorConstructionElement, doc, *result);
        OS_ASSERT(doorConstruction); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // fenestration constructions
      QDomNodeList fenestrationConstructionElements = projectElement.elementsByTagName("FenCons");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Fenestration Constructions"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(fenestrationConstructionElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < fenestrationConstructionElements.count(); i++){
        QDomElement fenestrationConstructionElement = fenestrationConstructionElements.at(i).toElement();
        boost::optional<model::ModelObject> fenestrationConstruction = translateFenestrationConstruction(fenestrationConstructionElement, doc, *result);
        OS_ASSERT(fenestrationConstruction); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      QDomNodeList crvDblQuadElements = projectElement.elementsByTagName("CrvDblQuad");
      for (int i = 0; i < crvDblQuadElements.count(); i++){
        QDomElement crvDblQuadElement = crvDblQuadElements.at(i).toElement();
        boost::optional<model::ModelObject> curve = translateCrvDblQuad(crvDblQuadElement, doc, *result);
        OS_ASSERT(curve);
      }

      QDomNodeList crvCubicElements = projectElement.elementsByTagName("CrvCubic");
      for (int i = 0; i < crvCubicElements.count(); i++){
        QDomElement crvCubicElement = crvCubicElements.at(i).toElement();
        boost::optional<model::ModelObject> curve = translateCrvCubic(crvCubicElement, doc, *result);
        OS_ASSERT(curve);
      }

      QDomNodeList crvQuadElements = projectElement.elementsByTagName("CrvQuad");
      for (int i = 0; i < crvQuadElements.count(); i++){
        QDomElement crvQuadElement = crvQuadElements.at(i).toElement();
        boost::optional<model::ModelObject> curve = translateCrvQuad(crvQuadElement, doc, *result);
        OS_ASSERT(curve);
      }

      // do schedules before loads
      QDomNodeList scheduleDayElements = projectElement.elementsByTagName("SchDay");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Day Schedules"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(scheduleDayElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < scheduleDayElements.count(); i++){
        QDomElement scheduleDayElement = scheduleDayElements.at(i).toElement();
        boost::optional<model::ModelObject> scheduleDay = translateScheduleDay(scheduleDayElement, doc, *result);
        OS_ASSERT(scheduleDay); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      QDomNodeList scheduleWeekElements = projectElement.elementsByTagName("SchWeek");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Week Schedules"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(scheduleWeekElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < scheduleWeekElements.count(); i++){
        QDomElement scheduleWeekElement = scheduleWeekElements.at(i).toElement();
        boost::optional<model::ModelObject> scheduleWeek = translateScheduleWeek(scheduleWeekElement, doc, *result);
        OS_ASSERT(scheduleWeek); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      QDomNodeList scheduleElements = projectElement.elementsByTagName("Sch");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Year Schedules"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(scheduleElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < scheduleElements.count(); i++){
        QDomElement scheduleElement = scheduleElements.at(i).toElement();
        boost::optional<model::ModelObject> schedule = translateSchedule(scheduleElement, doc, *result);
        OS_ASSERT(schedule); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      QDomNodeList holidayElements = projectElement.elementsByTagName("Hol");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Holidays"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(holidayElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < holidayElements.count(); i++){
        QDomElement holidayElement = holidayElements.at(i).toElement();
        boost::optional<model::ModelObject> holiday = translateHoliday(holidayElement, doc, *result);
        OS_ASSERT(holiday); // what type of error handling do we want?

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // FluidSys
      QDomNodeList fluidSysElements = projectElement.elementsByTagName("FluidSys");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Fluid Systems"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(fluidSysElements.count()); 
        m_progressBar->setValue(0);
      }

      // Translate condenser systems
      for (int i = 0; i < fluidSysElements.count(); i++){
        if (fluidSysElements.at(i).firstChildElement("Name").isNull()){
          continue;
        }
        if (fluidSysElements.at(i).firstChildElement("Type").text().toLower() != "condenserwater"){
          continue;
        }

        QDomElement fluidSysElement = fluidSysElements.at(i).toElement();
        boost::optional<model::ModelObject> plantLoop = translateFluidSys(fluidSysElement,doc,*result);
        OS_ASSERT(plantLoop);

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // Translate chilled and hot water systems
      for (int i = 0; i < fluidSysElements.count(); i++){
        if (fluidSysElements.at(i).firstChildElement("Name").isNull()){
          continue;
        }
        if (fluidSysElements.at(i).firstChildElement("Type").text().toLower() == "servicehotwater"){
          continue;
        }
        if (fluidSysElements.at(i).firstChildElement("Type").text().toLower() == "condenserwater"){
          continue;
        }

        QDomElement fluidSysElement = fluidSysElements.at(i).toElement();
        boost::optional<model::ModelObject> plantLoop = translateFluidSys(fluidSysElement,doc,*result);
        OS_ASSERT(plantLoop);

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // translate the building
      QDomElement buildingElement = projectElement.firstChildElement("Bldg");
      OS_ASSERT(!buildingElement.isNull()); // what type of error handling do we want?

      openstudio::model::Facility facility = result->getUniqueModelObject<openstudio::model::Facility>();

      boost::optional<model::ModelObject> building = translateBuilding(buildingElement, doc, *result);
      OS_ASSERT(building); // what type of error handling do we want?

      result->setFastNaming(false);

      // AirSystem
      QDomNodeList airSystemElements = buildingElement.elementsByTagName("AirSys");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Air Systems"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(airSystemElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < airSystemElements.count(); i++){
        if (airSystemElements.at(i).firstChildElement("Name").isNull()){
          continue;
        }

        QDomElement airSystemElement = airSystemElements.at(i).toElement();
        boost::optional<model::ModelObject> airLoopHVAC = translateAirSystem(airSystemElement,doc,*result);
        OS_ASSERT(airLoopHVAC);

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // ThermalZone
      QDomNodeList thermalZoneElements = buildingElement.elementsByTagName("ThrmlZn");
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(airSystemElements.count()); 
        m_progressBar->setValue(0);
      }

      for (int i = 0; i < thermalZoneElements.count(); i++){
        if (thermalZoneElements.at(i).firstChildElement("Name").isNull()){
          continue;
        }

        QDomElement thermalZoneElement = thermalZoneElements.at(i).toElement();
        boost::optional<model::ModelObject> thermalZone = translateThermalZone(thermalZoneElement,doc,*result);
        OS_ASSERT(thermalZone);

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }

      // request 15 min timestep
      model::Timestep timestep = result->getUniqueModelObject<model::Timestep>();
      timestep.setNumberOfTimestepsPerHour(4);

      // request output meters for TDV calculations
      std::set<int> fuelTypes = FuelType::getValues();
      BOOST_FOREACH(int fuelType, fuelTypes){

        if (fuelType == FuelType::Gasoline ||
            fuelType == FuelType::Diesel ||
            fuelType == FuelType::FuelOil_1 ||
            fuelType == FuelType::FuelOil_2 ||
            fuelType == FuelType::Propane ||
            fuelType == FuelType::Water ||
            fuelType == FuelType::Steam ||
            fuelType == FuelType::EnergyTransfer){
          // skip these to avoid E+ warning, EnergyTransfer is internal to the simulation
          continue;
        }

        // overall meter for this fuel type
        model::Meter meter(*result);
        meter.setFuelType(FuelType(fuelType));
        meter.setInstallLocationType(InstallLocationType::Facility);
        meter.setReportingFrequency("Hourly");

        std::set<int> endUseTypes = EndUseType::getValues();
        BOOST_FOREACH(int endUseType, endUseTypes){

          if (endUseType == EndUseType::HeatingCoils ||
              endUseType == EndUseType::CoolingCoils ||
              endUseType == EndUseType::Boilers ||
              endUseType == EndUseType::Baseboard ||
              endUseType == EndUseType::HeatRecoveryForCooling ||
              endUseType == EndUseType::HeatRecoveryForHeating){
            // ignore energy transfer meters
            continue;
          }

          // meter for this fuel type and end use
          // DLM: many of these will not be applicable and will cause E+ warnings
          model::Meter meter(*result);
          meter.setFuelType(FuelType(fuelType));
          meter.setEndUseType(EndUseType(endUseType));
          meter.setInstallLocationType(InstallLocationType::Facility);
          meter.setReportingFrequency("Hourly");
        }
      }

      // request specific meters
      // ElectricEquipment - Receptacle, Process, Refrig
      model::Meter meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::InteriorEquipment);
      meter.setSpecificEndUse("Receptacle");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::InteriorEquipment);
      meter.setSpecificEndUse("Process");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::InteriorEquipment);
      meter.setSpecificEndUse("Refrig");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::ExteriorEquipment);
      meter.setSpecificEndUse("Receptacle");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::ExteriorEquipment);
      meter.setSpecificEndUse("Process");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::ExteriorEquipment);
      meter.setSpecificEndUse("Refrig");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      // GasEquipment - Receptacle, Process
      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Gas);
      meter.setEndUseType(EndUseType::InteriorEquipment);
      meter.setSpecificEndUse("Receptacle");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Gas);
      meter.setEndUseType(EndUseType::InteriorEquipment);
      meter.setSpecificEndUse("Process");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Gas);
      meter.setEndUseType(EndUseType::ExteriorEquipment);
      meter.setSpecificEndUse("Receptacle");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Gas);
      meter.setEndUseType(EndUseType::ExteriorEquipment);
      meter.setSpecificEndUse("Process");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      // Lights - Reg Ltg, NonReg Ltg
      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::InteriorLights);
      meter.setSpecificEndUse("Reg Ltg");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::InteriorLights);
      meter.setSpecificEndUse("NonReg Ltg");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::ExteriorLights);
      meter.setSpecificEndUse("Reg Ltg");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      meter = model::Meter(*result);
      meter.setFuelType(FuelType::Electricity);
      meter.setEndUseType(EndUseType::ExteriorLights);
      meter.setSpecificEndUse("NonReg Ltg");
      meter.setInstallLocationType(InstallLocationType::Facility);
      meter.setReportingFrequency("Hourly");

      model::OutputControlReportingTolerances rt = result->getUniqueModelObject<model::OutputControlReportingTolerances>();
      rt.setToleranceforTimeCoolingSetpointNotMet(0.56);
      rt.setToleranceforTimeHeatingSetpointNotMet(0.56);
    }
    
    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateRunPeriod(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //<HVACAutoSizing>1</HVACAutoSizing>
    //<SimDsgnDays>0</SimDsgnDays>
    //<RunPeriodBeginMonth>1</RunPeriodBeginMonth>
    //<RunPeriodBeginDay>1</RunPeriodBeginDay>
    //<RunPeriodEndMonth>12</RunPeriodEndMonth>
    //<RunPeriodEndDay>31</RunPeriodEndDay>
    //<RunPeriodYear>1991</RunPeriodYear>

    boost::optional<openstudio::model::ModelObject> result;

    QDomElement hvacAutoSizingElement = element.firstChildElement("HVACAutoSizing");
    QDomElement runDesignDaysElement = element.firstChildElement("SimDsgnDays");
    QDomElement beginMonthElement = element.firstChildElement("RunPeriodBeginMonth");
    QDomElement beginDayElement = element.firstChildElement("RunPeriodBeginDay");
    QDomElement endMonthElement = element.firstChildElement("RunPeriodEndMonth");
    QDomElement endDayElement = element.firstChildElement("RunPeriodEndDay");
    QDomElement yearElement = element.firstChildElement("RunPeriodYear");

    bool test = true;

    if (hvacAutoSizingElement.isNull()){
      test = false;
      LOG(Error, "HVACAutoSizing not specified");
    }
    if (runDesignDaysElement.isNull()){
      test = false;
      LOG(Error, "SimDsgnDays not specified");
    }
    if (beginMonthElement.isNull()){
      test = false;
      LOG(Error, "RunPeriodBeginMonth not specified");
    }
    if (beginDayElement.isNull()){
      test = false;
      LOG(Error, "RunPeriodBeginDay not specified");
    }
    if (endMonthElement.isNull()){
      test = false;
      LOG(Error, "RunPeriodEndMonth not specified");
    }
    if (endDayElement.isNull()){
      test = false;
      LOG(Error, "RunPeriodEndDay not specified");
    }
    if (yearElement.isNull()){
      test = false;
      LOG(Error, "RunPeriodYear not specified");
    }

    if (!test){
      return boost::none;
    }

    model::SimulationControl simulationControl = model.getUniqueModelObject<model::SimulationControl>();
    
    //if ((hvacAutoSizingElement.text().toInt() == 0) && (runDesignDaysElement.text().toInt() == 0)){
    //  simulationControl.setRunSimulationforSizingPeriods(false);
    //}else{
    //  simulationControl.setRunSimulationforSizingPeriods(true);
    //}
    
    // We will probably never have completely autosized models so at least for now we need to ask for sizing runs.
    simulationControl.setRunSimulationforSizingPeriods(true);

    if (beginMonthElement.text().toInt() == 0){
      simulationControl.setRunSimulationforWeatherFileRunPeriods(false);

      // remove any existing run periods
      boost::optional<model::RunPeriod> runPeriod = model.getOptionalUniqueModelObject<model::RunPeriod>();
      if (runPeriod){
        runPeriod->remove();
      }

    }else{
      simulationControl.setRunSimulationforWeatherFileRunPeriods(true);

      model::YearDescription yearDescription = model.getUniqueModelObject<model::YearDescription>();
      yearDescription.setCalendarYear(yearElement.text().toInt());

      model::RunPeriod runPeriod = model.getUniqueModelObject<model::RunPeriod>();
      runPeriod.setBeginMonth(beginMonthElement.text().toInt());
      runPeriod.setBeginDayOfMonth(beginDayElement.text().toInt());
      runPeriod.setEndMonth(endMonthElement.text().toInt());
      runPeriod.setEndDayOfMonth(endDayElement.text().toInt());
      runPeriod.setUseWeatherFileHolidays(false);
      runPeriod.setUseWeatherFileDaylightSavings(false);
      runPeriod.setApplyWeekendHolidayRule(true);
      result = runPeriod;
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSite(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //<CliZn>ClimateZone12</CliZn>
    //<Lat>38.58</Lat>
    //<Long>-121.49</Long>
    //<Elevation>839.9</Elevation>

    QDomElement climateZoneElement = element.firstChildElement("CliZn");
    QDomElement latElement = element.firstChildElement("Lat");
    QDomElement longElement = element.firstChildElement("Long");
    QDomElement elevationElement = element.firstChildElement("Elevation");

    bool test = true;

    if (climateZoneElement.isNull()){
      //test = false;
      LOG(Error, "No climate zone specified");
    }
    if (latElement.isNull()){
      test = false;
      LOG(Error, "No lattitude specified");
    }
    if (longElement.isNull()){
      test = false;
      LOG(Error, "No longitude specified");
    }
    if (elevationElement.isNull()){
      test = false;
      LOG(Error, "No elevation specified");
    }

    if (!test){
      return boost::none;
    }

    // set lat, lon, elev
    model::Site site = model.getUniqueModelObject<model::Site>();

    // DLM: what about time zone and terrain?

    double latitude = latElement.text().toDouble();
    double longitude = longElement.text().toDouble();
    double elevationIP = elevationElement.text().toDouble();
    double elevationSI = 0.3048*elevationIP;

    site.setLatitude(latitude);
    site.setLongitude(longitude);
    site.setElevation(elevationSI);

    // DLM: always assume California?
    site.setTimeZone(-8.0);

    return site;
  }

  std::vector<WorkspaceObject> ReverseTranslator::translateDesignDays(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    std::vector<WorkspaceObject> result;

    //<DDWeatherFile>F:/360Local/AEC/CBECCn/CBECC Source Files/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.ddy</DDWeatherFile>

    QDomElement ddWeatherFileElement = element.firstChildElement("DDWeatherFile");

    if (ddWeatherFileElement.isNull()){
      LOG(Error, "No design day file specified");
      return result;
    }

    openstudio::path ddyFilePath = toPath(ddWeatherFileElement.text());
    if (!ddyFilePath.is_complete()){
      ddyFilePath = complete(ddyFilePath, m_path.parent_path());
    }

    if (!exists(ddyFilePath)){
      LOG(Error, "Design day file '" << toString(ddyFilePath) << "' does not exist");
      return result;
    }

    boost::optional<Workspace> ddyFile = Workspace::load(ddyFilePath);
    if (!ddyFile){
      LOG(Error, "Could not open design day file '" << toString(ddyFilePath) << "'");
      return result;
    }

    // convert ddy file to osm and add objects
    energyplus::ReverseTranslator rt;
    model::Model ddyModel = rt.translateWorkspace(*ddyFile);
    result = model.addObjects(ddyModel.objects());
    if (result.empty()){
      LOG(Error, "Failed to add design days to model");
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWeatherFile(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //<AnnualWeatherFile>F:/360Local/AEC/CBECCn/CBECC Source Files/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.epw</AnnualWeatherFile>

    QDomElement annualWeatherFileElement = element.firstChildElement("AnnualWeatherFile");

    if (annualWeatherFileElement.isNull()){
      LOG(Error, "No annual weather file specified");
      return boost::none;
    }

    openstudio::path epwFilePath = toPath(annualWeatherFileElement.text());
    if (!epwFilePath.is_complete()){
      epwFilePath = complete(epwFilePath, m_path.parent_path());
    }

    if (!exists(epwFilePath)){
      LOG(Error, "Annual weather file '" << toString(epwFilePath) << "' does not exist");
      return boost::none;
    }

    boost::optional<EpwFile> epwFile;
    try{
      epwFile = EpwFile(epwFilePath);
      OS_ASSERT(epwFile);
    }catch(std::exception&){
      LOG(Error, "Could not open epw file '" << toString(epwFilePath) << "'");
    }

    if (!epwFile){
      return boost::none;
    }

    // set weatherfile
    boost::optional<model::WeatherFile> weatherFile = model::WeatherFile::setWeatherFile(model, *epwFile);
    if (!weatherFile){
      LOG(Error, "Failed to set weather file for model");
      return boost::none;
    }

    return weatherFile.get();
  }

boost::optional<model::PlantLoop> ReverseTranslator::loopForSupplySegment(const QString & fluidSegmentName, const QDomDocument& doc, openstudio::model::Model& model)
{
  boost::optional<model::PlantLoop> result;

  QDomNodeList fluidSysElements = doc.documentElement().firstChildElement("Proj").elementsByTagName("FluidSys");

  for (int i = 0; i < fluidSysElements.count(); i++)
  {
    QDomElement fluidSysElement = fluidSysElements.at(i).toElement();

    QDomElement fluidSysNameElement = fluidSysElement.firstChildElement("Name");

    QDomNodeList fluidSegmentElements = fluidSysElement.elementsByTagName("FluidSeg");

    for (int i = 0; i < fluidSegmentElements.count(); i++)
    {
      QDomElement fluidSegmentElement = fluidSegmentElements.at(i).toElement();

      QDomElement nameElement = fluidSegmentElement.firstChildElement("Name");
      QDomElement typeElement = fluidSegmentElement.firstChildElement("Type");

      if( (typeElement.text().toLower() == "secondarysupply" ||
           typeElement.text().toLower() == "primarysupply" ) &&
          nameElement.text().toLower() == fluidSegmentName.toLower()
        )
      {
        boost::optional<model::PlantLoop> loop = model.getModelObjectByName<model::PlantLoop>(fluidSysNameElement.text().toStdString());

        return loop; 
      }
    }
  }

  return result;
}

boost::optional<model::PlantLoop> ReverseTranslator::serviceHotWaterLoopForSupplySegment(const QString & fluidSegmentName, const QDomDocument & doc, openstudio::model::Model& model)
{
  boost::optional<model::PlantLoop> result;

  QDomNodeList fluidSysElements = doc.documentElement().firstChildElement("Proj").elementsByTagName("FluidSys");

  for (int i = 0; i < fluidSysElements.count(); i++)
  {
    QDomElement fluidSysElement = fluidSysElements.at(i).toElement();

    QDomElement fluidSysNameElement = fluidSysElement.firstChildElement("Name");

    QDomElement fluidSysTypeElement = fluidSysElement.firstChildElement("Type");

    if( fluidSysTypeElement.text().toLower() == "servicehotwater" )
    {
      QDomNodeList fluidSegmentElements = fluidSysElement.elementsByTagName("FluidSeg");

      for (int i = 0; i < fluidSegmentElements.count(); i++)
      {
        QDomElement fluidSegmentElement = fluidSegmentElements.at(i).toElement();

        QDomElement nameElement = fluidSegmentElement.firstChildElement("Name");
        QDomElement typeElement = fluidSegmentElement.firstChildElement("Type");

        if( (typeElement.text().toLower() == "secondarysupply" ||
             typeElement.text().toLower() == "primarysupply" ) &&
            nameElement.text().toLower() == fluidSegmentName.toLower()
          )
        {
          if( boost::optional<model::PlantLoop> loop = model.getModelObjectByName<model::PlantLoop>(fluidSysNameElement.text().toStdString()) )
          {
            return loop; 
          }
          else
          {
            if( boost::optional<model::ModelObject> mo = translateFluidSys(fluidSysElement,doc,model) )
            {
              return mo->optionalCast<model::PlantLoop>();
            }
          }
        }
      }
    }
  }

  return result;
}

//TODO probably should be in OS proper
//helper method to do unit conversions;
boost::optional<double> ReverseTranslator::unitToUnit(const double& val, const std::string& fstUnitString, const std::string& secUnitString)
{
  //create the units from the strings
  boost::optional<Unit> fstUnit = UnitFactory::instance().createUnit(fstUnitString);
  boost::optional<Unit> secUnit = UnitFactory::instance().createUnit(secUnitString);

  //make sure both unit strings were valid
  if (fstUnit && secUnit) {

    //make the IP quantity
    Quantity ipQuant = Quantity(val, *fstUnit);

    //convert the IP quantity from IP to SI
    boost::optional<Quantity> siQuant = QuantityConverter::instance().convert(ipQuant, *secUnit);
  
    //if the conversion 
    if (siQuant) {
      return siQuant->value();
    }
  }

  return boost::none;
}


bool ReverseTranslator::autosize() const
{
  if( m_masterAutosize )
  {
    return true;
  }

  return m_autosize;
}

} // sdd
} // openstudio
