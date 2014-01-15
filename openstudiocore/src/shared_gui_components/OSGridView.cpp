/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "OSGridView.hpp"

#include <QAbstractButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include <model/Model_impl.hpp>
#include <model/ModelObject_impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

//void BillingPeriodWidget::attach(openstudio::model::BillingPeriod & billingPeriod)
//{
//  m_billingPeriod = billingPeriod;
//
//  if(m_billingPeriodIntEdit){
//    m_billingPeriodIntEdit->bind(
//      *m_billingPeriod,
//      OptionalIntGetter(boost::bind(&model::BillingPeriod::numberOfDays,m_billingPeriod.get_ptr())),
//      boost::optional<IntSetter>(boost::bind(&model::BillingPeriod::setNumberOfDays,m_billingPeriod.get_ptr(),_1)));
//  }
//}

OSGridView::OSGridView(std::vector<model::ModelObject> modelObjects, QWidget * parent)
  : QWidget(parent)
{
  //QButtonGroup * buttonGroup = new QButtonGroup();

  //HBoxLayout * buttonLayout = new HBoxLayout();

  //QPushButton * button = 0;

  //for(unsigned i=0; i<m_categories.size(); i++){
  //  button = new QPushButton();
  //  buttonLayout->addWidget(button);
  //}

  m_gridLayout = new QGridLayout();
  m_gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridLayout->setAlignment(Qt::AlignTop);
  setLayout(m_gridLayout);

  setContentsMargins(5,5,5,5);
}

void OSGridView::refreshAll()
{
  QLayoutItem *child;
  while((child = m_gridLayout->takeAt(0)) != 0)
  {
      QWidget * widget = child->widget();

      OS_ASSERT(widget);

      delete widget;

      delete child;
  }
}

void OSGridView::refresh(int row, int column)
{
}

CollapsibleHeader::CollapsibleHeader(const std::string& text,
  QWidget * parent)
  : QAbstractButton(parent),
  m_text(NULL),
  m_arrowLabel(NULL)
{
  createLayout(text);
}

void CollapsibleHeader::createLayout(const std::string& text)
{
  setFixedHeight(50);
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  setObjectName("CollapsibleHeader");
  setCheckable(true);

  QHBoxLayout * mainHLayout = new QHBoxLayout();
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

  setChecked(false);
}

void CollapsibleHeader::setImage(bool isChecked)
{
  if(isChecked){
    m_arrowLabel->setPixmap(QPixmap(":../shared_gui_components/images/toggle_arrow.png"));
  }else{
    m_arrowLabel->setPixmap(QPixmap(":../shared_gui_components/images/toggle_arrow_closed.png"));
  }
}

void CollapsibleHeader::setChecked(bool isChecked)
{
  QAbstractButton::setChecked(isChecked);
  setImage(isChecked);
  if(isChecked)
  {
    QString style;
    style.append("QWidget#CollapsibleHeader {");
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
    setStyleSheet("QWidget#CollapsibleHeader { background: #CECECE; border-bottom: 1px solid black;}");
  }
}

void CollapsibleHeader::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CollapsibleHeader::setText(const QString& text)
{
  m_text->setText(text);
}

} // openstudio

