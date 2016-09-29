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

#ifndef PROJECT_CLOUDSETTINGSRECORD_HPP
#define PROJECT_CLOUDSETTINGSRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {

class CloudSettings;

namespace project {

namespace detail {

  class CloudSettingsRecord_Impl;

} // detail

/** \class CloudSettingsRecordType
 *  \brief ObjectRecord types that derive from CloudSettingsRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(CloudSettingsRecordType,
    ((VagrantSettingsRecord))
    ((AWSSettingsRecord))
);
 *  \endcode */
OPENSTUDIO_ENUM(CloudSettingsRecordType,
    ((VagrantSettingsRecord))
    ((AWSSettingsRecord))
);

/** \class CloudSettingsRecordColumns
 *  \brief Column definitions for the CloudSettingsRecords table.
 *
 *  \relates CloudSettingsRecord */
OPENSTUDIO_ENUM(CloudSettingsRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((cloudSettingsRecordType)(INTEGER)(8))
  ((userAgreementSigned)(BOOLEAN)(9))
  ((serverPath)(TEXT)(10))
  ((serverUrlRecordId)(INTEGER)(11))
  ((workerPath)(TEXT)(12))
  ((workerUrlRecordId)(INTEGER)(13))
  ((haltOnStop)(BOOLEAN)(14))
  ((username)(TEXT)(15))
  ((numWorkers)(INTEGER)(16))
  ((terminationDelayEnabled)(BOOLEAN)(17))
  ((terminationDelay)(INTEGER)(18))
  ((region)(TEXT)(19))
  ((serverInstanceType)(TEXT)(20))
  ((workerInstanceType)(TEXT)(21))
);

/** CloudSettingsRecord is a ObjectRecord. */
class PROJECT_API CloudSettingsRecord : public ObjectRecord {
 public:

  typedef detail::CloudSettingsRecord_Impl ImplType;
  typedef CloudSettingsRecordColumns ColumnsType;
  typedef CloudSettingsRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~CloudSettingsRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get CloudSettingsRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<CloudSettingsRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static CloudSettingsRecord factoryFromCloudSettings(const CloudSettings& cloudSettings, ProjectDatabase& database);

  static std::vector<CloudSettingsRecord> getCloudSettingsRecords(ProjectDatabase& database);

  static boost::optional<CloudSettingsRecord> getCloudSettingsRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  CloudSettings cloudSettings() const;

  //@}
 protected:
  /// @cond
  explicit CloudSettingsRecord(std::shared_ptr<detail::CloudSettingsRecord_Impl> impl);

  friend class detail::CloudSettingsRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  CloudSettingsRecord(std::shared_ptr<detail::CloudSettingsRecord_Impl> impl,
                      ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.CloudSettingsRecord");
};

/** \relates CloudSettingsRecord*/
typedef boost::optional<CloudSettingsRecord> OptionalCloudSettingsRecord;

/** \relates CloudSettingsRecord*/
typedef std::vector<CloudSettingsRecord> CloudSettingsRecordVector;

} // project
} // openstudio

#endif // PROJECT_CLOUDSETTINGSRECORD_HPP

