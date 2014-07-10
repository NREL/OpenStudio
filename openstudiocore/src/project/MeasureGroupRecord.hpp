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

#ifndef PROJECT_MEASUREGROUPRECORD_HPP
#define PROJECT_MEASUREGROUPRECORD_HPP

#include "ProjectAPI.hpp"
#include "DiscreteVariableRecord.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {

namespace analysis{
  class MeasureGroup;
}

namespace project {

class MeasureRecord;

namespace detail{
  class MeasureGroupRecord_Impl;
}

class PROJECT_API MeasureGroupRecord : public DiscreteVariableRecord {
 public:

  typedef detail::MeasureGroupRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  MeasureGroupRecord(const analysis::MeasureGroup& measureGroup,
                     ProblemRecord& problemRecord,
                     int workflowIndex);

  MeasureGroupRecord(const analysis::MeasureGroup& measureGroup,
                     FunctionRecord& functionRecord,
                     int variableVectorIndex,
                     boost::optional<double> functionCoefficient);

  MeasureGroupRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~MeasureGroupRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<MeasureGroupRecord> factoryFromQuery(const QSqlQuery& query,
                                                              ProjectDatabase& database);

  static std::vector<MeasureGroupRecord> getMeasureGroupRecords(ProjectDatabase& database);

  static boost::optional<MeasureGroupRecord> getMeasureGroupRecord(int id,
                                                                   ProjectDatabase& database);

  //@}

  analysis::MeasureGroup measureGroup() const;

  /// number of measures
  unsigned numMeasures(bool selectedMeasuresOnly) const;

  /// all measure ids
  std::vector<int> measureRecordIds(bool selectedMeasuresOnly) const;

  /// all measures
  std::vector<MeasureRecord> measureRecords(bool selectedMeasuresOnly) const;

  /** Get the MeasureRecord at index. Throws if index >= numMeasures(false). */
  MeasureRecord getMeasureRecord(int vectorIndex) const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::MeasureGroupRecord_Impl;

  /** Construct from impl. */
  MeasureGroupRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl> impl,
                         ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit MeasureGroupRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.MeasureGroupRecord");

  void constructMeasureRecords(const analysis::MeasureGroup& measureGroup);

  void removeMeasureRecords(const std::vector<UUID>& uuidsToKeep,
                            ProjectDatabase& database);
};

/** \relates MeasureGroupRecord*/
typedef boost::optional<MeasureGroupRecord> OptionalMeasureGroupRecord;

/** \relates MeasureGroupRecord*/
typedef std::vector<MeasureGroupRecord> MeasureGroupRecordVector;

} // project
} // openstudio

#endif // PROJECT_MEASUREGROUPRECORD_HPP
