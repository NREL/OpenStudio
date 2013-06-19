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

#ifndef RULESET_STANDARDSFILTEROBJECTATTRIBUTE_HPP
#define RULESET_STANDARDSFILTEROBJECTATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause.hpp>

#include <standardsinterface/ObjectType.hxx>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableObject;

namespace detail {
  class StandardsFilterObjectAttribute_Impl;
} // detail

/** A StandardsFilterObjectAttribute is a StandardsFilterClause that matches attribute values 
 *  assigned to particular building model objects. This base class factors out the aspects of 
 *  testing attribute values associated with object filtering, and specifies several virtually 
 *  implemented methods for its derived classes. StandardsFilterObjectAttribute is essentially 
 *  abstract as it has no public constructors. */
class RULESET_API StandardsFilterObjectAttribute : public StandardsFilterClause {
 public:
  /// @cond
  typedef detail::StandardsFilterObjectAttribute_Impl ImplType;
  /// @endcond

  /** @name Getters */
  //@{

  std::string shortDescription() const;

  StandardsFilterAvailableObject object() const;

  //@}
  /** @name Setters */
  //@{

  /** Flip this filter to the opposite form. (True -> false and vice versa, equal -> not equal, 
   *  less than -> greater than or equal to, etc.) */
  void negate();

  /** Set the object attribute's value. Provided for import of rules from clear text formats. 
   *  Virtual implementation. */
  bool setValue(const std::string& value);

  //@}
  /** @name Serialization */
  //@{

  std::string printForMessage() const;

  //@}
  /** @name Queries */
  //@{

  /** Same as equivalentData, except that this method ignores differences in attribute value(s). */
  bool equivalentAttributeType(const StandardsFilterObjectAttribute& other) const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterObjectAttribute_Impl;
  friend class RulesetObject;

  // protected object setter used by deepClone.
  void mf_setObject(const StandardsFilterAvailableObject& object);

  /** Protected constructor from impl. */
  StandardsFilterObjectAttribute(boost::shared_ptr<detail::StandardsFilterObjectAttribute_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterObjectAttribute");
};

/** \relates StandardsFilterObjectAttribute */
typedef boost::optional<StandardsFilterObjectAttribute> OptionalStandardsFilterObjectAttribute;

/** \relates StandardsFilterObjectAttribute */
typedef std::vector<StandardsFilterObjectAttribute> StandardsFilterObjectAttributeVector;

/** Predicate for sorting \link StandardsFilterObjectAttribute StandardsFilterObjectAttributes 
 *  \endlink by the standardsinterface::ObjectTypes to which they apply. The desired ordering of 
 *  ObjectTypes must be specified. \relates StandardsFilterObjectAttribute */
bool RULESET_API objectTypeLess(const StandardsFilterObjectAttribute& left, 
                                const StandardsFilterObjectAttribute& right,
                                const std::vector<standardsinterface::ObjectType>& order);

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTATTRIBUTE_HPP
