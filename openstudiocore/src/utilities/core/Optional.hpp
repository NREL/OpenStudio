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

#ifndef UTILITIES_CORE_OPTIONAL_HPP
#define UTILITIES_CORE_OPTIONAL_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/optional.hpp>

#include <QVariant>
#include <QMetaType>
#include <QUrl>

#include <string>
#include <ostream>

namespace openstudio {
  // note: boost::optional<bool> is a bad idea, don't do it
  typedef boost::optional<double> OptionalDouble;
  typedef boost::optional<unsigned> OptionalUnsigned;
  typedef boost::optional<int> OptionalInt;
  typedef boost::optional<std::string> OptionalString;

  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<double>& x);
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<unsigned>& x);
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<int>& x);
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const boost::optional<std::string>& x);
}

// declare these types so we can use them as properties
Q_DECLARE_METATYPE(boost::optional<double>);
Q_DECLARE_METATYPE(boost::optional<unsigned>);
Q_DECLARE_METATYPE(boost::optional<int>);
Q_DECLARE_METATYPE(boost::optional<std::string>);

namespace openstudio {
namespace detail {

  // register meta datatypes
  struct OptionalMetaTypeInitializer{
    OptionalMetaTypeInitializer();
  };

}
}

#endif // UTILITIES_CORE_OPTIONAL_HPP
