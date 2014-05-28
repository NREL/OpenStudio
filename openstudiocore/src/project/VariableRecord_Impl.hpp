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

#ifndef PROJECT_VARIABLERECORD_IMPL_HPP
#define PROJECT_VARIABLERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"
#include "VariableRecord.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace project {

namespace detail{

  class PROJECT_API VariableRecord_Impl : public ObjectRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    VariableRecord_Impl(const analysis::Variable& variable,
                        const VariableRecordType& variableRecordType,
                        ProblemRecord& problemRecord,
                        int workflowIndex);

    VariableRecord_Impl(const analysis::Variable& variable,
                        const VariableRecordType& variableRecordType,
                        FunctionRecord& functionRecord,
                        int variableVectorIndex,
                        boost::optional<double> functionCoefficient);

    /// create from a query, throws if bad query
    VariableRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database);

    virtual ~VariableRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /// get name of the database table
    virtual std::string databaseTableName() const;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const;

    /// get join record objects
    virtual std::vector<JoinRecord> joinRecords() const;

    //@}
    /** @name Getters */
    //@{

    /** Returns this VariableRecord's parent FunctionRecord. */
    boost::optional<FunctionRecord> functionRecord() const;

    /** Index of this Variable in its (parent) Problem's vector of Variables. */
    int variableVectorIndex() const;

    boost::optional<double> functionCoefficient() const;

    /** Deserialize derived class, then cast to analysis::Variable. Structurally similar to the
     *  virtual clone function of RulesetObjects (especially as implemented by the Standards
     *  ruleset objects). */
    virtual analysis::Variable variable() const = 0;

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
    REGISTER_LOGGER("openstudio.project.VariableRecord");

    boost::optional<int> m_functionRecordId;
    int m_variableVectorIndex;
    boost::optional<double> m_functionCoefficient;
    VariableRecordType m_variableRecordType;

    boost::optional<int> m_lastFunctionRecordId;
    int m_lastVariableVectorIndex;
    boost::optional<double> m_lastFunctionCoefficient;
    VariableRecordType m_lastVariableRecordType;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_VARIABLERECORD_IMPL_HPP
