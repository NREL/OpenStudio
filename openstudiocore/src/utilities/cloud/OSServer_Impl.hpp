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

#ifndef UTILITIES_CLOUD_OSSERVER_IMPL_HPP
#define UTILITIES_CLOUD_OSSERVER_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/UUID.hpp"
#include "../core/Url.hpp"
#include "../core/Path.hpp"
#include "../core/Logger.hpp"

#include <QObject>

#include <string>

class QJsonArray;
class QNetworkAccessManager;
class QNetworkReply;
class QMutex;

namespace openstudio{
namespace detail{

  /// OSServer is a class for accessing the rails server started on machines provided by a CloudProvider.
  class UTILITIES_API OSServer_Impl : public QObject {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor
    OSServer_Impl(const Url& url);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OSServer_Impl();

    //@}
    /** @name Blocking class members */
    //@{

    bool available(int msec);
    bool lastAvailable() const;

    std::vector<UUID> projectUUIDs(int msec); 
    std::vector<UUID> lastProjectUUIDs() const; 

    bool createProject(const UUID& projectUUID, int msec); 
    bool lastCreateProjectSuccess() const; 

    bool deleteProject(const UUID& projectUUID, int msec); 
    bool lastDeleteProjectSuccess() const; 

    std::vector<UUID> analysisUUIDs(const UUID& projectUUID, int msec); 
    std::vector<UUID> lastAnalysisUUIDs() const; 

    bool postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON, int msec);
    bool lastPostAnalysisJSONSuccess() const;

    bool postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON, int msec);
    bool lastPostDataPointJSONSuccess() const;

    bool uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile, int msec);
    bool lastUploadAnalysisFilesSuccess() const;

    bool start(const UUID& analysisUUID, int msec);
    bool lastStartSuccess() const;
    
    bool isAnalysisQueued(const UUID& analysisUUID, int msec);
    bool lastIsAnalysisQueued() const;

    bool isAnalysisRunning(const UUID& analysisUUID, int msec);
    bool lastIsAnalysisRunning() const;

    bool isAnalysisComplete(const UUID& analysisUUID, int msec);
    bool lastIsAnalysisComplete() const;

    bool stop(const UUID& analysisUUID, int msec);
    bool lastStopSuccess() const;

    std::vector<UUID> dataPointUUIDs(const UUID& analysisUUID, int msec);
    std::vector<UUID> lastDataPointUUIDs() const;

    std::vector<UUID> queuedDataPointUUIDs(const UUID& analysisUUID, int msec);
    std::vector<UUID> lastQueuedDataPointUUIDs() const;

    std::vector<UUID> runningDataPointUUIDs(const UUID& analysisUUID, int msec);
    std::vector<UUID> lastRunningDataPointUUIDs() const;

    std::vector<UUID> completeDataPointUUIDs(const UUID& analysisUUID, int msec);
    std::vector<UUID> lastCompleteDataPointUUIDs() const;

    std::vector<UUID> downloadReadyDataPointUUIDs(const UUID& analysisUUID, int msec);
    std::vector<UUID> lastDownloadReadyDataPointUUIDs() const;

    std::string dataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID, int msec);
    std::string lastDataPointJSON() const;

    bool downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath, int msec);
    bool lastDownloadDataPointSuccess() const;

    bool deleteDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, int msec=30000);
    bool lastDeleteDataPointSuccess() const;

    bool waitForFinished(int msec);

    std::vector<std::string> errors() const;
    
    std::vector<std::string> warnings() const;

    //@}
    /** @name Non-blocking class members */
    //@{

    bool requestAvailable();

    bool requestProjectUUIDs(); 

    bool requestCreateProject(const UUID& projectUUID); 

    bool requestDeleteProject(const UUID& projectUUID); 

    bool requestAnalysisUUIDs(const UUID& projectUUID); 

    bool startPostAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON);

    bool startPostDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON);

    bool startUploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile);

    bool requestStart(const UUID& analysisUUID);

    bool requestIsAnalysisQueued(const UUID& analysisUUID);

    bool requestIsAnalysisRunning(const UUID& analysisUUID);

    bool requestIsAnalysisComplete(const UUID& analysisUUID);

    bool requestStop(const UUID& analysisUUID);

    bool requestDataPointUUIDs(const UUID& analysisUUID);

    bool requestRunningDataPointUUIDs(const UUID& analysisUUID);

    bool requestQueuedDataPointUUIDs(const UUID& analysisUUID);

    bool requestCompleteDataPointUUIDs(const UUID& analysisUUID);

    bool requestDownloadReadyDataPointUUIDs(const UUID& analysisUUID);

    bool requestDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID);

    bool startDownloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath);

    bool requestDeleteDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID);

    //@}
    /** @name Signals, Slots, Threads */
    //@{

    /** Connect signal from this OSServer to slot on qObject. */
//    bool connect(const std::string& signal,
//                 const QObject* qObject,
//                 const std::string& slot,
//                 Qt::ConnectionType type = Qt::AutoConnection) const;

    /** Disconnect signal from this OSServer to slot on receiver. */
//    bool disconnect(const char* signal=0,
//                    const QObject* receiver=0,
//                    const char* slot=0) const;

    //@}
  signals:

    void requestProcessed(bool success);

  protected slots:

    void processAvailable();

    void processProjectUUIDs(); 

    void processCreateProject();

    void processDeleteProject(); 

    void processAnalysisUUIDs(); 

    void processPostAnalysisJSON();

    void processPostDataPointJSON();

    void processUploadAnalysisFiles();

    void processStart();

    void processIsAnalysisQueued();

    void processIsAnalysisRunning();

    void processIsAnalysisComplete();

    void processStop();

    void processDataPointUUIDs();

    void processRunningDataPointUUIDs();

    void processQueuedDataPointUUIDs();

    void processCompleteDataPointUUIDs();

    void processDownloadReadyDataPointUUIDs();

    void processDataPointJSON();

    void processDownloadDataPointComplete();

    void processDeleteDataPoint();

    // slot provided for debugging
    void logUploadProgress(qint64 bytesSent, qint64 bytesTotal);

  private:

    Url m_url;
    std::shared_ptr<QNetworkAccessManager> m_networkAccessManager;
    QNetworkReply* m_networkReply;
    std::shared_ptr<QMutex> m_mutex;

    bool m_lastAvailable;
    std::vector<UUID> m_lastProjectUUIDs; 
    bool m_lastCreateProjectSuccess;
    bool m_lastDeleteProjectSuccess;
    std::vector<UUID> m_lastAnalysisUUIDs;
    bool m_lastPostAnalysisJSONSuccess;
    bool m_lastPostDataPointJSONSuccess;
    bool m_lastUploadAnalysisFilesSuccess;
    bool m_lastStartSuccess;
    bool m_lastIsAnalysisQueued;
    bool m_lastIsAnalysisRunning;
    bool m_lastIsAnalysisComplete;
    bool m_lastStopSuccess;
    std::vector<UUID> m_lastDataPointUUIDs;
    std::vector<UUID> m_lastQueuedDataPointUUIDs;
    std::vector<UUID> m_lastRunningDataPointUUIDs;
    std::vector<UUID> m_lastCompleteDataPointUUIDs;
    std::vector<UUID> m_lastDownloadReadyDataPointUUIDs;
    std::string m_lastDataPointJSON;
    bool m_lastDownloadDataPointSuccess;
    path m_lastDownloadDataPointPath;
    bool m_lastDeleteDataPointSuccess;

    mutable std::vector<std::string> m_errors;
    mutable std::vector<std::string> m_warnings;

    void clearErrorsAndWarnings();
    void logNetworkReply(const std::string& methodName) const;
    void logError(const std::string& error) const;
    void logNetworkError(int error) const;
    void logWarning(const std::string& warning) const;
    std::vector<UUID> processListOfUUID(const QJsonArray& array, bool& success) const;

    void resetNetworkReply();

    // configure logging
    REGISTER_LOGGER("utilities.cloud.OSServer");
  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_OSSERVER_IMPL_HPP
