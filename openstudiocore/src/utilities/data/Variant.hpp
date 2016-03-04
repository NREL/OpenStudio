/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef UTILITIES_DATA_VARIANT_HPP
#define UTILITIES_DATA_VARIANT_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"
#include "../core/Optional.hpp"

#include <boost/variant.hpp>

namespace openstudio {
  
/** \class VariantType
 *  \brief A listing of data types that can be held in a Variant. 
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro 
 *  call is: 
 *  \code
OPENSTUDIO_ENUM( VariantType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Integer)(Integer)(2))
  ((String)(String)(3))
);
 *  \endcode */
OPENSTUDIO_ENUM( VariantType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Integer)(Integer)(2))
  ((String)(String)(3))
);

/** A Variant is a flexible type that can hold several types of values.
 */
class UTILITIES_API Variant {
 public:

  /// constructors
  explicit Variant(bool value);
  explicit Variant(double value);
  explicit Variant(int value);
  explicit Variant(const char* value);
  explicit Variant(const std::string& value);

  /// get the variant value type
  VariantType variantType() const;

  /// get value as a bool
  bool valueAsBoolean() const;

  /// get value as int
  int valueAsInteger() const;

  /// get value as double
  double valueAsDouble() const;

  /// get value as string
  std::string valueAsString() const;

 private:

  REGISTER_LOGGER("openstudio.Variant");
  
  VariantType m_type;
  boost::variant< bool, int, double, std::string > m_value;

};

/** \relates Variant */
typedef boost::optional<Variant> OptionalVariant;

/** \relates Variant */
typedef std::vector<Variant> VariantVector;

} // openstudio

#endif // UTILITIES_DATA_VARIANT_HPP
