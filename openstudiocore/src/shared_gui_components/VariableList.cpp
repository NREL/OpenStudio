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

#include <analysis/DataPoint.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/NullPerturbation_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/WorkflowStep.hpp>
#include <analysisdriver/SimpleProject.hpp>
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
  std::map<MeasureType,QString> groups;
  groups[MeasureType::ModelMeasure] = QString("OpenStudio Measures");
  groups[MeasureType::EnergyPlusMeasure] = QString("EnergyPlus Measures");

  for( std::map<MeasureType,QString>::const_iterator it = groups.begin();
       it != groups.end();
       it++ )
  {
    QSharedPointer<VariableGroupItem> variableGroupItem;
    variableGroupItem = QSharedPointer<VariableGroupItem>(new VariableGroupItem(it->first,it->second, filterFixed, t_app));
    addItem(variableGroupItem);
  }
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

VariableGroupItem::VariableGroupItem(MeasureType type, const QString & label, bool filterFixed, BaseApp *t_baseApp)
  : OSListItem(),
    m_label(label)
{
  m_variableListController = QSharedPointer<VariableListController>(new VariableListController(type, filterFixed, t_baseApp));
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

    VariableGroupItemView * variableGroupItemView = new VariableGroupItemView(m_fixedMeasuresOnly);

    if (!m_fixedMeasuresOnly)
    {
      variableGroupItemView->variableGroupContentView->newGroupView->dropZone->setAcceptedMimeType(MeasureDragData::mimeType(variableListController->measureType()));

      bool bingo = connect(variableGroupItemView->variableGroupContentView->newGroupView->dropZone,SIGNAL(dataDropped(QDropEvent * )),
          variableListController.data(),SLOT(addItemForDroppedMeasure(QDropEvent *)));

      Q_ASSERT(bingo);
    }

    variableGroupItemView->variableGroupContentView->newFixedGroupView->dropZone->setAcceptedMimeType(MeasureDragData::mimeType(variableListController->measureType()));

    bool bingo = connect(variableGroupItemView->variableGroupContentView->newFixedGroupView->dropZone,SIGNAL(dataDropped(QDropEvent * )),
                         variableListController.data(),SLOT(addFixedItemForDroppedMeasure(QDropEvent *)));
    Q_ASSERT(bingo);

    variableGroupItemView->variableGroupContentView->variableListView->setListController(variableListController);
    variableGroupItemView->variableGroupContentView->variableListView->setDelegate(variableItemDelegate);

    variableGroupItemView->variableGroupHeader->label->setText(variableGroupItem->label());

    return variableGroupItemView;
  }

  return new QWidget();
}

VariableListController::VariableListController(MeasureType type, bool filterFixed, openstudio::BaseApp *t_app)
  : OSListController(),
    m_app(t_app),
    m_type(type),
    m_filterFixed(filterFixed)
{
}


MeasureType VariableListController::measureType() const
{
  return m_type;
}


QSharedPointer<OSListItem> VariableListController::itemAt(int i)
{
  std::vector<analysis::DiscreteVariable> vars = variables();

  if( i >= 0 && i < (int)vars.size() )
  {
    analysis::DiscreteVariable var = vars[i];

    QSharedPointer<VariableItem> item = QSharedPointer<VariableItem>(new VariableItem(var,m_type, m_app));

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

std::vector<analysis::DiscreteVariable> VariableListController::variables() const
{
  std::vector<analysis::DiscreteVariable> result;

  boost::optional<analysisdriver::SimpleProject> project = m_app->project();

  if( project ) {

    analysis::Problem problem = project->analysis().problem();
    analysis::WorkflowStepVector workflow = problem.workflow();

    if (measureType() == MeasureType::ModelMeasure) {
      analysis::OptionalDiscreteVariable modelSwapVariable = project->getAlternativeModelVariable();
      OptionalInt stopIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
      Q_ASSERT(stopIndex);
      for (int i = 0; i < *stopIndex; ++i) {
        if (workflow[i].isInputVariable()) {
          analysis::InputVariable var = workflow[i].inputVariable();
          if (analysis::OptionalDiscreteVariable dvar = var.optionalCast<analysis::DiscreteVariable>()) {
            if (modelSwapVariable && (*dvar == *modelSwapVariable)) {
              continue;
            }
            std::vector<analysis::DiscretePerturbation> discretePerts = dvar->perturbations(false);
            if (!m_filterFixed || discretePerts.size() > 1)
            {
              // don't add the fixed measures here, they are always selected
              std::vector<analysis::RubyPerturbation> rubyPerts = subsetCastVector<analysis::RubyPerturbation>(discretePerts);
              if (!rubyPerts.empty()) {
                result.push_back(*dvar);
              }
            }
          }
        }
      }
    }
    else {
      Q_ASSERT(measureType() == MeasureType::EnergyPlusMeasure);
      OptionalInt startIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ExpandObjects);
      OptionalInt stopIndex = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlusPreProcess);
      Q_ASSERT(startIndex && stopIndex);
      for (int i = (*startIndex + 1); i < *stopIndex; ++i) {
        if (workflow[i].isInputVariable()) {
          analysis::InputVariable var = workflow[i].inputVariable();
          if (analysis::OptionalDiscreteVariable dvar = var.optionalCast<analysis::DiscreteVariable>()) {
            std::vector<analysis::DiscretePerturbation> discretePerts = dvar->perturbations(false);
            if (!m_filterFixed || discretePerts.size() > 1)
            {
              // don't add the fixed measures here, they are always selected
              std::vector<analysis::RubyPerturbation> rubyPerts = subsetCastVector<analysis::RubyPerturbation>(discretePerts);
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

void VariableListController::removeItemForVariable(analysis::DiscreteVariable variable)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    std::vector<analysis::DiscreteVariable> vars = variables();

    int i = 0;
    bool bingo = false;

    for( std::vector<analysis::DiscreteVariable>::const_iterator it = vars.begin();
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

    QByteArray byteArray = mimeData->data(MeasureDragData::mimeType(m_type));

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
      analysis::DiscreteVariable dv(name, analysis::DiscretePerturbationVector());
      dv.setDisplayName(name);
   
      // measure
      analysis::RubyPerturbation pert(projectMeasure);
      try{
        pert.setArguments(m_app->measureManager().getArguments(*project, projectMeasure));
      } catch ( const RubyException&e ) {
        LOG(Error, "Failed to compute arguments for measure: " << e.what());
        QString errorMessage("Failed to compute arguments for measure: \n\n");
        errorMessage += QString::fromStdString(e.what());
        QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
        return;
      }

      if (!t_fixed)
      {
        // null perturbation
        analysis::NullPerturbation nullPert;
        dv.push(nullPert);
      }

      // the new perturbation
      name = m_app->measureManager().suggestMeasureName(projectMeasure, t_fixed);
      pert.setName(name);
      pert.setDisplayName(name);
      pert.setDescription(projectMeasure.description());
      dv.push(pert);

      // try to add to problem. can fail if measure is of wrong type.
      analysis::Problem problem = project->analysis().problem();
      OptionalInt index;
      FileReferenceType inType = projectMeasure.inputFileType();
      if (inType == FileReferenceType::OSM) {
        Q_ASSERT(measureType() == MeasureType::ModelMeasure);
        index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
      }
      else {
        Q_ASSERT(inType == FileReferenceType::IDF);
        Q_ASSERT(measureType() == MeasureType::EnergyPlusMeasure);
        index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlusPreProcess);
      }
      Q_ASSERT(index);
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

void VariableListController::moveUp(analysis::DiscreteVariable variable)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    // HERE - Logic seems questionable. Also check dirty flags.
    std::vector<analysis::DiscreteVariable> vars = variables();

    int i = 0;
    bool bingo = false;

    for( std::vector<analysis::DiscreteVariable>::const_iterator it = vars.begin();
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

void VariableListController::moveDown(analysis::DiscreteVariable variable)
{
  if( boost::optional<analysisdriver::SimpleProject> project = m_app->project() )
  {
    std::vector<analysis::DiscreteVariable> vars = variables();

    int i = 0;
    bool bingo = false;

    for( std::vector<analysis::DiscreteVariable>::const_iterator it = vars.begin();
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

VariableItem::VariableItem(const analysis::DiscreteVariable & variable, MeasureType type, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_variable(variable),
    m_type(type)
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
  return m_variable.numPerturbations(true) < 2;
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
    Q_ASSERT(bingo);

    QSharedPointer<MeasureListController> measureListController = variableItem->measureListController();
    variableItemView->variableContentView->measureListView->setListController(measureListController);

    if (!variableItem->isFixedMeasure())
    {
      variableItemView->variableContentView->dropZone->setAcceptedMimeType(MeasureDragData::mimeType(variableItem->measureType()));
      bingo = connect(variableItemView->variableContentView->dropZone,SIGNAL(dataDropped(QDropEvent *)),
          variableItem->measureListController().data(),SLOT(addItemForDroppedMeasure(QDropEvent *)));
      Q_ASSERT(bingo);
    }

    bingo = connect(variableItemView->variableHeaderView->removeButton,SIGNAL(clicked()),
                    variableItem.data(),SLOT(remove()));
    Q_ASSERT(bingo);

    bingo = connect(variableItemView->variableHeaderView->upButton,SIGNAL(clicked()),
                    variableItem.data(),SLOT(moveUp()));
    Q_ASSERT(bingo);

    bingo = connect(variableItemView->variableHeaderView->downButton,SIGNAL(clicked()),
                    variableItem.data(),SLOT(moveDown()));
    Q_ASSERT(bingo);

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
  std::vector<analysis::RubyPerturbation> perts = perturbations();

  if( i >= 0 && i < static_cast<int>(perts.size()) )
  {
    analysis::RubyPerturbation pert = perts[i];

    QSharedPointer<MeasureItem> item = QSharedPointer<MeasureItem>(new MeasureItem(pert, m_app));

    item->setController(this);

    return item;
  }

  return QSharedPointer<MeasureItem>();
}

int MeasureListController::count()
{
  return perturbations().size();
}

void MeasureListController::removeItemForPerturbation(const analysis::DiscretePerturbation & pert)
{
  std::vector<analysis::RubyPerturbation> perts = perturbations();

  int i = 0;
  bool bingo = false;

  for( std::vector<analysis::RubyPerturbation>::const_iterator it = perts.begin();
       it != perts.end();
       it++ )
  {
    if( pert == *it )
    {
      bingo = true;

      break;
    }

    i++;
  }

  if( bingo )
  {
    if( perts.size() == 1 )
    {
      qobject_cast<VariableListController *>(m_variableItem->controller())->removeItemForVariable(m_variableItem->variable());
    }
    else
    {
      m_variableItem->variable().erase(pert);

      emit itemRemoved(i);
    }
  }
}

std::vector<analysis::RubyPerturbation> MeasureListController::perturbations() const
{
  std::vector<analysis::RubyPerturbation> result;

  std::vector<analysis::DiscretePerturbation> allPerts = m_variableItem->variable().perturbations(false);

  for( std::vector<analysis::DiscretePerturbation>::iterator it = allPerts.begin();
      it != allPerts.end();
      it++ )
  {
    if( boost::optional<analysis::RubyPerturbation> rubyPert = it->optionalCast<analysis::RubyPerturbation>() )
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

      analysis::RubyPerturbation pert(projectMeasure);

      try {
        pert.setArguments(m_app->measureManager().getArguments(*project, projectMeasure));
      } catch ( const RubyException& e ) {
        LOG(Error, "Failed to compute arguments for measure: " << e.what());
        QString rejectMessage("Failed to compute arguments for measure: \n\n");
        rejectMessage += QString::fromStdString(e.what());
        QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), rejectMessage);
        return;
      }

      std::string name = m_app->measureManager().suggestMeasureName(projectMeasure, false);
      pert.setName(name);
      pert.setDisplayName(name);
      pert.setDescription(projectMeasure.description());

      bool ok = m_variableItem->variable().push(pert);
      if (ok) {
        emit itemInserted(perturbations().size() - 1);
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

MeasureItem::MeasureItem(const analysis::RubyPerturbation & pert, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_pert(pert)
{
}

analysis::RubyPerturbation MeasureItem::perturbation() const
{
  return m_pert;
}

QString MeasureItem::name() const
{
  return QString::fromStdString(m_pert.name());
}

void MeasureItem::setName(const QString & name)
{
  m_pert.setName(name.toStdString());
  m_pert.setDisplayName(name.toStdString());

  emit nameChanged(name);
}

QString MeasureItem::description() const
{
  // ETH: Was pulling from measure, but this should be an instance-specific description.
  return QString::fromStdString(m_pert.description());
}

QString MeasureItem::modelerDescription() const
{
  Q_ASSERT(m_pert.usesBCLMeasure());

  return QString::fromStdString(m_pert.measure()->modelerDescription());
}

QString MeasureItem::scriptFileName() const
{
  Q_ASSERT(m_pert.usesBCLMeasure());

  QString scriptName;

  if( boost::optional<openstudio::path> path = m_pert.measure()->primaryRubyScriptPath() )
  {
    scriptName = toQString(path->leaf());
  }

  return scriptName;
}

void MeasureItem::setDescription(const QString & description)
{
  Q_ASSERT(m_pert.usesBCLMeasure());

  // ETH: Was setting description on the measure itself (m_pert.measure()), however, this 
  // description should be attached to the instantiated perturbation, not the global measure.
  m_pert.setDescription(description.toStdString());

  emit descriptionChanged();
}

std::vector<ruleset::OSArgument> MeasureItem::arguments() const
{
  return m_pert.arguments();
}

bool MeasureItem::hasIncompleteArguments() const
{
  return m_pert.hasIncompleteArguments();
}

void MeasureItem::setArgument(const ruleset::OSArgument& argument)
{
  m_pert.setArgument(argument);

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

  qobject_cast<MeasureListController *>(controller())->removeItemForPerturbation(m_pert);
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
      if( boost::optional<analysis::RubyPerturbation> rubyPert = m_pert.optionalCast<analysis::RubyPerturbation>() )
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

    Q_ASSERT(bingo);

    // Remove

    bingo = connect(measureItemView->removeButton,SIGNAL(clicked()),measureItem.data(),SLOT(remove()));

    Q_ASSERT(bingo);

    // Selection

    measureItemView->measureItemButton->setHasEmphasis(measureItem->isSelected());

    bingo = connect(measureItemView->measureItemButton,SIGNAL(clicked()),measureItem.data(),SLOT(toggleSelected()));

    Q_ASSERT(bingo);

    bingo = connect(measureItem.data(),SIGNAL(selectedChanged(bool)),measureItemView->measureItemButton,SLOT(setHasEmphasis(bool)));

    Q_ASSERT(bingo);

    // Warning Icon

    measureItemView->measureItemButton->cautionLabel->setVisible(measureItem->hasIncompleteArguments());

    bingo = connect(measureItem.data(),SIGNAL(argumentsChanged(bool)),measureItemView->measureItemButton->cautionLabel,SLOT(setVisible(bool)));

    Q_ASSERT(bingo);

    return measureItemView;
  }

  return new QWidget();
}

} // measuretab


} // openstudio

