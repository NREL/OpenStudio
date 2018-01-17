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
