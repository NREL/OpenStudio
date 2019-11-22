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

#include "EditController.hpp"
#include "EditView.hpp"
#include "OSViewSwitcher.hpp"
#include "WorkflowController.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Assert.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>

namespace openstudio{

EditController::EditController(bool applyMeasureNow)
  : QObject()
{
  editView = new OSViewSwitcher();
  if(applyMeasureNow){
    m_editNullView = new EditNullView("Select a Measure to Apply");
  } else {
    m_editNullView = new EditNullView();
  }
  editRubyMeasureView = new EditRubyMeasureView(applyMeasureNow);

  reset();
}

EditController::~EditController()
{
  if( editView ) { delete editView; }
  if( m_editNullView ) { delete m_editNullView; }
  if( editRubyMeasureView ) { delete editRubyMeasureView; }
}

void EditController::setMeasureStepItem(measuretab::MeasureStepItem * measureStepItem, BaseApp *t_app)
{
  m_measureStepItem = measureStepItem;

  editRubyMeasureView->clear();

  editView->setView(editRubyMeasureView);

  // Ruby Measure Name
  editRubyMeasureView->nameLineEdit->setText(m_measureStepItem->name());
  connect(editRubyMeasureView->nameLineEdit, &QLineEdit::textEdited, m_measureStepItem.data(), &measuretab::MeasureStepItem::setName);

  // Measure Directory, uneditable so no connect needed
  editRubyMeasureView->nameNonEditableLineEdit->setText(m_measureStepItem->measureDirectory());

  // Measure Description
  editRubyMeasureView->descriptionTextEdit->setText(m_measureStepItem->description());
  connect(editRubyMeasureView->descriptionTextEdit, &QTextEdit::textChanged, this, &EditController::updateDescription);

  // Measure Modeler Description
  editRubyMeasureView->modelerDescriptionTextEdit->setText(m_measureStepItem->modelerDescription());

  // Inputs

  std::vector<measure::OSArgument> arguments = m_measureStepItem->arguments();

  for( const auto & arg : arguments )
  {
    QSharedPointer<InputController> inputController = QSharedPointer<InputController>(new InputController(this,arg,t_app));

    m_inputControllers.push_back(inputController);

    editRubyMeasureView->addInputView(inputController->inputView);
  }
}

measuretab::MeasureStepItem * EditController::measureStepItem() const
{
  return m_measureStepItem;
}

void EditController::updateDescription()
{
  m_measureStepItem->setDescription(editRubyMeasureView->descriptionTextEdit->toPlainText());
}

void EditController::reset()
{
  // Evan note: It's bad to play with null pointers
  if (!m_editNullView || !editView || !m_measureStepItem || !editRubyMeasureView) {
    //return;
  }

  editView->setView(m_editNullView);

  m_inputControllers.clear();

  m_measureStepItem = nullptr;

  editRubyMeasureView->nameLineEdit->disconnect();
  // Note: no need to disconnect nameNonEditableLineEdit since it wasn't connected to begin with

  editRubyMeasureView->descriptionTextEdit->disconnect();
}

class EditMeasureMessageBox : public QMessageBox
{
  Q_OBJECT

  public:

  // Warning used when a message is about to be edited and design points will be removed;
  // Returns true if the user chooses to edit anyway.
  static bool warning(BaseApp *t_app)
  {
    int result = QMessageBox::warning(t_app->mainWidget(),
                 "Clear Results?",
                 "Editing this measure will clear all results and save your project. Do you want to proceed?",
                 QMessageBox::Ok,
                 QMessageBox::Cancel);

    return (result == QMessageBox::Ok);
  }
};

InputController::InputController(EditController * editController,const measure::OSArgument & argument, BaseApp *t_app)
  : QObject(),
    m_app(t_app),
    m_editController(editController),
    m_argument(argument)
{
  if( m_argument.type() == measure::OSArgumentType::Double )
  {
    auto doubleInputView = new DoubleInputView();

    doubleInputView->setName(m_argument.displayName(), m_argument.units(), m_argument.description());

    if( m_argument.hasValue() )
    {
      doubleInputView->lineEdit->setText(QString::fromStdString(m_argument.valueAsString()));
    }
    else if( m_argument.hasDefaultValue() )
    {
      doubleInputView->lineEdit->setText(QString::fromStdString(m_argument.defaultValueAsString()));
    }

    connect(doubleInputView->lineEdit, &QLineEdit::textEdited, this, static_cast<void (InputController::*)(const QString &)>(&InputController::setValue));

    inputView = doubleInputView;
  }
  else if( m_argument.type() == measure::OSArgumentType::Choice )
  {
    auto choiceInputView = new ChoiceInputView();

    choiceInputView->setName(m_argument.displayName(), m_argument.units(), m_argument.description());

    // Add all of the choices from the argument

    const std::vector<std::string> & choices = m_argument.choiceValueDisplayNames();
    const std::vector<std::string> & values = m_argument.choiceValues();

    int i = 0;
    for( const auto & choice : choices )
    {
      choiceInputView->comboBox->addItem(QString::fromStdString(choice),QString::fromStdString(values[i]));

      i++;
    }

    // Add an empty choice if there is no default or if the default is not one of the choices from the argument.

    if( ! m_argument.hasDefaultValue() )
    {
      choiceInputView->comboBox->insertItem(0,"");
    }
    else
    {
      int index = choiceInputView->comboBox->findData(QString::fromStdString(m_argument.defaultValueAsString()));

      if (index == -1){
        index = choiceInputView->comboBox->findText(QString::fromStdString(m_argument.defaultValueAsString()));
      }

      if (index == -1)
      {
        // DLM: this is an error
        choiceInputView->comboBox->insertItem(0, "");
      }
    }

    // Set the initial value
    if( m_argument.hasValue() )
    {
      int index = choiceInputView->comboBox->findData(QString::fromStdString(m_argument.valueAsString()));

      if (index == -1){
        index = choiceInputView->comboBox->findText(QString::fromStdString(m_argument.valueAsString()));
      }

      if( index != -1 )
      {
        choiceInputView->comboBox->setCurrentIndex(index);
      }
      else
      {
        // DLM: this is an error
        choiceInputView->comboBox->setCurrentIndex(0);
      }
    }
    else if( m_argument.hasDefaultValue() )
    {
      int index = choiceInputView->comboBox->findData(QString::fromStdString(m_argument.defaultValueAsString()));

      if (index == -1){
        index = choiceInputView->comboBox->findText(QString::fromStdString(m_argument.defaultValueAsString()));
      }

      if( index != -1 )
      {
        choiceInputView->comboBox->setCurrentIndex(index);
      }
      else
      {
        // DLM: this is an error
        choiceInputView->comboBox->setCurrentIndex(0);
      }
    }
    else
    {
      choiceInputView->comboBox->setCurrentIndex(0);
    }

    connect(choiceInputView->comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &InputController::setValueForIndex);

    inputView = choiceInputView;
  }
  else if( m_argument.type() == measure::OSArgumentType::Boolean )
  {
    auto boolInputView = new BoolInputView();

    boolInputView->setName(m_argument.displayName(), m_argument.units(), m_argument.description());

    if( m_argument.hasValue() )
    {
      boolInputView->checkBox->setChecked(m_argument.valueAsBool());
    }
    else if( m_argument.hasDefaultValue() )
    {
      boolInputView->checkBox->setChecked(m_argument.defaultValueAsBool());
    }
    else {
      boolInputView->checkBox->setChecked(false);
      m_argument.setValue(false);
    }

    connect(boolInputView->checkBox, &InputCheckBox::clicked, this, static_cast<void (InputController::*)(bool)>(&InputController::setValue));

    inputView = boolInputView;
  }
  else if( m_argument.type() == measure::OSArgumentType::Integer )
  {
    auto integerInputView = new IntegerInputView();

    integerInputView->setName(m_argument.displayName(), m_argument.units(), m_argument.description());

    if( m_argument.hasValue() )
    {
      integerInputView->lineEdit->setText(QString::fromStdString(m_argument.valueAsString()));
    }
    else if( m_argument.hasDefaultValue() )
    {
      integerInputView->lineEdit->setText(QString::fromStdString(m_argument.defaultValueAsString()));
    }

    connect(integerInputView->lineEdit, &QLineEdit::textEdited, this, static_cast<void (InputController::*)(const QString &)>(&InputController::setValue));

    inputView = integerInputView;
  }
  else if( m_argument.type() == measure::OSArgumentType::String )
  {
    auto stringInputView = new StringInputView();

    stringInputView->setName(m_argument.displayName(), m_argument.units(), m_argument.description());

    if( m_argument.hasValue() )
    {
      stringInputView->lineEdit->setText(QString::fromStdString(m_argument.valueAsString()));
    }
    else if( m_argument.hasDefaultValue() )
    {
      stringInputView->lineEdit->setText(QString::fromStdString(m_argument.defaultValueAsString()));
    }

    connect(stringInputView->lineEdit, &QLineEdit::textEdited, this, static_cast<void (InputController::*)(const QString &)>(&InputController::setValue));

    inputView = stringInputView;
  }
  else
  {
    inputView = new InputView();
  }

  inputView->setIncomplete(isArgumentIncomplete());
}

InputController::~InputController()
{
  if( inputView ) { delete inputView; }
}

void InputController::setValue(const QString & text)
{
  if( isItOKToClearResults() )
  {
    if( text.isEmpty() )
    {
      m_argument.clearValue();
    }
    else
    {
      m_argument.setValue(text.toStdString());
    }


    m_editController->measureStepItem()->setArgument(m_argument);

    inputView->setIncomplete(isArgumentIncomplete());
  }
}

void InputController::setValue(bool value)
{
  if( isItOKToClearResults() )
  {
    m_argument.setValue(value);

    m_editController->measureStepItem()->setArgument(m_argument);

    inputView->setIncomplete(isArgumentIncomplete());
  }
}

void InputController::setValueForIndex(int index)
{
  if( isItOKToClearResults() )
  {
    QString value = qobject_cast<ChoiceInputView *>(inputView)->comboBox->itemData(index).toString();

    if( value.isEmpty() )
    {
      m_argument.clearValue();
    }
    else
    {
      m_argument.setValue(value.toStdString());
    }

    m_editController->measureStepItem()->setArgument(m_argument);

    inputView->setIncomplete(isArgumentIncomplete());
  }
}

bool InputController::isArgumentIncomplete() const
{
  bool result = false;
  std::vector<measure::OSArgument> incompleteArguments = m_editController->measureStepItem()->incompleteArguments();
  for (const auto& incompleteArgument : incompleteArguments){
    if (incompleteArgument.name() == m_argument.name()){
      result = true;
    }
  }
  return result;
}

bool InputController::isItOKToClearResults()
{
  // DLM: todo warn user when results are out of date
  return true;
}

} // openstudio

