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

#include <project/DiscretePerturbationRecord.hpp>
#include <project/DiscretePerturbationRecord_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/RubyPerturbationRecord.hpp>
#include <project/RubyPerturbationRecord_Impl.hpp>
#include <project/NullPerturbationRecord.hpp>
#include <project/NullPerturbationRecord_Impl.hpp>
#include <project/ModelRulesetPerturbationRecord.hpp>
#include <project/ModelRulesetPerturbationRecord_Impl.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/JoinRecord.hpp>

#include <analysis/DiscretePerturbation.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/NullPerturbation_Impl.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/ModelRulesetPerturbation.hpp>
#include <analysis/ModelRulesetPerturbation_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail{

  DiscretePerturbationRecord_Impl::DiscretePerturbationRecord_Impl(const analysis::DiscretePerturbation& discretePerturbation,
                                                                   const DiscretePerturbationRecordType& discretePerturbationRecordType,
                                                                   const DiscreteVariableRecord& discreteVariableRecord,
                                                                   int perturbationVectorIndex)
    : ObjectRecord_Impl(discreteVariableRecord.projectDatabase(),
                        discretePerturbation.uuid(),
                        discretePerturbation.name(),
                        discretePerturbation.displayName(),
                        discretePerturbation.description(),
                        discretePerturbation.versionUUID()),
      m_discretePerturbationRecordType(discretePerturbationRecordType),
      m_variableRecordId(discreteVariableRecord.id()),
      m_isSelected(discretePerturbation.isSelected()),
      m_perturbationVectorIndex(perturbationVectorIndex)
  {}

  DiscretePerturbationRecord_Impl::DiscretePerturbationRecord_Impl(const analysis::DiscretePerturbation& discretePerturbation,
                                                                   const DiscretePerturbationRecordType& discretePerturbationRecordType,
                                                                   ProjectDatabase& database)
    : ObjectRecord_Impl(database,
                        discretePerturbation.uuid(),
                        discretePerturbation.name(),
                        discretePerturbation.displayName(),
                        discretePerturbation.description(),
                        discretePerturbation.versionUUID()),
      m_discretePerturbationRecordType(discretePerturbationRecordType),
      m_isSelected(discretePerturbation.isSelected())
  {}

  DiscretePerturbationRecord_Impl::DiscretePerturbationRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::discretePerturbationRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_discretePerturbationRecordType = DiscretePerturbationRecordType(value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_variableRecordId = value.toInt();
    }

    value = query.value(DiscretePerturbationRecordColumns::isSelected);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_isSelected = value.toBool();

    value = query.value(DiscretePerturbationRecordColumns::perturbationVectorIndex);
    if (value.isValid() && !value.isNull()) {
      m_perturbationVectorIndex = value.toInt();
    }
  }

  std::string DiscretePerturbationRecord_Impl::databaseTableName() const
  {
    return DiscretePerturbationRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> DiscretePerturbationRecord_Impl::parent() const
  {
    OptionalObjectRecord result;
    if (OptionalDiscreteVariableRecord dvr = this->discreteVariableRecord()) {
      result = *dvr;
    }
    return result;
  }

  std::vector<ObjectRecord> DiscretePerturbationRecord_Impl::children() const {
    return ObjectRecordVector();
  }

  std::vector<ObjectRecord> DiscretePerturbationRecord_Impl::resources() const {
    return ObjectRecordVector();
  }

  std::vector<JoinRecord> DiscretePerturbationRecord_Impl::joinRecords() const
  {
    return std::vector<JoinRecord>();
  }

  boost::optional<DiscreteVariableRecord> DiscretePerturbationRecord_Impl::discreteVariableRecord() const
  {
    OptionalDiscreteVariableRecord result;

    if (m_variableRecordId) {
      ProjectDatabase database = this->projectDatabase();
      result = DiscreteVariableRecord::getDiscreteVariableRecord(*m_variableRecordId, database);
      BOOST_ASSERT(result);
    }

    return result;
  }

  bool DiscretePerturbationRecord_Impl::isSelected() const
  {
    return m_isSelected;
  }

  bool DiscretePerturbationRecord_Impl::setIsSelected(bool isSelected)
  {
    if (m_isSelected != isSelected){
      m_isSelected = isSelected;
      this->onChange();
    }
    return true;
  }

  boost::optional<int> DiscretePerturbationRecord_Impl::perturbationVectorIndex() const {
    return m_perturbationVectorIndex;
  }

  void DiscretePerturbationRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(DiscretePerturbationRecordColumns::discretePerturbationRecordType, m_discretePerturbationRecordType.value());
    if (m_variableRecordId) {
      query.bindValue(DiscretePerturbationRecordColumns::variableRecordId, *m_variableRecordId);
    }
    else {
      query.bindValue(DiscretePerturbationRecordColumns::variableRecordId, QVariant(QVariant::Int));
    }
    query.bindValue(DiscretePerturbationRecordColumns::isSelected, m_isSelected);
    if (m_perturbationVectorIndex) {
      query.bindValue(DiscretePerturbationRecordColumns::perturbationVectorIndex, *m_perturbationVectorIndex);
    }
    else {
      query.bindValue(DiscretePerturbationRecordColumns::perturbationVectorIndex, QVariant(QVariant::Int));
    }
  }

  void DiscretePerturbationRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::discretePerturbationRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastDiscretePerturbationRecordType = DiscretePerturbationRecordType(value.toInt());

    value = query.value(DiscretePerturbationRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastVariableRecordId = value.toInt();
    }
    else {
      m_lastVariableRecordId.reset();
    }

    value = query.value(DiscretePerturbationRecordColumns::isSelected);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastIsSelected = value.toBool();

    value = query.value(DiscretePerturbationRecordColumns::perturbationVectorIndex);
    if (value.isValid() && !value.isNull()) {
      m_lastPerturbationVectorIndex = value.toInt();
    }
    else {
      m_lastPerturbationVectorIndex.reset();
    }
  }

  bool DiscretePerturbationRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::discretePerturbationRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_discretePerturbationRecordType == DiscretePerturbationRecordType(value.toInt()));

    value = query.value(DiscretePerturbationRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_variableRecordId && (*m_variableRecordId == value.toInt());
    }
    else {
      result = result && !m_variableRecordId;
    }

    value = query.value(DiscretePerturbationRecordColumns::isSelected);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_isSelected == value.toBool());

    value = query.value(DiscretePerturbationRecordColumns::perturbationVectorIndex);
    if (value.isValid() && !value.isNull()) {
      result = result && m_perturbationVectorIndex && (*m_perturbationVectorIndex == value.toInt());
    }
    else {
      result = result && !m_perturbationVectorIndex;
    }

    return result;
  }

  void DiscretePerturbationRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastDiscretePerturbationRecordType = m_discretePerturbationRecordType;
    m_lastVariableRecordId = m_variableRecordId;
    m_lastIsSelected = m_isSelected;
    m_lastPerturbationVectorIndex = m_perturbationVectorIndex;
  }

  void DiscretePerturbationRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_discretePerturbationRecordType = m_lastDiscretePerturbationRecordType;
    m_variableRecordId = m_lastVariableRecordId;
    m_isSelected = m_lastIsSelected;
    m_perturbationVectorIndex = m_lastPerturbationVectorIndex;
  }

} // detail

std::string DiscretePerturbationRecord::databaseTableName()
{
  return "DiscretePerturbationRecords";
}

UpdateByIdQueryData DiscretePerturbationRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (std::set<int>::const_iterator it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      BOOST_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      std::set<int>::const_iterator nextIt = it;
      ++nextIt;
      if (nextIt == itend) {
        ss << " ";
      }
      else {
        ss << ", ";
      }
      ++expectedValue;
    }
    ss << "WHERE id=:id";
    result.queryString = ss.str();

    // null values
    for (std::set<int>::const_iterator it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(*it), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void DiscretePerturbationRecord::updatePathData(ProjectDatabase database,
                                                const openstudio::path& originalBase,
                                                const openstudio::path& newBase)
{}

DiscretePerturbationRecord::DiscretePerturbationRecord(boost::shared_ptr<detail::DiscretePerturbationRecord_Impl> impl, ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  BOOST_ASSERT(getImpl<detail::DiscretePerturbationRecord_Impl>());
}

DiscretePerturbationRecord::DiscretePerturbationRecord(boost::shared_ptr<detail::DiscretePerturbationRecord_Impl> impl)
  : ObjectRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::DiscretePerturbationRecord_Impl>());
}

std::vector<DiscretePerturbationRecord> DiscretePerturbationRecord::getDiscretePerturbationRecords(ProjectDatabase& database)
{
  std::vector<DiscretePerturbationRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    boost::optional<DiscretePerturbationRecord> perturbation = factoryFromQuery(query, database);
    BOOST_ASSERT(perturbation);
    result.push_back(*perturbation);
  }

  return result;
}

