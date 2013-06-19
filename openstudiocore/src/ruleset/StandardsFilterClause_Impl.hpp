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

#ifndef RULESET_STANDARDSFILTERCLAUSE_IMPL_HPP
#define RULESET_STANDARDSFILTERCLAUSE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/FilterClause_Impl.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsFilterClause;

namespace detail {

  class RULESET_API StandardsFilterClause_Impl : public FilterClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterClause_Impl();

    StandardsFilterClause_Impl(const UUID& uuid, const UUID& versionUUID);

    StandardsFilterClause_Impl(const QDomElement& element);

    /** Creates a clone of this object with disconnected data. Related objects are retained as-is 
     *  (data shared with this object). */
    virtual RulesetObject clone() const = 0;

    /** Creates a clone of this object with deeply disconnected data. Related objects are also 
     *  cloned. */
    virtual RulesetObject deepClone() const = 0;

    //@}
    /** @name Getters */
    //@{

    /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this Clause, and/or define
     *  variables used by this Clause. */
    virtual std::vector<StandardsFilterClause> referencedFilters() const;

    /** \link StandardsFilterClause StandardsFilterClauses \endlink implied by this Clause, that is referenced, and
     *  needed by CLIPS, but not necessary for the user to specify. These clauses are generated and
     *  printed on the fly as needed. */
    virtual std::vector<StandardsFilterClause> impliedFilters() const;

    /** Get fact name. */
    virtual std::string factName() const = 0;

    //@}
    /** @name Setters */
    //@{

    /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
     *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
     *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase. */
    virtual void replaceReferencedFilters(std::vector<StandardsFilterClause>& preferredInstances);

    //@}
    /** @name Serialization */
    //@{

    /** Print as CLIPS statement. */
    virtual std::string print() const = 0;

    /** If the fact matching this clause is to be retracted, prints the retract action clause.
     *  Otherwise, return value is .empty(). */
    std::string printRetractAction() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
   private:
    REGISTER_LOGGER("Ruleset.StandardsFilterClause");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTERCLAUSE_IMPL_HPP
