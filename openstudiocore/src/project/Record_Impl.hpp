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

#ifndef PROJECT_RECORD_IMPL_HPP
#define PROJECT_RECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "Record.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/time/DateTime.hpp"

#include <QObject>
#include <QSqlQuery>
#include <QVariant>

class QSqlDatabase;

namespace openstudio {
namespace project {

  class ProjectDatabase;
  class ObjectRecord;
  class JoinRecord;

  namespace detail {

    class ProjectDatabase_Impl;

    class PROJECT_API Record_Impl : public QObject, public std::enable_shared_from_this<Record_Impl>
    {

      Q_OBJECT;

      public:

        /// constructor with no database
        Record_Impl();

        /// constructor
        Record_Impl(const ProjectDatabase& projectDatabase);

        /// constructor with uuids synchronized with outside object
        Record_Impl(const ProjectDatabase& projectDatabase,
                    const UUID& handle);

        /// constructor, throws if bad query
        Record_Impl(const ProjectDatabase& projectDatabase, const QSqlQuery& query);

        /// virtual destructor
        virtual ~Record_Impl();

         /// get the project database, will throw if database has been destructed
        ProjectDatabase projectDatabase() const;

        /// get the id
        int id() const;

        /// get the handle
        UUID handle() const;

        /// get name of the database table, implement at table level
        virtual std::string databaseTableName() const = 0;

        /// get parent object
        virtual boost::optional<ObjectRecord> parent() const = 0;

        /// get child objects
        virtual std::vector<ObjectRecord> children() const = 0;

        /// get resource objects
        virtual std::vector<ObjectRecord> resources() const = 0;

        /// get join record objects
        virtual std::vector<JoinRecord> joinRecords() const = 0;

        /// save the row for just this object in the database
        virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database) = 0;

        /// revert just this object then save in the database
        /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
        bool revertRow(ProjectDatabase& projectDatabase);

        /** Get an object that wraps this impl. */
        template<typename T>
        T getObject() const
        {
          T result(std::dynamic_pointer_cast<typename T::ImplType>(
                     std::const_pointer_cast<Record_Impl>(shared_from_this())));
          return result;
        }

        bool connect(const std::string& signal, const QObject* qObject, const std::string& slot, Qt::ConnectionType type) const;

      signals:

        void onChange(const UUID& handle);

      protected:

        friend class openstudio::project::Record;

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

        /// try to find this objects id in the database using its handle
        boost::optional<int> findIdByHandle() const;

        /// insert row for just this object in the database and set id
        void insertRow(const std::shared_ptr<QSqlDatabase> &database);

        /// remove just this object from the database
        void removeRow(const std::shared_ptr<QSqlDatabase> &database);

        /// set the last state of just this object from the database (including id)
        /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
        void setLastValues(ProjectDatabase& projectDatabase);

        /// compare just this object to the one in the database and check if they are exactly equal except for id,
        /// requires ProjectDatabase as called in ProjectDatabase ctor/dtor
        bool databaseCompare(const ProjectDatabase& projectDatabase) const;

        /// get the query to select all
        void makeSelectAllQuery(QSqlQuery& query) const;

        /// get the query to select by id
        void makeSelectByIdQuery(QSqlQuery& query) const;

        /// get the query to select by handle
        void makeSelectByHandleQuery(QSqlQuery& query) const;

        /// get the query to delete by id
        void makeDeleteByIdQuery(QSqlQuery& query) const;

        /// get the query to delete by handle
        void makeDeleteByHandleQuery(QSqlQuery& query) const;

        /// do we have values to revert to
        bool haveLastValues() const;

        /// get the query to update by id
        template<typename T>
        void makeUpdateByIdQuery(QSqlQuery& query) const {
          UpdateByIdQueryData queryData = T::updateByIdQueryData();
          query.prepare(QString::fromStdString(queryData.queryString));
          auto colIndexIt = queryData.columnValues.begin();
          auto colIndexItEnd = queryData.columnValues.end();
          std::vector<QVariant>::const_iterator nullIt = queryData.nulls.begin();
          std::vector<QVariant>::const_iterator nullItEnd = queryData.nulls.end();
          for (; colIndexIt != colIndexItEnd; ++colIndexIt, ++nullIt)
          {
            OS_ASSERT(nullIt != nullItEnd);
            query.bindValue(*colIndexIt,*nullIt);
          }
          // bind id to the id in the WHERE clause
          query.bindValue(queryData.columnValues.size(), this->id());
        }

      private:

        REGISTER_LOGGER("openstudio.project.Record");

        std::weak_ptr<detail::ProjectDatabase_Impl> m_projectDatabaseWeakImpl;

        int m_id;
        UUID m_handle;

        bool m_haveLastValues;
        //int m_lastId; // cannot change these
        //UUID m_lastHandle; // cannot change these
    };

  } // detail
} // project
} // openstudio

#endif // PROJECT_OBJECTRECORD_IMPL_HPP
