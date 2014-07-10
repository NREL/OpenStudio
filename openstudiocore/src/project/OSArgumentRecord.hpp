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

#ifndef PROJECT_OSARGUMENTRECORD_HPP
#define PROJECT_OSARGUMENTRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../ruleset/OSArgument.hpp"

namespace openstudio {
namespace project {

class RubyMeasureRecord;
class RubyContinuousVariableRecord;

namespace detail {

  class OSArgumentRecord_Impl;

} // detail

/** \class OSArgumentRecordColumns
 *  \brief Column definitions for the OSArgumentRecords table.
 *  \details At some point, rubyPerturbationRecordId should be renamed to
 *  rubyMeasureRecordId. For general information, see the
 *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(OSArgumentRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((rubyPerturbationRecordId)(INTEGER)(8))
  ((rubyContinuousVariableRecordId)(INTEGER)(9))
  ((argumentType)(INTEGER)(10))
  ((required)(BOOLEAN)(11))
  ((argumentValue)(TEXT)(12))
  ((defaultArgumentValue)(TEXT)(13))
  ((domainType)(INTEGER)(14))
  ((domainValues)(TEXT)(15))
  ((choices)(TEXT)(16))
  ((choiceDisplayNames)(TEXT)(17))
  ((isRead)(BOOLEAN)(18))
  ((extension)(TEXT)(19))
);
 *  \endcode */
OPENSTUDIO_ENUM(OSArgumentRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((rubyPerturbationRecordId)(INTEGER)(8))
  ((rubyContinuousVariableRecordId)(INTEGER)(9))
  ((argumentType)(INTEGER)(10))
  ((required)(BOOLEAN)(11))
  ((argumentValue)(TEXT)(12))
  ((defaultArgumentValue)(TEXT)(13))
  ((domainType)(INTEGER)(14))
  ((domainValues)(TEXT)(15))
  ((choices)(TEXT)(16))
  ((choiceDisplayNames)(TEXT)(17))
  ((isRead)(BOOLEAN)(18))
  ((extension)(TEXT)(19))
);

/** OSArgumentRecord is a ObjectRecord. */
class PROJECT_API OSArgumentRecord : public ObjectRecord {
 public:

  typedef detail::OSArgumentRecord_Impl ImplType;
  typedef OSArgumentRecordColumns ColumnsType;
  typedef OSArgumentRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  OSArgumentRecord(const ruleset::OSArgument& osArgument,
                   RubyMeasureRecord& rubyMeasureRecord);

  OSArgumentRecord(const ruleset::OSArgument& osArgument,
                   RubyContinuousVariableRecord& rubyContinuousVariableRecord);

  OSArgumentRecord(const QSqlQuery& query, ProjectDatabase& database);

  /** Construct from impl. */
  OSArgumentRecord(std::shared_ptr<detail::OSArgumentRecord_Impl> impl,
                   ProjectDatabase database);

  virtual ~OSArgumentRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<OSArgumentRecord> factoryFromQuery(const QSqlQuery& query,
                                                            ProjectDatabase& database);

  static std::vector<OSArgumentRecord> getOSArgumentRecords(ProjectDatabase& database);

  static boost::optional<OSArgumentRecord> getOSArgumentRecord(int id,
                                                               ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Get the RubyMeasureRecord that parents this OSArgumentRecord. */
  boost::optional<RubyMeasureRecord> rubyMeasureRecord() const;

  /** Get the RubyContinuousVariableRecord that parents this OSArgumentRecord. */
  boost::optional<RubyContinuousVariableRecord> rubyContinuousVariableRecord() const;

  ruleset::OSArgument osArgument() const;

  ruleset::OSArgumentType type() const;

  bool hasValue() const;

  bool hasDefaultValue() const;

  openstudio::path valueAsPath() const;

  openstudio::path defaultValueAsPath() const;

  //@}
 protected:
  /// @cond
  explicit OSArgumentRecord(std::shared_ptr<detail::OSArgumentRecord_Impl> impl);

  friend class detail::OSArgumentRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.OSArgumentRecord");
};

/** \relates OSArgumentRecord*/
typedef boost::optional<OSArgumentRecord> OptionalOSArgumentRecord;

/** \relates OSArgumentRecord*/
typedef std::vector<OSArgumentRecord> OSArgumentRecordVector;

} // project
} // openstudio

#endif // PROJECT_OSARGUMENTRECORD_HPP

