/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../shared_gui_components/SyncMeasuresDialog.hpp"

#include "../shared_gui_components/Component.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/SyncMeasuresDialogCentralWidget.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Assert.hpp"

#include <vector>

#include <QBoxLayout>
#include <QPainter>
#include <QScrollArea>
#include <QSplitter>
#include <QStyleOption>
#include <QProgressBar>


namespace openstudio {

SyncMeasuresDialog::SyncMeasuresDialog(const WorkflowJSON& workflow,
  MeasureManager * measureManager,
  QWidget * parent)
: QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
  m_centralWidget(nullptr),
  m_rightScrollArea(nullptr),
  m_expandedComponent(nullptr),
  m_measuresNeedingUpdates(std::vector<BCLMeasure>()),
  m_workflow(workflow),
  m_measureManager(measureManager)
{
  createLayout();
  findUpdates();
}

void SyncMeasuresDialog::createLayout()
{
  setWindowTitle("Updates Available in Library");

  setModal(true);

  setSizeGripEnabled(true);

  setObjectName("BlueGradientWidget");

  // The central pane
  m_centralWidget = new SyncMeasuresDialogCentralWidget(m_workflow, m_measureManager);

  connect(m_centralWidget, &SyncMeasuresDialogCentralWidget::componentClicked, this, &SyncMeasuresDialog::on_componentClicked);

  connect(m_centralWidget, &SyncMeasuresDialogCentralWidget::noComponents, this, &SyncMeasuresDialog::on_noComponents);

  connect(m_centralWidget, &SyncMeasuresDialogCentralWidget::closeDlg, this, &SyncMeasuresDialog::closeDlg);

  auto centralScrollArea = new QScrollArea(this);
  centralScrollArea->setFrameStyle(QFrame::NoFrame);
  centralScrollArea->setObjectName("GrayWidget");
  centralScrollArea->setWidgetResizable(true);
  centralScrollArea->setWidget(m_centralWidget);

  // The right pane

  m_rightScrollArea = new QScrollArea(this);
  m_rightScrollArea->setFrameStyle(QFrame::NoFrame);
  m_rightScrollArea->setObjectName("GrayWidget");
  m_rightScrollArea->setWidgetResizable(true);

  auto splitter = new QSplitter(this);
  splitter->setOrientation(Qt::Horizontal);
  splitter->addWidget(centralScrollArea);
  splitter->addWidget(m_rightScrollArea);

  auto mainLayout = new QHBoxLayout();
  mainLayout->addWidget(splitter);

  setLayout(mainLayout);

  m_centralWidget->lowerPushButton->setFocus();

}

void SyncMeasuresDialog::findUpdates()
{
  // this will update the xmls
  m_measureManager->updateMeasuresLists();
  
  // DLM: measure manager will filter out duplicate measures for us
  std::vector<BCLMeasure> measures = m_measureManager->combinedMeasures();

  // DLM: should we sort these in any way?

  m_measuresNeedingUpdates.clear();

  m_centralWidget->progressBar->setVisible(true);
  m_centralWidget->progressBar->setStatusTip("Checking for updates");
  m_centralWidget->progressBar->setMinimum(0);
  m_centralWidget->progressBar->setMaximum(measures.size());
  
  int progressValue = 0;
  for (auto itr = measures.begin();
      itr != measures.end();
      ++itr)
  {
    m_centralWidget->progressBar->setValue(progressValue);

    boost::optional<BCLMeasure> workflowMeasure = m_workflow.getBCLMeasureByUUID(itr->uuid());
    if (workflowMeasure)
    {
      std::string version1 = toString(workflowMeasure->versionUUID());
      std::string version2 = toString(itr->versionUUID());
      if (workflowMeasure->versionUUID() != itr->versionUUID())
      {
        m_measuresNeedingUpdates.push_back(*itr);
      }
    }

    ++progressValue;
  }

  m_centralWidget->progressBar->setVisible(false);
  m_centralWidget->progressBar->reset();
  m_centralWidget->progressBar->setStatusTip("");

  // DLM: if m_measuresNeedingUpdates is empty should we do something else?  
  // just say "No updates available" and quit?

  m_centralWidget->setMeasures(m_measuresNeedingUpdates);

}

void SyncMeasuresDialog::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

///! Slots

void SyncMeasuresDialog::on_componentClicked(bool checked)
{
  if(m_expandedComponent){
    delete m_expandedComponent;
    m_expandedComponent = nullptr;
  }
  m_expandedComponent = new Component(*m_centralWidget->checkedComponent());
  m_expandedComponent->setCheckable(false);
  m_rightScrollArea->setWidget(m_expandedComponent);
}

void SyncMeasuresDialog::on_noComponents()
{
  m_expandedComponent = new Component();
  m_expandedComponent->setCheckable(false);
  m_rightScrollArea->setWidget(m_expandedComponent);
}

void SyncMeasuresDialog::closeDlg()
{
  QDialog::close();
}

} // namespace openstudio

