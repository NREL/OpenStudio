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

#include "LibraryTabWidget.hpp"

#include <QBoxLayout>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>

#include "../utilities/core/Assert.hpp"

#include <vector>

namespace openstudio {

LibraryTabWidget::LibraryTabWidget(QWidget * parent)
  : QWidget(parent)
{
  auto mainLayout = new QVBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(0,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setFixedHeight(29);

  m_tabBar->setObjectName("VBlueGradientWidget");

  m_tabBar->setContentsMargins(0,0,5,0);

  m_removeButton = new QPushButton(this);

  QString str;
  str.append("QWidget { ");
  str.append("border: none;");
  str.append(" background-image: url(\":/images/delete.png\")");
  str.append("}");

  m_removeButton->setFlat(true);
  m_removeButton->setStyleSheet(str);
  m_removeButton->setFixedSize(20, 20);
  m_removeButton->setToolTip("Remove object");

  // Default to hide
  m_removeButton->hide();

  auto isConnected = connect(m_removeButton, SIGNAL(clicked(bool)), this, SIGNAL(removeButtonClicked(bool)));
  OS_ASSERT(isConnected);
 
  auto hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->addStretch();
  hLayout->addWidget(m_removeButton, 0, Qt::AlignVCenter);

  m_tabBar->setLayout(hLayout);

  layout()->addWidget(m_tabBar);

  m_pageStack = new QStackedWidget();

  m_pageStack->setStyleSheet("QStackedWidget { border: none;}");

  m_pageStack->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_pageStack);
}

void LibraryTabWidget::showRemoveButton()
{
  m_removeButton->show();
}

void LibraryTabWidget::hideRemoveButton()
{
  m_removeButton->hide();
}

void LibraryTabWidget::addTab( QWidget * widget,
                        const QString & selectedImagePath,
                        const QString & unSelectedImagePath )
{
  auto button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(29,29));

  m_tabButtons.push_back(button);

  connect(button, &QPushButton::clicked, this, &LibraryTabWidget::select);

  m_pageStack->addWidget(widget);

  m_selectedPixmaps.push_back(selectedImagePath); 

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  setCurrentIndex(0);
}

void LibraryTabWidget::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
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

