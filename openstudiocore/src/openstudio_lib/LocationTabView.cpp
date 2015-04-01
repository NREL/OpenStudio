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

#include "LocationTabView.hpp"

#include "DesignDayGridView.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"
#include "SchedulesTabController.hpp"
#include "YearSettingsWidget.hpp"

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
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/SizingPeriod.hpp"
#include "../model/SizingPeriod_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFileConditionType.hpp"
#include "../model/WeatherFileDays.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/YearDescription_Impl.hpp"

#include "../energyplus/ReverseTranslator.hpp"

#include "../runmanager/lib/ConfigOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/idf/IdfFile.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <boost/smart_ptr.hpp>

#include <QBoxLayout>
#include <QComboBox>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>

#define NAME "Name: "
#define LATITUDE "Latitude: "
#define LONGITUDE "Longitude: "
#define ELEVATION "Elevation: "
#define TIME_ZONE "Time Zone: "
#define NUM_DESIGN_DAYS "Number of Design Days: "

namespace openstudio {

LocationTabView::LocationTabView(const model::Model & model,
  const QString& modelTempDir,
  QWidget * parent)
  : MainTabView("Site",true,parent)
{
}

LocationView::LocationView(bool isIP,
  const model::Model & model,
  const QString& modelTempDir)
  : QWidget(),
  m_isIP(isIP),
  m_model(model),
  m_modelTempDir(modelTempDir)
{
  QFrame * line = nullptr;
  QLabel * label = nullptr;
  QPushButton * btn = nullptr;
  QHBoxLayout * hLayout = nullptr;
  QVBoxLayout * vLayout = nullptr;

  m_yearDescription = m_model.getUniqueModelObject<model::YearDescription>();

  model::ClimateZones climateZones = m_model.getUniqueModelObject<model::ClimateZones>();

  // ***** Main Layout *****
  auto mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(10, 10, 10, 10);
  mainLayout->setSpacing(10);
  setLayout(mainLayout);




  QVBoxLayout * scrollLayout = new QVBoxLayout();
  scrollLayout->setSpacing(0);
  scrollLayout->setContentsMargins(0, 0, 0, 0);

  QWidget * scrollWidget = new QWidget();
  scrollWidget->setObjectName("ScrollWidget");
  scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
  scrollWidget->setLayout(scrollLayout);

  QScrollArea * scrollArea = new QScrollArea();
  scrollArea->setContentsMargins(0, 0, 0, 0);
  scrollArea->setFrameStyle(QFrame::NoFrame);
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);
  scrollArea->setBackgroundRole(QPalette::NoRole);
  mainLayout->addWidget(scrollArea);





  // ***** Upper Horizontal Layout *****
  auto upperHorizontalLayout = new QHBoxLayout();
  upperHorizontalLayout->setContentsMargins(10, 0, 10, 0);
  upperHorizontalLayout->setSpacing(0);

  // ***** Weather File Layout *****
  auto weatherFileLayout = new QVBoxLayout();
  weatherFileLayout->setContentsMargins(10, 0, 10, 0);
  weatherFileLayout->setSpacing(0);

  // ***** Schedules Layout *****
  auto schedulesLayout = new QVBoxLayout();
  schedulesLayout->setContentsMargins(10, 0, 10, 0);
  schedulesLayout->setSpacing(0);

  // ***** Upper Horizontal Layout Vertical Line *****
  line = new QFrame();
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Sunken);

  scrollLayout->addLayout(upperHorizontalLayout);
  upperHorizontalLayout->addLayout(weatherFileLayout);
  upperHorizontalLayout->addWidget(line);
  upperHorizontalLayout->addLayout(schedulesLayout);

  // ***** Weather File *****
  label = new QLabel("Weather File");
  label->setObjectName("H2");

  btn = new QPushButton("Set Weather File", this);
  btn->setFlat(true);
  btn->setObjectName("StandardGrayButton");
  connect(btn, &QPushButton::clicked, this, &LocationView::onWeatherFileBtnClicked);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 5, 0, 5);
  hLayout->setSpacing(5);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(btn, 0, Qt::AlignLeft);
  hLayout->addStretch();

  weatherFileLayout->addLayout(hLayout);

  // ***** Site Info *****
  m_nameLbl = new QLabel(NAME);
  weatherFileLayout->addWidget(m_nameLbl);

  m_latitudeLbl = new QLabel(LATITUDE);
  weatherFileLayout->addWidget(m_latitudeLbl);

  m_longitudeLbl = new QLabel(LONGITUDE);
  weatherFileLayout->addWidget(m_longitudeLbl);

  m_elevationLbl = new QLabel(ELEVATION);
  weatherFileLayout->addWidget(m_elevationLbl);

  m_timeZoneLbl = new QLabel(TIME_ZONE);
  weatherFileLayout->addWidget(m_timeZoneLbl);

  m_numDesignDaysLbl = new QLabel(NUM_DESIGN_DAYS);
  weatherFileLayout->addWidget(m_numDesignDaysLbl);

  // ***** Weather File Download Location *****
  label = new QLabel("Download weather files at <a href=\"http://www.energyplus.gov\">www.energyplus.gov</a>");
  label->setOpenExternalLinks(true);
  weatherFileLayout->addWidget(label);

  // ***** Climate Zones *****
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  weatherFileLayout->addWidget(line);

  label = new QLabel("Measure Tags (Optional):");
  label->setObjectName("H2");
  weatherFileLayout->addWidget(label);

  label = new QLabel("ASHRAE Climate Zone");
  label->setObjectName("StandardsInfo");

  m_ashraeClimateZone = new QComboBox();
  m_ashraeClimateZone->setFixedWidth(200);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 5, 0, 5);
  hLayout->setSpacing(5);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(m_ashraeClimateZone, 0, Qt::AlignLeft);
  //hLayout->addStretch();

  weatherFileLayout->addLayout(hLayout);

  m_ashraeClimateZone->addItem("");
  std::vector<std::string> ashraeClimateZoneValues = model::ClimateZones::validClimateZoneValues(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear());
  for (const std::string& climateZone : ashraeClimateZoneValues){
    m_ashraeClimateZone->addItem(toQString(climateZone));
  }

  model::ClimateZone ashraeClimateZone = climateZones.getClimateZone(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear());
  if (ashraeClimateZone.empty()){
    ashraeClimateZone = climateZones.appendClimateZone(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDefaultYear(), "");
  }
  ashraeClimateZone.setType(model::ClimateZones::ashraeInstitutionName(), model::ClimateZones::ashraeDocumentName(), model::ClimateZones::ashraeDefaultYear());
  
  std::string ashraeClimateZoneValue = ashraeClimateZone.value();
  int i = m_ashraeClimateZone->findText(toQString(ashraeClimateZoneValue));
  OS_ASSERT(i != -1);
  m_ashraeClimateZone->setCurrentIndex(i);

  connect(m_ashraeClimateZone, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &LocationView::onASHRAEClimateZoneChanged);

  label = new QLabel("CEC Climate Zone");
  label->setObjectName("StandardsInfo");

  m_cecClimateZone = new QComboBox();
  m_cecClimateZone->setFixedWidth(200);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 5, 0, 5);
  hLayout->setSpacing(5);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(m_cecClimateZone, 0, Qt::AlignLeft);
  //hLayout->addStretch();

  weatherFileLayout->addLayout(hLayout);

  m_cecClimateZone->addItem("");
  std::vector<std::string> cecClimateZoneValues = model::ClimateZones::validClimateZoneValues(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear());
  for (const std::string& climateZone : cecClimateZoneValues){
    m_cecClimateZone->addItem(toQString(climateZone));
  }

  model::ClimateZone cecClimateZone = climateZones.getClimateZone(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear());
   if (cecClimateZone.empty()){
    cecClimateZone = climateZones.appendClimateZone(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDefaultYear(), "");
  }
  cecClimateZone.setType(model::ClimateZones::cecInstitutionName(), model::ClimateZones::cecDocumentName(), model::ClimateZones::cecDefaultYear());

  std::string cecClimateZoneValue = cecClimateZone.value();
  i = m_cecClimateZone->findText(toQString(cecClimateZoneValue));
  OS_ASSERT(i != -1);
  m_cecClimateZone->setCurrentIndex(i);

  connect(m_cecClimateZone, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &LocationView::onCECClimateZoneChanged);

  // ***** Schedules *****
  auto yearSettingsWidget = new YearSettingsWidget(m_model);
  schedulesLayout->addWidget(yearSettingsWidget);

  connect(yearSettingsWidget, &YearSettingsWidget::calendarYearSelected, this, &LocationView::setCalendarYear);

  connect(yearSettingsWidget, &YearSettingsWidget::firstDayofYearSelected, this, &LocationView::setFirstDayofYear);

  connect(yearSettingsWidget, &YearSettingsWidget::daylightSavingTimeClicked, this, &LocationView::setDaylightSavingsTime);

  connect(yearSettingsWidget, &YearSettingsWidget::dstStartDayOfWeekAndMonthChanged, this, &LocationView::setDstStartDayOfWeekAndMonth);

  connect(yearSettingsWidget, &YearSettingsWidget::dstStartDateChanged, this, &LocationView::setDstStartDate);

  connect(yearSettingsWidget, &YearSettingsWidget::dstEndDayOfWeekAndMonthChanged, this, &LocationView::setDstEndDayOfWeekAndMonth);

  connect(yearSettingsWidget, &YearSettingsWidget::dstEndDateChanged, this, &LocationView::setDstEndDate);

  // ***** Upper Horizontal Layout Vertical Line *****
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);

  scrollLayout->addWidget(line);

  // ***** Design Days *****
  label = new QLabel("Design Days");
  label->setObjectName("H2");

  btn = new QPushButton("Import From DDY", this);
  btn->setFlat(true);
  btn->setObjectName("StandardGrayButton");
  connect(btn, &QPushButton::clicked, this, &LocationView::onDesignDayBtnClicked);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 5, 0, 5);
  hLayout->setSpacing(5);

  hLayout->addWidget(label, 0, Qt::AlignLeft);
  hLayout->addWidget(btn, 0, Qt::AlignLeft);
  hLayout->addStretch();

  scrollLayout->addLayout(hLayout);

  auto designDays = model.getModelObjects<model::DesignDay>();
  auto designDayModelObjects = subsetCastVector<model::ModelObject>(designDays);
  auto designDayGridController = new DesignDayGridController(m_isIP, "Design Days", IddObjectType::OS_SpaceType, model, designDayModelObjects);
  auto gridView = new OSGridView(designDayGridController, "Design Days", "Drop\nDesign Days", false, this->parentWidget());

  gridView->m_dropZone->hide();

  scrollLayout->addWidget(gridView);

  update();
}

void LocationView::update()
{
  QString info;
  QString temp;

  boost::optional<model::WeatherFile> weatherFile = m_model.getOptionalUniqueModelObject<model::WeatherFile>();
  if (weatherFile){

    boost::optional<openstudio::path> epwPath = weatherFile->path();
    if (epwPath){
      // Do great things
    }

    model::Site site = m_model.getUniqueModelObject<model::Site>();
    if (weatherFile->name()) {
      site.setName(weatherFile->name().get());
    }
    site.setLatitude(weatherFile->latitude());
    site.setLongitude(weatherFile->longitude());
    site.setElevation(weatherFile->elevation());
    site.setTimeZone(weatherFile->timeZone());
  } 

  boost::optional<model::Site> site = m_model.getOptionalUniqueModelObject<model::Site>();
  if (site){

    info = NAME;
    if (site->name()) {
      info += site->name().get().c_str();
    }
    m_nameLbl->setText(info);

    info = LATITUDE;
    temp.setNum(site->latitude());
    info += temp;
    m_latitudeLbl->setText(info);

    info = LONGITUDE;
    temp.setNum(site->longitude());
    info += temp;
    m_longitudeLbl->setText(info);

    info = ELEVATION;
    temp.setNum(site->elevation());
    info += temp;
    m_elevationLbl->setText(info);

    info = TIME_ZONE;
    temp.setNum(site->timeZone());
    info += temp;
    m_timeZoneLbl->setText(info);
  }

  unsigned numDesignDays = m_model.getModelObjects<model::SizingPeriod>().size();
  info = NUM_DESIGN_DAYS;
  temp.setNum(numDesignDays);
  info += temp;
  m_numDesignDaysLbl->setText(info);
}

// ***** SLOTS *****
void LocationView::onWeatherFileBtnClicked()
{
  QString fileTypes("Files (*.epw)");

  QString lastPath = m_lastEpwPathOpened;
  if (lastPath.isEmpty() && m_lastDdyPathOpened.isEmpty()){
    openstudio::runmanager::ConfigOptions co(true);
    lastPath = toQString(co.getDefaultEPWLocation().native());
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
      
      boost::filesystem::copy_file(epwPath, newPath, boost::filesystem::copy_option::overwrite_if_exists);
      
      // this can throw
      EpwFile epwFile(newPath);

      double totalDays = (epwFile.endDate() - epwFile.startDate()).totalDays() + 1;
      if (totalDays > 366){
        LOG_FREE(Error, "openstudio.EpwFile", "Cannot accept weather file with more than 366 days of data");
        throw openstudio::Exception("Cannot accept weather file with more than 366 days of data");
      }

      weatherFile = openstudio::model::WeatherFile::setWeatherFile(m_model, epwFile);
      OS_ASSERT(weatherFile);
      weatherFile->makeUrlRelative(toPath(m_modelTempDir) / toPath("resources"));

      if (!previousEPWPath.empty()){
        if (previousEPWPath.filename() != newPath.filename()){
          if (boost::filesystem::exists(previousEPWPath)){
            boost::filesystem::remove_all(previousEPWPath);
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
      }else{
        yearDescription.resetCalendarYear();
        yearDescription.setDayofWeekforStartDay(epwFile.startDayOfWeek().valueName());
      }

      m_lastEpwPathOpened = QFileInfo(fileName).absoluteFilePath();

      update();

    }catch(...){

      boost::filesystem::remove_all(newPath);

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
            }else if (!boost::filesystem::exists(previousEPWPath)){
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
    openstudio::runmanager::ConfigOptions co(true);
    lastPath = toQString(co.getDefaultEPWLocation().native());
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

        for (model::SizingPeriod sizingPeriod : m_model.getModelObjects<model::SizingPeriod>()){
          sizingPeriod.remove();
        }

        m_model.insertObjects(ddyModel.objects());

        m_lastDdyPathOpened = QFileInfo(fileName).absoluteFilePath();

        update();
      }
    }
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
}

void LocationView::setFirstDayofYear(const QString & firstDayofYear)
{
  m_yearDescription->resetCalendarYear();

  m_yearDescription->setDayofWeekforStartDay(firstDayofYear.toStdString());
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

} // openstudio
