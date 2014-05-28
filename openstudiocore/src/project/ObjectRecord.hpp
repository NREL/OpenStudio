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

#ifndef PROJECT_OBJECTRECORD_HPP
#define PROJECT_OBJECTRECORD_HPP

#include "ProjectAPI.hpp"
#include "Record.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/time/DateTime.hpp"
#include "../utilities/core/Path.hpp"

class QSqlQuery;
class QSqlDatabase;

namespace openstudio {
namespace project {

namespace detail{
  class ObjectRecord_Impl;
  class ProjectDatabase_Impl;
}

class JoinRecord;
class ProjectDatabase;

/** \class ObjectRecordColumns
 *  \brief There is no ObjectRecordColumns table, however all ObjectRecords begin with these
 *  members.
 *
 *  \relates ObjectRecord */
OPENSTUDIO_ENUM(ObjectRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                );

/**  ObjectRecord is the base class for objects represent individual objects. */
class PROJECT_API ObjectRecord : public Record {
 public:

  typedef detail::ObjectRecord_Impl ImplType;

  virtual ~ObjectRecord() {}

  // create indices for the table
  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  /// get the name
  std::string name() const;

  /// set the name
  bool setName(const std::string& name);

  /// get the display name
  std::string displayName() const;

  /// set the display name
  bool setDisplayName(const std::string& displayName);

  /// get the description
  std::string description() const;

  /// set the description
  bool setDescription(const std::string& description);

  /// get the timestamp the object was created at
  DateTime timestampCreate() const;

  /// get the timestamp the object was last edited at
  DateTime timestampLast() const;

  /// get the UUID of the object at last edit
  UUID uuidLast() const;

  /** Update any path data stored under originalBase to newBase. */
  void updatePathData(const openstudio::path& originalBase,
                      const openstudio::path& newBase);

 protected:

  friend class Record;

  /// constructor
  ObjectRecord(std::shared_ptr<detail::ObjectRecord_Impl> impl,
               ProjectDatabase projectDatabase);

  /// constructor, does not register in database, use with caution
  ObjectRecord(std::shared_ptr<detail::ObjectRecord_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.project.ObjectRecord");
};

/** \relates ObjectRecord */
typedef boost::optional<ObjectRecord> OptionalObjectRecord;

/** \relates ObjectRecord */
typedef std::vector<ObjectRecord> ObjectRecordVector;

} // project
} // openstudio

#endif // PROJECT_OBJECTRECORD_HPP
