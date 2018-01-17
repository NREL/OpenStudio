/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  connect(measuresTabView->selectBaselineButton, &QPushButton::clicked, this, &MeasuresTabController::selectBaseline);

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

