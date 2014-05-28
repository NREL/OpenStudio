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

#ifndef PROJECT_AWSSETTINGSRECORD_HPP
#define PROJECT_AWSSETTINGSRECORD_HPP

#include "ProjectAPI.hpp"
#include "CloudSettingsRecord.hpp"

namespace openstudio {

class AWSSettings;

namespace project {

namespace detail {

  class AWSSettingsRecord_Impl;

} // detail

/** AWSSettingsRecord is a CloudSettingsRecord. */
class PROJECT_API AWSSettingsRecord : public CloudSettingsRecord {
 public:

  typedef detail::AWSSettingsRecord_Impl ImplType;
  typedef CloudSettingsRecordColumns ColumnsType;
  typedef CloudSettingsRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  AWSSettingsRecord(const AWSSettings& aWSSettings, ProjectDatabase& database);

  AWSSettingsRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~AWSSettingsRecord() {}

  //@}

  /** Get AWSSettingsRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<AWSSettingsRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<AWSSettingsRecord> getAWSSettingsRecords(ProjectDatabase& database);

  static boost::optional<AWSSettingsRecord> getAWSSettingsRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  AWSSettings awsSettings() const;

  //@}
 protected:
  /// @cond
  explicit AWSSettingsRecord(std::shared_ptr<detail::AWSSettingsRecord_Impl> impl);

  friend class detail::AWSSettingsRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  AWSSettingsRecord(std::shared_ptr<detail::AWSSettingsRecord_Impl> impl,
                    ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.AWSSettingsRecord");
};

/** \relates AWSSettingsRecord*/
typedef boost::optional<AWSSettingsRecord> OptionalAWSSettingsRecord;

/** \relates AWSSettingsRecord*/
typedef std::vector<AWSSettingsRecord> AWSSettingsRecordVector;

} // project
} // openstudio

#endif // PROJECT_AWSSETTINGSRECORD_HPP

