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
#include "LocalBCL.hpp"
#include "OnDemandGenerator.hpp"
#include "RemoteBCL.hpp"
#include "../core/Application.hpp"
#include "../core/Assert.hpp"
#include "../core/Path.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/System.hpp"
#include "../core/UnzipFile.hpp"
#include "../core/ZipFile.hpp"

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslError>
#include <QTextStream>
#include <QSslSocket>
#include <QUrlQuery>

#define REMOTE_PRODUCTION_SERVER "https://bcl.nrel.gov"
#define REMOTE_DEVELOPMENT_SERVER "http://bcl7.development.nrel.gov"

namespace openstudio{

  std::ostream& operator<<(std::ostream& os, const QDomElement& element)
  {
    QString str;
    QTextStream qts(&str);
    element.save(qts, 2);
    os << str.toStdString();
    return os;
  }

  RemoteQueryResponse::RemoteQueryResponse(const QDomDocument& domDocument)
    : m_domDocument(domDocument)
  {
  }

  QDomDocument RemoteQueryResponse::domDocument() const
  {
    return m_domDocument;
  }

  RemoteBCL::RemoteBCL():
    m_networkManager(new QNetworkAccessManager()),
    m_mutex(new QMutex()),
    m_numResultsPerQuery(10),
    m_apiVersion("2.0")
  {
    // make sure application is initialized
    openstudio::Application::instance().application(true);

    m_prodAuthKey = LocalBCL::instance().prodAuthKey();
    m_devAuthKey = LocalBCL::instance().devAuthKey();
    validProdAuthKey = false;
    validDevAuthKey = false;

    useRemoteProductionUrl();

#ifndef QT_NO_OPENSSL
    bool isConnected = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(isConnected);
#endif
  }

  RemoteBCL::~RemoteBCL()
  {
    delete m_networkManager;
    delete m_mutex;
  }

  bool RemoteBCL::initializeSSL(const openstudio::path &t_pathToSSLLibraries)
  {
    QByteArray oldpath = qgetenv("PATH");
    if (!t_pathToSSLLibraries.empty())
    {
      qputenv("PATH", openstudio::toQString(t_pathToSSLLibraries.string()).toUtf8());
    }

#ifdef QT_NO_OPENSSL
    bool opensslloaded = false;
#else
    bool opensslloaded = QSslSocket::supportsSsl();
#endif

    if (!t_pathToSSLLibraries.empty())
    {
      qputenv("PATH", oldpath);
    }

    return opensslloaded;
  }

  
  ///////////////////////////////////////////////////////////////////////////
  /// Inherited members
  ///////////////////////////////////////////////////////////////////////////

  boost::optional<BCLComponent> RemoteBCL::getComponent(const std::string& uid, const std::string& versionId) const
  {
    bool downloadStarted = const_cast<RemoteBCL*>(this)->downloadComponent(uid);
    if(downloadStarted){
      return waitForComponentDownload();
    }
    return boost::none;
  }

  boost::optional<BCLMeasure> RemoteBCL::getMeasure(const std::string& uid, const std::string& versionId) const
  {
    bool downloadStarted = const_cast<RemoteBCL*>(this)->downloadMeasure(uid);
    if(downloadStarted){
      return waitForMeasureDownload();
    }
    return boost::none;
  }

  boost::optional<BCLComponent> RemoteBCL::getOnDemandComponent(const OnDemandGenerator& generator) const
  {
    bool downloadStarted = const_cast<RemoteBCL*>(this)->callOnDemandGenerator(generator);
    if(downloadStarted){
      return waitForComponentDownload();
    }
    return boost::none;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::metaSearchComponentLibrary(const std::string& searchTerm,
    const std::string& componentType, const std::string& filterType) const
  {
    bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibraryMetaSearch(searchTerm, componentType, filterType);
    if(searchStarted){
      return waitForMetaSearch();
    }
    return boost::none;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::metaSearchComponentLibrary(const std::string& searchTerm,
    const unsigned componentTypeTID, const std::string& filterType) const
  {
    bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibraryMetaSearch(searchTerm, componentTypeTID, filterType);
    if(searchStarted){
      return waitForMetaSearch();
    }
    return boost::none;
  }

  std::vector<BCLSearchResult> RemoteBCL::searchComponentLibrary(const std::string& searchTerm,
    const std::string& componentType, const unsigned page) const
  {
    // Perform metaSearch first
    metaSearchComponentLibrary(searchTerm, componentType, "nrel_component");
    if (lastTotalResults() == 0){
      return std::vector<BCLSearchResult>();
    }

    bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentType, "nrel_component", page);
    if(searchStarted){
      return waitForSearch();
    }
    return std::vector<BCLSearchResult>();
  }

  std::vector<BCLSearchResult> RemoteBCL::searchComponentLibrary(const std::string& searchTerm,
    const unsigned componentTypeTID, const unsigned page) const
  {
    // Perform metaSearch first
    metaSearchComponentLibrary(searchTerm, componentTypeTID, "nrel_component");
    if (lastTotalResults() == 0){
      return std::vector<BCLSearchResult>();
    }

    bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentTypeTID, "nrel_component", page);
    if(searchStarted){
      return waitForSearch();
    }
    return std::vector<BCLSearchResult>();
  }

  std::vector<BCLSearchResult> RemoteBCL::searchMeasureLibrary(const std::string& searchTerm,
    const std::string& componentType, const unsigned page) const
  {
    // Perform metaSearch first
    metaSearchComponentLibrary(searchTerm, componentType, "nrel_measure");
    if (lastTotalResults() == 0){
      return std::vector<BCLSearchResult>();
    }

    bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentType, "nrel_measure", page);
    if(searchStarted){
      return waitForSearch();
    }
    return std::vector<BCLSearchResult>();
  }

  std::vector<BCLSearchResult> RemoteBCL::searchMeasureLibrary(const std::string& searchTerm,
    const unsigned componentTypeTID, const unsigned page) const
  {
    // Perform metaSearch first
    metaSearchComponentLibrary(searchTerm, componentTypeTID, "nrel_measure");
    if (lastTotalResults() == 0){
      return std::vector<BCLSearchResult>();
    }

    bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentTypeTID, "nrel_measure", page);
    if(searchStarted){
      return waitForSearch();
    }
    return std::vector<BCLSearchResult>();
  }

  int RemoteBCL::checkForComponentUpdates()
  {
    m_componentsWithUpdates.clear();

    for (const BCLComponent& component : LocalBCL::instance().components()) {
      // can't start another search until the last one is done
      if (!m_mutex->tryLock()){
        return 0;
      }

      // disconnect all signals from m_networkManager to this
      disconnect(m_networkManager, nullptr, this, nullptr); // returns a bool, but was not checking, so removed

      const_cast<RemoteBCL*>(this)->m_lastSearch.clear();

      QString url = toQString(remoteUrl() + "/api/search/?fq[]=ss_uuid:%1&api_version=%2").arg(
        toQString(component.uid()),
        toQString(m_apiVersion)
      );
      //LOG(Warn, toString(url));

      // when the reply is finished call onSearchResponseComplete
      bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSearchResponseComplete(QNetworkReply*)));
      OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
      test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
        this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
      OS_ASSERT(test);
#endif

      QNetworkRequest request = QNetworkRequest(QUrl(url));
      request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
      request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
      m_networkManager->get(request);
      std::vector<BCLSearchResult> result = waitForSearch();
      if (result.size() > 0 && result[0].versionId() != component.versionId()){
        m_componentsWithUpdates.push_back(result[0]);
      }
    }

    return m_componentsWithUpdates.size();
  }

  int RemoteBCL::checkForMeasureUpdates()
  {
    m_measuresWithUpdates.clear();

    for (const BCLMeasure& measure : LocalBCL::instance().measures()) {
      // can't start another search until the last one is done
      if (!m_mutex->tryLock()){
        return 0;
      }

      // disconnect all signals from m_networkManager to this
      disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but no check, so removed

      const_cast<RemoteBCL*>(this)->m_lastSearch.clear();

      QString url = toQString(remoteUrl() + "/api/search/?fq[]=ss_uuid:%1&api_version=%2").arg(
        toQString(measure.uid()),
        toQString(m_apiVersion)
      );
      //LOG(Warn, toString(url));

      // when the reply is finished call onSearchResponseComplete
      bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSearchResponseComplete(QNetworkReply*)));
      OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
      test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
        this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
      OS_ASSERT(test);
