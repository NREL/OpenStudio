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

#ifndef OPENSTUDIO_SCHEDULESVIEW_HPP
#define OPENSTUDIO_SCHEDULESVIEW_HPP

#include "../model/Model.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"

#include <boost/optional.hpp>
#include <boost/smart_ptr.hpp>

#include <map>

#include <QCalendarWidget>
#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>

class QPushButton;

class QStackedWidget;

class QGraphicsScene;

class QGraphicsSceneMouseEvent;

class QGraphicsSceneHoverEvent;

class QLabel;

class QVBoxLayout;

class QHBoxLayout;

class QDateTimeEdit;

class QLineEdit;

class QDoubleSpinBox;

namespace openstudio {

namespace model {

class Model;

}

class ScheduleDayEditor;

class VCalendarSegmentItem;

class DaySchedulePlotArea;

class CalendarSegmentItem;

class DayScheduleScene;

class DayScheduleOverview;

class YearOverview;

class ScheduleTab;

class ScheduleTabHeader;

class ScheduleTabContent;

class OSCheckBox;

class OSCheckBox2;

class OSLineEdit;

class ScheduleDayView;

class OSItemId;

class YearView;

class MonthView;

class ScheduleCalendarWidget;

// Overall view for the schedules tab, includes left column selector
class SchedulesView : public QWidget
{
  Q_OBJECT

  public:

    static const std::vector<QColor> colors;

    static std::vector<QColor> initializeColors();

    SchedulesView(bool isIP, const model::Model & model);

    virtual ~SchedulesView() {}

    boost::optional<model::ScheduleRuleset> currentSchedule();

    ScheduleTab * tabForSchedule(const model::ScheduleRuleset schedule) const;

    void closeAllTabs() const;

  public slots:

    void setCurrentSchedule(const model::ScheduleRuleset & schedule);

    void showAddRulePage(const model::ScheduleRuleset & schedule);

    void showScheduleRule(model::ScheduleRule scheduleRule);

    void showDefaultScheduleDay(const model::ScheduleRuleset & schedule);

    void showSummerScheduleDay(model::ScheduleRuleset schedule);

    void showWinterScheduleDay(model::ScheduleRuleset schedule);

    // DLM: might remove this
    void showScheduleRuleset(const model::ScheduleRuleset & schedule);

    void showEmptyPage();

    void toggleUnits(bool displayIP);

    bool isIP() const;

  signals:

    void itemDropped(const OSItemId& itemId);

    void addScheduleClicked();

    void copySelectedScheduleClicked();

    void removeSelectedScheduleClicked();

    void purgeUnusedScheduleRulesetsClicked();

    void removeScheduleRuleClicked(model::ScheduleRule & scheduleRule);

    void addRuleClicked(model::ScheduleRuleset & scheduleRuleset, UUID dayScheduleHandle);

    void addSummerProfileClicked(model::ScheduleRuleset & scheduleRuleset, UUID dayScheduleHandle);

    void addWinterProfileClicked(model::ScheduleRuleset & scheduleRuleset, UUID dayScheduleHandle);

    void dayScheduleSceneChanged( DayScheduleScene * scene, double lowerValue, double upperValue );

    void startDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

    void endDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

    void changeVerticalAxisClicked(model::ScheduleDay scheduleDay);

    void toggleUnitsClicked(bool displayIP);

    void downloadComponentsClicked();

    void openLibDlgClicked();

    void modelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

  protected:

    void paintEvent ( QPaintEvent * event ) override;

  private slots: 

    void addSchedule( model::ScheduleRuleset & schedule);

    void addScheduleRule( model::ScheduleRule & scheduleRule );

    void onModelObjectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

    void onModelObjectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

    void onScheduleRuleRemoved(Handle handle);

  private:

    void updateRowColors();

    model::Model m_model;

    QVBoxLayout * m_leftVLayout;

    QHBoxLayout * m_contentLayout;

    bool m_isIP;

};

/******************************************************************************/
// Left column selector classes
/******************************************************************************/

// Overall item in left column selector, includes collapsible header and content
class ScheduleTab : public QWidget
{
  Q_OBJECT

public:

  ScheduleTab(const model::ScheduleRuleset & schedule, SchedulesView * schedulesView, QWidget * parent = nullptr);

  virtual ~ScheduleTab() {}

  model::ScheduleRuleset schedule();

  SchedulesView * schedulesView() const;

  void setSelected(bool selected);

  bool selected();

  ScheduleTabHeader * scheduleTabHeader() const;

  ScheduleTabContent * scheduleTabContent() const;

  void expand();

  void collapse();

  void toggle();

signals:

  void scheduleClicked(const model::ScheduleRuleset & schedule);

  // DLM: not sure if this is wired to anything?
  void removeScheduleClicked(const model::ScheduleRuleset & schedule);

protected:

  //void paintEvent(QPaintEvent * event);

  //void mouseReleaseEvent( QMouseEvent * event );

  //void mousePressEvent( QMouseEvent * event );

  //void leaveEvent(QEvent * event);

  //void resizeEvent(QResizeEvent * event);

  private slots:

  //void onClicked();

  //void onRemoveClicked();

  //void onObjectChanged();

private:

  //void refresh();

  //bool m_mouseDown;

  bool m_selected;

  ScheduleTabHeader * m_header;

  ScheduleTabContent * m_content;

  model::ScheduleRuleset m_schedule;

  SchedulesView * m_schedulesView;
};

// Collapsible header for each schedule ruleset in left column selector
class ScheduleTabHeader : public QWidget
{
  Q_OBJECT

  public:

  ScheduleTabHeader( ScheduleTab * scheduleTab, QWidget * parent = nullptr );

  virtual ~ScheduleTabHeader() {}

  ScheduleTab * scheduleTab() const;

  void setSelected(bool selected);

  void setText(const QString & text);

  void expand();

  void collapse();

  void toggle();

  signals:
  
  void scheduleClicked(const model::ScheduleRuleset & schedule);

  void toggleHeaderClicked( bool close );

  protected:

  void paintEvent ( QPaintEvent * event ) override;

  void resizeEvent ( QResizeEvent * event ) override;

  void mouseReleaseEvent( QMouseEvent * event ) override;

  void mousePressEvent( QMouseEvent * event ) override;

  private slots:

  void refresh();

  void refreshNow();

  private:

  QWidget * m_selectionWidget;

  QLabel * m_mainLabel;

  QPushButton * m_toggleButton;

  ScheduleTab * m_scheduleTab;

  bool m_mouseDown;

  bool m_dirty;
};

// Content under collapsible header, includes a button for each rule and default schedule
class ScheduleTabContent : public QWidget
{
  Q_OBJECT

public:

  ScheduleTabContent(ScheduleTab * scheduleTab, QWidget * parent = nullptr);

  virtual ~ScheduleTabContent() {}

  ScheduleTab * scheduleTab() const;

  void addScheduleRule(const model::ScheduleRule & scheduleRule);

signals:

  void scheduleRuleClicked(const model::ScheduleRuleset & schedule);

  void defaultScheduleClicked(const model::ScheduleRuleset & schedule);

  public slots:

  void scheduleRefresh();

  private slots:

  void refresh();

  void onScheduleRuleClicked();

  void onDefaultScheduleClicked();

private:

  ScheduleTab * m_scheduleTab;

  QVBoxLayout * m_ruleLayout;

  std::map<Handle, QPushButton *> m_ruleButtonMap;

  bool m_mouseDown;

  bool m_dirty;
};

// Inner item in ScheduleTabContent, represents a ScheduleRule
class ScheduleTabRule : public QWidget
{
  Q_OBJECT

  public:

  ScheduleTabRule( ScheduleTab * scheduleTab, 
                   const model::ScheduleRule & scheduleRule, 
                   QWidget * parent = nullptr );

  virtual ~ScheduleTabRule() {}

  signals:

  void clicked(model::ScheduleRule scheduleRule);

  protected:

  void mouseReleaseEvent( QMouseEvent * event ) override;

  void mousePressEvent( QMouseEvent * event ) override;

  void mouseMoveEvent( QMouseEvent * event ) override;

  void leaveEvent ( QEvent * event ) override;

  void paintEvent ( QPaintEvent * event ) override;

  private slots:

  void refresh();

  void scheduleRefresh();

  private:

  ScheduleTab * m_scheduleTab;

  model::ScheduleRule m_scheduleRule;

  bool m_mouseDown;

  QLabel * m_label;

  bool m_dirty;

  bool m_hovering;
};

// Inner item in ScheduleTabContent, represents a default schedule such as design day or the default schedule
class ScheduleTabDefault : public QWidget
{
  Q_OBJECT

  public:

  enum ScheduleTabDefaultType { DEFAULT, SUMMER, WINTER };

  ScheduleTabDefault(ScheduleTab * scheduleTab, ScheduleTabDefaultType type);

  virtual ~ScheduleTabDefault() {}

  signals:

  void defaultClicked(model::ScheduleRuleset scheduleRuleset);

  void summerClicked(model::ScheduleRuleset scheduleRuleset);

  void winterClicked(model::ScheduleRuleset scheduleRuleset);

  protected:

  void mouseReleaseEvent( QMouseEvent * event ) override;

  void mousePressEvent( QMouseEvent * event ) override;

  void leaveEvent ( QEvent * event ) override;

  void mouseMoveEvent( QMouseEvent * event ) override;

  void paintEvent ( QPaintEvent * event ) override;

  private:

  bool m_mouseDown;

  QLabel * m_label;
  
  ScheduleTab * m_scheduleTab;

  ScheduleTabDefaultType m_type;

  bool m_hovering;
};


/******************************************************************************/
// Main view classes
/******************************************************************************/

// View presented when need to make a new profile (either schedule rule or design day)
class NewProfileView : public QWidget
{
  Q_OBJECT

  public:

    enum NewProfileViewType { SCHEDULERULE, SUMMER, WINTER };

    NewProfileView(const model::ScheduleRuleset & scheduleRuleset, SchedulesView * schedulesView, NewProfileViewType type);

    virtual ~NewProfileView() {}

  signals:

    void addRuleClicked(model::ScheduleRuleset & scheduleRuleset, UUID dayScheduleHandle);

    void addSummerProfileClicked(model::ScheduleRuleset & scheduleRuleset, UUID dayScheduleHandle);

    void addWinterProfileClicked(model::ScheduleRuleset & scheduleRuleset, UUID dayScheduleHandle);

  private slots: 

    void onAddClicked();

  private:

    void populateComboBox(const model::ScheduleRuleset & scheduleRuleset);

    SchedulesView * m_schedulesView;

    model::ScheduleRuleset m_scheduleRuleset;

    NewProfileViewType m_type;

    QComboBox* m_scheduleRuleComboBox;
};


// View presented only showing schedule name widget
class ScheduleRulesetNameView : public QWidget
{
  Q_OBJECT

public:

  ScheduleRulesetNameView(const model::ScheduleRuleset & scheduleRuleset, SchedulesView * schedulesView);

  virtual ~ScheduleRulesetNameView() {}

private:

  SchedulesView * m_schedulesView;

  model::ScheduleRuleset m_scheduleRuleset;
};

// View the default day schedule of a schedule ruleset
class DefaultScheduleDayView : public QWidget
{
  Q_OBJECT

  public:

    DefaultScheduleDayView( bool isIP,
                            const model::ScheduleRuleset & scheduleRuleset, 
                            SchedulesView * schedulesView );

    virtual ~DefaultScheduleDayView() {}

  signals:

    void toggleUnitsClicked(bool displayIP);

};

// View a sizing day schedule of a schedule ruleset
class SizingScheduleDayView : public QWidget
{
  Q_OBJECT

public:

  enum SizingScheduleDayType { SUMMER, WINTER };

  SizingScheduleDayView(bool isIP,
                        const model::ScheduleRuleset & scheduleRuleset,
                        SchedulesView * schedulesView,
                        SizingScheduleDayType type);

