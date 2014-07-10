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

#include "MeasuresTabController.hpp"
#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/EditController.hpp"
#include "HorizontalTabWidget.hpp"
#include "MainRightColumnController.hpp"
#include "../shared_gui_components/MeasureDragData.hpp"
#include "MeasuresView.hpp"
#include "PatApp.hpp"
#include "PatMainWindow.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/DiscreteVariable.hpp"
#include "../analysis/DiscreteVariable_Impl.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/NullMeasure.hpp"
#include "../analysis/NullMeasure_Impl.hpp"
#include "../analysis/Problem.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../analysis/RubyMeasure_Impl.hpp"
#include "../analysis/WorkflowStep.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/RubyException.hpp"

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

namespace pat {


MeasuresTabController::MeasuresTabController()
  : QObject()
{
  measuresTabView = new MeasuresTabView();

  m_variableGroupListController = QSharedPointer<openstudio::measuretab::VariableGroupListController>(new openstudio::measuretab::VariableGroupListController(false, PatApp::instance()));
  m_variableGroupItemDelegate = QSharedPointer<openstudio::measuretab::VariableGroupItemDelegate>(new openstudio::measuretab::VariableGroupItemDelegate(false));

  measuresTabView->variableGroupListView->setListController(m_variableGroupListController);
  measuresTabView->variableGroupListView->setDelegate(m_variableGroupItemDelegate);

  bool bingo = connect(measuresTabView->selectBaselineButton,SIGNAL(clicked()),this,SLOT(selectBaseline()));
  OS_ASSERT(bingo);

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){
    // ensure that baseline exists
    analysis::DataPoint baselineDataPoint = project->baselineDataPoint();

    FileReference seed = project->seed();
    if (seed.displayName().empty() || seed.displayName() == "*"){
      measuresTabView->baselineCautionLabel->setHidden(false);
      measuresTabView->baselineNameBackground->setHidden(true);
      measuresTabView->baselineLabel->setText(QString());
    }else{
      measuresTabView->baselineCautionLabel->setHidden(true);
      measuresTabView->baselineNameBackground->setHidden(false);
      measuresTabView->baselineLabel->setText(toQString(seed.displayName()));
    }
  }
}

MeasuresTabController::~MeasuresTabController()
{
  if( measuresTabView ) { delete measuresTabView; }
}

void MeasuresTabController::refreshAllViews()
{
  measuresTabView->variableGroupListView->refreshAllViews();
}

void MeasuresTabController::selectBaseline()
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (!project){
    return;
  }

  if (project->isRunning()){
    // warn user that they will have to stop running to change baseline model
    QMessageBox::StandardButton test = QMessageBox::question( measuresTabView, "Stop Current Analysis?", "You must stop the currently running analysis before changing the baseline model, do you want to proceed?", QMessageBox::Yes |  QMessageBox::No, QMessageBox::No );
    if (test == QMessageBox::No){
      return;
    }

    project->stop();
  }

  if (project->analysis().completeDataPoints().size() > 0u){
    // warn user that this will blow away their data points
    QMessageBox::StandardButton test = QMessageBox::question( 
        measuresTabView, 
        "Change Baseline Model?", 
        "Changing your baseline model will remove all results and save your project, do you want to proceed?", 
        QMessageBox::Yes |  QMessageBox::No, 
        QMessageBox::No );
    if (test == QMessageBox::No){
      return;
    }
  }

  // DLM: todo, allow user to clear baseline?

  QString fileName = QFileDialog::getOpenFileName( measuresTabView,
                                                   tr("Select Baseline Model"),
                                                   QDir::homePath(),
                                                   tr("(*.osm)") );

  if (!fileName.isEmpty()){
    QDir projectDir(toQString(project->projectDir()));
    if (projectDir.relativeFilePath(fileName).startsWith("seed")){
      QMessageBox::warning( measuresTabView, "Changing Baseline Model Failed", QString("The new baseline model cannot be located within the project's seed directory."));
      return;
    }
    FileReference seed(toPath(fileName));
    // DLM: make sure to call setSeed on the real project, not a copy
    if (PatApp::instance()->setSeed(seed)){

      if (seed.displayName().empty() || seed.displayName() == "*"){
        measuresTabView->baselineCautionLabel->setHidden(false);
        measuresTabView->baselineNameBackground->setHidden(true);
        measuresTabView->baselineLabel->setText(QString());
      }else{
        measuresTabView->baselineCautionLabel->setHidden(true);
        measuresTabView->baselineNameBackground->setHidden(false);
        measuresTabView->baselineLabel->setText(toQString(seed.displayName()));
      }

      // change cannot be undone, save the project
      PatApp::instance()->project()->save();

      //QMessageBox::information( measuresTabView, "Baseline Model Changed", QString("Baseline model has been changed, please run a baseline simulation with this new baseline model before continuing on with new work.") );

      m_variableGroupListController->selectionController()->unselectAllItems();

    }else{
      // tell user that this failed
      QMessageBox::critical( measuresTabView, "Changing Baseline Model Failed", QString("Failed to change baseline model to file at '") + fileName + QString("'."));
    }
  }
}


} // pat

} // openstudio

