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

#include "SchedulesView.hpp"
#include "ScheduleDayView.hpp"
#include "OSAppBase.hpp"

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

/******************************************************************************/
// SchedulesView
/******************************************************************************/

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

SchedulesView::SchedulesView(bool isIP, const model::Model & model)
  : QWidget(),
    m_model(model),
    m_leftVLayout(nullptr),
    m_contentLayout(nullptr),
    m_isIP(isIP)
{
  setObjectName("GrayWidgetWithLeftTopBorders");

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(1, 1, 0, 0);
  mainHLayout->setSpacing(0);
  setLayout(mainHLayout);

  auto leftScrollArea = new QScrollArea();
  leftScrollArea->setFrameStyle(QFrame::NoFrame);

  auto outerLeftVLayout = new QVBoxLayout();
  outerLeftVLayout->setContentsMargins(0, 0, 0, 0);
  outerLeftVLayout->addWidget(leftScrollArea, 10);
  outerLeftVLayout->addStretch();

  auto selectorButtons = new OSItemSelectorButtons();
  selectorButtons->enableCopyButton();
  selectorButtons->enablePurgeButton();
  selectorButtons->enableRemoveButton();

  connect(selectorButtons, &OSItemSelectorButtons::itemDropped, this, &SchedulesView::itemDropped);

  connect(selectorButtons, &OSItemSelectorButtons::addClicked, this, &SchedulesView::addScheduleClicked);

  connect(selectorButtons, &OSItemSelectorButtons::copyClicked, this, &SchedulesView::copySelectedScheduleClicked);

  connect(selectorButtons, &OSItemSelectorButtons::removeClicked, this, &SchedulesView::removeSelectedScheduleClicked);

  connect(selectorButtons, &OSItemSelectorButtons::purgeClicked, this, &SchedulesView::purgeUnusedScheduleRulesetsClicked);

  connect(selectorButtons, &OSItemSelectorButtons::downloadComponentsClicked, this, &SchedulesView::downloadComponentsClicked);

  connect(this, &SchedulesView::toggleUnitsClicked, this, &SchedulesView::toggleUnits);

  outerLeftVLayout->addWidget(selectorButtons);

  mainHLayout->addLayout(outerLeftVLayout);

  auto vLine = new QWidget();
  vLine->setObjectName("VLine");
  vLine->setStyleSheet("QWidget#VLine { background: #445051;}");
  vLine->setFixedWidth(2);
  mainHLayout->addWidget(vLine);

  auto leftScrollWidget = new QWidget();
  leftScrollArea->setWidget(leftScrollWidget);
  leftScrollArea->setWidgetResizable(true);
  leftScrollWidget->setObjectName("GrayWidget");

  m_leftVLayout = new QVBoxLayout();
  m_leftVLayout->setContentsMargins(0, 0, 0, 0);
  m_leftVLayout->setSpacing(0);
  m_leftVLayout->addStretch();
  leftScrollWidget->setLayout(m_leftVLayout);

  m_contentLayout = new QHBoxLayout();
  m_contentLayout->setContentsMargins(0, 0, 0, 0);
  mainHLayout->addLayout(m_contentLayout, 100);

  // m_model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<SchedulesView, &SchedulesView::onModelObjectAdded>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &SchedulesView::onModelObjectAdded, Qt::QueuedConnection);

  m_model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<SchedulesView, &SchedulesView::onModelObjectRemoved>(this);
  //connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &SchedulesView::onModelObjectRemoved, Qt::QueuedConnection);

  // get all schedules
  std::vector<model::ScheduleRuleset> schedules = m_model.getConcreteModelObjects<model::ScheduleRuleset>();

  // sort by name
  std::sort(schedules.begin(), schedules.end(), WorkspaceObjectNameGreater());

  showEmptyPage();

  for (auto it = schedules.begin();
       it < schedules.end();
       ++it)
  {
    addSchedule(*it);

    std::vector<model::ScheduleRule> rules = it->scheduleRules();

    for (auto ruleIt = rules.begin();
         ruleIt < rules.end();
         ++ruleIt)
    {
      addScheduleRule(*ruleIt);
    }
  }

  updateRowColors();

  if (!schedules.empty()){
    setCurrentSchedule(schedules.back());
  }

}

void SchedulesView::closeAllTabs() const
{
  for (int i = 0; i < m_leftVLayout->count() - 1; i++)
  {
    ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    scheduleTab->scheduleTabContent()->hide();
  }
}

ScheduleTab * SchedulesView::tabForSchedule(const model::ScheduleRuleset schedule) const
{
  for (int i = 0; i < m_leftVLayout->count() - 1; i++)
  {
    ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if (scheduleTab->schedule().handle() == schedule.handle())
    {
      return scheduleTab;
    }
  }

  return nullptr;
}

void SchedulesView::updateRowColors()
{
  QString evenStyle;
  evenStyle.append("QWidget#ThermalZoneTab {");
  evenStyle.append("background: #F2F2F2; ");
  evenStyle.append("border-bottom: 1px solid black; ");
  evenStyle.append("}");
  evenStyle.append("QWidget#SideBar {background: #DEDEDE;}");

  QString oddStyle;
  oddStyle.append("QWidget#ThermalZoneTab {");
  oddStyle.append("background: #E6E6E6; ");
  oddStyle.append("border-bottom: 1px solid black; ");
  oddStyle.append("}");
  oddStyle.append("QWidget#SideBar {background: #CECECE;}");

  for (int i = 0; i < m_leftVLayout->count() - 1; i++)
  {
    ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if (i % 2 == 0)
    {
      scheduleTab->setStyleSheet(evenStyle);
    } else
    {
      scheduleTab->setStyleSheet(oddStyle);
    }
  }
}

void SchedulesView::addSchedule(model::ScheduleRuleset & schedule)
{
  auto scheduleTab = new ScheduleTab(schedule, this);
  connect(scheduleTab, &ScheduleTab::scheduleClicked, this, &SchedulesView::setCurrentSchedule);
  m_leftVLayout->insertWidget(0, scheduleTab);
}

void SchedulesView::addScheduleRule(model::ScheduleRule & rule)
{
  model::ScheduleRuleset scheduleRuleset = rule.scheduleRuleset();

  ScheduleTab * tab = tabForSchedule(scheduleRuleset);

  if (tab)
  {
    tab->scheduleTabContent()->scheduleRefresh(scheduleRuleset.handle()); // Handle as dummy

    rule.getImpl<model::detail::ScheduleRule_Impl>().get()->onRemoveFromWorkspace.connect<ScheduleTabContent, &ScheduleTabContent::scheduleRefresh>(tab->scheduleTabContent());
  }

}

void SchedulesView::onModelObjectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> workspaceObjectImpl,
                                       const openstudio::IddObjectType&,
                                       const openstudio::UUID&)
{
  boost::optional<model::ScheduleRuleset> schedule = m_model.getModelObject<model::ScheduleRuleset>(workspaceObjectImpl->handle());
  if (schedule)
  {
    this->addSchedule(schedule.get());
    this->setCurrentSchedule(schedule.get());
  }

  boost::optional<model::ScheduleRule> rule = m_model.getModelObject<model::ScheduleRule>(workspaceObjectImpl->handle());
  if (rule)
  {
    rule->getImpl<detail::WorkspaceObject_Impl>().get()->onRemoveFromWorkspace.connect<SchedulesView, &SchedulesView::onScheduleRuleRemoved>(this);

    addScheduleRule(rule.get());
  }
}

void SchedulesView::onModelObjectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> workspaceObjectImpl,
                                         const openstudio::IddObjectType&,
                                         const openstudio::UUID&)
{
  if (workspaceObjectImpl->iddObject().type() == IddObjectType::OS_Schedule_Ruleset)
  {
    int removedIndex = -1;

    bool wasSelected = false;

    ScheduleTab * scheduleTab = nullptr;

    for (int i = 0;
         i < m_leftVLayout->count() - 1;
         i++)
    {
      scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());
      if (scheduleTab->schedule().handle() == workspaceObjectImpl->handle())
      {
        m_leftVLayout->removeWidget(scheduleTab);
        wasSelected = scheduleTab->selected();
        removedIndex = i;
        delete scheduleTab;
        break;
      }
    }

    if (wasSelected)
    {
      int newIndex = -1;
      if (removedIndex > 0)
      {
        newIndex = removedIndex - 1;
      } else if (removedIndex == 0)
      {
        if (m_leftVLayout->count() > 1)
        {
          newIndex = 0;
        }
      }

      if (newIndex > -1)
      {
        ScheduleTab * scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(newIndex)->widget());

        if (!scheduleTab->schedule().handle().isNull()){
          this->setCurrentSchedule(scheduleTab->schedule());
        } else {
          this->showEmptyPage();
        }
      } else {
        this->showEmptyPage();
      }
    }
  }
}

void SchedulesView::setCurrentSchedule(const model::ScheduleRuleset & schedule)
{
  this->updateRowColors();

  for (int i = 0;
       i < m_leftVLayout->count() - 1;
       i++)
  {
    ScheduleTab * scheduleTab;

    scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if (scheduleTab->schedule() == schedule)
    {
      if (!scheduleTab->selected())
      {
        scheduleTab->setSelected(true);

        scheduleTab->expand();
      } else
      {
        scheduleTab->toggle();
      }
    } else {
      scheduleTab->setSelected(false);

      scheduleTab->collapse();
    }

    scheduleTab->update();
  }

  //showScheduleRuleset(schedule);

  // DLM: I don't think that the code below works because it gets called when the scene is not visible
  showDefaultScheduleDay(schedule);

}

void SchedulesView::showAddRulePage(const model::ScheduleRuleset & scheduleRuleset)
{
  this->setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();

    delete child;
  }

  auto newProfileView = new NewProfileView(scheduleRuleset, this, NewProfileView::SCHEDULERULE);

  m_contentLayout->addWidget(newProfileView, 100);

  this->setUpdatesEnabled(true);

  boost::optional<model::ModelObject> mo;
  emit modelObjectSelected(mo, true);
}

void SchedulesView::showScheduleRuleset(const model::ScheduleRuleset & schedule)
{
  this->setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();

    delete child;
  }

  auto scheduleRulesetNameView = new ScheduleRulesetNameView(schedule, this);
  m_contentLayout->addWidget(scheduleRulesetNameView, 100);

  this->setUpdatesEnabled(true);

  boost::optional<model::ModelObject> mo;
  emit modelObjectSelected(mo, true);
}

void SchedulesView::showScheduleRule(model::ScheduleRule scheduleRule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();

    delete child;
  }

  auto scheduleView = new ScheduleRuleView(m_isIP, scheduleRule, this);

  connect(this, &SchedulesView::toggleUnitsClicked, scheduleView, &ScheduleRuleView::toggleUnitsClicked);

  m_contentLayout->addWidget(scheduleView);

  scheduleView->show();

  setUpdatesEnabled(true);

  boost::optional<model::ModelObject> mo = scheduleRule.daySchedule();
  emit modelObjectSelected(mo, true);
}

void SchedulesView::onScheduleRuleRemoved(const Handle& handle)
{
  // DLM: todo, show something else?
  showEmptyPage();
}

void SchedulesView::showDefaultScheduleDay(const model::ScheduleRuleset & schedule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();
    delete child;
  }

  auto scheduleView = new DefaultScheduleDayView(m_isIP, schedule, this);

  connect(this, &SchedulesView::toggleUnitsClicked, scheduleView, &DefaultScheduleDayView::toggleUnitsClicked);

  m_contentLayout->addWidget(scheduleView);

  scheduleView->show();

  setUpdatesEnabled(true);

  boost::optional<model::ModelObject> mo = schedule.defaultDaySchedule();
  emit modelObjectSelected(mo, true);
}

void SchedulesView::showSummerScheduleDay(model::ScheduleRuleset schedule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();

    delete child;
  }

  if (!schedule.isSummerDesignDayScheduleDefaulted())
  {

    auto scheduleView = new SizingScheduleDayView(m_isIP, schedule, this, SizingScheduleDayView::SUMMER);

    connect(this, &SchedulesView::toggleUnitsClicked, scheduleView, &SizingScheduleDayView::toggleUnitsClicked);

    m_contentLayout->addWidget(scheduleView);

    scheduleView->show();

    boost::optional<model::ModelObject> mo = schedule.summerDesignDaySchedule();
    emit modelObjectSelected(mo, true);
  } else {
    auto newProfileView = new NewProfileView(schedule, this, NewProfileView::SUMMER);

    m_contentLayout->addWidget(newProfileView, 100);

    boost::optional<model::ModelObject> mo;
    emit modelObjectSelected(mo, true);
  }

  setUpdatesEnabled(true);
}

void SchedulesView::showWinterScheduleDay(model::ScheduleRuleset schedule)
{
  setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();

    delete child;
  }

  if (!schedule.isWinterDesignDayScheduleDefaulted())
  {
    auto scheduleView = new SizingScheduleDayView(m_isIP, schedule, this, SizingScheduleDayView::WINTER);

    connect(this, &SchedulesView::toggleUnitsClicked, scheduleView, &SizingScheduleDayView::toggleUnitsClicked);

    m_contentLayout->addWidget(scheduleView);

    scheduleView->show();

    boost::optional<model::ModelObject> mo = schedule.winterDesignDaySchedule();
    emit modelObjectSelected(mo, true);
  } else {
    auto newProfileView = new NewProfileView(schedule, this, NewProfileView::WINTER);

    m_contentLayout->addWidget(newProfileView, 100);

    boost::optional<model::ModelObject> mo;
    emit modelObjectSelected(mo, true);
  }

  setUpdatesEnabled(true);
}

