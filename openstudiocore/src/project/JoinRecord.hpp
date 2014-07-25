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

#ifndef PROJECT_JOINRECORD_HPP
#define PROJECT_JOINRECORD_HPP

#include "ProjectAPI.hpp"
#include "Record.hpp" 
#include "ProjectDatabase.hpp" 

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/UUID.hpp"

class QSqlQuery;

namespace openstudio {
namespace project {

namespace detail{
  class JoinRecord_Impl;
}

class ProjectDatabase;

/** \class JoinRecordColumns
   *  \brief There is no JoinRecords table, however all JoinRecords have only these members.
   * For general information, see the
   *  OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
   *  call is:
   *  \code
OPENSTUDIO_ENUM(JoinRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((leftId)(INTEGER)(2))
                ((leftHandle)(TEXT)(3))
                ((rightId)(INTEGER)(4))
                ((rightHandle)(TEXT)(5))
                );
   *  \endcode */
OPENSTUDIO_ENUM(JoinRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((leftId)(INTEGER)(2))
                ((leftHandle)(TEXT)(3))
                ((rightId)(INTEGER)(4))
                ((rightHandle)(TEXT)(5))
                );

/**  JoinRecord is the base class for join objects that can be saved to a ProjectDatabase.
 *    JoinRecords are named (LeftClass)_(RightClass)_JoinRecord, e.g. Variable_ParameterSpace_JoinRecord.
 **/
class PROJECT_API JoinRecord : public Record{
 public:

  // create indices for the table
  static void createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName);

  virtual ~JoinRecord() {}

  /// get join
  template<typename T>
  static boost::optional<T> getJoinRecord(int leftId, int rightId, ProjectDatabase projectDatabase)
  {
    boost::optional<T> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::databaseTableName() + " WHERE leftId=:leftId AND rightId=:rightId"));
    query.bindValue(":leftId", leftId);
    query.bindValue(":rightId", rightId);
    assertExec(query);
    if(query.first()){
      result = T(std::shared_ptr<typename T::ImplType>(new typename T::ImplType(query, projectDatabase)), projectDatabase);
    }

    return result;
  }

  /// get joins for left
  template<typename T>
  static std::vector<T> getJoinRecordsForLeftId(int leftId, ProjectDatabase projectDatabase)
  {
    std::vector<T> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::databaseTableName() + " WHERE leftId=:leftId"));
    query.bindValue(":leftId", leftId);
    assertExec(query);
    while(query.next()){
      T object(std::shared_ptr<typename T::ImplType>(new typename T::ImplType(query, projectDatabase)), projectDatabase);
      result.push_back(object);
    }

    return result;
  }

  /// get joins for right
  template<typename T>
  static std::vector<T> getJoinRecordsForRightId(int rightId, ProjectDatabase projectDatabase)
  {
    std::vector<T> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::databaseTableName() + " WHERE rightId=:rightId"));
    query.bindValue(":rightId", rightId);
    assertExec(query);
    while(query.next()){
      T object(std::shared_ptr<typename T::ImplType>(new typename T::ImplType(query, projectDatabase)), projectDatabase);
      result.push_back(object);
    }

    return result;
  }

  /// unlink
  template<typename T>
  static bool unlink(int leftId, int rightId, ProjectDatabase projectDatabase)
  {
    bool result = false;

    std::vector<T> toRemove;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::databaseTableName() + " WHERE leftId=:leftId AND rightId=:rightId"));
    query.bindValue(":leftId", leftId);
    query.bindValue(":rightId", rightId);
    assertExec(query);
    if (query.first()){
      T object(std::shared_ptr<typename T::ImplType>(new typename T::ImplType(query, projectDatabase)), projectDatabase);
      result = projectDatabase.removeRecord(object);
    }

    return result;
  }

  /// unlink left
  template<typename T>
  static bool unlinkLeft(int leftId, ProjectDatabase projectDatabase)
  {
    bool result = false;

    std::vector<T> toRemove;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::databaseTableName() + " WHERE leftId=:leftId"));
    query.bindValue(":leftId", leftId);
    assertExec(query);
    while(query.next()){
      T object(std::shared_ptr<typename T::ImplType>(new typename T::ImplType(query, projectDatabase)), projectDatabase);
      toRemove.push_back(object);
    }

    for (T object : toRemove){
      result = projectDatabase.removeRecord(object);
    }

    return result;
  }

  /// unlink right
  template<typename T>
  static bool unlinkRight(int rightId, ProjectDatabase projectDatabase)
  {
    bool result = false;

    std::vector<T> toRemove;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + T::databaseTableName() + " WHERE rightId=:rightId"));
    query.bindValue(":rightId", rightId);
    assertExec(query);
    while(query.next()){
      T object(std::shared_ptr<typename T::ImplType>(new typename T::ImplType(query, projectDatabase)), projectDatabase);
      toRemove.push_back(object);
    }

    for (T object : toRemove){
      result = projectDatabase.removeRecord(object);
    }

    return result;
  }

  /// get the left id
  int leftId() const;

  /// get the left handle
  UUID leftHandle() const;

  /// get the left object
  ObjectRecord leftObject() const;

  /// get the right id
  int rightId() const;

  /// get the right handle
  UUID rightHandle() const;

  /// get the right object
  ObjectRecord rightObject() const;

 protected:

  typedef detail::JoinRecord_Impl ImplType;

  /// constructor
  JoinRecord(std::shared_ptr<detail::JoinRecord_Impl> impl, ProjectDatabase projectDatabase);

  /// constructor
  JoinRecord(std::shared_ptr<detail::JoinRecord_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.project.JoinRecord");
};

/** \relates JoinRecord */
typedef boost::optional<JoinRecord> OptionalJoinRecord;

/** \relates JoinRecord */
typedef std::vector<JoinRecord> JoinRecordVector;

} // project
} // openstudio

#endif // PROJECT_JOINRECORD_HPP
