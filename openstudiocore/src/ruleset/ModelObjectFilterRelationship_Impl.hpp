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

#ifndef RULESET_MODELOBJECTFILTERRELATIONSHIP_IMPL_HPP
#define RULESET_MODELOBJECTFILTERRELATIONSHIP_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  /** ModelObjectFilterRelationship_Impl is a ModelObjectFilterClause_Impl that is the implementation class for ModelObjectFilterRelationship.*/
  class RULESET_API ModelObjectFilterRelationship_Impl : public ModelObjectFilterClause_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ModelObjectFilterRelationship_Impl(const std::string& relationshipName);

    /** Constructor provided for deserialization; not for general use. */
    ModelObjectFilterRelationship_Impl(const std::string& relationshipName, 
                                       const UUID& uuid, 
                                       const UUID& versionUUID); 

    ModelObjectFilterRelationship_Impl(const QDomElement& element);

    virtual ~ModelObjectFilterRelationship_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::string relationshipName() const;

    //@}
    /** @name Serialization */
    //@{

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
    /** @name Actions */
    //@{

     virtual bool check(model::ModelObject& modelObject) const;

    //@}
   protected:
   private:
    std::string m_relationshipName;

    REGISTER_LOGGER("openstudio.ruleset.ModelObjectFilterRelationship");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTFILTERRELATIONSHIP_IMPL_HPP

