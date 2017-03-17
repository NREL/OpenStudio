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

#include "CollapsibleComponentHeader.hpp"
#include "Component.hpp"
#include "PageNavigator.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>

#define NUM_PAGES_LISTED 5

namespace openstudio {

CollapsibleComponentHeader::CollapsibleComponentHeader(const std::string& text,
                                                       int numResults,
                                                       int numPages,
                                                       QWidget * parent)
  : QAbstractButton(parent),
  m_text(nullptr),
  m_arrowLabel(nullptr),
  m_pageNavigator(nullptr),
  m_numLabel(nullptr)
{
  createLayout(text, numResults,numPages);
}

void CollapsibleComponentHeader::createLayout(const std::string& text, int numResults, int numPages)
{
  setFixedHeight(50);
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  setObjectName("CollapsibleComponentHeader");
  setCheckable(true);

  auto mainHLayout = new QHBoxLayout();
  setLayout(mainHLayout);

  // Arrow
  m_arrowLabel = new QLabel();
  mainHLayout->addWidget(m_arrowLabel);

  // Name
  m_text = new QLabel(QString::fromStdString(text));
  m_text->setWordWrap(true);
  m_text->setObjectName("H2");
  mainHLayout->addWidget(m_text);

  // Stretch
  mainHLayout->addStretch();

  // Page Navigator
  m_pageNavigator = new PageNavigator(numPages,NUM_PAGES_LISTED,this);
  mainHLayout->addWidget(m_pageNavigator);

  connect(m_pageNavigator, &PageNavigator::getComponentsByPage, this, &CollapsibleComponentHeader::getComponentsByPage);

  connect(m_pageNavigator, &PageNavigator::getComponentsByPage, this, &CollapsibleComponentHeader::on_getComponentsByPage);

  // Stretch
  mainHLayout->addStretch();

  // Num results returned
  QString num;
  m_numLabel = new QLabel(num.setNum(numResults));
  m_numLabel->setWordWrap(true);
  m_numLabel->setObjectName("H2");
  mainHLayout->addWidget(m_numLabel);

  setChecked(false);

}

QSize CollapsibleComponentHeader::sizeHint() const
{
  return QSize(150,50);
}

void CollapsibleComponentHeader::setImage(bool isChecked)
{
  if(isChecked){
    m_arrowLabel->setPixmap(QPixmap(":../shared_gui_components/images/toggle_arrow.png"));
  }else{
    m_arrowLabel->setPixmap(QPixmap(":../shared_gui_components/images/toggle_arrow_closed.png"));
  }
}

void CollapsibleComponentHeader::setChecked(bool isChecked)
{
  QAbstractButton::setChecked(isChecked);
  setImage(isChecked);
  if(isChecked)
  {
    QString style;
    style.append("QWidget#CollapsibleComponentHeader {");
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
    setStyleSheet("QWidget#CollapsibleComponentHeader { background: #CECECE; border-bottom: 1px solid black;}");
  }
}

void CollapsibleComponentHeader::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CollapsibleComponentHeader::setText(const QString& text)
{
  m_text->setText(text);
}

void CollapsibleComponentHeader::setNumResults(int numResults)
{
  m_numLabel->setNum(numResults);
}

void CollapsibleComponentHeader::setNumPages(int numPages)
{
  m_pageNavigator->setNumPages(numPages);
}

void CollapsibleComponentHeader::firstPage()
{
  m_pageNavigator->firstPage();
}

///! SLOTS

void CollapsibleComponentHeader::on_getComponentsByPage(int pageIdx)
{
}

} // openstudio
