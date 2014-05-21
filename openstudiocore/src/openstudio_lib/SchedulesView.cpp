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

#include <openstudio_lib/SchedulesView.hpp>

#include "../shared_gui_components/OSCheckBox.hpp"
#include <openstudio_lib/OSItemSelectorButtons.hpp>
#include "../shared_gui_components/OSLineEdit.hpp"

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ScheduleRule.hpp>
#include <model/ScheduleRuleset.hpp>
#include <model/ScheduleRuleset_Impl.hpp>
#include <model/ScheduleRule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeLimits_Impl.hpp>

#include <utilities/time/Date.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>
#include <utilities/units/OSQuantityVector.hpp>

#include <utilities/core/Assert.hpp>

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

#define UPPER_LIMIT 1.0
#define LOWER_LIMIT 0.0

namespace openstudio {

struct WorkspaceObjectNameLess;

static const double MARGINTOP = 10;
static const double MARGINBOTTOM = 30;
static const double MARGINLEFT = 80;
static const double MARGINRIGHT = 20;
static const double SCENEWIDTH = 86400;
static const double SCENEHEIGHT = 86400;
static const double LINEWIDTH = 3000;
static const double PENWIDTH = 500;

const std::vector<QColor> SchedulesView::colors = SchedulesView::initializeColors();

std::vector<QColor> SchedulesView::initializeColors()
{
  std::vector<QColor> _colors(13);

  _colors[0] = QColor(170,68,153);
  _colors[1] = QColor(51,34,136);
  _colors[2] = QColor(17,119,51);
  _colors[3] = QColor(153,153,51);
  _colors[4] = QColor(221,204,119);
  _colors[5] = QColor(204,102,119);
  _colors[6] = QColor(136,34,85);
  _colors[7] = QColor(68,170,153);
  _colors[8] = QColor(102,153,204);
  _colors[9] = QColor(102,17,0);
  _colors[10] = QColor(170,68,102);
  _colors[11] = QColor(80,80,80);
  _colors[12] = QColor(136,204,238);

  return _colors;
}

SchedulesView::SchedulesView(bool isIP,
                             const model::Model & model)
  : QWidget(),
    m_model(model),
    m_leftVLayout(NULL),
    m_contentLayout(NULL),
    m_isIP(isIP)
{
  setObjectName("GrayWidgetWithLeftTopBorders");

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(1,1,0,0);
  mainHLayout->setSpacing(0);
  setLayout(mainHLayout);

  QScrollArea * leftScrollArea = new QScrollArea();
  leftScrollArea->setFrameStyle(QFrame::NoFrame);

  QVBoxLayout * outerLeftVLayout = new QVBoxLayout();
  outerLeftVLayout->setContentsMargins(0,0,0,0);
  outerLeftVLayout->addWidget(leftScrollArea,10);
  outerLeftVLayout->addStretch();

  OSItemSelectorButtons * selectorButtons = new OSItemSelectorButtons(); 
  selectorButtons->disableCopyButton();
  selectorButtons->enablePurgeButton();
  selectorButtons->enableRemoveButton();

  bool isConnected = false;

  isConnected = connect(selectorButtons,SIGNAL(itemDropped(const OSItemId &)),
                        this,SIGNAL(itemDropped(const OSItemId &)));
  OS_ASSERT(isConnected);

  isConnected = connect(selectorButtons,SIGNAL(addClicked()),
                        this,SIGNAL(addScheduleClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(selectorButtons,SIGNAL(removeClicked()),
                        this,SIGNAL(removeSelectedScheduleClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(selectorButtons,SIGNAL(purgeClicked()),
                        this,SIGNAL(purgeUnusedScheduleRulesetsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(selectorButtons,SIGNAL(downloadComponentsClicked()),
                        this,SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        this, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  outerLeftVLayout->addWidget(selectorButtons);

  mainHLayout->addLayout(outerLeftVLayout);

  QWidget * vLine = new QWidget();
  vLine->setObjectName("VLine");
  vLine->setStyleSheet("QWidget#VLine { background: #445051;}");
  vLine->setFixedWidth(2);
  mainHLayout->addWidget(vLine);

  QWidget * leftScrollWidget = new QWidget();
  leftScrollArea->setWidget(leftScrollWidget);
  leftScrollArea->setWidgetResizable(true);
  leftScrollWidget->setObjectName("GrayWidget");

  m_leftVLayout = new QVBoxLayout();
  m_leftVLayout->setContentsMargins(0,0,0,0);
  m_leftVLayout->setSpacing(0);
  m_leftVLayout->addStretch();
  leftScrollWidget->setLayout(m_leftVLayout);
  
  m_contentLayout = new QHBoxLayout();
  m_contentLayout->setContentsMargins(0,0,0,0);
  mainHLayout->addLayout(m_contentLayout,100);

  connect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
          SIGNAL(addWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>,
                                    const openstudio::IddObjectType&, const openstudio::UUID&)),
          this,
          SLOT(onModelAdd(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
          Qt::QueuedConnection);
 
  connect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
          SIGNAL(removeWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, 
                                       const openstudio::IddObjectType&, const openstudio::UUID&)),
          this,
          SLOT(removeSchedule(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, 
                              const openstudio::UUID&)),
          Qt::QueuedConnection);

  // get all schedules
  std::vector<model::ScheduleRuleset> schedules = m_model.getConcreteModelObjects<model::ScheduleRuleset>();

  // sort by name
  std::sort(schedules.begin(), schedules.end(), WorkspaceObjectNameGreater());

  for(std::vector<model::ScheduleRuleset>::iterator it = schedules.begin();
      it < schedules.end();
      ++it )
  {
    addSchedule(*it);

    std::vector<model::ScheduleRule> rules = it->scheduleRules();

    for( std::vector<model::ScheduleRule>::iterator ruleIt = rules.begin();
         ruleIt < rules.end();
         ++ruleIt )
    {
      addScheduleRule(*ruleIt);
    }
  }

  updateRowColors();

  showEmptyPage();
}

void SchedulesView::closeAllTabs() const
{
  for ( int i = 0;
        i < m_leftVLayout->count() - 1; 
        i++ )
  {
    ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    scheduleTab->scheduleTabContent()->hide();
  }
}

ScheduleTab * SchedulesView::tabForSchedule(const model::ScheduleRuleset schedule) const
{
  for ( int i = 0;
        i < m_leftVLayout->count() - 1; 
        i++ )
  {
    ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if( scheduleTab->schedule().handle() == schedule.handle() )
    {
      return scheduleTab;
    }
  }

  return NULL;
}

void SchedulesView::updateRowColors()
{
  QString evenStyle;
  evenStyle.append( "QWidget#ThermalZoneTab {" );
  evenStyle.append( "background: #F2F2F2; ");
  evenStyle.append( "border-bottom: 1px solid black; " );
  evenStyle.append( "}" );
  evenStyle.append( "QWidget#SideBar {background: #DEDEDE;}" );

  QString oddStyle;
  oddStyle.append( "QWidget#ThermalZoneTab {" );
  oddStyle.append( "background: #E6E6E6; ");
  oddStyle.append( "border-bottom: 1px solid black; " );
  oddStyle.append( "}" );
  oddStyle.append( "QWidget#SideBar {background: #CECECE;}" );

  for ( int i = 0;
        i < m_leftVLayout->count() - 1; 
        i++ )
  {
    ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if( i % 2 == 0 )
    {
      scheduleTab->setStyleSheet(evenStyle);
    }
    else
    {
      scheduleTab->setStyleSheet(oddStyle);
    }
  }
}

void SchedulesView::addSchedule( model::ScheduleRuleset & schedule )
{
  ScheduleTab * scheduleTab = new ScheduleTab(schedule,this);
  connect(scheduleTab,SIGNAL(scheduleClicked(const model::ScheduleRuleset &)),this,SLOT(setCurrentSchedule(const model::ScheduleRuleset &)));
  m_leftVLayout->insertWidget(0,scheduleTab);

  // todo: sort?

  this->setCurrentSchedule(schedule);
}

void SchedulesView::addScheduleRule( model::ScheduleRule & rule )
{
  model::ScheduleRuleset scheduleRuleset = rule.scheduleRuleset();

  ScheduleTab * tab = tabForSchedule(scheduleRuleset);

  if( tab )
  {
    tab->scheduleTabContent()->scheduleRefresh();
  
    connect(rule.getImpl<model::detail::ScheduleRule_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
            tab->scheduleTabContent(),SLOT(scheduleRefresh()));
  }

}

void SchedulesView::onModelAdd( boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> workspaceObjectImpl,
                                const openstudio::IddObjectType&, 
                                const openstudio::UUID& )
{
  boost::optional<model::ScheduleRuleset> schedule = m_model.getModelObject<model::ScheduleRuleset>(workspaceObjectImpl->handle());
  if( schedule )
  {
    this->addSchedule(schedule.get());
  }

  boost::optional<model::ScheduleRule> rule = m_model.getModelObject<model::ScheduleRule>(workspaceObjectImpl->handle());
  if( rule )
  {
    addScheduleRule(rule.get());
  }
}

void SchedulesView::removeSchedule( boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> workspaceObjectImpl, 
                                    const openstudio::IddObjectType&, 
                                    const openstudio::UUID& )
{
  if( workspaceObjectImpl->iddObject().type() == IddObjectType::OS_Schedule_Ruleset )
  {
    int removedIndex = -1;

    bool wasSelected = false;
  
    ScheduleTab * scheduleTab = 0;

    for ( int i = 0;
          i < m_leftVLayout->count() - 1; 
          i++ )
    {
      scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());
      if( scheduleTab->schedule().handle() == workspaceObjectImpl->handle() )
      {
        m_leftVLayout->removeWidget(scheduleTab);
        wasSelected = scheduleTab->selected();
        removedIndex = i;
        delete scheduleTab;
        break;
      }
    }

    if( wasSelected )
    {
      int newIndex = -1;
      if( removedIndex > 0 )
      {
        newIndex = removedIndex - 1; 
      }
      else if( removedIndex == 0 )
      {
        if( m_leftVLayout->count() > 1 )
        {
          newIndex = 0;
        }
      }

      if( newIndex > -1 )
      {
        ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(newIndex)->widget());

        this->setCurrentSchedule(scheduleTab->schedule());
      }
      else
      {
        this->showEmptyPage();
      }
    }
  }
}

void SchedulesView::setCurrentSchedule(const model::ScheduleRuleset & schedule)
{
  this->updateRowColors();

  for ( int i = 0;
        i < m_leftVLayout->count() - 1; 
        i++ )
  {
    ScheduleTab * scheduleTab;
    
    scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if( scheduleTab->schedule() == schedule )
    {
      if( ! scheduleTab->selected() )
      {
        scheduleTab->setSelected(true);

        scheduleTab->expand();
      }
      else
      {
        scheduleTab->toggle();
      }
    }
    else
    {
      scheduleTab->setSelected(false);

      scheduleTab->collapse();
    }

    scheduleTab->update();
  }

  showScheduleRuleset(schedule);
}

void SchedulesView::showAddRulePage(const model::ScheduleRuleset & scheduleRuleset)
{
  this->setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  NewRuleView * newRuleView = new NewRuleView(scheduleRuleset, this);

  m_contentLayout->addWidget(newRuleView,100);

  this->setUpdatesEnabled(true);
}

void SchedulesView::showScheduleRuleset(const model::ScheduleRuleset & schedule)
{
  this->setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  ScheduleRulesetView * scheduleRulesetView = new ScheduleRulesetView(schedule);

  m_contentLayout->addWidget(scheduleRulesetView,100);

  this->setUpdatesEnabled(true);
}

void SchedulesView::showScheduleRule(model::ScheduleRule scheduleRule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  ScheduleRuleView * scheduleView = new ScheduleRuleView(m_isIP,scheduleRule,this);
  
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             scheduleView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_contentLayout->addWidget(scheduleView);

  connect(scheduleRule.getImpl<openstudio::detail::WorkspaceObject_Impl>().get(),SIGNAL(onRemoveFromWorkspace(Handle)),
          this,SLOT(onScheduleRuleRemoved(Handle)));

  scheduleView->show();

  setUpdatesEnabled(true);
}

void SchedulesView::onScheduleRuleRemoved(Handle handle)
{
  showEmptyPage();
}

void SchedulesView::showDefaultScheduleDay(const model::ScheduleRuleset & schedule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  DefaultScheduleDayView * scheduleView = new DefaultScheduleDayView(m_isIP,schedule,this);
    
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             scheduleView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_contentLayout->addWidget(scheduleView);

  scheduleView->show();

  setUpdatesEnabled(true);
}

void SchedulesView::showSummerScheduleDay(model::ScheduleRuleset schedule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  if( ! schedule.isSummerDesignDayScheduleDefaulted() )
  {
    SizingScheduleDayView * scheduleView = new SizingScheduleDayView(m_isIP, schedule,this,SizingScheduleDayView::SUMMER);

    bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                               scheduleView, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);

    m_contentLayout->addWidget(scheduleView);

    scheduleView->show();
  }
  else
  {
    NewRuleView * newRuleView = new NewRuleView(schedule, this, NewRuleView::SUMMER);

    m_contentLayout->addWidget(newRuleView,100);
  }

  setUpdatesEnabled(true);
}

void SchedulesView::showWinterScheduleDay(model::ScheduleRuleset schedule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  if( ! schedule.isWinterDesignDayScheduleDefaulted() )
  {
    SizingScheduleDayView * scheduleView = new SizingScheduleDayView(m_isIP,schedule,this,SizingScheduleDayView::WINTER);
        
    bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                               scheduleView, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);

    m_contentLayout->addWidget(scheduleView);

    scheduleView->show();
  }
  else
  {
    NewRuleView * newRuleView = new NewRuleView(schedule, this, NewRuleView::WINTER);

    m_contentLayout->addWidget(newRuleView,100);
  }

  setUpdatesEnabled(true);
}

void SchedulesView::showEmptyPage()
{
  this->setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != 0) 
  {
    delete child->widget();

    delete child;
  }

  QWidget * emptyWidget = new QWidget();

  m_contentLayout->addWidget(emptyWidget,100);

  this->setUpdatesEnabled(true);
}

boost::optional<model::ScheduleRuleset> SchedulesView::currentSchedule()
{
  for ( int i = 0;
        i < m_leftVLayout->count() - 1; 
        i++ )
  {
    ScheduleTab * scheduleTab;
    
    scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if( scheduleTab->selected() )
    {
      if( boost::optional<model::ScheduleRuleset> schedule = scheduleTab->schedule() )
      {
        if( ! schedule->handle().isNull() ) return schedule;
      }
    }
  }

  return boost::none;
}

void SchedulesView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// ***** SLOTS *****

void SchedulesView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

ScheduleRuleView::ScheduleRuleView(bool isIP,
                                   const model::ScheduleRule & scheduleRule,
                                   SchedulesView * schedulesView)
  : QWidget(schedulesView),
    m_schedulesView(schedulesView),
    m_scheduleRule(scheduleRule),
    m_dirty(false)
{
  m_yearDescription = m_scheduleRule.model().getUniqueModelObject<model::YearDescription>();

  // Layout

  QHBoxLayout * mainHLayout = new QHBoxLayout();

  mainHLayout->setContentsMargins(0,0,0,0);

  mainHLayout->setSpacing(0);

  setLayout(mainHLayout);

  QVBoxLayout * mainVLayout = new QVBoxLayout();

  mainVLayout->setContentsMargins(10,10,10,10);

  mainVLayout->setSpacing(0);

  mainHLayout->addLayout(mainVLayout,100);

  // Rule Settings

  QHBoxLayout * ruleHLayout = new QHBoxLayout();
  ruleHLayout->setContentsMargins(0,0,0,0);
  ruleHLayout->setSpacing(0);
  mainVLayout->addLayout(ruleHLayout);

  QWidget * ruleWidget = new QWidget();
  ruleHLayout->addWidget(ruleWidget);
  ruleHLayout->addStretch(10);

  QPushButton * removeButton = new QPushButton();
  removeButton->setObjectName("DeleteButton");
  removeButton->setFlat(true);
  removeButton->setFixedSize(24,24);
  connect(removeButton,SIGNAL(clicked()),this,SLOT(onRemoveClicked()));
  ruleHLayout->addWidget(removeButton,0,Qt::AlignTop);
  ruleHLayout->addSpacing(MARGINRIGHT);

  QVBoxLayout * ruleVLayout = new QVBoxLayout();
  ruleVLayout->setContentsMargins(0,0,0,0);
  ruleVLayout->setSpacing(5);
  ruleWidget->setLayout(ruleVLayout);

  QHBoxLayout * nameHLayout = new QHBoxLayout();
  nameHLayout->setContentsMargins(0,0,0,0);

  QWidget * colorWidget = new QWidget();
  colorWidget->setFixedWidth(20);
  QString colorStyle;
  colorStyle.append("QWidget { background-color: ");
  int colorIndex = m_scheduleRule.ruleIndex();
  if( colorIndex > 12 )
  {
    colorIndex = 12;
  }
  QColor color = m_schedulesView->colors[colorIndex];
  colorStyle.append(color.name());
  colorStyle.append("; }");
  colorWidget->setStyleSheet(colorStyle);

  nameHLayout->addWidget(colorWidget);

  m_nameEditField = new OSLineEdit();
  m_nameEditField->bind(m_scheduleRule,"name");
  nameHLayout->addWidget(m_nameEditField);

  ruleVLayout->addLayout(nameHLayout);

  QHBoxLayout * dateHLayout = new QHBoxLayout();

  QLabel * dateRangeLabel = new QLabel("Date Range:");
  dateHLayout->addWidget(dateRangeLabel);
  
  m_startDateEdit = new QDateTimeEdit();
  m_startDateEdit->setDisplayFormat("MM/dd");
  m_startDateEdit->setCalendarPopup(true);
  dateHLayout->addWidget(m_startDateEdit);
  connect(m_startDateEdit,SIGNAL(dateTimeChanged(const QDateTime &)),this,SLOT(onStartDateTimeChanged(const QDateTime &)));

  m_endDateEdit = new QDateTimeEdit();
  m_endDateEdit->setDisplayFormat("MM/dd");
  m_endDateEdit->setCalendarPopup(true);
  dateHLayout->addWidget(m_endDateEdit);
  connect(m_endDateEdit,SIGNAL(dateTimeChanged(const QDateTime &)),this,SLOT(onEndDateTimeChanged(const QDateTime &)));

  dateHLayout->addStretch();

  ruleVLayout->addLayout(dateHLayout);

  QHBoxLayout * weekHLayout = new QHBoxLayout();
  QLabel * applyToLabel = new QLabel("Apply to:");
  weekHLayout->addWidget(applyToLabel);

  weekHLayout->addStretch();

  m_sundayButton = new OSCheckBox2();
  m_sundayButton->setText("S");
  m_sundayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applySunday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplySunday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_sundayButton);

