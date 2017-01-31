/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef OPENSTUDIO_YEARSETTINGSWIDGET_HPP
#define OPENSTUDIO_YEARSETTINGSWIDGET_HPP

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

#include <QRadioButton>
#include <QWidget>

class QDate;

class QDateEdit;

namespace openstudio {

class OSComboBox2;

class OSSwitch2;

class YearSettingsWidget : public QWidget, public Nano::Observer
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

  YearSettingsWidget(const model::Model & model, QWidget * parent = nullptr);

  bool calendarYearChecked();

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

  void onWorkspaceObjectAdd(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  void onWorkspaceObjectRemove(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

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
  QRadioButton * m_calendarYearButton = nullptr;

  OSComboBox2 * m_calendarYearEdit = nullptr;

  QRadioButton * m_firstDayOfYearButton = nullptr;

  OSComboBox2 * m_firstDayOfYearEdit = nullptr;

  // daylight savings section

  OSSwitch2 * m_dstOnOffButton = nullptr;

  QRadioButton * m_dayOfWeekAndMonthStartButton = nullptr;

  OSComboBox2 * m_startWeekBox = nullptr;

  OSComboBox2 * m_startDayBox = nullptr;

  OSComboBox2 * m_startMonthBox = nullptr;

  QRadioButton * m_dateStartButton = nullptr;

  QDateEdit * m_startDateEdit = nullptr;

  QRadioButton * m_dayOfWeekAndMonthEndButton = nullptr;

  OSComboBox2 * m_endWeekBox = nullptr;

  OSComboBox2 * m_endDayBox = nullptr;

  OSComboBox2 * m_endMonthBox = nullptr;

  QRadioButton * m_dateEndButton = nullptr;

  QDateEdit * m_endDateEdit = nullptr;

  // other

  boost::optional<model::YearDescription> m_yearDescription;

  model::Model m_model;

  bool m_dirty;

};

} // openstudio

#endif // OPENSTUDIO_YEARSETTINGSWIDGET_HPP

