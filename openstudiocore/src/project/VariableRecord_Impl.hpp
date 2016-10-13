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
    virtual std::string databaseTableName() const override;

    /// get parent object
    virtual boost::optional<ObjectRecord> parent() const override;

    /// get join record objects
    virtual std::vector<JoinRecord> joinRecords() const override;

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
