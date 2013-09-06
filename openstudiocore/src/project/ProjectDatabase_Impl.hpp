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

#ifndef PROJECT_PROJECTDATABASE_IMPL_HPP
#define PROJECT_PROJECTDATABASE_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/ProjectDatabaseRecord.hpp>
#include <project/Record.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/time/DateTime.hpp>

#include <runmanager/lib/RunManager.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <QSqlQuery>
#include <QObject>

#include <set>

class QSqlDatabase;

namespace openstudio {
namespace project {

  class RemoveUndo;
  class ComponentRecord;
  class WorkflowRecord;
  class ProblemRecord;
  class AlgorithmRecord;
  class VariableRecord;

  namespace detail {

    class Record_Impl;

    class PROJECT_API ProjectDatabase_Impl : public QObject, public boost::enable_shared_from_this<ProjectDatabase_Impl>
    {

      Q_OBJECT;

      public:

        /// file system constructor
        ProjectDatabase_Impl(const openstudio::path& path, const openstudio::runmanager::RunManager& runManager, bool forceNew);

        /// virtual destructor, will remove any unsaved changes
        virtual ~ProjectDatabase_Impl();

        /// get all workflow records
        std::vector<WorkflowRecord> workflowRecords();

        /// get all variable records
        std::vector<VariableRecord> variableRecords();

        /// Returns this database's ProjectDatabaseRecord.
        ProjectDatabaseRecord projectDatabaseRecord() const;

        /// is the database dirty
        bool isDirty() const;

         /// get the handle
        UUID handle() const;

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

        /// get the version
        std::string version() const;

        /// get the path
        openstudio::path path() const;

        openstudio::path originalBasePath() const;

        openstudio::path newBasePath() const;

        void setBasePaths(const openstudio::path& originalBasePath,
                          const openstudio::path& newBasePath);

        /// get the run manager path
        openstudio::path runManagerDBPath() const;

        /// get the run manager
        openstudio::runmanager::RunManager runManager() const;

        /// start a transaction
        bool startTransaction() const;

        /// commit transaction
        bool commitTransaction() const;

        /// save pending changes
        bool save();

        /// save a record
        bool saveRecord(Record& record, bool topLevelObject);

        /// remove a record
        boost::optional<RemoveUndo> removeRecord(Record& record, bool saveResult);

        // does this database include this record, will check all maps and database
        bool includesRecord(const Record& record) const;

        // is this record currently marked as new
        bool isNewRecord(const Record& record) const;

        // is this object record marked as clean, checks clean map and database
        bool isCleanRecord(const Record& record) const;

        // is this record currently marked as dirty
        bool isDirtyRecord(const Record& record) const;

        // is this record currently marked as removed
        bool isRemovedRecord(const Record& record) const;

        /// get the qSql database
        boost::shared_ptr<QSqlDatabase> qSqlDatabase() const;

        // find record by handle, will check all maps
        boost::optional<Record> findLoadedRecord(const UUID& handle) const;

        // unload any unused records from the clean record map
        void unloadUnusedCleanRecords();

        // saves removal of removed objects, neccesary to do before importing objects with same uuid
        // undo will no longer be possible
        void commitRemove(const RemoveUndo& removeUndo);

        /// Relocate all path data from originalBase to newBase.
        void updatePathData(const openstudio::path& originalBase, const openstudio::path& newBase);

     public slots:

       void recordChanged(const UUID& handle);

     protected:

       void connectSignals(const Record& record) const;

       /// add new record, will set id on the object record
       void addNewRecord(Record& record);

       /// add dirty record
       void addDirtyRecord(const Record& record);

       /// add clean record
       void addCleanRecord(const Record& record);

       /// Update database to latest schema.
       void updateDatabase(const std::string& dbVersion);

      private:

        friend class openstudio::project::ProjectDatabase;

        REGISTER_LOGGER("openstudio.project.ProjectDatabase");

        /// create table for type T
        template<typename T>
        void createTable() const{
          QSqlQuery query(*m_qSqlDatabase);

          std::stringstream ss;
          ss << "CREATE TABLE " << T::databaseTableName() << "( ";

          std::set<int> values = T::ColumnsType::getValues();
          int expectedValue = 0;
          for (std::set<int>::const_iterator it = values.begin(), itend = values.end(); it != itend; ++it){
            // require 0 based columns, don't skip any
            BOOST_ASSERT(*it == expectedValue);

            // column name is name, type is description
            ss << T::ColumnsType::valueName(*it) << " " << T::ColumnsType::valueDescription(*it);

            // is this the last column?
            std::set<int>::const_iterator nextIt = it;
            ++nextIt;
            if (nextIt == itend){
              ss << ");";
            }else{
              ss << ", ";
            }

            ++expectedValue;
          }

          query.prepare(QString::fromStdString(ss.str()));
          assertExec(query);

          T::createIndices(*m_qSqlDatabase, T::databaseTableName());
        }

        // create initial tables
        void initialize();

        // update old versions of tables
        void update_0_6_6_to_0_7_0(const VersionString& startVersion);
        void update_0_8_0_to_0_8_1(const VersionString& startVersion);
        void update_0_8_3_to_0_8_4(const VersionString& startVersion);
        void update_0_9_0_to_0_9_1(const VersionString& startVersion);
        void update_0_10_0_to_0_10_1(const VersionString& startVersion);
        void update_0_10_2_to_0_10_3(const VersionString& startVersion);
        void update_0_10_3_to_0_10_4(const VersionString& startVersion);
        void update_0_10_4_to_0_10_5(const VersionString& startVersion);
        void update_0_11_5_to_0_11_6(const VersionString& startVersion);
        void update_1_0_0_to_1_0_1(const VersionString& startVersion);
        void update_1_0_2_to_1_0_3(const VersionString& startVersion);
        void update_1_0_3_to_1_0_4(const VersionString& startVersion);
        void update_1_0_4_to_1_0_5(const VersionString& startVersion);

        void setProjectDatabaseRecord(const ProjectDatabaseRecord& projectDatabaseRecord);

        // members
        openstudio::runmanager::RunManager m_runManager;
        boost::shared_ptr<QSqlDatabase> m_qSqlDatabase;
        boost::optional<openstudio::project::ProjectDatabaseRecord> m_projectDatabaseRecord;
        openstudio::path m_path;
        openstudio::path m_originalBasePath;
        openstudio::path m_newBasePath;
        bool m_reloaded;
        bool m_ignoreSignals;

        // map of handle to record
        std::map<UUID, Record> m_handleNewRecordMap;
        std::map<UUID, Record> m_handleDirtyRecordMap;
        std::map<UUID, Record> m_handleCleanRecordMap;
        std::map<UUID, Record> m_handleRemovedRecordMap;

        std::vector<RemoveUndo> m_removeUndos;
    };

  } // detail
} // project
} // openstudio

#endif // PROJECT_PROJECT_HPP
