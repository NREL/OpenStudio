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

#ifndef UTILITIES_IDD_IDDKEY_IMPL_HPP
#define UTILITIES_IDD_IDDKEY_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "IddKeyProperties.hpp"

#include "../core/Logger.hpp"

#include <string>
#include <ostream>

namespace openstudio {

// private namespace
namespace detail {

  /** Implementation class for IddKey. */
  class UTILITIES_API IddKey_Impl {
   public:

    /// default constructor for serialization
    IddKey_Impl();

    /// equality operator
    bool operator==(const IddKey_Impl& other) const;

    /// get name
    std::string name() const;

    /// get properties
    const IddKeyProperties& properties() const;

    /// load by parsing text
    static std::shared_ptr<IddKey_Impl> load(const std::string& name, const std::string& text);

    /// print idd 
    std::ostream& print(std::ostream& os) const;

   private:

    /// partial constructor used by load
    IddKey_Impl(const std::string& name);

    // parse 
    void parse(const std::string& text);

    // name
    std::string m_name;

    // properties
    IddKeyProperties m_properties;

    // configure logging
    REGISTER_LOGGER("utilities.idd.IddKey");
  };

} // detail

} // openstudio

#endif // UTILITIES_IDD_IDDKEY_IMPL_HPP
