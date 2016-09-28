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

#ifndef PROJECT_OPTIMIZATIONPROBLEMRECORD_HPP
#define PROJECT_OPTIMIZATIONPROBLEMRECORD_HPP

#include "ProjectAPI.hpp"
#include "ProblemRecord.hpp"

namespace openstudio {
namespace analysis {
  class OptimizationProblem;
}
namespace project {

class FunctionRecord;

namespace detail {

  class OptimizationProblemRecord_Impl;

} // detail

/** OptimizationProblemRecord is a ProblemRecord*/
class PROJECT_API OptimizationProblemRecord : public ProblemRecord {
 public:

  typedef detail::OptimizationProblemRecord_Impl ImplType;
  typedef ProblemRecordColumns ColumnsType;
  typedef ProblemRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  OptimizationProblemRecord(const analysis::OptimizationProblem& optimizationProblem,
                            ProjectDatabase& database);

  OptimizationProblemRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~OptimizationProblemRecord() {}

  //@}

  static boost::optional<OptimizationProblemRecord> factoryFromQuery(const QSqlQuery& query,
                                                                     ProjectDatabase& database);

  static std::vector<OptimizationProblemRecord> getOptimizationProblemRecords(
      ProjectDatabase& database);

  static boost::optional<OptimizationProblemRecord> getOptimizationProblemRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  std::vector<FunctionRecord> objectiveRecords() const;

  analysis::OptimizationProblem optimizationProblem() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::OptimizationProblemRecord_Impl;

  /** Construct from impl. */
  OptimizationProblemRecord(std::shared_ptr<detail::OptimizationProblemRecord_Impl> impl,
                            ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit OptimizationProblemRecord(std::shared_ptr<detail::OptimizationProblemRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.OptimizationProblemRecord");

  void removeObjectiveRecords(const std::vector<UUID>& uuidsToKeep,
                              ProjectDatabase& database);
};

/** \relates OptimizationProblemRecord*/
typedef boost::optional<OptimizationProblemRecord> OptionalOptimizationProblemRecord;

/** \relates OptimizationProblemRecord*/
typedef std::vector<OptimizationProblemRecord> OptimizationProblemRecordVector;

} // project
} // openstudio

#endif // PROJECT_OPTIMIZATIONPROBLEMRECORD_HPP

