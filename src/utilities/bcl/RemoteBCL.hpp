/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_BCL_REMOTEBCL_HPP
#define UTILITIES_BCL_REMOTEBCL_HPP

#include "BCL.hpp"
#include "../core/Path.hpp"
#include "../core/Deprecated.hpp"

#if (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif
// Macro U from cpprestsdk is clashing with (cf boost https://github.com/microsoft/cpprestsdk/issues/1214)
#define _TURN_OFF_PLATFORM_STRING
#include <cpprest/http_client.h>
#if (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif

#include <nano/nano_signal_slot.hpp>
#include <pugixml.hpp>

#include <memory>

namespace openstudio {

/// This class is used to capture the xml response of a query and store it for later processing.
class UTILITIES_API RemoteQueryResponse
{
 public:
  RemoteQueryResponse(std::shared_ptr<pugi::xml_document>& domDocument);

  pugi::xml_node root() const;

 private:
  std::shared_ptr<pugi::xml_document> m_domDocument;
};

/// Class for accessing the remote BCL.
class UTILITIES_API RemoteBCL : public BCL
{
 public:
  /** @name Constructor */
  //@{

  /// Default constructor
  RemoteBCL();

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~RemoteBCL() override = default;

  //@}
  /** @name Inherited members */
  //@{

  /// Get the component by uid
  virtual boost::optional<BCLComponent> getComponent(const std::string& uid, const std::string& versionId = "") const override;

  /// Get the measure by uid
  virtual boost::optional<BCLMeasure> getMeasure(const std::string& uid, const std::string& versionId = "") const override;

  /// Perform a meta search on the library to identify number and types of results available.
  /// The total number of search results available can be used in the search method which requires a page number.
  boost::optional<BCLMetaSearchResult> metaSearchComponentLibrary(const std::string& searchTerm, const std::string& componentType,
                                                                  const std::string& filterType = "nrel_component") const;
  boost::optional<BCLMetaSearchResult> metaSearchComponentLibrary(const std::string& searchTerm, const unsigned componentTypeTID,
                                                                  const std::string& filterType = "nrel_component") const;

  /// Perform a component search of the library, results are returned in 'pages',
  /// the number of results per page is configurable and a metasearch should be performed
  /// to determine the total number of results pages available
  std::vector<BCLSearchResult> searchComponentLibrary(const std::string& searchTerm, const std::string& componentType, const unsigned page = 0) const;
  std::vector<BCLSearchResult> searchComponentLibrary(const std::string& searchTerm, const unsigned componentTypeTID, const unsigned page = 0) const;

  /// Perform a measure search of the library, results are returned in 'pages',
  /// the number of results per page is configurable and a metasearch should be performed
  /// to determine the total number of results pages available
  std::vector<BCLSearchResult> searchMeasureLibrary(const std::string& searchTerm, const std::string& componentType, const unsigned page = 0) const;
  std::vector<BCLSearchResult> searchMeasureLibrary(const std::string& searchTerm, const unsigned componentTypeTID, const unsigned page = 0) const;

  /// Compare the versionIds of all downloaded components/measures to the published version online
  /// and return the number that have updates
  int checkForComponentUpdates();
  int checkForMeasureUpdates();

  /// Return the uids for components and measures respectively that have updates available
  std::vector<BCLSearchResult> componentsWithUpdates() const;
  std::vector<BCLSearchResult> measuresWithUpdates() const;

  /// Replace local components and measures with the latest versions
  void updateComponents();
  void updateMeasures();

  //@}
  /** @name Blocking class members */
  //@{

  /// Returns true if there is an internet connection
  static bool isOnline();

  /// Returns the last downloaded component if there is one
  boost::optional<BCLComponent> lastComponentDownload() const;

  /// Returns the last downloaded measure if there is one
  boost::optional<BCLMeasure> lastMeasureDownload() const;

  /// Returns the last meta search result if there is one
  boost::optional<BCLMetaSearchResult> lastMetaSearch() const;

  /// Returns the last search results if there are any
  std::vector<BCLSearchResult> lastSearch() const;

  /// Get the current remote url
  std::string remoteUrl() const;

  /// Get the remote production url
  static std::string remoteProductionUrl();

  /// Get the remote development url
  static std::string remoteDevelopmentUrl();

  /// Use the development production server url
  void useRemoteDevelopmentUrl();

  /// Use the development production server url
  void useRemoteProductionUrl();

  /// Get the current OAuth key
  std::string authKey() const;

  /// Return production OAuth key
  std::string prodAuthKey() const;

  /// Set the production OAuth key
  bool setProdAuthKey(const std::string& prodAuthKey);

  /// Return development OAuth key
  std::string devAuthKey() const;

  /// Set the development OAuth key
  bool setDevAuthKey(const std::string& devAuthKey);

  /// Return the number of results per query
  int resultsPerQuery() const;

