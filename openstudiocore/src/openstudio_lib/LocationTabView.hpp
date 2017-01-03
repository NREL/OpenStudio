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

  void update();

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

  virtual ~LocationTabView();

private:

  LocationView * m_locationView;

};

} // openstudio

#endif // OPENSTUDIO_LOCATIONTABVIEW_HPP
