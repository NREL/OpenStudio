/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <openstudio_lib/LocationTabController.hpp>

#include <openstudio_lib/LifeCycleCostsTabView.hpp>
#include <openstudio_lib/LocationTabView.hpp>
#include <openstudio_lib/UtilityBillsView.hpp>
#include <openstudio_lib/UtilityBillsController.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

LocationTabController::LocationTabController(const model::Model & model,
                                             const QString& modelTempDir)
  : MainTabController(new LocationTabView(model,modelTempDir))
{
  LocationView * locationView = new LocationView(model, modelTempDir);
  mainContentWidget()->addSubTab("Weather File && Design Days",locationView,WEATHER_FILE);

  LifeCycleCostsView * lifeCycleCostsView = new LifeCycleCostsView(model);
  mainContentWidget()->addSubTab("Life Cycle Costs",lifeCycleCostsView,LIFE_CYCLE_COSTS);

  QLabel * label;
    
  label = new QLabel();
  label->setPixmap(QPixmap(":/images/utility_calibration_warning.png"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  m_utilityBillsController = boost::shared_ptr<UtilityBillsController>(new UtilityBillsController(model));

  m_utilityBillsStackedWidget = new QStackedWidget();
  m_warningWidgetIndex = m_utilityBillsStackedWidget->addWidget(label);
  m_visibleWidgetIndex = m_utilityBillsStackedWidget->addWidget(m_utilityBillsController->subTabView()); 

  mainContentWidget()->addSubTab("Utility Bills",m_utilityBillsStackedWidget,UTILITY_BILLS);

  m_utilityBillsStackedWidget->setCurrentIndex(m_warningWidgetIndex);

  bool isConnected = false;

  isConnected = connect(m_utilityBillsController->subTabView()->inspectorView(),SIGNAL(showSubTabView(bool)),
    this,SLOT(showSubTabView(bool)));
  OS_ASSERT(isConnected);

  // Hack code to remove when tab active
  label = new QLabel();
  label->setPixmap(QPixmap(":/images/coming_soon_utility_rates.png"));
  label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  mainContentWidget()->addSubTab("Utility Rates",label,UTILITY_RATES);

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

void LocationTabController::showSubTabView(bool showSubTabView)
{
  if(showSubTabView){
    m_utilityBillsStackedWidget->setCurrentIndex(m_visibleWidgetIndex);
  } else {
    m_utilityBillsStackedWidget->setCurrentIndex(m_warningWidgetIndex);
  }
}

} // openstudio
