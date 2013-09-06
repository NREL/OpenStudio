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

#include <project/ProjectDatabase.hpp>
#include <project/ProjectDatabase_Impl.hpp>
#include <project/ProjectDatabaseRecord.hpp>
#include <project/ProjectDatabaseRecord_Impl.hpp>

#include <project/ObjectRecord.hpp>
#include <project/ObjectRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/JoinRecord_Impl.hpp>

#include <project/AnalysisRecord.hpp>
#include <project/AnalysisRecord_Impl.hpp>
#include <project/AlgorithmRecord.hpp>
#include <project/AlgorithmRecord_Impl.hpp>
#include <project/AttributeRecord.hpp>
#include <project/AttributeRecord_Impl.hpp>
#include <project/CloudSessionRecord.hpp>
#include <project/CloudSessionRecord_Impl.hpp>
#include <project/CloudSettingsRecord.hpp>
#include <project/CloudSettingsRecord_Impl.hpp>
#include <project/ContinuousVariableRecord.hpp>
#include <project/DataPointRecord.hpp>
#include <project/DataPointRecord_Impl.hpp>
#include <project/DataPointValueRecord.hpp>
#include <project/DataPointValueRecord_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/FileReferenceRecord_Impl.hpp>
#include <project/FunctionRecord.hpp>
#include <project/FunctionRecord_Impl.hpp>
#include <project/OSArgumentRecord.hpp>
#include <project/OSArgumentRecord_Impl.hpp>
#include <project/OutputVariableRecord.hpp>
#include <project/MeasureRecord.hpp>
#include <project/MeasureRecord_Impl.hpp>
#include <project/ProblemRecord.hpp>
#include <project/ProblemRecord_Impl.hpp>
#include <project/TagRecord.hpp>
#include <project/TagRecord_Impl.hpp>
#include <project/UrlRecord.hpp>
#include <project/UrlRecord_Impl.hpp>
#include <project/URLSearchPathRecord.hpp>
#include <project/URLSearchPathRecord_Impl.hpp>
#include <project/VariableRecord.hpp>
#include <project/VariableRecord_Impl.hpp>
#include <project/WorkflowRecord.hpp>
#include <project/WorkflowRecord_Impl.hpp>

#include <project/DataPoint_Measure_JoinRecord.hpp>
#include <project/DataPoint_Measure_JoinRecord_Impl.hpp>

#include <analysis/DataPoint.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/Workflow.hpp>

#include <utilities/core/String.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/time/DateTime.hpp>

#include <OpenStudio.hxx>

#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>

using namespace openstudio::analysis;

namespace openstudio {
namespace project {
namespace detail {

  ProjectDatabase_Impl::ProjectDatabase_Impl(const openstudio::path& path,
                                             const openstudio::runmanager::RunManager& runManager,
                                             bool forceNew)
    : m_runManager(runManager),
      m_path(path),
      m_reloaded(true),
      m_ignoreSignals(false)
  {
    // do we need to create tables?
    bool needsInitialize = false;
    if(boost::filesystem::exists(path)){
      if (forceNew){
        boost::filesystem::remove(path);
        needsInitialize = true;
      }
    }else{
      needsInitialize = true;
    }

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", toQString(path));
    OS_ASSERT(database.isValid());
    database.setDatabaseName(toQString(path));
    OS_ASSERT(database.open());
    m_qSqlDatabase = boost::shared_ptr<QSqlDatabase>(new QSqlDatabase(database));

    QSqlQuery query(*m_qSqlDatabase);
    //query.prepare("PRAGMA synchronous = OFF");
    query.prepare("PRAGMA synchronous = NORMAL");
    //query.prepare("PRAGMA synchronous = FULL");
    query.exec();

    query.prepare("PRAGMA locking_mode=NORMAL");
    //query.prepare("PRAGMA locking_mode=EXCLUSIVE");
    query.exec();

    //query.prepare("PRAGMA journal_mode=DELETE");
    //query.prepare("PRAGMA journal_mode=TRUNCATE");
    query.prepare("PRAGMA journal_mode=PERSIST");
    //query.prepare("PRAGMA journal_mode=MEMORY");
    //query.prepare("PRAGMA journal_mode=WAL");
    //query.prepare("PRAGMA journal_mode=OFF");
    query.exec();

    if (needsInitialize){
      this->initialize();
      m_reloaded = false;
    }
  }

  ProjectDatabase_Impl::~ProjectDatabase_Impl()
  {
    LOG(Debug,"Beginning ProjectDatabase_Impl destructor.");
    bool didStartTransaction = this->startTransaction();

    m_ignoreSignals = true;

    // delete new objects
    std::map<UUID, Record>::iterator it = m_handleNewRecordMap.begin();
    std::map<UUID, Record>::iterator itend = m_handleNewRecordMap.end();
    for( ; it != itend; ++it){
      it->second.removeRow(m_qSqlDatabase);
    }
    m_handleNewRecordMap.clear();

    // re-add deleted objects
    it = m_handleRemovedRecordMap.begin();
    itend = m_handleRemovedRecordMap.end();
    for( ; it != itend; ++it){
      it->second.insertRow(m_qSqlDatabase);
      it->second.saveRow(m_qSqlDatabase);
    }
    m_handleRemovedRecordMap.clear();

    // release m_projectDatabaseRecord
    m_projectDatabaseRecord.reset();

    if (didStartTransaction){
      bool didCommitTransaction = this->commitTransaction();
      OS_ASSERT(didCommitTransaction);
    }else{
      // any uncommitted transactions will now be lost
    }

    // make sure we are the last one using database connection
    OS_ASSERT(m_qSqlDatabase.use_count() == 1);

    m_qSqlDatabase->close();
    m_qSqlDatabase.reset(); // actually delete the database before removeDatabase
    QSqlDatabase::removeDatabase(toQString(m_path));
    LOG(Debug,"Completing ProjectDatabase_Impl destructor.");
  }

  std::vector<WorkflowRecord> ProjectDatabase_Impl::workflowRecords()
  {
    ProjectDatabase other(this->shared_from_this());
    return WorkflowRecord::getWorkflowRecords(other);
  }

  std::vector<VariableRecord> ProjectDatabase_Impl::variableRecords()
  {
    ProjectDatabase other(this->shared_from_this());
    return VariableRecord::getVariableRecords(other);
  }

  ProjectDatabaseRecord ProjectDatabase_Impl::projectDatabaseRecord() const {
    return m_projectDatabaseRecord.get();
  }

  bool ProjectDatabase_Impl::isDirty() const
  {
    return !(m_handleNewRecordMap.empty() && m_handleDirtyRecordMap.empty() && m_handleRemovedRecordMap.empty());
  }

  UUID ProjectDatabase_Impl::handle() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->handle();
  }

  std::string ProjectDatabase_Impl::name() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->name();
  }

  bool ProjectDatabase_Impl::setName(const std::string& name)
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->setName(name);
  }

  std::string ProjectDatabase_Impl::displayName() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->displayName();
  }

  bool ProjectDatabase_Impl::setDisplayName(const std::string& displayName)
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->setDisplayName(displayName);
  }

  std::string ProjectDatabase_Impl::description() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->description();
  }

  bool ProjectDatabase_Impl::setDescription(const std::string& description)
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->setDescription(description);
  }

  DateTime ProjectDatabase_Impl::timestampCreate() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->timestampCreate();
  }

  DateTime ProjectDatabase_Impl::timestampLast() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->timestampLast();
  }

  UUID ProjectDatabase_Impl::uuidLast() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->uuidLast();
  }

  std::string ProjectDatabase_Impl::version() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->version();
  }

  openstudio::path ProjectDatabase_Impl::path() const
  {
    return m_path;
  }

  openstudio::path ProjectDatabase_Impl::originalBasePath() const {
    return m_originalBasePath;
  }

  openstudio::path ProjectDatabase_Impl::newBasePath() const {
    return m_newBasePath;
  }

  void ProjectDatabase_Impl::setBasePaths(const openstudio::path& originalBasePath,
                                          const openstudio::path& newBasePath)
  {
    m_originalBasePath = originalBasePath;
    m_newBasePath = newBasePath;
  }

  openstudio::path ProjectDatabase_Impl::runManagerDBPath() const
  {
    OS_ASSERT(m_projectDatabaseRecord);
    return m_projectDatabaseRecord->runManagerDBPath();
  }

  openstudio::runmanager::RunManager ProjectDatabase_Impl::runManager() const
  {
    return m_runManager;
  }

  bool ProjectDatabase_Impl::startTransaction() const
  {
    bool test = false;
    if (m_qSqlDatabase->driver()->hasFeature(QSqlDriver::Transactions)){
      test = m_qSqlDatabase->transaction();
    }
    return test;
  }

  bool ProjectDatabase_Impl::commitTransaction() const
  {
    bool test = false;
    if (m_qSqlDatabase->driver()->hasFeature(QSqlDriver::Transactions)){
      test = m_qSqlDatabase->commit();
    }
    return test;
  }

  bool ProjectDatabase_Impl::save()
  {
    bool didStartTransaction = this->startTransaction();
    bool didChange = false;

    m_ignoreSignals = true;

    ProjectDatabase other(this->shared_from_this());

    // save new objects and move to clean
    std::map<UUID, Record>::iterator it = m_handleNewRecordMap.begin();
    std::map<UUID, Record>::iterator itend = m_handleNewRecordMap.end();
    for( ; it != itend; ++it){
      m_handleCleanRecordMap.insert(*it);
      it->second.saveRow(other);
      didChange = true;
    }
    m_handleNewRecordMap.clear();

    // save dirty objects and move to clean
    it = m_handleDirtyRecordMap.begin();
    itend = m_handleDirtyRecordMap.end();
    for( ; it != itend; ++it){
      m_handleCleanRecordMap.insert(*it);
      it->second.saveRow(other);
      didChange = true;
    }
    m_handleDirtyRecordMap.clear();

    // purge clean records with use count 1
    this->unloadUnusedCleanRecords();

    // delete removed objects
    if (!m_handleRemovedRecordMap.empty()){
      didChange = true;
    }
    m_handleRemovedRecordMap.clear();

    // delete remove undo scripts
    m_removeUndos.clear();

    // update the project database record
    if (didChange){
      OS_ASSERT(m_projectDatabaseRecord);
      m_projectDatabaseRecord->onChange();
      m_projectDatabaseRecord->saveRow(other);
    }

    if (didStartTransaction){
      bool didCommitTransaction = this->commitTransaction();
      OS_ASSERT(didCommitTransaction);
    }

    m_ignoreSignals = false;

    return true;
  }

  // find the handle from RemoveUndo
  struct HandleFinder{
    HandleFinder(const UUID& handle)
      : m_handle(handle)
    {}

    bool operator()(const RemoveUndo removeUndo)
    {
      return (m_handle == removeUndo.primaryHandle());
    }

    UUID m_handle;
  };

  bool ProjectDatabase_Impl::saveRecord(Record& record, bool topLevelObject)
  {
    bool didStartTransaction = false;
    if (topLevelObject){
      didStartTransaction = this->startTransaction();
      m_ignoreSignals = true;
    }

    ProjectDatabase other(this->shared_from_this());

    // save children
    BOOST_FOREACH(ObjectRecord childRecord, record.children()){
      this->saveRecord(childRecord, false);
    }

    // save new object and move to clean
    std::map<UUID, Record>::iterator it = m_handleNewRecordMap.find(record.handle());
    if(it != m_handleNewRecordMap.end()){
      m_handleCleanRecordMap.insert(*it);
      it->second.saveRow(other);
      m_handleNewRecordMap.erase(it);
    }

    // save dirty object and move to clean
    it = m_handleDirtyRecordMap.find(record.handle());
    if(it != m_handleDirtyRecordMap.end()){
      m_handleCleanRecordMap.insert(*it);
      it->second.saveRow(other);
      m_handleDirtyRecordMap.erase(it);
    }

    // remove undo struct for this record
    HandleFinder finder(record.handle());
    std::remove_if(m_removeUndos.begin(), m_removeUndos.end(), finder);

    // delete removed object
    it = m_handleRemovedRecordMap.find(record.handle());
    if(it != m_handleRemovedRecordMap.end()){
      // erase self
      m_handleRemovedRecordMap.erase(it);
    }

    if (topLevelObject){
      if (didStartTransaction){
        bool didCommitTransaction = this->commitTransaction();
        OS_ASSERT(didCommitTransaction);
      }
      m_ignoreSignals = false;
    }

    return true;
  }

  boost::optional<RemoveUndo> ProjectDatabase_Impl::removeRecord(Record& record, bool saveResult)
  {
    OS_ASSERT(this->handle() == record.projectDatabase().handle());

    ProjectDatabase other(this->shared_from_this());

    boost::optional<RemoveUndo> result;

    // if object is already removed just return
    if (isRemovedRecord(record)){
      return result;
    }

    // try to find object in one of the maps and remove it
    std::map<UUID, Record>::iterator it;
    bool found = false;
    RemoveUndo::RemoveSource removeSource;

    if (!found){
      it = m_handleNewRecordMap.find(record.handle());
      if (it != m_handleNewRecordMap.end()){
        found = true;
        removeSource = RemoveUndo::FromNew;
      }
    }

    if (!found){
      it = m_handleDirtyRecordMap.find(record.handle());
      if (it != m_handleDirtyRecordMap.end()){
        found = true;
        removeSource = RemoveUndo::FromDirty;
      }
    }

    if (!found){
      it = m_handleCleanRecordMap.find(record.handle());
      if (it != m_handleCleanRecordMap.end()){
        found = true;
        removeSource = RemoveUndo::FromClean;
      }
    }

    // no need to load from database, if it is in database and loaded should be in
    // one of the maps

    if (found){

      bool didStartTransaction = this->startTransaction();

      // revert the object before adding it to the removed records
      it->second.revertRow(other);

      // add to removed records
      m_handleRemovedRecordMap.insert(*it);

      // save the remove
      result = RemoveUndo(it->first, removeSource);

      // remove children
      BOOST_FOREACH(ObjectRecord objectRecord, it->second.children()){
        boost::optional<RemoveUndo> childRemoveUndo = removeRecord(objectRecord, false);
        if (childRemoveUndo){
          result->concat(*childRemoveUndo);
        }
      }

      // remove joins
      BOOST_FOREACH(JoinRecord joinRecord, it->second.joinRecords()){
        boost::optional<RemoveUndo> joinRemoveUndo = removeRecord(joinRecord, false);
        if (joinRemoveUndo){
          result->concat(*joinRemoveUndo);
        }
      }

      // remove runmanager workflow
      if (OptionalWorkflowRecord workflowRecord = it->second.optionalCast<WorkflowRecord>()) {
        std::string workflowName = workflowRecord->workflow().getName();
        runManager().deleteWorkflowByName(workflowName);
      }

      // save the undo
      if (saveResult){
        m_removeUndos.push_back(*result);
      }

      // erase object
      switch(removeSource){
      case RemoveUndo::FromNew:
        m_handleNewRecordMap.erase(it);
        break;
      case RemoveUndo::FromDirty:
        m_handleDirtyRecordMap.erase(it);
        break;
      case RemoveUndo::FromClean:
        m_handleCleanRecordMap.erase(it);
        break;
      default:
        OS_ASSERT(false);
      }

      // delete row
      record.removeRow(other);

      if (didStartTransaction){
        bool didCommitTransaction = this->commitTransaction();
        OS_ASSERT(didCommitTransaction);
      }

    }

    return result;
  }

  void ProjectDatabase_Impl::updatePathData(const openstudio::path& originalBase,
                                            const openstudio::path& newBase)
  {
    bool didStartTransaction = startTransaction();

    ProjectDatabase other(this->shared_from_this());
    ProjectDatabaseRecord::updatePathData(other,originalBase,newBase);

    AlgorithmRecord::updatePathData(other,originalBase,newBase);
    AnalysisRecord::updatePathData(other,originalBase,newBase);
    AttributeRecord::updatePathData(other,originalBase,newBase);
    CloudSessionRecord::updatePathData(other,originalBase,newBase);
    CloudSettingsRecord::updatePathData(other,originalBase,newBase);
    DataPointRecord::updatePathData(other,originalBase,newBase);
    DataPointValueRecord::updatePathData(other,originalBase,newBase);
    FileReferenceRecord::updatePathData(other,originalBase,newBase);
    FunctionRecord::updatePathData(other,originalBase,newBase);
    MeasureRecord::updatePathData(other,originalBase,newBase);
    ProblemRecord::updatePathData(other,originalBase,newBase);
    TagRecord::updatePathData(other,originalBase,newBase);
    UrlRecord::updatePathData(other,originalBase,newBase);
    URLSearchPathRecord::updatePathData(other,originalBase,newBase);
    OSArgumentRecord::updatePathData(other,originalBase,newBase);
    VariableRecord::updatePathData(other,originalBase,newBase);
    WorkflowRecord::updatePathData(other,originalBase,newBase);

    save();

    if (didStartTransaction) {
      bool test = this->commitTransaction();
      OS_ASSERT(test);
    }
  }

  void ProjectDatabase_Impl::addNewRecord(Record& record)
  {
    OS_ASSERT(this->handle() == record.projectDatabase().handle());

    ProjectDatabase other(this->shared_from_this());

    UUID handle = record.handle();

    // make sure object isn't already loaded
    OS_ASSERT(!findLoadedRecord(handle));

    // insert object, will get id
    record.insertRow(other);

    // save to database
    record.saveRow(other);

    // connect signals
    connectSignals(record);

    // insert into map
    m_handleNewRecordMap.insert(std::make_pair<UUID, Record>(handle, record));

  }

  void ProjectDatabase_Impl::addDirtyRecord(const Record& record)
  {
    OS_ASSERT(this->handle() == record.projectDatabase().handle());

    ProjectDatabase other(this->shared_from_this());

    UUID handle = record.handle();

    // make sure object isn't already loaded
    OS_ASSERT(!findLoadedRecord(handle));

    // connect signals
    connectSignals(record);

    // insert into map
    m_handleDirtyRecordMap.insert(std::make_pair<UUID, Record>(handle, record));

  }

  void ProjectDatabase_Impl::addCleanRecord(const Record& record)
  {
    OS_ASSERT(this->handle() == record.projectDatabase().handle());

    ProjectDatabase other(this->shared_from_this());

    UUID handle = record.handle();

    // make sure object isn't already loaded
    OS_ASSERT(!findLoadedRecord(handle));

    // connect signals
    connectSignals(record);

    // insert into map
    m_handleCleanRecordMap.insert(std::make_pair<UUID, Record>(handle, record));
  }

  void ProjectDatabase_Impl::updateDatabase(const std::string& dbVersion) {
    VersionString osv(openStudioVersion());
    VersionString dbv(dbVersion);

    // cannot update version 0.6.3 and below
    if (dbv <= VersionString("0.6.3")) {
      LOG_AND_THROW("Cannot update databases from versions 0.6.3 and before. "
                    << "This database is version " << dbVersion << ".");
    }

    if (dbv < VersionString("0.7.0")) {
      update_0_6_6_to_0_7_0(dbv);
    }

    if (dbv < VersionString("0.8.1")) {
      // warning this update also does partial updates from
      // update_0_8_3_to_0_8_4, and update_0_10_3_to_0_10_4
      // in order to use the current AlgorithmRecord constructor
      // if these updates are performed here they should not be performed
      // in subsequent update steps
      update_0_8_0_to_0_8_1(dbv);
    }

    if (dbv < VersionString("0.8.4")) {
      update_0_8_3_to_0_8_4(dbv);
    }

    if (dbv < VersionString("0.9.1")) {
      update_0_9_0_to_0_9_1(dbv);
    }

    if (dbv < VersionString("0.10.1")) {
      update_0_10_0_to_0_10_1(dbv);
    }

    if (dbv < VersionString("0.10.3")) {
      update_0_10_2_to_0_10_3(dbv);
    }

    if (dbv < VersionString("0.10.4")) {
      update_0_10_3_to_0_10_4(dbv);
    }

    if (dbv < VersionString("0.10.5")) {
      update_0_10_4_to_0_10_5(dbv);
    }

    if (dbv < VersionString("0.11.6")) {
      update_0_11_5_to_0_11_6(dbv);
    }

    if (dbv < VersionString("1.0.1")) {
      update_1_0_0_to_1_0_1(dbv);
    }

    if (dbv < VersionString("1.0.3")) {
      update_1_0_2_to_1_0_3(dbv);
    }

    if (dbv < VersionString("1.0.4")) {
      update_1_0_3_to_1_0_4(dbv);
    }

    if (dbv < VersionString("1.0.5")) {
      update_1_0_4_to_1_0_5(dbv);
    }

    if ((dbv != osv) || (!dbv.fidelityEqual(osv))) {
      LOG(Info,"Updating database version to " << osv << ".");
      bool didStartTransaction = startTransaction();
      OS_ASSERT(didStartTransaction);
      m_projectDatabaseRecord->setVersion(osv.str());
      save();
      bool test = this->commitTransaction();
      OS_ASSERT(test);
    }
  }

  bool ProjectDatabase_Impl::includesRecord(const Record& record) const
  {
    if (this->handle() != record.projectDatabase().handle()){
      return false;
    }
    if (isNewRecord(record)){
      return true;
    }
    if (isDirtyRecord(record)){
      return true;
    }
    if (isRemovedRecord(record)){
      return true;
    }
    if (isCleanRecord(record)){
      return true;
    }
    return false;
  }

  bool ProjectDatabase_Impl::isNewRecord(const Record& record) const
  {
    if (this->handle() != record.projectDatabase().handle()){
      return false;
    }

    std::map<UUID, Record>::const_iterator it = m_handleNewRecordMap.find(record.handle());
    if (it != m_handleNewRecordMap.end()){
      return true;
    }

    return false;
  }

  bool ProjectDatabase_Impl::isCleanRecord(const Record& record) const
  {
    if (this->handle() != record.projectDatabase().handle()){
      return false;
    }

    std::map<UUID, Record>::const_iterator it = m_handleCleanRecordMap.find(record.handle());
    if (it != m_handleCleanRecordMap.end()){
      return true;
    }

    bool result = false;
    if (record.findIdByHandle()){
      result = (!isNewRecord(record) && !isDirtyRecord(record) && !isRemovedRecord(record));
    }

    return result;
  }

  bool ProjectDatabase_Impl::isDirtyRecord(const Record& record) const
  {
    if (this->handle() != record.projectDatabase().handle()){
      return false;
    }

    std::map<UUID, Record>::const_iterator it = m_handleDirtyRecordMap.find(record.handle());
    if (it != m_handleDirtyRecordMap.end()){
      return true;
    }

    return false;
  }

  bool ProjectDatabase_Impl::isRemovedRecord(const Record& record) const
  {
    if (this->handle() != record.projectDatabase().handle()){
      return false;
    }

    std::map<UUID, Record>::const_iterator it = m_handleRemovedRecordMap.find(record.handle());
    if (it != m_handleRemovedRecordMap.end()){
      return true;
    }

    return false;
  }

  void ProjectDatabase_Impl::recordChanged(const UUID& handle)
  {
    if (!m_ignoreSignals){
      // new and removed objects stay where they are

      // move clean objects to dirty
      std::map<UUID, Record>::iterator it = m_handleCleanRecordMap.find(handle);
      if (it != m_handleCleanRecordMap.end()){
        m_handleDirtyRecordMap.insert(*it);
        m_handleCleanRecordMap.erase(it);
      }
    }
  }

  boost::shared_ptr<QSqlDatabase> ProjectDatabase_Impl::qSqlDatabase() const
  {
    return m_qSqlDatabase;
  }

  boost::optional<Record> ProjectDatabase_Impl::findLoadedRecord(const UUID& handle) const
  {
    boost::optional<Record> result;

    std::map<UUID, Record>::const_iterator it = m_handleNewRecordMap.find(handle);
    if (it != m_handleNewRecordMap.end()){
      OS_ASSERT(!result);
      result = it->second;
    }

    it = m_handleDirtyRecordMap.find(handle);
    if (it != m_handleDirtyRecordMap.end()){
      OS_ASSERT(!result);
      result = it->second;
    }

    it = m_handleCleanRecordMap.find(handle);
    if (it != m_handleCleanRecordMap.end()){
      OS_ASSERT(!result);
      result = it->second;
    }

    it = m_handleRemovedRecordMap.find(handle);
    if (it != m_handleRemovedRecordMap.end()){
      OS_ASSERT(!result);
      result = it->second;
    }

    if (result) {
      Record record = *result;
      OS_ASSERT(handle == record.handle());
      std::string passedInHandleString = toString(handle);
      std::string loadedHandleString = toString(record.handle());
      OS_ASSERT(passedInHandleString == loadedHandleString);
    }
    return result;
  }

  void ProjectDatabase_Impl::unloadUnusedCleanRecords()
  {

    std::map<UUID, Record>::iterator it = m_handleCleanRecordMap.begin();
    std::map<UUID, Record>::iterator itend = m_handleCleanRecordMap.end();

    // there is no remove_if equivalent for maps, this is example provided for equivalent functionality
    for(; it != itend; ) {
      if (it->second.useCount() == 1) {
        m_handleCleanRecordMap.erase(it++);
      } else {
        ++it;
      }
    }

  }

  void ProjectDatabase_Impl::commitRemove(const RemoveUndo& removeUndo)
  {
    typedef std::pair<UUID, RemoveUndo::RemoveSource> RemovedObjectType;
    BOOST_FOREACH(RemovedObjectType removedObject, removeUndo.removedObjects()){
      // delete removed object
      std::map<UUID, Record>::iterator it = m_handleRemovedRecordMap.find(removedObject.first);
      if(it != m_handleRemovedRecordMap.end()){
        // erase removed object
        m_handleRemovedRecordMap.erase(it);
      }
    }
  }

  void ProjectDatabase_Impl::connectSignals(const Record& record) const
  {
    bool test = record.connect(SIGNAL(onChange(const UUID&)), this, SLOT(recordChanged(const UUID&)));
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::initialize() {
    bool didStartTransaction = this->startTransaction();
    OS_ASSERT(didStartTransaction);

    // create object tables
    createTable<AlgorithmRecord>();
    createTable<AnalysisRecord>();
    createTable<AttributeRecord>();
    createTable<CloudSessionRecord>();
    createTable<CloudSettingsRecord>();
    createTable<DataPointRecord>();
    createTable<DataPointValueRecord>();
    createTable<FileReferenceRecord>();
    createTable<FunctionRecord>();
    createTable<MeasureRecord>();
    createTable<ProblemRecord>();
    createTable<ProjectDatabaseRecord>();
    createTable<TagRecord>();
    createTable<OSArgumentRecord>();
    createTable<UrlRecord>();
    createTable<URLSearchPathRecord>();
    createTable<VariableRecord>();
    createTable<WorkflowRecord>();

    // create join tables
    createTable<DataPoint_Measure_JoinRecord>();

    bool test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_6_6_to_0_7_0(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // add functionType column to FunctionRecords and set any entries to FunctionType::Objective
    LOG(Info,"Adding functionType column to " << FunctionRecord::databaseTableName() << ".");

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    FunctionRecordColumns functionTypeColumn("functionType");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + FunctionRecord::databaseTableName() + " ADD COLUMN " +
        functionTypeColumn.valueName() + " " + functionTypeColumn.valueDescription()));
    assertExec(query);
    query.clear();

    query.prepare(toQString("UPDATE " + FunctionRecord::databaseTableName() +
                            " SET functionType=:functionType"));
    query.bindValue(":functionType",FunctionType::Objective);
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_8_0_to_0_8_1(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    LOG(Info,"Adding attributeVectorIndex and algorithmRecordId columns to AttributeRecords.");
    AttributeRecordColumns attributeVectorIndexColumn("attributeVectorIndex");
    query.prepare(QString::fromStdString("ALTER TABLE AttributeRecords ADD COLUMN " +
                                         attributeVectorIndexColumn.valueName() + " " +
                                         attributeVectorIndexColumn.valueDescription()));
    assertExec(query);
    query.clear();
    AttributeRecordColumns algorithmRecordIdColumn("algorithmRecordId");
    query.prepare(QString::fromStdString("ALTER TABLE AttributeRecords ADD COLUMN " +
                                         algorithmRecordIdColumn.valueName() + " " +
                                         algorithmRecordIdColumn.valueDescription()));
    assertExec(query);
    query.clear();

    // create values for attributeVectorIndex
    query.prepare(QString("SELECT * FROM AttributeRecords WHERE parentAttributeRecordId != NULL ORDER BY id"));
    assertExec(query);
    std::vector< std::pair<int,int> > currentIndices;
    QSqlQuery attUpdateQuery(*(database.qSqlDatabase()));
    while (query.next()) {
      int parentId = query.value(AttributeRecordColumns::parentAttributeRecordId).toInt();
      std::vector< std::pair<int,int> >::reverse_iterator rit = std::find_if(
          currentIndices.rbegin(),currentIndices.rend(),boost::bind(firstOfPairEqual<int,int>,_1,parentId));
      if (rit == currentIndices.rend()) {
        currentIndices.push_back(std::pair<int,int>(parentId,-1));
        rit = currentIndices.rbegin();
      }
      ++(rit->second);
      attUpdateQuery.prepare(QString("UPDATE AttributeRecords SET attributeVectorIndex=:attributeVectorIndex WHERE id=:id"));
      attUpdateQuery.bindValue(":attributeVectorIndex",rit->second);
      attUpdateQuery.bindValue(":id",query.value(AttributeRecordColumns::id));
      assertExec(attUpdateQuery);
      attUpdateQuery.clear();
    }
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // pre-0.8.1 AlgorithmRecordColumns
    /*OPENSTUDIO_ENUM(AlgorithmRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((algorithmRecordType)(INTEGER)(8))
                ((openStudioAlgorithmRecordType)(INTEGER)(9))
                ((dakotaAlgorithmRecordType)(INTEGER)(10))
                ((iter)(INTEGER)(11))
                ((maxIter)(INTEGER)(12))
                ((maxSims)(INTEGER)(13))
                ((designOfExperimentsType)(INTEGER)(14))
                ((objectiveToMinimizeFirst)(INTEGER)(15))
                ((ddaceAlgorithmType)(INTEGER)(16))
                ((seed)(INTEGER)(17))
                ((samples)(INTEGER)(18))
                ((symbols)(INTEGER)(19))
                );*/

    LOG(Info,"Restructuring AlgorithmRecords to store most options as AttributeRecords.");

    // rename the current AlgorithmRecords table and make a new one in the new format.
    query.prepare(QString("DROP INDEX AlgorithmRecordshandleindex"));
    assertExec(query);
    query.clear();
    query.prepare(QString("ALTER TABLE AlgorithmRecords RENAME TO AlgorithmRecordsOldFormat"));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // warning this will create the table in current format not in 0.8.1 format
    createTable<AlgorithmRecord>();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // extract previous data
    query.prepare(QString("SELECT * FROM AlgorithmRecordsOldFormat"));
    assertExec(query);
    QSqlQuery algAddQuery(*(database.qSqlDatabase()));
    algAddQuery.prepare(QString::fromStdString(
        "INSERT INTO " + AlgorithmRecord::databaseTableName() +
        " (id, handle, name, displayName, description, timestampCreate, timestampLast, " +
        "uuidLast, iter, algorithmRecordType, openStudioAlgorithmRecordType, " +
        "dakotaAlgorithmRecordType, designOfExperimentsType, ddaceAlgorithmType, " +
        "complete, failed) " +
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
    std::vector< std::pair<int,std::vector<Attribute> > > algorithmOptions;

    // loop through and move data to new format
    QVariantList ids, handles, names, displayNames, descriptions, timestampCreates;
    QVariantList timestampLasts, uuidLasts, iters, algorithmRecordTypes;
    QVariantList openstudioAlgorithmRecordTypes, dakotaAlgorithmRecordTypes;
    QVariantList designOfExperimentsTypes, ddaceAlgorithmTypes;
    QVariantList complete, failed;
    while (query.next()) {
      AttributeVector attributes;

      QVariant value = query.value(0);
      OS_ASSERT(value.isValid() && !value.isNull());
      int id = value.toInt();
      ids << value;

      handles << query.value(1);
      names << query.value(2);
      displayNames << query.value(3);
      descriptions << query.value(4);
      timestampCreates << query.value(5);
      timestampLasts << query.value(6);
      uuidLasts << query.value(7);
      algorithmRecordTypes << query.value(8);
      openstudioAlgorithmRecordTypes << query.value(9);
      dakotaAlgorithmRecordTypes << query.value(10);
      iters << query.value(11);

      value = query.value(12);
      if (value.isValid() && !value.isNull()) {
        attributes.push_back(Attribute("maxIter",value.toInt()));
      }

      value = query.value(13);
      if (value.isValid() && !value.isNull()) {
        attributes.push_back(Attribute("maxSims",value.toInt()));
      }

      designOfExperimentsTypes << query.value(14);

      value = query.value(15);
      if (value.isValid() && !value.isNull()) {
        attributes.push_back(Attribute("objectiveToMinimizeFirst",value.toInt()));
      }

      ddaceAlgorithmTypes << query.value(16);

      value = query.value(17);
      if (value.isValid() && !value.isNull()) {
        attributes.push_back(Attribute("seed",value.toInt()));
      }

      value = query.value(18);
      if (value.isValid() && !value.isNull()) {
        attributes.push_back(Attribute("samples",value.toInt()));
      }

      value = query.value(19);
      if (value.isValid() && !value.isNull()) {
        attributes.push_back(Attribute("symbols",value.toInt()));
      }

      if (!attributes.empty()) {
        algorithmOptions.push_back(std::pair<int,std::vector<Attribute> >(id,attributes));
      }

      complete << QVariant(false);
      failed << QVariant(false);
    }
    algAddQuery.addBindValue(ids);
    algAddQuery.addBindValue(handles);
    algAddQuery.addBindValue(names);
    algAddQuery.addBindValue(displayNames);
    algAddQuery.addBindValue(descriptions);
    algAddQuery.addBindValue(timestampCreates);
    algAddQuery.addBindValue(timestampLasts);
    algAddQuery.addBindValue(uuidLasts);
    algAddQuery.addBindValue(iters);
    algAddQuery.addBindValue(algorithmRecordTypes);
    algAddQuery.addBindValue(openstudioAlgorithmRecordTypes);
    algAddQuery.addBindValue(dakotaAlgorithmRecordTypes);
    algAddQuery.addBindValue(designOfExperimentsTypes);
    algAddQuery.addBindValue(ddaceAlgorithmTypes);
    algAddQuery.addBindValue(complete);
    algAddQuery.addBindValue(failed);
    test = algAddQuery.execBatch();
    OS_ASSERT(test);
    algAddQuery.clear();
    query.clear();

    // Code from update_0_8_3_to_0_8_4 needed to make AttributeRecord constructor work.
    LOG(Info,"Adding variableRecordId column to AttributeRecords.");
    AttributeRecordColumns variableRecordIdColumn("variableRecordId");
    query.prepare(QString::fromStdString("ALTER TABLE AttributeRecords ADD COLUMN " +
                                         variableRecordIdColumn.valueName() + " " +
                                         variableRecordIdColumn.valueDescription()));
    assertExec(query);
    query.clear();

    for (std::vector< std::pair<int,std::vector<Attribute> > >::const_iterator it = algorithmOptions.begin(),
         itEnd = algorithmOptions.end(); it != itEnd; ++it)
    {
      AlgorithmRecord algRec = AlgorithmRecord::getAlgorithmRecord(it->first,database).get();
      BOOST_FOREACH(const Attribute& att, it->second) {
        AttributeRecord algOpt(att,algRec);
      }
    }

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // delete old format table
    query.prepare(QString("DROP TABLE AlgorithmRecordsOldFormat"));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_8_3_to_0_8_4(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    LOG(Info,"Adding uncertaintyDescriptionType column to VariableRecords.");
    VariableRecordColumns uncertaintyDescriptionTypeColumn("uncertaintyDescriptionType");
    query.prepare(QString::fromStdString("ALTER TABLE VariableRecords ADD COLUMN " +
                                         uncertaintyDescriptionTypeColumn.valueName() + " " +
                                         uncertaintyDescriptionTypeColumn.valueDescription()));
    assertExec(query);
    query.clear();

    if (startVersion > VersionString("0.8.0")) {
      // this change has not been made yet
      LOG(Info,"Adding variableRecordId column to AttributeRecords.");
      AttributeRecordColumns variableRecordIdColumn("variableRecordId");
      query.prepare(QString::fromStdString("ALTER TABLE AttributeRecords ADD COLUMN " +
                                           variableRecordIdColumn.valueName() + " " +
                                           variableRecordIdColumn.valueDescription()));
      assertExec(query);
      query.clear();
    }

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_9_0_to_0_9_1(const VersionString &startVersion) {
    if (startVersion > VersionString("0.8.0")) {
      // If goes through 0.8.0 to 0.8.1 translator, automatically gets up-to-date columns.
      bool didStartTransaction = startTransaction();
      OS_ASSERT(didStartTransaction);

      ProjectDatabase database(this->shared_from_this());
      QSqlQuery query(*(database.qSqlDatabase()));

      LOG(Info,"Adding complete and failed columns to AlgorithmRecords.");
      AlgorithmRecordColumns algorithmCompleteColumn("complete");
      query.prepare(QString::fromStdString("ALTER TABLE AlgorithmRecords ADD COLUMN " +
                                           algorithmCompleteColumn.valueName() + " " +
                                           algorithmCompleteColumn.valueDescription() +
                                           " DEFAULT false"));
      assertExec(query);
      query.clear();
      AlgorithmRecordColumns algorithmFailedColumn("failed");
      query.prepare(QString::fromStdString("ALTER TABLE AlgorithmRecords ADD COLUMN " +
                                           algorithmFailedColumn.valueName() + " " +
                                           algorithmFailedColumn.valueDescription() +
                                           " DEFAULT false"));
      assertExec(query);
      query.clear();

      LOG(Info,"Adding dakotaRestartFileRecordId and dakotaOutFileRecordId columns to AlgorithmRecords.");
      AlgorithmRecordColumns dakotaRestartFileRecordIdColumn("dakotaRestartFileRecordId");
      query.prepare(QString::fromStdString("ALTER TABLE AlgorithmRecords ADD COLUMN " +
                                           dakotaRestartFileRecordIdColumn.valueName() + " " +
                                           dakotaRestartFileRecordIdColumn.valueDescription()));
      assertExec(query);
      query.clear();
      AlgorithmRecordColumns dakotaOutFileRecordIdColumn("dakotaOutFileRecordId");
      query.prepare(QString::fromStdString("ALTER TABLE AlgorithmRecords ADD COLUMN " +
                                           dakotaOutFileRecordIdColumn.valueName() + " " +
                                           dakotaOutFileRecordIdColumn.valueDescription()));
      assertExec(query);
      query.clear();

      save();
      bool test = this->commitTransaction();
      OS_ASSERT(test);
    }
  }

  void ProjectDatabase_Impl::update_0_10_0_to_0_10_1(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    // pre-0.10.1 UserScriptArgumentRecordColumns
    // OPENSTUDIO_ENUM(UserScriptArgumentRecordColumns,
    //   ((id)(INTEGER PRIMARY KEY)(0))
    //   ((handle)(TEXT)(1))
    //   ((name)(TEXT)(2))
    //   ((displayName)(TEXT)(3))
    //   ((description)(TEXT)(4))
    //   ((timestampCreate)(TEXT)(5))
    //   ((timestampLast)(TEXT)(6))
    //   ((uuidLast)(TEXT)(7))
    //   ((rubyPerturbationRecordId)(INTEGER)(8))
    //   ((rubyContinuousVariableRecordId)(INTEGER)(9))
    //   ((userScriptArgumentType)(INTEGER)(10))
    //   ((required)(BOOLEAN)(11))
    //   ((argumentValue)(TEXT)(12))
    //   ((defaultArgumentValue)(TEXT)(13))
    //   ((choices)(TEXT)(14))
    //   ((isRead)(BOOLEAN)(15))
    //   ((extension)(TEXT)(16))
    //   ((iddObjectType)(TEXT)(17))
    //   ((referenceName)(TEXT)(18))
    //   ((workspaceFileReferenceRecordId)(INTEGER)(19))
    // );

    LOG(Info,"Renaming and restructuring UserScriptArgumentRecords, now OSArgumentRecords.");
    createTable<OSArgumentRecord>();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // extract previous data
    query.prepare(QString("SELECT * FROM UserScriptArgumentRecords"));
    assertExec(query);
    QSqlQuery argAddQuery(*(database.qSqlDatabase()));
    argAddQuery.prepare(QString::fromStdString(
        "INSERT INTO " + OSArgumentRecord::databaseTableName() +
        " (id, handle, name, displayName, description, timestampCreate, timestampLast, " +
        "uuidLast, rubyPerturbationRecordId, rubyContinuousVariableRecordId, " +
        "argumentType, required, argumentValue, defaultArgumentValue, domainType, " +
        "domainValues, choices, choiceDisplayNames, isRead, extension) " +
        "VALUES (?, ?, ?, ?, ?, ?, ?, " +
                "?, ?, ?, " +
                "?, ?, ?, ?, ?, " +
                "?, ?, ?, ?, ?)"));

    // loop through and move data to new format
    QVariantList ids, handles, names, displayNames, descriptions, timestampCreates, timestampLasts;
    QVariantList uuidLasts, rubyPerturbationRecordIds, rubyContinuousVariableRecordIds;
    QVariantList argumentTypes, requireds, argumentValues, defaultArgumentValues, domainTypes;
    QVariantList domains, choices, choiceDisplayNames, isReads, extensions;
    while (query.next()) {
      QVariant value;

      // do not keep record if userScriptArgumentType is 7 (WorkspaceObject)
      value = query.value(10);
      OS_ASSERT(value.isValid() && !value.isNull());
      int argTypeCode = value.toInt();
      if (argTypeCode == 7) {
        LOG(Warn,"The WorkspaceObject argument type has been deprecated. The ProjectDatabase "
            << "version translation process is dropping an argument of this type named '"
            << query.value(2).toString().toStdString() << "' from the " << "database at "
            << toString(path()) << ".");
        continue;
      }

      ids << query.value(0);
      handles << query.value(1);
      names << query.value(2);
      displayNames << query.value(3);
      descriptions << query.value(4);
      timestampCreates << query.value(5);
      timestampLasts << query.value(6);

      uuidLasts << query.value(7);
      rubyPerturbationRecordIds << query.value(8);
      rubyContinuousVariableRecordIds << query.value(9);

      argumentTypes << query.value(10);
      requireds << query.value(11);
      argumentValues << query.value(12);
      defaultArgumentValues << query.value(13);
      ruleset::OSDomainType domainType(ruleset::OSDomainType::Enumeration);
      switch (argTypeCode) {
        case (1):
        case (2):
        case (3):
          domainType = ruleset::OSDomainType(ruleset::OSDomainType::Interval);
          break;
        default:
          break;
      }
      domainTypes << QVariant(domainType.value());

      domains << QVariant(QString());
      choices << query.value(14);
      choiceDisplayNames << QVariant(QString(""));
      isReads << query.value(15);
      extensions << query.value(16);
    }
    argAddQuery.addBindValue(ids);
    argAddQuery.addBindValue(handles);
    argAddQuery.addBindValue(names);
    argAddQuery.addBindValue(displayNames);
    argAddQuery.addBindValue(descriptions);
    argAddQuery.addBindValue(timestampCreates);
    argAddQuery.addBindValue(timestampLasts);

    argAddQuery.addBindValue(uuidLasts);
    argAddQuery.addBindValue(rubyPerturbationRecordIds);
    argAddQuery.addBindValue(rubyContinuousVariableRecordIds);

    argAddQuery.addBindValue(argumentTypes);
    argAddQuery.addBindValue(requireds);
    argAddQuery.addBindValue(argumentValues);
    argAddQuery.addBindValue(defaultArgumentValues);
    argAddQuery.addBindValue(domainTypes);

    argAddQuery.addBindValue(domains);
    argAddQuery.addBindValue(choices);
    argAddQuery.addBindValue(choiceDisplayNames);
    argAddQuery.addBindValue(isReads);
    argAddQuery.addBindValue(extensions);

    test = argAddQuery.execBatch();
    OS_ASSERT(test);
    argAddQuery.clear();
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // delete old table
    query.prepare(QString("DROP TABLE UserScriptArgumentRecords"));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_10_2_to_0_10_3(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Adding resultsAreInvalid and dataPointsAreInvalid columns to "
        << AnalysisRecord::databaseTableName() << ".");

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    AnalysisRecordColumns resultsAreInvalidColumn("resultsAreInvalid");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + AnalysisRecord::databaseTableName() + " ADD COLUMN " +
        resultsAreInvalidColumn.valueName() + " " + resultsAreInvalidColumn.valueDescription()));
    assertExec(query);
    query.clear();
    AnalysisRecordColumns dataPointsAreInvalidColumn("dataPointsAreInvalid");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + AnalysisRecord::databaseTableName() + " ADD COLUMN " +
        dataPointsAreInvalidColumn.valueName() + " " +
        dataPointsAreInvalidColumn.valueDescription()));
    assertExec(query);
    query.clear();

    query.prepare(toQString("UPDATE " + AnalysisRecord::databaseTableName() +
                            " SET resultsAreInvalid=:resultsAreInvalid"));
    query.bindValue(":resultsAreInvalid",false);
    assertExec(query);
    query.clear();

    query.prepare(toQString("UPDATE " + AnalysisRecord::databaseTableName() +
                            " SET dataPointsAreInvalid=:dataPointsAreInvalid"));
    query.bindValue(":dataPointsAreInvalid",false);
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_10_3_to_0_10_4(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    LOG(Info,"Adding usesBCLMeasure column to DiscretePerturbationRecords.");

    query.prepare(QString::fromStdString(
        "ALTER TABLE DiscretePerturbationRecords ADD COLUMN usesBCLMeasure BOOLEAN"));
    assertExec(query);
    query.clear();

    query.prepare(QString::fromStdString(
        "UPDATE DiscretePerturbationRecords SET usesBCLMeasure=:usesBCLMeasure"));
    query.bindValue(":usesBCLMeasure",false);
    assertExec(query);
    query.clear();

    LOG(Info,"Adding topLevelJobUUID and dakotaParametersFiles columns to "
      << DataPointRecord::databaseTableName() << ".");

    DataPointRecordColumns topLevelJobUUIDColumn = DataPointRecordColumns::topLevelJobUUID;
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + DataPointRecord::databaseTableName() + " ADD COLUMN " +
        topLevelJobUUIDColumn.valueName() + " " + topLevelJobUUIDColumn.valueDescription()));
    assertExec(query);
    query.clear();

    DataPointRecordColumns dakotaParametersFilesColumn = DataPointRecordColumns::dakotaParametersFiles;
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + DataPointRecord::databaseTableName() + " ADD COLUMN " +
        dakotaParametersFilesColumn.valueName() + " " +
        dakotaParametersFilesColumn.valueDescription()));
    assertExec(query);
    query.clear();

    query.prepare(toQString("UPDATE " + DataPointRecord::databaseTableName() +
                            " SET topLevelJobUUID=:topLevelJobUUID"));
    query.bindValue(":topLevelJobUUID", QVariant(QVariant::String));
    assertExec(query);
    query.clear();

    query.prepare(toQString("UPDATE " + DataPointRecord::databaseTableName() +
                            " SET dakotaParametersFiles=:dakotaParametersFiles"));
    query.bindValue(":dakotaParametersFiles", QVariant(QVariant::String));
    assertExec(query);
    query.clear();

    if (startVersion > VersionString("0.8.0")) {

      LOG(Info,"Adding jobUUID column to " << AlgorithmRecord::databaseTableName() << ".");

      // this change has not been made yet
      AlgorithmRecordColumns jobUUIDColumn = AlgorithmRecordColumns::jobUUID;
      query.prepare(QString::fromStdString(
          "ALTER TABLE " + AlgorithmRecord::databaseTableName() + " ADD COLUMN " +
          jobUUIDColumn.valueName() + " " +
          jobUUIDColumn.valueDescription()));
      assertExec(query);
      query.clear();

      query.prepare(toQString("UPDATE " + AlgorithmRecord::databaseTableName() +
                              " SET jobUUID=:jobUUID"));
      query.bindValue(":jobUUID", QVariant(QVariant::String));
      assertExec(query);
      query.clear();
    }

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_10_4_to_0_10_5(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Changing inheritance hierarchy of " << VariableRecord::databaseTableName() << ".");

    // pre-0.10.5 VariableRecordColumns
    // OPENSTUDIO_ENUM(VariableRecordColumns,
    //                 ((id)(INTEGER PRIMARY KEY)(0))
    //                 ((handle)(TEXT)(1))
    //                 ((name)(TEXT)(2))
    //                 ((displayName)(TEXT)(3))
    //                 ((description)(TEXT)(4))
    //                 ((timestampCreate)(TEXT)(5))
    //                 ((timestampLast)(TEXT)(6))
    //                 ((uuidLast)(TEXT)(7))
    //                 ((problemRecordId)(INTEGER)(8))
    //                 ((functionRecordId)(INTEGER)(9))
    //                 ((variableVectorIndex)(INTEGER)(10))
    //                 ((functionCoefficient)(REAL)(11))
    //                 ((variableRecordType)(INTEGER)(12))
    //                 ((continuousVariableRecordType)(INTEGER)(13))
    //                 ((minimum)(REAL)(14))
    //                 ((maximum)(REAL)(15))
    //                 ((increment)(REAL)(16))
    //                 ((nSteps)(INTEGER)(17))
    //                 ((attributeName)(TEXT)(18))
    //                 ((rulesetRecordId)(INTEGER)(19))
    //                 ((rubyPerturbationRecordId)(INTEGER)(20))
    //                 ((uncertaintyDescriptionType)(INTEGER)(21))
    //                 );

    // Add columns to VariableRecords

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    VariableRecordColumns inputVariableRecordTypeColumn("inputVariableRecordType");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + VariableRecord::databaseTableName() + " ADD COLUMN " +
        inputVariableRecordTypeColumn.valueName() + " " +
        inputVariableRecordTypeColumn.valueDescription()));
    assertExec(query);
    query.clear();
    VariableRecordColumns outputVariableRecordTypeColumn("outputVariableRecordType");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + VariableRecord::databaseTableName() + " ADD COLUMN " +
        outputVariableRecordTypeColumn.valueName() + " " +
        outputVariableRecordTypeColumn.valueDescription()));
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // Update each row in VariableRecords to follow the new hierarchy

    // pre-0.10.5 VariableRecordType
    // OPENSTUDIO_ENUM(VariableRecordType,
    //                 ((DiscreteVariableRecord)(DiscreteVariableRecord)(0))
    //                 ((ContinuousVariableRecord)(ContinuousVariableRecord)(1))
    //                 );

    // pre-0.10.5 ContinuousVariableRecordType
    // OPENSTUDIO_ENUM(ContinuousVariableRecordType,
    //   ((OutputAttributeContinuousVariableRecord))
    //   ((ModelRulesetContinuousVariableRecord))
    //   ((RubyContinuousVariableRecord))
    // );

    // discrete variables
    query.prepare(QString::fromStdString("UPDATE " + VariableRecord::databaseTableName() +
        " SET variableRecordType=:variableRecordType, " +
        "inputVariableRecordType=:inputVariableRecordType WHERE variableRecordType=0"));
    query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
    query.bindValue(":inputVariableRecordType",InputVariableRecordType::DiscreteVariableRecord);
    assertExec(query);
    query.clear();

    // ruby continuous variables
    query.prepare(QString::fromStdString("UPDATE " + VariableRecord::databaseTableName() +
        " SET variableRecordType=:variableRecordType, " +
        "inputVariableRecordType=:inputVariableRecordType, " +
        "continuousVariableRecordType=:continuousVariableRecordType " +
        "WHERE variableRecordType=1 AND continuousVariableRecordType=2"));
    query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
    query.bindValue(":inputVariableRecordType",
                    InputVariableRecordType::ContinuousVariableRecord);
    query.bindValue(":continuousVariableRecordType",1);
    assertExec(query);
    query.clear();

    // model ruleset continuous variables
    query.prepare(QString::fromStdString("UPDATE " + VariableRecord::databaseTableName() +
        " SET variableRecordType=:variableRecordType, " +
        "inputVariableRecordType=:inputVariableRecordType, " +
        "continuousVariableRecordType=:continuousVariableRecordType " +
        "WHERE variableRecordType=1 AND continuousVariableRecordType=1"));
    query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
    query.bindValue(":inputVariableRecordType",
                    InputVariableRecordType::ContinuousVariableRecord);
    query.bindValue(":continuousVariableRecordType",0);
    assertExec(query);
    query.clear();

    // output attribute variables
    query.prepare(QString::fromStdString("UPDATE " + VariableRecord::databaseTableName() +
        " SET variableRecordType=:variableRecordType, " +
        "outputVariableRecordType=:outputVariableRecordType, " +
        "continuousVariableRecordType=:continuousVariableRecordType " +
        "WHERE variableRecordType=1 AND continuousVariableRecordType=0"));
    query.bindValue(":variableRecordType",VariableRecordType::OutputVariableRecord);
    query.bindValue(":outputVariableRecordType",
                    OutputVariableRecordType::OutputAttributeVariableRecord);
    query.bindValue(":continuousVariableRecordType",QVariant(QVariant::Int)); // nullify
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Changing how Problems refer to their runmanager::Workflows.");

    // Add columns to WorkflowRecords

    WorkflowRecordColumns problemRecordIdColumn("problemRecordId");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + WorkflowRecord::databaseTableName() + " ADD COLUMN " +
        problemRecordIdColumn.valueName() + " " +
        problemRecordIdColumn.valueDescription()));
    assertExec(query);
    query.clear();
    WorkflowRecordColumns workflowIndexColumn("workflowIndex");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + WorkflowRecord::databaseTableName() + " ADD COLUMN " +
        workflowIndexColumn.valueName() + " " + workflowIndexColumn.valueDescription()));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // Populate new WorkflowRecords columns with data from ProblemRecords and VariableRecords
    query.prepare(QString::fromStdString("SELECT id, workflowRecordId FROM " +
        ProblemRecord::databaseTableName()));
    assertExec(query);
    QSqlQuery innerQuery(*(database.qSqlDatabase()));
    while (query.next()) {
      int problemId = query.value(ProblemRecordColumns::id).toInt();
      int workflowId = query.value(ProblemRecordColumns::workflowRecordId).toInt();
      innerQuery.prepare(QString::fromStdString("SELECT COUNT(id) FROM " +
          VariableRecord::databaseTableName() + " WHERE problemRecordId=:problemRecordId"));
      innerQuery.bindValue(":problemRecordId",problemId);
      assertExec(innerQuery);
      int numVars(0);
      if (innerQuery.first()) {
        numVars = innerQuery.value(0).toInt();
      }
      innerQuery.clear();
      innerQuery.prepare(QString::fromStdString("UPDATE " +
          WorkflowRecord::databaseTableName() + " SET problemRecordId=:problemRecordId, " +
          "workflowIndex=:workflowIndex WHERE id=:id"));
      innerQuery.bindValue(":problemRecordId",problemId);
      innerQuery.bindValue(":workflowIndex",numVars);
      innerQuery.bindValue(":id",workflowId);
      assertExec(innerQuery);
      innerQuery.clear();
    }
    query.clear();

    // Null out ProblemRecords's workflowRecordId column
    query.prepare(QString::fromStdString("UPDATE " + ProblemRecord::databaseTableName() +
        " SET workflowRecordId=:workflowRecordId"));
    query.bindValue(":workflowRecordId",QVariant(QVariant::Int));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_0_11_5_to_0_11_6(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Adding column for IDF input data to " << DataPointRecord::databaseTableName() << ".");

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    DataPointRecordColumns idfInputDataRecordIdColumn("idfInputDataRecordId");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + DataPointRecord::databaseTableName() + " ADD COLUMN " +
        idfInputDataRecordIdColumn.valueName() + " " +
        idfInputDataRecordIdColumn.valueDescription()));
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // If there is already an inputDataRecordId, move it to idfInputDataRecordId if
    // it points to an IDF file.
    query.prepare(QString::fromStdString("UPDATE " + DataPointRecord::databaseTableName() +
        " SET inputDataRecordId=NULL, idfInputDataRecordId=inputDataRecordId " +
        "WHERE EXISTS (" +
            "SELECT * FROM " + FileReferenceRecord::databaseTableName() + " f " +
            "WHERE f.id=inputDataRecordId AND f.fileReferenceType=:fileReferenceType)"));
    query.bindValue(":fileReferenceType",int(FileReferenceType::IDF));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_1_0_0_to_1_0_1(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Dropping deprecated table RulesetOptionRecords.");

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    query.prepare(QString("DROP TABLE RulesetOptionRecords"));
    assertExec(query);
    query.clear();

    save();
  	bool test = this->commitTransaction();
    OS_ASSERT(test);

    LOG(Info,"Removing all standards rule and ruleset information.");

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // remove RulesetRecord entries with rulesetRecordType == 1
    query.prepare(QString::fromStdString("DELETE FROM RulesetRecords WHERE rulesetRecordType=1"));
    assertExec(query);
    query.clear();

    save();
  	test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // remove RuleRecord entries with ruleRecordType > 0
    query.prepare(QString::fromStdString("DELETE FROM RuleRecords WHERE ruleRecordType>0"));
    assertExec(query);
    query.clear();

    save();
  	test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // remove ClauseRecord entries with (clauseRecordType == 0 AND filterClauseRecordType == 2) OR
    // (clauseRecordType == 1 AND actionClauseRecordType == 2)
    query.prepare(QString::fromStdString(std::string("DELETE FROM ClauseRecords WHERE ") + 
        std::string("(clauseRecordType=0 AND filterClauseRecordType=2) OR ") + 
        std::string("(clauseRecordType=1 AND actionClauseRecordType=2)")));
    assertExec(query);
    query.clear();

    save();
  	test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // remove orphaned join records
    query.prepare(QString::fromStdString(std::string("DELETE FROM Ruleset_Rule_JoinRecords WHERE ") + 
      std::string("NOT EXISTS (SELECT * FROM RulesetRecords r WHERE r.id=leftId)")));
    assertExec(query);
    query.clear();

    query.prepare(QString::fromStdString(std::string("DELETE FROM Rule_Clause_JoinRecords WHERE ") +
      std::string("NOT EXISTS (SELECT * FROM RuleRecords r WHERE r.id=leftId)")));
    assertExec(query);
    query.clear();

    save();
  	test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_1_0_2_to_1_0_3(const VersionString& startVersion) {
    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    // This is a big change to the database. All PAT-created OSPs should be preserved just
    // fine, but any OSPs with rulesets ARE NOT GUARANTEED TO BEHAVE WELL AFTER THIS UPGRADE.
    // Model ruleset perturbations and continuous variables are unceremoniously dropped; we
    // do not attempt to fix various vector indices and DataPoints back up after this is done.
    // We do issue warnings when we make such changes.

    bool usedRulesets(false);

    LOG(Info,"Dropping deprecated tables ClauseRecords, RuleRecords, RulesetRecords, "
        << "Rule_Clause_JoinRecords, Ruleset_Rule_JoinRecords.");

    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    query.prepare(QString("DROP TABLE ClauseRecords"));
    assertExec(query);
    query.clear();

    query.prepare(QString("DROP TABLE RuleRecords"));
    assertExec(query);
    query.clear();

    query.prepare(QString("DROP TABLE RulesetRecords"));
    assertExec(query);
    query.clear();

    query.prepare(QString("DROP TABLE Rule_Clause_JoinRecords"));
    assertExec(query);
    query.clear();

    query.prepare(QString("DROP TABLE Ruleset_Rule_JoinRecords"));
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);

    LOG(Info,"Adding column discreteVariableRecordType to table VariableRecords.");

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    VariableRecordColumns discreteVariableRecordTypeColumn("discreteVariableRecordType");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + VariableRecord::databaseTableName() + " ADD COLUMN " +
        discreteVariableRecordTypeColumn.valueName() + " " +
        discreteVariableRecordTypeColumn.valueDescription()));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    LOG(Info,"Erase old model ruleset continuous variables from " << VariableRecord::databaseTableName()
        << " table and update ContinuousVariableRecordType entries accordingly.");

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // First determine if anything is to be removed.
    query.prepare(QString::fromStdString("SELECT COUNT(id) FROM " + VariableRecord::databaseTableName() +
                                         " WHERE variableRecordType=:variableRecordType AND " +
                                         "inputVariableRecordType=:inputVariableRecordType AND " +
                                         "continuousVariableRecordType=:continuousVariableRecordType"));
    query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
    query.bindValue(":inputVariableRecordType",InputVariableRecordType::ContinuousVariableRecord);
    query.bindValue(":continuousVariableRecordType",QVariant(int(0))); // was 0
    assertExec(query);
    int count = query.value(0).toInt();
    if (count > 0) {
      usedRulesets = true;
      LOG(Info,"Removing " << count << " ModelRulesetContinuousVariables from this OSP. "
          << "ModelRulesetContinuousVariables are no longer supported by OpenStudio.");
      query.clear();

      // Then do the removal.
      query.prepare(QString::fromStdString("DELETE FROM " + VariableRecord::databaseTableName() +
                                           " WHERE variableRecordType=:variableRecordType AND " +
                                           "inputVariableRecordType=:inputVariableRecordType AND " +
                                           "continuousVariableRecordType=:continuousVariableRecordType"));
      query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
      query.bindValue(":inputVariableRecordType",InputVariableRecordType::ContinuousVariableRecord);
      query.bindValue(":continuousVariableRecordType",QVariant(int(0))); // was 0
      assertExec(query);
    }
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // Ruby continuous variables were ContinuousVariableRecordType 1, now they are 0.
    query.prepare(QString::fromStdString("UPDATE " + VariableRecord::databaseTableName() +
                                         " SET continuousVariableRecordType=:newContinuousVariableRecordType " +
                                         "WHERE variableRecordType=:variableRecordType AND " +
                                         "inputVariableRecordType=:inputVariableRecordType AND " +
                                         "continuousVariableRecordType=:oldContinuousVariableRecordType"));
    query.bindValue(":newContinuousVariableRecordType",ContinuousVariableRecordType::RubyContinuousVariableRecord);
    query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
    query.bindValue(":inputVariableRecordType",InputVariableRecordType::ContinuousVariableRecord);
    query.bindValue(":oldContinuousVariableRecordType",QVariant(int(1))); // was 1
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    LOG(Info,"Processing current DiscreteVariableRecords to turn them into MeasureGroupRecords.");

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // get VariableRecords with inputVariableRecordType == 0 (DiscreteVariableRecord) and
    // set their discreteVariableRecordType to 0 (MeasureGroupRecord)
    query.prepare(QString::fromStdString("UPDATE " + VariableRecord::databaseTableName() +
        " SET discreteVariableRecordType=:discreteVariableRecordType " +
        "WHERE inputVariableRecordType=:inputVariableRecordType"));
    query.bindValue(":discreteVariableRecordType",DiscreteVariableRecordType::MeasureGroupRecord);
    query.bindValue(":inputVariableRecordType",InputVariableRecordType::DiscreteVariableRecord);
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    LOG(Info,"Creating " << MeasureRecord::databaseTableName() << " table and populating it with "
        << "data from DiscretePerturbationRecords table, which will then be dropped.");

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    createTable<MeasureRecord>();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // extract previous data
    query.prepare(QString("SELECT * FROM DiscretePerturbationRecords"));
    assertExec(query);
    QSqlQuery measureAddQuery(*(database.qSqlDatabase()));
    measureAddQuery.prepare(QString::fromStdString(
        "INSERT INTO " + MeasureRecord::databaseTableName() +
        " (id, handle, name, displayName, description, timestampCreate, timestampLast, " +
        "uuidLast, measureRecordType, variableRecordId, measureVectorIndex, isSelected, " +
        "inputFileType, outputFileType, fileReferenceRecordId, isUserScript, usesBCLMeasure) " +
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

    // loop through and move data to new format
    QVariantList ids, handles, names, displayNames, descriptions, timestampCreates;
    QVariantList timestampLasts, uuidLasts, measureRecordTypes, variableRecordIds;
    QVariantList measureVectorIndices, isSelecteds, inputFileTypes, outputFileTypes;
    QVariantList fileReferenceRecordIds, isUserScripts, usesBCLMeasures;
    // note ids that are being dropped because they were associated with ModelRulesetPerturbations
    std::set<int> modelRulesetPerturbationIds;
    while (query.next()) {
      QVariant value = query.value(0);
      OS_ASSERT(value.isValid() && !value.isNull());

      int id = value.toInt();

      // drop model ruleset perturbations
      int recordType = query.value(8).toInt();
      if (recordType == 2) {
        usedRulesets = true;
        LOG(Info,"Dropping ModelRulesetPerturbation '" << query.value(2).toString().toStdString()
            << "' associated with VariableRecord " << query.value(9).toInt() << ".");
        modelRulesetPerturbationIds.insert(id);
        continue;
      }
      OS_ASSERT(recordType == 0 || recordType == 1);

      ids << value;

      handles << query.value(1);
      names << query.value(2);
      displayNames << query.value(3);
      descriptions << query.value(4);
      timestampCreates << query.value(5);
      timestampLasts << query.value(6);
      uuidLasts << query.value(7);

      measureRecordTypes << query.value(8);
      variableRecordIds << query.value(9);
      measureVectorIndices << query.value(11);
      isSelecteds << query.value(10);
      inputFileTypes << query.value(13);
      outputFileTypes << query.value(14);
      fileReferenceRecordIds << query.value(12);
      isUserScripts << query.value(15);
      usesBCLMeasures << query.value(17);
    }
    measureAddQuery.addBindValue(ids);
    measureAddQuery.addBindValue(handles);
    measureAddQuery.addBindValue(names);
    measureAddQuery.addBindValue(displayNames);
    measureAddQuery.addBindValue(descriptions);
    measureAddQuery.addBindValue(timestampCreates);
    measureAddQuery.addBindValue(timestampLasts);
    measureAddQuery.addBindValue(uuidLasts);

    measureAddQuery.addBindValue(measureRecordTypes);
    measureAddQuery.addBindValue(variableRecordIds);
    measureAddQuery.addBindValue(measureVectorIndices);
    measureAddQuery.addBindValue(isSelecteds);
    measureAddQuery.addBindValue(inputFileTypes);
    measureAddQuery.addBindValue(outputFileTypes);
    measureAddQuery.addBindValue(fileReferenceRecordIds);
    measureAddQuery.addBindValue(isUserScripts);
    measureAddQuery.addBindValue(usesBCLMeasures);

    test = measureAddQuery.execBatch();
    OS_ASSERT(test);
    measureAddQuery.clear();
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    query.prepare(QString("DROP TABLE DiscretePerturbationRecords"));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    LOG(Info,"Renaming DataPoint_DiscretePerturbation_JoinRecord to DataPoint_Measure_JoinRecord "
        << "and dropping entries corresponding to old ModelRulesetPerturbations.");

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    createTable<DataPoint_Measure_JoinRecord>();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // extract previous data
    query.prepare(QString("SELECT * FROM DataPoint_DiscretePerturbation_JoinRecords"));
    assertExec(query);
    QSqlQuery joinRecordAddQuery(*(database.qSqlDatabase()));
    joinRecordAddQuery.prepare(QString::fromStdString(
        "INSERT INTO " + DataPoint_Measure_JoinRecord::databaseTableName() +
        " (id, handle, leftId, leftHandle, rightId, rightHandle) " +
        "VALUES (?, ?, ?, ?, ?, ?)"));

    // loop through and move data to new format
    ids.clear();
    handles.clear();
    QVariantList leftIds, leftHandles, rightIds, rightHandles;
    // drop records with rightId in modelRulesetPerturbationIds
    while (query.next()) {
      int rightId = query.value(4).toInt();
      if (std::find(modelRulesetPerturbationIds.begin(),
                    modelRulesetPerturbationIds.end(),
                    rightId) != modelRulesetPerturbationIds.end())
      {
        // don't keep record
        continue;
      }

      ids << query.value(0);
      handles << query.value(1);
      leftIds << query.value(2);
      leftHandles << query.value(3);
      rightIds << query.value(4);
      rightHandles << query.value(5);
    }
    joinRecordAddQuery.addBindValue(ids);
    joinRecordAddQuery.addBindValue(handles);
    joinRecordAddQuery.addBindValue(leftIds);
    joinRecordAddQuery.addBindValue(leftHandles);
    joinRecordAddQuery.addBindValue(rightIds);
    joinRecordAddQuery.addBindValue(rightHandles);

    test = joinRecordAddQuery.execBatch();
    QSqlError error = joinRecordAddQuery.lastError();
    LOG(Debug,"Last QSql Query: " << joinRecordAddQuery.lastQuery().toStdString());
    LOG(Debug,"Last QSql Error: " << error.text().toStdString());
    OS_ASSERT(test);
    joinRecordAddQuery.clear();
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    query.prepare(QString("DROP TABLE DataPoint_DiscretePerturbation_JoinRecords"));
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);

    if (usedRulesets) {
      LOG(Warn,"This OSP contained ruleset::ModelRuleset information that was being used by a "
          << "ModelRulesetPerturbation or a ModelRulesetContinuousVariable. Those classes are "
          << "no longer available in or supported by OpenStudio. Since those classes were not "
          << "being widely used, the upgrade code did not attempt to fix up indices and other "
          << "values that are now corrupt because this data was dropped. If you would like to "
          << "keep using this file, you may need to hand-edit some data (using a basic SQLite "
          << "interface). In particular, take a look at MeasureRecords::measureVectorIndex, "
          << "VariableRecords::variableVectorIndex, and WorkflowRecords::workflowIndex. Any "
          << "DataPoint simulation results will also be invalid/out of date.");
    }

  }

  void ProjectDatabase_Impl::update_1_0_3_to_1_0_4(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Adding column for selected to " << DataPointRecord::databaseTableName() << ".");

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    DataPointRecordColumns selectedColumn("selected");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + DataPointRecord::databaseTableName() + " ADD COLUMN " +
        selectedColumn.valueName() + " " + selectedColumn.valueDescription()));
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // By default, set all DataPoints to selected.
    query.prepare(QString::fromStdString("UPDATE " + DataPointRecord::databaseTableName() +
        " SET selected=:selected, xmlOutputDataRecordId=NULL"));
    query.bindValue(":selected",true);
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
  }

  void ProjectDatabase_Impl::update_1_0_4_to_1_0_5(const VersionString& startVersion) {
    bool didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    LOG(Info,"Adding column for run type to " << DataPointRecord::databaseTableName() << ".");

    ProjectDatabase database(this->shared_from_this());
    QSqlQuery query(*(database.qSqlDatabase()));

    DataPointRecordColumns runTypeColumn("runType");
    query.prepare(QString::fromStdString(
        "ALTER TABLE " + DataPointRecord::databaseTableName() + " ADD COLUMN " +
        runTypeColumn.valueName() + " " + runTypeColumn.valueDescription()));
    assertExec(query);
    query.clear();

    save();
    bool test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    // By default, set all DataPoints to DataPointRunType::Local.
    query.prepare(QString::fromStdString("UPDATE " + DataPointRecord::databaseTableName() +
        " SET runType=:runType"));
    query.bindValue(":runType",DataPointRunType(DataPointRunType::Local).value());
    assertExec(query);
    query.clear();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);
    didStartTransaction = startTransaction();
    OS_ASSERT(didStartTransaction);

    createTable<CloudSessionRecord>();
    createTable<CloudSettingsRecord>();
    createTable<UrlRecord>();

    save();
    test = this->commitTransaction();
    OS_ASSERT(test);    
  }

  void ProjectDatabase_Impl::setProjectDatabaseRecord(const ProjectDatabaseRecord& projectDatabaseRecord)
  {
    m_projectDatabaseRecord = projectDatabaseRecord;
  }

} // detail

RemoveUndo::RemoveUndo(UUID handle, RemoveSource removeSource)
{
  m_removedObjects.push_back(std::make_pair<UUID, RemoveSource>(handle, removeSource));
}

void RemoveUndo::concat(const RemoveUndo& other)
{
  std::vector<std::pair<UUID, RemoveSource> > otherRemovedObjects = other.removedObjects();
  m_removedObjects.insert(m_removedObjects.end(), otherRemovedObjects.begin(), otherRemovedObjects.end());
}

UUID RemoveUndo::primaryHandle() const
{
  return m_removedObjects[0].first;
}

std::vector<std::pair<UUID, RemoveUndo::RemoveSource> > RemoveUndo::removedObjects() const
{
  return m_removedObjects;
}

ProjectDatabase::ProjectDatabase(const openstudio::path& path,
                                 const openstudio::runmanager::RunManager& runManager,
                                 bool forceNew)
  : m_impl(boost::shared_ptr<detail::ProjectDatabase_Impl>(
               new detail::ProjectDatabase_Impl(completeAndNormalize(path),
                                                runManager,
                                                forceNew)))
{
  initialize(path);
}

ProjectDatabase::ProjectDatabase(const openstudio::path& path,
                                 bool forceNew,
                                 bool pauseRunManager,
                                 bool initializeRunManagerUI)
  : m_impl(boost::shared_ptr<detail::ProjectDatabase_Impl>(
               new detail::ProjectDatabase_Impl(
                   completeAndNormalize(path),
                   runmanager::RunManager(setFileExtension(path,"db",true,false),
                                          forceNew,
                                          pauseRunManager,
                                          initializeRunManagerUI),
                   forceNew)))
{
  initialize(path);
}

void ProjectDatabase::initialize(const openstudio::path& path) {
  OS_ASSERT(m_impl);

  ProjectDatabase other(m_impl);

  // have to call save in wrapper because this is the first place we have impl
  QSqlQuery query(*(m_impl->qSqlDatabase()));
  query.prepare(QString::fromStdString("SELECT * FROM ProjectDatabaseRecords"));

  boost::optional<ProjectDatabaseRecord> projectDatabaseRecord;
  assertExec(query);
  if (query.first()) {
    boost::shared_ptr<detail::ProjectDatabaseRecord_Impl> impl(new detail::ProjectDatabaseRecord_Impl(query, other));
    projectDatabaseRecord = ProjectDatabaseRecord(impl);
  }
  query.clear();

  runmanager::RunManager runManager = this->runManager();

  if (projectDatabaseRecord) {
    openstudio::path originalPath = toPath(projectDatabaseRecord->name());
    openstudio::path normalizedPath = completeAndNormalize(path);
    // update to this version
    m_impl->setProjectDatabaseRecord(*projectDatabaseRecord);
    m_impl->addCleanRecord(*projectDatabaseRecord);
    updateDatabase(projectDatabaseRecord->version());
    if (originalPath != normalizedPath) {
      LOG(Trace,"Database moved from " << toString(originalPath) << " to "
          << toString(normalizedPath) << ".");
      m_impl->setBasePaths(originalPath.parent_path(),normalizedPath.parent_path());
      updatePathData(originalPath.parent_path(),normalizedPath.parent_path());
    }
    if (runManager.dbPath() != projectDatabaseRecord->runManagerDBPath()) {
      LOG_AND_THROW("Unable to open database at " << toString(normalizedPath)
                    << ", because the given RunManager database path, "
                    << toString(runManager.dbPath())
                    << " does not match the stored (possibly updated) value "
                    << toString(projectDatabaseRecord->runManagerDBPath()) << ".");
    }
  }
  else {
    boost::shared_ptr<detail::ProjectDatabaseRecord_Impl> impl(new detail::ProjectDatabaseRecord_Impl(openStudioVersion(), runManager.dbPath(), other));
    projectDatabaseRecord = ProjectDatabaseRecord(impl);
    m_impl->setProjectDatabaseRecord(*projectDatabaseRecord);
    m_impl->addCleanRecord(*projectDatabaseRecord);
    projectDatabaseRecord->insertRow(other);
    projectDatabaseRecord->saveRow(other);
  }

  // have to put database into transaction mode vs auto-commit
  save();
}

ProjectDatabase::ProjectDatabase(const ProjectDatabase& other)
  : m_impl(other.m_impl)
{
  OS_ASSERT(m_impl);
  OS_ASSERT(m_impl->qSqlDatabase());
  OS_ASSERT(m_impl->qSqlDatabase()->isOpen());
}

ProjectDatabase::ProjectDatabase(boost::shared_ptr<detail::ProjectDatabase_Impl> impl)
  : m_impl(impl)
{
  OS_ASSERT(m_impl);
  OS_ASSERT(m_impl->qSqlDatabase());
  OS_ASSERT(m_impl->qSqlDatabase()->isOpen());
}

boost::optional<ProjectDatabase> ProjectDatabase::open(const openstudio::path& path,
                                                       bool pauseRunManager,
                                                       bool initializeRunManagerUI)
{
  boost::optional<ProjectDatabase> result;
  boost::optional<openstudio::runmanager::RunManager> runManager;
  openstudio::path wpath = completeAndNormalize(path);

  if(boost::filesystem::exists(wpath)){

    // scope resolution to ensure database and query are destroyed before removeDatabase
    {
      // try to read database to find runmanager path
      QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", toQString(wpath));
      if (database.isValid()){
        database.setDatabaseName(toQString(wpath));
        if (database.open()){
          QSqlQuery query(database);
          query.prepare(QString::fromStdString("SELECT * FROM ProjectDatabaseRecords"));
          assertExec(query);
          if(query.first()){
            openstudio::path originalPath = toPath(query.value(ProjectDatabaseRecordColumns::name).toString());
            openstudio::path runManagerDBPath = toPath(query.value(ProjectDatabaseRecordColumns::runManagerDBPath).toString());
            if (originalPath != wpath) {
              LOG(Trace,"Database moved from " << toString(originalPath) << " to " <<
                  toString(wpath) << ". Will look for corresponding RunManager database in "
                  << "the new location.");
              openstudio::path temp = relocatePath(runManagerDBPath,originalPath.parent_path(),wpath.parent_path());
              if (!temp.empty()) {
                runManagerDBPath = temp;
                LOG(Trace,"originalPath.stem() " << toString(originalPath.stem())
                    << ", wpath.stem() " << toString(wpath.stem())
                    << ", runManagerDBPath.stem() " << toString(runManagerDBPath.stem()));
                if ((originalPath.stem() != wpath.stem()) &&
                    (runManagerDBPath.stem() == originalPath.stem()))
                {
                  // filename likely also changed
                  std::string ext = getFileExtension(runManagerDBPath);
                  runManagerDBPath = runManagerDBPath.parent_path() / wpath.stem();
                  runManagerDBPath = setFileExtension(runManagerDBPath,ext,true,false);
                }
              }
            }
            try {
              LOG(Trace,"Trying to load RunManager from " << toString(runManagerDBPath) << ".");
              runManager = openstudio::runmanager::RunManager(runManagerDBPath,false,pauseRunManager,initializeRunManagerUI);
            }
            catch (std::exception& e) {
              LOG(Error,"Unable to open RunManager from database path " << toString(runManagerDBPath)
                  << ", because " << e.what());
            }
          }
        }
      }
      database.close();
    }

    QSqlDatabase::removeDatabase(toQString(wpath));

    if (runManager) {
      try {
        result = ProjectDatabase(wpath, *runManager, false);
      }
      catch (std::exception& e) {
        LOG(Error,"Unable to open ProjectDatabase at " << toString(wpath) << ", because "
            << e.what());
      }
    }
  }

  return result;

}

