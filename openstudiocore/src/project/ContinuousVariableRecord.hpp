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

#ifndef PROJECT_CONTINUOUSVARIABLERECORD_HPP
#define PROJECT_CONTINUOUSVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "InputVariableRecord.hpp"

namespace openstudio {
namespace analysis {
  class ContinuousVariable;
}
namespace project {

namespace detail {

  class ContinuousVariableRecord_Impl;

} // detail

/** \class ContinuousVariableRecordType
 *  \brief ObjectRecord types that derive from ContinuousVariableRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(ContinuousVariableRecordType,
  ((RubyContinuousVariableRecord))
);
 *  \endcode */
OPENSTUDIO_ENUM(ContinuousVariableRecordType,
  ((RubyContinuousVariableRecord))
);

/** ContinuousVariableRecord is an InputVariableRecord*/
class PROJECT_API ContinuousVariableRecord : public InputVariableRecord {
 public:

  typedef detail::ContinuousVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~ContinuousVariableRecord() {}

  //@}

  /** Get ContinuousVariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<ContinuousVariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                                    ProjectDatabase& database);

  /** Create a ContinuousVariableRecord from variable that belongs to the problem serialized to
   *  ProblemRecord. The index is the vector index of variable's WorkflowStep in problem. */
  static ContinuousVariableRecord factoryFromContinuousVariable(
      const analysis::ContinuousVariable& variable,
      ProblemRecord& problemRecord,
      int workflowIndex);

  /** Create a ContinuousVariableRecord from variable that belongs to the function serialized to
   *  functionRecord. The index is the vector index of variable in function. */
  static ContinuousVariableRecord factoryFromContinuousVariable(
      const analysis::ContinuousVariable& variable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient);

  static std::vector<ContinuousVariableRecord> getContinuousVariableRecords(
      ProjectDatabase& database);

  static boost::optional<ContinuousVariableRecord> getContinuousVariableRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  boost::optional<double> minimum() const;

  boost::optional<double> maximum() const;

  boost::optional<double> increment() const;

  boost::optional<int> nSteps() const;

  analysis::ContinuousVariable continuousVariable() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ContinuousVariableRecord_Impl;

  /** Construct from impl. */
  ContinuousVariableRecord(std::shared_ptr<detail::ContinuousVariableRecord_Impl> impl,
                           ProjectDatabase database,
                           const boost::optional<analysis::ContinuousVariable>& variable);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ContinuousVariableRecord(std::shared_ptr<detail::ContinuousVariableRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ContinuousVariableRecord");
};

/** \relates ContinuousVariableRecord*/
typedef boost::optional<ContinuousVariableRecord> OptionalContinuousVariableRecord;

/** \relates ContinuousVariableRecord*/
typedef std::vector<ContinuousVariableRecord> ContinuousVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_CONTINUOUSVARIABLERECORD_HPP