  weekHLayout->addSpacing(10);

  m_mondayButton = new OSCheckBox2();
  m_mondayButton->setText("M");
  m_mondayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applyMonday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplyMonday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_mondayButton);

  weekHLayout->addSpacing(10);

  m_tuesdayButton = new OSCheckBox2();
  m_tuesdayButton->setText("T");
  m_tuesdayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applyTuesday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplyTuesday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_tuesdayButton);

  weekHLayout->addSpacing(10);

  m_wednesdayButton = new OSCheckBox2();
  m_wednesdayButton->setText("W");
  m_wednesdayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applyWednesday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplyWednesday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_wednesdayButton);

  weekHLayout->addSpacing(10);

  m_thursdayButton = new OSCheckBox2();
  m_thursdayButton->setText("T");
  m_thursdayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applyThursday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplyThursday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_thursdayButton);

  weekHLayout->addSpacing(10);

  m_fridayButton = new OSCheckBox2();
  m_fridayButton->setText("F");
  m_fridayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applyFriday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplyFriday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_fridayButton);

  weekHLayout->addSpacing(10);

  m_saturdayButton = new OSCheckBox2();
  m_saturdayButton->setText("S");
  m_saturdayButton->bind(
      m_scheduleRule,
      boost::bind(&model::ScheduleRule::applySaturday,&m_scheduleRule),
      boost::optional<BoolSetter>(boost::bind(&model::ScheduleRule::setApplySaturday,&m_scheduleRule,_1)));
  weekHLayout->addWidget(m_saturdayButton);

  ruleVLayout->addLayout(weekHLayout);

  // Schedule Day

  m_scheduleDayView = new ScheduleDayView(isIP,m_scheduleRule.daySchedule(),m_schedulesView);
    
  bool isConnected = false;

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_scheduleDayView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);  
  
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_scheduleDayView, SLOT(onToggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  mainVLayout->addWidget(m_scheduleDayView);

  // Year Overview

  QWidget * vLine2 = new QWidget();
  vLine2->setObjectName("VLine");
  vLine2->setStyleSheet("QWidget#VLine { background: #445051;}");
  vLine2->setFixedWidth(2);
  mainHLayout->addWidget(vLine2);

  YearOverview * yearOverview = new YearOverview(m_scheduleRule.scheduleRuleset());
  mainHLayout->addWidget(yearOverview);

  // Connect

  connect( m_scheduleRule.getImpl<openstudio::model::detail::ScheduleRule_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(scheduleRefresh()) );

  connect( m_yearDescription->getImpl<openstudio::model::detail::YearDescription_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(scheduleRefresh()) );

  connect( this,SIGNAL(startDateTimeChanged(model::ScheduleRule &, const QDateTime &)),
           m_schedulesView,SIGNAL(startDateTimeChanged(model::ScheduleRule &, const QDateTime &)) );

  connect( this,SIGNAL(endDateTimeChanged(model::ScheduleRule &, const QDateTime &)),
           m_schedulesView,SIGNAL(endDateTimeChanged(model::ScheduleRule &, const QDateTime &)) );

  connect( this,SIGNAL(removeScheduleRuleClicked(model::ScheduleRule &)),
           m_schedulesView,SIGNAL(removeScheduleRuleClicked(model::ScheduleRule &)) );

  // Refresh

  scheduleRefresh();
}

void ScheduleRuleView::onRemoveClicked()
{
  //m_scheduleRule.remove();
  m_scheduleRule.getImpl<openstudio::model::detail::ScheduleRule_Impl>();

  emit removeScheduleRuleClicked(m_scheduleRule);
}

model::ScheduleRule ScheduleRuleView::scheduleRule() const
{
  return m_scheduleRule;
}

void ScheduleRuleView::refresh()
{
  if( m_dirty )
  {
    m_startDateEdit->blockSignals(true);
    m_endDateEdit->blockSignals(true);

    QDate minDate(QDate(m_yearDescription->assumedYear(),1,1));
    QDate maxDate(QDate(m_yearDescription->assumedYear(),12,31));

    m_startDateEdit->setDateRange(minDate,maxDate);
    m_endDateEdit->setDateRange(minDate,maxDate);

    boost::optional<Date> startDate = m_scheduleRule.startDate();
    if( startDate )
    {
      QDate qstartDate(startDate->year(),startDate->monthOfYear().value(),startDate->dayOfMonth());
    
      m_startDateEdit->setDate(qstartDate);

      m_startDateEdit->calendarWidget()->setSelectedDate(qstartDate);
    }

    boost::optional<Date> endDate = m_scheduleRule.endDate();
    if( endDate )
    {
      QDate qendDate(endDate->year(),endDate->monthOfYear().value(),endDate->dayOfMonth());
    
      m_endDateEdit->setDate(qendDate);

      m_endDateEdit->calendarWidget()->setSelectedDate(qendDate);
    }

    m_startDateEdit->blockSignals(false);
    m_endDateEdit->blockSignals(false);

    m_dirty = false;
  }
}

void ScheduleRuleView::scheduleRefresh()
{
  m_dirty = true;
  
  QTimer::singleShot(0,this,SLOT(refresh()));
}

void ScheduleRuleView::onStartDateTimeChanged(const QDateTime & newDate)
{
  emit startDateTimeChanged(m_scheduleRule,newDate);
}

void ScheduleRuleView::onEndDateTimeChanged(const QDateTime & newDate)
{
  emit endDateTimeChanged(m_scheduleRule,newDate);
}

DefaultScheduleDayView::DefaultScheduleDayView( bool isIP,
                                                const model::ScheduleRuleset & scheduleRuleset,
                                                SchedulesView * schedulesView )
  : QWidget(schedulesView)
{
  // Layout

  QHBoxLayout * mainHLayout = new QHBoxLayout();

  mainHLayout->setContentsMargins(0,0,0,0);

  mainHLayout->setSpacing(0);

  setLayout(mainHLayout);

  QVBoxLayout * mainVLayout = new QVBoxLayout();

  mainVLayout->setContentsMargins(10,10,10,10);

  mainVLayout->setSpacing(0);

  mainHLayout->addLayout(mainVLayout,100);

  // Schedule Day

  ScheduleDayView * scheduleDayView = new ScheduleDayView(isIP,scheduleRuleset.defaultDaySchedule(),schedulesView); 
    
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             scheduleDayView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  mainVLayout->addWidget(scheduleDayView);

  // Year Overview

  QWidget * vLine2 = new QWidget();
  vLine2->setObjectName("VLine");
  vLine2->setStyleSheet("QWidget#VLine { background: #445051;}");
  vLine2->setFixedWidth(2);
  mainHLayout->addWidget(vLine2);

  YearOverview * yearOverview = new YearOverview(scheduleRuleset);
  mainHLayout->addWidget(yearOverview);
}

SizingScheduleDayView::SizingScheduleDayView( bool isIP,
                                              const model::ScheduleRuleset & scheduleRuleset,
                                              SchedulesView * schedulesView,
                                              ViewType type )
  : QWidget(schedulesView),
    m_type(type)
{
  // Layout

  QHBoxLayout * mainHLayout = new QHBoxLayout();

  mainHLayout->setContentsMargins(0,0,0,0);

  mainHLayout->setSpacing(0);

  setLayout(mainHLayout);

  QVBoxLayout * mainVLayout = new QVBoxLayout();

  mainVLayout->setContentsMargins(10,10,10,10);

  mainVLayout->setSpacing(0);

  mainHLayout->addLayout(mainVLayout,100);

  // Schedule Day

  if( m_type == SUMMER )
  {
    if( ! scheduleRuleset.isSummerDesignDayScheduleDefaulted() )
    {
      QLabel * label = new QLabel("Summer design day profile.");

      mainVLayout->addWidget(label);

      model::ScheduleDay scheduleDay = scheduleRuleset.summerDesignDaySchedule();

      ScheduleDayView * scheduleDayView = new ScheduleDayView(isIP,scheduleDay,schedulesView);

      bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                                 scheduleDayView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);

      mainVLayout->addWidget(scheduleDayView);
    }
  }
  else
  {
    if( ! scheduleRuleset.isWinterDesignDayScheduleDefaulted() )
    {
      QLabel * label = new QLabel("Winter design day profile.");

      mainVLayout->addWidget(label);

      model::ScheduleDay scheduleDay = scheduleRuleset.winterDesignDaySchedule();

      ScheduleDayView * scheduleDayView = new ScheduleDayView(isIP,scheduleDay,schedulesView);

      bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                                 scheduleDayView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);

      mainVLayout->addWidget(scheduleDayView);
    }
  }
}

