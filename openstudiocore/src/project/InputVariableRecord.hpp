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

#ifndef PROJECT_INPUTVARIABLERECORD_HPP
#define PROJECT_INPUTVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "VariableRecord.hpp"

namespace openstudio {
namespace analysis {
  class InputVariable;
}

namespace project {

namespace detail {

  class InputVariableRecord_Impl;

} // detail

/** \class InputVariableRecordType
 *  \brief ObjectRecord types that derive from InputVariableRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(InputVariableRecordType,
                ((DiscreteVariableRecord)(DiscreteVariableRecord)(0))
                ((ContinuousVariableRecord)(ContinuousVariableRecord)(1))
                );
 *  \endcode */
OPENSTUDIO_ENUM(InputVariableRecordType,
                ((DiscreteVariableRecord)(DiscreteVariableRecord)(0))
                ((ContinuousVariableRecord)(ContinuousVariableRecord)(1))
                );

/** InputVariableRecord is a VariableRecord. */
class PROJECT_API InputVariableRecord : public VariableRecord {
 public:
  typedef detail::InputVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~InputVariableRecord() {}

  //@}

  /** Get InputVariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<InputVariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                               ProjectDatabase& database);

  /** Create an InputVariableRecord from inputVariable that belongs to the problem serialized to
   *  ProblemRecord. The index is the vector index of inputVariable's WorkflowStep in problem. */
  static InputVariableRecord factoryFromInputVariable(const analysis::InputVariable& inputVariable,
                                                      ProblemRecord& problemRecord,
                                                      int workflowIndex);

  /** Create an InputVariableRecord from inputVariable that belongs to the function serialized to
   *  functionRecord. The index is the vector index of inputVariable in function. */
  static InputVariableRecord factoryFromInputVariable(const analysis::InputVariable& inputVariable,
                                                      FunctionRecord& functionRecord,
                                                      int variableVectorIndex,
                                                      boost::optional<double> functionCoefficient);

  static std::vector<InputVariableRecord> getInputVariableRecords(ProjectDatabase& database);

  static boost::optional<InputVariableRecord> getInputVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns this InputVariableRecord's parent ProblemRecord. */
  boost::optional<ProblemRecord> problemRecord() const;

  /** Returns this InputVariableRecord's AttributeRecords, which are used to store
   *  uncertainty description information. */
  std::vector<AttributeRecord> attributeRecords() const;

  boost::optional<analysis::UncertaintyDescription> uncertaintyDescription() const;

  analysis::InputVariable inputVariable() const;

  //@}
 protected:
  /// @cond
  explicit InputVariableRecord(std::shared_ptr<detail::InputVariableRecord_Impl> impl);

  friend class detail::InputVariableRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  InputVariableRecord(std::shared_ptr<detail::InputVariableRecord_Impl> impl,
                      ProjectDatabase database,
                      const boost::optional<analysis::InputVariable>& inputVariable);

  virtual void constructRelatedRecords(const analysis::InputVariable& inputVariable);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.InputVariableRecord");
};

/** \relates InputVariableRecord*/
typedef boost::optional<InputVariableRecord> OptionalInputVariableRecord;

/** \relates InputVariableRecord*/
typedef std::vector<InputVariableRecord> InputVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_INPUTVARIABLERECORD_HPP

