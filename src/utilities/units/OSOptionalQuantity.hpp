/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_OSOPTIONALQUANTITY_HPP
#define UTILITIES_UNITS_OSOPTIONALQUANTITY_HPP

#include "../UtilitiesAPI.hpp"

#include "Unit.hpp"

#include <boost/optional.hpp>

namespace openstudio {

class Quantity;

/** OSOptionalQuantity class to be used in OpenStudio ModelObject properties. Always contains a
 *  Unit. Optionally contains a value. The constructors and assignment operator ensure that this
 *  class behaves as "plain old data" (POD) by cloning any units information (to avoid multiple
 *  objects pointing to the exact same unit data). */
class UTILITIES_API OSOptionalQuantity
{
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit OSOptionalQuantity(const UnitSystem& sys = UnitSystem::Mixed);

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

}  // namespace openstudio

#endif  // UTILITIES_UNITS_OSOPTIONALQUANTITY_HPP
