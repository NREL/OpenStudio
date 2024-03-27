/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "LocalBCL.hpp"
#include "RemoteBCL.hpp"
#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/StringHelpers.hpp"
#include "../core/System.hpp"
#include "../core/UnzipFile.hpp"
#include "../core/DeprecatedHelpers.hpp"

#include <regex>

#define REMOTE_PRODUCTION_SERVER "https://bcl.nrel.gov"
#define REMOTE_DEVELOPMENT_SERVER "https://bcl2.nrel.gov"

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
web::http::client::http_client RemoteBCL::getClient(const std::string& url, unsigned timeOutSeconds) {
  web::http::client::http_client_config config;
  // bcl.nrel.gov can be slow to respond to client requests so bump the default of 30 seconds to 60 to account for lengthy response time.
  // this is timeout is for each send and receive operation on the client and not the entire client session.
  config.set_timeout(std::chrono::seconds(timeOutSeconds));
  config.set_validate_certificates(false);

  return web::http::client::http_client(utility::conversions::to_string_t(url), config);
}

unsigned RemoteBCL::timeOutSeconds() const {
  return m_timeOutSeconds;
}
bool RemoteBCL::setTimeOutSeconds(unsigned timeOutSeconds) {
  if (timeOutSeconds < 10) {
    LOG(Error, "Setting a timeout of " << timeOutSeconds << " is too low.");
    return false;
  } else if (timeOutSeconds < 60) {
    LOG(Warn, "Setting a timeout of " << timeOutSeconds << " appears low and you risk failures to download components and measures");
  }
  m_timeOutSeconds = timeOutSeconds;
  return true;
}

bool RemoteBCL::DownloadFile::open() {
  OS_ASSERT(!m_fileName.empty());
  m_ofs.open(toString(m_fileName).c_str(), std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
  return m_ofs.good();
}

RemoteBCL::DownloadFile::DownloadFile(openstudio::path t_path) : m_fileName(std::move(t_path)) {}

void RemoteBCL::DownloadFile::flush() {
  if (m_ofs.good()) {
    m_ofs.flush();
  }
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
    validDevAuthKey(false),
    m_timeOutSeconds(120) {
  useRemoteProductionUrl();
}

///////////////////////////////////////////////////////////////////////////
/// Inherited members
///////////////////////////////////////////////////////////////////////////

boost::optional<BCLComponent> RemoteBCL::getComponent(const std::string& uid, const std::string& /*versionId*/) const {
  bool downloadStarted = const_cast<RemoteBCL*>(this)->downloadComponent(uid);
  if (downloadStarted) {
    return waitForComponentDownload();
  }
  return boost::none;
}

boost::optional<BCLMeasure> RemoteBCL::getMeasure(const std::string& uid, const std::string& /*versionId*/) const {
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
    return {};
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentType, "nrel_component", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return {};
}

std::vector<BCLSearchResult> RemoteBCL::searchComponentLibrary(const std::string& searchTerm, const unsigned componentTypeTID,
                                                               const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentTypeTID, "nrel_component");
  if (lastTotalResults() == 0) {
    return {};
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentTypeTID, "nrel_component", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return {};
}

std::vector<BCLSearchResult> RemoteBCL::searchMeasureLibrary(const std::string& searchTerm, const std::string& componentType,
                                                             const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentType, "nrel_measure");
  if (lastTotalResults() == 0) {
    return {};
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentType, "nrel_measure", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return {};
}

std::vector<BCLSearchResult> RemoteBCL::searchMeasureLibrary(const std::string& searchTerm, const unsigned componentTypeTID,
                                                             const unsigned page) const {
  // Perform metaSearch first
  metaSearchComponentLibrary(searchTerm, componentTypeTID, "nrel_measure");
  if (lastTotalResults() == 0) {
    return {};
  }

  bool searchStarted = const_cast<RemoteBCL*>(this)->startComponentLibrarySearch(searchTerm, componentTypeTID, "nrel_measure", page);
  if (searchStarted) {
    return waitForSearch();
  }
  return {};
}

int RemoteBCL::checkForComponentUpdates() {
  m_componentsWithUpdates.clear();

  for (const BCLComponent& component : LocalBCL::instance().components()) {
    // can't start another request until last one is done
    if (m_httpResponse && !m_httpResponse->is_done()) {
      return 0;
    }

    m_lastSearch.clear();

    auto client = getClient(remoteUrl(), m_timeOutSeconds);
    web::uri_builder builder(to_string_t("/api/search/"));

    builder.append_path(to_string_t("*.xml"));

    builder.append_query(to_string_t("fq[]"), to_string_t("ss_uuid:" + component.uid()));

    builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));

    // LOG(Debug, m_remoteUrl << builder.to_string());

    m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                       .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                       .then([this](const std::string& xml) {
                         auto remoteQueryResponse = processReply(xml);

                         if (remoteQueryResponse) {
                           m_lastSearch = processSearchResponse(*remoteQueryResponse);
                         }
                       });

    std::vector<BCLSearchResult> result = waitForSearch();
    if (!result.empty() && result[0].versionId() != component.versionId()) {
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
      return 0;
    }

    m_lastSearch.clear();

    auto client = getClient(remoteUrl(), m_timeOutSeconds);
    web::uri_builder builder(to_string_t("/api/search/"));

    builder.append_path(to_string_t("*.xml"));

    builder.append_query(to_string_t("fq[]"), to_string_t("ss_uuid:" + measure.uid()));

    builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));

    // LOG(Debug, m_remoteUrl << builder.to_string());

    m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                       .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                       .then([this](const std::string& xml) {
                         auto remoteQueryResponse = processReply(xml);

                         if (remoteQueryResponse) {
                           m_lastSearch = processSearchResponse(*remoteQueryResponse);
                         }
                       });

    std::vector<BCLSearchResult> result = waitForSearch();
    if (!result.empty() && result[0].versionId() != measure.versionId()) {
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
  if (m_componentsWithUpdates.empty()) {
    checkForComponentUpdates();
  }

  for (const BCLSearchResult& component : m_componentsWithUpdates) {
    downloadComponent(component.uid());
    boost::optional<BCLComponent> newComponent = waitForComponentDownload();

    if (newComponent) {
      LocalBCL::instance().removeOutdatedLocalComponents(newComponent->uid(), newComponent->versionId());
    }
  }
}

