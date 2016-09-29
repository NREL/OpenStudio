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
