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

#ifndef OPENSTUDIO_LOCATIONTABVIEW_HPP
#define OPENSTUDIO_LOCATIONTABVIEW_HPP

#include "../model/Model.hpp"
#include "MainTabView.hpp"
#include <QWidget>

class QDir;
class QComboBox;

namespace openstudio {

class EpwFile;

namespace model {
  class Model;
}

class LocationView : public QWidget
{
  Q_OBJECT

public:
  LocationView(const model::Model & model,
               const QString& modelTempDir);
  virtual ~LocationView() {}

private slots:
  void onWeatherFileBtnClicked();
  void onDesignDayBtnClicked();
  void onASHRAEClimateZoneChanged(const QString& climateZone);
  void onCECClimateZoneChanged(const QString& climateZone);

private:
  void update();

  model::Model m_model;
  QString m_modelTempDir;
  QComboBox * m_ashraeClimateZone;
  QComboBox * m_cecClimateZone;
  QLabel * m_weatherFileLbl;
  QLabel * m_designDaysLbl;
  QLabel * m_nameLbl;
  QLabel * m_latitudeLbl;
  QLabel * m_longitudeLbl;
  QLabel * m_elevationLbl;
  QLabel * m_timeZoneLbl;
  QLabel * m_numDesignDaysLbl;
  QString m_lastEpwPathOpened;
  QString m_lastDdyPathOpened;
};

class LocationTabView : public MainTabView
{
  Q_OBJECT

public:
  LocationTabView(const model::Model & model,
                  const QString& modelTempDir,
                  QWidget * parent = 0);
  virtual ~LocationTabView() {}

private:
  LocationView * m_locationView;
};

} // openstudio

#endif // OPENSTUDIO_LOCATIONTABVIEW_HPP
