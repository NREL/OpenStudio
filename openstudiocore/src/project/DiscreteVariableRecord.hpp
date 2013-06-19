/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef PROJECT_DISCRETEVARIABLERECORD_HPP
#define PROJECT_DISCRETEVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/InputVariableRecord.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>

namespace openstudio {

namespace analysis{
  class DiscreteVariable;
}

namespace project {

namespace detail{
  class DiscreteVariableRecord_Impl;
}

class PROJECT_API DiscreteVariableRecord : public InputVariableRecord {
 public:

  typedef detail::DiscreteVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  DiscreteVariableRecord(const analysis::DiscreteVariable& discreteVariable,
                         ProblemRecord& problemRecord,
                         int variableVectorIndex);

  DiscreteVariableRecord(const analysis::DiscreteVariable& discreteVariable,
                         FunctionRecord& functionRecord,
                         int variableVectorIndex,
                         boost::optional<double> functionCoefficient);

  DiscreteVariableRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~DiscreteVariableRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<DiscreteVariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                                  ProjectDatabase& database);

  static std::vector<DiscreteVariableRecord> getDiscreteVariableRecords(ProjectDatabase& database);

  static boost::optional<DiscreteVariableRecord> getDiscreteVariableRecord(
      int id, ProjectDatabase& database);

  //@}

  analysis::DiscreteVariable discreteVariable() const;

  /// number of discrete perturbations
  unsigned numPerturbations(bool selectedPerturbationsOnly) const;

  /// all discrete perturbation ids
  std::vector<int> discretePerturbationRecordIds(bool selectedPerturbationsOnly) const;

  /// all discrete perturbations
  std::vector<DiscretePerturbationRecord> discretePerturbationRecords(
      bool selectedPerturbationsOnly) const;

  /** Get the DiscretePerturbationRecord at index. Throws if index >= numPerturbations(false). */
  DiscretePerturbationRecord getDiscretePerturbationRecord(int perturbationVectorIndex) const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::DiscreteVariableRecord_Impl;

  /** Construct from impl. */
  DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl,
                         ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.DiscreteVariableRecord");

  void constructDiscretePerturbationRecords(const analysis::DiscreteVariable& discreteVariable);

  void removeDiscretePerturbationRecords(const std::vector<UUID>& uuidsToKeep,
                                         ProjectDatabase& database);
};

/** \relates DiscreteVariableRecord*/
typedef boost::optional<DiscreteVariableRecord> OptionalDiscreteVariableRecord;

/** \relates DiscreteVariableRecord*/
typedef std::vector<DiscreteVariableRecord> DiscreteVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_DISCRETEVARIABLERECORD_HPP
