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

#ifndef PROJECT_OBJECTRECORD_IMPL_HPP
#define PROJECT_OBJECTRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "Record_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/time/DateTime.hpp"

#include <QObject>

class QSqlQuery;
class QSqlDatabase;

namespace openstudio {
namespace project {

class ProjectDatabase;
class ObjectRecord;
class JoinRecord;

namespace detail {

  class ProjectDatabase_Impl;

  class PROJECT_API ObjectRecord_Impl : public Record_Impl {
   public:

    /// constructor with no database
    ObjectRecord_Impl(const std::string& name,
                      const std::string& displayName,
                      const std::string& description);

    /// constructor
    ObjectRecord_Impl(const ProjectDatabase& projectDatabase,
                      const std::string& name,
                      const std::string& displayName,
                      const std::string& description);

    /// constructor with uuids synchronized with outside object
    ObjectRecord_Impl(const ProjectDatabase& projectDatabase,
                      const UUID& handle,
                      const std::string& name,
                      const std::string& displayName,
                      const std::string& description,
                      const UUID& uuidLast);

    /// constructor, throws if bad query
    ObjectRecord_Impl(const ProjectDatabase& projectDatabase, const QSqlQuery& query);

    /// virtual destructor
    virtual ~ObjectRecord_Impl();

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
    virtual void updatePathData(const openstudio::path& originalBase,
                                const openstudio::path& newBase);

    /** Impl-only method. Can be used during construction to determine if related 
     *  records should be refreshed. */
    UUID lastUuidLast() const;

   protected:

    /// bind values to a query for saving
    /// override in derived classes
    virtual void bindValues(QSqlQuery& query) const override;

    /// set the last state of this object from the query (including id)
    /// override in derived classes
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) override;

    /// check that values (except id) are same as query
    /// override in derived classes
    virtual bool compareValues(const QSqlQuery& query) const override;

    /// save values to last state
    /// override in derived classes
    virtual void saveLastValues() override;

    /// revert values back to last state
    /// override in derived classes
    virtual void revertToLastValues() override;

    /// when the object changes need to save to the database and emit onChange
    virtual void onChange(bool newVersion=true) override;

    /// set the UUID of the object at last edit
    bool setUuidLast(const UUID& uuidLast);

   private:

    REGISTER_LOGGER("openstudio.project.ObjectRecord");

    std::string m_name;
    std::string m_displayName;
    std::string m_description;
    DateTime m_timestampCreate;
    DateTime m_timestampLast;
    UUID m_uuidLast;

    std::string m_lastName;
    std::string m_lastDisplayName;
    std::string m_lastDescription;
    DateTime m_lastTimestampCreate;
    DateTime m_lastTimestampLast;
    UUID m_lastUUIDLast;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_OBJECTRECORD_IMPL_HPP
