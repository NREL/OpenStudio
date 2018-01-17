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

#ifndef PROJECT_LINEARFUNCTIONRECORD_HPP
#define PROJECT_LINEARFUNCTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include "FunctionRecord.hpp"

namespace openstudio {
namespace analysis {
  class LinearFunction;
}
namespace project {

namespace detail {

  class LinearFunctionRecord_Impl;

} // detail

/** LinearFunctionRecord is a FunctionRecord*/
class PROJECT_API LinearFunctionRecord : public FunctionRecord {
 public:

  typedef detail::LinearFunctionRecord_Impl ImplType;
  typedef FunctionRecordColumns ColumnsType;
  typedef FunctionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  LinearFunctionRecord(const analysis::LinearFunction& linearFunction,
                       ProblemRecord& problemRecord,
                       const FunctionType& functionType,
                       int functionVectorIndex);

  LinearFunctionRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~LinearFunctionRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<LinearFunctionRecord> factoryFromQuery(const QSqlQuery& query,
                                                                ProjectDatabase& database);

  static std::vector<LinearFunctionRecord> getLinearFunctionRecords(ProjectDatabase& database);

  static boost::optional<LinearFunctionRecord> getLinearFunctionRecord(int id,
                                                                       ProjectDatabase& database);

  //@}
  /** @name Getters */
  //@{

  analysis::LinearFunction linearFunction() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::LinearFunctionRecord_Impl;

  /** Construct from impl. */
  LinearFunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl> impl,
                       ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit LinearFunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.LinearFunctionRecord");

  void removeVariableRecords(const std::vector<UUID>& uuidsToKeep,
                             ProjectDatabase& database);
};

/** \relates LinearFunctionRecord*/
typedef boost::optional<LinearFunctionRecord> OptionalLinearFunctionRecord;

/** \relates LinearFunctionRecord*/
typedef std::vector<LinearFunctionRecord> LinearFunctionRecordVector;

} // project
} // openstudio

#endif // PROJECT_LINEARFUNCTIONRECORD_HPP

