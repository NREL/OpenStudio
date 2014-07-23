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

#ifndef UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP

#include "CloudProvider.hpp"
#include "CloudProvider_Impl.hpp"
#include "VagrantProvider.hpp"

#include "../core/Path.hpp"
#include "../core/Url.hpp"

#include <QProcess>

class QStringList;
class QString;
class QNetworkAccessManager;
class QNetworkReply;

namespace openstudio{
namespace detail{

  /// VagrantSettings_Impl is a CloudSettings_Impl.
  class UTILITIES_API VagrantSettings_Impl : public CloudSettings_Impl {
  public:

    /** @name Constructor */
    //@{

    VagrantSettings_Impl();

    VagrantSettings_Impl(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                         const openstudio::path& workerPath, const openstudio::Url& workerUrl,
                         bool haltOnStop, const std::string& username, const std::string& password,
                         bool terminationDelayEnabled, unsigned terminationDelay);

    /** Constructor provided for deserialization; not for general use. */
    VagrantSettings_Impl(const UUID& uuid,
                         const UUID& versionUUID,
                         bool userAgreementSigned,
                         const openstudio::path& serverPath,
                         const openstudio::Url& serverUrl,
                         const openstudio::path& workerPath,
                         const openstudio::Url& workerUrl,
                         bool haltOnStop,
                         const std::string& username,
                         bool terminationDelayEnabled, 
                         unsigned terminationDelay);

    //@}
    /** @name Destructors */
    //@{

    virtual ~VagrantSettings_Impl();

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

    openstudio::path serverPath()const; 

    void setServerPath(const openstudio::path& serverPath);

    openstudio::Url serverUrl() const;

    void setServerUrl(const openstudio::Url& serverUrl);

    openstudio::path workerPath() const;

    void setWorkerPath(const openstudio::path& workerPath);

    openstudio::Url workerUrl() const;

    void setWorkerUrl(const openstudio::Url& workerUrl);

    bool haltOnStop() const;

    void setHaltOnStop(bool haltOnStop);

    std::string username() const;

    void setUsername(const std::string& username);

    std::string password() const;

    void setPassword(const std::string& password);

    bool terminationDelayEnabled();

    void setTerminationDelayEnabled(bool enabled);

    unsigned terminationDelay();

    void setTerminationDelay(const unsigned delay);

    //@}

  private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.VagrantSettings");

    bool m_userAgreementSigned;
    openstudio::path m_serverPath;
    openstudio::Url m_serverUrl;
    openstudio::path m_workerPath;
    openstudio::Url m_workerUrl;
    bool m_haltOnStop;
    std::string m_username;
    std::string m_password;
    bool m_terminationDelayEnabled;
    unsigned m_terminationDelay;
  };

  /// VagrantSession_Impl is a CloudSession_Impl.
  class UTILITIES_API VagrantSession_Impl : public CloudSession_Impl {
  public:
    /** @name Constructor */
    //@{

    VagrantSession_Impl(const std::string& sessionId, 
                        const boost::optional<Url>& serverUrl, 
                        const std::vector<Url>& workerUrls);

    /** Constructor provided for deserialization; not for general use. */
    VagrantSession_Impl(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& sessionId,
                        const boost::optional<Url>& serverUrl,
                        const std::vector<Url>& workerUrls);
    
    //@}
    /** @name Destructors */
    //@{

    virtual ~VagrantSession_Impl();

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
    REGISTER_LOGGER("utilities.cloud.VagrantSession");
  };

  /// VagrantProvider is a CloudProvider that provides access to local Vagrant virtual machines for testing.
  class UTILITIES_API VagrantProvider_Impl : public CloudProvider_Impl {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor, loads settings
    VagrantProvider_Impl();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~VagrantProvider_Impl();

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

    //@}

  private slots:

    void processInternetAvailable();

    void onCheckServiceComplete(int, QProcess::ExitStatus);

    void onServerStarted(int, QProcess::ExitStatus);

    void onWorkerStarted(int, QProcess::ExitStatus);

    void onCheckServerRunningComplete(int, QProcess::ExitStatus);

    void onCheckWorkerRunningComplete(int, QProcess::ExitStatus);

    void onServerStopped(int, QProcess::ExitStatus);

    void onWorkerStopped(int, QProcess::ExitStatus);

    void onCheckTerminatedComplete(int, QProcess::ExitStatus);

  private:

    bool waitForFinished(int msec, const std::function<bool ()>& f);
    bool requestInternetAvailableRequestFinished() const;
    bool requestServiceAvailableFinished() const;
    bool requestValidateCredentialsFinished() const;
    bool requestResourcesAvailableToStartFinished() const;
    bool requestServerRunningFinished() const;
    bool requestWorkersRunningFinished() const;
    bool requestTerminateFinished() const;
    bool requestTerminateCompletedFinished() const;

    ProcessResults handleProcessCompleted(QProcess * t_qp);

    QProcess *makeCheckServiceProcess() const;
    QProcess *makeStartServerProcess() const;
    QProcess *makeStartWorkerProcess() const;
    QProcess *makeCheckServerRunningProcess() const;
    QProcess *makeCheckWorkerRunningProcess() const;
    QProcess *makeStopServerProcess() const;
    QProcess *makeStopWorkerProcess() const;
    QProcess *makeCheckTerminateProcess() const;

    bool parseServiceAvailableResults(const ProcessResults &);
    bool parseServerStartedResults(const ProcessResults &);
    bool parseWorkerStartedResults(const ProcessResults &);
    bool parseCheckServerRunningResults(const ProcessResults &);
    bool parseCheckWorkerRunningResults(const ProcessResults &);
    bool parseServerStoppedResults(const ProcessResults &);
    bool parseWorkerStoppedResults(const ProcessResults &);
    bool parseCheckTerminatedResults(const ProcessResults &);

    VagrantSettings m_vagrantSettings;
    VagrantSession m_vagrantSession;

    std::shared_ptr<QNetworkAccessManager> m_networkAccessManager;
    QNetworkReply* m_networkReply;

    QProcess* m_checkServiceProcess;
    QProcess* m_startServerProcess;
    QProcess* m_startWorkerProcess;
    QProcess* m_checkServerRunningProcess;
    QProcess* m_checkWorkerRunningProcess;
    QProcess* m_stopServerProcess;
    QProcess* m_stopWorkerProcess;
    QProcess* m_checkTerminatedProcess;
    bool m_lastInternetAvailable;
    bool m_lastServiceAvailable;
    bool m_lastValidateCredentials;
    bool m_lastResourcesAvailableToStart;
    bool m_serverStarted; // local state variable
    bool m_workerStarted; // local state variable
    bool m_lastServerRunning;
    bool m_lastWorkerRunning;
    bool m_serverStopped; // local state variable
    bool m_workerStopped; // local state variable
    bool m_terminateStarted; // local state variable
    bool m_lastTerminateCompleted;

    mutable std::vector<std::string> m_errors;
    mutable std::vector<std::string> m_warnings;

    void clearErrorsAndWarnings() const;
    void logError(const std::string& error) const;
    void logWarning(const std::string& warning) const;

    QString processName() const;
    void addProcessArguments(QStringList& args) const;

    // configure logging
    REGISTER_LOGGER("utilities.cloud.VagrantProvider");

  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP
