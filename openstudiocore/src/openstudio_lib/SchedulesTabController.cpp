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

#include "SchedulesTabController.hpp"

#include "MainTabView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"
#include "ScheduleDialog.hpp"
#include "ScheduleSetsController.hpp"
#include "SchedulesTabView.hpp"
#include "SchedulesView.hpp"
#include "SubTabView.hpp"
#include "YearSettingsWidget.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RunPeriodControlDaylightSavingTime.hpp"
#include "../model/RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"

#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/core/Assert.hpp"

#include "../energyplus/ReverseTranslator.hpp"

#include <QApplication>
#include <QDateTime>
#include <QDialog>
#include <QFileDialog>
#include <QString>

namespace openstudio {

SchedulesTabController::SchedulesTabController(bool isIP, const model::Model & model)
  : MainTabController(new SchedulesTabView(model)),
    m_yearSettingsWidget(NULL),
    m_model(model),
    m_scheduleDialog(NULL),
    m_isIP(isIP)
{
  m_yearDescription = m_model.getUniqueModelObject<model::YearDescription>();

  m_yearSettingsWidget = new YearSettingsWidget(m_model);
  addQObject(m_yearSettingsWidget);
  this->mainContentWidget()->addSubTab("Year Settings",m_yearSettingsWidget,YEAR_SETTINGS);

  m_scheduleSetsController = std::shared_ptr<ScheduleSetsController>( new ScheduleSetsController(m_model) );
  this->mainContentWidget()->addSubTab("Schedule Sets",m_scheduleSetsController->subTabView(),SCHEDULE_SETS);

  m_schedulesView = new SchedulesView(m_isIP,m_model);
  addQObject(m_schedulesView);
  this->mainContentWidget()->addSubTab("Schedules",m_schedulesView,SCHEDULES);

  bool isConnected = false;

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        this,SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_schedulesView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_scheduleSetsController.get(), SIGNAL(downloadComponentsClicked()),
                        this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_scheduleSetsController.get(), SIGNAL(openLibDlgClicked()),
                        this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_schedulesView,SIGNAL(addScheduleClicked()),this,SLOT(addScheduleRuleset()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_schedulesView,SIGNAL(removeSelectedScheduleClicked()),this,SLOT(removeSelectedSchedule()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_schedulesView,SIGNAL(purgeUnusedScheduleRulesetsClicked()),this,SLOT(purgeUnusedScheduleRulesets()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_schedulesView,SIGNAL(addRuleClicked(model::ScheduleRuleset &)),this,SLOT(addRule(model::ScheduleRuleset &)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_schedulesView,SIGNAL(addSummerProfileClicked(model::ScheduleRuleset &)),this,SLOT(addSummerProfile(model::ScheduleRuleset &)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_schedulesView,SIGNAL(addWinterProfileClicked(model::ScheduleRuleset &)),this,SLOT(addWinterProfile(model::ScheduleRuleset &)));
  OS_ASSERT(isConnected);

  isConnected = connect( m_schedulesView,SIGNAL(dayScheduleSceneChanged( DayScheduleScene *, double, double )),
           this,SLOT(onDayScheduleSceneChanged( DayScheduleScene *, double, double )));
  OS_ASSERT(isConnected);

  isConnected = connect( m_schedulesView,SIGNAL(startDateTimeChanged(model::ScheduleRule &, const QDateTime &)),
           this,SLOT(onStartDateTimeChanged(model::ScheduleRule &, const QDateTime &)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_schedulesView,SIGNAL(endDateTimeChanged(model::ScheduleRule &, const QDateTime &)),
           this,SLOT(onEndDateTimeChanged(model::ScheduleRule &, const QDateTime &)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_schedulesView,SIGNAL(removeScheduleRuleClicked(model::ScheduleRule &)),
           this,SLOT(removeScheduleRule(model::ScheduleRule &)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_schedulesView,SIGNAL(itemDropped(const OSItemId &)),
           this,SLOT(onItemDropped(const OSItemId &)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(calendarYearSelected(int)),
           this,SLOT(setCalendarYear(int)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(firstDayofYearSelected(const QString &)),
           this,SLOT(setFirstDayofYear(const QString &)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(daylightSavingTimeClicked(bool)),
           this,SLOT(setDaylightSavingsTime(bool)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(dstStartDayOfWeekAndMonthChanged(int, int, int)),
           this,SLOT(setDstStartDayOfWeekAndMonth(int, int, int)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(dstStartDateChanged(const QDate &)),
           this,SLOT(setDstStartDate(const QDate &)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(dstEndDayOfWeekAndMonthChanged(int, int, int)),
           this,SLOT(setDstEndDayOfWeekAndMonth(int, int, int)) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_yearSettingsWidget,SIGNAL(dstEndDateChanged(const QDate &)),
           this,SLOT(setDstEndDate(const QDate &)) );
  OS_ASSERT(isConnected);
}

YearSettingsWidget * SchedulesTabController::yearSettingsWidget()
{
  // assert non-null pointer
  OS_ASSERT(m_yearSettingsWidget);

  return m_yearSettingsWidget;
}

void SchedulesTabController::addScheduleRuleset()
{
  showScheduleDialog();
}

void SchedulesTabController::showScheduleDialog()
{
  if(!m_scheduleDialog){
    m_scheduleDialog = new ScheduleDialog(m_isIP,m_model);

    bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                               m_scheduleDialog, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);
  }
  m_scheduleDialog->show();
}

void SchedulesTabController::removeSelectedSchedule()
{
  if( boost::optional<model::ScheduleRuleset> schedule = m_schedulesView->currentSchedule() )
  {
    schedule->remove();
  }
}

void SchedulesTabController::purgeUnusedScheduleRulesets()
{
  std::vector<model::ScheduleRuleset> schedules = m_model.getConcreteModelObjects<model::ScheduleRuleset>();

  for( std::vector<model::ScheduleRuleset>::iterator it = schedules.begin();
       it != schedules.end();
       ++it )
  {
    if( it->directUseCount() == 0 )
    {
      it->remove();
    }
  }
}

void SchedulesTabController::addRule(model::ScheduleRuleset & scheduleRuleset)
{
  model::ScheduleRule rule(scheduleRuleset);

  rule.daySchedule().addValue(Time(1,0),defaultStartingValue(rule.daySchedule()));
}

void SchedulesTabController::addSummerProfile(model::ScheduleRuleset & scheduleRuleset)
{
  model::ScheduleDay scheduleDay(scheduleRuleset.model());

  scheduleDay.addValue(Time(1,0),defaultStartingValue(scheduleDay));

  scheduleRuleset.setSummerDesignDaySchedule(scheduleDay);

  scheduleDay.remove();

  m_schedulesView->showSummerScheduleDay(scheduleRuleset);
}

void SchedulesTabController::addWinterProfile(model::ScheduleRuleset & scheduleRuleset)
{
  model::ScheduleDay scheduleDay(scheduleRuleset.model());

  scheduleDay.addValue(Time(1,0),defaultStartingValue(scheduleDay));

  scheduleRuleset.setWinterDesignDaySchedule(scheduleDay);

  scheduleDay.remove();

  m_schedulesView->showWinterScheduleDay(scheduleRuleset);
}

void SchedulesTabController::onDayScheduleSceneChanged( DayScheduleScene * scene, double lowerLimitValue, double upperLimitValue )
{
  std::vector<CalendarSegmentItem *> segments = scene->segments();

  model::ScheduleDay scheduleDay = scene->scheduleDay();
  bool isIP = scene->scheduleDayView()->isIP();
  OptionalUnit units;
  if (model::OptionalScheduleTypeLimits typeLimits = scheduleDay.scheduleTypeLimits()) {
    units = typeLimits->getLowerLimitValue(isIP).units();
  }

  scheduleDay.clearValues();

  for( std::vector<CalendarSegmentItem *>::iterator it = segments.begin();
       it < segments.end();
       ++it )
  {
    openstudio::Time time(0,0,0,(*it)->endTime());

    float scaledvalue = (*it)->value();

    float value = lowerLimitValue + scaledvalue * (upperLimitValue - lowerLimitValue);

    if (units) {
      scheduleDay.addValue(time,Quantity(value,*units)); // going to be slower than it has to be
                                                         // better to assemble all values into OSQuantityVector and then set
    }
    else {
      scheduleDay.addValue(time,value);
    }
  }
}

void SchedulesTabController::onStartDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate)
{
  openstudio::Date date(newDate.date().month(),newDate.date().day(),newDate.date().year());

  if( scheduleRule.startDate() )
  {
    if( date != scheduleRule.startDate().get() )
    {
      scheduleRule.setStartDate(date);
    }
  }
  else
  {
    scheduleRule.setStartDate(date);
  }
}

void SchedulesTabController::onEndDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate)
{
  openstudio::Date date(newDate.date().month(),newDate.date().day(),newDate.date().year());

  if( scheduleRule.endDate() )
  {
    if( date != scheduleRule.endDate().get() )
    {
      scheduleRule.setEndDate(date);
    }
  }
  else
  {
    scheduleRule.setEndDate(date);
  }
}

void SchedulesTabController::removeScheduleRule(model::ScheduleRule & scheduleRule)
{
  scheduleRule.getImpl<openstudio::model::detail::ScheduleRule_Impl>();

  scheduleRule.remove();
}

void SchedulesTabController::setCalendarYear(int year)
{
  m_yearDescription->setCalendarYear(year);
}

void SchedulesTabController::setFirstDayofYear(const QString & firstDayofYear)
{
  m_yearDescription->resetCalendarYear();

  m_yearDescription->setDayofWeekforStartDay(firstDayofYear.toStdString());
}

void SchedulesTabController::setDaylightSavingsTime(bool enabled)
{
  if( enabled )
  {
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();
  }
  else
  {
    if( boost::optional<model::RunPeriodControlDaylightSavingTime> dst =
          m_model.getOptionalUniqueModelObject<model::RunPeriodControlDaylightSavingTime>() )
    {
      dst->remove();
    }
  }
}

void SchedulesTabController::setDstStartDayOfWeekAndMonth(int newWeek, int newDay, int newMonth)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setStartDate(NthDayOfWeekInMonth(newWeek), DayOfWeek(newDay), MonthOfYear(newMonth));
}

void SchedulesTabController::setDstStartDate(const QDate & newdate)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setStartDate(monthOfYear(newdate.month()),newdate.day());
}

void SchedulesTabController::setDstEndDayOfWeekAndMonth(int newWeek, int newDay, int newMonth)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setEndDate(NthDayOfWeekInMonth(newWeek), DayOfWeek(newDay), MonthOfYear(newMonth));
}

void SchedulesTabController::setDstEndDate(const QDate & newdate)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setEndDate(monthOfYear(newdate.month()),newdate.day());
}

void SchedulesTabController::onItemDropped(const OSItemId& itemId)
{
  if( OSAppBase::instance()->currentDocument()->fromComponentLibrary(itemId) )
  {
    if( boost::optional<model::ModelObject> mo = OSAppBase::instance()->currentDocument()->getModelObject(itemId) )
    {
      if( boost::optional<model::ScheduleRuleset> s = mo->optionalCast<model::ScheduleRuleset>() )
      {
        model::Model m = OSAppBase::instance()->currentDocument()->model();

        s->clone(m);
      }
    }
  }
}

void SchedulesTabController::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

double SchedulesTabController::defaultStartingValue(const model::ScheduleDay& scheduleDay) {
  double result(0.3);
  if (model::OptionalScheduleTypeLimits typeLimits = scheduleDay.scheduleTypeLimits()) {
    OptionalUnit siU = model::ScheduleTypeLimits::units(typeLimits->unitType(),false);
    if (siU) {
      std::string unitType = typeLimits->unitType();
      boost::to_lower(unitType);
      char firstLetter = unitType[0];
      OptionalQuantity siQ;
      switch (firstLetter) {
      case 'a' :
        { 
          if (unitType == "activitylevel") {          
            siQ = Quantity(100.0,*siU);
          }
          else if (unitType == "angle") {
            siQ = Quantity(0.0,*siU);
          }
          else if (unitType == "availability") {
            siQ = Quantity(1.0,*siU);
          }
          break;
        }
      case 'c' :
        {
          if (unitType == "capacity") {
            siQ = Quantity(100.0,*siU);
          }
          else if (unitType == "clothinginsulation") {
            siQ = Quantity(0.5,*siU);
          }
          else if (unitType == "controlmode") {
            siQ = Quantity(0.0,*siU);
          }
          else if (unitType == "convectioncoefficient") {
            siQ = Quantity(10.0,*siU);
          }
          break;
        }
      case 'd' :
        {
          if (unitType == "deltatemperature") {
            siQ = Quantity(3.0,*siU);
          }
          break;
        }
      case 'm' :
        {
          if (unitType == "massflowrate") {
            siQ = Quantity(1.0,*siU);
          }
          break;
        }
      case 'p' :
        {
          if (unitType == "percent") {
            siQ = Quantity(50.0,*siU);
          }
          else if (unitType == "power") {
            siQ = Quantity(100.0,*siU);
          }
          else if (unitType == "precipitationrate") {
            siQ = Quantity(0.01,*siU);
          }
          else if (unitType == "pressure") {
            siQ = Quantity(100.0,*siU);
          }
          break;
        }
      case 'r' :
        {
          siQ = Quantity(1500.0,*siU);
          break;
        }
      case 's' :
        {
          if (unitType == "solarenergy") {
            siQ = Quantity(1000.0,*siU);
          }
          break;
        }
      case 't' :
        {
          if (unitType == "temperature") {
            siQ = Quantity(23.5,*siU);
          }
          break;
        }
      case 'v' :
        {
          if (unitType == "velocity") {
            siQ = Quantity(1.0,*siU);
          }
          if (unitType == "volumetricflowrate") {
            siQ = Quantity(1.0E-4,*siU);
          }
          break;
        }
      default :
        break;
      } // switch
      if (siQ) {
        result = siQ->value();
      }
    } // if siU
  }
  return result;
}

} // openstudio

