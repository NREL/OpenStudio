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
                     const UUID& versionUUID);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSSettings_Impl();

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
    //@}
   private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSSettings");
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

    virtual ~AWSSession_Impl();

    //@}
    /** @name Inherited members */
    //@{

    virtual std::string cloudProviderType() const;

    //@}
    /** @name Class members */
    //@{

    //@}

  private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSSession");
  };

  /// AWSProvider is a CloudProvider that provides access to Amazon EC2 and CloudWatch services.
  class UTILITIES_API AWSProvider_Impl : public CloudProvider_Impl {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor
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

    virtual bool lastServerRunning() const;

    virtual bool lastWorkersRunning() const;

    virtual bool terminateStarted() const;

    virtual bool lastTerminateCompleted() const;

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

    virtual bool serverRunning(int msec);

    virtual bool workersRunning(int msec);

    virtual bool waitForTerminated(int msec);

    virtual bool terminateCompleted(int msec);

    //@}
    /** @name Inherited non-blocking class members */
    //@{

    virtual bool requestInternetAvailable();

    virtual bool requestServiceAvailable();

    virtual bool requestValidateCredentials();

    virtual bool requestStartServer();

    virtual bool requestStartWorkers();

    virtual bool requestServerRunning();

    virtual bool requestWorkersRunning();

    virtual bool requestTerminate();

    virtual bool requestTerminateCompleted();

    //@}
    /** @name Class members */
    //@{

    // returns the cloud provider type
    static std::string cloudProviderType();

    // returns the AWS access key
    std::string accessKey() const;

    // returns the AWS secret key
    std::string secretKey() const;

    // performs a cursory regex validation of both keys, and returns true if they match
    bool setKeys(std::string accessKey, std::string secretKey) const;

    // run an action against the AWS-SDK ruby gem
    QVariantMap awsRequest(std::string request, std::string service = "EC2") const;

    // set the number of worker nodes to start
    void setNumWorkers(const unsigned numWorkers);

    //@}

  private slots:

    void onServerStarted(int, QProcess::ExitStatus);
    void onWorkerStarted(int, QProcess::ExitStatus);
    void onServerStopped(int, QProcess::ExitStatus);
    void onWorkerStopped(int, QProcess::ExitStatus);

  private:

    AWSSettings m_awsSettings;
    AWSSession m_awsSession;

    bool loadCredentials() const;
    bool saveCredentials() const;
    bool validAccessKey(std::string accessKey) const;
    bool validSecretKey(std::string secretKey) const;

    mutable std::string m_accessKey;
    mutable std::string m_secretKey;
    mutable bool m_validAccessKey;
    mutable bool m_validSecretKey;

    unsigned m_numWorkers;

    QProcess* m_startServerProcess;
    QProcess* m_startWorkerProcess;
    QProcess* m_stopServerProcess;
    QProcess* m_stopWorkerProcess;
    bool m_serverStarted;
    bool m_workerStarted;
    bool m_serverStopped;
    bool m_workerStopped;
    bool m_terminated;

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
