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

#include "OSViewSwitcher.hpp"
#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QStyleOption>
#include <QPainter>

namespace openstudio {

OSViewSwitcher::OSViewSwitcher(QWidget * parent)
  : QWidget(parent),
    m_view(nullptr)
{
  auto layout = new QVBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);

  m_stack = new QStackedWidget();

  layout->addWidget(m_stack);
}

void OSViewSwitcher::setView(QWidget * view)
{
  if( QWidget * widget = m_stack->currentWidget() )
  {
    m_stack->removeWidget( widget );
  }

  m_view = view;

  m_stack->addWidget(m_view);
}

QWidget * OSViewSwitcher::view() const
{
  return m_view;
}

void OSViewSwitcher::clear()
{
  if( m_view )
  {
    m_stack->removeWidget(m_view);    
  }

  m_view = nullptr;
}

void OSViewSwitcher::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

}
