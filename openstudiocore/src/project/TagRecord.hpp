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

#ifndef PROJECT_TAGRECORD_HPP
#define PROJECT_TAGRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {

class Tag;

namespace project {

class FileReferenceRecord;
class DataPointRecord;

namespace detail {

  class TagRecord_Impl;

} // detail

/** \class TagRecordColumns
 *  \brief Column definitions for the TagRecords table.
 *
 *  \relates TagRecord */
OPENSTUDIO_ENUM( TagRecordColumns,
    ((id)(INTEGER PRIMARY KEY)(0))
    ((handle)(TEXT)(1))
    ((name)(TEXT)(2))
    ((displayName)(TEXT)(3))
    ((description)(TEXT)(4)) 
    ((timestampCreate)(TEXT)(5))
    ((timestampLast)(TEXT)(6))
    ((uuidLast)(TEXT)(7))
    ((fileReferenceRecordId)(INTEGER)(8))
    ((dataPointRecordId)(INTEGER)(9))
);

class PROJECT_API TagRecord : public ObjectRecord {
 public:

  typedef detail::TagRecord_Impl ImplType;
  typedef TagRecordColumns ColumnsType;
  typedef TagRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  TagRecord(const std::string& tag, 
            const FileReferenceRecord& fileReferenceRecord);

  TagRecord(const Tag& tag,
            const DataPointRecord& dataPointRecord);

  TagRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~TagRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<TagRecord> factoryFromQuery(const QSqlQuery& query, 
                                                     ProjectDatabase& database);

  static std::vector<TagRecord> getTagRecords(ProjectDatabase& database);

  static boost::optional<TagRecord> getTagRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns the FileReferenceRecord that parents this TagRecord. */
  boost::optional<FileReferenceRecord> fileReferenceRecord() const;

  /** Returns the DataPointRecord that parents this TagRecord. */
  boost::optional<DataPointRecord> dataPointRecord() const;

  /** Get tag. */
  Tag tag() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::TagRecord_Impl;

  /** Construct from impl. */
  TagRecord(std::shared_ptr<detail::TagRecord_Impl> impl, ProjectDatabase projectDatabase);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit TagRecord(std::shared_ptr<detail::TagRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.TagRecord");
};

/** \relates TagRecord*/
typedef boost::optional<TagRecord> OptionalTagRecord;

/** \relates TagRecord*/
typedef std::vector<TagRecord> TagRecordVector;

} // project
} // openstudio

#endif // PROJECT_TAGRECORD_HPP
