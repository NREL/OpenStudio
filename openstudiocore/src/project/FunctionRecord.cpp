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

#include <project/FunctionRecord.hpp>
#include <project/FunctionRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
#include <project/LinearFunctionRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/VariableRecord.hpp>

#include <analysis/LinearFunction.hpp>
#include <analysis/LinearFunction_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  FunctionRecord_Impl::FunctionRecord_Impl(const analysis::Function& function,
                                           const FunctionRecordType& functionRecordType,
                                           ProblemRecord& problemRecord,
                                           const FunctionType& functionType,
                                           int functionVectorIndex)
    : ObjectRecord_Impl(problemRecord.projectDatabase(),
                        function.uuid(),
                        function.name(),
                        function.displayName(),
                        function.description(),
                        function.versionUUID()),
      m_problemRecordId(problemRecord.id()),
      m_functionType(functionType),
      m_functionVectorIndex(functionVectorIndex),
      m_functionRecordType(functionRecordType)
  {}

  FunctionRecord_Impl::FunctionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(FunctionRecord::ColumnsType::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_problemRecordId = value.toInt();

    value = query.value(FunctionRecord::ColumnsType::functionType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_functionType = FunctionType(value.toInt());

    value = query.value(FunctionRecord::ColumnsType::functionVectorIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_functionVectorIndex = value.toInt();

    value = query.value(FunctionRecord::ColumnsType::functionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_functionRecordType = FunctionRecordType(value.toInt());
  }

  std::string FunctionRecord_Impl::databaseTableName() const {
    return FunctionRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> FunctionRecord_Impl::parent() const {
    return problemRecord();
  }

  std::vector<ObjectRecord> FunctionRecord_Impl::children() const {
    ObjectRecordVector result;
    VariableRecordVector variableRecords = this->variableRecords();
    result.insert(result.end(),variableRecords.begin(),variableRecords.end());
    return result;
  }

  std::vector<ObjectRecord> FunctionRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> FunctionRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void FunctionRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<FunctionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ProblemRecord FunctionRecord_Impl::problemRecord() const {
    ProjectDatabase database = projectDatabase();
    return ProblemRecord::getProblemRecord(m_problemRecordId,database).get();
  }

  std::vector<VariableRecord> FunctionRecord_Impl::variableRecords() const {
    VariableRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
        " WHERE functionRecordId=:functionRecordId ORDER BY variableVectorIndex"));
    query.bindValue(":functionRecordId",id());
    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()) {
      result.push_back(VariableRecord::factoryFromQuery(query, database).get());
      int index = result.back().variableVectorIndex();
      if (previousIndex) {
        if (index <= previousIndex.get()) {
          resort = true; // if variables are changed around, order in database itself can
                         // get out of date (as compared to database + dirty records).
        }
      }
      previousIndex = index;
    }

    if (resort) {
      VariableRecordVariableVectorIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  FunctionType FunctionRecord_Impl::functionType() const {
    return m_functionType;
  }

  int FunctionRecord_Impl::functionVectorIndex() const {
    return m_functionVectorIndex;
  }

  void FunctionRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(FunctionRecord::ColumnsType::problemRecordId,m_problemRecordId);
    query.bindValue(FunctionRecord::ColumnsType::functionType, m_functionType.value());
    query.bindValue(FunctionRecord::ColumnsType::functionVectorIndex, m_functionVectorIndex);
    query.bindValue(FunctionRecord::ColumnsType::functionRecordType, m_functionRecordType.value());
  }

  void FunctionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(FunctionRecord::ColumnsType::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastProblemRecordId = value.toInt();

    value = query.value(FunctionRecord::ColumnsType::functionType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastFunctionType = FunctionType(value.toInt());

    value = query.value(FunctionRecord::ColumnsType::functionVectorIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastFunctionVectorIndex = value.toInt();

    value = query.value(FunctionRecord::ColumnsType::functionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastFunctionRecordType = FunctionRecordType(value.toInt());
  }

  bool FunctionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(FunctionRecord::ColumnsType::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_problemRecordId == value.toInt());

    value = query.value(FunctionRecord::ColumnsType::functionType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_functionType == FunctionType(value.toInt()));

    value = query.value(FunctionRecord::ColumnsType::functionVectorIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_functionVectorIndex == value.toInt());

    value = query.value(FunctionRecord::ColumnsType::functionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_functionRecordType == FunctionRecordType(value.toInt()));

    return result;
  }

  void FunctionRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastProblemRecordId = m_problemRecordId;
    m_lastFunctionType = m_functionType;
    m_lastFunctionVectorIndex = m_functionVectorIndex;
    m_lastFunctionRecordType = m_functionRecordType;
  }

  void FunctionRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_problemRecordId = m_lastProblemRecordId;
    m_functionType = m_lastFunctionType;
    m_functionVectorIndex = m_lastFunctionVectorIndex;
    m_functionRecordType = m_lastFunctionRecordType;
  }

} // detail

FunctionRecord::FunctionRecord(boost::shared_ptr<detail::FunctionRecord_Impl> impl,
                               ProjectDatabase database)
  : ObjectRecord(impl,database)
{
  OS_ASSERT(getImpl<detail::FunctionRecord_Impl>());
}

std::string FunctionRecord::databaseTableName() {
  return "FunctionRecords";
}

UpdateByIdQueryData FunctionRecord::updateByIdQueryData() {
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
      OS_ASSERT(*it == expectedValue);
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

void FunctionRecord::updatePathData(ProjectDatabase database,
                                    const openstudio::path& originalBase,
                                    const openstudio::path& newBase)
{}

boost::optional<FunctionRecord> FunctionRecord::factoryFromQuery(const QSqlQuery& query,
                                                                 ProjectDatabase& database)
{
  OptionalFunctionRecord result;

  int functionRecordType = query.value(FunctionRecordColumns::functionRecordType).toInt();

  switch (functionRecordType) {
    case FunctionRecordType::LinearFunctionRecord :
      result = LinearFunctionRecord(query, database).cast<FunctionRecord>();
     break;
    default :
      LOG(Error,"Unknown FunctionRecordType " << functionRecordType);
      return boost::none;
  }

  return result;
}

FunctionRecord FunctionRecord::factoryFromFunction(const analysis::Function& function,
                                                   ProblemRecord& problemRecord,
                                                   const FunctionType& functionType,
                                                   int functionVectorIndex)
{
  if (function.optionalCast<analysis::LinearFunction>()) {
    return LinearFunctionRecord(function.cast<analysis::LinearFunction>(),
                                problemRecord,
                                functionType,
                                functionVectorIndex);
  }

  OS_ASSERT(false);
  return FunctionRecord(boost::shared_ptr<detail::FunctionRecord_Impl>());
}

std::vector<FunctionRecord> FunctionRecord::getFunctionRecords(ProjectDatabase& database) {
  std::vector<FunctionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FunctionRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    OptionalFunctionRecord functionRecord = FunctionRecord::factoryFromQuery(query, database);
    if (functionRecord) {
      result.push_back(*functionRecord);
    }
  }

  return result;
}

boost::optional<FunctionRecord> FunctionRecord::getFunctionRecord(int id, ProjectDatabase& database) {
  boost::optional<FunctionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FunctionRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = FunctionRecord::factoryFromQuery(query, database);
  }

  return result;
}

ProblemRecord FunctionRecord::problemRecord() const {
  return getImpl<detail::FunctionRecord_Impl>()->problemRecord();
}

std::vector<VariableRecord> FunctionRecord::variableRecords() const {
  return getImpl<detail::FunctionRecord_Impl>()->variableRecords();
}

FunctionType FunctionRecord::functionType() const {
  return getImpl<detail::FunctionRecord_Impl>()->functionType();
}

int FunctionRecord::functionVectorIndex() const {
  return getImpl<detail::FunctionRecord_Impl>()->functionVectorIndex();
}

analysis::Function FunctionRecord::function() const {
  return getImpl<detail::FunctionRecord_Impl>()->function();
}

/// @cond
FunctionRecord::FunctionRecord(boost::shared_ptr<detail::FunctionRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

bool FunctionRecordFunctionVectorIndexLess::operator()(const FunctionRecord& left,
                                                       const FunctionRecord& right) const
{
  return (left.functionVectorIndex() < right.functionVectorIndex());
}


} // project
} // openstudio

