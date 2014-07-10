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

#ifndef UTILITIES_UNITS_OSOPTIONALQUANTITY_HPP
#define UTILITIES_UNITS_OSOPTIONALQUANTITY_HPP

#include "../UtilitiesAPI.hpp"

#include "Unit.hpp"

#include <QMetaType>

#include <boost/optional.hpp>

namespace openstudio {

class Quantity;

/** OSOptionalQuantity class to be used in OpenStudio ModelObject properties. Always contains a
 *  Unit. Optionally contains a value. The constructors and assignment operator ensure that this
 *  class behaves as "plain old data" (POD) by cloning any units information (to avoid multiple
 *  objects pointing to the exact same unit data). */
class UTILITIES_API OSOptionalQuantity {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit OSOptionalQuantity(const UnitSystem& sys=UnitSystem::Mixed);

  explicit OSOptionalQuantity(const Unit& units);

  OSOptionalQuantity(const Quantity& quantity);

  OSOptionalQuantity(const OSOptionalQuantity& oq);

  OSOptionalQuantity& operator=(const OSOptionalQuantity& oq);

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns true if there is no value. */
  bool empty() const;

  /** Returns true if there is a value. */
  bool isSet() const;

  /** Returns the corresponding Quantity. Throws if empty(). */
  Quantity get() const;

  /** Returns a clone of this object's units. */
  Unit units() const;

  //@}
 private:
  boost::optional<double> m_value;
  Unit m_units;
};

} // openstudio

// declare this type so we can use it in properties
Q_DECLARE_METATYPE(openstudio::OSOptionalQuantity);

#endif // UTILITIES_UNITS_OSOPTIONALQUANTITY_HPP

