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

#include "ReverseTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleWeek.hpp"
#include "../model/ScheduleWeek_Impl.hpp"
#include "../model/ScheduleYear.hpp"
#include "../model/ScheduleYear_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"

#include "../utilities/time/Time.hpp"
#include "../utilities/time/Date.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QStringList>

namespace openstudio {
namespace gbxml {

  openstudio::model::ScheduleTypeLimits getScheduleTypeLimits(const std::string& type, openstudio::model::Model& model)
  {
    boost::optional<openstudio::model::ScheduleTypeLimits> result = model.getModelObjectByName<openstudio::model::ScheduleTypeLimits>(type);
    
    if (result){
      return *result;
    }

    result = openstudio::model::ScheduleTypeLimits(model);
    result->setName(type);

    if (type == "Temp"){
      result->setString(OS_ScheduleTypeLimitsFields::UnitType, "Temperature");
    }else if(type == "Fraction"){
      result->setString(OS_ScheduleTypeLimitsFields::UnitType, "Dimensionless");
    }else if(type == "OnOff"){
      result->setString(OS_ScheduleTypeLimitsFields::UnitType, "Control");
    }

    return *result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleDay(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QString id = element.attribute("id");
    QString type = element.attribute("type");

    openstudio::model::ScheduleDay result(model);
    result.setName(escapeName(id));

    openstudio::model::ScheduleTypeLimits scheduleTypeLimits = getScheduleTypeLimits(type.toStdString(), model);
    result.setScheduleTypeLimits(scheduleTypeLimits);

    QDomNodeList valueElements = element.elementsByTagName("ScheduleValue");
    openstudio::Time dt(1.0/((double) valueElements.size()));

    for (int i = 0; i < valueElements.count(); i++){
      double value = valueElements.at(i).toElement().text().toDouble();
      result.addValue( dt*(i+1) , value);
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleWeek(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QString id = element.attribute("id");
    QString type = element.attribute("type");

    openstudio::model::ScheduleWeek result(model);
    result.setName(escapeName(id));

    // don't need to translate type
    
    QDomNodeList dayElements = element.elementsByTagName("Day");
    for (int i = 0; i < dayElements.count(); i++){

      QString dayType = dayElements.at(i).toElement().attribute("dayType");
      QString dayScheduleIdRef = dayElements.at(i).toElement().attribute("dayScheduleIdRef");

      // this can be made more efficient using QXPath in QXmlPatterns later
      QDomNodeList dayScheduleElements = doc.documentElement().elementsByTagName("DaySchedule");
      for (int i = 0; i < dayScheduleElements.count(); i++){
        QDomElement dayScheduleElement = dayScheduleElements.at(i).toElement();
        QString thisId = dayScheduleElement.attribute("id");
        if (thisId == dayScheduleIdRef){

          boost::optional<openstudio::model::ModelObject> modelObject = translateScheduleDay(dayScheduleElement, doc, model);          
          if (modelObject){
            
            boost::optional<openstudio::model::ScheduleDay> scheduleDay = modelObject->cast<openstudio::model::ScheduleDay>();
            if (scheduleDay){
              
              if (dayType == "Weekday"){
                result.setWeekdaySchedule(*scheduleDay);
              }else if (dayType == "Weekend"){
                result.setWeekendSchedule(*scheduleDay);
              }else if (dayType == "Holiday"){
                result.setHolidaySchedule(*scheduleDay);
              }else if (dayType == "WeekendOrHoliday"){
                result.setWeekendSchedule(*scheduleDay);
                result.setHolidaySchedule(*scheduleDay);
              }else if (dayType == "HeatingDesignDay"){
                result.setWinterDesignDaySchedule(*scheduleDay);
              }else if (dayType == "CoolingDesignDay"){
                result.setSummerDesignDaySchedule(*scheduleDay);
              }else if (dayType == "Sun"){
                result.setSundaySchedule(*scheduleDay);
              }else if (dayType == "Mon"){
                result.setMondaySchedule(*scheduleDay);
              }else if (dayType == "Tue"){
                result.setTuesdaySchedule(*scheduleDay);
              }else if (dayType == "Wed"){
                result.setWednesdaySchedule(*scheduleDay);
              }else if (dayType == "Thu"){
                result.setThursdaySchedule(*scheduleDay);
              }else if (dayType == "Fri"){
                result.setFridaySchedule(*scheduleDay);
              }else if (dayType == "Sat"){
                result.setSaturdaySchedule(*scheduleDay);
              }else{
                // dayType can be "All"
                result.setAllSchedules(*scheduleDay);
              }
            }
          }

          break;
        }
      }
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSchedule(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QString id = element.attribute("id");
    QString type = element.attribute("type");
    QString name = element.elementsByTagName("Name").at(0).toElement().text();

    openstudio::model::ScheduleYear result(model);
    result.setName(escapeName(id));

    openstudio::model::ScheduleTypeLimits scheduleTypeLimits = getScheduleTypeLimits(type.toStdString(), model);
    result.setScheduleTypeLimits(scheduleTypeLimits);

    openstudio::model::YearDescription yd = model.getUniqueModelObject<openstudio::model::YearDescription>();

    QDomNodeList scheduleYearElements = element.elementsByTagName("YearSchedule");
    for (int i = 0; i < scheduleYearElements.count(); i++){
      QDomElement scheduleYearElement = scheduleYearElements.at(i).toElement();

      QString beginDateString = scheduleYearElement.elementsByTagName("BeginDate").at(0).toElement().text();
      QStringList beginDateParts = beginDateString.split('-'); // 2011-01-01
      OS_ASSERT(beginDateParts.size() == 3);
      yd.setCalendarYear(beginDateParts.at(0).toInt());
      openstudio::Date beginDate = yd.makeDate(beginDateParts.at(1).toInt(), beginDateParts.at(2).toInt());

      // handle case if schedule does not start on 1/1
      if ((i == 0) && (beginDate != yd.makeDate(1,1))){
        OS_ASSERT(false);
      }

      QString endDateString = scheduleYearElement.elementsByTagName("EndDate").at(0).toElement().text();
      QStringList endDateParts = endDateString.split('-'); // 2011-12-31
      OS_ASSERT(endDateParts.size() == 3);
      OS_ASSERT(yd.calendarYear());
      OS_ASSERT(yd.calendarYear().get() == endDateParts.at(0).toInt());
      openstudio::Date endDate = yd.makeDate(endDateParts.at(1).toInt(), endDateParts.at(2).toInt());
      
      QString weekScheduleId = element.elementsByTagName("WeekScheduleId").at(0).toElement().attribute("weekScheduleIdRef");

      // this can be made more efficient using QXPath in QXmlPatterns later
      QDomNodeList scheduleWeekElements = doc.documentElement().elementsByTagName("WeekSchedule");
      for (int i = 0; i < scheduleWeekElements.count(); i++){
        QDomElement scheduleWeekElement = scheduleWeekElements.at(i).toElement();
        QString thisId = scheduleWeekElement.attribute("id");
        if (thisId == weekScheduleId){

          boost::optional<openstudio::model::ModelObject> modelObject = translateScheduleWeek(scheduleWeekElement, doc, model);          
          if (modelObject){
            
            boost::optional<openstudio::model::ScheduleWeek> scheduleWeek = modelObject->cast<openstudio::model::ScheduleWeek>();
            if (scheduleWeek){
              result.addScheduleWeek(endDate, *scheduleWeek);
            }
          }

          break;
        }
      }
    }

    return result;
  }

} // gbxml
} // openstudio
