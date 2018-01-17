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

#ifndef UTILITIES_CLOUD_VAGRANTPROVIDER_HPP
#define UTILITIES_CLOUD_VAGRANTPROVIDER_HPP

#include "CloudProvider.hpp"

#include "../core/Path.hpp"
#include "../core/Url.hpp"

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
                    const std::string& username,
                    bool terminationDelayEnabled, 
                    unsigned terminationDelay);

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

    bool terminationDelayEnabled();

    void setTerminationDelayEnabled(bool enabled);

    unsigned terminationDelay();

    void setTerminationDelay(const unsigned delay);

    //@}

  protected:
    
    VagrantSettings(const std::shared_ptr<detail::VagrantSettings_Impl>& impl);
   
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

    VagrantSession(const std::shared_ptr<detail::VagrantSession_Impl>& impl);
    
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

  protected:

    VagrantProvider(const std::shared_ptr<detail::VagrantProvider_Impl>& impl);
    
    typedef detail::VagrantProvider_Impl ImplType;

    friend class CloudProvider;

  private:

  };

} // openstudio

#endif // UTILITIES_CLOUD_VAGRANTPROVIDER_HPP
