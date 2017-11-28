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

#include "LocationTabView.hpp"

#include "DesignDayGridView.hpp"
#include "ModelObjectListView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"
#include "SchedulesTabController.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../openstudio_app/OpenStudioApp.hpp"

#include "../model/ClimateZones.hpp"
#include "../model/ClimateZones_Impl.hpp"
#include "../model/DesignDay.hpp"
#include "../model/DesignDay_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriodControlDaylightSavingTime.hpp"
#include "../model/RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/SizingPeriod.hpp"
#include "../model/SizingPeriod_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFileConditionType.hpp"
#include "../model/WeatherFileDays.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/YearDescription_Impl.hpp"

#include "../energyplus/ReverseTranslator.hpp"

//#include "../runmanager/lib/ConfigOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <boost/smart_ptr.hpp>

#include <QBoxLayout>
#include <QComboBox>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>

#define NAME "Name: "
#define LATITUDE "Latitude: "
#define LONGITUDE "Longitude: "
#define ELEVATION "Elevation: "
#define TIME_ZONE "Time Zone: "
#define NUM_DESIGN_DAYS "Number of Design Days: "
#define SETWEATHERFILE "Set Weather File"
#define CHANGEWEATHERFILE "Change Weather File"

namespace openstudio {

LocationTabView::LocationTabView(const model::Model & model,
  const QString& modelTempDir,
  QWidget * parent)
  : MainTabView("Site", MainTabView::SUB_TAB, parent)
{
}

LocationTabView::~LocationTabView()
{
}

LocationView::LocationView(bool isIP,
  const model::Model & model,
  const QString& modelTempDir)
  : QWidget(),
  m_model(model),
  m_site(m_model.getUniqueModelObject<model::Site>()),
  m_yearDescription(m_model.getUniqueModelObject<model::YearDescription>()),
  m_modelTempDir(modelTempDir),
  m_isIP(isIP)
{
  OS_ASSERT(m_site);

  loadQSettings();

  model::ClimateZones climateZones = m_model.getUniqueModelObject<model::ClimateZones>();

  // ***** Main Layout *****
  auto mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);
  setLayout(mainLayout);

  // ***** Scroll *****
  auto scrollLayout = new QVBoxLayout();
  scrollLayout->setContentsMargins(0, 0, 0, 0);
  scrollLayout->setSpacing(0);

  auto scrollWidget = new QWidget();
  scrollWidget->setObjectName("ScrollWidget");
  scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
  scrollWidget->setLayout(scrollLayout);

  auto scrollArea = new QScrollArea();
  scrollArea->setContentsMargins(0, 0, 0, 0);
  scrollArea->setFrameStyle(QFrame::NoFrame);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);
  scrollArea->setBackgroundRole(QPalette::NoRole);
  mainLayout->addWidget(scrollArea);

  // ***** Upper Horizontal Layout *****
  auto upperHorizontalLayout = new QHBoxLayout();
  upperHorizontalLayout->setContentsMargins(0, 0, 0, 0);
  upperHorizontalLayout->setSpacing(0);

  // ***** Left Vertical Layout *****
  auto leftVLayout = new QVBoxLayout();
  leftVLayout->setContentsMargins(7, 2, 7, 7);
  leftVLayout->setSpacing(0);

  // ***** Weather File GridLayout *****
  auto weatherFileGridLayout = new QGridLayout();
  weatherFileGridLayout->setContentsMargins(7, 3, 7, 7);
  weatherFileGridLayout->setSpacing(7);

  // ***** Measure Tags GridLayout *****
  auto measureTagsGridLayout = new QGridLayout();
  measureTagsGridLayout->setContentsMargins(7, 7, 7, 7);
  measureTagsGridLayout->setSpacing(7);

  // ***** Schedules Layout *****
  auto schedulesLayout = new QVBoxLayout();
  schedulesLayout->setContentsMargins(0, 0, 0, 0);
  schedulesLayout->setSpacing(0);

  // ***** Vertical Line *****
  auto vLine = new QFrame();
  vLine->setFrameShape(QFrame::VLine);
  vLine->setFrameShadow(QFrame::Sunken);

  // ***** Small Horizontal Line *****
  auto smallHLine = new QFrame();
  smallHLine->setFrameShape(QFrame::HLine);
  smallHLine->setFrameShadow(QFrame::Sunken);

  // ***** Main Horizontal Line *****
  auto mainHLine = new QFrame();
  mainHLine->setFrameShape(QFrame::HLine);
  mainHLine->setFrameShadow(QFrame::Sunken);

  // ***** Weather File *****
  auto label = new QLabel("Weather File");
  label->setObjectName("H2");

  m_weatherFileBtn = new QPushButton(this);
  m_weatherFileBtn->setFlat(true);
  m_weatherFileBtn->setObjectName("StandardGrayButton");
  connect(m_weatherFileBtn, &QPushButton::clicked, this, &LocationView::onWeatherFileBtnClicked);

  auto hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->setSpacing(7);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(m_weatherFileBtn, 0, Qt::AlignLeft);
  hLayout->addStretch();

  leftVLayout->addLayout(hLayout);

  // ***** Site Info *****
  int i = 0;

  label = new QLabel(NAME);

  m_siteName = new QLineEdit();
  connect(m_siteName, &QLineEdit::textEdited, this, &LocationView::onSiteNameChanged);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->setSpacing(7);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(m_siteName, 0, Qt::AlignLeft);
  hLayout->addStretch();

  weatherFileGridLayout->addLayout(hLayout, i++, 0);

  m_latitudeLbl = new QLabel(LATITUDE);
  weatherFileGridLayout->addWidget(m_latitudeLbl, i++, 0);

  m_longitudeLbl = new QLabel(LONGITUDE);
  weatherFileGridLayout->addWidget(m_longitudeLbl, i++, 0);

  m_elevationLbl = new QLabel(ELEVATION);
  weatherFileGridLayout->addWidget(m_elevationLbl, i++, 0);

  m_timeZoneLbl = new QLabel(TIME_ZONE);
  weatherFileGridLayout->addWidget(m_timeZoneLbl, i++, 0);

  // ***** Weather File Download Location *****
  label = new QLabel("Download weather files at <a href=\"http://www.energyplus.net/weather\">www.energyplus.net/weather</a>");
  label->setOpenExternalLinks(true);
  weatherFileGridLayout->addWidget(label, i++, 0);

  // ***** Add Weather File GridLayout *****
  weatherFileGridLayout->setColumnStretch(i, 10);
  leftVLayout->addLayout(weatherFileGridLayout);

  // ***** Climate Zones *****
  label = new QLabel("Measure Tags (Optional):");
  label->setObjectName("H2");
  leftVLayout->addWidget(label);

  label = new QLabel("ASHRAE Climate Zone");
  label->setObjectName("StandardsInfo");

  m_ashraeClimateZone = new QComboBox();
  m_ashraeClimateZone->setFixedWidth(200);

  i = 0;

  measureTagsGridLayout->addWidget(label, i, 0);
  measureTagsGridLayout->addWidget(m_ashraeClimateZone, i++, 1);

  m_ashraeClimateZone->addItem("");
  std::vector<std::string> ashraeClimateZoneValues = model::ClimateZones::validClimateZoneValues(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear());
  for (const std::string& climateZone : ashraeClimateZoneValues){
    m_ashraeClimateZone->addItem(toQString(climateZone));
  }

  model::ClimateZone ashraeClimateZone = climateZones.getClimateZone(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear());
  if (ashraeClimateZone.empty()){
    ashraeClimateZone = climateZones.appendClimateZone(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear(), "");
  }
  //ashraeClimateZone.setType(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDocumentName(), model::ClimateZones::ashraeDefaultYear());
  
  std::string ashraeClimateZoneValue = ashraeClimateZone.value();
  auto idx = m_ashraeClimateZone->findText(toQString(ashraeClimateZoneValue));
  OS_ASSERT(idx != -1);
  m_ashraeClimateZone->setCurrentIndex(idx);

  connect(m_ashraeClimateZone, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &LocationView::onASHRAEClimateZoneChanged);

  label = new QLabel("CEC Climate Zone");
  label->setObjectName("StandardsInfo");

  m_cecClimateZone = new QComboBox();
  m_cecClimateZone->setFixedWidth(200);

  measureTagsGridLayout->addWidget(label, i, 0);
  measureTagsGridLayout->addWidget(m_cecClimateZone, i++, 1);

  m_cecClimateZone->addItem("");
  std::vector<std::string> cecClimateZoneValues = model::ClimateZones::validClimateZoneValues(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear());
  for (const std::string& climateZone : cecClimateZoneValues){
    m_cecClimateZone->addItem(toQString(climateZone));
  }

  model::ClimateZone cecClimateZone = climateZones.getClimateZone(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear());
   if (cecClimateZone.empty()){
    cecClimateZone = climateZones.appendClimateZone(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear(), "");
  }
  //cecClimateZone.setType(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDocumentName(), model::ClimateZones::cecDefaultYear());

  std::string cecClimateZoneValue = cecClimateZone.value();
  idx = m_cecClimateZone->findText(toQString(cecClimateZoneValue));
  OS_ASSERT(idx != -1);
  m_cecClimateZone->setCurrentIndex(idx);

  connect(m_cecClimateZone, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &LocationView::onCECClimateZoneChanged);

  // ***** Measure Tags GridLayout *****
  measureTagsGridLayout->setColumnStretch(i, 10);
  leftVLayout->addLayout(measureTagsGridLayout);

  // ***** Schedules *****
  m_yearSettingsWidget = new YearSettingsWidget(m_model);
  schedulesLayout->addWidget(m_yearSettingsWidget);

  connect(m_yearSettingsWidget, &YearSettingsWidget::calendarYearSelected, this, &LocationView::setCalendarYear);

  connect(m_yearSettingsWidget, &YearSettingsWidget::firstDayofYearSelected, this, &LocationView::setFirstDayofYear);

  connect(m_yearSettingsWidget, &YearSettingsWidget::daylightSavingTimeClicked, this, &LocationView::setDaylightSavingsTime);

  connect(m_yearSettingsWidget, &YearSettingsWidget::dstStartDayOfWeekAndMonthChanged, this, &LocationView::setDstStartDayOfWeekAndMonth);

  connect(m_yearSettingsWidget, &YearSettingsWidget::dstStartDateChanged, this, &LocationView::setDstStartDate);

  connect(m_yearSettingsWidget, &YearSettingsWidget::dstEndDayOfWeekAndMonthChanged, this, &LocationView::setDstEndDayOfWeekAndMonth);

  connect(m_yearSettingsWidget, &YearSettingsWidget::dstEndDateChanged, this, &LocationView::setDstEndDate);

  // ***** Add Vertical Layout *****
  upperHorizontalLayout->addLayout(leftVLayout);

  // ***** Add Vertical Line *****
  upperHorizontalLayout->addWidget(vLine);

  // ***** Add Schedules Layout *****
  upperHorizontalLayout->addLayout(schedulesLayout);

  // ***** Add Upper Horizontal Layout *****
  scrollLayout->addLayout(upperHorizontalLayout);

  // ***** Add Main Horizontal Line *****
  scrollLayout->addWidget(mainHLine);
  
  // ***** Design Days *****
  label = new QLabel("Design Days");
  label->setObjectName("H2");

  auto btn = new QPushButton("Import From DDY", this);
  btn->setFlat(true);
  btn->setObjectName("StandardGrayButton");
  connect(btn, &QPushButton::clicked, this, &LocationView::onDesignDayBtnClicked);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(7, 7, 0, 7);
  hLayout->setSpacing(7);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(btn, 0, Qt::AlignLeft);
  hLayout->addStretch();

  scrollLayout->addLayout(hLayout);

  m_designDaysGridView = new DesignDayGridView(m_isIP, model, this->parentWidget());
  connect(this, &LocationView::toggleUnitsClicked, this, &LocationView::toggleUnits);
  connect(this, &LocationView::toggleUnitsClicked, m_designDaysGridView, &DesignDayGridView::toggleUnitsClicked);

  scrollLayout->addWidget(m_designDaysGridView, 1, Qt::AlignTop);

  // ***** Item Selector Buttons *****
  m_itemSelectorButtons = new OSItemSelectorButtons();
  m_itemSelectorButtons->hideDropZone();
  mainLayout->addWidget(m_itemSelectorButtons, 0, Qt::AlignBottom);

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::addClicked, m_designDaysGridView, &DesignDayGridView::onAddClicked);
  connect(m_itemSelectorButtons, &OSItemSelectorButtons::copyClicked, m_designDaysGridView, &DesignDayGridView::onCopyClicked);
  connect(m_itemSelectorButtons, &OSItemSelectorButtons::removeClicked, m_designDaysGridView, &DesignDayGridView::onRemoveClicked);
  connect(m_itemSelectorButtons, &OSItemSelectorButtons::purgeClicked, m_designDaysGridView, &DesignDayGridView::onPurgeClicked);

  update();

  onSelectItem();
}

LocationView::~LocationView()
{
  saveQSettings();
}

bool LocationView::calendarYearChecked() {
  if (m_yearSettingsWidget) {
    return m_yearSettingsWidget->calendarYearChecked();
  }
  else {
    return false;
  }
}

std::vector<model::ModelObject> LocationView::selectedObjects() const
{
  return m_designDaysGridView->selectedObjects();
}

void LocationView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{}

void LocationView::onUpdate()
{}

void LocationView::refresh()
{}

void LocationView::toggleUnits(bool isIP)
{
  m_isIP = isIP;
}

void LocationView::onSiteNameChanged(const QString & text)
{
  auto temp = m_site->setName(text.toStdString());
  if (!temp) {
    m_siteName->setText("");
  }
  else if (QString(temp.get().c_str()) != text) {
    m_siteName->setText(temp.get().c_str());
  }
}

void LocationView::loadQSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  m_lastEpwPathOpened = settings.value("m_lastEpwPathOpened").toString();
  m_lastDdyPathOpened = settings.value("m_lastDdyPathOpened").toString();
}

void LocationView::saveQSettings() const
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue("m_lastEpwPathOpened", m_lastEpwPathOpened);
  settings.setValue("m_lastDdyPathOpened", m_lastDdyPathOpened);  
}

void LocationView::update()
{
  boost::optional<model::WeatherFile> weatherFile = m_model.getOptionalUniqueModelObject<model::WeatherFile>();
  if (weatherFile) {

    auto fileExists = QFile(m_lastEpwPathOpened).exists();
    if (!fileExists) {

      boost::optional<openstudio::path> epwPath = weatherFile->path();
      if (epwPath) {
        // If there is a path, and a file at that path, our job is done
        fileExists = QFile(epwPath->string().c_str()).exists();
        if (!fileExists) {
          // Construct the absolute path as dictated by the osm location, and check for the file
          QString savePath, filePath;

          openstudio::OSAppBase * app = OSAppBase::instance();
          if (app && app->currentDocument()) {

            savePath = app->currentDocument()->savePath();

            if (savePath.contains(".osm")) {
              savePath.chop(4);
              if (epwPath) {
                filePath = savePath + "/files/";
                filePath += (epwPath.get().filename()).string().c_str();
              }
            }

            fileExists = QFile(filePath).exists();
          }
        }
      }
    }

    if (fileExists) {
      m_weatherFileBtn->setText(CHANGEWEATHERFILE);
      setSiteInfo();
    }
    else {
      m_weatherFileBtn->setText(SETWEATHERFILE);
      clearSiteInfo();
    }
  }
  else {
    m_weatherFileBtn->setText(SETWEATHERFILE);
    clearSiteInfo();
  }

}

void LocationView::setSiteInfo()
{
  QString info;
  QString temp;

  if (m_site->name() && !m_site->name()->empty()) {
    m_siteName->setText(m_site->name().get().c_str());
  }
  else {
    m_siteName->setText("");
  }

  info = LATITUDE;
  temp.setNum(m_site->latitude());
  info += temp;
  m_latitudeLbl->setText(info);

  info = LONGITUDE;
  temp.setNum(m_site->longitude());
  info += temp;
  m_longitudeLbl->setText(info);

  info = ELEVATION;
  temp.setNum(m_site->elevation());
  info += temp;
  m_elevationLbl->setText(info);

  info = TIME_ZONE;
  temp.setNum(m_site->timeZone());
  info += temp;
  m_timeZoneLbl->setText(info);
}

void LocationView::clearSiteInfo()
{
  m_siteName->setText("");

  m_latitudeLbl->setText(LATITUDE);

  m_longitudeLbl->setText(LONGITUDE);

  m_elevationLbl->setText(ELEVATION);

  m_timeZoneLbl->setText(TIME_ZONE);
}

openstudio::path resourcesPath() {
  if (applicationIsRunningFromBuildDirectory()) {
    return getApplicationSourceDirectory() / openstudio::toPath("src/openstudio_app/Resources");
  } else {
    return getApplicationDirectory() / openstudio::toPath("../Resources");
  }
}

// ***** SLOTS *****
void LocationView::onWeatherFileBtnClicked()
{
  QString fileTypes("Files (*.epw)");
  
  QString lastPath = QString::fromStdString(resourcesPath().string()+"/weather");
  //QString lastPath = m_lastEpwPathOpened;

  if (lastPath.isEmpty() && m_lastDdyPathOpened.isEmpty()){
    //openstudio::runmanager::ConfigOptions co(true);
    //lastPath = toQString(co.getDefaultEPWLocation().native());
  } else if (lastPath.isEmpty()) {
    QString path = m_lastDdyPathOpened;
    lastPath = path.replace(".ddy", ".epw");
  }

  QString fileName = QFileDialog::getOpenFileName(this,"Open EPW File",lastPath,fileTypes);
  if(!fileName.isEmpty()){
    
    openstudio::path epwPath = toPath(fileName);
    openstudio::path newPath = toPath(m_modelTempDir) / toPath("resources/files") / epwPath.filename();
    openstudio::path previousEPWPath;

    StringStreamLogSink ss;
    ss.setChannelRegex(boost::regex(".*EpwFile.*"));
    ss.setLogLevel(Error);

    try{
      
      boost::optional<openstudio::model::WeatherFile> weatherFile = m_model.getOptionalUniqueModelObject<model::WeatherFile>();
      
      if (weatherFile){
        boost::optional<openstudio::path> temp = weatherFile->path();
        if (temp){
          openstudio::path previousEPWName = temp->filename();
          //std::string ex = toString(previousEPWName.extension());
          if (!previousEPWName.empty() && previousEPWName.extension() == toPath(".epw")){
            previousEPWPath = toPath(m_modelTempDir) / toPath("resources/files") / previousEPWName;
          }
        }
      }
      
      // duplicate code in OSDocument::fixWeatherFilePath

      openstudio::filesystem::copy_file(epwPath, newPath, openstudio::filesystem::copy_option::overwrite_if_exists);
      
      // this can throw
      EpwFile epwFile(newPath);

      double totalDays = (epwFile.endDate() - epwFile.startDate()).totalDays() + 1;
      if (totalDays > 366){
        LOG_FREE(Error, "openstudio.EpwFile", "Cannot accept weather file with more than 366 days of data");
        throw openstudio::Exception("Cannot accept weather file with more than 366 days of data");
      }

      weatherFile = openstudio::model::WeatherFile::setWeatherFile(m_model, epwFile);
      OS_ASSERT(weatherFile);
      weatherFile->makeUrlRelative(toPath(m_modelTempDir) / toPath("resources/files"));

      m_model.workflowJSON().setWeatherFile(newPath.filename());

      if (!previousEPWPath.empty()){
        if (previousEPWPath.filename() != newPath.filename()){
          if (openstudio::filesystem::exists(previousEPWPath)){
            openstudio::filesystem::remove_all(previousEPWPath);
          }
        }
      }

      // set run period based on weather file
      openstudio::model::RunPeriod runPeriod = m_model.getUniqueModelObject<openstudio::model::RunPeriod>();
      runPeriod.setBeginMonth(epwFile.startDate().monthOfYear().value());
      runPeriod.setBeginDayOfMonth(epwFile.startDate().dayOfMonth());
      runPeriod.setEndMonth(epwFile.endDate().monthOfYear().value());
      runPeriod.setEndDayOfMonth(epwFile.endDate().dayOfMonth());

      // set the calendar year or start day of week
      openstudio::model::YearDescription yearDescription = m_model.getUniqueModelObject<openstudio::model::YearDescription>();
      boost::optional<int> startDateActualYear = epwFile.startDateActualYear();
      if (startDateActualYear){
        yearDescription.resetDayofWeekforStartDay();
        yearDescription.setCalendarYear(*startDateActualYear);
      } else{
        yearDescription.resetCalendarYear();
        yearDescription.setDayofWeekforStartDay(epwFile.startDayOfWeek().valueName());
      }

      // update site info
      m_site->setName(weatherFile->city().c_str());
      m_site->setLatitude(weatherFile->latitude());
      m_site->setLongitude(weatherFile->longitude());
      m_site->setElevation(weatherFile->elevation());
      m_site->setTimeZone(weatherFile->timeZone());

      m_lastEpwPathOpened = QFileInfo(fileName).absoluteFilePath();

      update();

    }catch(...){

      openstudio::filesystem::remove_all(newPath);

      QMessageBox box(QMessageBox::Warning, "Failed To Set Weather File", QString("Failed To Set Weather File To ") + fileName, QMessageBox::Ok);
      box.setDetailedText(toQString(ss.string())); 
      box.exec();

      boost::optional<model::WeatherFile> weatherFile = m_model.weatherFile();
      if (weatherFile){
        boost::optional<openstudio::path> weatherFilePath = weatherFile->path();
        if (weatherFilePath){
          if (!previousEPWPath.empty()){
            if (previousEPWPath.filename() != weatherFilePath->filename()){
              weatherFile->remove();
            }else if (!openstudio::filesystem::exists(previousEPWPath)){
              weatherFile->remove();
            }
          }
        }
      }

      update();
    }
  }
}

void LocationView::onDesignDayBtnClicked()
{
  QString fileTypes("Files (*.ddy)");

  QString lastPath = m_lastDdyPathOpened;
  if (lastPath.isEmpty() && m_lastEpwPathOpened.isEmpty()){
    //openstudio::runmanager::ConfigOptions co(true);
    //lastPath = toQString(co.getDefaultEPWLocation().native());
  } else if (lastPath.isEmpty()) {
    QString path = m_lastEpwPathOpened;
    lastPath = path.replace(".epw", ".ddy");
  }

  QString fileName = QFileDialog::getOpenFileName(this,"Open DDY File",lastPath,fileTypes);
  if(!fileName.isEmpty()){

    boost::optional<IdfFile> ddyIdfFile = openstudio::IdfFile::load(toPath(fileName));
    if(ddyIdfFile){

      openstudio::Workspace ddyWorkspace(StrictnessLevel::None, IddFileType::EnergyPlus);
      for (IdfObject idfObject : ddyIdfFile->objects()){
        IddObjectType iddObjectType = idfObject.iddObject().type();
        if((iddObjectType == IddObjectType::SizingPeriod_DesignDay) ||
           (iddObjectType == IddObjectType::SizingPeriod_WeatherFileDays) ||
           (iddObjectType == IddObjectType::SizingPeriod_WeatherFileConditionType)){
           
          ddyWorkspace.addObject(idfObject);
        }
      }

      energyplus::ReverseTranslator reverseTranslator;
      model::Model ddyModel = reverseTranslator.translateWorkspace(ddyWorkspace);
          
      // Use a heuristic based on the ddy files provided by EnergyPlus
      // Filter out the days that are not helpful.
      if (!ddyModel.objects().empty()){
        // Containers to hold 99%, 99.6%, 2%, 1%, and 0.4% design points
        std::vector<model::DesignDay> days99;
        std::vector<model::DesignDay> days99_6;
        std::vector<model::DesignDay> days2;
        std::vector<model::DesignDay> days1;
        std::vector<model::DesignDay> days0_4;

        bool unknownDay = false;

        for (model::DesignDay designDay : ddyModel.getConcreteModelObjects<model::DesignDay>()) {
          boost::optional<std::string> name;
          name = designDay.name();

          if( name ) 
          {
            QString qname = QString::fromStdString(name.get()); 

            if( qname.contains("99%") ) 
            {
              days99.push_back(designDay);
            } 
            else if( qname.contains("99.6%") )
            {
              days99_6.push_back(designDay);
            }
            else if( qname.contains("2%") )
            {
              days2.push_back(designDay);
            }
            else if( qname.contains("1%") )
            {
              days1.push_back(designDay);
            }
            else if( qname.contains(".4%") )
            {
              days0_4.push_back(designDay);
            }
            else
            {
              unknownDay = true;
            }
          }
          
        }

        // Pick only the most stringent design points
        if( ! unknownDay )
        {
          if( days99_6.size() > 0 )
          {
            for (model::DesignDay designDay : days99) {
              designDay.remove();
            }
          }

          if( days0_4.size() > 0 )
          {
            for (model::DesignDay designDay : days1) {
              designDay.remove();
            }
            for (model::DesignDay designDay : days2) {
              designDay.remove();
            }
          }
          else if( days1.size() > 0 )
          {
            for (model::DesignDay designDay : days2) {
              designDay.remove();
            }
          }
        }

        // Evan note: do not remove existing design days
        //for (model::SizingPeriod sizingPeriod : m_model.getModelObjects<model::SizingPeriod>()){
        //  sizingPeriod.remove();
        //}

        m_model.insertObjects(ddyModel.objects());

        m_lastDdyPathOpened = QFileInfo(fileName).absoluteFilePath();

      }
    }

    QTimer::singleShot(0, this, SLOT(checkNumDesignDays()));

  }
}

