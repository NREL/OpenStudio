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

#ifndef PROJECT_MODELRULESETCONTINUOUSVARIABLERECORD_IMPL_HPP
#define PROJECT_MODELRULESETCONTINUOUSVARIABLERECORD_IMPL_HPP

#include <project/ProjectAPI.hpp>
#include <project/ContinuousVariableRecord_Impl.hpp>

namespace openstudio {
namespace analysis {
  class ModelRulesetContinuousVariable;
}
namespace project {

class ModelRulesetRecord;

namespace detail {

  /** ModelRulesetContinuousVariableRecord_Impl is a ContinuousVariableRecord_Impl that is the
   *  implementation class for ModelRulesetContinuousVariableRecord. */
  class PROJECT_API ModelRulesetContinuousVariableRecord_Impl : public ContinuousVariableRecord_Impl {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    ModelRulesetContinuousVariableRecord_Impl(
        const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable,
        ProblemRecord& problemRecord,
        int variableVectorIndex);

    ModelRulesetContinuousVariableRecord_Impl(
        const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable,
        FunctionRecord& functionRecord,
        int variableVectorIndex,
        boost::optional<double> functionCoefficient);

    /** Constructor from query. Throws if bad query. */
    ModelRulesetContinuousVariableRecord_Impl(const QSqlQuery& query,
                                              ProjectDatabase& database);

    virtual ~ModelRulesetContinuousVariableRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    virtual analysis::Variable variable() const;

    virtual analysis::InputVariable inputVariable() const;

    virtual analysis::ContinuousVariable continuousVariable() const;

    //@}
    /** @name Getters */
    //@{

    ModelRulesetRecord modelRulesetRecord() const;

    analysis::ModelRulesetContinuousVariable modelRulesetContinuousVariable() const;

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
    REGISTER_LOGGER("openstudio.project.ModelRulesetContinuousVariableRecord");

    std::string m_attributeName;
    int m_modelRulesetRecordId;

    std::string m_lastAttributeName;
    int m_lastModelRulesetRecordId;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_MODELRULESETCONTINUOUSVARIABLERECORD_IMPL_HPP

