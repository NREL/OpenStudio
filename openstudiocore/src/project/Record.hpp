/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
