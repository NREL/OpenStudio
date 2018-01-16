/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_UNITS_QUANTITY_HPP
#define UTILITIES_UNITS_QUANTITY_HPP

#include "../UtilitiesAPI.hpp"

#include "Unit.hpp"

#include <QMetaType>

namespace openstudio {

/** Quantity class defines value, unit pairs. Units are set at construction and are modified by
 *  arithmetic operations. The constructors and assignment operator ensure that this class
 *  behaves as "plain old data" (POD) by cloning any units information (to avoid multiple objects
 *  pointing to the exact same unit data). */
class UTILITIES_API Quantity {
 public:
  /** @name Constructors */
  //@{

  explicit Quantity(const UnitSystem& system=UnitSystem::Mixed);

  explicit Quantity(double value, const UnitSystem& system=UnitSystem::Mixed);

  Quantity(double value, const Unit& units);

  Quantity(const Quantity& q);

  Quantity& operator=(const Quantity& q);

  virtual ~Quantity() {};

  //@}
  /** @name Value */
  //@{

  double value() const;

  /** Value setter. */
  void setValue(double newValue);

  //@}
  /** @name Units */
  //@{

  /** Returns deep copy (clone) of this Quantity's units. */
  Unit units() const;

  UnitSystem system() const;

  /** Returns true if units().optionalCast<TemperatureUnit>(). */
  bool isTemperature() const;

  /** Vector of base units available to the Quantity. */
  std::vector<std::string> baseUnits() const;

  /** Base unit exponent getter. Returns 0 if baseUnit not present. */
  int baseUnitExponent(const std::string& baseUnit) const;

  /** Sets the exponent of a base unit in this Quantity's m_units. This function will throw
   *  if baseUnit is not a valid option for this type of unit. A generic Unit allows any base
   *  unit to be set. */
  void setBaseUnitExponent(const std::string& baseUnit, int exponent);

  /** Units in string form. LaTeX mathematical formatting--_ for subscripting, ^ for
   *  superscripting. Curly braces {} are used for grouping. \sa Unit::standardUnitsString */
  std::string standardUnitsString(bool withScale=true) const;

  /** Pretty string for this quantity's units. For instance N = kg*m/s^2.
   *  \sa Unit::prettyUnitsString */
  std::string prettyUnitsString(bool withScale=true) const;

  /** Sets the prettystring for this Quantity's m_units. openstudio::Unit attempts to keep up
   *  with prettyStrings, but makes no promises. Precondition: isCompoundUnit(str). Otherwise
   *  throws. */
  void setPrettyUnitsString(const std::string& str);

  /** Scale getter. */
  Scale scale() const;

  /// Sets the scale to the one registered in ScaleFactory for 10^scaleExponent.
  bool setScale(int scaleExponent);

  /// Sets the scale to the one registered in ScaleFactory under scaleAbbreviation.
  bool setScale(const std::string& scaleAbbreviation);

  //@}
  /** @name Temperature Methods
   *
   *  Quantities that have units of temperature need several extra functions to distinguish
   *  between absolute and relative measures. */
  //@{

  /** Throws if !units().optionalCast<TemperatureUnit>(). */
  bool isAbsolute() const;

  /** Throws if !units().optionalCast<TemperatureUnit>(). */
  bool isRelative() const;

  /** Set temperature as absolute. Throws if !units().optionalCast<TemperatureUnit>(). */
  void setAsAbsolute();

  /** Set temperature as relative. Throws if !units().optionalCast<TemperatureUnit>(). */
  void setAsRelative();

  //@}
  /** @name IP Methods */
  //@{

  /** Conversions for pounds mass to pounds force. These functions will throw if
   *  !units().optionalCast<IPUnit>(). */
  void lbmToLbf();

  /** Conversions for pounds mass to pounds force. These functions will throw if
   *  !units().optionalCast<IPUnit>(). */
  void lbfToLbm();

  //@}
  /** @name Mathematical Operators */
  //@{

  /** Add and assign operator. Throws if units are not equal. */
  Quantity& operator+=(const Quantity& rQuantity);

  /** Subtract and assign operator. Throws if units are not equal. */
  Quantity& operator-=(const Quantity& rQuantity);

  /** Multiply and assign operator. Throws if Quantity is a particular system and rQuantity is not
   *  the same system. */
  Quantity& operator*=(const Quantity& rQuantity);

  /** Divide and assign operator. Throws if Quantity is a particular system and rQuantity is not
   *  the same system. */
  Quantity& operator/=(const Quantity& rQuantity);

  /** Multiply by double. No impact to units or scale. Does not throw. */
  Quantity& operator*=(double d);

  /** Divide by double. No impact to units or scale. Does not throw. */
  Quantity& operator/=(double d);

  /** Raise Quantity to a rational power. Throws openstudio::Exception if expDenom is not a common divisor
   *  for all baseUnit and scale exponents. Returned scale exponent may differ from expectation
   *  based on initialization of openstudio::ScaleFactory, see Scale operators declared in
   *  ScaleFactory.hpp.  */
  Quantity& pow(int expNum,int expDenom=1);

  //@}

  friend UTILITIES_API std::ostream& operator<<(std::ostream& os,const Quantity& q);

 protected:
  double m_value;
  Unit m_units;

 private:
  REGISTER_LOGGER("openstudio.units.Quantity");
};

/** \relates Quantity */
UTILITIES_API std::ostream& operator<<(std::ostream& os,const Quantity& q);

/** \relates Quantity */
typedef boost::optional<Quantity> OptionalQuantity;

/** \relates Quantity */
typedef std::vector<Quantity> QuantityVector;

/** Negate a Quantity. */
UTILITIES_API Quantity operator-(const Quantity& rQuantity);
/** Add Quantities. Used with base class Quantities and mixed Quantity types. */
UTILITIES_API Quantity operator+(const Quantity& lQuantity,const Quantity& rQuantity);
/** Subtract Quantities. Used with base class Quantities and mixed Quantity types. */
UTILITIES_API Quantity operator-(const Quantity& lQuantity,const Quantity& rQuantity);
/** Multiply Quantities. Used with base class Quantities and mixed Quantity types. */
UTILITIES_API Quantity operator*(const Quantity& lQuantity,const Quantity& rQuantity);
/** Divide Quantities. Used with base class Quantities and mixed Quantity types. */
UTILITIES_API Quantity operator/(const Quantity& lQuantity,const Quantity& rQuantity);

/** Raise rQuantity to a rational power. Throws openstudio::Exception if the scale exponent or
 *  any base unit exponent is not cleanly divisible by expDenom. */
UTILITIES_API Quantity pow(const Quantity& rQuantity,int expNum,int expDenom=1);

/** Multiply a Quantity by a double. */
UTILITIES_API Quantity operator*(const Quantity& lQuantity,double d);
/** Multiply a Quantity by a double. */
UTILITIES_API Quantity operator*(double d,const Quantity& rQuantity);
/** Divide a Quantity by a double. */
UTILITIES_API Quantity operator/(const Quantity& lQuantity,double d);
/** Divide a double by a Quantity. */
UTILITIES_API Quantity operator/(double d,const Quantity& rQuantity);

UTILITIES_API bool operator==(const Quantity& lQuantity,const Quantity& rQuantity);

UTILITIES_API bool operator!=(const Quantity& lQuantity,const Quantity& rQuantity);

} // openstudio

// declare this type so we can use it in properties
Q_DECLARE_METATYPE(openstudio::Quantity);

#endif // UTILITIES_UNITS_QUANTITY_HPP