void LocationView::checkNumDesignDays()
{
  unsigned empty = m_model.getModelObjects<model::SizingPeriod>().empty();

  if (empty) {
    QMessageBox box(QMessageBox::Warning,
      "No Design Days in DDY File", 
      "This DDY file does not contain any valid design days.  Check the DDY file itself for errors or omissions.",
      QMessageBox::Ok);
    box.exec();  
  }
}

void LocationView::onASHRAEClimateZoneChanged(const QString& climateZone)
{
  model::ClimateZones climateZones = m_model.getUniqueModelObject<model::ClimateZones>();
    
  model::ClimateZone ashraeClimateZone = climateZones.getClimateZone(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear());
  ashraeClimateZone.setValue(toString(climateZone));
}

void LocationView::onCECClimateZoneChanged(const QString& climateZone)
{
  model::ClimateZones climateZones = m_model.getUniqueModelObject<model::ClimateZones>();
  model::ClimateZone cecClimateZone = climateZones.getClimateZone(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear());
  cecClimateZone.setValue(toString(climateZone));
}

void LocationView::setCalendarYear(int year)
{
  m_yearDescription->setCalendarYear(year);

  emit calendarYearSelectionChanged();
}

void LocationView::setFirstDayofYear(const QString & firstDayofYear)
{
  m_yearDescription->resetCalendarYear();

  m_yearDescription->setDayofWeekforStartDay(firstDayofYear.toStdString());

  emit calendarYearSelectionChanged();
}

void LocationView::setDaylightSavingsTime(bool enabled)
{
  if( enabled )
  {
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();
  }
  else
  {
    if( boost::optional<model::RunPeriodControlDaylightSavingTime> dst =
          m_model.getOptionalUniqueModelObject<model::RunPeriodControlDaylightSavingTime>() )
    {
      dst->remove();
    }
  }
}

void LocationView::setDstStartDayOfWeekAndMonth(int newWeek, int newDay, int newMonth)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setStartDate(NthDayOfWeekInMonth(newWeek), DayOfWeek(newDay), MonthOfYear(newMonth));
}

void LocationView::setDstStartDate(const QDate & newdate)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setStartDate(monthOfYear(newdate.month()),newdate.day());
}

void LocationView::setDstEndDayOfWeekAndMonth(int newWeek, int newDay, int newMonth)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setEndDate(NthDayOfWeekInMonth(newWeek), DayOfWeek(newDay), MonthOfYear(newMonth));
}

void LocationView::setDstEndDate(const QDate & newdate)
{
  model::RunPeriodControlDaylightSavingTime dst =
    m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  dst.setEndDate(monthOfYear(newdate.month()),newdate.day());
}

void LocationView::onSelectItem()
{
  m_itemSelectorButtons->enableCopyButton();
  m_itemSelectorButtons->enableRemoveButton();
  m_itemSelectorButtons->enablePurgeButton();
}

void LocationView::onClearSelection()
{
  m_itemSelectorButtons->disableCopyButton();
  m_itemSelectorButtons->disableRemoveButton();
  m_itemSelectorButtons->disablePurgeButton();
}

} // openstudio
