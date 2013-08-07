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

#include <ruleset/Clause.hpp>
#include <ruleset/Clause_Impl.hpp>
#include <ruleset/FilterClause.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterRelationship.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectActionSetAttribute.hpp>
#include <ruleset/ModelObjectActionSetRelationship.hpp>

#include <utilities/core/Checksum.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>

namespace openstudio {
namespace ruleset {

namespace detail {

  Clause_Impl::Clause_Impl()
    : RulesetObject_Impl()
  {}

  Clause_Impl::Clause_Impl(const UUID& uuid, const UUID& versionUUID)
    : RulesetObject_Impl(uuid, versionUUID)
  {}

  Clause_Impl::Clause_Impl(const QDomElement& element)
    : RulesetObject_Impl(element)
  {
  }

  void Clause_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    RulesetObject_Impl::writeValues(doc, element);
  }

  bool Clause_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && RulesetObject_Impl::equivalentData(other);
    return result;
  }

} // detail

boost::optional<Clause> Clause::factoryFromXml(const QDomElement& element)
{
  boost::optional<Clause> result;

  std::string xmlElementName = toString(element.tagName());
  if (xmlElementName == ModelObjectFilterBooleanAttribute::xmlElementName()){
    result = ModelObjectFilterBooleanAttribute(element);
  }else if (xmlElementName == ModelObjectFilterNumericAttribute::xmlElementName()){
    result = ModelObjectFilterNumericAttribute(element);
  }else if (xmlElementName == ModelObjectFilterStringAttribute::xmlElementName()){
    result = ModelObjectFilterStringAttribute(element);
  }else if (xmlElementName == ModelObjectFilterRelationship::xmlElementName()) {
    result = ModelObjectFilterRelationship(element);
  }else if (xmlElementName == ModelObjectFilterType::xmlElementName()){
    result = ModelObjectFilterType(element);
  }else if (xmlElementName == ModelObjectActionSetAttribute::xmlElementName()){
    result = ModelObjectActionSetAttribute(element);
  }else if (xmlElementName == ModelObjectActionSetRelationship::xmlElementName()){
    result = ModelObjectActionSetRelationship(element);
  }

  return result;
}

/// @cond
Clause::Clause(boost::shared_ptr<detail::Clause_Impl> impl)
  : RulesetObject(impl)
{
  OS_ASSERT(getImpl<detail::Clause_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
