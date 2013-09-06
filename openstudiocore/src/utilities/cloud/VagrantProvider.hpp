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

#ifndef UTILITIES_CLOUD_VAGRANTPROVIDER_HPP
#define UTILITIES_CLOUD_VAGRANTPROVIDER_HPP

#include <utilities/cloud/CloudProvider.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>

namespace openstudio{

  namespace detail{
    class VagrantSettings_Impl;
    class VagrantSession_Impl;
    class VagrantProvider_Impl;
  }

  /// VagrantSettings is a CloudSettings.
  class UTILITIES_API VagrantSettings : public CloudSettings {
  public:
    /** @name Constructor */
    //@{

    /// default constructor, loads defaults from settings
    VagrantSettings();

    /** Constructor provided for deserialization; not for general use. */
    VagrantSettings(const UUID& uuid,
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

    /// virtual destructor
    virtual ~VagrantSettings();

    //@}
    /** @name Inherited members */
    //@{

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

  protected:
    
    VagrantSettings(const boost::shared_ptr<detail::VagrantSettings_Impl>& impl);
   
    typedef detail::VagrantSettings_Impl ImplType;

    friend class CloudSettings;

  private:

  };

  /// VagrantSession is a CloudSession.
  class UTILITIES_API VagrantSession : public CloudSession {
  public:

    /** @name Constructor */
    //@{

    //constructor
    VagrantSession(const std::string& sessionId, 
                   const boost::optional<Url>& serverUrl, 
                   const std::vector<Url>& workerUrls);

    /** Constructor provided for deserialization; not for general use. */
    VagrantSession(const UUID& uuid,
                   const UUID& versionUUID,
                   const std::string& sessionId,
                   const boost::optional<Url>& serverUrl,
                   const std::vector<Url>& workerUrls);
    
    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~VagrantSession();

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

  protected:

    VagrantSession(const boost::shared_ptr<detail::VagrantSession_Impl>& impl);
    
    typedef detail::VagrantSession_Impl ImplType;

    friend class CloudSession;

  private:

  };

  /// VagrantProvider is a CloudProvider that provides access to local Vagrant virtual machines for testing.
  class UTILITIES_API VagrantProvider : public CloudProvider {
  public:

    /** @name Constructor */
    //@{

    /// default constructor, loads settings
    VagrantProvider();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~VagrantProvider();

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

    /// returns true if server and worker have terminated
    bool terminateComplete() const;

    //@}

  protected:

    VagrantProvider(const boost::shared_ptr<detail::VagrantProvider_Impl>& impl);
    
    typedef detail::VagrantProvider_Impl ImplType;

    friend class CloudProvider;

  private:

    // no body on purpose, do not want this generated
    VagrantProvider(const VagrantProvider& other);

  };

} // openstudio

#endif // UTILITIES_CLOUD_VAGRANTPROVIDER_HPP
