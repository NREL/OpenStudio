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

#ifndef UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP

#include "CloudProvider_Impl.hpp"
#include "AWSProvider.hpp"
#include "../core/Path.hpp"

#include <QProcess>
#include <QTemporaryFile>

namespace openstudio{

  struct AWSComputerInformation;

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
                     bool userAgreementSigned,
                     unsigned numWorkers,
                     bool terminationDelayEnabled,
                     unsigned terminationDelay,
                     std::string region,
                     std::string serverInstanceType,
                     std::string workerInstanceType);

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

    // returns the result of the last validAccessKey validation of the current accessKey
    bool validAccessKey() const;

    // performs a cursory regex and returns true if it's valid
    bool validSecretKey(const std::string& secretKey) const;

    // returns the result of the last validSecretKey validation of the current secretKey
    bool validSecretKey() const;

    // resets the AWS access key and deletes the secret key file
    void clearKeys();

    // returns the saved default number of workers
    unsigned numWorkers() const;

    // set the number of worker nodes to start (and returns the new number)
    unsigned setNumWorkers(const unsigned numWorkers);

    // returns true if there should be a delay before terminating after simulations are complete
    bool terminationDelayEnabled() const;

    // sets whether a termination delay should occur
    void setTerminationDelayEnabled(bool enabled);

    // returns the termination delay in minutes
    unsigned terminationDelay() const;

    // sets the termination delay in minutes
    void setTerminationDelay(const unsigned delay);

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
    REGISTER_LOGGER("utilities.cloud.AWSSettings");

    bool m_userAgreementSigned;
    std::string m_accessKey;
    std::string m_secretKey;
    bool m_validAccessKey;
    bool m_validSecretKey;
    unsigned m_numWorkers;
    bool m_terminationDelayEnabled;
    unsigned m_terminationDelay;
    std::string m_region;
    std::string m_serverInstanceType;
    std::string m_workerInstanceType;
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
                    const std::string& serverId,
                    const unsigned numServerProcessors,
                    const std::vector<Url>& workerUrls,
                    const std::vector<std::string>& workerIds,
                    const unsigned numWorkerProcessors,
                    const std::string& privateKey,
                    const std::string& timestamp,
                    const std::string& region,
                    const std::string& serverInstanceType,
                    const std::string& workerInstanceType);

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

    // returns the server instance ID
    std::string serverId() const;

    // sets the server instance ID
    void setServerId(const std::string& serverId);

    // returns the number of server processor cores
    unsigned numServerProcessors() const;

    // sets the number of server processor cores
    void setNumServerProcessors(const unsigned numServerProcessors);

    // returns the worker instance IDs
    std::vector<std::string> workerIds() const;

    // add a worker instance ID
    void addWorkerId(const std::string& workerId);

    // returns the number of processor cores per worker
    unsigned numWorkerProcessors() const;

    // sets the number of processor cores per worker
    void setNumWorkerProcessors(const unsigned numWorkerProcessors);

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

    // returns the total uptime in minutes of this session
    unsigned totalSessionUptime() const;

    // returns the total number of instances running on EC2 associated with this session
    unsigned totalSessionInstances() const;

    //@}

  private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSSession");

    std::string m_serverId;

    unsigned m_numServerProcessors;

    std::vector<std::string> m_workerIds;

    unsigned m_numWorkerProcessors;

    std::string m_privateKey;

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

    virtual bool lastResourcesAvailableToStart() const;

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

    virtual bool resourcesAvailableToStart(int msec);

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

    virtual bool requestResourcesAvailableToStart();

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

    // return a list of available AWS regions
    static std::vector<std::string> availableRegions();

    // return the recommended default region
    static std::string defaultRegion();

    // returns a list of server instance types
    static std::vector<std::string> serverInstanceTypes();

    // returns the recommended default server instance type
    static std::string defaultServerInstanceType();

    // returns a list of worker instance types
    static std::vector<std::string> workerInstanceTypes();

    // returns the recommended default worker instance type
    static std::string defaultWorkerInstanceType();

    static std::vector<unsigned> serverProcessorCounts();

    static std::vector<unsigned> workerProcessorCounts();

    static std::vector<std::string> serverPrettyNames();

    static std::vector<std::string> workerPrettyNames();

    static std::string getServerPrettyName(const std::string & instanceType);

    static std::string getWorkerPrettyName(const std::string & instanceType);

    static unsigned getServerProcessorCount(const std::string & instanceType);

    static unsigned getWorkerProcessorCount(const std::string & instanceType);

    static std::vector<AWSComputerInformation> serverInformation();  

    static std::vector<AWSComputerInformation> workerInformation();

    // returns the EC2 estimated charges from CloudWatch in USD
    double estimatedCharges(int msec);

    // returns the total number of instances running on EC2 in the current region
    unsigned totalInstances(int msec);

    bool requestEstimatedCharges();

    bool requestTotalInstances();

    double lastEstimatedCharges() const;

    unsigned lastTotalInstances() const;

    //@}

  signals:
    
    /// emitted when the estimated charges request completes
    void estimatedChargesAvailable();

    /// emitted when the total instances request completes
    void totalInstancesAvailable();

  private slots:

    void onCheckInternetComplete(int, QProcess::ExitStatus);

    void onCheckServiceComplete(int, QProcess::ExitStatus);

    void onCheckValidateComplete(int, QProcess::ExitStatus);

    void onCheckResourcesComplete(int, QProcess::ExitStatus);

    void onServerStarted(int, QProcess::ExitStatus);

    void onServerStartedError(QProcess::ProcessError error);

    void onWorkerStarted(int, QProcess::ExitStatus);

    void onCheckServerRunningComplete(int, QProcess::ExitStatus);

    void onCheckWorkerRunningComplete(int, QProcess::ExitStatus);

    void onInstancesStopped(int, QProcess::ExitStatus);

    void onCheckTerminatedComplete(int, QProcess::ExitStatus);

    void onCheckEstimatedChargesComplete(int, QProcess::ExitStatus);

    void onCheckTotalInstancesComplete(int, QProcess::ExitStatus);

  private:
    
    bool waitForFinished(int msec, const std::function<bool ()>& f);
    bool requestInternetAvailableFinished() const;
    bool requestServiceAvailableFinished() const;
    bool requestValidateCredentialsFinished() const;
    bool requestResourcesAvailableToStartFinished() const;
    bool requestServerStartedFinished() const;
    bool requestWorkerStartedFinished() const;
    bool requestServerRunningFinished() const;
    bool requestWorkersRunningFinished() const;
    bool requestTerminateFinished() const;
    bool requestTerminateCompletedFinished() const;
    bool requestEstimatedChargesFinished() const;
    bool requestTotalInstancesFinished() const;

    ProcessResults handleProcessCompleted(QProcess * t_qp);

    QProcess *makeCheckInternetProcess() const;
    QProcess *makeCheckServiceProcess() const;
    QProcess *makeCheckValidateProcess() const;
    QProcess *makeCheckResourcesProcess() const;
    QProcess *makeStartServerProcess() const;
    QProcess *makeStartWorkerProcess() const;
    QProcess *makeCheckServerRunningProcess() const;
    QProcess *makeCheckWorkerRunningProcess() const;
    QProcess *makeStopInstancesProcess() const;
    QProcess *makeCheckTerminateProcess() const;
    QProcess *makeCheckEstimatedChargesProcess() const;
    QProcess *makeCheckTotalInstancesProcess() const;

    bool parseServiceAvailableResults(const ProcessResults &);
    bool parseValidateCredentialsResults(const ProcessResults &);
    bool parseResourcesAvailableToStartResults(const ProcessResults &);
    bool parseServerStartedResults(const ProcessResults &);
    bool parseWorkerStartedResults(const ProcessResults &);
    bool parseCheckServerRunningResults(const ProcessResults &);
    bool parseCheckWorkerRunningResults(const ProcessResults &);
    bool parseInstancesStoppedResults(const ProcessResults &);
    bool parseCheckTerminatedResults(const ProcessResults &);
    double parseCheckEstimatedChargesResults(const ProcessResults &);
    unsigned parseCheckTotalInstancesResults(const ProcessResults &);

    bool userAgreementSigned() const;
    bool authenticated() const;

    AWSSettings m_awsSettings;
    AWSSession m_awsSession;

    path m_ruby;
    path m_script;
    mutable QTemporaryFile m_privateKey;

    QProcess* m_checkInternetProcess;
    QProcess* m_checkServiceProcess;
    QProcess* m_checkValidateProcess;
    QProcess* m_checkResourcesProcess;
    QProcess* m_startServerProcess;
    QProcess* m_startWorkerProcess;
    QProcess* m_checkServerRunningProcess;
    QProcess* m_checkWorkerRunningProcess;
    QProcess* m_stopInstancesProcess;
    QProcess* m_checkTerminatedProcess;
    QProcess* m_checkEstimatedChargesProcess;
    QProcess* m_checkTotalInstancesProcess;
    bool m_lastInternetAvailable;
    bool m_lastServiceAvailable;
    bool m_lastValidateCredentials;
    bool m_lastResourcesAvailableToStart;
    bool m_serverStarted;
    bool m_workerStarted;
    bool m_lastServerRunning;
    bool m_lastWorkerRunning;
    bool m_instancesStopped;
    bool m_terminateStarted;
    bool m_lastTerminateCompleted;
    double m_lastEstimatedCharges;
    unsigned m_lastTotalInstances;

    mutable std::vector<std::string> m_errors;
    mutable std::vector<std::string> m_warnings;

    void clearErrorsAndWarnings() const;
    void logError(const std::string& error) const;
    void logWarning(const std::string& warning) const;

    void addProcessArguments(QStringList& args) const;
    
    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSProvider");

  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP
