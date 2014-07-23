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

#ifndef PROJECT_ALGORITHMRECORD_IMPL_HPP
#define PROJECT_ALGORITHMRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "AlgorithmRecord.hpp"

namespace openstudio {
  class Attribute;

namespace analysis {
  class Algorithm;
} // analysis

namespace project {

namespace detail {

  /** AlgorithmRecord_Impl is a ObjectRecord_Impl that is the implementation class for AlgorithmRecord.*/
  class PROJECT_API AlgorithmRecord_Impl : public ObjectRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    AlgorithmRecord_Impl(const analysis::Algorithm& algorithm,
                         const AlgorithmRecordType& algorithmRecordType,
                         AnalysisRecord& analysisRecord);

    /** Constructor from query. Throws if bad query. */
    AlgorithmRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~AlgorithmRecord_Impl() {}

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

    //@}
    /** @name Getters */
    //@{

    /** Returns the parent AnalysisRecord of this AlgorithmRecord. */
    AnalysisRecord analysisRecord() const;

    bool complete() const;

    bool failed() const;

    int iter() const;

    std::vector<Attribute> options() const;

    virtual analysis::Algorithm algorithm() const = 0;

    //@}
    /** @name Setters */
    //@{

    /** Resets algorithm back to its initial state. Provided for callers operating
     *  directly on the database, not holding a copy of this analysis in memory. Use
     *  with caution. Does not do file system cleanup. */
    virtual void reset();

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
    REGISTER_LOGGER("openstudio.project.AlgorithmRecord");

    AlgorithmRecordType m_algorithmRecordType;
    bool m_complete;
    bool m_failed;
    int m_iter;

    AlgorithmRecordType m_lastAlgorithmRecordType;
    bool m_lastComplete;
    bool m_lastFailed;
    int m_lastIter;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_ALGORITHMRECORD_IMPL_HPP

