/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_IDD_IDDKEY_HPP
#define UTILITIES_IDD_IDDKEY_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/optional.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace openstudio{

struct IddKeyProperties;

namespace detail{
  class IddKey_Impl;
}

/** IddKey represents an enumeration value for an IDD field of type choice. */
class UTILITIES_API IddKey {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor */
  IddKey();

  /** Copy constructor shares implementation. */
  IddKey(const IddKey& other);

  //@}
  /** @name Getters */
  //@{

  /** Returns the key name */
  std::string name() const;

  /** Returns the key properties. */
  const IddKeyProperties& properties() const;

  //@}
  /** @name Queries */
  //@{

  /** Returns true if all data is exactly equal. */
  bool operator==(const IddKey& other) const;
  
  /** Negation of operator==. */
  bool operator!=(const IddKey& other) const;

  //@}
  /** @name Sertialization */
  //@{

  /** Load from text. */
  static boost::optional<IddKey> load(const std::string& name, const std::string& text);

  /** Print to os in standard IDD format */
  std::ostream& print(std::ostream& os) const;

  //@}
 private:
  ///@cond
  // pointer to implementation
  boost::shared_ptr<detail::IddKey_Impl> m_impl;

  // construct from impl
  IddKey(const boost::shared_ptr<detail::IddKey_Impl>& impl);
  ///@endcond

  // configure logging
  REGISTER_LOGGER("openstudio.idd.IddKey");
};

/** \relates IddKey */
typedef std::vector<IddKey> IddKeyVector;

/** \relates IddKey */
typedef boost::optional<IddKey> OptionalIddKey;

/** \relates IddKey */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IddKey& iddKey);

} // openstudio

#endif // UTILITIES_IDD_IDDKEY_HPP
