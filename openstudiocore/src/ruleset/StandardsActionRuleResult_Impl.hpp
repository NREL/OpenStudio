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

#ifndef RULESET_STANDARDSACTIONRULERESULT_IMPL_HPP
#define RULESET_STANDARDSACTIONRULERESULT_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterApplyRulesetResult.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsActionRuleResult;

namespace detail {

  class RULESET_API StandardsActionRuleResult_Impl : public StandardsActionClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsActionRuleResult_Impl(const StandardsFilterObjectAttribute& attributeTest);

    StandardsActionRuleResult_Impl(const StandardsFilterObjectAttribute& attributeTest,
                                   const UUID& uuid,
                                   const UUID& versionUUID);

    StandardsActionRuleResult_Impl(const QDomElement& element);

    //@}
    /** @name Getters */
    //@{

    /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this StandardsActionClause, and/or
     *  \link StandardsFilterClause StandardsFilterClauses \endlink that define variables used by this StandardsActionClause. */
    virtual std::vector<StandardsFilterClause> referencedFilters() const;

    virtual std::vector<StandardsFilterClause> impliedFilters() const;

    /** Variable indicating the type of model that triggers the result. */
    std::string modelFactName() const;

    StandardsFilterObjectAttribute attributeTest() const;

    //@}
    /** @name Setters */
    //@{

    /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
     *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
     *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase. */
    virtual void replaceReferencedFilters(const std::vector<StandardsFilterClause>& preferredInstances);

    void setAttributeTest(const StandardsFilterObjectAttribute& filter);

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
    /** @name Serialization */
    //@{

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
   protected:
    StandardsFilterObjectAttribute m_attributeTest;         // refers to its own available object filter
    boost::optional<StandardsFilterAvailableModel> m_model; // implied if !m_attributeTest.object().model()
   private:
    REGISTER_LOGGER("Ruleset.StandardsActionRuleResult");

    void mf_updateImpliedFilters();
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSACTIONRULERESULT_IMPL_HPP
