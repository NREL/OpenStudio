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

  /// VagrantProvider is a CloudProvider that provides access to local Vagrant virtual machines for testing.
  class UTILITIES_API VagrantProvider : public CloudProvider {
  public:

    /** @name Constructor */
    //@{

    /// constructor
    VagrantProvider(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                    const openstudio::path& workerPath, const openstudio::Url& workerUrl,
                    bool haltOnStop = true);

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
  private:

    // no body on purpose, do not want this generated
    VagrantProvider(const VagrantProvider& other);

  };

} // openstudio

#endif // UTILITIES_CLOUD_VAGRANTPROVIDER_HPP
