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

#ifndef UTILITIES_CLOUD_OSSERVER_HPP
#define UTILITIES_CLOUD_OSSERVER_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/core/Path.hpp>

#include <QObject>

#include <string>

// TODO: we will need to swig QUrl
class QUrl;

namespace openstudio{

  /// OSServer is a class for accessing the rails server started on machines provided by a CloudProvider.
  class UTILITIES_API OSServer : public QObject {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor
    OSServer(const QUrl& url);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~OSServer();

    //@}
    /** @name Class members */
    //@{

    /// Get the server view of all projects in JSON format
    // DLM: what file format is this?  do we have this defined or any examples? 
    // do we need this at all?  do we need to be able to parse this somewhere?
    std::string projectsJSON() const; 

    /// Send a project server view JSON file, adds the project
    // DLM: what file format is this?  do we have this defined or any examples? 
    bool postProjectJSON(const std::string& projectJSON) const;

    /// Get the server view of all analyses for project in JSON format
    // DLM: what file format is this?  do we have this defined or any examples? 
    // do we need this at all?  do we need to be able to parse this somewhere?
    std::string analysesJSON(const UUID& projectUUID) const; 

    /// Send an analysis server view JSON file for project, adds the analyis to project
    bool postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON) const;

    /// Send a datapoint server view JSON file for analysis, adds the dataPoint to analysis
    bool postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON) const;

    /// Upload a zip file of all the files needed for analysis
    // DLM: is the structure of this defined somewhere? 
    bool uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile);

    /// Send the server a request to start the analysis
    bool start(const UUID& analysisUUID) const;

    /// Send the server a request to stop the analysis
    bool stop(const UUID& analysisUUID) const;

    /// Get a list of all dataPoints in the analysis
    // DLM: what file format is this?  do we have this defined or any examples? 
    std::string dataPointsJSON(const UUID& analysisUUID) const;

    /// Get a list of all running dataPoints in the analysis
    // DLM: what file format is this?  do we have this defined or any examples? 
    std::string runningDataPointsJSON(const UUID& analysisUUID) const;

    /// Get a list of all queued dataPoints in the analysis
    // DLM: what file format is this?  do we have this defined or any examples? 
    std::string queuedDataPointsJSON(const UUID& analysisUUID) const;

    /// Get a list of all complete dataPoints in the analysis
    // DLM: what file format is this?  do we have this defined or any examples? 
    std::string completeDataPointsJSON(const UUID& analysisUUID) const;

    /// Get a full deserializable JSON of dataPoint
    std::string getDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID) const;

    /// Get a detailed results for dataPoint, will be a zip file of the dataPoint directory
    bool downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath) const;

    //@}

  private:

    // no body on purpose, do not want this generated
    OSServer(const OSServer& other);

  };

} // openstudio

#endif // UTILITIES_BCL_LOCALBCL_HPP
