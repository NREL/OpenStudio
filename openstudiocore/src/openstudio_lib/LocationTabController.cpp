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

#include "LocationTabController.hpp"

#include "LifeCycleCostsTabView.hpp"
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
  m_model(model),
  m_isIP(isIP),
  m_modelTempDir(modelTempDir)
{
  mainContentWidget()->addSubTab("Weather File && Design Days", WEATHER_FILE); 
  mainContentWidget()->addSubTab("Life Cycle Costs", LIFE_CYCLE_COSTS);
  mainContentWidget()->addSubTab("Utility Bills", UTILITY_BILLS);

  setSubTab(0);
}

LocationTabController::~LocationTabController()
{
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
  switch (index){
  case 0:
  {
    auto locationView = new LocationView(m_isIP, m_model, m_modelTempDir);
    connect(this, &LocationTabController::toggleUnitsClicked, locationView, &LocationView::toggleUnitsClicked);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &LocationTabController::setSubTab);
    this->mainContentWidget()->setSubTab(locationView);
    break;
  }
  case 1:
  {
    auto lifeCycleCostsView = new LifeCycleCostsView(m_model);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &LocationTabController::setSubTab);
    this->mainContentWidget()->setSubTab(lifeCycleCostsView);
    break;
  }
  case 2:
  {
    {
      if (showUtilityBills()) {
        auto utilityBillsController = new UtilityBillsController(m_model);
        connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &LocationTabController::setSubTab);
        this->mainContentWidget()->setSubTab(utilityBillsController->subTabView());
      }
      else {
        auto label = new QLabel();
        label->setPixmap(QPixmap(":/images/utility_calibration_warning.png"));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        this->mainContentWidget()->setSubTab(label);
      }
    }
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio
