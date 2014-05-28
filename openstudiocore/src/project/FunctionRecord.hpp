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

#ifndef PROJECT_FUNCTIONRECORD_HPP
#define PROJECT_FUNCTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {
namespace analysis {
  class Function;
}
namespace project {

class ProblemRecord;
class VariableRecord;

namespace detail {

  class FunctionRecord_Impl;

} // detail

/** \class FunctionType
 *  \brief Type of function from a problem formulation perspective.
 *
 *  \relates FunctionRecord */
OPENSTUDIO_ENUM(FunctionType,
                ((Response))
                ((Objective))
);

/** \class FunctionRecordType
 *  \relates FunctionRecord */
OPENSTUDIO_ENUM(FunctionRecordType,
  ((LinearFunctionRecord))
);

/** \class FunctionRecordColumns
 *  \brief Column definitions for the FunctionRecords table.
 *
 *  \relates FunctionRecord */
OPENSTUDIO_ENUM(FunctionRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((problemRecordId)(INTEGER)(8))
  ((functionVectorIndex)(INTEGER)(9))
  ((functionRecordType)(INTEGER)(10))
  ((functionType)(INTEGER)(11))
);

/** FunctionRecord is an ObjectRecord. */
class PROJECT_API FunctionRecord : public ObjectRecord {
 public:

  typedef detail::FunctionRecord_Impl ImplType;
  typedef FunctionRecordColumns ColumnsType;
  typedef FunctionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~FunctionRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get FunctionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<FunctionRecord> factoryFromQuery(const QSqlQuery& query,
                                                          ProjectDatabase& database);

  static FunctionRecord factoryFromFunction(const analysis::Function& function,
                                            ProblemRecord& problemRecord,
                                            const FunctionType& functionType,
                                            int functionVectorIndex);

  static std::vector<FunctionRecord> getFunctionRecords(ProjectDatabase& database);

  static boost::optional<FunctionRecord> getFunctionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  ProblemRecord problemRecord() const;

  std::vector<VariableRecord> variableRecords() const;

  FunctionType functionType() const;

  int functionVectorIndex() const;

  analysis::Function function() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::FunctionRecord_Impl;

  /** @name Constructors and Destructors */
  //@{

  /** Construct from impl. */
  FunctionRecord(std::shared_ptr<detail::FunctionRecord_Impl> impl,
                 ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit FunctionRecord(std::shared_ptr<detail::FunctionRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.FunctionRecord");
};

/** \relates FunctionRecord*/
typedef boost::optional<FunctionRecord> OptionalFunctionRecord;

/** \relates FunctionRecord*/
typedef std::vector<FunctionRecord> FunctionRecordVector;

/** Sorts FunctionRecords by functionVectorIndex. */
struct PROJECT_API FunctionRecordFunctionVectorIndexLess {
  bool operator()(const FunctionRecord& left, const FunctionRecord& right) const;
};

} // project
} // openstudio

#endif // PROJECT_FUNCTIONRECORD_HPP

