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

#ifndef UTILITIES_UNITS_OSQUANTITYVECTOR_HPP
#define UTILITIES_UNITS_OSQUANTITYVECTOR_HPP

#include "../UtilitiesAPI.hpp"

#include "Unit.hpp"

#include <vector>

namespace openstudio {

class Quantity;

/** Represents a vector of \link Quantity quantities \endlink all with the same units.
 *  Provides performance improvements over directly operating on std::vector<Quantity>. 
 *  The constructors and assignment operator ensure that this class behaves as "plain 
 *  old data" (POD) by cloning any units information (to avoid multiple objects 
 *  pointing to the exact same unit data). */
class UTILITIES_API OSQuantityVector {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit OSQuantityVector(const Unit& units=Unit());

  OSQuantityVector(const Unit& units, unsigned n, double value=0.0);

  OSQuantityVector(const Unit& units, const std::vector<double>& values);

  explicit OSQuantityVector(const std::vector<Quantity>& values);

  OSQuantityVector(const OSQuantityVector& other);

  OSQuantityVector& operator=(const OSQuantityVector& other);

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns a clone of this object's units. */
  Unit units() const;
  
  UnitSystem system() const;

  const Scale& scale() const;

  /** Creates a vector of separate quantities. Note that it is better to do mathematical 
   *  operations and convert units using OSOptionalQuantity. Only use this method once the 
   *  values and units of this OSOptionalQuantity are fixed. */
  std::vector<Quantity> quantities() const;

  /** Returns the underlying values of this OSQuantityVector. */
  std::vector<double> values() const;

  Quantity getQuantity(unsigned i) const;

  /** Return value is equivalent to values().empty(). */ 
  bool empty() const;

  /** Return value is equivalent to values().size(). */
  unsigned size() const;

  //@}
  /** @name Setters */
  //@{

  /// Sets the scale to the one registered in ScaleFactory for 10^scaleExponent.
  bool setScale(int scaleExponent);

  /// Sets the scale to the one registered in ScaleFactory under scaleAbbreviation.
  bool setScale(const std::string& scaleAbbreviation);

  /** Pushes q onto this vector if possible, otherwise throws. */
  void push_back(Quantity q);

  /** Removes the last value in this OSQuantityVector. */
  void pop_back();

  /** Changes the size of this OSQuantityVector to n, using value to fill in any newly 
   *  created elements as necessary. \post{size() == n} */
  void resize(unsigned n, double value=0.0);

  /** Empties all values out of this OSQuantityVector. \post{size() == 0u} \post{empty()} */
  void clear();

  //@}
  /** @name Temperature Methods
   *
   *  Quantities that have units of temperature need several extra functions to distinguish
   *  between absolute and relative measures. */
  //@{

  /** Returns true if units().optionalCast<TemperatureUnit>(). */
  bool isTemperature() const;

  /** Throws if !units().optionalCast<TemperatureUnit>(). */
  bool isAbsolute() const;

  /** Throws if !units().optionalCast<TemperatureUnit>(). */
  bool isRelative() const;

  /** Set temperature as absolute. Throws if !units().optionalCast<TemperatureUnit>(). 
   *  \post{isAbsolute()} */
  void setAsAbsolute();

  /** Set temperature as relative. Throws if !units().optionalCast<TemperatureUnit>(). 
   *  \post{isRelative()} */
  void setAsRelative();

  //@}
  /** @name IP Methods */
  //@{

  /** Conversions for pounds mass to pounds force. These functions will throw if
   *  !units().optionalCast<IPUnit>(). \post{units().baseUnitExponent("lbm") == 0} */
  void lbmToLbf();

  /** Conversions for pounds mass to pounds force. These functions will throw if
   *  !units().optionalCast<IPUnit>(). \post{units().baseUnitExponent("lbf") == 0} */
  void lbfToLbm();

  //@}
  /** @name Mathematical Operators */
  //@{

  /** Adds rVector to this vector, if possible. Throws if vectors are of different size or units
   *  are different. */
  OSQuantityVector& operator+=(OSQuantityVector rVector);

  /** Adds rQuantity to this vector element-wise, if possible. Throws if units are different. */
  OSQuantityVector& operator+=(Quantity rQuantity);

  /** Subtracts rVector from this vector, if possible. Throws if vectors are of different size or
   *  units are different. */
  OSQuantityVector& operator-=(OSQuantityVector rVector);

  /** Subtracts rQuantity from this vector element-wise, if possible. Throws if units are
   *  different. */
  OSQuantityVector& operator-=(Quantity rQuantity);

  /** Multiplies all values of this vector by rQuantity. */
  OSQuantityVector& operator*=(const Quantity& rQuantity);

  /** Divides all values of this vector by rQuantity. */
  OSQuantityVector& operator/=(const Quantity& rQuantity);

  /** Multiplies all values of this vector by d. */
  OSQuantityVector& operator*=(double d);

  /** Divides all values of this vector by d. */
  OSQuantityVector& operator/=(double d);

  //@}
 private:
  Unit m_units;
  std::vector<double> m_values;

  REGISTER_LOGGER("openstudio.units.OSQuantityVector")
};

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator+(const OSQuantityVector& lVector,
                                         const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator+(const OSQuantityVector& lVector,
                                         const Quantity& rQuantity);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator+(const Quantity& lQuantity,
                                         const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const OSQuantityVector& lVector,
                                         const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const OSQuantityVector& lVector,
                                         const Quantity& rQuantity);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const Quantity& lQuantity,
                                         const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator*(const OSQuantityVector& lVector,
                                         const Quantity& rQuantity);
                                         
/** \relates OSQuantityVector */                                         
UTILITIES_API OSQuantityVector operator*(const OSQuantityVector& lVector, double d);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator*(const Quantity& lQuantity,
                                         const OSQuantityVector& rVector);
                                         
/** \relates OSQuantityVector */                                         
UTILITIES_API OSQuantityVector operator*(double d,
                                         const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator/(const OSQuantityVector& lVector,
                                         const Quantity& rQuantity);
                                         
/** \relates OSQuantityVector */                                         
UTILITIES_API OSQuantityVector operator/(const OSQuantityVector& lVector, double d);

/** \relates OSQuantityVector */
UTILITIES_API Quantity dot(OSQuantityVector lVector, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API bool operator==(const OSQuantityVector& lVector,const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API bool operator!=(const OSQuantityVector& lVector,const OSQuantityVector& rVector);

} // openstudio

#endif // UTILITIES_UNITS_OSQUANTITYVECTOR_HPP

