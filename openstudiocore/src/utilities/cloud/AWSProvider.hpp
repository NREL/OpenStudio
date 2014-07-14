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

#ifndef UTILITIES_CLOUD_AWSPROVIDER_HPP
#define UTILITIES_CLOUD_AWSPROVIDER_HPP

#include "CloudProvider.hpp"

namespace openstudio {
namespace detail {

  class AWSSettings_Impl;
  class AWSSession_Impl;
  class AWSProvider_Impl;

}

  /// AWSSettings is a CloudSettings.
  class UTILITIES_API AWSSettings : public CloudSettings {
   public:
    /** @name Constructor */
    //@{

    /// default constructor, loads defaults from settings
    AWSSettings();

    /** Constructor provided for deserialization; not for general use. */
    AWSSettings(const UUID& uuid,
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
    virtual ~AWSSettings() {};

    //@}
    /** @name Inherited members */
    //@{

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
   protected:

    AWSSettings(const std::shared_ptr<detail::AWSSettings_Impl>& impl);

    typedef detail::AWSSettings_Impl ImplType;

    friend class CloudSettings;

   private:

  };

  /// \relates AWSSettings
  typedef boost::optional<AWSSettings> OptionalAWSSettings;

  /// AWSSession is a CloudSession.
  class UTILITIES_API AWSSession : public CloudSession {
   public:

    /** @name Constructor */
    //@{

    //constructor
    AWSSession(const std::string& sessionId,
               const boost::optional<Url>& serverUrl,
               const std::vector<Url>& workerUrls);

    /** Constructor provided for deserialization; not for general use. */
    AWSSession(const UUID& uuid,
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

    /// virtual destructor
    virtual ~AWSSession() {};

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

    // returns the server instance ID
    std::string serverId() const;

    // returns the number of server processor cores
    unsigned numServerProcessors() const;

    // returns the worker instance IDs
    std::vector<std::string> workerIds() const;

    // returns the number of processor cores per worker
    unsigned numWorkerProcessors() const;

    // returns the key pair's private key
    std::string privateKey() const;

    // returns the timestamp associated with the security group and key pair
    std::string timestamp() const;
    
    // returns the AWS region
    std::string region() const;

    // returns the server instance type
    std::string serverInstanceType() const;

    // returns the worker instance type
    std::string workerInstanceType() const;

    // returns the number of workers for this session
    unsigned numWorkers() const;

    // returns the total uptime in minutes of this session
    unsigned totalSessionUptime() const;

    // returns the total number of instances running on EC2 associated with this session
    unsigned totalSessionInstances() const;

    //@}

   protected:

    AWSSession(const std::shared_ptr<detail::AWSSession_Impl>& impl);

    typedef detail::AWSSession_Impl ImplType;

    friend class CloudSession;
    friend class detail::AWSProvider_Impl;

    // sets the server instance ID
    void setServerId(const std::string& serverId);

    // sets the number of server processor cores
    void setNumServerProcessors(const unsigned numServerProcessors);

    // add a worker instance ID
    void addWorkerId(const std::string& workerId);

    // sets the number of processor cores per worker
    void setNumWorkerProcessors(const unsigned numWorkerProcessors);

    // sets the key pair's private key
    void setPrivateKey(const std::string& privateKey);

    // sets the timestamp
    void setTimestamp(const std::string& timestamp);

    // sets the AWS region
    void setRegion(const std::string& region);

    // sets the server instance type
    void setServerInstanceType(const std::string& instanceType);

    // sets the worker instance type
    void setWorkerInstanceType(const std::string& instanceType);

   private:

  };

  struct UTILITIES_API AWSComputerInformation {
    public:
      std::string instanceType;
      std::string prettyName;
      unsigned processorCount;
  };

  /// \relates AWSSession
  typedef boost::optional<AWSSession> OptionalAWSSession;

  /// AWSProvider is a CloudProvider that provides access to Amazon EC2 and CloudWatch services.
  class UTILITIES_API AWSProvider : public CloudProvider {
  public:

    /** @name Constructor */
    //@{

    /// default constructor
    AWSProvider();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSProvider() {};

    //@}
    /** @name Class members */
    //@{

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

    // returns true if the cloud server successfully requests the estimated charges
    bool requestEstimatedCharges();

    // returns true if the cloud server successfully requests the total number of instances
    bool requestTotalInstances();

    // returns the EC2 estimated charges from CloudWatch in USD
    double estimatedCharges(int msec=30000);

    // returns the total number of instances running on EC2 in the current region
    unsigned totalInstances(int msec=30000);

    // returns the last value of the estimated charges check
    double lastEstimatedCharges() const;

    // returns the last value of the total instances check
    unsigned lastTotalInstances() const;

    //@}
  protected:

    AWSProvider(const std::shared_ptr<detail::AWSProvider_Impl>& impl);

    typedef detail::AWSProvider_Impl ImplType;

    friend class CloudProvider;

  };

} // openstudio

#endif // UTILITIES_CLOUD_AWSPROVIDER_HPP
