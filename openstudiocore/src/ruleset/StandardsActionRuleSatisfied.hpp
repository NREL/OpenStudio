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

#ifndef RULESET_STANDARDSACTIONRULESATISFIED_HPP
#define RULESET_STANDARDSACTIONRULESATISFIED_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionRuleResult.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsActionRuleSatisfied_Impl;
} // detail

/** A StandardsActionRuleSatisfied StandardsActionRuleResult provides a CLIPS construct 
 *  for printing an informative message containing a brief description of attributeTest() and the 
 *  actual attribute value of the object that triggered the rule. Lets the user verify that a
 *  building energy standards rule of a particular type fired, checked, and confirmed to pass. 
 *  Best used with StandardsAttributeCheckRule. */
class RULESET_API StandardsActionRuleSatisfied : public StandardsActionRuleResult {
 public:
  /// @cond
  typedef detail::StandardsActionRuleSatisfied_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  StandardsActionRuleSatisfied(const StandardsFilterObjectAttribute& attributeTest);

  /** Constructor provided for deserialization; not for general use. */
  StandardsActionRuleSatisfied(const StandardsFilterObjectAttribute& attributeTest,
                               const UUID& uuid,
                               const UUID& versionUUID);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsActionRuleSatisfied_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsActionRuleSatisfied(boost::shared_ptr<detail::StandardsActionRuleSatisfied_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsActionRuleSatisfied");
};

/** \relates StandardsActionRuleSatisfied */
typedef boost::optional<StandardsActionRuleSatisfied> OptionalStandardsActionRuleSatisfied;

/** \relates StandardsActionRuleSatisfied */
typedef std::vector<StandardsActionRuleSatisfied> StandardsActionRuleSatisfiedVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONRULESATISFIED_HPP
