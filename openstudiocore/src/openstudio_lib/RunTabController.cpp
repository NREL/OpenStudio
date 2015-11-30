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

#include "RunTabController.hpp"

#include "RunTabView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../runmanager/lib/JobStatusWidget.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

RunTabController::RunTabController(const model::Model & model, const openstudio::path &t_modelPath,
    const openstudio::path &t_tempFolder, openstudio::runmanager::RunManager t_runManager)
  : MainTabController(new RunTabView(model)),
    m_model(model),
    m_modelPath(t_modelPath),
    m_tempFolder(t_tempFolder),
    m_runManager(t_runManager)
{
  mainContentWidget()->addSubTab("Output", OUTPUT);
  mainContentWidget()->addSubTab("Tree", TREE);

  connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &RunTabController::setSubTab);
}

RunTabController::~RunTabController()
{
  disconnect(this->mainContentWidget(), &MainTabView::tabSelected, this, &RunTabController::setSubTab);
}

void RunTabController::setSubTab(int index)
{
  if (m_currentIndex == index) {
    return;
  }
  else {
    m_currentIndex = index;
  }

  if (qobject_cast<RunView *>(m_currentView)) {
    disconnect(qobject_cast<RunView *>(m_currentView), &RunView::resultsGenerated, this, &RunTabController::resultsGenerated);
    disconnect(qobject_cast<RunView *>(m_currentView), &RunView::toolsUpdated, this, &RunTabController::toolsUpdated);
  }
  else if (qobject_cast< openstudio::runmanager::JobStatusWidget *>(m_currentView)) {
  }
  else if (m_currentView) {
    // Oops! Should never get here
    OS_ASSERT(false);
  }

  switch (index){
  case 0:
  {
    auto runView = new RunView(m_model, m_modelPath, m_tempFolder, m_runManager);
    connect(runView, &RunView::resultsGenerated, this, &RunTabController::resultsGenerated);
    connect(runView, &RunView::toolsUpdated, this, &RunTabController::toolsUpdated);
    this->mainContentWidget()->setSubTab(runView);
    m_currentView = runView;
    break;
  }
  case 1:
  {
    auto status = new openstudio::runmanager::JobStatusWidget(m_runManager);
    this->mainContentWidget()->setSubTab(status);
    m_currentView = status;
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio

