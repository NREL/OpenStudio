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

#ifndef PROJECT_PERTURBATIONRECORD_IMPL_HPP
#define PROJECT_PERTURBATIONRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord_Impl.hpp>
#include <project/DiscretePerturbationRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>

#include <QVariant>

class QSqlQuery;

namespace openstudio {
namespace project {

class DiscretePerturbationRecord;
class DiscreteVariableRecord;

namespace detail {

  /** DiscretePerturbationRecord_Impl is a ObjectRecord_Impl that is the implementation class for 
   *  DiscretePerturbationRecord.*/
  class PROJECT_API DiscretePerturbationRecord_Impl : public ObjectRecord_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /// constructors
    DiscretePerturbationRecord_Impl(const analysis::DiscretePerturbation& discretePerturbation, 
                                    const DiscretePerturbationRecordType& discretePerturbationRecordType, 
                                    const DiscreteVariableRecord& discreteVariableRecord, 
                                    int perturbationVectorIndex);

    /// constructor for RubyPerturbationRecords that are resources of RubyContinuousVariableRecords
    DiscretePerturbationRecord_Impl(const analysis::DiscretePerturbation& discretePerturbation,
                                    const DiscretePerturbationRecordType& discretePerturbationRecordType,
                                    ProjectDatabase& database);

    /// create from a query, throws if bad query
    DiscretePerturbationRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database);

    virtual ~DiscretePerturbationRecord_Impl() {}

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

    /// get the variable record
    boost::optional<DiscreteVariableRecord> discreteVariableRecord() const;

    /// is this perturbation selected
    bool isSelected() const;

    /// set if this perturbation is selected
    bool setIsSelected(bool isSelected);

    /** Index of this DiscretePerturbation in its (parent) DiscreteVariable's vector of 
     *  perturbations. */
    boost::optional<int> perturbationVectorIndex() const;

    /** Deserialize derived class, then cast to analysis::DiscretePerturbation. Structurally 
     *  similar to the virtual clone function of RulesetObjects (especially as implemented by 
     *  the Standards ruleset objects). */
    virtual analysis::DiscretePerturbation discretePerturbation() const = 0;

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

    REGISTER_LOGGER("openstudio.project.DiscretePerturbationRecord");

    DiscretePerturbationRecordType m_discretePerturbationRecordType;
    boost::optional<int> m_variableRecordId;
    bool m_isSelected;
    boost::optional<int> m_perturbationVectorIndex;

    DiscretePerturbationRecordType m_lastDiscretePerturbationRecordType;
    boost::optional<int> m_lastVariableRecordId;
    bool m_lastIsSelected;
    boost::optional<int> m_lastPerturbationVectorIndex;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_PERTURBATIONRECORD_IMPL_HPP
