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

#ifndef PROJECT_MODELRULESETCONTINUOUSVARIABLERECORD_HPP
#define PROJECT_MODELRULESETCONTINUOUSVARIABLERECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/ContinuousVariableRecord.hpp>

namespace openstudio {
namespace analysis {
  class ModelRulesetContinuousVariable;
}
namespace project {

class ModelRulesetRecord;

namespace detail {

  class ModelRulesetContinuousVariableRecord_Impl;

} // detail

/** ModelRulesetContinuousVariableRecord is a ContinuousVariableRecord*/
class PROJECT_API ModelRulesetContinuousVariableRecord : public ContinuousVariableRecord {
 public:

  typedef detail::ModelRulesetContinuousVariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  ModelRulesetContinuousVariableRecord(
      const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable, 
      ProblemRecord& problemRecord,
      int variableVectorIndex);

  ModelRulesetContinuousVariableRecord(
      const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable, 
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient);

  ModelRulesetContinuousVariableRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~ModelRulesetContinuousVariableRecord() {}

  //@}

  static boost::optional<ModelRulesetContinuousVariableRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<ModelRulesetContinuousVariableRecord> 
  getModelRulesetContinuousVariableRecords(ProjectDatabase& database);

  static boost::optional<ModelRulesetContinuousVariableRecord> 
  getModelRulesetContinuousVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  ModelRulesetRecord modelRulesetRecord() const;

  analysis::ModelRulesetContinuousVariable modelRulesetContinuousVariable() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelRulesetContinuousVariableRecord_Impl;

  /** Construct from impl. */
  ModelRulesetContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl> impl,
                                       ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelRulesetContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelRulesetContinuousVariableRecord");
};

/** \relates ModelRulesetContinuousVariableRecord*/
typedef boost::optional<ModelRulesetContinuousVariableRecord> OptionalModelRulesetContinuousVariableRecord;

/** \relates ModelRulesetContinuousVariableRecord*/
typedef std::vector<ModelRulesetContinuousVariableRecord> ModelRulesetContinuousVariableRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELRULESETCONTINUOUSVARIABLERECORD_HPP

