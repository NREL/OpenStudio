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

#include "PatVerticalTabWidget.hpp"

#include "../shared_gui_components/OSViewSwitcher.hpp"

#include "../utilities/core/Assert.hpp"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QShortcut>
#include <QTimer>
#include <QVBoxLayout>

#include <algorithm>
#include <vector>

namespace openstudio {
  
namespace pat {

PatVerticalTabWidget::PatVerticalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  m_buttonGroup = new QButtonGroup(this);
  m_buttonGroup->setExclusive(true);

  bool isConnected = false;

  isConnected = connect(m_buttonGroup,SIGNAL(buttonClicked(int)),
                        this,SLOT(on_buttonClicked(int)));
  OS_ASSERT(isConnected);

  auto mainLayout = new QHBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(5,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setMinimumWidth(39);

  m_tabBar->setMaximumWidth(39);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  mainViewSwitcher = new OSViewSwitcher();

  layout()->addWidget(mainViewSwitcher);

  currentIndex = -1;

  QShortcut* nextTabShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab), this);
  isConnected = connect(nextTabShortcut, SIGNAL(activated()), this, SLOT(nextTab()));
  OS_ASSERT(isConnected);

  QShortcut* previousTabShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Tab), this);
  isConnected = connect(previousTabShortcut, SIGNAL(activated()), this, SLOT(previousTab()));
  OS_ASSERT(isConnected);
}

PatVerticalTabWidget::~PatVerticalTabWidget() 
{ 
  if (mainViewSwitcher){
    mainViewSwitcher->clear(); 
  }
}

void PatVerticalTabWidget::addTab( int id,
                                   QString toolTip,
                                   const QString & selectedImagePath,
                                   const QString & unSelectedImagePath,
                                   const QString & disabledImagePath,
                                   const QString & doneImagePath,
                                   bool enabled )
{
  auto button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(39,42));

  button->setToolTip(toolTip);

  button->setCheckable(true);

  button->setEnabled(enabled);

  m_buttonGroup->addButton(button);
  m_buttonGroup->setId(button,m_buttonGroup->buttons().size() - 1);

  m_selectedPixmaps.push_back(selectedImagePath); 

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  m_disabledPixmaps.push_back(disabledImagePath);

  m_donePixmaps.push_back(doneImagePath);

  setCurrentIndex(0);
}

void PatVerticalTabWidget::enableTab(bool enable, int index)
{
  OS_ASSERT(index >= 0);
  OS_ASSERT(index < (int)m_buttonGroup->buttons().size());

  // Ignore disabling disabled tabs, and enabling enabled tabs
  if (enable == m_buttonGroup->button(index)->isEnabled())
  {
    return;
  }

  m_buttonGroup->button(index)->setEnabled(enable);
  QTimer::singleShot(0, this, SLOT(refreshIcons()));
}

void PatVerticalTabWidget::enableTabs(bool enable)
{
  for(int i=0; i<m_buttonGroup->buttons().size(); i++)
  {
    enableTab(enable,i);
  }
}

void PatVerticalTabWidget::selectTab(int id)
{
  //TODO m_buttonGroup->button(id)->setChecked(true);
  m_buttonGroup->button(id)->click();
}

void PatVerticalTabWidget::selectNone()
{
  for(int i = 0; i < m_buttonGroup->buttons().size(); i++)
  {  
    QAbstractButton * button = m_buttonGroup->button(i);

    button->setChecked(false);

    currentIndex = -1;
  }
  QTimer::singleShot(0, this, SLOT(refreshIcons()));
}

void PatVerticalTabWidget::nextTab()
{
  int size = m_buttonGroup->buttons().size(),
      nextIndex = (currentIndex + 1) % size;
  for(int i = 0; i < size; i++)
  {
    if (!m_buttonGroup->button(nextIndex)->isEnabled()){
      nextIndex = (nextIndex + 1) % size;
    }else{
      selectTab(nextIndex);
    }
  }
}

void PatVerticalTabWidget::previousTab()
{
  int size = m_buttonGroup->buttons().size(),
      previousIndex = (((currentIndex < 0 ? -1 : currentIndex - 1) % size) + size) % size;
  for(int i = 0; i < size; i++)
  {
    if (!m_buttonGroup->button(previousIndex)->isEnabled()){
      previousIndex = (((previousIndex - 1) % size) + size) % size;
    }else{
      selectTab(previousIndex);
    }
  }
}

void PatVerticalTabWidget::setCurrentIndex(int index)
{
  int yPos = 25;
  
  for(int i = 0; i < m_buttonGroup->buttons().size(); i++)
  {
    QAbstractButton * button = m_buttonGroup->button(i);

    button->move(0,yPos);

    yPos += button->height();

    // Ignore clicks to the already active tab
    if(button->isEnabled() && button->isChecked() && currentIndex != index){
      currentIndex = index;
      emit tabSelected(i);
    }
  }
  QTimer::singleShot(0, this, SLOT(refreshIcons()));
}

void PatVerticalTabWidget::refreshIcons()
{
  for(int i = 0; i < m_buttonGroup->buttons().size(); i++)
  {
    // TODO "done" not handled
    //imagePath = m_donePixmaps[i]; 

    QAbstractButton * button = m_buttonGroup->button(i);
  
    QString imagePath;
    if(button->isEnabled() && button->isChecked()){
      imagePath = m_selectedPixmaps[i];
    }
    else if(button->isEnabled() && !button->isChecked()){
      imagePath = m_unSelectedPixmaps[i];
    }
    else if(!button->isEnabled() && button->isChecked()){
      imagePath = m_selectedPixmaps[i];
    }
    else if(!button->isEnabled() && !button->isChecked()){
      imagePath = m_disabledPixmaps[i];
    }
    else{
      // you should not be here
      OS_ASSERT(false);
    }

    QString style;
    style.append("QPushButton { background-color: blue; background-image: url(\"");
    style.append(imagePath);
    style.append("\"); border: none; background-repeat: 0; }");

    button->setStyleSheet(style);
  }
}

//////////////////////////////// SLOTS ///////////////////////////////

void PatVerticalTabWidget::on_buttonClicked(int id)
{
  setCurrentIndex(id);
}

} // namespace pat

}

