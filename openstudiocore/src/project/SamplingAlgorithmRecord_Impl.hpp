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

#ifndef PROJECT_SAMPLINGALGORITHMRECORD_IMPL_HPP
#define PROJECT_SAMPLINGALGORITHMRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "DakotaAlgorithmRecord_Impl.hpp"

#include "../analysis/SamplingAlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {
  class SamplingAlgorithm;
}
namespace project {

namespace detail {

  /** SamplingAlgorithmRecord_Impl is a DakotaAlgorithmRecord_Impl that is the implementation class
   *  for SamplingAlgorithmRecord.*/
  class PROJECT_API SamplingAlgorithmRecord_Impl : public DakotaAlgorithmRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SamplingAlgorithmRecord_Impl(const analysis::SamplingAlgorithm& samplingAlgorithm,
                                AnalysisRecord& analysisRecord);

    /** Constructor from query. Throws if bad query. */
    SamplingAlgorithmRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~SamplingAlgorithmRecord_Impl() {}

    //@}

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Returns join relationships between this object and others. Such relationships will be 
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual analysis::Algorithm algorithm() const;

    virtual analysis::DakotaAlgorithm dakotaAlgorithm() const;

    analysis::SamplingAlgorithm samplingAlgorithm() const;

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
    REGISTER_LOGGER("openstudio.project.SamplingAlgorithmRecord");

    boost::optional<analysis::SamplingAlgorithmSampleType> m_sampleType;
    boost::optional<analysis::SamplingAlgorithmRNGType> m_rngType;

    boost::optional<analysis::SamplingAlgorithmSampleType> m_lastSampleType;
    boost::optional<analysis::SamplingAlgorithmRNGType> m_lastRNGType;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_SAMPLINGALGORITHMRECORD_IMPL_HPP

