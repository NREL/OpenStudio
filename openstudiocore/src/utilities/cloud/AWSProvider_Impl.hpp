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

#ifndef UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP

#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/AWSProvider.hpp>

#include <QProcess>

namespace openstudio{
namespace detail{

  /// AWSSettings_Impl is a CloudSettings_Impl.
  class UTILITIES_API AWSSettings_Impl : public CloudSettings_Impl {
   public:

    /** @name Constructor */
    //@{

    /// default constructor, loads defaults from settings
    AWSSettings_Impl();

    /** Constructor provided for deserialization; not for general use. */
    AWSSettings_Impl(const UUID& uuid,
                     const UUID& versionUUID,
                     bool userAgreementSigned);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSSettings_Impl() {};

    //@}
    /** @name Inherited members */
    //@{

    virtual std::string cloudProviderType() const;

    virtual std::string userAgreementText() const;

    virtual bool userAgreementSigned() const;

    virtual void signUserAgreement(bool agree);

    virtual bool loadSettings(bool overwriteExisting = false);

    virtual bool saveToSettings(bool overwriteExisting = false) const;

    //@}
    /** @name Class members */
    //@{

    // returns the AWS access key
    std::string accessKey() const;

    // sets the AWS access key if it's valid
    bool setAccessKey(const std::string& accessKey);

    // returns the AWS secret key
    std::string secretKey() const;

    // sets the AWS secret key if it's valid
    bool setSecretKey(const std::string& secretKey);

    // performs a cursory regex and returns true if it's valid
    bool validAccessKey(const std::string& accessKey) const;

    // performs a cursory regex and returns true if it's valid
    bool validSecretKey(const std::string& secretKey) const;

    // returns true if there should be a delay before terminating after simulations are complete
    bool terminationDelayEnabled();

    // sets whether a termination delay should occur
    void setTerminationDelayEnabled(bool enabled);

    // returns the termination delay in minutes
    unsigned terminationDelay();

    // sets the termination delay in minutes
    void setTerminationDelay(const unsigned delay);

    //@}
   private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSSettings");

    bool m_userAgreementSigned;
    mutable std::string m_accessKey;
    mutable std::string m_secretKey;
    mutable bool m_validAccessKey;
    mutable bool m_validSecretKey;
    bool m_terminationDelayEnabled;
    unsigned m_terminationDelay;
  };

  /// AWSSession_Impl is a CloudSession_Impl.
  class UTILITIES_API AWSSession_Impl : public CloudSession_Impl {
  public:
    /** @name Constructor */
    //@{

    AWSSession_Impl(const std::string& sessionId,
                        const boost::optional<Url>& serverUrl,
                        const std::vector<Url>& workerUrls);

    /** Constructor provided for deserialization; not for general use. */
    AWSSession_Impl(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& sessionId,
                        const boost::optional<Url>& serverUrl,
                        const std::vector<Url>& workerUrls);

    //@}
    /** @name Destructors */
    //@{

    virtual ~AWSSession_Impl() {};

    //@}
    /** @name Inherited members */
    //@{

    virtual std::string cloudProviderType() const;

    //@}
    /** @name Class members */
    //@{
    
    // returns the url of the server node
    Url serverUrl() const;

    // sets the url of the server node
    void setServerUrl(const Url& serverUrl);

    // returns the urls of all worker nodes 
    std::vector<Url> workerUrls() const;

    // set the urls of all worker nodes
    void setWorkerUrls(const std::vector<Url>& workerUrls);

    // returns the key pair's private key
    std::string privateKey() const;

    // sets the key pair's private key
    void setPrivateKey(const std::string& privateKey);

    // returns the timestamp associated with the security group and key pair
    std::string timestamp() const;

    // sets the timestamp
    void setTimestamp(const std::string& timestamp);

    // returns the AWS region
    std::string region() const;

    // sets the AWS region
    void setRegion(const std::string& region);

    // returns the server instance type
    std::string serverInstanceType() const;

    // sets the server instance type
    void setServerInstanceType(const std::string& instanceType);

    // returns the worker instance type
    std::string workerInstanceType() const;

    // sets the worker instance type
    void setWorkerInstanceType(const std::string& instanceType);

    //@}

  private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSSession");

    Url m_serverUrl;

    std::vector<Url> m_workerUrls;

    std::string m_timestamp;

    std::string m_region;

    std::string m_serverInstanceType;

    std::string m_workerInstanceType;
  };

  /// AWSProvider is a CloudProvider that provides access to Amazon EC2 and CloudWatch services.
  class UTILITIES_API AWSProvider_Impl : public CloudProvider_Impl {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor, loads settings
    AWSProvider_Impl();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSProvider_Impl() {};

    //@}
    /** @name Inherited accessor class members */
    //@{

    virtual std::string type() const;

    virtual unsigned numWorkers() const;

    virtual CloudSettings settings() const;

    virtual bool setSettings(const CloudSettings& settings);

    virtual CloudSession session() const;

    virtual bool setSession(const CloudSession& session);

    virtual bool lastInternetAvailable() const;

    virtual bool lastServiceAvailable() const;

    virtual bool lastValidateCredentials() const;

    virtual bool serverStarted() const;

    virtual bool workersStarted() const;

    virtual bool running() const;

    virtual bool terminateStarted() const;

    virtual bool terminateCompleted() const;

    virtual std::vector<std::string> errors() const;
    
    virtual std::vector<std::string> warnings() const;

    //@}
    /** @name Inherited blocking class members */
    //@{

    virtual bool internetAvailable(int msec);

    virtual bool serviceAvailable(int msec);

    virtual bool validateCredentials(int msec);

    virtual bool waitForServer(int msec);

    virtual bool waitForWorkers(int msec);

    virtual bool waitForTerminated(int msec);

    //@}
    /** @name Inherited non-blocking class members */
    //@{

    virtual bool requestInternetAvailable();

    virtual bool requestServiceAvailable();

    virtual bool requestValidateCredentials();

    virtual bool requestStartServer();

    virtual bool requestStartWorkers();

    virtual bool requestTerminate();

    //@}
    /** @name Class members */
    //@{

    // returns the cloud provider type
    static std::string cloudProviderType();

    std::string userAgreementText() const;

    bool userAgreementSigned() const;

    void signUserAgreement(bool agree);

    // returns the AWS access key
    std::string accessKey() const;

    // sets the AWS access key if it's valid
    bool setAccessKey(const std::string& accessKey);

    // returns the AWS secret key
    std::string secretKey() const;

    // sets the AWS secret key if it's valid
    bool setSecretKey(const std::string& secretKey);

    // run an action against the AWS-SDK ruby gem
    QVariantMap awsRequest(std::string request, std::string service = "EC2") const;

    // set the number of worker nodes to start
    void setNumWorkers(const unsigned numWorkers);

    // return a list of available AWS regions
    std::vector<std::string> availableRegions() const;

    // returns the AWS region
    std::string region() const;

    // sets the AWS region
    void setRegion(const std::string& region);

    // returns a list of server instance types
    std::vector<std::string> serverInstanceTypes() const;

    // returns the recommended default server instance type
    std::string defaultServerInstanceType() const;

    // returns the server instance type
    std::string serverInstanceType() const;

    // sets the server instance type
    void setServerInstanceType(const std::string& instanceType);

    // returns a list of worker instance types
    std::vector<std::string> workerInstanceTypes() const;

    // returns the recommended default worker instance type
    std::string defaultWorkerInstanceType() const;

    // returns the worker instance type
    std::string workerInstanceType() const;

    // sets the worker instance type
    void setWorkerInstanceType(const std::string& instanceType);

    // returns true if there should be a delay before terminating after simulations are complete
    bool terminationDelayEnabled();

    // sets whether a termination delay should occur
    void setTerminationDelayEnabled(bool enabled);

    // returns the termination delay in minutes
    unsigned terminationDelay();

    // sets the termination delay in minutes
    void setTerminationDelay(const unsigned delay);

    //@}

  private slots:

    void onServerStarted(int, QProcess::ExitStatus);

    void onWorkersStarted(int, QProcess::ExitStatus);

    void onServerStopped(int, QProcess::ExitStatus);

    void onWorkersStopped(int, QProcess::ExitStatus);

  private:

    bool requestInternetAvailableRequestFinished() const;
    bool requestServiceAvailableFinished() const;
    bool requestValidateCredentialsFinished() const;

    AWSSettings m_awsSettings;
    AWSSession m_awsSession;

    unsigned m_numWorkers;
    std::vector<std::string> m_regions;
    std::vector<std::string> m_serverInstanceTypes;
    std::vector<std::string> m_workerInstanceTypes;

    QProcess* m_startServerProcess;
    QProcess* m_startWorkersProcess;
    QProcess* m_stopServerProcess;
    QProcess* m_stopWorkersProcess;
    bool m_lastInternetAvailable;
    bool m_lastServiceAvailable;
    bool m_lastValidateCredentials;
    bool m_serverStarted;
    bool m_workersStarted;
    bool m_serverStopped;
    bool m_workersStopped;
    bool m_terminateStarted;

    mutable std::vector<std::string> m_errors;
    mutable std::vector<std::string> m_warnings;

    void clearErrorsAndWarnings() const;
    void logError(const std::string& error) const;
    void logWarning(const std::string& warning) const;

    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSProvider");

  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP
