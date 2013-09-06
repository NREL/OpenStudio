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

#ifndef PROJECT_CLOUDSESSIONRECORD_HPP
#define PROJECT_CLOUDSESSIONRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/ObjectRecord.hpp>

namespace openstudio {

class CloudSession;

namespace project {

class UrlRecord;

namespace detail {

  class CloudSessionRecord_Impl;

} // detail

/** \class CloudSessionRecordType
 *  \brief ObjectRecord types that derive from CloudSessionRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(CloudSessionRecordType,
    ((VagrantSessionRecord))
);
 *  \endcode */
OPENSTUDIO_ENUM(CloudSessionRecordType,
    ((VagrantSessionRecord))
);

/** \class CloudSessionRecordColumns
 *  \brief Column definitions for the CloudSessionRecords table.
 *
 *  \relates CloudSessionRecord */
OPENSTUDIO_ENUM(CloudSessionRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((cloudSessionRecordType)(INTEGER)(8))
  ((sessionId)(TEXT)(9))
  ((serverUrlRecordId)(TEXT)(10))
);

/** CloudSessionRecord is a ObjectRecord. */
class PROJECT_API CloudSessionRecord : public ObjectRecord {
 public:

  typedef detail::CloudSessionRecord_Impl ImplType;
  typedef CloudSessionRecordColumns ColumnsType;
  typedef CloudSessionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~CloudSessionRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get CloudSessionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<CloudSessionRecord> factoryFromQuery(const QSqlQuery& query, 
                                                              ProjectDatabase& database);

  static CloudSessionRecord factoryFromCloudSession(const CloudSession& cloudSession, 
                                                    ProjectDatabase& database);

  static std::vector<CloudSessionRecord> getCloudSessionRecords(ProjectDatabase& database);

  static boost::optional<CloudSessionRecord> getCloudSessionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  std::string sessionId() const;

  boost::optional<UrlRecord> serverUrlRecord() const;

  std::vector<UrlRecord> workerUrlRecords() const;

  CloudSession cloudSession() const;

  //@}
 protected:
  /// @cond
  explicit CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl> impl);

  friend class detail::CloudSessionRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl> impl,
                     ProjectDatabase database);

  /// @endcond

  void constructRelatedRecords(const CloudSession& cloudSession,
                               ProjectDatabase& database);
 private:
  REGISTER_LOGGER("openstudio.project.CloudSessionRecord");
};

/** \relates CloudSessionRecord*/
typedef boost::optional<CloudSessionRecord> OptionalCloudSessionRecord;

/** \relates CloudSessionRecord*/
typedef std::vector<CloudSessionRecord> CloudSessionRecordVector;

} // project
} // openstudio

#endif // PROJECT_CLOUDSESSIONRECORD_HPP

