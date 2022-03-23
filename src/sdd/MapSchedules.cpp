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
#include "Helpers.hpp"

#include "../model/ExternalFile.hpp"
#include "../model/ExternalFile_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleFile.hpp"
#include "../model/ScheduleFile_Impl.hpp"
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

#include <pugixml.hpp>

namespace openstudio {
namespace sdd {

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleDay(const pugi::xml_node& element,
                                                                                          openstudio::model::Model& model) {
    pugi::xml_node nameElement = element.child("Name");
    pugi::xml_node typeElement = element.child("Type");

    std::string name;
    if (!nameElement) {
      LOG(Error, "SchDay element 'Name' is empty.")
    } else {
      name = escapeName(nameElement.text().as_string());
    }

    if (!typeElement) {
      LOG(Error, "SchDay element 'Type' is empty for SchDay named '" << name << "'.  ScheduleDay will not be created");
      return boost::none;
    }
    std::string type = escapeName(typeElement.text().as_string());

    std::vector<pugi::xml_node> hrElements = makeVectorOfChildren(element, "Hr");

    if (hrElements.size() != 24) {
      LOG(Error, "SchDay does not have 24 'Hr' elements empty for SchDay named '" << name << "'.  ScheduleDay will not be created");
      return boost::none;
    }

    model::ScheduleDay scheduleDay(model);
    scheduleDay.setName(name);

    boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>(type);
    bool isTemperature = false;
    if (type == "Temperature") {
      isTemperature = true;
    }

    if (!scheduleTypeLimits) {
      if (type == "Fraction") {
        scheduleTypeLimits = model::ScheduleTypeLimits(model);
        scheduleTypeLimits->setName("Fraction");
        scheduleTypeLimits->setLowerLimitValue(0);
        scheduleTypeLimits->setUpperLimitValue(1);
        scheduleTypeLimits->setNumericType("Continuous");
        scheduleTypeLimits->setUnitType("Dimensionless");
      } else if (type == "Temperature") {
        scheduleTypeLimits = model::ScheduleTypeLimits(model);
        scheduleTypeLimits->setName("Temperature");
        scheduleTypeLimits->setUnitType("Temperature");
      } else if (type == "OnOff") {
        scheduleTypeLimits = model::ScheduleTypeLimits(model);
        scheduleTypeLimits->setName("OnOff");
        scheduleTypeLimits->setLowerLimitValue(0);
        scheduleTypeLimits->setUpperLimitValue(1);
        scheduleTypeLimits->setNumericType("Discrete");
        scheduleTypeLimits->setUnitType("Availability");
      } else {
        LOG(Error, "Unknown schedule type '" << type << "'");
      }
    }
    if (scheduleTypeLimits) {
      scheduleDay.setScheduleTypeLimits(*scheduleTypeLimits);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < hrElements.size(); ++i) {
      pugi::xml_node hrElement = hrElements[i];
      double value = hrElement.text().as_double();

      if (isTemperature) {
        value = (value - 32.0) / 1.8;  // deg F to deg C
      }

      scheduleDay.addValue(openstudio::Time(0, i + 1, 0, 0), value);
    }

    return scheduleDay;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleWeek(const pugi::xml_node& element,
                                                                                           openstudio::model::Model& model) {
    pugi::xml_node nameElement = element.child("Name");
    pugi::xml_node typeElement = element.child("Type");
    pugi::xml_node schDaySunRefElement = element.child("SchDaySunRef");
    pugi::xml_node schDayMonRefElement = element.child("SchDayMonRef");
    pugi::xml_node schDayTueRefElement = element.child("SchDayTueRef");
    pugi::xml_node schDayWedRefElement = element.child("SchDayWedRef");
    pugi::xml_node schDayThuRefElement = element.child("SchDayThuRef");
    pugi::xml_node schDayFriRefElement = element.child("SchDayFriRef");
    pugi::xml_node schDaySatRefElement = element.child("SchDaySatRef");
    pugi::xml_node schDayHolRefElement = element.child("SchDayHolRef");
    pugi::xml_node schDayClgDDRefElement = element.child("SchDayClgDDRef");
    pugi::xml_node schDayHtgDDRefElement = element.child("SchDayHtgDDRef");

    std::string name;
    if (!nameElement) {
      LOG(Error, "SchWeek element 'Name' is empty.")
    } else {
      name = escapeName(nameElement.text().as_string());
    }

    if (!typeElement) {
      LOG(Error, "SchWeek element 'Type' is empty for SchWeek named '" << name << "'.  ScheduleWeek will not be created");
      return boost::none;
    }
    std::string type = escapeName(typeElement.text().as_string());

    model::ScheduleWeek scheduleWeek(model);
    scheduleWeek.setName(name);

    boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>(type);
    if (scheduleTypeLimits) {
      //scheduleWeek.setScheduleTypeLimits(*scheduleTypeLimits);
    }

    if (schDaySunRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDaySunRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setSundaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Sunday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayMonRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayMonRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setMondaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Monday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayTueRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayTueRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setTuesdaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Tuesday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayWedRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayWedRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setWednesdaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Wednesday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayThuRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayThuRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setThursdaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Thursday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayFriRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayFriRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setFridaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Friday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDaySatRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDaySatRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setSaturdaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Saturday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayHolRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayHolRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setHolidaySchedule(*scheduleDay);
        scheduleWeek.setCustomDay1Schedule(*scheduleDay);
        scheduleWeek.setCustomDay2Schedule(*scheduleDay);
      } else {
        LOG(Error, "No Holiday schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayClgDDRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayClgDDRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setSummerDesignDaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Summer Design Day schedule found for week schedule '" << name << "'");
      }
    }

    if (schDayHtgDDRefElement) {
      boost::optional<model::ScheduleDay> scheduleDay =
        model.getModelObjectByName<model::ScheduleDay>(escapeName(schDayHtgDDRefElement.text().as_string()));
      if (scheduleDay) {
        scheduleWeek.setWinterDesignDaySchedule(*scheduleDay);
      } else {
        LOG(Error, "No Winter Design Day schedule found for week schedule '" << name << "'");
      }
    }

    return scheduleWeek;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSchedule(const pugi::xml_node& element,
                                                                                       openstudio::model::Model& model) {
    boost::optional<openstudio::model::ModelObject> result;

    pugi::xml_node nameElement = element.child("Name");
    std::string name;
    if (!nameElement) {
      LOG(Error, "Sch element 'Name' is empty.")
    } else {
      name = escapeName(nameElement.text().as_string());
    }

    pugi::xml_node typeElement = element.child("Type");

    if (!typeElement) {
      LOG(Error, "Sch element 'Type' is empty for Sch named '" << name << "'.  ScheduleYear will not be created");
      return boost::none;
    }
    std::string type = escapeName(typeElement.text().as_string());

    if (istringEqual("File", type)) {
      result = translateScheduleFile(element, model);
    } else {
      result = translateScheduleYear(element, model);
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleFile(const pugi::xml_node& element,
                                                                                       openstudio::model::Model& model) {
    const auto name = escapeName(element.child("Name").text().as_string());

    const auto path = element.child("SchFileName").text().as_string();
    const auto externalFile = openstudio::model::ExternalFile::getExternalFile(model, path);
    if (! externalFile) {
      LOG(Error, "The external file path '" << path << "' could not be located");
      return boost::none;
    }
    openstudio::model::ScheduleFile schedule(externalFile.get());
    schedule.setName(name);

    const auto colIdx = element.child("ColIdx");
    if (colIdx) {
      schedule.setColumnNumber(colIdx.text().as_int());
    }

    const auto rowstoSkipatTop = element.child("RowsToSkipAtTop");
    if (rowstoSkipatTop) {
      schedule.setRowstoSkipatTop(rowstoSkipatTop.text().as_int());
    }

    const auto colSeparator = element.child("ColSeparator");
    if (colSeparator) {
      schedule.setColumnSeparator(colSeparator.text().as_string());
    }

    const auto interpToTimestep = element.child("InterpToTimestep");
    if (interpToTimestep) {
      schedule.setInterpolatetoTimestep(interpToTimestep.text().as_bool());
    }

    const auto minutesPerRow = element.child("MinutesPerRow");
    if (minutesPerRow) {
      schedule.setMinutesperItem(minutesPerRow.text().as_string());
    } else {
      schedule.setMinutesperItem("60");
    }

    return schedule;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateScheduleYear(const pugi::xml_node& element,
                                                                                       openstudio::model::Model& model) {
    const auto name = escapeName(element.child("Name").text().as_string());
    const auto type = escapeName(element.child("Type").text().as_string());

    std::vector<pugi::xml_node> endMonthElements = makeVectorOfChildren(element, "EndMonth");
    std::vector<pugi::xml_node> endDayElements = makeVectorOfChildren(element, "EndDay");
    std::vector<pugi::xml_node> schWeekRefElements = makeVectorOfChildren(element, "SchWeekRef");

    if (endMonthElements.size() != endDayElements.size()) {
      LOG(Error,
          "Number of 'EndMonth' elements not equal to number of 'EndDay' elements for Sch named '" << name << "'.  ScheduleYear will not be created");
      return boost::none;
    }

    if (endMonthElements.size() != schWeekRefElements.size()) {
      LOG(Error, "Number of 'EndMonth' elements not equal to number of 'SchWeekRef' elements for Sch named '"
                   << name << "'.  ScheduleYear will not be created");
      return boost::none;
    }

    model::ScheduleYear scheduleYear(model);
    scheduleYear.setName(name);

    boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>(type);
    if (scheduleTypeLimits) {
      scheduleYear.setScheduleTypeLimits(*scheduleTypeLimits);
    }

    for (std::vector<pugi::xml_node>::size_type i = 0; i < endMonthElements.size(); ++i) {
      pugi::xml_node endMonthElement = endMonthElements[i];
      pugi::xml_node endDayElement = endDayElements[i];
      pugi::xml_node schWeekRefElement = schWeekRefElements[i];

      boost::optional<model::ScheduleWeek> scheduleWeek =
        model.getModelObjectByName<model::ScheduleWeek>(escapeName(schWeekRefElement.text().as_string()));
      if (scheduleWeek) {

        boost::optional<model::YearDescription> yearDescription = model.getOptionalUniqueModelObject<model::YearDescription>();
        if (yearDescription) {
          MonthOfYear monthOfYear(endMonthElement.text().as_uint());
          unsigned dayOfMonth = endDayElement.text().as_uint();
          Date untilDate(monthOfYear, dayOfMonth, yearDescription->assumedYear());
          scheduleYear.addScheduleWeek(untilDate, *scheduleWeek);
        } else {
          MonthOfYear monthOfYear(endMonthElement.text().as_uint());
          unsigned dayOfMonth = endDayElement.text().as_uint();
          Date untilDate(monthOfYear, dayOfMonth);
          scheduleYear.addScheduleWeek(untilDate, *scheduleWeek);
        }
      }
    }

    return scheduleYear;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHoliday(const pugi::xml_node& element,
                                                                                      openstudio::model::Model& model) {
    //<Name>Thanksgiving Day</Name>
    //<SpecMthd>Fourth</SpecMthd>
    //<DayOfWeek>Thursday</DayOfWeek>
    //<Month>November</Month>

    //<Name>Christmas Day</Name>
    //<SpecMthd>Date</SpecMthd>
    //<Month>December</Month>
    //<Day>25</Day>

    boost::optional<openstudio::model::ModelObject> result;

    pugi::xml_node nameElement = element.child("Name");
    std::string name;
    if (!nameElement) {
      LOG(Error, "Hol element 'Name' is empty.")
    } else {
      name = escapeName(nameElement.text().as_string());
    }

    pugi::xml_node specificationMethodElement = element.child("SpecMthd");
    if (!specificationMethodElement) {
      LOG(Error, "Hol element 'SpecMthd' is empty for Hol named '" << name << "'.  Holiday will not be created");
      return boost::none;
    }

    if (openstudio::istringEqual(specificationMethodElement.text().as_string(), "Date")) {
      pugi::xml_node monthElement = element.child("Month");
      pugi::xml_node dayElement = element.child("Day");

      if (!monthElement) {
        LOG(Error, "Hol element 'Month' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      if (!dayElement) {
        LOG(Error, "Hol element 'Day' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      MonthOfYear monthOfYear(monthElement.text().as_string());
      unsigned day = dayElement.text().as_uint();

      result = model::RunPeriodControlSpecialDays(monthOfYear, day, model);
      result->setName(escapeName(nameElement.text().as_string()));

    } else {
      pugi::xml_node dayOfWeekElement = element.child("DayOfWeek");
      pugi::xml_node monthElement = element.child("Month");

      if (!dayOfWeekElement) {
        LOG(Error, "Hol element 'DayOfWeek' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      if (!monthElement) {
        LOG(Error, "Hol element 'Month' is empty for Hol named '" << name << "'.  Holiday will not be created");
        return boost::none;
      }

      // fifth is treated equivalently to last
      std::string specificationMethod = specificationMethodElement.text().as_string();
      if (specificationMethod == "Last") {
        specificationMethod = "Fifth";
      }

      NthDayOfWeekInMonth nth(specificationMethod);
      DayOfWeek dayOfWeek(dayOfWeekElement.text().as_string());
      MonthOfYear monthOfYear(monthElement.text().as_string());

      result = model::RunPeriodControlSpecialDays(nth, dayOfWeek, monthOfYear, model);
      result->setName(escapeName(nameElement.text().as_string()));
    }

    return result;
  }

}  // namespace sdd
}  // namespace openstudio
