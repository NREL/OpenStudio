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

#ifndef PROJECT_DATAPOINTRECORD_IMPL_HPP
#define PROJECT_DATAPOINTRECORD_IMPL_HPP

#include <project/ProjectAPI.hpp>
#include <project/ObjectRecord_Impl.hpp>

#include <project/DataPointRecord.hpp>

#include <analysis/DataPoint.hpp>

namespace openstudio {
namespace project {

class AnalysisRecord;
class FileReferenceRecord;
class DataPointValueRecord;

namespace detail {

  /** DataPointRecord_Impl is a ObjectRecord_Impl that is the implementation class for DataPointRecord.*/
  class PROJECT_API DataPointRecord_Impl : public ObjectRecord_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    DataPointRecord_Impl(const analysis::DataPoint& dataPoint,
                         const DataPointRecordType& dataPointRecordType,
                         AnalysisRecord& analysisRecord,
                         const ProblemRecord& problemRecord);

    /** Constructor from query. Throws if bad query. */
    DataPointRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~DataPointRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the name of the database table for this record. Should be implemented by the base
     *  class for the table. */
    virtual std::string databaseTableName() const;

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Returns join relationships between this object and others. Such relationships will be
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    virtual void updatePathData(const openstudio::path& originalBase,
                                const openstudio::path& newBase);

    //@}
    /** @name Getters */
    //@{

    /** Returns the AnalysisRecord to which this DataPointRecord belongs (as a child). */
    AnalysisRecord analysisRecord() const;

    /** Returns the ProblemRecord associated with this DataPointRecord (as a resource). */
    ProblemRecord problemRecord() const;

    /** Returns true if this data point has been run/simulated. Returns true even if failed().
     *  \deprecated */
    bool isComplete() const;

    bool complete() const;

    /** Returns true if this data point has been run, but the simulation failed. Returns false
     *  otherwise. (Always returns false if !complete().) */
    bool failed() const;

    bool selected() const;

    analysis::DataPointRunType runType() const;

    openstudio::path directory() const;

    std::vector<DataPointValueRecord> continuousVariableValueRecords() const;

    std::vector<DataPointValueRecord> responseValueRecords() const;

    std::vector<double> responseValues() const;

    boost::optional<FileReferenceRecord> osmInputDataRecord() const;

    boost::optional<FileReferenceRecord> idfInputDataRecord() const;

    boost::optional<FileReferenceRecord> sqlOutputDataRecord() const;

    /** Returns the FileReferenceRecords that point to this DataPointRecord's XML output data. */
    std::vector<FileReferenceRecord> xmlOutputDataRecords() const;

    boost::optional<openstudio::UUID> topLevelJobUUID() const;

    std::vector<TagRecord> tagRecords() const;

    /** Assembles all the AttributeRecords associated with xmlOutputDataRecords(). */
    std::vector<AttributeRecord> attributeRecords() const;

    virtual analysis::DataPoint dataPoint() const;

    //}
    /** @name Setters */
    //@{

    void setDirectory(const openstudio::path& directory);

    /** Provided for callers operating directly on the database, not holding a copy of this
     *  analysis in memory. Use with caution. Does not do file system cleanup. */
    virtual void clearResults();

    /** Reverts record id values back to last state. For use in DataPointRecord constructor
     *  so can access old related records for comparison and possible removal. */
    void revertToLastRecordIds();

    /** Impl-only method. Used to make two-way connection with FileReferences. */
    void setOsmInputDataRecordId(int id);

    void clearOsmInputDataRecordId();

    /** Impl-only method. Used to make two-way connection with FileReferences. */
    void setIdfInputDataRecordId(int id);

    void clearIdfInputDataRecordId();

    /** Impl-only method. Used to make two-way connection with FileReferences. */
    void setSqlOutputDataRecordId(int id);

    void clearSqlOutputDataRecordId();

    //@}
   protected:
    /** Bind data member values to a query for saving. */
    virtual void bindValues(QSqlQuery& query) const;

    /** Set the last state of this object from the query (including id). */
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /** Check that values (except id) are same as query. */
    virtual bool compareValues(const QSqlQuery& query) const;

    /** Save values to last state. */
    virtual void saveLastValues();

    /** Revert values back to last state. */
    virtual void revertToLastValues();

   private:
    REGISTER_LOGGER("openstudio.project.DataPointRecord");

    int m_analysisRecordId;
    int m_problemRecordId;
    DataPointRecordType m_dataPointRecordType;
    bool m_complete;
    bool m_failed;
    bool m_selected;
    analysis::DataPointRunType m_runType;
    openstudio::path m_directory;
    boost::optional<int> m_osmInputDataRecordId;
    boost::optional<int> m_idfInputDataRecordId;
    boost::optional<int> m_sqlOutputDataRecordId;
    boost::optional<openstudio::UUID> m_topLevelJobUUID;
    std::vector<openstudio::path> m_dakotaParametersFiles;

    int m_lastAnalysisRecordId;
    int m_lastProblemRecordId;
    DataPointRecordType m_lastDataPointRecordType;
    bool m_lastComplete;
    bool m_lastFailed;
    bool m_lastSelected;
    analysis::DataPointRunType m_lastRunType;
    openstudio::path m_lastDirectory;
    boost::optional<int> m_lastOsmInputDataRecordId;
    boost::optional<int> m_lastIdfInputDataRecordId;
    boost::optional<int> m_lastSqlOutputDataRecordId;
    boost::optional<openstudio::UUID> m_lastTopLevelJobUUID;
    std::vector<openstudio::path> m_lastDakotaParametersFiles;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_DATAPOINTRECORD_IMPL_HPP

