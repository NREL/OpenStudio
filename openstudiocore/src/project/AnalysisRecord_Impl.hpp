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

#ifndef PROJECT_ANALYSISRECORD_IMPL_HPP
#define PROJECT_ANALYSISRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"
#include "AnalysisRecord.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/core/Logger.hpp"

#include <QVariant>

class QSqlQuery;

namespace openstudio {
namespace analysis {
  class Analysis;
}

namespace project {

class ProblemRecord;
class AlgorithmRecord;
class FileReferenceRecord;
class DataPointRecord;

namespace detail {

  /** AnalysisRecord_Impl is a ObjectRecord_Impl that is the implementation class for AnalysisRecord.*/
  class PROJECT_API AnalysisRecord_Impl : public ObjectRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AnalysisRecord_Impl(const analysis::Analysis& analysis, ProjectDatabase& database);

    /** Constructor from query. Throws if bad query. */
    AnalysisRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~AnalysisRecord_Impl() {}

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
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    //@}
    /** @name Getters */
    //@{

    /** Return the ProblemRecord (required resource) associated with this AnalysisRecord. */
    ProblemRecord problemRecord() const;

    /** Return the AlgorithmRecord (required resource) associated with this AnalysisRecord. */
    boost::optional<AlgorithmRecord> algorithmRecord() const;

    /** Return the seed FileReferenceRecord (required child) of this AnalysisRecord. */
    FileReferenceRecord seedFileReferenceRecord() const;

    /** Return the weather FileReferenceRecord (optional child) of this AnalysisRecord. */
    boost::optional<FileReferenceRecord> weatherFileReferenceRecord() const;

    /** Return the \link DataPointRecord DataPointRecords \endlink (children) of this
     *  AnalysisRecord. */
    std::vector<DataPointRecord> dataPointRecords() const;

    /** Return the DataPointRecords with complete == false. */
    std::vector<DataPointRecord> incompleteDataPointRecords() const;

    std::vector<DataPointRecord> completeDataPointRecords() const;

    std::vector<DataPointRecord> successfulDataPointRecords() const;

    std::vector<DataPointRecord> failedDataPointRecords() const;

    std::vector<DataPointRecord> getDataPointRecords(
        const std::vector<QVariant>& variableValues) const;

    std::vector<DataPointRecord> getDataPointRecords(
        const std::vector<int>& discretePerturbationRecordIds) const;

    std::vector<DataPointRecord> getDataPointRecords(const std::string& tag) const;

    bool resultsAreInvalid() const;

    bool dataPointsAreInvalid() const;

    /** Deserialize this record and all its children and resources to analysis::Analysis. */
    analysis::Analysis analysis() const;

    //@}
    /** @name Setters */
    //@{

    void setResultsAreInvalid(bool value);

    void setDataPointsAreInvalid(bool value);

    //@}
    /** @name Construction Helpers */
    //@{

    /** Reverts record id values back to last state. For use in AnalysisRecord constructor
     *  so can access old related records for comparison and possible removal. */
    void revertToLastRecordIds();

    void setProblemRecordId(int id);

    void setAlgorithmRecordId(int id);

    /** Private method in public class. Used to make two-way connection with FileReferences. */
    void setSeedFileReferenceRecordId(int id);

    /** Private method in public class. Used to make two-way connection with FileReferences. */
    void setWeatherFileReferenceRecordId(int id);

    //@}
   protected:
    /** @name Virtual Methods */
    //@{

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

    //@}
   private:
    REGISTER_LOGGER("openstudio.project.AnalysisRecord");

    int m_problemRecordId;
    boost::optional<int> m_algorithmRecordId;
    int m_seedFileReferenceRecordId;
    boost::optional<int> m_weatherFileReferenceRecordId;
    bool m_resultsAreInvalid;
    bool m_dataPointsAreInvalid;

    int m_lastProblemRecordId;
    boost::optional<int> m_lastAlgorithmRecordId;
    int m_lastSeedFileReferenceRecordId;
    boost::optional<int> m_lastWeatherFileReferenceRecordId;
    bool m_lastResultsAreInvalid;
    bool m_lastDataPointsAreInvalid;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_ANALYSISRECORD_IMPL_HPP

