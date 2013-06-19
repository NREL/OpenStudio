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

#ifndef PROJECT_STANDARDSFILTEROBJECTDOUBLEATTRIBUTERECORD_HPP
#define PROJECT_STANDARDSFILTEROBJECTDOUBLEATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterObjectDoubleAttribute;
}
namespace project {

namespace detail {
  class StandardsFilterObjectDoubleAttributeRecord_Impl;
}

/** StandardsFilterObjectDoubleAttributeRecord is a StandardsFilterObjectAttributeRecord. */
class PROJECT_API StandardsFilterObjectDoubleAttributeRecord : public StandardsFilterObjectAttributeRecord {
 public:

  typedef detail::StandardsFilterObjectDoubleAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsFilterObjectDoubleAttributeRecord(
      const openstudio::ruleset::StandardsFilterObjectDoubleAttribute& standardsFilterObjectDoubleAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsFilterObjectDoubleAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~StandardsFilterObjectDoubleAttributeRecord() {}

  static boost::optional<StandardsFilterObjectDoubleAttributeRecord> factoryFromQuery(
     const QSqlQuery& query,ProjectDatabase& database);

  /// get all records in database
  static std::vector<StandardsFilterObjectDoubleAttributeRecord> 
  getStandardsFilterObjectDoubleAttributeRecords(ProjectDatabase& projectDatabase);

  /// get record in database
  static boost::optional<StandardsFilterObjectDoubleAttributeRecord> 
  getStandardsFilterObjectDoubleAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::StandardsFilterObjectDoubleAttribute 
  standardsFilterObjectDoubleAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsFilterObjectDoubleAttributeRecord_Impl;

  // from impl
  StandardsFilterObjectDoubleAttributeRecord(
      boost::shared_ptr<detail::StandardsFilterObjectDoubleAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsFilterObjectDoubleAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectDoubleAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsFilterObjectDoubleAttributeRecord");
};

/** \relates StandardsFilterObjectDoubleAttributeRecord*/
typedef boost::optional<StandardsFilterObjectDoubleAttributeRecord> OptionalStandardsFilterObjectDoubleAttributeRecord;

/** \relates StandardsFilterObjectDoubleAttributeRecord*/
typedef std::vector<StandardsFilterObjectDoubleAttributeRecord> StandardsFilterObjectDoubleAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTEROBJECTDOUBLEATTRIBUTERECORD_HPP
