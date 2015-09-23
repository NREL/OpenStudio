/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "ScheduleDayView.hpp"
#include "SubTabView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/units/Unit.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/units/Quantity.hpp"

#include "../energyplus/ReverseTranslator.hpp"

#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QString>

namespace openstudio {

SchedulesTabController::SchedulesTabController(bool isIP, const model::Model & model)
  : MainTabController(new SchedulesTabView(model)),
    m_model(model),
    m_scheduleDialog(nullptr),
    m_isIP(isIP)
{
  m_scheduleSetsController = std::shared_ptr<ScheduleSetsController>( new ScheduleSetsController(m_model) );
  this->mainContentWidget()->addSubTab("Schedule Sets",m_scheduleSetsController->subTabView(),SCHEDULE_SETS);

  m_schedulesView = new SchedulesView(m_isIP,m_model);
  addQObject(m_schedulesView);
  this->mainContentWidget()->addSubTab("Schedules",m_schedulesView,SCHEDULES);

  connect(this, &SchedulesTabController::toggleUnitsClicked, this, &SchedulesTabController::toggleUnits);

  connect(this, &SchedulesTabController::toggleUnitsClicked, m_schedulesView, &SchedulesView::toggleUnitsClicked);

  connect(m_scheduleSetsController.get(), &ScheduleSetsController::downloadComponentsClicked, this, &SchedulesTabController::downloadComponentsClicked);

  connect(m_scheduleSetsController.get(), &ScheduleSetsController::openLibDlgClicked, this, &SchedulesTabController::openLibDlgClicked);

  connect(m_schedulesView, &SchedulesView::addScheduleClicked, this, &SchedulesTabController::addScheduleRuleset);
  
  connect(m_schedulesView, &SchedulesView::copySelectedScheduleClicked, this, &SchedulesTabController::copySelectedSchedule);

  connect(m_schedulesView, &SchedulesView::removeSelectedScheduleClicked, this, &SchedulesTabController::removeSelectedSchedule);

  connect(m_schedulesView, &SchedulesView::purgeUnusedScheduleRulesetsClicked, this, &SchedulesTabController::purgeUnusedScheduleRulesets);

  connect(m_schedulesView, &SchedulesView::addRuleClicked, this, &SchedulesTabController::addRule);

  connect(m_schedulesView, &SchedulesView::addSummerProfileClicked, this, &SchedulesTabController::addSummerProfile);

  connect(m_schedulesView, &SchedulesView::addWinterProfileClicked, this, &SchedulesTabController::addWinterProfile);

  connect(m_schedulesView, &SchedulesView::dayScheduleSceneChanged, this, &SchedulesTabController::onDayScheduleSceneChanged);

  connect(m_schedulesView, &SchedulesView::startDateTimeChanged, this, &SchedulesTabController::onStartDateTimeChanged);

  connect(m_schedulesView, &SchedulesView::endDateTimeChanged, this, &SchedulesTabController::onEndDateTimeChanged);

  connect(m_schedulesView, &SchedulesView::removeScheduleRuleClicked, this, &SchedulesTabController::removeScheduleRule);

  connect(m_schedulesView, &SchedulesView::itemDropped, this, &SchedulesTabController::onItemDropped);

  connect(m_schedulesView, &SchedulesView::modelObjectSelected, this, &SchedulesTabController::modelObjectSelected);
}

void SchedulesTabController::addScheduleRuleset()
{
  showScheduleDialog();
}

void SchedulesTabController::showScheduleDialog()
{
  if(!m_scheduleDialog){
    m_scheduleDialog = new ScheduleDialog(m_isIP,m_model);

    connect(this, &SchedulesTabController::toggleUnitsClicked, m_scheduleDialog, &ScheduleDialog::toggleUnitsClicked);
  }
  m_scheduleDialog->show();
}

void SchedulesTabController::copySelectedSchedule()
{
  if (boost::optional<model::ScheduleRuleset> schedule = m_schedulesView->currentSchedule())
  {
    schedule->clone();
  }
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

  for( auto it = schedules.begin();
       it != schedules.end();
       ++it )
  {
    if( it->directUseCount(true) == 0 )
    {
      it->remove();
    }
  }
}

void SchedulesTabController::addRule(model::ScheduleRuleset & scheduleRuleset, UUID scheduleDayHandle)
{
  boost::optional<model::ScheduleRule> rule;
  if (!scheduleDayHandle.isNull()){
    boost::optional<model::ScheduleDay> scheduleDayToCopy = scheduleRuleset.model().getModelObject<model::ScheduleDay>(scheduleDayHandle);
    if (scheduleDayToCopy){
      rule = model::ScheduleRule(scheduleRuleset, scheduleDayToCopy->clone().cast<model::ScheduleDay>());
    }
  }
  if (!rule){
    rule = model::ScheduleRule(scheduleRuleset);
    rule->daySchedule().addValue(Time(1, 0), defaultStartingValue(rule->daySchedule()));
  }
  OS_ASSERT(rule);
}

void SchedulesTabController::addSummerProfile(model::ScheduleRuleset & scheduleRuleset, UUID scheduleDayHandle)
{
  boost::optional<model::ScheduleDay> scheduleDay;
  if (!scheduleDayHandle.isNull()){
    boost::optional<model::ScheduleDay> scheduleDayToCopy = scheduleRuleset.model().getModelObject<model::ScheduleDay>(scheduleDayHandle);
    if (scheduleDayToCopy){
      scheduleDay = scheduleDayToCopy->clone().cast<model::ScheduleDay>();
    }
  }
  if (!scheduleDay){
    scheduleDay = model::ScheduleDay(scheduleRuleset.model());
    boost::optional<model::ScheduleTypeLimits> limits = scheduleRuleset.scheduleTypeLimits();
    if (limits) {
      scheduleDay->setScheduleTypeLimits(*limits);
    }
    scheduleDay->addValue(Time(1, 0), defaultStartingValue(*scheduleDay));
  }
  OS_ASSERT(scheduleDay);

  scheduleRuleset.setSummerDesignDaySchedule(*scheduleDay);

  scheduleDay->remove();

  m_schedulesView->showSummerScheduleDay(scheduleRuleset);
}

void SchedulesTabController::addWinterProfile(model::ScheduleRuleset & scheduleRuleset, UUID scheduleDayHandle)
{
  boost::optional<model::ScheduleDay> scheduleDay;
  if (!scheduleDayHandle.isNull()){
    boost::optional<model::ScheduleDay> scheduleDayToCopy = scheduleRuleset.model().getModelObject<model::ScheduleDay>(scheduleDayHandle);
    if (scheduleDayToCopy){
      scheduleDay = scheduleDayToCopy->clone().cast<model::ScheduleDay>();
    }
  }
  if (!scheduleDay){
    scheduleDay = model::ScheduleDay(scheduleRuleset.model());
    boost::optional<model::ScheduleTypeLimits> limits = scheduleRuleset.scheduleTypeLimits();
    if (limits) {
      scheduleDay->setScheduleTypeLimits(*limits);
    }
    scheduleDay->addValue(Time(1, 0), defaultStartingValue(*scheduleDay));
  }
  OS_ASSERT(scheduleDay);

  scheduleRuleset.setWinterDesignDaySchedule(*scheduleDay);

  scheduleDay->remove();

  m_schedulesView->showWinterScheduleDay(scheduleRuleset);
}

void SchedulesTabController::onDayScheduleSceneChanged( DayScheduleScene * scene, double lowerLimitValue, double upperLimitValue )
{
  std::vector<CalendarSegmentItem *> segments = scene->segments();

  model::ScheduleDay scheduleDay = scene->scheduleDay();

  OptionalUnit units = scene->scheduleDayView()->units();

  scheduleDay.clearValues();

  for( auto it = segments.begin();
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

  if (m_scheduleDialog){
    m_scheduleDialog->setIsIP(displayIP);
  }
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

