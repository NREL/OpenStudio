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
