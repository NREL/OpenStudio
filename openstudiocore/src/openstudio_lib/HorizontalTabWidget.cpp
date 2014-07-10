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

#include "HorizontalTabWidget.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QStyleOption>

#include <vector>

#include <algorithm>

namespace openstudio {

HorizontalTabWidget::HorizontalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  //setObjectName("GrayWidget");

  QVBoxLayout * mainLayout = new QVBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(0,0,0,0);

  setLayout(mainLayout);

  // Tab bar

  m_tabBar = new QWidget(this);

  m_tabBar->setFixedHeight(31);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  // Tab bar line

  m_tabBarLine = new QWidget(m_tabBar);

  m_tabBarLine->setFixedHeight(5);

  m_tabBarLine->move(0,m_tabBar->height() - 5);

  m_tabBarLine->setStyleSheet("QWidget { border-top: 1px solid black; background-color: #95B3DE; }");

  // Page stack
  
  m_pageStack = new QStackedWidget();

  m_pageStack->setObjectName("GrayWidget");

  m_pageStack->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_pageStack);
}


void HorizontalTabWidget::addTab( QWidget * widget,
                                  int id,
                                  const QString & label )
{
  QPushButton * button = new QPushButton(m_tabBar);

  button->setText(label);

  button->setFixedHeight(27);

  m_tabButtons.push_back(button);

  connect(button,SIGNAL(clicked()),this,SLOT(select()));

  m_pageStack->addWidget(widget);

  m_ids.push_back(id);

  setCurrentIndex(0);
}

void HorizontalTabWidget::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( std::vector<QPushButton*>::iterator  it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it )
  {
    if( *it == button )
    {
      break;
    }
    else
    {
      index++;
    }
  } 

  setCurrentIndex(index);
}

void HorizontalTabWidget::setCurrentId(int id)
{
  std::vector<int>::iterator it;

  it = std::find(m_ids.begin(),m_ids.end(),id);

  if( it != m_ids.end() )
  {
    setCurrentIndex(*it);
  }
}

void HorizontalTabWidget::setCurrentIndex(int index)
{
  int xPos = 0;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    if(button->isHidden()){
      button->move(0,0);
      continue;
    }

    QString style;

    style.append("QPushButton { border: none; background-color: #808080; ");
    style.append("              border-right: 1px solid black;");
    style.append("              border-bottom: 1px solid black;");
    style.append("              padding-left: 10px;");
    style.append("              padding-right: 10px;");
    style.append("              padding-top: 5px;");
    style.append("              color: white;");
    if( i == m_tabButtons.size() - 1 )
    {
      style.append("            border-right: none;");
    }
    style.append("}");

    button->setStyleSheet(style); 

    button->adjustSize();

    button->move(xPos,0);

    xPos = xPos + button->width();
  }

  m_tabBarLine->setFixedWidth(xPos);
  m_tabBarLine->raise();

  QPushButton * button = m_tabButtons[index];

  QString style;

  style.append("QPushButton { border: none; background-color: #95B3DE; ");
  style.append("              border-right: 1px solid black;");
  style.append("              padding-left: 10px;");
  style.append("              padding-right: 10px;");
  style.append("              padding-top: 5px;");
  style.append("              color: white;");
  if( index == int(m_tabButtons.size() - 1) )
  {
    style.append("            border-right: none;");
  }
  style.append("}");

  button->setStyleSheet(style); 
  button->raise();

  m_pageStack->setCurrentIndex(index);
}

void HorizontalTabWidget::setCurrentWidget(QWidget * widget)
{
  int i = m_pageStack->indexOf(widget);

  setCurrentIndex(i);
}

void HorizontalTabWidget::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void HorizontalTabWidget::hideTab(QWidget * widget, bool hide)
{
  int index = m_pageStack->indexOf(widget);
  OS_ASSERT(index >= 0);
  
  int currentIndex = m_pageStack->currentIndex();
  if(currentIndex == index){
    if(currentIndex + 1 < m_pageStack->count()){
      currentIndex++;
    } else if (currentIndex != 0) {
      currentIndex = 0;
    } else {
      // index and currentIndex are both 0
      // can't hide both the tab and the page
      return;
    }
  }

  QPushButton * button = 0;
  button = m_tabButtons.at(index);
  OS_ASSERT(button);
  if(hide){
    button->hide();
  } else {
    button->show();
  }

  setCurrentIndex(currentIndex);
}

} // namespace openstudio

