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

#ifndef UTILITIES_CLOUD_AWSPROVIDER_HPP
#define UTILITIES_CLOUD_AWSPROVIDER_HPP

#include <utilities/cloud/CloudProvider.hpp>

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
                const UUID& versionUUID);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSSettings();

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{
    //@}
   protected:

    AWSSettings(const boost::shared_ptr<detail::AWSSettings_Impl>& impl);

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
               const std::vector<Url>& workerUrls);

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSSession();

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

   protected:

    AWSSession(const boost::shared_ptr<detail::AWSSession_Impl>& impl);

    typedef detail::AWSSession_Impl ImplType;

    friend class CloudSession;

   private:

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

    // returns the current number of worker nodes
    unsigned numWorkers();

    //@}
  protected:

    AWSProvider(const boost::shared_ptr<detail::AWSProvider_Impl>& impl);

    typedef detail::AWSProvider_Impl ImplType;

    friend class CloudProvider;

  private:

    // no body on purpose, do not want this generated
    AWSProvider(const AWSProvider& other);

  };

} // openstudio

#endif // UTILITIES_CLOUD_AWSPROVIDER_HPP
