/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(9, 0, 9, 0);
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
