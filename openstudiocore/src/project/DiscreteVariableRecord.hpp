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

#include <project/ProjectAPI.hpp>
#include <project/InputVariableRecord.hpp>

namespace openstudio {
namespace project {

namespace detail {

  class DiscreteVariableRecord_Impl;

} // detail

/** DiscreteVariableRecord is a InputVariableRecord. */
class PROJECT_API DiscreteVariableRecord : public InputVariableRecord {
 public:
  typedef detail::DiscreteVariableRecord_Impl ImplType;
  // TODO: Check this typedef. The generator script assumes that the hierarchy is only two deep   // from ObjectRecord.
  typedef InputVariableRecordColumns ColumnsType;
  // TODO: Check this typedef too.
  typedef InputVariableRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  // TODO: Delete if DiscreteVariable is abstract, make private if DiscreteVariable has derived classes.
  // TODO: Replace ProjectDatabase& database (or add another object if it is ok for DiscreteVariableRecord to be and orphan) with const& to parent Record if the Table contains a parent id.
  // TODO: Find-replace on 'NAMESPACE'.
  DiscreteVariableRecord(const NAMESPACE::DiscreteVariable& discreteVariable, ProjectDatabase& database);

  // TODO: Delete if DiscreteVariable is abstract, make private if DiscreteVariable has derived classes.
  DiscreteVariableRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~DiscreteVariableRecord() {}

  //@}

  /** Get DiscreteVariableRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<DiscreteVariableRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  // TODO: Delete if no derived classes.
  static DiscreteVariableRecord factoryFromDiscreteVariable(const NAMESPACE::DiscreteVariable& discreteVariable, ProjectDatabase& database);

  static std::vector<DiscreteVariableRecord> getDiscreteVariableRecords(ProjectDatabase& database);

  static boost::optional<DiscreteVariableRecord> getDiscreteVariableRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED

  // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED

  NAMESPACE::DiscreteVariable discreteVariable() const;

  //@}
 protected:
  /// @cond
  typedef detail::DiscreteVariableRecord_Impl ImplType;

  explicit DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl);

  friend class detail::DiscreteVariableRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl,
                         ProjectDatabase database);

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

