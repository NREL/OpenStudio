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

#include "ScriptsTabController.hpp"

#include "OSAppBase.hpp"
#include "ScriptsTabView.hpp"

#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/WorkflowController.hpp"
#include "../shared_gui_components/WorkflowView.hpp"

#include "../model/Model.hpp"

namespace openstudio {

ScriptsTabController::ScriptsTabController()
  : MainTabController(new ScriptsTabView(nullptr)), scriptsTabView(dynamic_cast<ScriptsTabView *>(mainContentWidget()))
{
  auto app = OSAppBase::instance();

  // save the current osm to a temp location
  app->measureManager().saveTempModel();

  // update measures
  app->measureManager().updateMeasuresLists();

  m_workflowController = QSharedPointer<openstudio::measuretab::WorkflowController>(new openstudio::measuretab::WorkflowController(OSAppBase::instance()));
  m_workflowSectionItemDelegate = QSharedPointer<openstudio::measuretab::WorkflowSectionItemDelegate>(new openstudio::measuretab::WorkflowSectionItemDelegate());

  scriptsTabView->workflowView->setListController(m_workflowController);
  scriptsTabView->workflowView->setDelegate(m_workflowSectionItemDelegate);
}

ScriptsTabController::~ScriptsTabController()
{
  if( scriptsTabView ) { delete scriptsTabView; }
}

} // openstudio
