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

