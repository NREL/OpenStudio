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

#ifndef PROJECT_PROJECTDATABASERECORD_IMPL_HPP
#define PROJECT_PROJECTDATABASERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"

class QSqlQuery;

namespace openstudio {
namespace project {

namespace detail {

  class PROJECT_API ProjectDatabaseRecord_Impl : public ObjectRecord_Impl {
   public:

    /// constructors
    ProjectDatabaseRecord_Impl(const std::string& version,
                               const openstudio::path& runManagerDBPath,
                               const ProjectDatabase& projectDatabase);

    /// create an attribute from a query, throws if bad query
    explicit ProjectDatabaseRecord_Impl(const QSqlQuery& query,
                                        const ProjectDatabase& projectDatabase);

    virtual ~ProjectDatabaseRecord_Impl() {}

    /// get name of the database table
    virtual std::string databaseTableName() const;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const;

    /// get child objects
    virtual std::vector<ObjectRecord> children() const;

    /// get resource objects
    virtual std::vector<ObjectRecord> resources() const;

    /// get join record objects
    virtual std::vector<JoinRecord> joinRecords() const;

    /// save the row for just this object in the database
    /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual void updatePathData(const openstudio::path& originalBase,
                                const openstudio::path& newBase);

    /** @name Getters */
    //@{

    /// get the version
    std::string version() const;

    /// get the run manager path
    openstudio::path runManagerDBPath() const;

    //@}
    /** @name Setters */
    //@{

    void setVersion(const std::string& version);

    void setRunManagerDBPath(const openstudio::path& newPath);

    //@}
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

   private:

    REGISTER_LOGGER("openstudio.project.ProjectDatabaseRecord");

    std::string m_version;
    openstudio::path m_runManagerDBPath;

    std::string m_lastVersion;
    openstudio::path m_lastRunManagerDBPath;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_PROJECTRECORD_IMPL_HPP
