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

#include <openstudio_lib/YearSettingsWidget.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include "../shared_gui_components/OSSwitch.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/RunPeriodControlDaylightSavingTime.hpp>
#include <model/RunPeriodControlDaylightSavingTime_Impl.hpp>
#include <utilities/time/Date.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/filetypes/EpwFile.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QDateEdit>
#include <QTimer>

namespace openstudio {

const int YearSettingsWidget::FIRSTYEAR = 1900;
const int YearSettingsWidget::LASTYEAR = 2100;

YearSettingsWidget::YearSettingsWidget(const model::Model & model, QWidget * parent)
  : QWidget(parent),
    m_dstOnOffButton(NULL),
    m_model(model),
    m_dirty(false)
{
  // Main Layout

  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10,10,10,10);
  mainVLayout->setSpacing(10);

  setLayout(mainVLayout);

  // YearDescription

  m_yearDescription = m_model.getUniqueModelObject<model::YearDescription>();

  // Year

  QLabel * selectYearLabel = new QLabel("Select Year by:");
  selectYearLabel->setObjectName("H2");
  mainVLayout->addWidget(selectYearLabel);

  QGridLayout * yearGridLayout = new QGridLayout();
  yearGridLayout->setContentsMargins(20,10,10,0);
  //yearGridLayout->setContentsMargins(20,10,10,0);
  yearGridLayout->setSpacing(10);
  mainVLayout->addLayout(yearGridLayout);

  QButtonGroup * yearButtonGroup = new QButtonGroup(this);

  m_calendarYearButton = new QRadioButton("Calendar Year",this);
  yearGridLayout->addWidget(m_calendarYearButton,0,0);
  yearButtonGroup->addButton(m_calendarYearButton);
  connect(m_calendarYearButton,SIGNAL(clicked()),this,SLOT(onCalendarYearButtonClicked()));

  m_calendarYearEdit = new OSComboBox();
  for( int i = FIRSTYEAR;
       i <= LASTYEAR;
       i++ )
  {
    m_calendarYearEdit->addItem(QString::number(i));
  }
  m_calendarYearEdit->setCurrentIndex(100);
  connect(m_calendarYearEdit,SIGNAL(currentIndexChanged(int)),this,SLOT(onCalendarYearChanged(int)));
  yearGridLayout->addWidget(m_calendarYearEdit,0,1);

  m_firstDayOfYearButton = new QRadioButton("First Day of Year",this);
  yearGridLayout->addWidget(m_firstDayOfYearButton,1,0);
  yearButtonGroup->addButton(m_firstDayOfYearButton);
  connect(m_firstDayOfYearButton,SIGNAL(clicked()),this,SLOT(onFirstDayofYearClicked()));

  m_firstDayOfYearEdit = new OSComboBox();
  std::vector<std::string> dayOfWeekValues = model::YearDescription::validDayofWeekforStartDayValues();
  for( std::vector<std::string>::iterator it = dayOfWeekValues.begin();
       it < dayOfWeekValues.end();
       it++ )
  {
    m_firstDayOfYearEdit->addItem(QString::fromStdString(*it));
  }
  connect(m_firstDayOfYearEdit,SIGNAL(currentIndexChanged(const QString &)),this,SIGNAL(firstDayofYearSelected(const QString &)));
  yearGridLayout->addWidget(m_firstDayOfYearEdit,1,1);

  yearGridLayout->setColumnStretch(2,10);

