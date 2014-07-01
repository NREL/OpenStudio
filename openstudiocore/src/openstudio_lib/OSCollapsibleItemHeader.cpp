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

#include "OSCollapsibleItemHeader.hpp"

#include "OSItem.hpp"
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

namespace openstudio {

OSCollapsibleItemHeader::OSCollapsibleItemHeader(const std::string & text,
                                                 const OSItemId& itemId,
                                                 OSItemType type,
                                                 QWidget * parent)
  : QWidget(parent),
    m_mouseDown(false)
{
  setFixedHeight(50);
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  setObjectName("OSCollapsibleItemHeader");

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  setLayout(mainHLayout);

  // Label

  QLabel * textLabel = new QLabel(QString::fromStdString(text));
  textLabel->setWordWrap(true);
  textLabel->setObjectName("H2");
  mainHLayout->addWidget(textLabel,10);

  mainHLayout->addStretch();

  // Arrow

  m_arrowLabel = new QLabel();
  mainHLayout->addWidget(m_arrowLabel);

  setSelected(false);

  setExpanded(false);
}

QSize OSCollapsibleItemHeader::sizeHint() const
{
  return QSize(150,50);
}

bool OSCollapsibleItemHeader::expanded() const
{
  return m_expanded;
}

void OSCollapsibleItemHeader::setExpanded(bool expanded)
{
  m_expanded = expanded;
  setImage(m_expanded);
}

void OSCollapsibleItemHeader::setImage(bool expanded)
{
  if(m_expanded){
    m_arrowLabel->setPixmap(QPixmap(":/images/toggle_arrow.png"));
  }else{
    m_arrowLabel->setPixmap(QPixmap(":/images/toggle_arrow_closed.png"));
  }
}

bool OSCollapsibleItemHeader::selected() const
{
  return m_selected;
}

void OSCollapsibleItemHeader::setSelected(bool isSelected)
{
  m_selected = isSelected;

  if( m_selected )
  {
    QString style;
    style.append("QWidget#OSCollapsibleItemHeader {");
    style.append(" background: qlineargradient(x1:0,y1:0,x2:0,y2:1,");
    style.append(" stop: 0.0 #636161,");
    style.append(" stop: 0.10 #636161,");
    style.append(" stop: 0.15 #A3A3A3,");
    style.append(" stop: 1.0 #A3A3A3);");
    style.append(" border-bottom: 1px solid black;");
    style.append(" }");
    setStyleSheet(style);
  }
  else
  {
    setStyleSheet("QWidget#OSCollapsibleItemHeader { background: #CECECE; border-bottom: 1px solid black;}");
  }
}

void OSCollapsibleItemHeader::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OSCollapsibleItemHeader::mouseReleaseEvent(QMouseEvent * event)
{
  if (event->button() == Qt::LeftButton){
    if( m_mouseDown ){
      emit clicked(this);
      setSelected(true);
      m_mouseDown = false;
    }
  }
}

void OSCollapsibleItemHeader::mousePressEvent(QMouseEvent * event)
{
  if (event->button() == Qt::LeftButton){
    m_mouseDown = true;
  }
}

void OSCollapsibleItemHeader::leaveEvent(QEvent * event)
{
  m_mouseDown = false;
}

} // openstudio
