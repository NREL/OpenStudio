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

#include <ruleset/ModelObjectFilterRelationship.hpp>
#include <ruleset/ModelObjectFilterRelationship_Impl.hpp>

#include <model/ModelObject.hpp>
#include <model/Relationship.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterRelationship_Impl::ModelObjectFilterRelationship_Impl(
      const std::string& relationshipName)
    : ModelObjectFilterClause_Impl(), m_relationshipName(relationshipName)
  {}

  ModelObjectFilterRelationship_Impl::ModelObjectFilterRelationship_Impl(
      const std::string& relationshipName, 
      const UUID& uuid, 
      const UUID& versionUUID)
    : ModelObjectFilterClause_Impl(uuid, versionUUID), m_relationshipName(relationshipName)
  {}

  ModelObjectFilterRelationship_Impl::ModelObjectFilterRelationship_Impl(
      const QDomElement& element)
    : ModelObjectFilterClause_Impl(element)
  {
    OS_ASSERT(!element.isNull());
    OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

    QDomElement relationshipNameElement = element.firstChildElement(QString::fromStdString("RelationshipName"));

    OS_ASSERT(!relationshipNameElement.isNull());

    m_relationshipName = relationshipNameElement.firstChild().nodeValue().toStdString();
  }

  std::string ModelObjectFilterRelationship_Impl::relationshipName() const {
    return m_relationshipName;
  }

  std::string ModelObjectFilterRelationship_Impl::xmlElementName() const {
    return ModelObjectFilterRelationship::xmlElementName();
  }

  void ModelObjectFilterRelationship_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ModelObjectFilterClause_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("RelationshipName"));
    text = doc.createTextNode(toQString(m_relationshipName));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool ModelObjectFilterRelationship_Impl::equivalentData(const RulesetObject& other) const
  {
    // not a real implementation
    return ModelObjectFilterClause_Impl::equivalentData(other);
  }

  bool ModelObjectFilterRelationship_Impl::check(model::ModelObject& modelObject) const {
    IddObjectType originalType = modelObject.iddObject().type();
    bool result(false), retrieval(false);
    if (model::OptionalRelationship oRelationship = modelObject.getRelationship(relationshipName())) {
      model::Relationship relationship = *oRelationship;
      retrieval = true;
      if (relationship.isSingular()) {
        if (model::OptionalModelObject oModelObject = relationship.relatedModelObject()) {
          modelObject = *oModelObject;
          result = true;
        }
      }
    }

    LOG(Debug, "Retrieval of relationship '" << this->relationshipName() << "' from ModelObject of type '"
        << originalType.valueDescription() << (retrieval ? "' succeeded" : "' failed"));
    if (result) {
      LOG(Debug, "Relationship filter resulted in change from ModelObject of type "
          << originalType.valueDescription() << " to "
          << modelObject.iddObject().type().valueDescription() << ".");
    }
    return result;
  }

} // detail

std::string ModelObjectFilterRelationship::xmlElementName() {
  return "ModelObjectFilterRelationship";
}

ModelObjectFilterRelationship::ModelObjectFilterRelationship(const std::string& relationshipName)
  : ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterRelationship_Impl>(
                                new detail::ModelObjectFilterRelationship_Impl(relationshipName)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterRelationship_Impl>());
}

ModelObjectFilterRelationship::ModelObjectFilterRelationship(const std::string& relationshipName, 
                                                             const UUID& uuid, 
                                                             const UUID& versionUUID)
  : ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterRelationship_Impl>(
        new detail::ModelObjectFilterRelationship_Impl(relationshipName,uuid,versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterRelationship_Impl>());
}

ModelObjectFilterRelationship::ModelObjectFilterRelationship(const QDomElement& element)
: ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterRelationship_Impl>(
        new detail::ModelObjectFilterRelationship_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterRelationship_Impl>());
}

std::string ModelObjectFilterRelationship::relationshipName() const {
  return getImpl<detail::ModelObjectFilterRelationship_Impl>()->relationshipName();
}

/// @cond
ModelObjectFilterRelationship::ModelObjectFilterRelationship(boost::shared_ptr<detail::ModelObjectFilterRelationship_Impl> impl)
  : ModelObjectFilterClause(impl)
{}
/// @endcond

} // ruleset
} // openstudio

