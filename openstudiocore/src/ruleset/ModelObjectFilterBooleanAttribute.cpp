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

#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute_Impl.hpp>

#include <model/ModelObject.hpp>

#include <utilities/data/Attribute.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterBooleanAttribute_Impl::ModelObjectFilterBooleanAttribute_Impl(const std::string& attributeName, bool testValue)
    : ModelObjectFilterAttribute_Impl(attributeName), m_testValue(testValue)
  {}

  ModelObjectFilterBooleanAttribute_Impl::ModelObjectFilterBooleanAttribute_Impl(const std::string& attributeName, bool testValue,
                                            const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterAttribute_Impl(attributeName, uuid, versionUUID), m_testValue(testValue)
  {}

  ModelObjectFilterBooleanAttribute_Impl::ModelObjectFilterBooleanAttribute_Impl(const QDomElement& element)
    : ModelObjectFilterAttribute_Impl(element)
  {
    OS_ASSERT(!element.isNull());
    OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

    QDomElement testValueElement = element.firstChildElement(QString::fromStdString("TestValue"));

    OS_ASSERT(!testValueElement.isNull());

    if (testValueElement.firstChild().nodeValue() == "true"){
      m_testValue = true;
    }else{
      m_testValue = false;
    }
  }

  bool ModelObjectFilterBooleanAttribute_Impl::testValue() const
  {
    return m_testValue;
  }

  std::string ModelObjectFilterBooleanAttribute_Impl::xmlElementName() const
  {
    return ModelObjectFilterBooleanAttribute::xmlElementName();
  }

  void ModelObjectFilterBooleanAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ModelObjectFilterAttribute_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("TestValue"));
    text = doc.createTextNode(m_testValue ? QString::fromStdString("true") : QString::fromStdString("false"));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool ModelObjectFilterBooleanAttribute_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ModelObjectFilterAttribute_Impl::equivalentData(other);
    return result;
  }

  bool ModelObjectFilterBooleanAttribute_Impl::check(model::ModelObject& modelObject) const
  {
    bool result = false;

    boost::optional<openstudio::Attribute> attribute = modelObject.getAttribute(this->attributeName());

    LOG(Debug, "Retrieval of attribute '" << this->attributeName() << "' from ModelObject of type '"
      << modelObject.iddObject().name() << (attribute ? "' succeeded" : "' failed"));

    if (attribute && attribute->valueType() == openstudio::AttributeValueType::Boolean){

      LOG(Debug, "Test value is '" << m_testValue << "' and ModelObject value is '" << attribute->valueAsBoolean() << "'");

      result = (m_testValue == attribute->valueAsBoolean());
    }

    return result;
  }

} // detail

std::string ModelObjectFilterBooleanAttribute::xmlElementName()
{
  return "ModelObjectFilterBooleanAttribute";
}

ModelObjectFilterBooleanAttribute::ModelObjectFilterBooleanAttribute(const std::string& attributeName, bool testValue)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterBooleanAttribute_Impl>(new detail::ModelObjectFilterBooleanAttribute_Impl(attributeName, testValue)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttribute_Impl>());
}

ModelObjectFilterBooleanAttribute::ModelObjectFilterBooleanAttribute(const std::string& attributeName, bool testValue,
                                                                     const UUID& uuid, const UUID& versionUUID)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterBooleanAttribute_Impl>(new detail::ModelObjectFilterBooleanAttribute_Impl(attributeName, testValue, uuid, versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttribute_Impl>());
}

ModelObjectFilterBooleanAttribute::ModelObjectFilterBooleanAttribute(const QDomElement& element)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterBooleanAttribute_Impl>(new detail::ModelObjectFilterBooleanAttribute_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttribute_Impl>());
}

/// @cond
ModelObjectFilterBooleanAttribute::ModelObjectFilterBooleanAttribute(const boost::shared_ptr<detail::ModelObjectFilterBooleanAttribute_Impl>& impl)
  : ModelObjectFilterAttribute(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttribute_Impl>());
}
/// @endcond

bool ModelObjectFilterBooleanAttribute::testValue() const
{
  return getImpl<detail::ModelObjectFilterBooleanAttribute_Impl>()->testValue();
}

} // ruleset
} // openstudio
