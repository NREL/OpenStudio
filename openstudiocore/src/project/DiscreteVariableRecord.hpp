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

