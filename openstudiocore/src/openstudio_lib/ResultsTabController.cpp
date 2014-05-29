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

#include "ResultsTabController.hpp"
#include "ResultsTabView.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

namespace openstudio {

ResultsTabController::ResultsTabController()
  : MainTabController(new ResultsTabView("Results Summary",false))
{
  m_resultsTabView = static_cast<ResultsTabView *>(mainContentWidget());

  connect(m_resultsTabView,SIGNAL(removeResultClicked()),
          this,SLOT(removeResult()));

  connect(m_resultsTabView,SIGNAL(importResultClicked()),this,SLOT(importResult()));
}

//void ResultsController::removeResult(model::myModelClass & result)
void ResultsTabController::removeResult()
{
  //result.remove();
}
  
void ResultsTabController::importResult()
{
}

void ResultsTabController::resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceOutputFile)
{
  LOG(Debug, "resultsGenerated " << openstudio::toString(t_sqlFile) << " " << openstudio::toString(t_radianceOutputFile));
  m_resultsTabView->resultsGenerated(t_sqlFile, t_radianceOutputFile);
}

void ResultsTabController::searchForExistingResults(const openstudio::path &t_runDir)
{
  LOG(Debug, "searchForExistingResults " << openstudio::toString(t_runDir));
  m_resultsTabView->searchForExistingResults(t_runDir);
}

void ResultsTabController::onUnitSystemChange(bool t_isIP)
{
  LOG(Debug, "onUnitSystemChange isIP " << t_isIP);
  m_resultsTabView->onUnitSystemChange(t_isIP);
}

} // openstudio

