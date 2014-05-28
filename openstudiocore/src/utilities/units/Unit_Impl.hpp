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

#ifndef UTILITIES_UNITS_UNIT_IMPL_HPP
#define UTILITIES_UNITS_UNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "Unit.hpp"
#include "Scale.hpp"

#include <boost/smart_ptr.hpp>

namespace openstudio {
namespace detail {

  /** Unit_Impl is the implementation class for Unit.*/
  class UTILITIES_API Unit_Impl : public std::enable_shared_from_this<Unit_Impl> {
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
    Unit_Impl(int scaleExponent = 0,const std::string& prettyString="");

    /** Constructor using scale abbreviations registered in ScaleFactory. For instance, pass
     *  in "k" to access kilo = 10^3 (using the default ScaleFactory initialization). Throws if
     *  there is no entry for scaleAbbreviation in ScaleFactory. */
    Unit_Impl(const std::string& scaleAbbreviation,const std::string& prettyString="");

    virtual ~Unit_Impl() {}

    /** Deep copy constructor. */
    virtual Unit clone() const;

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
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent);

    /// Scale getter.
    const Scale& scale() const;

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
    virtual bool operator==(const Unit& rUnit) const;

    /** Multiply-assign operator. Adds exponents on baseUnits and scales. Returned scale exponent
     *  may differ from expectation based on initialization of openstudio::ScaleFactory, see
     *  Scale operators declared in ScaleFactory.hpp. */
    virtual void operator*=(const Unit& rUnit);

    /** Divide-assign operator. Subtracts exponents on rUnit's baseUnits and scales from lUnits's.
     *  Returned scale exponent may differ from expectation based on initialization of
     *  openstudio::ScaleFactory, see Scale operators declared in ScaleFactory.hpp. */
    virtual void operator/=(const Unit& rUnit);

    /** Raise Unit to a rational power. Throws openstudio::Exception if expDenom is not a common divisor
     *  for all baseUnit and scale exponents. Returned scale exponent may differ from expectation
     *  based on initialization of openstudio::ScaleFactory, see Scale operators declared in
     *  ScaleFactory.hpp.  */
    void pow(int expNum,int expDenom=1,bool okToCallFactory=true);

    //@}
    /** @name Type Casting */
    //@{

    /** Get a public object that wraps this impl.*/
    template<typename T>
    T getPublicObject() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<Unit_Impl>(shared_from_this())));
      return result;
    }

    //@}
   protected:
    typedef std::pair<std::string,int> UnitElement;
    std::vector<UnitElement> m_units;
    ScaleConstant m_scale;
    std::string m_prettyString;
    UnitSystem m_system;

    // constructor for derived classes
    Unit_Impl(int scaleExponent,
              const std::string& prettyString,
              int numBaseUnits,
              UnitSystem system);

    // constructor for derived classes
    Unit_Impl(const std::string& scaleAbbreviation,
              const std::string& prettyString,
              int numBaseUnits,
              UnitSystem system);

    std::vector<UnitElement>::iterator findBaseUnit(const std::string&);

    std::vector<UnitElement>::const_iterator findBaseUnit(const std::string&) const;

   private:
    REGISTER_LOGGER("openstudio.units.Unit");
  };

} // detail

} // openstudio

#endif // UTILITIES_UNITS_UNIT_IMPL_HPP

