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
    m_runView(new RunView(model, t_modelPath, t_tempFolder, t_runManager)),
    m_status(new openstudio::runmanager::JobStatusWidget(t_runManager)),
    m_model(model)
{
  mainContentWidget()->addSubTab("Output", OUTPUT);
  mainContentWidget()->addSubTab("Tree", TREE);

  setSubTab(0);
}

openstudio::RunView * RunTabController::runView(){

  // make sure non-null pointer
  OS_ASSERT(m_runView);

  return m_runView;
}

runmanager::RunManager RunTabController::runManager() {
  return m_runView->runManager();
}

void RunTabController::updateToolsWarnings()
{
  m_runView->updateToolsWarnings();
}

void RunTabController::setSubTab(int index)
{
  switch (index){
  case 0:
  {
    connect(m_runView, &RunView::resultsGenerated, this, &RunTabController::resultsGenerated);
    connect(m_runView, &RunView::toolsUpdated, this, &RunTabController::toolsUpdated);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &RunTabController::setSubTab);
    this->mainContentWidget()->setSubTab(m_runView);
    break;
  }
  case 1:
  {
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &RunTabController::setSubTab);
    this->mainContentWidget()->setSubTab(m_status);
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio

