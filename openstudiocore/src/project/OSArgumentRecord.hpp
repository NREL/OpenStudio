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

#ifndef PROJECT_OSARGUMENTRECORD_HPP
#define PROJECT_OSARGUMENTRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../ruleset/OSArgument.hpp"

namespace openstudio {
namespace project {

class RubyMeasureRecord;
class RubyContinuousVariableRecord;

namespace detail {

  class OSArgumentRecord_Impl;

} // detail

/** \class OSArgumentRecordColumns
 *  \brief Column definitions for the OSArgumentRecords table.
 *  \details At some point, rubyPerturbationRecordId should be renamed to
 *  rubyMeasureRecordId. For general information, see the
 *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(OSArgumentRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((rubyPerturbationRecordId)(INTEGER)(8))
  ((rubyContinuousVariableRecordId)(INTEGER)(9))
  ((argumentType)(INTEGER)(10))
  ((required)(BOOLEAN)(11))
  ((argumentValue)(TEXT)(12))
  ((defaultArgumentValue)(TEXT)(13))
  ((domainType)(INTEGER)(14))
  ((domainValues)(TEXT)(15))
  ((choices)(TEXT)(16))
  ((choiceDisplayNames)(TEXT)(17))
  ((isRead)(BOOLEAN)(18))
  ((extension)(TEXT)(19))
  ((units)(TEXT)(20))
  ((modelDependent)(BOOLEAN)(21))
);
 *  \endcode */
OPENSTUDIO_ENUM(OSArgumentRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((rubyPerturbationRecordId)(INTEGER)(8))
  ((rubyContinuousVariableRecordId)(INTEGER)(9))
  ((argumentType)(INTEGER)(10))
  ((required)(BOOLEAN)(11))
  ((argumentValue)(TEXT)(12))
  ((defaultArgumentValue)(TEXT)(13))
  ((domainType)(INTEGER)(14))
  ((domainValues)(TEXT)(15))
  ((choices)(TEXT)(16))
  ((choiceDisplayNames)(TEXT)(17))
  ((isRead)(BOOLEAN)(18))
  ((extension)(TEXT)(19))
  ((units)(TEXT)(20))
  ((modelDependent)(BOOLEAN)(21))
);

/** OSArgumentRecord is a ObjectRecord. */
class PROJECT_API OSArgumentRecord : public ObjectRecord {
 public:

  typedef detail::OSArgumentRecord_Impl ImplType;
  typedef OSArgumentRecordColumns ColumnsType;
  typedef OSArgumentRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  OSArgumentRecord(const ruleset::OSArgument& osArgument,
                   RubyMeasureRecord& rubyMeasureRecord);

  OSArgumentRecord(const ruleset::OSArgument& osArgument,
                   RubyContinuousVariableRecord& rubyContinuousVariableRecord);

  OSArgumentRecord(const QSqlQuery& query, ProjectDatabase& database);

  /** Construct from impl. */
  OSArgumentRecord(std::shared_ptr<detail::OSArgumentRecord_Impl> impl,
                   ProjectDatabase database);

  virtual ~OSArgumentRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<OSArgumentRecord> factoryFromQuery(const QSqlQuery& query,
                                                            ProjectDatabase& database);

  static std::vector<OSArgumentRecord> getOSArgumentRecords(ProjectDatabase& database);

  static boost::optional<OSArgumentRecord> getOSArgumentRecord(int id,
                                                               ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Get the RubyMeasureRecord that parents this OSArgumentRecord. */
  boost::optional<RubyMeasureRecord> rubyMeasureRecord() const;

  /** Get the RubyContinuousVariableRecord that parents this OSArgumentRecord. */
  boost::optional<RubyContinuousVariableRecord> rubyContinuousVariableRecord() const;

  ruleset::OSArgument osArgument() const;

  ruleset::OSArgumentType type() const;

  bool hasValue() const;

  bool hasDefaultValue() const;

  openstudio::path valueAsPath() const;

  openstudio::path defaultValueAsPath() const;

  //@}
 protected:
  /// @cond
  explicit OSArgumentRecord(std::shared_ptr<detail::OSArgumentRecord_Impl> impl);

  friend class detail::OSArgumentRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.OSArgumentRecord");
};

/** \relates OSArgumentRecord*/
typedef boost::optional<OSArgumentRecord> OptionalOSArgumentRecord;

/** \relates OSArgumentRecord*/
typedef std::vector<OSArgumentRecord> OSArgumentRecordVector;

} // project
} // openstudio

#endif // PROJECT_OSARGUMENTRECORD_HPP