#endif

      QNetworkRequest request = QNetworkRequest(QUrl(url));
      request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
      request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
      m_networkManager->get(request);
      std::vector<BCLSearchResult> result = waitForSearch();
      if (result.size() > 0 && result[0].versionId() != measure.versionId()){
        m_measuresWithUpdates.push_back(result[0]);
      }
    }

    return m_measuresWithUpdates.size();
  }
    
  std::vector<BCLSearchResult> RemoteBCL::componentsWithUpdates() const
  {
    return m_componentsWithUpdates;
  }

  std::vector<BCLSearchResult> RemoteBCL::measuresWithUpdates() const
  {
    return m_measuresWithUpdates;
  }

  void RemoteBCL::updateComponents()
  {
    if (m_componentsWithUpdates.size() == 0){
      checkForComponentUpdates();
    }

    for (const BCLSearchResult& component : m_componentsWithUpdates) {
      downloadMeasure(component.uid());
      boost::optional<BCLComponent> newComponent = waitForComponentDownload();

      if (newComponent){
        boost::optional<BCLComponent> oldComponent = LocalBCL::instance().getComponent(newComponent->uid());
        if (oldComponent && oldComponent->versionId() != newComponent->versionId()){
          LocalBCL::instance().removeComponent(*oldComponent);
        }
      }
    }
  }

  void RemoteBCL::updateMeasures()
  {
    if (m_measuresWithUpdates.size() == 0){
      checkForMeasureUpdates();
    }

    for (const BCLSearchResult& measure : m_measuresWithUpdates) {
      downloadMeasure(measure.uid());
      boost::optional<BCLMeasure> newMeasure = waitForMeasureDownload();

      if (newMeasure){
        boost::optional<BCLMeasure> oldMeasure = LocalBCL::instance().getMeasure(newMeasure->uid());
        if (oldMeasure && oldMeasure->versionId() != newMeasure->versionId()){
          LocalBCL::instance().removeMeasure(*oldMeasure);
        }
      }
    }
  }


  ///////////////////////////////////////////////////////////////////////////
  /// Blocking class members
  ///////////////////////////////////////////////////////////////////////////

  boost::optional<BCLComponent> RemoteBCL::lastComponentDownload() const
  {
    return m_lastComponentDownload;
  }

  boost::optional<BCLMeasure> RemoteBCL::lastMeasureDownload() const
  {
    return m_lastMeasureDownload;
  }

  boost::optional<OnDemandGenerator> RemoteBCL::lastOnDemandGenerator() const
  {
    return m_lastOnDemandGenerator;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::lastMetaSearch() const
  {
    return m_lastMetaSearch;
  }

  std::vector<BCLSearchResult> RemoteBCL::lastSearch() const
  {
    return m_lastSearch;
  }

  std::string RemoteBCL::remoteUrl() const
  {
    return m_remoteUrl;
  }

  std::string RemoteBCL::remoteProductionUrl() const
  {
    return std::string(REMOTE_PRODUCTION_SERVER);
  }

  std::string RemoteBCL::remoteDevelopmentUrl() const
  {
    return std::string(REMOTE_DEVELOPMENT_SERVER);
  }

  void RemoteBCL::useRemoteDevelopmentUrl()
  {
    m_useRemoteDevelopmentUrl = true;
    m_remoteUrl = remoteDevelopmentUrl();
    m_authKey = m_devAuthKey;
  }

  void RemoteBCL::useRemoteProductionUrl()
  {
    m_useRemoteDevelopmentUrl = false;
    m_remoteUrl = remoteProductionUrl();
    m_authKey = m_prodAuthKey;
  }

  std::string RemoteBCL::authKey() const
  {
    return m_authKey;
  }

  std::string RemoteBCL::prodAuthKey() const
  {
    return m_prodAuthKey;
  }

  bool RemoteBCL::setProdAuthKey(const std::string& prodAuthKey)
  {
    bool previousValidity = validProdAuthKey;
    if (validateAuthKey(prodAuthKey, remoteProductionUrl())){
      m_prodAuthKey = prodAuthKey;
      if (!m_useRemoteDevelopmentUrl){
        m_authKey = prodAuthKey;
      }
      return true;
    }
    validProdAuthKey = previousValidity;
    return false;
  }

  std::string RemoteBCL::devAuthKey() const
  {
    return m_devAuthKey;
  }

  bool RemoteBCL::setDevAuthKey(const std::string& devAuthKey)
  {
    bool previousValidity = validDevAuthKey;
    if (validateAuthKey(devAuthKey, remoteDevelopmentUrl())){
      m_devAuthKey = devAuthKey;
      if (m_useRemoteDevelopmentUrl){
        m_authKey = devAuthKey;
      }
      return true;
    }
    validDevAuthKey = previousValidity;
    return false;
  }

  int RemoteBCL::resultsPerQuery() const
  {
    return m_numResultsPerQuery;
  }

  int RemoteBCL::lastTotalResults() const
  {
    return m_lastTotalResults;
  }

  int RemoteBCL::numResultPages() const
  {
    double numerator(lastTotalResults());
    double denominator(resultsPerQuery());
    return int(std::ceil(numerator/denominator));
  }

  bool RemoteBCL::validateAuthKey(const std::string& authKey, const std::string& remoteUrl)
  {
    if (authKey.length() == 32)
    {
      std::string previousUrl = this->remoteUrl();

      // Check if validation has already run for the given key
      if (remoteUrl == remoteProductionUrl() && authKey == prodAuthKey() && validProdAuthKey) {
        return true;
      }
      else if (remoteUrl == remoteDevelopmentUrl() && authKey == devAuthKey() && validDevAuthKey) {
        return true;
      }

      // Temporarily set url
      if (remoteUrl == remoteDevelopmentUrl()){
        useRemoteDevelopmentUrl();
      }else{
        useRemoteProductionUrl();
      }

      // Run validation query
      if (!m_mutex->tryLock()){
        return false;
      }

      const_cast<RemoteBCL*>(this)->m_lastSearch.clear();

      QString url = toQString(remoteUrl + "/api/search/?api_version=%1&show_rows=0").arg(
        toQString(m_apiVersion)
      );
      //LOG(Warn, toString(url));

      // disconnect all signals from m_networkManager to this
      disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed

      // when the reply is finished call onSearchResponseComplete
      bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSearchResponseComplete(QNetworkReply*)));
      OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
      test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
        this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
      OS_ASSERT(test);
#endif

      QNetworkRequest request = QNetworkRequest(QUrl(url));
      request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
      request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
      m_networkManager->get(request);
      waitForSearch();

      // Restore url
      if (previousUrl == remoteDevelopmentUrl()){
        useRemoteDevelopmentUrl();
      }else{
        useRemoteProductionUrl();
      }
      
      if (remoteUrl == remoteDevelopmentUrl()){
        return validDevAuthKey;
      }else{
        return validProdAuthKey;
      }
    }
    return false;
  }

  boost::optional<BCLComponent> RemoteBCL::waitForComponentDownload(int msec) const
  {
    waitForLock(msec);
    return m_lastComponentDownload;
  }

  boost::optional<BCLMeasure> RemoteBCL::waitForMeasureDownload(int msec) const
  {
    waitForLock(msec);
    return m_lastMeasureDownload;
  }

  boost::optional<OnDemandGenerator> RemoteBCL::waitForOnDemandGenerator(int msec) const
  {
    waitForLock(msec);
    return m_lastOnDemandGenerator;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::waitForMetaSearch(int msec) const
  {
    waitForLock(msec);
    return m_lastMetaSearch;
  }

  std::vector<BCLSearchResult> RemoteBCL::waitForSearch(int msec) const
  {
    waitForLock(msec);
    return m_lastSearch;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// Non-blocking class members
  ///////////////////////////////////////////////////////////////////////////

  bool RemoteBCL::downloadComponent(const std::string& uid)
  {
    // Check for empty uid
    if (uid.empty()){
      LOG(Error, "Error: No uid provided");
      return false;
    }

    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_downloadFile = std::shared_ptr<QFile>(new QFile(QDir::tempPath().append(toQString("/"+uid+".bcl"))));
    if (!m_downloadFile->open(QIODevice::WriteOnly | QIODevice::Truncate)){
      m_mutex->unlock();
      return false;
    }
    
    m_downloadUid = uid;

    QUrl url = toQString(remoteUrl() + "/api/component/download");

    QByteArray data;
    QUrlQuery query;
    query.addQueryItem("uids", toQString(uid));
    //query.addQueryItem("api_version", toQString(m_apiVersion));
    url.setQuery(query);

    data.append(url.query());
    LOG(Warn, url.toString().toStdString());

    QNetworkRequest request(toQString(remoteUrl() + "/api/component/download"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");

    // for testing
    std::string testString = url.toString().toStdString();
    LOG(Info, testString);

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onDownloadComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    m_downloadReply = m_networkManager->post(request, data);
        
    test = connect(m_downloadReply, SIGNAL(readyRead()), this, SLOT(downloadData()));
    OS_ASSERT(test);

    return true;
  }

  bool RemoteBCL::downloadMeasure(const std::string& uid)
  {
    return downloadComponent(uid);
  }

  bool RemoteBCL::downloadOnDemandGenerator(const std::string& uid)
  {
    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_lastOnDemandGenerator.reset();

    QString url = toQString(remoteUrl() + "/api/ondemandhelp/%1").arg(toQString(uid));
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onOnDemandGeneratorResponseComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onOnDemandGeneratorResponseComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::callOnDemandGenerator(const OnDemandGenerator& generator)
  {
    // check if generator has all arguments it needs
    if (!generator.checkArgumentValues()){
      return false;
    }

    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_downloadFile = std::shared_ptr<QFile>(new QFile(QDir::tempPath().append(toQString("/"+generator.uid()+".bcl"))));
    if (!m_downloadFile->open(QIODevice::WriteOnly | QIODevice::Truncate)){
      m_mutex->unlock();
      return false;
    }
    
    m_downloadUid = generator.uid();

    QUrl url = toQString(remoteUrl() + "/api/ondemand/%1").arg(toQString(generator.uid()));

    QString arguments;
    std::vector<OnDemandGeneratorArgument> activeArguments = generator.activeArguments();
    for(std::vector<OnDemandGeneratorArgument>::const_iterator it = activeArguments.begin(), itend = activeArguments.end(); it != itend; ++it){
      
      arguments += toQString(it->name() + ":");

      if (it->dataType() == OnDemandGeneratorArgumentType::String){
        boost::optional<std::string> value = it->valueAsString();
        OS_ASSERT(value);
        arguments += toQString(*value);
      }else if (it->dataType() == OnDemandGeneratorArgumentType::Float){
        boost::optional<double> value = it->valueAsDouble();
        OS_ASSERT(value);
        arguments += QString::number(*value);
      }else if (it->dataType() == OnDemandGeneratorArgumentType::Integer){
        boost::optional<int> value = it->valueAsInteger();
        OS_ASSERT(value);
        arguments += QString::number(*value);
      }else{
        OS_ASSERT(false);
      }

      if (it < itend-1){
        arguments += "|";
      }
    }
    QUrlQuery query;
    query.addQueryItem("arguments", arguments);
    url.setQuery(query);
    

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");

    // for testing
    //LOG(Warn, url.toString().toStdString());

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onDownloadComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    m_downloadReply = m_networkManager->get(request);

    test = connect(m_downloadReply, SIGNAL(readyRead()), this, SLOT(downloadData()));
    OS_ASSERT(test);

    return true;
  }

  bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType)
  {
    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_lastMetaSearch.reset();

    QString url;
    if (componentType.empty() || componentType == "*"){
      url = toQString(remoteUrl() + "/api/metasearch/%1?fq[]=bundle:%2&api_version=%3").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(m_apiVersion)
      );
    }else{
      url = toQString(remoteUrl() + "/api/metasearch/%1?fq[]=bundle:%2&fq[]=%3:\"%4\"&api_version=%5").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        (filterType == "nrel_component" ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags"),
        toQString(componentType),
        toQString(m_apiVersion)
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onMetaSearchResponseComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onMetaSearchResponseComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType)
  {
    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_lastMetaSearch.reset();

    QString url;
    if (componentTypeTID == 0){
      url = toQString(remoteUrl() + "/api/metasearch/%1?fq[]=bundle:%2&api_version=%3").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(m_apiVersion)
      );
    }else{
      url = toQString(remoteUrl() + "/api/metasearch/%1?fq[]=bundle:%2&fq[]=tid:%3&api_version=%4").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        QString::number(componentTypeTID),
        toQString(m_apiVersion)
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onMetaSearchResponseComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onMetaSearchResponseComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType, const unsigned page)
  {
    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_lastSearch.clear();

    QString url;
    if (componentType.empty() || componentType == "*"){
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&api_version=%3&show_rows=%4&page=%5").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(m_apiVersion),
        QString::number(m_numResultsPerQuery),
        QString::number(page)
      );
    }else{
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&fq[]=%3:\"%4\""
        "&api_version=%5&show_rows=%6&page=%7").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        (filterType == "nrel_component" ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags"),
        toQString(componentType),
        toQString(m_apiVersion),
        QString::number(m_numResultsPerQuery),
        QString::number(page)
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onSearchResponseComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSearchResponseComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType, const unsigned page)
  {
    // can't start another download until last one is done
    if (!m_mutex->tryLock()){
      return false;
    }

    m_lastSearch.clear();

    QString url;
    if (componentTypeTID == 0){
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&api_version=%3&show_rows=%4&page=%5").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(m_apiVersion),
        QString::number(m_numResultsPerQuery),
        QString::number(page)
      );
    }else{
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&fq[]=tid:%3&api_version=%4&show_rows=%5&page=%6").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        QString::number(componentTypeTID),
        toQString(m_apiVersion),
        QString::number(m_numResultsPerQuery),
        QString::number(page)
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onSearchResponseComplete
    bool test = connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSearchResponseComplete(QNetworkReply*)));
    OS_ASSERT(test);

#ifndef QT_NO_OPENSSL
    test = connect(m_networkManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), 
      this, SLOT(catchSslErrors(QNetworkReply*, const QList<QSslError>&)));
    OS_ASSERT(test);
#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::waitForLock(int msec) const
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
        LOG(Error, "waitForLock timeout");
        break;
      }

      ++current;
    }

    return false;
  }

  boost::optional<RemoteQueryResponse> RemoteBCL::processReply(QNetworkReply* reply)
  {
    if (!reply){
      LOG(Error, "Error: Empty reply");
    }else{
      //Print response headers
      /*for(int i=0; i<reply->rawHeaderList().size(); ++i){
        QString str(reply->rawHeaderList()[i].constData());
        std::cout << toString(str) << ": " << toString(reply->rawHeader(reply->rawHeaderList()[i] ).constData()) << std::endl;
      }*/
      if (reply->error() == QNetworkReply::NoError){
        QDomDocument document;
        QString error;
        if (document.setContent(reply, &error)){
          if (!m_useRemoteDevelopmentUrl){
            validProdAuthKey = true;
          } else {
            validDevAuthKey = true;
          }
          return RemoteQueryResponse(document);
        }else{
          LOG(Error, "Bad XML Response: " << toString(error));
        }
      }else if (reply->error() == QNetworkReply::AuthenticationRequiredError){
        if (!m_useRemoteDevelopmentUrl){
          if (m_prodAuthKey.empty()){
            LOG(Error, "Error: prodAuthKey missing");
          }else{
            LOG(Error, "Error: Invalid prodAuthKey");
          }
          validProdAuthKey = false;
        }else{
          if (m_devAuthKey.empty()){
            LOG(Error, "Error: devAuthKey missing");
          }else{
            LOG(Error, "Error: Invalid devAuthKey");
          }
          validDevAuthKey = false;
        }
      }else if (reply->error() == QNetworkReply::ConnectionRefusedError){
        LOG(Error, "Network Error: " << "Connection failed");
      }else if (reply->error() == QNetworkReply::UnknownContentError){
        LOG(Error, "Network Error: Unknown Content.  Verify the User-Agent request header");
      }else if (reply->error() == QNetworkReply::HostNotFoundError){
        LOG(Error, "Network Error: Host " << remoteUrl() << " not found");
      }else if (reply->error() == QNetworkReply::UnknownNetworkError && reply->errorString().startsWith("Error creating SSL context")){
        LOG(Error, "Network Error: Unable to create SSL connection.  Verify that SSL libraries are in the system path.");
        //QMessageBox::warning(0, "Unable to Create SSL Connection", "Verify that SSL libraries are in the system path");
      }else{
        LOG(Error, "Network Error: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << " - " << reply->errorString().toStdString());
      }
    }
    return boost::none;
  }

  boost::optional<OnDemandGenerator> RemoteBCL::processOnDemandGeneratorResponse(const RemoteQueryResponse& remoteQueryResponse) const
  {
    QDomElement root = remoteQueryResponse.domDocument().documentElement();

    if (!root.isNull() && (root.tagName() == "ondemand")){
      QDomElement nameElement = root.firstChildElement("name");
      if (!nameElement.isNull()){
        return OnDemandGenerator(root);
      }
    }

    return boost::none;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::processMetaSearchResponse(const RemoteQueryResponse& remoteQueryResponse) const
  {
    QDomElement root = remoteQueryResponse.domDocument().documentElement();

    if (!root.isNull() && (root.tagName().startsWith("result"))){
      QDomElement numResultsElement = root.firstChildElement("result_count");
      if (!numResultsElement.isNull()){
        return BCLMetaSearchResult(root);
      }
    }

    return boost::none;
  }

  std::vector<BCLSearchResult> RemoteBCL::processSearchResponse(const RemoteQueryResponse& remoteQueryResponse) const
  {
    std::vector<BCLSearchResult> searchResults;

    QDomElement root = remoteQueryResponse.domDocument().documentElement();
    QDomElement result = root.firstChildElement("result");
    if (result.isNull()){
      if (root.hasChildNodes()){
        // single result
        BCLSearchResult searchResult(root);
        searchResults.push_back(searchResult);
      }
    }else{
      // many results
      QDomElement componentElement = result.firstChildElement();

      //Basic check to see if it's non-empty
      while (!componentElement.firstChildElement("name").isNull())
      {
        //Skip components without a uid or version_id
        if (!componentElement.firstChildElement("uuid").isNull() && !componentElement.firstChildElement("vuuid").isNull())
        {
          BCLSearchResult searchResult(componentElement);
          searchResults.push_back(searchResult);
        }
        result = result.nextSiblingElement("result");
        componentElement = result.firstChildElement();
      }
    }

    return searchResults;
  }

  int RemoteBCL::setResultsPerQuery(const int numResults)
  {
    m_numResultsPerQuery = numResults <= 0 ? 10 : numResults > 100 ? 100 : numResults;
    return m_numResultsPerQuery;
  }

  void RemoteBCL::setLastTotalResults(const int lastTotalResults)
  {
    m_lastTotalResults = lastTotalResults;
  }


  ///////////////////////////////////////////////////////////////////////////
  /// Slots
  ///////////////////////////////////////////////////////////////////////////

  void RemoteBCL::downloadData()
  {
    OS_ASSERT(m_downloadReply);
    m_downloadFile->write(m_downloadReply->readAll());
  }

  void RemoteBCL::onDownloadComplete(QNetworkReply* reply)
  {
    m_downloadFile->close();
    std::string componentType;

    if (!reply){
      LOG(Error, "Empty reply");
    }else{
      if (reply->error() == QNetworkReply::NoError){
        //Print response headers
        /*for(int i=0; i<reply->rawHeaderList().size(); ++i){
          QString str(reply->rawHeaderList()[i].constData());
          std::cout << toString(str) << ": " << toString(reply->rawHeader(reply->rawHeaderList()[i] ).constData()) << std::endl;
        }*/
        
        // Extract the files to a temp location
        openstudio::path src = toPath(QDir::tempPath().append(toQString("/"+m_downloadUid+".bcl")));
        openstudio::path tempDest = toPath(QDir::tempPath().append(toQString("/"+m_downloadUid+"/")));

        if (QDir().exists(toQString(tempDest))){
          removeDirectory(tempDest);
        }
        QDir().mkpath(toQString(tempDest));

        openstudio::UnzipFile uf(src);
        std::vector<openstudio::path> createdFiles = uf.extractAllFiles(tempDest);
        QFile::remove(toQString(src));

        // search for component.xml or measure.xml file
        boost::optional<openstudio::path> xmlPath;
        
        for (const openstudio::path& path : createdFiles) {
          if (path.filename() == toPath("component.xml")) {
            componentType = "component";
            m_lastComponentDownload.reset();
            xmlPath = path;
            break;
          } else if (path.filename() == toPath("measure.xml")) {
            componentType = "measure";
            m_lastMeasureDownload.reset();
            xmlPath = path;
            break;
          }
        }

        if (xmlPath){
          path src = xmlPath->parent_path();
          path dest = src.parent_path();
          QFile::remove(toQString(dest / toPath("DISCLAIMER.txt")));
          QFile::remove(toQString(dest / toPath("README.txt")));
          QFile::remove(toQString(dest / toPath("output.xml")));
          copyDirectory(src, dest);
          removeDirectory(src);

          if (componentType == "component")
          {
            path componentXmlPath = dest / toPath("component.xml");
            // open the component to figure out uid and vid
            BCLComponent component(toString(componentXmlPath.parent_path()));
            std::string uid = component.uid();
            std::string versionId = component.versionId();

            // check if component has proper uid and vid
            if (!uid.empty() && !versionId.empty()){

              dest = toPath(LocalBCL::instance().libraryPath().append(toQString("/"+uid+"/"+versionId)));

              removeDirectory(dest);
              if (copyDirectory(componentXmlPath.parent_path(), dest))
              {
                // Add to LocalBCL
                m_lastComponentDownload = BCLComponent(toString(dest));
                LocalBCL::instance().addComponent(*m_lastComponentDownload);
              }
            }
          } else if (componentType == "measure") {
            path measureXmlPath = dest / toPath("measure.xml");
            // open the measure to figure out uid and vid
            boost::optional<BCLMeasure> measure;
            try{
              measure = BCLMeasure(measureXmlPath.parent_path());

              std::string uid = measure->uid();
              std::string versionId = measure->versionId();

              // check if component has proper uid and vid
              if (!uid.empty() && !versionId.empty()){

                dest = toPath(LocalBCL::instance().libraryPath().append(toQString("/"+uid+"/"+versionId)));

                removeDirectory(dest);
                if (copyDirectory(measureXmlPath.parent_path(), dest))
                {
                  // Add to LocalBCL
                  m_lastMeasureDownload = BCLMeasure(dest);
                  LocalBCL::instance().addMeasure(*m_lastMeasureDownload);
                }
              }
            }catch(const std::exception&){
              LOG(Error, "Unable to create measure from download: " + toString(measureXmlPath.parent_path()));
            }
          }
        }else{
          LOG(Error, "No component.xml or measure.xml file found in downloaded contents");
        }

        // delete the temp unzip directory
        removeDirectory(tempDest);

      } else {
        LOG(Error, "Network Error: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
      }
    }

    if (componentType == "measure") {
      emit measureDownloaded(m_downloadUid, m_lastMeasureDownload);
    } else {
      emit componentDownloaded(m_downloadUid, m_lastComponentDownload);
    }

    reply->deleteLater();
    m_mutex->unlock();
  }

  void RemoteBCL::onOnDemandGeneratorResponseComplete(QNetworkReply* reply)
  {
    boost::optional<RemoteQueryResponse> remoteQueryResponse = this->processReply(reply);
    //Print response headers
    /*for(int i=0; i<reply->rawHeaderList().size(); ++i){
      QString str(reply->rawHeaderList()[i].constData());
      std::cout << toString(str) << ": " << toString(reply->rawHeader(reply->rawHeaderList()[i] ).constData()) << std::endl;
    }*/

    if (remoteQueryResponse){
      m_lastOnDemandGenerator = processOnDemandGeneratorResponse(*remoteQueryResponse);
    }

    emit onDemandGeneratorRecieved(m_downloadUid, m_lastOnDemandGenerator);

    reply->deleteLater();
    m_mutex->unlock();
  }

  void RemoteBCL::onMetaSearchResponseComplete(QNetworkReply* reply)
  {
    boost::optional<RemoteQueryResponse> remoteQueryResponse = this->processReply(reply);
    if (remoteQueryResponse){
      m_lastMetaSearch = processMetaSearchResponse(*remoteQueryResponse);
    }

    if (m_lastMetaSearch){
      setLastTotalResults(m_lastMetaSearch->numResults());
    }else{
      setLastTotalResults(0);
    }

    emit metaSearchCompleted(m_lastMetaSearch);

    reply->deleteLater();
    m_mutex->unlock();
  }

  void RemoteBCL::onSearchResponseComplete(QNetworkReply* reply)
  {
    boost::optional<RemoteQueryResponse> remoteQueryResponse = this->processReply(reply);
    if (remoteQueryResponse){
      m_lastSearch = processSearchResponse(*remoteQueryResponse);
    }

    emit searchCompleted(m_lastSearch);

    reply->deleteLater();
    m_mutex->unlock();
  }

  void RemoteBCL::catchSslErrors(QNetworkReply* reply, const QList<QSslError>& errorList)
  {
    /*for (const QSslError& error : errorList) {
      std::cout << error.errorString().toStdString() << std::endl;
    }*/
  }

} // openstudio
