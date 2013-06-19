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

#ifndef PROJECT_CLAUSERECORD_HPP
#define PROJECT_CLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace project {

namespace detail {
  class ClauseRecord_Impl;
}

/** \class ClauseRecordType
 *
 *  \relates ClauseRecord */
OPENSTUDIO_ENUM( ClauseRecordType,
                 ((FilterClauseRecord)(FilterClauseRecord)(0))
                 ((ActionClauseRecord)(ActionClauseRecord)(1))
                 );

/** \class ClauseRecordColumns
 *  \brief Column definitions for the ClauseRecords table.
 *
 *  \relates ClauseRecord */
OPENSTUDIO_ENUM( ClauseRecordColumns,
                 ((id)(INTEGER PRIMARY KEY)(0))
                 ((handle)(TEXT)(1))
                 ((name)(TEXT)(2))
                 ((displayName)(TEXT)(3))
                 ((description)(TEXT)(4))
                 ((timestampCreate)(TEXT)(5))
                 ((timestampLast)(TEXT)(6))
                 ((uuidLast)(TEXT)(7))
                 ((clauseRecordType)(INTEGER)(8))
                 ((filterClauseRecordType)(INTEGER)(9))
                 ((modelObjectFilterClauseRecordType)(INTEGER)(10))
                 ((modelObjectFilterAttributeRecordType)(INTEGER)(11))
                 ((standardsFilterClauseRecordType)(INTEGER)(12))
                 ((standardsFilterObjectAttributeRecordType)(INTEGER)(13))
                 ((actionClauseRecordType)(INTEGER)(14))
                 ((modelObjectActionClauseRecordType)(INTEGER)(15))
                 ((standardsActionClauseRecordType)(INTEGER)(16))
                 ((parentClauseRecordId)(INTEGER)(17))
                 ((componentRecordId)(INTEGER)(18))
                 ((attributeName)(TEXT)(19))
                 ((attributeValueType)(INTEGER)(20))
                 ((attributeValue)(TEXT)(21))
                 ((attributeUnits)(TEXT)(22))
                 ((relationshipName)(TEXT)(23))
                 ((predicateType)(INTEGER)(24))
                 ((predicate)(INTEGER)(25))
                 ((iddObjectType)(TEXT)(26))
                 ((standardsObjectType)(TEXT)(27))
                 ((standardsFilterAvailableModelRecordId)(INTEGER)(28))
                 ((standardsFilterAvailableModelId)(TEXT)(29))
                 ((standardsRulesetModelType)(TEXT)(30))
                 ((standardsFilterAvailableObjectId)(TEXT)(31))
                 ((standardsFilterObjectAttributeRecordId)(TEXT)(32))
                 ((standardsRulesetOptionId)(INTEGER)(33))
                 ((standardsActionRuleResultRecordType)(TEXT)(34))
                 );

/** ClauseRecord is an abstract ObjectRecord for storing ruleset::Clauses. */
class PROJECT_API ClauseRecord : public ObjectRecord {
 public:

  typedef detail::ClauseRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~ClauseRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /// get all clauses
  static std::vector<ClauseRecord> getClauseRecords(ProjectDatabase& database);

  /// get clause by id
  static boost::optional<ClauseRecord> getClauseRecord(int id, ProjectDatabase& database);

  /// get clause from a query
  static boost::optional<ClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;

  // from impl
  ClauseRecord(boost::shared_ptr<detail::ClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  ClauseRecord(boost::shared_ptr<detail::ClauseRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ClauseRecord");
};

/** \relates ClauseRecord */
typedef boost::optional<ClauseRecord> OptionalClauseRecord;

/** \relates ClauseRecord */
typedef std::vector<ClauseRecord> ClauseRecordVector;

} // project
} // openstudio

#endif // PROJECT_CLAUSERECORD_HPP
