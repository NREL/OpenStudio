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

#include <pat_app/DesignAlternativesTabController.hpp>
#include <pat_app/MeasuresTabController.hpp>
#include <pat_app/DesignAlternativesView.hpp>
#include <pat_app/PatApp.hpp>

#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/OSListView.hpp"

#include <pat_app/PatMainWindow.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/Problem.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/NullMeasure_Impl.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/MeasureGroup_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>

#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
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
  m_variableGroupItemDelegate = QSharedPointer<VariableGroupItemDelegate>(new VariableGroupItemDelegate());

  designAlternativesTabView->measuresListView->setListController(m_variableGroupListController);
  designAlternativesTabView->measuresListView->setDelegate(m_variableGroupItemDelegate);

  m_designAltListController = QSharedPointer<altstab::DesignAltListController>(new altstab::DesignAltListController(m_variableGroupListController->selectionController()));
  m_designAltItemDelegate = QSharedPointer<altstab::DesignAltItemDelegate>(new altstab::DesignAltItemDelegate());

  designAlternativesTabView->designAltsView->designAltsListView->setListController(m_designAltListController);
  designAlternativesTabView->designAltsView->designAltsListView->setDelegate(m_designAltItemDelegate);

  bool bingo = connect(designAlternativesTabView->designAltsView->createOneWithSelectedMeasuresButton,SIGNAL(clicked()),m_designAltListController.data(),SLOT(addOneItemWithAllSelectedMeasures()));
  OS_ASSERT(bingo);

  bingo = connect(designAlternativesTabView->designAltsView->createOneForEachSelectedMeasureButton,SIGNAL(clicked()),m_designAltListController.data(),SLOT(addOneItemForEachSelectedMeasure()));
  OS_ASSERT(bingo);

  bingo = connect(designAlternativesTabView->designAltsView->createFromFileButton,SIGNAL(clicked()),m_designAltListController.data(),SLOT(addItemFromExternalFile()));
  OS_ASSERT(bingo);

  bingo = connect(designAlternativesTabView->clearSelectionsButton,SIGNAL(clicked()),m_variableGroupListController->selectionController().data(),SLOT(unselectAllItems()));
  OS_ASSERT(bingo);

  bingo = connect(designAlternativesTabView->selectAllButton,SIGNAL(clicked()),m_variableGroupListController->selectionController().data(),SLOT(selectAllItems()));
  OS_ASSERT(bingo);

  bingo = connect(m_variableGroupListController->selectionController().data(),SIGNAL(selectionChanged(std::vector<QPointer<OSListItem> >)),this,SLOT(updateButtonStatusBasedOnSelection()));
  OS_ASSERT(bingo);
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
    for( std::vector<QPointer<OSListItem> >::const_iterator it = items.begin(); it != items.end(); it++ ) {
      if( measuretab::MeasureItem * measureItem = qobject_cast<measuretab::MeasureItem *>(*it) ){
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
             it++ )
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
             varit++ )
        {

          int varSelections = 0;

          std::vector<analysis::Measure> varperts = varit->measures(false);

          for( std::vector<analysis::Measure>::const_iterator varPertIt = varperts.begin();
               varPertIt != varperts.end();
               varPertIt++ )
          {
            std::vector<analysis::Measure>::iterator match = std::find(selectedperts.begin(),selectedperts.end(),*varPertIt);

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

QWidget * VariableGroupItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<measuretab::VariableGroupItem> variableGroupItem = dataSource.objectCast<measuretab::VariableGroupItem>() )
  {
    QSharedPointer<measuretab::VariableListController> variableListController = variableGroupItem->variableListController();
    QSharedPointer<VariableItemDelegate> variableItemDelegate = QSharedPointer<VariableItemDelegate>(new VariableItemDelegate());

    VariableGroupItemView * variableGroupItemView = new VariableGroupItemView(); 

    variableGroupItemView->variableGroupHeaderView->label->setText(variableGroupItem->label());
    variableGroupItemView->variableGroupContentView->setListController(variableListController);
    variableGroupItemView->variableGroupContentView->setDelegate(variableItemDelegate);

    if( variableListController->count() == 0 )
    {
      variableGroupItemView->variableGroupHeaderView->hide();
    }

    return variableGroupItemView;
  }

  return new QWidget();
}

QWidget * VariableItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if( QSharedPointer<measuretab::VariableItem> variableItem = dataSource.objectCast<measuretab::VariableItem>() )
  {
    QSharedPointer<measuretab::MeasureListController> measureListController = variableItem->measureListController();
    QSharedPointer<MeasureItemDelegate> measureItemDelegate = QSharedPointer<MeasureItemDelegate>(new MeasureItemDelegate());

    VariableItemView * variableItemView = new VariableItemView(); 

    variableItemView->variableHeaderView->setText(variableItem->name());
    variableItemView->variableContentView->setListController(measureListController);
    variableItemView->variableContentView->setDelegate(measureItemDelegate);

    return variableItemView;
  }

  return new QWidget();
}

