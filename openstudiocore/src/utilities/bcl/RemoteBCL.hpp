/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_BCL_REMOTEBCL_HPP
#define UTILITIES_BCL_REMOTEBCL_HPP

#include "BCL.hpp"
#include "../core/Path.hpp"

#include <QDomDocument>

class QNetworkAccessManager;
class QNetworkRequest;
class QMutex;
class QSslError;
class QFile;

namespace openstudio{

  /// This class is used to capture the xml response of a query and store it for later processing.
  class UTILITIES_API RemoteQueryResponse {
  public:

    RemoteQueryResponse(const QDomDocument& domDocument);

    QDomDocument domDocument() const;

  private:

    QDomDocument m_domDocument;
  };

  /// Class for accessing the remote BCL.
  class UTILITIES_API RemoteBCL : public BCL {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// Default constructor
    RemoteBCL();

    //@}
    /** @name Destructor */
    //@{

    /// Virtual destructor
    virtual ~RemoteBCL();

    //@}
    /** @name Inherited members */
    //@{

    /// Get the component by uid
    virtual boost::optional<BCLComponent> getComponent(const std::string& uid, const std::string& versionId = "") const override;

    /// Get the measure by uid
    virtual boost::optional<BCLMeasure> getMeasure(const std::string& uid, const std::string& versionId = "") const override;

    /// Perform a meta search on the library to identify number and types of results available.
    /// The total number of search results available can be used in the search method which requires a page number.
    boost::optional<BCLMetaSearchResult> metaSearchComponentLibrary(const std::string& searchTerm,
      const std::string& componentType, const std::string& filterType = "nrel_component") const;
    boost::optional<BCLMetaSearchResult> metaSearchComponentLibrary(const std::string& searchTerm,
      const unsigned componentTypeTID, const std::string& filterType = "nrel_component") const;

    /// Perform a component search of the library, results are returned in 'pages',
    /// the number of results per page is configurable and a metasearch should be performed
    /// to determine the total number of results pages available
    std::vector<BCLSearchResult> searchComponentLibrary(const std::string& searchTerm,
      const std::string& componentType, const unsigned page = 0) const;
    std::vector<BCLSearchResult> searchComponentLibrary(const std::string& searchTerm,
      const unsigned componentTypeTID, const unsigned page = 0) const;

    /// Perform a measure search of the library, results are returned in 'pages',
    /// the number of results per page is configurable and a metasearch should be performed
    /// to determine the total number of results pages available
    std::vector<BCLSearchResult> searchMeasureLibrary(const std::string& searchTerm,
      const std::string& componentType, const unsigned page = 0) const;
    std::vector<BCLSearchResult> searchMeasureLibrary(const std::string& searchTerm,
      const unsigned componentTypeTID, const unsigned page = 0) const;

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
    bool isOnline() const;

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
    std::string remoteProductionUrl() const;

    /// Get the remote development url
    std::string remoteDevelopmentUrl() const;

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

    /// Wait number of milliseconds for download to complete
    /// Returns the download if it completed in the allowable time
    boost::optional<BCLComponent> waitForComponentDownload(int msec = 50000) const;

    /// Wait number of milliseconds for download to complete
    /// Returns the download if it completed in the allowable time
    boost::optional<BCLMeasure> waitForMeasureDownload(int msec = 50000) const;

    /// Wait number of milliseconds for download to complete
    /// Returns the download if it completed in the allowable time
    boost::optional<BCLMetaSearchResult> waitForMetaSearch(int msec = 50000) const;

    /// Wait number of milliseconds for download to complete
    /// Returns the download if it completed in the allowable time
    std::vector<BCLSearchResult> waitForSearch(int msec = 50000) const;

    static bool initializeSSL(const openstudio::path &t_pathToSSLLibraries = openstudio::path());

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
    bool startComponentLibrarySearch(const std::string& searchTerm, const std::string& componentType, const std::string& filterType, const unsigned page = 0);
    bool startComponentLibrarySearch(const std::string& searchTerm, const unsigned componentTypeTID, const std::string& filterType, const unsigned page = 0);

    //@}
  signals:

    /// Emitted when a component download completes
    void componentDownloaded(const std::string& uid, const boost::optional<BCLComponent>& component) const;

    /// Emitted when a measure download completes
    void measureDownloaded(const std::string& uid, const boost::optional<BCLMeasure>& measure) const;

    /// Emitted when a meta search request completes
    void metaSearchCompleted(const boost::optional<BCLMetaSearchResult>& metaSearchResult) const;

    /// Emitted when a search request completes
    void searchCompleted(const std::vector<BCLSearchResult>& searchResults) const;

  protected slots:

    void downloadData();

    void onDownloadComplete(QNetworkReply* reply);

    void onMetaSearchResponseComplete(QNetworkReply* reply);

    void onSearchResponseComplete(QNetworkReply* reply);

  private slots:

    void catchSslErrors(QNetworkReply* reply, const QList<QSslError>& errorList);

  private:

    REGISTER_LOGGER("openstudio.RemoteBCL");

    // no body on purpose, do not want this generated
    RemoteBCL(const RemoteBCL& other);

    // DLM: once this actually uses the website it will need a non-blocking implementation
    /// Validate an OAuth key
    bool validateAuthKey(const std::string& authKey, const std::string& remoteUrl);

    QString checkForRedirect(const QNetworkReply* reply) const;

    bool waitForLock(int msec) const;

    boost::optional<RemoteQueryResponse> processReply(QNetworkReply* reply);

    boost::optional<BCLMetaSearchResult> processMetaSearchResponse(const RemoteQueryResponse& remoteQueryResponse) const;

    std::vector<BCLSearchResult> processSearchResponse(const RemoteQueryResponse& remoteQueryResponse) const;

    int setResultsPerQuery(const int numResults);

    void setLastTotalResults(const int lastTotalResults);

    // members

    QNetworkAccessManager* m_networkManager;

    QNetworkReply* m_downloadReply;

    QMutex* m_mutex;

    boost::optional<RemoteQueryResponse> m_queryResponse;

    std::shared_ptr<QFile> m_downloadFile;

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

  };

} // openstudio

#endif // UTILITIES_BCL_REMOTEBCL_HPP
