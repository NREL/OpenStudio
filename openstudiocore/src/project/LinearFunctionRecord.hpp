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

#ifndef PROJECT_LINEARFUNCTIONRECORD_HPP
#define PROJECT_LINEARFUNCTIONRECORD_HPP

#include "ProjectAPI.hpp"
#include "FunctionRecord.hpp"

namespace openstudio {
namespace analysis {
  class LinearFunction;
}
namespace project {

namespace detail {

  class LinearFunctionRecord_Impl;

} // detail

/** LinearFunctionRecord is a FunctionRecord*/
class PROJECT_API LinearFunctionRecord : public FunctionRecord {
 public:

  typedef detail::LinearFunctionRecord_Impl ImplType;
  typedef FunctionRecordColumns ColumnsType;
  typedef FunctionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  LinearFunctionRecord(const analysis::LinearFunction& linearFunction,
                       ProblemRecord& problemRecord,
                       const FunctionType& functionType,
                       int functionVectorIndex);

  LinearFunctionRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~LinearFunctionRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<LinearFunctionRecord> factoryFromQuery(const QSqlQuery& query,
                                                                ProjectDatabase& database);

  static std::vector<LinearFunctionRecord> getLinearFunctionRecords(ProjectDatabase& database);

  static boost::optional<LinearFunctionRecord> getLinearFunctionRecord(int id,
                                                                       ProjectDatabase& database);

  //@}
  /** @name Getters */
  //@{

  analysis::LinearFunction linearFunction() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::LinearFunctionRecord_Impl;

  /** Construct from impl. */
  LinearFunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl> impl,
                       ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit LinearFunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.LinearFunctionRecord");

  void removeVariableRecords(const std::vector<UUID>& uuidsToKeep,
                             ProjectDatabase& database);
};

/** \relates LinearFunctionRecord*/
typedef boost::optional<LinearFunctionRecord> OptionalLinearFunctionRecord;

/** \relates LinearFunctionRecord*/
typedef std::vector<LinearFunctionRecord> LinearFunctionRecordVector;

} // project
} // openstudio

#endif // PROJECT_LINEARFUNCTIONRECORD_HPP

