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

  if (qobject_cast<LocationView *>(m_currentView)) {
    disconnect(this, &LocationTabController::toggleUnitsClicked, qobject_cast<LocationView *>(m_currentView), &LocationView::toggleUnitsClicked);
  }
  else if (qobject_cast<LifeCycleCostsView *>(m_currentView)) {
  }
  else if (qobject_cast<UtilityBillsInspectorView *>(m_currentView) || qobject_cast<QLabel *>(m_currentView)) {
  }
  else if (m_currentView) {
    // Oops! Should never get here
    OS_ASSERT(false);
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
