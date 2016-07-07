/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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


#include "MeasureItem.hpp"
#include "MeasureItemView.hpp"
#include "MeasureDragData.hpp"
#include "MeasureManager.hpp"
#include "EditController.hpp"
#include "BaseApp.hpp"
#include "LocalLibraryController.hpp"
#include "WorkflowTools.hpp"

#include "../energyplus/ForwardTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/RubyException.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <QByteArray>
#include <QDialog>
#include <QDropEvent>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QRadioButton>


namespace openstudio{


namespace measuretab {

MeasureItem::MeasureItem(const MeasureStep & measureStep, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_measureStep(measureStep)
{
  //OS_ASSERT(m_measure.usesBCLMeasure());
}

MeasureStep MeasureItem::measureStep() const
{
  return m_measureStep;
}
/*
OptionalBCLMeasure MeasureItem::bclMeasure() const
{
  return m_measure.bclMeasure();
}
*/
/*
bool MeasureItem::isAlternativeModelMeasure() const
{
  if (boost::optional<analysisdriver::SimpleProject> project = m_app->project()){

    analysis::OptionalMeasureGroup modelSwapVariable = project->getAlternativeModelVariable();

    if (modelSwapVariable) {
      std::vector<analysis::Measure> measures = modelSwapVariable->measures(false);
      for (const auto measure : measures){
        if (measure == m_measure){
          return true;
        }
      }
    }
  }

  return false;
}
*/
QString MeasureItem::name() const
{
  QString result;
  boost::optional<std::string> s = m_measureStep.name();
  if (s){
    result = QString::fromStdString(*s);
  }
  return result;
}

void MeasureItem::setName(const QString & name)
{
  m_measureStep.setName(name.toStdString());

  // TODO: uncomment
  //emit nameChanged(name);
}

QString MeasureItem::description() const
{
  QString result;
  boost::optional<std::string> s = m_measureStep.description();
  if (s){
    result = QString::fromStdString(*s);
  }
  return result;
}

QString MeasureItem::modelerDescription() const
{
  QString result;
  boost::optional<std::string> s = m_measureStep.modelerDescription();
  if (s){
    result = QString::fromStdString(*s);
  }
  return result;
}

QString MeasureItem::scriptFileName() const
{
  QString scriptName;
  // DLM: todo
  /*
  boost::optional<BCLMeasure> bclMeasure = m_measure.bclMeasure();
  if (bclMeasure){
    if( boost::optional<openstudio::path> path = m_measure.measure()->primaryRubyScriptPath() )
    {
      scriptName = toQString(path->leaf());
    }
  }else{
    LOG(Error, "Cannot load BCLMeasure '" << toString(m_measure.bclMeasureUUID()) << "' from '" << toString(m_measure.bclMeasureDirectory()) << "'");
  }
  */
  return scriptName;
}

void MeasureItem::setDescription(const QString & description)
{
  // ETH: Was setting description on the measure itself (m_measure.bclMeasure()), however, this
  // description should be attached to the instantiated measure, not the global measure.
  m_measureStep.setDescription(description.toStdString());

  emit descriptionChanged();
}
/*
std::vector<ruleset::OSArgument> MeasureItem::arguments() const
{
  return m_measure.arguments();
}
*/
/*
bool MeasureItem::hasIncompleteArguments() const
{
  return m_measure.hasIncompleteArguments();
}
*/
/*
void MeasureItem::setArgument(const measure::OSArgument& argument)
{
  m_measureStep.setArgument(argument);

  emit argumentsChanged(hasIncompleteArguments());
}
*/
/*
void MeasureItem::duplicate()
{
  // check if we have data points other than the baseline
  if (boost::optional<analysisdriver::SimpleProject> project = m_app->project()){
    if (project->analysis().dataPoints().size() > 1){
      // warn user that this will blow away their data points
      QMessageBox::StandardButton test = QMessageBox::question( 
        m_app->mainWidget(), 
        "Duplicate Measure?", 
        "Duplicating a measure will remove existing design alternatives and save your project, do you want to proceed?", 
        QMessageBox::Yes |  QMessageBox::No, 
        QMessageBox::No );
      if (test == QMessageBox::No){
        return;
      }
    }
  }

  m_app->editController()->reset();

  qobject_cast<MeasureListController *>(controller())->addItemForDuplicateMeasure(m_measure);
}
*/
/*
void MeasureItem::remove()
{
  // check if we have data points other than the baseline
  if (boost::optional<analysisdriver::SimpleProject> project = m_app->project()){
    if (project->analysis().dataPoints().size() > 1){
      // warn user that this will blow away their data points
      QMessageBox::StandardButton test = QMessageBox::question( 
        m_app->mainWidget(), 
        "Remove Measure?", 
        "Removing a measure will remove existing design alternatives and save your project, do you want to proceed?", 
        QMessageBox::Yes |  QMessageBox::No, 
        QMessageBox::No );
      if (test == QMessageBox::No){
        return;
      }
    }
  }

  // if this measure is being edited, clear the edit controller
  if (m_app->editController()->measureItem() == this){
    m_app->editController()->reset();
  }

  qobject_cast<MeasureListController *>(controller())->removeItemForMeasure(m_measure);
}
*/
void MeasureItem::setSelected(bool isSelected)
{
  OSListItem::setSelected(isSelected);

  // Ugly hack to prevent the controller from doing this for tab 2.
  // Please somebody fix, perhaps be using a new signal from OSItemSelectionController.
  if( ! controller()->selectionController()->allowMultipleSelections() )
  {
    if( isSelected )
    {
      // DLM: todo
      /*
      if( boost::optional<analysis::RubyMeasure> rubyPert = m_measure.optionalCast<analysis::RubyMeasure>() )
      {
        m_app->chooseHorizontalEditTab();

        m_app->editController()->setMeasureItem(this, m_app);
      }
      */
    }
    else
    {
      m_app->editController()->reset();
    }
  }

  //TODO
}

MeasureItemDelegate::MeasureItemDelegate(bool fixed)
  : m_fixed(fixed)
{}

QWidget * MeasureItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<MeasureItem> measureItem = dataSource.objectCast<MeasureItem>())
  {
    auto measureItemView = new MeasureItemView(m_fixed);

    // Name

    measureItemView->measureItemButton->nameLabel->setText(measureItem->name());

    connect(measureItem.data(), &MeasureItem::displayNameChanged, measureItemView->measureItemButton->nameLabel, &QLabel::setText);

    // Duplicate

    // DLM: todo
    //connect(measureItemView->duplicateButton, &QPushButton::clicked, measureItem.data(), &MeasureItem::duplicate);

    // Remove

    // DLM: todo
    //connect(measureItemView->removeButton, &QPushButton::clicked, measureItem.data(), &MeasureItem::remove);

    // Selection

    measureItemView->measureItemButton->setHasEmphasis(measureItem->isSelected());

    connect(measureItemView->measureItemButton, &MeasureItemButton::clicked, measureItem.data(), &MeasureItem::toggleSelected);

    connect(measureItem.data(), &MeasureItem::selectedChanged, measureItemView->measureItemButton, &MeasureItemButton::setHasEmphasis);

    // Warning Icon

    // DLM: todo
    //measureItemView->measureItemButton->cautionLabel->setVisible(measureItem->hasIncompleteArguments());
    //connect(measureItem.data(), &MeasureItem::argumentsChanged, measureItemView->measureItemButton->cautionLabel, &QLabel::setVisible);

    return measureItemView;
  }

  return new QWidget();
}

} // measuretab


} // openstudio


