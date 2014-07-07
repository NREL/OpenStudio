/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "OSArgumentRecord.hpp"
#include "OSArgumentRecord_Impl.hpp"
#include "RubyMeasureRecord.hpp"
#include "RubyContinuousVariableRecord.hpp"

#include "JoinRecord.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/PathHelpers.hpp"

namespace openstudio {
namespace project {

namespace detail {

  OSArgumentRecord_Impl::OSArgumentRecord_Impl(
      const ruleset::OSArgument& osArgument,
      RubyMeasureRecord& rubyMeasureRecord)
    : ObjectRecord_Impl(rubyMeasureRecord.projectDatabase(),
                        osArgument.uuid(),
                        osArgument.name(),
                        osArgument.displayName(),
                        "",
                        osArgument.versionUUID()),
      m_rubyMeasureRecordId(rubyMeasureRecord.id()),
      m_argumentType(osArgument.type()),
      m_required(osArgument.required()),
      m_domainType(osArgument.domainType()),
      m_isRead(osArgument.isRead()),
      m_extension(osArgument.extension())
  {
    if (osArgument.hasValue()) {
      m_argumentValue = osArgument.valueAsString();
      if (type() == ruleset::OSArgumentType::Path) {
        setValue(valueAsPath()); // calls completeAndNormalize
      }
    }
    if (osArgument.hasDefaultValue()) {
      m_defaultArgumentValue = osArgument.defaultValueAsString();
      if (type() == ruleset::OSArgumentType::Path) {
        setDefaultValue(defaultValueAsPath()); // calls completeAndNormalize
      }
    }
    if (osArgument.hasDomain()) {
      m_domain = stringVectorToString(osArgument.domainAsString());
    }
    m_choices = stringVectorToString(osArgument.choiceValues());
    m_choiceDisplayNames = stringVectorToString(osArgument.choiceValueDisplayNames());
  }

  OSArgumentRecord_Impl::OSArgumentRecord_Impl(
      const ruleset::OSArgument& osArgument,
      RubyContinuousVariableRecord& rubyContinuousVariableRecord)
    : ObjectRecord_Impl(rubyContinuousVariableRecord.projectDatabase(),
                        osArgument.uuid(),
                        osArgument.name(),
                        osArgument.displayName(),
                        "",
                        osArgument.versionUUID()),
      m_rubyContinuousVariableRecordId(rubyContinuousVariableRecord.id()),
      m_argumentType(osArgument.type()),
      m_required(osArgument.required()),
      m_domainType(osArgument.domainType()),
      m_isRead(osArgument.isRead()),
      m_extension(osArgument.extension())
  {
    if (osArgument.hasValue()) {
      m_argumentValue = osArgument.valueAsString();
      if (type() == ruleset::OSArgumentType::Path) {
        setValue(valueAsPath()); // calls completeAndNormalize
      }
    }
    if (osArgument.hasDefaultValue()) {
      m_defaultArgumentValue = osArgument.defaultValueAsString();
      if (type() == ruleset::OSArgumentType::Path) {
        setDefaultValue(defaultValueAsPath()); // calls completeAndNormalize
      }
    }
    if (osArgument.hasDomain()) {
      m_domain = stringVectorToString(osArgument.domainAsString());
    }
    m_choices = stringVectorToString(osArgument.choiceValues());
    m_choiceDisplayNames = stringVectorToString(osArgument.choiceValueDisplayNames());
  }

  OSArgumentRecord_Impl::OSArgumentRecord_Impl(const QSqlQuery& query,
                                               ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(OSArgumentRecord::ColumnsType::rubyPerturbationRecordId);
    if (value.isValid() && !value.isNull()) {
      m_rubyMeasureRecordId = value.toInt();
    }

    value = query.value(OSArgumentRecord::ColumnsType::rubyContinuousVariableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_rubyContinuousVariableRecordId = value.toInt();
    }

    value = query.value(OSArgumentRecord::ColumnsType::argumentType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_argumentType = ruleset::OSArgumentType(value.toInt());

    value = query.value(OSArgumentRecord::ColumnsType::required);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_required = value.toBool();

    value = query.value(OSArgumentRecord::ColumnsType::argumentValue);
    if (value.isValid() && !value.isNull()) {
      m_argumentValue = value.toString().toStdString();
    }

    value = query.value(OSArgumentRecord::ColumnsType::defaultArgumentValue);
    if (value.isValid() && !value.isNull()) {
      m_defaultArgumentValue = value.toString().toStdString();
    }

    value = query.value(OSArgumentRecord::ColumnsType::domainType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_domainType = ruleset::OSDomainType(value.toInt());

    value = query.value(OSArgumentRecord::ColumnsType::domainValues);
    if (value.isValid() && !value.isNull()) {
      m_domain = value.toString().toStdString();
    }

    value = query.value(OSArgumentRecord::ColumnsType::choices);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_choices = value.toString().toStdString();

    value = query.value(OSArgumentRecord::ColumnsType::choiceDisplayNames);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_choiceDisplayNames = value.toString().toStdString();

    value = query.value(OSArgumentRecord::ColumnsType::isRead);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_isRead = value.toBool();

    value = query.value(OSArgumentRecord::ColumnsType::extension);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_extension = value.toString().toStdString();
  }

  std::string OSArgumentRecord_Impl::databaseTableName() const {
    return OSArgumentRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> OSArgumentRecord_Impl::parent() const {
    OptionalObjectRecord result;
    if (m_rubyMeasureRecordId) {
      result = rubyMeasureRecord().get();
    }
    else {
      OS_ASSERT(m_rubyContinuousVariableRecordId);
      result = rubyContinuousVariableRecord().get();
    }
    return result;
  }

  std::vector<ObjectRecord> OSArgumentRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> OSArgumentRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> OSArgumentRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void OSArgumentRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<OSArgumentRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void OSArgumentRecord_Impl::updatePathData(const openstudio::path& originalBase,
                                             const openstudio::path& newBase)
  {
    if (type() == ruleset::OSArgumentType::Path) {
      openstudio::path temp;
      if (hasValue()) {
        temp = relocatePath(valueAsPath(),originalBase,newBase);
        if (!temp.empty()) {
          setValue(temp);
        }
      }
      if (hasDefaultValue()) {
        temp = relocatePath(defaultValueAsPath(),originalBase,newBase);
        if (!temp.empty()) {
          setDefaultValue(temp);
        }
      }
    }
  }

  boost::optional<RubyMeasureRecord> OSArgumentRecord_Impl::rubyMeasureRecord() const {
    OptionalRubyMeasureRecord result;
    if (m_rubyMeasureRecordId) {
      ProjectDatabase database = projectDatabase();
      result = RubyMeasureRecord::getRubyMeasureRecord(*m_rubyMeasureRecordId,database);
      OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<RubyContinuousVariableRecord> OSArgumentRecord_Impl::rubyContinuousVariableRecord() const
  {
    OptionalRubyContinuousVariableRecord result;
    if (m_rubyContinuousVariableRecordId) {
      ProjectDatabase database = projectDatabase();
      result = RubyContinuousVariableRecord::getRubyContinuousVariableRecord(*m_rubyContinuousVariableRecordId,database);
      OS_ASSERT(result);
    }
    return result;
  }

  ruleset::OSArgument OSArgumentRecord_Impl::osArgument() const {
    StringVector domain;
    if (m_domain) {
      domain = stringToStringVector(*m_domain);
    }
    return ruleset::OSArgument(handle(),
                               uuidLast(),
                               name(),
                               displayName(),
                               m_argumentType,
                               m_required,
                               m_argumentValue,
                               m_defaultArgumentValue,
                               m_domainType,
                               domain,
                               stringToStringVector(m_choices),
                               stringToStringVector(m_choiceDisplayNames),
                               m_isRead,
                               m_extension);
  }

  ruleset::OSArgumentType OSArgumentRecord_Impl::type() const {
    return m_argumentType;
  }

  bool OSArgumentRecord_Impl::hasValue() const {
    return (m_argumentValue);
  }

  bool OSArgumentRecord_Impl::hasDefaultValue() const {
    return (m_defaultArgumentValue);
  }

  openstudio::path OSArgumentRecord_Impl::valueAsPath() const {
    return toPath(m_argumentValue.get());
  }

  void OSArgumentRecord_Impl::setValue(const openstudio::path& value) {
    if (type() == ruleset::OSArgumentType::Path) {
      m_argumentValue = toString(completeAndNormalize(value));
      onChange();
    }
  }

  openstudio::path OSArgumentRecord_Impl::defaultValueAsPath() const {
    return toPath(m_defaultArgumentValue.get());
  }

  void OSArgumentRecord_Impl::setDefaultValue(const openstudio::path& defaultValue) {
    if (type() == ruleset::OSArgumentType::Path) {
      m_defaultArgumentValue = toString(completeAndNormalize(defaultValue));
      onChange();
    }
  }

  void OSArgumentRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    if (m_rubyMeasureRecordId) {
      query.bindValue(OSArgumentRecord::ColumnsType::rubyPerturbationRecordId,
                      *m_rubyMeasureRecordId);
    }
    else {
      query.bindValue(OSArgumentRecord::ColumnsType::rubyPerturbationRecordId,
                      QVariant(QVariant::Int));
    }

    if (m_rubyContinuousVariableRecordId) {
      query.bindValue(OSArgumentRecordColumns::rubyContinuousVariableRecordId,
                      *m_rubyContinuousVariableRecordId);
    }
    else {
      query.bindValue(OSArgumentRecordColumns::rubyContinuousVariableRecordId,
                      QVariant(QVariant::Int));
    }

    query.bindValue(OSArgumentRecordColumns::argumentType,m_argumentType.value());
    query.bindValue(OSArgumentRecordColumns::required,m_required);

    if (m_argumentValue) {
      query.bindValue(OSArgumentRecordColumns::argumentValue,
                      toQString(*m_argumentValue));
    }
    else {
      query.bindValue(OSArgumentRecordColumns::argumentValue,
                      QVariant(QVariant::String));
    }
    if (m_defaultArgumentValue) {
      query.bindValue(OSArgumentRecordColumns::defaultArgumentValue,
                      toQString(*m_defaultArgumentValue));
    }
    else {
      query.bindValue(OSArgumentRecordColumns::defaultArgumentValue,
                      QVariant(QVariant::String));
    }

    query.bindValue(OSArgumentRecordColumns::domainType, m_domainType.value());
    if (m_domain) {
      query.bindValue(OSArgumentRecordColumns::domainValues, toQString(*m_domain));
    }
    else {
      query.bindValue(OSArgumentRecordColumns::domainValues, QVariant(QVariant::String));
    }
    query.bindValue(OSArgumentRecordColumns::choices,toQString(m_choices));
    query.bindValue(OSArgumentRecordColumns::choiceDisplayNames,
                    toQString(m_choiceDisplayNames));
    query.bindValue(OSArgumentRecordColumns::isRead,m_isRead);
    query.bindValue(OSArgumentRecordColumns::extension,toQString(m_extension));
  }

  void OSArgumentRecord_Impl::setLastValues(const QSqlQuery& query,
                                            ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(OSArgumentRecord::ColumnsType::rubyPerturbationRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastRubyMeasureRecordId = value.toInt();
    }
    else {
      m_lastRubyMeasureRecordId.reset();
    }

    value = query.value(OSArgumentRecord::ColumnsType::rubyContinuousVariableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastRubyContinuousVariableRecordId = value.toInt();
    }
    else {
      m_lastRubyContinuousVariableRecordId.reset();
    }

    value = query.value(OSArgumentRecord::ColumnsType::argumentType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastArgumentType = ruleset::OSArgumentType(value.toInt());

    value = query.value(OSArgumentRecord::ColumnsType::required);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRequired = value.toBool();

    value = query.value(OSArgumentRecord::ColumnsType::argumentValue);
    if (value.isValid() && !value.isNull()) {
      m_lastArgumentValue = value.toString().toStdString();
    }
    else {
      m_lastArgumentValue.reset();
    }

    value = query.value(OSArgumentRecord::ColumnsType::defaultArgumentValue);
    if (value.isValid() && !value.isNull()) {
      m_lastDefaultArgumentValue = value.toString().toStdString();
    }
    else {
      m_lastDefaultArgumentValue.reset();
    }

    value = query.value(OSArgumentRecord::ColumnsType::domainType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDomainType = ruleset::OSDomainType(value.toInt());

    value = query.value(OSArgumentRecord::ColumnsType::domainValues);
    if (value.isValid() && !value.isNull()) {
      m_lastDomain = value.toString().toStdString();
    }
    else {
      m_lastDomain.reset();
    }

    value = query.value(OSArgumentRecord::ColumnsType::choices);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastChoices = value.toString().toStdString();

    value = query.value(OSArgumentRecord::ColumnsType::choiceDisplayNames);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastChoiceDisplayNames = value.toString().toStdString();

    value = query.value(OSArgumentRecord::ColumnsType::isRead);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastIsRead = value.toBool();

    value = query.value(OSArgumentRecord::ColumnsType::extension);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastExtension = value.toString().toStdString();
  }

  bool OSArgumentRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(OSArgumentRecordColumns::rubyPerturbationRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_rubyMeasureRecordId && (*m_rubyMeasureRecordId == value.toInt());
    }
    else {
      result = result && !m_rubyMeasureRecordId;
    }

    value = query.value(OSArgumentRecordColumns::rubyContinuousVariableRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_rubyContinuousVariableRecordId && (*m_rubyContinuousVariableRecordId == value.toInt());
    }
    else {
      result = result && !m_rubyContinuousVariableRecordId;
    }

    value = query.value(OSArgumentRecordColumns::argumentType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_argumentType == ruleset::OSArgumentType(value.toInt()));

    value = query.value(OSArgumentRecordColumns::required);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_required == value.toBool());

    value = query.value(OSArgumentRecordColumns::argumentValue);
    if (value.isValid() && !value.isNull()) {
      result = result && m_argumentValue && (m_argumentValue.get() == value.toString().toStdString());
    }
    else {
      result = result && !m_argumentValue;
    }

    value = query.value(OSArgumentRecordColumns::defaultArgumentValue);
    if (value.isValid() && !value.isNull()) {
      result = result && m_defaultArgumentValue && (m_defaultArgumentValue.get() == value.toString().toStdString());
    }
    else {
      result = result && !m_defaultArgumentValue;
    }

    value = query.value(OSArgumentRecordColumns::domainType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_domainType == ruleset::OSDomainType(value.toInt()));

    value = query.value(OSArgumentRecordColumns::domainValues);
    if (value.isValid() && !value.isNull()) {
      result = result && m_domain && (m_domain.get() == value.toString().toStdString());
    }
    else {
      result = result && !m_domain;
    }

    value = query.value(OSArgumentRecordColumns::choices);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_choices == value.toString().toStdString());

    value = query.value(OSArgumentRecordColumns::choiceDisplayNames);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_choiceDisplayNames == value.toString().toStdString());

    value = query.value(OSArgumentRecordColumns::isRead);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_isRead == value.toBool());

    value = query.value(OSArgumentRecordColumns::extension);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_extension == value.toString().toStdString());

    return result;
  }

  void OSArgumentRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastRubyMeasureRecordId = m_rubyMeasureRecordId;
    m_lastRubyContinuousVariableRecordId = m_rubyContinuousVariableRecordId;
    m_lastArgumentType = m_argumentType;
    m_lastRequired = m_required;
    m_lastArgumentValue = m_argumentValue;
    m_lastDefaultArgumentValue = m_defaultArgumentValue;
    m_lastDomainType = m_domainType;
    m_lastDomain = m_domain;
    m_lastChoices = m_choices;
    m_lastChoiceDisplayNames = m_choiceDisplayNames;
    m_lastIsRead = m_isRead;
    m_lastExtension = m_extension;
  }

  void OSArgumentRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_rubyMeasureRecordId = m_lastRubyMeasureRecordId;
    m_rubyContinuousVariableRecordId = m_lastRubyContinuousVariableRecordId;
    m_argumentType = m_lastArgumentType;
    m_required = m_lastRequired;
    m_argumentValue = m_lastArgumentValue;
    m_defaultArgumentValue = m_lastDefaultArgumentValue;
    m_domainType = m_lastDomainType;
    m_domain = m_lastDomain;
    m_choices = m_lastChoices;
    m_choiceDisplayNames = m_lastChoiceDisplayNames;
    m_isRead = m_lastIsRead;
    m_extension = m_lastExtension;
  }

  std::string OSArgumentRecord_Impl::stringVectorToString(const std::vector<std::string>& strs) const
  {
    // DLM: was previously using functionality of Table class and CSV serialization

    std::string result;
    for (const std::string& str: strs) {
      // DLM: TODO, escape ','
      result += str + ",";
    }
    return result;
  }

  std::vector<std::string> OSArgumentRecord_Impl::stringToStringVector(const std::string& str) const
  {
    // DLM: was previously using functionality of Table class and CSV serialization

    StringVector result;
    QString qStr = toQString(str);
    QStringList qStringList = qStr.split(",");
    for (const QString& q : qStringList) {
      // DLM: TODO, unescape ','
      result.push_back(toString(q));
    }

    return result;
  }

} // detail

OSArgumentRecord::OSArgumentRecord(
    const ruleset::OSArgument& osArgument,
    RubyMeasureRecord& rubyMeasureRecord)
  : ObjectRecord(std::shared_ptr<detail::OSArgumentRecord_Impl>(
        new detail::OSArgumentRecord_Impl(osArgument, rubyMeasureRecord)),
        rubyMeasureRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::OSArgumentRecord_Impl>());
}

OSArgumentRecord::OSArgumentRecord(
    const ruleset::OSArgument& osArgument,
    RubyContinuousVariableRecord& rubyContinuousVariableRecord)
  : ObjectRecord(std::shared_ptr<detail::OSArgumentRecord_Impl>(
        new detail::OSArgumentRecord_Impl(osArgument, rubyContinuousVariableRecord)),
        rubyContinuousVariableRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::OSArgumentRecord_Impl>());
}

OSArgumentRecord::OSArgumentRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::OSArgumentRecord_Impl>(
        new detail::OSArgumentRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::OSArgumentRecord_Impl>());
}

OSArgumentRecord::OSArgumentRecord(std::shared_ptr<detail::OSArgumentRecord_Impl> impl,
                                                   ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::OSArgumentRecord_Impl>());
}

std::string OSArgumentRecord::databaseTableName() {
  return "OSArgumentRecords";
}

UpdateByIdQueryData OSArgumentRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (auto it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      auto nextIt = it;
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
    for (const auto & columnValue : result.columnValues)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(columnValue), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void OSArgumentRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  ObjectRecord::createIndices(qSqlDatabase,databaseTableName);

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "rubyPerturbationRecordIdIndex ON " + databaseTableName + " (rubyPerturbationRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "rubyContinuousVariableRecordIdIndex ON " + databaseTableName + " (rubyContinuousVariableRecordId)"));
  assertExec(query);
}

void OSArgumentRecord::updatePathData(ProjectDatabase database,
                                      const openstudio::path& originalBase,
                                      const openstudio::path& newBase)
{
  OSArgumentRecordVector records = getOSArgumentRecords(database);
  for (OSArgumentRecord& record : records) {
    record.getImpl<detail::OSArgumentRecord_Impl>()->updatePathData(originalBase,newBase);
  }
}

boost::optional<OSArgumentRecord> OSArgumentRecord::factoryFromQuery(
    const QSqlQuery& query,
    ProjectDatabase& database)
{
  OptionalOSArgumentRecord result;

  try {
    result = OSArgumentRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct OSArgumentRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<OSArgumentRecord> OSArgumentRecord::getOSArgumentRecords(ProjectDatabase& database) {
  std::vector<OSArgumentRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + OSArgumentRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(OSArgumentRecord(query, database));
  }

  return result;
}

boost::optional<OSArgumentRecord> OSArgumentRecord::getOSArgumentRecord(int id, ProjectDatabase& database) {
  boost::optional<OSArgumentRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + OSArgumentRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OSArgumentRecord(query, database);
  }

  return result;
}

boost::optional<RubyMeasureRecord> OSArgumentRecord::rubyMeasureRecord() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->rubyMeasureRecord();
}

boost::optional<RubyContinuousVariableRecord> OSArgumentRecord::rubyContinuousVariableRecord() const  {
  return getImpl<detail::OSArgumentRecord_Impl>()->rubyContinuousVariableRecord();
}

ruleset::OSArgument OSArgumentRecord::osArgument() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->osArgument();
}

ruleset::OSArgumentType OSArgumentRecord::type() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->type();
}

bool OSArgumentRecord::hasValue() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->hasValue();
}

bool OSArgumentRecord::hasDefaultValue() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->hasDefaultValue();
}

openstudio::path OSArgumentRecord::valueAsPath() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->valueAsPath();
}

openstudio::path OSArgumentRecord::defaultValueAsPath() const {
  return getImpl<detail::OSArgumentRecord_Impl>()->defaultValueAsPath();
}

/// @cond
OSArgumentRecord::OSArgumentRecord(std::shared_ptr<detail::OSArgumentRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

} // project
} // openstudio


