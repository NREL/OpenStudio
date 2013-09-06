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

#ifndef PROJECT_URLRECORD_HPP
#define PROJECT_URLRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/ObjectRecord.hpp>

#include <utilities/core/Url.hpp>

namespace openstudio {
namespace project {

namespace detail {

  class UrlRecord_Impl;

} // detail

/** \class UrlRecordColumns
 *  \brief Column definitions for the UrlRecords table.
 *  
 *  \relates UrlRecord */
OPENSTUDIO_ENUM(UrlRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((parentDatabaseTableName)(TEXT)(8))
  ((parentRecordId)(INTEGER)(9))
  ((url)(TEXT)(10))
);

/** UrlRecord is a ObjectRecord. */
class PROJECT_API UrlRecord : public ObjectRecord {
 public:

  typedef detail::UrlRecord_Impl ImplType;
  typedef UrlRecordColumns ColumnsType;
  typedef UrlRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  UrlRecord(const openstudio::Url& url, ObjectRecord& parentRecord);

  UrlRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~UrlRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get UrlRecord from query. Returned object will be of the correct derived type. */
  static boost::optional<UrlRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<UrlRecord> getUrlRecords(ProjectDatabase& database);

  static boost::optional<UrlRecord> getUrlRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  openstudio::Url url() const;

  //@}
 protected:
  /// @cond
  explicit UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl> impl);

  friend class detail::UrlRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl> impl,
            ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.UrlRecord");
};

/** \relates UrlRecord*/
typedef boost::optional<UrlRecord> OptionalUrlRecord;

/** \relates UrlRecord*/
typedef std::vector<UrlRecord> UrlRecordVector;

} // project
} // openstudio

#endif // PROJECT_URLRECORD_HPP

