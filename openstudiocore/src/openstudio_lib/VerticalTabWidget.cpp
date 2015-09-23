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

#include "VerticalTabWidget.hpp"

#include "MainTabView.hpp"

#include "../shared_gui_components/OSViewSwitcher.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QPushButton>

#include <algorithm>

#include <vector>

namespace openstudio {

VerticalTabWidget::VerticalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  auto mainLayout = new QHBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(5,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setMinimumWidth(39);

  m_tabBar->setMaximumWidth(39);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  m_viewSwitcher = new OSViewSwitcher();

  m_viewSwitcher->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_viewSwitcher);
}

void VerticalTabWidget::addTabButton(int id,
  QString toolTip,
  const QString & selectedImagePath,
  const QString & unSelectedImagePath,
  const QString & disabledImagePath)
{
  auto button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(39, 42));

  button->setToolTip(toolTip);

  m_tabButtons.push_back(button);

  connect(button, &QPushButton::clicked, this, &VerticalTabWidget::select);

  m_selectedPixmaps.push_back(selectedImagePath);

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  m_disabledPixmaps.push_back(disabledImagePath);

  m_ids.push_back(id);
}

void VerticalTabWidget::setView(MainTabView * view, int id)
{
  m_viewSwitcher->setView(view);

  setCurrentIndex(getIndex(id));
}

MainTabView * VerticalTabWidget::view() const
{
  MainTabView * view = qobject_cast<MainTabView *>(m_viewSwitcher->view());
  OS_ASSERT(view);

  return view;
}

void VerticalTabWidget::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it ) {
    if( *it == button ){
      break;
    } else {
      index++;
    }
  } 

  setCurrentIndex(index);
}

int VerticalTabWidget::getIndex(int id)
{
  int index = -1;
  std::vector<int>::iterator it;

  it = std::find(m_ids.begin(),m_ids.end(),id);

  if( it != m_ids.end() ){
    index = it - m_ids.begin();
  }
  OS_ASSERT(index >= 0);
  return index;
}

void VerticalTabWidget::refreshTabButtons()
{
  setCurrentIndex(currentIndex);
}

void VerticalTabWidget::setCurrentIndex(int index)
{
  int yPos = 25;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    button->move(0,yPos);

    yPos = yPos + button->height();

    QString imagePath;

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

        currentIndex = index;

        emit tabSelected(m_ids[index]);
      }
    }
    else
    {
      if (button->isEnabled()){
        imagePath = m_unSelectedPixmaps[i];
        style.append("QPushButton { background-image: url(\"");
        style.append(imagePath);
        style.append("\"); border: none; background-color: red; background-repeat: 0; }");
      }
      else {
        imagePath = m_disabledPixmaps[i];
        style.append("QPushButton { background-image: url(\"");
        style.append(imagePath);
        style.append("\"); border: none; background-color: red; background-repeat: 0; }");
      }

      button->setStyleSheet(style); 
    }
  }
}

void VerticalTabWidget::enableTabButton(int id, bool enable)
{
  m_tabButtons.at(getIndex(id))->setEnabled(enable);
}

int VerticalTabWidget::verticalTabIndex()
{
  return currentIndex;
}

} // namespace openstudio
