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

#include <ruleset/ModelObjectFilterAttribute.hpp>
#include <ruleset/ModelObjectFilterAttribute_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterAttribute_Impl::ModelObjectFilterAttribute_Impl(const std::string& attributeName)
    : ModelObjectFilterClause_Impl(), m_attributeName(attributeName)
  {}

  ModelObjectFilterAttribute_Impl::ModelObjectFilterAttribute_Impl(const std::string& attributeName,
                                     const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterClause_Impl(uuid, versionUUID), m_attributeName(attributeName)
  {}

  ModelObjectFilterAttribute_Impl::ModelObjectFilterAttribute_Impl(const QDomElement& element)
    : ModelObjectFilterClause_Impl(element)
  {
    OS_ASSERT(!element.isNull());

    QDomElement attributeNameElement = element.firstChildElement(QString::fromStdString("AttributeName"));

    OS_ASSERT(!attributeNameElement.isNull());

    m_attributeName = attributeNameElement.firstChild().nodeValue().toStdString();
  }

  std::string ModelObjectFilterAttribute_Impl::attributeName() const
  {
    return m_attributeName;
  }

  void ModelObjectFilterAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ModelObjectFilterClause_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("AttributeName"));
    text = doc.createTextNode(toQString(m_attributeName));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool ModelObjectFilterAttribute_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ModelObjectFilterClause_Impl::equivalentData(other);
    return result;
  }

} // detail

/// @cond
ModelObjectFilterAttribute::ModelObjectFilterAttribute(
    boost::shared_ptr<detail::ModelObjectFilterAttribute_Impl> impl)
  : ModelObjectFilterClause(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterAttribute_Impl>());
}
/// @endcond

std::string ModelObjectFilterAttribute::attributeName() const
{
  return getImpl<detail::ModelObjectFilterAttribute_Impl>()->attributeName();
}

} // ruleset
} // openstudio
