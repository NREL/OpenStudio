/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_UNITS_UNIT_HPP
#define UTILITIES_UNITS_UNIT_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Enum.hpp"

#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>

#include <vector>
#include <string>
#include <iostream>

namespace openstudio {

struct Scale;

namespace detail {

  class Unit_Impl;

} // detail

/** \class UnitSystem
 *  \brief Unit systems supported by OpenStudio's runtime units utility.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp.
 *  The actual macro call is:
 *  \code
OPENSTUDIO_ENUM(UnitSystem,
  ((Mixed))
  ((SI))
  ((IP))
  ((BTU))
  ((CFM))
  ((GPD))
  ((MPH))
  ((Wh))
  ((Therm))
  ((Misc1))
  ((Celsius))
  ((Fahrenheit))
);
 *  \endcode
 *  \relates Unit */
OPENSTUDIO_ENUM(UnitSystem,
  ((Mixed))
  ((SI))
  ((IP))
  ((BTU))
  ((CFM))
  ((GPD))
  ((MPH))
  ((Wh))
  ((Therm))
  ((Misc1))
  ((Celsius))
  ((Fahrenheit))
);

/** Base class that holds unit information in the form of (string,exponent) pairs. The
 *  strings are called baseUnits and are typically the standard abbreviations such as
 *  "m", "ft", "kg", etc. Unit currently only supports integer exponents.
 *
 *  Related functions and operators are defined in Unit.hpp. */
class UTILITIES_API Unit {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Can set non-zero exponent (base 10) for the scale, and prettyString
   *  (alias for standardString, as in J = kg*m^2/s^2). The actual scale stored with Unit depends
   *  on what scales are registered in ScaleFactory. Throws if there is no entry for
   *  scaleExponent in ScaleFactory. Example: \verbatim
      openstudio::Unit u(-2,"J");
      u.setBaseUnitExponent("kg",1);
      u.setBaseUnitExponent("m",2);
      u.setBaseUnitExponent("s",-2);
      std::cout << u; // would print cJ, using default openstudio::ScaleFactory initialization \endverbatim
   *  Note that this is not the preferred method for constructing Joules. See openstudio::SIUnit,
   *  and openstudio::UnitFactory for alternatives. */
  Unit(int scaleExponent = 0,const std::string& prettyString="");

  /** Constructor using scale abbreviations registered in ScaleFactory. For instance, pass
   *  in "k" to access kilo = 10^3 (using the default ScaleFactory initialization). Throws if
   *  there is no entry for scaleAbbreviation in ScaleFactory. */
  Unit(const std::string& scaleAbbreviation,const std::string& prettyString="");

  virtual ~Unit() {}

  /** Deep copy constructor. */
  Unit clone() const;

  /** Deep copy constructor that discards system designation. Postcondition: system() == UnitSystem::Mixed. */
  Unit cloneToMixed() const;

  //@}
  /** @name Base Units and Scale */
  //@{

  /// Returns base units already available in Unit.
  std::vector<std::string> baseUnits() const;

  /// Returns true if baseUnit is available in Unit.
  bool isBaseUnit(const std::string& baseUnit) const;

  /// Base unit exponent getter. Returns 0 if baseUnit not present. */
  int baseUnitExponent(const std::string& baseUnit) const;

  /// Sets baseUnit^exponent. If baseUnit not yet present, is added and return value is true.
  void setBaseUnitExponent(const std::string& baseUnit,int exponent);

  /// Scale getter.
  Scale scale() const;

  /** Sets the scale to the one registered in ScaleFactory for 10^scaleExponent. Returns true
   *  if successful, false if no Scale with exponent == scaleExponent exists in ScaleFactory.
   *  In that case, Unit remains unchanged. */
  bool setScale(int scaleExponent);

  /** Sets the scale to the one registered in ScaleFactory under scaleAbbreviation. Returns true
   *  if successful, false if no Scale with abbr == scaleAbbreviation exists in ScaleFactory.
   *  In that case, Unit remains unchanged. */
  bool setScale(const std::string& scaleAbbreviation);

  //@}
  /** @name Serialization */
  //@{

  /** Returns the standard output string for this unit. Version withScale==false is used as a
   *  signature elsewhere in the utility. LaTeX formatting is used. Examples of valid strings:
   *
   *  <ul>
   *    <li>kg*m/s^2</li>
   *    <li>Btu/h</li>
   *  </ul>
   *
   *  Units with positive exponents come first, then '/', then units with negative exponents.
   *  ^ indicates a non-unity exponent.
   *
   *  If withScale==true (the default since this is probably preferred by users), scale
   *  abbreviation is prefixed. Prefers km^2 to M(m^2), kBtu/ft^2 to k(Btu/ft^2).
   */
  std::string standardString(bool withScale=true) const;

  /** Returns the preferred output string for this unit. For instance, the prettyString
   *  for SI energy is 'J' for Joules, but the standard string is 'kg*m^2/s^2'.
   *
   *  If withScale==true (the default since this is probably preferred by users), scale
   *  abbreviation is prefixed. Prefers km^2 to M(m^2), kBtu/ft^2 to k(Btu/ft^2). */
  std::string prettyString(bool withScale=true) const;

  /** Setter for prettyString (do not include scale abbreviation). openstudio::Unit attempts
   *  to keep up with prettyStrings, but makes no promises. Precondition: isCompoundUnit(str).
   *  Otherwise throws. */
  void setPrettyString(const std::string& str);

  /** Returns prettyString(withScale) if it exists; otherwise returns standardString(withScale). */
  std::string print(bool withScale=true) const;

  //@}
  /** @name Unit System */
  //@{

  /** Getter for this Unit's system */
  UnitSystem system() const;

  //@}
  /** @name Mathematical Operators */
  //@{

  /** Equality for units. Does not require identical systems, scales, or prettyStrings. */
  bool operator==(const Unit& rUnit) const;

  /** Multiply-assign operator. Adds exponents on baseUnits and scales. Returned scale exponent
   *  may differ from expectation based on initialization of openstudio::ScaleFactory, see
   *  Scale operators declared in ScaleFactory.hpp. */
  Unit& operator*=(const Unit& rUnit);

  /** Divide-assign operator. Subtracts exponents on rUnit's baseUnits and scales from lUnits's.
   *  Returned scale exponent may differ from expectation based on initialization of
   *  openstudio::ScaleFactory, see Scale operators declared in ScaleFactory.hpp. */
  Unit& operator/=(const Unit& rUnit);

  /** Raise Unit to a rational power. Throws openstudio::Exception if expDenom is not a common divisor
   *  for all baseUnit and scale exponents. Returned scale exponent may differ from expectation
   *  based on initialization of openstudio::ScaleFactory, see Scale operators declared in
   *  ScaleFactory.hpp.  */
  Unit& pow(int expNum,int expDenom=1,bool okToCallFactory=true);

  //@}
  /** @name Type Casting */
  //@{

  /** Get the impl pointer */
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(std::move(impl));
  }

  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false)
   *  if object is not a T. */
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(std::move(impl));
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::Unit_Impl ImplType;

  explicit Unit(std::shared_ptr<detail::Unit_Impl> impl);

  friend class detail::Unit_Impl;

  /// @endcond
 private:

  std::shared_ptr<detail::Unit_Impl> m_impl;

  REGISTER_LOGGER("openstudio.units.Unit");
};

/** \relates Unit*/
typedef boost::optional<Unit> OptionalUnit;

/** \relates Unit*/
typedef std::vector<Unit> UnitVector;

/** Print u to a stream. Scale abbreviation, if any, is printed before prettyString (if
 *  non-empty) or standardString. */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Unit& u);

/** Parses unit string into a Unit object. Precondition: isUnit == true. Otherwise, throws.
 *  Be forewarned--this does dumb parsing. Doesn't work to extract scales from expressions
 *  like "kN", doesn't match base units up with unit systems.
 *
 *  Postcondition: prettyString() == "".
 *  \sa UnitFactory */
UTILITIES_API Unit parseUnitString(const std::string& unitString);

/** Inequality for units. Does not compare scales or prettyStrings. */
UTILITIES_API bool operator!=(const Unit& lUnit,const Unit& rUnit);

/** Unit multiplication. Adds exponents on baseUnits and scales. Returned scale exponent
 *  may differ from expectation based on initialization of openstudio::ScaleFactory, see
 *  Scale operators declared in ScaleFactory.hpp. */
UTILITIES_API Unit operator*(const Unit& lUnit,const Unit& rUnit);

/** Unit division. Subtracts exponents on rUnit's baseUnits and scales from lUnits's.
 *  Returned scale exponent may differ from expectation based on initialization of
 *  openstudio::ScaleFactory, see Scale operators declared in ScaleFactory.hpp.*/
UTILITIES_API Unit operator/(const Unit& lUnit,const Unit& rUnit);

/** Calculate rUnit^(expNum/expDenom). Throws openstudio::Exception if expDenom is not a common divisor
 *  for all baseUnit and scale exponents. Returned scale exponent may differ from expectation
 *  based on initialization of openstudio::ScaleFactory, see Scale operators declared in
 *  ScaleFactory.hpp. */
UTILITIES_API Unit pow(const Unit& rUnit,int expNum,int expDenom=1);

} // openstudio

#endif // UTILITIES_UNITS_UNIT_HPP

