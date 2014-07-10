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

#ifndef PROJECT_OUTPUTVARIABLERECORD_HPP
#define PROJECT_OUTPUTVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "VariableRecord.hpp"

namespace openstudio {
namespace analysis {
  class OutputVariable;
}

namespace project {

namespace detail {

  class OutputVariableRecord_Impl;

} // detail

/** \class OutputVariableRecordType
 *  \brief ObjectRecord types that derive from OutputVariableRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(OutputVariableRecordType,
                ((OutputAttributeVariableRecord)(OutputAttributeVariableRecord)(0))
                );
 *  \endcode */
OPENSTUDIO_ENUM(OutputVariableRecordType,
                ((OutputAttributeVariableRecord)(OutputAttributeVariableRecord)(0))
                );

/** OutputVariableRecord is a VariableRecord. */
class PROJECT_API OutputVariableRecord : public VariableRecord {
 public:
  typedef detail::OutputVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~OutputVariableRecord() {}

  //@}

  /** Get OutputVariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<OutputVariableRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  /** Create an OutputVariableRecord from variable that belongs to the function serialized to
   *  functionRecord. The index is the vector index of variable in function. */
  static OutputVariableRecord factoryFromOutputVariable(const analysis::OutputVariable& outputVariable,
                                                        FunctionRecord& functionRecord,
                                                        int variableVectorIndex,
                                                        boost::optional<double> functionCoefficient);

  static std::vector<OutputVariableRecord> getOutputVariableRecords(ProjectDatabase& database);

  static boost::optional<OutputVariableRecord> getOutputVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::OutputVariable outputVariable() const;

  //@}
 protected:
  /// @cond
  explicit OutputVariableRecord(std::shared_ptr<detail::OutputVariableRecord_Impl> impl);

  friend class detail::OutputVariableRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  OutputVariableRecord(std::shared_ptr<detail::OutputVariableRecord_Impl> impl,
                       ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.OutputVariableRecord");
};

/** \relates OutputVariableRecord*/
typedef boost::optional<OutputVariableRecord> OptionalOutputVariableRecord;

/** \relates OutputVariableRecord*/
typedef std::vector<OutputVariableRecord> OutputVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_OUTPUTVARIABLERECORD_HPP
