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

#ifndef UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP

#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>

#include <QProcess>

class QStringList;

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
                         bool haltOnStop, const std::string& username, const std::string& password);

    /** Constructor provided for deserialization; not for general use. */
    VagrantSettings_Impl(const UUID& uuid,
                         const UUID& versionUUID,
                         bool userAgreementSigned,
                         const openstudio::path& serverPath,
                         const openstudio::Url& serverUrl,
                         const openstudio::path& workerPath,
                         const openstudio::Url& workerUrl,
                         bool haltOnStop,
                         const std::string& username);

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
    /** @name Inherited members */
    //@{

    /// returns the name of this type of cloud provider, e.g. 'AWSProvider'
    /// blocking call
    virtual std::string type() const;

    /// returns true if this computer is connected to the internet
    /// blocking call, clears errors and warnings
    virtual bool internetAvailable() const;

    /// returns true if the cloud service can be reached (e.g. ping)
    /// blocking call, clears errors and warnings
    virtual bool serviceAvailable() const;

    /// returns true if the cloud service validates user credentials
    /// blocking call, clears errors and warnings
    virtual bool validateCredentials() const;

    /// returns the current settings
    /// blocking call
    virtual CloudSettings settings() const;

    /// returns true if can assign settings
    /// blocking call, clears errors and warnings
    virtual bool setSettings(const CloudSettings& settings);

    /// returns the current session id
    /// blocking call
    virtual CloudSession session() const;

    /// returns true if can connect to a previously started sessionID using data in QSettings
    /// blocking call, clears errors and warnings
    virtual bool reconnect(const CloudSession& session);

    /// returns the ip address of the cloud server if it is started and running
    virtual boost::optional<Url> serverUrl() const;

    /// returns true if the cloud server successfully begins to start the server node
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    virtual bool startServer();

    /// returns the ip address of all cloud workers that are started and running
    virtual std::vector<Url> workerUrls() const;

    /// returns the number of workers to be requested
    virtual unsigned numWorkers() const;

    /// returns true if the cloud server successfully begins to start all worker nodes
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    virtual bool startWorkers();

    /// returns true if the server and all workers are running
    virtual bool running() const;

    /// returns true if the cloud server successfully begins to stop all nodes
    /// returns false if not running
    /// non-blocking call, clears errors and warnings
    virtual bool terminate();

    /// returns true if terminate has been called
    virtual bool terminated() const;

    /// returns errors generated by the last operation
    virtual std::vector<std::string> errors() const;
    
    /// returns warnings generated by the last operation
    virtual std::vector<std::string> warnings() const;

    //@}
    /** @name Class members */
    //@{

    // returns the cloud provider type
    static std::string cloudProviderType();

    /// returns true if server and worker have terminated 
    // DLM: give different name to avoid clash with signal terminateComplete
    bool is_terminateComplete() const;

    //@}

  private slots:

    void onServerStarted(int, QProcess::ExitStatus);
    void onWorkerStarted(int, QProcess::ExitStatus);
    void onServerStopped(int, QProcess::ExitStatus);
    void onWorkerStopped(int, QProcess::ExitStatus);

  private:

    VagrantSettings m_vagrantSettings;
    VagrantSession m_vagrantSession;

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

    QString processName() const;
    void addProcessArguments(QStringList& args) const;

    // configure logging
    REGISTER_LOGGER("utilities.cloud.VagrantProvider");

  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP
