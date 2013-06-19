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

#ifndef RULESET_STANDARDSFILTEROBJECTBOOLEANATTRIBUTE_HPP
#define RULESET_STANDARDSFILTEROBJECTBOOLEANATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>

#include <standardsinterface/BooleanAttribute.hxx>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsFilterObjectBooleanAttribute_Impl;
} // detail

/** A StandardsFilterBooleanAttribute is a StandardsFilterObjectAttribute that is matched by 
 *  an object() with a BooleanAttribute attribute() equal to value(). */
class RULESET_API StandardsFilterObjectBooleanAttribute : public StandardsFilterObjectAttribute {
 public:
  /// @cond
  typedef detail::StandardsFilterObjectBooleanAttribute_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  StandardsFilterObjectBooleanAttribute(const StandardsFilterAvailableObject& object,
                               const standardsinterface::BooleanAttribute& attribute,
                               bool value);

  /** Constructor provided for deserialization; not for general use. */
  StandardsFilterObjectBooleanAttribute(const StandardsFilterAvailableObject& object,
                               const standardsinterface::BooleanAttribute& attribute,
                               bool value,
                               const UUID& uuid,
                               const UUID& versionUUID);

  //@}
  /** @name Getters */
  //@{

  standardsinterface::BooleanAttribute attribute() const;

  bool value() const;

  //@}
  /** @name Setters */
  //@{

  void setAttribute(const standardsinterface::BooleanAttribute& attribute);

  bool setValue(bool value);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterObjectBooleanAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterObjectBooleanAttribute(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttribute_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterObjectBooleanAttribute");
};

/** \relates StandardsFilterObjectBooleanAttribute */
typedef boost::optional<StandardsFilterObjectBooleanAttribute> OptionalStandardsFilterObjectBooleanAttribute;

/** \relates StandardsFilterObjectBooleanAttribute */
typedef std::vector<StandardsFilterObjectBooleanAttribute> StandardsFilterObjectBooleanAttributeVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTBOOLEANATTRIBUTE_HPP
