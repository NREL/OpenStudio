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

#ifndef RULESET_RULE_IMPL_HPP
#define RULESET_RULE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetObject_Impl.hpp>
#include <ruleset/FilterClause.hpp>
#include <ruleset/ActionClause.hpp>

namespace openstudio {
namespace ruleset {

class Rule;

namespace detail {

  class RULESET_API Rule_Impl : public RulesetObject_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    Rule_Impl(const std::string& name);

    Rule_Impl(const std::string& name, 
              bool active, 
              const UUID& uuid, 
              const UUID& versionUUID,
              const std::vector<FilterClause>& filters, 
              const std::vector<ActionClause>& actions);

    Rule_Impl(const QDomElement& element);

    /** Returns public object associated with this Impl. */
    Rule rule() const;

    virtual ~Rule_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::string name() const;

    bool active() const;

    std::vector<FilterClause> filters() const;

    template <typename T>
    std::vector<T> getFilters() const {
      std::vector<T> result;
      std::vector<FilterClause> candidates = filters();
      for (std::vector<FilterClause>::const_iterator it = candidates.begin(),
           itEnd = candidates.end(); it != itEnd; ++it)
      {
        boost::optional<T> oFilter = it->optionalCast<T>();
        if (oFilter) { result.push_back(*oFilter); }
      }
      return result;
    }

    std::vector<ActionClause> actions() const;

    template <typename T>
    std::vector<T> getActions() const {
      std::vector<T> result;
      std::vector<ActionClause> candidates = actions();
      for (std::vector<ActionClause>::const_iterator it = candidates.begin(),
           itEnd = candidates.end(); it != itEnd; ++it)
      {
        boost::optional<T> oAction = it->optionalCast<T>();
        if (oAction) { result.push_back(*oAction); }
      }
      return result;
    }

    //@}
    /** @name Setters */
    //@{

    void setName(const std::string& name);

    /** After this call rule.active() == true. */
    void activate();

    /** After this call rule.active() == false. */
    void disable();

    virtual bool add(const FilterClause& filter);

    virtual bool add(const std::vector<FilterClause>& filters);

    virtual bool add(const ActionClause& action);

    virtual bool remove(const FilterClause& filter);

    virtual bool remove(const ActionClause& action);

    virtual unsigned clearFilters();

    virtual unsigned clearActions();

    //@}
    /** @name Serialization */
    //@{

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    unsigned numFilters() const;

    unsigned numActions() const;

    bool contains(const FilterClause& filter) const;

    /** Returns true if there is a FilterClause in this rule for which filter.equivalentData returns
     *  true. */
    bool containsEquivalentData(const FilterClause& filter) const;

    bool contains(const ActionClause& action) const;

    /** Returns true if there is an ActionClause in this rule for which action.equivalentData returns
     *  true. */
    bool containsEquivalentData(const ActionClause& action) const;

    //@}
   protected:
    std::vector<FilterClause> m_filters;
    std::vector<ActionClause> m_actions;
   private:
    std::string m_name;
    bool m_active;

    REGISTER_LOGGER("Ruleset.Rule");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_RULE_IMPL_HPP