ScheduleDayView::ScheduleDayView(bool isIP,
                                 const model::ScheduleDay & scheduleDay,
                                 SchedulesView * schedulesView)
  : QWidget(schedulesView),
    m_zoom(1.0),
    m_snap(3600.0),
    m_scheduleDay(scheduleDay),
    m_schedulesView(schedulesView),
    m_dayScheduleScene(NULL),
    m_scheduleDayEditor(NULL),
    m_scheduleOverview(NULL),
    m_upperLimitSpinBox(NULL),
    m_lowerLimitSpinBox(NULL),
    m_dirty(false),
    m_isIP(isIP)
{
  bool isConnected = false;

  // Note: QDoubleSpinBox are used, rather than OSQuantityEdit
  // because limits are not connected via Q_PROPERTY. 
  m_lowerLimitSpinBox = new QDoubleSpinBox();
  double lowerLimitSpinBoxValue = LOWER_LIMIT;
  model::OptionalScheduleTypeLimits typeLimits = scheduleDay.scheduleTypeLimits();
  if (typeLimits) {
    OSOptionalQuantity lowerLimitQ = typeLimits->getLowerLimitValue(m_isIP);
    if (lowerLimitQ.isSet()) {
      lowerLimitSpinBoxValue = lowerLimitQ.get().value();
    }
    else {
      if (OptionalDouble temp = typeLimits->lowerLimitValue()) {
        lowerLimitSpinBoxValue = *temp;
      }
    }
  }  
  m_lowerLimitSpinBox->setValue(lowerLimitSpinBoxValue);
  m_lowerLimitSpinBox->setMinimum(-1E10);
  isConnected = connect(m_lowerLimitSpinBox,SIGNAL(valueChanged(double)),
                        this,SLOT(onLowerValueChanged(double)));
  OS_ASSERT(isConnected);

  m_upperLimitSpinBox = new QDoubleSpinBox();
  double upperLimitSpinBoxValue = UPPER_LIMIT;
  if (typeLimits) {
    OSOptionalQuantity upperLimitQ = typeLimits->getUpperLimitValue(m_isIP);
    if (upperLimitQ.isSet()) {
      upperLimitSpinBoxValue = upperLimitQ.get().value();
    }
    else {
      if (OptionalDouble temp = typeLimits->upperLimitValue()) {
        upperLimitSpinBoxValue = *temp;
      }
    }
  }
  m_upperLimitSpinBox->setValue(upperLimitSpinBoxValue);
  m_upperLimitSpinBox->setMaximum(1E10);
  isConnected = connect(m_upperLimitSpinBox,SIGNAL(valueChanged(double)),
                        this,SLOT(onUpperValueChanged(double)));
  OS_ASSERT(isConnected);

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        this,SLOT(onToggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  // Scene

  m_dayScheduleScene = new DayScheduleScene(this,m_scheduleDay);

  m_dayScheduleScene->setSceneRect(0,0,SCENEWIDTH,SCENEHEIGHT);

  // Layout

  QVBoxLayout * mainVLayout = new QVBoxLayout();

  mainVLayout->setContentsMargins(0,0,0,0);

  mainVLayout->setSpacing(10);

  setLayout(mainVLayout);

  // Name

  QHBoxLayout * hLayout = NULL;

  QLabel * label = NULL;

  label = new QLabel("Name:");
  label->setObjectName("H2");
  mainVLayout->addWidget(label);

  QString name;
  boost::optional<model::ScheduleRuleset> currentSchedule = schedulesView->currentSchedule();
  if(currentSchedule){
    boost::optional<std::string> optionalString = currentSchedule->name();
    if(optionalString){
      name = optionalString->c_str();
    }
  }
  QLineEdit * lineEdit = new QLineEdit(name);
  lineEdit->setReadOnly(true);

  mainVLayout->addWidget(lineEdit);

  // Lower Limit

  hLayout = new QHBoxLayout();
  mainVLayout->addLayout(hLayout);

  label = new QLabel("Lower Limit");
  hLayout->addWidget(label);

  hLayout->addWidget(m_lowerLimitSpinBox);

  hLayout->addSpacing(20);

  // Upper Limit

  label = new QLabel("Upper Limit");
  hLayout->addWidget(label);

  hLayout->addWidget(m_upperLimitSpinBox);

  hLayout->addStretch();

  // Day Schedule

  m_scheduleDayEditor = new ScheduleDayEditor(isIP, this, m_scheduleDay );

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_scheduleDayEditor, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

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
 
  connect(hourlyZoomButton,SIGNAL(clicked()),this,SLOT(setHourlyZoom()));

  zoomButtonLayout->addWidget(hourlyZoomButton);
  zoomButtonLayout->addSpacing(10);

  QPushButton * quarterHourlyZoomButton = new QPushButton();
  quarterHourlyZoomButton->setObjectName("StandardGrayButton");

  quarterHourlyZoomButton->setCheckable(true);

  zoomButtonGroup->addButton(quarterHourlyZoomButton);

  quarterHourlyZoomButton->setText("15 Minutes");
 
  connect(quarterHourlyZoomButton,SIGNAL(clicked()),this,SLOT(setQuarterHourlyZoom()));

  zoomButtonLayout->addWidget(quarterHourlyZoomButton);
  zoomButtonLayout->addSpacing(10);

  QPushButton * oneMinuteZommButton = new QPushButton();
  oneMinuteZommButton->setObjectName("StandardGrayButton");

  oneMinuteZommButton->setCheckable(true);

  zoomButtonGroup->addButton(oneMinuteZommButton);
  
  oneMinuteZommButton->setText("1 Minute");
 
  connect(oneMinuteZommButton,SIGNAL(clicked()),this,SLOT(setOneMinuteZoom()));

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
}

double ScheduleDayView::upperLimit() const
{
  if(m_upperLimitSpinBox){
    return m_upperLimitSpinBox->value();
  }
  else{
    return UPPER_LIMIT;
  }
}

void ScheduleDayView::setUpperLimit(double value)
{
  if(m_upperLimitSpinBox){
    m_upperLimitSpinBox->setValue(value);
    m_upperLimitSpinBox->setMinimum(value);
  }
}

double ScheduleDayView::lowerLimit() const
{
  if(m_lowerLimitSpinBox){
    return m_lowerLimitSpinBox->value();
  }
  else{
    return LOWER_LIMIT;
  }
}

void ScheduleDayView::setLowerLimit(double value)
{
  if(m_lowerLimitSpinBox){
    m_lowerLimitSpinBox->setValue(value);
    m_lowerLimitSpinBox->setMaximum(value);
  }
}

void ScheduleDayView::onLowerValueChanged(double d)
{
  m_upperLimitSpinBox->setMinimum(d);
  if(scene()){
    scene()->scheduleRefresh();
  }
}

void ScheduleDayView::onUpperValueChanged(double d)
{
  m_lowerLimitSpinBox->setMaximum(d);
  if(scene()){
    scene()->scheduleRefresh();
  }
}

bool ScheduleDayView::isIP() const {
  return m_isIP;
}

void ScheduleDayView::onToggleUnitsClicked(bool displayIP) {
  model::OptionalScheduleTypeLimits typeLimits = m_scheduleDay.scheduleTypeLimits();
  if (typeLimits) {
    Unit oldUnits = typeLimits->getLowerLimitValue(m_isIP).units();
    Unit newUnits = typeLimits->getLowerLimitValue(displayIP).units();
    if (oldUnits != newUnits) {
      Quantity oldLowerLimit(lowerLimit(),oldUnits);
      OptionalQuantity newLowerLimit = convert(oldLowerLimit,newUnits);
      if (newLowerLimit) {
        setLowerLimit(newLowerLimit->value());
      }
      Quantity oldUpperLimit(upperLimit(),oldUnits);
      OptionalQuantity newUpperLimit = convert(oldUpperLimit,newUnits);
      if (newUpperLimit) {
        setUpperLimit(newUpperLimit->value());
      }
    }
  }
  m_isIP = displayIP;
  if (scene()) {
    scene()->scheduleRefresh();
  }
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

ScheduleDayEditor::ScheduleDayEditor(bool isIP, ScheduleDayView * scheduleDayView, const model::ScheduleDay & scheduleDay) : 
  QWidget(scheduleDayView),
  m_scheduleDayView(scheduleDayView),
  m_scheduleDay(scheduleDay),
  m_dirty(true),
  m_isIP(isIP)
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
  setLabelText();

  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

  //connect( m_scheduleDay.scheduleTypeLimits().getImpl<model::detail::ScheduleTypeLimits_Impl>().get(),
  //         SIGNAL(onChange()),
  //         this,
  //         SLOT(scheduleRefresh()) );

  connect( this, SIGNAL(changeVerticalAxisClicked(model::ScheduleDay)),
           m_scheduleDayView->schedulesView(),SIGNAL(changeVerticalAxisClicked(model::ScheduleDay)));

  QTimer::singleShot(0,this,SLOT(fitInView()));
}

void ScheduleDayEditor::setLabelText()
{
  boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = m_scheduleDay.scheduleTypeLimits();
  if (scheduleTypeLimits) {
    OSOptionalQuantity optionalQuantity = scheduleTypeLimits->getLowerLimitValue(m_isIP);
    std::stringstream ss;
    ss << optionalQuantity.units();
    QString text = QString::fromStdString(ss.str());
    m_yLabel->setText(text);
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

  m_yLabel->move(0, MARGINTOP + m_graphicsView->size().height() / 2 - buttonHeight / 2.0);

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

  if( abs(startTimeSeconds - prevSnapTime) < 0.00001 )
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

  double yStartValue = m_scheduleDayView->upperLimit();
  double yEndValue = m_scheduleDayView->lowerLimit();

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
}

// ***** SLOTS *****

void ScheduleDayEditor::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
  setLabelText();
}

CalendarSegmentItem::CalendarSegmentItem( QGraphicsItem * parent )
  : QGraphicsItem(parent),
    m_previousVCalendarItem(NULL),
    m_nextVCalendarItem(NULL),
    m_isHovering(false),
    m_mouseDown(false),
    m_endTime(SCENEWIDTH)
{
  setAcceptHoverEvents(true); 

  setToolTip("Double click to cut segment");

  setZValue(100);
}

double CalendarSegmentItem::vCenterPos() const
{
  return pos().y() + boundingRect().height() / 2.0;
}

CalendarSegmentItem * CalendarSegmentItem::nextCalendarItem() const
{
  if( VCalendarSegmentItem * item = nextVCalendarItem() )
  {
    return item->nextCalendarItem();
  }
  else
  {
    return NULL;
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
    return NULL;
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
    return NULL;
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

  double upperLimitValue = scene()->scheduleDayView()->upperLimit();
  double lowerLimitValue = scene()->scheduleDayView()->lowerLimit();

  float fullscalevalue = lowerLimitValue + value * (upperLimitValue - lowerLimitValue);

  QString tooltip = QString::number(fullscalevalue,'g',3);
  tooltip.append(" - Double click to cut segment"); 

  setToolTip(tooltip);
}

VCalendarSegmentItem::VCalendarSegmentItem( QGraphicsItem * parent )
  : QGraphicsItem(parent),
    m_isHovering(false)
{
  setAcceptHoverEvents(true);

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
    return NULL;
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

  double length = abs(nextY - prevY);

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
    item->setNextVCalendarItem(NULL);
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

DayScheduleOverview::DayScheduleOverview(ScheduleDayView * scheduleRuleView)
  : QWidget(scheduleRuleView),
    m_lastMousePos(0)
{
  m_graphicsView = new QGraphicsView(this);

  m_graphicsView->move(0.0,0.0);

  m_scheduleRuleView = scheduleRuleView;

  setFixedHeight(100);

  m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  m_graphicsView->setScene(m_scheduleRuleView->scene());

  // Focus Rectangle

  m_focusRectangle = new QWidget(this);

  m_focusRectangle->setStyleSheet("QWidget { border: 1px solid black; }");

  QTimer::singleShot(0,this,SLOT(fitInView()));

  updateFocusRectangleGeometry();

  connect(m_scheduleRuleView,SIGNAL(zoomChanged(double)),this,SLOT(onZoomChange(double)));
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
    m_currentItem(NULL),
    m_currentHoverItem(NULL)
{
  connect(this,SIGNAL(dayScheduleSceneChanged( DayScheduleScene *, double, double )),
          m_scheduleDayEditor->scheduleDayView()->schedulesView(),SIGNAL(dayScheduleSceneChanged( DayScheduleScene *, double, double )));

  setFocusPolicy(Qt::StrongFocus);
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

      emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerLimit(),scene()->scheduleDayView()->upperLimit());
    }
    else if( VCalendarSegmentItem * calendarItem = dynamic_cast<VCalendarSegmentItem *>(item) )
    {
      calendarItem->remove();
      
      event->accept();

      emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerLimit(),scene()->scheduleDayView()->upperLimit());
    }
  }
}

void DaySchedulePlotArea::mouseMoveEvent(QMouseEvent * event)
{
  QPointF scenePos = mapToScene(event->pos()); 

  m_currentHoverItem = NULL;

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

      calendarItem->update();
    }
  }

  m_lastScenePos = scenePos;

  QGraphicsView::mouseMoveEvent(event);
}

void DaySchedulePlotArea::mousePressEvent(QMouseEvent * event)
{
  m_lastScenePos = mapToScene(event->pos()); 

  m_currentItem = NULL;

  m_currentHoverItem = NULL;

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

  emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerLimit(),scene()->scheduleDayView()->upperLimit());

  m_currentItem = NULL;

  QGraphicsView::mouseReleaseEvent(event);
}

void DaySchedulePlotArea::keyPressEvent(QKeyEvent * event)
{
  if( m_currentHoverItem )
  {
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
      ScheduleDayView * scheduleDayView = scene()->scheduleDayView();

      double upperLimit = scheduleDayView->upperLimit();
      double lowerLimit = scheduleDayView->lowerLimit();

      double value = m_keyboardInputValue.toDouble();

      double scaledValue = (value - lowerLimit) / (upperLimit - lowerLimit);

      m_currentHoverItem->setValue(scaledValue);

      m_keyboardInputValue.clear();

      m_currentHoverItem->setHovering(false);

      m_currentHoverItem = NULL;

      emit dayScheduleSceneChanged(scene(),scene()->scheduleDayView()->lowerLimit(),scene()->scheduleDayView()->upperLimit());
    }
    else if( event->key() == Qt::Key_Minus )
    {
      m_keyboardInputValue.clear();

      m_keyboardInputValue.append(event->text());
    }
    else if( event->key() == Qt::Key_0 || 
             event->key() == Qt::Key_1 ||
             event->key() == Qt::Key_2 ||
             event->key() == Qt::Key_3 ||
             event->key() == Qt::Key_4 ||
             event->key() == Qt::Key_5 ||
             event->key() == Qt::Key_6 ||
             event->key() == Qt::Key_7 ||
             event->key() == Qt::Key_8 ||
             event->key() == Qt::Key_9 ||
             event->key() == Qt::Key_Period
           )
    {
      m_keyboardInputValue.append(event->text());
    }
  }
}

DayScheduleScene::DayScheduleScene(ScheduleDayView * scheduleDayView, const model::ScheduleDay & scheduleDay)
  : QGraphicsScene(scheduleDayView),
    m_firstSegment(NULL),
    m_scheduleDayView(scheduleDayView),
    m_scheduleDay(scheduleDay),
    m_dirty(true)
{
  setSceneRect(0,0,SCENEWIDTH,SCENEHEIGHT);

  connect( m_scheduleDay.getImpl<openstudio::model::detail::ScheduleDay_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(scheduleRefresh()) );

  refresh();
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
      OSQuantityVector quantities = m_scheduleDay.getValues(m_scheduleDayView->isIP());
      realvalues = quantities.values();
    }
    else {
      realvalues = m_scheduleDay.values();
    }

    // The min and max values of the schedule as it appears on the screen
    double minvalue = * std::min_element(realvalues.begin(),realvalues.end());
    double maxvalue = * std::max_element(realvalues.begin(),realvalues.end());

    // The upper and lower limt are the limits input from the user input fields.
    double upperLimit = m_scheduleDayView->upperLimit();
    double lowerLimit = m_scheduleDayView->lowerLimit();

    if( maxvalue > upperLimit )
    {
      upperLimit = maxvalue; // + 0.05 * (maxvalue - minvalue);
      m_scheduleDayView->setUpperLimit(upperLimit);
    }

    if( minvalue < lowerLimit )
    {
      lowerLimit = minvalue; // - 0.05 * (maxvalue - minvalue);
      m_scheduleDayView->setLowerLimit(lowerLimit);
    }

    int i  = 0;
    double lastTime = 0.0;
    CalendarSegmentItem * previousSegment = NULL;

    for( std::vector<openstudio::Time>::iterator it = times.begin();
         it < times.end();
         ++it )
    {
      double scaledValue = (realvalues[i] - lowerLimit) / (upperLimit - lowerLimit);

      CalendarSegmentItem * segment = new CalendarSegmentItem();
      addItem(segment);
      segment->setValue(scaledValue);
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

  return NULL;
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

  m_firstSegment = NULL;
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

  QGraphicsItem * result = NULL;

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

ScheduleCalendarWidget::ScheduleCalendarWidget(MonthView * monthView)
  : QCalendarWidget(monthView),
    m_monthView(monthView)
{
}

void ScheduleCalendarWidget::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
  painter->setBrush(QBrush(QColor(230,230,230)));

  if( date.month() == m_monthView->month() )
  {
    int dayOfYear = date.dayOfYear();

    int ruleIndex = m_monthView->yearOverview()->activeRuleIndices()[dayOfYear - 1];

    QColor ruleColor = SchedulesView::colors[12];

    if( ruleIndex > 12 )
    {
      ruleIndex = 12;
    }

    if( ruleIndex > -1 )
    {
      ruleColor = SchedulesView::colors[ruleIndex];
    }

    QString dateString = QString::number(date.day());

    painter->setBrush(QBrush(ruleColor));

    painter->drawRect(rect);

    painter->drawText(rect,Qt::AlignHCenter | Qt::AlignVCenter,dateString);
  }
  else
  {
    painter->drawRect(rect);
  }
}

MonthView::MonthView( YearOverview * yearOverview )
  : QWidget(yearOverview),
    m_yearOverview(yearOverview),
    m_month(0)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();

  setLayout(mainVLayout);

  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);

  m_monthLabel = new QLabel("January");

  mainVLayout->addWidget(m_monthLabel,0,Qt::AlignCenter);

  mainVLayout->addSpacing(5);

  m_calendarWidget = new ScheduleCalendarWidget(this);
  m_calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
  m_calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
  m_calendarWidget->setNavigationBarVisible(false);
  m_calendarWidget->setSelectionMode(QCalendarWidget::NoSelection);

  mainVLayout->addWidget(m_calendarWidget);

  QFrame * line1 = new QFrame();
  line1->setFrameShape(QFrame::HLine);
  line1->setFixedHeight(1);
  
  mainVLayout->addWidget(line1);
}

int MonthView::month() const
{
  return m_month;
}

YearOverview * MonthView::yearOverview() const
{
  return m_yearOverview;
}

void MonthView::setMonth(int month)
{
  m_month = month;

  std::string monthName = monthOfYear(month).valueName();

  m_monthLabel->setText(QString::fromStdString(monthName));

  update();
}

void MonthView::update()
{
  int year = m_yearOverview->scheduleRuleset().model().getUniqueModelObject<model::YearDescription>().assumedYear();

  QDate startDate(year,m_month,1);
  QDate endDate(year,m_month,startDate.daysInMonth());

  m_calendarWidget->setMinimumDate(startDate);
  m_calendarWidget->setMaximumDate(endDate);

  m_calendarWidget->update();

  QWidget::update();
}

YearOverview::YearOverview( const model::ScheduleRuleset & scheduleRuleset, QWidget * parent )
  : QWidget(parent),
    m_scheduleRuleset(scheduleRuleset),
    m_dirty(false)
{
  refreshActiveRuleIndices();

  QVBoxLayout * mainScrollLayout = new QVBoxLayout();

  mainScrollLayout->setContentsMargins(0,0,0,0);

  setLayout(mainScrollLayout);

  QScrollArea * scrollArea = new QScrollArea();
  scrollArea->setFrameStyle(QFrame::NoFrame);
  scrollArea->setWidgetResizable(true);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  mainScrollLayout->addWidget(scrollArea);

  QWidget * scrollWidget = new QWidget();

  scrollArea->setWidget(scrollWidget);

  QVBoxLayout * mainLayout = new QVBoxLayout();

  mainLayout->setContentsMargins(0,0,0,0);

  scrollWidget->setLayout(mainLayout); 

  QGridLayout * gridLayout = new QGridLayout();

  gridLayout->setContentsMargins(0,0,0,0);

  m_januaryView = new MonthView(this);
  m_januaryView->setMonth(1);
  gridLayout->addWidget(m_januaryView,0,0);

  m_februaryView = new MonthView(this);
  m_februaryView->setMonth(2);
  gridLayout->addWidget(m_februaryView,1,0);

  m_marchView = new MonthView(this);
  m_marchView->setMonth(3);
  gridLayout->addWidget(m_marchView,2,0);

  m_aprilView = new MonthView(this);
  m_aprilView->setMonth(4);
  gridLayout->addWidget(m_aprilView,3,0);

  m_mayView = new MonthView(this);
  m_mayView->setMonth(5);
  gridLayout->addWidget(m_mayView,4,0);

  m_juneView = new MonthView(this);
  m_juneView->setMonth(6);
  gridLayout->addWidget(m_juneView,5,0);

  m_julyView = new MonthView(this);
  m_julyView->setMonth(7);
  gridLayout->addWidget(m_julyView,6,0);

  m_augustView = new MonthView(this);
  m_augustView->setMonth(8);
  gridLayout->addWidget(m_augustView,7,0);

  m_septemberView = new MonthView(this);
  m_septemberView->setMonth(9);
  gridLayout->addWidget(m_septemberView,8,0);

  m_octoberView = new MonthView(this);
  m_octoberView->setMonth(10);
  gridLayout->addWidget(m_octoberView,9,0);

  m_novemberView = new MonthView(this);
  m_novemberView->setMonth(11);
  gridLayout->addWidget(m_novemberView,10,0);

  m_decemberView = new MonthView(this);
  m_decemberView->setMonth(12);
  gridLayout->addWidget(m_decemberView,11,0);

  mainLayout->addLayout(gridLayout);

  mainLayout->addStretch(10);

  connect(m_scheduleRuleset.model().getImpl<openstudio::model::detail::Model_Impl>().get(),
          SIGNAL(addWorkspaceObject(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>,
                                    const openstudio::IddObjectType&, const openstudio::UUID&)),
          this,
          SLOT(onModelAdd(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
          Qt::QueuedConnection);

  std::vector<model::ScheduleRule> scheduleRules = m_scheduleRuleset.scheduleRules();

  for( std::vector<model::ScheduleRule>::iterator it = scheduleRules.begin();
       it < scheduleRules.end();
       ++it )
  {
    connect( it->getImpl<openstudio::model::detail::ScheduleRule_Impl>().get(),
             SIGNAL(onChange()),
             this,
             SLOT(scheduleRefresh()) );
  }

  model::YearDescription yearDescription = m_scheduleRuleset.model().getUniqueModelObject<model::YearDescription>();

  connect( yearDescription.getImpl<openstudio::model::detail::YearDescription_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(scheduleRefresh()) );

  refresh();
}

void YearOverview::refreshActiveRuleIndices()
{
  int year = m_scheduleRuleset.model().getUniqueModelObject<model::YearDescription>().assumedYear();

  Date startDate(1,1,year);
  Date endDate(12,31,year);

  m_activeRuleIndices = m_scheduleRuleset.getActiveRuleIndices(startDate,endDate);

  m_dirty = false;
}

void YearOverview::onModelAdd( boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo, 
                               const openstudio::IddObjectType& iddType, 
                               const openstudio::UUID& uuid )
{
  boost::optional<model::ScheduleRule> scheduleRule = m_scheduleRuleset.model().getModelObject<model::ScheduleRule>(wo->handle());

  if( scheduleRule )
  {
    if( scheduleRule->scheduleRuleset().handle() == m_scheduleRuleset.handle() )
    {
      connect( scheduleRule->getImpl<openstudio::model::detail::ScheduleRule_Impl>().get(),
               SIGNAL(onChange()),
               this,
               SLOT(scheduleRefresh()) );
    }
  }
}

std::vector<int> YearOverview::activeRuleIndices() const
{
  return m_activeRuleIndices;
}

void YearOverview::scheduleRefresh()
{
  m_dirty = true;
  
  QTimer::singleShot(0,this,SLOT(refresh()));
}

void YearOverview::refresh()
{
  if( m_dirty )
  {
    refreshActiveRuleIndices();
    
    m_januaryView->update();
    m_februaryView->update();
    m_marchView->update();
    m_aprilView->update();
    m_mayView->update();
    m_juneView->update();
    m_julyView->update();
    m_augustView->update();
    m_septemberView->update();
    m_octoberView->update();
    m_novemberView->update();
    m_decemberView->update();
  }

  update();
}

model::ScheduleRuleset YearOverview::scheduleRuleset() const
{
  return m_scheduleRuleset;
}

ScheduleTabHeader::ScheduleTabHeader( ScheduleTab * scheduleTab, QWidget * parent )
  : QWidget(parent),
    m_scheduleTab(scheduleTab),
    m_mouseDown(false),
    m_dirty(true)
{
  setObjectName("ScheduleTabHeader");

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  QString style;

  style.append("QWidget#ScheduleTabHeader { ");
  style.append("background-color: #CECECE;");
  style.append("}");
  setStyleSheet(style);

  m_mainLabel = new QLabel(this);
  mainHLayout->addWidget(m_mainLabel,10);
  m_mainLabel->setWordWrap(true);
  //m_mainLabel->setFixedWidth(120);

  mainHLayout->addStretch();

  m_toggleButton = new QPushButton();
  m_toggleButton->setFlat(true);
  m_toggleButton->setCheckable(true);
  m_toggleButton->setChecked(false);
  m_toggleButton->setEnabled(false);
  connect(m_toggleButton,SIGNAL(toggled(bool)),this,SIGNAL(toggleHeaderClicked(bool)));
  mainHLayout->addWidget(m_toggleButton);
  style.clear();
  style.append("QPushButton { ");
  style.append("border: none; ");
  style.append("background-image: url(\":/images/toggle_arrow.png\"); ");
  style.append("} ");

  style.append("QPushButton:checked { ");
  style.append("background-image: url(\":/images/toggle_arrow_closed.png\"); ");
  style.append("} ");
  m_toggleButton->setFixedSize(11,11);
  m_toggleButton->setStyleSheet(style);

  setFixedHeight(50);

  m_selectionWidget = new QWidget(this);
  m_selectionWidget->setFixedHeight(50);
  style.clear();
  style.append("QWidget { ");
  style.append("background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,");
  style.append(" stop: 0.0 #636161,");
  style.append(" stop: 0.10 #636161,");
  style.append(" stop: 0.15 #A3A3A3,");
  style.append(" stop: 1.0 #A3A3A3);");
  style.append("}");
  m_selectionWidget->setStyleSheet(style);

  m_selectionWidget->hide();

  refresh();

  connect(this,SIGNAL(scheduleClicked(const model::ScheduleRuleset &)),m_scheduleTab,SIGNAL(scheduleClicked(const model::ScheduleRuleset &)));

  connect( m_scheduleTab->schedule().getImpl<openstudio::model::detail::ScheduleRuleset_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(refresh()) );
}

void ScheduleTabHeader::expand()
{
  m_toggleButton->setChecked(false);
}

void ScheduleTabHeader::collapse()
{
  m_toggleButton->setChecked(true);
}

void ScheduleTabHeader::toggle()
{
  m_toggleButton->toggle();
}

void ScheduleTabHeader::refresh()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(refreshNow()));
}

void ScheduleTabHeader::refreshNow()
{
  if( m_dirty )
  {
    setText(QString::fromStdString(m_scheduleTab->schedule().name().get()));
  
    m_dirty = false;
  }
}

void ScheduleTabHeader::mouseReleaseEvent( QMouseEvent * event )
{
  if( m_mouseDown )
  {
    emit scheduleClicked(scheduleTab()->schedule());
  }

  m_mouseDown = false;

  event->accept();
}

void ScheduleTabHeader::mousePressEvent( QMouseEvent * event )
{
  m_mouseDown = true;

  event->accept();
}

ScheduleTab * ScheduleTabHeader::scheduleTab() const
{
  return m_scheduleTab;
}

void ScheduleTabHeader::setSelected(bool selected)
{
  m_mainLabel->raise();

  m_toggleButton->raise();

  if( selected )
  {
    m_selectionWidget->show();
  }
  else
  {
    m_selectionWidget->hide();
  }
}

void ScheduleTabHeader::setText(const QString & text)
{
  m_mainLabel->setText(text);
}

void ScheduleTabHeader::resizeEvent ( QResizeEvent * event )
{
  m_selectionWidget->resize(event->size().width(),67);

  QWidget::resizeEvent(event);
}

void ScheduleTabHeader::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ScheduleTabContent::ScheduleTabContent( ScheduleTab * scheduleTab, QWidget * parent )
  : QWidget(parent),
    m_scheduleTab(scheduleTab)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(5,5,5,5);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);

  QLabel * designDayLabel = new QLabel("Design Day Profiles");
  mainVLayout->addWidget(designDayLabel);

  QHBoxLayout * summerDesignDayLayout = new QHBoxLayout();
  summerDesignDayLayout->setContentsMargins(0,0,0,0);
  mainVLayout->addLayout(summerDesignDayLayout);

  DefaultTab * summerTab = new DefaultTab(m_scheduleTab,DefaultTab::SUMMER);
  summerDesignDayLayout->addWidget(summerTab);

  QHBoxLayout * winterDesignDayLayout = new QHBoxLayout();
  winterDesignDayLayout->setContentsMargins(0,0,0,0);
  mainVLayout->addLayout(winterDesignDayLayout);

  DefaultTab * winterTab = new DefaultTab(m_scheduleTab,DefaultTab::WINTER);
  winterDesignDayLayout->addWidget(winterTab);

  QHBoxLayout * runPeriodLayout = new QHBoxLayout();
  runPeriodLayout->setContentsMargins(0,0,0,0);
  mainVLayout->addLayout(runPeriodLayout);
  QPushButton * runPeriodButton = new QPushButton();
  runPeriodButton->setText("Run Period Profiles");
  runPeriodButton->setObjectName("Button");
  QString style;
  style.append("QWidget#Button { ");
  style.append("background: none; border: none; ");
  style.append("} ");
  style.append("QWidget#Button:hover { ");
  style.append("color: #2C3233;");
  style.append("}");
  runPeriodButton->setStyleSheet(style);
  runPeriodButton->setToolTip("Click to add new run period profile");
  runPeriodLayout->addWidget(runPeriodButton);
  runPeriodLayout->addStretch();
  connect(runPeriodButton,SIGNAL(clicked()),this,SLOT(onRunPeriodRulesClicked()));
  connect( this,SIGNAL(runPeriodRulesClicked(const model::ScheduleRuleset)),
           m_scheduleTab->schedulesView(),SLOT(showAddRulePage(const model::ScheduleRuleset)));

  QPushButton * addButton = new QPushButton();
  addButton->setStyleSheet("QPushButton { border: none; background-image: url(\":/images/add-small.png\"); }");
  addButton->setFixedSize(20,20);
  connect(addButton,SIGNAL(clicked()),this,SLOT(onRunPeriodRulesClicked()));
  runPeriodLayout->addWidget(addButton);

  m_ruleLayout = new QVBoxLayout();
  m_ruleLayout->setContentsMargins(0,0,0,0);
  m_ruleLayout->setSpacing(0);
  mainVLayout->addLayout(m_ruleLayout);

  QHBoxLayout * defaultLayout = new QHBoxLayout();
  defaultLayout->setContentsMargins(0,0,0,0);
  mainVLayout->addLayout(defaultLayout);

  DefaultTab * defaultTab = new DefaultTab(m_scheduleTab);
  defaultLayout->addWidget(defaultTab);
}

void ScheduleTabContent::refresh()
{
  if( m_dirty )
  {
    model::ScheduleRuleset scheduleRuleset = m_scheduleTab->schedule();

    QLayoutItem *child;
    while ((child = m_ruleLayout->takeAt(0)) != 0) 
    {
      delete child->widget();

      delete child;
    }

    std::vector<model::ScheduleRule> scheduleRules = scheduleRuleset.scheduleRules();

    for( std::vector<model::ScheduleRule>::iterator it = scheduleRules.begin();
         it < scheduleRules.end();
         ++it )
    {
      ScheduleTabRule * scheduleTabRule = new ScheduleTabRule(m_scheduleTab,*it);
      m_ruleLayout->addWidget(scheduleTabRule);
    }

    m_dirty = false;
  }
}

void ScheduleTabContent::scheduleRefresh()
{
  m_dirty = true;
  
  QTimer::singleShot(0,this,SLOT(refresh()));
}

void ScheduleTabContent::onRunPeriodRulesClicked()
{
  emit runPeriodRulesClicked(scheduleTab()->schedule());
}

void ScheduleTabContent::onDefaultClicked()
{
  emit defaultClicked(scheduleTab()->schedule());
}

void ScheduleTabContent::addScheduleRule( const model::ScheduleRule & scheduleRule )
{
  ScheduleTabRule * scheduleTabRule = new ScheduleTabRule(m_scheduleTab,scheduleRule);
  m_ruleLayout->insertWidget(0,scheduleTabRule);
}

ScheduleTab * ScheduleTabContent::scheduleTab() const
{
  return m_scheduleTab;
}

DefaultTab::DefaultTab( ScheduleTab * scheduleTab, TabType type  )
  : QWidget( scheduleTab ),
    m_mouseDown(false),
    m_scheduleTab(scheduleTab),
    m_type(type),
    m_hovering(false)
{
  setFixedHeight(25);
  
  bool isConnected = false;

  isConnected = connect( this,SIGNAL(defaultClicked(model::ScheduleRuleset)),
                         m_scheduleTab->schedulesView(),SLOT(showDefaultScheduleDay(const model::ScheduleRuleset)) );
  OS_ASSERT(isConnected);

  isConnected = connect( this,SIGNAL(winterClicked(model::ScheduleRuleset)),
                         m_scheduleTab->schedulesView(),SLOT(showWinterScheduleDay(model::ScheduleRuleset)) );
  OS_ASSERT(isConnected);
  
  isConnected = connect( this,SIGNAL(summerClicked(model::ScheduleRuleset)),
                         m_scheduleTab->schedulesView(),SLOT(showSummerScheduleDay(model::ScheduleRuleset)) );
  OS_ASSERT(isConnected);

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(10,0,0,0);
  setLayout(mainHLayout);

  //m_label = new QLabel("Default");

  switch( m_type )
  {
    case SUMMER :
      m_label = new QLabel("Summer");
      setToolTip("Click to edit summer design day profile");
      break;
    case WINTER :
      m_label = new QLabel("Winter");
      setToolTip("Click to edit winter design day profile");
      break;
    default :
      m_label = new QLabel("Default");
      setToolTip("Click to edit default profile");
  }

  m_label->setMouseTracking(true);

  mainHLayout->addWidget(m_label);

  setMouseTracking(true);
}

void DefaultTab::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  if( m_hovering )
  {
    p.setBrush(QBrush(QColor(207,207,207)));
    p.setPen(Qt::NoPen);

    p.drawRect(0,0,size().width() - 1,size().height() - 1);
  }

  p.setPen(Qt::SolidLine);

  p.setBrush(QBrush(m_scheduleTab->schedulesView()->colors[12]));

  p.drawRect(0,0,5,size().height() - 1);

  p.setBrush(QBrush(QColor(Qt::black)));

  p.drawLine(0,0,size().width(),0);

  p.drawLine(0,size().height() - 1,size().width(),size().height() - 1);
}

void DefaultTab::mouseReleaseEvent( QMouseEvent * event )
{
  if( m_mouseDown )
  {
    model::ScheduleRuleset schedule = m_scheduleTab->schedule();

    switch( m_type )
    {
      case DEFAULT :
        emit defaultClicked(schedule);
        break;
      case SUMMER :
        emit summerClicked(schedule);
        break;
      case WINTER :
        emit winterClicked(schedule);
        break;
    }
  }

  m_mouseDown = false;

  event->accept();
}

void DefaultTab::mouseMoveEvent( QMouseEvent * event )
{
  m_hovering = true;    

  update();

  event->accept();
}

void DefaultTab::mousePressEvent( QMouseEvent * event )
{
  m_mouseDown = true;

  event->accept();
}

void DefaultTab::leaveEvent ( QEvent * event )
{
  m_mouseDown = false;

  m_hovering = false;

  update();

  event->accept();
}

ScheduleTabRule::ScheduleTabRule( ScheduleTab * scheduleTab, 
                                  const model::ScheduleRule & scheduleRule, 
                                  QWidget * parent )
  : QWidget( parent ),
    m_scheduleTab(scheduleTab),
    m_scheduleRule(scheduleRule),
    m_mouseDown(false),
    m_dirty(true),
    m_hovering(false)
{
  setFixedHeight(25);

  connect(this,SIGNAL(clicked(model::ScheduleRule)),
          m_scheduleTab->schedulesView(),SLOT(showScheduleRule(model::ScheduleRule)));

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(10,0,0,0);
  setLayout(mainHLayout);

  m_label = new QLabel();
  m_label->setMouseTracking(true);
  mainHLayout->addWidget(m_label);

  scheduleRefresh();

  connect( m_scheduleRule.getImpl<openstudio::model::detail::ScheduleRule_Impl>().get(),
           SIGNAL(onChange()),
           this,
           SLOT(scheduleRefresh()) );

  setMouseTracking(true); 
}

