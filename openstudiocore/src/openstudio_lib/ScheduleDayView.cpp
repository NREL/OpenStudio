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

#include "ScheduleDayView.hpp"
#include "SchedulesView.hpp"

#include "../shared_gui_components/OSCheckBox.hpp"
#include "OSItem.hpp"
#include "OSItemSelectorButtons.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"

#include "../utilities/time/Date.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/units/OSQuantityVector.hpp"

#include "../utilities/core/Assert.hpp"

#include <QButtonGroup>
#include <QCalendarWidget>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QStackedWidget>
#include <QString>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>
#include <iterator>

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

static const double UPPER_LIMIT = 1.0;
static const double LOWER_LIMIT = 0.0;
static const double MARGINTOP = 20;
static const double MARGINBOTTOM = 30;
static const double MARGINLEFT = 60;
static const double MARGINRIGHT = 20;
static const double SCENEWIDTH = 86400;
static const double SCENEHEIGHT = 86400;
static const double LINEWIDTH = 3000;
static const double PENWIDTH = 500;

ScheduleDayView::ScheduleDayView(bool isIP,
                                 const model::ScheduleDay & scheduleDay,
                                 SchedulesView * schedulesView)
  : QWidget(schedulesView),
    m_focusStartTime(0.0),
    m_zoom(1.0),
    m_snap(3600.0),
    m_scheduleDay(scheduleDay),
    m_schedulesView(schedulesView),
    m_dayScheduleScene(nullptr),
    m_scheduleDayEditor(nullptr),
    m_scheduleOverview(nullptr),
    m_scheduleLimitsView(nullptr),
    m_dirty(false)
{

  boost::optional<model::ScheduleRuleset> currentSchedule = schedulesView->currentSchedule();
  boost::optional<model::ScheduleTypeLimits> currentScheduleTypeLimits;
  if (currentSchedule){
    currentScheduleTypeLimits = currentSchedule->scheduleTypeLimits();
  }
 
  // Scene

  m_dayScheduleScene = new DayScheduleScene(this,m_scheduleDay);

  m_dayScheduleScene->setSceneRect(0,0,SCENEWIDTH,SCENEHEIGHT);

  // Layout

  QVBoxLayout * mainVLayout = new QVBoxLayout();

  mainVLayout->setContentsMargins(0, 0, 0, 0);

  mainVLayout->setSpacing(10);

  setLayout(mainVLayout);

  QHBoxLayout * hLayout = nullptr;

  QLabel * label = nullptr;

  // Name

  //label = new QLabel("Name:");
  //label->setObjectName("H2");
  //mainVLayout->addWidget(label);

  //QString name;
  //boost::optional<std::string> optionalString = scheduleDay.name();
  //if(optionalString){
  //  name = optionalString->c_str();
  //}
  //QLineEdit * lineEdit = new QLineEdit(name);
  //lineEdit->setReadOnly(true);

  //mainVLayout->addWidget(lineEdit);

  // Schedule Limits View

  m_scheduleLimitsView = new ScheduleLimitsView(isIP, currentScheduleTypeLimits, this);
  mainVLayout->addWidget(m_scheduleLimitsView);

  // Day Schedule

  m_scheduleDayEditor = new ScheduleDayEditor(isIP, this, m_scheduleDay);

  connect(this, &ScheduleDayView::toggleUnitsClicked, m_scheduleDayEditor, &ScheduleDayEditor::toggleUnits);

  mainVLayout->addWidget(m_scheduleDayEditor);

  // Zoom button group

  QHBoxLayout * zoomButtonLayout = new QHBoxLayout();

  QButtonGroup * zoomButtonGroup = new QButtonGroup(this);

  zoomButtonLayout->addStretch();

  QPushButton * hourlyZoomButton = new QPushButton();
  hourlyZoomButton->setObjectName("StandardGrayButton");

  hourlyZoomButton->setCheckable(true);

  hourlyZoomButton->setChecked(true);

  zoomButtonGroup->addButton(hourlyZoomButton);

  hourlyZoomButton->setText("Hourly");
 
  connect(hourlyZoomButton, &QPushButton::clicked, this, &ScheduleDayView::setHourlyZoom);
  zoomButtonLayout->addWidget(hourlyZoomButton);
  zoomButtonLayout->addSpacing(10);

  QPushButton * quarterHourlyZoomButton = new QPushButton();
  quarterHourlyZoomButton->setObjectName("StandardGrayButton");

  quarterHourlyZoomButton->setCheckable(true);

  zoomButtonGroup->addButton(quarterHourlyZoomButton);

  quarterHourlyZoomButton->setText("15 Minutes");
 
  connect(quarterHourlyZoomButton, &QPushButton::clicked, this, &ScheduleDayView::setQuarterHourlyZoom);
  zoomButtonLayout->addWidget(quarterHourlyZoomButton);
  zoomButtonLayout->addSpacing(10);

  QPushButton * oneMinuteZommButton = new QPushButton();
  oneMinuteZommButton->setObjectName("StandardGrayButton");

  oneMinuteZommButton->setCheckable(true);

  zoomButtonGroup->addButton(oneMinuteZommButton);
  
  oneMinuteZommButton->setText("1 Minute");
 
  connect(oneMinuteZommButton, &QPushButton::clicked, this, &ScheduleDayView::setOneMinuteZoom);
  zoomButtonLayout->addWidget(oneMinuteZommButton);

  zoomButtonLayout->addStretch();

  mainVLayout->addLayout(zoomButtonLayout);

  // Day Overview

  QHBoxLayout * overviewLayout = new QHBoxLayout();

  m_scheduleOverview = new DayScheduleOverview( this );

  overviewLayout->addSpacing(MARGINLEFT);

  overviewLayout->addWidget(m_scheduleOverview);

  overviewLayout->addSpacing(MARGINRIGHT);

  mainVLayout->addLayout(overviewLayout);

  connect(this, &ScheduleDayView::toggleUnitsClicked, this, &ScheduleDayView::onToggleUnitsClicked);
}

double ScheduleDayView::upperViewLimit() const
{
  if (m_scheduleLimitsView){
    return m_scheduleLimitsView->upperViewLimit();
  }
  return UPPER_LIMIT;
}

boost::optional<double> ScheduleDayView::upperTypeLimit() const
{
  if (m_scheduleLimitsView){
    return m_scheduleLimitsView->upperTypeLimit();
  }
  return boost::none;
}

void ScheduleDayView::setMaximumScheduleValue(double value)
{
  if (m_scheduleLimitsView){
    m_scheduleLimitsView->setMaximumScheduleValue(value);
  }
}

void ScheduleDayView::onUpperViewLimitChanged(double d)
{
  if (m_scheduleLimitsView){
    m_scheduleLimitsView->onUpperViewLimitChanged(d);
  }
  if (scene()){
    scene()->scheduleRefresh();
  }
}

double ScheduleDayView::lowerViewLimit() const
{
  if (m_scheduleLimitsView){
    return m_scheduleLimitsView->lowerViewLimit();
  }
  return LOWER_LIMIT;
}

boost::optional<double> ScheduleDayView::lowerTypeLimit() const
{
  if (m_scheduleLimitsView){
    return m_scheduleLimitsView->lowerTypeLimit();
  }
  return boost::none;
}

void ScheduleDayView::setMinimumScheduleValue(double value)
{
  if (m_scheduleLimitsView){
    m_scheduleLimitsView->setMinimumScheduleValue(value);
  }
}

void ScheduleDayView::onLowerViewLimitChanged(double d)
{
  if (m_scheduleLimitsView){
    m_scheduleLimitsView->onLowerViewLimitChanged(d);
  }
  if(scene()){
    scene()->scheduleRefresh();
  }
}

void ScheduleDayView::onToggleUnitsClicked(bool displayIP) {
  if (m_scheduleLimitsView){
    m_scheduleLimitsView->onToggleUnitsClicked(displayIP);
  }
  if (scene()) {
    scene()->scheduleRefresh();
  }
}

boost::optional<Unit> ScheduleDayView::units() const
{
  if (m_scheduleLimitsView){
    return m_scheduleLimitsView->units();
  }
  return boost::none;
}

boost::optional<Unit> ScheduleDayView::units(bool displayIP) const
{
  if (m_scheduleLimitsView){
    return m_scheduleLimitsView->units(displayIP);
  }
  return boost::none;
}

SchedulesView * ScheduleDayView::schedulesView() const
{
  return m_schedulesView;
}

model::ScheduleDay ScheduleDayView::scheduleDay() const
{
  return m_scheduleDay;
}

double ScheduleDayView::focusStartTime() const
{
  return m_focusStartTime;
}

double ScheduleDayView::focusEndTime() const
{
  return focusStartTime() + duration();
}

double ScheduleDayView::duration() const
{
  return SCENEWIDTH / zoom();
}

DayScheduleScene * ScheduleDayView::scene() const
{
  return m_dayScheduleScene;
}

double ScheduleDayView::zoom() const
{
  return m_zoom;
}

double ScheduleDayView::snap() const
{
  return m_snap;
}

void ScheduleDayView::setHourlyZoom()
{
  m_zoom = 1.0;
  
  m_snap = 3600.0;

  emit zoomChanged(m_zoom);

  m_scheduleDayEditor->fitInView();

  if( focusStartTime() > SCENEWIDTH - duration() )
  {
    m_focusStartTime = SCENEWIDTH - duration();
  }

  m_scheduleOverview->updateFocusRectangleGeometry();

  m_scheduleDayEditor->update();
}

void ScheduleDayView::setQuarterHourlyZoom()
{
  m_zoom = 4.0;

  m_snap = 900.0;

  emit zoomChanged(m_zoom);

  m_scheduleDayEditor->fitInView();

  if( focusStartTime() > SCENEWIDTH - duration() )
  {
    m_focusStartTime = SCENEWIDTH - duration();
  }

  m_scheduleOverview->updateFocusRectangleGeometry();

  m_scheduleDayEditor->update();
}

void ScheduleDayView::setOneMinuteZoom()
{
  m_zoom = 60.0;

  m_snap = 60.0;

  emit zoomChanged(m_zoom);

  m_scheduleDayEditor->fitInView();

  if( focusStartTime() > SCENEWIDTH - duration() )
  {
    m_focusStartTime = SCENEWIDTH - duration();
  }

  m_scheduleOverview->updateFocusRectangleGeometry();

  m_scheduleDayEditor->update();
}

bool ScheduleDayView::setFocusStartTime(double proposedStartTime)
{
  double startTime = proposedStartTime;

  if( proposedStartTime < 0.0 )
  {
    startTime = 0.0;
  }
  else if( proposedStartTime > SCENEWIDTH - duration() )
  {
    startTime = SCENEWIDTH - duration();
  }

  m_focusStartTime = startTime;

  m_scheduleOverview->updateFocusRectangleGeometry();

  m_scheduleDayEditor->fitInView();

  m_scheduleDayEditor->update();

  return true;
}


ScheduleLimitsView::ScheduleLimitsView(bool isIP,
                                       const boost::optional<model::ScheduleTypeLimits>& scheduleTypeLimits,
                                       ScheduleDayView* scheduleDayView)
  : m_isIP(isIP),
    m_scheduleTypeLimits(scheduleTypeLimits),
    m_upperViewLimitSpinBox(nullptr),
    m_lowerViewLimitSpinBox(nullptr)
{
  QHBoxLayout * mainHLayout = new QHBoxLayout();

  mainHLayout->setContentsMargins(MARGINLEFT, 0, 0, 0);

  mainHLayout->setSpacing(0);

  setLayout(mainHLayout);

  QLabel * label = nullptr;

  // Lower Limit

  // Note: QDoubleSpinBox are used, rather than OSQuantityEdit
  // because limits are not connected via Q_PROPERTY. 
  m_lowerViewLimitSpinBox = new QDoubleSpinBox();
  m_lowerViewLimitSpinBox->setFixedWidth(100);

  connect(m_lowerViewLimitSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          scheduleDayView, &ScheduleDayView::onLowerViewLimitChanged);

  label = new QLabel("Lower Limit: ");
  mainHLayout->addWidget(label);

  mainHLayout->addWidget(m_lowerViewLimitSpinBox);

  mainHLayout->addSpacing(20);

  // Upper Limit

  m_upperViewLimitSpinBox = new QDoubleSpinBox();
  m_upperViewLimitSpinBox->setFixedWidth(100);
 
  connect(m_upperViewLimitSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          scheduleDayView, &ScheduleDayView::onUpperViewLimitChanged);

  label = new QLabel("Upper Limit: ");
  mainHLayout->addWidget(label);

  mainHLayout->addWidget(m_upperViewLimitSpinBox);

  mainHLayout->addStretch();

  onToggleUnitsClicked(isIP);
}

double ScheduleLimitsView::upperViewLimit() const
{
  OS_ASSERT(m_upperViewLimitSpinBox);
  return m_upperViewLimitSpinBox->value();
}

boost::optional<double> ScheduleLimitsView::upperTypeLimit() const
{
  return m_upperTypeLimit;
}

void ScheduleLimitsView::setMaximumScheduleValue(double value)
{
  OS_ASSERT(m_upperViewLimitSpinBox);

  m_upperViewLimitSpinBox->setMinimum(value);

  if (value > m_upperViewLimitSpinBox->value()){
    m_upperViewLimitSpinBox->setValue(value);
  }
}

void ScheduleLimitsView::onUpperViewLimitChanged(double d)
{
  OS_ASSERT(m_lowerViewLimitSpinBox);
  m_lowerViewLimitSpinBox->setMaximum(d); // lower limit can't go above value in upper limit
}

double ScheduleLimitsView::lowerViewLimit() const
{
  OS_ASSERT(m_lowerViewLimitSpinBox);
  return m_lowerViewLimitSpinBox->value();
}

boost::optional<double> ScheduleLimitsView::lowerTypeLimit() const
{
  return m_lowerTypeLimit;
}

void ScheduleLimitsView::setMinimumScheduleValue(double value)
{
  OS_ASSERT(m_lowerViewLimitSpinBox);

  m_lowerViewLimitSpinBox->setMaximum(value);

  if (value < m_lowerViewLimitSpinBox->value()){
    m_lowerViewLimitSpinBox->setValue(value);
  }
}

void ScheduleLimitsView::onLowerViewLimitChanged(double d)
{
  OS_ASSERT(m_lowerViewLimitSpinBox);
  m_upperViewLimitSpinBox->setMinimum(d); // upper limit can't go below value in lower limit
}

void ScheduleLimitsView::onToggleUnitsClicked(bool displayIP)
{
  m_isIP = displayIP;
  m_lowerTypeLimit.reset();
  m_upperTypeLimit.reset();

  double lowerViewLimitSpinBoxValue = LOWER_LIMIT;
  if (m_scheduleTypeLimits) {
    OSOptionalQuantity lowerLimitQ = m_scheduleTypeLimits->getLowerLimitValue(m_isIP);
    if (lowerLimitQ.isSet()) {
      m_lowerTypeLimit = lowerLimitQ.get().value();
    } else {
      // DLM: used for dimensionless numbers
      m_lowerTypeLimit = m_scheduleTypeLimits->lowerLimitValue();
    }
    if (m_lowerTypeLimit){
      lowerViewLimitSpinBoxValue = *m_lowerTypeLimit;
    }
  }
  m_lowerViewLimitSpinBox->setValue(lowerViewLimitSpinBoxValue);
  m_lowerViewLimitSpinBox->setMinimum(-1E10);
  //onLowerViewLimitChanged(lowerViewLimitSpinBoxValue);

  double upperViewLimitSpinBoxValue = UPPER_LIMIT;
  if (m_scheduleTypeLimits) {
    OSOptionalQuantity upperLimitQ = m_scheduleTypeLimits->getUpperLimitValue(m_isIP);
    if (upperLimitQ.isSet()) {
      m_upperTypeLimit = upperLimitQ.get().value();
    } else {
      // DLM: used for dimensionless numbers
      m_upperTypeLimit = m_scheduleTypeLimits->upperLimitValue();
    }
    if (m_upperTypeLimit){
      upperViewLimitSpinBoxValue = *m_upperTypeLimit;
    }
  }
  m_upperViewLimitSpinBox->setValue(upperViewLimitSpinBoxValue);
  m_upperViewLimitSpinBox->setMaximum(1E10);
  //onUpperViewLimitChanged(upperViewLimitSpinBoxValue);
}

