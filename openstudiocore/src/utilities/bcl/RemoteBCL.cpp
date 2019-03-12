/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "LocalBCL.hpp"
#include "RemoteBCL.hpp"
#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/StringHelpers.hpp"
#include "../core/System.hpp"
#include "../core/UnzipFile.hpp"

#include <QNetworkReply>

#define REMOTE_PRODUCTION_SERVER "https://bcl.nrel.gov"
#define REMOTE_DEVELOPMENT_SERVER "http://bcl7.development.nrel.gov"

///
/// TODO: Remove this helper when Qt is fully removed
///
static auto toQString(const std::string &s) {
  return QString::fromStdString(s);
};

namespace openstudio{

  std::ostream& operator<<(std::ostream& os, const pugi::xml_document& element)
  {
    element.save(os, "  ");
    return os;
  }

  RemoteQueryResponse::RemoteQueryResponse(std::shared_ptr<pugi::xml_document>& domDocument)
    : m_domDocument(domDocument)
  {
  }

  pugi::xml_node RemoteQueryResponse::root() const
  {
    return m_domDocument->document_element();
  }


  bool RemoteBCL::DownloadFile::open()
  {
    OS_ASSERT(!m_fileName.empty());
    m_ofs.open(toString(m_fileName).c_str(), std::ios_base::trunc | std::ios_base::out);
    return m_ofs.good();
  }

  RemoteBCL::DownloadFile::DownloadFile(openstudio::path t_path)
    : m_fileName(std::move(t_path))
  {
  }

  void RemoteBCL::DownloadFile::flush()
  {
    if (m_ofs.good()) { m_ofs.flush(); }
  }

  const openstudio::path& RemoteBCL::DownloadFile::fileName() const noexcept
  {
    return m_fileName;
  }

  void RemoteBCL::DownloadFile::close()
  {
    m_ofs.close();
  }

  void RemoteBCL::DownloadFile::write(const std::string &data)
  {
    m_ofs.write(data.c_str(), data.size());
  }


  RemoteBCL::RemoteBCL():
    m_networkManager(new QNetworkAccessManager()),
    m_numResultsPerQuery(10),
    m_apiVersion("2.0")
  {
    // TODO: QT-Separation-Move
    // make sure application is initialized
    //openstudio::Application::instance().application(false);

    m_prodAuthKey = LocalBCL::instance().prodAuthKey();
    m_devAuthKey = LocalBCL::instance().devAuthKey();
    validProdAuthKey = false;
    validDevAuthKey = false;

    useRemoteProductionUrl();

//#ifndef QT_NO_OPENSSL
//    connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif
  }

  RemoteBCL::~RemoteBCL()
  {
    delete m_networkManager;
  }

  bool RemoteBCL::initializeSSL(const openstudio::path &t_pathToSSLLibraries)
  {
    const auto oldpath = System::getenv("PATH");
    if (!t_pathToSSLLibraries.empty())
    {
      System::setenv("PATH", t_pathToSSLLibraries.string());
    }

#ifdef QT_NO_OPENSSL
    bool opensslloaded = false;
#else
    bool opensslloaded = QSslSocket::supportsSsl();
#endif

    if (!t_pathToSSLLibraries.empty() && oldpath)
    {
      System::setenv("PATH", *oldpath);
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
      if (!m_mutex.try_lock()){
        return 0;
      }

      // disconnect all signals from m_networkManager to this
      //disconnect(m_networkManager, nullptr, this, nullptr); // returns a bool, but was not checking, so removed

      const_cast<RemoteBCL*>(this)->m_lastSearch.clear();

      QString url = toQString(remoteUrl() + "/api/search/?fq[]=ss_uuid:%1&api_version=%2").arg(
        toQString(component.uid()),
        toQString(m_apiVersion)
      );
      //LOG(Warn, toString(url));

      // when the reply is finished call onSearchResponseComplete
      //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//      connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

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
      if (!m_mutex.try_lock()){
        return 0;
      }

      // disconnect all signals from m_networkManager to this
      //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but no check, so removed

      const_cast<RemoteBCL*>(this)->m_lastSearch.clear();

      QString url = toQString(remoteUrl() + "/api/search/?fq[]=ss_uuid:%1&api_version=%2").arg(
        toQString(measure.uid()),
        toQString(m_apiVersion)
      );
      //LOG(Warn, toString(url));

      // when the reply is finished call onSearchResponseComplete
      //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//      connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

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


  bool RemoteBCL::isOnline() const
  {
    if (m_networkManager->networkAccessible() == QNetworkAccessManager::NotAccessible){
      return false;
    }
    return true;
  }

  boost::optional<BCLComponent> RemoteBCL::lastComponentDownload() const
  {
    return m_lastComponentDownload;
  }

  boost::optional<BCLMeasure> RemoteBCL::lastMeasureDownload() const
  {
    return m_lastMeasureDownload;
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
      if (!m_mutex.try_lock()){
        return false;
      }

      const_cast<RemoteBCL*>(this)->m_lastSearch.clear();

      QString url = toQString(remoteUrl + "/api/search/?api_version=%1&show_rows=0").arg(
        toQString(m_apiVersion)
      );
      //LOG(Warn, toString(url));

      // disconnect all signals from m_networkManager to this
      //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed

      // when the reply is finished call onSearchResponseComplete
      //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//      connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

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

  //QString RemoteBCL::checkForRedirect(const QNetworkReply* reply) const
  //{
  //  // In Qt > 5.3, use QNetworkRequest::FollowRedirectsAttribute instead
  //  // TODO: JM 2018-12-14 QNetworkRequest will have to be removed anyway. Now just converting to int to remove last Q Variant
  //  int replyStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  //  if (replyStatus == 301 || replyStatus == 302) {
  //    return reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
  //  }
  //  return QString();
  //}

  boost::optional<BCLComponent> RemoteBCL::waitForComponentDownload(int msec) const
  {
    if (waitForLock(msec)){
      return m_lastComponentDownload;
    }
    return boost::none;
  }

  boost::optional<BCLMeasure> RemoteBCL::waitForMeasureDownload(int msec) const
  {
    if (waitForLock(msec)){
      return m_lastMeasureDownload;
    }
    return boost::none;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::waitForMetaSearch(int msec) const
  {
    if (waitForLock(msec)){
      return m_lastMetaSearch;
    }
    return boost::none;
  }

  std::vector<BCLSearchResult> RemoteBCL::waitForSearch(int msec) const
  {
    if (waitForLock(msec)){
      return m_lastSearch;
    }
    return std::vector<BCLSearchResult>();
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
    if (!m_mutex.try_lock()){
      LOG(Debug, "Cannot get mutex lock");
      return false;
    }

    m_downloadFile = std::make_unique<DownloadFile>(openstudio::filesystem::temp_directory_path() / toPath(uid + ".bcl"));
    if (!m_downloadFile->open()){
      m_mutex.unlock();
      return false;
    }

    m_downloadUid = uid;

    QString url = toQString(remoteUrl() + "/api/component/download?uids=" + uid);
    //LOG(Debug, "Download URL '" << toString(url) << "'");

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");

    // disconnect all signals from m_networkManager to this
    //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onDownloadComplete
    //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onDownloadComplete);

//#ifndef QT_NO_OPENSSL
//    connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

    m_downloadReply = m_networkManager->get(request);
    if (m_downloadReply->isRunning()) {
      LOG(Debug, "Download running");
    }else{
      m_mutex.unlock();
      m_downloadReply->deleteLater();
      m_downloadReply = nullptr;
      LOG(Debug, "Download failed to start");
      return false;
    }

    //connect(m_downloadReply, &QNetworkReply::readyRead, this, &RemoteBCL::downloadData);

    return true;
  }

  bool RemoteBCL::downloadMeasure(const std::string& uid)
  {
    return downloadComponent(uid);
  }

  bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType)
  {
    // can't start another download until last one is done
    if (!m_mutex.try_lock()){
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
    //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onMetaSearchResponseComplete
    //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onMetaSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//    connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType)
  {
    // can't start another download until last one is done
    if (!m_mutex.try_lock()){
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
        toQString(openstudio::string_conversions::number(componentTypeTID)),
        toQString(m_apiVersion)
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onMetaSearchResponseComplete
    //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onMetaSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//    connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType, const unsigned page)
  {
    // can't start another download until last one is done
    if (!m_mutex.try_lock()){
      return false;
    }

    m_lastSearch.clear();

    QString url;
    if (componentType.empty() || componentType == "*"){
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&api_version=%3&show_rows=%4&page=%5").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(m_apiVersion),
        toQString(openstudio::string_conversions::number(m_numResultsPerQuery)),
        toQString(openstudio::string_conversions::number(page))
      );
    }else{
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&fq[]=%3:\"%4\""
        "&api_version=%5&show_rows=%6&page=%7").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        (filterType == "nrel_component" ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags"),
        toQString(componentType),
        toQString(m_apiVersion),
        toQString(openstudio::string_conversions::number(m_numResultsPerQuery)),
        toQString(openstudio::string_conversions::number(page))
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onSearchResponseComplete
    //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//    connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
    m_networkManager->get(request);

    return true;
  }

  bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType, const unsigned page)
  {
    // can't start another download until last one is done
    if (!m_mutex.try_lock()){
      return false;
    }

    m_lastSearch.clear();

    QString url;
    if (componentTypeTID == 0){
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&api_version=%3&show_rows=%4&page=%5").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(m_apiVersion),
        toQString(openstudio::string_conversions::number(m_numResultsPerQuery)),
        toQString(openstudio::string_conversions::number(page))
      );
    }else{
      url = toQString(remoteUrl() + "/api/search/%1?fq[]=bundle:%2&fq[]=tid:%3&api_version=%4&show_rows=%5&page=%6").arg(
        toQString(searchTerm == "*" ? "" : searchTerm != "" ? searchTerm + ".xml" : "").replace("+", "%2B"),
        toQString(filterType),
        toQString(openstudio::string_conversions::number(componentTypeTID)),
        toQString(m_apiVersion),
        toQString(openstudio::string_conversions::number(m_numResultsPerQuery)),
        toQString(openstudio::string_conversions::number(page))
      );
    }
    //LOG(Warn, toString(url));

    // disconnect all signals from m_networkManager to this
    //disconnect(m_networkManager, nullptr, this, nullptr); // returns bool, but not checked, so removed
    //OS_ASSERT(test);

    // when the reply is finished call onSearchResponseComplete
    //connect(m_networkManager, &QNetworkAccessManager::finished, this, &RemoteBCL::onSearchResponseComplete);

//#ifndef QT_NO_OPENSSL
//    connect(m_networkManager, &QNetworkAccessManager::sslErrors, this, &RemoteBCL::catchSslErrors);
//#endif

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
      if (m_mutex.try_lock()){
        m_mutex.unlock();
        return true;
      }

      // DLM: no longer calls Application::instance().processEvents(msecPerLoop);
      // this calls process events
      System::msleep(msecPerLoop);

      if (current > numTries){
        LOG(Error, "waitForLock timeout");
        break;
      }

      ++current;
    }

    m_mutex.unlock();
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
        std::shared_ptr<pugi::xml_document> document = std::make_shared<pugi::xml_document>();
        auto result = document->load_string(reply->readAll().data());
        if (!result) {
          LOG(Error, "Bad XML Response: " << result.description());
        } else {
          if (!m_useRemoteDevelopmentUrl){
            validProdAuthKey = true;
          } else {
            validDevAuthKey = true;
          }
          return RemoteQueryResponse(document);
        }
      } else if (reply->error() == QNetworkReply::AuthenticationRequiredError){
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
      }else{
        LOG(Error, "Network Error: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << " - " << reply->errorString().toStdString());
      }
    }
    return boost::none;
  }

  boost::optional<BCLMetaSearchResult> RemoteBCL::processMetaSearchResponse(const RemoteQueryResponse& remoteQueryResponse) const
  {
    auto root = remoteQueryResponse.root();

    if (root) {
      if (std::string(root.name()).compare(0,6,"result") == 0) { // C++20: std::string(root.name()).starts_with("result")
        auto numResultsElement = root.child("result_count");
        if (numResultsElement) {
          return BCLMetaSearchResult(root);
        }
      }
    }

    return boost::none;
  }

  std::vector<BCLSearchResult> RemoteBCL::processSearchResponse(const RemoteQueryResponse& remoteQueryResponse) const
  {
    std::vector<BCLSearchResult> searchResults;

    auto root = remoteQueryResponse.root();
    auto result = root.child("result");
    if (result){
      auto componentElement = result.first_child();

      //Basic check to see if it's non-empty
      while (componentElement.child("name")) {
        //Skip components without a uid or version_id
        if (componentElement.child("uuid") && componentElement.child("vuuid")) {
          BCLSearchResult searchResult(componentElement);
          searchResults.push_back(searchResult);
        }
        result = result.next_sibling("result");
        componentElement = result.first_child();
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

  //void RemoteBCL::downloadData()
  //{
  //  OS_ASSERT(m_downloadReply);
  //  const auto result = m_downloadReply->readAll();
  //  const auto string = std::string{result.begin(), result.end()};
  //  m_downloadFile->write(string);
  //}

  ///void RemoteBCL::onDownloadComplete(QNetworkReply* reply)
  //{
  //  QString redirect = this->checkForRedirect(reply);
  //  if (!redirect.isEmpty()) {
  //    QNetworkRequest request = QNetworkRequest(QUrl(redirect));
  //    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
  //    m_downloadReply = m_networkManager->get(request);
  //    connect(m_downloadReply, &QNetworkReply::readyRead, this, &RemoteBCL::downloadData);
  //  } else {
  //    const auto src = m_downloadFile->fileName();
  //    m_downloadFile->flush();
  //    m_downloadFile->close();
  //    std::string componentType;

  //   if (!reply) {
  //      LOG(Error, "Empty reply");
  //    } else {
  //      if (reply->error() == QNetworkReply::NoError) {
  //        //Print response headers
  //     /*for(int i=0; i<reply->rawHeaderList().size(); ++i){
  //          QString str(reply->rawHeaderList()[i].constData());
  //          std::cout << toString(str) << ": " << toString(reply->rawHeader(reply->rawHeaderList()[i] ).constData()) << std::endl;
  //      }*/
  //
  //        // Extract the files to a temp location
  //        openstudio::path tempDest = openstudio::filesystem::temp_directory_path() / toPath(m_downloadUid  + '/');
  //
  //        if (openstudio::filesystem::is_directory(tempDest)) {
  //          removeDirectory(tempDest);
  //        }
  //        openstudio::filesystem::create_directories(tempDest);
  //
  //        std::vector<openstudio::path> createdFiles;
  //        try {
  //          openstudio::UnzipFile uf(src);
  //          createdFiles = uf.extractAllFiles(tempDest);
  //        } catch (const std::exception &e) {
  //          LOG(Error, "Cannot unzip file: " << e.what());
  //        }
  //        openstudio::filesystem::remove(src);
  //
  //        // search for component.xml or measure.xml file
  //        boost::optional<openstudio::path> xmlPath;
  //
  //        for (const openstudio::path& path : createdFiles) {
  //          if (path.filename() == toPath("component.xml")) {
  //            componentType = "component";
  //            m_lastComponentDownload.reset();
  //            xmlPath = path;
  //            break;
  //          } else if (path.filename() == toPath("measure.xml")) {
  //            componentType = "measure";
  //            m_lastMeasureDownload.reset();
  //            xmlPath = path;
  //            break;
  //          }
  //        }
  //
  //        if (xmlPath) {
  //          path src = xmlPath->parent_path();
  //          path dest = src.parent_path();
  //          openstudio::filesystem::remove(dest / toPath("DISCLAIMER.txt"));
  //          openstudio::filesystem::remove(dest / toPath("README.txt"));
  //          openstudio::filesystem::remove(dest / toPath("output.xml"));
  //          copyDirectory(src, dest);
  //          removeDirectory(src);
  // 
  //          if (componentType == "component")
  //          {
  //            path componentXmlPath = dest / toPath("component.xml");
  //            // open the component to figure out uid and vid
  //            BCLComponent component(toString(componentXmlPath.parent_path()));
  //            std::string uid = component.uid();
  //            std::string versionId = component.versionId();
  //
  //            // check if component has proper uid and vid
  //            if (!uid.empty() && !versionId.empty()) {
  //             dest = LocalBCL::instance().libraryPath() / uid / versionId;
  //
  //              removeDirectory(dest);
  //              if (copyDirectory(componentXmlPath.parent_path(), dest))
  //              {
  //                // Add to LocalBCL
  //                m_lastComponentDownload = BCLComponent(toString(dest));
  //                LocalBCL::instance().addComponent(*m_lastComponentDownload);
  //              }
  //            }
  //          } else if (componentType == "measure") {
  //            path measureXmlPath = dest / toPath("measure.xml");
  //            // open the measure to figure out uid and vid
  //            boost::optional<BCLMeasure> measure;
  //            try {
  //              measure = BCLMeasure(measureXmlPath.parent_path());
  //
  //              std::string uid = measure->uid();
  //              std::string versionId = measure->versionId();
  //
  //              // check if component has proper uid and vid
  //              if (!uid.empty() && !versionId.empty()) {
  //                dest = LocalBCL::instance().libraryPath() / uid / versionId;
  //                removeDirectory(dest);
  //                if (copyDirectory(measureXmlPath.parent_path(), dest))
  //                {
  //                  // Add to LocalBCL
  //                  m_lastMeasureDownload = BCLMeasure(dest);
  //                  LocalBCL::instance().addMeasure(*m_lastMeasureDownload);
  //                }
  //              }
  //            } catch (const std::exception&) {
  //              LOG(Error, "Unable to create measure from download: " + toString(measureXmlPath.parent_path()));
  //            }
  //          }
  //        } else {
  //          LOG(Error, "No component.xml or measure.xml file found in downloaded contents");
  //        }
  //
  //        // delete the temp unzip directory
  //        removeDirectory(tempDest);
  //
  //      } else {
  //        LOG(Error, "Network Error: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
  //      }
  //    }
  //
  //    if (componentType == "measure") {
  //      //emit measureDownloaded(m_downloadUid, m_lastMeasureDownload);
  //    } else {
  //      //emit componentDownloaded(m_downloadUid, m_lastComponentDownload);
  //    }
  //
  //    m_mutex.unlock();
  //  }
  //  reply->deleteLater();
  //}

  //void RemoteBCL::onMetaSearchResponseComplete(QNetworkReply* reply)
  //{
  //  QString redirect = this->checkForRedirect(reply);
  //  if (!redirect.isEmpty()) {
  //    QNetworkRequest request = QNetworkRequest(QUrl(redirect));
  //    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  //    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
  //    m_networkManager->get(request);
  //  } else {
  //    boost::optional<RemoteQueryResponse> remoteQueryResponse = this->processReply(reply);
  //    if (remoteQueryResponse) {
  //      m_lastMetaSearch = processMetaSearchResponse(*remoteQueryResponse);
  //    }
  //
  //    if (m_lastMetaSearch) {
  //      setLastTotalResults(m_lastMetaSearch->numResults());
  //    } else {
  //      setLastTotalResults(0);
  //    }
  //
  //    //emit metaSearchCompleted(m_lastMetaSearch);
  //
  //    m_mutex.unlock();
  //  }
  //  reply->deleteLater();
  //}

  //void RemoteBCL::onSearchResponseComplete(QNetworkReply* reply)
  //{
  //  QString redirect = this->checkForRedirect(reply);
  //  if (!redirect.isEmpty()) {
  //    QNetworkRequest request = QNetworkRequest(QUrl(redirect));
  //    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
  //    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
  //    m_networkManager->get(request);
  //  } else {
  //    boost::optional<RemoteQueryResponse> remoteQueryResponse = this->processReply(reply);
  //    if (remoteQueryResponse) {
  //      m_lastSearch = processSearchResponse(*remoteQueryResponse);
  //    }
  //
  //    //emit searchCompleted(m_lastSearch);
  //
  //    m_mutex.unlock();
  //  }
  //  reply->deleteLater();
  //}

  //void RemoteBCL::catchSslErrors(QNetworkReply* reply, const QList<QSslError>& errorList)
  //{
  //  for (const QSslError& error : errorList) {
  //    LOG(Error, error.errorString().toStdString());
  //  }
  //}

} // openstudio
