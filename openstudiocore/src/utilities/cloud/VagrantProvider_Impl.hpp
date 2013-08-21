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

#include <utilities/cloud/CloudProvider_Impl.hpp>

namespace openstudio{
namespace detail{

  /// VagrantProvider is a CloudProvider that provides access to local Vagrant virtual machines for testing.
  class UTILITIES_API VagrantProvider_Impl : public CloudProvider_Impl {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor
    VagrantProvider_Impl();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~VagrantProvider_Impl();

    //@}
    /** @name Inherited members */
    //@{

    //@}
    /** @name Class members */
    //@{

    //@}
  private:

    // configure logging
    REGISTER_LOGGER("utilities.cloud.VagrantProvider");

  };

} // detail
} // openstudio

#endif // UTILITIES_CLOUD_VAGRANTPROVIDER_IMPL_HPP
