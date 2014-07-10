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

#ifndef PROJECT_VARIABLERECORD_HPP
#define PROJECT_VARIABLERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"

namespace openstudio {

namespace analysis {
  class Variable;
  class UncertaintyDescription;
}

namespace project {

class ProblemRecord;
class FunctionRecord;
class DiscretePerturbationRecord;
class AttributeRecord;

namespace detail{
  class VariableRecord_Impl;
}

/** \class VariableRecordType
 *  \brief ObjectRecord types that derive from VariableRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(VariableRecordType,
                ((InputVariableRecord)(InputVariableRecord)(0))
                ((OutputVariableRecord)(OutputVariableRecord)(1))
                );
 *  \endcode */
OPENSTUDIO_ENUM(VariableRecordType,
                ((InputVariableRecord)(InputVariableRecord)(0))
                ((OutputVariableRecord)(OutputVariableRecord)(1))
                );

/** \class VariableRecordColumns
 *  \brief Column definitions for the VariableRecords table.
 *  \details At some point, variableVectorIndex should be renamed to vectorIndex;
 *  rulesetRecordId shoudl be deprecated; and rubyPerturbationRecordId should be
 *  renamed to measureRecordId. For general information, see the
 *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(VariableRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((problemRecordId)(INTEGER)(8))
                ((functionRecordId)(INTEGER)(9))
                ((variableVectorIndex)(INTEGER)(10))
                ((functionCoefficient)(REAL)(11))
                ((variableRecordType)(INTEGER)(12))
                ((continuousVariableRecordType)(INTEGER)(13))
                ((minimum)(REAL)(14))
                ((maximum)(REAL)(15))
                ((increment)(REAL)(16))
                ((nSteps)(INTEGER)(17))
                ((attributeName)(TEXT)(18))
                ((rulesetRecordId)(INTEGER)(19))
                ((rubyPerturbationRecordId)(INTEGER)(20))
                ((uncertaintyDescriptionType)(INTEGER)(21))
                ((inputVariableRecordType)(INTEGER)(22))
                ((outputVariableRecordType)(INTEGER)(23))
                ((discreteVariableRecordType)(INTEGER)(24))
                );
 *  \endcode */
OPENSTUDIO_ENUM(VariableRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((problemRecordId)(INTEGER)(8))
                ((functionRecordId)(INTEGER)(9))
                ((variableVectorIndex)(INTEGER)(10))
                ((functionCoefficient)(REAL)(11))
                ((variableRecordType)(INTEGER)(12))
                ((continuousVariableRecordType)(INTEGER)(13))
                ((minimum)(REAL)(14))
                ((maximum)(REAL)(15))
                ((increment)(REAL)(16))
                ((nSteps)(INTEGER)(17))
                ((attributeName)(TEXT)(18))
                ((rulesetRecordId)(INTEGER)(19))
                ((rubyPerturbationRecordId)(INTEGER)(20))
                ((uncertaintyDescriptionType)(INTEGER)(21))
                ((inputVariableRecordType)(INTEGER)(22))
                ((outputVariableRecordType)(INTEGER)(23))
                ((discreteVariableRecordType)(INTEGER)(24))
                );

/** A VariableRecord is an ObjectRecord parented by a ProblemRecord. */
class PROJECT_API VariableRecord : public ObjectRecord {
 public:

  typedef detail::VariableRecord_Impl ImplType;
  typedef VariableRecordColumns ColumnsType;
  typedef VariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~VariableRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get VariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VariableRecord> factoryFromQuery(const QSqlQuery& query,
                                                          ProjectDatabase& database);

  /** Create a VariableRecord from variable that belongs to the function serialized to
   *  functionRecord. The index is the vector index of variable in function. */
  static VariableRecord factoryFromVariable(const analysis::Variable& variable,
                                            FunctionRecord& functionRecord,
                                            int variableVectorIndex,
                                            boost::optional<double> functionCoefficient);

  static std::vector<VariableRecord> getVariableRecords(ProjectDatabase& database);

  /// get variable by id
  static boost::optional<VariableRecord> getVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns this VariableRecord's parent FunctionRecord. */
  boost::optional<FunctionRecord> functionRecord() const;

  /** Index of this Variable in its parent's vector of Variables or WorkflowSteps. */
  int variableVectorIndex() const;

  boost::optional<double> functionCoefficient() const;

  analysis::Variable variable() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::VariableRecord_Impl;

  /** Construct from impl. */
  VariableRecord(std::shared_ptr<detail::VariableRecord_Impl> impl,
                 ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit VariableRecord(std::shared_ptr<detail::VariableRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.VariableRecord");
};

/** \relates VariableRecord*/
typedef boost::optional<VariableRecord> OptionalVariableRecord;

/** \relates VariableRecord*/
typedef std::vector<VariableRecord> VariableRecordVector;

/** Sorts VariableRecords by variableVectorIndex. */
struct PROJECT_API VariableRecordVariableVectorIndexLess {
  bool operator()(const VariableRecord& left, const VariableRecord& right) const;
};

} // project
} // openstudio

#endif // PROJECT_VARIABLERECORD_HPP