void RemoteBCL::updateMeasures() {
  if (m_measuresWithUpdates.empty()) {
    checkForMeasureUpdates();
  }

  for (const BCLSearchResult& measure : m_measuresWithUpdates) {
    downloadMeasure(measure.uid());
    boost::optional<BCLMeasure> newMeasure = waitForMeasureDownload();

    if (newMeasure) {
      LocalBCL::instance().removeOutdatedLocalMeasures(newMeasure->uid(), newMeasure->versionId());
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
  return {REMOTE_PRODUCTION_SERVER};
}

std::string RemoteBCL::remoteDevelopmentUrl() {
  return {REMOTE_DEVELOPMENT_SERVER};
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

    auto client = getClient(remoteUrl, m_timeOutSeconds);
    web::uri_builder builder(to_string_t("/api/search/"));

    builder.append_path(to_string_t("*.xml"));

    builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));
    builder.append_query(to_string_t("show_rows"), to_string_t("0"));

    // LOG(Debug, m_remoteUrl << builder.to_string());

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

boost::optional<BCLComponent> RemoteBCL::waitForComponentDownload() const {
  if (waitForLock()) {
    return m_lastComponentDownload;
  }
  return boost::none;
}

boost::optional<BCLComponent> RemoteBCL::waitForComponentDownload(int) const {
  DEPRECATED_AT_MSG(3, 2, 1, "The int parameter is unused. Use waitForComponentDownload() instead");
  return waitForComponentDownload();
}

boost::optional<BCLMeasure> RemoteBCL::waitForMeasureDownload() const {
  if (waitForLock()) {
    return m_lastMeasureDownload;
  }
  return boost::none;
}

boost::optional<BCLMeasure> RemoteBCL::waitForMeasureDownload(int) const {
  DEPRECATED_AT_MSG(3, 2, 1, "The int parameter is unused. Use waitForMeasureDownload() instead");
  return waitForMeasureDownload();
}

boost::optional<BCLMetaSearchResult> RemoteBCL::waitForMetaSearch() const {
  if (waitForLock()) {
    return m_lastMetaSearch;
  }
  return boost::none;
}

boost::optional<BCLMetaSearchResult> RemoteBCL::waitForMetaSearch(int) const {
  DEPRECATED_AT_MSG(3, 2, 1, "The int parameter is unused. Use waitForMetaSearch() instead");
  return waitForMetaSearch();
}

std::vector<BCLSearchResult> RemoteBCL::waitForSearch() const {
  if (waitForLock()) {
    return m_lastSearch;
  }
  return {};
}

std::vector<BCLSearchResult> RemoteBCL::waitForSearch(int) const {
  DEPRECATED_AT_MSG(3, 2, 1, "The int parameter is unused. Use waitForSearch() instead");
  return waitForSearch();
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
  auto client = getClient(remoteUrl(), m_timeOutSeconds);
  web::uri_builder builder(to_string_t("/api/component/download"));

  builder.append_query(to_string_t("uids"), to_string_t(uid));

  web::http::http_request msg(web::http::methods::GET);
  msg.headers().add(to_string_t("User-Agent"),
                    to_string_t("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17"));
  msg.set_request_uri(builder.to_string());

  // LOG(Debug, m_remoteUrl << builder.to_string());

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_vector(); })
                     .then([this](const std::vector<unsigned char>& zip) {
                       m_downloadFile->write(zip);
                       m_downloadFile->flush();
                       m_downloadFile->close();
                     })
                     .then([this]() { onDownloadComplete(); });

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

  auto client = getClient(remoteUrl(), m_timeOutSeconds);
  web::uri_builder builder(to_string_t("/api/metasearch/"));

  // web::uri::encode_data_string will Encodes a string by converting all characters
  // except for RFC 3986 unreserved characters to their hexadecimal representation. (eg: '+' => %2B, ' ' => %20)
  std::string query = searchTerm.empty() ? "*" : searchTerm;
  builder.append_path(web::uri::encode_data_string(utility::conversions::to_string_t(query + ".xml")));

  builder.append_query(to_string_t("fq[]"), to_string_t("bundle:" + filterType));

  if (!componentType.empty() && componentType != "*") {
    std::string filter = (filterType == "nrel_component") ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags";
    filter += ":\"" + componentType + "\"";
    builder.append_query(to_string_t("fq[]"), to_string_t(filter));
  }

  builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));

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

  // LOG(Debug, m_remoteUrl << builder.to_string());

  return true;
}

bool RemoteBCL::startComponentLibraryMetaSearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastMetaSearch.reset();

  auto client = getClient(remoteUrl(), m_timeOutSeconds);
  web::uri_builder builder(to_string_t("/api/metasearch/"));

  std::string query = searchTerm.empty() ? "*" : searchTerm;
  builder.append_path(web::uri::encode_data_string(utility::conversions::to_string_t(query + ".xml")));

  builder.append_query(to_string_t("fq[]"), to_string_t("bundle:" + filterType));

  if (componentTypeTID != 0) {
    std::string filter = "tid:" + openstudio::string_conversions::number(componentTypeTID);
    builder.append_query(to_string_t("fq[]"), to_string_t(filter));
  }

  builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));

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

  // LOG(Debug, m_remoteUrl << builder.to_string());

  return true;
}

bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType,
                                            const unsigned page) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastSearch.clear();

  auto client = getClient(remoteUrl(), m_timeOutSeconds);
  web::uri_builder builder(to_string_t("/api/search/"));

  std::string query = searchTerm.empty() ? "*" : searchTerm;
  builder.append_path(web::uri::encode_data_string(utility::conversions::to_string_t(query + ".xml")));

  builder.append_query(to_string_t("fq[]"), to_string_t("bundle:" + filterType));

  if (!componentType.empty() && componentType != "*") {
    std::string filter = (filterType == "nrel_component") ? "sm_vid_Component_Tags" : "sm_vid_Measure_Tags";
    filter += ":\"" + componentType + "\"";
    builder.append_query(to_string_t("fq[]"), to_string_t(filter));
  }

  builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));
  builder.append_query(to_string_t("show_rows"), to_string_t(openstudio::string_conversions::number(m_numResultsPerQuery)));
  builder.append_query(to_string_t("page"), to_string_t(openstudio::string_conversions::number(page)));

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) {
                       auto remoteQueryResponse = processReply(xml);

                       if (remoteQueryResponse) {
                         m_lastSearch = processSearchResponse(*remoteQueryResponse);
                       }
                     });

  // LOG(Debug, m_remoteUrl << builder.to_string());

  return true;
}

bool RemoteBCL::startComponentLibrarySearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType,
                                            const unsigned page) {
  // can't start another request until last one is done
  if (m_httpResponse && !m_httpResponse->is_done()) {
    return false;
  }

  m_lastSearch.clear();

  auto client = getClient(remoteUrl(), m_timeOutSeconds);
  web::uri_builder builder(to_string_t("/api/search/"));

  std::string query = searchTerm.empty() ? "*" : searchTerm;
  builder.append_path(web::uri::encode_data_string(utility::conversions::to_string_t(query + ".xml")));

  builder.append_query(to_string_t("fq[]"), to_string_t("bundle:" + filterType));

  if (componentTypeTID != 0) {
    std::string filter = "tid:" + openstudio::string_conversions::number(componentTypeTID);
    builder.append_query(to_string_t("fq[]"), to_string_t(filter));
  }
  builder.append_query(to_string_t("api_version"), to_string_t(m_apiVersion));
  builder.append_query(to_string_t("show_rows"), to_string_t(openstudio::string_conversions::number(m_numResultsPerQuery)));
  builder.append_query(to_string_t("page"), to_string_t(openstudio::string_conversions::number(page)));

  m_httpResponse = client.request(web::http::methods::GET, builder.to_string())
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) {
                       auto remoteQueryResponse = processReply(xml);

                       if (remoteQueryResponse) {
                         m_lastSearch = processSearchResponse(*remoteQueryResponse);
                       }
                     });

  // LOG(Debug, m_remoteUrl << builder.to_string());

  return true;
}

bool RemoteBCL::waitForLock() const {

  if (!m_httpResponse) {
    return false;
  }

  try {
    m_httpResponse->wait();
    return true;
  } catch (const std::exception& e) {
    LOG(Error, "Request to url '" << m_remoteUrl << " 'failed with message: " << e.what());
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
    while (componentElement.child("name") != nullptr) {
      //Skip components without a uid or version_id
      if ((componentElement.child("uuid") != nullptr) && (componentElement.child("vuuid") != nullptr)) {
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

  for (const auto& path : createdFiles) {
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
    this->measureDownloaded.nano_emit(m_downloadUid, m_lastMeasureDownload);
  } else {
    this->componentDownloaded.nano_emit(m_downloadUid, m_lastComponentDownload);
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
