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

#ifndef PROJECT_DATAPOINTRECORD_IMPL_HPP
#define PROJECT_DATAPOINTRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "DataPointRecord.hpp"

#include "../analysis/DataPoint.hpp"

namespace openstudio {
namespace project {

class AnalysisRecord;
class FileReferenceRecord;
class DataPointValueRecord;
class MeasureRecord;

namespace detail {

  /** DataPointRecord_Impl is a ObjectRecord_Impl that is the implementation class for DataPointRecord.*/
  class PROJECT_API DataPointRecord_Impl : public ObjectRecord_Impl {
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
    virtual std::string databaseTableName() const override;

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const override;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const override;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const override;

    /** Returns join relationships between this object and others. Such relationships will be
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const override;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database) override;

    virtual void updatePathData(const openstudio::path& originalBase,
                                const openstudio::path& newBase) override;

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

    std::vector<QVariant> variableValues() const;

    /** Returns the measures associated with this DataPoint via MeasureGroup selection. Returns them
     *  in ProblemRecord::inputVariableRecords order. */
    std::vector<MeasureRecord> measureRecords() const;

    /** Returns the continuous variable values associated with this DataPoint. Returns them in 
     *  ProblemRecord::inputVariableRecords order. */
    std::vector<DataPointValueRecord> continuousVariableValueRecords() const;

    std::vector<DataPointValueRecord> responseValueRecords() const;

    std::vector<double> responseValues() const;

    boost::optional<FileReferenceRecord> osmInputDataRecord() const;

    boost::optional<FileReferenceRecord> idfInputDataRecord() const;

    boost::optional<FileReferenceRecord> sqlOutputDataRecord() const;

    /** \deprecated Will always return empty vector; call attributeRecords() instead. */
    std::vector<FileReferenceRecord> xmlOutputDataRecords() const;

    boost::optional<openstudio::UUID> topLevelJobUUID() const;

    std::vector<TagRecord> tagRecords() const;

    /** Returns this DataPointRecord's attributeRecords. */
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
    virtual void bindValues(QSqlQuery& query) const override;

    /** Set the last state of this object from the query (including id). */
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) override;

    /** Check that values (except id) are same as query. */
    virtual bool compareValues(const QSqlQuery& query) const override;

    /** Save values to last state. */
    virtual void saveLastValues() override;

    /** Revert values back to last state. */
    virtual void revertToLastValues() override;

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

