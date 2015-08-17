/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#include "DesignAlternativesTabController.hpp"
#include "MeasuresTabController.hpp"
#include "DesignAlternativesView.hpp"
#include "PatApp.hpp"

#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/OSListView.hpp"

#include "PatMainWindow.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/Problem.hpp"
#include "../analysis/NullMeasure.hpp"
#include "../analysis/NullMeasure_Impl.hpp"
#include "../analysis/MeasureGroup.hpp"
#include "../analysis/MeasureGroup_Impl.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../analysis/RubyMeasure_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

#include <algorithm>

#include "../shared_gui_components/VariableList.hpp"

namespace openstudio{
  
namespace pat {

using namespace altstab;

DesignAlternativesTabController::DesignAlternativesTabController()
  : QObject()
{
  m_isButtonStateDirty = false;

  designAlternativesTabView = new DesignAlternativesTabView();

  m_variableGroupListController = QSharedPointer<measuretab::VariableGroupListController>(new measuretab::VariableGroupListController(true, PatApp::instance()));
  m_variableGroupListController->selectionController()->setAllowMultipleSelections(true);
  m_altsTabVariableGroupItemDelegate = QSharedPointer<altstab::AltsTabVariableGroupItemDelegate>(new altstab::AltsTabVariableGroupItemDelegate());

  designAlternativesTabView->measuresListView->setListController(m_variableGroupListController);
  designAlternativesTabView->measuresListView->setDelegate(m_altsTabVariableGroupItemDelegate);

  m_designAltListController = QSharedPointer<altstab::DesignAltListController>(new altstab::DesignAltListController(m_variableGroupListController->selectionController()));
  m_designAltItemDelegate = QSharedPointer<altstab::DesignAltItemDelegate>(new altstab::DesignAltItemDelegate());

  designAlternativesTabView->designAltsView->designAltsListView->setListController(m_designAltListController);
  designAlternativesTabView->designAltsView->designAltsListView->setDelegate(m_designAltItemDelegate);

  connect(designAlternativesTabView->designAltsView->createOneWithSelectedMeasuresButton, &QPushButton::clicked, m_designAltListController.data(), &DesignAltListController::addOneItemWithAllSelectedMeasures);

  connect(designAlternativesTabView->designAltsView->createOneForEachSelectedMeasureButton, &QPushButton::clicked, m_designAltListController.data(), &DesignAltListController::addOneItemForEachSelectedMeasure);

  connect(designAlternativesTabView->designAltsView->createFromFileButton, &QPushButton::clicked, m_designAltListController.data(), &DesignAltListController::addItemFromExternalFile);

  connect(designAlternativesTabView->clearSelectionsButton, &QPushButton::clicked, m_variableGroupListController->selectionController().data(), &OSItemSelectionController::unselectAllItems);

  connect(designAlternativesTabView->selectAllButton, &QPushButton::clicked, m_variableGroupListController->selectionController().data(), &OSItemSelectionController::selectAllItems);

  connect(m_variableGroupListController->selectionController().data(), &OSItemSelectionController::selectionChanged, this, &DesignAlternativesTabController::updateButtonStatusBasedOnSelection);
}

DesignAlternativesTabController::~DesignAlternativesTabController()
{
  if( designAlternativesTabView ) { delete designAlternativesTabView; }
}

void DesignAlternativesTabController::updateButtonStatusBasedOnSelection()
{
  m_isButtonStateDirty = true;

  QTimer::singleShot(0,this,SLOT(updateButtonStatusBasedOnSelectionNow()));
}

void DesignAlternativesTabController::updateButtonStatusBasedOnSelectionNow()
{
  if( m_isButtonStateDirty )
  {
    // Get all of the selected items.
    std::vector<QPointer<OSListItem> > items = m_variableGroupListController->selectionController()->selectedItems();

    bool measuresSelected = false;
    for( std::vector<QPointer<OSListItem> >::const_iterator it = items.begin(); it != items.end(); ++it ) {
      if( qobject_cast<measuretab::MeasureItem *>(*it) ){
        measuresSelected = true;
        break;
      }
    }

    if( measuresSelected )
    {
      designAlternativesTabView->clearSelectionsButton->setEnabled(true);
      designAlternativesTabView->designAltsView->createOneForEachSelectedMeasureButton->setEnabled(true);

      // Determine if we should enable the createOneWithSelectedMeasuresButton.
      boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

      if( project )
      {
        analysis::Problem problem = project->analysis().problem();

        // Get all of the selected measures.
        std::vector<analysis::Measure> selectedperts;

        for( std::vector<QPointer<OSListItem> >::const_iterator it = items.begin(); 
             it != items.end();
             ++it )
        {
          if( measuretab::MeasureItem * measureItem = qobject_cast<measuretab::MeasureItem *>(*it) )
          {
            selectedperts.push_back(measureItem->measure());
          }
        }

        // Get all of the variables.
        std::vector<analysis::InputVariable> vars = problem.variables();
        std::vector<analysis::MeasureGroup> measureGroups = subsetCastVector<analysis::MeasureGroup>(vars);

        // If any of the variables have more than one pert, then flag.

        // It would be nice if AnalysisObject where sortable.  (Had a < operator)
        // Then we could clean this up using std::intersection or similar.

        bool duplicateVarSelections = false;

        for( std::vector<analysis::MeasureGroup>::const_iterator varit = measureGroups.begin();
             varit != measureGroups.end();
             ++varit )
        {

          int varSelections = 0;

          std::vector<analysis::Measure> varperts = varit->measures(false);

          for( std::vector<analysis::Measure>::const_iterator varPertIt = varperts.begin();
               varPertIt != varperts.end();
               ++varPertIt )
          {
            auto match = std::find(selectedperts.begin(),selectedperts.end(),*varPertIt);

            if(match != selectedperts.end())
            {
              varSelections++;
            }
          }

          if( varSelections > 1 )
          {
            duplicateVarSelections = true;
            break;
          }
        }
        
        if( duplicateVarSelections )
        {
          designAlternativesTabView->designAltsView->createOneWithSelectedMeasuresButton->setEnabled(false);
        }
        else
        {
          designAlternativesTabView->designAltsView->createOneWithSelectedMeasuresButton->setEnabled(true);
        }
      }
    }
    else
    {
      designAlternativesTabView->clearSelectionsButton->setEnabled(false);
      designAlternativesTabView->designAltsView->createOneWithSelectedMeasuresButton->setEnabled(false);
      designAlternativesTabView->designAltsView->createOneForEachSelectedMeasureButton->setEnabled(false);
    }

    m_isButtonStateDirty = false;
  }
}

namespace altstab {

QWidget* AltsTabVariableGroupItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<measuretab::VariableGroupItem> variableGroupItem = dataSource.objectCast<measuretab::VariableGroupItem>() )
  {
    QSharedPointer<measuretab::VariableListController> variableListController = variableGroupItem->variableListController();
    QSharedPointer<altstab::AltsTabVariableItemDelegate> altsTabVariableItemDelegate = QSharedPointer<altstab::AltsTabVariableItemDelegate>(new altstab::AltsTabVariableItemDelegate());

    auto altsTabVariableGroupItemView = new altstab::AltsTabVariableGroupItemView(); 

    altsTabVariableGroupItemView->variableGroupHeaderView->label->setText(variableGroupItem->label());
    altsTabVariableGroupItemView->variableGroupContentView->setListController(variableListController);
    altsTabVariableGroupItemView->variableGroupContentView->setDelegate(altsTabVariableItemDelegate);

    if( variableListController->count() == 0 )
    {
      altsTabVariableGroupItemView->variableGroupHeaderView->hide();
    }

    return altsTabVariableGroupItemView;
  }

  return new QWidget();
}

QWidget* AltsTabVariableItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<measuretab::VariableItem> variableItem = dataSource.objectCast<measuretab::VariableItem>() )
  {
    QSharedPointer<measuretab::MeasureListController> measureListController = variableItem->measureListController();
    QSharedPointer<altstab::AltsTabMeasureItemDelegate> altsTabMeasureItemDelegate = QSharedPointer<altstab::AltsTabMeasureItemDelegate>(new altstab::AltsTabMeasureItemDelegate());

    auto altsTabVariableItemView = new altstab::AltsTabVariableItemView();

    altsTabVariableItemView->variableHeaderView->setText(variableItem->displayName());
    altsTabVariableItemView->variableContentView->setListController(measureListController);
    altsTabVariableItemView->variableContentView->setDelegate(altsTabMeasureItemDelegate);

    return altsTabVariableItemView;
  }

  return new QWidget();
}

QWidget* AltsTabMeasureItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<measuretab::MeasureItem> measureItem = dataSource.objectCast<measuretab::MeasureItem>())
  {
    auto altsTabMeasureItemView = new altstab::AltsTabMeasureItemView();

    // Name

    altsTabMeasureItemView->label->setText(measureItem->displayName());

    connect(measureItem.data(), &measuretab::MeasureItem::displayNameChanged, altsTabMeasureItemView->label, &QLabel::setText);

    // Selection

    altsTabMeasureItemView->setHasEmphasis(measureItem->isSelected());

    connect(altsTabMeasureItemView, &altstab::AltsTabMeasureItemView::clicked, measureItem.data(), &measuretab::MeasureItem::toggleSelected);

    connect(measureItem.data(), &measuretab::MeasureItem::selectedChanged, altsTabMeasureItemView, &altstab::AltsTabMeasureItemView::setHasEmphasis);

    return altsTabMeasureItemView;
  }

  return new QWidget();
}

DesignAltListController::DesignAltListController(QSharedPointer<OSItemSelectionController> measureSelectionController)
  : OSListController(),
    m_measureSelectionController(measureSelectionController)
{
}

QSharedPointer<OSListItem> DesignAltListController::itemAt(int i)
{
  if( i >= 0 && i < count() )
  {
    analysis::DataPoint datapoint = dataPoints()[i];

    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

    bool isBaseline = false;
    if (project){
      isBaseline = (datapoint == project->baselineDataPoint());
    }

    bool isAlternativeModel = false;
    bool alternateModelMeasureNeedsUpdate = false;
    if (project){
      analysis::OptionalMeasureGroup modelSwapVariable = project->getAlternativeModelVariable();
      if (modelSwapVariable){
        analysis::Measure swapMeasure = modelSwapVariable->getMeasure(datapoint);
        // check if we are using the ruby measure for this variable rather than the null one
        analysis::OptionalRubyMeasure rubySwapMeasure = swapMeasure.optionalCast<analysis::RubyMeasure>();
        if (rubySwapMeasure){
          isAlternativeModel = true;
          alternateModelMeasureNeedsUpdate = true;

          // look for the new argument
          for (const ruleset::OSArgument& argument : rubySwapMeasure->arguments()){
            if (argument.name() == "measures_json"){
              alternateModelMeasureNeedsUpdate = false;
            }
          }
        }
      }
    }

    QSharedPointer<DesignAltItem> item = QSharedPointer<DesignAltItem>(new DesignAltItem(datapoint, isBaseline, isAlternativeModel, alternateModelMeasureNeedsUpdate));

    item->setController(this);

    return item;
  }

  return QSharedPointer<OSListItem>();
}

int DesignAltListController::count()
{
  return (int)dataPoints().size();
}

std::vector<analysis::DataPoint> DesignAltListController::dataPoints() const
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

  // Ensure baseline is created
  project->baselineDataPoint();

  std::vector<analysis::DataPoint> dataPoints = project->analysis().dataPoints();

  return dataPoints;
}

std::string DesignAltListController::suggestDesignAltName(const boost::optional<analysis::Measure>& measure) const
{
  std::string baseName = "New Design Alternative";
  if (measure){
    baseName = measure->displayName() + " Only";
  }

  std::set<std::string> allNames;
  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() ){
    for (const analysis::DataPoint& dataPoint : project->analysis().dataPoints()) {
      allNames.insert(dataPoint.name());
      allNames.insert(dataPoint.displayName());
    }
  }

  std::string result = baseName;
  int i = 1;
  while (allNames.find(result) != allNames.end()){
    result = baseName + " " + QString::number(i).toStdString();
    i++;
  }

  return result;
}

void DesignAltListController::removeItemForDataPoint(analysis::DataPoint datapoint)
{
  std::vector<analysis::DataPoint> points = dataPoints();

  std::vector<analysis::DataPoint>::const_iterator it = std::find(points.begin(),points.end(),datapoint);

  OS_ASSERT(it != points.end());

  int index = it - points.begin();

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

  if(!project.is_initialized()) return;

  analysis::Analysis analysis = project->analysis();
  analysisdriver::AnalysisDriver analysisDriver = project->analysisDriver();
  bool bingo = analysisdriver::removeDataPoint(analysis,datapoint,analysisDriver);

  OS_ASSERT(bingo);

  emit itemRemoved(index);
}

void DesignAltListController::addItemFromExternalFile()
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

  if( project )
  {
    QString fileName = QFileDialog::getOpenFileName( PatApp::instance()->mainWindow.data(),
                                                     tr("Select Model"),
                                                     QDir::homePath(),
                                                     tr("(*.osm)") );

    if (!fileName.isEmpty())
    {
      bool bingo = project->addAlternateModel(toPath(fileName));

      if( bingo )
      {
        emit itemInserted(count() - 1);
      }
    }
  }
}

void DesignAltListController::addOneItemForEachSelectedMeasure()
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

  if( project )
  {
    analysis::Problem problem = project->analysis().problem();

    int duplicates = 0;

    // Get all of the selected measures.
    std::vector<QPointer<OSListItem> > items = m_measureSelectionController->selectedItems();

    std::vector<analysis::Measure> selectedperts;

    for( std::vector<QPointer<OSListItem> >::const_iterator it = items.begin(); 
         it != items.end();
         ++it )
    {
      if( measuretab::MeasureItem * measureItem = qobject_cast<measuretab::MeasureItem *>(*it) )
      {
        selectedperts.push_back(measureItem->measure());
      }
    }

    // Get all of the variables in the problem.
    std::vector<analysis::InputVariable> vars = problem.variables();
    std::vector<analysis::MeasureGroup> measureGroups = subsetCastVector<analysis::MeasureGroup>(vars);

    // Loop over the selected perts
    for( std::vector<analysis::Measure>::const_iterator pertIter = selectedperts.begin();
         pertIter != selectedperts.end();
         ++pertIter )
    {
      std::vector<analysis::Measure> newPointPerts;

      for( std::vector<analysis::MeasureGroup>::const_iterator varIter = measureGroups.begin();
           varIter != measureGroups.end();
           ++varIter )
      {
        std::vector<analysis::Measure> varperts = varIter->measures(false);

        if (varperts.size() == 1)
        {
          // this is a fixed measure
          newPointPerts.push_back(varperts.front());
        } else {
          // See if the selected pert is in the variable
          std::vector<analysis::Measure>::const_iterator matchIt = std::find(varperts.begin(),varperts.end(),*pertIter);

          if( matchIt == varperts.end())
          {
            // get the null pert for this variable and add it to selectedperts
            std::vector<analysis::NullMeasure> nullPerts = subsetCastVector<analysis::NullMeasure>(varperts);

            OS_ASSERT(nullPerts.size() > 0);

            newPointPerts.push_back(nullPerts.front());
          } else {
            // add the match to the new point
            newPointPerts.push_back(*matchIt);
          }
        }
      }

      analysis::OptionalDataPoint dataPoint = problem.createDataPoint(newPointPerts);

      if( dataPoint )
      {
        std::string name = suggestDesignAltName(*pertIter);
        dataPoint->setName(name);
        dataPoint->setDisplayName(name);

        bool bingo = project->analysis().addDataPoint(*dataPoint);

        if( bingo )
        {
          emit itemInserted(count() - 1);
        }
        else
        {
          duplicates++;
        }
      }
    }

    if(duplicates > 0)
    {
      QString msg;
      msg = msg.setNum(duplicates);
      if(duplicates == 1)
      {
        msg += (" Design Alternative already exists in your project and will not be duplicated.");
      }
      else
      {
        msg += (" Design Alternatives already exist in your project and will not be duplicated.");
      }

      QMessageBox::warning(PatApp::instance()->mainWindow, PatApp::instance()->applicationName(),
                           tr(msg.toStdString().c_str()),
                           QMessageBox::Ok);
    }
  }
}

void DesignAltListController::addOneItemWithAllSelectedMeasures()
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

  if( project )
  {
    analysis::Problem problem = project->analysis().problem();

    // Get all of the selected measures.
    std::vector<QPointer<OSListItem> > items = m_measureSelectionController->selectedItems();

    std::vector<analysis::Measure> selectedperts;

    for( std::vector<QPointer<OSListItem> >::const_iterator it = items.begin(); 
         it != items.end();
         ++it )
    {
      if( measuretab::MeasureItem * measureItem = qobject_cast<measuretab::MeasureItem *>(*it) )
      {
        selectedperts.push_back(measureItem->measure());
      }
    }

    std::vector<analysis::Measure> newPointPerts;

    // Get all of the variables in the problem.  If a variable does not have a selected measure, then use the null pert.
    std::vector<analysis::InputVariable> vars = problem.variables();
    std::vector<analysis::MeasureGroup> measureGroups = subsetCastVector<analysis::MeasureGroup>(vars);

    for( std::vector<analysis::MeasureGroup>::const_iterator it = measureGroups.begin();
        it != measureGroups.end();
        ++it )
    {
      std::vector<analysis::Measure> varperts = it->measures(false);


      if (varperts.size() == 1)
      {
        // this is a fixed measure
        newPointPerts.push_back(varperts.front());
      } else {
        std::vector<analysis::Measure>::const_iterator matchIt = std::find_first_of(selectedperts.begin(),selectedperts.end(),varperts.begin(),varperts.end());
        if( matchIt == selectedperts.end())
        {
          // get the null pert for this variable and add it to selectedperts
          std::vector<analysis::NullMeasure> nullPerts = subsetCastVector<analysis::NullMeasure>(varperts);

          OS_ASSERT(nullPerts.size() > 0);

          newPointPerts.push_back(nullPerts.front());
        } else {
          // add the match to the new point
          newPointPerts.push_back(*matchIt);
        }
      }

    }

    analysis::OptionalDataPoint dataPoint = problem.createDataPoint(newPointPerts);

    if( dataPoint )
    {
      std::string name = suggestDesignAltName(boost::none);
      dataPoint->setName(name);
      dataPoint->setDisplayName(name);

      bool bingo = project->analysis().addDataPoint(*dataPoint);

      if( bingo )
      {
        emit itemInserted(count() - 1);

        return;
      }
      else
      {
        QString msg("This Design Alternative already exists in your project and will not be duplicated.");

        QMessageBox::warning(PatApp::instance()->mainWindow, PatApp::instance()->applicationName(),
                             msg,
                             QMessageBox::Ok);
      }
    }
  }
}

DesignAltItem::DesignAltItem(const analysis::DataPoint & dataPoint, bool isBaseline, bool isAlternativeModel, bool alternateModelMeasureNeedsUpdate)
  : OSListItem(),
    m_dataPoint(dataPoint),
    m_isBaseline(isBaseline),
    m_isAlternativeModel(isAlternativeModel), 
    m_alternateModelMeasureNeedsUpdate(alternateModelMeasureNeedsUpdate)
{
  m_perturbationListController = QSharedPointer<PerturbationListController>(new PerturbationListController(this));
  m_alternativeModelMeasureListController = QSharedPointer<AlternativeModelMeasureListController>(new AlternativeModelMeasureListController(this));
}

QString DesignAltItem::name() const
{
  return QString::fromStdString(m_dataPoint.displayName());
}

QString DesignAltItem::description() const
{
  return QString::fromStdString(m_dataPoint.description());
}

void DesignAltItem::setName(const QString & name)
{
  m_dataPoint.setName(name.toStdString());
  m_dataPoint.setDisplayName(name.toStdString());
}

void DesignAltItem::setDescription(const QString & description)
{
  m_dataPoint.setDescription(description.toStdString());
}

bool DesignAltItem::isBaseline() const
{
  return m_isBaseline;
}

bool DesignAltItem::isAlternativeModel() const
{
  return m_isAlternativeModel;
}

bool DesignAltItem::alternateModelMeasureNeedsUpdate() const
{
  return m_alternateModelMeasureNeedsUpdate;
}

analysis::DataPoint DesignAltItem::dataPoint() const
{
  return m_dataPoint;
}

void DesignAltItem::remove()
{
  qobject_cast<DesignAltListController *>(controller())->removeItemForDataPoint(m_dataPoint);
}

QSharedPointer<PerturbationListController> DesignAltItem::perturbationListController() const
{
  return m_perturbationListController;
}

QSharedPointer<AlternativeModelMeasureListController> DesignAltItem::alternativeModelMeasureListController() const
{
  return m_alternativeModelMeasureListController;
}

QWidget * DesignAltItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<DesignAltItem> designAltItem = dataSource.objectCast<DesignAltItem>())
  {
    auto designAltItemView = new DesignAltItemView(designAltItem->isBaseline(), designAltItem->isAlternativeModel(), designAltItem->alternateModelMeasureNeedsUpdate());

    // Name

    designAltItemView->designAltHeaderView->designAltNameEdit->setText(designAltItem->name());

    connect(designAltItemView->designAltHeaderView->designAltNameEdit, &QLineEdit::textEdited, designAltItem.data(), &DesignAltItem::setName);

    // Description

    designAltItemView->designAltContentView->descriptionTextEdit->setText(designAltItem->description());

    connect(designAltItemView->designAltContentView, &DesignAltContentView::descriptionChanged, designAltItem.data(), &DesignAltItem::setDescription);

    // Remove

    connect(designAltItemView->designAltHeaderView->removeButton, &QPushButton::clicked, designAltItem.data(), &DesignAltItem::remove);

    // Perturbations

    QSharedPointer<PerturbationListController> perturbationListController = designAltItem->perturbationListController();
    QSharedPointer<PerturbationItemDelegate> perturbationItemDelegate = QSharedPointer<PerturbationItemDelegate>(new PerturbationItemDelegate());

    designAltItemView->designAltContentView->perturbationListView->setListController(perturbationListController);
    designAltItemView->designAltContentView->perturbationListView->setDelegate(perturbationItemDelegate);

    // Alternative Model Measures
    if (designAltItem->isAlternativeModel())
    {
      QSharedPointer<AlternativeModelMeasureListController> alternativeModelMeasureListController = designAltItem->alternativeModelMeasureListController();
      QSharedPointer<AlternativeModelMeasureItemDelegate> alternativeModelMeasureItemDelegate = QSharedPointer<AlternativeModelMeasureItemDelegate>(new AlternativeModelMeasureItemDelegate());

      designAltItemView->designAltContentView->alternativeModelMeasureListView->setListController(alternativeModelMeasureListController);
      designAltItemView->designAltContentView->alternativeModelMeasureListView->setDelegate(alternativeModelMeasureItemDelegate);
    
      connect(designAltItemView->designAltContentView, &DesignAltContentView::addAlternativeModelMeasureClicked, alternativeModelMeasureListController.data(), &AlternativeModelMeasureListController::addAlternativeModelMeasure);
     
      connect(designAltItemView->designAltContentView, &DesignAltContentView::descriptionChanged, designAltItem.data(), &DesignAltItem::setDescription);
    }

    return designAltItemView;
  }

  return new QWidget();
}

