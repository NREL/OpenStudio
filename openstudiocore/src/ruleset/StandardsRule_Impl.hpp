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

#ifndef RULESET_STANDARDSRULE_IMPL_HPP
#define RULESET_STANDARDSRULE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Rule_Impl.hpp>

#include <standardsinterface/DataDictionary.hpp>

namespace openstudio {
namespace ruleset {

class StandardsRule;
class StandardsRulesetPrintMode;
class StandardsFilterAvailableObject;
class StandardsFilterObjectAttribute;

namespace detail {

  class RULESET_API StandardsRule_Impl : public Rule_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsRule_Impl(const std::string& name);

    StandardsRule_Impl(const std::string& name, 
                       bool active, 
                       const UUID& uuid, 
                       const UUID& versionUUID,
                       const std::vector<FilterClause>& filters, 
                       const std::vector<ActionClause>& actions);

    StandardsRule_Impl(const QDomElement& element);
    
    virtual ~StandardsRule_Impl() {}

    //@}
    /** @name Getters */
    //@{

    virtual std::vector<StandardsRule> impliedRules() const;

    //@}
    /** @name Setters */
    //@{

    virtual bool add(const FilterClause& filter);

    virtual bool add(const std::vector<FilterClause>& filters);

    virtual bool add(const ActionClause& action);

    virtual bool remove(const FilterClause& filter);

    virtual bool remove(const ActionClause& action);

    //@}
    /** @name Serialization */
    //@{

    std::string print(const StandardsRulesetPrintMode& mode) const;

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    /** Same as containsEquivalentData except that the attribute values do not have to match. */
    bool containsEquivalentAttributeType(const StandardsFilterObjectAttribute& attributeFilter) const;

    /** \overload */
    bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                         const standardsinterface::BooleanAttribute& attribute) const;

    /** \overload */
    bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                         const standardsinterface::DoubleAttribute& attribute) const;

    /** \overload */
    bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                         const standardsinterface::EnumAttribute& attribute) const;

    /** \overload */
    bool containsEquivalentAttributeType(const standardsinterface::ObjectType& objectType,
                                         const standardsinterface::IntegerAttribute& attribute) const;

    //@}
   protected:
    typedef std::map<FilterClause,std::set<Clause> > FilterToSetOfClausesMap;
    FilterToSetOfClausesMap m_filterReferencedByMap;

    StandardsRule mf_basicGetAttributeImpliedRule(
        const StandardsFilterObjectAttribute& attribute) const;

   private:
    REGISTER_LOGGER("Ruleset.StandardsRule");

    /** Tries to intuit the appropriate StandardsFilterAvailableModel to associate with the 
     *  a test filter to give the best chance for an equivalentData-type match. */
    boost::optional<StandardsFilterAvailableObject> mf_getLikelyMatchObjectFilter(
        const standardsinterface::ObjectType& objectType) const;
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULE_IMPL_HPP
