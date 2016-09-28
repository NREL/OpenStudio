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

#ifndef PROJECT_URLRECORD_HPP
#define PROJECT_URLRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Url.hpp"

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
  explicit UrlRecord(std::shared_ptr<detail::UrlRecord_Impl> impl);

  friend class detail::UrlRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  UrlRecord(std::shared_ptr<detail::UrlRecord_Impl> impl,
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

