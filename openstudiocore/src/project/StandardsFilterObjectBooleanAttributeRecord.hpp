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

#ifndef PROJECT_STANDARDSFILTEROBJECTBOOLEANATTRIBUTERECORD_HPP
#define PROJECT_STANDARDSFILTEROBJECTBOOLEANATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterObjectBooleanAttribute;
}
namespace project {

namespace detail {
  class StandardsFilterObjectBooleanAttributeRecord_Impl;
}

/** StandardsFilterObjectBooleanAttributeRecord is a StandardsFilterObjectAttributeRecord. */
class PROJECT_API StandardsFilterObjectBooleanAttributeRecord : public StandardsFilterObjectAttributeRecord {
 public:

  typedef detail::StandardsFilterObjectBooleanAttributeRecord_Impl ImplType;
  typedef ClauseRecordColumns ColumnsType;
  typedef ClauseRecord ObjectRecordType;

  /// constructors
  StandardsFilterObjectBooleanAttributeRecord(
      const openstudio::ruleset::StandardsFilterObjectBooleanAttribute& standardsFilterObjectBooleanAttribute, 
      ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  StandardsFilterObjectBooleanAttributeRecord(const QSqlQuery& query, 
                                              ProjectDatabase& projectDatabase);

  virtual ~StandardsFilterObjectBooleanAttributeRecord() {}

  static boost::optional<StandardsFilterObjectBooleanAttributeRecord> factoryFromQuery(
     const QSqlQuery& query,ProjectDatabase& database);

  /// get all records in database
  static std::vector<StandardsFilterObjectBooleanAttributeRecord> 
  getStandardsFilterObjectBooleanAttributeRecords(ProjectDatabase& projectDatabase);

  /// get record in database
  static boost::optional<StandardsFilterObjectBooleanAttributeRecord> 
  getStandardsFilterObjectBooleanAttributeRecord(int id, ProjectDatabase& projectDatabase);

  /// get the filter from the record
  openstudio::ruleset::StandardsFilterObjectBooleanAttribute 
  standardsFilterObjectBooleanAttribute() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::StandardsFilterObjectBooleanAttributeRecord_Impl;

  // from impl
  StandardsFilterObjectBooleanAttributeRecord(
      boost::shared_ptr<detail::StandardsFilterObjectBooleanAttributeRecord_Impl> impl, 
      ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit StandardsFilterObjectBooleanAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectBooleanAttributeRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.StandardsFilterObjectBooleanAttributeRecord");
};

/** \relates StandardsFilterObjectBooleanAttributeRecord*/
typedef boost::optional<StandardsFilterObjectBooleanAttributeRecord> OptionalStandardsFilterObjectBooleanAttributeRecord;

/** \relates StandardsFilterObjectBooleanAttributeRecord*/
typedef std::vector<StandardsFilterObjectBooleanAttributeRecord> StandardsFilterObjectBooleanAttributeRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTEROBJECTBOOLEANATTRIBUTERECORD_HPP
