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

#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterType_Impl.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>

#include <utilities/core/Containers.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterType_Impl::ModelObjectFilterType_Impl(const openstudio::IddObjectType& iddObjectType)
    : ModelObjectFilterClause_Impl(), m_iddObjectType(iddObjectType)
  {}

  ModelObjectFilterType_Impl::ModelObjectFilterType_Impl(const openstudio::IddObjectType& iddObjectType,
                               const UUID& uuid, const UUID& versionUUID)
    : ModelObjectFilterClause_Impl(uuid, versionUUID), m_iddObjectType(iddObjectType)
  {}

  ModelObjectFilterType_Impl::ModelObjectFilterType_Impl(const QDomElement& element)
    : ModelObjectFilterClause_Impl(element)
  {
    OS_ASSERT(!element.isNull());
    OS_ASSERT(element.tagName() == toQString(this->xmlElementName()));

    QDomElement iddObjectTypeElement = element.firstChildElement(QString::fromStdString("IddObjectType"));

    OS_ASSERT(!iddObjectTypeElement.isNull());

    m_iddObjectType = IddObjectType(iddObjectTypeElement.firstChild().nodeValue().toStdString());
  }

  openstudio::IddObjectType ModelObjectFilterType_Impl::iddObjectType() const
  {
    return m_iddObjectType;
  }

  std::string ModelObjectFilterType_Impl::xmlElementName() const
  {
    return ModelObjectFilterType::xmlElementName();
  }

  void ModelObjectFilterType_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ModelObjectFilterClause_Impl::writeValues(doc, element);

    QDomElement childElement;
    QDomText text;

    childElement = doc.createElement(QString::fromStdString("IddObjectType"));
    text = doc.createTextNode(toQString(m_iddObjectType.valueName()));
    childElement.appendChild(text);
    element.appendChild(childElement);
  }

  bool ModelObjectFilterType_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ModelObjectFilterClause_Impl::equivalentData(other);
    return result;
  }

  bool ModelObjectFilterType_Impl::check(model::ModelObject& modelObject) const
  {
    bool result = (m_iddObjectType == modelObject.iddObject().type());
    if (result) {
      LOG(Debug,"Found object of type " << m_iddObjectType.valueDescription() << ".");
    }
    return result;
  }

} // detail

std::string ModelObjectFilterType::xmlElementName()
{
  return "ModelObjectFilterType";
}

ModelObjectFilterType::ModelObjectFilterType(const openstudio::IddObjectType& iddObjectType)
  : ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterType_Impl>(new detail::ModelObjectFilterType_Impl(iddObjectType)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterType_Impl>());
}

ModelObjectFilterType::ModelObjectFilterType(const openstudio::IddObjectType& iddObjectType,
                                             const UUID& uuid, const UUID& versionUUID)
  : ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterType_Impl>(new detail::ModelObjectFilterType_Impl(iddObjectType,uuid,versionUUID)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterType_Impl>());
}

ModelObjectFilterType::ModelObjectFilterType(const QDomElement& element)
  : ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterType_Impl>(new detail::ModelObjectFilterType_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterType_Impl>());
}

/// @cond
ModelObjectFilterType::ModelObjectFilterType(const boost::shared_ptr<detail::ModelObjectFilterType_Impl>& impl)
  : ModelObjectFilterClause(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterType_Impl>());
}
/// @endcond

openstudio::IddObjectType ModelObjectFilterType::iddObjectType() const
{
  return getImpl<detail::ModelObjectFilterType_Impl>()->iddObjectType();
}

std::vector<model::ModelObject> applyTypeFilters(
    const model::Model& model,
    const std::vector<ModelObjectFilterClause>& filters)
{
  model::ModelObjectVector result;
  ModelObjectFilterTypeVector typeFilters = subsetCastVector<ModelObjectFilterType>(filters);
  if (typeFilters.empty()) {
    result = castVector<model::ModelObject>(model.objects());
  }
  else {
    IddObjectType type = typeFilters[0].iddObjectType();
    for (int i = 1, n = typeFilters.size(); i < n; ++i) {
      if (typeFilters[i].iddObjectType() != type) {
        return result;
      }
    }
    result = castVector<model::ModelObject>(model.getObjectsByType(type));
  }
  return result;
}

} // ruleset
} // openstudio
