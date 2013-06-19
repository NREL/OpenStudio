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

#ifndef PROJECT_STANDARDSFILTEROBJECTENUMATTRIBUTERECORD_HPP
#define PROJECT_STANDARDSFILTEROBJECTENUMATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterObjectEnumAttribute;
}
namespace project {

namespace detail {
  class StandardsFilterObjectEnumAttributeRecord_Impl;
}

/** StandardsFilterObjectEnumAttributeRecord is a StandardsFilterObjectAttributeRecord. */
class PROJECT_API StandardsFilterObjectEnumAttributeRecord : public StandardsFilterObjectAttributeRecord {
 public:

  typedef detail::StandardsFilterObjectEnumAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsFilterObjectEnumAttributeRecord(
      const openstudio::ruleset::StandardsFilterObjectEnumAttribute& standardsFilterObjectEnumAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsFilterObjectEnumAttributeRecord(const QSqlQuery& query, 
                                           ProjectDatabase& projectDatabase);

  virtual ~StandardsFilterObjectEnumAttributeRecord() {}

  static boost::optional<StandardsFilterObjectEnumAttributeRecord> factoryFromQuery(
     const QSqlQuery& query,ProjectDatabase& database);

  /// get all records in database
  static std::vector<StandardsFilterObjectEnumAttributeRecord> 
  getStandardsFilterObjectEnumAttributeRecords(ProjectDatabase& projectDatabase);

  /// get record in database
  static boost::optional<StandardsFilterObjectEnumAttributeRecord> 
  getStandardsFilterObjectEnumAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::StandardsFilterObjectEnumAttribute standardsFilterObjectEnumAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsFilterObjectEnumAttributeRecord_Impl;

  // from impl
  StandardsFilterObjectEnumAttributeRecord(
      boost::shared_ptr<detail::StandardsFilterObjectEnumAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsFilterObjectEnumAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectEnumAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsFilterObjectEnumAttributeRecord");
};

/** \relates StandardsFilterObjectEnumAttributeRecord*/
typedef boost::optional<StandardsFilterObjectEnumAttributeRecord> OptionalStandardsFilterObjectEnumAttributeRecord;

/** \relates StandardsFilterObjectEnumAttributeRecord*/
typedef std::vector<StandardsFilterObjectEnumAttributeRecord> StandardsFilterObjectEnumAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTEROBJECTENUMATTRIBUTERECORD_HPP
