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

#ifndef UTILITIES_CLOUD_CLOUDPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_CLOUDPROVIDER_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/core/Url.hpp>
#include <utilities/core/Logger.hpp>

#include <QObject>

namespace openstudio{
  
  class CloudSettings;
  class CloudSession;

namespace detail{

  /// CloudSettings_Impl is an abstract base class that returns the information needed to use a CloudProvider (e.g. username, password, etc)
  class UTILITIES_API CloudSettings_Impl {
  public:

    /** Constructor provided for deserialization; not for general use. */
    CloudSettings_Impl(const UUID& uuid,
                       const UUID& versionUUID);

    virtual ~CloudSettings_Impl();

    /** Unique identifier for OSP. */
    UUID uuid() const;

    /** Unique identifier of version for OSP. (To avoid unnecessary overhead when nothing 
     *  has changed.) */
    UUID versionUUID() const;

    virtual std::string cloudProviderType() const = 0;

    virtual std::string userAgreementText() const = 0;

    virtual bool userAgreementSigned() const = 0;

    virtual void signUserAgreement(bool agree) = 0;

    virtual bool loadSettings(bool overwriteExisting = false) = 0;

    virtual bool saveToSettings(bool overwriteExisting = false) const = 0;

  protected:

    CloudSettings_Impl();

    /** Changes the versionUUID. */
    void onChange();

  private:
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudSettings");

    UUID m_uuid;
    UUID m_versionUUID;
  };

  /// CloudSession_Impl is an abstract base class for the information needed to identify and reconnect to compute nodes started by a previous CloudProvider.
  class UTILITIES_API CloudSession_Impl {
   public:

    /** Constructor provided for deserialization; not for general use. */
    CloudSession_Impl(const UUID& uuid,
                      const UUID& versionUUID,
                      const std::string& sessionId,
                      const boost::optional<Url>& serverUrl,
                      const std::vector<Url>& workerUrls);

    virtual ~CloudSession_Impl();

    /** Unique identifier for OSP. */
    UUID uuid() const;

    /** Unique identifier of version for OSP. (To avoid unnecessary overhead when nothing 
     *  has changed.) */
    UUID versionUUID() const;

    virtual std::string cloudProviderType() const = 0;
    
    std::string sessionId() const;
    
    boost::optional<Url> serverUrl() const;
    
    void setServerUrl(const Url& serverUrl);
    
    void resetServerUrl();
    
    std::vector<Url> workerUrls() const;
    
    void addWorkerUrl(const Url& workerUrl);
    
    void clearWorkerUrls();
  
   protected:
    CloudSession_Impl(const std::string& sessionId, 
                      const boost::optional<Url>& serverUrl, 
                      const std::vector<Url>& workerUrls);

    /** Changes the versionUUID. */
    void onChange();

   private:
    UUID m_uuid;
    UUID m_versionUUID;
    std::string m_sessionId;
    boost::optional<Url> m_serverUrl;
    std::vector<Url> m_workerUrls;

    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudSession");
  };

  /// CloudProvider_Impl is an abstract base class for classes that provide cloud resources.
  class UTILITIES_API CloudProvider_Impl : public QObject {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// constructor
    CloudProvider_Impl();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~CloudProvider_Impl();

    //@}
    /** @name Class members */
    //@{
        
    /// returns the name of this type of cloud provider, e.g. 'AWSProvider'
    /// blocking call
    virtual std::string type() const = 0;

    /// returns true if this computer is connected to the internet
    /// blocking call, clears errors and warnings
    virtual bool internetAvailable() const = 0;

    /// returns true if the cloud service can be reached (e.g. ping)
    /// blocking call, clears errors and warnings
    virtual bool serviceAvailable() const = 0;

    /// returns true if the cloud service validates user credentials
    /// blocking call, clears errors and warnings
    virtual bool validateCredentials() const = 0;

    /// returns the current settings
    /// blocking call
    virtual CloudSettings settings() const = 0;

    /// returns true if can assign settings
    /// blocking call, clears errors and warnings
    virtual bool setSettings(const CloudSettings& settings) = 0;

    /// returns the current session id
    /// blocking call
    virtual CloudSession session() const = 0;

    /// returns true if can connect to a previously started sessionID using data in QSettings
    /// blocking call, clears errors and warnings
    virtual bool reconnect(const CloudSession& session) = 0;

    /// returns the ip address of the cloud server if it is started and running
    virtual boost::optional<Url> serverUrl() const = 0;

    /// returns true if the cloud server successfully begins to start the server node
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    virtual bool startServer() = 0;

    /// returns the ip address of all cloud workers that are started and running
    virtual std::vector<Url> workerUrls() const = 0;

    /// returns the number of workers to be requested
    virtual unsigned numWorkers() const = 0;

    /// returns true if the cloud server successfully begins to start all worker nodes
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    virtual bool startWorkers() = 0;

    /// returns true if the server and all workers are running
    virtual bool running() const = 0;

    /// returns true if the cloud server successfully begins to stop all nodes
    /// returns false if not running
    /// non-blocking call, clears errors and warnings
    virtual bool terminate() = 0;

    /// returns true if terminate has been called
    virtual bool terminated() const = 0;

    /// returns errors generated by the last operation
    virtual std::vector<std::string> errors() const = 0;
    
    /// returns warnings generated by the last operation
    virtual std::vector<std::string> warnings() const = 0;

    //@}

  signals:
    
    /// emitted when the server is starting
    void serverStarting();

    /// emitted when the server has been started
    void serverStarted(const Url& url);

    /// emitted when a worker is starting
    void workerStarting();

    /// emitted when a worker has been started
    void workerStarted(const Url& url);

    /// emitted when all nodes have been started
    void allWorkersStarted();

    /// emitted when all nodes have begun to shut down
    void terminating();

    /// emitted when all nodes have been shut down
    void terminateComplete();

  private:
  
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudProvider");
  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_CLOUDPROVIDER_IMPL_HPP
