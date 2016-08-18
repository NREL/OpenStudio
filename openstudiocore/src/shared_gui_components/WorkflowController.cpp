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


#include "WorkflowController.hpp"
#include "WorkflowView.hpp"
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
#include "../utilities/filetypes/WorkflowStep_Impl.hpp"
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

WorkflowController::WorkflowController(BaseApp *t_app)
  : OSListController()
{
  QSharedPointer<WorkflowSectionItem> workflowSectionItem;

  workflowSectionItem = QSharedPointer<WorkflowSectionItem>(new WorkflowSectionItem(MeasureType::ModelMeasure, "OpenStudio Measures", t_app));
  addItem(workflowSectionItem);

  workflowSectionItem = QSharedPointer<WorkflowSectionItem>(new WorkflowSectionItem(MeasureType::EnergyPlusMeasure, "EnergyPlus Measures", t_app));
  addItem(workflowSectionItem);

  workflowSectionItem = QSharedPointer<WorkflowSectionItem>(new WorkflowSectionItem(MeasureType::ReportingMeasure, "Reporting Measures", t_app));
  addItem(workflowSectionItem);
}

void WorkflowController::addItem(QSharedPointer<OSListItem> item)
{
  if( QSharedPointer<WorkflowSectionItem> workflowSectionItem = item.dynamicCast<WorkflowSectionItem>() )
  {
    m_workflowSectionItems.push_back(workflowSectionItem);
    workflowSectionItem->setController(this);
    workflowSectionItem->workflowStepController()->setSelectionController(selectionController());
  }
}

QSharedPointer<OSListItem> WorkflowController::itemAt(int i)
{
  if( i >= 0 && i < count() )
  {
    return m_workflowSectionItems[i];
  }

  return QSharedPointer<OSListItem>();
}

int WorkflowController::count()
{
  return m_workflowSectionItems.size();
}

WorkflowSectionItem::WorkflowSectionItem(MeasureType measureType, const QString & label, BaseApp *t_baseApp)
  : OSListItem(),
    m_label(label),
    m_measureType(measureType)
{
  m_workflowStepController = QSharedPointer<WorkflowStepController>(new MeasureStepController(measureType, t_baseApp));
}

QString WorkflowSectionItem::label() const
{
  return m_label;
}

QSharedPointer<WorkflowStepController> WorkflowSectionItem::workflowStepController() const
{
  return m_workflowStepController;
}

WorkflowSectionItemDelegate::WorkflowSectionItemDelegate()
{
}

QWidget * WorkflowSectionItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<WorkflowSectionItem> workflowSectionItem = dataSource.objectCast<WorkflowSectionItem>() )
  {
    QSharedPointer<WorkflowStepController> workflowStepController = workflowSectionItem->workflowStepController();

    if (QSharedPointer<MeasureStepController> measureStepController = qSharedPointerCast<MeasureStepController>(workflowStepController)){
      
      QSharedPointer<MeasureStepItemDelegate> measureStepItemDelegate = QSharedPointer<MeasureStepItemDelegate>(new MeasureStepItemDelegate());

      MeasureType measureType = measureStepController->measureType();

      QString acceptedMimeType = MeasureDragData::mimeType(measureType);

      auto workflowSectionView = new WorkflowSectionView(measureType);

      workflowSectionView->content->newMeasureDropZone->setAcceptedMimeType(acceptedMimeType);

      connect(workflowSectionView->content->newMeasureDropZone, &NewMeasureDropZone::dataDropped, measureStepController.data(), &MeasureStepController::addItemForDroppedMeasure);

      workflowSectionView->content->workflowStepsView->setListController(measureStepController);
      workflowSectionView->content->workflowStepsView->setDelegate(measureStepItemDelegate);

      workflowSectionView->header->label->setText(workflowSectionItem->label());

      return workflowSectionView;
    }
  }

  return new QWidget();
}

WorkflowStepController::WorkflowStepController(openstudio::BaseApp *t_app)
  : OSListController()
{}

MeasureStepController::MeasureStepController(MeasureType measureType, openstudio::BaseApp *t_app)
  : WorkflowStepController(t_app),
    m_app(t_app),
    m_measureType(measureType)
{
}

