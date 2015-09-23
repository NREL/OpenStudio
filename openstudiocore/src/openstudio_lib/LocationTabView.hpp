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

#ifndef OPENSTUDIO_LOCATIONTABVIEW_HPP
#define OPENSTUDIO_LOCATIONTABVIEW_HPP

#include "../model/Model.hpp"
#include "../model/Site.hpp"
#include "../model/YearDescription.hpp"

#include "MainTabView.hpp"
#include "YearSettingsWidget.hpp"

#include <QWidget>

class QComboBox;
class QDir;
class QLineEdit;

namespace openstudio {

class EpwFile;
class DesignDayGridView;
class OSItemSelectorButtons;

namespace model {
  class Model;
  class Site;
  class YearDescription;
}

class LocationView : public QWidget
{

  Q_OBJECT

public:

  LocationView(bool isIP,
    const model::Model & model,
    const QString& modelTempDir);

  virtual ~LocationView();

  virtual bool supportsMultipleObjectSelection() const { return true; }

  virtual std::vector<model::ModelObject> selectedObjects() const;

  bool calendarYearChecked();

protected:

  void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

  void onUpdate();

private:

  void update(bool weatherFileBtnClicked = false);

  void loadQSettings();

  void saveQSettings() const;

  void setSiteInfo();

  void clearSiteInfo();

  model::Model m_model;
  boost::optional<model::Site> m_site;
  boost::optional<model::YearDescription> m_yearDescription;
  YearSettingsWidget * m_yearSettingsWidget = nullptr;
  DesignDayGridView * m_designDaysGridView = nullptr;
  OSItemSelectorButtons * m_itemSelectorButtons = nullptr;
  QString m_modelTempDir = QString();
  QString m_lastEpwPathOpened = QString();
  QString m_lastDdyPathOpened = QString();
  QComboBox * m_ashraeClimateZone = nullptr;
  QComboBox * m_cecClimateZone = nullptr;
  QLineEdit * m_siteName = nullptr;
  QLabel * m_latitudeLbl = nullptr;
  QLabel * m_longitudeLbl = nullptr;
  QLabel * m_elevationLbl = nullptr;
  QLabel * m_timeZoneLbl = nullptr;
  QPushButton * m_weatherFileBtn = nullptr;
  bool m_isIP;

signals:

  void calendarYearSelectionChanged();

  void modelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

  void toggleUnitsClicked(bool displayIP);

public slots:

  void refresh();

private slots:

  void onSelectItem();

  void onClearSelection();

  void toggleUnits(bool);

  void onSiteNameChanged(const QString & text);

  void checkNumDesignDays();

  void setCalendarYear(int year);

  void setFirstDayofYear(const QString & firstDayofYear);

  void setDaylightSavingsTime(bool enabled);

  void setDstStartDayOfWeekAndMonth(int newWeek, int newDay, int newMonth);

  void setDstStartDate(const QDate & newdate);

  void setDstEndDayOfWeekAndMonth(int newWeek, int newDay, int newMonth);

  void setDstEndDate(const QDate & newdate);

  void onWeatherFileBtnClicked();

  void onDesignDayBtnClicked();

  void onASHRAEClimateZoneChanged(const QString& climateZone);

  void onCECClimateZoneChanged(const QString& climateZone);

};

class LocationTabView : public MainTabView
{
  Q_OBJECT

public:

  LocationTabView(const model::Model & model,
                  const QString& modelTempDir,
                  QWidget * parent = nullptr);
  virtual ~LocationTabView() {}

private:

  LocationView * m_locationView;

};

} // openstudio

#endif // OPENSTUDIO_LOCATIONTABVIEW_HPP
