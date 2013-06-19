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

#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <iomanip>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterAvailableModel_Impl::StandardsFilterAvailableModel_Impl(const std::string& id)
    : StandardsFilterClause_Impl(), m_id(id)
  {}

  StandardsFilterAvailableModel_Impl::StandardsFilterAvailableModel_Impl(
      const std::string& id,const UUID& uuid, const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_id(id)
  {}

  StandardsFilterAvailableModel_Impl::StandardsFilterAvailableModel_Impl(
      const StandardsRulesetModelType& modelType)
    : StandardsFilterClause_Impl(), m_modelType(modelType)
  {}

  StandardsFilterAvailableModel_Impl::StandardsFilterAvailableModel_Impl(
      const StandardsRulesetModelType& modelType,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_modelType(modelType)
  {}

  StandardsFilterAvailableModel_Impl::StandardsFilterAvailableModel_Impl(const QDomElement& element)
    : StandardsFilterClause_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsFilterAvailableModel_Impl::clone() const {
    boost::shared_ptr<StandardsFilterAvailableModel_Impl> impl(new StandardsFilterAvailableModel_Impl(*this));
    return StandardsFilterAvailableModel(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterAvailableModel_Impl::deepClone() const {
    StandardsFilterAvailableModel result = clone().cast<StandardsFilterAvailableModel>();
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterAvailableModel_Impl::factName() const {
    return ("availableModel" + this->printUUID());
  }

  std::string StandardsFilterAvailableModel_Impl::id() const {
    return m_id;
  }

  boost::optional<StandardsRulesetModelType> StandardsFilterAvailableModel_Impl::modelType() const {
    return m_modelType;
  }

  std::string StandardsFilterAvailableModel_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- (AvailableModel ";
    unsigned indentSize(ss.str().length());
    ss << "(name ?" << factName() << "_Name)" << std::endl;
    if (m_id.empty()) {
      ss << std::setw(indentSize) << " " << "(id ?" << factName() << "_Id)" << std::endl;
    }
    else {
      ss << std::setw(indentSize) << " " << "(id " << m_id << ")" << std::endl;
    }
    ss << std::setw(indentSize) << " " << "(modelType ?" << factName() << "_ModelType";
    if (m_modelType) {
      ss << "&" << m_modelType->valueName();
    }
    ss << "))" << std::endl;
    return ss.str();
  }

  std::string StandardsFilterAvailableModel_Impl::xmlElementName() const
  {
    return StandardsFilterAvailableModel::xmlElementName();
  }

  void StandardsFilterAvailableModel_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterClause_Impl::writeValues(doc, element);
  }

  bool StandardsFilterAvailableModel_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsFilterAvailableModel oAvailableModel = other.optionalCast<StandardsFilterAvailableModel>();
    if (oAvailableModel) {
      // ids both empty or both specified (from AvailableObject)
      if ((m_id.empty() && oAvailableModel->id().empty()) ||
          (!m_id.empty() && !(oAvailableModel->id().empty())))
      {
        // retractOnMatch, and same modelType specification
        return ((!m_modelType && !oAvailableModel->modelType()) ||
                (m_modelType && oAvailableModel->modelType() &&
                 (*m_modelType == *(oAvailableModel->modelType()))));
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsFilterAvailableModel::xmlElementName()
{
  return "StandardsFilterAvailableModel";
}
/// @endcond

StandardsFilterAvailableModel::StandardsFilterAvailableModel(const std::string& id)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl>(new
                     detail::StandardsFilterAvailableModel_Impl(id)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModel_Impl>());
}

StandardsFilterAvailableModel::StandardsFilterAvailableModel(const std::string& id,
                                 const UUID& uuid, const UUID& versionUUID)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl>(new
                     detail::StandardsFilterAvailableModel_Impl(id, uuid, versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModel_Impl>());
}

StandardsFilterAvailableModel::StandardsFilterAvailableModel(const StandardsRulesetModelType& modelType)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl>(new
                     detail::StandardsFilterAvailableModel_Impl(modelType)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModel_Impl>());
}

StandardsFilterAvailableModel::StandardsFilterAvailableModel(const StandardsRulesetModelType& modelType,
                                 const UUID& uuid, const UUID& versionUUID)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl>(new
                     detail::StandardsFilterAvailableModel_Impl(modelType, uuid, versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModel_Impl>());
}

StandardsFilterAvailableModel::StandardsFilterAvailableModel(const QDomElement& element)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl>(new
                     detail::StandardsFilterAvailableModel_Impl(element)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModel_Impl>());
}
std::string StandardsFilterAvailableModel::id() const {
  return getImpl<detail::StandardsFilterAvailableModel_Impl>()->id();
}

boost::optional<StandardsRulesetModelType> StandardsFilterAvailableModel::modelType() const {
  return getImpl<detail::StandardsFilterAvailableModel_Impl>()->modelType();
}

/// @cond
StandardsFilterAvailableModel::StandardsFilterAvailableModel(
    boost::shared_ptr<detail::StandardsFilterAvailableModel_Impl> impl)
  : StandardsFilterClause(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableModel_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
