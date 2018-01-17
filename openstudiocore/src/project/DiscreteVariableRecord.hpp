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

#ifndef PROJECT_DISCRETEVARIABLERECORD_HPP
#define PROJECT_DISCRETEVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "InputVariableRecord.hpp"

namespace openstudio {
namespace analysis {
  class DiscreteVariable;
}
namespace project {

namespace detail {

  class DiscreteVariableRecord_Impl;

} // detail

/** \class DiscreteVariableRecordType
 *  \brief ObjectRecord types that derive from DiscreteVariableRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(DiscreteVariableRecordType,
    ((MeasureGroupRecord))
);
 *  \endcode */
OPENSTUDIO_ENUM(DiscreteVariableRecordType,
    ((MeasureGroupRecord))
);

/** DiscreteVariableRecord is a InputVariableRecord. */
class PROJECT_API DiscreteVariableRecord : public InputVariableRecord {
 public:

  typedef detail::DiscreteVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~DiscreteVariableRecord() {}

  //@}

  /** Get DiscreteVariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<DiscreteVariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                                  ProjectDatabase& database);

  /** Create a DiscreteVariableRecord from discreteVariable that belongs to the problem serialized to
   *  ProblemRecord. The index is the vector index of discreteVariable's WorkflowStep in problem. */
  static DiscreteVariableRecord factoryFromDiscreteVariable(const analysis::DiscreteVariable& discreteVariable,
                                                            ProblemRecord& problemRecord,
                                                            int workflowIndex);

  /** Create a DiscreteVariableRecord from discreteVariable that belongs to the function serialized to
   *  functionRecord. The index is the vector index of discreteVariable in function. */
  static DiscreteVariableRecord factoryFromDiscreteVariable(const analysis::DiscreteVariable& discreteVariable,
                                                            FunctionRecord& functionRecord,
                                                            int variableVectorIndex,
                                                            boost::optional<double> functionCoefficient);

  static std::vector<DiscreteVariableRecord> getDiscreteVariableRecords(ProjectDatabase& database);

  static boost::optional<DiscreteVariableRecord> getDiscreteVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::DiscreteVariable discreteVariable() const;

  //@}
 protected:
  /// @cond
  explicit DiscreteVariableRecord(std::shared_ptr<detail::DiscreteVariableRecord_Impl> impl);

  friend class detail::DiscreteVariableRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  DiscreteVariableRecord(std::shared_ptr<detail::DiscreteVariableRecord_Impl> impl,
                         ProjectDatabase database,
                         const boost::optional<analysis::DiscreteVariable>& discreteVariable);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.DiscreteVariableRecord");
};

/** \relates DiscreteVariableRecord*/
typedef boost::optional<DiscreteVariableRecord> OptionalDiscreteVariableRecord;

/** \relates DiscreteVariableRecord*/
typedef std::vector<DiscreteVariableRecord> DiscreteVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_DISCRETEVARIABLERECORD_HPP

