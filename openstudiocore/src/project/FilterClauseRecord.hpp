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

#ifndef PROJECT_FILTERCLAUSERECORD_HPP
#define PROJECT_FILTERCLAUSERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class FilterClause;
}
namespace project {

namespace detail {
  class FilterClauseRecord_Impl;
}

/** \class FilterClauseRecordType 
 *
 *  \relates FilterClauseRecord */
OPENSTUDIO_ENUM( FilterClauseRecordType,
  ((ModelFilterClauseRecord)(ModelFilterClauseRecord)(0))
  ((ModelObjectFilterClauseRecord)(ModelObjectFilterClauseRecord)(1))
  ((StandardsFilterClauseRecord)(StandardsFilterClauseRecord)(2))
);

class PROJECT_API FilterClauseRecord : public ClauseRecord {
 public:

  typedef detail::FilterClauseRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  virtual ~FilterClauseRecord() {}

  /// get all filter clauses 
  static std::vector<FilterClauseRecord> getFilterClauseRecords(ProjectDatabase& database);

  /// get filter clause by id
  static boost::optional<FilterClauseRecord> getFilterClauseRecord(int id, ProjectDatabase& database);

  /// get filter clause from a query
  static boost::optional<FilterClauseRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  /// try to find the filter in the database by UUID, if not found try to create a new record
  static FilterClauseRecord factoryFromFilter(const openstudio::ruleset::FilterClause& filter, ProjectDatabase& projectDatabase);

  /// get the corresponding filter clause
  openstudio::ruleset::FilterClause filterClause() const;

 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  FilterClauseRecord(boost::shared_ptr<detail::FilterClauseRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  FilterClauseRecord(boost::shared_ptr<openstudio::project::detail::FilterClauseRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.FilterClauseRecord");
};

/** \relates FilterClauseRecord */
typedef boost::optional<FilterClauseRecord> OptionalFilterClauseRecord;

/** \relates FilterClauseRecord */
typedef std::vector<FilterClauseRecord> FilterClauseRecordVector;

} // project
} // openstudio

#endif // PROJECT_FILTERCLAUSERECORD_HPP
