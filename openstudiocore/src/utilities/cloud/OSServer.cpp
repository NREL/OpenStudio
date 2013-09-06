/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
#include <utilities/cloud/OSServer.hpp>
#include <utilities/cloud/OSServer_Impl.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/System.hpp>
#include <utilities/core/Json.hpp>
#include <utilities/core/Assert.hpp>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QMutex>
#include <QFile>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace openstudio{
  namespace detail{

    OSServer_Impl::OSServer_Impl(const QUrl& url)
      : QObject(), m_url(url), m_networkAccessManager(new QNetworkAccessManager()),
        m_networkReply(0), m_mutex(new QMutex()),
        m_lastAvailable(false),
        m_lastProjectUUIDs(), 
        m_lastCreateProjectSuccess(false),
        m_lastDeleteProjectSuccess(false),
        m_lastAnalysisUUIDs(),
        m_lastPostAnalysisJSONSuccess(false),
        m_lastPostDataPointJSONSuccess(false),
        m_lastUploadAnalysisFilesSuccess(false),
        m_lastStartSuccess(false),
        m_lastIsAnalysisQueued(false),
        m_lastIsAnalysisRunning(false),
        m_lastStopSuccess(false),
        m_lastDataPointUUIDs(),
        m_lastRunningDataPointUUIDs(),
        m_lastQueuedDataPointUUIDs(),
        m_lastCompleteDataPointUUIDs(),
        m_lastDataPointJSON(),
        m_lastDownloadDataPointSuccess(false),
        m_errors(),
        m_warnings()
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();
    }

    OSServer_Impl::~OSServer_Impl()
    {
      if (m_networkReply){
        m_networkReply->blockSignals(true);
        m_networkReply->deleteLater();
        m_networkReply = 0;
      };
    }
 
    bool OSServer_Impl::available(int msec)
    {
      if (requestAvailable()){
        waitForFinished(msec);
      }
      return lastAvailable();
    }

    bool OSServer_Impl::lastAvailable() const
    {
      return m_lastAvailable;
    }

    std::vector<UUID> OSServer_Impl::projectUUIDs(int msec)
    {
      if (requestProjectUUIDs()){
        waitForFinished(msec);
      }
      return lastProjectUUIDs();
    }

    std::vector<UUID> OSServer_Impl::lastProjectUUIDs() const
    {
      return m_lastProjectUUIDs;
    }

    bool OSServer_Impl::createProject(const UUID& projectUUID, int msec) 
    {
      if (requestCreateProject(projectUUID)){
        waitForFinished(msec);
      }
      return lastCreateProjectSuccess();
    }

    bool OSServer_Impl::lastCreateProjectSuccess() const
    {
      return m_lastCreateProjectSuccess;
    }

    bool OSServer_Impl::deleteProject(const UUID& projectUUID, int msec) 
    {
      if (requestDeleteProject(projectUUID)){
        waitForFinished(msec);
      }
      return lastDeleteProjectSuccess();
    }

    bool OSServer_Impl::lastDeleteProjectSuccess() const
    {
      return m_lastDeleteProjectSuccess;
    }

    std::vector<UUID> OSServer_Impl::analysisUUIDs(const UUID& projectUUID, int msec)
    {
      if (requestAnalysisUUIDs(projectUUID)){
        waitForFinished(msec);
      }
      return lastAnalysisUUIDs();
    }

    std::vector<UUID> OSServer_Impl::lastAnalysisUUIDs() const
    {
      return m_lastAnalysisUUIDs;
    } 

    bool OSServer_Impl::postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON, int msec)
    {
      if (startPostAnalysisJSON(projectUUID, analysisJSON)){
        waitForFinished(msec);
      }
      return lastPostAnalysisJSONSuccess();
    }

    bool OSServer_Impl::lastPostAnalysisJSONSuccess() const
    {
      return m_lastPostAnalysisJSONSuccess;
    }

    bool OSServer_Impl::postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON, int msec)
    {
      if (startPostDataPointJSON(analysisUUID, dataPointJSON)){
        waitForFinished(msec);
      }
      return lastPostDataPointJSONSuccess();
    }

    bool OSServer_Impl::lastPostDataPointJSONSuccess() const
    {
      return m_lastPostDataPointJSONSuccess;
    }

    bool OSServer_Impl::uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile, int msec)
    {
      if (startUploadAnalysisFiles(analysisUUID, analysisZipFile)){
        waitForFinished(msec);
      }
      return lastUploadAnalysisFilesSuccess();
    }

    bool OSServer_Impl::lastUploadAnalysisFilesSuccess() const
    {
      return m_lastUploadAnalysisFilesSuccess;
    }

    bool OSServer_Impl::start(const UUID& analysisUUID, int msec)
    {
      if (requestStart(analysisUUID)){
        waitForFinished(msec);
      }
      return lastStartSuccess();
    }

    bool OSServer_Impl::lastStartSuccess() const
    {
      return m_lastStartSuccess;
    }

    bool OSServer_Impl::isAnalysisQueued(const UUID& analysisUUID, int msec)
    {
      if (requestIsAnalysisQueued(analysisUUID)){
        waitForFinished(msec);
      }
      return lastIsAnalysisQueued();
    }

    bool OSServer_Impl::lastIsAnalysisQueued() const
    {
      return m_lastIsAnalysisQueued;
    }

    bool OSServer_Impl::isAnalysisRunning(const UUID& analysisUUID, int msec)
    {
      if (requestIsAnalysisRunning(analysisUUID)){
        waitForFinished(msec);
      }
      return lastIsAnalysisRunning();
    }

    bool OSServer_Impl::lastIsAnalysisRunning() const
    {
      return m_lastIsAnalysisRunning;
    }

    bool OSServer_Impl::stop(const UUID& analysisUUID, int msec)
    {
      if (requestStop(analysisUUID)){
        waitForFinished(msec);
      }
      return lastStopSuccess();
    }

    bool OSServer_Impl::lastStopSuccess() const
    {
      return m_lastStopSuccess;
    }

    std::vector<UUID> OSServer_Impl::dataPointUUIDs(const UUID& analysisUUID, int msec)
    {
      if (requestDataPointUUIDs(analysisUUID)){
        waitForFinished(msec);
      }
      return lastDataPointUUIDs();
    }

    std::vector<UUID> OSServer_Impl::lastDataPointUUIDs() const
    {
      return m_lastDataPointUUIDs;
    }

    std::vector<UUID> OSServer_Impl::runningDataPointUUIDs(const UUID& analysisUUID, int msec)
    {
      if (requestRunningDataPointUUIDs(analysisUUID)){
        waitForFinished(msec);
      }
      return lastRunningDataPointUUIDs();
    }

    std::vector<UUID> OSServer_Impl::lastRunningDataPointUUIDs() const
    {
      return m_lastRunningDataPointUUIDs;
    }

    std::vector<UUID> OSServer_Impl::queuedDataPointUUIDs(const UUID& analysisUUID, int msec)
    {
      if (requestQueuedDataPointUUIDs(analysisUUID)){
        waitForFinished(msec);
      }
      return lastQueuedDataPointUUIDs();
    }

    std::vector<UUID> OSServer_Impl::lastQueuedDataPointUUIDs() const
    {
      return m_lastQueuedDataPointUUIDs;
    }

    std::vector<UUID> OSServer_Impl::completeDataPointUUIDs(const UUID& analysisUUID, int msec)
    {
      if (requestCompleteDataPointUUIDs(analysisUUID)){
        waitForFinished(msec);
      }
      return lastCompleteDataPointUUIDs();
    }

    std::vector<UUID> OSServer_Impl::lastCompleteDataPointUUIDs() const
    {
      return m_lastCompleteDataPointUUIDs;
    }

    std::string OSServer_Impl::dataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID, int msec)
    {
      if (requestDataPointJSON(analysisUUID, dataPointUUID)){
        waitForFinished(msec);
      }
      return lastDataPointJSON();
    }

    std::string OSServer_Impl::lastDataPointJSON() const
    {
      return m_lastDataPointJSON;
    }

    bool OSServer_Impl::downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath, int msec)
    {
      if (startDownloadDataPoint(analysisUUID, dataPointUUID, downloadPath)){
        waitForFinished(msec);
      }
      return lastDownloadDataPointSuccess();
    }

    bool OSServer_Impl::lastDownloadDataPointSuccess() const
    {
      return m_lastDownloadDataPointSuccess;
    }

    bool OSServer_Impl::waitForFinished(int msec)
    {
      int msecPerLoop = 20;
      int numTries = msec / msecPerLoop;
      int current = 0;
      while (true)
      {
    
        // if we can get the lock then the download is complete
        if (m_mutex->tryLock()){
          m_mutex->unlock();
          return true;
        }

        // this calls process events
        System::msleep(msecPerLoop);

        if (current > numTries){
          logError("Response timed out");
          break;
        }

        ++current;
      }

      m_networkReply->blockSignals(true);
      m_networkReply->deleteLater();
      m_networkReply = 0;
      m_mutex->unlock();

      emit requestProcessed(false);

      return false;
    }

    std::vector<std::string> OSServer_Impl::errors() const
    {
      return m_errors;
    }

    std::vector<std::string> OSServer_Impl::warnings() const
    {
      return m_warnings;
    }

    bool OSServer_Impl::requestAvailable()
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastAvailable = false;

      QUrl url(m_url);
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processAvailable()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestProjectUUIDs()
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastProjectUUIDs.clear();

      QUrl url(m_url.toString().append("/projects.json"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processProjectUUIDs()));
      OS_ASSERT(test);

      return true;
    } 
    
    bool OSServer_Impl::requestCreateProject(const UUID& projectUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastCreateProjectSuccess = false;

      QString id = toQString(removeBraces(projectUUID));
      QUrl url(m_url.toString().append("/projects.json"));
  
      QVariantMap map;
      map["_id"] = id;
      //"created_at": "2013-08-27T20:40:38Z",
      map["name"] = id;
      //"updated_at": "2013-08-27T20:40:38Z",
      map["uuid"] = id;
      map["analyses"] = QVariantList();
      
      bool test;

      QJson::Serializer serializer;
      QByteArray json = serializer.serialize(map, &test);
      if (test){

        QByteArray postData; 
        postData.append(json); 

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        m_networkReply = m_networkAccessManager->post(request, postData);

        bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processCreateProject()));
        OS_ASSERT(test);

        return true;
      }

      return false;
    }

    bool OSServer_Impl::requestDeleteProject(const UUID& projectUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastDeleteProjectSuccess = false;

      QString id = toQString(removeBraces(projectUUID));
      QUrl url(m_url.toString().append("/projects/").append(id));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->deleteResource(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processDeleteProject()));
      OS_ASSERT(test);

      return true;
    } 

    bool OSServer_Impl::requestAnalysisUUIDs(const UUID& projectUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastAnalysisUUIDs.clear();

      QString id = toQString(removeBraces(projectUUID));
      //QUrl url(m_url.toString().append("/projects/").append(id).append("/analyses.json"));
      QUrl url(m_url.toString().append("/projects/").append(id).append(".json"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processAnalysisUUIDs()));
      OS_ASSERT(test);

      return true;
    } 

    bool OSServer_Impl::startPostAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastPostAnalysisJSONSuccess = false;

      QString id = toQString(removeBraces(projectUUID));
      QUrl url(m_url.toString().append("/projects/").append(id).append("/analyses.json"));

      QByteArray postData; 
      postData.append(toQString(analysisJSON)); 

      QNetworkRequest request(url);
      request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

      m_networkReply = m_networkAccessManager->post(request, postData);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processPostAnalysisJSON()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::startPostDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastPostDataPointJSONSuccess = false;

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/data_points.json"));

      QByteArray postData; 
      postData.append(toQString(dataPointJSON)); 

      QNetworkRequest request(url);
      request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

      m_networkReply = m_networkAccessManager->post(request, postData);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processPostDataPointJSON()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::startUploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastUploadAnalysisFilesSuccess = false;

      if (exists(analysisZipFile)){

        QFile file(toQString(analysisZipFile));
        if (file.open(QIODevice::ReadOnly)){

          QString bound="-------3dpj1k39xoa84u4804ee1156snfxl6"; 

          QByteArray data(QString("--" + bound + "\r\n").toAscii());
          //data += "Content-Disposition: form-data; name=\"action\"\r\n\r\n";
          //data += "\r\n";
          //data += QString("--" + bound + "\r\n").toAscii();
          data += "Content-Disposition: form-data; name=\"file\"; filename=\"seed_zip.zip\"\r\n";
          data += "Content-Type: application/zip\r\n\r\n";
          data.append(file.readAll());
          data += "\r\n";
          data += QString("--" + bound + "\r\n.").toAscii();
          data += "\r\n";
          file.close();

          QString id = toQString(removeBraces(analysisUUID));
          QUrl url(m_url.toString().append("/analyses/").append(id).append("/upload.json")); 

          QNetworkRequest request(url);
          request.setRawHeader(QString("Accept").toAscii(), QString("*/*; q=0.5, application/xml").toAscii());
          request.setRawHeader(QString("Accept-Encoding").toAscii(), QString("gzip,deflate").toAscii());
          request.setRawHeader(QString("Content-Type").toAscii(),QString("multipart/form-data; boundary=" + bound).toAscii());
          request.setRawHeader(QString("Content-Length").toAscii(), QString::number(data.length()).toAscii());
         
          m_networkReply = m_networkAccessManager->post(request, data);

          bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processUploadAnalysisFiles()));
          OS_ASSERT(test);

          return true;
        }

      }else{
        logError("File does not exist");
      }

      return false;
    }

    bool OSServer_Impl::requestStart(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastStartSuccess = false;

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/action.json"));

      QVariantMap map;
      map["action"] = "start";
      
      bool test;
      QJson::Serializer serializer;
      QByteArray json = serializer.serialize(map, &test);
      if (test){

        QByteArray postData; 
        postData.append(json); 

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        m_networkReply = m_networkAccessManager->post(request, postData);

        bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processStart()));
        OS_ASSERT(test);

        return true;
      }

      return false;
    }

    bool OSServer_Impl::requestIsAnalysisQueued(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastIsAnalysisQueued = false;

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append(".json"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processIsAnalysisQueued()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestIsAnalysisRunning(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastIsAnalysisRunning = false;

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append(".json"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processIsAnalysisRunning()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestStop(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastStopSuccess = false;

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/action.json"));

      QVariantMap map;
      map["action"] = "stop";
      
      bool test;
      QJson::Serializer serializer;
      QByteArray json = serializer.serialize(map, &test);
      if (test){

        QByteArray postData; 
        postData.append(json); 

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        m_networkReply = m_networkAccessManager->post(request, postData);

        bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processStop()));
        OS_ASSERT(test);

        return true;
      }

      return false;
    }

    bool OSServer_Impl::requestDataPointUUIDs(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastDataPointUUIDs.clear();

      QString id = toQString(removeBraces(analysisUUID));
      //QUrl url(m_url.toString().append("/analyses/").append(id).append("/data_points.json"));
      //QUrl url(m_url.toString().append("/analyses/").append(id).append(".json"));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/status.json"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processDataPointUUIDs()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestRunningDataPointUUIDs(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastRunningDataPointUUIDs.clear();

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/status.json?jobs=running"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processRunningDataPointUUIDs()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestQueuedDataPointUUIDs(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastQueuedDataPointUUIDs.clear();

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/status.json?jobs=queued"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processQueuedDataPointUUIDs()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestCompleteDataPointUUIDs(const UUID& analysisUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastCompleteDataPointUUIDs.clear();

      QString id = toQString(removeBraces(analysisUUID));
      QUrl url(m_url.toString().append("/analyses/").append(id).append("/status.json?jobs=complete"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processCompleteDataPointUUIDs()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::requestDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID)
    {
      if (!m_mutex->tryLock()){
        return false;
      }

      clearErrorsAndWarnings();

      m_lastDataPointJSON = "";

      QString id = toQString(removeBraces(dataPointUUID));
      //QUrl url(m_url.toString().append("/data_points/").append(id).append("/data_points.json"));
      QUrl url(m_url.toString().append("/data_points/").append(id).append(".json"));
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = connect(m_networkReply, SIGNAL(finished()), this, SLOT(processDataPointJSON()));
      OS_ASSERT(test);

      return true;
    }

    bool OSServer_Impl::startDownloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath)
    {
      return false;
    }

    void OSServer_Impl::processAvailable()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastAvailable = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processProjectUUIDs()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastProjectUUIDs = processListOfUUID(m_networkReply->readAll(), success);
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processCreateProject()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastCreateProjectSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processDeleteProject()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastDeleteProjectSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processAnalysisUUIDs()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        //m_lastAnalysisUUIDs = processListOfUUID(m_networkReply->readAll(), success);

        bool test;
        QJson::Parser parser;
        QVariant variant = parser.parse(m_networkReply->readAll(), &test);

        if (test){

          QVariantMap map = variant.toMap();

          if (map.contains("analyses")){

            QVariantList list = map["analyses"].toList();

            QJson::Serializer serializer;
            QByteArray json = serializer.serialize(list, &test);
            if (test){
              m_lastAnalysisUUIDs = processListOfUUID(json, success);
            }

          }else{
            logError("Incorrect JSON response");
          }

        }else{
          logError("Could not parse JSON response");
        }

      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processPostAnalysisJSON()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastPostAnalysisJSONSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processPostDataPointJSON()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastPostDataPointJSONSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processUploadAnalysisFiles()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastUploadAnalysisFilesSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processStart()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastStartSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processIsAnalysisQueued()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){

        bool test;
        QJson::Parser parser;
        QVariant variant = parser.parse(m_networkReply->readAll(), &test);

        if (test){

          QVariantMap map = variant.toMap();

          if (map.contains("status")){
            success = true;
            QString status = map["status"].toString();

            if (status == "queued"){
              m_lastIsAnalysisQueued = true;
            }else{
              m_lastIsAnalysisQueued = false;
            }

          }else{
            logError("Incorrect JSON response");
          }

        }else{
          logError("Could not parse JSON response");
        }

      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processIsAnalysisRunning()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){

        bool test;
        QJson::Parser parser;
        QVariant variant = parser.parse(m_networkReply->readAll(), &test);

        if (test){

          QVariantMap map = variant.toMap();

          if (map.contains("status")){
            success = true;
            QString status = map["status"].toString();

            if (status == "running"){
              m_lastIsAnalysisRunning = true;
            }else{
              m_lastIsAnalysisRunning = false;
            }

          }else{
            logError("Incorrect JSON response");
          }

        }else{
          logError("Could not parse JSON response");
        }

      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processStop()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastStopSuccess = true;
        success = true;
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processDataPointUUIDs()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        //m_lastDataPointUUIDs = processListOfUUID(m_networkReply->readAll(), success);

        bool test;
        QJson::Parser parser;
        QVariant variant = parser.parse(m_networkReply->readAll(), &test);

        if (test){

          QVariantMap map = variant.toMap();

          if (map.contains("data_points")){
            
            QVariantList list = map["data_points"].toList();

            QJson::Serializer serializer;
            QByteArray json = serializer.serialize(list, &test);
            if (test){
              m_lastDataPointUUIDs = processListOfUUID(json, success);
            }
  
          }else{
            logError("Incorrect JSON response");
          }

        }else{
          logError("Could not parse JSON response");
        }
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processRunningDataPointUUIDs()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastRunningDataPointUUIDs = processListOfUUID(m_networkReply->readAll(), success);
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processQueuedDataPointUUIDs()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastQueuedDataPointUUIDs = processListOfUUID(m_networkReply->readAll(), success);
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processCompleteDataPointUUIDs()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastCompleteDataPointUUIDs = processListOfUUID(m_networkReply->readAll(), success);
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }

    void OSServer_Impl::processDataPointJSON()
    {
      bool success = false;

      if (m_networkReply->error() == QNetworkReply::NoError){
        bool test;
        QJson::Parser parser;
        QVariant variant = parser.parse(m_networkReply->readAll(), &test);
        if (test){
          QJson::Serializer serializer;
          QByteArray json = serializer.serialize(variant, &test);
          if (test){
            success = true;
            m_lastDataPointJSON = QString(json).toStdString();
          }
        }
      }else{
        logNetworkError(m_networkReply->error());
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;

      m_mutex->unlock();

      emit requestProcessed(success);
    }
    
    void OSServer_Impl::clearErrorsAndWarnings()
    {
      m_errors.clear();
      m_warnings.clear();
    }

    void OSServer_Impl::logError(const std::string& error) const
    {
      m_errors.push_back(error);
      LOG(Error, error);
    }

    void OSServer_Impl::logNetworkError(int error) const
    {
      std::stringstream ss; 
      ss << "Network error '";
      switch(error){
        case QNetworkReply::NoError:
          ss << "NoError";
          break;
        case QNetworkReply::ConnectionRefusedError:
          ss << "ConnectionRefusedError";
          break;
        case QNetworkReply::RemoteHostClosedError:
          ss << "RemoteHostClosedError";
          break;
        case QNetworkReply::HostNotFoundError:
          ss << "HostNotFoundError";
          break;
        case QNetworkReply::TimeoutError:
          ss << "TimeoutError";
          break;
        case QNetworkReply::OperationCanceledError:
          ss << "OperationCanceledError";
          break;
        case QNetworkReply::SslHandshakeFailedError:
          ss << "SslHandshakeFailedError";
          break;
        case QNetworkReply::TemporaryNetworkFailureError:
          ss << "TemporaryNetworkFailureError";
          break;
        case QNetworkReply::ProxyConnectionRefusedError:
          ss << "ProxyConnectionRefusedError";
          break;
        case QNetworkReply::ProxyConnectionClosedError:
          ss << "ProxyConnectionClosedError";
          break;
        case QNetworkReply::ProxyNotFoundError:
          ss << "ProxyNotFoundError";
          break;
        case QNetworkReply::ProxyTimeoutError:
          ss << "ProxyTimeoutError";
          break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
          ss << "ProxyAuthenticationRequiredError";
          break;
        case QNetworkReply::ContentAccessDenied:
          ss << "ContentAccessDenied";
          break;
        case QNetworkReply::ContentOperationNotPermittedError:
          ss << "ContentOperationNotPermittedError";
          break;
        case QNetworkReply::ContentNotFoundError:
          ss << "ContentNotFoundError";
          break;
        case QNetworkReply::AuthenticationRequiredError:
          ss << "AuthenticationRequiredError";
          break;
        case QNetworkReply::ContentReSendError:
          ss << "ContentReSendError";
          break;
        case QNetworkReply::ProtocolUnknownError:
          ss << "ProtocolUnknownError";
          break;
        case QNetworkReply::ProtocolInvalidOperationError:
          ss << "ProtocolInvalidOperationError";
          break;
        case QNetworkReply::UnknownNetworkError:
          ss << "UnknownNetworkError";
          break;
        case QNetworkReply::UnknownProxyError:
          ss << "UnknownProxyError";
          break;
        case QNetworkReply::UnknownContentError:
          ss << "UnknownContentError";
          break;
        case QNetworkReply::ProtocolFailure:
          ss << "ProtocolFailure";
          break;
        default:
          ss << "Unknown";
      }
      ss<< "' occurred";

      m_errors.push_back(ss.str());
      LOG(Error, ss.str());
    }

    void OSServer_Impl::logWarning(const std::string& warning) const
    {
      m_warnings.push_back(warning);
      LOG(Warn, warning);
    }

    std::vector<UUID> OSServer_Impl::processListOfUUID(const QByteArray& bytes, bool& success) const
    {
      success = false;

      std::vector<UUID> result;

      bool test;
      QJson::Parser parser;
      QVariant variant = parser.parse(bytes, &test);

      if (test){
        success = true;

        QVariantList list = variant.toList();
        Q_FOREACH(QVariant projectVariant, variant.toList()){
          QVariantMap map = projectVariant.toMap();
          if (map.contains("_id")){
            QString id = map["_id"].toString();
            UUID uuid(id);
            result.push_back(uuid);
          }else{
            success = false;
            result.clear();
            logError("Incorrect JSON response");
            break;
          }
        }
      }else{
        logError("Could not parse JSON response");
      }

      return result;
    }

  }

  OSServer::OSServer(const QUrl& url)
    : m_impl(boost::shared_ptr<detail::OSServer_Impl>(new detail::OSServer_Impl(url)))
  {
  }

  OSServer::~OSServer()
  {
  }

  bool OSServer::available(int msec)
  {
    return getImpl<detail::OSServer_Impl>()->available(msec);
  }

  bool OSServer::lastAvailable() const
  {
    return getImpl<detail::OSServer_Impl>()->lastAvailable();
  }

  std::vector<UUID> OSServer::projectUUIDs(int msec)
  {
    return getImpl<detail::OSServer_Impl>()->projectUUIDs(msec);
  }

  std::vector<UUID> OSServer::lastProjectUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->lastProjectUUIDs();
  } 

  bool OSServer::createProject(const UUID& projectUUID, int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->createProject(projectUUID, msec);
  }

  bool OSServer::lastCreateProjectSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastCreateProjectSuccess();
  }

  bool OSServer::deleteProject(const UUID& projectUUID, int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->deleteProject(projectUUID, msec);
  }

  bool OSServer::lastDeleteProjectSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastDeleteProjectSuccess();
  }

  std::vector<UUID> OSServer::analysisUUIDs(const UUID& projectUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->analysisUUIDs(projectUUID, msec);
  }

  std::vector<UUID> OSServer::lastAnalysisUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->lastAnalysisUUIDs();
  }

  bool OSServer::postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->postAnalysisJSON(projectUUID, analysisJSON, msec);
  }

  bool OSServer::lastPostAnalysisJSONSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastPostAnalysisJSONSuccess();
  }

  bool OSServer::postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->postDataPointJSON(analysisUUID, dataPointJSON, msec);
  }

  bool OSServer::lastPostDataPointJSONSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastPostDataPointJSONSuccess();
  }

  bool OSServer::uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->uploadAnalysisFiles(analysisUUID, analysisZipFile, msec);
  }

  bool OSServer::lastUploadAnalysisFilesSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastUploadAnalysisFilesSuccess();
  }

  bool OSServer::start(const UUID& analysisUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->start(analysisUUID, msec);
  }

  bool OSServer::lastStartSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastStartSuccess();
  }

  bool OSServer::isAnalysisQueued(const UUID& analysisUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->isAnalysisQueued(analysisUUID, msec);
  }

  bool OSServer::lastIsAnalysisQueued() const
  {
    return getImpl<detail::OSServer_Impl>()->lastIsAnalysisQueued();
  }

  bool OSServer::isAnalysisRunning(const UUID& analysisUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->isAnalysisRunning(analysisUUID, msec);
  }

  bool OSServer::lastIsAnalysisRunning() const
  {
    return getImpl<detail::OSServer_Impl>()->lastIsAnalysisRunning();
  }

  bool OSServer::stop(const UUID& analysisUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->stop(analysisUUID, msec);
  }

  bool OSServer::lastStopSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastStopSuccess();
  }

  std::vector<UUID> OSServer::dataPointUUIDs(const UUID& analysisUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->dataPointUUIDs(analysisUUID, msec);
  }

  std::vector<UUID> OSServer::lastDataPointUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->lastDataPointUUIDs();
  }

  std::vector<UUID> OSServer::runningDataPointUUIDs(const UUID& analysisUUID, int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->runningDataPointUUIDs(analysisUUID, msec);
  }

  std::vector<UUID> OSServer::lastRunningDataPointUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->lastRunningDataPointUUIDs();
  }

  std::vector<UUID> OSServer::queuedDataPointUUIDs(const UUID& analysisUUID, int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->queuedDataPointUUIDs(analysisUUID, msec);
  }

  std::vector<UUID> OSServer::lastQueuedDataPointUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->lastQueuedDataPointUUIDs();
  }

  std::vector<UUID> OSServer::completeDataPointUUIDs(const UUID& analysisUUID, int msec)
  {
    return getImpl<detail::OSServer_Impl>()->completeDataPointUUIDs(analysisUUID, msec);
  }

  std::vector<UUID> OSServer::lastCompleteDataPointUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->lastCompleteDataPointUUIDs();
  }

  std::string OSServer::dataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID, int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->dataPointJSON(analysisUUID, dataPointUUID, msec);
  }

  std::string OSServer::lastDataPointJSON() const
  {
    return getImpl<detail::OSServer_Impl>()->lastDataPointJSON();
  }

  bool OSServer::downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath, int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->downloadDataPoint(analysisUUID, dataPointUUID, downloadPath, msec);
  }

  bool OSServer::lastDownloadDataPointSuccess() const
  {
    return getImpl<detail::OSServer_Impl>()->lastDownloadDataPointSuccess();
  }

  bool OSServer::waitForFinished(int msec) 
  {
    return getImpl<detail::OSServer_Impl>()->waitForFinished(msec);
  }

  std::vector<std::string> OSServer::errors() const
  {
    return getImpl<detail::OSServer_Impl>()->errors();
  }

  std::vector<std::string> OSServer::warnings() const
  {
    return getImpl<detail::OSServer_Impl>()->warnings();
  }

  bool OSServer::requestAvailable()
  {
    return getImpl<detail::OSServer_Impl>()->requestAvailable();
  }

  bool OSServer::requestProjectUUIDs()
  {
    return getImpl<detail::OSServer_Impl>()->requestProjectUUIDs();
  } 

  bool OSServer::requestCreateProject(const UUID& projectUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestCreateProject(projectUUID);
  }

  bool OSServer::requestDeleteProject(const UUID& projectUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestDeleteProject(projectUUID);
  }

  bool OSServer::requestAnalysisUUIDs(const UUID& projectUUID)
  {
    return getImpl<detail::OSServer_Impl>()->requestAnalysisUUIDs(projectUUID);
  }

  bool OSServer::startPostAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON) 
  {
    return getImpl<detail::OSServer_Impl>()->startPostAnalysisJSON(projectUUID, analysisJSON);
  }

  bool OSServer::startPostDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON) 
  {
    return getImpl<detail::OSServer_Impl>()->startPostDataPointJSON(analysisUUID, dataPointJSON);
  }

  bool OSServer::startUploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile) 
  {
    return getImpl<detail::OSServer_Impl>()->startUploadAnalysisFiles(analysisUUID, analysisZipFile);
  }

  bool OSServer::requestStart(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestStart(analysisUUID);
  }

  bool OSServer::requestIsAnalysisQueued(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestIsAnalysisQueued(analysisUUID);
  }

  bool OSServer::requestIsAnalysisRunning(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestIsAnalysisRunning(analysisUUID);
  }

  bool OSServer::requestStop(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestStop(analysisUUID);
  }

  bool OSServer::requestDataPointUUIDs(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestDataPointUUIDs(analysisUUID);
  }

  bool OSServer::requestRunningDataPointUUIDs(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestRunningDataPointUUIDs(analysisUUID);
  }

  bool OSServer::requestQueuedDataPointUUIDs(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestQueuedDataPointUUIDs(analysisUUID);
  }

  bool OSServer::requestCompleteDataPointUUIDs(const UUID& analysisUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestCompleteDataPointUUIDs(analysisUUID);
  }

  bool OSServer::requestDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID) 
  {
    return getImpl<detail::OSServer_Impl>()->requestDataPointJSON(analysisUUID, dataPointUUID);
  }

  bool OSServer::startDownloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath) 
  {
    return getImpl<detail::OSServer_Impl>()->startDownloadDataPoint(analysisUUID, dataPointUUID, downloadPath);
  }

} // openstudio
