/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

  // Editable Name, maps to 'name'
  QLabel * measureOptionTitleLabel = new QLabel("Name");
  measureOptionTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(measureOptionTitleLabel);

  QRegExp nameRegex("^\\S.*");
  auto validator = new QRegExpValidator(nameRegex, this);

  nameLineEdit = new QLineEdit();
  nameLineEdit->setValidator(validator);
  m_mainVLayout->addWidget(nameLineEdit);

  // Non Editable name, maps to 'measure_dir_name'
  QLabel * measureOptionTitleLabel2 = new QLabel("Measure Directory Name");
  measureOptionTitleLabel2->setObjectName("H2");
  m_mainVLayout->addWidget(measureOptionTitleLabel2);

  nameNonEditableLineEdit = new QLineEdit();
  m_mainVLayout->addWidget(nameNonEditableLineEdit);
  nameNonEditableLineEdit->setStyleSheet("background: #E6E6E6;");
  nameNonEditableLineEdit->setReadOnly(true);

  QLabel * descriptionTitleLabel = new QLabel("Description");
  descriptionTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(descriptionTitleLabel);

  // Description
  descriptionTextEdit = new QTextEdit();
  descriptionTextEdit->setFixedHeight(70);
  descriptionTextEdit->setAcceptRichText(false);
  descriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  descriptionTextEdit->setTabChangesFocus(true);
  m_mainVLayout->addWidget(descriptionTextEdit);

  // Modeler Description
  QLabel * modelerDescriptionTitleLabel = new QLabel("Modeler Description");
  modelerDescriptionTitleLabel->setObjectName("H2");
  m_mainVLayout->addWidget(modelerDescriptionTitleLabel);

  modelerDescriptionTextEdit = new QTextEdit();
  modelerDescriptionTextEdit->setStyleSheet("background: #E6E6E6;");
  modelerDescriptionTextEdit->setFixedHeight(70);
  modelerDescriptionTextEdit->setAcceptRichText(false);
  modelerDescriptionTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  modelerDescriptionTextEdit->setTabChangesFocus(true);
  modelerDescriptionTextEdit->setReadOnly(true);
  m_mainVLayout->addWidget(modelerDescriptionTextEdit);

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
    nameNonEditableLineEdit->setDisabled(true);
    modelerDescriptionTextEdit->setDisabled(true);
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
  nameLabel->setTextFormat(Qt::RichText);
  nameLabel->setWordWrap(true);
  vLayout->addWidget(nameLabel);

  lineEdit = new QLineEdit();
  lineEdit->setValidator(new QDoubleValidator(lineEdit));
  vLayout->addWidget(lineEdit);
}

void DoubleInputView::setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description)
{
  QString text;
  text += QString::fromStdString(name);
  if (units){
    text += QString::fromStdString(" (" + units.get() + ")");
  }
  if (description){
    text += QString::fromStdString("<div style=\"font-size:small;margin-top:2px;\">" + description.get() + "</div>");
  }

  nameLabel->setText(text);
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

void ChoiceInputView::setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description)
{
  QString text;
  text += QString::fromStdString(name);
  if (units){
    text += QString::fromStdString(" (" + units.get() + ")");
  }
  if (description){
    text += QString::fromStdString("<div style=\"font-size:small;margin-top:2px;\">" + description.get() + "</div>");
  }

  nameLabel->setText(text);
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

void BoolInputView::setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description)
{
  QString text;
  text += QString::fromStdString(name);
  if (units){
    text += QString::fromStdString(" (" + units.get() + ")");
  }
  if (description){
    text += QString::fromStdString("<div style=\"font-size:small;margin-top:2px;\">" + description.get() + "</div>");
  }

  checkBox->setText(text);
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

void IntegerInputView::setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description)
{
  QString text;
  text += QString::fromStdString(name);
  if (units){
    text += QString::fromStdString(" (" + units.get() + ")");
  }
  if (description){
    text += QString::fromStdString("<div style=\"font-size:small;margin-top:2px;\">" + description.get() + "</div>");
  }

  nameLabel->setText(text);
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

void StringInputView::setName(const std::string& name, const boost::optional<std::string>& units, const boost::optional<std::string>& description)
{
  QString text;
  text += QString::fromStdString(name);
  if (units){
    text += QString::fromStdString(" (" + units.get() + ")");
  }
  if (description){
    text += QString::fromStdString("<div style=\"font-size:small;margin-top:2px;\">" + description.get() + "</div>");
  }

  nameLabel->setText(text);
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

  m_label = new QLabel();
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

