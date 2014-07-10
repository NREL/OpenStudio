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

#include "VerticalTabWidget.hpp"

#include "../utilities/core/Assert.hpp"

#include <QHBoxLayout>
#include <QKeySequence>
#include <QPixmap>
#include <QPushButton>
#include <QShortcut>
#include <QStackedWidget>
#include <QVBoxLayout>

#include <algorithm>
#include <vector>

namespace openstudio {

VerticalTabWidget::VerticalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  QHBoxLayout * mainLayout = new QHBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(5,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setMinimumWidth(39);

  m_tabBar->setMaximumWidth(39);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  m_pageStack = new QStackedWidget();

  m_pageStack->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_pageStack);

  currentIndex = -1;

  QShortcut* nextTabShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab), this);
  bool isConnected = connect(nextTabShortcut, SIGNAL(activated()), this, SLOT(nextTab()));
  OS_ASSERT(isConnected);

  QShortcut* previousTabShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Tab), this);
  isConnected = connect(previousTabShortcut, SIGNAL(activated()), this, SLOT(previousTab()));
  OS_ASSERT(isConnected);
}

void VerticalTabWidget::addTab( QWidget * widget,
                                int id,
                                QString toolTip,
                                const QString & selectedImagePath,
                                const QString & unSelectedImagePath )
{
  QPushButton * button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(39,42));

  button->setToolTip(toolTip);

  m_tabButtons.push_back(button);

  connect(button,SIGNAL(clicked()),this,SLOT(select()));

  m_pageStack->addWidget(widget);

  m_selectedPixmaps.push_back(selectedImagePath); 

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  m_ids.push_back(id);

  setCurrentIndex(0);
}

void VerticalTabWidget::deleteAllTabs()
{
  for (QPushButton * button : m_tabButtons){
    delete button;
    button = 0;
  }
  m_tabButtons.clear();

  QWidget * widget = 0;
  for(int i = m_pageStack->count() - 1; i >= 0; --i){
    widget = m_pageStack->widget(i);
    m_pageStack->removeWidget(widget);
    delete widget;
    widget = 0;
  }

  m_selectedPixmaps.clear();

  m_unSelectedPixmaps.clear();

  m_ids.clear();

  currentIndex = -1;

}

void VerticalTabWidget::select()
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

void VerticalTabWidget::setCurrentId(int id)
{
  std::vector<int>::iterator it;

  it = std::find(m_ids.begin(),m_ids.end(),id);

  if( it != m_ids.end() )
  {
    setCurrentIndex(*it);
  }
}

void VerticalTabWidget::setCurrentIndex(int index)
{
  int yPos = 25;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    button->move(0,yPos);

    yPos = yPos + button->height();

    QString imagePath = m_unSelectedPixmaps[i]; 

    QString style;

    if (i == static_cast<unsigned>(index))
    {
      imagePath = m_selectedPixmaps[i];

      // Ignore clicks to the already active tab
      if(currentIndex != index){
        currentIndex = index;
        
        style.append("QPushButton { background-color: blue; background-image: url(\"");
        style.append(imagePath);
        style.append("\"); border: none; background-repeat: 0; }");

        button->setStyleSheet(style); 

        m_pageStack->setCurrentIndex(index);

        emit tabSelected(m_ids[index]);
      }
    }
    else
    {
      style.append("QPushButton { background-image: url(\"");
      style.append(imagePath);
      style.append("\"); border: none; background-color: red; background-repeat: 0; }");

      button->setStyleSheet(style); 
    }
  }
}

int VerticalTabWidget::verticalTabIndex()
{
  return m_pageStack->currentIndex();
}

QWidget* VerticalTabWidget::verticalTabWidgetByIndex(int index)
{
  return m_pageStack->widget(index);
}

void VerticalTabWidget::setCurrentWidget(QWidget * widget)
{
  int i = m_pageStack->indexOf(widget);

  setCurrentIndex(i);
}

void VerticalTabWidget::nextTab()
{
  int size = m_pageStack->count(),
      nextIndex = (currentIndex + 1) % size;
  for(int i = 0; i < size; i++)
  {
    if (!m_pageStack->widget(nextIndex)->isEnabled()){
      nextIndex = (nextIndex + 1) % size;
    }else{
      setCurrentIndex(nextIndex);
    }
  }
}

void VerticalTabWidget::previousTab()
{
  int size = m_pageStack->count(),
      previousIndex = (((currentIndex < 0 ? -1 : currentIndex - 1) % size) + size) % size;
  for(int i = 0; i < size; i++)
  {
    if (!m_pageStack->widget(previousIndex)->isEnabled()){
      previousIndex = (((previousIndex - 1) % size) + size) % size;
    }else{
      setCurrentIndex(previousIndex);
    }
  }
}

} // namespace openstudio

