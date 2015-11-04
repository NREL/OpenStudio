namespace openstudio {
namespace project {

/** \mainpage OpenStudio Project
 *
 *  \section quick_links_project Quick Links
 *
 *  \li ProjectDatabase
 *  \li Record
 *  \li ObjectRecord
 *  \li JoinRecord
 *
 *  \section introduction_project Introduction
 *
 *  The OpenStudio ProjectDatabase stores energy simulation project data for projects with less than
 *  10,000 models. In addition, the ProjectDatabase is a basic serialization format for a number of
 *  OpenStudio classes, and allows for fast query of high-level information about files and data
 *  points stored as attributes and tags. The ProjectDatabase is saved to the OSP (OpenStudio
 *  Project) file format, which is a SQLite database. The ProjectDatabase stores all of its data in
 *  objects that derive from class Record.  ProjectDatabase and all Records are implemented
 *  using the pointer to implementation (pImpl) idiom, so normal copy semantics result in wrapper
 *  objects sharing pointers to the same implementation object.
 *
 *  \subsection record_subsec Record
 *
 *  Each Record object represents a single row in a ProjectDatabase table. The two important classes
 *  deriving directly from Record are ObjectRecord, which stores information about a single object,
 *  and JoinRecord, which records a relationship between two \link ObjectRecord ObjectRecords
 *  \endlink. The ProjectDatabase uses single table inheritance with a single table for each
 *  specialization directly below ObjectRecord or JoinRecord.  For example, DiscreteVariableRecord
 *  and ContinuousVariableRecord both derive from VariableRecord, which derives from ObjectRecord.
 *  So \link DiscreteVariableRecord DiscreteVariableRecords \endlink and \link
 *  ContinuousVariableRecord ContinuousVariableRecords \endlink (which is further specialized into
 *  ModelRulesetContinuousVariableRecord and OutputAttributeContinuousVariableRecord) are both
 *  stored in the table VariableRecords.  The columns of the VariableRecords table are defined
 *  using the OPENSTUDIO_ENUM macro as shown below:
 *
 * \code
 * OPENSTUDIO_ENUM( VariableRecordColumns,
 *   ((id)(INTEGER PRIMARY KEY)(0))
 *   ((handle)(TEXT)(1))
 *   ((name)(TEXT)(2))
 *   ((displayName)(TEXT)(3))
 *   ((description)(TEXT)(4))
 *   ((timestampCreate)(TEXT)(5))
 *   ((timestampLast)(TEXT)(6))
 *   ((uuidLast)(TEXT)(7))
 *   ((problemRecordId)(INTEGER)(8))
 *   ((functionRecordId)(INTEGER)(9))
 *   ((variableVectorIndex)(INTEGER)(10))
 *   ((functionCoefficient)(REAL)(11))
 *   ((variableRecordType)(INTEGER)(12))
 *   ((continuousVariableRecordType)(INTEGER)(13))
 *   ((minimum)(REAL)(14))
 *   ((maximum)(REAL)(15))
 *   ((increment)(REAL)(16))
 *   ((nSteps)(INTEGER)(17))
 *   ((attributeName)(TEXT)(18))
 *   ((rulesetRecordId)(INTEGER)(19))
 * );
 * \endcode
 *
 * Tables for all \link Record Records \endlink begin with the columns id and handle.  The id
 * column is an integer primary key which is used to uniquely identify objects within a single
 * database. The handle column records an openstudio::UUID which can be used to uniquely identify
 * objects across databases.  Tables for all \link ObjectRecord ObjectRecords \endlink additionally
 * include the columns name, displayName, description, timestampCreate (time that the  was
 * created), timestampLast (time the object was last updated), and uuidLast (openstudio::UUID which
 * uniquely identifies the last update to the object).  After these columns, each table includes
 * columns to store data for all derived types.  Additional columns are required to store the type
 * of each derived class, for example the column variableRecordType in the example above records
 * whether the given row is a discrete or a continuous variable, and the column
 * continuousVariableRecordType further designates whether a ContinuousVariableRecord is a
 * ModelRulesetContinousVariableRecord or an OutputAttributeContinuousVariableRecord. If two
 * derived class share the same data member, then those objects should save space by reusing the
 * same column to store that data member. For instance, ModelRulesetContinousVariableRecord and
 * OutputAttributeContinuousVariableRecord both use the field attributeName.
 *
 * \todo Open Question: We need to standardize on whether to store OPENSTUDIO_ENUM attributes as
 * strings for portability or as integers for better performance.
 *
 * The following virtual methods are reimplemented for each class that derives from Record :
 * <ol>
 *   <li> std::string databaseTableName() - name of the database table implemented at the first
 *        class after ObjectRecord or JoinRecord</li>
 *   <li> std::vector<ObjectRecord> children() - objects directly owned by this record that are to
 *        be removed when this record is removed</li>
 *   <li> std::vector<ObjectRecord> resources() - objects referenced, but not owned, by this
 *        record</li>
 *   <li> std::vector<JoinRecord> joinRecords() - relationships between this object and another
 *        one, to be removed when either record is removed</li>
 *   <li> void saveRow(ProjectDatabase& projectDatabase) - save just this object (does not save
 *        children or resources) in the database</li>
 *   <li> void bindValues(QSqlQuery& query) - bind values to a query for saving</li>
 *   <li> void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) - set the
 *        last state of this object from the query (including id and handle)</li>
 *   <li> bool compareValues(const QSqlQuery& query) const - check that values (including handle
 *        but excluding id) are same as query</li>
 *   <li> void saveLastValues() - save current values to last state</li>
 *   <li> void revertToLastValues() - revert values back to last state</li>
 * </ol>
 *
 * \subsubsection objectrecord_subsubsec ObjectRecord
 *
 * Two patterns have been established for serializing objects to \link ObjectRecord ObjectRecords
 * \endlink in the ProjectDatabase. The first pattern applies when the object is meant to be
 * serialized entirely to the database.  In this case, the object is paired with an ObjectRecord
 * which serializes and deserializes the object fully from the ProjectDatabase. The object stores
 * an openstudio::UUID which is used to determine if the database already contains a record for
 * the object before a new record is created, this pattern is shown below. Objects serialized to
 * the database have parent/child and resource relationships similar to the OpenStudio
 * openstudio::model::ModelObjects. Adding an object to the database will add the object and
 * ensure that all of its resources exist.  Removing an object from the database will remove all
 * children but will not remove resources.  A RemoveUndo object is returned by
 * ProjectDatabase::removeRecord, this object records all of the \link Record Records \endlink
 * removed and can be used to commit or revert the remove operation.
 *
 * \image html ObjectSerialization.jpg
 *
 * The second pattern applies to larger objects which are serialized to their own file format (e.g.
 * openstudio::model::Model is saved to the OSM format). For these objects, the database does not
 * need to fully serialize and deserialize the object, it only needs to maintain a reference to the
 * serialized file on disk as well as information about the timestamp of the file and a checksum
 * which can verify that the file has not changed since the Record was last updated. \link
 * FileReferenceRecord FileReferenceRecords \endlink, which serialize openstudio::FileReference
 * (see utilities/core/FileReference.hpp), are used for this purpose. In most circumstances, a
 * FileReferenceRecord will be a child of another record that knows/specifies that file's type and
 * purpose. openstudio::model::Components are a notable exception, as FileReferenceRecord includes
 * a specialized constructor and getter for this file/object type.
 *
 * \image html OpenStudioModelSerialization.jpg
 *
 * \subsection projectdatabase_subsec ProjectDatabase
 *
 * To ensure data consistency, ProjectDatabase objects must ensure that duplicate \link Record
 * Records\endlink, that is, multiple Record objects that represent the same row in the database,
 * cannot exist. The ProjectDatabase class does this by maintaining maps of object handles to new,
 * clean, dirty, and removed \link Record Records \endlink. When adding, retrieving, or removing
 * \link Record Records \endlink, the ProjectDatabase first checks if the Record already exists in
 * one of these maps.  If the Record is already in a map, then a new wrapper pointing to the
 * existing implementation object is returned. If the Record is in a map, then a new
 * implementation object is constructed and placed in the appropriate map. The problem with this
 * solution is that SQL queries cannot be executed on objects loaded in these maps. Therefore, each
 * operation that changes a Record is immediately saved in the database (including inserts and
 * removes). The last saved state of the Record is stored as member variables in the Record to
 * support reverting later, if the ProjectDatabase is not saved before its destructor is called. A
 * RemoveUndo struct is used to undo removals of \link Record Records\endlink. This
 * synchronization scheme is shown below:
 *
 * \image html ObjectRecordSynchronization.jpg
 *
 * A side effect of this synchronization strategy is that operations which add, remove, or modify
 * \link Record Records \endlink can take a long time, as they each require database writes. To
 * avoid this, it is best to group add, remove, or modify statements within a transaction.
 * Transactions are started using ProjectDatabase::startTransaction and saved using
 * ProjectDatabase::commitTransaction. It is good practice to ensure that a transaction was
 * actually started before trying to commit it:
 *
 * \code
 *
 * bool didStartTransaction = database.startTransaction();
 *
 * // create, remove, modify records
 *
 * // database.save starts a new transaction if one is not already active
 * // in this case it does not start a new transaction
 * database.save();
 *
 * if (didStartTransaction){
 *   bool didCommitTransaction = database.commitTransaction();
 *   OS_ASSERT(didCommitTransaction);
 * }
 *
 * // in this case a new transaction is started and committed within database.save()
 * database.save();
 *
 * \endcode
 *
 * A "gotcha" condition in this scheme is that queries no longer work on removed \link Record Records \endlink.
 * Consider the following example:
 *
 * \code
 *
 * void removeRecordAndSave(ProjectDatabase database, Record record)
 * {
 *
 *   // successfully returns children found in database by using SQL query
 *   record.children();
 *
 *   // remove only this record and it's children
 *   boost::optional<RemoveUndo> removeUndo = database.removeRecord(record);
 *
 *   // no longer returns children because they were removed from database and not found by SQL query
 *   record.children();
 *
 *   // saves removal of the record only, but does not save removal of children because call to children() fails
 *   // database.saveRecord(record); // don't do this
 *
 *   // check if objects were removed by removeRecord()
 *   if (removeUndo){
 *     // ask database to remove all objects removed by removeRecord()
 *     database.commitRemove(*removeUndo); // do this
 *   }
 *
 * }
 *
 * \endcode
 *
 */

}
}
