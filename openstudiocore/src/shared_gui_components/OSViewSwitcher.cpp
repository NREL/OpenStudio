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

#include "OSViewSwitcher.hpp"

#include <QBoxLayout>
#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>

#include "../utilities/core/Assert.hpp"

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

OSViewSwitcher::~OSViewSwitcher()
{
}

void OSViewSwitcher::setView(QWidget * view)
{
  // Evan note: It's bad to play with null pointers
  // Unfortunately, the app crashes if you don't them
  if (!view) {
    //return;
  }
  if( QWidget * widget = m_stack->currentWidget() )
  {
    m_stack->removeWidget( widget );
    // When we determine why there is a crash in Pat frm not
    // using a null pointer, we can again delte these widgets
    //widget->deleteLater();
  }
  OS_ASSERT(m_stack->count() == 0);

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
