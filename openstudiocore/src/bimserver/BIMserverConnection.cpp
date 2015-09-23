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

#include "BIMserverConnection.hpp"

#include "../utilities/core/System.hpp"

#include <QString>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QByteArray>
#include <QFileInfo>
#include <iostream>
#include <boost/none.hpp>


namespace openstudio {
namespace bimserver {

  BIMserverConnection::BIMserverConnection(QObject * parent, QString bimserverAddr, QString bimserverPort) :
    QObject(parent),
    m_networkManager(new QNetworkAccessManager)  
  {
    QString bimserver = "http://" + bimserverAddr + ":" + bimserverPort + "/json";
    m_bimserverURL = QUrl(bimserver);
    m_operationDone = true;
  }

  BIMserverConnection::~BIMserverConnection() {
    delete m_networkManager;
  }

  void BIMserverConnection::login(QString username, QString password) {
    
    if (!m_operationDone) {
      emit errorOccured(QString("The last process has not end yet. Please wait and try again."));
      return;
    }
    m_operationDone = false;
    if (!username.isEmpty() && !password.isEmpty()) {
      m_username = username;
      m_password = password;
      sendLoginRequest();
    } else {
      emit errorOccured(QString("Username or password empty!"));
    }
  }

  void BIMserverConnection::sendLoginRequest() {
    //construct login json
    QJsonObject parameters;
    parameters["username"] = QJsonValue(m_username);
    parameters["password"] = QJsonValue(m_password);
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1AuthInterface"));
    request["method"] = QJsonValue(QString("login"));
    request["parameters"] = parameters;
    QJsonObject jsonRequest;
    jsonRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(jsonRequest);

    QByteArray jsonByteArray = doc.toJson();

    //setup network connection

    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processLoginRequest);
    m_networkManager->post(qNetworkRequest, jsonByteArray);
  }

  void BIMserverConnection::processLoginRequest(QNetworkReply *rep) {
    if (rep) {//extract token from login Request.
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject responseObj = responseDoc.object();
      QJsonObject response = responseObj["response"].toObject();

      //if error occurs, emit signals. Otherwise, set m_token
      if (!containsError(response)) {

        m_token = response["result"].toString();
        m_loginSuccess = true;
        m_operationDone = true;
        emit operationSucceeded(QString("login"));

      } else {
        // if login contains any kinds of error, such as username/password incorrect, it is a bimserver error
        emit bimserverError();
      }

    } else {
      emit bimserverError();
    }
  }

  void BIMserverConnection::getAllProjects() { 
    if (!m_operationDone) {
      emit errorOccured(QString("The last process has not end yet. Please wait and try again."));
      return;
    }
    m_operationDone = false;
    sendGetAllProjectsRequest();
  }

  void BIMserverConnection::sendGetAllProjectsRequest() {
    QJsonObject parameters;
    parameters["onlyTopLevel"] = QJsonValue(true);
    parameters["onlyActive"] = QJsonValue(true);
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("getAllProjects"));
    request["parameters"] = parameters;
    QJsonObject jsonRequest;

    jsonRequest["token"] = QJsonValue(m_token);
    jsonRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(jsonRequest);

    QByteArray downloadJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processGetAllProjectsRequest);
    m_networkManager->post(qNetworkRequest, downloadJson);
  }

  void BIMserverConnection::processGetAllProjectsRequest(QNetworkReply *rep) {
    if (rep){
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject responseObj = responseDoc.object();

      QJsonObject response = responseObj["response"].toObject();

      if (!containsError(response)) {
        QJsonArray result = response["result"].toArray();
        QStringList projectList;
        foreach(const QJsonValue & value, result) {
          QJsonObject ifcProject = value.toObject();
          int oid = ifcProject["oid"].toInt();
          QString projectID = QString::number(oid);
          QString projectName = ifcProject["name"].toString();
          QString project = projectID.append(":").append(projectName);
          projectList.append(project);
        }
        m_projectList = projectList;
        m_operationDone = true;
        emit listAllProjects(projectList);
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
    
  }

  void BIMserverConnection::download(QString revisionID) {

    if (!m_operationDone) {
      emit errorOccured(QString("The last process has not end yet. Please wait and try again."));
      return;
    }
    m_operationDone = false;
    if (!revisionID.isEmpty()) {
      m_roid = revisionID;
      sendGetSerializerRequest();
    } else {
      emit errorOccured(QString("Revision ID is empty"));
    }
  }

  void BIMserverConnection::sendGetSerializerRequest() {
    QJsonObject parameters;
    parameters["serializerName"] = QJsonValue(QString("Osm"));
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("getSerializerByName"));
    request["parameters"] = parameters;
    QJsonObject jsonRequest;
    jsonRequest["token"] = QJsonValue(m_token);
    jsonRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(jsonRequest);

    QByteArray jsonByteArray = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processGetSerializerRequest);
    m_networkManager->post(qNetworkRequest, jsonByteArray);
  }

  void BIMserverConnection::processGetSerializerRequest(QNetworkReply *rep) {
    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject downloadResponse = responseDoc.object();
      QJsonObject response = downloadResponse["response"].toObject();

      if (!containsError(response))
      {
        QJsonObject result = response["result"].toObject();
        m_serializerOid = QString::number(result["oid"].toInt());
        sendDownloadRequest();
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
  }

  void BIMserverConnection::sendDownloadRequest() {
    QJsonObject parameters;
    parameters["roid"] = QJsonValue(m_roid);
    parameters["serializerOid"] = QJsonValue(m_serializerOid);
    parameters["showOwn"] = QJsonValue(false);
    parameters["sync"] = QJsonValue(false);
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("download"));
    request["parameters"] = parameters;
    QJsonObject jsonRequest;
    jsonRequest["token"] = QJsonValue(m_token);
    jsonRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(jsonRequest);

    QByteArray jsonByteArray = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processDownloadRequest);
    m_networkManager->post(qNetworkRequest, jsonByteArray);
  }

  void BIMserverConnection::processDownloadRequest(QNetworkReply *rep) {
    //extract token from login Request
    if (rep) {
      QByteArray responseArray = rep->readAll();
      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject downloadResponse = responseDoc.object();
      QJsonObject response = downloadResponse["response"].toObject();
      
      if (!containsError(response)) {
        m_actionId = QString::number(response["result"].toInt());
        sendGetProgressRequest(m_actionId, QString("download"));
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
  }

  void BIMserverConnection::sendGetDownloadDataRequest() {
    QJsonObject parameters;
    parameters["actionId"] = QJsonValue(m_actionId);
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("getDownloadData"));
    request["parameters"] = parameters;
    QJsonObject getDownloadDataRequest;
    getDownloadDataRequest["token"] = QJsonValue(m_token);
    getDownloadDataRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(getDownloadDataRequest);

    QByteArray getDownloadDataJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processGetDownloadDataRequest);
    m_networkManager->post(qNetworkRequest, getDownloadDataJson);
  }

  void BIMserverConnection::processGetDownloadDataRequest(QNetworkReply *rep) {
    //extract token from login Request

    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject downloadResponse = responseDoc.object();
      QJsonObject response = downloadResponse["response"].toObject();

      if (!containsError(response)) {
        QJsonObject result = response["result"].toObject();
        QString file = result["file"].toString();
        //decode the response
        QByteArray byteArray;
        byteArray.append(file);
        QString OSMFile = QByteArray::fromBase64(byteArray);

        m_osmModel = OSMFile;
        m_operationDone = true;
        emit osmStringRetrieved(OSMFile);
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
  }

  //
  void BIMserverConnection::createProject(QString projectName) {

    if (!m_operationDone) {
      emit errorOccured(QString("The last process has not end yet. Please wait and try again."));
      return;
    }
    m_operationDone = false;

    if (!projectName.isEmpty()) {
      sendCreateProjectRequest(projectName);
    } else {
      emit errorOccured(QString("Project Name is empty!"));
    }
  }

  void BIMserverConnection::sendCreateProjectRequest(QString projectName) {
    
    QJsonObject parameters;
    parameters["projectName"] = projectName;
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("addProject"));
    request["parameters"] = parameters;
    QJsonObject createProjectRequest;
    createProjectRequest["token"] = QJsonValue(m_token);
    createProjectRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(createProjectRequest);

    QByteArray createProjectRequestJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processCreateProjectRequest);
    m_networkManager->post(qNetworkRequest, createProjectRequestJson);
    
  }

  void BIMserverConnection::processCreateProjectRequest(QNetworkReply *rep) {
    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject createProjectResponse = responseDoc.object();
      QJsonObject response = createProjectResponse["response"].toObject();

      if (!containsError(response)) {
        m_createProjectSuccess = true;
        m_operationDone = true;
        emit operationSucceeded(QString("createProject"));
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
    
  }

    void BIMserverConnection::deleteProject(QString projectID) {

    if (!m_operationDone) {
      emit errorOccured(QString("The last process has not end yet. Please wait and try again."));
      return;
    }
    m_operationDone = false;

    if (!projectID.isEmpty()) {
      sendDeleteProjectRequest(projectID);
    } else {
      emit errorOccured(QString("Project ID is empty!"));
    }
  }

  void BIMserverConnection::sendDeleteProjectRequest(QString projectID) {
    
    QJsonObject parameters;
    parameters["poid"] = projectID;
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("deleteProject"));
    request["parameters"] = parameters;
    QJsonObject deleteProjectRequest;
    deleteProjectRequest["token"] = QJsonValue(m_token);
    deleteProjectRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(deleteProjectRequest);

    QByteArray deleteProjectRequestJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processDeleteProjectRequest);
    m_networkManager->post(qNetworkRequest, deleteProjectRequestJson);
    
  }


  void BIMserverConnection::processDeleteProjectRequest(QNetworkReply *rep) {
    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject deleteProjectResponse = responseDoc.object();
      QJsonObject response = deleteProjectResponse["response"].toObject();

      if (!containsError(response)) {
        m_deleteProjectSuccess = true;
        m_operationDone = true;
        emit operationSucceeded(QString("deleteProject"));
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
    
  }
  
  void BIMserverConnection::checkInIFCFile(QString projectID, QString IFCFilePath) {

    if (!m_operationDone) {
      emit errorOccured(QString("The last process has not end yet. Please wait and try again."));
      return;
    }
    m_operationDone = false;

    if (!projectID.isEmpty() && !IFCFilePath.isEmpty()) {
      m_poid = projectID;
      m_filePath = IFCFilePath;
      sendGetDeserializerRequest();
    } else {
      emit errorOccured(QString("Please provide valid projectID and IFC file path!"));
    }
  }

  void BIMserverConnection::sendGetDeserializerRequest() {
    QJsonObject parameters;
    parameters["extension"] = QJsonValue(QString("ifc"));
    
    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("getSuggestedDeserializerForExtension"));
    request["parameters"] = parameters;

    QJsonObject getDeserializerRequest;
    getDeserializerRequest["token"] = QJsonValue(m_token);
    getDeserializerRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(getDeserializerRequest);
    QByteArray getDeserializerRequestJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processGetDeserializerRequest);
    m_networkManager->post(qNetworkRequest, getDeserializerRequestJson);
  }

  void BIMserverConnection::processGetDeserializerRequest(QNetworkReply *rep) {
    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject getDeserializerResponse = responseDoc.object();
      QJsonObject response = getDeserializerResponse["response"].toObject();

      if (!containsError(response)) {
        QJsonObject result = response["result"].toObject();
        m_deserializerOid = QString::number(result["oid"].toInt());
        sendCheckInIFCRequest(m_filePath);
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
  }

  void BIMserverConnection::sendCheckInIFCRequest(QString IFCFilePath) {
    QFile file(IFCFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
      emit errorOccured(QString("Cannot open file, please verify and try again"));
      return;
    }
    QFileInfo info(IFCFilePath);

    QJsonObject parameters;
    parameters["poid"] = QJsonValue(m_poid);
    parameters["comment"] = QJsonValue(QString(""));
    parameters["deserializerOid"] = QJsonValue(m_deserializerOid);
    parameters["fileSize"] = QJsonValue(QString::number(info.size()));
    parameters["fileName"] = QJsonValue(info.completeBaseName());
    //encode file into Base64
    QByteArray fileArray = file.readAll();
    QByteArray fileArrayEncoded = fileArray.toBase64();
    QString fileEncoded(fileArrayEncoded);
    parameters["data"] = QJsonValue(fileEncoded);
    parameters["merge"] = QJsonValue(false);
    parameters["sync"] = QJsonValue(false);

    QJsonObject request;
    request["interface"] = QJsonValue(QString("ServiceInterface"));
    request["method"] = QJsonValue(QString("checkin"));
    request["parameters"] = parameters;

    QJsonObject checkInIFCRequest;
    checkInIFCRequest["token"] = QJsonValue(m_token);
    checkInIFCRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(checkInIFCRequest);
    QByteArray checkInIFCRequestJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processCheckInIFCRequest);
    m_networkManager->post(qNetworkRequest, checkInIFCRequestJson);
  }

  void BIMserverConnection::processCheckInIFCRequest(QNetworkReply *rep) {
    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject checkInIFCResponse = responseDoc.object();
      QJsonObject response = checkInIFCResponse["response"].toObject();

      if (!containsError(response)) {
        QString topicId = QString::number(response["result"].toInt());
        sendGetProgressRequest(topicId, "checkInIFC");
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
  }

  void BIMserverConnection::getIFCRevisionList(QString projectID) {
    if (!projectID.isEmpty()) {
      sendGetProjectByIDRequest(projectID);
    } else {
      emit errorOccured(QString("Project ID empty!"));
    }
  }

  void BIMserverConnection::sendGetProjectByIDRequest(QString projectID) {
    QJsonObject parameters;
    parameters["poid"] = QJsonValue(projectID);

    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1ServiceInterface"));
    request["method"] = QJsonValue(QString("getAllRevisionsOfProject"));
    request["parameters"] = parameters;

    QJsonObject getIFCRevisionListRequest;
    getIFCRevisionListRequest["token"] = QJsonValue(m_token);
    getIFCRevisionListRequest["request"] = request;

    QJsonDocument doc;
    doc.setObject(getIFCRevisionListRequest);

    QByteArray getIFCRevisionListRequestJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &BIMserverConnection::processGetProjectByIDRequest);
    m_networkManager->post(qNetworkRequest, getIFCRevisionListRequestJson);

  }

  void BIMserverConnection::processGetProjectByIDRequest(QNetworkReply *rep) {
    if (rep) {
      QByteArray responseArray = rep->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject responseObj = responseDoc.object();

      QJsonObject response = responseObj["response"].toObject();

      if (!containsError(response)) {
        QJsonArray result = response["result"].toArray();
        QStringList revisionList;
        foreach(const QJsonValue & value, result) {
          QJsonObject ifcRevision = value.toObject();
          QString revision = QString::number(ifcRevision["oid"].toInt());
          double time = ifcRevision["date"].toDouble();
          QDateTime timestamp;
          timestamp.setTime_t(static_cast<unsigned int>(time / 1000 + 0.5));
          QString project = revision.append(":").append(timestamp.toString(Qt::SystemLocaleShortDate));

          revisionList.append(revision);
        }
        m_ifcList = revisionList;
        m_operationDone = true;
        emit listAllIFCRevisions(revisionList);
      } else {
        emitErrorMessage(response);
      }

    } else {
      emit bimserverError();
    }
    
  }

  void BIMserverConnection::sendGetProgressRequest(QString topicId, QString action) {
    QJsonObject parameters;
    parameters["topicId"] = QJsonValue(topicId);

    QJsonObject request;
    request["interface"] = QJsonValue(QString("Bimsie1NotificationRegistryInterface"));
    request["method"] = QJsonValue(QString("getProgress"));
    request["parameters"] = parameters;

    QJsonObject getProgressRequest;
    getProgressRequest["token"] = QJsonValue(m_token);
    getProgressRequest["request"] = request;
    
    QJsonDocument doc;
    doc.setObject(getProgressRequest);

    QByteArray getProgressRequestJson = doc.toJson();

    //setup network connection
    QNetworkRequest qNetworkRequest(m_bimserverURL);
    qNetworkRequest.setRawHeader("Content-Type", "application/json");

    QNetworkReply *reply = m_networkManager->post(qNetworkRequest, getProgressRequestJson);
    reply->setProperty("sender", QVariant(action));
    disconnect(m_networkManager, nullptr, this, nullptr);
    disconnect(reply, nullptr, this, nullptr);
    connect(reply, &QNetworkReply::finished, this, &BIMserverConnection::processGetProgressRequest);
  }

  void BIMserverConnection::processGetProgressRequest() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
      QByteArray responseArray = reply->readAll();

      QJsonDocument responseDoc = QJsonDocument::fromJson(responseArray);
      QJsonObject downloadResponse = responseDoc.object();
      QJsonObject response = downloadResponse["response"].toObject();

      if (response["errors"].toArray().isEmpty()) {
        QString sender = reply->property("sender").toString();

        if (sender == "download") {
          sendGetDownloadDataRequest();
        } else if (sender == "checkInIFC") {
          m_checkInIFCSuccess = true;
          m_operationDone = true;
          emit operationSucceeded(QString("checkInIFC"));
        }
      } else {
        QJsonArray error = response["errors"].toArray();
        QString errorMessage;
        foreach(const QJsonValue & value, error) {
          errorMessage = errorMessage + value.toString() + QString("\n");
          }
        emit errorOccured(errorMessage);
      }
    } else {
      emit errorOccured(QString("No Response from BIMserver"));
    }

    reply->deleteLater();
  }

  //BIMserver error reporting
  bool BIMserverConnection::containsError(QJsonObject responseMessage) {

    if (responseMessage.isEmpty()) {
      return true;
    }
    else {
      if (responseMessage.contains("exception")) {
        return true;
      } else {
        return false;
      }
    }
  }

  void BIMserverConnection::emitErrorMessage(QJsonObject responseMessage) {

    if (responseMessage.isEmpty()) {
      emit bimserverError();
    }
    else { 
      QJsonObject exception = responseMessage["exception"].toObject();
      QString errorMessage = exception["message"].toString();
      emit errorOccured(errorMessage);
    }
  }


  bool BIMserverConnection::loginBlocked(QString username, QString password, int timeout) {
    m_loginSuccess = false;
    login(username, password);
    waitForLock(timeout);
    return m_loginSuccess;
  }

  boost::optional<QString> BIMserverConnection::downloadBlocked(QString projectID, int timeout) {
    m_osmModel=boost::none;
    download(projectID);
    waitForLock(timeout);
    return m_osmModel;
  }

  boost::optional<QStringList> BIMserverConnection::getAllProjectsBlocked(int timeout) {
    m_projectList = boost::none;
    getAllProjects();
    waitForLock(timeout);
    return m_projectList;
  }

  bool BIMserverConnection::createProjectBlocked(QString projectName, int timeout) {
    m_createProjectSuccess = false;
    createProject(projectName);
    waitForLock(timeout);
    return m_createProjectSuccess;
  }

  bool BIMserverConnection::deleteProjectBlocked(QString projectID, int timeout) {
    m_deleteProjectSuccess = false;
    deleteProject(projectID);
    waitForLock(timeout);
    return m_deleteProjectSuccess;
  }

  bool BIMserverConnection::checkInIFCFileBlocked(QString projectID, QString IFCFilePath, int timeout) {
    m_checkInIFCSuccess = false;
    checkInIFCFile(projectID, IFCFilePath);
    waitForLock(timeout);
    return m_checkInIFCSuccess;
  }

  boost::optional<QStringList> BIMserverConnection::getIFCRevisionListBlocked(QString projectID, int timeout) {
    m_ifcList = boost::none;
    getIFCRevisionList(projectID);
    waitForLock(timeout);
    return m_ifcList;
  }
  
  bool BIMserverConnection::waitForLock(int msec) const {
    int msecPerLoop = 20;
    int numTries = msec / msecPerLoop;
    int current = 0;
    while (true) {

      // if we can get the lock then the download is complete
      if (m_operationDone) {
        return true;
      }

      // this calls process events
      System::msleep(msecPerLoop);

      if (current > numTries) {
        LOG(Error, "waitForLock timeout");
        break;
      }

      ++current;
    }

    return false;
  }

} // bimserver
} // openstudio
