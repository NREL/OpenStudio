/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <regex>

#define REMOTE_PRODUCTION_SERVER "https://bcl.nrel.gov"
#define REMOTE_DEVELOPMENT_SERVER "http://bcl7.development.nrel.gov"

using namespace utility::conversions;

namespace openstudio {

std::ostream& operator<<(std::ostream& os, const pugi::xml_document& element) {
  element.save(os, "  ");
  return os;
}

RemoteQueryResponse::RemoteQueryResponse(std::shared_ptr<pugi::xml_document>& domDocument) : m_domDocument(domDocument) {}

pugi::xml_node RemoteQueryResponse::root() const {
  return m_domDocument->document_element();
}

// TODO: please note that you should use getClient everywhere after instead of instantiating your own http_client_config
// as it will allow us to change http_client_config (SSL settings etc) in  only one place
web::http::client::http_client RemoteBCL::getClient(const std::string& url) {
  web::http::client::http_client_config config;
  // bcl.nrel.gov can be slow to respond to client requests so bump the default of 30 seconds to 60 to account for lengthy response time.
  // this is timeout is for each send and receive operation on the client and not the entire client session.
  config.set_timeout(std::chrono::seconds(60));
  config.set_validate_certificates(false);

  return web::http::client::http_client(utility::conversions::to_string_t(url), config);
}

bool RemoteBCL::DownloadFile::open() {
  OS_ASSERT(!m_fileName.empty());
  m_ofs.open(toString(m_fileName).c_str(), std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
  return m_ofs.good();
}

RemoteBCL::DownloadFile::DownloadFile(openstudio::path t_path) : m_fileName(std::move(t_path)) {}

void RemoteBCL::DownloadFile::flush() {
  if (m_ofs.good()) m_ofs.flush();
}

const openstudio::path& RemoteBCL::DownloadFile::fileName() const noexcept {
  return m_fileName;
}

void RemoteBCL::DownloadFile::close() {
  m_ofs.close();
}

void RemoteBCL::DownloadFile::write(const std::vector<unsigned char>& data) {
  m_ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
}

RemoteBCL::RemoteBCL()
  : m_prodAuthKey(LocalBCL::instance().prodAuthKey()),
    m_devAuthKey(LocalBCL::instance().devAuthKey()),
    m_numResultsPerQuery(10),
    m_lastTotalResults(0),
    m_apiVersion("2.0"),
    validProdAuthKey(false),
    validDevAuthKey(false) {
  useRemoteProductionUrl();
}

RemoteBCL::~RemoteBCL() {}

///////////////////////////////////////////////////////////////////////////
/// Inherited members
///////////////////////////////////////////////////////////////////////////

boost::optional<BCLComponent> RemoteBCL::getComponent(const std::string& uid, const std::string& versionId) const {
  bool downloadStarted = const_cast<RemoteBCL*>(this)->downloadComponent(uid);
  if (downloadStarted) {
    return waitForComponentDownload();
  }
  return boost::none;
}

boost::optional<BCLMeasure> RemoteBCL::getMeasure(const std::string& uid, const std::string& versionId) const {
  bool downloadStarted = const_cast<RemoteBCL*>(this)->downloadMeasure(uid);
  if (downloadStarted) {
    return waitForMeasureDownload();
  }
  return boost::none;
}

boost::optional<BCLMetaSearchResult> RemoteBCL::metaSearchComponentLibrary(const std::string& searchTerm, const std::string& componentType,
                                                                           const std::string& filterType) const {
  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibraryMetaSearch(searchTerm, componentType, filterType);
  if (searchStarted) {
    return waitForMetaSearch();
  }
  return boost::none;
}

boost::optional<BCLMetaSearchResult> RemoteBCL::metaSearchComponentLibrary(const std::string& searchTerm, const unsigned componentTypeTID,
                                                                           const std::string& filterType) const {
  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibraryMetaSearch(searchTerm, componentTypeTID, filterType);
  if (searchStarted) {
    return waitForMetaSearch();
  }
  return boost::none;
}

std::vector<BCLSearchResult> RemoteBCL::searchComponentLibrary(const std::string& searchTerm, const std::string& componentType,
                                                               const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentType, "nrel_component");
  if (lastTotalResults() == 0) {
    return std::vector<BCLSearchResult>();
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentType, "nrel_component", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return std::vector<BCLSearchResult>();
}

std::vector<BCLSearchResult> RemoteBCL::searchComponentLibrary(const std::string& searchTerm, const unsigned componentTypeTID,
                                                               const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentTypeTID, "nrel_component");
  if (lastTotalResults() == 0) {
    return std::vector<BCLSearchResult>();
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentTypeTID, "nrel_component", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return std::vector<BCLSearchResult>();
}

std::vector<BCLSearchResult> RemoteBCL::searchMeasureLibrary(const std::string& searchTerm, const std::string& componentType,
                                                             const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentType, "nrel_measure");
  if (lastTotalResults() == 0) {
    return std::vector<BCLSearchResult>();
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentType, "nrel_measure", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return std::vector<BCLSearchResult>();
}

std::vector<BCLSearchResult> RemoteBCL::searchMeasureLibrary(const std::string& searchTerm, const unsigned componentTypeTID,
                                                             const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentTypeTID, "nrel_measure");
  if (lastTotalResults() == 0) {
    return std::vector<BCLSearchResult>();
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentTypeTID, "nrel_measure", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return std::vector<BCLSearchResult>();
}

int RemoteBCL::checkForComponentUpdates() {
  m_componentsWithUpdates.clear();

  for (const BCLComponent& component : LocalBCL::instance().components()) {
    // can't start another request until last one is done
    if (m_httpResponse && !m_httpResponse->is_done()) {
      return false;
    }

    m_lastSearch.clear();

    auto client = getClient(remoteUrl());
    web::uri_builder builder(U("/api/search/"));

    builder.append_path(U("*.xml"));

    builder.append_query(U("fq[]"), to_string_t("ss_uuid:" + component.uid()));

    builder.append_query(U("api_version"), to_string_t(m_apiVersion));

    //LOG(Warn, remoteUrl() + toString(builder.to_string()));

    m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                       .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                       .then([this](const std::string& xml) {
                         auto remoteQueryResponse = processReply(xml);

                         if (remoteQueryResponse) {
                           m_lastSearch = processSearchResponse(*remoteQueryResponse);
                         }
                       });

    std::vector<BCLSearchResult> result = waitForSearch();
    if (result.size() > 0 && result[0].versionId() != component.versionId()) {
      m_componentsWithUpdates.push_back(result[0]);
    }
  }

  return m_componentsWithUpdates.size();
}

int RemoteBCL::checkForMeasureUpdates() {
  m_measuresWithUpdates.clear();

  for (const BCLMeasure& measure : LocalBCL::instance().measures()) {
    // can't start another request until last one is done
    if (m_httpResponse && !m_httpResponse->is_done()) {
      return false;
    }

    m_lastSearch.clear();

    auto client = getClient(remoteUrl());
    web::uri_builder builder(U("/api/search/"));

    builder.append_path(U("*.xml"));

    builder.append_query(U("fq[]"), to_string_t("ss_uuid:" + measure.uid()));

    builder.append_query(U("api_version"), to_string_t(m_apiVersion));

    //LOG(Warn, remoteUrl() + toString(builder.to_string()));

    m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                       .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                       .then([this](const std::string& xml) {
                         auto remoteQueryResponse = processReply(xml);

                         if (remoteQueryResponse) {
                           m_lastSearch = processSearchResponse(*remoteQueryResponse);
                         }
                       });

    std::vector<BCLSearchResult> result = waitForSearch();
    if (result.size() > 0 && result[0].versionId() != measure.versionId()) {
      m_measuresWithUpdates.push_back(result[0]);
    }
  }

  return m_measuresWithUpdates.size();
}

std::vector<BCLSearchResult> RemoteBCL::componentsWithUpdates() const {
  return m_componentsWithUpdates;
}

std::vector<BCLSearchResult> RemoteBCL::measuresWithUpdates() const {
  return m_measuresWithUpdates;
}

void RemoteBCL::updateComponents() {
  if (m_componentsWithUpdates.size() == 0) {
    checkForComponentUpdates();
  }

  for (const BCLSearchResult& component : m_componentsWithUpdates) {
    downloadMeasure(component.uid());
    boost::optional<BCLComponent> newComponent = waitForComponentDownload();

    if (newComponent) {
      boost::optional<BCLComponent> oldComponent = LocalBCL::instance().getComponent(newComponent->uid());
      if (oldComponent && oldComponent->versionId() != newComponent->versionId()) {
        LocalBCL::instance().removeComponent(*oldComponent);
      }
    }
  }
}

void RemoteBCL::updateMeasures() {
  if (m_measuresWithUpdates.size() == 0) {
    checkForMeasureUpdates();
  }

  for (const BCLSearchResult& measure : m_measuresWithUpdates) {
    downloadMeasure(measure.uid());
    boost::optional<BCLMeasure> newMeasure = waitForMeasureDownload();

    if (newMeasure) {
      boost::optional<BCLMeasure> oldMeasure = LocalBCL::instance().getMeasure(newMeasure->uid());
      if (oldMeasure && oldMeasure->versionId() != newMeasure->versionId()) {
        LocalBCL::instance().removeMeasure(*oldMeasure);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
/// Blocking class members
///////////////////////////////////////////////////////////////////////////

bool RemoteBCL::isOnline() {
  try {
    auto ip = getClient("https://checkip.amazonaws.com/")
                .request(web::http::methods::GET)
                .then([](web::http::http_response response) {
                  auto statusCode = response.status_code();
                  if (statusCode != 200) {
                    std::stringstream ss;
                    ss << "Error: response code was " << statusCode;
                    return ss.str();
                  }

                  auto body = response.extract_utf8string(true).get();
                  // Remove trailing line ending
                  return body.erase(body.find_last_not_of("\n") + 1);
                })
                .get();

    std::regex ipRegex("^\\d{1,3}(?:\\.\\d{1,3}){3}$");
    return std::regex_search(ip, ipRegex);
  } catch (const std::exception&) {
    // not online
  }
  return false;
}

boost::optional<BCLComponent> RemoteBCL::lastComponentDownload() const {
  return m_lastComponentDownload;
}

boost::optional<BCLMeasure> RemoteBCL::lastMeasureDownload() const {
  return m_lastMeasureDownload;
}

boost::optional<BCLMetaSearchResult> RemoteBCL::lastMetaSearch() const {
  return m_lastMetaSearch;
}

std::vector<BCLSearchResult> RemoteBCL::lastSearch() const {
  return m_lastSearch;
}

std::string RemoteBCL::remoteUrl() const {
  return m_remoteUrl;
}

std::string RemoteBCL::remoteProductionUrl() {
  return std::string(REMOTE_PRODUCTION_SERVER);
}

std::string RemoteBCL::remoteDevelopmentUrl() {
  return std::string(REMOTE_DEVELOPMENT_SERVER);
}

void RemoteBCL::useRemoteDevelopmentUrl() {
  m_useRemoteDevelopmentUrl = true;
  m_remoteUrl = remoteDevelopmentUrl();
  m_authKey = m_devAuthKey;
}

void RemoteBCL::useRemoteProductionUrl() {
  m_useRemoteDevelopmentUrl = false;
  m_remoteUrl = remoteProductionUrl();
  m_authKey = m_prodAuthKey;
}

std::string RemoteBCL::authKey() const {
  return m_authKey;
}

std::string RemoteBCL::prodAuthKey() const {
  return m_prodAuthKey;
}

bool RemoteBCL::setProdAuthKey(const std::string& prodAuthKey) {
  bool previousValidity = validProdAuthKey;
  if (validateAuthKey(prodAuthKey, remoteProductionUrl())) {
    m_prodAuthKey = prodAuthKey;
    if (!m_useRemoteDevelopmentUrl) {
      m_authKey = prodAuthKey;
    }
    return true;
  }
  validProdAuthKey = previousValidity;
  return false;
}

std::string RemoteBCL::devAuthKey() const {
  return m_devAuthKey;
}

bool RemoteBCL::setDevAuthKey(const std::string& devAuthKey) {
  bool previousValidity = validDevAuthKey;
  if (validateAuthKey(devAuthKey, remoteDevelopmentUrl())) {
    m_devAuthKey = devAuthKey;
    if (m_useRemoteDevelopmentUrl) {
      m_authKey = devAuthKey;
    }
    return true;
  }
  validDevAuthKey = previousValidity;
  return false;
}

int RemoteBCL::resultsPerQuery() const {
  return m_numResultsPerQuery;
}

int RemoteBCL::lastTotalResults() const {
  return m_lastTotalResults;
}

int RemoteBCL::numResultPages() const {
  double numerator(lastTotalResults());
  double denominator(resultsPerQuery());
  return int(std::ceil(numerator / denominator));
}

bool RemoteBCL::validateAuthKey(const std::string& authKey, const std::string& remoteUrl) {
  if (authKey.length() == 32) {
    std::string previousUrl = this->remoteUrl();

    // Check if validation has already run for the given key
    if (remoteUrl == remoteProductionUrl() && authKey == prodAuthKey() && validProdAuthKey) {
      return true;
    } else if (remoteUrl == remoteDevelopmentUrl() && authKey == devAuthKey() && validDevAuthKey) {
      return true;
    }

    // Temporarily set url
    if (remoteUrl == remoteDevelopmentUrl()) {
      useRemoteDevelopmentUrl();
    } else {
      useRemoteProductionUrl();
    }

    // can't start another request until last one is done
    if (m_httpResponse && !m_httpResponse->is_done()) {
      return false;
    }

    m_lastSearch.clear();

    auto client = getClient(remoteUrl);
    web::uri_builder builder(U("/api/search/"));

    builder.append_path(U("*.xml"));

    builder.append_query(U("api_version"), to_string_t(m_apiVersion));
    builder.append_query(U("show_rows"), U("0"));

    //LOG(Warn, remoteUrl() + toString(builder.to_string()));

    m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                       .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                       .then([this](const std::string& xml) {
                         auto remoteQueryResponse = processReply(xml);

                         if (remoteQueryResponse) {
                           m_lastSearch = processSearchResponse(*remoteQueryResponse);
                         }
                       });

    waitForSearch();

    // Restore url
    if (previousUrl == remoteDevelopmentUrl()) {
      useRemoteDevelopmentUrl();
    } else {
      useRemoteProductionUrl();
    }

    if (remoteUrl == remoteDevelopmentUrl()) {
      return validDevAuthKey;
    } else {
      return validProdAuthKey;
    }
  }
  return false;
}

boost::optional<BCLComponent> RemoteBCL::waitForComponentDownload(int msec) const {
  if (waitForLock(msec)) {
    return m_lastComponentDownload;
  }
  return boost::none;
}

boost::optional<BCLMeasure> RemoteBCL::waitForMeasureDownload(int msec) const {
  if (waitForLock(msec)) {
    return m_lastMeasureDownload;
  }
  return boost::none;
}

boost::optional<BCLMetaSearchResult> RemoteBCL::waitForMetaSearch(int msec) const {
  if (waitForLock(msec)) {
    return m_lastMetaSearch;
  }
  return boost::none;
}

std::vector<BCLSearchResult> RemoteBCL::waitForSearch(int msec) const {
  if (waitForLock(msec)) {
    return m_lastSearch;
  }
  return std::vector<BCLSearchResult>();
}

///////////////////////////////////////////////////////////////////////////
/// Non-blocking class members
///////////////////////////////////////////////////////////////////////////

bool RemoteBCL::downloadComponent(const std::string& uid) {
  // Check for empty uid
  if (uid.empty()) {
    LOG(Error, "Error: No uid provided");
    return false;
  }

  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    LOG(Debug, "Cannot get mutex lock");
    return false;
  }

  m_downloadFile = std::make_unique<DownloadFile>(openstudio::filesystem::temp_directory_path() / toPath(uid + ".bcl"));
  if (!m_downloadFile->open()) {
    return false;
  }

  m_downloadUid = uid;
  // request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17");
  auto client = getClient(remoteUrl());
  web::uri_builder builder(U("/api/component/download"));

  builder.append_query(U("uids"), to_string_t(uid));

  web::http::http_request msg(web::http::methods::GET);
  msg.headers().add(U("User-Agent"),
                    U("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17"));
  msg.set_request_uri(builder.to_string());

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_vector(); })
                     .then([this](const std::vector<unsigned char>& zip) {
                       m_downloadFile->write(zip);
                       m_downloadFile->flush();
                       m_downloadFile->close();
                     })
                     .then([this]() { onDownloadComplete(); });

  //LOG(Warn, remoteUrl() + toString(builder.to_string()));

  return true;
}

bool RemoteBCL::downloadMeasure(const std::string& uid) {
  return downloadComponent(uid);
}

bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastMetaSearch.reset();

  auto client = getClient(remoteUrl());
  web::uri_builder builder(U("/api/metasearch/"));

  auto query = searchTerm.empty() ? "*" : searchTerm;
  query = std::regex_replace(query, std::regex("\\+"), "%2B");
  builder.append_path(to_string_t(query + ".xml"));

  builder.append_query(U("fq[]"), to_string_t("bundle:" + filterType));

  if (!componentType.empty() && componentType != "*") {
    std::string filter = (filterType == "nrel_component") ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags";
    filter += ":\"" + componentType + "\"";
    builder.append_query(U("fq[]"), to_string_t(filter));
  }

  builder.append_query(U("api_version"), to_string_t(m_apiVersion));

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) {
                       auto remoteQueryResponse = processReply(xml);

                       if (remoteQueryResponse) {
                         m_lastMetaSearch = processMetaSearchResponse(*remoteQueryResponse);
                       }

                       if (m_lastMetaSearch) {
                         setLastTotalResults(m_lastMetaSearch->numResults());
                       } else {
                         setLastTotalResults(0);
                       }
                     });

  //LOG(Warn, remoteUrl() + toString(builder.to_string()));

  return true;
}

bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastMetaSearch.reset();

  auto client = getClient(remoteUrl());
  web::uri_builder builder(U("/api/metasearch/"));

  auto query = searchTerm.empty() ? "*" : searchTerm;
  query = std::regex_replace(query, std::regex("\\+"), "%2B");
  builder.append_path(to_string_t(query + ".xml"));

  builder.append_query(U("fq[]"), to_string_t("bundle:" + filterType));

  if (componentTypeTID != 0) {
    std::string filter = "tid:" + openstudio::string_conversions::number(componentTypeTID);
    builder.append_query(U("fq[]"), to_string_t(filter));
  }

  builder.append_query(U("api_version"), to_string_t(m_apiVersion));

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) {
                       auto remoteQueryResponse = processReply(xml);

                       if (remoteQueryResponse) {
                         m_lastMetaSearch = processMetaSearchResponse(*remoteQueryResponse);
                       }

                       if (m_lastMetaSearch) {
                         setLastTotalResults(m_lastMetaSearch->numResults());
                       } else {
                         setLastTotalResults(0);
                       }
                     });

  //LOG(Warn, remoteUrl() + toString(builder.to_string()));

  return true;
}

bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType,
                                            const unsigned page) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastSearch.clear();

  auto client = getClient(remoteUrl());
  web::uri_builder builder(U("/api/search/"));

  auto query = searchTerm.empty() ? "*" : searchTerm;
  query = std::regex_replace(query, std::regex("\\+"), "%2B");
  builder.append_path(to_string_t(query + ".xml"));

  builder.append_query(U("fq[]"), to_string_t("bundle:" + filterType));

  if (!componentType.empty() && componentType != "*") {
    std::string filter = (filterType == "nrel_component") ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags";
    filter += ":\"" + componentType + "\"";
    builder.append_query(U("fq[]"), to_string_t(filter));
  }

  builder.append_query(U("api_version"), to_string_t(m_apiVersion));
  builder.append_query(U("show_rows"), to_string_t(openstudio::string_conversions::number(m_numResultsPerQuery)));
  builder.append_query(U("page"), to_string_t(openstudio::string_conversions::number(page)));

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) {
                       auto remoteQueryResponse = processReply(xml);

                       if (remoteQueryResponse) {
                         m_lastSearch = processSearchResponse(*remoteQueryResponse);
                       }
                     });

  //LOG(Warn, remoteUrl() + toString(builder.to_string()));

  return true;
}

bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType,
                                            const unsigned page) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastSearch.clear();

  auto client = getClient(remoteUrl());
  web::uri_builder builder(U("/api/search/"));

  auto query = searchTerm.empty() ? "*" : searchTerm;
  query = std::regex_replace(query, std::regex("\\+"), "%2B");
  builder.append_path(to_string_t(query + ".xml"));

  builder.append_query(U("fq[]"), to_string_t("bundle:" + filterType));

  if (componentTypeTID != 0) {
    std::string filter = "tid:" + openstudio::string_conversions::number(componentTypeTID);
    builder.append_query(U("fq[]"), to_string_t(filter));
  }
  builder.append_query(U("api_version"), to_string_t(m_apiVersion));
  builder.append_query(U("show_rows"), to_string_t(openstudio::string_conversions::number(m_numResultsPerQuery)));
  builder.append_query(U("page"), to_string_t(openstudio::string_conversions::number(page)));

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) {
                       auto remoteQueryResponse = processReply(xml);

                       if (remoteQueryResponse) {
                         m_lastSearch = processSearchResponse(*remoteQueryResponse);
                       }
                     });

  //LOG(Warn, remoteUrl() + toString(builder.to_string()));

  return true;
}

bool RemoteBCL::waitForLock(int msec) const {
  int msecPerLoop = 20;
  int numTries = msec / msecPerLoop;
  int current = 0;
  while (true) {
    // if no request was made and the optional is empty return
    if (!m_httpResponse) {
      return false;
    }

    // if we can get the lock then the download is complete
    if (m_httpResponse->is_done()) {
      return true;
    }

    // DLM: no longer calls Application::instance().processEvents(msecPerLoop);
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

boost::optional<RemoteQueryResponse> RemoteBCL::processReply(const std::string& reply) {
  std::shared_ptr<pugi::xml_document> document = std::make_shared<pugi::xml_document>();
  auto result = document->load_string(reply.c_str());
  if (!result) {
    LOG(Error, "Bad XML Response: " << result.description());
  } else {
    if (!m_useRemoteDevelopmentUrl) {
      validProdAuthKey = true;
    } else {
      validDevAuthKey = true;
    }
    return RemoteQueryResponse(document);
  }

  return boost::none;
}

boost::optional<BCLMetaSearchResult> RemoteBCL::processMetaSearchResponse(const RemoteQueryResponse& remoteQueryResponse) {
  auto root = remoteQueryResponse.root();

  if (root) {
    if (std::string(root.name()).compare(0, 6, "result") == 0) {  // C++20: std::string(root.name()).starts_with("result")
      auto numResultsElement = root.child("result_count");
      if (numResultsElement) {
        return BCLMetaSearchResult(root);
      }
    }
  }

  return boost::none;
}

std::vector<BCLSearchResult> RemoteBCL::processSearchResponse(const RemoteQueryResponse& remoteQueryResponse) {
  std::vector<BCLSearchResult> searchResults;

  auto root = remoteQueryResponse.root();
  auto result = root.child("result");
  if (result) {
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

void RemoteBCL::onDownloadComplete() {
  const auto src = m_downloadFile->fileName();
  std::string componentType;

  // Extract the files to a temp location
  openstudio::path tempDest = openstudio::filesystem::temp_directory_path() / toPath(m_downloadUid + '/');

  if (openstudio::filesystem::is_directory(tempDest)) {
    removeDirectory(tempDest);
  }
  openstudio::filesystem::create_directories(tempDest);

  std::vector<openstudio::path> createdFiles;
  try {
    openstudio::UnzipFile uf(src);
    createdFiles = uf.extractAllFiles(tempDest);
  } catch (const std::exception& e) {
    LOG(Error, "Cannot unzip file: " << e.what());
  }
  openstudio::filesystem::remove(src);

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

  if (xmlPath) {
    // cppcheck-suppress shadowVariable
    path src = xmlPath->parent_path();
    path dest = src.parent_path();
    openstudio::filesystem::remove(dest / toPath("DISCLAIMER.txt"));
    openstudio::filesystem::remove(dest / toPath("README.txt"));
    openstudio::filesystem::remove(dest / toPath("output.xml"));
    copyDirectory(src, dest);
    removeDirectory(src);

    if (componentType == "component") {
      path componentXmlPath = dest / toPath("component.xml");
      // open the component to figure out uid and vid
      BCLComponent component(toString(componentXmlPath.parent_path()));
      std::string uid = component.uid();
      std::string versionId = component.versionId();

      // check if component has proper uid and vid
      if (!uid.empty() && !versionId.empty()) {
        dest = LocalBCL::instance().libraryPath() / uid / versionId;

        removeDirectory(dest);
        if (copyDirectory(componentXmlPath.parent_path(), dest)) {
          // Add to LocalBCL
          m_lastComponentDownload = BCLComponent(toString(dest));
          LocalBCL::instance().addComponent(*m_lastComponentDownload);
        }
      }
    } else if (componentType == "measure") {
      path measureXmlPath = dest / toPath("measure.xml");
      // open the measure to figure out uid and vid
      boost::optional<BCLMeasure> measure;
      try {
        measure = BCLMeasure(measureXmlPath.parent_path());

        std::string uid = measure->uid();
        std::string versionId = measure->versionId();

        // check if component has proper uid and vid
        if (!uid.empty() && !versionId.empty()) {
          dest = LocalBCL::instance().libraryPath() / uid / versionId;
          removeDirectory(dest);
          if (copyDirectory(measureXmlPath.parent_path(), dest)) {
            // Add to LocalBCL
            m_lastMeasureDownload = BCLMeasure(dest);
            LocalBCL::instance().addMeasure(*m_lastMeasureDownload);
          }
        }
      } catch (const std::exception&) {
        LOG(Error, "Unable to create measure from download: " + toString(measureXmlPath.parent_path()));
      }
    }
  } else {
    LOG(Error, "No component.xml or measure.xml file found in downloaded contents");
  }

  // delete the temp unzip directory
  removeDirectory(tempDest);

  if (componentType == "measure") {
    this->measureDownloaded.fire(m_downloadUid, m_lastMeasureDownload);
  } else {
    this->componentDownloaded.fire(m_downloadUid, m_lastComponentDownload);
  }
}

int RemoteBCL::setResultsPerQuery(const int numResults) {
  m_numResultsPerQuery = numResults <= 0 ? 10 : numResults > 100 ? 100 : numResults;
  return m_numResultsPerQuery;
}

void RemoteBCL::setLastTotalResults(const int lastTotalResults) {
  m_lastTotalResults = lastTotalResults;
}

}  // namespace openstudio
