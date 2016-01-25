/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ReverseTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleWeek.hpp"
#include "../model/ScheduleWeek_Impl.hpp"
#include "../model/ScheduleYear.hpp"
#include "../model/ScheduleYear_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"
#include "../model/RunPeriodControlSpecialDays.hpp"
#include "../model/RunPeriodControlSpecialDays_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"

#include "../utilities/time/Time.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/core/Assert.hpp"

#include <QDomElement>

namespace openstudio {
namespace sdd {

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleDay(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement typeElement = element.firstChildElement("Type");

    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "SchDay element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }

    if (typeElement.isNull()){
      LOG(Error, "SchDay element 'Type' is empty for SchDay named '" << name << "'.  ScheduleDay will not be created");
      return boost::none;
    }
    std::string type = escapeName(typeElement.text());

    QDomNodeList hrElements = element.elementsByTagName("Hr");
    if (hrElements.count() != 24){
      LOG(Error, "SchDay does not have 24 'Hr' elements empty for SchDay named '" << name << "'.  ScheduleDay will not be created");
      return boost::none;
    }

    model::ScheduleDay scheduleDay(model);
    scheduleDay.setName(name);
   
    boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>(type);
    bool isTemperature = false;
    if (type == "Temperature"){
      isTemperature = true;
    }

    if (!scheduleTypeLimits){
      if (type == "Fraction"){
        scheduleTypeLimits = model::ScheduleTypeLimits(model);
        scheduleTypeLimits->setName("Fraction");
        scheduleTypeLimits->setLowerLimitValue(0);
        scheduleTypeLimits->setUpperLimitValue(1);
        scheduleTypeLimits->setNumericType("Continuous");
        scheduleTypeLimits->setUnitType("Dimensionless");
      }else if (type == "Temperature"){
        scheduleTypeLimits = model::ScheduleTypeLimits(model);
        scheduleTypeLimits->setName("Temperature");
        scheduleTypeLimits->setUnitType("Temperature");
      }else if (type == "OnOff"){
        scheduleTypeLimits = model::ScheduleTypeLimits(model);
        scheduleTypeLimits->setName("OnOff");
        scheduleTypeLimits->setLowerLimitValue(0);
        scheduleTypeLimits->setUpperLimitValue(1);
        scheduleTypeLimits->setNumericType("Discrete");
        scheduleTypeLimits->setUnitType("Availability");
      }else{
        LOG(Error, "Unknown schedule type '" << type << "'");
      }
    }
    if (scheduleTypeLimits){
      scheduleDay.setScheduleTypeLimits(*scheduleTypeLimits);
    }

    for (int i = 0; i < hrElements.count(); i++){
      QDomElement hrElement = hrElements.at(i).toElement();
      double value = hrElement.text().toDouble();

      if (isTemperature){
        value = (value-32.0)/1.8; // deg F to deg C
      }

      scheduleDay.addValue(openstudio::Time(0, i+1, 0, 0), value);
    }

    return scheduleDay;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleWeek(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement typeElement = element.firstChildElement("Type");
    QDomElement schDaySunRefElement = element.firstChildElement("SchDaySunRef");
    QDomElement schDayMonRefElement = element.firstChildElement("SchDayMonRef");
    QDomElement schDayTueRefElement = element.firstChildElement("SchDayTueRef");
    QDomElement schDayWedRefElement = element.firstChildElement("SchDayWedRef");
    QDomElement schDayThuRefElement = element.firstChildElement("SchDayThuRef");
    QDomElement schDayFriRefElement = element.firstChildElement("SchDayFriRef");
    QDomElement schDaySatRefElement = element.firstChildElement("SchDaySatRef");
    QDomElement schDayHolRefElement = element.firstChildElement("SchDayHolRef");
    QDomElement schDayClgDDRefElement = element.firstChildElement("SchDayClgDDRef");
    QDomElement schDayHtgDDRefElement = element.firstChildElement("SchDayHtgDDRef");

    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "SchWeek element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }

    if (typeElement.isNull()){
      LOG(Error, "SchWeek element 'Type' is empty for SchWeek named '" << name << "'.  ScheduleWeek will not be created");
      return boost::none;
    }
    std::string type = escapeName(typeElement.text());

    model::ScheduleWeek scheduleWeek(model);
    scheduleWeek.setName(name);


    boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>(type);
    if (scheduleTypeLimits){
      //scheduleWeek.setScheduleTypeLimits(*scheduleTypeLimits);
    }

    if (!schDaySunRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDaySunRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setSundaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Sunday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayMonRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayMonRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setMondaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Monday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayTueRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayTueRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setTuesdaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Tuesday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayWedRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayWedRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setWednesdaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Wednesday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayThuRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayThuRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setThursdaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Thursday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayFriRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayFriRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setFridaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Friday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDaySatRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDaySatRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setSaturdaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Saturday schedule found for week schedule '" << name << "'");
      }
    }
   
    if (!schDayHolRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayHolRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setHolidaySchedule(*scheduleDay);
        scheduleWeek.setCustomDay1Schedule(*scheduleDay);
        scheduleWeek.setCustomDay2Schedule(*scheduleDay);
      }else{
        LOG(Error, "No Holiday schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayClgDDRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayClgDDRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setSummerDesignDaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Summer Design Day schedule found for week schedule '" << name << "'");
      }
    }

