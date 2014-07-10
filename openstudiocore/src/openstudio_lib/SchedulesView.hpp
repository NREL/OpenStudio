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

class OSCheckBox;

class OSCheckBox2;

class OSLineEdit;

class MonthView;

class ScheduleDayView;

class OSItemId;

class SchedulesView : public QWidget
{
  Q_OBJECT

  public:

  SchedulesView(bool isIP,
                const model::Model & model);

  virtual ~SchedulesView() {}

  boost::optional<model::ScheduleRuleset> currentSchedule();

  ScheduleTab * tabForSchedule(const model::ScheduleRuleset schedule) const;

  void closeAllTabs() const;

  static const std::vector<QColor> colors;

  static std::vector<QColor> initializeColors();

  public slots:

  void setCurrentSchedule(const model::ScheduleRuleset & schedule);

  void showAddRulePage(const model::ScheduleRuleset & schedule);

  void showScheduleRule(model::ScheduleRule scheduleRule);

  void showDefaultScheduleDay(const model::ScheduleRuleset & schedule);

  void showSummerScheduleDay(model::ScheduleRuleset schedule);

  void showWinterScheduleDay(model::ScheduleRuleset schedule);

  void showScheduleRuleset(const model::ScheduleRuleset & schedule);

  void showEmptyPage();

  void toggleUnits(bool displayIP);

  signals:

  void itemDropped(const OSItemId& itemId);

  void addScheduleClicked();

  void removeSelectedScheduleClicked();

  void purgeUnusedScheduleRulesetsClicked();

  void removeScheduleRuleClicked(model::ScheduleRule & scheduleRule);

  void addRuleClicked(model::ScheduleRuleset & scheduleRuleset);

  void addSummerProfileClicked(model::ScheduleRuleset & scheduleRuleset);

  void addWinterProfileClicked(model::ScheduleRuleset & scheduleRuleset);

  void dayScheduleSceneChanged( DayScheduleScene * scene, double lowerValue, double upperValue );

  void startDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  void endDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  void changeVerticalAxisClicked(model::ScheduleDay scheduleDay);

  void toggleUnitsClicked(bool displayIP);

  void downloadComponentsClicked();

  void openLibDlgClicked();

  protected:

  void paintEvent ( QPaintEvent * event );

  private slots: 

  void addSchedule( model::ScheduleRuleset & schedule );

  void addScheduleRule( model::ScheduleRule & scheduleRule );

  void onModelAdd( std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID& );

  void removeSchedule( std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID& );

  void onScheduleRuleRemoved(Handle handle);

  private:

  void updateRowColors();

  model::Model m_model;

  QVBoxLayout * m_leftVLayout;

  QHBoxLayout * m_contentLayout;

  bool m_isIP;

};

class ScheduleTabHeader : public QWidget
{
  Q_OBJECT

  public:

  ScheduleTabHeader( ScheduleTab * scheduleTab, QWidget * parent = 0 );

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

  void paintEvent ( QPaintEvent * event );

  void resizeEvent ( QResizeEvent * event );

  void mouseReleaseEvent( QMouseEvent * event );

  void mousePressEvent( QMouseEvent * event );

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

class ScheduleTabRule : public QWidget
{
  Q_OBJECT

  public:

  ScheduleTabRule( ScheduleTab * scheduleTab, 
                   const model::ScheduleRule & scheduleRule, 
                   QWidget * parent = 0 );

  virtual ~ScheduleTabRule() {}

  signals:

  void clicked(model::ScheduleRule scheduleRule);

  protected:

  void mouseReleaseEvent( QMouseEvent * event );

  void mousePressEvent( QMouseEvent * event );

  void mouseMoveEvent( QMouseEvent * event );

  void leaveEvent ( QEvent * event );

  void paintEvent ( QPaintEvent * event );

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

class DefaultTab : public QWidget
{
  Q_OBJECT

  public:

  enum TabType { DEFAULT, SUMMER, WINTER };

  DefaultTab( ScheduleTab * scheduleTab, TabType type = DEFAULT );

  virtual ~DefaultTab() {}

  signals:

  void defaultClicked(model::ScheduleRuleset scheduleRuleset);

  void summerClicked(model::ScheduleRuleset scheduleRuleset);

  void winterClicked(model::ScheduleRuleset scheduleRuleset);

  protected:

  void mouseReleaseEvent( QMouseEvent * event );

  void mousePressEvent( QMouseEvent * event );

  void leaveEvent ( QEvent * event );

  void mouseMoveEvent( QMouseEvent * event );

  void paintEvent ( QPaintEvent * event );

  private:

  bool m_mouseDown;

  QLabel * m_label;
  
  ScheduleTab * m_scheduleTab;

  TabType m_type;

  bool m_hovering;
};

class ScheduleTabContent : public QWidget
{
  Q_OBJECT

  public:

  ScheduleTabContent( ScheduleTab * scheduleTab, QWidget * parent = 0 );

  virtual ~ScheduleTabContent() {}

  ScheduleTab * scheduleTab() const;

  void addScheduleRule( const model::ScheduleRule & scheduleRule );

  signals:

  void runPeriodRulesClicked(const model::ScheduleRuleset & schedule);

  void defaultClicked(const model::ScheduleRuleset & schedule);

  public slots:

  void scheduleRefresh();
  
  private slots:

  void refresh();

  void onRunPeriodRulesClicked();

  void onDefaultClicked();

  private:

  ScheduleTab * m_scheduleTab;

  QVBoxLayout * m_ruleLayout;

  std::map<Handle,QPushButton *> m_ruleButtonMap;

  bool m_mouseDown;

  bool m_dirty;
};

class ScheduleTab : public QWidget
{
  Q_OBJECT

  public:

  ScheduleTab(const model::ScheduleRuleset & schedule, SchedulesView * schedulesView, QWidget * parent = 0);

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
  
  void removeZoneClicked(const model::ScheduleRuleset & schedule);

  protected:

  void paintEvent ( QPaintEvent * event );

  //void mouseReleaseEvent( QMouseEvent * event );

  //void mousePressEvent( QMouseEvent * event );

  void leaveEvent ( QEvent * event );

  void resizeEvent ( QResizeEvent * event );

  private slots:

  //void onClicked();

  void onRemoveClicked();

  void onObjectChanged();

  void onToggleHeaderClicked(bool close);

  private:

  void refresh();

  bool m_mouseDown;

  bool m_selected;

  ScheduleTabHeader * m_header;

  ScheduleTabContent * m_content;

  model::ScheduleRuleset m_schedule;

  SchedulesView * m_schedulesView;
};

class NewRuleView : public QWidget
{
  Q_OBJECT

  public:

  enum Type { DEFAULT, SUMMER, WINTER };

  NewRuleView( const model::ScheduleRuleset & scheduleRuleset, SchedulesView * schedulesView, Type type = DEFAULT );

  virtual ~NewRuleView() {}

  signals:

  void addRuleClicked(model::ScheduleRuleset & scheduleRuleset);

  void addSummerProfileClicked(model::ScheduleRuleset & scheduleRuleset);

  void addWinterProfileClicked(model::ScheduleRuleset & scheduleRuleset);

  private slots: 

  void onAddClicked();

  private:

  SchedulesView * m_schedulesView;

  model::ScheduleRuleset m_scheduleRuleset;

  Type m_type;
};

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

class ScheduleRulesetView : public QWidget
{
  Q_OBJECT

  public:

  ScheduleRulesetView( const model::ScheduleRuleset & scheduleRuleset);

  virtual ~ScheduleRulesetView() {}

  private:

  model::ScheduleRuleset m_scheduleRuleset;
};

class SizingScheduleDayView : public QWidget
{
  Q_OBJECT

  public:

  enum ViewType { SUMMER, WINTER };

  SizingScheduleDayView( bool isIP,
                         const model::ScheduleRuleset & scheduleRuleset, 
                         SchedulesView * schedulesView,
                         ViewType type );

  virtual ~SizingScheduleDayView() {}

  signals:

  void toggleUnitsClicked(bool displayIP);

  private:

  ViewType m_type;
};

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

class ScheduleDayView : public QWidget
{
  Q_OBJECT

 public:

  ScheduleDayView(bool isIP,
                  const model::ScheduleDay & scheduleDay,
                  SchedulesView * schedulesView);

  virtual ~ScheduleDayView() {}

  SchedulesView * schedulesView() const;

  DayScheduleScene * scene() const;

  double zoom() const;

  double snap() const;

  bool setFocusStartTime(double time);

  double focusStartTime() const;

  double focusEndTime() const;

  double duration() const;

  model::ScheduleDay scheduleDay() const;

  double upperLimit() const;

  void setUpperLimit(double value);

  double lowerLimit() const;

  void setLowerLimit(double value);

  bool isIP() const;

 signals:

  void zoomChanged(double zoom);

  void toggleUnitsClicked(bool displayIP);

  //void startDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  //void endDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

 private slots:

  //void refresh();

  //void scheduleRefresh();

  //void onStartDateTimeChanged(const QDateTime & newDate);

  //void onEndDateTimeChanged(const QDateTime & newDate);

  void setHourlyZoom();

  void setQuarterHourlyZoom();

  void setOneMinuteZoom();

  void onLowerValueChanged(double d);

  void onUpperValueChanged(double d);

  void onToggleUnitsClicked(bool displayIP);

 private:

  boost::optional<model::YearDescription> m_yearDescription;

  double m_focusStartTime;

  double m_zoom;

  double m_snap;

  model::ScheduleDay m_scheduleDay;

  SchedulesView * m_schedulesView;

  DayScheduleScene * m_dayScheduleScene;

  ScheduleDayEditor * m_scheduleDayEditor;

  DayScheduleOverview * m_scheduleOverview;

  QDoubleSpinBox * m_upperLimitSpinBox;

  QDoubleSpinBox * m_lowerLimitSpinBox;

  bool m_dirty;

  bool m_isIP;

};

class ScheduleDayEditor : public QWidget
{
  Q_OBJECT

  public:

  ScheduleDayEditor(bool isIP, ScheduleDayView * scheduleView, const model::ScheduleDay & scheduleDay);

  virtual ~ScheduleDayEditor() {}

  ScheduleDayView * scheduleDayView() const;

  signals:

  void changeVerticalAxisClicked(model::ScheduleDay scheduleDay);

  public slots:

  void fitInView();

  void toggleUnits(bool displayIP);

  protected:

  void mouseDoubleClickEvent(QMouseEvent * event);

  void paintEvent(QPaintEvent * event);

  void resizeEvent(QResizeEvent * event);

  //void refresh();

  //void scheduleRefresh();

  private:

  //! Set m_yLabel with units data from m_scheduleDay's scheduleTypeLimits
  void setLabelText();

  DaySchedulePlotArea * m_graphicsView;

  ScheduleDayView * m_scheduleDayView;

  model::ScheduleDay m_scheduleDay;

  bool m_dirty;

  QLabel * m_yLabel;

  bool m_isIP;

};

class DayScheduleScene : public QGraphicsScene
{
  Q_OBJECT

  public:

  DayScheduleScene(ScheduleDayView * scheduleRuleView, const model::ScheduleDay & scheduleDay);

  virtual ~DayScheduleScene() {}

  ScheduleDayView * scheduleDayView() const;

  model::ScheduleDay scheduleDay() const;
  
  void addSegment(double untilTime,double value);

  CalendarSegmentItem * addSegment(double untilTime);

  void clearSegments();

  std::vector<CalendarSegmentItem *> segments() const;

  CalendarSegmentItem * segmentAt(double time) const;

  boost::optional<double> valueAt(double time) const;

  double timeAt(double x) const;

  double xAt(double time) const;

  QGraphicsItem * segmentAt(double x, double y, double zoom) const;

  public slots:

  void scheduleRefresh();

  private slots:

  void refresh();

  private:

  CalendarSegmentItem * m_firstSegment;

  ScheduleDayView * m_scheduleDayView;

  model::ScheduleDay m_scheduleDay;

  bool m_dirty;
};

class DaySchedulePlotArea : public QGraphicsView
{
  Q_OBJECT

  public:

  DaySchedulePlotArea(ScheduleDayEditor * mainScheduleView);

  virtual ~DaySchedulePlotArea() {}

  QTransform vItemTransform(); 
  
  QGraphicsItem * segmentAt(QPoint point) const;
  
  int timeAt(double x) const;

  boost::optional<double> valueAt(double x) const;

  DayScheduleScene * scene() const;

  signals:

  void dayScheduleSceneChanged( DayScheduleScene * scene, double lowerValue, double upperValue );

  protected:

  void mouseDoubleClickEvent(QMouseEvent * event);

  void mouseMoveEvent(QMouseEvent * event);

  void mousePressEvent(QMouseEvent * event);

