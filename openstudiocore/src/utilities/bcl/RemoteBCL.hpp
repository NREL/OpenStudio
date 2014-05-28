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

#ifndef UTILITIES_BCL_REMOTEBCL_HPP
#define UTILITIES_BCL_REMOTEBCL_HPP

#include "BCL.hpp"
#include "OnDemandGenerator.hpp"
#include "../core/Path.hpp"

#include <QDomDocument>

class QNetworkAccessManager;
class QNetworkRequest;
class QMutex;
class QSslError;

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
    virtual boost::optional<BCLComponent> getComponent(const std::string& uid, const std::string& versionId = "") const;

    /// Get the measure by uid
    virtual boost::optional<BCLMeasure> getMeasure(const std::string& uid, const std::string& versionId = "") const;
    
    /// Searches the library for an on demand generated component matching this generator.
    /// This generator should have values for all arguments set.
    virtual boost::optional<BCLComponent> getOnDemandComponent(const OnDemandGenerator& generator) const;

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

    /// Returns the last downloaded component if there is one
    boost::optional<BCLComponent> lastComponentDownload() const;

    /// Returns the last downloaded measure if there is one
    boost::optional<BCLMeasure> lastMeasureDownload() const;

    /// Returns the last on demand generator if there is one
    boost::optional<OnDemandGenerator> lastOnDemandGenerator() const;

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
    boost::optional<OnDemandGenerator> waitForOnDemandGenerator(int msec = 50000) const;

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

    /// Starts downloading an on demand generator definition, if successful this will start a download
    bool downloadOnDemandGenerator(const std::string& uid);

    /// Call an on demand generator, if successful this will start a download
    bool callOnDemandGenerator(const OnDemandGenerator& generator);

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

    /// Emitted when an on demand generator request completes
    void onDemandGeneratorRecieved(const std::string& uid, const boost::optional<OnDemandGenerator>& onDemandGenerator) const;

    /// Emitted when a meta search request completes
    void metaSearchCompleted(const boost::optional<BCLMetaSearchResult>& metaSearchResult) const;

    /// Emitted when a search request completes
    void searchCompleted(const std::vector<BCLSearchResult>& searchResults) const;

  protected slots:

    void downloadData();

    void onDownloadComplete(QNetworkReply* reply);

    void onOnDemandGeneratorResponseComplete(QNetworkReply* reply);

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

    bool waitForLock(int msec) const;

    boost::optional<RemoteQueryResponse> processReply(QNetworkReply* reply);

    boost::optional<OnDemandGenerator> processOnDemandGeneratorResponse(const RemoteQueryResponse& remoteQueryResponse) const;

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

    boost::optional<OnDemandGenerator> m_lastOnDemandGenerator;

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
