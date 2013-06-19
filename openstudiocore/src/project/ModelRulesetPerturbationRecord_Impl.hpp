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

#ifndef PROJECT_MODELRULESETPERTURBATIONRECORD_IMPL_HPP
#define PROJECT_MODELRULESETPERTURBATIONRECORD_IMPL_HPP

#include <project/ProjectAPI.hpp>
#include <project/DiscretePerturbationRecord_Impl.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

namespace openstudio {

namespace analysis {
  class ModelRulesetPerturbation;
}

namespace project {

  class VariableRecord;
  class ModelRulesetRecord;

namespace detail {

  /** ModelRulesetPerturbationRecord_Impl is a DiscretePerturbationRecord_Impl that is the implementation class for ModelRulesetPerturbationRecord.*/
  class PROJECT_API ModelRulesetPerturbationRecord_Impl : public DiscretePerturbationRecord_Impl {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    ModelRulesetPerturbationRecord_Impl(const analysis::ModelRulesetPerturbation& modelRulesetPerturbation, 
                                        DiscreteVariableRecord& discreteVariableRecord,
                                        int perturbationVectorIndex);

    /** Constructor from query. Throws if bad query. */
    ModelRulesetPerturbationRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~ModelRulesetPerturbationRecord_Impl() {}

    //@}

    /** Returns objects directly owned by this Record. Children are removed when this Record 
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    /** Returns the ModelRulesetRecord (child) that defines this perturbation. */
    ModelRulesetRecord modelRulesetRecord() const;

    virtual analysis::DiscretePerturbation discretePerturbation() const;

    analysis::ModelRulesetPerturbation modelRulesetPerturbation() const;

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
    REGISTER_LOGGER("openstudio.project.ModelRulesetPerturbationRecord");

    int m_modelRulesetRecordId;

    int m_lastModelRulesetRecordId;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_MODELRULESETPERTURBATIONRECORD_IMPL_HPP

