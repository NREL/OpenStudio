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

#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/ModelObjectActionSetAttribute_Impl.hpp>

#include <model/Model.hpp>

#include <utilities/core/Assert.hpp>

#include <QDomDocument>
#include <QDomElement>

using openstudio::AttributeValueType;

namespace openstudio {
namespace ruleset {
  namespace detail {

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, bool value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Boolean), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, bool value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Boolean), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, unsigned value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Unsigned), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, unsigned value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Unsigned), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, int value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Integer), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, int value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Integer), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, double value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Double), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, double value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::Double), m_value(value)
    {}

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const std::string& value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::String)
    {
      m_value.setValue(value);
    }

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const std::string& value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::String)
    {
      m_value.setValue(value);
    }

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const char* value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::String)
    {
      m_value.setValue(std::string(value));
    }

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const char* value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(AttributeValueType::String)
    {
      m_value.setValue(std::string(value));
    }

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const openstudio::AttributeValueType& attributeValueType, const QVariant& value)
      : ModelObjectActionClause_Impl(), m_attributeName(attributeName), m_attributeValueType(attributeValueType), m_value(value)
    {
    }

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const std::string& attributeName, const openstudio::AttributeValueType& attributeValueType, const QVariant& value,
                                            const UUID& uuid, const UUID& versionUUID)
      : ModelObjectActionClause_Impl(uuid, versionUUID), m_attributeName(attributeName), m_attributeValueType(attributeValueType), m_value(value)
    {
    }

    ModelObjectActionSetAttribute_Impl::ModelObjectActionSetAttribute_Impl(const QDomElement& element)
      : ModelObjectActionClause_Impl(element)
    {
      OS_ASSERT(!element.isNull());
      OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

      QDomElement attributeNameElement = element.firstChildElement(QString::fromStdString("AttributeName"));
      QDomElement attributeValueTypeElement = element.firstChildElement(QString::fromStdString("AttributeValueType"));
      QDomElement valueElement = element.firstChildElement(QString::fromStdString("Value"));

      OS_ASSERT(!attributeNameElement.isNull());
      OS_ASSERT(!attributeValueTypeElement.isNull());
      OS_ASSERT(!valueElement.isNull());

      m_attributeName = attributeNameElement.firstChild().nodeValue().toStdString();
      m_attributeValueType = AttributeValueType(attributeValueTypeElement.firstChild().nodeValue().toStdString());

      switch(m_attributeValueType.value()){
        case AttributeValueType::Boolean:
          if (valueElement.firstChild().nodeValue() == "true"){
            m_value = true;
          }else{
            m_value = false;
          }
          break;
        case AttributeValueType::Integer:
          m_value = valueElement.firstChild().nodeValue().toInt();
          break;
        case AttributeValueType::Unsigned:
          m_value = valueElement.firstChild().nodeValue().toUInt();
          break;
        case AttributeValueType::Double:
          m_value = valueElement.firstChild().nodeValue().toDouble();
          break;
        case AttributeValueType::String:
          m_value.setValue(valueElement.firstChild().nodeValue().toStdString());
          break;
        case AttributeValueType::AttributeVector:
          OS_ASSERT(false);
          break;
        default:
          OS_ASSERT(false);
          break;
      }
    }

    std::string ModelObjectActionSetAttribute_Impl::attributeName() const
    {
      return m_attributeName;
    }
    
    AttributeValueType ModelObjectActionSetAttribute_Impl::attributeValueType() const
    {
      return m_attributeValueType;
    }

    QVariant ModelObjectActionSetAttribute_Impl::attributeValue() const
    {
      return m_value;
    }

    bool ModelObjectActionSetAttribute_Impl::apply(openstudio::model::ModelObject& modelObject) const
    {
      bool result = false;

      switch(m_attributeValueType.value()){
        case AttributeValueType::Boolean:
          result = modelObject.setAttribute(m_attributeName, m_value.toBool());
          break;
        case AttributeValueType::Unsigned:
          result = modelObject.setAttribute(m_attributeName, m_value.toUInt());
          break;
        case AttributeValueType::Integer:
          result = modelObject.setAttribute(m_attributeName, m_value.toInt());
          break;
        case AttributeValueType::Double:
          result = modelObject.setAttribute(m_attributeName, m_value.toDouble());
          break;
        case AttributeValueType::String:
          result = modelObject.setAttribute(m_attributeName, m_value.value<std::string>());
          break;
        default:
          OS_ASSERT(false);
      }

      LOG(Debug, "Setting attribute '" << this->attributeName() << "' for ModelObject of type '"
        << modelObject.iddObject().name() << (result ? "' succeeded" : " failed"));

      return result;
    }

    std::string ModelObjectActionSetAttribute_Impl::xmlElementName() const
    {
      return ModelObjectActionSetAttribute::xmlElementName();
    }

    void ModelObjectActionSetAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
    {
      ModelObjectActionClause_Impl::writeValues(doc, element);

      QDomElement childElement;
      QDomText text;

      childElement = doc.createElement(QString::fromStdString("AttributeName"));
      text = doc.createTextNode(toQString(m_attributeName));
      childElement.appendChild(text);
      element.appendChild(childElement);

      childElement = doc.createElement(QString::fromStdString("AttributeValueType"));
      text = doc.createTextNode(toQString(m_attributeValueType.valueName()));
      childElement.appendChild(text);
      element.appendChild(childElement);

      childElement = doc.createElement(QString::fromStdString("Value"));
      switch(m_attributeValueType.value()){
        case AttributeValueType::Boolean:
          text = doc.createTextNode(m_value.toBool() ? QString::fromStdString("true") : QString::fromStdString("false"));
          childElement.appendChild(text);
          break;
        case AttributeValueType::Integer:
          text = doc.createTextNode(QString::number(m_value.toInt()));
          childElement.appendChild(text);
          break;
        case AttributeValueType::Unsigned:
          text = doc.createTextNode(QString::number(m_value.toUInt()));
          childElement.appendChild(text);
          break;
        case AttributeValueType::Double:
          text = doc.createTextNode(QString::number(m_value.toDouble()));
          childElement.appendChild(text);
          break;
        case AttributeValueType::String:
          text = doc.createTextNode(toQString(m_value.value<std::string>()));
          childElement.appendChild(text);
          break;
        case AttributeValueType::AttributeVector:
          OS_ASSERT(false);
          break;
        default:
          OS_ASSERT(false);
          break;
      }
      element.appendChild(childElement);
    }

     bool ModelObjectActionSetAttribute_Impl::equivalentData(const RulesetObject& other) const {
      bool result = true;
      result = result && ModelObjectActionClause_Impl::equivalentData(other);
      return result;
     }

  } // detail

  std::string ModelObjectActionSetAttribute::xmlElementName()
  {
    return "ModelObjectActionSetAttribute";
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, bool value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, bool value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, unsigned value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, unsigned value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, int value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, int value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, double value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, double value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, const std::string& value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, const std::string& value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, const char* value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, const char* value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, const openstudio::AttributeValueType& attributeValueType, const QVariant& value)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, attributeValueType, value)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const std::string& attributeName, const openstudio::AttributeValueType& attributeValueType, const QVariant& value,
                                                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(attributeName, attributeValueType, value, uuid, versionUUID)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const QDomElement& element)
    : ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>(new detail::ModelObjectActionSetAttribute_Impl(element)))
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }

  /// @cond
  ModelObjectActionSetAttribute::ModelObjectActionSetAttribute(const boost::shared_ptr<detail::ModelObjectActionSetAttribute_Impl>& impl)
    : ModelObjectActionClause(impl)
  {
    OS_ASSERT(getImpl<detail::ModelObjectActionSetAttribute_Impl>());
  }
  /// @endcond

  std::string ModelObjectActionSetAttribute::attributeName() const
  {
    return getImpl<detail::ModelObjectActionSetAttribute_Impl>()->attributeName();
  }

  AttributeValueType ModelObjectActionSetAttribute::attributeValueType() const
  {
    return getImpl<detail::ModelObjectActionSetAttribute_Impl>()->attributeValueType();
  }

  QVariant ModelObjectActionSetAttribute::attributeValue() const
  {
    return getImpl<detail::ModelObjectActionSetAttribute_Impl>()->attributeValue();
  }

} // ruleset
} // openstudio
