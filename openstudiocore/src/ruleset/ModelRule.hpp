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

#ifndef RULESET_MODELRULE_HPP
#define RULESET_MODELRULE_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Rule.hpp>

namespace openstudio {
namespace model {
  class Model;
}

namespace ruleset {

namespace detail {
  class ModelRule_Impl;
} // detail

/** A ModelRule is a Rule that applies directly to model::Models (as opposed to being indirectly
 *  applied through the standardsinterface and a rulesengine). */
class RULESET_API ModelRule : public Rule {
 public:
  /// @cond
  typedef detail::ModelRule_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  ModelRule(const std::string& name);

  /** Constructor provided for deserialization; not for general use. */
  ModelRule(const std::string& name, 
            bool active, 
            const UUID& uuid, 
            const UUID& versionUUID,
            const std::vector<FilterClause>& filters, 
            const std::vector<ActionClause>& actions);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelRule(const QDomElement& element);
  
  virtual ~ModelRule() {}

  //@}
  /** @name Actions */
  //@{

  bool apply(model::Model& model) const;

  //@}
 protected:
  /// @cond
  friend class detail::ModelRule_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelRule(boost::shared_ptr<detail::ModelRule_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.ModelRule");
};

/** \relates ModelRule */
typedef std::vector<ModelRule> ModelRuleVector;

} // ruleset
} // openstudio

#endif // RULESET_MODELRULE_HPP