  void mouseReleaseEvent(QMouseEvent * event);

  void keyPressEvent (QKeyEvent * event);

  private:

  ScheduleDayEditor * m_scheduleDayEditor;

  QGraphicsItem * m_currentItem;

  CalendarSegmentItem * m_currentHoverItem;

  QString m_keyboardInputValue;

  QPointF m_lastScenePos;
};

class DayScheduleOverview : public QWidget
{
  Q_OBJECT

  public:

  DayScheduleOverview(ScheduleDayView * scheduleRuleView);

  virtual ~DayScheduleOverview() {}

  void updateFocusRectangleGeometry();

  protected:

  void resizeEvent (QResizeEvent * event);

  void mouseMoveEvent(QMouseEvent * event);

  void mousePressEvent(QMouseEvent * event);

  void mouseReleaseEvent(QMouseEvent * event);

  private slots:

  void fitInView();

  void onZoomChange(double newZoom);

  private:

  double timeAt(int x) const;

  int xAt(double time) const;

  ScheduleDayView * m_scheduleRuleView;

  QGraphicsView * m_graphicsView;

  QWidget * m_focusRectangle;

  bool m_isMouseDown;

  double m_lastMousePos;
};

class VCalendarSegmentItem : public QGraphicsItem
{
  public:

  VCalendarSegmentItem( QGraphicsItem * parent = 0 );

  virtual ~VCalendarSegmentItem() {}

  DayScheduleScene * scene() const; 

  void setTime(double time);

  double time() const;

  bool isMouseDown() const;

  QRectF boundingRect() const;

  void setLength(double length);

  double length() const;

  CalendarSegmentItem * nextCalendarItem() const;

  void setNextCalendarItem(CalendarSegmentItem * item);

  CalendarSegmentItem * previousCalendarItem() const;

  void setPreviousCalendarItem(CalendarSegmentItem * item);

  void updateLength();

  double vCenterPos() const;

  double hCenterPos() const;

  protected:

  void setMouseDown(bool mouseDown);

  void setHovering(bool hovering);

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget );

  private:

  void remove();

  bool m_mouseDown;

  bool m_isHovering;

  double m_length;

  CalendarSegmentItem * m_nextCalendarItem;

  CalendarSegmentItem * m_previousCalendarItem;

  friend class DaySchedulePlotArea;
};

class CalendarSegmentItem : public QGraphicsItem
{
  public:

  CalendarSegmentItem( QGraphicsItem * parent = 0 );

  virtual ~CalendarSegmentItem() {}

  DayScheduleScene * scene() const; 

  double startTime() const;

  double endTime() const;

  void setStartTime(double time);

  void setEndTime(double time);

  double value() const;

  void setValue(double value);

  double vCenterPos() const;

  bool isMouseDown() const;

  QRectF boundingRect() const;

  VCalendarSegmentItem * nextVCalendarItem() const;

  void setNextVCalendarItem(VCalendarSegmentItem * item);

  VCalendarSegmentItem * previousVCalendarItem() const;

  void setPreviousVCalendarItem(VCalendarSegmentItem * item);

  CalendarSegmentItem * nextCalendarItem() const;

  CalendarSegmentItem * previousCalendarItem() const;

  void splitSegment(double time);

  protected:

  void setMouseDown(bool mouseDown);

  void setHovering(bool hovering);

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget );

  private:

  VCalendarSegmentItem * m_previousVCalendarItem;

  VCalendarSegmentItem * m_nextVCalendarItem;

  bool m_isHovering;

  bool m_mouseDown;

  double m_endTime;

  friend class DaySchedulePlotArea;
};

class ScheduleCalendarWidget : public QCalendarWidget
{
  Q_OBJECT

  public:

  ScheduleCalendarWidget(MonthView * monthView = 0);

  virtual ~ScheduleCalendarWidget() {}

  protected:

  void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;

  private:

  MonthView * m_monthView;
};

class MonthView : public QWidget
{
  Q_OBJECT

  public:
  
  MonthView( YearOverview * yearOverview );

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

class YearOverview : public QWidget
{
  Q_OBJECT

  public:

  YearOverview( const model::ScheduleRuleset & scheduleRuleset, QWidget * parent = 0 );

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

} // openstudio

#endif // OPENSTUDIO_SCHEDULESVIEW_HPP