    if (!schDayHtgDDRefElement.isNull()){
      boost::optional<model::ScheduleDay> scheduleDay = model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayHtgDDRefElement.text()));
      if (scheduleDay){
        scheduleWeek.setWinterDesignDaySchedule(*scheduleDay);
      }else{
        LOG(Error, "No Winter Design Day schedule found for week schedule '" << name << "'");
      }
    }

    return scheduleWeek;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSchedule(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement typeElement = element.firstChildElement("Type");

    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "Sch element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }

    if (typeElement.isNull()){
      LOG(Error, "Sch element 'Type' is empty for Sch named '" << name << "'.  ScheduleYear will not be created");
      return boost::none;
    }
    std::string type = escapeName(typeElement.text());

    QDomNodeList endMonthElements = element.elementsByTagName("EndMonth");
    QDomNodeList endDayElements = element.elementsByTagName("EndDay");
    QDomNodeList schWeekRefElements = element.elementsByTagName("SchWeekRef");

    if (endMonthElements.count() != endDayElements.count()){
      LOG(Error, "Number of 'EndMonth' elements not equal to number of 'EndDay' elements for Sch named '" << name << "'.  ScheduleYear will not be created");
      return boost::none;
    }

    if (endMonthElements.count() != schWeekRefElements.count()){
      LOG(Error, "Number of 'EndMonth' elements not equal to number of 'SchWeekRef' elements for Sch named '" << name << "'.  ScheduleYear will not be created");
      return boost::none;
    }

    model::ScheduleYear scheduleYear(model);
    scheduleYear.setName(name);
    
    boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>(type);
    if (scheduleTypeLimits){
      scheduleYear.setScheduleTypeLimits(*scheduleTypeLimits);
    }

    for (int i = 0; i < endMonthElements.count(); i++){
      QDomElement endMonthElement = endMonthElements.at(i).toElement();
      QDomElement endDayElement = endDayElements.at(i).toElement();
      QDomElement schWeekRefElement = schWeekRefElements.at(i).toElement();

      boost::optional<model::ScheduleWeek> scheduleWeek = model.getModelObjectByName<model::ScheduleWeek>(escapeName(schWeekRefElement.text()));
      if (scheduleWeek){

        boost::optional<model::YearDescription> yearDescription = model.getOptionalUniqueModelObject<model::YearDescription>();
        if (yearDescription){
          MonthOfYear monthOfYear(endMonthElement.text().toUInt());
          unsigned dayOfMonth = endDayElement.text().toUInt();
          Date untilDate(monthOfYear, dayOfMonth, yearDescription->assumedYear());
          scheduleYear.addScheduleWeek(untilDate, *scheduleWeek);
        }else{
          MonthOfYear monthOfYear(endMonthElement.text().toUInt());
          unsigned dayOfMonth = endDayElement.text().toUInt();
          Date untilDate(monthOfYear, dayOfMonth);
          scheduleYear.addScheduleWeek(untilDate, *scheduleWeek);
        }
      }
    }

    return scheduleYear;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHoliday(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //<Name>Thanksgiving Day</Name>
    //<SpecMthd>Fourth</SpecMthd>
    //<DayOfWeek>Thursday</DayOfWeek>
    //<Month>November</Month>

    //<Name>Christmas Day</Name>
    //<SpecMthd>Date</SpecMthd>
    //<Month>December</Month>
    //<Day>25</Day>

    boost::optional<openstudio::model::ModelObject> result;

    QDomElement nameElement = element.firstChildElement("Name");
    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "Hol element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }

    QDomElement specificationMethodElement = element.firstChildElement("SpecMthd");
    if (specificationMethodElement.isNull()){
      LOG(Error, "Hol element 'SpecMthd' is empty for Hol named '" << name << "'.  Holiday will not be created");
      return boost::none;
    }

    if (specificationMethodElement.text() == "Date"){
      QDomElement monthElement = element.firstChildElement("Month");
      QDomElement dayElement = element.firstChildElement("Day");

      if (monthElement.isNull()){
        LOG(Error, "Hol element 'Month' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      if (dayElement.isNull()){
        LOG(Error, "Hol element 'Day' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      MonthOfYear monthOfYear(toString(monthElement.text()));
      unsigned day = dayElement.text().toUInt();

      result = model::RunPeriodControlSpecialDays(monthOfYear, day, model);
      result->setName(escapeName(nameElement.text()));

    }else{
      QDomElement dayOfWeekElement = element.firstChildElement("DayOfWeek");
      QDomElement monthElement = element.firstChildElement("Month");

      if (dayOfWeekElement.isNull()){
        LOG(Error, "Hol element 'DayOfWeek' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      if (monthElement.isNull()){
        LOG(Error, "Hol element 'Month' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      // fifth is treated equivalently to last
      std::string specificationMethod = toString(specificationMethodElement.text());
      if (specificationMethod == "Last"){
        specificationMethod = "Fifth";
      }

      NthDayOfWeekInMonth nth(specificationMethod);
      DayOfWeek dayOfWeek(toString(dayOfWeekElement.text()));
      MonthOfYear monthOfYear(toString(monthElement.text()));

      result = model::RunPeriodControlSpecialDays(nth, dayOfWeek, monthOfYear, model);
      result->setName(escapeName(nameElement.text()));
    }

    return result;
  }

} // sdd
} // openstudio
