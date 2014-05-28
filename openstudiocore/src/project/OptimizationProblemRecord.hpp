/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