MeasureType MeasureStepController::measureType() const
{
  return m_measureType;
}

std::vector<MeasureStep> MeasureStepController::measureSteps() const
{
  WorkflowJSON workflowJSON = m_app->currentModel()->workflowJSON();
  std::vector<MeasureStep> result = workflowJSON.getMeasureSteps(m_measureType);
  return result;
}

QSharedPointer<OSListItem> MeasureStepController::itemAt(int i)
{
  std::vector<MeasureStep> steps = this->measureSteps();

  if( i >= 0 && i < (int)steps.size() )
  {
    MeasureStep step = steps[i];

    QSharedPointer<MeasureStepItem> item = QSharedPointer<MeasureStepItem>(new MeasureStepItem(m_measureType, step, m_app));

    item->setController(this);

    return item;
  }

  return QSharedPointer<MeasureStepItem>();
}

int MeasureStepController::count()
{
  return measureSteps().size();
}

void MeasureStepController::removeItemForStep(MeasureStep step)
{
  std::vector<MeasureStep> oldMeasureSteps = measureSteps();

  bool didRemove = false;
  std::vector<MeasureStep> newMeasureSteps;
  newMeasureSteps.reserve(oldMeasureSteps.size());
  for (const auto& oldMeasureStep : oldMeasureSteps){
    if (oldMeasureStep == step){
      didRemove = true;
    } else{
      newMeasureSteps.push_back(oldMeasureStep);
    }
  }

  if (didRemove){
    m_app->currentModel()->workflowJSON().setMeasureSteps(m_measureType, newMeasureSteps);
  }
}

void MeasureStepController::addItemForDroppedMeasure(QDropEvent *event)
{
  // accept the event to make the icon refresh
  event->accept();

  const QMimeData * mimeData = event->mimeData();

  QByteArray byteArray;

  byteArray = mimeData->data(MeasureDragData::mimeType(m_measureType));

  MeasureDragData measureDragData(byteArray);

  UUID id = measureDragData.id();

  // don't allow user to drag standard reports or other managed measures 
  if (m_app->measureManager().isManagedMeasure(id)){
    QMessageBox::warning( m_app->mainWidget(), 
        "Cannot add measure", 
        "This measure conflicts with a managed measure and cannot be added. Create a duplicate of this measure to add to this project.");
    return;
  }

  boost::optional<BCLMeasure> projectMeasure;
  try {

    // Get the measure, will throw if error occurs
    projectMeasure = m_app->measureManager().insertReplaceMeasure(id);

  } catch (const std::exception& e) {

    QString errorMessage("Failed to add measure: \n\n");
    errorMessage += QString::fromStdString(e.what());
    QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
    return;
  }
  OS_ASSERT(projectMeasure);

  if (projectMeasure->measureType() != m_measureType){
    QString errorMessage("Failed to add measure at this workflow location.");
    QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
    return;
  }

  // DLM: TODO, would be nice to be able to construct from BCLMeasure or path
  MeasureStep measureStep(toString(projectMeasure->directory().stem()));
  try{
    //measure.setArguments(m_app->measureManager().getArguments(*project, projectMeasure));
  } catch ( const RubyException&e ) {
    QString errorMessage("Failed to compute arguments for measure: \n\n");
    errorMessage += QString::fromStdString(e.what());
    QMessageBox::information(m_app->mainWidget(), QString("Failed to add measure"), errorMessage);
    return;
  }

  // the new measure
  //std::string name = m_app->measureManager().suggestMeasureName(projectMeasure, t_fixed);
  std::string name = projectMeasure->name();
  measureStep.setName(name);
  //measureStep.setDisplayName(name); // DLM: TODO
  measureStep.setDescription(projectMeasure->description());
  measureStep.setModelerDescription(projectMeasure->modelerDescription());
      
  WorkflowJSON workflowJSON = m_app->currentModel()->workflowJSON();

  std::vector<MeasureStep> measureSteps = workflowJSON.getMeasureSteps(m_measureType);
  measureSteps.push_back(measureStep);
  bool test = workflowJSON.setMeasureSteps(m_measureType, measureSteps);
  OS_ASSERT(test);

  workflowJSON.save();

  emit modelReset();
}

