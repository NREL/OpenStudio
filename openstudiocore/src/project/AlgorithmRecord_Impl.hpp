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
    virtual std::string databaseTableName() const override;

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const override;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const override;

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

