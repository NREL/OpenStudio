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

#include "OSDocument.hpp"

#include "../openstudio_app/OpenStudioApp.hpp"

#include "../model/DesignDay.hpp"
#include "../model/DesignDay_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/SizingPeriod.hpp"
#include "../model/SizingPeriod_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/ClimateZones.hpp"
#include "../model/ClimateZones_Impl.hpp"
#include "../model/WeatherFileDays.hpp"
#include "../model/WeatherFileConditionType.hpp"
#include "../model/YearDescription.hpp"
#include "../model/YearDescription_Impl.hpp"

#include "../energyplus/ReverseTranslator.hpp"

#include "../runmanager/lib/ConfigOptions.hpp"

#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <boost/smart_ptr.hpp>

#include <QBoxLayout>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QMessageBox>
#include <QComboBox>

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

LocationView::LocationView(const model::Model & model,
                           const QString& modelTempDir)
  : QWidget(),
    m_model(model),
    m_modelTempDir(modelTempDir)
{
  QLabel * label = 0;
  QPushButton * btn = 0;
  QHBoxLayout * hLayout = 0;
  QVBoxLayout * vLayout = 0;
  QFont boldFont;

  model::ClimateZones climateZones = m_model.getUniqueModelObject<model::ClimateZones>();

  // ***** Main Layout *****
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(10,10,10,10);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  // ***** Climate Zones *****
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line);

  label = new QLabel("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainVLayout->addWidget(label);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0,5,0,5);
  hLayout->setSpacing(5);

  vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(10,0,10,0);
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel("ASHRAE Climate Zone");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_ashraeClimateZone = new QComboBox();
  m_ashraeClimateZone->setFixedWidth(200);
  vLayout->addWidget(m_ashraeClimateZone);

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

  vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(10,0,10,0);
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel("CEC Climate Zone");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_cecClimateZone = new QComboBox();
  m_cecClimateZone->setFixedWidth(200);
  vLayout->addWidget(m_cecClimateZone);

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

  hLayout->addStretch();
  mainVLayout->addLayout(hLayout);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(line);

  // ***** Weather File *****
  label = new QLabel("Weather File");
  label->setObjectName("H2");
  mainVLayout->addWidget(label);

  m_weatherFileLbl = new QLabel();
  m_weatherFileLbl->setStyleSheet("border: 1px solid black");
  m_weatherFileLbl->setFixedWidth(400);

  btn = new QPushButton("Browse",this);
  btn->setFlat(true);
  btn->setObjectName("StandardGrayButton");
  connect(btn, &QPushButton::clicked, this, &LocationView::onWeatherFileBtnClicked);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0,5,0,5);
  hLayout->setSpacing(5);
  hLayout->addWidget(m_weatherFileLbl,0,Qt::AlignLeft);
  hLayout->addWidget(btn,0,Qt::AlignLeft);
  hLayout->addStretch();

  vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(10,0,10,0);
  vLayout->setSpacing(0);

  label = new QLabel("EPW File Path");
  vLayout->addWidget(label);

  vLayout->addLayout(hLayout);

  label = new QLabel("Download weather files at <a href=\"http://www.energyplus.gov\">www.energyplus.gov</a>");
  label->setOpenExternalLinks(true);

  vLayout->addWidget(label);

  mainVLayout->addLayout(vLayout);
  mainVLayout->addSpacing(10);

  // Design Days
  label = new QLabel("Design Days");
  label->setObjectName("H2");
  mainVLayout->addWidget(label);

  m_designDaysLbl = new QLabel();
  m_designDaysLbl->setStyleSheet("border: 1px solid black");
  m_designDaysLbl->setFixedWidth(400);

  btn = new QPushButton("Browse",this);
  btn->setFlat(true);
  btn->setObjectName("StandardGrayButton");
  connect(btn, &QPushButton::clicked, this, &LocationView::onDesignDayBtnClicked);

  hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0,5,0,5);
  hLayout->setSpacing(5);
  hLayout->addWidget(m_designDaysLbl,0,Qt::AlignLeft);
  hLayout->addWidget(btn,0,Qt::AlignLeft);
  hLayout->addStretch();

  vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(10,0,10,0);
  vLayout->setSpacing(0);

  label = new QLabel("DDY File Path");
  vLayout->addWidget(label);

  vLayout->addLayout(hLayout);

  mainVLayout->addLayout(vLayout);
  mainVLayout->addSpacing(10);

  // Location
  label = new QLabel("Location");
  boldFont = label->font();
  boldFont.setBold(true);
  label->setFont(boldFont);
  mainVLayout->addWidget(label);

  vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(10,0,10,0);

  m_nameLbl = new QLabel(NAME);
  vLayout->addWidget(m_nameLbl);

  m_latitudeLbl = new QLabel(LATITUDE);
  vLayout->addWidget(m_latitudeLbl);

  m_longitudeLbl = new QLabel(LONGITUDE);
  vLayout->addWidget(m_longitudeLbl);

  m_elevationLbl = new QLabel(ELEVATION);
  vLayout->addWidget(m_elevationLbl);

  m_timeZoneLbl = new QLabel(TIME_ZONE);
  vLayout->addWidget(m_timeZoneLbl);

  m_numDesignDaysLbl = new QLabel(NUM_DESIGN_DAYS);
  vLayout->addWidget(m_numDesignDaysLbl);

  mainVLayout->addLayout(vLayout);
  mainVLayout->addStretch();

  update();
}

void LocationView::update()
{
  QString info;
  QString temp;

  boost::optional<model::WeatherFile> weatherFile = m_model.getOptionalUniqueModelObject<model::WeatherFile>();
  if (weatherFile){

    info = NAME;
    info += weatherFile->city().c_str();
    m_nameLbl->setText(info);

    boost::optional<openstudio::path> epwPath = weatherFile->path();
    if (epwPath){
      m_weatherFileLbl->setText(toQString(*epwPath));
    }

    model::Site site = m_model.getUniqueModelObject<model::Site>();
    site.setLatitude(weatherFile->latitude());
    site.setLongitude(weatherFile->longitude());
    site.setElevation(weatherFile->elevation());
    site.setTimeZone(weatherFile->timeZone());
  } else {
    m_nameLbl->setText("");
    m_weatherFileLbl->setText("");
  } 

  boost::optional<model::Site> site = m_model.getOptionalUniqueModelObject<model::Site>();
  if (site){

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
  } else {
    m_latitudeLbl->setText("");
    m_longitudeLbl->setText("");
    m_elevationLbl->setText("");
    m_timeZoneLbl->setText("");
  } 

  m_designDaysLbl->setText(m_lastDdyPathOpened);

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

} // openstudio
