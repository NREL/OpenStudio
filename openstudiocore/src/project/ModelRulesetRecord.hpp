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

#ifndef PROJECT_MODELRULESETRECORD_HPP
#define PROJECT_MODELRULESETRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/RulesetRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset{
  class ModelRuleset;
  class ModelRule;
}

namespace project {

  namespace detail {
    class ModelRulesetRecord_Impl;
  }

  class ModelRuleRecord;

  /** ModelRulesetRecord
  **/
  class PROJECT_API ModelRulesetRecord : public RulesetRecord {
    public:

      typedef detail::ModelRulesetRecord_Impl ImplType;
      typedef RulesetRecordColumns ColumnsType;
      typedef RulesetRecord ObjectRecordType;

      /// constructors
      ModelRulesetRecord(const openstudio::ruleset::ModelRuleset& modelRuleset, ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      ModelRulesetRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      virtual ~ModelRulesetRecord() {}

      static boost::optional<ModelRulesetRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                  ProjectDatabase& database);

      /// get all model ruleset records in database
      static std::vector<ModelRulesetRecord> getModelRulesetRecords(ProjectDatabase& projectDatabase);

      /// get model ruleset record in database
      static boost::optional<ModelRulesetRecord> getModelRulesetRecord(int id, ProjectDatabase& projectDatabase);

      /// get the model ruleset
      openstudio::ruleset::ModelRuleset modelRuleset() const;

      /// get the model rule records
      std::vector<ModelRuleRecord> modelRuleRecords() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      ModelRulesetRecord(boost::shared_ptr<detail::ModelRulesetRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      ModelRulesetRecord(boost::shared_ptr<detail::ModelRulesetRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.ModelRulesetRecord");
  };

  /** \relates ModelRulesetRecord */
  typedef boost::optional<ModelRulesetRecord> OptionalModelRulesetRecord;

  /** \relates ModelRulesetRecord */
  typedef std::vector<ModelRulesetRecord> ModelRulesetRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELRULESETRECORD_HPP
