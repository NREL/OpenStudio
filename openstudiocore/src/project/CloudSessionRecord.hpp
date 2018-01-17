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

#ifndef PROJECT_CLOUDSESSIONRECORD_HPP
#define PROJECT_CLOUDSESSIONRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

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
    ((AWSSessionRecord))
);
 *  \endcode */
OPENSTUDIO_ENUM(CloudSessionRecordType,
    ((VagrantSessionRecord))
    ((AWSSessionRecord))
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
  ((numServerProcessors)(INTEGER)(11))
  ((numWorkerProcessors)(INTEGER)(12))
  ((privateKey)(TEXT)(13))
  ((timestamp)(TEXT)(14))
  ((region)(TEXT)(15))
  ((serverInstanceType)(TEXT)(16))
  ((workerInstanceType)(TEXT)(17))
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
  explicit CloudSessionRecord(std::shared_ptr<detail::CloudSessionRecord_Impl> impl);

  friend class detail::CloudSessionRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  CloudSessionRecord(std::shared_ptr<detail::CloudSessionRecord_Impl> impl,
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

