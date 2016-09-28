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

#ifndef PROJECT_MEASURERECORD_IMPL_HPP
#define PROJECT_MEASURERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"
#include "MeasureRecord.hpp"

#include "../utilities/core/Logger.hpp"

#include <QVariant>

class QSqlQuery;

namespace openstudio {
namespace project {

class MeasureRecord;
class MeasureGroupRecord;

namespace detail {

  /** MeasureRecord_Impl is a ObjectRecord_Impl that is the implementation class for
   *  MeasureRecord.*/
  class PROJECT_API MeasureRecord_Impl : public ObjectRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// constructors
    MeasureRecord_Impl(const analysis::Measure& measure,
                       const MeasureRecordType& measureRecordType,
                       const MeasureGroupRecord& measureGroupRecord,
                       int measureVectorIndex);

    /// constructor for RubyMeasureRecords that are resources of RubyContinuousVariableRecords
    MeasureRecord_Impl(const analysis::Measure& measure,
                       const MeasureRecordType& measureRecordType,
                       ProjectDatabase& database);

    /// create from a query, throws if bad query
    MeasureRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database);

    virtual ~MeasureRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /// get name of the database table, implement at table level
    virtual std::string databaseTableName() const override;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const override;

    /** Returns objects directly owned by this Record. Children are removed when this Record 
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const override;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const override;

    /** Returns join relationships between this object and others. Such relationships will be 
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<int> variableRecordId() const;

    /// get the variable record
    boost::optional<MeasureGroupRecord> measureGroupRecord() const;

    /// is this measure selected
    bool isSelected() const;

    /// set if this measure is selected
    bool setIsSelected(bool isSelected);

    /** Index of this Measure in its (parent) MeasureGroup's vector of
     *  measures. */
    boost::optional<int> measureVectorIndex() const;

    /** Deserialize derived class, then cast to analysis::Measure. Structurally
     *  similar to the virtual clone function of RulesetObjects (especially as implemented by 
     *  the Standards ruleset objects). */
    virtual analysis::Measure measure() const = 0;

    //@}
   protected:

    /// bind values to a query for saving
    /// override in derived classes
    virtual void bindValues(QSqlQuery& query) const override;

    /// set the last state of this object from the query (including id)
    /// override in derived classes
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) override;

    /// check that values (except id) are same as query
    /// override in derived classes
    virtual bool compareValues(const QSqlQuery& query) const override;

    /// save values to last state
    /// override in derived classes
    virtual void saveLastValues() override;

    /// revert values back to last state
    /// override in derived classes
    virtual void revertToLastValues() override;

   private:

    REGISTER_LOGGER("openstudio.project.MeasureRecord");

    MeasureRecordType m_measureRecordType;
    boost::optional<int> m_variableRecordId;
    bool m_isSelected;
    boost::optional<int> m_measureVectorIndex;

    MeasureRecordType m_lastMeasureRecordType;
    boost::optional<int> m_lastVariableRecordId;
    bool m_lastIsSelected;
    boost::optional<int> m_lastMeasureVectorIndex;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_MEASURERECORD_IMPL_HPP
