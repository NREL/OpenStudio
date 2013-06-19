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

#ifndef RULESET_STANDARDSFILTEROBJECTENUMATTRIBUTE_HPP
#define RULESET_STANDARDSFILTEROBJECTENUMATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

#include <standardsinterface/EnumAttribute.hxx>

namespace openstudio {
namespace ruleset {

class RulesetStringPredicate;

namespace detail {
  class StandardsFilterObjectEnumAttribute_Impl;
} // detail

/** A StandardsFilterObjectEnumAttribute is a StandardsFilterObjectAttribute that matches when
 *  object() has an EnumAttribute attribute() that satisfies the predicate/value combination
 *  of this filter. */
class RULESET_API StandardsFilterObjectEnumAttribute : public StandardsFilterObjectAttribute {
 public:
  /// @cond
  typedef detail::StandardsFilterObjectEnumAttribute_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  /** Currently predicate is only used to indicate equals/not equals, but comparison is done 
   *  by an OPENSTUDIO_ENUM, which is even more flexible than an IEquals. */
  StandardsFilterObjectEnumAttribute(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::EnumAttribute& attribute,
                                     const RulesetStringPredicate& predicate,
                                     const std::string value);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterObjectEnumAttribute(const StandardsFilterAvailableObject& object,
                                     const standardsinterface::EnumAttribute& attribute,
                                     const RulesetStringPredicate& predicate,
                                     const std::string value,
                                     const UUID& uuid,
                                     const UUID& versionUUID);

  //@}
  /** @name Getters */
  //@{

  standardsinterface::EnumAttribute attribute() const;

  RulesetStringPredicate predicate() const;

  std::string value() const;

  //@}
  /** @name Setters */
  //@{

  void setPredicate(const RulesetStringPredicate& predicate);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterObjectEnumAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterObjectEnumAttribute(boost::shared_ptr<detail::StandardsFilterObjectEnumAttribute_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterObjectEnumAttribute");
};

/** \relates StandardsFilterObjectEnumAttribute */
typedef boost::optional<StandardsFilterObjectEnumAttribute> OptionalStandardsFilterObjectEnumAttribute;

/** \relates StandardsFilterObjectEnumAttribute */
typedef std::vector<StandardsFilterObjectEnumAttribute> StandardsFilterObjectEnumAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTENUMATTRIBUTE_HPP
