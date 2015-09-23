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
  : MainTabController(new LocationTabView(model,modelTempDir)),
  m_model(model)
{
  m_locationView = new LocationView(isIP, model, modelTempDir);
  mainContentWidget()->addSubTab("Weather File && Design Days", m_locationView, WEATHER_FILE);
  connect(this, &LocationTabController::toggleUnitsClicked, m_locationView, &LocationView::toggleUnitsClicked);
  connect(m_locationView, &LocationView::calendarYearSelectionChanged, this, &LocationTabController::showUtilityBillSubTab);

  auto lifeCycleCostsView = new LifeCycleCostsView(model);
  mainContentWidget()->addSubTab("Life Cycle Costs",lifeCycleCostsView,LIFE_CYCLE_COSTS);

  QLabel * label;

  label = new QLabel();
  label->setPixmap(QPixmap(":/images/utility_calibration_warning.png"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  m_utilityBillsController = std::shared_ptr<UtilityBillsController>(new UtilityBillsController(model));

  m_utilityBillsStackedWidget = new QStackedWidget();
  m_warningWidgetIndex = m_utilityBillsStackedWidget->addWidget(label);
  m_visibleWidgetIndex = m_utilityBillsStackedWidget->addWidget(m_utilityBillsController->subTabView()); 

  mainContentWidget()->addSubTab("Utility Bills",m_utilityBillsStackedWidget,UTILITY_BILLS);

  showUtilityBillSubTab();

  // Hack code to remove when tab active
  label = new QLabel();
  label->setPixmap(QPixmap(":/images/coming_soon_utility_rates.png"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //mainContentWidget()->addSubTab("Utility Rates",label,UTILITY_RATES); This is too slow in coming, so hide until ready

  // Hack code to remove when tab active
  //label = new QLabel();
  //label->setPixmap(QPixmap(":/images/coming_soon_ground_temperature.png"));
  //label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //mainContentWidget()->addSubTab("Ground Temperature",label,GROUND_TEMPERATURE);

  // Hack code to remove when tab active
  //label = new QLabel();
  //label->setPixmap(QPixmap(":/images/coming_soon_water_mains_temperature.png"));
  //label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //mainContentWidget()->addSubTab("Water Mains Temperature",label,WATER_MAINS_TEMPERATURE);
}

void LocationTabController::showUtilityBillSubTab()
{
  // Determine if the utility bill sub-tab is shown
  boost::optional<model::YearDescription> yearDescription = m_model.yearDescription();
  if (yearDescription){
    boost::optional<int> calendarYear = yearDescription.get().calendarYear();
    if (calendarYearChecked() && calendarYear){
      boost::optional<model::WeatherFile> weatherFile = m_model.weatherFile();
      if (weatherFile){
        boost::optional<model::RunPeriod> runPeriod = m_model.getOptionalUniqueModelObject<model::RunPeriod>();
        if (runPeriod.is_initialized()){
          m_utilityBillsStackedWidget->setCurrentIndex(m_visibleWidgetIndex);
          return;
        }
        else {
          m_utilityBillsStackedWidget->setCurrentIndex(m_warningWidgetIndex);
          return;
        }
      }
    }
  }
  // Oops, missing some needed object above, so default to warning
  m_utilityBillsStackedWidget->setCurrentIndex(m_warningWidgetIndex);
}

bool LocationTabController::calendarYearChecked() {
  if (m_locationView) {
    return m_locationView->calendarYearChecked();
  }
  else {
    return false;
  }
}

void LocationTabController::showSubTabView(bool showSubTabView)
{
  if(showSubTabView){
    m_utilityBillsStackedWidget->setCurrentIndex(m_visibleWidgetIndex);
  } else {
    m_utilityBillsStackedWidget->setCurrentIndex(m_warningWidgetIndex);
  }
}

} // openstudio
