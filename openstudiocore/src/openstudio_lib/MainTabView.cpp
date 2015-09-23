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

#include "MainTabView.hpp"

#include "../utilities/core/Assert.hpp"

#include <QStackedWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QResizeEvent>
#include <vector>

static const int TAB_SEPARATION = 10;

namespace openstudio {

MainTabView::MainTabView(const QString & tabLabel, TabType tabType, QWidget * parent)
  : QWidget(parent),
  m_tabType(tabType)
{
  this->setObjectName("BlueGradientWidget");

  m_tabLabel = new QLabel(tabLabel,this);
  m_tabLabel->setFixedHeight(20);
  m_tabLabel->setStyleSheet("QLabel { color: white; }");
  m_tabLabel->adjustSize();
  m_tabLabel->setFixedWidth(m_tabLabel->width());
  m_tabLabel->move(7,5);

  m_mainWidget = new QWidget(this);
  m_mainWidget->setObjectName("MainTabView");
  m_mainWidget->move(7,25);

  auto innerLayout = new QVBoxLayout();
  innerLayout->setSpacing(0);
  m_mainWidget->setLayout(innerLayout);

  m_stackedWidget = new QStackedWidget();
  m_stackedWidget->setContentsMargins(0,0,0,0);
  innerLayout->addWidget(m_stackedWidget);

  setTabType(tabType);
}

void MainTabView::setTabType(MainTabView::TabType tabType)
{
  QString style;

  switch (tabType)
  {
  case MAIN_TAB:
    style.append("QWidget#MainTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(0, 0, 0, 0);
    break;
  case SUB_TAB:
    style.append("QWidget#MainTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("  border-top-left-radius: 5px; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(7, 10, 0, 0);
    break;
  case GRIDVIEW_SUB_TAB:
    style.append("QWidget#MainTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("  border-top-left-radius: 5px; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(1, 2, 0, 0);
    break;
  default:
    OS_ASSERT(false);
  }

  m_mainWidget->setStyleSheet(style);
}

bool MainTabView::addTabWidget(QWidget * widget)
{
  // This method should only be called in cases where the tab will not have sub tabs
  OS_ASSERT(m_tabType == MAIN_TAB);
  if (m_tabType != MAIN_TAB) return false;

  m_stackedWidget->addWidget(widget);
  return true;
}

bool MainTabView::addSubTab(const QString & subTablabel, QWidget * widget, int id)
{
  // This method should only be called in cases where the tab will have sub tabs
  OS_ASSERT(m_tabType != MAIN_TAB);
  if (m_tabType == MAIN_TAB) return false;

  auto button = new QPushButton(this);
  button->setText(subTablabel);
  button->setFixedHeight(21);
  m_tabButtons.push_back(button);
  connect(button, &QPushButton::clicked, this, &MainTabView::select);

  m_stackedWidget->addWidget(widget);

  m_ids.push_back(id);

  setCurrentIndex(0);
  return true;
}

void MainTabView::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it ){
    if( *it == button ){
      break;
    } else {
      index++;
    }
  } 

  setCurrentIndex(index);
}

void MainTabView::setCurrentIndex(int index)
{
  int xPos = m_tabLabel->width() + TAB_SEPARATION;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];
    QString style;

    style.append("QPushButton { border: none; background-color: #BBCDE3; ");
    style.append("              border-right: 1px solid black;");
    style.append("              border-bottom: 1px solid black;");
    style.append("              border-top: 1px solid black;");
    style.append("              border-left: 1px solid black;");
    style.append("              border-top-left-radius: 5px;");
    style.append("              border-top-right-radius: 5px;");
    style.append("              padding-left: 10px;");
    style.append("              padding-right: 10px;");
    style.append("              color: black;");
    style.append("}");

    button->setStyleSheet(style); 
    button->adjustSize();
    button->move(xPos,5);

    button->stackUnder(m_mainWidget);

    xPos += TAB_SEPARATION + button->width();
  }

  QPushButton * button = m_tabButtons[index];

  QString style;

  style.append("QPushButton { border: none; background-color: #E6E6E6; ");
  style.append("              border-right: 1px solid black;");
  style.append("              border-bottom: none;");
  style.append("              border-top: 1px solid black;");
  style.append("              border-left: 1px solid black;");
  style.append("              border-top-left-radius: 5px;");
  style.append("              border-top-right-radius: 5px;");
  style.append("              padding-left: 10px;");
  style.append("              padding-right: 10px;");
  style.append("              color: black;");
  style.append("}");

  button->setStyleSheet(style); 
  button->raise();

  m_stackedWidget->setCurrentIndex(index);

  emit tabSelected(m_ids[index]);
}

void MainTabView::setCurrentWidget(QWidget * widget)
{
  int i = m_stackedWidget->indexOf(widget);

  setCurrentIndex(i);
}

void MainTabView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainTabView::resizeEvent( QResizeEvent * event )
{
  QSize size = event->size();  

  QSize newSize(size.width() - 7,size.height() - 25);

  m_mainWidget->resize(newSize);
}

int MainTabView::subTabId() const
{
  if (m_tabType != MAIN_TAB)
  {
    return m_ids[subTabIndex()];
  }
  else
  {
    return -1;
  }
}

int MainTabView::subTabIndex() const
{
  return m_stackedWidget->currentIndex();
}

bool MainTabView::selectSubTabByIndex(int index)
{
  if (m_tabType != MAIN_TAB){
    if(index < 0 || index >= m_stackedWidget->count()){
      return false;
    } else {
      setCurrentIndex(index);
      return true;
    }
  }
  return false;
}

} // namespace openstudio

