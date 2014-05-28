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

#ifndef UTILITIES_CLOUD_CLOUDPROVIDER_HPP
#define UTILITIES_CLOUD_CLOUDPROVIDER_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Optional.hpp"
#include "../core/UUID.hpp"
#include "../core/Url.hpp"
#include "../core/Logger.hpp"

#include <vector>

namespace openstudio{
  namespace detail {
    class CloudSettings_Impl;
    class CloudSession_Impl;
    class CloudProvider_Impl;
  }

  /// CloudSettings returns the information needed to use a CloudProvider (e.g. username, password, etc)
  class UTILITIES_API CloudSettings {
  public:

    virtual ~CloudSettings();

    /** Unique identifier for OSP. */
    UUID uuid() const;

    /** Unique identifier of version for OSP. (To avoid unnecessary overhead when nothing 
     *  has changed.) */
    UUID versionUUID() const;

    std::string cloudProviderType() const;

    /// returns the user agreement text
    std::string userAgreementText() const;

    /// returns true if the user has signed the user agreement
    bool userAgreementSigned() const;

    /// signs the user agreement if passed in true, unsigns if passed in false
    void signUserAgreement(bool agree);

    bool loadSettings(bool overwriteExisting = false);

    bool saveToSettings(bool overwriteExisting = false) const;

  //@}
  /** @name Type Casting */
  //@{

    template<typename T>
      std::shared_ptr<T> getImpl() const
    {  return std::dynamic_pointer_cast<T>(m_impl); }

    /// cast to type T, can throw std::bad_cast
    template<typename T>
    T cast() const{
      std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
      if (!impl){
        throw(std::bad_cast());
      }
      return T(impl);
    }

    /// cast to optional of type T
    template<typename T>
    boost::optional<T> optionalCast() const{
      boost::optional<T> result;
      std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
      if (impl){
        result = T(impl);
      }
      return result;
    }

  //@}

  protected:

    CloudSettings(const std::shared_ptr<detail::CloudSettings_Impl>& impl);

    typedef detail::CloudSettings_Impl ImplType;

  private:

    std::shared_ptr<detail::CloudSettings_Impl> m_impl;
    
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudSettings");
  };

  /// CloudSession returns the information needed to identify and reconnect to compute nodes started by a previous CloudProvider.
  class UTILITIES_API CloudSession {
  public:

    virtual ~CloudSession();

    /** Unique identifier for OSP. */
    UUID uuid() const;

    /** Unique identifier of version for OSP. (To avoid unnecessary overhead when nothing 
     *  has changed.) */
    UUID versionUUID() const;

    std::string cloudProviderType() const;

    std::string sessionId() const;

    boost::optional<Url> serverUrl() const;

    void setServerUrl(const Url& serverUrl);

    void resetServerUrl();

    std::vector<Url> workerUrls() const;

    void addWorkerUrl(const Url& workerUrl);

    void clearWorkerUrls();

  //@}
  /** @name Type Casting */
  //@{

    template<typename T>
      std::shared_ptr<T> getImpl() const
    {  return std::dynamic_pointer_cast<T>(m_impl); }

    /// cast to type T, can throw std::bad_cast
    template<typename T>
    T cast() const{
      std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
      if (!impl){
        throw(std::bad_cast());
      }
      return T(impl);
    }

    /// cast to optional of type T
    template<typename T>
    boost::optional<T> optionalCast() const{
      boost::optional<T> result;
      std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
      if (impl){
        result = T(impl);
      }
      return result;
    }

  //@}

  protected:

    CloudSession(const std::shared_ptr<detail::CloudSession_Impl>& impl);

    typedef detail::CloudSession_Impl ImplType;

  private:

    std::shared_ptr<detail::CloudSession_Impl> m_impl;
    
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudSession");
  };

  /// CloudProvider is an abstract base class for classes that provide cloud resources.
  /// CloudProvider is designed to be used for one time access.  Once the CloudProvider
  /// is terminated it cannot be used again.
  class UTILITIES_API CloudProvider {
  public:

    /** @name Constructor */
    //@{

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~CloudProvider();

    //@}
    /** @name Accessor class members */
    //@{

    /// returns the name of this type of cloud provider, e.g. 'AWSProvider'
    std::string type() const;

    /// returns the number of workers to be requested
    unsigned numWorkers() const;

    /// returns the current settings
    CloudSettings settings() const;

    /// returns true if can assign settings
    /// clears errors and warnings
    bool setSettings(const CloudSettings& settings);

    /// returns the current session
    CloudSession session() const;

    /// attempts to restore a previously started session
    /// returns false if the session does not have serverUrl or workerUrls specified
    /// this call does not check that the session is still valid or that the server can be reached
    /// clears errors and warnings
    bool setSession(const CloudSession& session);

    /// returns the last state of the internet available check
    bool lastInternetAvailable() const;

    /// returns the last state of the service available check
    bool lastServiceAvailable() const;

    /// returns the last state of the validate credentials check
    bool lastValidateCredentials() const;

    /// returns the last state of the resources available to start check
    bool lastResourcesAvailableToStart() const;

    /// returns true if requestStartServer has been called
    /// relies on local state variable, not network requests
    /// this will return true before serverRunning() returns true
    bool serverStarted() const;

    /// returns true if requestStartWorkers has been called
    /// relies on local state variable, not network requests
    /// this will return true before serverRunning() returns true
    bool workersStarted() const;

    /// returns the last state of requestServerRunning
    bool lastServerRunning() const;

    /// returns the last state of requestWorkersRunning
    bool lastWorkersRunning() const;

    /// returns true if lastServerRunning and lastWorkersRunning
    bool lastRunning() const;

    /// returns true if requestTerminate has been called
    /// relies on local state variable, not network requests
    /// this will return true before serverRunning() returns true
    bool terminateStarted() const;

    /// returns the last state of requestTerminateCompleted
    bool lastTerminateCompleted() const;

    /// returns errors generated by the last operation
    std::vector<std::string> errors() const;
    
    /// returns warnings generated by the last operation
    std::vector<std::string> warnings() const;

    //@}
    /** @name Blocking class members */
    //@{

    /// returns true if this computer is connected to the internet
    /// clears errors and warnings
    bool internetAvailable(int msec=30000);

    /// returns true if the cloud service can be reached (e.g. ping)
    /// clears errors and warnings
    bool serviceAvailable(int msec=30000);

    /// returns true if the cloud service validates user credentials
    /// blocking call, clears errors and warnings
    bool validateCredentials(int msec=30000);

    /// returns true if the cloud service has sufficient resources to start the requested instances
    /// blocking call, clears errors and warnings
    bool resourcesAvailableToStart(int msec=30000);

    /// waits up to msec milliseconds and returns true if server url becomes available in that time
    /// call requestStartServer before calling this function
    bool waitForServer(int msec=300000);

    /// waits up to msec milliseconds and returns true if worker urls become available in that time
    /// call requestStartWorkers before calling this function
    bool waitForWorkers(int msec=300000);

    /// returns true if the cloud service shows the server is running
    /// blocking call, clears errors and warnings
    bool serverRunning(int msec=30000);

    /// returns true if the cloud service shows the workers are running
    /// blocking call, clears errors and warnings
    bool workersRunning(int msec=30000);

    /// waits up to msec milliseconds and returns true if server and workers are fully terminated in that time
    /// call requestTerminate before calling this function
    bool waitForTerminated(int msec=300000);

    /// returns true if the cloud service shows the server and workers are terminated
    /// blocking call, clears errors and warnings
    bool terminateCompleted(int msec=30000);


    //@}
    /** @name Non-blocking class members */
    //@{

    /// starts a request to check for internet connectivity
    /// clears errors and warnings
    bool requestInternetAvailable();

    /// starts a request to check if the cloud service can be reached 
    /// clears errors and warnings
    bool requestServiceAvailable();

    /// starts a request to check if the cloud service validates user credentials
    /// clears errors and warnings
    bool requestValidateCredentials();

    /// starts a request to check if the cloud service has sufficient resources to start the requested instances
    /// clears errors and warnings
    bool requestResourcesAvailableToStart();

    /// returns true if the cloud server successfully begins to start the server node
    /// returns false if terminated, clears errors and warnings
    bool requestStartServer();

    /// returns true if the cloud server successfully begins to start all worker nodes
    /// returns false if terminated, clears errors and warnings
    bool requestStartWorkers();

    /// returns true if the cloud server successfully requests the running state of the server
    bool requestServerRunning();

    /// returns true if the cloud server successfully requests the running state of the workers
    bool requestWorkersRunning();

    /// returns true if the cloud server successfully begins to stop all nodes
    /// returns false if not running,  clears errors and warnings
    bool requestTerminate();

    /// returns true if the cloud server successfully requests the terminated state of the server and workers
    bool requestTerminateCompleted();
    

  //@}
  /** @name Type Casting */
  //@{

    template<typename T>
      std::shared_ptr<T> getImpl() const
    {  return std::dynamic_pointer_cast<T>(m_impl); }

    /// cast to type T, can throw std::bad_cast
    template<typename T>
    T cast() const{
      std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
      if (!impl){
        throw(std::bad_cast());
      }
      return T(impl);
    }

    /// cast to optional of type T
    template<typename T>
    boost::optional<T> optionalCast() const{
      boost::optional<T> result;
      std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
      if (impl){
        result = T(impl);
      }
      return result;
    }

  //@}

  protected:

    CloudProvider(const std::shared_ptr<detail::CloudProvider_Impl>& impl);

    typedef detail::CloudProvider_Impl ImplType;

  private:

    std::shared_ptr<detail::CloudProvider_Impl> m_impl;
    
    // configure logging
    REGISTER_LOGGER("utilities.cloud.CloudProvider");
  };

} // openstudio

#endif // UTILITIES_CLOUD_CLOUDPROVIDER_HPP
