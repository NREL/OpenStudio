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

#include "PatMainTabView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>

#include <vector>

namespace openstudio {
  
namespace pat { 

PatMainTabView::PatMainTabView()
  : QWidget()
{
  this->setObjectName("OrangeGradientWidget");

  m_titleLabel = new QLabel(this);
  m_titleLabel->setFixedHeight(20);
  m_titleLabel->setStyleSheet("QLabel { font: bold; }");
  m_titleLabel->move(7,5);

  m_mainWidget = new QWidget(this);
  m_mainWidget->setObjectName("MainTabView");
  QString style;
  style.append("QWidget#MainTabView { " );
  style.append("  background: #E6E6E6; " );
  style.append("  border-left: 1px solid black; " );
  style.append("  border-top: 1px solid black; " );
  style.append("}" );
  m_mainWidget->setStyleSheet(style);

  m_mainWidget->move(7,30);

  auto innerLayout = new QVBoxLayout();
  innerLayout->setContentsMargins(1,1,0,0);
  innerLayout->setSpacing(0);
  m_mainWidget->setLayout(innerLayout);

  viewSwitcher = new OSViewSwitcher(); 
  innerLayout->addWidget(viewSwitcher);
}

void PatMainTabView::setTitle(const QString & title)
{
  m_titleLabel->setText(title);
  m_titleLabel->adjustSize();
}

void PatMainTabView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PatMainTabView::resizeEvent( QResizeEvent * event )
{
  QSize size = event->size();  

  QSize newSize(size.width() - 7,size.height() - 30);

  m_mainWidget->resize(newSize);
}

} // namespace pat

}

