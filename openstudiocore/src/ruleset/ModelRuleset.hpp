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

#ifndef RULESET_MODELRULESET_HPP
#define RULESET_MODELRULESET_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Ruleset.hpp>

namespace openstudio {
namespace model {
  class Model;
}
namespace ruleset {

class ModelRule;

namespace detail {
  class ModelRuleset_Impl;
} // detail

/** A ModelRuleset Ruleset consists of \link ModelRule ModelRules \endlink, and can be directly 
 *  applied to a model::Model. */
class RULESET_API ModelRuleset : public Ruleset {
 public:
  /// @cond
  typedef detail::ModelRuleset_Impl ImplType;
  /// @endcond

  static std::string xmlElementName();

  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Creates empty ruleset. */
  ModelRuleset(const std::string& name);

  /** Constructor provided for deserialization; not for general use. */
  ModelRuleset(const std::string& name, const UUID& uuid, const UUID& versionUUID,
               const std::vector<ModelRule>& rules);

  /** Constructor provided for deserialization; not for general use. */
  explicit ModelRuleset(const QDomElement& element);
  
  virtual ~ModelRuleset() {}

  //@}
  /** @name Getters */
  //@{

  std::vector<ModelRule> rules() const;

  //@}
  /** @name Setters */
  //@{

  void add(const ModelRule& rule);

  //@}
  /** @name Actions */
  //@{

  bool apply(openstudio::model::Model& model);

  //@}
 protected:
  /// @cond
  friend class Ruleset;
  friend class detail::ModelRuleset_Impl;
  friend class RulesetObject;

  /** Protected constructor from impl. */
  ModelRuleset(boost::shared_ptr<detail::ModelRuleset_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("Ruleset.ModelRuleset");
};

} // ruleset
} // openstudio

#endif // RULESET_MODELRULESET_HPP