void SchedulesView::showEmptyPage()
{
  this->setUpdatesEnabled(false);

  QLayoutItem *child;
  while ((child = m_contentLayout->takeAt(0)) != nullptr)
  {
    delete child->widget();
    delete child;
  }

  auto emptyWidget = new QWidget();

  m_contentLayout->addWidget(emptyWidget, 100);

  this->setUpdatesEnabled(true);

  boost::optional<model::ModelObject> mo;
  emit modelObjectSelected(mo, true);
}

boost::optional<model::ScheduleRuleset> SchedulesView::currentSchedule()
{
  for (int i = 0;
       i < m_leftVLayout->count() - 1;
       i++)
  {
    ScheduleTab * scheduleTab;

    scheduleTab = qobject_cast<ScheduleTab *>(m_leftVLayout->itemAt(i)->widget());

    if (scheduleTab->selected())
    {
      if (boost::optional<model::ScheduleRuleset> schedule = scheduleTab->schedule())
      {
        if (!schedule->handle().isNull()) {
          return schedule;
        }
      }
    }
  }

  return boost::none;
}

void SchedulesView::paintEvent(QPaintEvent * event)
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

bool SchedulesView::isIP() const
{
  return m_isIP;
}

/******************************************************************************/
// ScheduleTab
/******************************************************************************/

ScheduleTab::ScheduleTab(const model::ScheduleRuleset & schedule, SchedulesView * schedulesView, QWidget * parent)
  : QWidget(parent),
  //m_mouseDown(false),
  m_selected(false),
  m_schedule(schedule),
  m_schedulesView(schedulesView)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);

  m_header = new ScheduleTabHeader(this);
  m_header->setText(QString::fromStdString(m_schedule.name().get()));
  mainVLayout->addWidget(m_header);

  auto line1 = new QFrame();
  line1->setFrameShape(QFrame::HLine);
  line1->setFixedHeight(1);
  mainVLayout->addWidget(line1);

  m_content = new ScheduleTabContent(this);
  mainVLayout->addWidget(m_content);
  m_content->setVisible(false);

  auto line2 = new QFrame();
  line2->setFrameShape(QFrame::HLine);
  line2->setFixedHeight(1);
  mainVLayout->addWidget(line2);
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

  m_content->setVisible(!m_content->isVisible());
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

//void ScheduleTab::paintEvent ( QPaintEvent * event )
//{
//  QStyleOption opt;
//  opt.init(this);
//  QPainter p(this);
//  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}

//void ScheduleTab::leaveEvent ( QEvent * event )
//{
//  m_mouseDown = false;
//
//  event->accept();
//}

//void ScheduleTab::resizeEvent ( QResizeEvent * event )
//{
//}

//void ScheduleTab::onRemoveClicked()
//{
//}

//void ScheduleTab::onObjectChanged()
//{
//}

//void ScheduleTab::refresh()
//{
//}


/******************************************************************************/
// ScheduleTabHeader
/******************************************************************************/

ScheduleTabHeader::ScheduleTabHeader(ScheduleTab * scheduleTab, QWidget * parent)
  : QWidget(parent),
  m_scheduleTab(scheduleTab),
  m_mouseDown(false),
  m_dirty(true)
{
  setObjectName("ScheduleTabHeader");

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5, 5, 5, 5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  QString style;

  style.append("QWidget#ScheduleTabHeader { ");
  style.append("background-color: #CECECE;");
  style.append("}");
  setStyleSheet(style);

  m_mainLabel = new QLabel(this);
  mainHLayout->addWidget(m_mainLabel, 10);
  m_mainLabel->setWordWrap(true);
  //m_mainLabel->setFixedWidth(120);

  mainHLayout->addStretch();

  m_toggleButton = new QPushButton();
  m_toggleButton->setFlat(true);
  m_toggleButton->setCheckable(true);
  m_toggleButton->setChecked(false);
  m_toggleButton->setEnabled(false);
  connect(m_toggleButton, &QPushButton::toggled, this, &ScheduleTabHeader::toggleHeaderClicked);
  mainHLayout->addWidget(m_toggleButton);
  style.clear();
  style.append("QPushButton { ");
  style.append("border: none; ");
  style.append("background-image: url(\":/images/toggle_arrow.png\"); ");
  style.append("} ");

  style.append("QPushButton:checked { ");
  style.append("background-image: url(\":/images/toggle_arrow_closed.png\"); ");
  style.append("} ");
  m_toggleButton->setFixedSize(11, 11);
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

  connect(this, &ScheduleTabHeader::scheduleClicked, m_scheduleTab, &ScheduleTab::scheduleClicked);

  m_scheduleTab->schedule().getImpl<model::detail::ScheduleRuleset_Impl>().get()->onChange.connect<ScheduleTabHeader, &ScheduleTabHeader::refresh>(this);
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

  QTimer::singleShot(0, this, SLOT(refreshNow()));
}

void ScheduleTabHeader::refreshNow()
{
  if (m_dirty)
  {
    setText(QString::fromStdString(m_scheduleTab->schedule().name().get()));

    m_dirty = false;
  }
}

void ScheduleTabHeader::mouseReleaseEvent(QMouseEvent * event)
{
  if (m_mouseDown)
  {
    emit scheduleClicked(scheduleTab()->schedule());
  }

  m_mouseDown = false;

  event->accept();
}

void ScheduleTabHeader::mousePressEvent(QMouseEvent * event)
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

  if (selected)
  {
    m_selectionWidget->show();
  } else
  {
    m_selectionWidget->hide();
  }
}

void ScheduleTabHeader::setText(const QString & text)
{
  m_mainLabel->setText(text);
}

void ScheduleTabHeader::resizeEvent(QResizeEvent * event)
{
  m_selectionWidget->resize(event->size().width(), 67);

  QWidget::resizeEvent(event);
}

void ScheduleTabHeader::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/******************************************************************************/
// ScheduleTabContent
/******************************************************************************/

ScheduleTabContent::ScheduleTabContent(ScheduleTab * scheduleTab, QWidget * parent)
  : QWidget(parent),
  m_scheduleTab(scheduleTab)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(5, 5, 5, 5);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);

  QLabel * designDayLabel = new QLabel("Design Day Profiles");
  mainVLayout->addWidget(designDayLabel);

  auto summerDesignDayLayout = new QHBoxLayout();
  summerDesignDayLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->addLayout(summerDesignDayLayout);

  auto summerTab = new ScheduleTabDefault(m_scheduleTab, ScheduleTabDefault::SUMMER);
  summerDesignDayLayout->addWidget(summerTab);

  auto winterDesignDayLayout = new QHBoxLayout();
  winterDesignDayLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->addLayout(winterDesignDayLayout);

  auto winterTab = new ScheduleTabDefault(m_scheduleTab, ScheduleTabDefault::WINTER);
  winterDesignDayLayout->addWidget(winterTab);

  auto runPeriodLayout = new QHBoxLayout();
  runPeriodLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->addLayout(runPeriodLayout);
  auto runPeriodButton = new QPushButton();
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
  connect(runPeriodButton, &QPushButton::clicked, this, &ScheduleTabContent::onScheduleRuleClicked);
  connect(this, &ScheduleTabContent::scheduleRuleClicked, m_scheduleTab->schedulesView(), &SchedulesView::showAddRulePage);

  auto addButton = new QPushButton();
  addButton->setStyleSheet("QPushButton { border: none; background-image: url(\":/images/add-small.png\"); }");
  addButton->setFixedSize(20, 20);
  connect(addButton, &QPushButton::clicked, this, &ScheduleTabContent::onScheduleRuleClicked);
  runPeriodLayout->addWidget(addButton);

  m_ruleLayout = new QVBoxLayout();
  m_ruleLayout->setContentsMargins(0, 0, 0, 0);
  m_ruleLayout->setSpacing(0);
  mainVLayout->addLayout(m_ruleLayout);

  auto defaultLayout = new QHBoxLayout();
  defaultLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->addLayout(defaultLayout);

  auto defaultTab = new ScheduleTabDefault(m_scheduleTab, ScheduleTabDefault::DEFAULT);
  defaultLayout->addWidget(defaultTab);
}

void ScheduleTabContent::refresh()
{
  if (m_dirty)
  {
    model::ScheduleRuleset scheduleRuleset = m_scheduleTab->schedule();

    QLayoutItem *child;
    while ((child = m_ruleLayout->takeAt(0)) != nullptr)
    {
      delete child->widget();

      delete child;
    }

    std::vector<model::ScheduleRule> scheduleRules = scheduleRuleset.scheduleRules();

    for (auto it = scheduleRules.begin();
         it < scheduleRules.end();
         ++it)
    {
      auto scheduleTabRule = new ScheduleTabRule(m_scheduleTab, *it);
      m_ruleLayout->addWidget(scheduleTabRule);
    }

    m_dirty = false;
  }
}

void ScheduleTabContent::scheduleRefresh(const Handle& handle)
{
  m_dirty = true;

  QTimer::singleShot(0, this, SLOT(refresh()));
}

void ScheduleTabContent::onScheduleRuleClicked()
{
  emit scheduleRuleClicked(scheduleTab()->schedule());
}

void ScheduleTabContent::onDefaultScheduleClicked()
{
  emit defaultScheduleClicked(scheduleTab()->schedule());
}

void ScheduleTabContent::addScheduleRule(const model::ScheduleRule & scheduleRule)
{
  auto scheduleTabRule = new ScheduleTabRule(m_scheduleTab, scheduleRule);
  m_ruleLayout->insertWidget(0, scheduleTabRule);
}

ScheduleTab * ScheduleTabContent::scheduleTab() const
{
  return m_scheduleTab;
}

/******************************************************************************/
// ScheduleTabRule
/******************************************************************************/

ScheduleTabRule::ScheduleTabRule(ScheduleTab * scheduleTab,
                                 const model::ScheduleRule & scheduleRule,
                                 QWidget * parent)
                                 : QWidget(parent),
                                 m_scheduleTab(scheduleTab),
                                 m_scheduleRule(scheduleRule),
                                 m_mouseDown(false),
                                 m_dirty(true),
                                 m_hovering(false)
{
  setFixedHeight(25);

  connect(this, &ScheduleTabRule::clicked, m_scheduleTab->schedulesView(), &SchedulesView::showScheduleRule);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(10, 0, 0, 0);
  setLayout(mainHLayout);

  m_label = new QLabel();
  m_label->setMouseTracking(true);
  mainHLayout->addWidget(m_label);

  scheduleRefresh();

  m_scheduleRule.getImpl<model::detail::ScheduleRule_Impl>().get()->onChange.connect<ScheduleTabRule, &ScheduleTabRule::scheduleRefresh>(this);

  setMouseTracking(true);
}

void ScheduleTabRule::refresh()
{
  if (m_dirty)
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

  QTimer::singleShot(0, this, SLOT(refresh()));
}

void ScheduleTabRule::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  int i = m_scheduleRule.ruleIndex();

  if (i > 12)
  {
    i = 12;
  }

  if (m_hovering)
  {
    p.setBrush(QBrush(QColor(207, 207, 207)));
    p.setPen(Qt::NoPen);

    p.drawRect(0, 0, size().width() - 1, size().height() - 1);
  }

  p.setBrush(QBrush(m_scheduleTab->schedulesView()->colors[i]));

  p.setPen(Qt::SolidLine);

  p.drawRect(0, 0, 5, size().height() - 1);

  p.setBrush(QBrush(QColor(Qt::black)));
  if (m_scheduleRule.ruleIndex() == 0)
  {
    p.drawLine(0, 0, size().width(), 0);
  }
  p.drawLine(0, size().height() - 1, size().width(), size().height() - 1);
}

void ScheduleTabRule::mouseMoveEvent(QMouseEvent * event)
{
  m_hovering = true;

  update();

  event->accept();
}

void ScheduleTabRule::mouseReleaseEvent(QMouseEvent * event)
{
  if (m_mouseDown)
  {
    emit clicked(m_scheduleRule);
  }

  m_mouseDown = false;

  event->accept();
}

void ScheduleTabRule::mousePressEvent(QMouseEvent * event)
{
  m_mouseDown = true;

  event->accept();
}

void ScheduleTabRule::leaveEvent(QEvent * event)
{
  m_mouseDown = false;

  m_hovering = false;

  update();

  event->accept();
}

/******************************************************************************/
// ScheduleTabDefault
/******************************************************************************/

ScheduleTabDefault::ScheduleTabDefault(ScheduleTab * scheduleTab, ScheduleTabDefaultType type)
  : QWidget(scheduleTab),
  m_mouseDown(false),
  m_scheduleTab(scheduleTab),
  m_type(type),
  m_hovering(false)
{
  setFixedHeight(25);

  connect(this, &ScheduleTabDefault::defaultClicked, m_scheduleTab->schedulesView(), &SchedulesView::showDefaultScheduleDay);

  connect(this, &ScheduleTabDefault::winterClicked, m_scheduleTab->schedulesView(), &SchedulesView::showWinterScheduleDay);

  connect(this, &ScheduleTabDefault::summerClicked, m_scheduleTab->schedulesView(), &SchedulesView::showSummerScheduleDay);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(10, 0, 0, 0);
  setLayout(mainHLayout);

  //m_label = new QLabel("Default");

  switch (m_type)
  {
  case SUMMER:
    m_label = new QLabel("Summer");
    setToolTip("Click to edit summer design day profile");
    break;
  case WINTER:
    m_label = new QLabel("Winter");
    setToolTip("Click to edit winter design day profile");
    break;
  default:
    m_label = new QLabel("Default");
    setToolTip("Click to edit default profile");
  }

  m_label->setMouseTracking(true);

  mainHLayout->addWidget(m_label);

  setMouseTracking(true);
}

