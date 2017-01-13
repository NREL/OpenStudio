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

#include "ScriptsTabController.hpp"

#include "OSAppBase.hpp"
#include "OSDocument.hpp"
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
  boost::optional<path> tempDir = app->tempDir();
  OS_ASSERT(tempDir);

  // save the current osm to a temp location
  app->measureManager().saveTempModel(*tempDir);

  // update measures
  app->currentDocument()->disable();
  app->measureManager().updateMeasuresLists();
  app->currentDocument()->enable();

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