  QFrame * line1 = new QFrame();
  line1->setFrameShape(QFrame::HLine);
  line1->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line1);

  // DST

  QHBoxLayout * dstHLayout1 = new QHBoxLayout();
  dstHLayout1->setContentsMargins(0,0,0,0);
  dstHLayout1->setSpacing(10);

  QLabel * dstLabel = new QLabel("Daylight Savings Time:");
  dstLabel->setObjectName("H2");
  dstHLayout1->addWidget(dstLabel);

  m_dstOnOffButton = new OSSwitch();
  dstHLayout1->addWidget(m_dstOnOffButton);
  connect(m_dstOnOffButton,SIGNAL(clicked(bool)),this,SIGNAL(daylightSavingTimeClicked(bool)));

  dstHLayout1->addStretch();

  mainVLayout->addLayout(dstHLayout1);

  QGridLayout * dstGridLayout = new QGridLayout();
  dstGridLayout->setContentsMargins(20,10,10,10);
  dstGridLayout->setSpacing(10);
  mainVLayout->addLayout(dstGridLayout);

  // Starts

  QLabel * startsLabel = new QLabel("Starts");
  startsLabel->setObjectName("H2");
  dstGridLayout->addWidget(startsLabel,0,0);

  QButtonGroup * dstStartButtonGroup = new QButtonGroup(this);

  m_dayOfWeekAndMonthStartButton = new QRadioButton("Define by Day of The Week And Month",this);
  dstStartButtonGroup->addButton(m_dayOfWeekAndMonthStartButton);
  dstGridLayout->addWidget(m_dayOfWeekAndMonthStartButton,1,0);
  connect(m_dayOfWeekAndMonthStartButton,SIGNAL(clicked()),
          this,SLOT(onDefineStartByDayWeekMonthClicked()));

  m_startWeekBox = new OSComboBox();
  dstGridLayout->addWidget(m_startWeekBox,1,1);

  m_startDayBox = new OSComboBox();
  dstGridLayout->addWidget(m_startDayBox,1,2);

  m_startMonthBox = new OSComboBox();
  dstGridLayout->addWidget(m_startMonthBox,1,3);

  m_dateStartButton = new QRadioButton("Define by Date",this);
  dstStartButtonGroup->addButton(m_dateStartButton);
  dstGridLayout->addWidget(m_dateStartButton,2,0);
  connect(m_dateStartButton,SIGNAL(clicked()),
          this,SLOT(onDefineStartByDateClicked()));

  m_startDateEdit = new QDateEdit();
  m_startDateEdit->setCalendarPopup(true);
  dstGridLayout->addWidget(m_startDateEdit,2,1);

  // Ends

  QLabel * endsLabel = new QLabel("Ends");
  endsLabel->setObjectName("H2");
  dstGridLayout->addWidget(endsLabel,3,0);

  QButtonGroup * dstEndButtonGroup = new QButtonGroup(this);

  m_dayOfWeekAndMonthEndButton = new QRadioButton("Define by Day of The Week And Month",this);
  dstEndButtonGroup->addButton(m_dayOfWeekAndMonthEndButton);
  dstGridLayout->addWidget(m_dayOfWeekAndMonthEndButton,4,0);
  connect(m_dayOfWeekAndMonthEndButton,SIGNAL(clicked()),
          this,SLOT(onDefineEndByDayWeekMonthClicked()));

  m_endWeekBox = new OSComboBox();
  dstGridLayout->addWidget(m_endWeekBox,4,1);

  m_endDayBox = new OSComboBox();
  dstGridLayout->addWidget(m_endDayBox,4,2);

  m_endMonthBox = new OSComboBox();
  dstGridLayout->addWidget(m_endMonthBox,4,3);

  m_dateEndButton = new QRadioButton("Define by Date",this);
  dstEndButtonGroup->addButton(m_dateEndButton);
  dstGridLayout->addWidget(m_dateEndButton,5,0);
  connect(m_dateEndButton,SIGNAL(clicked()),
          this,SLOT(onDefineEndByDateClicked()));

  m_endDateEdit = new QDateEdit();
  m_endDateEdit->setCalendarPopup(true);
  dstGridLayout->addWidget(m_endDateEdit,5,1);

  dstGridLayout->setColumnStretch(4,10);

  // choices

  std::vector<std::string> weeks = YearSettingsWidget::weeksInMonth();
  std::vector<std::string> days = YearSettingsWidget::daysOfWeek();
  std::vector<std::string> months = YearSettingsWidget::months();

  for( std::vector<std::string>::iterator it = weeks.begin();
       it < weeks.end();
       it++ )
  {
    m_startWeekBox->addItem(QString::fromStdString(*it));
    m_endWeekBox->addItem(QString::fromStdString(*it));
  }

  for( std::vector<std::string>::iterator it = days.begin();
       it < days.end();
       it++ )
  {
    m_startDayBox->addItem(QString::fromStdString(*it));
    m_endDayBox->addItem(QString::fromStdString(*it));
  }

  for( std::vector<std::string>::iterator it = months.begin();
       it < months.end();
       it++ )
  {
    m_startMonthBox->addItem(QString::fromStdString(*it));
    m_endMonthBox->addItem(QString::fromStdString(*it));
  }

  // Stretch

  mainVLayout->addStretch();

  // Default Button State

  m_dateStartButton->setChecked(true);
  m_dateEndButton->setChecked(true);

  // Refresh

  scheduleRefresh();

  // Connect

  connect( m_yearDescription->getImpl<openstudio::model::detail::YearDescription_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(scheduleRefresh()) );

  QObject::connect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
                   SIGNAL(addWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>,
                   const openstudio::IddObjectType&, const openstudio::UUID&)),
                   this,
                   SLOT(onWorkspaceObjectAdd(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)));

  QObject::connect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
                   SIGNAL(removeWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>,
                   const openstudio::IddObjectType&, const openstudio::UUID&)),
                   this,
                   SLOT(onWorkspaceObjectRemove(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)));

  connect(m_startWeekBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDstStartDayWeekMonthChanged()));
  connect(m_startDayBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDstStartDayWeekMonthChanged()));
  connect(m_startMonthBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDstStartDayWeekMonthChanged()));
  connect(m_startDateEdit,SIGNAL(dateChanged(const QDate &)),this,SIGNAL(dstStartDateChanged(const QDate &)));

  connect(m_endWeekBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDstEndDayWeekMonthChanged()));
  connect(m_endDayBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDstEndDayWeekMonthChanged()));
  connect(m_endMonthBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDstEndDayWeekMonthChanged()));
  connect(m_endDateEdit,SIGNAL(dateChanged(const QDate &)),this,SIGNAL(dstEndDateChanged(const QDate &)));

}

void YearSettingsWidget::onWorkspaceObjectAdd(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo)
{
  if(wo->iddObject().type() == IddObjectType::OS_RunPeriodControl_DaylightSavingTime)
  {
    connect( wo.get(),
             SIGNAL(onChange()),
             this,
             SLOT(scheduleRefresh()) );

    scheduleRefresh();
  }
}

void YearSettingsWidget::onWorkspaceObjectRemove(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo)
{
  if(wo->iddObject().type() == IddObjectType::OS_RunPeriodControl_DaylightSavingTime)
  {
    scheduleRefresh();
  }
}

void YearSettingsWidget::scheduleRefresh()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(refresh()));
}

void YearSettingsWidget::refresh()
{
  if( m_dirty )
  {
    m_startWeekBox->blockSignals(true);
    m_startDayBox->blockSignals(true);
    m_startMonthBox->blockSignals(true);
    m_startDateEdit->blockSignals(true);
    m_endWeekBox->blockSignals(true);
    m_endDayBox->blockSignals(true);
    m_endMonthBox->blockSignals(true);
    m_endDateEdit->blockSignals(true);

    // Refresh Select By:

    if( m_yearDescription->calendarYear() )
    {
      m_calendarYearButton->setChecked(true);

      m_calendarYearEdit->setEnabled(true);

      m_firstDayOfYearEdit->setEnabled(false);
    }
    else // First Day of Year
    {
      m_firstDayOfYearButton->setChecked(true);

      m_calendarYearEdit->setEnabled(false);

      unsigned dayOfWeekIndex = 0;

      std::string dayOfWeekString = m_yearDescription->dayofWeekforStartDay();

      std::vector<std::string> dayOfWeekValues = model::YearDescription::validDayofWeekforStartDayValues();

      for( dayOfWeekIndex = 0; dayOfWeekIndex < dayOfWeekValues.size(); dayOfWeekIndex++ )
      {
        if( istringEqual(dayOfWeekValues[dayOfWeekIndex],dayOfWeekString) )
        {
          break;
        }
      }

      m_firstDayOfYearEdit->setCurrentIndex(dayOfWeekIndex);

      m_firstDayOfYearEdit->setEnabled(true);
    }

    // check if we have a weather file
    // DLM: this is not the right place for this code, however this is an example of how to get the start day
    // of week from the epw file.  The right approach would be to put all this in the model, however the model
    // would have to know the resource path.
    boost::optional<EpwFile> epwFile;
    boost::optional<model::WeatherFile> weatherFile = m_model.getOptionalUniqueModelObject<model::WeatherFile>();
    if (weatherFile){
      boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
      openstudio::path resourcesPath = openstudio::toPath(doc->modelTempDir()) / openstudio::toPath("resources");
      epwFile = weatherFile->file(resourcesPath);
      if (epwFile){
        DayOfWeek epwStartDayOfWeek = epwFile->startDayOfWeek();
      }
    }

    // Refresh Daylight Savings Time

    boost::optional<model::RunPeriodControlDaylightSavingTime> dst =
      m_model.getOptionalUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

    if( dst )
    {
      m_dstOnOffButton->setChecked(true);
      m_dayOfWeekAndMonthStartButton->setEnabled(true);
      m_dateStartButton->setEnabled(true);
      m_dayOfWeekAndMonthEndButton->setEnabled(true);
      m_dateEndButton->setEnabled(true);

      // DST Start

      Date startDate = dst->startDate();

      QDate qstartDate(startDate.year(), startDate.monthOfYear().value(), startDate.dayOfMonth());

      boost::optional<openstudio::NthDayOfWeekInMonth> startNthDayOfWeekInMonth = dst->startNthDayOfWeekInMonth();

      if (startNthDayOfWeekInMonth){
        m_dayOfWeekAndMonthStartButton->setChecked(true);

        m_startWeekBox->setEnabled(true);
        m_startWeekBox->setCurrentIndex(startNthDayOfWeekInMonth->value() - 1);

        m_startDayBox->setEnabled(true);
        m_startDayBox->setCurrentIndex(startDate.dayOfWeek().value());

        m_startMonthBox->setEnabled(true);
        m_startMonthBox->setCurrentIndex(startDate.monthOfYear().value() - 1);

        m_startDateEdit->setEnabled(false);
        m_startDateEdit->setDate(qstartDate);
      }else{
        m_dateStartButton->setChecked(true);

        m_startWeekBox->setEnabled(false);
        m_startWeekBox->setCurrentIndex(nthDayOfWeekInMonth(startDate).value() - 1);

        m_startDayBox->setEnabled(false);
        m_startDayBox->setCurrentIndex(startDate.dayOfWeek().value());

        m_startMonthBox->setEnabled(false);
        m_startMonthBox->setCurrentIndex(startDate.monthOfYear().value() - 1);

        m_startDateEdit->setEnabled(true);
        m_startDateEdit->setDate(qstartDate);
      }

      // DST End

      Date endDate = dst->endDate();

      QDate qendDate(endDate.year(), endDate.monthOfYear().value(), endDate.dayOfMonth());

      boost::optional<openstudio::NthDayOfWeekInMonth> endNthDayOfWeekInMonth = dst->endNthDayOfWeekInMonth();

      if (endNthDayOfWeekInMonth){
        m_dayOfWeekAndMonthEndButton->setChecked(true);

        m_endWeekBox->setEnabled(true);
        m_endWeekBox->setCurrentIndex(endNthDayOfWeekInMonth->value() - 1);

        m_endDayBox->setEnabled(true);
        m_endDayBox->setCurrentIndex(endDate.dayOfWeek().value());

        m_endMonthBox->setEnabled(true);
        m_endMonthBox->setCurrentIndex(endDate.monthOfYear().value() - 1);

        m_endDateEdit->setEnabled(false);
        m_endDateEdit->setDate(qendDate);
      }else{
        m_dateEndButton->setChecked(true);

        m_endWeekBox->setEnabled(false);
        m_endWeekBox->setCurrentIndex(nthDayOfWeekInMonth(endDate).value() - 1);

        m_endDayBox->setEnabled(false);
        m_endDayBox->setCurrentIndex(endDate.dayOfWeek().value());

        m_endMonthBox->setEnabled(false);
        m_endMonthBox->setCurrentIndex(endDate.monthOfYear().value() - 1);

        m_endDateEdit->setEnabled(true);
        m_endDateEdit->setDate(qendDate);
      }
    }else{

      m_dstOnOffButton->setChecked(false);
      m_dayOfWeekAndMonthStartButton->setEnabled(false);
      m_dateStartButton->setEnabled(false);
      m_dayOfWeekAndMonthEndButton->setEnabled(false);
      m_dateEndButton->setEnabled(false);

      m_startWeekBox->setEnabled(false);
      m_startDayBox->setEnabled(false);
      m_startMonthBox->setEnabled(false);
      m_endWeekBox->setEnabled(false);
      m_endDayBox->setEnabled(false);
      m_endMonthBox->setEnabled(false);
      m_startDateEdit->setEnabled(false);
      m_endDateEdit->setEnabled(false);
    }

    m_startWeekBox->blockSignals(false);
    m_startDayBox->blockSignals(false);
    m_startMonthBox->blockSignals(false);
    m_startDateEdit->blockSignals(false);
    m_endWeekBox->blockSignals(false);
    m_endDayBox->blockSignals(false);
    m_endMonthBox->blockSignals(false);
    m_endDateEdit->blockSignals(false);

    m_dirty = false;
  }
}

void YearSettingsWidget::onCalendarYearChanged(int index)
{
  emit calendarYearSelected(index + FIRSTYEAR);
}

void YearSettingsWidget::onCalendarYearButtonClicked()
{
  int index = m_calendarYearEdit->currentIndex();

  emit calendarYearSelected(index + FIRSTYEAR);
}

void YearSettingsWidget::onFirstDayofYearClicked()
{
  emit firstDayofYearSelected(m_firstDayOfYearEdit->currentText());
}

void YearSettingsWidget::onDstStartDayWeekMonthChanged()
{
  std::string s_dayOfWeek;
  std::string s_monthOfYear;

  int i_nthDayOfWeekInMonth = m_startWeekBox->currentIndex();
  s_dayOfWeek = m_startDayBox->currentText().toStdString();
  s_monthOfYear = m_startMonthBox->currentText().toStdString();

  NthDayOfWeekInMonth _nth(i_nthDayOfWeekInMonth + 1);
  DayOfWeek _dayOfWeek = dayOfWeek(s_dayOfWeek);
  MonthOfYear _monthOfYear = monthOfYear(s_monthOfYear);

  emit dstStartDayOfWeekAndMonthChanged(_nth.value(), _dayOfWeek.value(), _monthOfYear.value());
}

void YearSettingsWidget::onDstEndDayWeekMonthChanged()
{
  std::string s_dayOfWeek;
  std::string s_monthOfYear;

  int i_nthDayOfWeekInMonth = m_endWeekBox->currentIndex();
  s_dayOfWeek = m_endDayBox->currentText().toStdString();
  s_monthOfYear = m_endMonthBox->currentText().toStdString();

  NthDayOfWeekInMonth _nth(i_nthDayOfWeekInMonth + 1);
  DayOfWeek _dayOfWeek = dayOfWeek(s_dayOfWeek);
  MonthOfYear _monthOfYear = monthOfYear(s_monthOfYear);

  emit dstEndDayOfWeekAndMonthChanged(_nth.value(), _dayOfWeek.value(), _monthOfYear.value());
}

std::vector<std::string> YearSettingsWidget::weeksInMonth()
{
  std::vector<std::string> result(5);

  result[0] = "First";
  result[1] = "Second";
  result[2] = "Third";
  result[3] = "Fourth";
  result[4] = "Last";

  return result;
}

std::vector<std::string> YearSettingsWidget::daysOfWeek()
{
  std::vector<std::string> result;

  std::map<int, std::string> nameMap = DayOfWeek::getNames();

  for( std::map<int, std::string>::iterator it = nameMap.begin();
       it != nameMap.end();
       it++ )
  {
    result.push_back(it->second);
  }

  return result;
}

std::vector<std::string> YearSettingsWidget::months()
{
  std::vector<std::string> result;

  std::map<int, std::string> nameMap = MonthOfYear::getDescriptions();

  for( std::map<int, std::string>::iterator it = nameMap.begin();
       it != nameMap.end();
       it++ )
  {
    if (it->first == boost::date_time::NumMonths){
      // do nothing
    }else if (it->first == boost::date_time::NotAMonth){
      // do nothing
    }else{
      result.push_back(it->second);
    }
  }

  return result;
}

void YearSettingsWidget::onDefineStartByDayWeekMonthClicked()
{
  m_startWeekBox->setEnabled(true);

  m_startDayBox->setEnabled(true);

  m_startMonthBox->setEnabled(true);

  m_startDateEdit->setEnabled(false);

  onDstStartDayWeekMonthChanged();
}

void YearSettingsWidget::onDefineEndByDayWeekMonthClicked()
{
  m_endWeekBox->setEnabled(true);

  m_endDayBox->setEnabled(true);

  m_endMonthBox->setEnabled(true);

  m_endDateEdit->setEnabled(false);

  onDstEndDayWeekMonthChanged();
}

void YearSettingsWidget::onDefineStartByDateClicked()
{
  m_startWeekBox->setEnabled(false);

  m_startDayBox->setEnabled(false);

  m_startMonthBox->setEnabled(false);

  m_startDateEdit->setEnabled(true);

  m_startDateEdit->setDate(m_startDateEdit->date());
}

void YearSettingsWidget::onDefineEndByDateClicked()
{
  m_endWeekBox->setEnabled(false);

  m_endDayBox->setEnabled(false);

  m_endMonthBox->setEnabled(false);

  m_endDateEdit->setEnabled(true);

  m_endDateEdit->setDate(m_endDateEdit->date());
}

} // openstudio

