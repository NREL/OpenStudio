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

#ifndef BIMSERVER_BIMSERVERCONNECTION_HPP
#define BIMSERVER_BIMSERVERCONNECTION_HPP

#include "BIMserverAPI.hpp"

#include <QString>
#include <QUrl>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


namespace openstudio {

  class ProgressBar;

namespace model {
  class Model;
  class ModelObject;
  class Surface;
}

namespace bimserver {

  /// This provides utilities to connect to BIMserver
  class BIMSERVER_API BIMserverConnection : public QObject
  {
    Q_OBJECT
  
  public:
    /// Default constructor
    BIMserverConnection(QObject * parent, QString bimserverAddr, QString bimserverPort);
    /// destructor
    ~BIMserverConnection();
    /// login with username and password
    void login(QString username, QString password);
    /// download the osm model
    void download(QString projectID);
    /// get all projects
    void getAllProjects();
    /// create new project
    void createProject(QString projectName);
    /// check in new ifc file
    void checkInIFCFile(QString projectID, QString IFCFilePath);
    /// get all revisions of IFC files of a project
    void getIFCRevisionList(QString projectID);

  signals:
    /// send the retrieved osmString to GUI
    void osmStringRetrieved(QString osmString);

    ///send the list of all projects to GUI
    void listAllProjects(QStringList projectList);

    ///send the list of all ifc revisions
    void listAllIFCRevisions(QStringList ifcRevisionList);

    ///emit error occurs signal 
    void errorOccured(QString errorMessage);

    ///operationSucceeded() will send one of the following QString: 
    void operationSucceeded(QString successMessage);

  private slots:
    //slots used by this class only
    /// log in to BIMserver
    void processLoginRequest(QNetworkReply *rep);
    /// get all projects
    void processGetAllProjectsRequest(QNetworkReply *rep);
    /// get serializer id
    void processGetSerializerRequest(QNetworkReply *rep);
    /// get download action id
    void processDownloadRequest(QNetworkReply *rep);
    /// get download data
    void processGetDownloadDataRequest(QNetworkReply *rep);
    /// create new project
    void processCreateProjectRequest(QNetworkReply *rep);
    /// get ifc deserializer
    void processGetDeserializerRequest(QNetworkReply *rep);
    /// checkIn new IFC
    void processCheckInIFCRequest(QNetworkReply *rep);
    /// get ifc revision list
    void processGetProjectByIDRequest(QNetworkReply *rep);

    void processGetProgressRequest();

  private:
    void sendLoginRequest();
    void sendGetAllProjectsRequest();
    void sendGetSerializerRequest();
    void sendDownloadRequest();
    void sendGetDownloadDataRequest();
    void sendCreateProjectRequest(QString projectName);
    void sendGetDeserializerRequest();
    void sendCheckInIFCRequest(QString IFCFilePath);
    void sendGetProjectByIDRequest(QString projectID);
    void sendGetProgressRequest(QString topicId, QString action);

    bool containsError(QJsonObject responseMessage);
    void extractErrorMessage(QJsonObject responseMessage);

    QNetworkAccessManager* m_networkManager;
    QUrl m_bimserverURL;
    QString m_username;
    QString m_password;
    QString m_token;
    QString m_serializerOid;
    QString m_deserializerOid;
    QString m_roid;
    QString m_actionId;
    QString m_poid;
    QString m_filePath;
  };

} // bimserver
} // openstudio

#endif // BIMSERVER_BIMSERVERCONNECTION_HPP
