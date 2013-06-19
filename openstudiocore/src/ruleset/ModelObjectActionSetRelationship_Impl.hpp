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

#ifndef RULESET_MODELOBJECTACTIONSETRELATIONSHIP_IMPL_HPP
#define RULESET_MODELOBJECTACTIONSETRELATIONSHIP_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/ModelObjectActionClause_Impl.hpp>

#include <model/Component.hpp>

#include <utilities/core/FileReference.hpp>

#include <boost/optional.hpp>

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

  class ModelObjectActionSetRelationship;

namespace detail {


  class RULESET_API ModelObjectActionSetRelationship_Impl : public ModelObjectActionClause_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    ModelObjectActionSetRelationship_Impl(const std::string& relationshipName, 
                                          const openstudio::path& componentPath);

    ModelObjectActionSetRelationship_Impl(const std::string& relationshipName,
                                          const FileReference& componentReference);

    ModelObjectActionSetRelationship_Impl(const std::string& relationshipName, 
                                          const FileReference& componentReference,
                                          const UUID& uuid, 
                                          const UUID& versionUUID);

    ModelObjectActionSetRelationship_Impl(const QDomElement& element);
    
    virtual ~ModelObjectActionSetRelationship_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::string relationshipName() const;

    openstudio::path componentPath() const;

    FileReference componentReference() const;

    boost::optional<openstudio::model::Component> component() const;

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

    virtual bool apply(openstudio::model::ModelObject& modelObject) const;

    //@}

   private:
    REGISTER_LOGGER("Ruleset.ModelObjectActionSetRelationship");

    std::string m_relationshipName;
    FileReference m_componentReference;
    // cache component after it is requested
    mutable boost::optional<openstudio::model::Component> m_component;
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_MODELOBJECTACTIONSETRELATIONSHIP_IMPL_HPP