QWidget * MeasureItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<measuretab::MeasureItem> measureItem = dataSource.objectCast<measuretab::MeasureItem>())
  {
    MeasureItemView * measureItemView = new MeasureItemView();

    // Name

    measureItemView->label->setText(measureItem->name());

    bool bingo = connect(measureItem.data(),SIGNAL(nameChanged(const QString &)),measureItemView->label,SLOT(setText(const QString &)));

    OS_ASSERT(bingo);

    // Selection

    measureItemView->setHasEmphasis(measureItem->isSelected());

    bingo = connect(measureItemView,SIGNAL(clicked()),measureItem.data(),SLOT(toggleSelected()));

    OS_ASSERT(bingo);

    bingo = connect(measureItem.data(),SIGNAL(selectedChanged(bool)),measureItemView,SLOT(setHasEmphasis(bool)));

    OS_ASSERT(bingo);

    return measureItemView;
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
    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

    analysis::DataPoint datapoint = dataPoints()[i];
    QSharedPointer<DesignAltItem> item = QSharedPointer<DesignAltItem>(new DesignAltItem(datapoint, project && datapoint == project->baselineDataPoint()));

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
    Q_FOREACH(const analysis::DataPoint& dataPoint, project->analysis().dataPoints()){
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
         it++ )
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
         pertIter++ )
    {
      std::vector<analysis::Measure> newPointPerts;

      for( std::vector<analysis::MeasureGroup>::const_iterator varIter = measureGroups.begin();
           varIter != measureGroups.end();
           varIter++ )
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
         it++ )
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
        it++ )
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

DesignAltItem::DesignAltItem(const analysis::DataPoint & dataPoint, bool isBaseline)
  : OSListItem(),
    m_dataPoint(dataPoint),
    m_isBaseline(isBaseline)
{
  m_perturbationListController = QSharedPointer<PerturbationListController>(new PerturbationListController(this));
}

QString DesignAltItem::name() const
{
  return QString::fromStdString(m_dataPoint.name());
}

QString DesignAltItem::description() const
{
  return QString::fromStdString(m_dataPoint.description());
}

void DesignAltItem::setName(const QString & name)
{
  m_dataPoint.setName(name.toStdString());
}

void DesignAltItem::setDescription(const QString & description)
{
  m_dataPoint.setDescription(description.toStdString());
}

bool DesignAltItem::isBaseline() const
{
  return m_isBaseline;
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

QWidget * DesignAltItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  if(QSharedPointer<DesignAltItem> designAltItem = dataSource.objectCast<DesignAltItem>())
  {
    DesignAltItemView * designAltItemView = new DesignAltItemView(designAltItem->isBaseline());

    // Name

    designAltItemView->designAltHeaderView->designAltNameEdit->setText(designAltItem->name());

    bool bingo = connect(designAltItemView->designAltHeaderView->designAltNameEdit,SIGNAL(textEdited(const QString &)),designAltItem.data(),SLOT(setName(const QString &)));

    OS_ASSERT(bingo);

    // Description

    designAltItemView->designAltContentView->descriptionTextEdit->setText(designAltItem->description());

    bingo = connect(designAltItemView->designAltContentView,SIGNAL(descriptionChanged(const QString &)),designAltItem.data(),SLOT(setDescription(const QString &)));

    OS_ASSERT(bingo);

    // Remove

    bingo = connect(designAltItemView->designAltHeaderView->removeButton,SIGNAL(clicked()),designAltItem.data(),SLOT(remove()));

    OS_ASSERT(bingo);

    // Perturbations

    QSharedPointer<PerturbationListController> perturbationListController = designAltItem->perturbationListController();
    QSharedPointer<PerturbationItemDelegate> perturbationItemDelegate = QSharedPointer<PerturbationItemDelegate>(new PerturbationItemDelegate());

    designAltItemView->designAltContentView->perturbationListView->setListController(perturbationListController);
    designAltItemView->designAltContentView->perturbationListView->setDelegate(perturbationItemDelegate);

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

    for( std::vector<boost::optional<analysis::Measure> >::iterator it = measures.begin();
         it != measures.end();
         it++ )
    {
      if( *it && (! (*it)->optionalCast<analysis::NullMeasure>()) )
      {
        result.push_back(**it);
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
  return openstudio::toQString(m_measure.name());
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
    QLabel * perturbationItemView = new QLabel();

    perturbationItemView->setText(perturbationItem->name());

    if (perturbationItem->isFixedMeasureItem())
    {
      perturbationItemView->setStyleSheet("font:italic");
    }

    return perturbationItemView;
  }

  return new QWidget();
}

} // altstab

} // pat

} // openstudio


