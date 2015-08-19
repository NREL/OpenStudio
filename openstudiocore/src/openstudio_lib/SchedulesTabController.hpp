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

#ifndef OPENSTUDIO_SCHEDULESTABCONTROLLER_HPP
#define OPENSTUDIO_SCHEDULESTABCONTROLLER_HPP

#include "MainTabController.hpp"

#include "../model/Model.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"

#include "../utilities/core/UUID.hpp"

#include <boost/smart_ptr.hpp>

#include <QObject>

namespace openstudio {

class OSItemId;

namespace model {

class ScheduleCompact;

}

class DayScheduleScene;

class MainTabView;

class ScheduleDialog;

class ScheduleSetsController;

class SchedulesView;

class SchedulesTabController : public MainTabController
{
  Q_OBJECT

  public:

  SchedulesTabController(bool isIP, const model::Model & model);

  virtual ~SchedulesTabController() {}

  enum TabID
  {
    YEAR_SETTINGS,
    SCHEDULE_SETS,
    SCHEDULES
  };

  static double defaultStartingValue(const model::ScheduleDay& scheduleDay);

  public slots:

  void toggleUnits(bool displayIP);

  private slots:

  void addScheduleRuleset();

  void copySelectedSchedule();

  void removeSelectedSchedule();

  void purgeUnusedScheduleRulesets();

  void addRule(model::ScheduleRuleset & scheduleRuleset, UUID scheduleDayHandle);

  void addSummerProfile(model::ScheduleRuleset & scheduleRuleset, UUID scheduleDayHandle);

  void addWinterProfile(model::ScheduleRuleset & scheduleRuleset, UUID scheduleDayHandle);

  void removeScheduleRule(model::ScheduleRule & scheduleRule);

  void onDayScheduleSceneChanged( DayScheduleScene * scene, double lowerLimitValue, double upperLimitValue );

  void onStartDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  void onEndDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  void onItemDropped(const OSItemId& itemId);

 private:

  void showScheduleDialog();

  SchedulesView * m_schedulesView;

  std::shared_ptr<ScheduleSetsController> m_scheduleSetsController;

  model::Model m_model;

  ScheduleDialog * m_scheduleDialog;

  bool m_isIP;
};

} // openstudio

#endif // OPENSTUDIO_SCHEDULESTABCONTROLLER_HPP

