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

#include "RunTabController.hpp"
#include "RunTabView.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
//#include "../runmanager/lib/JobStatusWidget.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

RunTabController::RunTabController(const model::Model & model, const openstudio::path &t_modelPath,
    const openstudio::path &t_tempFolder)//, openstudio::runmanager::RunManager t_runManager)
  : MainTabController(new RunTabView(model))//,
    //m_runView(new RunView(model, t_modelPath, t_tempFolder, t_runManager)),
    //m_status(new openstudio::runmanager::JobStatusWidget(t_runManager))
{
  //mainContentWidget()->addSubTab("Output", m_runView,OUTPUT);
  //mainContentWidget()->addSubTab("Tree", m_status,TREE);

  //connect(m_runView, &RunView::resultsGenerated, this, &RunTabController::resultsGenerated);

  //connect(m_runView, &RunView::toolsUpdated, this, &RunTabController::toolsUpdated);

  //connect(m_runView, &RunView::useRadianceStateChanged, this, &RunTabController::useRadianceStateChanged);
}

//openstudio::RunView * RunTabController::runView(){
//
//  // make sure non-null pointer
//  OS_ASSERT(m_runView);
//
//  return m_runView;
//}

//runmanager::RunManager RunTabController::runManager() {
//  return m_runView->runManager();
//}

void RunTabController::updateToolsWarnings()
{
  //m_runView->updateToolsWarnings();
}

} // openstudio

