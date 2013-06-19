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

#ifndef RULESET_STANDARDSFILTEROBJECTNUMERICALATTRIBUTE_HPP
#define RULESET_STANDARDSFILTEROBJECTNUMERICALATTRIBUTE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsFilterObjectNumericalAttribute_Impl;
} // detail

/** StandardsFilterObjectNumericalAttribute is a StandardsFilterObjectAttribute that serves as a 
 *  base class for StandardsFilterObjectIntegerAttribute and StandardsFilterObjectDoubleAttribute. 
 *  It factors out the notions of a RulesetNumericalPredicate and a unit string ("J/m^2", etc.). */
class RULESET_API StandardsFilterObjectNumericalAttribute : public StandardsFilterObjectAttribute {
 public: 
  /// @cond
  typedef detail::StandardsFilterObjectNumericalAttribute_Impl ImplType;
  /// @endcond

  /** @name Getters */
  //@{

  RulesetNumericalPredicate predicate() const;

  std::string units() const;

  //@}
  /** @name Setters */
  //@{

  void setPredicate(const RulesetNumericalPredicate& predicate);

  void setUnits(const std::string& unitString);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsFilterObjectNumericalAttribute_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsFilterObjectNumericalAttribute(boost::shared_ptr<detail::StandardsFilterObjectNumericalAttribute_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsFilterObjectNumericalAttribute");
};

/** \relates StandardsFilterObjectNumericalAttribute */
typedef boost::optional<StandardsFilterObjectNumericalAttribute> OptionalStandardsFilterObjectNumericalAttribute;

/** \relates StandardsFilterObjectNumericalAttribute */
typedef std::vector<StandardsFilterObjectNumericalAttribute> StandardsFilterObjectNumericalAttributeVector;


} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTNUMERICALATTRIBUTE_HPP
