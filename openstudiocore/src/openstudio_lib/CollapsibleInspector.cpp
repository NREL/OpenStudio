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

#include "CollapsibleInspector.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

namespace openstudio {

CollapsibleInspector::CollapsibleInspector(QString text,
  QWidget * inspector,
  QWidget * parent)
  : QWidget(parent),
  m_header(new CollapsibleInspectorHeader(text)),
  m_inspector(inspector)
{
  createLayout();
}

void CollapsibleInspector::createLayout()
{
  setContentsMargins(0,0,0,0);

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(0,0,0,0);
  mainLayout->setSpacing(0);
  setLayout(mainLayout);

  mainLayout->addWidget(m_header);
  mainLayout->addWidget(m_inspector);

  bool isConnected = connect(m_header,SIGNAL(toggled(bool)),
                             this,SLOT(on_headerToggled(bool)));
  OS_ASSERT(isConnected);
}

///! SLOTS

void CollapsibleInspector::on_headerToggled(bool checked)
{
  if(checked){
    m_inspector->show();
  }
  else{
    m_inspector->hide();
  }
}

////////////////////////////////////////////////////////////////////////////////

CollapsibleInspectorHeader::CollapsibleInspectorHeader(QString text,
  QWidget * parent)
  : QAbstractButton(parent),
  m_text(text)
{
  createLayout();
}

void CollapsibleInspectorHeader::createLayout()
{
  setContentsMargins(0,0,0,0);
  setFixedHeight(50);
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  setObjectName("CollapsibleInspectorHeader");
  setCheckable(true);

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0,0,0,0);
  mainHLayout->setSpacing(10);
  setLayout(mainHLayout);

  // Arrow
  m_arrowLabel = new QLabel();
  mainHLayout->addWidget(m_arrowLabel);

  // Name
  QLabel * textLabel = new QLabel(m_text);
  textLabel->setWordWrap(false);
  textLabel->setObjectName("H2");
  mainHLayout->addWidget(textLabel);

  // Stretch
  mainHLayout->addStretch();

  bool isConnected = connect(this,SIGNAL(toggled(bool)),
                             this,SLOT(on_toggled(bool)));
  OS_ASSERT(isConnected);

  setChecked(false);
}

QSize CollapsibleInspectorHeader::sizeHint() const
{
  return QSize(150,50);
}

void CollapsibleInspectorHeader::setImage(bool isChecked)
{
  if(isChecked){
    m_arrowLabel->setPixmap(QPixmap(":/images/toggle_arrow.png"));
  }else{
    m_arrowLabel->setPixmap(QPixmap(":/images/toggle_arrow_closed.png"));
  }
}

void CollapsibleInspectorHeader::setChecked(bool isChecked)
{
  QAbstractButton::setChecked(isChecked);
  setImage(isChecked);
  if(isChecked)
  {
    QString style;
    style.append("QWidget#CollapsibleInspectorHeader {");
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
    setStyleSheet("QWidget#CollapsibleInspectorHeader { background: #CECECE; border-bottom: 1px solid black;}");
  }
}

void CollapsibleInspectorHeader::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

///! SLOTS

void CollapsibleInspectorHeader::on_toggled(bool checked)
{
  setChecked(checked);
}

} // openstudio
