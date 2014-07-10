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

#include "LibraryTabWidget.hpp"
#include <QStackedWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <vector>

namespace openstudio {

LibraryTabWidget::LibraryTabWidget(QWidget * parent)
  : QWidget(parent)
{
  QVBoxLayout * mainLayout = new QVBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(0,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setFixedHeight(29);

  m_tabBar->setObjectName("VBlueGradientWidget");

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  m_pageStack = new QStackedWidget();

  m_pageStack->setStyleSheet("QStackedWidget { border: none;}");

  m_pageStack->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_pageStack);
}


void LibraryTabWidget::addTab( QWidget * widget,
                        const QString & selectedImagePath,
                        const QString & unSelectedImagePath )
{
  QPushButton * button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(29,29));

  m_tabButtons.push_back(button);

  connect(button,SIGNAL(clicked()),this,SLOT(select()));

  m_pageStack->addWidget(widget);

  m_selectedPixmaps.push_back(selectedImagePath); 

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  setCurrentIndex(0);
}

void LibraryTabWidget::select()
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

void LibraryTabWidget::setCurrentIndex(int index)
{
  int xPos = 0;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    button->move(xPos,0);

    xPos = xPos + button->width();

    QString imagePath = m_unSelectedPixmaps[i]; 

    QString style;

    style.append("QPushButton { background: none; background-image: url(\"");
    style.append(imagePath);
    style.append("\"); border: none; }");

    button->setStyleSheet(style); 
  }

  QPushButton * button = m_tabButtons[index];

  QString imagePath = m_selectedPixmaps[index]; 

  QString style;

  style.append("QPushButton { background: none; background-image: url(\"");
  style.append(imagePath);
  style.append("\"); border: none; background-repeat: 0; }");

  button->setStyleSheet(style); 

  m_pageStack->setCurrentIndex(index);
}

void LibraryTabWidget::setCurrentWidget(QWidget * widget)
{
  int i = m_pageStack->indexOf(widget);

  setCurrentIndex(i);
}

} // namespace openstudio