void ScheduleTabRule::refresh()
{
  if( m_dirty )
  {
    QString stringLabel("Priority ");
    stringLabel.append(QString::number(m_scheduleRule.ruleIndex() + 1));

    m_label->setText(stringLabel);

    QString tooltip("Click to edit ");
    tooltip.append(QString::fromStdString(m_scheduleRule.name().get()));

    setToolTip(tooltip);

    m_dirty = false;
  }
}

void ScheduleTabRule::scheduleRefresh()
{
  m_dirty = true;
  
  QTimer::singleShot(0,this,SLOT(refresh()));
}

void ScheduleTabRule::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  int i = m_scheduleRule.ruleIndex();

  if( i > 12 )
  {
    i = 12;
  }

  if( m_hovering )
  {
    p.setBrush(QBrush(QColor(207,207,207)));
    p.setPen(Qt::NoPen);

    p.drawRect(0,0,size().width() - 1,size().height() - 1);
  }

  p.setBrush(QBrush(m_scheduleTab->schedulesView()->colors[i]));

  p.setPen(Qt::SolidLine);

  p.drawRect(0,0,5,size().height() - 1);

  p.setBrush(QBrush(QColor(Qt::black)));
  if( m_scheduleRule.ruleIndex() == 0 )
  {
    p.drawLine(0,0,size().width(),0);
  }
  p.drawLine(0,size().height() - 1,size().width(),size().height() - 1);
}

void ScheduleTabRule::mouseMoveEvent( QMouseEvent * event )
{
  m_hovering = true;    

  update();

  event->accept();
}

void ScheduleTabRule::mouseReleaseEvent( QMouseEvent * event )
{
  if( m_mouseDown )
  {
    emit clicked(m_scheduleRule);
  }

  m_mouseDown = false;

  event->accept();
}

void ScheduleTabRule::mousePressEvent( QMouseEvent * event )
{
  m_mouseDown = true;

  event->accept();
}

void ScheduleTabRule::leaveEvent ( QEvent * event )
{
  m_mouseDown = false;

  m_hovering = false;

  update();

  event->accept();
}

ScheduleTab::ScheduleTab(const model::ScheduleRuleset & schedule, SchedulesView * schedulesView, QWidget * parent)
  : QWidget(parent),
    m_mouseDown(false),
    m_selected(false),
    m_schedule(schedule),
    m_schedulesView(schedulesView)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);

  m_header = new ScheduleTabHeader(this);
  m_header->setText(QString::fromStdString(m_schedule.name().get()));
  mainVLayout->addWidget(m_header);

  QFrame * line1 = new QFrame();
  line1->setFrameShape(QFrame::HLine);
  line1->setFixedHeight(1);
  mainVLayout->addWidget(line1);

  m_content = new ScheduleTabContent(this);
  mainVLayout->addWidget(m_content);
  m_content->setVisible(false);

  QFrame * line2 = new QFrame();
  line2->setFrameShape(QFrame::HLine);
  line2->setFixedHeight(1);
  mainVLayout->addWidget(line2);

  connect(m_header,SIGNAL(toggleHeaderClicked( bool)),this,SLOT(onToggleHeaderClicked(bool)));
}

void ScheduleTab::expand()
{
  m_content->show();

  m_header->expand();
}

void ScheduleTab::collapse()
{
  m_content->hide();

  m_header->collapse();
}

void ScheduleTab::toggle()
{
  m_header->toggle();

  m_content->setVisible(! m_content->isVisible() );
}

void ScheduleTab::onToggleHeaderClicked(bool close)
{
  //if( close )
  //{
  //  m_content->hide();
  //}
  //else
  //{
  //  m_content->show();
  //}
}

ScheduleTabHeader * ScheduleTab::scheduleTabHeader() const
{
  return m_header;
}

ScheduleTabContent * ScheduleTab::scheduleTabContent() const
{
  return m_content;
}

SchedulesView * ScheduleTab::schedulesView() const
{
  return m_schedulesView;
}

model::ScheduleRuleset ScheduleTab::schedule()
{
  return m_schedule;
}

void ScheduleTab::setSelected(bool selected)
{
  m_header->setSelected(selected);

  m_selected = selected;
}

bool ScheduleTab::selected()
{
  return m_selected;
}

void ScheduleTab::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ScheduleTab::leaveEvent ( QEvent * event )
{
  m_mouseDown = false;

  event->accept();
}

void ScheduleTab::resizeEvent ( QResizeEvent * event )
{
}

void ScheduleTab::onRemoveClicked()
{
}

void ScheduleTab::onObjectChanged()
{
}

void ScheduleTab::refresh()
{
}

NewRuleView::NewRuleView( const model::ScheduleRuleset & scheduleRuleset, SchedulesView * schedulesView, Type type )
  : QWidget(schedulesView),
    m_schedulesView(schedulesView),
    m_scheduleRuleset(scheduleRuleset),
    m_type(type)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(20,20,20,20);

  switch( m_type )
  {
    case SUMMER :
    {
      QLabel * label = new QLabel();
      QString text;
      text.append("The summer design day profile is not set, therefore the default run period profile will be used.");
      text.append("  To override the default run period profile select one of the following:");
      label->setText(text);
      label->setWordWrap(true);
      mainVLayout->addWidget(label);
      mainVLayout->addSpacing(10);
      break;
    }
    case WINTER :
    {
      QLabel * label = new QLabel();
      QString text;
      text.append("The winter design day profile is not set, therefore the default run period profile will be used.");
      text.append("  To override the default run period profile select one of the following:");
      label->setText(text);
      label->setWordWrap(true);
      mainVLayout->addWidget(label);
      mainVLayout->addSpacing(10);
      break;
    }
    default :
      break;
  }

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0,0,0,0);
  setLayout(mainVLayout);

  QWidget * box = new QWidget();

  QVBoxLayout * innerVLayout = new QVBoxLayout();
  innerVLayout->setContentsMargins(0,0,0,0);
  box->setLayout(innerVLayout);
  mainHLayout->addWidget(box);

  mainVLayout->addLayout(mainHLayout);

  if( m_type == DEFAULT )
  {
    QLabel * selectLabel = new QLabel();
    selectLabel->setText("Select One of the Following:");
    innerVLayout->addWidget(selectLabel);
  }

  QHBoxLayout * newEmptyLayout = new QHBoxLayout();
  newEmptyLayout->setContentsMargins(0,0,0,0);
  QRadioButton * newEmptyButton = new QRadioButton();
  newEmptyButton->setText("Make a New Profile");
  newEmptyButton->setChecked(true);
  newEmptyLayout->addWidget(newEmptyButton);
  innerVLayout->addLayout(newEmptyLayout);

  //QHBoxLayout * newLayout = new QHBoxLayout();
  //newLayout->setContentsMargins(0,0,0,0);
  //QRadioButton * newButton = new QRadioButton();
  //newLayout->addWidget(newButton);
  //newButton->setText("Make a New Profile Based on:");
  //QComboBox * newComboBox = new QComboBox();
  //newLayout->addWidget(newComboBox);
  //innerVLayout->addLayout(newLayout);

  QHBoxLayout * addLayout = new QHBoxLayout();
  addLayout->addStretch();
  addLayout->setContentsMargins(0,0,0,0);
  QPushButton * addButton = new QPushButton();
  addButton->setText("Add");
  addButton->setObjectName("StandardBlueButton");
  addLayout->addWidget(addButton);
  innerVLayout->addLayout(addLayout);
  connect(addButton,SIGNAL(clicked()),SLOT(onAddClicked()));
  connect(this,SIGNAL(addRuleClicked(model::ScheduleRuleset &)),schedulesView,SIGNAL(addRuleClicked(model::ScheduleRuleset &)));
  connect(this,SIGNAL(addSummerProfileClicked(model::ScheduleRuleset &)),schedulesView,SIGNAL(addSummerProfileClicked(model::ScheduleRuleset &)));
  connect(this,SIGNAL(addWinterProfileClicked(model::ScheduleRuleset &)),schedulesView,SIGNAL(addWinterProfileClicked(model::ScheduleRuleset &)));

  innerVLayout->addStretch();

  mainHLayout->addStretch();
}

void NewRuleView::onAddClicked()
{
  switch( m_type )
  {
    case SUMMER :
      emit addSummerProfileClicked(m_scheduleRuleset);
      break;
    case WINTER :
      emit addWinterProfileClicked(m_scheduleRuleset);
      break;
    default :
     emit addRuleClicked(m_scheduleRuleset);
  }
}

ScheduleRulesetView::ScheduleRulesetView(const model::ScheduleRuleset & scheduleRuleset)
  : QWidget(),
    m_scheduleRuleset(scheduleRuleset)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10,10,10,10);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QLabel * nameLabel = new QLabel("Name:");
  nameLabel->setObjectName("H2");
  mainVLayout->addWidget(nameLabel);

  OSLineEdit * lineEdit = new OSLineEdit();
  lineEdit->bind(m_scheduleRuleset,"name");
  mainVLayout->addWidget(lineEdit); 

  mainVLayout->addStretch();
}

} // openstudio

