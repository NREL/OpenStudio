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

#ifndef PROJECT_OUTPUTATTRIBUTEVARIABLERECORD_HPP
#define PROJECT_OUTPUTATTRIBUTEVARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "OutputVariableRecord.hpp"

namespace openstudio {
namespace analysis {
  class OutputAttributeVariable;
}
namespace project {

namespace detail {

  class OutputAttributeVariableRecord_Impl;

} // detail

/** OutputAttributeVariableRecord is an OutputVariableRecord*/
class PROJECT_API OutputAttributeVariableRecord : public OutputVariableRecord {
 public:

  typedef detail::OutputAttributeVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  OutputAttributeVariableRecord(
      const analysis::OutputAttributeVariable& outputAttributeVariable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient);

  OutputAttributeVariableRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~OutputAttributeVariableRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<OutputAttributeVariableRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<OutputAttributeVariableRecord>
  getOutputAttributeVariableRecords(ProjectDatabase& database);

  static boost::optional<OutputAttributeVariableRecord>
  getOutputAttributeVariableRecord(int id, ProjectDatabase& database);

  //@}
  /** @name Getters */
  //@{

  analysis::OutputAttributeVariable outputAttributeVariable() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::OutputAttributeVariableRecord_Impl;

  /** Construct from impl. */
  OutputAttributeVariableRecord(
      std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl,
      ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit OutputAttributeVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.OutputAttributeVariableRecord");
};

/** \relates OutputAttributeVariableRecord*/
typedef boost::optional<OutputAttributeVariableRecord> OptionalOutputAttributeVariableRecord;

/** \relates OutputAttributeVariableRecord*/
typedef std::vector<OutputAttributeVariableRecord> OutputAttributeVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_OUTPUTATTRIBUTEVARIABLERECORD_HPP
