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

#include "EditController.hpp"
#include "EditView.hpp"
#include "OSViewSwitcher.hpp"

#include "../analysisdriver/SimpleProject.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"

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

void EditController::setMeasureItem(measuretab::MeasureItem * measureItem, BaseApp *t_app)
{
  m_measureItem = measureItem;

  editRubyMeasureView->clear();

  editView->setView(editRubyMeasureView);

  // Ruby Measure Name

  editRubyMeasureView->nameLineEdit->setText(m_measureItem->name());

  bool bingo = connect(editRubyMeasureView->nameLineEdit,SIGNAL(textEdited(const QString &)),m_measureItem.data(),SLOT(setName(const QString &)));

  OS_ASSERT(bingo);

  // Measure Description

  editRubyMeasureView->descriptionTextEdit->setText(m_measureItem->description());

  bingo = connect(editRubyMeasureView->descriptionTextEdit,SIGNAL(textChanged()),this,SLOT(updateDescription()));

  OS_ASSERT(bingo);

  // Measure Modeler Description

  editRubyMeasureView->modelerDescriptionLabel->setText(m_measureItem->modelerDescription());

  // Inputs

  std::vector<ruleset::OSArgument> arguments = m_measureItem->arguments();

  for( const auto & arg : arguments )
  {
    QSharedPointer<InputController> inputController = QSharedPointer<InputController>(new InputController(this,arg,t_app));

    m_inputControllers.push_back(inputController);

    editRubyMeasureView->addInputView(inputController->inputView);
  }
}

measuretab::MeasureItem * EditController::measureItem() const
{
  return m_measureItem;
}

void EditController::updateDescription()
{
  m_measureItem->setDescription(editRubyMeasureView->descriptionTextEdit->toPlainText());
}

void EditController::reset()
{
  editView->setView(m_editNullView);

  m_inputControllers.clear();

  m_measureItem = nullptr;

  editRubyMeasureView->nameLineEdit->disconnect();

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

InputController::InputController(EditController * editController,const ruleset::OSArgument & argument, BaseApp *t_app)
  : QObject(),
    m_app(t_app),
    m_editController(editController),
    m_argument(argument)
{
  if( m_argument.type() == ruleset::OSArgumentType::Double )
  {
    auto doubleInputView = new DoubleInputView();

    doubleInputView->nameLabel->setText(QString::fromStdString(m_argument.displayName()));

    if( m_argument.hasValue() )
    {
      doubleInputView->lineEdit->setText(QString::fromStdString(m_argument.valueAsString()));
    }
    else if( m_argument.hasDefaultValue() )
    {
      doubleInputView->lineEdit->setText(QString::fromStdString(m_argument.defaultValueAsString()));
    }

    bool bingo = connect(doubleInputView->lineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(setValue(const QString &)));

    OS_ASSERT(bingo);

    inputView = doubleInputView;
  }
  else if( m_argument.type() == ruleset::OSArgumentType::Choice )
  {
    auto choiceInputView = new ChoiceInputView();

    choiceInputView->nameLabel->setText(QString::fromStdString(m_argument.displayName()));

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
    else if( choiceInputView->comboBox->findText(QString::fromStdString(m_argument.defaultValueAsString())) == -1 )
    {
      choiceInputView->comboBox->insertItem(0,"");
    }

    // Set the initial value
    if( m_argument.hasValue() )
    {
      int index = choiceInputView->comboBox->findData(QString::fromStdString(m_argument.valueAsString()));

      choiceInputView->comboBox->setCurrentIndex(index);
    }
    else if( m_argument.hasDefaultValue() )
    {
      int index = choiceInputView->comboBox->findData(QString::fromStdString(m_argument.defaultValueAsString()));

      if( index != -1 )
      {
        choiceInputView->comboBox->setCurrentIndex(index);
      }
      else
      {
        choiceInputView->comboBox->setCurrentIndex(0);
      }
    }
    else
    {
      choiceInputView->comboBox->setCurrentIndex(0);
    }

    bool bingo = connect(choiceInputView->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setValueForIndex(int)));

    OS_ASSERT(bingo);

    inputView = choiceInputView;
  }
  else if( m_argument.type() == ruleset::OSArgumentType::Boolean )
  {
    auto boolInputView = new BoolInputView();

    boolInputView->checkBox->setText(QString::fromStdString(m_argument.displayName()));

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

    bool bingo = connect(boolInputView->checkBox,SIGNAL(clicked(bool)),this,SLOT(setValue(bool)));

    OS_ASSERT(bingo);

    inputView = boolInputView;
  }
  else if( m_argument.type() == ruleset::OSArgumentType::Integer )
  {
    auto integerInputView = new IntegerInputView();

    integerInputView->nameLabel->setText(QString::fromStdString(m_argument.displayName()));

    if( m_argument.hasValue() )
    {
      integerInputView->lineEdit->setText(QString::fromStdString(m_argument.valueAsString()));
    }
    else if( m_argument.hasDefaultValue() )
    {
      integerInputView->lineEdit->setText(QString::fromStdString(m_argument.defaultValueAsString()));
    }

    bool bingo = connect(integerInputView->lineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(setValue(const QString &)));

    OS_ASSERT(bingo);

    inputView = integerInputView;
  }
  else if( m_argument.type() == ruleset::OSArgumentType::String )
  {
    auto stringInputView = new StringInputView();

    stringInputView->nameLabel->setText(QString::fromStdString(m_argument.displayName()));

    if( m_argument.hasValue() )
    {
      stringInputView->lineEdit->setText(QString::fromStdString(m_argument.valueAsString()));
    }
    else if( m_argument.hasDefaultValue() )
    {
      stringInputView->lineEdit->setText(QString::fromStdString(m_argument.defaultValueAsString()));
    }

    bool bingo = connect(stringInputView->lineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(setValue(const QString &)));

    OS_ASSERT(bingo);

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

    m_editController->measureItem()->setArgument(m_argument);

    inputView->setIncomplete(isArgumentIncomplete());
  }
}

void InputController::setValue(bool value)
{
  if( isItOKToClearResults() )
  {
    m_argument.setValue(value);

    m_editController->measureItem()->setArgument(m_argument);

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

    m_editController->measureItem()->setArgument(m_argument);

    inputView->setIncomplete(isArgumentIncomplete());
  }
}

bool InputController::isArgumentIncomplete() const
{
  // Get the argument from the ruby perturbation by the same name as m_argument

  std::vector<ruleset::OSArgument> argumentVector = m_editController->measureItem()->measure().arguments();

  std::map<std::string,ruleset::OSArgument> argumentMap = convertOSArgumentVectorToMap(argumentVector);

  auto it = argumentMap.find(m_argument.name());

  if(it != argumentMap.end())
  {
    // Check the criteria for being incomplete

    if (it->second.required() && !(it->second.hasValue() || it->second.hasDefaultValue()))
    {
      return true;
    }
  }

  return false;
}

bool InputController::isItOKToClearResults()
{
  bool doOperation(true);

  // Find out if there are results
  boost::optional<analysisdriver::SimpleProject> project = m_app->project();

  if( ! project ) { return false; }

  if( ! project->analysis().completeDataPoints().empty() )
  {
    // Ask user if they are sure they want to edit
    doOperation = EditMeasureMessageBox::warning(m_app);
  }

  if( ! doOperation )
  {
    // Reset user interface to argument's value

    if( m_argument.hasValue() )
    {
      QString argumentValue = QString::fromStdString(m_argument.valueAsString());

      inputView->setDisplayValue(argumentValue);
    }
    else if( m_argument.hasDefaultValue() )
    {
      inputView->setDisplayValue(QString::fromStdString(m_argument.defaultValueAsString()));
    }
    else
    {
      inputView->setDisplayValue("");
    }

    return false;
  }

  return true;
}

} // openstudio

