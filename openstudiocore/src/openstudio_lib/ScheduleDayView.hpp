/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef OPENSTUDIO_SCHEDULEDAYVIEW_HPP
#define OPENSTUDIO_SCHEDULEDAYVIEW_HPP

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/YearDescription.hpp"

#include <boost/optional.hpp>
#include <boost/smart_ptr.hpp>

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>

class QGraphicsScene;

class QGraphicsSceneMouseEvent;

class QGraphicsSceneHoverEvent;

class QLabel;

class QDoubleSpinBox;

namespace openstudio {

class Unit;

namespace model {

class Model;

}

class ScheduleDayEditor;

class VCalendarSegmentItem;

class DaySchedulePlotArea;

class CalendarSegmentItem;

class ScheduleTypeLimitItem;

class DayScheduleScene;

class DayScheduleOverview;

class SchedulesView;

class ScheduleDayView;

class ScheduleLimitsView;

// Main widget allowing editing of a schedule day
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

  // returns the upper value of the view
  double upperViewLimit() const;

  // returns the upper schedule type limit
  boost::optional<double> upperTypeLimit() const;

  // sets the current maximum value for the schedule
  void setMaximumScheduleValue(double value);

  // returns the lower value of the view
  double lowerViewLimit() const;

  // returns the lower schedule type limit
  boost::optional<double> lowerTypeLimit() const;

  // sets the current minimum value for the schedule
  void setMinimumScheduleValue(double value);

  // gets the current units for the schedule using current is IP
  boost::optional<Unit> units() const;

  // gets the current units for the schedule
  boost::optional<Unit> units(bool displayIP) const;

 signals:

  void zoomChanged(double zoom);

  void toggleUnitsClicked(bool displayIP);

  //void startDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

  //void endDateTimeChanged(model::ScheduleRule & scheduleRule, const QDateTime & newDate);

 public slots:

  void onToggleUnitsClicked(bool displayIP);

  // called when value in the spin box changes
  void onLowerViewLimitChanged(double d);

  // called when value in the spin box changes
  void onUpperViewLimitChanged(double d);

 private slots:

  //void refresh();

  //void scheduleRefresh();

  //void onStartDateTimeChanged(const QDateTime & newDate);

  //void onEndDateTimeChanged(const QDateTime & newDate);

  void setHourlyZoom();

  void setQuarterHourlyZoom();

  void setOneMinuteZoom();

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

  ScheduleLimitsView * m_scheduleLimitsView;

  bool m_dirty;

};

// Allows user to change limits of the view, provides starting view based on schedule type limits
class ScheduleLimitsView : public QWidget
{
  Q_OBJECT

public:
  ScheduleLimitsView(bool isIP,
                     const boost::optional<model::ScheduleTypeLimits>& scheduleTypeLimits,
                     ScheduleDayView* scheduleDayView);

  virtual ~ScheduleLimitsView() {}

  double upperViewLimit() const;

  boost::optional<double> upperTypeLimit() const;

  void setMaximumScheduleValue(double value);

  void onUpperViewLimitChanged(double d);

  double lowerViewLimit() const;

  boost::optional<double> lowerTypeLimit() const;

  void setMinimumScheduleValue(double value);

  void onLowerViewLimitChanged(double d);

  void onToggleUnitsClicked(bool displayIP);

  boost::optional<Unit> units() const;

  boost::optional<Unit> units(bool displayIP) const;

private:

  bool m_isIP;

  boost::optional<model::ScheduleTypeLimits> m_scheduleTypeLimits;

  QDoubleSpinBox * m_upperViewLimitSpinBox;

  boost::optional<double> m_upperTypeLimit;

  QDoubleSpinBox * m_lowerViewLimitSpinBox;

  boost::optional<double> m_lowerTypeLimit;
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

  void toggleUnits(bool isIP);

  void updateKeyboardPrompt(const QString& keyboardPrompt);

  protected:

  void mouseDoubleClickEvent(QMouseEvent * event) override;

  void paintEvent(QPaintEvent * event) override;

  void resizeEvent(QResizeEvent * event) override;

  //void refresh();

  //void scheduleRefresh();

  private:

  void setLabelText(bool isIP);

  DaySchedulePlotArea * m_graphicsView;

  ScheduleDayView * m_scheduleDayView;

  model::ScheduleDay m_scheduleDay;

  bool m_dirty;

  QLabel * m_yLabel;

  QLabel * m_keyboardPrompt;

};

class DayScheduleScene : public QGraphicsScene, public Nano::Observer
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

  ScheduleTypeLimitItem * m_upperScheduleTypeLimitItem;

  ScheduleTypeLimitItem * m_lowerScheduleTypeLimitItem;

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

  void keyboardPromptChanged(QString keyboardPrompt);

  protected:

  void mouseDoubleClickEvent(QMouseEvent * event) override;

  void mouseMoveEvent(QMouseEvent * event) override;

  void mousePressEvent(QMouseEvent * event) override;

  void mouseReleaseEvent(QMouseEvent * event) override;

  void keyPressEvent (QKeyEvent * event) override;

  private:

  void updateKeyboardPrompt();

  ScheduleDayEditor * m_scheduleDayEditor;

  QGraphicsItem * m_currentItem;

  QGraphicsItem * m_currentHoverItem;

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

  void resizeEvent (QResizeEvent * event) override;

  void mouseMoveEvent(QMouseEvent * event) override;

  void mousePressEvent(QMouseEvent * event) override;

  void mouseReleaseEvent(QMouseEvent * event) override;

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

  VCalendarSegmentItem( QGraphicsItem * parent = nullptr );

  virtual ~VCalendarSegmentItem() {}

  DayScheduleScene * scene() const;

  void setTime(double time);

  double time() const;

  bool isMouseDown() const;

  QRectF boundingRect() const override;

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
              QWidget *widget ) override;

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

  CalendarSegmentItem( QGraphicsItem * parent = nullptr );

  virtual ~CalendarSegmentItem() {}

  DayScheduleScene * scene() const;

  double startTime() const;

  double endTime() const;

  void setStartTime(double time);

  void setEndTime(double time);

  double value() const;

  void setValue(double value);

  bool isOutOfTypeLimits() const;

  void setIsOutOfTypeLimits(bool isOutOfTypeLimits);

  double vCenterPos() const;

  double hCenterPos() const;

  bool isMouseDown() const;

  QRectF boundingRect() const override;

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
              QWidget *widget ) override;

  private:

  VCalendarSegmentItem * m_previousVCalendarItem;

  VCalendarSegmentItem * m_nextVCalendarItem;

  bool m_isHovering;

  bool m_mouseDown;

  double m_endTime;

  bool m_isOutOfTypeLimits;

  friend class DaySchedulePlotArea;
};

class ScheduleTypeLimitItem : public QGraphicsItem
{
public:

  ScheduleTypeLimitItem(bool isUpperLimit, QGraphicsItem * parent = nullptr);

  virtual ~ScheduleTypeLimitItem() {}

  DayScheduleScene * scene() const;

  //double startTime() const;

  //double endTime() const;

  //void setStartTime(double time);

  //void setEndTime(double time);

  double value() const;

  void setValue(double value);

  double vCenterPos() const;

  QRectF boundingRect() const override;

protected:

  void paint(QPainter *painter,
             const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

private:

  bool m_isUpperLimit;

  friend class DaySchedulePlotArea;
};

} // openstudio

#endif // OPENSTUDIO_SCHEDULEDAYVIEW_HPP

