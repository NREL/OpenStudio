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

#ifndef PROJECT_RECORD_HPP
#define PROJECT_RECORD_HPP

#include "ProjectAPI.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/time/DateTime.hpp"
#include "../utilities/core/Path.hpp"

class QSqlQuery;
class QSqlDatabase;
class QThread;

namespace openstudio {
namespace project {

class ObjectRecord;
class JoinRecord;
class ProjectDatabase;

namespace detail{
  class Record_Impl;
  class ProjectDatabase_Impl;
}

struct UpdateByIdQueryData {
  std::set<int> columnValues;
  std::string queryString;
  std::vector<QVariant> nulls;
};

/** \class RecordColumns
 *  \brief There is no RecordColumns table, however all Records begin with these
 *  members. */
OPENSTUDIO_ENUM(RecordColumns,
    ((id)(INTEGER PRIMARY KEY)(0))
    ((handle)(TEXT)(1))
);

/**  Record is the base class for saving and retrieving objects in a ProjectDatabase. */
class PROJECT_API Record {
 public:

  /** @name Constructors and Destructors */
  //@{
  virtual ~Record() {}

  //@}
  /** @name Getters */
  //@{

  /// Returns the id.
  int id() const;

  /// Returns the handle.
  UUID handle() const;

  //@}
  /** @name Setters */
  //@{
  //@}
  /** @name Template Methods */
  //@{

  /// Cast to Record of type T, can throw std::bad_cast
  template<typename T>
  T cast() const{
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl){
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /// Cast to optional Record of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}

  /// Returns the ProjectDatabase, will throw if ProjectDatabase has been destructed.
  ProjectDatabase projectDatabase() const;

  /// Returns the name of the database table.
  std::string databaseTableName() const;

  /// Returns all child Records in the database.  Note, will not find child
  /// Records which have been removed.
  std::vector<ObjectRecord> children() const;

  /// Returns all resource Records in the database.  Note, will not find resource
  /// Records which have been removed.
  std::vector<ObjectRecord> resources() const;

  /// Returns all join Records in the database.  Note, will not find join
  /// Records which have been removed.
  std::vector<JoinRecord> joinRecords() const;

  /// Move the implementation object to target thread.
  void moveToThread( QThread * targetThread );

  /// Connect signal from this Records implementation object to slot on qObject.
  bool connect(const std::string& signal, const QObject* qObject, const std::string& slot, Qt::ConnectionType type = Qt::AutoConnection) const;

  /// Returns number of shared references to this Records implementation object.
  unsigned useCount() const;

 protected:

  /// @cond

  friend class detail::ProjectDatabase_Impl;
  typedef detail::Record_Impl ImplType;

  /// @endcond

  /// Bind values to a query for saving.
  void bindValues(QSqlQuery& query) const;

  /// Set the last state of this object from the query (including id).
  void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

  /// Checks that values (except id) are same as query.
  bool compareValues(const QSqlQuery& query) const;

  /// Saves current values as last state.
  void saveLastValues();

  /// Revert values back to last state.
  void revertToLastValues();

  /// Try to find this Record's id in the database by using its handle.
  boost::optional<int> findIdByHandle() const;

  /// Inserts a row in the database for this Record and sets id.
  void insertRow(ProjectDatabase& projectDatabase);

  /// Saves just this Record in the database, does not save children.
  void saveRow(ProjectDatabase& projectDatabase);

  /// Removes just this Record from the database, does not remove children.
  void removeRow(ProjectDatabase& projectDatabase);

  /// Inserts a row in the database for this Record and sets id.
  void insertRow(const std::shared_ptr<QSqlDatabase>& db);

  /// Saves just this Record in the database, does not save children.
  void saveRow(const std::shared_ptr<QSqlDatabase>& db);

  /// Removes just this Record from the database, does not remove children.
  void removeRow(const std::shared_ptr<QSqlDatabase>& db);

  /// Reverts just this Record then saves it in the database, does not revert children.
  bool revertRow(ProjectDatabase& projectDatabase);

  /// Sets the last state of just this Record from the database (including id), does not affect children.
  void setLastValues(ProjectDatabase& projectDatabase);

  /// Compares just this Record to the one in the database and check if they are exactly equal except for id,
  /// does not check children
  bool databaseCompare(const ProjectDatabase& projectDatabase) const;

  /// Method called whenever the Record changes, emits onChange signal.
  void onChange();

  /// Makes a query to select all Records in this Record's database table.
  void makeSelectAllQuery(QSqlQuery& query) const;

  /// Makes a query to select Record with this id in this Record's database table.
  void makeSelectByIdQuery(QSqlQuery& query) const;

  /// Makes a query to select Record with this handle in this Record's database table.
  void makeSelectByHandleQuery(QSqlQuery& query) const;

  /// Makes a query to delete Record with this id from this Record's database table.
  void makeDeleteByIdQuery(QSqlQuery& query) const;

  /// Makes a query to delete Record with this handle from this Record's database table.
  void makeDeleteByHandleQuery(QSqlQuery& query) const;

  /// Checks if last values are set for this Record to revert back to.
  bool haveLastValues() const;

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new Record. If an implementation object for the same row already exists in the
  /// projectDatabase passed in, then the implementation object passed in will be discarded and
  /// the new Record will refer to the existing implementation object.
  Record(std::shared_ptr<detail::Record_Impl> impl, ProjectDatabase projectDatabase);

  /// Constructs a new Record but does not create or save to a row in the database, use with caution.
  Record(std::shared_ptr<detail::Record_Impl> impl);

  //@}
  /** @name Template Methods */
  //@{

  /// Returns the implementation object casted to type T.
  template<typename T>
  std::shared_ptr<T> getImpl() const
  {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  //@}

 private:

  REGISTER_LOGGER("openstudio.project.Record");

  std::shared_ptr<detail::Record_Impl> m_impl;
};

/// Compares handles of two Records for sorting operations.
PROJECT_API bool operator<(const Record& left, const Record& right);

/// Asserts that the query executes, throws if it does not.
PROJECT_API bool assertExec(QSqlQuery& query);

/// Asserts that the query is positioned at first record of the result set, throws if result set is empty.
PROJECT_API bool assertFirst(QSqlQuery& query);

/// Asserts that the query is positioned at last record of the result set, throws if result set is empty.
PROJECT_API bool assertLast(QSqlQuery& query);

} // project
} // openstudio

#endif // PROJECT_OBJECTRECORD_HPP
