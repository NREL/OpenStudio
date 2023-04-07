/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
class UTILITIES_API OSQuantityVector
{
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit OSQuantityVector(const Unit& units = Unit());

  OSQuantityVector(const Unit& units, unsigned n, double value = 0.0);

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

  Scale scale() const;

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
  void resize(unsigned n, double value = 0.0);

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
UTILITIES_API OSQuantityVector operator+(const OSQuantityVector& lVector, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator+(const OSQuantityVector& lVector, const Quantity& rQuantity);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator+(const Quantity& lQuantity, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const OSQuantityVector& lVector, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const OSQuantityVector& lVector, const Quantity& rQuantity);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator-(const Quantity& lQuantity, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator*(const OSQuantityVector& lVector, const Quantity& rQuantity);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator*(const OSQuantityVector& lVector, double d);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator*(const Quantity& lQuantity, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator*(double d, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator/(const OSQuantityVector& lVector, const Quantity& rQuantity);

/** \relates OSQuantityVector */
UTILITIES_API OSQuantityVector operator/(const OSQuantityVector& lVector, double d);

/** \relates OSQuantityVector */
UTILITIES_API Quantity dot(OSQuantityVector lVector, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API bool operator==(const OSQuantityVector& lVector, const OSQuantityVector& rVector);

/** \relates OSQuantityVector */
UTILITIES_API bool operator!=(const OSQuantityVector& lVector, const OSQuantityVector& rVector);

}  // namespace openstudio

#endif  // UTILITIES_UNITS_OSQUANTITYVECTOR_HPP
