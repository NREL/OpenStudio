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

#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute_Impl.hpp>

#include <model/ModelObject.hpp>

#include <utilities/data/Attribute.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const std::string& attributeName, const RulesetNumericalPredicate& predicate, unsigned testValue)
    : ModelObjectFilterAttribute_Impl(attributeName), m_predicate(predicate), m_attributeValueType(openstudio::AttributeValueType::Unsigned), m_testValue(testValue)
  {}

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const std::string& attributeName, const RulesetNumericalPredicate& predicate, unsigned testValue,
                                            const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterAttribute_Impl(attributeName, uuid, versionUUID), m_predicate(predicate), m_attributeValueType(openstudio::AttributeValueType::Unsigned), m_testValue(testValue)
  {}

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const std::string& attributeName, const RulesetNumericalPredicate& predicate, int testValue)
    : ModelObjectFilterAttribute_Impl(attributeName), m_predicate(predicate), m_attributeValueType(openstudio::AttributeValueType::Integer), m_testValue(testValue)
  {}

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const std::string& attributeName, const RulesetNumericalPredicate& predicate, int testValue,
                                            const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterAttribute_Impl(attributeName, uuid, versionUUID), m_predicate(predicate), m_attributeValueType(openstudio::AttributeValueType::Integer), m_testValue(testValue)
  {}

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const std::string& attributeName, const RulesetNumericalPredicate& predicate, double testValue)
    : ModelObjectFilterAttribute_Impl(attributeName), m_predicate(predicate), m_attributeValueType(openstudio::AttributeValueType::Double), m_testValue(testValue)
  {}

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const std::string& attributeName, const RulesetNumericalPredicate& predicate, double testValue,
                                            const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterAttribute_Impl(attributeName, uuid, versionUUID), m_predicate(predicate), m_attributeValueType(openstudio::AttributeValueType::Double), m_testValue(testValue)
  {}

  ModelObjectFilterNumericAttribute_Impl::ModelObjectFilterNumericAttribute_Impl(const QDomElement& element)
    : ModelObjectFilterAttribute_Impl(element)
  {
    OS_ASSERT(!element.isNull());
    OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

    QDomElement predicateElement = element.firstChildElement(QString::fromStdString("Predicate"));
    QDomElement attributeValueTypeElement = element.firstChildElement(QString::fromStdString("AttributeValueType"));
    QDomElement testValueElement = element.firstChildElement(QString::fromStdString("TestValue"));

    OS_ASSERT(!predicateElement.isNull());
    OS_ASSERT(!attributeValueTypeElement.isNull());
    OS_ASSERT(!testValueElement.isNull());

    m_predicate = RulesetNumericalPredicate(predicateElement.firstChild().nodeValue().toStdString());
    m_attributeValueType = AttributeValueType(attributeValueTypeElement.firstChild().nodeValue().toStdString());

    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        OS_ASSERT(false);
        break;
      case AttributeValueType::Integer:
        m_testValue = testValueElement.firstChild().nodeValue().toInt();
        break;
      case AttributeValueType::Unsigned:
        m_testValue = testValueElement.firstChild().nodeValue().toUInt();
        break;
      case AttributeValueType::Double:
        m_testValue = testValueElement.firstChild().nodeValue().toDouble();
        break;
      case AttributeValueType::String:
        OS_ASSERT(false);
        break;
      case AttributeValueType::AttributeVector:
        OS_ASSERT(false);
        break;
      default:
        OS_ASSERT(false);
        break;
      }
  }

  RulesetNumericalPredicate ModelObjectFilterNumericAttribute_Impl::predicate() const
  {
    return m_predicate;
  }

  openstudio::AttributeValueType ModelObjectFilterNumericAttribute_Impl::attributeValueType() const
  {
    return m_attributeValueType;
  }

  QVariant ModelObjectFilterNumericAttribute_Impl::testValue() const
  {
    return m_testValue;
  }

 std::string ModelObjectFilterNumericAttribute_Impl::xmlElementName() const
  {
    return ModelObjectFilterNumericAttribute::xmlElementName();
  }

  void ModelObjectFilterNumericAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ModelObjectFilterAttribute_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("Predicate"));
    text = doc.createTextNode(toQString(m_predicate.valueName()));
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("AttributeValueType"));
    text = doc.createTextNode(toQString(m_attributeValueType.valueName()));
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("TestValue"));
    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        OS_ASSERT(false);
        break;
      case AttributeValueType::Integer:
        text = doc.createTextNode(QString::number(m_testValue.toInt()));
        childElement.appendChild(text);
        break;
      case AttributeValueType::Unsigned:
        text = doc.createTextNode(QString::number(m_testValue.toUInt()));
        childElement.appendChild(text);
        break;
      case AttributeValueType::Double:
        text = doc.createTextNode(QString::number(m_testValue.toDouble()));
        childElement.appendChild(text);
        break;
      case AttributeValueType::String:
        OS_ASSERT(false);
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

  bool ModelObjectFilterNumericAttribute_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ModelObjectFilterAttribute_Impl::equivalentData(other);
    return result;
  }

  bool ModelObjectFilterNumericAttribute_Impl::check(model::ModelObject& modelObject) const
  {
    bool result = false;

    boost::optional<openstudio::Attribute> attribute = modelObject.getAttribute(this->attributeName());

    LOG(Debug, "Retrieval of attribute '" << this->attributeName() << "' from ModelObject of type '"
      << modelObject.iddObject().name() << (attribute ? "' succeeded" : "' failed"));

    if (attribute){

      switch (attribute->valueType().value()){
        case openstudio::AttributeValueType::Unsigned:

          LOG(Debug, "Test value is '" << m_testValue.toUInt() << "', ModelObject value is '" << attribute->valueAsUnsigned()
            << "', and predicate is '" << m_predicate.valueName() << "'");

          switch (m_predicate.value()){
            case RulesetNumericalPredicate::Equals:
              result = (attribute->valueAsUnsigned() == m_testValue.toUInt());
              break;
            case RulesetNumericalPredicate::NotEquals:
              result = (attribute->valueAsUnsigned() != m_testValue.toUInt());
              break;
            case RulesetNumericalPredicate::GreaterThan:
              result = (attribute->valueAsUnsigned() > m_testValue.toUInt());
              break;
            case RulesetNumericalPredicate::GreaterThanOrEqualTo:
              result = (attribute->valueAsUnsigned() >= m_testValue.toUInt());
              break;
            case RulesetNumericalPredicate::LessThan:
              result = (attribute->valueAsUnsigned() < m_testValue.toUInt());
              break;
            case RulesetNumericalPredicate::LessThanOrEqualTo:
              result = (attribute->valueAsUnsigned() <= m_testValue.toUInt());
              break;
            default:
              OS_ASSERT(false);
          }

          break;
        case openstudio::AttributeValueType::Integer:

          LOG(Debug, "Test value is '" << m_testValue.toInt() << "', ModelObject value is '" << attribute->valueAsInteger()
            << "', and predicate is '" << m_predicate.valueName() << "'");

          switch (m_predicate.value()){
            case RulesetNumericalPredicate::Equals:
              result = (attribute->valueAsInteger() == m_testValue.toInt());
              break;
            case RulesetNumericalPredicate::NotEquals:
              result = (attribute->valueAsInteger() != m_testValue.toInt());
              break;
            case RulesetNumericalPredicate::GreaterThan:
              result = (attribute->valueAsInteger() > m_testValue.toInt());
              break;
            case RulesetNumericalPredicate::GreaterThanOrEqualTo:
              result = (attribute->valueAsInteger() >= m_testValue.toInt());
              break;
            case RulesetNumericalPredicate::LessThan:
              result = (attribute->valueAsInteger() < m_testValue.toInt());
              break;
            case RulesetNumericalPredicate::LessThanOrEqualTo:
              result = (attribute->valueAsInteger() <= m_testValue.toInt());
              break;
            default:
              OS_ASSERT(false);
          }

          break;
        case openstudio::AttributeValueType::Double:

          LOG(Debug, "Test value is '" << m_testValue.toDouble() << "', ModelObject value is '" << attribute->valueAsDouble()
            << "', and predicate is '" << m_predicate.valueName() << "'");

          switch (m_predicate.value()){
            case RulesetNumericalPredicate::Equals:
              result = (attribute->valueAsDouble() == m_testValue.toDouble());
              break;
            case RulesetNumericalPredicate::NotEquals:
              result = (attribute->valueAsDouble() != m_testValue.toDouble());
              break;
            case RulesetNumericalPredicate::GreaterThan:
              result = (attribute->valueAsDouble() > m_testValue.toDouble());
              break;
            case RulesetNumericalPredicate::GreaterThanOrEqualTo:
              result = (attribute->valueAsDouble() >= m_testValue.toDouble());
              break;
            case RulesetNumericalPredicate::LessThan:
              result = (attribute->valueAsDouble() < m_testValue.toDouble());
              break;
            case RulesetNumericalPredicate::LessThanOrEqualTo:
              result = (attribute->valueAsDouble() <= m_testValue.toDouble());
              break;
            default:
              OS_ASSERT(false);
          }

          break;
        default:
          OS_ASSERT(false);
      }
    }

    return result;
  }

} // detail

std::string ModelObjectFilterNumericAttribute::xmlElementName()
{
  return "ModelObjectFilterNumericAttribute";
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const std::string& attributeName, const RulesetNumericalPredicate& predicate, unsigned testValue)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(attributeName, predicate, testValue)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const std::string& attributeName, const RulesetNumericalPredicate& predicate, unsigned testValue,
                                                                     const UUID& uuid, const UUID& versionUUID)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(attributeName, predicate, testValue, uuid, versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const std::string& attributeName, const RulesetNumericalPredicate& predicate, int testValue)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(attributeName, predicate, testValue)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const std::string& attributeName, const RulesetNumericalPredicate& predicate, int testValue,
                                                                     const UUID& uuid, const UUID& versionUUID)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(attributeName, predicate, testValue, uuid, versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const std::string& attributeName, const RulesetNumericalPredicate& predicate, double testValue)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(attributeName, predicate, testValue)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const std::string& attributeName, const RulesetNumericalPredicate& predicate, double testValue,
                                                                     const UUID& uuid, const UUID& versionUUID)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(attributeName, predicate, testValue, uuid, versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const QDomElement& element)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>(new detail::ModelObjectFilterNumericAttribute_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}

/// @cond
ModelObjectFilterNumericAttribute::ModelObjectFilterNumericAttribute(const boost::shared_ptr<detail::ModelObjectFilterNumericAttribute_Impl>& impl)
  : ModelObjectFilterAttribute(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttribute_Impl>());
}
/// @endcond

RulesetNumericalPredicate ModelObjectFilterNumericAttribute::predicate() const
{
  return getImpl<detail::ModelObjectFilterNumericAttribute_Impl>()->predicate();
}

openstudio::AttributeValueType ModelObjectFilterNumericAttribute::attributeValueType() const
{
  return getImpl<detail::ModelObjectFilterNumericAttribute_Impl>()->attributeValueType();
}

QVariant ModelObjectFilterNumericAttribute::testValue() const
{
  return getImpl<detail::ModelObjectFilterNumericAttribute_Impl>()->testValue();
}

} // ruleset
} // openstudio