boost::optional<DiscretePerturbationRecord> DiscretePerturbationRecord::getDiscretePerturbationRecord(int id, ProjectDatabase& database)
{
  boost::optional<DiscretePerturbationRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if(query.first()) {
    boost::optional<DiscretePerturbationRecord> perturbation = factoryFromQuery(query, database);
    BOOST_ASSERT(perturbation);
    result = perturbation;
  }

  return result;
}

boost::optional<DiscretePerturbationRecord> DiscretePerturbationRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::shared_ptr<detail::DiscretePerturbationRecord_Impl> impl;

  int discretePerturbationRecordType = query.value(DiscretePerturbationRecordColumns::discretePerturbationRecordType).toInt();

  switch (discretePerturbationRecordType){
    case DiscretePerturbationRecordType::RubyPerturbationRecord:
      impl = boost::shared_ptr<detail::RubyPerturbationRecord_Impl>(new detail::RubyPerturbationRecord_Impl(query, database));
      break;
    case DiscretePerturbationRecordType::NullPerturbationRecord:
      impl = boost::shared_ptr<detail::NullPerturbationRecord_Impl>(new detail::NullPerturbationRecord_Impl(query, database));
      break;
    case DiscretePerturbationRecordType::ModelRulesetPerturbationRecord:
      impl = boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl>(new detail::ModelRulesetPerturbationRecord_Impl(query, database));
      break;
    default:
      LOG(Error, "Unknown discretePerturbationRecordType " << discretePerturbationRecordType);
      return boost::none;
  }

  return DiscretePerturbationRecord(impl, database);
}

DiscretePerturbationRecord DiscretePerturbationRecord::factoryFromDiscretePerturbation(
    const analysis::DiscretePerturbation& discretePerturbation,
    DiscreteVariableRecord& discreteVariableRecord,
    int perturbationVectorIndex)
{
  if (discretePerturbation.optionalCast<analysis::NullPerturbation>()) {
    return NullPerturbationRecord(discretePerturbation.cast<analysis::NullPerturbation>(),
                                  discreteVariableRecord,
                                  perturbationVectorIndex);
  }
  else if (discretePerturbation.optionalCast<analysis::RubyPerturbation>()) {
    return RubyPerturbationRecord(discretePerturbation.cast<analysis::RubyPerturbation>(),
                                  discreteVariableRecord,
                                  perturbationVectorIndex);
  }
  else if (discretePerturbation.optionalCast<analysis::ModelRulesetPerturbation>()) {
    return ModelRulesetPerturbationRecord(
        discretePerturbation.cast<analysis::ModelRulesetPerturbation>(),
        discreteVariableRecord,
        perturbationVectorIndex);
  }

  BOOST_ASSERT(false);
  return DiscretePerturbationRecord(boost::shared_ptr<detail::DiscretePerturbationRecord_Impl>());
}


boost::optional<DiscreteVariableRecord> DiscretePerturbationRecord::discreteVariableRecord() const
{
  return this->getImpl<detail::DiscretePerturbationRecord_Impl>()->discreteVariableRecord();
}

bool DiscretePerturbationRecord::isSelected() const
{
  return this->getImpl<detail::DiscretePerturbationRecord_Impl>()->isSelected();
}

bool DiscretePerturbationRecord::setIsSelected(bool isSelected)
{
  return this->getImpl<detail::DiscretePerturbationRecord_Impl>()->setIsSelected(isSelected);
}

boost::optional<int> DiscretePerturbationRecord::perturbationVectorIndex() const {
  return getImpl<detail::DiscretePerturbationRecord_Impl>()->perturbationVectorIndex();
}

analysis::DiscretePerturbation DiscretePerturbationRecord::discretePerturbation() const {
  return getImpl<detail::DiscretePerturbationRecord_Impl>()->discretePerturbation();
}

bool DiscretePerturbationRecordPerturbationVectorIndexLess::operator()(
    const DiscretePerturbationRecord& left,
    const DiscretePerturbationRecord& right) const
{
  OptionalInt leftIndex = left.perturbationVectorIndex();
  OptionalInt rightIndex = right.perturbationVectorIndex();
  if (leftIndex && rightIndex) {
    return (leftIndex.get() < rightIndex.get());
  }
  return false;
}

} // project
} // openstudio

