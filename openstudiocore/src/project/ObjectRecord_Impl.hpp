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

#ifndef PROJECT_OBJECTRECORD_IMPL_HPP
#define PROJECT_OBJECTRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/Record_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/time/DateTime.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

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
    Q_OBJECT;
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
    virtual void bindValues(QSqlQuery& query) const;

    /// set the last state of this object from the query (including id)
    /// override in derived classes
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /// check that values (except id) are same as query
    /// override in derived classes
    virtual bool compareValues(const QSqlQuery& query) const;

    /// save values to last state
    /// override in derived classes
    virtual void saveLastValues();

    /// revert values back to last state
    /// override in derived classes
    virtual void revertToLastValues();

    /// when the object changes need to save to the database and emit onChange
    virtual void onChange(bool newVersion=true);

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
