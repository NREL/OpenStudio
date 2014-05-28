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

#ifndef OPENSTUDIO_YEARSETTINGSWIDGET_HPP
#define OPENSTUDIO_YEARSETTINGSWIDGET_HPP

#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include <QWidget>

class QRadioButton;

class QDateEdit;

class QDate;

namespace openstudio {

class OSComboBox;

class OSSwitch;

class YearSettingsWidget : public QWidget
{
  Q_OBJECT

  public:

  static const int FIRSTYEAR;

  static const int LASTYEAR;

  // A list of the weeks in the month.  1st, 2nd, 3rd, 4th, Last
  static std::vector<std::string> weeksInMonth();

  // A list of the days in the week.  Monday, Tuesday...
  static std::vector<std::string> daysOfWeek();

  // A list of the months.
  static std::vector<std::string> months();

  YearSettingsWidget(const model::Model & model, QWidget * parent = 0);

  virtual ~YearSettingsWidget() {}

  signals:

  void calendarYearSelected(int year);

  void firstDayofYearSelected(const QString & firstDayofYear);

  void daylightSavingTimeClicked(bool enabled);

  void dstStartDayOfWeekAndMonthChanged(int newWeek, int intDay, int newMonth);

  void dstStartDateChanged(const QDate & newdate);

  void dstEndDayOfWeekAndMonthChanged(int newWeek, int newDay, int newMonth);

  void dstEndDateChanged(const QDate & newdate);

  public slots:

  void scheduleRefresh();

  private slots:

  void refresh();

  void onCalendarYearChanged(int index);

  void onCalendarYearButtonClicked();

  void onFirstDayofYearClicked();

  void onWorkspaceObjectAdd(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo);

  void onWorkspaceObjectRemove(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo);

  void onDstStartDayWeekMonthChanged();

  //void onDstStartDateChanged();

  void onDstEndDayWeekMonthChanged();

  //void onDstEndDateChanged();

  void onDefineStartByDayWeekMonthClicked();

  void onDefineEndByDayWeekMonthClicked();

  void onDefineStartByDateClicked();

  void onDefineEndByDateClicked();

  //void emitNewDSTStartDate();

  //void emitNewDSTEndDate();

  private:

  // year selection section
  QRadioButton * m_calendarYearButton;

  OSComboBox * m_calendarYearEdit;

  QRadioButton * m_firstDayOfYearButton;

  OSComboBox * m_firstDayOfYearEdit;

  // daylight savings section

  OSSwitch * m_dstOnOffButton;

  QRadioButton * m_dayOfWeekAndMonthStartButton;

  OSComboBox * m_startWeekBox;

  OSComboBox * m_startDayBox;

  OSComboBox * m_startMonthBox;

  QRadioButton * m_dateStartButton;

  QDateEdit * m_startDateEdit;

  QRadioButton * m_dayOfWeekAndMonthEndButton;

  OSComboBox * m_endWeekBox;

  OSComboBox * m_endDayBox;

  OSComboBox * m_endMonthBox;

  QRadioButton * m_dateEndButton;

  QDateEdit * m_endDateEdit;

  // other

  boost::optional<model::YearDescription> m_yearDescription;

  model::Model m_model;

  bool m_dirty;

};

} // openstudio

#endif // OPENSTUDIO_YEARSETTINGSWIDGET_HPP

