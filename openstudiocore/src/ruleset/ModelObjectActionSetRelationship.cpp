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

#include <ruleset/ModelObjectActionSetRelationship.hpp>
#include <ruleset/ModelObjectActionSetRelationship_Impl.hpp>

#include <osversion/VersionTranslator.hpp>

#include <model/Component.hpp>
#include <model/ComponentData.hpp>
#include <model/ModelObject.hpp>
#include <model/Relationship.hpp>

#include <utilities/core/Assert.hpp>

#include <QDomElement>
#include <QDomDocument>

namespace openstudio {
namespace ruleset {
  namespace detail {

    ModelObjectActionSetRelationship_Impl::ModelObjectActionSetRelationship_Impl(
        const std::string& relationshipName, 
        const openstudio::path& componentPath)
      : ModelObjectActionClause_Impl(),
        m_relationshipName(relationshipName), 
        m_componentReference(componentPath)
    {}

    ModelObjectActionSetRelationship_Impl::ModelObjectActionSetRelationship_Impl(
        const std::string& relationshipName, 
        const FileReference& componentReference)
      : ModelObjectActionClause_Impl(),
        m_relationshipName(relationshipName), 
        m_componentReference(componentReference)
    {}

    ModelObjectActionSetRelationship_Impl::ModelObjectActionSetRelationship_Impl(
        const std::string& relationshipName, 
        const FileReference& componentReference,
        const UUID& uuid, 
        const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), 
        m_relationshipName(relationshipName), 
        m_componentReference(componentReference)
    {}

    ModelObjectActionSetRelationship_Impl::ModelObjectActionSetRelationship_Impl(
        const QDomElement& element)
      : ModelObjectActionClause_Impl(element),
        m_componentReference(openstudio::path())
    {
      OS_ASSERT(!element.isNull());
      OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

      QDomElement relationshipNameElement = element.firstChildElement(QString::fromStdString("RelationshipName"));
      QDomElement componentPathElement = element.firstChildElement(QString::fromStdString("ComponentPath"));
      QDomElement componentUUIDElement = element.firstChildElement(QString::fromStdString("ComponentUUID"));
      QDomElement componentVersionUUIDElement = element.firstChildElement(QString::fromStdString("ComponentVersionUUID"));

      OS_ASSERT(!relationshipNameElement.isNull());
      OS_ASSERT(!componentPathElement.isNull());
      OS_ASSERT(!componentUUIDElement.isNull());
      OS_ASSERT(!componentVersionUUIDElement.isNull());

      m_relationshipName = relationshipNameElement.firstChild().nodeValue().toStdString();
      // ETH@20111003 Not pretty because FileReference not serialized to XML. 
      m_componentReference = FileReference(toPath(componentPathElement.firstChild().nodeValue().toStdString()));

      UUID componentUUID = toUUID(componentUUIDElement.firstChild().nodeValue().toStdString());
      UUID componentVersionUUID = toUUID(componentVersionUUIDElement.firstChild().nodeValue().toStdString());

      if (model::OptionalComponent component = this->component()) {
        if (componentUUID != component->componentData().uuid()){
          LOG(Error, "Component UUID '" << toString(component->componentData().uuid())
            << "' does not match expected '" << toString(componentUUID) << "'");
        }
        if (componentVersionUUID != component->componentData().versionUUID()){
           LOG(Error, "Component version UUID '" << toString(component->componentData().versionUUID())
            << "' does not match expected '" << toString(componentVersionUUID) << "'");
        }
      }else{
        LOG(Error, "Could not load component from '" << toString(componentPath()) << "'");
      }
    }

    std::string ModelObjectActionSetRelationship_Impl::relationshipName() const
    {
      return m_relationshipName;
    }

    openstudio::path ModelObjectActionSetRelationship_Impl::componentPath() const
    {
      return componentReference().path();
    }

    FileReference ModelObjectActionSetRelationship_Impl::componentReference() const {
      return m_componentReference;
    }

    boost::optional<openstudio::model::Component> ModelObjectActionSetRelationship_Impl::component() const
    {
      if (!m_component) {
        osversion::VersionTranslator translator;
        m_component = translator.loadComponent(m_componentReference.path());
      }
      return m_component;
    }

    std::string ModelObjectActionSetRelationship_Impl::xmlElementName() const
    {
      return ModelObjectActionSetRelationship::xmlElementName();
    }

    void ModelObjectActionSetRelationship_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
    {
      ModelObjectActionClause_Impl::writeValues(doc, element);

      QDomElement childElement;
      QDomText text;

      childElement = doc.createElement(QString::fromStdString("RelationshipName"));
      text = doc.createTextNode(toQString(m_relationshipName));
      childElement.appendChild(text);
      element.appendChild(childElement);

      childElement = doc.createElement(QString::fromStdString("ComponentPath"));
      text = doc.createTextNode(toQString(componentPath()));
      childElement.appendChild(text);
      element.appendChild(childElement);

      if (model::OptionalComponent component = this->component()){
        childElement = doc.createElement(QString::fromStdString("ComponentUUID"));
        text = doc.createTextNode(toQString(toString(component->componentData().uuid())));
        childElement.appendChild(text);
        element.appendChild(childElement);

        childElement = doc.createElement(QString::fromStdString("ComponentVersionUUID"));
        text = doc.createTextNode(toQString(toString(component->componentData().versionUUID())));
        childElement.appendChild(text);
        element.appendChild(childElement);
      }
    }

    bool ModelObjectActionSetRelationship_Impl::apply(openstudio::model::ModelObject& modelObject) const
    {
      bool result = false;

      if (model::OptionalComponent component = this->component()){

        boost::optional<openstudio::model::Relationship> relationship = modelObject.getRelationship(m_relationshipName);
        if (relationship){

          boost::optional<openstudio::model::ComponentData> componentData = modelObject.model().insertComponent(*component);
          if (componentData){

            result = relationship->setRelatedModelObject(componentData->primaryComponentObject());

          }
        }
      }

      LOG(Debug, "Setting relationship '" << this->relationshipName() << "' for ModelObject of type '"
        << modelObject.iddObject().name() << (result ? "' succeeded" : " failed"));

      return result;
    }

    bool ModelObjectActionSetRelationship_Impl::equivalentData(const RulesetObject& other) const {
      bool result = true;
      result = result && ModelObjectActionClause_Impl::equivalentData(other);
      return result;
    }

  } // detail

  std::string ModelObjectActionSetRelationship::xmlElementName()
  {
    return "ModelObjectActionSetRelationship";
  }

  ModelObjectActionSetRelationship::ModelObjectActionSetRelationship(
      const std::string& relationshipName, const openstudio::path& componentPath)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetRelationship_Impl>(
          new detail::ModelObjectActionSetRelationship_Impl(relationshipName, componentPath)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetRelationship_Impl>());
  }

  ModelObjectActionSetRelationship::ModelObjectActionSetRelationship(
      const std::string& relationshipName, const FileReference& componentReference)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetRelationship_Impl>(
          new detail::ModelObjectActionSetRelationship_Impl(relationshipName, componentReference)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetRelationship_Impl>());
  }

  ModelObjectActionSetRelationship::ModelObjectActionSetRelationship(
      const std::string& relationshipName, 
      const FileReference& componentReference,
      const UUID& uuid, 
      const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetRelationship_Impl>(
          new detail::ModelObjectActionSetRelationship_Impl(relationshipName, 
                                                            componentReference, 
                                                            uuid, 
                                                            versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetRelationship_Impl>());
  }

  ModelObjectActionSetRelationship::ModelObjectActionSetRelationship(const QDomElement& element)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetRelationship_Impl>(new detail::ModelObjectActionSetRelationship_Impl(element)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetRelationship_Impl>());
  }

  /// @cond
  ModelObjectActionSetRelationship::ModelObjectActionSetRelationship(boost::shared_ptr<detail::ModelObjectActionSetRelationship_Impl> impl)
    : ModelObjectActionClause(impl)
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetRelationship_Impl>());
  }
  /// @endcond

  std::string ModelObjectActionSetRelationship::relationshipName() const
  {
    return getImpl<detail::ModelObjectActionSetRelationship_Impl>()->relationshipName();
  }

  openstudio::path ModelObjectActionSetRelationship::componentPath() const
  {
    return getImpl<detail::ModelObjectActionSetRelationship_Impl>()->componentPath();
  }

  FileReference ModelObjectActionSetRelationship::componentReference() const
  {
    return getImpl<detail::ModelObjectActionSetRelationship_Impl>()->componentReference();
  }

  boost::optional<openstudio::model::Component> ModelObjectActionSetRelationship::component() const
  {
    return getImpl<detail::ModelObjectActionSetRelationship_Impl>()->component();
  }

} // ruleset
} // openstudio
