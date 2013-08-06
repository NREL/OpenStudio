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

#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute_Impl.hpp>

#include <model/ModelObject.hpp>

#include <utilities/data/Attribute.hpp>
#include <utilities/core/Compare.hpp>

#include <boost/regex.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterStringAttribute_Impl::ModelObjectFilterStringAttribute_Impl(const std::string& attributeName, const RulesetStringPredicate& predicate, const std::string& testValue)
    : ModelObjectFilterAttribute_Impl(attributeName), m_predicate(predicate), m_testValue(testValue)
  {}

  ModelObjectFilterStringAttribute_Impl::ModelObjectFilterStringAttribute_Impl(const std::string& attributeName, const RulesetStringPredicate& predicate, const std::string& testValue,
                                           const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterAttribute_Impl(attributeName, uuid, versionUUID), m_predicate(predicate), m_testValue(testValue)
  {}

  ModelObjectFilterStringAttribute_Impl::ModelObjectFilterStringAttribute_Impl(const QDomElement& element)
      : ModelObjectFilterAttribute_Impl(element)
    {
      OS_ASSERT(!element.isNull());
      OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

      QDomElement predicateElement = element.firstChildElement(QString::fromStdString("Predicate"));
      QDomElement testValueElement = element.firstChildElement(QString::fromStdString("TestValue"));

      OS_ASSERT(!predicateElement.isNull());
      OS_ASSERT(!testValueElement.isNull());

      m_predicate = RulesetStringPredicate(predicateElement.firstChild().nodeValue().toStdString());
      m_testValue = testValueElement.firstChild().nodeValue().toStdString();
 }

  RulesetStringPredicate ModelObjectFilterStringAttribute_Impl::predicate() const
  {
    return m_predicate;
  }

  std::string ModelObjectFilterStringAttribute_Impl::testValue() const
  {
    return m_testValue;
  }

  std::string ModelObjectFilterStringAttribute_Impl::xmlElementName() const
  {
    return ModelObjectFilterStringAttribute::xmlElementName();
  }

  void ModelObjectFilterStringAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ModelObjectFilterAttribute_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("Predicate"));
    text = doc.createTextNode(toQString(m_predicate.valueName()));
    childElement.appendChild(text);
    element.appendChild(childElement);

    childElement = doc.createElement(QString::fromStdString("TestValue"));
    text = doc.createTextNode(toQString(m_testValue));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool ModelObjectFilterStringAttribute_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ModelObjectFilterAttribute_Impl::equivalentData(other);
    return result;
  }

  bool ModelObjectFilterStringAttribute_Impl::check(openstudio::model::ModelObject& modelObject) const
  {
    bool result = false;

    boost::optional<openstudio::Attribute> attribute = modelObject.getAttribute(this->attributeName());

    LOG(Debug, "Retrieval of attribute '" << this->attributeName() << "' from ModelObject of type '"
      << modelObject.iddObject().name() << (attribute ? "' succeeded" : "' failed"));

    if (attribute){

      LOG(Debug, "Test value is '" << m_testValue << "', ModelObject value is '" << attribute->valueAsString()
            << "', and predicate is '" << m_predicate.valueName() << "'");

      boost::regex testRegex;

      switch (m_predicate.value()){
        case RulesetStringPredicate::Equals:
          result = (m_testValue == attribute->valueAsString());
          break;
        case RulesetStringPredicate::NotEquals:
          result = (m_testValue != attribute->valueAsString());
          break;
        case RulesetStringPredicate::IEquals:
          result = (openstudio::istringEqual(m_testValue, attribute->valueAsString()));
          break;
        case RulesetStringPredicate::NotIEquals:
          result = (!openstudio::istringEqual(m_testValue, attribute->valueAsString()));
          break;
        case RulesetStringPredicate::Match:
          testRegex = boost::regex(m_testValue);
          result = (boost::regex_match(attribute->valueAsString(), testRegex));
          break;
        case RulesetStringPredicate::NotMatch:
          testRegex = boost::regex(m_testValue);
          result = (!boost::regex_match(attribute->valueAsString(), testRegex));
          break;
        default:
          OS_ASSERT(false);
      }
    }

    return result;
  }

} // detail

std::string ModelObjectFilterStringAttribute::xmlElementName()
{
  return "ModelObjectFilterStringAttribute";
}

ModelObjectFilterStringAttribute::ModelObjectFilterStringAttribute(const std::string& attributeName,const RulesetStringPredicate& predicate, const std::string& testValue)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterStringAttribute_Impl>(new detail::ModelObjectFilterStringAttribute_Impl(attributeName, predicate, testValue)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterStringAttribute_Impl>());
}

ModelObjectFilterStringAttribute::ModelObjectFilterStringAttribute(const std::string& attributeName,const RulesetStringPredicate& predicate, const std::string& testValue,
                                                                   const UUID& uuid, const UUID& versionUUID)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterStringAttribute_Impl>(new detail::ModelObjectFilterStringAttribute_Impl(attributeName, predicate, testValue, uuid, versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterStringAttribute_Impl>());
}

ModelObjectFilterStringAttribute::ModelObjectFilterStringAttribute(const QDomElement& element)
  : ModelObjectFilterAttribute(boost::shared_ptr<detail::ModelObjectFilterStringAttribute_Impl>(new detail::ModelObjectFilterStringAttribute_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterStringAttribute_Impl>());
}

/// @cond
ModelObjectFilterStringAttribute::ModelObjectFilterStringAttribute(const boost::shared_ptr<detail::ModelObjectFilterStringAttribute_Impl>& impl)
  : ModelObjectFilterAttribute(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterStringAttribute_Impl>());
}
/// @endcond

RulesetStringPredicate ModelObjectFilterStringAttribute::predicate() const
{
  return getImpl<detail::ModelObjectFilterStringAttribute_Impl>()->predicate();
}

std::string ModelObjectFilterStringAttribute::testValue() const
{
  return getImpl<detail::ModelObjectFilterStringAttribute_Impl>()->testValue();
}

} // ruleset
} // openstudio