  /// Return the total number of results
  int lastTotalResults() const;

  /// Return the number of pages of results
  int numResultPages() const;

  unsigned timeOutSeconds() const;
  bool setTimeOutSeconds(unsigned timeOutSeconds);

  /// Wait number of milliseconds for download to complete
  /// Returns the download if it completed in the allowable time
  boost::optional<BCLComponent> waitForComponentDownload() const;
  OS_DEPRECATED(3, 2, 1) boost::optional<BCLComponent> waitForComponentDownload(int) const;

  /// Wait number of milliseconds for download to complete
  /// Returns the download if it completed in the allowable time
  boost::optional<BCLMeasure> waitForMeasureDownload() const;
  OS_DEPRECATED(3, 2, 1) boost::optional<BCLMeasure> waitForMeasureDownload(int) const;

  /// Wait number of milliseconds for download to complete
  /// Returns the download if it completed in the allowable time
  boost::optional<BCLMetaSearchResult> waitForMetaSearch() const;
  OS_DEPRECATED(3, 2, 1) boost::optional<BCLMetaSearchResult> waitForMetaSearch(int) const;

  /// Wait number of milliseconds for download to complete
  /// Returns the download if it completed in the allowable time
  std::vector<BCLSearchResult> waitForSearch() const;
  OS_DEPRECATED(3, 2, 1) std::vector<BCLSearchResult> waitForSearch(int) const;

  //@}
  /** @name Non-blocking class members */
  //@{

  /// Starts downloading an individual component by uid, if successful this will start a download
  bool downloadComponent(const std::string& uid);

  /// Starts downloading an individual measure by uid, if successful this will start a download
  bool downloadMeasure(const std::string& uid);

  /// Start a meta search, if successful this will start a download
  bool startComponentLibraryMetaSearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType);
  bool startComponentLibraryMetaSearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType);

  /// Start a search, if successful this will start a download
  bool startComponentLibrarySearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType,
                                   const unsigned page = 0);
  bool startComponentLibrarySearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType,
                                   const unsigned page = 0);

  //@}
  // signals:

  /// Emitted when a component download completes
  Nano::Signal<void(const std::string& uid, const boost::optional<BCLComponent>& component)> componentDownloaded;

  /// Emitted when a measure download completes
  Nano::Signal<void(const std::string& uid, const boost::optional<BCLMeasure>& measure)> measureDownloaded;

 private:
  REGISTER_LOGGER("openstudio.RemoteBCL");

  // explicitly deleted copy constructor
  RemoteBCL(const RemoteBCL& other) = delete;

  // DLM: once this actually uses the website it will need a non-blocking implementation
  /// Validate an OAuth key
  bool validateAuthKey(const std::string& authKey, const std::string& remoteUrl);

  bool waitForLock() const;

  boost::optional<RemoteQueryResponse> processReply(const std::string& reply);

  static boost::optional<BCLMetaSearchResult> processMetaSearchResponse(const RemoteQueryResponse& remoteQueryResponse);

  static std::vector<BCLSearchResult> processSearchResponse(const RemoteQueryResponse& remoteQueryResponse);

  void onDownloadComplete();

  int setResultsPerQuery(const int numResults);

  void setLastTotalResults(const int lastTotalResults);

  // members

  // A helper function to prepare a client, allowing us to change the http_client_config in one place only
  static web::http::client::http_client getClient(const std::string& url, unsigned timeOutSeconds = 60);

  boost::optional<pplx::task<void>> m_httpResponse;

  struct DownloadFile
  {
    DownloadFile(const DownloadFile&) = delete;
    explicit DownloadFile(openstudio::path);
    void flush();
    void close();
    const openstudio::path& fileName() const noexcept;
    void write(const std::vector<unsigned char>& data);
    bool open();

   private:
    openstudio::path m_fileName;
    std::ofstream m_ofs;
  };

  std::unique_ptr<DownloadFile> m_downloadFile;

  std::string m_downloadUid;

  boost::optional<BCLComponent> m_lastComponentDownload;

  boost::optional<BCLMeasure> m_lastMeasureDownload;

  boost::optional<BCLMetaSearchResult> m_lastMetaSearch;

  std::vector<BCLSearchResult> m_lastSearch;

  std::string m_authKey;

  std::string m_prodAuthKey;

  std::string m_devAuthKey;

  bool m_useRemoteDevelopmentUrl;

  std::string m_remoteUrl;

  int m_numResultsPerQuery;

  int m_lastTotalResults;

  std::string m_apiVersion;

  bool validProdAuthKey;

  bool validDevAuthKey;

  std::vector<BCLSearchResult> m_componentsWithUpdates;

  std::vector<BCLSearchResult> m_measuresWithUpdates;

  unsigned m_timeOutSeconds;
};

}  // namespace openstudio

#endif  // UTILITIES_BCL_REMOTEBCL_HPP
