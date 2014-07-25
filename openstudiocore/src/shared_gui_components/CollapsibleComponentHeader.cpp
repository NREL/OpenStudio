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

  bool isConnected = false;

  isConnected = connect(m_pageNavigator, SIGNAL(getComponentsByPage(int)),
                             this, SIGNAL(getComponentsByPage(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_pageNavigator, SIGNAL(getComponentsByPage(int)),
                        this, SLOT(on_getComponentsByPage(int)));
  OS_ASSERT(isConnected);

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