void ScheduleTabDefault::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  if (m_hovering)
  {
    p.setBrush(QBrush(QColor(207, 207, 207)));
    p.setPen(Qt::NoPen);

    p.drawRect(0, 0, size().width() - 1, size().height() - 1);
  }

  p.setPen(Qt::SolidLine);

  // DLM: don't draw color squares for summer and winter design days
  if (m_type == DEFAULT)
  {
    p.setBrush(QBrush(m_scheduleTab->schedulesView()->colors[12]));
    p.drawRect(0, 0, 5, size().height() - 1);
  }

  p.setBrush(QBrush(QColor(Qt::black)));

  p.drawLine(0, 0, size().width(), 0);

  p.drawLine(0, size().height() - 1, size().width(), size().height() - 1);
}

void ScheduleTabDefault::mouseReleaseEvent(QMouseEvent * event)
{
  if (m_mouseDown)
  {
    model::ScheduleRuleset schedule = m_scheduleTab->schedule();

    switch (m_type)
    {
    case DEFAULT:
      emit defaultClicked(schedule);
      break;
    case SUMMER:
      emit summerClicked(schedule);
      break;
    case WINTER:
      emit winterClicked(schedule);
      break;
    }
  }

  m_mouseDown = false;

  event->accept();
}

void ScheduleTabDefault::mouseMoveEvent(QMouseEvent * event)
{
  m_hovering = true;

  update();

  event->accept();
}

void ScheduleTabDefault::mousePressEvent(QMouseEvent * event)
{
  m_mouseDown = true;

  event->accept();
}

void ScheduleTabDefault::leaveEvent(QEvent * event)
{
  m_mouseDown = false;

  m_hovering = false;

  update();

  event->accept();
}

/******************************************************************************/
// NewProfileView
/******************************************************************************/

NewProfileView::NewProfileView(const model::ScheduleRuleset & scheduleRuleset, SchedulesView * schedulesView, NewProfileViewType type)
  : QWidget(schedulesView),
  m_schedulesView(schedulesView),
  m_scheduleRuleset(scheduleRuleset),
  m_type(type)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10, 10, 10, 10);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  auto scheduleRulesetNameWidget = new ScheduleRulesetNameWidget(scheduleRuleset);
  mainVLayout->addWidget(scheduleRulesetNameWidget);

  switch (m_type)
  {
  case SUMMER:
  {
    auto label = new QLabel();
    QString text;
    text.append("The summer design day profile is not set, therefore the default run period profile will be used.");
    text.append("  Create a new profile to override the default run period profile.");
    label->setText(text);
    label->setWordWrap(true);
    mainVLayout->addWidget(label);
    mainVLayout->addSpacing(10);
    break;
  }
  case WINTER:
  {
    auto label = new QLabel();
    QString text;
    text.append("The winter design day profile is not set, therefore the default run period profile will be used.");
    text.append("  Create a new profile to override the default run period profile.");
    label->setText(text);
    label->setWordWrap(true);
    mainVLayout->addWidget(label);
    mainVLayout->addSpacing(10);
    break;
  }
  default:
    break;
  }

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0, 0, 0, 0);


  auto box = new QWidget();

  auto innerVLayout = new QVBoxLayout();
  innerVLayout->setContentsMargins(0, 0, 0, 0);
  box->setLayout(innerVLayout);
  mainHLayout->addWidget(box);

  mainVLayout->addLayout(mainHLayout);

  if (m_type == SCHEDULERULE)
  {
    auto selectLabel = new QLabel();
    selectLabel->setText("Create a new profile.");
    innerVLayout->addWidget(selectLabel);
  }

  auto gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(0, 0, 0, 0);

  QLabel * label = new QLabel("Make a New Profile Based on:");
  gridLayout->addWidget(label, 0, 0);

  m_scheduleRuleComboBox = new QComboBox();
  populateComboBox(scheduleRuleset);
  gridLayout->addWidget(m_scheduleRuleComboBox, 0, 1);

  auto addButton = new QPushButton();
  addButton->setText("Add");
  addButton->setObjectName("StandardBlueButton");
  gridLayout->addWidget(addButton, 1, 1);

  gridLayout->setColumnStretch(2, 100);
  innerVLayout->addLayout(gridLayout);

  connect(addButton, &QPushButton::clicked, this, &NewProfileView::onAddClicked);
  connect(this, &NewProfileView::addRuleClicked, schedulesView, &SchedulesView::addRuleClicked);
  connect(this, &NewProfileView::addSummerProfileClicked, schedulesView, &SchedulesView::addSummerProfileClicked);
  connect(this, &NewProfileView::addWinterProfileClicked, schedulesView, &SchedulesView::addWinterProfileClicked);

  innerVLayout->addStretch();

  mainHLayout->addStretch();
}

void NewProfileView::onAddClicked()
{
  int currentIndex = m_scheduleRuleComboBox->currentIndex();
  UUID handle = toUUID(m_scheduleRuleComboBox->itemData(currentIndex).toString());

  switch (m_type)
  {
  case SUMMER:
    emit addSummerProfileClicked(m_scheduleRuleset, handle);
    break;
  case WINTER:
    emit addWinterProfileClicked(m_scheduleRuleset, handle);
    break;
  default:
    emit addRuleClicked(m_scheduleRuleset, handle);
  }
}

void NewProfileView::populateComboBox(const model::ScheduleRuleset & scheduleRuleset)
{


  m_scheduleRuleComboBox->addItem("<New Profile>", toQString(UUID()));

  m_scheduleRuleComboBox->addItem("Default Day Schedule", toQString(scheduleRuleset.defaultDaySchedule().handle()));

  if (!scheduleRuleset.isSummerDesignDayScheduleDefaulted()){
    m_scheduleRuleComboBox->addItem("Summer Design Day Schedule", toQString(scheduleRuleset.summerDesignDaySchedule().handle()));
  }

  if (!scheduleRuleset.isWinterDesignDayScheduleDefaulted()){
    m_scheduleRuleComboBox->addItem("Winter Design Day Schedule", toQString(scheduleRuleset.winterDesignDaySchedule().handle()));
  }

  for (const auto& rule : scheduleRuleset.scheduleRules()){
    m_scheduleRuleComboBox->addItem(toQString(rule.name().get()), toQString(rule.daySchedule().handle()));
  }

  m_scheduleRuleComboBox->setCurrentIndex(0);
}

/******************************************************************************/
// ScheduleRulesetNameView
/******************************************************************************/

ScheduleRulesetNameView::ScheduleRulesetNameView(const model::ScheduleRuleset & scheduleRuleset, SchedulesView * schedulesView)
  : QWidget(schedulesView),
  m_schedulesView(schedulesView),
  m_scheduleRuleset(scheduleRuleset)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10, 10, 10, 10);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  auto scheduleRulesetNameWidget = new ScheduleRulesetNameWidget(scheduleRuleset);
  mainVLayout->addWidget(scheduleRulesetNameWidget);

  mainVLayout->addStretch();
}

/******************************************************************************/
// DefaultScheduleDayView
/******************************************************************************/

DefaultScheduleDayView::DefaultScheduleDayView(bool isIP,
                                               const model::ScheduleRuleset & scheduleRuleset,
                                               SchedulesView * schedulesView)
                                               : QWidget(schedulesView)
{
  // Layout

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0, 0, 0, 0);
  mainHLayout->setSpacing(0);
  setLayout(mainHLayout);

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10, 10, 10, 10);
  mainVLayout->setSpacing(10);
  mainHLayout->addLayout(mainVLayout, 100);

  // Name

  auto scheduleRulesetNameWidget = new ScheduleRulesetNameWidget(scheduleRuleset);
  mainVLayout->addWidget(scheduleRulesetNameWidget);

  QLabel * label = new QLabel("Default day profile.");
  label->setObjectName("H2");

  auto hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(60, 0, 0, 10);
  hLayout->addWidget(label);
  mainVLayout->addLayout(hLayout);

  // Schedule Day

  ScheduleDayView * scheduleDayView = new ScheduleDayView(isIP, scheduleRuleset.defaultDaySchedule(), schedulesView);

  connect(this, &DefaultScheduleDayView::toggleUnitsClicked, scheduleDayView, &ScheduleDayView::toggleUnitsClicked);

  mainVLayout->addWidget(scheduleDayView);

  // Year Overview

  auto vLine2 = new QWidget();
  vLine2->setObjectName("VLine");
  vLine2->setStyleSheet("QWidget#VLine { background: #445051;}");
  vLine2->setFixedWidth(2);
  mainHLayout->addWidget(vLine2);

  auto yearOverview = new YearOverview(scheduleRuleset);
  mainHLayout->addWidget(yearOverview);
}

/******************************************************************************/
// SizingScheduleDayView
/******************************************************************************/

SizingScheduleDayView::SizingScheduleDayView(bool isIP,
                                             const model::ScheduleRuleset & scheduleRuleset,
                                             SchedulesView * schedulesView,
                                             SizingScheduleDayType type)
                                             : QWidget(schedulesView),
                                             m_type(type)
{
  // Layout

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0, 0, 0, 0);
  mainHLayout->setSpacing(0);
  setLayout(mainHLayout);

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10, 10, 10, 10);
  mainVLayout->setSpacing(10);
  mainHLayout->addLayout(mainVLayout, 100);

  // Name

  auto scheduleRulesetNameWidget = new ScheduleRulesetNameWidget(scheduleRuleset);
  mainVLayout->addWidget(scheduleRulesetNameWidget);

  // Schedule Day

  if (m_type == SUMMER)
  {
    if (!scheduleRuleset.isSummerDesignDayScheduleDefaulted())
    {
      QLabel * label = new QLabel("Summer design day profile.");
      label->setObjectName("H2");

      auto hLayout = new QHBoxLayout();
      hLayout->setContentsMargins(60, 0, 0, 10);
      hLayout->addWidget(label);
      mainVLayout->addLayout(hLayout);

      model::ScheduleDay scheduleDay = scheduleRuleset.summerDesignDaySchedule();

      auto scheduleDayView = new ScheduleDayView(isIP, scheduleDay, schedulesView);

      connect(this, &SizingScheduleDayView::toggleUnitsClicked, scheduleDayView, &ScheduleDayView::toggleUnitsClicked);

      mainVLayout->addWidget(scheduleDayView);
    }
  } else
  {
    if (!scheduleRuleset.isWinterDesignDayScheduleDefaulted())
    {
      QLabel * label = new QLabel("Winter design day profile.");
      label->setObjectName("H2");

      auto hLayout = new QHBoxLayout();
      hLayout->setContentsMargins(60, 0, 0, 10);
      hLayout->addWidget(label);
      mainVLayout->addLayout(hLayout);

      model::ScheduleDay scheduleDay = scheduleRuleset.winterDesignDaySchedule();

      auto scheduleDayView = new ScheduleDayView(isIP, scheduleDay, schedulesView);

      connect(this, &SizingScheduleDayView::toggleUnitsClicked, scheduleDayView, &ScheduleDayView::toggleUnitsClicked);

      mainVLayout->addWidget(scheduleDayView);
    }
  }
}

