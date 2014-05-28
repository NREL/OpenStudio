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

#ifndef PROJECT_PROJECTDATABASE_HPP
#define PROJECT_PROJECTDATABASE_HPP

#include "ProjectAPI.hpp"
#include "Record.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/time/DateTime.hpp"

#include "../runmanager/lib/RunManager.hpp"

#include <QSqlQuery>

#include <sstream>

class QSqlDatabase;

namespace openstudio {
namespace project {

namespace detail {
  class ObjectRecord_Impl;
  class ProjectDatabase_Impl;
}

class ProjectRecord;
class ComponentRecord;
class WorkflowRecord;
class ProblemRecord;
class VariableRecord;
class ProjectDatabaseRecord;

/** RemoveUndo is a simple class used to commit or revert a removeRecord operation in the ProjectDatabase.
 */
class PROJECT_API RemoveUndo {
public:

  /// Map that Record was removed from.
  enum RemoveSource {FromNew, FromDirty, FromClean, FromRemoved};

  /// Constructor takes Record handle and RemoveSource.
  RemoveUndo(UUID handle, RemoveSource removeSource);

  /// Concatenate this RemoveUndo object with another one. Does not
  /// change the primaryHandle.
  void concat(const RemoveUndo& other);

  /// Returns the handle of the primary Record removed, e.g. the one
  /// passed to removeRecord.
  UUID primaryHandle() const;

  /// Returns a vector of pairs with Record handle and RemoveSource.
  std::vector<std::pair<UUID, RemoveSource> > removedObjects() const;

private:

  std::vector<std::pair<UUID, RemoveSource> > m_removedObjects;

};

/** ProjectDatabase.
 *
 *
 *
 */
class PROJECT_API ProjectDatabase {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Construct a ProjectDatabase at path using runManager.  If forceNew is true will remove
   *  existing database otherwise will try to open existing database. Throws if bad path or open
   *  fails. */
  ProjectDatabase(const openstudio::path& path,
                  const openstudio::runmanager::RunManager& runManager,
                  bool forceNew=false);

  /** Construct a ProjectDatabase and its RunManager. The corresponding RunManager database
   *  location is path, but with the .db, rather than the .osp extension. If forceNew will remove
   *  any existing databases; otherwise will try to open existing databases. Throws if bad path
   *  or if open fails for any reason. */
  ProjectDatabase(const openstudio::path& path,
                  bool forceNew=false,
                  bool pauseRunManager=false,
                  bool initializeRunManagerUI=false);

  /// Copy constructor, shares implementation object with other.
  ProjectDatabase(const ProjectDatabase& other);

  /** Attempt to open an existing ProjectDatabase. By default, will try to replace saved
   *  ProjectDatabase.path().parent_path() with path.parent_path(). */
  static boost::optional<ProjectDatabase> open(const openstudio::path& path,
                                               bool pauseRunManager=false,
                                               bool initializeRunManagerUI=false);

  virtual ~ProjectDatabase() {}

  //@}
  /** @name Getters */
  //@{

  /// Returns this database's ProjectDatabaseRecord.
  ProjectDatabaseRecord projectDatabaseRecord() const;

  /// Returns the handle for the ProjectDatabase.
  UUID handle() const;

  /// Returns the name of the ProjectDatabase.
  std::string name() const;

  /// Returns the display name of the ProjectDatabase.
  std::string displayName() const;

  /// Returns the description of the ProjectDatabase.
  std::string description() const;

  /// Returns the timestamp the ProjectDatabase was created at.
  DateTime timestampCreate() const;

  /// Returns the timestamp the ProjectDatabase was last saved at.
  DateTime timestampLast() const;

  /// Returns the UUID identifying the last ProjectDatabase save.
  UUID uuidLast() const;

  /// Returns the ProjectDatabase version.
  std::string version() const;

  /// Returns the path to the ProjectDatabase.
  openstudio::path path() const;

  /** Returns the path().parent_path() for the previous location of this database. Provided
   *  so users can finish repairing their path data after calling open. */
  openstudio::path originalBasePath() const;

  /** Returns path().parent_path().  Provided so users can finish repairing their path data
   *  after calling open. */
  openstudio::path newBasePath() const;

  /// Returns the path to the RunManager.
  openstudio::path runManagerDBPath() const;

  /// Returns the RunManager.
  openstudio::runmanager::RunManager runManager() const;

  //@}
  /** @name Setters */
  //@{

  /// Sets the name of the ProjectDatabase.
  bool setName(const std::string& name);

  /// Sets the display name of the ProjectDatabase.
  bool setDisplayName(const std::string& displayName);

  /// Sets the description of the ProjectDatabase.
  bool setDescription(const std::string& description);

  //@}
  /** @name Template Methods */
  //@{

  /// Gets Record from a query, checking maps first to prevent duplicate Records for the same entry.
  template<typename T>
  T getFromQuery(const QSqlQuery& query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value = query.value(T::ColumnsType::handle);
    OS_ASSERT(value.isValid() && !value.isNull());
    UUID handle = toUUID(value.toString().toStdString());

    boost::optional<Record> record = this->findLoadedRecord(handle);
    if (record){
      boost::optional<T> result = record->optionalCast<T>();
      OS_ASSERT(result);
      return *result;
    }

    boost::optional<T> result = T::factoryFromQuery(query, *this);
    OS_ASSERT(result);
    return *result;
  }

  /// Gets an ObjectRecord by name.
  template <typename T>
  boost::optional<T> getObjectRecordByName(const std::string& name) {
    boost::optional<T> result;

    QSqlQuery query(*(this->qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::ObjectRecordType::databaseTableName() + " WHERE name=:name"));
    query.bindValue(":name",toQString(name));
    try {
      assertExec(query);
      if (query.first()) {
        result = this->template getFromQuery<T>(query);
      }
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to execute getObjectRecordByName query, because '" << e.what() << "'.");
    }
    return result;
  }

  /// Gets an ObjectRecord by handle.
  template <typename T>
  boost::optional<T> getObjectRecordByHandle(const openstudio::UUID& handle) {
    boost::optional<T> result;

    boost::optional<Record> record = this->findLoadedRecord(handle);
    if (record){
      result = record->optionalCast<T>();
      OS_ASSERT(result);
      return *result;
    }

    QSqlQuery query(*(this->qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::ObjectRecordType::databaseTableName() + " WHERE handle=:handle"));
    query.bindValue(":handle",handle.toString());
    try {
      assertExec(query);
      if (query.first()) {
        result = this->template getFromQuery<T>(query);
      }
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to execute getObjectRecordByName query, because '" << e.what() << "'.");
    }
    return result;
  }

  //@}

  /// Returns all \link WorkflowRecord WorkflowRecords \endlink.
  std::vector<WorkflowRecord> workflowRecords();

  /// Returns all \link VariableRecord VariableRecords \endlink.
  std::vector<VariableRecord> variableRecords();

  /// Returns true if the database is dirty, e.g. has unsaved Records.
  bool isDirty() const;

  /// Start a transaction.  Will return false if another transaction is already active.
  bool startTransaction() const;

  /// Commit a transaction.  Will return false if there is no active transaction.
  bool commitTransaction() const;

  /// Save pending changes to all Records. Will start and commit a transaction if a transaction
  /// is not already active.
  bool save();

  /// Save pending changes to single Record and its children. Will start and commit a transaction if a transaction
  /// is not already active.
  bool saveRecord(Record& record);

  /// Remove a single Record and its children.  Will start and commit a transaction if a transaction
  /// is not already active.  If Records were removed during this operation the returned RemoveUndo
  /// object can be used to revert or commit the remove operation.
  boost::optional<RemoveUndo> removeRecord(Record& record);

  /// Checks if this database includes this Record.
  bool includesRecord(const Record& record) const;

  /// Checks if this Record is currently marked as new.
  bool isNewRecord(const Record& record) const;

  /// Checks if this Record is currently marked as clean.
  bool isCleanRecord(const Record& record) const;

  /// Checks if this Record is currently marked as dirty.
  bool isDirtyRecord(const Record& record) const;

  /// Checks if this Record is currently marked as removed.
  bool isRemovedRecord(const Record& record) const;

  // DLM@20110615: can this be protected?
  /// Returns the QSqlDatabase.
  std::shared_ptr<QSqlDatabase> qSqlDatabase() const;

  /// Finds Record by handle, will check all maps but does not query database.
  boost::optional<Record> findLoadedRecord(const UUID& handle) const;

  /// Unload any unused Records from the clean record map.
  /// \todo Want to have a timer that periodically calls this.
  void unloadUnusedCleanRecords();

  /// Finalizes removal of removed objects, necessary to do before importing objects with same UUID.
  /// Undoing a removal will no longer be possible after this.
  void commitRemove(const RemoveUndo& removeUndo);

  /// Relocate path data from originalBase to newBase.
  void updatePathData(const openstudio::path& originalBase, const openstudio::path& newBase);

 protected:

  /// Constructor from implementation object.
  ProjectDatabase(std::shared_ptr<detail::ProjectDatabase_Impl> impl);

  /// Add a new Record, will set id on the object record.
  void addNewRecord(Record& record);

  /// Add a dirty Record.
  void addDirtyRecord(const Record& record);

  /// Add a clean Record.
  void addCleanRecord(const Record& record);

  /// Update database to latest schema.
  void updateDatabase(const std::string& dbVersion);

  /// @cond

  friend class Record;
  friend class detail::ProjectDatabase_Impl;
  friend class detail::Record_Impl;

  typedef detail::ProjectDatabase_Impl ImplType;
  /// @endcond
 private:
  /// @cond
  REGISTER_LOGGER("openstudio.project.ProjectDatabase");

  std::shared_ptr<detail::ProjectDatabase_Impl> m_impl;
  /// @endcond

  void initialize(const openstudio::path& path);

};

typedef boost::optional<ProjectDatabase> OptionalProjectDatabase;

} // project
} // openstudio

#endif // PROJECT_PROJECTDATABASE_HPP
