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

#ifndef PROJECT_FUNCTIONRECORD_HPP
#define PROJECT_FUNCTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {
namespace analysis {
  class Function;
}
namespace project {

class ProblemRecord;
class VariableRecord;

namespace detail {

  class FunctionRecord_Impl;

} // detail

/** \class FunctionType
 *  \brief Type of function from a problem formulation perspective.
 *
 *  \relates FunctionRecord */
OPENSTUDIO_ENUM(FunctionType,
                ((Response))
                ((Objective))
);

/** \class FunctionRecordType
 *  \relates FunctionRecord */
OPENSTUDIO_ENUM(FunctionRecordType,
  ((LinearFunctionRecord))
);

/** \class FunctionRecordColumns
 *  \brief Column definitions for the FunctionRecords table.
 *
 *  \relates FunctionRecord */
OPENSTUDIO_ENUM(FunctionRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((problemRecordId)(INTEGER)(8))
  ((functionVectorIndex)(INTEGER)(9))
  ((functionRecordType)(INTEGER)(10))
  ((functionType)(INTEGER)(11))
);

/** FunctionRecord is an ObjectRecord. */
class PROJECT_API FunctionRecord : public ObjectRecord {
 public:

  typedef detail::FunctionRecord_Impl ImplType;
  typedef FunctionRecordColumns ColumnsType;
  typedef FunctionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~FunctionRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get FunctionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<FunctionRecord> factoryFromQuery(const QSqlQuery& query,
                                                          ProjectDatabase& database);

  static FunctionRecord factoryFromFunction(const analysis::Function& function,
                                            ProblemRecord& problemRecord,
                                            const FunctionType& functionType,
                                            int functionVectorIndex);

  static std::vector<FunctionRecord> getFunctionRecords(ProjectDatabase& database);

  static boost::optional<FunctionRecord> getFunctionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  ProblemRecord problemRecord() const;

  std::vector<VariableRecord> variableRecords() const;

  FunctionType functionType() const;

  int functionVectorIndex() const;

  analysis::Function function() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::FunctionRecord_Impl;

  /** @name Constructors and Destructors */
  //@{

  /** Construct from impl. */
  FunctionRecord(std::shared_ptr<detail::FunctionRecord_Impl> impl,
                 ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit FunctionRecord(std::shared_ptr<detail::FunctionRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.FunctionRecord");
};

/** \relates FunctionRecord*/
typedef boost::optional<FunctionRecord> OptionalFunctionRecord;

/** \relates FunctionRecord*/
typedef std::vector<FunctionRecord> FunctionRecordVector;

/** Sorts FunctionRecords by functionVectorIndex. */
struct PROJECT_API FunctionRecordFunctionVectorIndexLess {
  bool operator()(const FunctionRecord& left, const FunctionRecord& right) const;
};

} // project
} // openstudio

#endif // PROJECT_FUNCTIONRECORD_HPP