PerturbationListController::PerturbationListController(DesignAltItem * designAltItem)
  : OSListController(),
    m_designAltItem(designAltItem)
{
}

QSharedPointer<OSListItem> PerturbationListController::itemAt(int i)
{
  if( i >= 0 && i < count() )
  {
    analysis::Measure measure = measures()[i];

    QSharedPointer<PerturbationItem> item = QSharedPointer<PerturbationItem>(new PerturbationItem(measure));

    return item;
  }

  return QSharedPointer<OSListItem>();
}

int PerturbationListController::count()
{
  return (int)measures().size();
}

std::vector<analysis::Measure> PerturbationListController::measures() const
{
  std::vector<analysis::Measure > result;

  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    analysis::Problem problem = project->analysis().problem();

    std::vector<QVariant> variableValues = m_designAltItem->dataPoint().variableValues();

    std::vector<boost::optional<analysis::Measure> > measures = problem.getMeasures(variableValues);

    for( auto & measure : measures )
    {
      if( measure && (! measure->optionalCast<analysis::NullMeasure>()) )
      {
        result.push_back(*measure);
      }
    }
  }

  return result;
}

PerturbationItem::PerturbationItem(const analysis::Measure & measure)
  : OSListItem(),
    m_measure(measure)
{
}

QString PerturbationItem::name() const
{
  return openstudio::toQString(m_measure.displayName());
}

bool PerturbationItem::isFixedMeasureItem() const
{
  boost::optional<analysis::MeasureGroup> parent = measureGroupParent();

  if (parent)
  {
    return parent->measures(false).size() == 1;
  }

  return false;
}

boost::optional<analysis::MeasureGroup> PerturbationItem::measureGroupParent() const
{
  boost::optional<analysis::AnalysisObject> parent = m_measure.parent();

  if (parent)
  {
    boost::optional<analysis::MeasureGroup> var = parent->cast<analysis::MeasureGroup>();
    if (var)
    {
      return var;
    }
  }

  return boost::optional<analysis::MeasureGroup>();
}

QWidget * PerturbationItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<PerturbationItem> perturbationItem = dataSource.objectCast<PerturbationItem>())
  {
    auto perturbationItemView = new QLabel();

    perturbationItemView->setText(perturbationItem->name());

    if (perturbationItem->isFixedMeasureItem())
    {
      perturbationItemView->setStyleSheet("font:italic");
    }

    return perturbationItemView;
  }

  return new QWidget();
}


AlternativeModelMeasureListController::AlternativeModelMeasureListController(DesignAltItem * designAltItem)
  : OSListController(),
    m_designAltItem(designAltItem),
    m_count((int)this->modelMeasures().size())
{
}

QSharedPointer<OSListItem> AlternativeModelMeasureListController::itemAt(int i)
{
  if (i >= 0 && i < m_count)
  {
    return alternativeModelMeasureItems()[i];
  }

  return QSharedPointer<OSListItem>();
}

int AlternativeModelMeasureListController::count()
{
  return m_count;
}

void AlternativeModelMeasureListController::addAlternativeModelMeasure()
{
  if (boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project()){
    if (project->analysis().completeDataPoints().size() > 0u){
      QMessageBox::StandardButton test = QMessageBox::question(
        PatApp::instance()->mainWidget(),
        "Clear results?",
        "Adding user defined measure will remove all results, do you want to proceed?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
      if (test == QMessageBox::No){
        return;
      }
    }
  }

  QJsonArray modelMeasures = this->modelMeasures();

  QJsonObject newMeasure;
  newMeasure.insert("uuid", UUID::createUuid().toString());
  newMeasure.insert("displayName", QString("New User Defined Measure"));
  newMeasure.insert("description", QString("New User Defined Measure Description"));
  newMeasure.insert("taxonomyTag", QString("Envelope.Form"));
  newMeasure.insert("capitalCost", 0.0);

  modelMeasures.push_back(newMeasure);

  setModelMeasures(modelMeasures, true);
}

void AlternativeModelMeasureListController::alternativeModelMeasureItemChanged()
{
  if (boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project()){
    if (project->analysis().completeDataPoints().size() > 0u){
      QMessageBox::StandardButton test = QMessageBox::question(
        PatApp::instance()->mainWidget(),
        "Clear results?",
        "Changing user defined measure will remove all results, do you want to proceed?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
      if (test == QMessageBox::No){

        // DLM: no change was made, need to reset view to stored arguments
        // redraw all the views
        QTimer::singleShot(0, this, SLOT(emitModelReset()));

        return;
      }
    }
  }

  QObject* sender = this->sender();

  AlternativeModelMeasureItem* alternativeModelMeasureItem = qobject_cast<AlternativeModelMeasureItem*>(sender);
  OS_ASSERT(alternativeModelMeasureItem);

  QJsonArray modelMeasures = this->modelMeasures();
  QJsonArray updatedModelMeasures;
  for (QJsonValue jsonValue : modelMeasures) {
    QJsonObject jsonObject = jsonValue.toObject();
    if (jsonObject["uuid"].toString() == alternativeModelMeasureItem->uuid()){

      jsonObject.insert("displayName", alternativeModelMeasureItem->displayName());
      jsonObject.insert("description", alternativeModelMeasureItem->description());
      jsonObject.insert("taxonomyTag", alternativeModelMeasureItem->taxonomyTag());
      jsonObject.insert("capitalCost", alternativeModelMeasureItem->capitalCost());

    } 

    updatedModelMeasures.push_back(jsonObject);
  }

  setModelMeasures(updatedModelMeasures, false);
}

void AlternativeModelMeasureListController::alternativeModelMeasureItemRemoved()
{
  if (boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project()){
    if (project->analysis().completeDataPoints().size() > 0u){
      QMessageBox::StandardButton test = QMessageBox::question(
        PatApp::instance()->mainWidget(),
        "Clear results?",
        "Removing user defined measure will remove all results, do you want to proceed?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
      if (test == QMessageBox::No){
        return;
      }
    }
  }

  QObject* sender = this->sender();

  AlternativeModelMeasureItem* alternativeModelMeasureItem = qobject_cast<AlternativeModelMeasureItem*>(sender);
  OS_ASSERT(alternativeModelMeasureItem);

  QJsonArray modelMeasures = this->modelMeasures();
  QJsonArray newModelMeasures;
  for (QJsonValue jsonValue : modelMeasures) {
    QJsonObject jsonObject = jsonValue.toObject();
    if (jsonObject["uuid"].toString() != alternativeModelMeasureItem->uuid()){
      newModelMeasures.push_back(jsonObject);
    }
  }

  setModelMeasures(newModelMeasures, true);
}

void AlternativeModelMeasureListController::emitModelReset()
{
  modelReset();
}

analysis::OptionalRubyMeasure AlternativeModelMeasureListController::rubySwapMeasure() const
{
  if (boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project()){

    analysis::OptionalMeasureGroup modelSwapVariable = project->getAlternativeModelVariable();

    if (modelSwapVariable){
      analysis::Measure swapMeasure = modelSwapVariable->getMeasure(m_designAltItem->dataPoint());

      return swapMeasure.optionalCast<analysis::RubyMeasure>();
    }
  }

  return boost::none;
}

QJsonArray AlternativeModelMeasureListController::modelMeasures() const
{
  QJsonArray result;

  if (analysis::OptionalRubyMeasure rubySwapMeasure = this->rubySwapMeasure()){
    for (const ruleset::OSArgument& argument : rubySwapMeasure->arguments()){
      if (argument.name() == "measures_json"){
        if (argument.hasValue()){
          QString value = toQString(argument.valueAsString());

          QJsonParseError parseError;
          QJsonDocument jsonDoc = QJsonDocument::fromJson(value.toUtf8(), &parseError);
          if (QJsonParseError::NoError == parseError.error) {
            result = jsonDoc.array();
          }
        }
      }
    }
  }


  return result;
}

void AlternativeModelMeasureListController::setModelMeasures(const QJsonArray& modelMeasures, bool forceRefresh)
{
  bool test = false;

  if (analysis::OptionalRubyMeasure rubySwapMeasure = this->rubySwapMeasure()){
    std::vector<ruleset::OSArgument> arguments = rubySwapMeasure->arguments();
    for (ruleset::OSArgument& argument : arguments){
      if (argument.name() == "measures_json"){

        QJsonDocument jsonDoc;
        jsonDoc.setArray(modelMeasures);
        QString qValue(jsonDoc.toJson(QJsonDocument::Compact));
        std::string value = toString(qValue);

        test = argument.setValue(value);

        break;
      }
    }

    if (test){
      // set the modified arguments
      rubySwapMeasure->setArguments(arguments);

      // update the count
      m_count = modelMeasures.size();

      if (forceRefresh){
        // redraw all the views
        modelReset();
      }
    } else{
      // what happened?
      test = false;
    }
  }
}

std::vector<QSharedPointer<AlternativeModelMeasureItem> > AlternativeModelMeasureListController::alternativeModelMeasureItems() const
{
  std::vector<QSharedPointer<AlternativeModelMeasureItem> > result;

  QJsonArray modelMeasures = this->modelMeasures();

  for (const QJsonValue & jsonValue : modelMeasures) {
    QJsonObject jsonObject = jsonValue.toObject();
    QString uuid = jsonObject["uuid"].toString();
    QString displayName = jsonObject["displayName"].toString();
    QString description = jsonObject["description"].toString();
    QString taxonomyTag = jsonObject["taxonomyTag"].toString();
    double capitalCost = jsonObject["capitalCost"].toDouble();

    QSharedPointer<AlternativeModelMeasureItem> alternativeModelMeasureItem(new AlternativeModelMeasureItem(uuid, displayName, description, taxonomyTag, capitalCost));

    connect(alternativeModelMeasureItem.data(), &AlternativeModelMeasureItem::changed, this, &AlternativeModelMeasureListController::alternativeModelMeasureItemChanged);
    
    connect(alternativeModelMeasureItem.data(), &AlternativeModelMeasureItem::removed, this, &AlternativeModelMeasureListController::alternativeModelMeasureItemRemoved);

    result.push_back(alternativeModelMeasureItem);
  }

  return result;
}

AlternativeModelMeasureItem::AlternativeModelMeasureItem(const QString& uuid,
                                                         const QString& displayName,
                                                         const QString& description,
                                                         const QString& taxonomyTag,
                                                         double capitalCost)
  : OSListItem(),
  m_uuid(uuid),
  m_displayName(displayName),
  m_description(description),
  m_taxonomyTag(taxonomyTag),
  m_capitalCost(capitalCost)
{
}

QString AlternativeModelMeasureItem::uuid() const
{
  return m_uuid;
}

QString AlternativeModelMeasureItem::displayName() const
{
  return m_displayName;
}

QString AlternativeModelMeasureItem::description() const
{
  return m_description;
}

QString AlternativeModelMeasureItem::taxonomyTag() const
{
  return m_taxonomyTag;
}

double AlternativeModelMeasureItem::capitalCost() const
{
  return m_capitalCost;
}

void AlternativeModelMeasureItem::viewChanged() 
{
  QObject* sender = this->sender();

  AlternativeModelMeasureItemView* alternativeModelMeasureItemView = qobject_cast<AlternativeModelMeasureItemView*>(sender);
  OS_ASSERT(alternativeModelMeasureItemView);

  m_displayName = alternativeModelMeasureItemView->displayName();
  m_description = alternativeModelMeasureItemView->description();
  m_taxonomyTag = alternativeModelMeasureItemView->taxonomyTag();
  m_capitalCost = alternativeModelMeasureItemView->capitalCost();

  emit changed();
}

QWidget * AlternativeModelMeasureItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if (QSharedPointer<AlternativeModelMeasureItem> alternativeModelMeasureItem = dataSource.objectCast<AlternativeModelMeasureItem>())
  {
    auto alternativeModelMeasureItemView = new AlternativeModelMeasureItemView(alternativeModelMeasureItem->uuid());

    alternativeModelMeasureItemView->setDisplayName(alternativeModelMeasureItem->displayName());
    alternativeModelMeasureItemView->setDescription(alternativeModelMeasureItem->description());
    alternativeModelMeasureItemView->setTaxonomyTag(alternativeModelMeasureItem->taxonomyTag());
    alternativeModelMeasureItemView->setCapitalCost(alternativeModelMeasureItem->capitalCost());

    connect(alternativeModelMeasureItemView, &AlternativeModelMeasureItemView::changed, alternativeModelMeasureItem.data(), &AlternativeModelMeasureItem::viewChanged);

    connect(alternativeModelMeasureItemView->removeAlternativeModelMeasure, &QPushButton::clicked, alternativeModelMeasureItem.data(), &AlternativeModelMeasureItem::removed);


    return alternativeModelMeasureItemView;
  }

  return new QWidget();
}

} // altstab

} // pat

} // openstudio


