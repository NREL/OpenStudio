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

#ifndef RULESET_STANDARDSFILTEROBJECTINTEGERATTRIBUTE_HPP
#define RULESET_STANDARDSFILTEROBJECTINTEGERATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectNumericalAttribute.hpp>

#include <standardsinterface/IntegerAttribute.hxx>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsFilterObjectIntegerAttribute_Impl;
} // detail

/** A StandardsFilterObjectIntegerAttribute is a StandardsFilterObjectNumericalAttribute with an
 *  integer value(). The filter matches any object() for which attribute() is available and 
 *  satisfies the predicate/value/unitString combination. At this time, the unitString is only 
 *  present for display purposes, it is not yet used in any calculation. Almost always, unitString
 *  should be in SI. */
class RULESET_API StandardsFilterObjectIntegerAttribute : public StandardsFilterObjectNumericalAttribute {
 public:
  /// @cond
  typedef detail::StandardsFilterObjectIntegerAttribute_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  StandardsFilterObjectIntegerAttribute(const StandardsFilterAvailableObject& object,
                                        const standardsinterface::IntegerAttribute& attribute,
                                        const RulesetNumericalPredicate& predicate,
                                        int value,
                                        const std::string& unitString);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterObjectIntegerAttribute(const StandardsFilterAvailableObject& object,
                                        const standardsinterface::IntegerAttribute& attribute,
                                        const RulesetNumericalPredicate& predicate,
                                        int value,
                                        const std::string& unitString,
                                        const UUID& uuid,
                                        const UUID& versionUUID);

  //@}
  /** @name Getters */
  //@{

  standardsinterface::IntegerAttribute attribute() const;

  int value() const;

  //@}
  /** @name Setters */
  //@{

  void setAttribute(const standardsinterface::IntegerAttribute& attribute);

  bool setValue(int value);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterObjectIntegerAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterObjectIntegerAttribute(boost::shared_ptr<detail::StandardsFilterObjectIntegerAttribute_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterObjectIntegerAttribute");
};

/** \relates StandardsFilterObjectIntegerAttribute */
typedef boost::optional<StandardsFilterObjectIntegerAttribute> OptionalStandardsFilterObjectIntegerAttribute;

/** \relates StandardsFilterObjectIntegerAttribute */
typedef std::vector<StandardsFilterObjectIntegerAttribute> StandardsFilterObjectIntegerAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTINTEGERATTRIBUTE_HPP
