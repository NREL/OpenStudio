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

#ifndef RULESET_STANDARDSACTIONRULEVIOLATED_HPP
#define RULESET_STANDARDSACTIONRULEVIOLATED_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionRuleResult.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {
  class StandardsActionRuleViolated_Impl;
} // detail

/** A StandardsActionRuleViolated StandardsActionRuleResult provides the CLIPS constructs 
 *  necessary to set the rulesengine::ApplyRulesetResult::overallResultFlag to 
 *  StandardsRulesetResult::Fail, and print an informative message containing a brief 
 *  description of attributeTest(), and the actual attribute value of the object that triggered
 *  the rule. Best used with StandardsAttributeCheckRule. */
class RULESET_API StandardsActionRuleViolated : public StandardsActionRuleResult {
 public:
  /// @cond
  typedef detail::StandardsActionRuleViolated_Impl ImplType;

  static std::string xmlElementName();
  /// @endcond

  /** @name Constructors and Destructors */
  //@{

  StandardsActionRuleViolated(const StandardsFilterObjectAttribute& attributeTest);

  /** Constructor provided for deserialization; not for general use. */
  StandardsActionRuleViolated(const StandardsFilterObjectAttribute& attributeTest,
                              const UUID& uuid,
                              const UUID& versionUUID);

  //@}
 protected:
  /// @cond
  friend class detail::StandardsActionRuleViolated_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  StandardsActionRuleViolated(boost::shared_ptr<detail::StandardsActionRuleViolated_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsActionRuleViolated");
};

/** \relates StandardsActionRuleViolated */
typedef boost::optional<StandardsActionRuleViolated> OptionalStandardsActionRuleViolated;

/** \relates StandardsActionRuleViolated */
typedef std::vector<StandardsActionRuleViolated> StandardsActionRuleViolatedVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONRULEVIOLATED_HPP
