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

#ifndef OPENSTUDIO_LOCATIONTABCONTROLLER_H
#define OPENSTUDIO_LOCATIONTABCONTROLLER_H

#include <model/Model.hpp>
#include <openstudio_lib/MainTabController.hpp>
#include <boost/smart_ptr.hpp>
#include <QDir>
#include <QObject>

class QStackedWidget;

namespace openstudio {

namespace model {
  //class myModelClass;
}

class LocationTabView;
class UtilityBillsController;

class LocationTabController : public MainTabController
{
  Q_OBJECT

public:
  LocationTabController(const model::Model& model,
                        const QString& modelTempDir);

  virtual ~LocationTabController() {}

  enum TabID
  {
    WEATHER_FILE,
    LIFE_CYCLE_COSTS,
    UTILITY_BILLS,
    UTILITY_RATES,
    GROUND_TEMPERATURE,
    WATER_MAINS_TEMPERATURE 
  };

private:
  boost::shared_ptr<UtilityBillsController> m_utilityBillsController;
  QStackedWidget * m_utilityBillsStackedWidget;
  int m_warningWidgetIndex;
  int m_visibleWidgetIndex;

private slots:
  void showSubTabView(bool showSubTabView);
 
};

} // openstudio

#endif // OPENSTUDIO_LOCATIONTABCONTROLLER_H
