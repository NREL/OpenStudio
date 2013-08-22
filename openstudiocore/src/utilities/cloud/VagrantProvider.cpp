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
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>

namespace openstudio{
  namespace detail{

    VagrantProvider_Impl::VagrantProvider_Impl(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                                               const openstudio::path& workerPath, const openstudio::Url& workerUrl)
      : CloudProvider_Impl(), m_cloudSession(this->type(), toString(createUUID()), boost::none, std::vector<Url>()),
        m_serverPath(serverPath), m_serverUrl(serverUrl), m_workerPath(workerPath), m_workerUrl(workerUrl)
    {
    }

    VagrantProvider_Impl::~VagrantProvider_Impl()
    {
    }

    std::string VagrantProvider_Impl::type() const
    {
      return "VagrantProvider";
    }

    /// returns true if this computer is connected to the internet
    /// blocking call, clears errors and warnings
    bool VagrantProvider_Impl::internetAvailable() const
    {
      clearErrorsAndWarnings();
      return false;
    }

    /// returns true if the cloud service can be reached (e.g. ping)
    /// blocking call, clears errors and warnings
    bool VagrantProvider_Impl::serviceAvailable() const
    {
      clearErrorsAndWarnings();
      return false;
    }

    /// returns true if the cloud service validates user credentials
    /// blocking call, clears errors and warnings
    bool VagrantProvider_Impl::validateCredentials() const
    {
      clearErrorsAndWarnings();
      return false;
    }

    /// returns the current session id
    /// blocking call
    CloudSession VagrantProvider_Impl::session() const
    {
      return m_cloudSession;
    }

    /// returns true if can connect to a previously started sessionID using data in QSettings
    /// blocking call, clears errors and warnings
    bool VagrantProvider_Impl::reconnect(const CloudSession& session)
    {
      clearErrorsAndWarnings();
      return false;
    }

    /// returns the ip address of the cloud server if it is started and running
    boost::optional<Url> VagrantProvider_Impl::serverUrl() const
    {
      return boost::none;
    }

    /// returns true if the cloud server successfully begins to start the server node
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    bool VagrantProvider_Impl::startServer()
    {
      clearErrorsAndWarnings();
      return false;
    }

    /// returns the ip address of all cloud workers that are started and running
    std::vector<Url> VagrantProvider_Impl::workerUrls() const
    {
      return std::vector<Url>();
    }

    unsigned VagrantProvider_Impl::numWorkers() const
    {
      return 1;
    }

    /// returns true if the cloud server successfully begins to start all worker nodes
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    bool VagrantProvider_Impl::startWorkers()
    {
      clearErrorsAndWarnings();
      return false;
    }

    bool VagrantProvider_Impl::running() const
    {
      return false;
    }

    /// returns true if the cloud server successfully begins to stop all nodes
    /// returns false if not running
    /// non-blocking call, clears errors and warnings
    bool VagrantProvider_Impl::terminate()
    {
      clearErrorsAndWarnings();
      return false;
    }

    bool VagrantProvider_Impl::terminated() const
    {
      return false;
    }

    std::vector<std::string> VagrantProvider_Impl::errors() const
    {
      return m_errors;
    }
    
    std::vector<std::string> VagrantProvider_Impl::warnings() const
    {
      return m_warnings;
    }

    void VagrantProvider_Impl::clearErrorsAndWarnings() const
    {
      m_errors.clear();
      m_warnings.clear();
    }

  }// detail

  VagrantProvider::VagrantProvider(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                                   const openstudio::path& workerPath, const openstudio::Url& workerUrl)
    : CloudProvider(boost::shared_ptr<detail::VagrantProvider_Impl>(new detail::VagrantProvider_Impl(serverPath, serverUrl, workerPath, workerUrl)))
  {
  }

  VagrantProvider::~VagrantProvider()
  {
  }

} // openstudio
