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

#ifndef RULESET_STANDARDSFILTERAPPLYRULESETRESULT_HPP
#define RULESET_STANDARDSFILTERAPPLYRULESETRESULT_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause.hpp>

#include <standardsinterface/ObjectType.hxx>

namespace openstudio {
namespace ruleset {

class StandardsFilterAvailableModel;

namespace detail {
  class StandardsFilterApplyRulesetResult_Impl;

  /** Get the ApplyRulesetResult for the Model referred to by StandardsFilterAvailableObject. This is a
   *  back-end filter added by Rule when there is an StandardsActionRuleResult. */
  class RULESET_API StandardsFilterApplyRulesetResult : public StandardsFilterClause {
   public:

     typedef StandardsFilterApplyRulesetResult_Impl ImplType;

     static std::string xmlElementName();

    /** @name Constructors and Destructors */
    //@{

    StandardsFilterApplyRulesetResult(const StandardsFilterAvailableModel& model);

    StandardsFilterApplyRulesetResult(const StandardsFilterAvailableModel& model,
                                      const UUID& uuid,
                                      const UUID& versionUUID);

    //@}
    /** @name Getters */
    //@{

    StandardsFilterAvailableModel model() const;

    //@}
   protected:

    friend class StandardsFilterApplyRulesetResult_Impl;
    friend class RulesetObject;

    // Protected setter provided for deepClone.
    void mf_setModel(const StandardsFilterAvailableModel& model);

    /** Protected constructor from impl. */
    StandardsFilterApplyRulesetResult(boost::shared_ptr<StandardsFilterApplyRulesetResult_Impl> impl);

   private:
    REGISTER_LOGGER("Ruleset.StandardsFilterApplyRulesetResult");
  };

} // detail
} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERAPPLYRULESETRESULT_HPP
