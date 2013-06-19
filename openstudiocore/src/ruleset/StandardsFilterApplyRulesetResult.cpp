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

#include <ruleset/StandardsFilterApplyRulesetResult.hpp>
#include <ruleset/StandardsFilterApplyRulesetResult_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  // IMPL

  StandardsFilterApplyRulesetResult_Impl::StandardsFilterApplyRulesetResult_Impl(
      const StandardsFilterAvailableModel& model)
    : StandardsFilterClause_Impl(), m_model(model)
  {
    this->connectChild(m_model);
  }

  StandardsFilterApplyRulesetResult_Impl::StandardsFilterApplyRulesetResult_Impl(
      const StandardsFilterAvailableModel& model,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_model(model)
  {
    this->connectChild(m_model);
  }

  StandardsFilterApplyRulesetResult_Impl::StandardsFilterApplyRulesetResult_Impl(const QDomElement& element)
    : StandardsFilterClause_Impl(element),
      m_model(StandardsFilterAvailableModel(element.firstChildElement(QString::fromStdString("Model"))))
  {
    this->connectChild(m_model);
  }

  RulesetObject StandardsFilterApplyRulesetResult_Impl::clone() const {
    boost::shared_ptr<StandardsFilterApplyRulesetResult_Impl> impl(new StandardsFilterApplyRulesetResult_Impl(*this));
    return StandardsFilterApplyRulesetResult(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterApplyRulesetResult_Impl::deepClone() const {
    StandardsFilterApplyRulesetResult result = clone().cast<StandardsFilterApplyRulesetResult>();
    result.mf_setModel(model().deepClone().cast<StandardsFilterAvailableModel>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterApplyRulesetResult_Impl::factName() const {
    return ("applyRulesetResult" + this->printUUID());
  }

  StandardsFilterAvailableModel StandardsFilterApplyRulesetResult_Impl::model() const {
    return m_model;
  }

  void StandardsFilterApplyRulesetResult_Impl::setModel(const StandardsFilterAvailableModel& model) {
    this->disconnectChild(m_model);
    m_model = model;
    this->connectChild(m_model);
  }

  std::string StandardsFilterApplyRulesetResult_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- ";
    ss << "(ApplyRulesetResult ";
    unsigned indentSize(ss.str().length());
    ss << "(modelName ?" << m_model.factName() << "_Name)" << std::endl;
    ss << std::setw(indentSize) << " " << "(result ?" << factName() << "_Result))" << std::endl;
    return ss.str();
  }

  std::string StandardsFilterApplyRulesetResult_Impl::xmlElementName() const
  {
    return StandardsFilterApplyRulesetResult::xmlElementName();
  }

  void StandardsFilterApplyRulesetResult_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterClause_Impl::writeValues(doc, element);
  }

  bool StandardsFilterApplyRulesetResult_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && StandardsFilterClause_Impl::equivalentData(other);
    return result;
  }

  // OBJECT

  std::string StandardsFilterApplyRulesetResult::xmlElementName()
  {
    return "StandardsFilterApplyRulesetResult";
  }

  StandardsFilterApplyRulesetResult::StandardsFilterApplyRulesetResult(const StandardsFilterAvailableModel& model)
    : StandardsFilterClause(boost::shared_ptr<StandardsFilterApplyRulesetResult_Impl>(
                       new StandardsFilterApplyRulesetResult_Impl(model)))
  {}

  StandardsFilterApplyRulesetResult::StandardsFilterApplyRulesetResult(const StandardsFilterAvailableModel& model,
                                       const UUID& uuid, const UUID& versionUUID)
    : StandardsFilterClause(boost::shared_ptr<StandardsFilterApplyRulesetResult_Impl>(
                       new StandardsFilterApplyRulesetResult_Impl(model, uuid, versionUUID)))
  {}

  StandardsFilterAvailableModel StandardsFilterApplyRulesetResult::model() const {
    return getImpl<StandardsFilterApplyRulesetResult_Impl>()->model();
  }

  void StandardsFilterApplyRulesetResult::mf_setModel(const StandardsFilterAvailableModel& model) {
    getImpl<StandardsFilterApplyRulesetResult_Impl>()->setModel(model);
  }

  StandardsFilterApplyRulesetResult::StandardsFilterApplyRulesetResult(
      boost::shared_ptr<StandardsFilterApplyRulesetResult_Impl> impl)
    : StandardsFilterClause(impl)
  {}

} // detail

} // ruleset
} // openstudio
