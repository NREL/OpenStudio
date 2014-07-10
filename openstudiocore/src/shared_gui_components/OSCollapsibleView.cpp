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

#include "OSCollapsibleView.hpp"
#include "OSViewSwitcher.hpp"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

OSHeader::OSHeader(QPushButton * button)
  : QPushButton()
{
  this->setFlat(true);
  toggleButton = button;
}

void OSHeader::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

OSCollapsibleView::OSCollapsibleView(bool alwaysExpanded, QWidget * parent)
  : QWidget(parent),
  m_alwaysExpanded(alwaysExpanded)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);
  
  m_headerContainer = new OSViewSwitcher();
  mainVLayout->addWidget(m_headerContainer);

  m_contentContainer = new OSViewSwitcher();
  mainVLayout->addWidget(m_contentContainer);
  m_contentContainer->hide();
}

void OSCollapsibleView::setHeader(QWidget * header)
{
  m_headerContainer->setView(header);

  m_osHeader = qobject_cast<OSHeader *>(header);

  if( m_osHeader )
  {
    bool bingo = connect(m_osHeader->toggleButton,SIGNAL(toggled(bool)),this,SLOT(setExpanded(bool)));

    OS_ASSERT(bingo);
  }
}

void OSCollapsibleView::setContent(QWidget * content)
{
  m_contentContainer->setView(content);
}

void OSCollapsibleView::expand()
{
  setExpanded(true);
}

void OSCollapsibleView::collapse()
{
  setExpanded(false);
}

void OSCollapsibleView::setExpanded(bool expanded)
{
  if( m_alwaysExpanded || expanded )
  {
    if( m_osHeader )
    {
      m_osHeader->toggleButton->blockSignals(true);
      m_osHeader->toggleButton->setChecked(true);
      m_osHeader->toggleButton->blockSignals(false);
    }

    m_contentContainer->show();
  }
  else
  {

    if( m_osHeader )
    {
      m_osHeader->toggleButton->blockSignals(true);
      m_osHeader->toggleButton->setChecked(false);
      m_osHeader->toggleButton->blockSignals(false);
    }

    m_contentContainer->hide();
  }

}

bool OSCollapsibleView::isExpanded()
{
  return ! m_contentContainer->isHidden();
}

} // openstudio

