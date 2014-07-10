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

#ifndef PROJECT_PROJECTDATABASERECORD_HPP
#define PROJECT_PROJECTDATABASERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

class QSqlQuery;

namespace openstudio {
namespace project {

namespace detail {
  class ProjectDatabaseRecord_Impl;
  class ProjectDatabase_Impl;
}

/** \class ProjectDatabaseRecordColumns
 *  \brief Column definitions for the ProjectDatabaseRecords table.
 *
 *  \relates ProjectDatabaseRecord */
OPENSTUDIO_ENUM( ProjectDatabaseRecordColumns,
                 ((id)(INTEGER PRIMARY KEY)(0))
                 ((handle)(TEXT)(1))
                 ((name)(TEXT)(2))
                 ((displayName)(TEXT)(3))
                 ((description)(TEXT)(4))
                 ((timestampCreate)(TEXT)(5))
                 ((timestampLast)(TEXT)(6))
                 ((uuidLast)(TEXT)(7))
                 ((version)(TEXT)(8))
                 ((runManagerDBPath)(TEXT)(9))
                 );

class PROJECT_API ProjectDatabaseRecord : public ObjectRecord {
 public:

  typedef detail::ProjectDatabaseRecord_Impl ImplType;
  typedef ProjectDatabaseRecordColumns ColumnsType;

  /** @name Constructors and Destructors */
  //@{

  ProjectDatabaseRecord(const std::string& version,
                        const openstudio::path& runManagerDBPath,
                        ProjectDatabase projectDatabase);

  virtual ~ProjectDatabaseRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static ProjectDatabaseRecord getProjectDatabaseRecord(const ProjectDatabase& database);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** @name Getters */
  //@{

  /// get the version
  std::string version() const;

  /// get the run manager path
  openstudio::path runManagerDBPath() const;

  //@}
 protected:

  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ProjectDatabase_Impl;

  /// constructor, does not register in database, use with caution
  ProjectDatabaseRecord(std::shared_ptr<detail::ProjectDatabaseRecord_Impl> impl);

  void setVersion(const std::string& version);

 private:
  REGISTER_LOGGER("openstudio.project.ProjectDatabaseRecord");
};

} // project
} // openstudio

#endif // PROJECT_PROJECTDATABASERECORD_HPP
