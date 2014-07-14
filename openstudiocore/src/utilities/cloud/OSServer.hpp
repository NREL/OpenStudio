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

#ifndef UTILITIES_CLOUD_OSSERVER_HPP
#define UTILITIES_CLOUD_OSSERVER_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/UUID.hpp"
#include "../core/Path.hpp"
#include "../core/Url.hpp"
#include "../core/Logger.hpp"

#include <string>

namespace openstudio{
  namespace detail{
    class OSServer_Impl;
  }

  /// OSServer is a class for accessing the rails server started on machines provided by a CloudProvider.
  class UTILITIES_API OSServer {
  public:

    /** @name Constructor */
    //@{

    // constructor
    OSServer(const Url& url);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OSServer();

    //@}
    /** @name Blocking class members */
    //@{

    bool available(int msec=30000);
    bool lastAvailable() const;

    std::vector<UUID> projectUUIDs(int msec=30000); 
    std::vector<UUID> lastProjectUUIDs() const; 

    /** Typically, projectUUID corresponds to a project::ProjectDatabase::handle(). */
    bool createProject(const UUID& projectUUID, int msec=30000); 
    bool lastCreateProjectSuccess() const; 

    bool deleteProject(const UUID& projectUUID, int msec=30000); 
    bool lastDeleteProjectSuccess() const; 

    std::vector<UUID> analysisUUIDs(const UUID& projectUUID, int msec=30000); 
    std::vector<UUID> lastAnalysisUUIDs() const; 

    /** Posts file format created by analysis::Analysis::toJSON. Creates an analysis on the 
     *  openstudio-server with UUID equal to analysis::Analysis::uuid(). */
    bool postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON, int msec=30000);
    bool lastPostAnalysisJSONSuccess() const;

    /** Posts file format created by 
     * 
     *  \code 
     *  analysis::toJSON(const std::vector<DataPoint>&, const DataPointSerializationOptions&);
     *  \endcode
     *
     *  in analysis/DataPoint.hpp */
    bool postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON, int msec=30000);
    bool lastPostDataPointJSONSuccess() const;

    /** Upload the zip file created by analysisdriver::SimpleProject::zipFileForCloud(). */
    bool uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile, int msec=30000);
    bool lastUploadAnalysisFilesSuccess() const;

    bool start(const UUID& analysisUUID, int msec=30000);
    bool lastStartSuccess() const;

    bool isAnalysisQueued(const UUID& analysisUUID, int msec=30000);
    bool lastIsAnalysisQueued() const;

    bool isAnalysisRunning(const UUID& analysisUUID, int msec=30000);
    bool lastIsAnalysisRunning() const;

    bool isAnalysisComplete(const UUID& analysisUUID, int msec=30000);
    bool lastIsAnalysisComplete() const;

    bool stop(const UUID& analysisUUID, int msec=30000);
    bool lastStopSuccess() const;

    std::vector<UUID> dataPointUUIDs(const UUID& analysisUUID, int msec=30000);
    std::vector<UUID> lastDataPointUUIDs() const;

    std::vector<UUID> queuedDataPointUUIDs(const UUID& analysisUUID, int msec=30000);
    std::vector<UUID> lastQueuedDataPointUUIDs() const;

    std::vector<UUID> runningDataPointUUIDs(const UUID& analysisUUID, int msec=30000);
    std::vector<UUID> lastRunningDataPointUUIDs() const;

    std::vector<UUID> completeDataPointUUIDs(const UUID& analysisUUID, int msec=30000);
    std::vector<UUID> lastCompleteDataPointUUIDs() const;

    std::vector<UUID> downloadReadyDataPointUUIDs(const UUID& analysisUUID, int msec=30000);
    std::vector<UUID> lastDownloadReadyDataPointUUIDs() const;

    /** Returns JSON format that can be loaded with analysis::DataPoint::loadJSON. This format 
     *  can also be used to update an existing DataPoint (analysis::DataPoint::updateFromJSON). */
    std::string dataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID, int msec=30000);
    std::string lastDataPointJSON() const;

    /** Returns a zip file containing detailed simulation results for dataPointUUID. Should be 
     *  called after getting the high-level results from dataPointJSON. */
    bool downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath, int msec=30000);
    bool lastDownloadDataPointSuccess() const;

    bool deleteDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, int msec=30000);
    bool lastDeleteDataPointSuccess() const;

    bool waitForFinished(int msec=30000);

    std::vector<std::string> errors() const;
    
    std::vector<std::string> warnings() const;

    //@}
    /** @name Non-blocking class members */
    //@{

    bool requestAvailable();

    bool requestProjectUUIDs(); 

    /** Typically, projectUUID corresponds to a project::ProjectDatabase::handle(). */
    bool requestCreateProject(const UUID& projectUUID); 

    bool requestDeleteProject(const UUID& projectUUID); 

    bool requestAnalysisUUIDs(const UUID& projectUUID); 

    /** Posts file format created by analysis::Analysis::toJSON. Creates an analysis on the 
     *  openstudio-server with UUID equal to analysis::Analysis::uuid(). */
    bool startPostAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON);

    /** Posts file format created by 
     * 
     *  \code 
     *  analysis::toJSON(const std::vector<DataPoint>&, const DataPointSerializationOptions&);
     *  \endcode
     *
     *  in analysis/DataPoint.hpp */
    bool startPostDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON);

    /** Upload the zip file created by analysisdriver::SimpleProject::zipFileForCloud(). */
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

    /** Returns JSON format that can be loaded with analysis::DataPoint::loadJSON. This format 
     *  can also be used to update an existing DataPoint (analysis::DataPoint::updateFromJSON). */
    bool requestDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID);

    /** Returns a zip file containing detailed simulation results for dataPointUUID. Should be 
     *  called after getting the high-level results from dataPointJSON. */
    bool startDownloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath);

    bool requestDeleteDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID);

    //@}
    /** @name Signals, Slots, Threads */
    //@{

    /** Connect signal from this OSServer to slot on qObject. */
    bool connect(const char* signal,
                 const QObject* qObject,
                 const char* slot,
                 Qt::ConnectionType type = Qt::AutoConnection) const;

    /** Disconnect signal from this OSServer to slot on receiver. */
    bool disconnect(const char* signal=nullptr,
                    const QObject* receiver=nullptr,
                    const char* slot=nullptr) const;

    //@}
    /** @name Type Casting */
    //@{

      template<typename T>
        std::shared_ptr<T> getImpl() const
      {  return std::dynamic_pointer_cast<T>(m_impl); }

      /// cast to type T, can throw std::bad_cast
      template<typename T>
      T cast() const{
        std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
        if (!impl){
          throw(std::bad_cast());
        }
        return T(impl);
      }

      /// cast to optional of type T
      template<typename T>
      boost::optional<T> optionalCast() const{
        boost::optional<T> result;
        std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
        if (impl){
          result = T(impl);
        }
        return result;
      }

    //@}

  protected:

    OSServer(const std::shared_ptr<detail::OSServer_Impl>& impl);

  private:

    std::shared_ptr<detail::OSServer_Impl> m_impl;
    
    // configure logging
    REGISTER_LOGGER("utilities.cloud.OSServer");

  };

} // openstudio

#endif // UTILITIES_CLOUD_OSSERVER_HPP
