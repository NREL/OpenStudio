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

#include "../shared_gui_components/SyncMeasuresDialog.hpp"

#include "../shared_gui_components/Component.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/SyncMeasuresDialogCentralWidget.hpp"

#include <analysisdriver/SimpleProject.hpp>

#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/Assert.hpp>

#include <vector>

#include <QBoxLayout>
#include <QPainter>
#include <QScrollArea>
#include <QSplitter>
#include <QStyleOption>

namespace openstudio {

SyncMeasuresDialog::SyncMeasuresDialog(analysisdriver::SimpleProject * project,
  MeasureManager * measureManager,
  QWidget * parent)
: QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
  m_centralWidget(NULL),
  m_rightScrollArea(NULL),
  m_expandedComponent(NULL),
  m_measuresNeedingUpdates(std::vector<BCLMeasure>()),
  m_project(project),
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
  m_centralWidget = new SyncMeasuresDialogCentralWidget(m_project,m_measureManager);

  bool isConnected = false;

  isConnected = connect(m_centralWidget, SIGNAL(componentClicked(bool)), this, SLOT(on_componentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(noComponents()), this, SLOT(on_noComponents()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(closeDlg()), this, SLOT(closeDlg()));
  OS_ASSERT(isConnected);

  QScrollArea * centralScrollArea = new QScrollArea(this);
  centralScrollArea->setFrameStyle(QFrame::NoFrame);
  centralScrollArea->setObjectName("GrayWidget");
  centralScrollArea->setWidgetResizable(true);
  centralScrollArea->setWidget(m_centralWidget);

  // The right pane

  m_rightScrollArea = new QScrollArea(this);
  m_rightScrollArea->setFrameStyle(QFrame::NoFrame);
  m_rightScrollArea->setObjectName("GrayWidget");
  m_rightScrollArea->setWidgetResizable(true);

  QSplitter * splitter = new QSplitter(this);
  splitter->setOrientation(Qt::Horizontal);
  splitter->addWidget(centralScrollArea);
  splitter->addWidget(m_rightScrollArea);

  QHBoxLayout * mainLayout = new QHBoxLayout();
  mainLayout->addWidget(splitter);

  setLayout(mainLayout);

  m_centralWidget->lowerPushButton->setFocus();

}

void SyncMeasuresDialog::findUpdates()
{
  // DLM: measure manager will filter out duplicate measures for us
  m_measureManager->updateMeasuresLists();
  std::vector<BCLMeasure> measures = m_measureManager->combinedMeasures();

  // DLM: should we sort these in any way?

  m_measuresNeedingUpdates.clear();

  for (std::vector<BCLMeasure>::iterator itr = measures.begin();
      itr != measures.end();
      ++itr)
  {
    bool isNewVersion = itr->checkForUpdates();
    if (isNewVersion) {
      itr->save();
    }

    boost::optional<BCLMeasure> projectmeasure = m_project->getMeasureByUUID(itr->uuid());
    if (projectmeasure)
    {
      if (projectmeasure->versionUUID() != itr->versionUUID())
      {
        m_measuresNeedingUpdates.push_back(*itr);
      }
    }
  }

  // DLM: if m_measuresNeedingUpdates is empty should we do something else?  
  // just say "No updates available and quit"

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
    m_expandedComponent = NULL;
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