std::vector<WorkflowRecord> ProjectDatabase::workflowRecords()
{
  return m_impl->workflowRecords();
}

std::vector<VariableRecord> ProjectDatabase::variableRecords()
{
  return m_impl->variableRecords();
}

ProjectDatabaseRecord ProjectDatabase::projectDatabaseRecord() const {
  return m_impl->projectDatabaseRecord();
}

bool ProjectDatabase::isDirty() const
{
  return m_impl->isDirty();
}

UUID ProjectDatabase::handle() const
{
  return m_impl->handle();
}

std::string ProjectDatabase::name() const
{
  return m_impl->name();
}

bool ProjectDatabase::setName(const std::string& name)
{
  return m_impl->setName(name);
}

std::string ProjectDatabase::displayName() const
{
  return m_impl->displayName();
}

bool ProjectDatabase::setDisplayName(const std::string& displayName)
{
  return m_impl->setDisplayName(displayName);
}

std::string ProjectDatabase::description() const
{
  return m_impl->description();
}

bool ProjectDatabase::setDescription(const std::string& description)
{
  return m_impl->setDescription(description);
}

DateTime ProjectDatabase::timestampCreate() const
{
  return m_impl->timestampCreate();
}

DateTime ProjectDatabase::timestampLast() const
{
  return m_impl->timestampLast();
}

