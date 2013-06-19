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

#ifndef RULESET_STANDARDSACTIONRULERESULT_HPP
#define RULESET_STANDARDSACTIONRULERESULT_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause.hpp>

#include <ruleset/StandardsRulesetEnums.hpp>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableObject;
class StandardsFilterObjectAttribute;

namespace detail {
  class StandardsActionRuleResult_Impl;
} // detail

/** A StandardsActionRuleResult StandardsActionClause is a base class for actions that test the 
 *  value of an object's attribute, with the purpose of arriving at a pass or fail decision for 
 *  compliance. */
class RULESET_API StandardsActionRuleResult : public StandardsActionClause {
 public:
  /// @cond
  typedef detail::StandardsActionRuleResult_Impl ImplType;
  /// @endcond

  /** @name Getters */
  //@{

  /** Fact name of the model that triggers the result. */
  std::string modelFactName() const;

  StandardsFilterObjectAttribute attributeTest() const;

  //@}
 protected:
  /// @cond
  friend class detail::StandardsActionRuleResult_Impl;
  friend class RulesetObject;

  // Protected setter provided for deepClone.
  void mf_setAttributeTest(const StandardsFilterObjectAttribute& filter);

  /** Protected constructor from impl. */
  StandardsActionRuleResult(boost::shared_ptr<detail::StandardsActionRuleResult_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("Ruleset.StandardsActionRuleResult");
};

/** \relates StandardsActionRuleResult */
typedef boost::optional<StandardsActionRuleResult> OptionalStandardsActionRuleResult;

/** \relates StandardsActionRuleResult */
typedef std::vector<StandardsActionRuleResult> StandardsActionRuleResultVector;

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONRULERESULT_HPP