void MeasureStepController::moveUp(MeasureStep step)  
{
  std::vector<MeasureStep> oldMeasureSteps = measureSteps();
  
  bool found = false;
  size_t i;
  size_t n = oldMeasureSteps.size();
  for (i = 0; i < n; ++i){
    if (oldMeasureSteps[i] == step){
      found = true;
      break;
    }
  }

  if( found && i > 0 )
  {
    // swap i with i-1
    MeasureStep temp = oldMeasureSteps[i-1];
    oldMeasureSteps[i-1] = oldMeasureSteps[i];
    oldMeasureSteps[i] = temp;

    m_app->currentModel()->workflowJSON().setMeasureSteps(m_measureType, oldMeasureSteps);

    emit itemChanged(i - 1);
    emit itemChanged(i);
  }
}


void MeasureStepController::moveDown(MeasureStep step)
{
  std::vector<MeasureStep> oldMeasureSteps = measureSteps();
  
  bool found = false;
  size_t i;
  size_t n = oldMeasureSteps.size();
  for (i = 0; i < n; ++i){
    if (oldMeasureSteps[i] == step){
      found = true;
      break;
    }
  }

  if( found && i < (n-1) )
  {
    // swap i with i+1
    MeasureStep temp = oldMeasureSteps[i+1];
    oldMeasureSteps[i+1] = oldMeasureSteps[i];
    oldMeasureSteps[i] = temp;

    m_app->currentModel()->workflowJSON().setMeasureSteps(m_measureType, oldMeasureSteps);

    emit itemChanged(i - 1);
    emit itemChanged(i);
  }
}

MeasureStepItem::MeasureStepItem(MeasureType measureType, MeasureStep step, openstudio::BaseApp *t_app)
  : OSListItem(),
    m_app(t_app),
    m_measureType(measureType),
    m_step(step)
{
}

QString MeasureStepItem::name() const
{ 
  QString result;
  if (boost::optional<std::string> name = m_step.name()){
    return result = QString::fromStdString(*name);
  }
  return result;
}

QString MeasureStepItem::displayName() const
{
  // DLM: TODO, add display name
  QString result;
  if (boost::optional<std::string> name = m_step.name()){
    return result = QString::fromStdString(*name);
  }
  return result;
}

MeasureType MeasureStepItem::measureType() const
{
  OptionalBCLMeasure bclMeasure = this->bclMeasure();
  OS_ASSERT(bclMeasure);
  return bclMeasure->measureType();
}

QString MeasureStepItem::description() const
{
  QString result;
  if (boost::optional<std::string> description = m_step.description()){
    return result = QString::fromStdString(*description);
  }
  return result;
}

QString MeasureStepItem::modelerDescription() const
{
  QString result;
  if (boost::optional<std::string> modelerDescription = m_step.modelerDescription()){
    return result = QString::fromStdString(*modelerDescription);
  }
  return result;
}

QString MeasureStepItem::scriptFileName() const
{
  QString result;
  OptionalBCLMeasure bclMeasure = this->bclMeasure();
  if (bclMeasure){
    boost::optional<path> primaryRubyScriptPath = bclMeasure->primaryRubyScriptPath();
    if (primaryRubyScriptPath){
      result = toQString(*primaryRubyScriptPath);
    }
  }
  return result;
}

OptionalBCLMeasure MeasureStepItem::bclMeasure() const
{
  // TODO: DLM
  return boost::none;
}

std::vector<measure::OSArgument> MeasureStepItem::arguments() const
{
  // TODO: DLM
  return std::vector<measure::OSArgument>();
}

bool MeasureStepItem::hasIncompleteArguments() const
{
  // TODO: DLM
  return false;
}


void MeasureStepItem::remove()
{
/* DLM: TODO
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

  // if any of this variable's measures are being edited, clear the edit controller
  MeasureItem* measureItem = m_app->editController()->measureItem();
  if (measureItem){
    analysis::RubyMeasure rubyMeasure = measureItem->measure();
    for (const auto & measure : m_variable.measures(false)){
      if (measure == rubyMeasure){
        m_app->editController()->reset();
        break;
      }
    }
  }

  qobject_cast<VariableListController *>(controller())->removeItemForVariable(m_variable);
}
*/
}

void MeasureStepItem::moveUp()
{
  // DLM: TODO
  //qobject_cast<WorkflowStepController *>(controller())->moveUp(m_step);
}

void MeasureStepItem::moveDown()
{
  // DLM: TODO
  //qobject_cast<WorkflowStepController *>(controller())->moveDown(m_step);
}

void MeasureStepItem::setName(const QString & name)
{
  m_step.setName(name.toStdString());
}

void MeasureStepItem::setDisplayName(const QString & displayName)
{
  // DLM: TODO, add setDisplayName
  m_step.setName(displayName.toStdString());
}

void MeasureStepItem::setArgument(const measure::OSArgument& argument)
{
  // DLM: TODO
}

MeasureStepItemDelegate::MeasureStepItemDelegate()
{}

QWidget * MeasureStepItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<MeasureStepItem> measureStepItem = dataSource.objectCast<MeasureStepItem>())
  {
    auto workflowStepView = new WorkflowStepView();
    workflowStepView->workflowStepButton->nameLabel->setText(measureStepItem->displayName());

    // DLM: TODO connect for when editing in the tab
    //connect(measureStepItem.data(), &MeasureStepItem::displayNameChanged, workflowStepView->measureItemButton->nameLabel, &QLabel::setText);

    // Remove

    connect(workflowStepView->removeButton, &QPushButton::clicked, measureStepItem.data(), &MeasureStepItem::remove);

    // Selection

    workflowStepView->workflowStepButton->setHasEmphasis(measureStepItem->isSelected());

    connect(workflowStepView->workflowStepButton, &WorkflowStepButton::clicked, measureStepItem.data(), &MeasureStepItem::toggleSelected);

    connect(measureStepItem.data(), &MeasureStepItem::selectedChanged, workflowStepView->workflowStepButton, &WorkflowStepButton::setHasEmphasis);

    // Warning Icon

    workflowStepView->workflowStepButton->cautionLabel->setVisible(measureStepItem->hasIncompleteArguments());

    // DLM: TODO connect for when editing in the tab
    //connect(measureStepItem.data(), &MeasureItem::argumentsChanged, workflowStepView->workflowStepButton->cautionLabel, &QLabel::setVisible);

    return workflowStepView;
  }

  return new QWidget();
}


/* DLM: DELETE
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

void MeasureListController::addItemForDuplicateMeasure(const analysis::Measure& measure) {
  // create duplicate
  analysis::RubyMeasure duplicateMeasure = measure.clone().cast<analysis::RubyMeasure>();

  // set name
  if (OptionalBCLMeasure projectMeasure = duplicateMeasure.bclMeasure()) {
    std::string name = m_app->measureManager().suggestMeasureName(projectMeasure.get(), false);
    duplicateMeasure.setName(name);
    duplicateMeasure.setDisplayName(name);
  }
  else {
    LOG(Debug,"Unable to retrieve BCLMeasure object.");
  }

  // add to variable
  analysis::MeasureGroup measureGroup = m_variableItem->variable();
  int n = measureGroup.numMeasures(false);
  int index(0);
  if (OptionalInt i = measureGroup.getIndexByUUID(measure)) {
    index = i.get() + 1;
  }
  else {
    index = n;
    LOG(Debug,"Should not get here.");
  }

  bool ok = measureGroup.insert(index,duplicateMeasure);
  OS_ASSERT(ok);
  OS_ASSERT(measureGroup.numMeasures(false) == static_cast<unsigned>(n + 1));
  // let everyone know about the new item
  analysis::RubyMeasureVector measuresInList = measures();
  analysis::RubyMeasureVector::const_iterator it = std::find_if(
      measuresInList.begin(),
      measuresInList.end(),
      std::bind(uuidsEqual<analysis::RubyMeasure,analysis::RubyMeasure>,duplicateMeasure,std::placeholders::_1));
  emit itemInserted(int(it - measuresInList.begin()));
}

void MeasureListController::removeItemForMeasure(const analysis::Measure & measure)
{
  std::vector<analysis::RubyMeasure> measures = this->measures();

  int i = 0;
  bool bingo = false;

  for( std::vector<analysis::RubyMeasure>::const_iterator it = measures.begin();
       it != measures.end();
       ++it )
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

  for( const auto & measure : allPerts )
  {
    if( boost::optional<analysis::RubyMeasure> rubyPert = measure.optionalCast<analysis::RubyMeasure>() )
    {
      //if( boost::optional<BCLMeasure> measure = rubyPert->measure() )
      //{
      //  if( measure->measureType() ==  m_variableItem->measureType() )
      //  {
      //    result.push_back(measure);
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
  OS_ASSERT(m_measure.usesBCLMeasure());
}

analysis::RubyMeasure MeasureItem::measure() const
{
  return m_measure;
}

OptionalBCLMeasure MeasureItem::bclMeasure() const
{
  return m_measure.bclMeasure();
}

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

QString MeasureItem::name() const
{
  return QString::fromStdString(m_measure.name());
}

void MeasureItem::setName(const QString & name)
{
  m_measure.setName(name.toStdString());

  // TODO: uncomment
  //emit nameChanged(name);
}

QString MeasureItem::displayName() const
{
  return QString::fromStdString(m_measure.displayName());
}

void MeasureItem::setDisplayName(const QString & displayName)
{
  m_measure.setDisplayName(displayName.toStdString());

  emit displayNameChanged(displayName);
}

QString MeasureItem::description() const
{
  // ETH: Was pulling from bcl measure, but this should be an instance-specific description.
  return QString::fromStdString(m_measure.description());
}

QString MeasureItem::modelerDescription() const
{
  QString result;

  boost::optional<BCLMeasure> bclMeasure = m_measure.bclMeasure();
  if (bclMeasure){
    result = QString::fromStdString(bclMeasure->modelerDescription());
  }else{
    LOG(Error, "Cannot load BCLMeasure '" << toString(m_measure.bclMeasureUUID()) << "' from '" << toString(m_measure.bclMeasureDirectory()) << "'");
  }
  return result; 
}

QString MeasureItem::scriptFileName() const
{
  QString scriptName;

  boost::optional<BCLMeasure> bclMeasure = m_measure.bclMeasure();
  if (bclMeasure){
    if( boost::optional<openstudio::path> path = m_measure.measure()->primaryRubyScriptPath() )
    {
      scriptName = toQString(path->leaf());
    }
  }else{
    LOG(Error, "Cannot load BCLMeasure '" << toString(m_measure.bclMeasureUUID()) << "' from '" << toString(m_measure.bclMeasureDirectory()) << "'");
  }

  return scriptName;
}

void MeasureItem::setDescription(const QString & description)
{
  // ETH: Was setting description on the measure itself (m_measure.bclMeasure()), however, this
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

void MeasureItem::setSelected(bool isSelected)
{
  OSListItem::setSelected(isSelected);

  // Ugly hack to prevent the controller from doing this for tab 2.
  // Please somebody fix, perhaps be using a new signal from OSItemSelectionController.
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

    measureItemView->measureItemButton->nameLabel->setText(measureItem->displayName());

    connect(measureItem.data(), &MeasureItem::displayNameChanged, measureItemView->measureItemButton->nameLabel, &QLabel::setText);

    // Duplicate

    connect(measureItemView->duplicateButton, &QPushButton::clicked, measureItem.data(), &MeasureItem::duplicate);

    // Remove

    connect(measureItemView->removeButton, &QPushButton::clicked, measureItem.data(), &MeasureItem::remove);

    // Selection

    measureItemView->measureItemButton->setHasEmphasis(measureItem->isSelected());

    connect(measureItemView->measureItemButton, &MeasureItemButton::clicked, measureItem.data(), &MeasureItem::toggleSelected);

    connect(measureItem.data(), &MeasureItem::selectedChanged, measureItemView->measureItemButton, &MeasureItemButton::setHasEmphasis);

    // Warning Icon

    measureItemView->measureItemButton->cautionLabel->setVisible(measureItem->hasIncompleteArguments());

    connect(measureItem.data(), &MeasureItem::argumentsChanged, measureItemView->measureItemButton->cautionLabel, &QLabel::setVisible);

    return measureItemView;
  }

  return new QWidget();
}
*/
} // measuretab


} // openstudio