  virtual ~SizingScheduleDayView() {}

signals:

  void toggleUnitsClicked(bool displayIP);

private:

  SizingScheduleDayType m_type;
};

// View a schedule rule of a schedule ruleset
class ScheduleRuleView : public QWidget
{
  Q_OBJECT

public:

  ScheduleRuleView(bool isIP,
                   const model::ScheduleRule & scheduleRule,
                   SchedulesView * schedulesView);

  virtual ~ScheduleRuleView() {}

  model::ScheduleRule scheduleRule() const;

signals:

  void startDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  void endDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  void removeScheduleRuleClicked(model::ScheduleRule & scheduleRule);

  private slots:

  void refresh();

  void scheduleRefresh();

  void onRemoveClicked();

  void onStartDateTimeChanged(const QDateTime & newDate);

  void onEndDateTimeChanged(const QDateTime & newDate);

signals:

  void toggleUnitsClicked(bool displayIP);

private:

  SchedulesView * m_schedulesView;

  model::ScheduleRule m_scheduleRule;

  boost::optional<model::YearDescription> m_yearDescription;

  OSCheckBox2 * m_sundayButton;

  OSCheckBox2 * m_mondayButton;

  OSCheckBox2 * m_tuesdayButton;

  OSCheckBox2 * m_wednesdayButton;

  OSCheckBox2 * m_thursdayButton;

  OSCheckBox2 * m_fridayButton;

  OSCheckBox2 * m_saturdayButton;

  OSLineEdit * m_nameEditField;

  QDateTimeEdit * m_startDateEdit;

  QDateTimeEdit * m_endDateEdit;

  bool m_dirty;

  ScheduleDayView * m_scheduleDayView;
};


// Widget which shows the name and schedule type limits of a schedule ruleset
class ScheduleRulesetNameWidget : public QWidget
{
  Q_OBJECT

  public:

    ScheduleRulesetNameWidget(const model::ScheduleRuleset & scheduleRuleset);

    virtual ~ScheduleRulesetNameWidget() {}

  private:

    model::ScheduleRuleset m_scheduleRuleset;
};

// Overview of the year, held by ScheduleRuleView
class YearOverview : public QWidget
{
  Q_OBJECT

  public:

  YearOverview( const model::ScheduleRuleset & scheduleRuleset, QWidget * parent = nullptr );

  virtual ~YearOverview() {}

  model::ScheduleRuleset scheduleRuleset() const;

  std::vector<int> activeRuleIndices() const;

  private slots:

  void refresh();

  void scheduleRefresh();

  void onModelAdd(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

  private:

  void refreshActiveRuleIndices();

  MonthView * m_januaryView; 

  MonthView * m_februaryView; 

  MonthView * m_marchView; 

  MonthView * m_aprilView; 

  MonthView * m_mayView; 

  MonthView * m_juneView; 

  MonthView * m_julyView; 

  MonthView * m_augustView; 

  MonthView * m_septemberView; 

  MonthView * m_octoberView; 

  MonthView * m_novemberView; 

  MonthView * m_decemberView; 

  model::ScheduleRuleset m_scheduleRuleset;

  std::vector<int> m_activeRuleIndices;

  bool m_dirty;
};

// Overview of the month, held by YearOverview
class MonthView : public QWidget
{
  Q_OBJECT

public:

  MonthView(YearOverview * yearOverview);

  virtual ~MonthView() {}

  void setMonth(int month);

  YearOverview * yearOverview() const;

  int month() const;

  void update();

private:

  ScheduleCalendarWidget * m_calendarWidget;

  YearOverview * m_yearOverview;

  QLabel * m_monthLabel;

  int m_month;
};

// The calendar widget, held by MonthView
class ScheduleCalendarWidget : public QCalendarWidget
{
  Q_OBJECT

public:

  ScheduleCalendarWidget(MonthView * monthView = nullptr);

  virtual ~ScheduleCalendarWidget() {}

protected:

  void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const override;

private:

  MonthView * m_monthView;
};

} // openstudio

#endif // OPENSTUDIO_SCHEDULESVIEW_HPP

