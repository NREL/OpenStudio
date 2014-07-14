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
    virtual std::string databaseTableName() const;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const;

    /** Returns objects directly owned by this Record. Children are removed when this Record 
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Returns join relationships between this object and others. Such relationships will be 
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const;

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
    virtual void bindValues(QSqlQuery& query) const;

    /// set the last state of this object from the query (including id)
    /// override in derived classes
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /// check that values (except id) are same as query
    /// override in derived classes
    virtual bool compareValues(const QSqlQuery& query) const;

    /// save values to last state
    /// override in derived classes
    virtual void saveLastValues();

    /// revert values back to last state
    /// override in derived classes
    virtual void revertToLastValues();

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
