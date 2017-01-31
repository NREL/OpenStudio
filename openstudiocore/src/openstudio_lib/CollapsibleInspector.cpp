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

  auto mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(0,0,0,0);
  mainLayout->setSpacing(0);
  setLayout(mainLayout);

  mainLayout->addWidget(m_header);
  mainLayout->addWidget(m_inspector);

  connect(m_header, &CollapsibleInspectorHeader::toggled, this, &CollapsibleInspector::on_headerToggled);
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

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(10,0,10,0);
  mainHLayout->setSpacing(7);
  setLayout(mainHLayout);

  // Arrow
  m_arrowLabel = new QLabel();
  mainHLayout->addWidget(m_arrowLabel, 0, Qt::AlignLeft);

  // Name
  auto textLabel = new QLabel(m_text);
  textLabel->setWordWrap(false);
  textLabel->setObjectName("H2");
  mainHLayout->addWidget(textLabel, 0, Qt::AlignLeft);

  // Stretch
  mainHLayout->addStretch();

  connect(this, &CollapsibleInspectorHeader::toggled, this, &CollapsibleInspectorHeader::on_toggled);

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
    m_arrowLabel->setPixmap(QPixmap(":/images/toggle_arrow_closed2.png"));
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
