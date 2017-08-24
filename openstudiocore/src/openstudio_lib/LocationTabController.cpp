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

#include "LocationTabController.hpp"

#include "LifeCycleCostsTabView.hpp"
#include "LocationTabView.hpp"
#include "UtilityBillsView.hpp"
#include "UtilityBillsController.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"

#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

LocationTabController::LocationTabController(bool isIP,
  const model::Model & model,
  const QString& modelTempDir)
  : MainTabController(new LocationTabView(model, modelTempDir)),
  m_modelTempDir(modelTempDir),
  m_model(model),
  m_isIP(isIP)
{
  mainContentWidget()->addSubTab("Weather File && Design Days", WEATHER_FILE); 
  mainContentWidget()->addSubTab("Life Cycle Costs", LIFE_CYCLE_COSTS);
  mainContentWidget()->addSubTab("Utility Bills", UTILITY_BILLS);

  setSubTab(0);
  connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &LocationTabController::setSubTab);
}

LocationTabController::~LocationTabController()
{
  disconnect(this->mainContentWidget(), &MainTabView::tabSelected, this, &LocationTabController::setSubTab);
}

bool LocationTabController::showUtilityBills()
{
  // Determine if the utility bill sub-tab is shown
  boost::optional<model::YearDescription> yearDescription = m_model.yearDescription();
  if (yearDescription){
    boost::optional<int> calendarYear = yearDescription.get().calendarYear();
    if (calendarYear){
      boost::optional<model::WeatherFile> weatherFile = m_model.weatherFile();
      if (weatherFile){
        boost::optional<model::RunPeriod> runPeriod = m_model.getOptionalUniqueModelObject<model::RunPeriod>();
        if (runPeriod.is_initialized()){
          return true;
        }
        else {
          return false;
        }
      }
    }
  }
  // Oops, missing some needed object above, so default to warning
  return false;
}

void LocationTabController::setSubTab(int index)
{
  if (m_currentIndex == index) {
    return;
  }
  else {
    m_currentIndex = index;
  }

  if (m_currentView) {
    m_currentView->disconnect();
    delete m_currentView;
  }

  switch (index){
  case 0:
  {
    auto locationView = new LocationView(m_isIP, m_model, m_modelTempDir);
    connect(this, &LocationTabController::toggleUnitsClicked, locationView, &LocationView::toggleUnitsClicked);
    this->mainContentWidget()->setSubTab(locationView);
    m_currentView = locationView;
    break;
  }
  case 1:
  {
    auto lifeCycleCostsView = new LifeCycleCostsView(m_model);
    this->mainContentWidget()->setSubTab(lifeCycleCostsView);
    m_currentView = lifeCycleCostsView;
    break;
  }
  case 2:
  {
    if (showUtilityBills()) {
      auto utilityBillsController = new UtilityBillsController(m_model);
      this->mainContentWidget()->setSubTab(utilityBillsController->subTabView());
      m_currentView = utilityBillsController->subTabView()->inspectorView();
    }
    else {
      auto label = new QLabel();
      label->setPixmap(QPixmap(":/images/utility_calibration_warning.png"));
      label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      this->mainContentWidget()->setSubTab(label);
      m_currentView = label;
    }
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio
