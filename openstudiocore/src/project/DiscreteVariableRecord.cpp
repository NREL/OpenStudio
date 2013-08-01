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

#include <project/DiscreteVariableRecord.hpp>
#include <project/DiscreteVariableRecord_Impl.hpp>

#include <project/JoinRecord.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(const NAMESPACE::DiscreteVariable& discreteVariable, ProjectDatabase& database)
    : InputVariableRecord_Impl(discreteVariable, database)
  {
    BOOST_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : InputVariableRecord_Impl(query, database)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;

    // TODO: Extract data members from query. Templates follow.

    // Required data member
    // value = query.value(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME);
    // BOOST_ASSERT(value.isValid() && !value.isNull());
    // m_DATAMEMBERNAME = value.toTYPE();

    // Optional data member
    // value = query.value(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_DATAMEMBERNAME = value.toTYPE();
    // }

  }

  boost::optional<ObjectRecord> DiscreteVariableRecord_Impl::parent() const {
    // Return this object's parent, if it has one. See ComponentAttributeRecord_Impl
    // for an example.
    BOOST_ASSERT(false);
    return boost::none;
  }

  std::vector<ObjectRecord> DiscreteVariableRecord_Impl::children() const {
    // Return this object's children. See ComponentReferenceRecord_Impl for an example.
    BOOST_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> DiscreteVariableRecord_Impl::resources() const {
    // Return this object's resources. See ModelObjectActionSetRelationshipRecord_Impl
    // for an example.
    BOOST_ASSERT(false);
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> DiscreteVariableRecord_Impl::joinRecords() const {
    // Return the join relationships between this object and others. See
    // ModelObjectActionSetRelationshipRecord_Impl for an example.
    BOOST_ASSERT(false);
    JoinRecordVector result;
    return result;
  }

  void DiscreteVariableRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<DiscreteVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  NAMESPACE::DiscreteVariable DiscreteVariableRecord::discreteVariable() const {
    // TODO: De-serialize the object here.
    BOOST_ASSERT(false);
  }

  void DiscreteVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    InputVariableRecord_Impl::bindValues(query);

    // Template for required data.
    // query.bindValue(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME,m_DATAMEMBERNAME);
    // Template for optional data.
    // if (m_DATAMEMBERNAME) {
    //   query.bindValue(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME,*m_DATAMEMBERNAME);
    // }
    // else {
    //   query.bindValue(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME,QVariant(QVariant::TYPE));
    // }
  }

  void DiscreteVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    InputVariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    // Template for required data.
    // value = query.value(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME);
    // BOOST_ASSERT(value.isValid() && !value.isNull());
    // m_lastDATAMEMBERNAME = value.toTYPE();

    // Template for optional data.
    // value = query.value(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   m_lastDATAMEMBERNAME = value.toTYPE();
    // }
    // else {
    //   m_lastDATAMEMBERNAME.reset();
    // }
  }

  bool DiscreteVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    bool result = InputVariableRecord_Impl::compareValues(query);

    QVariant value;

    // Template for required data.
    // value = query.value(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME);
    // BOOST_ASSERT(value.isValid() && !value.isNull());
    // result = result && (m_DATAMEMBERNAME == value.toTYPE());

    // Template for optional data.
    // value = query.value(DiscreteVariableRecord::ColumnsType::DATAMEMBERNAME);
    // if (value.isValid() && !value.isNull()) {
    //   result = result && m_DATAMEMBERNAME && (*m_DATAMEMBERNAME == value.toTYPE());
    // }
    // else {
    //   result = result && !m_DATAMEMBERNAME;
    // }

    return result;
  }

  void DiscreteVariableRecord_Impl::saveLastValues() {
    InputVariableRecord_Impl::saveLastValues();

    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void DiscreteVariableRecord_Impl::revertToLastValues() {
    InputVariableRecord_Impl::revertToLastValues();

    // m_DATAMEMBERNAME = m_lastDATAMEMBERNAME;
  }

} // detail

DiscreteVariableRecord::DiscreteVariableRecord(const NAMESPACE::DiscreteVariable& discreteVariable, ProjectDatabase& database)
  : InputVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl>(
        new detail::DiscreteVariableRecord_Impl(discreteVariable, database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());

  BOOST_ASSERT(false);
  // TODO: Align with final public constructors.
  // TODO: Handle relationships (setting id fields) as needed.
}

DiscreteVariableRecord::DiscreteVariableRecord(const QSqlQuery& query, ProjectDatabase& database)
  : InputVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl>(
        new detail::DiscreteVariableRecord_Impl(query, database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());
}

boost::optional<DiscreteVariableRecord> DiscreteVariableRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalDiscreteVariableRecord result;

  // Template for base classes. See, for instance, DiscretePerturbationRecord::factoryFromQuery.
  // int discreteVariableRecordType = query.value(DiscreteVariableRecordColumns::discreteVariableRecordType).toInt();

  // switch (discreteVariableRecordType) {
  //   case DiscreteVariableRecordType::FIRSTDERIVEDTYPE : 
  //     result = FIRSTDERIVEDTYPE(query, database).cast<DiscreteVariableRecord>();
  //    break;
  //   default :
  //     LOG(Error,"Unknown DiscreteVariableRecordType " << discreteVariableRecordType);
  //     return boost::none;
  // }

  // Template for classes with no derived classes.
  // try {
  //   result = DiscreteVariableRecord(query,database);
  // }
  // catch (const std::exception& e) {
  //   LOG(Error,"Unable to construct DiscreteVariableRecord from query, because '"
  //       << e.what() << "'.");
  // }

  return result;
}

DiscreteVariableRecord DiscreteVariableRecord::factoryFromDiscreteVariable(const NAMESPACE::DiscreteVariable& discreteVariable, ProjectDatabase& database)
{
  // TODO: Delete if no derived classes.
  BOOST_ASSERT(false);

  // Template. See, for instance, StandardsFilterObjectAttributeRecord::factoryFromFilter.

  // if (discreteVariable.optionalCast<NAMESPACE::FIRST_DERIVED_CLASS>()) {
  //   return FIRST_DERIVED_CLASSRecord(discreteVariable.cast<NAMESPACE::FIRST_DERIVED_CLASS>(), database);
  // else if {
  //   ...
  // }

  BOOST_ASSERT(false);
  return DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl>());
}

std::vector<DiscreteVariableRecord> DiscreteVariableRecord::getDiscreteVariableRecords(ProjectDatabase& database) {
  std::vector<DiscreteVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + InputVariableRecord::databaseTableName() + " WHERE inputVariableRecordType=:inputVariableRecordType"));
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  assertExec(query);
  while (query.next()) {
    // TODO: Choose appropriate implementation.

    // OptionalDiscreteVariableRecord discreteVariableRecord = DiscreteVariableRecord::factoryFromQuery(query, database);
    // if (discreteVariableRecord) {
    //   result.push_back(*discreteVariableRecord);
    // }

    // result.push_back(DiscreteVariableRecord(query, database));
  }

  return result;
}

boost::optional<DiscreteVariableRecord> DiscreteVariableRecord::getDiscreteVariableRecord(int id, ProjectDatabase& database) {
  boost::optional<DiscreteVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check the WHERE portion of the query.
  query.prepare(toQString("SELECT * FROM " + InputVariableRecord::databaseTableName() + " WHERE inputVariableRecordType=:inputVariableRecordType AND id=:id"));
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    // TODO: Choose appropriate implementation.

    // result = DiscreteVariableRecord::factoryFromQuery(query, database);

    // result = DiscreteVariableRecord(query, database);
  }

  return result;
}

NAMESPACE::DiscreteVariable DiscreteVariableRecord::discreteVariable() const {
  return getImpl<detail::DiscreteVariableRecord_Impl>()->discreteVariable();
}

/// @cond
DiscreteVariableRecord::DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl)
  : InputVariableRecord(impl)
{}

DiscreteVariableRecord::DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl,
                                               ProjectDatabase database)
  : InputVariableRecord(impl, database)
{
  BOOST_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