/******************************************************************************/
// ScheduleRuleView
/******************************************************************************/

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

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0,0,0,0);
  mainHLayout->setSpacing(0);
  setLayout(mainHLayout);

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10,10,10,10);
  mainVLayout->setSpacing(10);
  mainHLayout->addLayout(mainVLayout,100);

  // Name

  model::ScheduleRuleset scheduleRuleset = scheduleRule.scheduleRuleset();
  auto scheduleRulesetNameWidget = new ScheduleRulesetNameWidget(scheduleRuleset);
  mainVLayout->addWidget(scheduleRulesetNameWidget);

  // Rule Settings

  auto ruleHLayout = new QHBoxLayout();
  ruleHLayout->setContentsMargins(60,0,0,10);
  ruleHLayout->setSpacing(0);
  mainVLayout->addLayout(ruleHLayout);

  auto ruleWidget = new QWidget();
  ruleHLayout->addWidget(ruleWidget);
  ruleHLayout->addStretch(10);

  auto removeButton = new QPushButton();
  removeButton->setObjectName("DeleteButton");
  removeButton->setFlat(true);
  removeButton->setFixedSize(24,24);
  connect(removeButton, &QPushButton::clicked, this, &ScheduleRuleView::onRemoveClicked);
  ruleHLayout->addWidget(removeButton,0,Qt::AlignTop);
  ruleHLayout->addSpacing(20);

  auto ruleVLayout = new QVBoxLayout();
  ruleVLayout->setContentsMargins(0,0,0,0);
  ruleVLayout->setSpacing(5);
  ruleWidget->setLayout(ruleVLayout);

  auto nameHLayout = new QHBoxLayout();
  nameHLayout->setContentsMargins(0,0,0,0);

  auto colorWidget = new QWidget();
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

  QString priorityString("Priority ");
  priorityString.append(QString::number(m_scheduleRule.ruleIndex() + 1));
  auto priorityLabel = new QLabel(priorityString);
  nameHLayout->addWidget(priorityLabel);

  nameHLayout->addSpacing(10);

  m_nameEditField = new OSLineEdit2();

  m_nameEditField->bind(
    m_scheduleRule,
    OptionalStringGetter(std::bind(&model::ScheduleRule::name, &m_scheduleRule,true)),
    boost::optional<StringSetter>(std::bind(&model::ScheduleRule::setName, &m_scheduleRule,std::placeholders::_1))
  );

  nameHLayout->addWidget(m_nameEditField);

  ruleVLayout->addLayout(nameHLayout);

  auto dateHLayout = new QHBoxLayout();

  QLabel * dateRangeLabel = new QLabel("Date Range:");
  dateHLayout->addWidget(dateRangeLabel);

  m_startDateEdit = new QDateTimeEdit();
  m_startDateEdit->setDisplayFormat("MM/dd");
  m_startDateEdit->setCalendarPopup(true);
  dateHLayout->addWidget(m_startDateEdit);
  connect(m_startDateEdit, &QDateTimeEdit::dateTimeChanged, this, &ScheduleRuleView::onStartDateTimeChanged);
  m_endDateEdit = new QDateTimeEdit();
  m_endDateEdit->setDisplayFormat("MM/dd");
  m_endDateEdit->setCalendarPopup(true);
  dateHLayout->addWidget(m_endDateEdit);
  connect(m_endDateEdit, &QDateTimeEdit::dateTimeChanged, this, &ScheduleRuleView::onEndDateTimeChanged);
  dateHLayout->addStretch();

  ruleVLayout->addLayout(dateHLayout);

  auto weekHLayout = new QHBoxLayout();
  QLabel * applyToLabel = new QLabel("Apply to:");
  weekHLayout->addWidget(applyToLabel);

  weekHLayout->addStretch();

  m_sundayButton = new OSCheckBox2();
  m_sundayButton->setText("S");
  m_sundayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applySunday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplySunday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_sundayButton);

  weekHLayout->addSpacing(10);

  m_mondayButton = new OSCheckBox2();
  m_mondayButton->setText("M");
  m_mondayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applyMonday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplyMonday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_mondayButton);

  weekHLayout->addSpacing(10);

  m_tuesdayButton = new OSCheckBox2();
  m_tuesdayButton->setText("T");
  m_tuesdayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applyTuesday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplyTuesday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_tuesdayButton);

  weekHLayout->addSpacing(10);

  m_wednesdayButton = new OSCheckBox2();
  m_wednesdayButton->setText("W");
  m_wednesdayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applyWednesday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplyWednesday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_wednesdayButton);

  weekHLayout->addSpacing(10);

  m_thursdayButton = new OSCheckBox2();
  m_thursdayButton->setText("T");
  m_thursdayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applyThursday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplyThursday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_thursdayButton);

  weekHLayout->addSpacing(10);

  m_fridayButton = new OSCheckBox2();
  m_fridayButton->setText("F");
  m_fridayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applyFriday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplyFriday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_fridayButton);

  weekHLayout->addSpacing(10);

  m_saturdayButton = new OSCheckBox2();
  m_saturdayButton->setText("S");
  m_saturdayButton->bind(
      m_scheduleRule,
      std::bind(&model::ScheduleRule::applySaturday,&m_scheduleRule),
      boost::optional<BoolSetter>(std::bind(&model::ScheduleRule::setApplySaturday,&m_scheduleRule,std::placeholders::_1)));
  weekHLayout->addWidget(m_saturdayButton);

  ruleVLayout->addLayout(weekHLayout);

  // Schedule Day

  m_scheduleDayView = new ScheduleDayView(isIP,m_scheduleRule.daySchedule(),m_schedulesView);

  connect(this, &ScheduleRuleView::toggleUnitsClicked, m_scheduleDayView, &ScheduleDayView::toggleUnitsClicked);

  connect(this, &ScheduleRuleView::toggleUnitsClicked, m_scheduleDayView, &ScheduleDayView::onToggleUnitsClicked);

  mainVLayout->addWidget(m_scheduleDayView);

  // Year Overview

  auto vLine2 = new QWidget();
  vLine2->setObjectName("VLine");
  vLine2->setStyleSheet("QWidget#VLine { background: #445051;}");
  vLine2->setFixedWidth(2);
  mainHLayout->addWidget(vLine2);

  YearOverview * yearOverview = new YearOverview(m_scheduleRule.scheduleRuleset());
  mainHLayout->addWidget(yearOverview);

  // Connect

  m_scheduleRule.getImpl<model::detail::ScheduleRule_Impl>().get()->onChange.connect<ScheduleRuleView, &ScheduleRuleView::scheduleRefresh>(this);

  m_yearDescription->getImpl<model::detail::YearDescription_Impl>().get()->onChange.connect<ScheduleRuleView, &ScheduleRuleView::scheduleRefresh>(this);

  connect(this, &ScheduleRuleView::startDateTimeChanged, m_schedulesView, &SchedulesView::startDateTimeChanged);

  connect(this, &ScheduleRuleView::endDateTimeChanged, m_schedulesView, &SchedulesView::endDateTimeChanged);

  connect(this, &ScheduleRuleView::removeScheduleRuleClicked, m_schedulesView, &SchedulesView::removeScheduleRuleClicked);
  // Refresh

  scheduleRefresh();
}

void ScheduleRuleView::onRemoveClicked()
{
  std::cout << "ScheduleRuleView::onRemoveClicked: " << m_scheduleRule << std::endl;

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

/******************************************************************************/
// ScheduleRulesetNameWidget
/******************************************************************************/

ScheduleRulesetNameWidget::ScheduleRulesetNameWidget(const model::ScheduleRuleset & scheduleRuleset)
  : QWidget(),
  m_scheduleRuleset(scheduleRuleset)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  QLabel * label;

  // Name

  auto hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->setSpacing(10);
  mainVLayout->addLayout(hLayout);

  label = new QLabel("Schedule Name:");
  label->setObjectName("H2");
  hLayout->addWidget(label);

  auto lineEdit = new OSLineEdit2();
  // lineEdit->bind(m_scheduleRuleset, "name");
  opt_scheduleRuleset = m_scheduleRuleset;
  lineEdit->bind(
    *opt_scheduleRuleset,
    OptionalStringGetter(std::bind(&model::ScheduleRuleset::name, opt_scheduleRuleset.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::ScheduleRuleset::setName, opt_scheduleRuleset.get_ptr(),std::placeholders::_1))
  );

  hLayout->addWidget(lineEdit);

  // Schedule Type
  label = new QLabel("Schedule Type:");
  label->setObjectName("H2");
  hLayout->addWidget(label);

  QString scheduleTypeString("");
  boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = scheduleRuleset.scheduleTypeLimits();
  if (scheduleTypeLimits){
    //boost::optional<std::string> numericType = scheduleTypeLimits->numericType();
    //if (numericType){
    //  scheduleTypeString.append(toQString(*numericType));
    //}

    scheduleTypeString.append(toQString(scheduleTypeLimits->name().get()));
  } else {
    scheduleTypeString.append("");
  }
  scheduleTypeString.append("");
  label = new QLabel(scheduleTypeString);
  hLayout->addWidget(label);

  hLayout->addStretch();

  // line
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line);
}

/******************************************************************************/
// YearOverview
/******************************************************************************/

YearOverview::YearOverview(const model::ScheduleRuleset & scheduleRuleset, QWidget * parent)
  : QWidget(parent),
  m_scheduleRuleset(scheduleRuleset),
  m_dirty(false)
{
  refreshActiveRuleIndices();

  auto mainScrollLayout = new QVBoxLayout();

  mainScrollLayout->setContentsMargins(0, 0, 0, 0);

  setLayout(mainScrollLayout);

  auto scrollArea = new QScrollArea();
  scrollArea->setFrameStyle(QFrame::NoFrame);
  scrollArea->setWidgetResizable(true);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  mainScrollLayout->addWidget(scrollArea);

  auto scrollWidget = new QWidget();

  scrollArea->setWidget(scrollWidget);

  auto mainLayout = new QVBoxLayout();

  mainLayout->setContentsMargins(0, 0, 0, 0);

  scrollWidget->setLayout(mainLayout);

  auto gridLayout = new QGridLayout();

  gridLayout->setContentsMargins(0, 0, 0, 0);

  m_januaryView = new MonthView(this);
  m_januaryView->setMonth(1);
  gridLayout->addWidget(m_januaryView, 0, 0);

  m_februaryView = new MonthView(this);
  m_februaryView->setMonth(2);
  gridLayout->addWidget(m_februaryView, 1, 0);

  m_marchView = new MonthView(this);
  m_marchView->setMonth(3);
  gridLayout->addWidget(m_marchView, 2, 0);

  m_aprilView = new MonthView(this);
  m_aprilView->setMonth(4);
  gridLayout->addWidget(m_aprilView, 3, 0);

  m_mayView = new MonthView(this);
  m_mayView->setMonth(5);
  gridLayout->addWidget(m_mayView, 4, 0);

  m_juneView = new MonthView(this);
  m_juneView->setMonth(6);
  gridLayout->addWidget(m_juneView, 5, 0);

  m_julyView = new MonthView(this);
  m_julyView->setMonth(7);
  gridLayout->addWidget(m_julyView, 6, 0);

  m_augustView = new MonthView(this);
  m_augustView->setMonth(8);
  gridLayout->addWidget(m_augustView, 7, 0);

  m_septemberView = new MonthView(this);
  m_septemberView->setMonth(9);
  gridLayout->addWidget(m_septemberView, 8, 0);

  m_octoberView = new MonthView(this);
  m_octoberView->setMonth(10);
  gridLayout->addWidget(m_octoberView, 9, 0);

  m_novemberView = new MonthView(this);
  m_novemberView->setMonth(11);
  gridLayout->addWidget(m_novemberView, 10, 0);

  m_decemberView = new MonthView(this);
  m_decemberView->setMonth(12);
  gridLayout->addWidget(m_decemberView, 11, 0);

  mainLayout->addLayout(gridLayout);

  mainLayout->addStretch(10);

  //m_scheduleRuleset.model().getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<YearOverview, &YearOverview::onModelAdd>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &YearOverview::onModelAdd, Qt::QueuedConnection);


  std::vector<model::ScheduleRule> scheduleRules = m_scheduleRuleset.scheduleRules();

  for (auto it = scheduleRules.begin();
       it < scheduleRules.end();
       ++it)
  {
    it->getImpl<model::detail::ScheduleRule_Impl>().get()->onChange.connect<YearOverview, &YearOverview::scheduleRefresh>(this);
  }

  model::YearDescription yearDescription = m_scheduleRuleset.model().getUniqueModelObject<model::YearDescription>();

  yearDescription.getImpl<model::detail::YearDescription_Impl>().get()->onChange.connect<YearOverview, &YearOverview::scheduleRefresh>(this);

  refresh();
}

void YearOverview::refreshActiveRuleIndices()
{
  int year = m_scheduleRuleset.model().getUniqueModelObject<model::YearDescription>().assumedYear();

  Date startDate(1, 1, year);
  Date endDate(12, 31, year);

  m_activeRuleIndices = m_scheduleRuleset.getActiveRuleIndices(startDate, endDate);

  m_dirty = false;
}

void YearOverview::onModelAdd(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wo,
                              const openstudio::IddObjectType& iddType,
                              const openstudio::UUID& uuid)
{
  boost::optional<model::ScheduleRule> scheduleRule = m_scheduleRuleset.model().getModelObject<model::ScheduleRule>(wo->handle());

  if (scheduleRule)
  {
    if (scheduleRule->scheduleRuleset().handle() == m_scheduleRuleset.handle())
    {
      scheduleRule->getImpl<model::detail::ScheduleRule_Impl>().get()->onChange.connect<YearOverview, &YearOverview::scheduleRefresh>(this);
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

  QTimer::singleShot(0, this, SLOT(refresh()));
}

void YearOverview::refresh()
{
  if (m_dirty)
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

/******************************************************************************/
// MonthView
/******************************************************************************/

MonthView::MonthView(YearOverview * yearOverview)
  : QWidget(yearOverview),
  m_yearOverview(yearOverview),
  m_month(0)
{
  auto mainVLayout = new QVBoxLayout();

  setLayout(mainVLayout);

  mainVLayout->setContentsMargins(0, 0, 0, 0);
  mainVLayout->setSpacing(0);

  m_monthLabel = new QLabel("January");

  mainVLayout->addWidget(m_monthLabel, 0, Qt::AlignCenter);

  mainVLayout->addSpacing(5);

  m_calendarWidget = new ScheduleCalendarWidget(this);
  m_calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
  m_calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
  m_calendarWidget->setNavigationBarVisible(false);
  m_calendarWidget->setSelectionMode(QCalendarWidget::NoSelection);

  mainVLayout->addWidget(m_calendarWidget);

  auto line1 = new QFrame();
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

  QDate startDate(year, m_month, 1);
  QDate endDate(year, m_month, startDate.daysInMonth());

  m_calendarWidget->setMinimumDate(startDate);
  m_calendarWidget->setMaximumDate(endDate);

  m_calendarWidget->update();

  QWidget::update();
}

/******************************************************************************/
// ScheduleCalendarWidget
/******************************************************************************/

ScheduleCalendarWidget::ScheduleCalendarWidget(MonthView * monthView)
  : QCalendarWidget(monthView),
  m_monthView(monthView)
{
}

void ScheduleCalendarWidget::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
  painter->setBrush(QBrush(QColor(230, 230, 230)));

  if (date.month() == m_monthView->month())
  {
    int dayOfYear = date.dayOfYear();

    int ruleIndex = m_monthView->yearOverview()->activeRuleIndices()[dayOfYear - 1];

    QColor ruleColor = SchedulesView::colors[12];

    if (ruleIndex > 12)
    {
      ruleIndex = 12;
    }

    if (ruleIndex > -1)
    {
      ruleColor = SchedulesView::colors[ruleIndex];
    }

    QString dateString = QString::number(date.day());

    painter->setBrush(QBrush(ruleColor));

    painter->drawRect(rect);

    painter->drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, dateString);
  } else
  {
    painter->drawRect(rect);
  }
}


















} // openstudio

