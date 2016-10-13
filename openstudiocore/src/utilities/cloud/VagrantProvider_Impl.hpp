/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

    virtual std::string cloudProviderType() const override;

    virtual std::string userAgreementText() const override;

    virtual bool userAgreementSigned() const override;

    virtual void signUserAgreement(bool agree) override;

    virtual bool loadSettings(bool overwriteExisting = false) override;

    virtual bool saveToSettings(bool overwriteExisting = false) const override;

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

    virtual std::string cloudProviderType() const override;

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

    virtual std::string type() const override;

    virtual unsigned numWorkers() const override;

    virtual CloudSettings settings() const override;

    virtual bool setSettings(const CloudSettings& settings) override;

    virtual CloudSession session() const override;

    virtual bool setSession(const CloudSession& session) override;

    virtual bool lastInternetAvailable() const override;

    virtual bool lastServiceAvailable() const override;

    virtual bool lastValidateCredentials() const override;

    virtual bool lastResourcesAvailableToStart() const override;

    virtual bool serverStarted() const override;

    virtual bool workersStarted() const override;

    virtual bool lastServerRunning() const override;

    virtual bool lastWorkersRunning() const override;

    virtual bool terminateStarted() const override;

    virtual bool lastTerminateCompleted() const override;

    virtual std::vector<std::string> errors() const override;
    
    virtual std::vector<std::string> warnings() const override;

    //@}
    /** @name Inherited blocking class members */
    //@{

    virtual bool internetAvailable(int msec) override;

    virtual bool serviceAvailable(int msec) override;

    virtual bool validateCredentials(int msec) override;

    virtual bool resourcesAvailableToStart(int msec) override;

    virtual bool waitForServer(int msec) override;

    virtual bool waitForWorkers(int msec) override;

    virtual bool serverRunning(int msec) override;

    virtual bool workersRunning(int msec) override;

    virtual bool waitForTerminated(int msec) override;

    virtual bool terminateCompleted(int msec) override;

    //@}
    /** @name Inherited non-blocking class members */
    //@{

    virtual bool requestInternetAvailable() override;

    virtual bool requestServiceAvailable() override;

    virtual bool requestValidateCredentials() override;

    virtual bool requestResourcesAvailableToStart() override;

    virtual bool requestStartServer() override;

    virtual bool requestStartWorkers() override;

    virtual bool requestServerRunning() override;

    virtual bool requestWorkersRunning() override;

    virtual bool requestTerminate() override;

    virtual bool requestTerminateCompleted() override;

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
