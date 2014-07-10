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

#ifndef PROJECT_URLSEARCHPATHRECORD_HPP
#define PROJECT_URLSEARCHPATHRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {

class URLSearchPath;

namespace project {

namespace detail {
  class URLSearchPathRecord_Impl;
}

/** \class URLSearchPathRecordColumns
 *  \brief Columns of the URLSearchPathRecords table.
 *  \relates URLSearchPathRecord */
OPENSTUDIO_ENUM(URLSearchPathRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((url)(TEXT)(8))
                ((relativity)(INTEGER)(9))
                ((iddObjectType)(TEXT)(10))
                );

/** URLSearchPathRecord is an ObjectRecord that stores URLSearchPaths, that is,
 *  locations that should be searched to find resources of certain types. */
class PROJECT_API URLSearchPathRecord : public ObjectRecord {
 public:

  typedef detail::URLSearchPathRecord_Impl ImplType;
  typedef URLSearchPathRecordColumns ColumnsType;
  typedef URLSearchPathRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  /// constructors
  URLSearchPathRecord(const openstudio::URLSearchPath& urlSearchPath, ProjectDatabase& projectDatabase);

  /// constructor from query, throws if bad query
  URLSearchPathRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  virtual ~URLSearchPathRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<URLSearchPathRecord> factoryFromQuery(const QSqlQuery& query,
                                                               ProjectDatabase& database);

  /// get all model records in database
  static std::vector<URLSearchPathRecord> getURLSearchPathRecords(ProjectDatabase& projectDatabase);

  /// get model record in database
  static boost::optional<URLSearchPathRecord> getURLSearchPathRecord(int id, ProjectDatabase& projectDatabase);

  /** @name Getters */
  //@{

  /// get the path
  openstudio::URLSearchPath urlSearchPath() const;

  //@}
 protected:

  friend class Record;
  friend class ProjectDatabase;

  // from impl
  URLSearchPathRecord(std::shared_ptr<detail::URLSearchPathRecord_Impl> impl, ProjectDatabase projectDatabase);

  // from impl, does not register in database, use with caution
  URLSearchPathRecord(std::shared_ptr<detail::URLSearchPathRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.URLSearchPathRecord");
};

/** \relates URLSearchPathRecord */
typedef boost::optional<URLSearchPathRecord> OptionalURLSearchPathRecord;

/** \relates URLSearchPathRecord */
typedef std::vector<URLSearchPathRecord> URLSearchPathRecordVector;

} // project
} // openstudio

#endif // PROJECT_URLSEARCHPATHRECORD_HPP