UUID ProjectDatabase::uuidLast() const
{
  return m_impl->uuidLast();
}

std::string ProjectDatabase::version() const
{
  return m_impl->version();
}

openstudio::path ProjectDatabase::path() const
{
  return m_impl->path();
}

openstudio::path ProjectDatabase::originalBasePath() const
{
  return m_impl->originalBasePath();
}

openstudio::path ProjectDatabase::newBasePath() const
{
  return m_impl->newBasePath();
}

openstudio::path ProjectDatabase::runManagerDBPath() const
{
  return m_impl->runManagerDBPath();
}

openstudio::runmanager::RunManager ProjectDatabase::runManager() const
{
  return m_impl->runManager();
}

bool ProjectDatabase::startTransaction() const
{
  return m_impl->startTransaction();
}

bool ProjectDatabase::commitTransaction() const
{
  return m_impl->commitTransaction();
}

bool ProjectDatabase::save()
{
  return m_impl->save();
}

bool ProjectDatabase::saveRecord(Record& record)
{
  return m_impl->saveRecord(record, true);
}

boost::optional<RemoveUndo> ProjectDatabase::removeRecord(Record& record)
{
  return m_impl->removeRecord(record, true);
}

bool ProjectDatabase::includesRecord(const Record& record) const
{
  return m_impl->includesRecord(record);
}

bool ProjectDatabase::isNewRecord(const Record& record) const
{
  return m_impl->isNewRecord(record);
}

bool ProjectDatabase::isCleanRecord(const Record& record) const
{
  return m_impl->isCleanRecord(record);
}

bool ProjectDatabase::isDirtyRecord(const Record& record) const
{
  return m_impl->isDirtyRecord(record);
}

bool ProjectDatabase::isRemovedRecord(const Record& record) const
{
  return m_impl->isRemovedRecord(record);
}

boost::shared_ptr<QSqlDatabase> ProjectDatabase::qSqlDatabase() const
{
  return m_impl->qSqlDatabase();
}

boost::optional<Record> ProjectDatabase::findLoadedRecord(const UUID& handle) const
{
  return m_impl->findLoadedRecord(handle);
}

void ProjectDatabase::unloadUnusedCleanRecords()
{
  return m_impl->unloadUnusedCleanRecords();
}

void ProjectDatabase::commitRemove(const RemoveUndo& removeUndo)
{
  return m_impl->commitRemove(removeUndo);
}

void ProjectDatabase::updatePathData(const openstudio::path& originalBase, const openstudio::path& newBase) {
  m_impl->updatePathData(originalBase,newBase);
}

void ProjectDatabase::addNewRecord(Record& record)
{
  m_impl->addNewRecord(record);
}

void ProjectDatabase::addDirtyRecord(const Record& record)
{
  m_impl->addDirtyRecord(record);
}

void ProjectDatabase::addCleanRecord(const Record& record)
{
  m_impl->addCleanRecord(record);
}

void ProjectDatabase::updateDatabase(const std::string& dbVersion) {
  m_impl->updateDatabase(dbVersion);
}

} // project
} // openstudio
