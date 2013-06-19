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

#ifndef ANALYSIS_MODELRULESETPERTURBATION_HPP
#define ANALYSIS_MODELRULESETPERTURBATION_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/DiscretePerturbation.hpp>

namespace openstudio {

namespace ruleset {
  class ModelRuleset;
}

namespace analysis {

namespace detail {

  class ModelRulesetPerturbation_Impl;

} // detail

/** ModelRulesetPerturbation is a DiscretePerturbation that applies a ruleset::ModelRuleset to an
 *  OpenStudio model::Model to generate a perturbed Model. */
class ANALYSIS_API ModelRulesetPerturbation : public DiscretePerturbation {
 public:
  /** @name Constructors and Destructors */
  //@{

  ModelRulesetPerturbation(const ruleset::ModelRuleset& ruleset,
                           bool isSelected = true);

  /** Constructor provided for deserialization; not for general use. */
  ModelRulesetPerturbation(const UUID& uuid,
                           const UUID& versionUUID,
                           const std::string& name,
                           const std::string& displayName,
                           const std::string& description,
                           bool isSelected,
                           const ruleset::ModelRuleset& ruleset);

  virtual ~ModelRulesetPerturbation() {}

  //@}
  /** @name Getters and Queries */
  //@{

  ruleset::ModelRuleset ruleset() const;

  //@}
 protected:
  /// @cond
  typedef detail::ModelRulesetPerturbation_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::ModelRulesetPerturbation_Impl;

  explicit ModelRulesetPerturbation(boost::shared_ptr<detail::ModelRulesetPerturbation_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.ModelRulesetPerturbation");
};

/** \relates ModelRulesetPerturbation*/
typedef boost::optional<ModelRulesetPerturbation> OptionalModelRulesetPerturbation;

/** \relates ModelRulesetPerturbation*/
typedef std::vector<ModelRulesetPerturbation> ModelRulesetPerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_MODELRULESETPERTURBATION_HPP

