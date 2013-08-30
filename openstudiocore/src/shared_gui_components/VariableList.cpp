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


#include "VariableList.hpp"
#include "VariableView.hpp"
#include "MeasureDragData.hpp"
#include "MeasureManager.hpp"
#include "EditController.hpp"
#include "BaseApp.hpp"
#include "LocalLibraryController.hpp"

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/MeasureGroup_Impl.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/NullMeasure_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/RubyMeasure.hpp>
#include <analysis/RubyMeasure_Impl.hpp>
#include <analysis/WorkflowStep.hpp>
#include <analysisdriver/SimpleProject.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/RubyException.hpp>

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

VariableGroupListController::VariableGroupListController(bool filterFixed, BaseApp *t_app)
  : OSListController()
{
  QSharedPointer<VariableGroupItem> variableGroupItem;
  std::vector<MeasureType> measureTypes;

  measureTypes.clear();
  measureTypes.push_back(MeasureType::ModelMeasure);
  variableGroupItem = QSharedPointer<VariableGroupItem>(new VariableGroupItem(measureTypes, "OpenStudio Measures", filterFixed, t_app));
  addItem(variableGroupItem);

  measureTypes.clear();
  measureTypes.push_back(MeasureType::EnergyPlusMeasure);
  variableGroupItem = QSharedPointer<VariableGroupItem>(new VariableGroupItem(measureTypes, "EnergyPlus Measures", filterFixed, t_app));
  addItem(variableGroupItem);

  measureTypes.clear();
  measureTypes.push_back(MeasureType::ModelMeasure);
  measureTypes.push_back(MeasureType::EnergyPlusMeasure);
  variableGroupItem = QSharedPointer<VariableGroupItem>(new VariableGroupItem(measureTypes, "Reporting Measures", filterFixed, t_app));
  addItem(variableGroupItem);
}

void VariableGroupListController::addItem(QSharedPointer<OSListItem> item)
{
  if( QSharedPointer<VariableGroupItem> variableGroupItem = item.dynamicCast<VariableGroupItem>() )
  {
    m_variableGroupItems.push_back(variableGroupItem);
    variableGroupItem->setController(this);
    variableGroupItem->variableListController()->setSelectionController(selectionController());
  }
}

QSharedPointer<OSListItem> VariableGroupListController::itemAt(int i)
{
  if( i >= 0 && i < count() )
  {
    return m_variableGroupItems[i];
  }

  return QSharedPointer<OSListItem>();
}

int VariableGroupListController::count()
{
  return m_variableGroupItems.size();
}

VariableGroupItem::VariableGroupItem(std::vector<MeasureType> measureTypes, const QString & label, bool filterFixed, BaseApp *t_baseApp)
  : OSListItem(),
    m_label(label)
{
  m_variableListController = QSharedPointer<VariableListController>(new VariableListController(measureTypes, filterFixed, t_baseApp));
}

QString VariableGroupItem::label() const
{
  return m_label;
}

QSharedPointer<VariableListController> VariableGroupItem::variableListController() const
{
  return m_variableListController;
}

VariableGroupItemDelegate::VariableGroupItemDelegate(bool t_fixedMeasuresOnly)
  : m_fixedMeasuresOnly(t_fixedMeasuresOnly)
{
}

QWidget * VariableGroupItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<VariableGroupItem> variableGroupItem = dataSource.objectCast<VariableGroupItem>() )
  {
    QSharedPointer<VariableListController> variableListController = variableGroupItem->variableListController();
    QSharedPointer<VariableItemDelegate> variableItemDelegate = QSharedPointer<VariableItemDelegate>(new VariableItemDelegate());

    QStringList acceptedMimeTypes;
    Q_FOREACH(MeasureType measureType, variableListController->measureTypes()){
      acceptedMimeTypes << MeasureDragData::mimeType(measureType);
    }

    // for right now only model and energyplus measures can have measure groups, reporting measures cannot
    bool fixedMeasuresOnly = (m_fixedMeasuresOnly || (acceptedMimeTypes.size() > 1));
    VariableGroupItemView * variableGroupItemView = new VariableGroupItemView(fixedMeasuresOnly);

    if (!fixedMeasuresOnly)
    {
      variableGroupItemView->variableGroupContentView->newGroupView->setVisible(true);
      variableGroupItemView->variableGroupContentView->newGroupView->dropZone->setAcceptedMimeTypes(acceptedMimeTypes);

      bool bingo = connect(variableGroupItemView->variableGroupContentView->newGroupView->dropZone,SIGNAL(dataDropped(QDropEvent * )),
          variableListController.data(),SLOT(addItemForDroppedMeasure(QDropEvent *)));

      OS_ASSERT(bingo);
    }

    variableGroupItemView->variableGroupContentView->newFixedGroupView->dropZone->setAcceptedMimeTypes(acceptedMimeTypes);

    bool bingo = connect(variableGroupItemView->variableGroupContentView->newFixedGroupView->dropZone,SIGNAL(dataDropped(QDropEvent * )),
                         variableListController.data(),SLOT(addFixedItemForDroppedMeasure(QDropEvent *)));
    OS_ASSERT(bingo);

    variableGroupItemView->variableGroupContentView->variableListView->setListController(variableListController);
    variableGroupItemView->variableGroupContentView->variableListView->setDelegate(variableItemDelegate);

    variableGroupItemView->variableGroupHeader->label->setText(variableGroupItem->label());

    return variableGroupItemView;
  }

  return new QWidget();
}

VariableListController::VariableListController(std::vector<MeasureType> measureTypes, bool filterFixed, openstudio::BaseApp *t_app)
  : OSListController(),
    m_app(t_app),
    m_measureTypes(measureTypes),
    m_filterFixed(filterFixed)
{
}


std::vector<MeasureType> VariableListController::measureTypes() const
{
  return m_measureTypes;
}


QSharedPointer<OSListItem> VariableListController::itemAt(int i)
{
  std::vector<analysis::MeasureGroup> vars = variables();

  if( i >= 0 && i < (int)vars.size() )
  {
    analysis::MeasureGroup var = vars[i];

    OS_ASSERT(!m_measureTypes.empty());
    QSharedPointer<VariableItem> item = QSharedPointer<VariableItem>(new VariableItem(var, m_measureTypes[0], m_app));

    item->setController(this);

    item->measureListController()->setSelectionController(selectionController());

    return item;
  }

  return QSharedPointer<VariableItem>();
}

int VariableListController::count()
{
  return variables().size();
}

std::vector<analysis::MeasureGroup> VariableListController::variables() const
{
  std::vector<analysis::MeasureGroup> result;

  boost::optional<analysisdriver::SimpleProject> project = m_app->project();

  if( project ) {

    analysis::Problem problem = project->analysis().problem();
    analysis::WorkflowStepVector workflow = problem.workflow();

    std::vector<MeasureType> measureTypes = this->measureTypes();

    if ((measureTypes.size() == 1) && (measureTypes[0] == MeasureType::ModelMeasure)) {
      analysis::OptionalMeasureGroup modelSwapVariable = project->getAlternativeModelVariable();
      OptionalInt stopIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
      OS_ASSERT(stopIndex);
      for (int i = 0; i < *stopIndex; ++i) {
        if (workflow[i].isInputVariable()) {
          analysis::InputVariable var = workflow[i].inputVariable();
          if (analysis::OptionalMeasureGroup dvar = var.optionalCast<analysis::MeasureGroup>()) {
            if (modelSwapVariable && (*dvar == *modelSwapVariable)) {
              continue;
            }
            std::vector<analysis::Measure> measures = dvar->measures(false);
            if (!m_filterFixed || measures.size() > 1)
            {
              // don't add the fixed measures here, they are always selected
              std::vector<analysis::RubyMeasure> rubyPerts = subsetCastVector<analysis::RubyMeasure>(measures);
              if (!rubyPerts.empty()) {
                result.push_back(*dvar);
              }
            }
          }
        }
      }

    } else if ((measureTypes.size() == 1) && (measureTypes[0] == MeasureType::EnergyPlusMeasure)){

      OptionalInt startIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ExpandObjects);
      OptionalInt stopIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlusPreProcess);
      OS_ASSERT(startIndex && stopIndex);
      for (int i = (*startIndex + 1); i < *stopIndex; ++i) {
        if (workflow[i].isInputVariable()) {
          analysis::InputVariable var = workflow[i].inputVariable();
          if (analysis::OptionalMeasureGroup dvar = var.optionalCast<analysis::MeasureGroup>()) {
            std::vector<analysis::Measure> measures = dvar->measures(false);
            if (!m_filterFixed || measures.size() > 1)
            {
              // don't add the fixed measures here, they are always selected
              std::vector<analysis::RubyMeasure> rubyPerts = subsetCastVector<analysis::RubyMeasure>(measures);
              if (!rubyPerts.empty()) {
                result.push_back(*dvar);
              }
            }
          }
        }
      }

    }else{
      OS_ASSERT(measureTypes.size() == 2);

      OptionalInt startIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlus);
      OptionalInt stopIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::OpenStudioPostProcess);
      OS_ASSERT(startIndex && stopIndex);
      for (int i = (*startIndex + 1); i < *stopIndex; ++i) {
        if (workflow[i].isInputVariable()) {
          analysis::InputVariable var = workflow[i].inputVariable();
          if (analysis::OptionalMeasureGroup dvar = var.optionalCast<analysis::MeasureGroup>()) {
            std::vector<analysis::Measure> measures = dvar->measures(false);
            if (!m_filterFixed || measures.size() > 1)
            {
              // don't add the fixed measures here, they are always selected
              std::vector<analysis::RubyMeasure> rubyPerts = subsetCastVector<analysis::RubyMeasure>(measures);
              if (!rubyPerts.empty()) {
                result.push_back(*dvar);
              }
            }
          }
        }
      }
    }
  }

  return result;
}

void VariableListController::removeItemForVariable(analysis::MeasureGroup variable)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    std::vector<analysis::MeasureGroup> vars = variables();

    int i = 0;
    bool bingo = false;

    for( std::vector<analysis::MeasureGroup>::const_iterator it = vars.begin();
         it != vars.end();
         it++ )
    {
      if( variable == *it )
      {
        bingo = true;

        break;
      }

      i++;
    }


    if( bingo )
    {
      analysis::Problem problem = project->analysis().problem();

      problem.erase(variable);

      emit itemRemoved(i);
    }
  }
}

void VariableListController::addItemForDroppedMeasure(QDropEvent *event)
{
  addItemForDroppedMeasureImpl(event, false);
}

void VariableListController::addFixedItemForDroppedMeasure(QDropEvent *event)
{
  addItemForDroppedMeasureImpl(event, true);
}



void VariableListController::addItemForDroppedMeasureImpl(QDropEvent * event, bool t_fixed)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    // Get the measure

    const QMimeData * mimeData = event->mimeData();

    QByteArray byteArray;
    Q_FOREACH(MeasureType measureType, m_measureTypes){
      byteArray = mimeData->data(MeasureDragData::mimeType(measureType));
      if (byteArray.size() > 0){
        break;
      }
    }

    MeasureDragData measureDragData(byteArray);

    UUID id = measureDragData.id();

    // accept the event to make the icon refresh
    event->accept();

    // check if we have data points other than the baseline
    if (project->analysis().dataPoints().size() > 1){
      // warn user that this will blow away their data points
      QMessageBox::StandardButton test = QMessageBox::question( m_app->mainWidget(), 
          "Add New Measure Group?", 
          "Adding a new measure group will remove existing design alternatives and save your project, do you want to proceed?", 
          QMessageBox::Yes |  QMessageBox::No, 
          QMessageBox::No );
      if (test == QMessageBox::No){
        return;
      }
    }

    try {
      // Make a new variable
      BCLMeasure projectMeasure = m_app->measureManager().insertReplaceMeasure(*project, id);

      // prep discrete variable
      std::string name = m_app->measureManager().suggestMeasureGroupName(projectMeasure);
      analysis::MeasureGroup dv(name, analysis::MeasureVector());
      dv.setDisplayName(name);
   
      // measure
      analysis::RubyMeasure measure(projectMeasure);
      try{
        measure.setArguments(m_app->measureManager().getArguments(*project, projectMeasure));
      } catch ( const RubyException&e ) {
        LOG(Error, "Failed to compute arguments for measure: " << e.what());
        QString errorMessage("Failed to compute arguments for measure: \n\n");
        errorMessage += QString::fromStdString(e.what());
        QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
        return;
      }

      if (!t_fixed)
      {
        // null measure
        analysis::NullMeasure nullPert;
        dv.push(nullPert);
      }

      // the new measure
      name = m_app->measureManager().suggestMeasureName(projectMeasure, t_fixed);
      measure.setName(name);
      measure.setDisplayName(name);
      measure.setDescription(projectMeasure.description());
      dv.push(measure);

      std::vector<MeasureType> measureTypes = this->measureTypes();

      // try to add to problem. can fail if measure is of wrong type.
      analysis::Problem problem = project->analysis().problem();
      OptionalInt index;
      FileReferenceType inType = projectMeasure.inputFileType();
      if (inType == FileReferenceType::OSM) {
        if ((measureTypes.size() == 1) && (measureTypes[0] == MeasureType::ModelMeasure)){
          index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
        }else if (measureTypes.size() == 2){
          index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::OpenStudioPostProcess);
        }
      }
      else {
        OS_ASSERT(inType == FileReferenceType::IDF);
        if ((measureTypes.size() == 1) && (measureTypes[0] == MeasureType::EnergyPlusMeasure)){
          index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlusPreProcess);
        }else if (measureTypes.size() == 2){
          index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::OpenStudioPostProcess);
        }
      }
      OS_ASSERT(index);
      bool ok = problem.insert(*index,dv);
      if (ok) {
        emit itemInserted(variables().size() - 1);
      }else{
        LOG(Error, "Failed to add measure at this workflow location.");
        QString errorMessage("Failed to add measure at this workflow location.");
        QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
      }
    } catch (const std::exception &) {
      // the user canceled the update of the measure
      LOG(Debug, "User canceled update of measure.");
      return;
    }
  }
}

void VariableListController::moveUp(analysis::MeasureGroup variable)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    // HERE - Logic seems questionable. Also check dirty flags.
    std::vector<analysis::MeasureGroup> vars = variables();

    int i = 0;
    bool bingo = false;

    for( std::vector<analysis::MeasureGroup>::const_iterator it = vars.begin();
         it != vars.end();
         it++ )
    {
      if( variable == *it )
      {
        bingo = true;

        break;
      }

      i++;
    }


    if( bingo && i > 0 )
    {
      analysis::Problem problem = project->analysis().problem();

      problem.swap(variable,vars[i - 1]);

      emit itemChanged(i - 1);
      emit itemChanged(i);
    }
  }
}

void VariableListController::moveDown(analysis::MeasureGroup variable)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    std::vector<analysis::MeasureGroup> vars = variables();

    int i = 0;
    bool bingo = false;

    for( std::vector<analysis::MeasureGroup>::const_iterator it = vars.begin();
         it != vars.end();
         it++ )
    {
      if( variable == *it )
      {
        bingo = true;

        break;
      }

      i++;
    }


    if( bingo && i < (int)vars.size() - 1 )
    {
      analysis::Problem problem = project->analysis().problem();

      problem.swap(vars[i + 1],variable);

      emit itemChanged(i);
      emit itemChanged(i + 1);
    }
  }
}

VariableItem::VariableItem(const analysis::MeasureGroup & variable, MeasureType measureType, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_variable(variable),
    m_measureType(measureType)
{
  m_measureListController = QSharedPointer<MeasureListController>(new MeasureListController(this, t_app));
}

QString VariableItem::name() const
{
  return QString::fromStdString(m_variable.displayName());
}

void VariableItem::setName(const QString & name)
{
  m_variable.setName(name.toStdString());
  m_variable.setDisplayName(name.toStdString());
}

bool VariableItem::isFixedMeasure()
{
  // logic borrowed from Problem.cpp:numStaticTransformations
  return m_variable.numMeasures(true) < 2;
}

void VariableItem::remove()
{
  // check if we have data points other than the baseline
  if (boost::optional<analysisdriver::SimpleProject> project = m_app->project()){
    if (project->analysis().dataPoints().size() > 1){
      // warn user that this will blow away their data points
      QMessageBox::StandardButton test = QMessageBox::question( 
          m_app->mainWidget(), 
          "Remove Measure Group?", 
          "Removing a measure group will remove existing design alternatives and save your project, do you want to proceed?", 
          QMessageBox::Yes |  QMessageBox::No, 
          QMessageBox::No );
      if (test == QMessageBox::No){
        return;
      }
    }
  }

  qobject_cast<VariableListController *>(controller())->removeItemForVariable(m_variable);
}

void VariableItem::moveUp()
{
  qobject_cast<VariableListController *>(controller())->moveUp(m_variable);
}

void VariableItem::moveDown()
{
  qobject_cast<VariableListController *>(controller())->moveDown(m_variable);
}

QWidget * VariableItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<VariableItem> variableItem = dataSource.objectCast<VariableItem>())
  {
    VariableItemView * variableItemView = new VariableItemView(variableItem->isFixedMeasure());
    variableItemView->variableHeaderView->variableNameEdit->setText(variableItem->name());

    bool bingo = connect(variableItemView->variableHeaderView->variableNameEdit,SIGNAL(textEdited(const QString &)),
                         variableItem.data(),SLOT(setName(const QString &)));
    OS_ASSERT(bingo);

    QSharedPointer<MeasureListController> measureListController = variableItem->measureListController();
    variableItemView->variableContentView->measureListView->setListController(measureListController);

    QStringList acceptedMimeTypes;
    acceptedMimeTypes << MeasureDragData::mimeType(variableItem->measureType());
      
    if (!variableItem->isFixedMeasure())
    {
      variableItemView->variableContentView->dropZone->setAcceptedMimeTypes(acceptedMimeTypes);
      bingo = connect(variableItemView->variableContentView->dropZone,SIGNAL(dataDropped(QDropEvent *)),
          variableItem->measureListController().data(),SLOT(addItemForDroppedMeasure(QDropEvent *)));
      OS_ASSERT(bingo);
    }

    bingo = connect(variableItemView->variableHeaderView->removeButton,SIGNAL(clicked()),
                    variableItem.data(),SLOT(remove()));
    OS_ASSERT(bingo);

    bingo = connect(variableItemView->variableHeaderView->upButton,SIGNAL(clicked()),
                    variableItem.data(),SLOT(moveUp()));
    OS_ASSERT(bingo);

    bingo = connect(variableItemView->variableHeaderView->downButton,SIGNAL(clicked()),
                    variableItem.data(),SLOT(moveDown()));
    OS_ASSERT(bingo);

    QSharedPointer<MeasureItemDelegate> measureItemDelegate = QSharedPointer<MeasureItemDelegate>(new MeasureItemDelegate(variableItem->isFixedMeasure()));
    variableItemView->variableContentView->measureListView->setDelegate(measureItemDelegate);

    if (variableItem->isFixedMeasure())
    {
      variableItemView->variableHeaderView->measureListView->setListController(measureListController);
      variableItemView->variableHeaderView->measureListView->setDelegate(measureItemDelegate);
    }

    return variableItemView;
  }

  return new QWidget();
}

MeasureListController::MeasureListController(VariableItem * variableItem, openstudio::BaseApp *t_app)
  : OSListController(),
    m_app(t_app),
    m_variableItem(variableItem)
{
}

QSharedPointer<OSListItem> MeasureListController::itemAt(int i)
{
  std::vector<analysis::RubyMeasure> measures = this->measures();

  if( i >= 0 && i < static_cast<int>(measures.size()) )
  {
    analysis::RubyMeasure measure = measures[i];

    QSharedPointer<MeasureItem> item = QSharedPointer<MeasureItem>(new MeasureItem(measure, m_app));

    item->setController(this);

    return item;
  }

  return QSharedPointer<MeasureItem>();
}

int MeasureListController::count()
{
  return measures().size();
}

void MeasureListController::removeItemForMeasure(const analysis::Measure & measure)
{
  std::vector<analysis::RubyMeasure> measures = this->measures();

  int i = 0;
  bool bingo = false;

  for( std::vector<analysis::RubyMeasure>::const_iterator it = measures.begin();
       it != measures.end();
       it++ )
  {
    if( measure == *it )
    {
      bingo = true;

      break;
    }

    i++;
  }

  if( bingo )
  {
    if( measures.size() == 1 )
    {
      qobject_cast<VariableListController *>(m_variableItem->controller())->removeItemForVariable(m_variableItem->variable());
    }
    else
    {
      m_variableItem->variable().erase(measure);

      emit itemRemoved(i);
    }
  }
}

std::vector<analysis::RubyMeasure> MeasureListController::measures() const
{
  std::vector<analysis::RubyMeasure> result;

  std::vector<analysis::Measure> allPerts = m_variableItem->variable().measures(false);

  for( std::vector<analysis::Measure>::iterator it = allPerts.begin();
      it != allPerts.end();
      it++ )
  {
    if( boost::optional<analysis::RubyMeasure> rubyPert = it->optionalCast<analysis::RubyMeasure>() )
    {
      //if( boost::optional<BCLMeasure> measure = rubyPert->measure() )
      //{
      //  if( measure->measureType() ==  m_variableItem->measureType() )
      //  {
      //    result.push_back(*it);
      //  }
      //}
      result.push_back(rubyPert.get());
    }
  }

  return result;
}

void MeasureListController::addItemForDroppedMeasure(QDropEvent * event)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    const QMimeData * mimeData = event->mimeData();

    QByteArray byteArray = mimeData->data(MeasureDragData::mimeType(m_variableItem->measureType()));

    MeasureDragData measureDragData(byteArray);

    UUID id = measureDragData.id();

    // accept the event to make the icon refresh
    event->accept();

    try {
      BCLMeasure projectMeasure = m_app->measureManager().insertReplaceMeasure(*project, id);

      analysis::RubyMeasure measure(projectMeasure);

      try {
        measure.setArguments(m_app->measureManager().getArguments(*project, projectMeasure));
      } catch ( const RubyException& e ) {
        LOG(Error, "Failed to compute arguments for measure: " << e.what());
        QString rejectMessage("Failed to compute arguments for measure: \n\n");
        rejectMessage += QString::fromStdString(e.what());
        QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), rejectMessage);
        return;
      }

      std::string name = m_app->measureManager().suggestMeasureName(projectMeasure, false);
      measure.setName(name);
      measure.setDisplayName(name);
      measure.setDescription(projectMeasure.description());

      bool ok = m_variableItem->variable().push(measure);
      if (ok) {
        emit itemInserted(measures().size() - 1);
      }else{
        LOG(Error, "Failed to add measure at this workflow location.");
        QString rejectMessage("Failed to add measure at this workflow location.");
        QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), rejectMessage);
      }
    } catch (const std::exception &) {
      // the user canceled the update of the measure
      LOG(Debug, "User canceled update of measure.");
      return;
    }
  }
}

MeasureItem::MeasureItem(const analysis::RubyMeasure & measure, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_measure(measure)
{
}

analysis::RubyMeasure MeasureItem::measure() const
{
  return m_measure;
}

QString MeasureItem::name() const
{
  return QString::fromStdString(m_measure.name());
}

void MeasureItem::setName(const QString & name)
{
  m_measure.setName(name.toStdString());
  m_measure.setDisplayName(name.toStdString());

  emit nameChanged(name);
}

QString MeasureItem::description() const
{
  // ETH: Was pulling from measure, but this should be an instance-specific description.
  return QString::fromStdString(m_measure.description());
}

QString MeasureItem::modelerDescription() const
{
  OS_ASSERT(m_measure.usesBCLMeasure());

  return QString::fromStdString(m_measure.measure()->modelerDescription());
}

QString MeasureItem::scriptFileName() const
{
  OS_ASSERT(m_measure.usesBCLMeasure());

  QString scriptName;

  if( boost::optional<openstudio::path> path = m_measure.measure()->primaryRubyScriptPath() )
  {
    scriptName = toQString(path->leaf());
  }

  return scriptName;
}

void MeasureItem::setDescription(const QString & description)
{
  OS_ASSERT(m_measure.usesBCLMeasure());

  // ETH: Was setting description on the measure itself (m_measure.measure()), however, this
  // description should be attached to the instantiated measure, not the global measure.
  m_measure.setDescription(description.toStdString());

  emit descriptionChanged();
}

std::vector<ruleset::OSArgument> MeasureItem::arguments() const
{
  return m_measure.arguments();
}

bool MeasureItem::hasIncompleteArguments() const
{
  return m_measure.hasIncompleteArguments();
}

void MeasureItem::setArgument(const ruleset::OSArgument& argument)
{
  m_measure.setArgument(argument);

  emit argumentsChanged(hasIncompleteArguments());
}

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

  m_app->editController()->reset();

  qobject_cast<MeasureListController *>(controller())->removeItemForMeasure(m_measure);
}

void MeasureItem::setSelected(bool isSelected)
{
  OSListItem::setSelected(isSelected);

  // Ugly hack to prevent the controller from doing this for tab 2.
  // Please someboday fix, perhaps be using a new signal from OSItemSelectionController.
  if( ! controller()->selectionController()->allowMultipleSelections() )
  {
    if( isSelected )
    {
      if( boost::optional<analysis::RubyMeasure> rubyPert = m_measure.optionalCast<analysis::RubyMeasure>() )
      {
        m_app->chooseHorizontalEditTab();

        m_app->editController()->setMeasureItem(this, m_app);
      }
    }
    else
    {
      m_app->editController()->reset();
    }
  }
}

QWidget * MeasureItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<MeasureItem> measureItem = dataSource.objectCast<MeasureItem>())
  {
    MeasureItemView * measureItemView = new MeasureItemView(m_fixed);

    // Name

    measureItemView->measureItemButton->nameLabel->setText(measureItem->name());

    bool bingo = connect(measureItem.data(),SIGNAL(nameChanged(const QString &)),measureItemView->measureItemButton->nameLabel,SLOT(setText(const QString &)));

    OS_ASSERT(bingo);

    // Remove

    bingo = connect(measureItemView->removeButton,SIGNAL(clicked()),measureItem.data(),SLOT(remove()));

    OS_ASSERT(bingo);

    // Selection

    measureItemView->measureItemButton->setHasEmphasis(measureItem->isSelected());

    bingo = connect(measureItemView->measureItemButton,SIGNAL(clicked()),measureItem.data(),SLOT(toggleSelected()));

    OS_ASSERT(bingo);

    bingo = connect(measureItem.data(),SIGNAL(selectedChanged(bool)),measureItemView->measureItemButton,SLOT(setHasEmphasis(bool)));

    OS_ASSERT(bingo);

    // Warning Icon

    measureItemView->measureItemButton->cautionLabel->setVisible(measureItem->hasIncompleteArguments());

    bingo = connect(measureItem.data(),SIGNAL(argumentsChanged(bool)),measureItemView->measureItemButton->cautionLabel,SLOT(setVisible(bool)));

    OS_ASSERT(bingo);

    return measureItemView;
  }

  return new QWidget();
}

} // measuretab


} // openstudio


