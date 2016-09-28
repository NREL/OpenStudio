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
