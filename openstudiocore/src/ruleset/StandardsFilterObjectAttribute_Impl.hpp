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

#ifndef RULESET_STANDARDSFILTEROBJECTATTRIBUTE_IMPL_HPP
#define RULESET_STANDARDSFILTEROBJECTATTRIBUTE_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

class StandardsFilterObjectAttribute;

namespace detail {

  class RULESET_API StandardsFilterObjectAttribute_Impl : public StandardsFilterClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    StandardsFilterObjectAttribute_Impl(const StandardsFilterAvailableObject& object);

    StandardsFilterObjectAttribute_Impl(const StandardsFilterAvailableObject& object,
                                        const UUID& uuid,
                                        const UUID& versionUUID);

    StandardsFilterObjectAttribute_Impl(const QDomElement& element);

    //@}
    /** @name Getters */
    //@{

    /** \link StandardsFilterClause StandardsFilterClauses \endlink used to construct this ActionClause, and/or
     *  \link StandardsFilterClause StandardsFilterClauses \endlink that define variables used by this ActionClause. */
    virtual std::vector<StandardsFilterClause> referencedFilters() const;

    virtual std::string shortDescription() const = 0;

    StandardsFilterAvailableObject object() const;

    //@}
    /** @name Setters */
    //@{

    /** Any referenced filters whose UUIDs match preferredInstances[i] are to be removed and 
     *  replaced with preferredInstances[i]. This method assists in reconstruction of \link 
     *  StandardsRule StandardsRules \endlink stored in a project::ProjectDatabase. */
    virtual void replaceReferencedFilters(std::vector<StandardsFilterClause>& preferredInstances);

    void setObject(const StandardsFilterAvailableObject& object);

    /** Flip this filter to the opposite form. (True -> false and vice versa, equal -> not equal, 
     *  less than -> greater than or equal to, etc.) */
    virtual void negate() = 0;

    /** Set the object attribute's value. Provided for import of rules from clear text formats. 
     *  Virtual implementation. */
    virtual bool setValue(const std::string& value) = 0;

    //@}
    /** @name Serialization */
    //@{

    virtual std::string print() const = 0;

    virtual std::string printForMessage() const = 0;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    /** Same as equivalentData, except that this method ignores differences in attribute value(s). */
    virtual bool equivalentAttributeType(const StandardsFilterObjectAttribute& other) const;

    //@}
   protected:
    StandardsFilterAvailableObject m_object;

   private:
    REGISTER_LOGGER("Ruleset.StandardsFilterObjectAttribute");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSFILTEROBJECTATTRIBUTE_IMPL_HPP
