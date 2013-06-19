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

#ifndef PROJECT_STANDARDSFILTEROBJECTINTEGERATTRIBUTERECORD_HPP
#define PROJECT_STANDARDSFILTEROBJECTINTEGERATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterObjectIntegerAttribute;
}
namespace project {

namespace detail {
  class StandardsFilterObjectIntegerAttributeRecord_Impl;
}

/** StandardsFilterObjectIntegerAttributeRecord is a StandardsFilterObjectAttributeRecord. */
class PROJECT_API StandardsFilterObjectIntegerAttributeRecord : public StandardsFilterObjectAttributeRecord {
 public:

  typedef detail::StandardsFilterObjectIntegerAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsFilterObjectIntegerAttributeRecord(
      const openstudio::ruleset::StandardsFilterObjectIntegerAttribute& standardsFilterObjectIntegerAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsFilterObjectIntegerAttributeRecord(const QSqlQuery& query, 
                                              ProjectDatabase& projectDatabase);

  virtual ~StandardsFilterObjectIntegerAttributeRecord() {}

  static boost::optional<StandardsFilterObjectIntegerAttributeRecord> factoryFromQuery(
     const QSqlQuery& query,ProjectDatabase& database);

  /// get all records in database
  static std::vector<StandardsFilterObjectIntegerAttributeRecord> 
  getStandardsFilterObjectIntegerAttributeRecords(ProjectDatabase& projectDatabase);

  /// get record in database
  static boost::optional<StandardsFilterObjectIntegerAttributeRecord> 
  getStandardsFilterObjectIntegerAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::StandardsFilterObjectIntegerAttribute 
  standardsFilterObjectIntegerAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsFilterObjectIntegerAttributeRecord_Impl;

  // from impl
  StandardsFilterObjectIntegerAttributeRecord(
      boost::shared_ptr<detail::StandardsFilterObjectIntegerAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsFilterObjectIntegerAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectIntegerAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsFilterObjectIntegerAttributeRecord");
};

/** \relates StandardsFilterObjectIntegerAttributeRecord*/
typedef boost::optional<StandardsFilterObjectIntegerAttributeRecord> OptionalStandardsFilterObjectIntegerAttributeRecord;

/** \relates StandardsFilterObjectIntegerAttributeRecord*/
typedef std::vector<StandardsFilterObjectIntegerAttributeRecord> StandardsFilterObjectIntegerAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTEROBJECTINTEGERATTRIBUTERECORD_HPP
