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

#include "EditView.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QPainter>
#include <QTextEdit>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QScrollArea>
#include <QCheckBox>
#include <QWheelEvent>
#include <QApplication>

#include "../utilities/core/Assert.hpp"

namespace openstudio{
  
EditRubyMeasureView::EditRubyMeasureView(bool applyMeasureNow)
  : QWidget()
{
  auto layout = new QVBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);
  auto scrollArea = new QScrollArea();
  layout->addWidget(scrollArea);
  scrollArea->setWidgetResizable(true);

  auto scrollWidget = new QWidget();
  scrollWidget->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Preferred);
  scrollArea->setWidget(scrollWidget);

  m_mainVLayout = new QVBoxLayout();
  m_mainVLayout->setContentsMargins(5,5,5,5);
  m_mainVLayout->setSpacing(5);
  m_mainVLayout->setAlignment(Qt::AlignTop);
  scrollWidget->setLayout(m_mainVLayout);

  QLabel * measureOptionTitleLabel = new QLabel("Name");
  measureOptionTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(measureOptionTitleLabel);

  QRegExp nameRegex("^\\S.*");
  auto validator = new QRegExpValidator(nameRegex, this);

  nameLineEdit = new QLineEdit();
  nameLineEdit->setValidator(validator);
  m_mainVLayout->addWidget(nameLineEdit);

  QLabel * descriptionTitleLabel = new QLabel("Description");
  descriptionTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(descriptionTitleLabel);

  descriptionTextEdit = new QTextEdit();
  descriptionTextEdit->setFixedHeight(70);
  descriptionTextEdit->setAcceptRichText(false);
  descriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  descriptionTextEdit->setTabChangesFocus(true);
  m_mainVLayout->addWidget(descriptionTextEdit);

  QLabel * modelerDescriptionTitleLabel = new QLabel("Modeler Description");
  modelerDescriptionTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(modelerDescriptionTitleLabel);

  modelerDescriptionLabel = new QLabel();
  modelerDescriptionLabel->setWordWrap(true);
  m_mainVLayout->addWidget(modelerDescriptionLabel);

  auto line2 = new QFrame();
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Sunken);
  m_mainVLayout->addWidget(line2);

  QLabel * inputsTitleLabel = new QLabel("Inputs");
  inputsTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(inputsTitleLabel);

  m_inputsVLayout = new QVBoxLayout();
  m_inputsVLayout->setContentsMargins(0,0,0,0);
  m_inputsVLayout->setSpacing(10);

  m_mainVLayout->addLayout(m_inputsVLayout);

  m_mainVLayout->addStretch();

  if(applyMeasureNow){
    nameLineEdit->setReadOnly(true);
    descriptionTextEdit->setReadOnly(true);
    nameLineEdit->setDisabled(true);
    descriptionTextEdit->setDisabled(true);
  }
}

void EditRubyMeasureView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void EditRubyMeasureView::addInputView(QWidget * widget)
{
  m_inputsVLayout->addWidget(widget);
}

void EditRubyMeasureView::clear()
{
 QLayoutItem * child;

 while((child = m_inputsVLayout->takeAt(0)) != nullptr)
 {
   QWidget * widget = child->widget();

   OS_ASSERT(widget);

   delete widget;

   delete child;
 }
}

EditNullView::EditNullView(const QString & text) 
  : QWidget()
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(5,5,5,5);
  mainVLayout->setSpacing(5);
  setLayout(mainVLayout);
  
  auto label = new QLabel();
  label->setText(text);
  label->setWordWrap(true);
  label->setAlignment(Qt::AlignCenter);
  mainVLayout->addWidget(label);

  label->setStyleSheet("QLabel { font-size: 24px; font: bold; color: #6D6D6D }");
}

void EditNullView::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

DoubleInputView::DoubleInputView()
  : InputView()
{
  auto vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->setSpacing(5);
  setLayout(vLayout);

  nameLabel = new QLabel();
  nameLabel->setWordWrap(true);
  vLayout->addWidget(nameLabel);

  lineEdit = new QLineEdit();
  lineEdit->setValidator(new QDoubleValidator(lineEdit));
  vLayout->addWidget(lineEdit);
}

void DoubleInputView::setIncomplete(bool incomplete)
{
  if( incomplete )
  {
    nameLabel->setStyleSheet("QLabel { color: #DD0A05;}");
  }
  else
  {
    nameLabel->setStyleSheet("QLabel { color: black;}");
  }
}

void DoubleInputView::setDisplayValue(const QVariant & value)
{
  lineEdit->setText(value.toString());
}

ChoiceInputView::ChoiceInputView()
  : InputView()
{
  auto vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->setSpacing(5);
  setLayout(vLayout);

  nameLabel = new QLabel();
  nameLabel->setWordWrap(true);
  vLayout->addWidget(nameLabel);

  comboBox = new InputComboBox();
  vLayout->addWidget(comboBox);
}

void ChoiceInputView::setIncomplete(bool incomplete)
{
  if( incomplete )
  {
    nameLabel->setStyleSheet("QLabel { color: #DD0A05;}");
  }
  else
  {
    nameLabel->setStyleSheet("QLabel { color: black;}");
  }
}

void ChoiceInputView::setDisplayValue(const QVariant & value)
{
  comboBox->blockSignals(true);

  comboBox->setCurrentIndex(comboBox->findText(value.toString()));

  comboBox->blockSignals(false);
}

BoolInputView::BoolInputView()
  : InputView()
{
  auto vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->setSpacing(5);
  setLayout(vLayout);

  checkBox = new InputCheckBox();
  vLayout->addWidget(checkBox);
}

void BoolInputView::setIncomplete(bool incomplete)
{
  checkBox->setIncomplete(incomplete);
}

void BoolInputView::setDisplayValue(const QVariant & value)
{
  checkBox->setChecked(value.toBool());
}

IntegerInputView::IntegerInputView()
  : InputView()
{
  auto vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->setSpacing(5);
  setLayout(vLayout);

  nameLabel = new QLabel();
  nameLabel->setWordWrap(true);
  vLayout->addWidget(nameLabel);

  lineEdit = new QLineEdit();
  lineEdit->setValidator(new QIntValidator(lineEdit));
  vLayout->addWidget(lineEdit);
}

void IntegerInputView::setIncomplete(bool incomplete)
{
  if( incomplete )
  {
    nameLabel->setStyleSheet("QLabel { color: #DD0A05;}");
  }
  else
  {
    nameLabel->setStyleSheet("QLabel { color: black;}");
  }
}

void IntegerInputView::setDisplayValue(const QVariant & value)
{
  lineEdit->setText(value.toString());
}

StringInputView::StringInputView()
  : InputView()
{
  auto vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->setSpacing(5);
  setLayout(vLayout);

  nameLabel = new QLabel();
  nameLabel->setWordWrap(true);
  vLayout->addWidget(nameLabel);

  lineEdit = new QLineEdit();
  vLayout->addWidget(lineEdit);
}

void StringInputView::setIncomplete(bool incomplete)
{
  if( incomplete )
  {
    nameLabel->setStyleSheet("QLabel { color: #DD0A05;}");
  }
  else
  {
    nameLabel->setStyleSheet("QLabel { color: black;}");
  }
}

void StringInputView::setDisplayValue(const QVariant & value)
{
  lineEdit->setText(value.toString());
}

void InputComboBox::wheelEvent(QWheelEvent * e)
{
  e->ignore();
}

InputCheckBox::InputCheckBox()
  : QAbstractButton()
{
  setCheckable(true);

  auto mainHLayout = new QHBoxLayout();
  QStyleOptionButton opt;
  QRect checkRect = style()->subElementRect(QStyle::SE_CheckBoxIndicator,&opt);
  int spacing = 5;
  mainHLayout->setContentsMargins(checkRect.width() + spacing,0,0,0);
  mainHLayout->setSpacing(spacing);
  setLayout(mainHLayout);

  m_label = new QLabel("hello world");
  m_label->setWordWrap(true);
  mainHLayout->addWidget(m_label);

  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
}

InputCheckBox::~InputCheckBox()
{
}

void InputCheckBox::setText(const QString & text)
{
  m_label->setText(text);

  QAbstractButton::setText(text);
}

void InputCheckBox::paintEvent(QPaintEvent * event)
{
  QStyleOptionButton opt;
  opt.init(this);
  QPainter p(this);
  QPalette palette;

  if( isChecked() )
  {
    opt.state |= QStyle::State_On;
  }

  style()->drawControl(QStyle::CE_CheckBox,&opt,&p,this);
}

void InputCheckBox::setIncomplete(bool incomplete)
{
  if( incomplete )
  {
    m_label->setStyleSheet("QLabel { color: #DD0A05;}");
  }
  else
  {
    m_label->setStyleSheet("QLabel { color: black;}");
  }
}


} // openstudio

