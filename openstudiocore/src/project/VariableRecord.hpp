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

#ifndef PROJECT_VARIABLERECORD_HPP
#define PROJECT_VARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"

namespace openstudio {

namespace analysis {
  class Variable;
  class UncertaintyDescription;
}

namespace project {

class ProblemRecord;
class FunctionRecord;
class DiscretePerturbationRecord;
class AttributeRecord;

namespace detail{
  class VariableRecord_Impl;
}

/** \class VariableRecordType
 *  \brief ObjectRecord types that derive from VariableRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(VariableRecordType,
                ((InputVariableRecord)(InputVariableRecord)(0))
                ((OutputVariableRecord)(OutputVariableRecord)(1))
                );
 *  \endcode */
OPENSTUDIO_ENUM(VariableRecordType,
                ((InputVariableRecord)(InputVariableRecord)(0))
                ((OutputVariableRecord)(OutputVariableRecord)(1))
                );

/** \class VariableRecordColumns
 *  \brief Column definitions for the VariableRecords table.
 *  \details At some point, variableVectorIndex should be renamed to vectorIndex;
 *  rulesetRecordId shoudl be deprecated; and rubyPerturbationRecordId should be
 *  renamed to measureRecordId. For general information, see the
 *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(VariableRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((problemRecordId)(INTEGER)(8))
                ((functionRecordId)(INTEGER)(9))
                ((variableVectorIndex)(INTEGER)(10))
                ((functionCoefficient)(REAL)(11))
                ((variableRecordType)(INTEGER)(12))
                ((continuousVariableRecordType)(INTEGER)(13))
                ((minimum)(REAL)(14))
                ((maximum)(REAL)(15))
                ((increment)(REAL)(16))
                ((nSteps)(INTEGER)(17))
                ((attributeName)(TEXT)(18))
                ((rulesetRecordId)(INTEGER)(19))
                ((rubyPerturbationRecordId)(INTEGER)(20))
                ((uncertaintyDescriptionType)(INTEGER)(21))
                ((inputVariableRecordType)(INTEGER)(22))
                ((outputVariableRecordType)(INTEGER)(23))
                ((discreteVariableRecordType)(INTEGER)(24))
                );
 *  \endcode */
OPENSTUDIO_ENUM(VariableRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((problemRecordId)(INTEGER)(8))
                ((functionRecordId)(INTEGER)(9))
                ((variableVectorIndex)(INTEGER)(10))
                ((functionCoefficient)(REAL)(11))
                ((variableRecordType)(INTEGER)(12))
                ((continuousVariableRecordType)(INTEGER)(13))
                ((minimum)(REAL)(14))
                ((maximum)(REAL)(15))
                ((increment)(REAL)(16))
                ((nSteps)(INTEGER)(17))
                ((attributeName)(TEXT)(18))
                ((rulesetRecordId)(INTEGER)(19))
                ((rubyPerturbationRecordId)(INTEGER)(20))
                ((uncertaintyDescriptionType)(INTEGER)(21))
                ((inputVariableRecordType)(INTEGER)(22))
                ((outputVariableRecordType)(INTEGER)(23))
                ((discreteVariableRecordType)(INTEGER)(24))
                );

/** A VariableRecord is an ObjectRecord parented by a ProblemRecord. */
class PROJECT_API VariableRecord : public ObjectRecord {
 public:

  typedef detail::VariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~VariableRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get VariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                          ProjectDatabase& database);

  /** Create a VariableRecord from variable that belongs to the function serialized to
   *  functionRecord. The index is the vector index of variable in function. */
  static VariableRecord factoryFromVariable(const analysis::Variable& variable,
                                            FunctionRecord& functionRecord,
                                            int variableVectorIndex,
                                            boost::optional<double> functionCoefficient);

  static std::vector<VariableRecord> getVariableRecords(ProjectDatabase& database);

  /// get variable by id
  static boost::optional<VariableRecord> getVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns this VariableRecord's parent FunctionRecord. */
  boost::optional<FunctionRecord> functionRecord() const;

  /** Index of this Variable in its parent's vector of Variables or WorkflowSteps. */
  int variableVectorIndex() const;

  boost::optional<double> functionCoefficient() const;

  analysis::Variable variable() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::VariableRecord_Impl;

  /** Construct from impl. */
  VariableRecord(std::shared_ptr<detail::VariableRecord_Impl> impl,
                 ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit VariableRecord(std::shared_ptr<detail::VariableRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.VariableRecord");
};

/** \relates VariableRecord*/
typedef boost::optional<VariableRecord> OptionalVariableRecord;

/** \relates VariableRecord*/
typedef std::vector<VariableRecord> VariableRecordVector;

/** Sorts VariableRecords by variableVectorIndex. */
struct PROJECT_API VariableRecordVariableVectorIndexLess {
  bool operator()(const VariableRecord& left, const VariableRecord& right) const;
};

} // project
} // openstudio

#endif // PROJECT_VARIABLERECORD_HPP
