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

#ifndef PROJECT_AWSSESSIONRECORD_HPP
#define PROJECT_AWSSESSIONRECORD_HPP

#include "ProjectAPI.hpp"
#include "CloudSessionRecord.hpp"

namespace openstudio {

class AWSSession;

namespace project {

namespace detail {

  class AWSSessionRecord_Impl;

} // detail

/** AWSSessionRecord is a CloudSessionRecord. */
class PROJECT_API AWSSessionRecord : public CloudSessionRecord {
 public:

  typedef detail::AWSSessionRecord_Impl ImplType;
  typedef CloudSessionRecordColumns ColumnsType;
  typedef CloudSessionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  AWSSessionRecord(const AWSSession& awsSession, ProjectDatabase& database);

  AWSSessionRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~AWSSessionRecord() {}

  //@}

  /** Get AWSSessionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<AWSSessionRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<AWSSessionRecord> getAWSSessionRecords(ProjectDatabase& database);

  static boost::optional<AWSSessionRecord> getAWSSessionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  AWSSession awsSession() const;

  //@}
 protected:
  /// @cond
  explicit AWSSessionRecord(std::shared_ptr<detail::AWSSessionRecord_Impl> impl);

  friend class detail::AWSSessionRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  AWSSessionRecord(std::shared_ptr<detail::AWSSessionRecord_Impl> impl,
                   ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.AWSSessionRecord");
};

/** \relates AWSSessionRecord*/
typedef boost::optional<AWSSessionRecord> OptionalAWSSessionRecord;

/** \relates AWSSessionRecord*/
typedef std::vector<AWSSessionRecord> AWSSessionRecordVector;

} // project
} // openstudio

#endif // PROJECT_AWSSESSIONRECORD_HPP

