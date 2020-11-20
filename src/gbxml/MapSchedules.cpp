/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ReverseTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/AdditionalProperties.hpp"
#include "../model/AdditionalProperties_Impl.hpp"
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
#include "../utilities/core/StringHelpers.hpp"

#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>

#include <pugixml.hpp>

namespace openstudio {
namespace gbxml {

  openstudio::model::ScheduleTypeLimits getScheduleTypeLimits(const std::string& type, openstudio::model::Model& model) {
    boost::optional<openstudio::model::ScheduleTypeLimits> result = model.getModelObjectByName<openstudio::model::ScheduleTypeLimits>(type);

    if (result) {
      return *result;
    }

    result = openstudio::model::ScheduleTypeLimits(model);
    result->setName(type);

    if (type == "Temp") {
      result->setString(OS_ScheduleTypeLimitsFields::UnitType, "Temperature");
    } else if (type == "Fraction") {
      result->setString(OS_ScheduleTypeLimitsFields::UnitType, "Dimensionless");
    } else if (type == "OnOff") {
      result->setString(OS_ScheduleTypeLimitsFields::UnitType, "Control");
    }

    return *result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleDay(const pugi::xml_node& element,
                                                                                          openstudio::model::Model& model) {
    std::string id = element.attribute("id").value();
    std::string type = element.attribute("type").value();

    openstudio::model::ScheduleDay result(model);
    m_idToObjectMap.insert(std::make_pair(id, result));
    result.additionalProperties().setFeature("gbXMLId", id);

    std::string name = element.child("Name").text().as_string();
    result.setName(escapeName(id, name));

    openstudio::model::ScheduleTypeLimits scheduleTypeLimits = getScheduleTypeLimits(type, model);
    result.setScheduleTypeLimits(scheduleTypeLimits);

    auto valueElements = element.children("ScheduleValue");
    auto N{std::distance(valueElements.begin(), valueElements.end())};
    openstudio::Time dt(1.0 / ((double)N));

    auto valEl = *valueElements.begin();
    for (int i = 0; i < N; i++) {
      double value = valEl.text().as_double();
      result.addValue(dt * (i + 1), value);
      valEl = valEl.next_sibling("ScheduleValue");
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleWeek(const pugi::xml_node& element, const pugi::xml_node& root,
                                                                                           openstudio::model::Model& model) {
    std::string id = element.attribute("id").value();
    std::string type = element.attribute("type").value();

    openstudio::model::ScheduleWeek result(model);
    m_idToObjectMap.insert(std::make_pair(id, result));
    result.additionalProperties().setFeature("gbXMLId", id);

    std::string name = element.child("Name").text().as_string();
    result.setName(escapeName(id, name));

    // don't need to translate type

    for (auto& dayElement : element.children("Day")) {

      std::string dayType = dayElement.attribute("dayType").value();
      std::string dayScheduleIdRef = dayElement.attribute("dayScheduleIdRef").value();

      // this can be made more efficient using QXPath in QXmlPatterns later
      for (auto& dayScheduleElement : root.children("DaySchedule")) {
        std::string thisId = dayScheduleElement.attribute("id").value();
        if (thisId == dayScheduleIdRef) {

          boost::optional<openstudio::model::ModelObject> modelObject = translateScheduleDay(dayScheduleElement, model);
          if (modelObject) {

            boost::optional<openstudio::model::ScheduleDay> scheduleDay = modelObject->cast<openstudio::model::ScheduleDay>();
            if (scheduleDay) {

              if (dayType == "Weekday") {
                result.setWeekdaySchedule(*scheduleDay);
              } else if (dayType == "Weekend") {
                result.setWeekendSchedule(*scheduleDay);
              } else if (dayType == "Holiday") {
                result.setHolidaySchedule(*scheduleDay);
              } else if (dayType == "WeekendOrHoliday") {
                result.setWeekendSchedule(*scheduleDay);
                result.setHolidaySchedule(*scheduleDay);
              } else if (dayType == "HeatingDesignDay") {
                result.setWinterDesignDaySchedule(*scheduleDay);
              } else if (dayType == "CoolingDesignDay") {
                result.setSummerDesignDaySchedule(*scheduleDay);
              } else if (dayType == "Sun") {
                result.setSundaySchedule(*scheduleDay);
              } else if (dayType == "Mon") {
                result.setMondaySchedule(*scheduleDay);
              } else if (dayType == "Tue") {
                result.setTuesdaySchedule(*scheduleDay);
              } else if (dayType == "Wed") {
                result.setWednesdaySchedule(*scheduleDay);
              } else if (dayType == "Thu") {
                result.setThursdaySchedule(*scheduleDay);
              } else if (dayType == "Fri") {
                result.setFridaySchedule(*scheduleDay);
              } else if (dayType == "Sat") {
                result.setSaturdaySchedule(*scheduleDay);
              } else {
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

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSchedule(const pugi::xml_node& element, const pugi::xml_node& root,
                                                                                       openstudio::model::Model& model) {
    std::string id = element.attribute("id").value();
    std::string type = element.attribute("type").value();

    openstudio::model::ScheduleYear result(model);
    m_idToObjectMap.insert(std::make_pair(id, result));
    result.additionalProperties().setFeature("gbXMLId", id);

    std::string name = element.child("Name").text().as_string();
    result.setName(escapeName(id, name));

    openstudio::model::ScheduleTypeLimits scheduleTypeLimits = getScheduleTypeLimits(type, model);
    result.setScheduleTypeLimits(scheduleTypeLimits);

    openstudio::model::YearDescription yd = model.getUniqueModelObject<openstudio::model::YearDescription>();

    auto first{true};
    for (auto& scheduleYearElement : element.children("YearSchedule")) {
      std::string beginDateString = scheduleYearElement.child("BeginDate").text().as_string();
      auto beginDateParts = splitString(beginDateString, '-');  // 2011-01-01
      OS_ASSERT(beginDateParts.size() == 3);
      yd.setCalendarYear(std::stoi(beginDateParts.at(0)));
      openstudio::Date beginDate = yd.makeDate(std::stoi(beginDateParts.at(1)), std::stoi(beginDateParts.at(2)));

      // handle case if schedule does not start on 1/1
      if (first && (beginDate != yd.makeDate(1, 1))) {
        OS_ASSERT(false);
      }
      first = false;

      std::string endDateString = scheduleYearElement.child("EndDate").text().as_string();
      auto endDateParts = splitString(endDateString, '-');  // 2011-12-31
      OS_ASSERT(endDateParts.size() == 3);
      OS_ASSERT(yd.calendarYear());
      OS_ASSERT(yd.calendarYear().get() == std::stoi(endDateParts.at(0)));
      openstudio::Date endDate = yd.makeDate(std::stoi(endDateParts.at(1)), std::stoi(endDateParts.at(2)));

      std::string weekScheduleId = element.child("WeekScheduleId").attribute("weekScheduleIdRef").value();

      // this can be made more efficient using QXPath in QXmlPatterns later
      for (auto& scheduleWeekElement : root.children("WeekSchedule")) {
        std::string thisId = scheduleWeekElement.attribute("id").value();
        if (thisId == weekScheduleId) {

          boost::optional<openstudio::model::ModelObject> modelObject = translateScheduleWeek(scheduleWeekElement, root, model);
          if (modelObject) {

            boost::optional<openstudio::model::ScheduleWeek> scheduleWeek = modelObject->cast<openstudio::model::ScheduleWeek>();
            if (scheduleWeek) {
              result.addScheduleWeek(endDate, *scheduleWeek);
            }
          }

          break;
        }
      }
    }

    return result;
  }

}  // namespace gbxml
}  // namespace openstudio
