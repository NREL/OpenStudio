/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