boost::optional<Unit> ScheduleLimitsView::units() const
{
  if (m_scheduleTypeLimits){
    return m_scheduleTypeLimits->units(m_isIP);
  }
  return boost::none;
}

boost::optional<Unit> ScheduleLimitsView::units(bool displayIP) const
{
  if (m_scheduleTypeLimits){
    return m_scheduleTypeLimits->units(displayIP);
  }
  return boost::none;
}

ScheduleDayEditor::ScheduleDayEditor(bool isIP, ScheduleDayView * scheduleDayView, const model::ScheduleDay & scheduleDay) :
  QWidget(scheduleDayView),
  m_scheduleDayView(scheduleDayView),
  m_scheduleDay(scheduleDay),
  m_dirty(true)
{
  m_graphicsView = new DaySchedulePlotArea(this);

  m_graphicsView->setCacheMode(QGraphicsView::CacheNone);

  m_graphicsView->resize(SCENEWIDTH, SCENEHEIGHT);

  m_graphicsView->move(MARGINLEFT,MARGINTOP);

  m_graphicsView->setFrameShape(QFrame::NoFrame);

  m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_graphicsView->setScene(scheduleDayView->scene());

  m_yLabel = new QLabel(this);
  setLabelText(isIP);

  m_keyboardPrompt = new QLabel(this);

  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

  //connect(m_scheduleDay.scheduleTypeLimits()->getImpl<model::detail::ScheduleTypeLimits_Impl>().get(),
  //  &model::detail::ScheduleTypeLimits_Impl::onChange, this, &ScheduleDayEditor::scheduleRefresh);

  connect(this, &ScheduleDayEditor::changeVerticalAxisClicked, m_scheduleDayView->schedulesView(), &SchedulesView::changeVerticalAxisClicked);
  QTimer::singleShot(0,this,SLOT(fitInView()));
}

void ScheduleDayEditor::setLabelText(bool isIP)
{
  boost::optional<Unit> units = m_scheduleDayView->units(isIP);
  if (units) {
    std::stringstream ss;
    ss << *units;
    QString text = QString::fromStdString(ss.str());
    m_yLabel->setText(text);
  } else{
    m_yLabel->setText("");
  }
}

ScheduleDayView * ScheduleDayEditor::scheduleDayView() const
{
  return m_scheduleDayView;
}

void ScheduleDayEditor::fitInView()
{
  m_graphicsView->fitInView( QRectF(SCENEWIDTH / SCENEWIDTH * m_scheduleDayView->focusStartTime(),0.0,
                             SCENEWIDTH / m_scheduleDayView->zoom() - 2.0,SCENEHEIGHT - 2.0));
}

void ScheduleDayEditor::resizeEvent ( QResizeEvent * event )
{
  m_graphicsView->resize(event->size().width() - MARGINLEFT - MARGINRIGHT, event->size().height() - MARGINTOP - MARGINBOTTOM);

  int buttonHeight = 140; // Can't ask for button height until it is first rendered

  fitInView();

  QWidget::resizeEvent( event );
}

void ScheduleDayEditor::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void ScheduleDayEditor::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;

  opt.init(this);

  QPainter p(this);

  QTransform transform;

  p.setTransform(transform);

  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  // Draw border

  p.drawLine( MARGINLEFT - 1.0, height() - MARGINBOTTOM, width() - MARGINRIGHT, height() - MARGINBOTTOM );

  p.drawLine( MARGINLEFT - 1.0, MARGINTOP, MARGINLEFT - 1.0, height() - MARGINBOTTOM );

  p.drawLine( MARGINLEFT - 1.0, MARGINTOP - 1.0, width() - MARGINRIGHT, MARGINTOP - 1.0 );

  p.drawLine( width() - MARGINRIGHT, MARGINTOP, width() - MARGINRIGHT, height() - MARGINBOTTOM );

  // Draw horizontal tick marks

  double startTimeSeconds = scheduleDayView()->focusStartTime();
  double endTimeSeconds = scheduleDayView()->focusEndTime();
  
  double xstart = MARGINLEFT;
  double xend = width() - MARGINRIGHT;

  double timeSnapLength = scheduleDayView()->snap() * 4.0;

  double prevSnapTime = (int)(startTimeSeconds / timeSnapLength) * timeSnapLength;

  double time = prevSnapTime + timeSnapLength;

  if( std::fabs(startTimeSeconds - prevSnapTime) < 0.00001 )
  {
    time = startTimeSeconds;
  }

  double x = xstart + (time - startTimeSeconds) * (xend - xstart) / (endTimeSeconds - startTimeSeconds);

  double xSnapLength = timeSnapLength * (xend - xstart) / (endTimeSeconds - startTimeSeconds);

  while( x <= xend + 1.0 && x >= xstart - 1.0 )
  {
    p.drawLine(x,height() - MARGINBOTTOM,x,height() - MARGINBOTTOM + 7.0);

    Time osTime(0,0,0,time);

    QString timeString;

    if( osTime.days() == 1 )
    {
      timeString.append("24");
      timeString.append(":");
      timeString.append("00");
    }
    else
    {
      timeString.append(QString::number(osTime.hours()));
      timeString.append(":");
      int minutes = osTime.minutes();
      if( minutes < 10 )
      {
        timeString.append("0");
      }
      timeString.append(QString::number(minutes));
    }

    p.drawText( QRect(x - xSnapLength / 2.0,
                height() - MARGINBOTTOM + 7.0,
                xSnapLength,MARGINBOTTOM - 7.0 ),
                Qt::AlignHCenter | Qt::AlignTop,timeString);

    x = x + xSnapLength;

    time = time + timeSnapLength;
  }

  // Draw vertical tick marks

  double ystart = MARGINTOP;
  double yend = height() - MARGINBOTTOM;

  double yStartValue = m_scheduleDayView->upperViewLimit();
  double yEndValue = m_scheduleDayView->lowerViewLimit();

  double ySnap = 7.0;

  double yStride = (yend - ystart) / ySnap;
  double yValueStride = (yStartValue - yEndValue) / ySnap;

  double y = ystart;
  double yValue = yStartValue;

  while( y >= ystart - 1.0 && y <= yend + 1.0 )
  {
    p.drawLine(MARGINLEFT - 7.0,y,MARGINLEFT,y);

    double roundedValue = static_cast<double>(static_cast<int>(yValue*100+0.5))/100.0;

    QString valueString = QString::number(roundedValue,'g',3);

    p.drawText(QRect(7.0,y - 5.0,MARGINLEFT - 15.0,10.0),Qt::AlignRight | Qt::AlignCenter,valueString);

    y = y + yStride;

    yValue = yValue - yValueStride;
  }

  m_yLabel->move(0, MARGINTOP + 0.5*ySnap*yStride - 5.0);

}

// ***** SLOTS *****

void ScheduleDayEditor::toggleUnits(bool isIP)
{
  setLabelText(isIP);
}

void ScheduleDayEditor::updateKeyboardPrompt(const QString& keyboardPrompt)
{
  if (keyboardPrompt != m_keyboardPrompt->text()){
    m_keyboardPrompt->setText(keyboardPrompt);

    // Draw keyboard text
    QRectF r1 = m_keyboardPrompt->rect();
    m_keyboardPrompt->adjustSize();
    QRectF r2 = m_keyboardPrompt->rect();
    m_keyboardPrompt->move(width() - MARGINRIGHT - m_keyboardPrompt->width() - 5, 0);
    m_keyboardPrompt->update();

    this->update();
  }
  
}

CalendarSegmentItem::CalendarSegmentItem( QGraphicsItem * parent )
  : QGraphicsItem(parent),
    m_previousVCalendarItem(nullptr),
    m_nextVCalendarItem(nullptr),
    m_isHovering(false),
    m_mouseDown(false),
    m_endTime(SCENEWIDTH),
    m_isOutOfTypeLimits(false)
{
  setAcceptHoverEvents(true); 

  setToolTip("Double click to cut segment");

  setZValue(100);
}

double CalendarSegmentItem::vCenterPos() const
{
  return pos().y() + boundingRect().height() / 2.0;
}

double CalendarSegmentItem::hCenterPos() const
{
  return pos().x() + boundingRect().width() / 2.0;
}

CalendarSegmentItem * CalendarSegmentItem::nextCalendarItem() const
{
  if( VCalendarSegmentItem * item = nextVCalendarItem() )
  {
    return item->nextCalendarItem();
  }
  else
  {
    return nullptr;
  }
}

CalendarSegmentItem * CalendarSegmentItem::previousCalendarItem() const
{
  if( VCalendarSegmentItem * item = previousVCalendarItem() )
  {
    return item->previousCalendarItem();
  }
  else
  {
    return nullptr;
  }
}

bool CalendarSegmentItem::isMouseDown() const
{
  return m_mouseDown;
}

void CalendarSegmentItem::setMouseDown(bool mouseDown)
{
  m_mouseDown = mouseDown;
}

void CalendarSegmentItem::setHovering(bool hovering)
{
  m_isHovering = hovering;
}

bool CalendarSegmentItem::isOutOfTypeLimits() const
{
  return m_isOutOfTypeLimits;
}

void CalendarSegmentItem::setIsOutOfTypeLimits(bool isOutOfTypeLimits)
{
  m_isOutOfTypeLimits = isOutOfTypeLimits;
}

QRectF CalendarSegmentItem::boundingRect () const
{
  return QRectF(0.0,0.0,(endTime() - startTime()) * SCENEWIDTH / SCENEWIDTH,LINEWIDTH);
}

void CalendarSegmentItem::paint( QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget )
{
  if( m_isHovering )
  {
    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(Qt::blue));

    painter->drawRect(boundingRect());
  }

  QPen pen;

  //SCENEHEIGHT

  double penwidth = PENWIDTH;

  if( QWidget * view = qobject_cast<QWidget *>(widget->parent()) )
  {
    double height = view->height();

    penwidth = (PENWIDTH / height) * 400.0;
  }

  pen.setWidth(penwidth);

  pen.setCapStyle(Qt::FlatCap);

  if (m_isOutOfTypeLimits){
    pen.setColor(Qt::red);
  }

  painter->setPen(pen);

  painter->drawLine(QPointF(0.0,LINEWIDTH / 2.0),QPointF(boundingRect().width(),LINEWIDTH / 2.0));
}

void CalendarSegmentItem::splitSegment(double splitTime)
{
  double oldEndTime = endTime();

  VCalendarSegmentItem * oldNextVCalendarItem;

  oldNextVCalendarItem = this->nextVCalendarItem();

  CalendarSegmentItem * item = new CalendarSegmentItem;

  scene()->addItem(item);

  VCalendarSegmentItem * vitem = new VCalendarSegmentItem;

  scene()->addItem(vitem);

  vitem->setPreviousCalendarItem(this);

  vitem->setNextCalendarItem(item);

  this->setNextVCalendarItem(vitem);

  item->setPreviousVCalendarItem(vitem);

  item->setNextVCalendarItem(oldNextVCalendarItem);


  setEndTime(splitTime);

  item->setStartTime(splitTime);

  item->setEndTime(oldEndTime);

  item->setValue(value());

  vitem->setTime(splitTime);

  if( oldNextVCalendarItem )
  {
    oldNextVCalendarItem->setPreviousCalendarItem(item);
  }

  vitem->updateLength();
}

DayScheduleScene * CalendarSegmentItem::scene() const
{
  if( QGraphicsScene * scene = QGraphicsItem::scene() )
  {
    return qobject_cast<DayScheduleScene *>(scene);
  }
  else
  {
    return nullptr;
  }
}

VCalendarSegmentItem * CalendarSegmentItem::nextVCalendarItem() const
{
  return m_nextVCalendarItem;
}

void CalendarSegmentItem::setNextVCalendarItem(VCalendarSegmentItem * item)
{
  m_nextVCalendarItem = item;
}

VCalendarSegmentItem * CalendarSegmentItem::previousVCalendarItem() const
{
  return m_previousVCalendarItem;
}

void CalendarSegmentItem::setPreviousVCalendarItem(VCalendarSegmentItem * item)
{
  m_previousVCalendarItem = item;
}

double CalendarSegmentItem::startTime() const
{
  return scenePos().x() / SCENEWIDTH * SCENEWIDTH;
}

double CalendarSegmentItem::endTime() const
{
  return m_endTime;
}

void CalendarSegmentItem::setStartTime(double time)
{
  prepareGeometryChange();

  setX( time * SCENEWIDTH / SCENEWIDTH );
}

void CalendarSegmentItem::setEndTime(double time)
{
  prepareGeometryChange();

  m_endTime = time;
}

double CalendarSegmentItem::value() const
{
  return (SCENEHEIGHT - (scenePos().y() + boundingRect().height() / 2.0) ) / SCENEHEIGHT;
}

void CalendarSegmentItem::setValue(double value)
{
  setY(SCENEHEIGHT - (value * SCENEHEIGHT) - (boundingRect().height() / 2.0));

  double upperLimitValue = scene()->scheduleDayView()->upperViewLimit();
  double lowerLimitValue = scene()->scheduleDayView()->lowerViewLimit();

  float fullscalevalue = lowerLimitValue + value * (upperLimitValue - lowerLimitValue);

  QString tooltip = QString::number(fullscalevalue,'g',3);
  
  boost::optional<Unit> units = scene()->scheduleDayView()->units();
  if (units){
    if (!units->standardString().empty()){
      std::stringstream ss;
      ss << " (" << units->standardString() << ")";
      tooltip.append(toQString(ss.str()));
    }
  }

  tooltip.append(" - Double click to cut segment"); 

  setToolTip(tooltip);
}

VCalendarSegmentItem::VCalendarSegmentItem( QGraphicsItem * parent )
  : QGraphicsItem(parent),
    m_isHovering(false)
{
  setAcceptHoverEvents(true);

  setToolTip("Double click to delete segment");

  setLength(LINEWIDTH * 3.0);
}

DayScheduleScene * VCalendarSegmentItem::scene() const
{
  if( QGraphicsScene * scene = QGraphicsItem::scene() )
  {
    return qobject_cast<DayScheduleScene *>(scene);
  }
  else
  {
    return nullptr;
  }
}

double VCalendarSegmentItem::vCenterPos() const
{
  return pos().y() + boundingRect().height() / 2.0;
}

double VCalendarSegmentItem::hCenterPos() const
{
  return pos().x() + boundingRect().width() / 2.0;
}

void VCalendarSegmentItem::setMouseDown(bool mouseDown)
{
  m_mouseDown = mouseDown;
}

void VCalendarSegmentItem::setHovering(bool hovering)
{
  m_isHovering = hovering;
}

QRectF VCalendarSegmentItem::boundingRect() const
{
  return QRectF(0.0,0.0,LINEWIDTH,m_length);
}

void VCalendarSegmentItem::paint( QPainter *painter,
                                  const QStyleOptionGraphicsItem *option,
                                  QWidget *widget )
{
  if( ScheduleDayEditor * mainScheduleView = qobject_cast<ScheduleDayEditor *>(widget->parent()->parent()) )
  {
    QTransform transform = painter->transform();

    double zoom = mainScheduleView->scheduleDayView()->zoom();

    double dx = 0.5 * LINEWIDTH * ( 1.0 - 1.0 / zoom );

    transform.translate(dx,0.0);

    transform.scale(1.0 / zoom,1.0);

    painter->setTransform(transform);
  }

  if( m_isHovering )
  {
    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(Qt::blue));

    painter->drawRect(boundingRect());
  }

  QPen pen;

  pen.setWidth(PENWIDTH);

  pen.setCapStyle(Qt::SquareCap);

  painter->setPen(pen);

  painter->drawLine(LINEWIDTH / 2.0,0.0,LINEWIDTH / 2.0,boundingRect().height() - 1.0);
}

bool VCalendarSegmentItem::isMouseDown() const
{
  return m_mouseDown;
}

CalendarSegmentItem * VCalendarSegmentItem::nextCalendarItem() const
{
  return m_nextCalendarItem;
}

void VCalendarSegmentItem::setNextCalendarItem(CalendarSegmentItem * item)
{
  m_nextCalendarItem = item;
}

CalendarSegmentItem * VCalendarSegmentItem::previousCalendarItem() const
{
  return m_previousCalendarItem;
}

void VCalendarSegmentItem::setPreviousCalendarItem(CalendarSegmentItem * item)
{
  m_previousCalendarItem = item;
}

void VCalendarSegmentItem::setLength(double length)
{
  prepareGeometryChange();

  m_length = length;
}

double VCalendarSegmentItem::length() const
{
  return m_length;
}

void VCalendarSegmentItem::updateLength()
{
  CalendarSegmentItem * previousCalendarItem = this->previousCalendarItem();
  
  CalendarSegmentItem * nextCalendarItem = this->nextCalendarItem();
  
  double nextY = nextCalendarItem->pos().y() + nextCalendarItem->boundingRect().height() / 2.0;
  
  double prevY = previousCalendarItem->pos().y() + previousCalendarItem->boundingRect().height() / 2.0;

  double length = std::fabs(nextY - prevY);

  if( prevY < nextY )
  {
    if( length < LINEWIDTH * 3.0 )
    {
      this->setLength(LINEWIDTH * 3.0);

      this->setPos(this->pos().x(),0.5 * prevY + 0.5 * (nextY - LINEWIDTH * 3.0));
    }
    else
    {
      this->setPos(this->pos().x(),prevY);
    
      this->setLength(length);
    }
  }
  else
  {
    if( length < LINEWIDTH * 3.0 )
    {
      this->setLength(LINEWIDTH * 3.0);

      this->setPos(this->pos().x(),0.5 * nextY + 0.5 * (prevY - LINEWIDTH * 3.0));
    }
    else
    {
      this->setPos(this->pos().x(),nextY);
    
      this->setLength(length + 1.0);
    }
  }

  update();
}

void VCalendarSegmentItem::remove()
{
  CalendarSegmentItem * oldNextItem = nextCalendarItem();

  VCalendarSegmentItem * newNextVItem = oldNextItem->nextVCalendarItem();

  CalendarSegmentItem * item = previousCalendarItem();

  if( newNextVItem )
  {
    item->setNextVCalendarItem(newNextVItem);

    newNextVItem->setPreviousCalendarItem(item);

    newNextVItem->updateLength();
  }
  else
  {
    item->setNextVCalendarItem(nullptr);
  }

  item->setEndTime(oldNextItem->endTime());

  delete oldNextItem;

  delete this;
}

double VCalendarSegmentItem::time() const
{
  return scene()->timeAt(scenePos().x() + boundingRect().width() / 2.0);
}

void VCalendarSegmentItem::setTime(double time)
{
  double newX = scene()->xAt(time) - boundingRect().width() / 2.0;

  setX(newX);

  previousCalendarItem()->setEndTime(time);
  
  nextCalendarItem()->setStartTime(time);
}


ScheduleTypeLimitItem::ScheduleTypeLimitItem(bool isUpperLimit, QGraphicsItem * parent)
  : QGraphicsItem(parent),
    m_isUpperLimit(isUpperLimit)
{
  setAcceptHoverEvents(true);

  setToolTip("Schedule Type Limit");

  setZValue(90);
}

double ScheduleTypeLimitItem::vCenterPos() const
{
  return pos().y() + boundingRect().height() / 2.0;
}

QRectF ScheduleTypeLimitItem::boundingRect() const
{
  return QRectF(0.0, 0.0, SCENEWIDTH, LINEWIDTH);
}

void ScheduleTypeLimitItem::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
  QPen pen;

  //SCENEHEIGHT

  double penwidth = PENWIDTH;

  if (QWidget * view = qobject_cast<QWidget *>(widget->parent()))
  {
    double height = view->height();

    penwidth = (PENWIDTH / height) * 400.0;
  }

  pen.setWidth(penwidth);

  pen.setCapStyle(Qt::FlatCap);

  pen.setStyle(Qt::DashLine);

  pen.setColor(Qt::red);

  painter->setPen(pen);

  painter->drawLine(QPointF(0.0, LINEWIDTH / 2.0), QPointF(boundingRect().width(), LINEWIDTH / 2.0));
}

DayScheduleScene * ScheduleTypeLimitItem::scene() const
{
  if (QGraphicsScene * scene = QGraphicsItem::scene())
  {
    return qobject_cast<DayScheduleScene *>(scene);
  } else
  {
    return nullptr;
  }
}
/*
double ScheduleTypeLimitItem::startTime() const
{
  return scenePos().x() / SCENEWIDTH * SCENEWIDTH;
}

double ScheduleTypeLimitItem::endTime() const
{
  return m_endTime;
}

void ScheduleTypeLimitItem::setStartTime(double time)
{
  prepareGeometryChange();

  setX(time * SCENEWIDTH / SCENEWIDTH);
}

void ScheduleTypeLimitItem::setEndTime(double time)
{
  prepareGeometryChange();

  m_endTime = time;
}
*/
double ScheduleTypeLimitItem::value() const
{
  return (SCENEHEIGHT - (scenePos().y() + boundingRect().height() / 2.0)) / SCENEHEIGHT;
}

void ScheduleTypeLimitItem::setValue(double value)
{
  setY(SCENEHEIGHT - (value * SCENEHEIGHT) - (boundingRect().height() / 2.0));

  double upperLimitValue = scene()->scheduleDayView()->upperViewLimit();
  double lowerLimitValue = scene()->scheduleDayView()->lowerViewLimit();

  float fullscalevalue = lowerLimitValue + value * (upperLimitValue - lowerLimitValue);

  QString tooltip = QString::number(fullscalevalue, 'g', 3);

  boost::optional<Unit> units = scene()->scheduleDayView()->units();
  if (units){
    if (!units->standardString().empty()){
      std::stringstream ss;
      ss << "(" << units->standardString() << ")";
      tooltip.append(toQString(ss.str()));
    }
  }

  if (m_isUpperLimit){
    tooltip.append(" - Upper Limit");
  } else{
    tooltip.append(" - Lower Limit");
  }

  setToolTip(tooltip);
}

DayScheduleOverview::DayScheduleOverview(ScheduleDayView * scheduleRuleView)
  : QWidget(scheduleRuleView),
  m_lastMousePos(0)
{
  m_graphicsView = new QGraphicsView(this);

  m_graphicsView->move(0.0, 0.0);

  m_scheduleRuleView = scheduleRuleView;

  setFixedHeight(100);

  m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_graphicsView->setScene(m_scheduleRuleView->scene());

  // Focus Rectangle

  m_focusRectangle = new QWidget(this);

  m_focusRectangle->setStyleSheet("QWidget { border: 1px solid black; }");

  QTimer::singleShot(0, this, SLOT(fitInView()));

  updateFocusRectangleGeometry();

  connect(m_scheduleRuleView, &ScheduleDayView::zoomChanged, this, &DayScheduleOverview::onZoomChange);
}

void DayScheduleOverview::onZoomChange(double newZoom)
{
  updateFocusRectangleGeometry();  
}

void DayScheduleOverview::updateFocusRectangleGeometry()
{
  QPoint currentPos = m_focusRectangle->pos();

  int x = xAt(m_scheduleRuleView->focusStartTime());

  m_focusRectangle->move(x,currentPos.y());

  m_focusRectangle->resize(width() / m_scheduleRuleView->zoom() - 1.0,100 - 1.0);
}

double DayScheduleOverview::timeAt(int x) const
{
  return SCENEWIDTH / width() * x;
}

int DayScheduleOverview::xAt(double time) const
{
  return time / SCENEWIDTH * width();
}

void DayScheduleOverview::fitInView()
{
  m_graphicsView->fitInView(QRectF(0,0,SCENEWIDTH,SCENEHEIGHT));
}

void DayScheduleOverview::resizeEvent(QResizeEvent * event)
{
  m_graphicsView->resize(event->size());

  fitInView();

  updateFocusRectangleGeometry();
}

void DayScheduleOverview::mouseMoveEvent(QMouseEvent * event)
{
  if( m_isMouseDown )
  {
    double dt = timeAt(event->pos().x()) - m_lastMousePos;

    double currentT = m_scheduleRuleView->focusStartTime();

    double proposedStartTime = currentT + dt;

    if( m_scheduleRuleView->setFocusStartTime(proposedStartTime) )
    {
      event->accept(); 
    }
  }

  m_lastMousePos = timeAt(event->pos().x());
}

void DayScheduleOverview::mousePressEvent(QMouseEvent * event)
{
  if( (timeAt(event->pos().x()) <= m_scheduleRuleView->focusEndTime()) && (timeAt(event->pos().x()) >= m_scheduleRuleView->focusStartTime()) )
  {
    m_isMouseDown = true;
  
    m_lastMousePos = timeAt(event->pos().x());

    event->accept();
  }
}

void DayScheduleOverview::mouseReleaseEvent(QMouseEvent * event)
{
  m_isMouseDown = false;
  
  m_lastMousePos = 0;

  event->accept();
}

DaySchedulePlotArea::DaySchedulePlotArea(ScheduleDayEditor * scheduleDayEditor)
  : QGraphicsView(scheduleDayEditor),
    m_scheduleDayEditor(scheduleDayEditor),
    m_currentItem(nullptr),
    m_currentHoverItem(nullptr)
{
  connect(this, &DaySchedulePlotArea::dayScheduleSceneChanged, m_scheduleDayEditor->scheduleDayView()->schedulesView(), &SchedulesView::dayScheduleSceneChanged);
  setFocusPolicy(Qt::StrongFocus);

  connect(this, &DaySchedulePlotArea::keyboardPromptChanged, m_scheduleDayEditor, &ScheduleDayEditor::updateKeyboardPrompt);
  setFocusPolicy(Qt::StrongFocus);
}

void DaySchedulePlotArea::updateKeyboardPrompt()
{
  QString result;

  if (m_currentHoverItem){

    if (CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(m_currentHoverItem))
    {

      if (m_keyboardInputValue.isEmpty()){
        
        ScheduleDayView * scheduleDayView = scene()->scheduleDayView();

        double upperLimit = scheduleDayView->upperViewLimit();
        double lowerLimit = scheduleDayView->lowerViewLimit();

        double value = calendarItem->value()*(upperLimit - lowerLimit) + lowerLimit;

        result = QString("Type value and press enter: <span style='font-family:\"Courier New\";border-style:solid;white-space:pre;background-color:#ffffff'>%1</span>").arg(value, 10, 'g', 6, ' ');
      } else {
        double value = m_keyboardInputValue.toDouble();
        result = QString("Type value and press enter: <span style='font-family:\"Courier New\";border-style:solid;white-space:pre;background-color:#ffffff'>%1</span>").arg(value, 10, 'g', 6, ' ');
      }

      //boost::optional<Unit> units = scene()->scheduleDayView()->units();
      //if (units){
      //  if (!units->standardString().empty()){
      //    std::stringstream ss;
      //    ss << " (" << units->standardString() << ")";
      //    result.append(toQString(ss.str()));
      //  }
      //}

    } 
    else if (VCalendarSegmentItem * vCalendarItem = dynamic_cast<VCalendarSegmentItem *>(m_currentHoverItem))
    {
      result = "Drag vertical line to adjust";
    }

  } else{
    result = "Mouse over horizontal line to set value";
  }

  emit keyboardPromptChanged(result);
}


QGraphicsItem * DaySchedulePlotArea::segmentAt(QPoint point) const
{
  double zoom = m_scheduleDayEditor->scheduleDayView()->zoom();

  QPointF scenepoint = mapToScene(point);

  return scene()->segmentAt(scenepoint.x(),scenepoint.y(),zoom);  
}

int DaySchedulePlotArea::timeAt(double x) const
{
  QPointF scenepoint = mapToScene(QPoint(x,0));

  return scene()->timeAt(scenepoint.x());
}

boost::optional<double> DaySchedulePlotArea::valueAt(double x) const
{
  int time = timeAt(x);

  return scene()->valueAt(time);
}

DayScheduleScene * DaySchedulePlotArea::scene() const
{
  return qobject_cast<DayScheduleScene *>(QGraphicsView::scene());
}

void DaySchedulePlotArea::mouseDoubleClickEvent(QMouseEvent * event)
{
  QGraphicsItem * item = segmentAt(event->pos());

  m_currentHoverItem = nullptr;
  m_keyboardInputValue.clear();
  updateKeyboardPrompt();

  if( item )
  {
    if( dynamic_cast<CalendarSegmentItem *>(item) )
    {
      double time = scene()->timeAt(mapToScene(event->pos()).x());

      double newTime = time;

      if( newTime < 0.0 )
      {
        newTime = 0.0;
      }
      else if( newTime > SCENEWIDTH )
      {
        newTime = SCENEWIDTH;
      }

      double timeSnapLength = scene()->scheduleDayView()->snap();

      double prevSnapTime = (int)(newTime / timeSnapLength) * timeSnapLength;

      double nextSnapTime = prevSnapTime + timeSnapLength;

      if( (newTime - prevSnapTime) < (timeSnapLength / 2.0) )
      {
        newTime = prevSnapTime;
      }
      else if( (nextSnapTime - newTime) < (timeSnapLength / 2.0) )
      {
        newTime = nextSnapTime;
      }

      scene()->addSegment(newTime);
      
      event->accept();

      emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerViewLimit(),scene()->scheduleDayView()->upperViewLimit());
    }
    else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(item) )
    {
      calendarItem->remove();
      
      event->accept();

      emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerViewLimit(),scene()->scheduleDayView()->upperViewLimit());
    }
  }
}

void DaySchedulePlotArea::mouseMoveEvent(QMouseEvent * event)
{
  QPointF scenePos = mapToScene(event->pos()); 

  m_currentHoverItem = nullptr;
  m_keyboardInputValue.clear();

  if( m_currentItem )
  {
    if( CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(m_currentItem) )
    {
      calendarItem->setHovering(true);

      m_currentHoverItem = calendarItem;

      setFocus();

      calendarItem->update();

      double value = 1.0 - (mapToScene(event->pos()).y() / SCENEHEIGHT);

      if( value > 1.0 ) { value = 1.0; }

      if( value < 0.0 ) { value = 0.0; }

      calendarItem->setValue(value);

      // Fixup next V item

      VCalendarSegmentItem * _nextVCalendarItem = calendarItem->nextVCalendarItem();

      if( _nextVCalendarItem )
      {
        _nextVCalendarItem->updateLength();
      }

      // Fixup prev V item

      VCalendarSegmentItem * _previousVCalendarItem = calendarItem->previousVCalendarItem();

      if( _previousVCalendarItem )
      {
        _previousVCalendarItem->updateLength();
      }

      scene()->update();
    }
    else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(m_currentItem) )
    {
      calendarItem->setHovering(true);

      m_currentHoverItem = calendarItem;

      calendarItem->update();

      if( calendarItem->isMouseDown() )
      {
        double time = scene()->timeAt(mapToScene(event->pos()).x());

        double newTime = time;

        if( newTime < 0.0 )
        {
          newTime = 0.0;
        }
        else if( newTime > SCENEWIDTH )
        {
          newTime = SCENEWIDTH;
        }

        double timeSnapLength = scene()->scheduleDayView()->snap();

        if( CalendarSegmentItem * nextCalendarItem = calendarItem->nextCalendarItem() )
        {
          if( nextCalendarItem->endTime() - timeSnapLength - time < 1E-10 )
          {
            newTime = nextCalendarItem->endTime() - timeSnapLength;
          }
        }

        if( CalendarSegmentItem * prevCalendarItem = calendarItem->previousCalendarItem() )
        {
          if( time - timeSnapLength - prevCalendarItem->startTime() < 1E-10 )
          {
            newTime = prevCalendarItem->startTime() + timeSnapLength;
          }
        }

        double prevSnapTime = (int)(newTime / timeSnapLength) * timeSnapLength;

        double nextSnapTime = prevSnapTime + timeSnapLength;

        if( (newTime - prevSnapTime) < (timeSnapLength / 2.0) )
        {
          newTime = prevSnapTime;
        }
        else if( (nextSnapTime - newTime) < (timeSnapLength / 2.0) )
        {
          newTime = nextSnapTime;
        }

        calendarItem->setTime(newTime);
  
        scene()->update();
      }
    }
  }
  else
  {
    QList<QGraphicsItem *> items = this->items();

    for( QList<QGraphicsItem *>::iterator it = items.begin();
         it < items.end();
         ++it )
    {
      if( CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(*it) )
      {
        calendarItem->setHovering(false);

        calendarItem->update();
      }
      else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(*it) )
      {
        calendarItem->setHovering(false);

        calendarItem->update();
      }
    }

    QGraphicsItem * item = segmentAt(mapFromScene(m_lastScenePos));

    if( CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(item) )
    {
      calendarItem->setHovering(true);

      m_currentHoverItem = calendarItem;

      setFocus();

      calendarItem->update();
    }
    else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(item) )
    {
      calendarItem->setHovering(true);

      m_currentHoverItem = calendarItem;

      calendarItem->update();
    }
  }

  m_lastScenePos = scenePos;

  updateKeyboardPrompt();

  QGraphicsView::mouseMoveEvent(event);
}

void DaySchedulePlotArea::mousePressEvent(QMouseEvent * event)
{
  m_lastScenePos = mapToScene(event->pos()); 
  
  m_currentItem = nullptr;

  m_currentHoverItem = nullptr;
  m_keyboardInputValue.clear();
  updateKeyboardPrompt();

  QGraphicsItem * item = segmentAt(event->pos());

  if( item )
  {
    if( CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(item) )
    {
      calendarItem->setMouseDown(true);

      m_currentItem = calendarItem;
    }
    else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(item) )
    {
      calendarItem->setMouseDown(true);

      m_currentItem = calendarItem;
    }
  }

  QGraphicsView::mousePressEvent(event);
}

void DaySchedulePlotArea::mouseReleaseEvent(QMouseEvent * event)
{
  QList<QGraphicsItem *> items = this->items();

  for( QList<QGraphicsItem *>::iterator it = items.begin();
       it < items.end();
       ++it )
  {
    if( CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(*it) )
    {
      calendarItem->setMouseDown(false);
    }
    else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(*it) )
    {
      calendarItem->setMouseDown(false);
    }
  }

  emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerViewLimit(),scene()->scheduleDayView()->upperViewLimit());

  m_currentItem = nullptr;

  QGraphicsView::mouseReleaseEvent(event);
}

void DaySchedulePlotArea::keyPressEvent(QKeyEvent * event)
{
  if( m_currentHoverItem )
  {
    if (CalendarSegmentItem * calendarItem = dynamic_cast<CalendarSegmentItem *>(m_currentHoverItem))
    {
      if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
      {
        bool ok;
        double value = m_keyboardInputValue.toDouble(&ok);
        if (ok){

          ScheduleDayView * scheduleDayView = scene()->scheduleDayView();

          double upperLimit = scheduleDayView->upperViewLimit();
          double lowerLimit = scheduleDayView->lowerViewLimit();

          double scaledValue = (value - lowerLimit) / (upperLimit - lowerLimit);

          calendarItem->setValue(scaledValue);
        
          calendarItem->setHovering(false);

          m_currentHoverItem = nullptr;
          m_keyboardInputValue.clear();
          updateKeyboardPrompt();

          emit dayScheduleSceneChanged(scene(), lowerLimit, upperLimit);
        }

      } 
      else if (event->key() == Qt::Key_Minus)
      {

        m_keyboardInputValue = event->text();
        updateKeyboardPrompt();

      } 
      else if (event->key() == Qt::Key_Backspace)
      {
        m_keyboardInputValue.chop(1);
        updateKeyboardPrompt();

      } 
      else if (event->key() == Qt::Key_0 ||
               event->key() == Qt::Key_1 ||
               event->key() == Qt::Key_2 ||
               event->key() == Qt::Key_3 ||
               event->key() == Qt::Key_4 ||
               event->key() == Qt::Key_5 ||
               event->key() == Qt::Key_6 ||
               event->key() == Qt::Key_7 ||
               event->key() == Qt::Key_8 ||
               event->key() == Qt::Key_9
             )
      {
        
        m_keyboardInputValue.append(event->text());
        updateKeyboardPrompt();

      } 
      else if (event->key() == Qt::Key_Period)
      {
        if (!m_keyboardInputValue.contains(".")){
          m_keyboardInputValue.append(event->text());
          updateKeyboardPrompt();
        }
      }
    }
  }
}

DayScheduleScene::DayScheduleScene(ScheduleDayView * scheduleDayView, const model::ScheduleDay & scheduleDay)
  : QGraphicsScene(scheduleDayView),
    m_firstSegment(nullptr),
    m_upperScheduleTypeLimitItem(nullptr),
    m_lowerScheduleTypeLimitItem(nullptr),
    m_scheduleDayView(scheduleDayView),
    m_scheduleDay(scheduleDay),
    m_dirty(true)
{
  setSceneRect(0,0,SCENEWIDTH,SCENEHEIGHT);

  connect(m_scheduleDay.getImpl<model::detail::ScheduleDay_Impl>().get(), &model::detail::ScheduleDay_Impl::onChange, this, &DayScheduleScene::scheduleRefresh);
  
  refresh();
  //scheduleRefresh();
}

model::ScheduleDay DayScheduleScene::scheduleDay() const
{
  return m_scheduleDay;
}

void DayScheduleScene::scheduleRefresh()
{
  m_dirty = true;
  
  QTimer::singleShot(0,this,SLOT(refresh()));
}

void DayScheduleScene::refresh()
{
  if( m_dirty )
  {
    clearSegments();  

    std::vector<openstudio::Time> times = m_scheduleDay.times();

    std::vector<double> realvalues;
    if (m_scheduleDay.scheduleTypeLimits()) {
      OSQuantityVector quantities = m_scheduleDay.getValues(m_scheduleDayView->schedulesView()->isIP());
      realvalues = quantities.values();
    }
    else {
      realvalues = m_scheduleDay.values();
    }

    // The upper and lower type limits come from the model ScheduleTypeLimits.
    boost::optional<double> upperTypeLimit = m_scheduleDayView->upperTypeLimit();
    boost::optional<double> lowerTypeLimit = m_scheduleDayView->lowerTypeLimit();

    // The min and max values of the schedule as it appears on the screen
    double minvalue = * std::min_element(realvalues.begin(),realvalues.end());
    double maxvalue = * std::max_element(realvalues.begin(),realvalues.end());
    
    m_scheduleDayView->setMaximumScheduleValue(maxvalue);
    m_scheduleDayView->setMinimumScheduleValue(minvalue);

    // The upper and lower view limits are based on the view
    double upperViewLimit = m_scheduleDayView->upperViewLimit();
    double lowerViewLimit = m_scheduleDayView->lowerViewLimit();

    // if maxvalue > upperViewLimit then upperViewLimit will be reset
    if( maxvalue > upperViewLimit )
    {
      upperViewLimit = maxvalue; // + 0.05 * (maxvalue - minvalue);
    }

    // if minvalue < lowerViewLimit then lowerViewLimit will be reset
    if( minvalue < lowerViewLimit )
    {
      lowerViewLimit = minvalue; // - 0.05 * (maxvalue - minvalue);
    }

    if (upperTypeLimit){
      double scaledValue = (*upperTypeLimit - lowerViewLimit) / (upperViewLimit - lowerViewLimit);
      if (scaledValue > 0.0 && scaledValue < 1.0){
        m_upperScheduleTypeLimitItem = new ScheduleTypeLimitItem(true);
        addItem(m_upperScheduleTypeLimitItem);
        m_upperScheduleTypeLimitItem->setValue(scaledValue);
      }
    }

    if (lowerTypeLimit){
      double scaledValue = (*lowerTypeLimit - lowerViewLimit) / (upperViewLimit - lowerViewLimit);
      if (scaledValue > 0.0 && scaledValue < 1.0){
        m_lowerScheduleTypeLimitItem = new ScheduleTypeLimitItem(false);
        addItem(m_lowerScheduleTypeLimitItem);
        m_lowerScheduleTypeLimitItem->setValue(scaledValue);
      }
    }

    int i  = 0;
    double lastTime = 0.0;
    CalendarSegmentItem * previousSegment = nullptr;

    for( std::vector<openstudio::Time>::iterator it = times.begin();
         it < times.end();
         ++it )
    {
      bool isOutOfTypeLimits = false;
      if (upperTypeLimit && (realvalues[i] > *upperTypeLimit)){
        isOutOfTypeLimits = true;
      } else if (lowerTypeLimit && (realvalues[i] < *lowerTypeLimit)){
        isOutOfTypeLimits = true;
      }

      double scaledValue = (realvalues[i] - lowerViewLimit) / (upperViewLimit - lowerViewLimit);

      CalendarSegmentItem * segment = new CalendarSegmentItem();
      addItem(segment);
      segment->setValue(scaledValue);
      segment->setIsOutOfTypeLimits(isOutOfTypeLimits);
      segment->setStartTime(lastTime);
      double time = it->totalSeconds();
      segment->setEndTime(time);

      if( previousSegment )
      {
        VCalendarSegmentItem * vSegment = new VCalendarSegmentItem();
        addItem(vSegment);

        segment->setPreviousVCalendarItem(vSegment);
        vSegment->setNextCalendarItem(segment);
        vSegment->setPreviousCalendarItem(previousSegment);
        previousSegment->setNextVCalendarItem(vSegment);

        vSegment->setTime(lastTime);

        vSegment->updateLength();
      }
      else
      {
        m_firstSegment = segment;
      }

      previousSegment = segment;

      lastTime = time;

      i++;
    }

    m_scheduleDayView->update();

    m_dirty = false;
  }
}

ScheduleDayView * DayScheduleScene::scheduleDayView() const
{
  return m_scheduleDayView;
}

CalendarSegmentItem * DayScheduleScene::segmentAt(double time) const
{
  CalendarSegmentItem * segment = m_firstSegment;

  while( segment )
  {
    if( segment->endTime() >= time )
    {
      return segment;
    }
    else
    {
      segment = segment->nextCalendarItem();
    }
  }

  return nullptr;
}

boost::optional<double> DayScheduleScene::valueAt(double time) const
{
  if( CalendarSegmentItem * item = segmentAt(time) )
  {
    return item->value();
  }

  return boost::none;
}
  
void DayScheduleScene::addSegment(double untilTime,double value)
{
  CalendarSegmentItem * segment = addSegment(untilTime);

  segment->setValue(value);
}

CalendarSegmentItem * DayScheduleScene::addSegment(double untilTime)
{
  CalendarSegmentItem * segment = segmentAt(untilTime);

  double startTime = segment->startTime();

  VCalendarSegmentItem * prevVCalendarItem = segment->previousVCalendarItem();

  CalendarSegmentItem * item = new CalendarSegmentItem;

  addItem(item);

  VCalendarSegmentItem * vitem = new VCalendarSegmentItem;

  addItem(vitem);

  vitem->setPreviousCalendarItem(item);

  vitem->setNextCalendarItem(segment);

  segment->setPreviousVCalendarItem(vitem);

  item->setPreviousVCalendarItem(prevVCalendarItem);

  item->setNextVCalendarItem(vitem);


  segment->setStartTime(untilTime);

  item->setStartTime(startTime);

  item->setEndTime(untilTime);

  item->setValue(segment->value());

  vitem->setTime(untilTime);

  if( prevVCalendarItem )
  {
    prevVCalendarItem->setNextCalendarItem(item);
  }
  else
  {
    m_firstSegment = item;
  }

  vitem->updateLength();

  return item;
}

void DayScheduleScene::clearSegments()
{
  std::vector<CalendarSegmentItem *> _segments = segments();

  for( std::vector<CalendarSegmentItem *>::iterator it = _segments.begin();
       it < _segments.end();
       ++it )
  {
    if( VCalendarSegmentItem * vItem = (*it)->nextVCalendarItem() )
    {
      delete vItem;
    }

    delete *it;
  }

  m_firstSegment = nullptr;

  if (m_upperScheduleTypeLimitItem){
    delete m_upperScheduleTypeLimitItem;
  }
  m_upperScheduleTypeLimitItem = nullptr;

  if (m_lowerScheduleTypeLimitItem){
    delete m_lowerScheduleTypeLimitItem;
  }
  m_lowerScheduleTypeLimitItem = nullptr;

}

std::vector<CalendarSegmentItem *> DayScheduleScene::segments() const
{
  std::vector<CalendarSegmentItem *> result;

  CalendarSegmentItem * segment = m_firstSegment;

  while( segment )
  {
    result.push_back(segment);

    segment = segment->nextCalendarItem();
  }

  return result;
}

QGraphicsItem * DayScheduleScene::segmentAt(double x,double y, double zoom) const
{
  int time = timeAt(x);

  CalendarSegmentItem * segment = segmentAt(time);

  QGraphicsItem * result = nullptr;

  double vtol = LINEWIDTH / 2.0;

  double htol = LINEWIDTH / 2.0 / zoom;

  if( segment )
  {
    if( (segment->vCenterPos() < y + vtol) && (segment->vCenterPos() > y - vtol) )
    {
      result = segment;
    }
    
    if( ! result )
    {
      if( VCalendarSegmentItem * item = segment->nextVCalendarItem() )
      {
        if( (item->hCenterPos() < x + htol) && (item->hCenterPos() > x - htol) )
        {
          if( (item->vCenterPos() < y + item->boundingRect().height() / 2.0) )
          {
            result = item;
          }
        }
      }
    }
    
    if( ! result )
    {
      if( VCalendarSegmentItem * item = segment->previousVCalendarItem() )
      {
        if( (item->hCenterPos() < x + htol) && (item->hCenterPos() > x - htol) )
        {
          if( (item->vCenterPos() < y + item->boundingRect().height() / 2.0) )
          {
            result = item;
          }
        }
      }
    }
  }

  return result;
}

double DayScheduleScene::timeAt(double x) const
{
  return x * SCENEWIDTH / SCENEWIDTH;
}

double DayScheduleScene::xAt(double time) const
{
  return time * SCENEWIDTH / SCENEWIDTH;
}


} // openstudio

